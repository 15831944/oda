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




#ifndef OD_SURFACECURVE2DTO3D_H
#define OD_SURFACECURVE2DTO3D_H /*!DOM*/

#include "Ge/GeCurve3d.h"

#include "TD_PackPush.h"

/** \details
    This class provides a 3D representation for curves defined in the 2D surface parameter space.

    \remarks
    A OdGeSurfaceCurve2dTo3d object is a 3D curve, and a point of this curve at specific parameter value can be calculated by evaluating a point of the UV-curve at the corresponding parameter value:
    pointCurve3d = pSurf->evalPoint(pUVCurve->evalPoint(param));

    Corresponding C++ library: TD_Ge

   <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeSurfaceCurve2dTo3d : public OdGeCurve3d
{
public:

  /** \details
    Default constructor for the OdGeSurfaceCurve2dTo3d class.
  */
  OdGeSurfaceCurve2dTo3d();
  
  /** \details
    Copy constructor for the OdGeSurfaceCurve2dTo3d class. Copies the specified object.
    
    \param source [in]  Object to copy.
  */
  OdGeSurfaceCurve2dTo3d(
    const OdGeSurfaceCurve2dTo3d& source);
  
  /** \details
    Constructor for the OdGeSurfaceCurve2dTo3d class. Constructs an object with a curve defined in the parametric space of the specified surface.
    
    \param uvc  [in]  The 2D curve defined in parameters space of the specified surface.
    \param surf [in]  The surface that contains the curve.
  */
  OdGeSurfaceCurve2dTo3d(
    const OdGeCurve2d& uvc, 
    const OdGeSurface& surf);

  /** \details
    Defines ownership flag states.
  */
  enum OwnershipFlag
  {
    /**OdGeSurfaceCurve2dTo3d object copies the input curve.*/
    kCurveCopy = 0x01,
    /**OdGeSurfaceCurve2dTo3d owns the input curve and deletes it when the object is destroyed.*/
    kCurveOwn = 0x02,
    /**OdGeSurfaceCurve2dTo3d uses the input curve but does not destroy it.*/
    kCurveReference = 0x03,
    /**OdGeSurfaceCurve2dTo3d makes a copy of the input surface.*/
    kSurfaceCopy = 0x01 << 2,
    /**OdGeSurfaceCurve2dTo3d owns the input surface and deletes it when the object is destroyed.*/
    kSurfaceOwn = 0x02 << 2,
    /**OdGeSurfaceCurve2dTo3d uses the input surface but does not destroy it.*/
    kSurfaceReference = 0x03 << 2,
    /**OdGeSurfaceCurve2dTo3d makes a copy of the input curve and surface.*/
    kCopy = kCurveCopy | kSurfaceCopy,
    /**OdGeSurfaceCurve2dTo3d owns the input curve and surface, and deletes them when the object is destroyed.*/
    kOwn = kCurveOwn | kSurfaceOwn,
    /**OdGeSurfaceCurve2dTo3d uses the input curve and surface but does not destroy them.*/
    kReference = kCurveReference | kSurfaceReference,

    //DOM-IGNORE-BEGIN
    kCurveMask = 0x03,
    kSurfaceMask = 0x03 << 2,
    //DOM-IGNORE-END
  };
  /** \details
    Constructor for the OdGeSurfaceCurve2dTo3d class. Constructs an object with the specified type of curve and surface ownership.

    \param pUvCurve [in] A uv-curve on a pSurface.
    \param pSurface [in] A surface that generates a 3d curve.
    \param ownership [in] A curve and surface ownership type.

    \remarks
    Beware: pUvCurve or pSurface objects can be modified when passed with kCurveOwn/kCurveReference or kSurfaceOwn/kSurfaceReference flags.
    Does not call non-const methods to keep the original state of pUvCurve and pSurface objects!

    Throws an exception if at least one of the pUvCurve or pSurface ojbects is NULL or the ownership type isn't a combination of kCurve* and kSurface* values from the OwnershipFlag enum.
  */
  OdGeSurfaceCurve2dTo3d(const OdGeCurve2d* pUvCurve, const OdGeSurface* pSurface, OwnershipFlag ownership);

  /** \details
    Copies the input OdGeSurfaceCurve2dTo3d.

    \param src [in] The Source OdGeSurfaceCurve2dTo3d object to be cloned.
  */
  OdGeSurfaceCurve2dTo3d& operator =(
    const OdGeSurfaceCurve2dTo3d& src);

  /** \details
    Returns the stored UV curve.
  */
  const OdGeCurve2d* curve() const;

  /** \details
    Returns the stored parametric surface.
  */
  const OdGeSurface* surface() const;

  /** \details
    Returns the geometric extents of this curve.

    \param extents [out]  Receives the geometric extents.
  */
  void getGeomExtents(
    OdGeExtents3d& extents) const;
};

#include "TD_PackPop.h"


/** \details
  Unites OdGeSurfaceCurve2dTo3d::OwnershipFlag values.
  \param a [in] first ownership flag.
  \param b [in] second ownership flag.
*/
inline OdGeSurfaceCurve2dTo3d::OwnershipFlag operator| (OdGeSurfaceCurve2dTo3d::OwnershipFlag a, OdGeSurfaceCurve2dTo3d::OwnershipFlag b) {
  return static_cast<OdGeSurfaceCurve2dTo3d::OwnershipFlag>(static_cast<int>(a) | static_cast<int>(b));
}

#endif

