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

#ifndef _EXDIMASSOCPE_INCLUDED
#define _EXDIMASSOCPE_INCLUDED

#include "DbEvalWatchers.h"
#include "DbViewport.h"
class OdDbDimAssoc;
class OdExDimAssoc;
class OdDbRotatedDimension;
class OdDbAlignedDimension;
class OdDbRadialDimension;
class OdDbDiametricDimension;
class OdDbArcDimension;
class OdDb2LineAngularDimension;
class OdDb3PointAngularDimension;
class OdDbRadialDimensionLarge;
class OdDbOrdinateDimension;
class OdDbLeader;
class OdDbMLeader;
class OdGePoint3d;

class ExDimAssocWatcherPE : public OdDbDimAssocWatcherPE
{
public:
  virtual void modified(OdDbObject* pDimAssoc, const OdDbObject* pAssocObj);
  virtual void openedForModify(OdDbObject* pDimAssoc, const OdDbObject* pAssocObj);
  
  virtual void copied(OdDbObject *pDimAssoc, const OdDbObject *pAssocObj, const OdDbObject *pNewObj);
  virtual void erased(OdDbObject *pDimAssoc, const OdDbObject *pAssocObj, bool erasing = true);
  virtual void CheckForAssociativity(OdDbObject *pDimAssoc);

  OdResult UpdateDimension( OdDbEntityPtr &pEnt, OdDbDimAssoc* pThis, OdGePoint3d * ptVals );
  OdResult updateDimension(OdDbDimAssoc* pThis, OdDbRotatedDimension* pRotDim, OdGePoint3d* pNewPts, bool bUpdate = true);
  OdResult updateDimension(OdDbDimAssoc* pThis, OdDbAlignedDimension* pAlignDim, OdGePoint3d* pNewPts, bool bUpdate = true);
  OdResult updateDimension(OdDbDimAssoc* pThis, OdDbRadialDimension* pRadDim, OdGePoint3d* pNewPts, bool bUpdate = true);
  OdResult updateDimension(OdDbDimAssoc* pThis, OdDbDiametricDimension* pDiamDim, OdGePoint3d* pNewPts, bool bUpdate = true);
  OdResult updateDimension(OdDbDimAssoc* pThis, OdDbArcDimension* pArcDim, OdGePoint3d* pNewPts, bool bUpdate = true);
  OdResult updateDimension(OdDbDimAssoc* pThis, OdDb2LineAngularDimension* pAngDim, OdGePoint3d* pNewPts, bool bUpdate = true);
  OdResult updateDimension(OdDbDimAssoc* pThis, OdDb3PointAngularDimension* pAngDim, OdGePoint3d* pNewPts, bool bUpdate = true);
  OdResult updateDimension(OdDbDimAssoc* pThis, OdDbRadialDimensionLarge* pRadDim, OdGePoint3d* pNewPts, bool bUpdate = true);
  OdResult updateDimension(OdDbDimAssoc* pThis, OdDbOrdinateDimension* pOrdDim, OdGePoint3d* pNewPts, bool bUpdate = true);
  OdResult updateDimension(OdDbDimAssoc* pThis, OdDbLeader* pLeader, OdGePoint3d* pNewPts, bool bUpdate = true);
  OdResult updateDimension(OdDbDimAssoc* pThis, OdDbMLeader* pMLeader, OdGePoint3d* pNewPts, bool bUpdate = true);

  friend class OdExDimAssoc;
  
private:
  OdResult GetEntityRadius( const OdDbFullSubentPath &m_MainEntity, double &radius );
  OdResult CheckPointRefAssociativity( OdExDimAssoc* pThisEx, OdDbObject* pModObj );
  OdDbViewportPtr GetViewPort(OdExDimAssoc* pThisEx);
};

#endif
