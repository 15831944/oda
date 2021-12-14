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

#include "CmColorBase.h"
#include "RxDictionary.h"

#include "TGVersion.h"
#include "DgDatabase.h"

#include "DgShape.h"
#include "DgCone.h"
#include "DgMesh.h"
#include "Dg3DObject.h"
#include "DgArc.h"
#include "DgLine.h"
#include "DgLineString.h"
#include "DgTable.h"
#include "DgMaterialTableRecord.h"
#include "DgAttributeLinkage.h"
#include "DgText.h"

#include "MemoryStream.h"

#include "ExDgnMatFiller.h"
#include "Ge/GeScale3d.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a
#ifndef _tcout
#ifdef TD_UNICODE
#define _tcout wcout
#else
#define _tcout cout
#endif
#endif

OdExDgnMatFiller::OdExDgnMatFiller()
{
  
}

void OdExDgnMatFiller::fillDatabase(OdDgDatabase* pDb)
{
  /********************************************************************/
  /* Fill the model                                                   */
  /********************************************************************/
  m_pModel3d = pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);

  //initialize measure units
  {
    OdDgModel::UnitDescription description;

    OdDgModel::fillUnitDescriptor( OdDgModel::kMeters, description );
    m_pModel3d->setMasterUnit( description );
    OdDgModel::fillUnitDescriptor( OdDgModel::kMillimeters, description );
    m_pModel3d->setSubUnit( description );

    m_pModel3d->setWorkingUnit( OdDgModel::kWuMasterUnit );
  }

  fillModel();

  m_pModel3d->fitToView();
}

//------------------------------------------------------------------------------

void OdExDgnMatFiller::fillModel()
{
  createDiffuseSection();
  createAmbientSection();
  createSpecularSection();
  createFinishSection();
  createTransparencySection();
  createTextureSection();
}

//------------------------------------------------------------------------------

void OdExDgnMatFiller::createSection( OdGePoint3d ptOrigin, OdString strCaption )
{
  OdDgLineString3dPtr pLine = OdDgLineString3d::createObject();

  pLine->addVertex( ptOrigin + OdGeVector3d(0,0,0) );
  pLine->addVertex( ptOrigin + OdGeVector3d(48,0,0) );
  pLine->addVertex( ptOrigin + OdGeVector3d(48,18,0) );
  pLine->addVertex( ptOrigin + OdGeVector3d(0,18,0) );
  pLine->addVertex( ptOrigin + OdGeVector3d(0,0,0) );

  m_pModel3d->addElement( pLine );

  OdDgLine3dPtr pCaptionLine = OdDgLine3d::createObject();

  pCaptionLine->setStartPoint( ptOrigin + OdGeVector3d(0,12,0) );
  pCaptionLine->setEndPoint( ptOrigin + OdGeVector3d(48,12,0) );

  m_pModel3d->addElement( pCaptionLine );

  OdDgText3dPtr pCaption = OdDgText3d::createObject();

  pCaption->setHeightMultiplier( 2 );
  pCaption->setLengthMultiplier( 2 );
  pCaption->setText( strCaption );
  pCaption->setOrigin( ptOrigin + OdGeVector3d(5,14,0));

  m_pModel3d->addElement( pCaption );
}

//------------------------------------------------------------------------------

void OdExDgnMatFiller::createDiffuseSection()
{
  createSection( OdGePoint3d(0,0,0), OD_T("Diffuse property") );

  OdDgElementId idPlane  = addPlane( OdGePoint3d(6,6,0), 10 );
  OdDgElementId idCone   = addCylinder( OdGePoint3d(18,6,0), 10 );
  OdDgElementId idBox    = addBox( OdGePoint3d(30,6,0), 10 );
  OdDgElementId idSphere = addSphere( OdGePoint3d(42,6,0), 10 );

  addDiffuseMaterials();

  applyMaterial( idPlane, OD_T("Diffuse1"));
  applyMaterial( idCone, OD_T("Diffuse2"));
  applyMaterial( idBox, OD_T("Diffuse3"));
  applyMaterial( idSphere, OD_T("Diffuse4"));
}

