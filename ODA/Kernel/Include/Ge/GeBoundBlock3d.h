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

#ifndef _OD_GEBOUNDBLOCK3D_H
#define _OD_GEBOUNDBLOCK3D_H /*!DOM*/

#include "Ge/GeEntity3d.h"
#include "Ge/GeExtents3d.h"

#include "TD_PackPush.h"

/** \details
    This class implements 3D bounding volumes.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeBoundBlock3D, Working with Bounding Blocks>
*/
class GE_TOOLKIT_EXPORT OdGeBoundBlock3d : public OdGeEntity3d
{
public: 


  /** \details 
    The default constructor for the OdGeBoundBlock3d class. Constructs a parallelepiped reduced to the coordinate origin.
    \remarks
    A parallelepiped is a parallelogram extruded in an arbitrary direction. 
  */
  OdGeBoundBlock3d();
  
  /** 
    \details Constructor for the OdGeBoundBlock3d class.
    \param base [in]  Base of a parallelogram bounding block.
    \param side1 [in]  First side of a parallelogram bounding block.
    \param side2 [in]  Second side of a parallelogram bounding block.
    \param side3 [in]  Third side of a parallelogram bounding block.
    
    \remarks
    The constructor constructs a parallelogram with given point and three sides.
  */
  OdGeBoundBlock3d(
    const OdGePoint3d& base, 
    const OdGeVector3d& side1,
    const OdGeVector3d& side2, 
    const OdGeVector3d& side3);
  // TD Special
  
  /** 
    \details Constructor for the OdGeBoundBlock3d class.
    \param point1 [in]  First point of a coordinate-aligned box.
    \param point2 [in]  Second point of a coordinate-aligned box.

    \remarks
    The constructor constructs a box bounded by two input points.
  */
  OdGeBoundBlock3d(
    const OdGePoint3d& point1, 
    const OdGePoint3d& point2);
  //
  
  /** 
    \details Constructor for the OdGeBoundBlock3d class.
    \param source [in]  Object to be cloned.
    
    \remarks
    Copies the input OdGeBoundBlock3d object.
  */
  OdGeBoundBlock3d(
    const OdGeBoundBlock3d& source);

  /** \details
    Constructor for the OdGeBoundBlock3d class.
    \param lcs [in]  Local coordinate system (must be orthogonal).
    \param localBox [in]  Axis-aligned box in the specified lcs.
  */
  OdGeBoundBlock3d(
    const OdGeMatrix3d& lcs, 
    const OdGeExtents3d& localBox);

  /** \details
    Returns the extents of the bounding block.

    \param p1 [out]  Receives the first corner of the extents.
    \param p2 [out]  Receives the second corner of the extents.
  */
   void getMinMaxPoints(
    OdGePoint3d& p1, 
    OdGePoint3d& p2) const;

  /** \details
    Returns base and sides of bounding block.

    \param base [out]  Receives the base of the bounding box.
    \param side1 [out]  Receives the first side.
    \param side2 [out]  Receives the second side.
    \param side3 [out]  Receives the third side.
  */
   void get(
    OdGePoint3d& base,
    OdGeVector3d& side1,
    OdGeVector3d& side2,
    OdGeVector3d& side3) const;

  /** \details
    Sets the bounding block to a coordinate-aligned box.

    \param p1 [in]  First point of a coordinate-aligned box.
    \param p2 [in]  Second point of a coordinate-aligned box.
  */
   OdGeBoundBlock3d& set(
    const OdGePoint3d& p1,
    const OdGePoint3d& p2);
    
  /** \details
    Sets the bounding block to a parallelepiped bounding block.

    \param base [in]  Base of parallelepiped bounding block.
    \param side1 [in]  First side of parallelepiped bounding block.
    \param side2 [in]  Second side of parallelepiped bounding block.
    \param side3 [in]  Third side of parallelepiped bounding block.
  */
   OdGeBoundBlock3d& set(
    const OdGePoint3d& base,
    const OdGeVector3d& side1,
    const OdGeVector3d& side2,
    const OdGeVector3d& side3);

  /** \details
    Extends the bounding block to contain
    the specified point.

    \param point [in]  Any 3D point.
  */
  OdGeBoundBlock3d& extend(
    const OdGePoint3d& point);

