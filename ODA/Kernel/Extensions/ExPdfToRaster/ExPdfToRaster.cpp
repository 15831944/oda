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

#include "OdaCommon.h"

#include "ExPdfToRaster.h"
#include "MemoryStream.h"
#include "OdPlatformStreamer.h"
#include "RxRasterServices.h"
#include "RxDynamicModule.h"
#include "RxObjectImpl.h"
#include "DynamicLinker.h"

#include "fpdf_formfill.h"

#if defined(_MSC_VER)
#pragma warning ( disable : 4100 ) //  unreferenced formal parameter
#endif

#define TOCHAR(a) ((char *)OdAnsiString(OdString(a)).c_str())

ODRX_CONS_DEFINE_MEMBERS(OdPdf2ImageConverterPdfium, OdPdf2ImageConverter, RXIMPL_CONSTR)

typedef struct {
  FPDF_DOCUMENT m_doc;
  OdArray<FPDF_PAGE> m_pages;
  FPDF_ARRAY m_pLayers;
  FPDF_ARRAY m_pONLayers;
  FPDF_ARRAY m_pOFFLayers;
} pdf_doc;

OdPdf2ImageConverterPdfium::OdPdf2ImageConverterPdfium()
  : m_pDoc(0)
  , m_activePage(0)
{
}

OdPdf2ImageConverterPdfium::~OdPdf2ImageConverterPdfium()
{
  unloadPdf();
}

OdResult OdPdf2ImageConverterPdfium::loadPdf(const OdString &filename, const  OdString &password)
{
#if defined(PDFIUM_MODULE_ENABLED)
  unloadPdf();

  pdf_doc* pRes = new pdf_doc;
  pRes->m_doc = FPDF_LoadDocument(filename.c_str(), TOCHAR(password.c_str()));
  if (!pRes->m_doc)
  {
    delete pRes;
    return eCantOpenFile;
  }

  int pageCount = FPDF_GetPageCount(pRes->m_doc);
  for (int i = 0; i < pageCount; i++)
    pRes->m_pages.append(FPDF_LoadPage(pRes->m_doc, i));

  pRes->m_pLayers = 0;
  pRes->m_pONLayers = 0;
  pRes->m_pOFFLayers = 0;
  if (!FPDF_LoadLayers(pRes->m_doc, pRes->m_pLayers, pRes->m_pONLayers, pRes->m_pOFFLayers))
  {
    unloadPdf();
    return eCantOpenFile;
  }

  m_activePage = 0;
  m_pDoc = pRes;
  m_fileName = filename;
  return eOk;
#else
  return eNotHandled;
#endif
}

OdResult OdPdf2ImageConverterPdfium::loadPdf(OdStreamBufPtr &pBuf, const  OdString &password)
{
#if defined(PDFIUM_MODULE_ENABLED)
  unloadPdf();

  pBuf->rewind();
  m_data.resize(pBuf->length());
  pBuf->getBytes(m_data.asArrayPtr(), pBuf->length());

  pdf_doc* pRes = new pdf_doc;
  int len = (int)m_data.size();
  pRes->m_doc = FPDF_LoadMemDocument(m_data.asArrayPtr(), len, TOCHAR(password.c_str()));
  if (!pRes->m_doc)
  {
    delete pRes;
    return eCantOpenFile;
  }

  int pageCount = FPDF_GetPageCount(pRes->m_doc);
  for (int i = 0; i < pageCount; i++)
    pRes->m_pages.append(FPDF_LoadPage(pRes->m_doc, i));

  pRes->m_pLayers = 0;
  pRes->m_pONLayers = 0;
  pRes->m_pOFFLayers = 0;
  if (!FPDF_LoadLayers(pRes->m_doc, pRes->m_pLayers, pRes->m_pONLayers, pRes->m_pOFFLayers))
  {
    unloadPdf();
    return eCantOpenFile;
  }

  m_activePage = 0;
  m_pDoc = pRes;
  return eOk;
#else
  return eNotHandled;
#endif
}

bool OdPdf2ImageConverterPdfium::isLoaded()
{
  return m_pDoc != 0;
}

OdString OdPdf2ImageConverterPdfium::getFilename()
{
  return m_fileName;
}

