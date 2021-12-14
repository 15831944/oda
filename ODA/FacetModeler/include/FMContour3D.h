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

#ifndef __FMCONTOUR3D_H__
#define __FMCONTOUR3D_H__

#include "FMContour2D.h"
#include "FMWrapper3D.h"

namespace FacetModeler
{

  /** \details
     2D contour in 3D space.

     \remarks
     Contains a 2D contour and 3D transformation matrix.
  */
  class FMGEOMETRY_API Contour3D : public Contour2D, public Wrapper3D
  {
  public:
    /** \details
       Default constructor.
    */
    Contour3D();

    /** \details
       Constructor. Creates a Contour3D object with the specified 2D contour and transformation matrix.
      
       \param rFrom2d [in] 2D contour.
       \param rMatrix [in] Transformation matrix.
    */
    explicit Contour3D(const Contour2D& rFrom2d,
      const OdGeMatrix3d& rMatrix = OdGeMatrix3d::kIdentity);

    /** \details
       Constructor. Creates a Contour3D object with the specified 2D contour and plane axes.
      
       \param rFrom2d [in] 2D contour.
       \param vXAxis  [in] Plane X axis as OdGeVector3d.
       \param vYAxis  [in] Plane Y axis as OdGeVector3d.
    */
    explicit Contour3D(const Contour2D& rFrom2d,
      const OdGeVector3d& vXAxis, const OdGeVector3d& vYAxis);

    /** \details
       Constructor. Creates a Contour3D object with the specified plane axes and origin point.
      
       \param vXAxis   [in] Plane X axis as OdGeVector3d.
       \param vYAxis   [in] Plane Y axis as OdGeVector3d.
       \param ptOrigin [in] Origin of the plane as an OdGePoint3d object.
    */
    explicit Contour3D(const OdGeVector3d& vXAxis,
      const OdGeVector3d& vYAxis, 
      const OdGePoint3d& ptOrigin = OdGePoint3d::kOrigin);

    /** \details
       Constructor. Creates a Contour3D object with the specified plane.
      
       \param rPlane [in] Plane as an OdGePlane object.
    */
    explicit Contour3D(const OdGePlane& rPlane);

    /** \details
       Applies the specified transformation matrix (transforms all points of the contour).
      
       \param mMatrix [in] 2D transformation matrix.
       \returns eOk if the transformation succeeded, or a corresponding error type otherwise.
    */
    Result transformBy(const OdGeMatrix2d& mMatrix);

    /** \details
       Applies the specified transformation matrix (transforms the Wrapper3D matrix).
      
       \param geMatrix [in] 3D transformation matrix.
       \returns eOk if the transformation succeeded, or a corresponding error type otherwise.
    */
    Result transformBy(const OdGeMatrix3d& geMatrix);

    /** \details
       Gets a 3D point of the contour at the specified index.
      
       \param iIndex     [in] Vertex index.
       \param ptPoint3D [out] Resulting point.
       \returns eOk if the point calculation succeeded, or a corresponding error type otherwise.
    */
    Result getPoint3DAt(OdUInt32 iIndex,
      OdGePoint3d& ptPoint3D) const;

    /** \details
       Gets a 3D point at the specified parameter.
      
       \param dParam    [in] Point parameter.
       \param ptPoint3D [in] Resulting point.
       \returns eOk if the point calculation succeeded, or a corresponding error type otherwise.
    */
    Result getPoint3D(double dParam,
      OdGePoint3d& ptPoint3D) const;

    /** \details
       Gets a 3D tangent vector at the specified parameter.
      
       \param dParam      [in] Point parameter.
       \param vTangent3D [out] Calculated tangent vector at dParam.
       \returns eOk if the tangent calculation succeeded, or a corresponding error type otherwise.
    */
    Result getTangent3D(double dParam,
      OdGeVector3d& vTangent3D) const;

    /** \details 
      Destructor.
    */
    ~Contour3D();
  };

};

#endif  //__FMCONTOUR3D_H__