void OdExDgnMatFiller::createAmbientSection()
{
  createSection( OdGePoint3d(50,0,0), OD_T("Ambient property") );

  OdDgElementId idSphere1 = addSphere( OdGePoint3d(56,6,0), 10 );
  OdDgElementId idSphere2 = addSphere( OdGePoint3d(68,6,0), 10 );
  OdDgElementId idSphere3 = addSphere( OdGePoint3d(80,6,0), 10 );
  OdDgElementId idSphere4 = addSphere( OdGePoint3d(92,6,0), 10 );

  addAmbientMaterials();

  applyMaterial( idSphere1, OD_T("Ambient1"));
  applyMaterial( idSphere2, OD_T("Ambient2"));
  applyMaterial( idSphere3, OD_T("Ambient3"));
  applyMaterial( idSphere4, OD_T("Ambient4"));
}

void OdExDgnMatFiller::createSpecularSection()
{
  createSection( OdGePoint3d(0,20,0), OD_T("Specular property") );

  OdDgElementId idCone1   = addCylinder( OdGePoint3d(6,26,0), 10 );
  OdDgElementId idCone2   = addCylinder( OdGePoint3d(18,26,0), 10 );
  OdDgElementId idCone3   = addCylinder( OdGePoint3d(30,26,0), 10 );
  OdDgElementId idCone4   = addCylinder( OdGePoint3d(42,26,0), 10 );

  addSpecularMaterials();

  applyMaterial( idCone1, OD_T("Specular1"));
  applyMaterial( idCone2, OD_T("Specular2"));
  applyMaterial( idCone3, OD_T("Specular3"));
  applyMaterial( idCone4, OD_T("Specular4"));
}

void OdExDgnMatFiller::createFinishSection()
{
  createSection( OdGePoint3d(50,20,0), OD_T("Finish property") );

  OdDgElementId idSphere1  = addSphere( OdGePoint3d(56,26,0), 10 );
  OdDgElementId idSphere2  = addSphere( OdGePoint3d(68,26,0), 10 );
  OdDgElementId idSphere3  = addSphere( OdGePoint3d(80,26,0), 10 );
  OdDgElementId idSphere4  = addSphere( OdGePoint3d(92,26,0), 10 );

  addFinishMaterials();

  applyMaterial( idSphere1, OD_T("Finish1"));
  applyMaterial( idSphere2, OD_T("Finish2"));
  applyMaterial( idSphere3, OD_T("Finish3"));
  applyMaterial( idSphere4, OD_T("Finish4"));
}

void OdExDgnMatFiller::createTransparencySection()
{
  createSection( OdGePoint3d(0,40,0), OD_T("Transparency property") );

  OdDgElementId idPlane  = addPlane( OdGePoint3d(6,46,0), 10 );
  OdDgElementId idCone   = addCylinder( OdGePoint3d(18,46,0), 10 );
  OdDgElementId idBox    = addBox( OdGePoint3d(30,46,0), 10 );
  OdDgElementId idSphere = addSphere( OdGePoint3d(42,46,0), 10 );

  OdDgShape3dPtr pBgShape = OdDgShape3d::createObject();

  pBgShape->addVertex( OdGePoint3d( 0.5,40.5,-6) );
  pBgShape->addVertex( OdGePoint3d( 47.5,40.5,-6) );
  pBgShape->addVertex( OdGePoint3d( 47.5,51.5,-6) );
  pBgShape->addVertex( OdGePoint3d( 0.5,51.5,-6) );
  pBgShape->addVertex( OdGePoint3d( 0.5,40.5,-6) );

  m_pModel3d->addElement( pBgShape );

  addTransparencyMaterials();

  applyMaterial( idPlane, OD_T("Transparency1"));
  applyMaterial( idCone,  OD_T("Transparency2"));
  applyMaterial( idBox,   OD_T("Transparency3"));
  applyMaterial( idSphere,OD_T("Transparency4"));
}

void OdExDgnMatFiller::createTextureSection()
{
  createSection( OdGePoint3d(50,40,0), OD_T("Texture property") );

  OdDgElementId idPlane  = addPlane( OdGePoint3d(56,46,0), 10 );
  OdDgElementId idCone   = addCylinder( OdGePoint3d(68,46,0), 10 );
  OdDgElementId idBox    = addBox( OdGePoint3d(80,46,0), 10 );
  OdDgElementId idSphere = addSphere( OdGePoint3d(92,46,0), 10 );

  addTextureMaterials();

  applyMaterial( idPlane, OD_T("Texture1"));
  applyMaterial( idCone, OD_T("Texture2"));
  applyMaterial( idBox, OD_T("Texture3"));
  applyMaterial( idSphere, OD_T("Texture4"));
}

