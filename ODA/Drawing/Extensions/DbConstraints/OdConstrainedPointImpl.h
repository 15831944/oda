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

#ifndef OdConstrainedPointImpl_INCLUDED
#define OdConstrainedPointImpl_INCLUDED

#include "OdConstrainedGeometryImpl.h"
#include "Ge/GePoint3d.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of the constrained point node in the owning constraint group. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdConstrainedPointImpl: public OdConstrainedGeometryImpl
{ 
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdConstrainedPointImpl();
 
  /** \details
    Initializes this constrained point object.
  */
  virtual OdResult init(const OdDbObjectId depId);

  /** \details
    Returns the three-dimesional position relative to the plane of the owning group object.
  */
  virtual OdGePoint3d point() const;

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \details
    Compares two constrained points on equality.
  */
  virtual bool operator==(const OdConstrainedGeometryImpl& geom) const;

  /** \details
    Compares two constrained points on unequality.
  */
  virtual bool operator!=(const OdConstrainedGeometryImpl& geom) const;

  /** \details
    Sets data from a given geometry dependency using transformation matrix.
  */
  virtual OdResult setDataFromGeomDependency(const OdDbObjectId geomDependencyId,
                                             const OdGeMatrix3d& matrFromWrd);

  /** \details
    Updates the database object.
  */
  virtual OdResult updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd);

  /** \details
    Sets the three-dimesional position of the owning group object.
  */
  virtual OdResult setPoint(const OdGePoint3d& val);

  /** \details
    Returns the three-dimesional original position of the owning group object.
  */
  virtual const OdGePoint3d getOriginalPoint() const;

  /** \details
    Returns an array of full subent paths.
  */
  virtual OdResult getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const;
protected:
  /*!DOM*/
  OdGePoint3d m_pointOrig;
  /*!DOM*/
  OdGePoint3d m_point;
}; 

#include "TD_PackPop.h"
#endif
