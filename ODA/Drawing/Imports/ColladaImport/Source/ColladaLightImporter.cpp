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
#include "ColladaLightImporter.h"
#include "ColladaDocumentImporter.h"
#include "COLLADAFWLight.h"
#include "DbLight.h"
#include "CmColor.h"

namespace TD_COLLADA_IMPORT
{
  LightImporter::LightImporter( DocumentImporter* documentImporter )
    : ImporterBase(documentImporter)
  {
  }

  LightImporter::~LightImporter()
  {
  }

  bool LightImporter::import(const COLLADAFW::Light& colladaLight)
  {
    OdDbBlockTableRecordPtr pBTR = this->getDocumentImporter()->getGeometryTmpBTR();
    OdDbLightPtr pLight = OdDbLight::createObject();
    pLight->setDatabaseDefaults(pBTR->database());
    pBTR->appendOdDbEntity(pLight);

    COLLADAFW::Light::LightType lightType = colladaLight.getLightType();
    switch (lightType)
    {
    case COLLADAFW::Light::AMBIENT_LIGHT:
      pLight->setLightType(OdGiDrawable::kAmbientLight);
      return 0;
    case COLLADAFW::Light::POINT_LIGHT: 
      pLight->setLightType(OdGiDrawable::kPointLight);
      pLight->setPosition(OdGePoint3d::kOrigin);
      break;
    case COLLADAFW::Light::DIRECTIONAL_LIGHT: 
      pLight->setLightType(OdGiDrawable::kDistantLight);
      pLight->setTargetLocation(OdGePoint3d::kOrigin - OdGeVector3d::kZAxis);
      break;
    case COLLADAFW::Light::SPOT_LIGHT: 
      pLight->setLightType(OdGiDrawable::kSpotLight);
      pLight->setPosition(OdGePoint3d::kOrigin);
      pLight->setTargetLocation(OdGePoint3d::kOrigin - OdGeVector3d::kZAxis);
      break;
    default: 
      return 0;
    }
     
    COLLADAFW::String pStr = colladaLight.getName();
    OdString odStr(pStr.c_str());
    pLight->setName(odStr);

    addUniqueIdObjectIdPair(colladaLight.getUniqueId(), pLight->objectId());
    addUniqueIdSubUniqueIdPair(colladaLight.getUniqueId(), colladaLight.getUniqueId());

    if (lightType == COLLADAFW::Light::POINT_LIGHT || lightType == COLLADAFW::Light::SPOT_LIGHT)
    {
      OdGiLightAttenuation atten;
      if (colladaLight.getConstantAttenuation() > 0) 
      {
        atten.setAttenuationType(OdGiLightAttenuation::kNone);
        atten.setUseLimits(false);
        pLight->setIntensity(colladaLight.getConstantAttenuation());
      }
      else if (colladaLight.getLinearAttenuation() > 0) 
      {
        atten.setAttenuationType(OdGiLightAttenuation::kInverseLinear);
        atten.setLimits(1, 0);
        atten.setUseLimits(true);
        pLight->setIntensity(colladaLight.getLinearAttenuation());
      }
      else if (colladaLight.getQuadraticAttenuation() > 0) 
      {
        atten.setAttenuationType(OdGiLightAttenuation::kInverseSquare);
        atten.setLimits(1, 0);
        atten.setUseLimits(true);
        pLight->setIntensity(colladaLight.getQuadraticAttenuation());
      }
      pLight->setLightAttenuation(atten);
    }

    if (lightType == COLLADAFW::Light::SPOT_LIGHT || lightType == COLLADAFW::Light::DIRECTIONAL_LIGHT)
    {
      // Angles are shared between spot and directional lights.
      pLight->setHotspotAndFalloff(0, colladaLight.getFallOffAngle());
    }

    COLLADAFW::Color pCldCollor = colladaLight.getColor(); 
    OdCmColor color;
    color.setRGB((OdUInt8)pCldCollor.getRed(), (OdUInt8)pCldCollor.getGreen(), (OdUInt8)pCldCollor.getBlue());
    pLight->setLightColor(color);

    return true;
  }
}