//------------------------------------------------------------------------------

void OdExDgnMatFiller::addDiffuseMaterials()
{
  OdDgDatabase* pDb = m_pModel3d->database();

  OdDgMaterialTablePtr pMatTable = pDb->getMaterialTable( OdDg::kForWrite );

  OdGiMaterialColor difColor;
  OdDgMaterialMap   difMap;

  OdGiMaterialColor specColor;

  OdDgMaterialTableRecordPtr pMatSpec = OdDgMaterialTableRecord::createObject();

  specColor.setColor( OdDgCmEntityColor(0,0,0) );
  specColor.setMethod( OdGiMaterialColor::kOverride );
  specColor.setFactor( 0.1 );

  OdDgMaterialTableRecordPtr pMatDif = OdDgMaterialTableRecord::createObject();

  difColor.setColor( OdDgCmEntityColor(255,255,0) );
  difColor.setMethod( OdGiMaterialColor::kOverride );
  difColor.setFactor( 0.5 );

  pMatDif->setName(OD_T("Diffuse1"));
  pMatDif->setDiffuse( difColor, difMap );
  pMatDif->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );
  
  pMatTable->add( pMatDif );

  pMatDif = OdDgMaterialTableRecord::createObject();

  difColor.setColor( OdDgCmEntityColor(0,255,0) );
  difColor.setMethod( OdGiMaterialColor::kOverride );
  difColor.setFactor( 1 );

  pMatDif->setName(OD_T("Diffuse2"));
  pMatDif->setDiffuse( difColor, difMap );
  pMatDif->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatDif );

  pMatDif = OdDgMaterialTableRecord::createObject();

  difColor.setColor( OdDgCmEntityColor(0,255,255) );
  difColor.setMethod( OdGiMaterialColor::kOverride );
  difColor.setFactor( 0.2 );

  pMatDif->setName(OD_T("Diffuse3"));
  pMatDif->setDiffuse( difColor, difMap );
  pMatDif->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatDif );

  pMatDif = OdDgMaterialTableRecord::createObject();

  difColor.setColor( OdDgCmEntityColor(0,0,255) );
  difColor.setMethod( OdGiMaterialColor::kOverride );
  difColor.setFactor( 1.1 );

  pMatDif->setName(OD_T("Diffuse4"));
  pMatDif->setDiffuse( difColor, difMap );
  pMatDif->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatDif );
}

void OdExDgnMatFiller::addAmbientMaterials()
{
  OdDgDatabase* pDb = m_pModel3d->database();

  OdDgMaterialTablePtr pMatTable = pDb->getMaterialTable( OdDg::kForWrite );

  OdDgMaterialTableRecordPtr pMatAmb = OdDgMaterialTableRecord::createObject();

  OdGiMaterialColor difColor;
  OdDgMaterialMap   difMap;

  OdGiMaterialColor specColor;

  difColor.setColor( OdDgCmEntityColor(255,255,255) );
  difColor.setMethod( OdGiMaterialColor::kOverride );
  difColor.setFactor( 0.3 );

  pMatAmb->setAmbientFactor( 0.1 );
  pMatAmb->setName(OD_T("Ambient1"));
  pMatAmb->setDiffuse( difColor, difMap );

  pMatTable->add(pMatAmb);

  pMatAmb = OdDgMaterialTableRecord::createObject();

  pMatAmb->setAmbientFactor( 1.0 );
  pMatAmb->setName(OD_T("Ambient2"));
  pMatAmb->setDiffuse( difColor, difMap );

  pMatTable->add(pMatAmb);

  pMatAmb = OdDgMaterialTableRecord::createObject();

  pMatAmb->setAmbientFactor( 2.0 );
  pMatAmb->setName(OD_T("Ambient3"));
  pMatAmb->setDiffuse( difColor, difMap );

  pMatTable->add(pMatAmb);

  pMatAmb = OdDgMaterialTableRecord::createObject();

  pMatAmb->setAmbientFactor( 3.0 );
  pMatAmb->setName(OD_T("Ambient4"));
  pMatAmb->setDiffuse( difColor, difMap );

  pMatTable->add(pMatAmb);
}

