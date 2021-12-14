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



#ifndef _INC_DDBRENTITY_3F819553013C_INCLUDED
#define _INC_DDBRENTITY_3F819553013C_INCLUDED


#include "OdaCommon.h"
#include "Br/BrExport.h"
#include "Br/BrEnums.h"
#include "Br/BrMassProps.h"

#include "TD_PackPush.h"
#include "SharedPtr.h"
#include "DbBaseSubentId.h"
#include "../Ge/Ge.h"

class OdBrBrep;
class OdBrHit;
class OdGePoint3d;
class OdGeLinearEnt3d;
class OdGeBoundBlock3d;


/** \details
  This class is the interface base class for BREP topology objects.

  Corresponding C++ library: TD_Br

  <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrEntity
{
public:

  /** \details
    Returns true if and only if there is no ACIS model associated with this
    entity object.

    \returns
    True if there is no ACIS model associated with this entity object; false
    otherwise.
  */
  bool isNull() const;

  /** \details
    Returns true if and only if the specified entity object is equivalent to
    this entity object.

    \param pOtherEntity [in] Pointer to any OdBrEntity object.

    \returns
    True if the specified entity object is equivalent to this entity object;
    false otherwise.

    \remarks
    Two OdBrEntity objects are equivalent if and only if they are non-NULL, and
    have the same subentity path.
  */
  bool isEqualTo(
    const OdBrEntity* pOtherEntity) const;

  /** \details
    Returns the owner of this entity object.
    
    \param brep [out] Owner of this entity object.

    \returns
    True if successful; false otherwise.
  */
  bool getBrep(OdBrBrep& brep) const;

  /** \details
    Validates the element.

    \returns
    True if successful; false otherwise.
  */
  bool checkEntity() const;

  /** \details
    Returns the FullSubentPath of this entity object.

    \param subPath [out] Full subentity path of this entity object.

    \returns
    odbrOK if the operation is successful or the appropriate error code
    otherwise.

    \sa
    OdDbBody::setSubentPath().
  */
  OdBrErrorStatus getSubentPath(OdDbBaseFullSubentPath& subPath) const;

  /** \details
    Destructor for objects of the OdBrEntity class.
  */
  virtual ~OdBrEntity();

  /** \details
    This method is for internal use only. Use OdDbBody::setSubentPath() instead.

    \param brep    [in] Owner.
    \param subpath [in] Full subentity path.

    \returns
    odbrOK if the operation is successful or the appropriate error code
    otherwise.
  */
  OdBrErrorStatus setSubentPath(OdBrBrep &brep, OdDbBaseFullSubentPath& subpath);

  /** \details
    Gets the point containment and the corresponding container entity.

    \param point           [in] Point to get containment of.
    \param containment    [out] Containment type. Can be one of the following:
                                <table>
                                Value          Description
                                kInside        Point is inside the boundary
                                kOutside       Point is outside the boundary
                                kOnBoundary    Point on the boundary
                                </table>
    \param brEntContainer [out] Container entity. Can be one of the following types:<br>
                                * OdBrBrep
                                * OdBrFace
                                * OdBrEdge
                                * OdBrVertex

    \returns
    odbrOK if the operation is successful or the appropriate error code
    otherwise.

    \remarks
    Currently not implemented. Returns odbrNotImplementedYet.
  */
  OdBrErrorStatus getPointContainment(const OdGePoint3d&      point,
                                      OdGe::PointContainment& containment,
                                      OdBrEntity*&            brEntContainer ) const;

  /** \details
    Gets the number of line containment hits and the array of contexted hit
    objects.

    \param line           [in] Line to get containment of.
    \param iNumHitsWanted [in] Expected number of containment hits.
    \param iNumHitsFound [out] Actual number of containment hits found.
    \param brHit         [out] Array of contexted hit objects.

    \returns
    odbrOK if the operation is successful or the appropriate error code
    otherwise.

    \remarks
    Currently not implemented. Returns odbrNotImplementedYet.
  */
  OdBrErrorStatus getLineContainment( const OdGeLinearEnt3d& line,
                                      const OdUInt32&        iNumHitsWanted,
                                      OdUInt32&              iNumHitsFound,
                                      OdBrHit*&              brHit ) const;


/*
  void* getEntity() const;
  OdBrErrorStatus setEntity(void* pData, OdDbBaseFullSubentPath& subPath = *(OdDbBaseFullSubentPath*)NULL);
*/

  /** \details
    Calculates the entity geometric bounds.

    \param block [out] Model space bounding box as OdGeBoundBlock3d.

    \returns
    * odbrNotImplementedYet &mdash; if the method is not implemented for the current modeler.
    * odbrUninitialisedObject &mdash; if the object is uninitialized.
    * odbrInvalidInput &mdash; if the calculation was unsuccessful.
    * odbrOK &mdash; if the calculation was successful.
  */
  OdBrErrorStatus getBoundBlock(OdGeBoundBlock3d& block) const;

  // Validation

  /** \details
    Sets the validation level.

    \param level [in] Validation level value. Can be one of the following:<br>
                      * odbrFullValidation to set the object as validated.
                      * odbrNoValidation to set the object as not validated.

    \returns
    odbrOK if the operation is successful or the appropriate error code
    otherwise.
  */
  OdBrErrorStatus setValidationLevel(const BrValidationLevel& level);

  /** \details
     Gets the validation level.

     \param level [out] Validation level value. Can be one of the following:<br>
                        * odbrFullValidation if the object is validated.
                        * odbrNoValidation if the object is not validated.

     \returns
     odbrOK if the operation is successful or the appropriate error code
     otherwise.
  */
  OdBrErrorStatus getValidationLevel(BrValidationLevel& level) const;

  //BrEntityFlags

  /** \details
    Returns the flags of the object.

    \returns
    Flags of the object.

    \sa
    BrEntityFlags enumeration.
  */
  OdUInt32 getFlags() const;

  /** \details
    Gets the Gs marker.

    \param marker [out] Gs marker value.

    \returns
    odbrOK if the operation is successful or the appropriate error code
    otherwise.
  */
  OdBrErrorStatus getGsMarker(OdGsMarker& marker) const;
