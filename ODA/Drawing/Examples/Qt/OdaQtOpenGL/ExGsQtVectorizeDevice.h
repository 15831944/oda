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
// ExGsQtVectorizeDevice.h
//

#ifndef ODGSQTVECTORIZEDEVICE
#define ODGSQTVECTORIZEDEVICE

#include "GsOpenGLVectorizer.h"
#include "ExGsQtDevice.h"
#include "CommonOpenGLProps.h"
// Comment it, if you want compile with non-streamed version of vectorizer
#include "GsOpenGLStreamVectorizer.h"

#ifdef OD_OGL_ENABLEMETASTREAM
OD_OPENGL_DEFINE_COMMON_PROPS(ExGsOpenGLVectorizeDeviceBase, OdGsOpenGLStreamVectorizeDevice, ExGsQtDevice);
typedef OdGsOpenGLStreamVectorizeView ExGsOpenGLVectorizeViewBase;
#else
OD_OPENGL_DEFINE_COMMON_PROPS(ExGsOpenGLVectorizeDeviceBase, OdGsOpenGLVectorizeDevice, ExGsQtDevice);
typedef OdGsOpenGLVectorizeView ExGsOpenGLVectorizeViewBase;
#endif

/** Description:
  This class implements a Win32 OpenGL Vectorizer Device objects.
  Library: Source *code* provided.
  {group:ExRender_Windows_Classes} 
*/
class ExGsQtVectorizeDevice : public ExGsOpenGLVectorizeDeviceBase
{
  bool m_bContextCreated;
  //HGLRC        m_hGLRC;

public:
  //OdUInt32 getWGLContext() const;
  //void setWGLContext(OdUInt32 ctx);
protected:
  ODRX_USING_HEAP_OPERATORS(ExGsOpenGLVectorizeDeviceBase);

  ODRX_DECLARE_DYNAMIC_PROPERTY_MAP(ExGsQtVectorizeDevice);

  ExGsQtVectorizeDevice();

  virtual ~ExGsQtVectorizeDevice();

  void update(OdGsDCRect* pUpdatedRect);
  void updateScreen();

  /** Description:
    Creates a context for this Device object.
  */
  virtual void createContext();
  virtual void deleteContext();

  virtual void preReleaseResource();
  virtual void postReleaseResource();

  //virtual void createPalette(ODGSPALETTE *logicalPalette);

 // temp here instead ExGsQtDevice : HDC m_hDC;
  //HDC getHDC() const 
  //{ 
  //  QGLWidget* pGLWidget = getGLWidget();
  //  ODA_ASSERT_ONCE(pGLWidget);
  //  return pGLWidget->getDC();
  //}

  //was moved to ExGsQtDevice
  //virtual int getDeviceColorDepth() const;

  /** Description:
    Checks OpenGL extensions support.
  */
  virtual bool checkExtensionFuncSupport(OpenGLExtension extIdx) const;
  /** Description:
    Returns extension function pointer by function name.
  */
  virtual void *getExtensionFuncPtrFromListInt(const char *pFuncName) const;
};

/** Description:
  This class implements Win32 OpenGL Vectorizer View objects.
  Library: Source *code* provided.
  {group:ExRender_Windows_Classes} 
*/
class ExGsQtVectorizeView : public ExGsOpenGLVectorizeViewBase
{
  ExGsQtVectorizeDevice* device()
  {
    return static_cast<ExGsQtVectorizeDevice*>(OdGsOpenGLVectorizeView::device());
  }
  bool m_bSelecting;
public:
  ExGsQtVectorizeView();

  void select(const OdGePoint2d* pts, int nPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode);

  void polylineOut(OdInt32 numPoints, const OdGePoint3d* vertexList);

  //void meshProc(OdInt32 numRows,
  //  OdInt32 numColumns,
  //  const OdGePoint3d* vertexList,
  //  const OdGiEdgeData* pEdgeData = 0,
  //  const OdGiFaceData* pFaceData = 0,
  //  const OdGiVertexData* pVertexData = 0);

  //void shellProc(OdInt32 numVertices,
  //  const OdGePoint3d* vertexList,
  //  OdInt32 faceListSize,
  //  const OdInt32* faceList,
  //  const OdGiEdgeData* pEdgeData = 0,
  //  const OdGiFaceData* pFaceData = 0,
  //  const OdGiVertexData* pVertexData = 0);

  //void rasterImageProc(const OdGePoint3d& origin,
  //  const OdGeVector3d& u,
  //  const OdGeVector3d& v,
  //  const OdGiRasterImage* pImage, 
  //  const OdGePoint2d* uvBoundary,
  //  OdUInt32 numBoundPts,
  //  bool transparency = false,
  //  double brightness = 50.0,
  //  double contrast = 50.0,
  //  double fade = 0.0);

  //void metafileProc(const OdGePoint3d& origin,
  //  const OdGeVector3d& u,
  //  const OdGeVector3d& v,
  //  const OdGiMetafile* pMetafile,
  //  bool dcAligned = true,      
  //  bool allowClipping = false);

  void ownerDrawDc(const OdGePoint3d& origin,
                   const OdGeVector3d& u,
                   const OdGeVector3d& v,
                   const OdGiSelfGdiDrawable* pDrawable,
                   bool dcAligned = true,      
                   bool allowClipping = false);
};

#endif // ODGSQTVECTORIZEDEVICE
