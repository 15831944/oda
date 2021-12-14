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

#ifndef __FMWRAPPER3D_H__
#define __FMWRAPPER3D_H__

#include "FMContoursBase.h"
#include <Ge/GeMatrix3d.h>
#include <Ge/GePlane.h>
#include <Ge/GePoint3d.h>

namespace FacetModeler
{
  /** \details
    Represents wrapper for 2D entities for using in 3D.
  */
  class FMGEOMETRY_API Wrapper3D
  {
  public:
    /** \details
       Default constructor.
    */
    Wrapper3D();

    /** \details
       Constructor. Creates a wrapper object with the specified transformation matrix.
      
       \param rMatrix [in] Transformation matrix.
    */
    explicit Wrapper3D(const OdGeMatrix3d& rMatrix);

    /** \details
       Constructor. Creates a wrapper object with the specified axes and point of origin.
      
       \param vXAxis   [in] X axis.
       \param vYAxis   [in] Y axis.
       \param ptOrigin [in] Point of origin.
    */
    explicit Wrapper3D(const OdGeVector3d& vXAxis,
      const OdGeVector3d& vYAxis,
      const OdGePoint3d& ptOrigin = OdGePoint3d::kOrigin);

    /** \details
       Constructor. Creates a wrapper object with the specified plane.
      
       \param rPlane [in] Plane.
    */
    explicit Wrapper3D(const OdGePlane& rPlane);

    /** \details
       Gets the plane.

       \returns Plane as OdGePlane.
    */
    OdGePlane plane() const;

    /** \details
       Gets the transformation matrix.
      
       \returns Reference to the transformation matrix.
    */
    const OdGeMatrix3d& transform() const;

    /** \details
       Gets the inverse transformation matrix.
      
       \returns Reference to the inverse transformation matrix.
    */
    const OdGeMatrix3d& inversedTransform() const;

    /** \details
       Sets the transformation matrix.
      
       \param mTransform [in] New transformation matrix to set.
       \returns eOk if the matrix set successfully, or a corresponding error type otherwise.
    */
    Result setTransform(const OdGeMatrix3d& mTransform);

    /** \details
       Sets a transformation matrix with the specified axes and point of origin.
      
       \param vXAxis   [in] X axis.
       \param vYAxis   [in] Y axis.
       \param ptOrigin [in] Point of origin.
       \returns eOk if the matrix set successfully, or a corresponding error type otherwise.
    */
    Result setTransform(const OdGeVector3d& vXAxis,
      const OdGeVector3d& vYAxis,
      const OdGePoint3d& ptOrigin = OdGePoint3d::kOrigin);

    /** \details
       Applies the given transformation matrix to the Wrapper3D object.
      
       \param mMatrix [in] Transformation matrix to apply.
       \returns eOk if the matrix applied successfully, or a corresponding error type otherwise.
    */
    Result transformBy(const OdGeMatrix3d& mMatrix);

    /** \details
       Transforms a point to the 3D coordinate system by applying a transformation matrix.
      
       \param rSource2D [in] 2D point to transform.
       \param rDest3D  [out] 3D resulting point.
    */
    void to3D(const OdGePoint2d& rSource2D,
      OdGePoint3d& rDest3D) const;

    /** \details
       Transforms a vector to the 3D coordinate system by applying a transformation matrix.
      
       \param rSource2D [in] 2D vector to transform.
       \param rDest3D  [out] 3D resulting vector.
    */
    void to3D(const OdGeVector2d& rSource2D,
      OdGeVector3d& rDest3D) const;

    /** \details
       Transforms a point from the 3D coordinate system to 2D by applying an inverse transformation matrix.
      
       \param rSource3D [in] 3D point to transform.
       \param rDest2D  [out] 2D resulting point.
       \returns eOk if the source object lies on the plane of this wrapper object, or erWarnPointNotOnThis otherwise.
    */
    Result to2D(const OdGePoint3d& rSource3D,
      OdGePoint2d& rDest2D) const;

    /** \details
       Transforms the vector from the 3D coordinate system to 2D by applying a inverse transformation matrix.
      
       \param rSource3D [in] 3D Vector to transform.
       \param rDest2D  [out] 2D Resulting vector.
       \returns eOk if the source object lies on the plane of this wrapper object, erWarnPointNotOnThis - otherwise.
    */
    Result to2D(const OdGeVector3d& rSource3D,
      OdGeVector2d& rDest2D) const;

    /** \details
       Swaps the transformation of this object and the specified objects.
      
       \param rAnother [in] Wrapper object to swap with.
    */
    void swap(Wrapper3D& rAnother);

  private:
    OdGeMatrix3d m_mTrans;       // Transformation matrix.
    OdGeMatrix3d m_mInvertTrans; // Inverted transformation matrix.
  };

};

#endif  //__FMWRAPPER3D_H__