/*
  bool brepChanged() const;

  // Queries & Initialisers
  OdBrErrorStatus set(const BrValidationLevel& path);
  OdBrErrorStatus get(BrValidationLevel& path) const;
  OdBrErrorStatus set(OdBrBrepData* data);
  OdBrErrorStatus get(OdBrBrepData*& data) const;
*/

  // Geometric properties

  /** \details
    Provides the mass properties for the entity.
    The mass properties are returned as an instance of OdBrMassProps structure.
    The density is used for the mass properties scaling and is the analogue of
    the physical value of the density of materials.
    The mass properties are applicable for those entities, that represent
    topology objects having the volume property.

    \param massProps    [out] Mass properties of the entity.
    \param dDensity      [in] Density for calculating mass properties of the entity.
    \param dTolRequired  [in] Tolerance required for the result of the calculation.
    \param dTolAchieved [out] Tolerance achieved in the calculation.

    \returns
    Value of the OdBrErrorStatus type.

    \remarks
    If density is not specified (dUnspecified), the default value is 1.
    If the mass properties are not applicable to the entity, odbrNotApplicable
    is returned.
    If an error occurs, the value of the mass properties argument remains
    unchanged.
  */
  OdBrErrorStatus getMassProps(OdBrMassProps& massProps,
    const double& dDensity     = dUnspecified,
    const double& dTolRequired = dUnspecified,
    double&       dTolAchieved = dUnspecified) const;

  /** \details
    Provides the volume of the entity as a double value.
    The volume property is applicable for most entities, that represent
    volumetric figures.

    \param dVolume      [out] Volume of the entity.
    \param dTolRequired  [in] Tolerance required for the result of the calculation.
    \param dTolAchieved [out] Tolerance achieved in the calculation.

    \returns
    Value of the OdBrErrorStatus type.

    \remarks
    If the volume property is not applicable to the entity, odbrNotApplicable is
    returned.
    If an error occurs, the value of the volume argument remains unchanged.
  */
  OdBrErrorStatus getVolume(double& dVolume,
    const double& dTolRequired = dUnspecified,
    double&       dTolAchieved = dUnspecified) const;

  /** \details
    Provides the surface area of the entity as a double value.
    The surface area property is applicable for most entities, that represent
    volumetric figures.

    \param dArea        [out] Surface area of the entity.
    \param dTolRequired  [in] Tolerance required for the result of the calculation.
    \param dTolAchieved [out] Tolerance achieved in the calculation.

    \returns
    Value of the OdBrErrorStatus type.

    \remarks
    If the surface area property is not applicable to the entity,
    odbrNotApplicable is returned.
    If an error occurs, the value of the surface area argument remains
    unchanged.
  */
  OdBrErrorStatus getSurfaceArea(double& dArea,
    const double& dTolRequired = dUnspecified,
    double&       dTolAchieved = dUnspecified) const;

  /** \details
    Provides the perimeter length of the entity as a double value.
    The perimeter length property is applicable for most entities, that
    represent plane figures and some 3D figures.

    \param dLength      [out] Perimeter length of the entity.
    \param dTolRequired  [in] Tolerance required for the result of the calculation.
    \param dTolAchieved [out] Tolerance achieved in the calculation.

    \returns
    Value of the OdBrErrorStatus type.

    \remarks
    If the perimeter length property is not applicable to the entity,
    odbrNotApplicable is returned.
    If an error occurs, the value of the perimeter length argument remains
    unchanged.
  */
  OdBrErrorStatus getPerimeterLength(double& dLength,
    const double& dTolRequired = dUnspecified,
    double&       dTolAchieved = dUnspecified) const;

  //DOM-IGNORE-BEGIN
  ODBR_TOOLKIT_EXPORT_STATIC static double dUnspecified;

protected:
  void *m_pImp;
  OdSharedPtr<OdDbStubPtrArray> m_pFSubentPath;
  bool m_bIsValidate; //The enum has only 2 values.

  OdBrEntity();
#ifdef OD_ADD_MOVE_CTOR_HAVE_DEF_DEL_FUNC
  //DOM-IGNORE-BEGIN
  OdBrEntity(const OdBrEntity&) = default;
  OdBrEntity& operator=(const OdBrEntity&) = default;
  OdBrEntity(OdBrEntity&&) = default;
  OdBrEntity& operator=(OdBrEntity&&) = default;
  //DOM-IGNORE-END
#endif

  friend class OdBrEntityInternals;
  //DOM-IGNORE-END
};

#include "TD_PackPop.h"

#endif /* _INC_DDBRENTITY_3F819553013C_INCLUDED */

