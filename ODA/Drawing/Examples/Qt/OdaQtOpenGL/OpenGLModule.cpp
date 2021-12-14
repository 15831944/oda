/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2021, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Open Design Alliance software pursuant to a license 
//   agreement with Open Design Alliance.
//   Open Design Alliance Copyright (C) 2002-2021 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////
//
// OpenGLModule.cpp : Defines the entry point for the DLL application.
//

#include "OdaCommon.h"
#include "Gs/Gs.h"
#include "Gi/GiRasterImage.h"
#include "StaticRxObject.h"
#include "RxDynamicModule.h"
#include "ExGsQtScreenDevice.h"
#include "ExGsQtVectorizeDevice.h"

class OdqOffscreenGL : public QGLWidget
{
  public:
    explicit OdqOffscreenGL(const QGLFormat& format, Qt::WindowFlags f=0)
      : QGLWidget(format, 0, 0, f)
    {
    }

    typedef void (*callbackFn)(ExGsQtVectorizeDevice *pImpl, OdGsDCRect *pRect);
  protected:
    callbackFn m_pCb;
    ExGsQtVectorizeDevice *m_pImpl;
    OdGsDCRect *m_pRect;

    void paintGL()
    {
      m_pCb(m_pImpl, m_pRect);
    }

  public:
    QPixmap renderPixmap(callbackFn pCb, ExGsQtVectorizeDevice *pImpl, OdGsDCRect *pRect)
    {
      m_pCb = pCb;
      m_pImpl = pImpl;
      m_pRect = pRect;
      return QGLWidget::renderPixmap(pImpl->width(), pImpl->height());
    }
};

class ExGsOpenGLBitmapVectorizeDevice : public ExGsQtVectorizeDevice, public OdStaticRxObject<OdGiRasterImage>
{
ODRX_DECLARE_PROPERTY(RasterImage)
ODRX_DECLARE_PROPERTY(BitPerPixel)
ODRX_DECLARE_PROPERTY(QImageFrame)

ODRX_DECLARE_DYNAMIC_PROPERTY_MAP(ExGsOpenGLBitmapVectorizeDevice);

public:
  static OdGsDevicePtr createObject()
  {
    return OdRxObjectImpl<ExGsOpenGLBitmapVectorizeDevice, OdGsDevice>::createObject();
  }

  OdUInt32 m_bitsCnt;
  OdqOffscreenGL *m_pCtx;
  QImage m_bits;
  QImage *m_pFrame;
  ODRX_USING_HEAP_OPERATORS(ExGsQtVectorizeDevice);

  ExGsOpenGLBitmapVectorizeDevice() : m_pCtx(NULL), m_pFrame(NULL) 
  {
  }

  virtual ~ExGsOpenGLBitmapVectorizeDevice()
  {
    deleteContext();
  }

  void createContext()
  {
    if (m_pCtx != NULL)
      return;
    m_bDoubleBufferEnabled = false;
    m_bColorIndexMode = true;
    QGLFormat format; // = QGLFormat::defaultFormat();
    format.setDoubleBuffer(false);
    //useless //format.setDirectRendering(false);
    format.setRgba(true);
    format.setAccum(false);
    format.setAlpha(false);
    format.setBlueBufferSize(8);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setStencil(true);
    format.setStencilBufferSize(2);
    format.setDepth(true);
    format.setDepthBufferSize(24);
    QGLFormat::setDefaultFormat(format); // Format for 24bpp always
    m_pCtx = new OdqOffscreenGL(format);
  }
  void deleteContext()
  {
    delete m_pCtx;
    m_pCtx = NULL;
  }

  bool supportPartialUpdate() const
  {
    return OdGsBaseVectorizeDevice::supportPartialUpdate();
  }

  static void caller(ExGsQtVectorizeDevice *pImpl, OdGsDCRect *pRect)
  {
    static_cast<ExGsOpenGLBitmapVectorizeDevice*>(pImpl)->ExGsQtVectorizeDevice::update(pRect);
  }

