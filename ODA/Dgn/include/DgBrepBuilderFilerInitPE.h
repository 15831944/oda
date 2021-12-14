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
#ifndef __DG_BREPBUILDERINITIALDATA_H__
#define __DG_BREPBUILDERINITIALDATA_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgBSplineSurface.h"
#include "DgCone.h"
#include "BrepBuilderInitialData.h"

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgBrepBuilderInitialDataPE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdDgBrepBuilderInitialDataPE);
  long        m_references;     //used by each object separately
public:

  /** \details
    Allows to get initial data to construct brep by brep builder.
  */

  virtual bool getBrepBuilderInitialData(const OdDgElement* obj, BrepBuilderInitialData& brepData, double& dScale) const;

};

typedef OdSmartPtr<OdDgBrepBuilderInitialDataPE> OdDgBrepBuilderInitialDataPEPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgBSplineSurfaceBrepBuilderInitialDataPE : public OdDgBrepBuilderInitialDataPE
{
  ODRX_DECLARE_MEMBERS(OdDgBSplineSurfaceBrepBuilderInitialDataPE);

public:
  virtual bool getBrepBuilderInitialData(const OdDgElement* obj, BrepBuilderInitialData& brepData, double& dScale) const;
};

typedef OdSmartPtr<OdDgBSplineSurfaceBrepBuilderInitialDataPE> OdDgBSplineSurfaceBrepBuilderInitialDataPEPtr;

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgConeBrepBuilderInitialDataPE : public OdDgBrepBuilderInitialDataPE
{
   ODRX_DECLARE_MEMBERS(OdDgConeBrepBuilderInitialDataPE);

public:
  virtual bool getBrepBuilderInitialData(const OdDgElement* obj, BrepBuilderInitialData& brepData, double& dScale ) const;
};

typedef OdSmartPtr<OdDgConeBrepBuilderInitialDataPE> OdDgConeBrepBuilderInitialDataPEPtr;

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
struct OdDgSolidGeLoop
{
  bool m_bLoopIsHole;
  OdArray<OdGeCurve3dPtr> m_arrCurves;
};

//-----------------------------------------------------------------------------------------

typedef OdArray<OdDgSolidGeLoop> OdDgSolidGeBoundary;

//-----------------------------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
struct OdDgRevolveSolidData
{
  OdDgSolidGeBoundary             m_boundary;
  OdGeVector3d                    m_vrBoundaryNormal;
  OdGePoint3d                     m_ptRevolveOrigin;
  OdGeVector3d                    m_vrRevolveAxis;
  double                          m_dRevolveAngle;
};

//-----------------------------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDg3dSolidBrepBuilderInitialDataPE : public OdDgBrepBuilderInitialDataPE
{
   ODRX_DECLARE_MEMBERS(OdDg3dSolidBrepBuilderInitialDataPE);
public:

  enum OdDg3dSolidBaseType
  {
    kExtrudeBased = 0,
    kRevolveBased = 1,
    kExtrudeAndRevolveBased = 2
  };
public:
  virtual bool getBrepBuilderInitialData( const OdDgElement* obj, BrepBuilderInitialData& brepData, double& dScale ) const;

  virtual OdDg3dSolidBaseType getDgnSolidBaseType( const OdDgElement* obj ) const;
  virtual bool getRevolveSolidData( const OdDgElement* obj, OdArray<OdDgRevolveSolidData>& arrData ) const;
  static  bool createRegionData( const OdDgSolidGeBoundary& geBoundary, const OdGeVector3d& vrBoundaryNormal, BrepBuilderInitialData& brepData );
  static  bool getRegionData( const OdDgElement* obj, BrepBuilderInitialData& brepData );
};

typedef OdSmartPtr<OdDg3dSolidBrepBuilderInitialDataPE> OdDg3dSolidBrepBuilderInitialDataPEPtr;

#include "TD_PackPop.h"

#endif // __DG_BREPBUILDERINITIALDATA_H__
