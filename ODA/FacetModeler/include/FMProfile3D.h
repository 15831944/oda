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

#ifndef __FMPROFILE3D_H__
#define __FMPROFILE3D_H__

#include "FMProfile2D.h"
#include "FMWrapper3D.h"

namespace FacetModeler
{

  /** \details
     2D profile in 3D space.

     \remarks
     Contains a 2D profile and 3D transformation matrix.
  */
  class FMGEOMETRY_API Profile3D : public Profile2D, public Wrapper3D
  {
  public:
    /** \details
       Default constructor.
    */
    Profile3D();

    /** \details
       Constructor. Creates a Profile3D object using the specified Profile2D object without a transformation.
      
       \param cFrom2d [in] 2D profile.
    */
    explicit Profile3D(const Profile2D& cFrom2d);

    /** \details
       Constructor. Creates a Profile3D object using the specified Profile2D object and a transformation matrix.
      
       \param cFrom2d [in] 2D profile.
       \param mMatrix [in] Transformation matrix as OdGeMatrix3d.
    */
    explicit Profile3D(const Profile2D& cFrom2d,
      const OdGeMatrix3d& mMatrix);

    /** \details
       Constructor. Creates a Profile3D object using the specified Profile2D object and plane axes.
      
       \param cFrom2d [in] 2D profile.
       \param vXAxis  [in] Plane X axis vector as OdGeVector3d.
       \param vYAxis  [in] Plane Y axis vector as OdGeVector3d.
    */
    explicit Profile3D(const Profile2D& cFrom2d,
      const OdGeVector3d& vXAxis, const OdGeVector3d& vYAxis);

    /** \details
       Constructor. Creates a Profile3D object using the specified Profile2D object and plane.
      
       \param cFrom2d [in] 2D profile.
       \param rPlane  [in] Profile plane as OdGePlane.
    */
    explicit Profile3D(const Profile2D& cFrom2d,
      const OdGePlane& rPlane);

    /** \details
       Constructor. Creates a Profile3D object using the specified transformation matrix.
      
       \param mMatrix [in] Transformation matrix as OdGeMatrix3d.
    */
    explicit Profile3D(const OdGeMatrix3d& mMatrix);

    /** \details
       Constructor. Creates a Profile3D object using the specified plane axes and origin point.
      
       \param vXAxis   [in] Plane X axis vector as OdGeVector3d.
       \param vYAxis   [in] Plane Y axis vector as OdGeVector3d.
       \param ptOrigin [in] Origin point as OdGePoint3d.
    */
    explicit Profile3D(const OdGeVector3d& vXAxis,
      const OdGeVector3d& vYAxis, 
      const OdGePoint3d& ptOrigin = OdGePoint3d::kOrigin);

    /** \details
       Constructor. Creates a Profile3D object using the specified plane.
      
       \param pPlane [in] Profile plane as OdGePlane.
    */
    explicit Profile3D(const OdGePlane& pPlane);

    /** \details
       Applies the specified 2D transformation matrix to the profile with specified deviation parameters (transforms all points of the profile).
      
       \param mMatrix      [in] 2D transformation matrix.
       \param devDeviation [in] Deviation parameters (specifies the maximum deviation: the minimum and maximum number of faces on arcs during faceting).
       \returns eOk if the transformation succeeded, or a corresponding error type otherwise.
    */
    Result transformBy(const OdGeMatrix2d& mMatrix,
      const DeviationParams& devDeviation = FMGeGbl::gDefDev);

    /** \details
       Applies the specified transformation matrix to the profile (transforms the Wrapper3D matrix).
      
       \param mMatrix [in] 3D transformation matrix.
       \returns eOk if the transformation succeeded, or a corresponding error type otherwise.
    */
    Result transformBy(const OdGeMatrix3d& mMatrix);

    /** \details
       Swaps 2D data of this profile with another 2D profile but keeps the 3D transform.
      
       \param rAnother [in] 2D profile to swap data with.
    */
    void swap(Profile2D& rAnother);

    /** \details
       Swaps 2D data and the 3D transform of this profile with another 3D profile.
      
       \param rAnother [in] 3D profile to swap data with.
    */
    void swap(Profile3D& rAnother);

    /** \details
       Casts this profile to a Profile2D object.
      
       \returns Reference to the resulting Profile2D object as a const Profile2D.
    */
    inline const Profile2D& as2d() const { return *this; }

    /** \details
       Casts this profile to a Profile2D object.
      
       \returns Reference to the resulting Profile2D object.
    */
    inline       Profile2D& as2d() { return *this; }
    
    /** \details 
      Destructor.
    */
    ~Profile3D();
  };

};

#endif  //__FMPROFILE3D_H__
