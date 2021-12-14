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
#include "DbDatabase.h"
#include "OdFileBuf.h"

#include "DbAveDumper.h"
#include "toString.h"

#include "Ve/AveScene.h"
#include "Ve/AveLight.h"
#include "Ve/AveMaterial.h"

#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "DbLayerTable.h"
#include "DbLayerTableRecord.h"
#include "DbViewTable.h"
#include "DbViewTableRecord.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"

#include "Db3dSolid.h"

//----------------------------------------------------------
//
// DbAveDumper
//
//----------------------------------------------------------


/************************************************************************/
/* Dump the AVE information in the specified database                   */
/************************************************************************/
void DbAveDumper::dump(OdDbDatabase* pDb, int indent)
{
  dumpMaterials(pDb, indent);
  dumpLights(pDb, indent);
  dumpScenes(pDb, indent);
  dumpEntityMats(pDb, indent);
  dumpLayerMats(pDb, indent);
  dumpAciMats(pDb, indent);
}

/************************************************************************/
/* Dump the AVE materials in the specified database                     */
/************************************************************************/
void DbAveDumper::dumpMaterials(OdDbDatabase* pDb, int indent)
{
  /**********************************************************************/
  /* Get an iterator                                                    */
  /**********************************************************************/
  OdRxIteratorPtr pIt = oddbGetMaterials(pDb);
  /**********************************************************************/
  /* Dump the materials                                                 */
  /**********************************************************************/
  dumpMaterials(pIt, indent);
}

/************************************************************************/
/* Dump the AVE materials in the specified MLI file                     */
/************************************************************************/
void DbAveDumper::dumpMaterials(OdStreamBuf* pStreamBuf, int indent)
{
  /**********************************************************************/
  /* Get an iterator                                                    */
  /**********************************************************************/
  OdRxIteratorPtr pIt = oddbGetMaterials(pStreamBuf);
  /**********************************************************************/
  /* Dump the materials                                                 */
  /**********************************************************************/
  dumpMaterials(pIt, indent);
}