void OdExDgnMatFiller::addSpecularMaterials()
{
  OdDgDatabase* pDb = m_pModel3d->database();

  OdDgMaterialTablePtr pMatTable = pDb->getMaterialTable( OdDg::kForWrite );

  OdGiMaterialColor difColor;
  OdDgMaterialMap   difMap;

  difColor.setColor( OdDgCmEntityColor(0,255,0) );
  difColor.setMethod( OdGiMaterialColor::kOverride );
  difColor.setFactor( 1.0 );

  OdGiMaterialColor specColor;

  OdDgMaterialTableRecordPtr pMatSpec = OdDgMaterialTableRecord::createObject();

  specColor.setColor( OdDgCmEntityColor(0,0,255) );
  specColor.setMethod( OdGiMaterialColor::kOverride );
  specColor.setFactor( 3.0 );

  pMatSpec->setName(OD_T("Specular1"));
  pMatSpec->setDiffuse( difColor, difMap);
  pMatSpec->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatSpec );

  pMatSpec = OdDgMaterialTableRecord::createObject();

  specColor.setColor( OdDgCmEntityColor(255,0,0) );
  specColor.setMethod( OdGiMaterialColor::kOverride );
  specColor.setFactor( 2.5 );

  pMatSpec->setName(OD_T("Specular2"));
  pMatSpec->setDiffuse( difColor, difMap);
  pMatSpec->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatSpec );

  pMatSpec = OdDgMaterialTableRecord::createObject();

  specColor.setColor( OdDgCmEntityColor(255,0,255) );
  specColor.setMethod( OdGiMaterialColor::kOverride );
  specColor.setFactor( 2.9 );

  pMatSpec->setName(OD_T("Specular3"));
  pMatSpec->setDiffuse( difColor, difMap);
  pMatSpec->setSpecular( specColor, OdDgMaterialTableRecord::kPlastic, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatSpec );

  pMatSpec = OdDgMaterialTableRecord::createObject();

  specColor.setColor( OdDgCmEntityColor(255,128,0) );
  specColor.setMethod( OdGiMaterialColor::kOverride );
  specColor.setFactor( 3.0 );

  pMatSpec->setName(OD_T("Specular4"));
  pMatSpec->setDiffuse( difColor, difMap);
  pMatSpec->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatSpec );
}

void OdExDgnMatFiller::addFinishMaterials()
{
  OdDgDatabase* pDb = m_pModel3d->database();

  OdDgMaterialTablePtr pMatTable = pDb->getMaterialTable( OdDg::kForWrite );

  OdGiMaterialColor difColor;
  OdGiMaterialColor specColor;
  OdDgMaterialMap   difMap;

  difColor.setColor( OdDgCmEntityColor(0,0,255) );
  difColor.setMethod( OdGiMaterialColor::kOverride );
  difColor.setFactor( 1.0 );

  specColor.setColor( OdDgCmEntityColor(255,255,255) );
  specColor.setMethod( OdGiMaterialColor::kOverride );
  specColor.setFactor( 1.0 );

  OdDgMaterialTableRecordPtr pMatSpec = OdDgMaterialTableRecord::createObject();

  pMatSpec->setName(OD_T("Finish1"));
  pMatSpec->setDiffuse( difColor, difMap);
  pMatSpec->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, convertMSFinishToGlossFactor(1), OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatSpec );

  pMatSpec = OdDgMaterialTableRecord::createObject();

  pMatSpec->setName(OD_T("Finish2"));
  pMatSpec->setDiffuse( difColor, difMap);
  pMatSpec->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, convertMSFinishToGlossFactor(10), OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatSpec );

  pMatSpec = OdDgMaterialTableRecord::createObject();

  pMatSpec->setName(OD_T("Finish3"));
  pMatSpec->setDiffuse( difColor, difMap);
  pMatSpec->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, convertMSFinishToGlossFactor(50), OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatSpec );

  pMatSpec = OdDgMaterialTableRecord::createObject();

  pMatSpec->setName(OD_T("Finish4"));
  pMatSpec->setDiffuse( difColor, difMap);
  pMatSpec->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, convertMSFinishToGlossFactor(100), OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatSpec );
}

