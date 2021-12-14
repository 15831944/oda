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

#ifndef _ODDBTEXTGRIPPOINTS_INCLUDED
#define _ODDBTEXTGRIPPOINTS_INCLUDED

#include "DbText.h"
#include "DbGripPoints.h"

/************************************************************************/
/* This class is an implementation of the OdDbGripPointsPE class for    */
/* OdDbText entities.                                                 */
/************************************************************************/
class OdDbTextGripPointsPE : public OdDbGripPointsPE
{
  bool   is_Justify_Left   (const OdDbText* pText) const;
  bool   is_Justify_Aligned(const OdDbText* pText) const;
  bool   is_Justify_Fit    (const OdDbText* pText) const;

public:
  virtual OdResult getGripPoints( const OdDbEntity* pEntity, OdGePoint3dArray& gripPoints ) const;
  virtual OdResult moveGripPointsAt( OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset );
  virtual OdResult getStretchPoints( const OdDbEntity* pEntity, OdGePoint3dArray& stretchPoints ) const;
  virtual OdResult moveStretchPointsAt( OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset );
  virtual OdResult getOsnapPoints( 
    const OdDbEntity* pEntity, 
    OdDb::OsnapMode osnapMode, 
    OdGsMarker gsSelectionMark, 
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint, 
    const OdGeMatrix3d& xWorldToEye, 
    OdGePoint3dArray& snapPoints) const;
};

#endif // _ODDBTEXTGRIPPOINTS_INCLUDED
