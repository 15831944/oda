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
#ifndef __EX_DGNFILLER_H__
#define __EX_DGNFILLER_H__

#include "Ge/GePoint2d.h"
#include "DgModel.h"

class OdDgModel;

class OdExDgnMatFiller
{
public:
  OdExDgnMatFiller();

  void fillDatabase(OdDgDatabase* pDb);
  void fillModel();

private:

  OdDgElementId addPlane( OdGePoint3d ptCenter, double dSize );
  OdDgElementId addCylinder( OdGePoint3d ptCenter, double dSize );
  OdDgElementId addBox( OdGePoint3d ptCenter, double dSize );
  OdDgElementId addSphere( OdGePoint3d ptCenter, double dSize );

  void createSection( OdGePoint3d ptOrigin, OdString strCaption );

  void applyMaterial( OdDgElementId idElement, OdString strMaterial );

  double convertMSFinishToGlossFactor( double dFinish );

  void addDiffuseMaterials();
  void addAmbientMaterials();
  void addSpecularMaterials();
  void addFinishMaterials();
  void addTransparencyMaterials();
  void addTextureMaterials();

  void createDiffuseSection();
  void createAmbientSection();
  void createSpecularSection();
  void createFinishSection();
  void createTransparencySection();
  void createTextureSection();

private:
  OdDgModelPtr m_pModel3d;
};

#endif // __EX_DGNFILLER_H__
