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

// ExLine.cpp: implementation of the ExLine function.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "DbCurve.h"
#include "DbAssocManager.h"
#include "DbAssocAction.h"
#include "DbAssocArrayActionBody.h"
#include "DbAssocArrayRectangularParameters.h"
#include "DbAssocArrayPathParameters.h"
#include "DbAssocArrayPolarParameters.h"
#include "DbHostAppServices.h"


static OdDbObjectId _Rect_array(OdDbUserIO* pIO, OdDbSelectionSet* pSSet)
{
  OdDbObjectId arrayId, bodyId;
  OdDbVertexRefPtr pBasePointRef = OdDbVertexRef::createObject( OdGePoint3d( 0, 0, 0 ) );

  OdDbAssocArrayRectangularParametersPtr pRectParam = OdDbAssocArrayRectangularParameters::createObject(  );
  pRectParam->setRowCount(3);
  pRectParam->setColumnCount(4);

  // Detect extents of source geometry
  OdGeExtents3d extAll;
  OdDbObjectIdArray idsArray = pSSet->objectIdArray();
  for (OdDbObjectIdArray::const_iterator pId = idsArray.begin(); pId < idsArray.end(); ++pId)
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(pId->openObject());
    if (!pEnt.isNull())
    {
      OdGeExtents3d extEnt;
      if (pEnt->getGeomExtents(extEnt) == eOk)
        extAll.addExt(extEnt);
    }
  }
  // Set spacing
  double dSizeX = extAll.maxPoint().x - extAll.minPoint().x;
  double dSizeY = extAll.maxPoint().y - extAll.minPoint().y;
  if (OdPositive(dSizeX))
    pRectParam->setColumnSpacing(dSizeX * 1.5);
  if (OdPositive(dSizeY))
    pRectParam->setRowSpacing(dSizeY * 1.5);

  OdDbAssocArrayActionBody::createInstance(idsArray, *pBasePointRef.get(), pRectParam, arrayId, bodyId );

  odrxAssocArrayAdjustLocation(bodyId, pRectParam); // to get result like ACAD command    ARRAY RECT

  return arrayId;
}

static OdDbObjectId _Polar_array(OdDbUserIO* pIO, OdDbSelectionSet* pSSet)
{
  OdGePoint3d ptCenter = pIO->getPoint(L"Specify center point of array:");

  OdDbObjectId arrayId, bodyId;
  OdDbVertexRefPtr pBasePointRef = OdDbVertexRef::createObject( OdGePoint3d( 0, 0, 0 ) );
  OdDbAssocArrayPolarParametersPtr pPolarParam = OdDbAssocArrayPolarParameters::createObject();

  OdDbAssocArrayActionBody::createInstance( pSSet->objectIdArray(), *pBasePointRef.get(), pPolarParam, arrayId, bodyId );

  OdDbGeomRefPtr pOriginRef = OdDbVertexRef::createObject(ptCenter);
  OdDbObjectId paramId;
  pPolarParam->setGeomParam(L"Origin", pOriginRef, paramId); // is using for adjust

  odrxAssocArrayAdjustLocation(bodyId, pPolarParam); // to get result like ACAD command    ARRAY RECT

  pPolarParam->setItemCount(6); // rowCount
  pPolarParam->setAngleBetweenItems(60.0); // degree
  pPolarParam->setFillAngle(60.0 * 6); // degree
  //
  //pPolarParam->setRowSpacing(200); pPolarParam->setLevelSpacing(200); pPolarParam->setRowElevation(0.0);
  //
  //pPolarParam->setRotateItems(true); // translated into setValueParam(L"AlignItems", ...)
  //pPolarParam->setDirection(OdDbAssocArrayPolarParameters::kClockwise);

  return arrayId;
}