OdUInt16 OdPdf2ImageConverterPdfium::getPagesCount()
{
#if defined(PDFIUM_MODULE_ENABLED)
  if (!m_pDoc)
    return 0;
  pdf_doc* pDoc = static_cast<pdf_doc*>(m_pDoc);
  if (!pDoc)
    return 0;
  return pDoc->m_pages.size();
#else
  return 0;
#endif
}

OdResult OdPdf2ImageConverterPdfium::setActivePage(OdUInt16 pageNum)
{
#if defined(PDFIUM_MODULE_ENABLED)
  if (pageNum < 1)
    pageNum = 1;
  m_activePage = --pageNum;

  if (!m_pDoc)
    return eNullPtr;
  pdf_doc* pDoc = static_cast<pdf_doc*>(m_pDoc);
  if (!pDoc)
    return eWrongObjectType;
  if (!pDoc->m_doc)
    return eNullPtr;
  if (m_activePage >= (int)pDoc->m_pages.size())
    return eInvalidIndex;
  return eOk;
#else
  return eNotHandled;
#endif
}

OdUInt16 OdPdf2ImageConverterPdfium::getActivePage()
{
  return m_activePage + 1;
}

OdResult OdPdf2ImageConverterPdfium::getPageSize(OdGsDCRect& rect)
{
#if defined(PDFIUM_MODULE_ENABLED)
  if (!m_pDoc)
    return eNullPtr;
  pdf_doc* pDoc = static_cast<pdf_doc*>(m_pDoc);
  if (!pDoc)
    return eWrongObjectType;
  if (!pDoc->m_doc)
    return eNullPtr;
  if (m_activePage >= (int)pDoc->m_pages.size())
    return eInvalidIndex;
  if (!pDoc->m_pages[m_activePage])
    return eNullIterator;

  rect.m_min.x = 0;
  rect.m_min.y = 0;
  rect.m_max.x = FPDF_GetPageWidth(pDoc->m_pages[m_activePage]);
  rect.m_max.y = FPDF_GetPageHeight(pDoc->m_pages[m_activePage]);
  return eOk;
#else
  return eNotHandled;
#endif
}

OdResult OdPdf2ImageConverterPdfium::getLayers(OdPdfLayerArray& layers)
{
#if defined(PDFIUM_MODULE_ENABLED)
  if (!m_pDoc)
    return eNullPtr;
  pdf_doc* pDoc = static_cast<pdf_doc*>(m_pDoc);
  if (!pDoc)
    return eWrongObjectType;
  if (!pDoc->m_doc)
    return eNullPtr;
  if (m_activePage >= (int)pDoc->m_pages.size())
    return eInvalidIndex;
  if (!pDoc->m_pages[m_activePage])
    return eNullIterator;

  if (!FPDF_GetPageLayers(pDoc->m_doc, pDoc->m_pages[m_activePage], m_activePage, m_info))
    return eExtendedError;

  layers.clear();
  for (int i = 0; i < m_info.nol; i++)
  {
    bool isOn = FPDF_IsLayerEnabled(pDoc->m_pOFFLayers, m_info.names[i]);
    pdf_layer ll{ m_info.names[i], isOn };
    layers.append(ll);
  }
  //if (layers.isEmpty())
  //  layers.append(L"0");

  return eOk;
#else
  return eNotHandled;
#endif
}

