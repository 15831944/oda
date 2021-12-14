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

#ifndef _DB_AVEDUMPER_H
#define _DB_AVEDUMPER_H

#include "Ve/AveDefs.h"
#include "Ve/AveMaterial.h"
#include "Ve/AveLight.h"
        
class DbAveDumper
{
public:
  DbAveDumper() {}
  void dump(OdDbDatabase* pDb, int indent = 0);
  void dumpMaterials(OdStreamBuf* pStreamBuf, int indent = 0);

private:
  void dumpMaterials(OdDbDatabase* pDb, int indent = 0);
  void dumpMaterials(OdRxIteratorPtr& pIt, int indent = 0);
  void dumpLights(OdDbDatabase* pDb, int indent = 0);
  void dumpScenes(OdDbDatabase* pDb, int indent = 0);
  void dumpEntityMats(OdDbDatabase* pDb, int indent = 0);
  void dumpLayerMats(OdDbDatabase* pDb, int indent = 0);
  void dumpAciMats(OdDbDatabase* pDb, int indent = 0);
};

class DbAveFiller
{
public:
  DbAveFiller() {}

  OdAveMaterialPtr addMaterial(OdDbDatabase* pDb,
                               const OdString& name,
                               double diffuse,
                               OdUInt32 diffuseColor,
                               double ambient,
                               OdUInt32 ambientColor,
                               double specular,
                               OdUInt32 specularColor,
                               double roughness,
                               double transparency);
  OdAveLightPtr addLight(OdDbDatabase* pDb,
                         const OdString& name,
                         OdGsLight::LightType type,
                         double intensity,
                         OdUInt32 color,
                         const OdGePoint3d& position,
                         const OdGePoint3d& target,
                         OdGsLight::LightAttenuation attenuation,
                         double innerConeAngle,
                         double outerConeAngle);

  void addScene(OdDbDatabase* pDb,
                const OdString& name,
                const OdDbObjectId& viewId,
                const OdDbObjectIdArray& lightIds);

  void fillDatabase(OdDbDatabase* pDb);
};

#endif