void OdExDgnMatFiller::addTransparencyMaterials()
{
  OdDgDatabase* pDb = m_pModel3d->database();

  OdDgMaterialTablePtr pMatTable = pDb->getMaterialTable( OdDg::kForWrite );

  OdGiMaterialColor difColor;
  OdGiMaterialColor specColor;
  OdDgMaterialMap   difMap;

  difColor.setColor( OdDgCmEntityColor(255,0,0) );
  difColor.setMethod( OdGiMaterialColor::kOverride );
  difColor.setFactor( 1.0 );

  specColor.setColor( OdDgCmEntityColor(255,255,255) );
  specColor.setMethod( OdGiMaterialColor::kOverride );
  specColor.setFactor( 0.1 );

  OdDgMaterialTableRecordPtr pMatTrans = OdDgMaterialTableRecord::createObject();

  pMatTrans->setName(OD_T("Transparency1"));
  pMatTrans->setDiffuse( difColor, difMap);
  pMatTrans->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );
  pMatTrans->setOpacity( 0.1 );

  pMatTable->add( pMatTrans );

  pMatTrans = OdDgMaterialTableRecord::createObject();

  pMatTrans->setName(OD_T("Transparency2"));
  pMatTrans->setDiffuse( difColor, difMap);
  pMatTrans->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );
  pMatTrans->setOpacity( 0.3 );

  pMatTable->add( pMatTrans );

  pMatTrans = OdDgMaterialTableRecord::createObject();

  pMatTrans->setName(OD_T("Transparency3"));
  pMatTrans->setDiffuse( difColor, difMap);
  pMatTrans->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );
  pMatTrans->setOpacity( 0.7 );

  pMatTable->add( pMatTrans );

  pMatTrans = OdDgMaterialTableRecord::createObject();

  pMatTrans->setName(OD_T("Transparency4"));
  pMatTrans->setDiffuse( difColor, difMap);
  pMatTrans->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );
  pMatTrans->setOpacity( 0.9 );

  pMatTable->add( pMatTrans );
}

void OdExDgnMatFiller::addTextureMaterials()
{
  OdDgDatabase* pDb = m_pModel3d->database();

  OdDgMaterialTablePtr pMatTable = pDb->getMaterialTable( OdDg::kForWrite );

  OdGiMaterialColor difColor;
  OdGiMaterialColor specColor;
  OdDgMaterialMap   difMap;

  difColor.setColor( OdDgCmEntityColor(255,255,255) );
  difColor.setMethod( OdGiMaterialColor::kOverride );
  difColor.setFactor( 1.0 );

  OdGiMaterialMap matMap;

  matMap.setSource( OdGiMaterialMap::kFile );
  matMap.setSourceFileName( OD_T("Texture.jpg") );
  OdGiMapper& matMaper = matMap.mapper();
  matMaper.setTransform( OdGeMatrix3d::scaling(5) );

  difMap.setTextureScaleMode( OdDgMaterialMap::kMasterUnitMode );
  difMap.setMappingType( OdDgMaterialMap::kParametric );
  difMap.setGiMaterialMap( matMap );

  specColor.setColor( OdDgCmEntityColor(255,255,255) );
  specColor.setMethod( OdGiMaterialColor::kOverride );
  specColor.setFactor( 0.1 );

  OdDgMaterialTableRecordPtr pMatTex = OdDgMaterialTableRecord::createObject();

  pMatTex->setName(OD_T("Texture1"));
  pMatTex->setDiffuse( difColor, difMap);
  pMatTex->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatTex );

  pMatTex = OdDgMaterialTableRecord::createObject();

  difMap.setMappingType( OdDgMaterialMap::kCylindrical );

  pMatTex->setName(OD_T("Texture2"));
  pMatTex->setDiffuse( difColor, difMap);
  pMatTex->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatTex );

  pMatTex = OdDgMaterialTableRecord::createObject();

  difMap.setMappingType( OdDgMaterialMap::kCubic );

  pMatTex->setName(OD_T("Texture3"));
  pMatTex->setDiffuse( difColor, difMap);
  pMatTex->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatTex );

  pMatTex = OdDgMaterialTableRecord::createObject();

  difMap.setMappingType( OdDgMaterialMap::kSpherical );

  pMatTex->setName(OD_T("Texture4"));
  pMatTex->setDiffuse( difColor, difMap);
  pMatTex->setSpecular( specColor, OdDgMaterialTableRecord::kCustom, 0.7, OdDgMaterialTableRecord::kGlossFactorCustom );

  pMatTable->add( pMatTex );
}

//------------------------------------------------------------------------------