  void update(OdGsDCRect *pRect)
  {
    if (m_pCtx == NULL)
    {
      createContext();
    }
    m_bits = m_pCtx->renderPixmap(caller, this, pRect).toImage();
    if (m_pFrame) { *m_pFrame = m_bits; }
    // @@@test
    //m_pFrame->save("C:\\test.bmp", "BMP");
  }

  OdGiRasterImage *getRasterImage() { return this; }
  void putRasterImage(OdGiRasterImage *) { }

  OdUInt32 pixelWidth() const {
    return (OdUInt32)width();
  }
  OdUInt32 pixelHeight() const {
    return (OdUInt32)height();
  }
  OdUInt32 colorDepth() const {
    return (OdUInt32)m_bits.depth();
  }
  OdUInt32 numColors() const {
    return 0;
  }
  ODCOLORREF color(OdUInt32 /*colorIndex*/) const {
    return 0xFF000000;
  }
  OdUInt32 paletteDataSize() const {
    return 0;
  }
  void paletteData(OdUInt8* /*pBytes*/) const {
  }
  void scanLines(OdUInt8* pBytes, OdUInt32 index, OdUInt32 numLines) const {
    while (numLines)
    {
      memcpy(pBytes, m_bits.scanLine((int)index), scanLineSize());
      pBytes += scanLineSize();
      index++;
      numLines--;
    }
  }
  const OdUInt8* scanLines() const {
    return (const OdUInt8*)m_bits.bits();
  }
  PixelFormatInfo pixelFormat() const {
    tagPixelFormatInfo px;
    px.setRGB();
    return px;
  }
  OdUInt32 scanLineSize() const {
    return (OdUInt32)m_bits.bytesPerLine();
  }
  OdUInt32 scanLinesAlignment() const {
    return 4;
  }

  void put_QImageFrame(OdIntPtr i) {
    m_pFrame = (QImage*)i;
  }
  OdIntPtr get_QImageFrame() const {
    return (OdIntPtr)m_pFrame;
  }
};

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExGsOpenGLBitmapVectorizeDevice)
ODRX_INHERIT_PROPERTIES(ExGsQtDevice);
ODRX_GENERATE_PROPERTY(RasterImage)
ODRX_GENERATE_PROPERTY(QImageFrame)
//ODRX_GENERATE_PROPERTY(BitPerPixel) // Couldn't choose
ODRX_END_DYNAMIC_PROPERTY_MAP(ExGsOpenGLBitmapVectorizeDevice)

ODRX_DEFINE_PROPERTY_OBJECT_PREFIX(ExGsOpenGLBitmapVectorizeDevice::, RasterImage, ExGsOpenGLBitmapVectorizeDevice, getRasterImage, putRasterImage, OdGiRasterImage)
ODRX_DEFINE_PROPERTY_PREFIX(ExGsOpenGLBitmapVectorizeDevice::, QImageFrame, ExGsOpenGLBitmapVectorizeDevice, getIntPtr)
//ODRX_DEFINE_PROPERTY_PREFIX(ExGsOpenGLBitmapVectorizeDevice::, BitPerPixel, ExGsOpenGLBitmapVectorizeDevice, getInt32)

class OdqOpenGLModule : public OdGsBaseModule
{
protected:
  void initApp()
  {
    OdGsBaseModule::initApp();

    // Add extra initializations here...
  }
  void uninitApp()
  {
    // Add extra uninitializations here...

    OdGsBaseModule::uninitApp();
  }

  OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject()
  {
    return ExGsQtScreenDevice<ExGsQtVectorizeDevice>::createObject();
  }

  OdSmartPtr<OdGsViewImpl> createViewObject()
  {
    return OdRxObjectImpl<ExGsQtVectorizeView, OdGsViewImpl>::createObject();
  }

  OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
  {
    return ExGsOpenGLBitmapVectorizeDevice::createObject();
  }

  OdSmartPtr<OdGsViewImpl> createBitmapViewObject()
  {
    return OdRxObjectImpl<ExGsQtVectorizeView, OdGsViewImpl>::createObject();
  }
};

ODRX_DEFINE_DYNAMIC_MODULE(OdqOpenGLModule);