OdGiRasterImagePtr OdPdf2ImageConverterPdfium::convertPdf(const OdPdf2ImageConversionParams* params)
{
#if defined(PDFIUM_MODULE_ENABLED)
  OdGiRasterImagePtr pImg;
  if (!m_pDoc)
    return pImg;
  pdf_doc* pDoc = static_cast<pdf_doc*>(m_pDoc);
  if (!pDoc)
    return pImg;
  if (m_activePage >= (int)pDoc->m_pages.size())
    return pImg;
  if (!pDoc->m_pages[m_activePage])
    return pImg;

  double pic_width = FPDF_GetPageWidth(pDoc->m_pages[m_activePage]);
  double pic_height = FPDF_GetPageHeight(pDoc->m_pages[m_activePage]);

  //////////////////////////////////////////////////////////////////////////
  //double xScale = FPDF_GetMeasureDictInfo(pDoc->m_doc, m_activePage);
  //double measureScale = (xScale == 0.) ? 1. / 72. : (xScale);
  //////////////////////////////////////////////////////////////////////////

  OdUInt32 bgColor = 0xFFFFFFFF;
  int flags = 0;
  float dpi = 72.;
  if (params)
  {
    bgColor = params->bg_color;
    flags = params->flags;
    if (params->layers.size())
      setupLayers(m_pDoc, params->layers);
    if (params->dpi > 0)
      dpi = (float)params->dpi;
    if (dpi > 3000)
      dpi = 3000; //to prevent creation of the huge images, there is the same resolution restriction as in the PdfUnderlays modules for DWG drawings
  }
  double dpi_ratio = dpi / 72.;//default dpi is 72. for pdf files

  while (pic_width*dpi_ratio * pic_height*dpi_ratio > 22000000) //It gives us a bit less than 90000000 bytes for 4 bytes-for-pixel bitmap
    dpi_ratio *= .95;

  pic_width *= dpi_ratio;
  pic_height *= dpi_ratio;
  pic_height = OdRoundToLong(pic_height);
  pic_width = OdRoundToLong(pic_width);

  OdGsDCRect imageSize;
  imageSize.m_min.x = 0;
  imageSize.m_min.y = 0;
  imageSize.m_max.x = pic_width;
  imageSize.m_max.y = pic_height;

  OdGsDCRect cropArea = imageSize;

  if (params && !params->cropArea.is_null())
  {
    cropArea = params->cropArea;
    cropArea.m_min.x = cropArea.m_min.x * dpi_ratio;
    cropArea.m_min.y = cropArea.m_min.y * dpi_ratio;
    cropArea.m_max.x = cropArea.m_max.x * dpi_ratio;
    cropArea.m_max.y = cropArea.m_max.y * dpi_ratio;

    cropArea.intersectWith(imageSize);
    if (cropArea.is_null())
      return pImg; //crop area is outside the page
  }

  pic_width = cropArea.m_max.x - cropArea.m_min.x;
  pic_height = cropArea.m_max.y - cropArea.m_min.y;

  FPDF_BITMAP bitmap = FPDFBitmap_Create(pic_width, pic_height, 1);
  FPDFBitmap_FillRect(bitmap, 0, 0, pic_width, pic_height, bgColor);
  FPDF_RenderPageBitmap(bitmap, pDoc->m_pages[m_activePage],
    -cropArea.m_min.x,
    cropArea.m_max.y - imageSize.m_max.y,
    imageSize.m_max.x, imageSize.m_max.y, 0, flags);

  BITMAPINFOHEADER bmiHeader;
  bmiHeader.biSize = 40;
  bmiHeader.biWidth = pic_width;
  bmiHeader.biHeight = -pic_height;  // top-down image
  bmiHeader.biPlanes = 1;
  bmiHeader.biBitCount = 32;
  bmiHeader.biCompression = 0L; //BI_RGB;
  bmiHeader.biSizeImage = FPDFBitmap_GetStride(bitmap)*(int)pic_height;
  unsigned char *imagedata = (unsigned char *)FPDFBitmap_GetBuffer(bitmap);

  if (!imagedata)
  {
    FPDFBitmap_Destroy(bitmap);
    return pImg;
  }
  else
  {
    FPDF_FORMFILLINFO formInfo = {};
    formInfo.version = 1;
    FPDF_FORMHANDLE fHandle = FPDFDOC_InitFormFillEnvironment(pDoc->m_doc, &formInfo);

    FPDF_FFLDraw(fHandle, bitmap, pDoc->m_pages[m_activePage], -cropArea.m_min.x,
      cropArea.m_max.y - imageSize.m_max.y, imageSize.m_max.x, imageSize.m_max.y, 0, flags);
    FPDFDOC_ExitFormFillEnvironment(fHandle);
  }

  pImg = convertBmpToRaster(bmiHeader, imagedata);
  FPDFBitmap_Destroy(bitmap);
  return pImg;
#else
  return eNotHandled;
#endif
}

