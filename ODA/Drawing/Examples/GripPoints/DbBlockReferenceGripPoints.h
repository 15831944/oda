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

#ifndef _ODDBBLOCKREFERENCEGRIPPOINTS_H_INCLUDED
#define _ODDBBLOCKREFERENCEGRIPPOINTS_H_INCLUDED

class OdDbBlockReferenceGripPointsPE : public OdDbEntityGripPointsPE
{
public:
  virtual OdResult getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const;
  virtual OdResult moveStretchPointsAt( OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset );
  virtual OdResult getOsnapPoints(
    const OdDbEntity* ent, 
    OdDb::OsnapMode osnapMode, 
    OdGsMarker gsSelectionMark, 
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint,
    const OdGeMatrix3d& viewXform,
    OdGePoint3dArray& snapPoints ) const;
  virtual OdResult getGripPoints(
    const OdDbEntity* pEntity,
    OdDbGripDataPtrArray& grips,
    const double curViewUnitSize,
    const int gripSize,
    const OdGeVector3d& curViewDir,
    const int bitFlags) const;
  virtual OdResult moveGripPointsAt(
    OdDbEntity* pEntity,
    const OdDbVoidPtrArray& grips,
    const OdGeVector3d& offset,
    int bitFlags);
};

class OdDbBlockGripAppData : public OdRxObject
{
public:
  OdDbBlockGripAppData() : m_nClipGripGripIndex(-1), m_nAttributeIndex(-1), m_bClipInvertGrip(false){}
  ODRX_DECLARE_MEMBERS(OdDbBlockGripAppData);
  int m_nAttributeIndex;
  int m_nClipGripGripIndex;
  bool m_bClipInvertGrip;
  OdGeVector3d m_vClipInvertOrientation;
};

#endif