/************************************************************************/
/* Dump the AVE materials with the specified iterator                   */
/************************************************************************/
void DbAveDumper::dumpMaterials(OdRxIteratorPtr& pIt, int indent)
{
  writeLine();
  writeLine(indent,OD_T("Materials:"));
  if (pIt->done())
  {
    writeLine();
    writeLine(indent+1, OD_T("*none*"));
  }
  
  for (; !pIt->done(); pIt->next())
  {
    OdAveMaterialPtr pMat = pIt->object();
    if (!pMat.isNull())
    {
      writeLine();
      switch (pMat->type())
      {
        case OdAve::kStandard:
          {   
            writeLine(indent+1, pMat->name(),     toString(pMat->type()));
            writeLine(indent+2, OD_T("Diffuse factor"), toString(pMat->diffuseFactor()));
            writeLine(indent+2, OD_T("Diffuse color"),  toRGBString(pMat->diffuseColor()));
            writeLine(indent+2, OD_T("Ambient factor"), toString(pMat->ambientFactor()));
            writeLine(indent+2, OD_T("Ambient color"),  toRGBString(pMat->ambientColor()));
            writeLine(indent+2, OD_T("Ambient lock"),   toString(pMat->ambientLock()));
          }
          break;
        case OdAve::kGranite:
          {
            OdAveGraniteMaterialPtr pGrMat = pMat;
            writeLine(indent+1, pGrMat->name(),   toString(pGrMat->type()));
            writeLine(indent+2, OD_T("kFirst"));
            writeLine(indent+3, OD_T("Color factor"),   toString(pGrMat->colorFactor(OdAveGraniteMaterial::kFirst)));
            writeLine(indent+3, OD_T("Color"),          toRGBString(pGrMat->color(   OdAveGraniteMaterial::kFirst)));
            writeLine(indent+2, OD_T("kSecond"));
            writeLine(indent+3, OD_T("Color factor"),   toString(pGrMat->colorFactor(OdAveGraniteMaterial::kSecond)));
            writeLine(indent+3, OD_T("Color"),          toRGBString(pGrMat->color(   OdAveGraniteMaterial::kSecond)));
            writeLine(indent+2, OD_T("kThird"));
            writeLine(indent+3, OD_T("Color factor"),   toString(pGrMat->colorFactor(OdAveGraniteMaterial::kThird)));
            writeLine(indent+3, OD_T("Color"),          toRGBString(pGrMat->color(   OdAveGraniteMaterial::kThird)));
            writeLine(indent+2, OD_T("kFourth"));
            writeLine(indent+3, OD_T("Color factor"),   toString(pGrMat->colorFactor(OdAveGraniteMaterial::kFourth)));
            writeLine(indent+3, OD_T("Color"),          toRGBString(pGrMat->color(   OdAveGraniteMaterial::kFourth)));
          }
          break;
        case OdAve::kMarble:
        {
          OdAveMarbleMaterialPtr pMrMat = pMat;
          writeLine(indent+1, pMrMat->name(),  toString(pMrMat->type()));
          writeLine(indent+2, OD_T("Stone color"),   toRGBString(pMrMat->stoneColor()));
          writeLine(indent+2, OD_T("Vein color"),    toRGBString(pMrMat->veinColor()));
          writeLine(indent+2, OD_T("Turbulence"),    toString((int)pMrMat->turbulence()));
          writeLine(indent+2, OD_T("Sharpness"),     toString(pMrMat->sharpness()));
          writeLine(indent+2, OD_T("Scale"),         toString(pMrMat->scale()));
          break;
        }
        case OdAve::kWood:
          {
            OdAveWoodMaterialPtr pWdMat = pMat;
          writeLine(indent+1, pWdMat->name(),  toString(pWdMat->type()));
          writeLine(indent+2, OD_T("Light color"),   toRGBString(pWdMat->lightColor()));
          writeLine(indent+2, OD_T("Dark color"),    toRGBString(pWdMat->darkColor()));
          writeLine(indent+2, OD_T("Light/Dark"),    toString(pWdMat->lightDark()));
          writeLine(indent+2, OD_T("Ring density"),  toString(pWdMat->ringDensity()));
          writeLine(indent+2, OD_T("Ring width"),    toString(pWdMat->ringWidth()));
          writeLine(indent+2, OD_T("Ring shape"),    toString(pWdMat->ringShape()));
          writeLine(indent+2, OD_T("Scale"),         toString(pWdMat->scale()));
          }
          break;
      }
      writeLine(indent+2, OD_T("Specular factor"),   toString(pMat->specularFactor()));
      writeLine(indent+2, OD_T("Specular color"),    toRGBString(pMat->specularColor()));
      writeLine(indent+2, OD_T("Specular mirror"),   toString(pMat->specularMirror()));
      writeLine(indent+2, OD_T("Specular lock"),     toString(pMat->specularLock()));
      writeLine(indent+2, OD_T("Roughness"),         toString(pMat->roughness()));
    }
  }
}

/************************************************************************/
/* Dump the AVE lights in the specified database                        */
/************************************************************************/
void DbAveDumper::dumpLights(OdDbDatabase* pDb, int indent)
{

  writeLine();
  writeLine(indent,OD_T("Lights:"));

  OdRxIteratorPtr pIt = oddbGetLights(pDb);
  if (pIt->done())
  {
    writeLine();
    writeLine(indent+1, OD_T("*none*"));
  }

  for (; !pIt->done(); pIt->next())
  {
    OdAveLightPtr pLt = pIt->object();
    if (!pLt.isNull())
    {
      writeLine();
      writeLine(indent+1, pLt->name(),        toString(pLt->type()));
      writeLine(indent+2, OD_T("Intensity"),        toString(pLt->intensity()));
      writeLine(indent+2, OD_T("Color"),            toRGBString(pLt->color()));
      writeLine(indent+2, OD_T("Position"),         toString(pLt->position()));
      writeLine(indent+2, OD_T("Target"),           toString(pLt->target()));
      writeLine(indent+2, OD_T("Attenuation"),      toString(pLt->attenuation()));
      writeLine(indent+2, OD_T("Inner cone angle"), toString(pLt->innerConeAngle())+OD_T("d"));
      writeLine(indent+2, OD_T("Outer cone angle"), toString(pLt->outerConeAngle())+OD_T("d"));
    }
  }
}

