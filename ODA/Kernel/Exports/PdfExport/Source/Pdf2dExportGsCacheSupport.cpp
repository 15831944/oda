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


///////////////////////////////////////////////////////////////////////////////
//
// Pdf2dExportGsCacheSupport.cpp : play methods.
//
///////////////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
#include "PdfExportCommon.h"

#include "Pdf2dExportGsCacheSupport.h"
#include "Pdf2dExportView.h"
#include "Gs/GsEntityNode.h"


namespace TD_PDF_2D_EXPORT {

void RecColorBitmapDPI::play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const
{
  PDF2dExportView* pVect = static_cast<PDF2dExportView*>(pCtx);
  if(!pVect || !pVect->isPlayingMfForDisplay())
    return;
  pVect->deviceGeom()->setColorBitmapDPI(m_value);
}

void RecFillRule::play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const
{
  PDF2dExportView* pVect = static_cast<PDF2dExportView*>(pCtx);
  if(!pVect || !pVect->isPlayingMfForDisplay())
    return;
  pVect->deviceGeom()->setFillRule(m_fillRule);
}

void RecHiddenText::play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const
{
  PDF2dExportView* pVect = static_cast<PDF2dExportView*>(pCtx);
  if(!pVect || !pVect->isPlayingMfForDisplay() || !pVect->deviceGeom())
    return;
  pVect->deviceGeom()->setExportHiddenText(m_hidden);
}

void RecFillContour::play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const
{
  PDF2dExportView* pVect = static_cast<PDF2dExportView*>(pCtx);
  if(!pVect || !pVect->isPlayingMfForDisplay())
    return;
  pVect->deviceGeom()->dc_fill();
}

void RecDrawingContour::play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const
{
  PDF2dExportView* pVect = static_cast<PDF2dExportView*>(pCtx);
  if(!pVect || !pVect->isPlayingMfForDisplay())
    return;
  if(m_bStart)
  {
    pVect->enableClipper(false);
    pVect->deviceGeom()->startDrawingContour();
  }
  else
  {
    pVect->deviceGeom()->endDrawingContour();
    pVect->enableClipper(true);
  }
}

void RecHatchDrawingMode::play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const
{
  PDF2dExportView* pVect = static_cast<PDF2dExportView*>(pCtx);
  if(!pVect || !pVect->isPlayingMfForDisplay())
    return;
  pVect->deviceGeom()->setHatchDrawingMode(m_bMode);
}

void RecEnableClipper::play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const
{
  PDF2dExportView* pVect = static_cast<PDF2dExportView*>(pCtx);
  if(!pVect || !pVect->isPlayingMfForDisplay())
    return;

  pVect->enableClipper(m_bEnable);
}

void RecDisableColorMerge::play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const
{
  PDF2dExportView* pVect = static_cast<PDF2dExportView*>(pCtx);
  if(!pVect || !pVect->isPlayingMfForDisplay())
    return;
  pVect->disableColorMerge();
}

void RecHyperlink::play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const
{
  PDF2dExportView* pVect = static_cast<PDF2dExportView*>(pCtx);
  if(!pVect || !pVect->isPlayingMfForDisplay())
    return;
  if(!pVect->mfPlayNode() || !pVect->mfPlayNode()->hasExtents())
    return;

  pVect->passHyperlinkToGeometry(m_data, pVect->mfPlayNode()->extents(), true);
}

void RecTextAnnot::play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const
{
  PDF2dExportView* pVect = static_cast<PDF2dExportView*>(pCtx);
  if(!pVect || !pVect->isPlayingMfForDisplay())
    return;
  if(!pVect->mfPlayNode() || !pVect->mfPlayNode()->hasExtents())
    return;
  pVect->passTextAnnotationToGeometry(m_text, m_layer, pVect->mfPlayNode()->extents(), true);
}

void getShellAsImages(bool bFromMf, const OdGeExtents3d& extents,
  PDF2dExportView& vect, ImageArray& images,
  OdInt32 numVertices, const OdGePoint3d* vertexList,
  OdInt32 faceListSize, const OdInt32* faceList,
  const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData,
  const OdGiVertexData* pVertexData);
bool isFullyClipped(const OdGeExtents3d& extents, OdGiConveyorGeometry& geom);

void RecHatchAsBitmap::createImage(const OdGeExtents3d& extents,
  PDF2dExportView& vect) const
{
  if(!m_bImageCreated)
  {
    getShellAsImages(true, extents, vect, m_images,
      m_shell.numVertices(), m_shell.vertices(),
      m_shell.faceListSize(), m_shell.faceList(),
      m_shell.edgeData(), m_shell.faceData(), m_shell.vertexData());
    m_bImageCreated = true;
  }
}

void RecHatchAsBitmap::play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const
{
  if(!pGeom)
    return;

  PDF2dExportView* pVect = static_cast<PDF2dExportView*>(pCtx);
  if(!pVect || !pVect->isPlayingMfForDisplay())
  {
    pGeom->shellProc(m_shell.numVertices(), m_shell.vertices(),
      m_shell.faceListSize(), m_shell.faceList(),
      m_shell.edgeData(), m_shell.faceData(), m_shell.vertexData());
    return;
  }
  const OdGeExtents3d& extents = pVect->mfPlayNode()->extents();
  if(extents.isValidExtents() && isFullyClipped(extents, *pGeom))
    return;
  createImage(extents, *pVect);

  pVect->deviceGeom()->setHatchDrawingMode(true);

  if(!m_images.size())
  {
    pGeom->shellProc(m_shell.numVertices(), m_shell.vertices(),
      m_shell.faceListSize(), m_shell.faceList(),
      m_shell.edgeData(), m_shell.faceData(), m_shell.vertexData());
    pVect->deviceGeom()->setHatchDrawingMode(false);
    return;
  }

  OdGePoint3d aBnd[4];
  OdGePoint2d aBnd2d[4];
  for(OdUInt32 i = 0; i < m_images.size(); ++i)
  {
    const ImageData& img = m_images[i];
    //OdUInt32 imgWidth = img.m_pImage->pixelWidth();
    //OdUInt32 imgHeight = img.m_pImage->pixelHeight();
    pGeom->rasterImageProc(img.m_origin, img.m_uVec, img.m_vVec, img.m_pImage, 0, 0, true);
  }

  pVect->deviceGeom()->setHatchDrawingMode(false);
}

} // TD_PDF_2D_EXPORT namespace
