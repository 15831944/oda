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

#include "StdAfx.h"
#include "DbDictionary.h"
#include "DbRasterImage.h"
#include "DbRasterImageDef.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiRasterWrappers.h"


/************************************************************************/
/* OdRasterImgEx command demonstates the OdDbRasterImage entity         */
/* functionality  and pupulates database with Raster Image entity.      */
/************************************************************************/


void showStat(OdDbRasterImage* pImage, OdDbRasterImageDef* pImageDef, OdDbUserIO* pIO)
{
  OdString message;

  pIO->putString(L"\n/////////////IMAGE DEFINITION DATA/////////////");
  // Source file name
  message.format(L"\nSource File Name %s", pImageDef->sourceFileName().c_str());
  pIO->putString(message);
  // Loading status
  message.format(L"\nSource Image is %s", pImageDef->isLoaded()? L"loaded" : L"not loaded");
  pIO->putString(message);
  // Image size
  message.format(L"\nSource Image Size = %f , %f", pImageDef->size().x, pImageDef->size().y);
  pIO->putString(message);

  pIO->putString(L"\n/////////////IMAGE DATA/////////////");
  // kShow status
  message.format(L"\nIs shown? = %s", pImage->isSetDisplayOpt(OdDbRasterImage::kShow)? L"Yes" : L"No");
  pIO->putString(message);
  // Brightness value
  message.format(L"\nBrightness = %i", pImage->brightness());
  pIO->putString(message);
  // Contrast value
  message.format(L"\nContrast = %i", pImage->contrast());
  pIO->putString(message);
  // Fade value
  message.format(L"\nFade = %i", pImage->fade());
  pIO->putString(message);
  // Orientation data
  OdGePoint3d origin; OdGeVector3d u,v;
  pImage->getOrientation(origin,u,v);
  message.format(L"\nOrigin = [%f %f %f]", origin.x, origin.y, origin.z);
  pIO->putString(message);
  message.format(L"\nU = [%f %f %f]", u.x, u.y, u.z);
  pIO->putString(message);
  message.format(L"\nV = [%f %f %f]", v.x, v.y, v.z);
  pIO->putString(message);
  // Image corners
  OdGePoint3dArray vertices;
  pImage->getVertices(vertices);
  for (unsigned int i = 0; i<vertices.size(); i++)
  {
    message.format(L"\nPoint[%i] = %f %f %f", i, vertices[i].x, vertices[i].y, vertices[i].z);
    pIO->putString(message);
  }
  // Image size
  OdGeVector2d imSize = pImage->imageSize();
  message.format(L"\nImage Size = %f %f", imSize.x, imSize.y);
  pIO->putString(message);
  // isClipped status
  message.format(L"\nImage is %s", pImage->isClipped()? L"clipped" : L"not clipped");
  pIO->putString(message);
  if (pImage->isClipped())
  {
    // Clip boundary type
    message.format(L"\nClip BoundaryType = %i", pImage->clipBoundaryType());
    pIO->putString(message);
    // Clip boundary points
    OdGePoint2dArray clipArr =  pImage->clipBoundary();
    for (unsigned int i = 0; i < clipArr.size(); i++)
    {
      message.format(L"\nClip Point[%i] = %f %f", i, clipArr[i].x, clipArr[i].y);
      pIO->putString(message);
    }
    // Clip inversion status
    message.format(L"\nClip boundary is %s", pImage->isClipped()? L"inverted" : L"not inverted");
    pIO->putString(message);
  }
}


void _OdRasterImgEx_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  /**********************************************************************/
  /* Create the pointer to current active space                         */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = DevGuideCommandsUtils::activeBlockTableRecord(pDb).safeOpenObject(OdDb::kForWrite);


  /**********************************************************************/
  /* Create raster image entity                                         */
  /**********************************************************************/
  // Open the Image Dictionary
  OdDbObjectId imageDictId = OdDbRasterImageDef::createImageDictionary(pDb);
  OdDbDictionaryPtr pImageDict = imageDictId.safeOpenObject(OdDb::kForWrite);

  // Create an ImageDef object
  OdDbRasterImageDefPtr pImageDef = OdDbRasterImageDef::createObject();
  OdDbObjectId imageDefId = pImageDict->setAt(OD_T("OdRasterImageEx"), pImageDef);

  // Set some parameters
  OdString filter;
  filter += L"Bitmap files (*.bmp)|*.bmp|";
  filter += L"Jpeg files (*.jpg)|*.jpg|";
  filter += L"PNG files (*.png)|*.png|";
  filter += L"All files (*.*)|*.*|";
  filter += L"|";
  OdString path = pIO->getFilePath(L"Select file to insert",
                                    OdEd::kGfpForOpen,
                                    L"Select file to insert", L"bmp", L"", filter);
  if (path.isEmpty())
    return;

  pImageDef->setSourceFileName(path);
  
  // Load image
  pImageDef->image();
  {
    // Create an Image object
    OdDbRasterImagePtr pImage = OdDbRasterImage::createObject();
    pImage->setDatabaseDefaults(pDb);
    bBTR->appendOdDbEntity(pImage);

    // Set some parameters
    pImage->setImageDefId(imageDefId);
    pImage->setOrientation(OdGePoint3d(0,0,5), OdGeVector3d(50, 0, 0), OdGeVector3d(0, 50, 0));
    pImage->setDisplayOpt(OdDbRasterImage::kShow, true);
    pImage->setDisplayOpt(OdDbRasterImage::kShowUnAligned, true);
    pImage->setFade(20);
    pImage->setBrightness(70);
    pImage->setContrast(60);
    OdGePoint2dArray clip;
    clip.setLogicalLength(4);
    clip[0].x = 120; clip[0].y = 0;
    clip[1].x = 250; clip[1].y = 500;
    clip[2].x = 500; clip[2].y = 200; 
    clip[3].x = 120; clip[3].y = 0;
    pImage->setClipBoundary(clip);
    pImage->setClipInverted(true);
    showStat(pImage, pImageDef, pIO);
  }
}
