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

#include <OdaCommon.h>
#include "DgnExportImages.h"
#include "DgnExportImpl.h"
#include "DgnExportCommon.h"
#include "DgnExportContext.h"
#include <DgRasterAttach.h>
#include <DgRasterFrame.h>
#include <DbRasterImage.h>
#include <DbRasterImageDef.h>
#include <DgLineString.h>
#include <DgOle2Frame.h>
#include <DbOle2Frame.h>
#include <DgShape.h>
#include <MemoryStream.h>
#include <DbWipeout.h>

namespace TD_DGN_EXPORT {

//---------------------------------------------------------------------------------------------------

void OdDbOle2FrameExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbOle2Frame* pDbOle = (OdDbOle2Frame*)(pDbEntity);

  if( !pDbOle )
    return;

  OdDgGraphicsElementPtr pDgOle2Frame;
  OdDgGraphicsElementPtr pDgShape;

  if( OdDgnExportContext::is3d() )
  {
    OdDgOle2Frame3dPtr pDgOle = OdDgOle2Frame3d::createObject();
    pDgOle->setOleType((OdDgOle2Frame::OdDgOleObjectType)(pDbOle->getType()));
    pDgOle->setDrawAspect(OdDgOle2Frame::kContent);
    pDgOle->setViewRotationMode(OdDgOle2Frame::kViewDependent);
    pDgOle->setAspectRatioFlag(true);
    pDgOle->setTransparentBackgroundFlag(false);
    pDgOle->setCanBeLinkedAndEmbeddedFlag(false);
    pDgOle->setCanBePictureFlag(true);

    OdUInt32 uDataSize = pDbOle->getCompoundDocumentDataSize();
    OdStreamBufPtr pStream = OdMemoryStream::createNew(uDataSize);
    pDbOle->getCompoundDocument(*pStream);

    if( pStream->length() != 0 )
    { 
      pStream->rewind();
      pDgOle->setCompoundDocument(uDataSize, *pStream);
      pDgOle->setScale(pDbOle->wcsWidth(), pDbOle->wcsHeight());

      OdRectangle3d rect3d;
      pDbOle->position(rect3d);

      OdGeVector3d vrX = rect3d.lowRight - rect3d.lowLeft;

      if (vrX.isZeroLength())
        vrX = rect3d.upRight - rect3d.upLeft;

      OdGeVector3d vrY = rect3d.upLeft - rect3d.lowLeft;

      if (vrY.isZeroLength())
        vrY = rect3d.upRight - rect3d.lowRight;

      if (vrX.isZeroLength())
        vrX = OdGeVector3d::kXAxis;
      else
        vrX.normalize();

      if (vrY.isZeroLength())
        vrY = OdGeVector3d::kYAxis;
      else
        vrY.normalize();

      OdGeVector3d vrZ = vrX.crossProduct(vrY);

      vrZ.normalize();

      vrY = vrZ.crossProduct(vrX);

      vrY.normalize();

      OdGeMatrix3d matRotation;
      matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrX, vrY, vrZ);
      OdGeQuaternion quatRotation;
      quatRotation.set(matRotation);

      pDgOle->setRotation(quatRotation);
      pDgOle->setPosition(rect3d.lowLeft);

      OdDgShape3dPtr pFrame = OdDgShape3d::createObject();
      pFrame->addVertex(rect3d.lowLeft);
      pFrame->addVertex(rect3d.lowLeft + vrX * pDbOle->wcsWidth());
      pFrame->addVertex(rect3d.lowLeft + vrX * pDbOle->wcsWidth() + vrY * pDbOle->wcsHeight());
      pFrame->addVertex(rect3d.lowLeft + vrY * pDbOle->wcsHeight());
      pFrame->addVertex(rect3d.lowLeft);

      pDgOle->add(pFrame);

      pDgShape = pFrame;

      pDgOle2Frame = pDgOle;
    }
  }
  else
  {
    OdDgOle2Frame2dPtr pDgOle = OdDgOle2Frame2d::createObject();

    pDgOle->setOleType((OdDgOle2Frame::OdDgOleObjectType)(pDbOle->getType()));
    pDgOle->setDrawAspect(OdDgOle2Frame::kContent);
    pDgOle->setViewRotationMode(OdDgOle2Frame::kViewDependent);
    pDgOle->setAspectRatioFlag(true);
    pDgOle->setTransparentBackgroundFlag(false);
    pDgOle->setCanBeLinkedAndEmbeddedFlag(false);
    pDgOle->setCanBePictureFlag(true);

    OdUInt32 uDataSize = pDbOle->getCompoundDocumentDataSize();
    OdStreamBufPtr pStream = OdMemoryStream::createNew(uDataSize);
    pDbOle->getCompoundDocument(*pStream);

    if (pStream->length() != 0)
    {
      pStream->rewind();
      pDgOle->setCompoundDocument(uDataSize, *pStream);
      pDgOle->setScale(pDbOle->wcsWidth(), pDbOle->wcsHeight());

      OdRectangle3d rect3d;
      pDbOle->position(rect3d);

      pDgOle->setRotationAngle(pDbOle->rotation());
      pDgOle->setPosition(rect3d.lowLeft.convert2d());

      OdDgShape2dPtr pFrame = OdDgShape2d::createObject();

      OdGeVector2d vrX = OdGeVector2d::kXAxis;
      OdGeVector2d vrY = OdGeVector2d::kYAxis;
      vrX.rotateBy(pDbOle->rotation());
      vrY.rotateBy(pDbOle->rotation());
      pFrame->addVertex(rect3d.lowLeft.convert2d());
      pFrame->addVertex(rect3d.lowLeft.convert2d() + vrX * pDbOle->wcsWidth());
      pFrame->addVertex(rect3d.lowLeft.convert2d() + vrX * pDbOle->wcsWidth() + vrY * pDbOle->wcsHeight());
      pFrame->addVertex(rect3d.lowLeft.convert2d() + vrY * pDbOle->wcsHeight());
      pFrame->addVertex(rect3d.lowLeft.convert2d());

      pDgOle->add(pFrame);

      pDgShape = pFrame;

      pDgOle2Frame = pDgOle;
    }
  }

  if( !pDgOle2Frame.isNull() )
  {
    addElementToDatabase(pDgOle2Frame, owner);
    copyElementProperties(pDgOle2Frame, pDbOle, owner->database());
    copyElementProperties(pDgShape, pDbOle, owner->database());
    appendOdDgElementToOwner(owner, pDgOle2Frame);
    OdDgnExportContext::addDgnElement(pDbOle->objectId(), pDgOle2Frame->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

void OdDbWipeoutExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbWipeout* pDbWipeout = (OdDbWipeout*)(pDbEntity);

  if (!pDbWipeout)
    return;

  OdGePoint3d  ptOrigin;
  OdGeVector3d vrU, vrV;
  pDbWipeout->getOrientation(ptOrigin, vrU, vrV);

  OdGePoint2dArray arrClipPts2d = pDbWipeout->clipBoundary();

  OdGePoint3dArray  ptsClip;
  for (OdUInt32 i = 0; i < arrClipPts2d.size(); i++)
  {
    OdGePoint3d ptClip = ptOrigin + vrU * (arrClipPts2d[i].x + 0.5) + vrV * (1.0 - (arrClipPts2d[i].y + 0.5));
    ptsClip.push_back(ptClip);
  }

  if( ptsClip.size() > 2)
  {
    OdDgGraphicsElementPtr pShape = createDgShape(ptsClip, pDbEntity, owner->database());

    if (!pShape.isNull())
      pShape->setFillColor(255);

    copyElementProperties(pShape, pDbEntity, owner->database());
    appendOdDgElementToOwner(owner, pShape);
    OdDgnExportContext::addDgnElement(pDbEntity->objectId(), pShape->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

void OdDbRasterImageExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  if( !owner->isKindOf(OdDgModel::desc()) )
    return;

  OdDbRasterImage* pDbImage = (OdDbRasterImage*)(pDbEntity);

  if( !pDbImage)
    return;

  OdDbObjectId idDbImageDef = pDbImage->imageDefId();

  if(idDbImageDef.isNull() )
    return;

  OdDbRasterImageDefPtr pDbImageDef = idDbImageDef.openObject(OdDb::kForRead);

  if( pDbImageDef.isNull() )
    return;

  OdGePoint3d  ptOrigin;
  OdGeVector3d vrU, vrV;

  OdDgRasterAttachmentHeaderPtr pDgRaster = OdDgRasterAttachmentHeader::createObject();
  addElementToDatabase(pDgRaster, owner);
  appendOdDgElementToOwner(owner, pDgRaster);

  pDgRaster->setFilename(pDbImageDef->activeFileName());
  pDgRaster->setFilePath(pDbImageDef->sourceFileName());
  OdGeVector2d vrImageSize = pDbImage->imageSize();

  if (OdZero(vrImageSize.x))
    vrImageSize.x = 100.0;

  if (OdZero(vrImageSize.y))
    vrImageSize.y = 100.0;

  pDbImage->getOrientation(ptOrigin, vrU, vrV);
  pDgRaster->setOrientation(ptOrigin, vrU / vrImageSize.x, vrV / vrImageSize.y);
  pDgRaster->setBrightness((pDbImage->brightness() - 50.0) / 50.0);
  pDgRaster->setContrast((pDbImage->contrast() - 50.0) / 50.0);

  OdGePoint2dArray arrClipPts = pDbImage->clipBoundary();

  for (OdUInt32 i = 0; i < arrClipPts.size(); i++)
    arrClipPts[i].y = vrImageSize.y - arrClipPts[i].y;

  if (!arrClipPts.isEmpty())
  {
    if( pDbImage->isClipInverted() )
      arrClipPts.insertAt(0, OdDgLineString2d::getBreakLineValue());

    pDgRaster->addBoundaryClipVertices(arrClipPts);
  }

  pDgRaster->setClippingFlag(pDbImage->isClipped());

  OdDgRasterFramePtr pRasterFrame;

  if (!pDgRaster->getRasterFrameId().isNull())
    pRasterFrame = pDgRaster->getRasterFrameId().openObject(OdDg::kForWrite);
  else
  {
    OdDgElementId idRasterFrame = pDgRaster->createRasterFrame();
    pRasterFrame = idRasterFrame.openObject(OdDg::kForWrite);
  }

  copyElementProperties(pRasterFrame, pDbImage, owner->database(), true);

  pRasterFrame->setDrawingPlane(OdDgRasterFrame::kDesignPlane);

  if( !pDbImage->isSetDisplayOpt(OdDbRasterImage::kShow) )
  {
    for (OdUInt32 i = 0; i < 8; i++)
      pRasterFrame->setDisplayAtViewFlag(i, false);
  }

  pRasterFrame->setTransparentFlag(pDbImage->isSetDisplayOpt(OdDbRasterImage::kTransparent));

  OdDgnExportContext::addDgnElement(pDbImage->objectId(), pRasterFrame->elementId());
}

//---------------------------------------------------------------------------------------------------

}