static OdDbObjectId _Path_array(OdDbUserIO* pIO, OdDbSelectionSet* pSSet)
{
  class SourceEntFilter : public OdStaticRxObject<OdEdSSetTracker>
  {
    OdDbSelectionSet* m_pSSet;

  public:
    SourceEntFilter( OdDbSelectionSet* pSSet )
      : m_pSSet(pSSet)
    {}
    virtual int addDrawables(OdGsView*){return 0;}
    virtual void removeDrawables(OdGsView*){}

    virtual bool append(const OdDbObjectId& entId, const OdDbSelectionMethod* )
    {   
      bool res = entId.safeOpenObject()->isKindOf( OdDbCurve::desc() );
      res &= ! m_pSSet->objectIdArray().contains( entId );
      return res;
    }
    virtual bool remove(const OdDbObjectId& , const OdDbSelectionMethod* )
    {
      return true;
    }
  } srcEntFilter(pSSet);

  OdDbSelectionSetPtr pSPath;
  do
  {
    pSPath = pIO->select(L"Select path curve:", 
      OdEd::kSelSinglePass, 0, OdString::kEmpty, &srcEntFilter );
  } 
  while ( !pSPath->numEntities() );

  OdDbCompoundObjectId compoundObjectId;
  OdDbObjectId curveID = pSPath->objectIdArray().first();
  compoundObjectId.set( curveID );
  OdDbCurvePtr crv = OdDbCurve::cast( curveID.openObject( OdDb::kForRead ) );
  OdGeCurve3d *pCurve = NULL;
  crv->getOdGeCurve( pCurve );
  OdDbEdgeRefPtr pEdgeRef = OdDbEdgeRef::createObject( compoundObjectId, pCurve );
  delete pCurve;
  
  OdDbObjectId arrayId, bodyId;
  OdDbVertexRefPtr pBasePointRef = OdDbVertexRef::createObject( OdGePoint3d( 0, 0, 0 ) );

  OdDbAssocArrayPathParametersPtr pPathParam = OdDbAssocArrayPathParameters::createObject(  );
  pPathParam->setPath( *pEdgeRef.get() );
 
  OdDbAssocArrayActionBody::createInstance( pSSet->objectIdArray(), *pBasePointRef.get(), pPathParam, arrayId, bodyId );
  return arrayId;
}

void _Array_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdDbHostAppServices* pSvs = pDb->appServices();

  OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
  OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

  OdGeVector3d vNormal(ExCommandsUtils::ucsNormal(pDb));

  OdDbSelectionSetPtr pSSet = pIO->select(L"Select objects:", OdEd::kSelAllowEmpty);
  if ( !pSSet->numEntities() )
    return;

  pDb->startTransaction();
  bool bOk = true;
  OdDbObjectId idArray;
  try {
    OdRxModulePtr pConstrMod = ::odrxDynamicLinker()->loadModule(L"DbConstraints");
    OdInt16 nDefArrayType = pSvs->getARRAYTYPE();
    OdInt16 nArrayType =  pIO->getKeyword(L"Enter array type[Rectangular/PAth/POlar]", L"Rectangular PAth POlar", nDefArrayType ); 
    switch (nArrayType)
    {
    case 0:
      idArray = _Rect_array( pIO, pSSet );
      break;

    case 1:
      idArray = _Path_array( pIO, pSSet );
      break;

    case 2:
      idArray = _Polar_array( pIO, pSSet );
      break;
    }
    if (nArrayType != nDefArrayType)
    {
      pSvs->setARRAYTYPE(nArrayType);
    }
    if (!pSvs->getARRAYASSOCIATIVITY())
    {
      OdDbEntityPtr pArray = idArray.safeOpenObject();
      OdDbObjectIdArray newIds;
      OdDbAssocArrayActionBody::explode(pArray, newIds);
    }
  }
  catch (...)
  {
    bOk = false;
  }

  if (bOk)
  {
    pDb->endTransaction();
    OdDbAssocManager::evaluateTopLevelNetwork( pDb );
    pIO->putString(L"Ready");
  }
  else
  {
    pDb->abortTransaction();
    pIO->putString(L"Failed");
  }
}
