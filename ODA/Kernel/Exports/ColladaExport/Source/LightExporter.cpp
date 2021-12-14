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

#include "ColladaExportView.h"
#include "COLLADASWExtraTechnique.h"

namespace TD_COLLADA_EXPORT
{
  LightExporter::LightExporter(COLLADASW::StreamWriter* streamWriter, OdStringArray& lightNamesArr, OdGePoint3dArray& lightPositionsArr, OdGeVector3dArray& lightDirsArr)
    : COLLADASW::LibraryLights(streamWriter)
    , m_lightNamesArr(lightNamesArr)
    , m_lightPositionsArr(lightPositionsArr)
    , m_lightDirsArr(lightDirsArr)
  {
    openLibrary();
  }

  LightExporter::~LightExporter()
  { 
    closeLibrary();
  }

  void LightExporter::exportLight(unsigned int iLightCounter, OdGiLightTraitsData* pLightTData, const ODCOLORREF* pPallete)
  {
    OdString buffer;

    COLLADASW::Light* colladaLight = 0;
    unsigned int iIndex = 0;

    switch (pLightTData->type())
    {
    case OdGiLightTraitsData::kDistantLight:
      {
        OdGiDistantLightTraitsData* pLightTraits = static_cast<OdGiDistantLightTraitsData*>(pLightTData);
        buffer.format(OD_T("Light-%d-distant"), iLightCounter); 
        iIndex = m_lightNamesArr.append(buffer);
        m_lightPositionsArr.append(OdGePoint3d::kOrigin);
        m_lightDirsArr.append(pLightTraits->direction());
        colladaLight = new COLLADASW::DirectionalLight(COLLADASW::LibraryLights::mSW, COLLADASW::NativeString(m_lightNamesArr.at(iIndex).c_str()).toString());
        colladaLight->setConstantAttenuation(pLightTraits->physicalIntensity());
        /*
        virtual bool isSunlight() const;*/
        break;
      }
    case OdGiLightTraitsData::kPointLight:
      {
        OdGiPointLightTraitsData* pLightTraits = static_cast<OdGiPointLightTraitsData*>(pLightTData);

        buffer.format(OD_T("Light-%d-point"), iLightCounter); 
        iIndex = m_lightNamesArr.append(buffer);
        m_lightPositionsArr.append(pLightTraits->position());
        m_lightDirsArr.append(-OdGeVector3d::kZAxis);
        colladaLight = new COLLADASW::PointLight(COLLADASW::LibraryLights::mSW, COLLADASW::NativeString(m_lightNamesArr.at(iIndex).c_str()).toString());
        OdGiLightAttenuation att = pLightTraits->attenuation();
        switch(att.attenuationType())
        {
        case OdGiLightAttenuation::kInverseLinear:
          {
            colladaLight->setLinearAttenuation(pLightTraits->physicalIntensity());
            break;
          }
        case OdGiLightAttenuation::kInverseSquare:
          {
            colladaLight->setQuadraticAttenuation(pLightTraits->physicalIntensity());
            break;
          }
        default:
          colladaLight->setConstantAttenuation(pLightTraits->physicalIntensity());
        }

        //virtual OdGePoint3d position() const;
        //((COLLADASW::BaseExtraTechnique*)colladaLight)->addExtraTechniqueChildParameter("OpenCOLLADATeigha", "positionX", "1");
        //((COLLADASW::BaseExtraTechnique*)colladaLight)->addExtraTechniqueChildParameter("OpenCOLLADATeigha", "positionY", "1");
        //((COLLADASW::BaseExtraTechnique*)colladaLight)->addExtraTechniqueChildParameter("OpenCOLLADATeigha", "positionZ", "1");
        break;
      }
    case OdGiLightTraitsData::kSpotLight:
      {
        OdGiSpotLightTraitsData* pLightTraits = static_cast<OdGiSpotLightTraitsData*>(pLightTData);
        buffer.format(OD_T("Light-%d-spot"), iLightCounter); 
        iIndex = m_lightNamesArr.append(buffer);
        m_lightPositionsArr.append(pLightTraits->position());
        OdGeVector3d dir = pLightTraits->target() - pLightTraits->position();
        double dirLength = dir.normalizeGetLength();
        ODA_ASSERT_ONCE(!OdZero(dirLength));
        m_lightDirsArr.append(OdZero(dirLength) ? -OdGeVector3d::kZAxis : dir);
        colladaLight = new COLLADASW::SpotLight(COLLADASW::LibraryLights::mSW, COLLADASW::NativeString(m_lightNamesArr.at(iIndex).c_str()).toString());
        OdGiLightAttenuation att = pLightTraits->attenuation();
        switch(att.attenuationType())
        {
        case OdGiLightAttenuation::kInverseLinear:
          {
            colladaLight->setLinearAttenuation(pLightTraits->physicalIntensity());
            break;
          }
        case OdGiLightAttenuation::kInverseSquare:
          {
            colladaLight->setQuadraticAttenuation(pLightTraits->physicalIntensity());
            break;
          }
        default:
          colladaLight->setConstantAttenuation(pLightTraits->physicalIntensity());
        }
        colladaLight->setFallOffAngle(pLightTraits->falloff());
        break;
      }
    }
    colladaLight->setIntensity(pLightTData->intensity());
    OdCmEntityColor color = fixByACI(pPallete, pLightTData->color());
    colladaLight->setColor(COLLADASW::Color(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0, 1.0));

    addLight(*colladaLight);
    delete colladaLight;
  }
}