OdResult OdPdf2ImageConverterPdfium::unloadPdf()
{
#if defined(PDFIUM_MODULE_ENABLED)
  if (!m_pDoc)
    return eNullPtr;
  pdf_doc* pDoc = static_cast<pdf_doc*>(m_pDoc);
  if (!pDoc)
    return eWrongObjectType;
  for (OdUInt32 i = 0; i < pDoc->m_pages.size(); i++)
    FPDF_ClosePage(pDoc->m_pages[i]);
  FPDF_CloseDocument(pDoc->m_doc);
  delete pDoc;
  m_pDoc = 0;

  m_data.clear();
  m_activePage = 0;
  m_fileName.empty();

  return eOk;
#else
  return eNotHandled;
#endif
}

bool OdPdf2ImageConverterPdfium::setupLayers(PDFDOC pInput, const OdPdfLayerArray& layers)
{
#if defined(PDFIUM_MODULE_ENABLED)
  if (!pInput)
    return false;
  pdf_doc* pDoc = static_cast<pdf_doc*>(pInput);
  if (!pDoc)
    return false;
  if (m_activePage >= (int)pDoc->m_pages.size())
    return false;

  if (!pDoc->m_pLayers || !pDoc->m_pOFFLayers || !pDoc->m_pONLayers)
    return false;

  for (OdUInt32 f = 0; f < layers.size(); ++f) //enable layers from the list
    FPDF_EnableLayer(pDoc->m_pLayers, pDoc->m_pONLayers, pDoc->m_pOFFLayers, layers[f].layer_name.c_str(), layers[f].is_on);

  return true;
#else
  return eNotHandled;
#endif
}

OdGiRasterImagePtr OdPdf2ImageConverterPdfium::convertBmpToRaster(const BITMAPINFOHEADER &bmi, const unsigned char *imagedata)  const
{
  OdGiRasterImagePtr pTmp;
#if defined(PDFIUM_MODULE_ENABLED)
  OdStreamBufPtr pFileBuf = OdMemoryStream::createNew();

  OdPlatformStreamer::wrInt16(*pFileBuf, 0x4d42);
  OdUInt32 scanLinesPos = 14 + 40;  // Below version is correct only on Windows
  OdUInt32 size = scanLinesPos + bmi.biWidth * 32 * Od_abs(bmi.biHeight);

  OdPlatformStreamer::wrInt32(*pFileBuf, size);
  OdPlatformStreamer::wrInt32(*pFileBuf, 0); // reserved
  OdPlatformStreamer::wrInt32(*pFileBuf, scanLinesPos); // offBits
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biSize);  // not portable: sizeof(BITMAPINFOHEADER));
  // save BITMAPINFOHEADER
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biWidth);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biHeight);
  OdPlatformStreamer::wrInt16(*pFileBuf, bmi.biPlanes);
  OdPlatformStreamer::wrInt16(*pFileBuf, bmi.biBitCount);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biCompression);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biSizeImage);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biXPelsPerMeter);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biYPelsPerMeter);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biClrUsed);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biClrImportant);

  pFileBuf->putBytes(imagedata, bmi.biSizeImage);

  pFileBuf->rewind();

  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  if (!pRasSvcs.isNull())
    pTmp = pRasSvcs->loadRasterImage(pFileBuf);
  return pTmp;
#else
  return pTmp;
#endif
}

/** Description:
    The user override of this function should register any custom objects defined in the
    custom application, using the OdRxObject::rxInit function.  It should also register
    custom commands defined in the module.
*/
void ExPdfToRasterModule::initApp()
{
  ODA_TRACE0("ExPdfToRasterModule::initApp() called\n");
#if defined(PDFIUM_MODULE_ENABLED)
  FPDF_InitLibrary();
#endif
  OdPdf2ImageConverterPdfium::rxInit();
}

/** Description:
    The user override of this function should unregister any custom objects defined in the
    custom application, using the OdRxObject::rxUninit function.  It should also
    remove any custom commands that were registered in the initApp function.
*/
void ExPdfToRasterModule::uninitApp()
{
  OdPdf2ImageConverterPdfium::rxUninit();
#if defined(PDFIUM_MODULE_ENABLED)
  FPDF_DestroyLibrary();
#endif
}

OdPdf2ImageConverterPtr ExPdfToRasterModule::createConverter()
{
  return OdPdf2ImageConverterPdfium::createObject();
};

ODRX_DEFINE_DYNAMIC_MODULE(ExPdfToRasterModule);