/************************************************************************/
/* Dump the AVE materials attached to entities in the specified database*/
/************************************************************************/
void DbAveDumper::dumpEntityMats(OdDbDatabase* pDb, int indent)
{

  writeLine();
  writeLine(indent,OD_T("Entities with Attached Materials:"));
  writeLine();

  /**********************************************************************/
  /* Open the block table                                               */
  /**********************************************************************/
  OdDbBlockTablePtr pBlocks = pDb->getBlockTableId().safeOpenObject();

  /**********************************************************************/
  /* Get a block table iterator                                         */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pBlkIter = pBlocks->newIterator();
  
  bool any = false;

  /**********************************************************************/
  /* Step through the block table                                       */
  /**********************************************************************/
  for (pBlkIter->start(); ! pBlkIter->done(); pBlkIter->step())
  {
    /********************************************************************/
    /* Open the block table record                                      */
    /********************************************************************/
    OdDbBlockTableRecordPtr pBlock = pBlkIter->getRecordId().safeOpenObject();
    
    /********************************************************************/
    /* Get an entity iterator                                           */
    /********************************************************************/
    OdDbObjectIteratorPtr pEntIter = pBlock->newIterator();
    
    /********************************************************************/
    /* Step through the block table record                              */
    /********************************************************************/
    for (; !pEntIter->done(); pEntIter->step())
    {
      OdDbObjectId matId;
      if (OdAveMaterial::getAttachedMaterial(pEntIter->objectId(), matId))
      {
        writeLine(indent+1, 
          toString(pEntIter->objectId()) + OD_T(" ") + toString(pEntIter->objectId().getHandle()), 
          OdAveMaterial::getMaterialName(matId));
        any = true;
      }
    }
  }

  if (!any)
    writeLine(indent+1, OD_T("*none*"));
}

/************************************************************************/
/* Dump the AVE materials attached to layers in the specified database  */
/************************************************************************/
void DbAveDumper::dumpLayerMats(OdDbDatabase* pDb, int indent)
{
  writeLine();
  writeLine(indent,OD_T("Layers with Attached Materials:"));
  writeLine();

  /**********************************************************************/
  /* Open the layer table                                               */
  /**********************************************************************/
  OdDbLayerTablePtr pLayers = pDb->getLayerTableId().safeOpenObject();


  /**********************************************************************/
  /* Get a layer table iterator                                         */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pLyrIter = pLayers->newIterator();

  bool any = false;

  /**********************************************************************/
  /* Step through the layer table                                       */
  /**********************************************************************/
  for (pLyrIter->start(); !pLyrIter->done(); pLyrIter->step())
  {
    // Get attached material
    OdDbObjectId matId;
    if (OdAveMaterial::getAttachedMaterial(pLyrIter->getRecordId(), matId))
    {
      /******************************************************************/
      /* Open the layer table record                                    */
      /******************************************************************/
      OdDbLayerTableRecordPtr pLayer = pLyrIter->getRecordId().safeOpenObject();
      writeLine(indent+1, toString(pLayer->getName()), OdAveMaterial::getMaterialName(matId));
      any = true;
    }
  }

  if (!any)
    writeLine(indent+1, OD_T("*none*"));
}