double OdExDgnMatFiller::convertMSFinishToGlossFactor( double dFinish )
{
  double dStoredFinish = 0.4*exp(dFinish/27.43);

  double dGlossFactor  = 1 - (log(dStoredFinish) / log( 1.037 ) + 25)/200.0;

  return dGlossFactor;
}

//------------------------------------------------------------------------------

void OdExDgnMatFiller::applyMaterial( OdDgElementId idElement, OdString strMaterial )
{
  if( idElement.isNull() )
  {
    ODA_FAIL_ONCE();
    return;
  }

  OdDgElementPtr pElement = idElement.openObject( OdDg::kForWrite );

  OdDgDatabase* pDb = m_pModel3d->database();

  OdDgMaterialTablePtr pMatTable = pDb->getMaterialTable( OdDg::kForWrite );

  OdDgElementId idMaterial = pMatTable->getAt( strMaterial );

  if( idMaterial.isNull() )
  {
    ODA_FAIL_ONCE();
    return;
  }

  if( pElement->isKindOf( OdDgGraphicsElement::desc()) )
  {
    OdDgGraphicsElementPtr pGrElement = pElement;

    if( pGrElement->isMaterialPropertyAvailble() )
      pGrElement->setMaterial(idMaterial);
  }
}

//------------------------------------------------------------------------------

OdDgElementId OdExDgnMatFiller::addPlane( OdGePoint3d ptCenter, double dSize )
{
  OdDgElementId idRet;

  OdDgShape3dPtr pPlane = OdDgShape3d::createObject();

  pPlane->addVertex( ptCenter - OdGeVector3d::kXAxis * dSize/2.0 - OdGeVector3d::kYAxis * dSize/2.0 );
  pPlane->addVertex( ptCenter - OdGeVector3d::kXAxis * dSize/2.0 + OdGeVector3d::kYAxis * dSize/2.0 );
  pPlane->addVertex( ptCenter + OdGeVector3d::kXAxis * dSize/2.0 + OdGeVector3d::kYAxis * dSize/2.0 );
  pPlane->addVertex( ptCenter + OdGeVector3d::kXAxis * dSize/2.0 - OdGeVector3d::kYAxis * dSize/2.0 );
  pPlane->addVertex( ptCenter - OdGeVector3d::kXAxis * dSize/2.0 - OdGeVector3d::kYAxis * dSize/2.0 );

  idRet = m_pModel3d->addElement( pPlane );

  return idRet;
}

OdDgElementId OdExDgnMatFiller::addCylinder( OdGePoint3d ptCenter, double dSize )
{
  OdDgElementId idRet;

  OdDgConePtr pCone = OdDgCone::createObject();

  pCone->setCenter1( ptCenter - OdGeVector3d::kYAxis * dSize/2.0);
  pCone->setCenter2( ptCenter + OdGeVector3d::kYAxis * dSize/2.0);
  pCone->setRadius1( dSize/2.0 );
  pCone->setRadius2( dSize/2.0 );

  OdGeMatrix3d matRot;
  matRot.setToRotation(OdaPI2, OdGeVector3d::kXAxis);
  OdGeQuaternion quatRot;
  quatRot.set(matRot);
  pCone->setRotation( quatRot );

  idRet = m_pModel3d->addElement( pCone );

  return idRet;
}