  /** \details
    Moves the walls of the bounding block the specified distance.

    \param distance [in]  Distance.
  */
  OdGeBoundBlock3d& swell(
    double distance);

  /** \details
    Checks whether the passed point is contained in the bounding block.

    \param point [in]  Any 3D point.
    \param tol   [in]  Geometric tolerance.
    \returns
    true if the specified point is contained in the bounding block, false otherwise.
  */
  bool contains(
    const OdGePoint3d& point, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Checks whether the specified bounding block
    does not intersect this bounding block.

    \param block [in]  Any 3D bounding block.
    \param tol   [in]  Geometric tolerance.
    \returns
    true if the specified bounding block does not intersect this bounding block, false otherwise.
  */
  bool isDisjoint(
    const OdGeBoundBlock3d& block,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if this bounding block is a
    coordinate-aligned box.
  */
  bool isBox() const;

  /** \details
    Sets this bounding block to a coordinate-aligned box, or a
    parallelogram bounding block. Returns a reference to this 
    bounding block.

    \param toBox [in]  If true, sets this bounding block to a 
    coordinate-aligned otherwise, sets it to a 
    parallelepiped bounding block.
  */
  OdGeBoundBlock3d&  setToBox(
    bool toBox);

  /** 
    \details The assignment operator for the OdGeBoundBlock3d class.
    \param block [in]  Input OdGeBoundBlock3d object.

    \remarks
    Makes this block identical to the input block.
  */
  OdGeBoundBlock3d& operator =(const OdGeBoundBlock3d& block);

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** 
    \details Gets the minimum point of the OdGeBoundBlock3d object.

    \remarks
    The min point of the object extents is one of two points that are used to construct a bounding block. 
    The min point has minor values for each coordinate axis compared to the max point.
  */
   OdGePoint3d minPoint() const;
   
   /** 
    \details Gets the maximum point of the OdGeBoundBlock3d object.

    \remarks
    The max point of the object extents is one of two points that are used to construct a bounding block. 
    The max point has greater values for each coordinate axis compared to the min point.
  */
   OdGePoint3d maxPoint() const;

  /** \details
    Returns the center of the bounding block.
  */
   OdGePoint3d center() const;

  /** \details
  Sets this bounding block to a coordinate-aligned box if the specified axes are parallel to world coordinate space axes.

  \param dir1 [in]  First coordinate system axis.
  \param dir2 [in]  Second coordinate system axis.
  \param dir3 [in]  Third coordinate system axis.
  */
   void setToBoxOrtho(const OdGeVector3d &dir1, const OdGeVector3d &dir2, const OdGeVector3d &dir3);
  
  /** \details
  Sets this bounding block to a coordinate-aligned box if axes of this bound block are parallel to world coordinate space axes.
  */
   void setToBoxOrtho();
};

inline void OdGeBoundBlock3d::setToBoxOrtho(const OdGeVector3d &dir1, const OdGeVector3d &dir2, const OdGeVector3d &dir3)
{
  OdGeError status;
  if((dir1.isParallelTo(OdGeVector3d::kXAxis)
      || dir1.isParallelTo(OdGeVector3d::kYAxis)
      || dir1.isParallelTo(OdGeVector3d::kZAxis, OdGeContext::gTol, status)
      || status == OdGe::k0This)
      && (dir2.isParallelTo(OdGeVector3d::kXAxis)
      || dir2.isParallelTo(OdGeVector3d::kYAxis)
      || dir2.isParallelTo(OdGeVector3d::kZAxis, OdGeContext::gTol, status)
      || status == OdGe::k0This)
      && (dir3.isParallelTo(OdGeVector3d::kXAxis)
      || dir3.isParallelTo(OdGeVector3d::kYAxis)
      || dir3.isParallelTo(OdGeVector3d::kZAxis, OdGeContext::gTol, status)
      || status == OdGe::k0This))
    setToBox(true);
}

inline void OdGeBoundBlock3d::setToBoxOrtho()
{
  OdGePoint3d basePt;
  OdGeVector3d dir1, dir2, dir3;
  get(basePt, dir1, dir2, dir3);
  setToBoxOrtho(dir1, dir2, dir3);
}

#include "TD_PackPop.h"

#endif