/************************************************************************/
/* Dump the AVE materials attached to ACIs in the specified database    */
/************************************************************************/
void DbAveDumper::dumpAciMats(OdDbDatabase* pDb, int indent)
{
  writeLine();
  writeLine(indent,OD_T("ACIs with Attached Materials:"));
  writeLine();
  
  /**********************************************************************/
  /* Get the ACI Material map                                           */
  /**********************************************************************/
  OdDbObjectIdArray matIds;
  OdAveMaterial::getAciMaterialMap(pDb, matIds);

  bool any = false;
  
  /**********************************************************************/
  /* Step through the material map                                      */
  /**********************************************************************/
  for (OdUInt16 i = 0; i < matIds.size(); ++i)
  {
    if (!matIds[i].isNull())
    {
      writeLine(indent+1, toString(i+1), OdAveMaterial::getMaterialName(matIds[i]));
      any = true;
    }
  }

  if (!any)
    writeLine(indent+1, OD_T("*none*"));
}

/************************************************************************/
/* Dump the AVE scenes in the specified database                        */
/************************************************************************/
void DbAveDumper::dumpScenes(OdDbDatabase* pDb, int indent)
{

  writeLine();
  writeLine(indent,OD_T("Scenes"));

  OdRxIteratorPtr pIt = oddbGetScenes(pDb);
  if (pIt->done())
  {
    writeLine();
    writeLine(indent+1, OD_T("*none*"));
  }

  for (; !pIt->done(); pIt->next())
  {
    OdAveScenePtr pSc = pIt->object();
    if (!pSc.isNull())
    {
      /******************************************************************/
      /* Dump the name of the view associated with this scene           */
      /******************************************************************/      
      OdDbViewTableRecordPtr pView =  OdDbViewTableRecord::cast(pSc->view().openObject());
      OdString sViewName = OD_T("*CURRENT*");
      if (!pView.isNull())
        sViewName = pView->getName();
      writeLine();
      writeLine(indent+1, pSc->name());
      writeLine(indent+2, OD_T("View"), sViewName);

      /******************************************************************/
      /* Dump the names of the lights associated with this scene        */
      /******************************************************************/      
      OdDbObjectIdArray lightIds;
      pSc->lights(lightIds);
      for (OdUInt16 i = 0; i < lightIds.size(); ++i)
      {
        if (!lightIds[i].isNull())
        {
          writeLine(indent+2, OD_T("Light"), OdAveLight::getLightName(lightIds[i]));
        }
      }
    }
  }
}

//----------------------------------------------------------
//
// DbAveFiller
//
//----------------------------------------------------------
/************************************************************************/
/* Add a new kStandard material to the specified database               */
/************************************************************************/
OdAveMaterialPtr DbAveFiller::addMaterial(OdDbDatabase* pDb,
                                          const OdString& name,
                                          double diffuseFactor,
                                          OdUInt32 diffuseColor,
                                          double ambientFactor,
                                          OdUInt32 ambientColor,
                                          double specularFactor,
                                          OdUInt32 specularColor,
                                          double roughness,
                                          double transparency)
{
  OdAveMaterialPtr pNewMat = OdAveMaterial::createObject();

  pNewMat->setName(name);
  pNewMat->setDiffuseFactor(diffuseFactor);
  pNewMat->setDiffuseColor(diffuseColor);
  pNewMat->setAmbientFactor(ambientFactor);
  pNewMat->setAmbientColor(ambientColor);
  pNewMat->setAmbientLock(false);
  pNewMat->setSpecularFactor(specularFactor);
  pNewMat->setSpecularColor(specularColor);
  pNewMat->setSpecularMirror(false);
  pNewMat->setSpecularLock(false);
  pNewMat->setRoughness(roughness);
  pNewMat->setTransparency(transparency);

  if (pNewMat->writeMaterial(pDb) != eOk)
    pNewMat = NULL;

  return pNewMat;
}

/************************************************************************/
/* Add a new light to the specified database                            */
/************************************************************************/
OdAveLightPtr DbAveFiller::addLight(OdDbDatabase* pDb,
                                    const OdString& name,
                                    OdGsLight::LightType type,
                                    double intensity,
                                    OdUInt32 color,
                                    const OdGePoint3d& position,
                                    const OdGePoint3d& target,
                                    OdGsLight::LightAttenuation attenuation,
                                    double innerConeAngle,
                                    double outerConeAngle)
{
  OdAveLightPtr pNewLight = OdAveLight::createObject();

  pNewLight->setName(name);
  pNewLight->setType(type);
  pNewLight->setIntensity(intensity);
  pNewLight->setColor(color);
  pNewLight->setPosition(position);
  pNewLight->setTarget(target);
  pNewLight->setAttenuation(attenuation);

  if (type == OdGsLight::kSpotlight)
  {
    pNewLight->setConeAngle(innerConeAngle, outerConeAngle);
  }

  pNewLight->writeLight(pDb);

  return pNewLight;
}