OdDgElementId OdExDgnMatFiller::addBox( OdGePoint3d ptCenter, double dSize )
{
  OdDgElementId idRet;

  OdDgMeshFaceLoopsPtr pBox = OdDgMeshFaceLoops::createObject();

  idRet = m_pModel3d->addElement( pBox );

  pBox->setPointsNumber( 8 );

  pBox->setPoint( 0, ptCenter - OdGeVector3d::kYAxis * dSize/2.0 - OdGeVector3d::kXAxis * dSize / 2.0 );
  pBox->setPoint( 1, ptCenter - OdGeVector3d::kYAxis * dSize/2.0 - OdGeVector3d::kZAxis * dSize / 2.0 );
  pBox->setPoint( 2, ptCenter - OdGeVector3d::kYAxis * dSize/2.0 + OdGeVector3d::kXAxis * dSize / 2.0 );
  pBox->setPoint( 3, ptCenter - OdGeVector3d::kYAxis * dSize/2.0 + OdGeVector3d::kZAxis * dSize / 2.0 );
  pBox->setPoint( 4, ptCenter + OdGeVector3d::kYAxis * dSize/2.0 - OdGeVector3d::kXAxis * dSize / 2.0 );
  pBox->setPoint( 5, ptCenter + OdGeVector3d::kYAxis * dSize/2.0 - OdGeVector3d::kZAxis * dSize / 2.0 );
  pBox->setPoint( 6, ptCenter + OdGeVector3d::kYAxis * dSize/2.0 + OdGeVector3d::kXAxis * dSize / 2.0 );
  pBox->setPoint( 7, ptCenter + OdGeVector3d::kYAxis * dSize/2.0 + OdGeVector3d::kZAxis * dSize / 2.0 );

  OdDgMeshFaceLoops::VerticesArray vertices;
  vertices.resize( 4 );

  pBox->setFacesNumber( 6 );
  vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 2; vertices[ 3 ].m_pointIndex = 3; 
  pBox->setFace( 0, vertices );
  vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 5; vertices[ 3 ].m_pointIndex = 4; 
  pBox->setFace( 1, vertices );
  vertices[ 0 ].m_pointIndex = 1; vertices[ 1 ].m_pointIndex = 2; vertices[ 2 ].m_pointIndex = 6; vertices[ 3 ].m_pointIndex = 5; 
  pBox->setFace( 2, vertices );
  vertices[ 0 ].m_pointIndex = 2; vertices[ 1 ].m_pointIndex = 3; vertices[ 2 ].m_pointIndex = 7; vertices[ 3 ].m_pointIndex = 6; 
  pBox->setFace( 3, vertices );
  vertices[ 0 ].m_pointIndex = 3; vertices[ 1 ].m_pointIndex = 0; vertices[ 2 ].m_pointIndex = 4; vertices[ 3 ].m_pointIndex = 7; 
  pBox->setFace( 4, vertices );
  vertices[ 0 ].m_pointIndex = 4; vertices[ 1 ].m_pointIndex = 5; vertices[ 2 ].m_pointIndex = 6; vertices[ 3 ].m_pointIndex = 7; 
  pBox->setFace( 5, vertices );
  
  return idRet;
}

OdDgElementId OdExDgnMatFiller::addSphere( OdGePoint3d ptCenter, double dSize )
{
  OdDgElementId idRet;

  OdDgSurfacePtr pSphere = OdDgSurface::createObject();

  idRet = m_pModel3d->addElement( pSphere );

  pSphere->setNonPlanarFlag(true);
  pSphere->setNotSnappableFlag(true);
  pSphere->setType( OdDgSurface::kSurfaceRevolution );

  OdDg3dObjectHelper helper( pSphere );

  OdGeMatrix3d matRot;
  OdGeMatrix3d matRot1;
  OdGeQuaternion quatRot;

  OdDgArc3dPtr pBoundary1 = OdDgArc3d::createObject();

  pBoundary1->setOrigin( ptCenter );
  pBoundary1->setPrimaryAxis( dSize/2.0 );
  pBoundary1->setSecondaryAxis( dSize/2.0 );
  pBoundary1->setStartAngle( 0 );
  pBoundary1->setSweepAngle( OdaPI );

  helper.addToBoundary( pBoundary1 );

  OdDgArc3dPtr pBoundary2 = OdDgArc3d::createObject();

  pBoundary2->setOrigin( ptCenter );
  pBoundary2->setPrimaryAxis( dSize/2.0 );
  pBoundary2->setSecondaryAxis( dSize/2.0 );
  pBoundary2->setStartAngle( 0 );
  pBoundary2->setSweepAngle( OdaPI );

  helper.addToBoundary( pBoundary2 );

  matRot.setToRotation(OdaPI2, OdGeVector3d::kYAxis);
  matRot1.setToRotation(OdaPI2, OdGeVector3d::kZAxis);
  matRot = matRot*matRot1;
  quatRot.set(matRot);

  OdDgArc3dPtr pRule = OdDgArc3d::createObject();

  pRule->setOrigin( ptCenter );
  pRule->setPrimaryAxis( dSize/2.0 );
  pRule->setSecondaryAxis( dSize/2.0 );
  pRule->setStartAngle( 0 );
  pRule->setSweepAngle( Oda2PI );
  pRule->setRotation( quatRot );

  helper.addToRule( pRule );

  return idRet;
}