/************************************************************************/
/* Add a new scene to the specified database                            */
/************************************************************************/
void DbAveFiller::addScene(OdDbDatabase* pDb,
                           const OdString& name,
                           const OdDbObjectId& viewId,
                           const OdDbObjectIdArray& lightIds)
{
  OdAveScenePtr pNewScene = OdAveScene::createObject();

  pNewScene->setName(name);
  pNewScene->setView(viewId);
  pNewScene->setLights(lightIds);
  pNewScene->writeScene(pDb);
}

/************************************************************************/
/* Populate the specified database with AVE stuff                       */
/************************************************************************/
void DbAveFiller::fillDatabase(OdDbDatabase* pDb)
{
  /**********************************************************************/
  /* Add a test object                                                  */
  /**********************************************************************/

  /**********************************************************************/
  /* Open Model Space for Writing                                       */
  /**********************************************************************/
  OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Create a 3d Solid                                                  */
  /**********************************************************************/
  OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
  OdDbObjectId exEntId = pMs->appendOdDbEntity(p3dSolid);

  /**********************************************************************/
  /* Read an SAT file, or create a solid                                */
  /**********************************************************************/
  const OdChar* satFileName = OD_T("OdAveEx.sat");
  if(::odSystemServices()->accessFile(satFileName, Oda::kFileRead))
  {
    OdRdFileBuf satFile(satFileName);
    p3dSolid->acisIn(&satFile);
  }
  else
  {
    p3dSolid->createBox( 7.0, 4.0, 5.0 );
    p3dSolid->transformBy(OdGeMatrix3d::translation(OdGeVector3d(10.0, 4.0, 2.5)));
  }
  
  /**********************************************************************/
  /* Open the active viewport                                           */
  /**********************************************************************/
  OdDbViewportTableRecordPtr pVp = 
    OdDbViewportTablePtr(pDb->getViewportTableId().safeOpenObject())
    ->getActiveViewportId().safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Set the view direction and center point                            */
  /**********************************************************************/
  pVp->setViewDirection(OdGeVector3d(-1.0, 1.0, 1.0));
  pVp->setCenterPoint(OdGePoint2d(-10.0, 4.0));

  /**********************************************************************/
  /* Add a standard material                                            */
  /**********************************************************************/
  OdAveMaterialPtr pMat = addMaterial(pDb,
                                      OD_T("BLUE GLASS EXMPL"),
                                      0.784314,
                                      ODRGB(108,129,255),
                                      0.784314,
                                      ODRGB(108,129,255),
                                      1.0,
                                      ODRGB(255,255,255),
                                      0.48,
                                      0.65);

  /**********************************************************************/
  /* Assign it to our test object                                       */
  /**********************************************************************/
  if (!pMat.isNull())
    pMat->attachToObject(exEntId);

  /**********************************************************************/
  /* Add a spotlight                                                    */
  /**********************************************************************/
  OdAveLightPtr pLt = addLight(pDb, 
                               OD_T("SP_LT"),
                               OdGsLight::kSpotlight,
                               1.0,
                               ODRGB(255,255,255),
                               OdGePoint3d(10.0, 4.0, 7.0),
                               OdGePoint3d(10.0, 4.0, 5.0),
                               OdGsLight::kAttenNone,
                               22.5, 30.0);


  OdDbObjectIdArray lightIds;
  lightIds.push_back(pLt->holderId());

  /**********************************************************************/
  /* Add a scene                                                        */
  /**********************************************************************/
  addScene(pDb, OD_T("SCENE_EX"), OdDbObjectId::kNull, lightIds);
}

