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

#ifndef _ODGSCULLINGVOLUME_H_INCLUDED_
#define _ODGSCULLINGVOLUME_H_INCLUDED_

#include "OdaCommon.h"
#include "RxObject.h"
#include "Gs/GsExport.h"
#include "Ge/GeBoundBlock3d.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeSphere.h"
#include "Ge/GeVector3d.h"

#include "TD_PackPush.h"

// Culling primitives:
// BBox (AABB - Axis Aligned Bounding Box)
// BSphere (Bounding Sphere)
// OBBox (OBB - Oriented Bounding Box)

/** \details
    Base class for culling primitives.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsCullingPrimitive
{
  public:
    /** \details
      Type of culling primitive.

      \remarks
      <table>
      Name               Value    Description
      kPrimBBox          0        Axis Alligned Bounding Box.
      kPrimBSphere       1        Bounding Sphere.
      kPrimOBBox         2        Oriented Bounding Box.
      </table>
    */
    enum PrimitiveType
    {
      /** Axis-aligned Bounding box defines culling volume shape. */
      kPrimBBox,
      /** Bounding sphere defines culling volume shape. */
      kPrimBSphere,
      /** Oriented bounding box defines culling volume shape. */
      kPrimOBBox
    };
  public:
    /** \details
      Default constructor for the OdGsCullingPrimitive class.
    */
    OdGsCullingPrimitive()
    {
    }

    /** \details
      Destructor for the OdGsCullingPrimitive class.
    */
    virtual ~OdGsCullingPrimitive()
    {
    }

    /** \details
      Returns culling primitive type.
      
      \remarks
      <table>
      Name               Value    Description
      kPrimBBox          0        Axis Alligned Bounding Box.
      kPrimBSphere       1        Bounding Sphere.
      kPrimOBBox         2        Oriented Bounding Box.
      </table>
    */
    virtual PrimitiveType primitiveType() const = 0;
};

/** \details
    Culling primitive: axis-aligned bounding box.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsCullingBBox : public OdGsCullingPrimitive, public OdGeExtents3d
{
  public:
    /** \details
      Default constructor for the OdGsCullingBBox class. Creates a culling box with invalid extents.
    */
    OdGsCullingBBox()
      : OdGsCullingPrimitive()
      , OdGeExtents3d()
    {
    }

    /** \details
      Constructor for the OdGsCullingBBox class. Creates a culling box with specified minimum and maximum points for the extents.
      
      \param min  [in]  Minimum extents point.
      \param max  [in]  Maximum extents point.
    */
    OdGsCullingBBox(const OdGePoint3d &min, const OdGePoint3d &max)
      : OdGsCullingPrimitive()
      , OdGeExtents3d(min, max)
    {
    }

    /** \details
      Constructor for the OdGsCullingBBox class. Creates a culling box with specified extents.
      
      \param ext  [in]  Extents to copy.
    */
    OdGsCullingBBox(const OdGeExtents3d &ext)
      : OdGsCullingPrimitive()
      , OdGeExtents3d(ext.minPoint(), ext.maxPoint())
    {
    }

    /** \details
      Copy constructor for the OdGsCullingBBox class.
      
      \param aabb  [in]  Culling box to copy.
    */
    OdGsCullingBBox(const OdGsCullingBBox &aabb)
      : OdGsCullingPrimitive()
      , OdGeExtents3d(aabb.minPoint(), aabb.maxPoint())
    {
    }

    /** \details
      Destructor for the OdGsCullingBBox class.
    */
    ~OdGsCullingBBox()
    {
    }

    /** \details
      Assignment operator for the OdGsCullingBBox class. Assigns points of the passed extents object.
      
      \param ext  [in]  Extents to assign.
    */
    OdGsCullingBBox &operator =(const OdGeExtents3d &ext)
    {
      set(ext.minPoint(), ext.maxPoint());
      return *this;
    }

    /** \details
      Assignment operator for the OdGsCullingBBox class. Assigns points of the passed culling box object.
      
      \param aabb  [in]  Culling box to assign.
    */
    OdGsCullingBBox &operator =(const OdGsCullingBBox &aabb)
    {
      set(aabb.minPoint(), aabb.maxPoint());
      return *this;
    }

    // OdGsCullingPrimitive overrides

    /** \details
      Returns culling primitive type.
    */
    PrimitiveType primitiveType() const
    {
      return kPrimBBox;
    }
};

/** \details
    Culling primitive: bounding sphere.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsCullingBSphere : public OdGsCullingPrimitive, public OdGeSphere // GeSphere has many unnecessary data for culling
{
  public:
    /** \details
      Default constructor for the OdGsCullingBSphere class. Creates a culling sphere at the origin of world coordinates and with 1.0 radius.
    */
    OdGsCullingBSphere()
      : OdGsCullingPrimitive()
      , OdGeSphere()
    {
    }

    /** \details
      Constructor for the OdGsCullingBSphere class. Creates a culling sphere with specified center and radius.
      
      \param radius  [in]  Sphere radius.
      \param center  [in]  Sphere center.
    */
    OdGsCullingBSphere(double radius, const OdGePoint3d &center)
      : OdGsCullingPrimitive()
      , OdGeSphere(radius, center)
    {
    }

    /** \details
      Constructor for the OdGsCullingBSphere class. Creates a culling sphere based on the passed GE sphere.
      
      \param sphere  [in]  Sphere to copy.
    */
    OdGsCullingBSphere(const OdGeSphere &sphere)
      : OdGsCullingPrimitive()
      , OdGeSphere(sphere)
    {
    }

    /** \details
      Copy constructor for the OdGsCullingBSphere class.
      
      \param bsphere  [in]  Culling sphere to copy.
    */
    OdGsCullingBSphere(const OdGsCullingBSphere &bsphere)
      : OdGsCullingPrimitive()
      , OdGeSphere(bsphere)
    {
    }

    /** \details
      Destructor for the OdGsCullingBSphere class.
    */
    ~OdGsCullingBSphere()
    {
    }

    /** \details
      Assignment operator for the OdGsCullingBSphere class. Assigns parameters of the passed sphere object.
      
      \param sphere  [in]  Sphere to assign.
    */
    OdGsCullingBSphere &operator =(const OdGeSphere &sphere)
    {
      set(sphere.radius(), sphere.center());
      return *this;
    }

    /** \details
      Assignment operator for the OdGsCullingBSphere class. Assigns parameters of the passed culling sphere object.
      
      \param bsphere  [in]  Culling sphere to assign.
    */
    OdGsCullingBSphere &operator =(const OdGsCullingBSphere &bsphere)
    {
      set(bsphere.radius(), bsphere.center());
      return *this;
    }

    // OdGsCullingPrimitive overrides

    /** \details
      Returns culling volume type.
    */
    PrimitiveType primitiveType() const
    {
      return kPrimBSphere;
    }
};

/** \details
    Culling primitive: oriented bounding box.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsCullingOBBox : public OdGsCullingPrimitive, public OdGeBoundBlock3d
{
  public:
    /** \details
      Default constructor for the OdGsCullingOBBox class. Creates an oriented culling box with origin point at (0,0,0) extending 1.0 unit in a positive direction for x-, y- and z-axis.
    */
    OdGsCullingOBBox()
      : OdGsCullingPrimitive()
      , OdGeBoundBlock3d()
    {
    }

    /** \details
      Constructor for the OdGsCullingOBBox class. Creates an oriented culling box with specified base point and directions.
      
      \param base  [in]  Base of the bounding box.
      \param xAxis  [in]  X-axis direction.
      \param yAxis  [in]  Y-axis direction.
      \param zAxis  [in]  Z-axis direction.
    */
    OdGsCullingOBBox(const OdGePoint3d &base, const OdGeVector3d &xAxis, const OdGeVector3d &yAxis, const OdGeVector3d &zAxis)
      : OdGsCullingPrimitive()
      , OdGeBoundBlock3d(base, xAxis, yAxis, zAxis)
    {
    }

    /** \details
      Constructor for the OdGsCullingOBBox class. Creates an oriented culling box based on the provided bounding block.
      
      \param bb  [in]  Bounding box.
    */
    OdGsCullingOBBox(const OdGeBoundBlock3d &bb)
      : OdGsCullingPrimitive()
      , OdGeBoundBlock3d(bb)
    {
    }

    /** \details
      Copy constructor for the OdGsCullingOBBox class.
      
      \param obb  [in]  Oriented bounding box.
    */
    OdGsCullingOBBox(const OdGsCullingOBBox &obb)
      : OdGsCullingPrimitive()
      , OdGeBoundBlock3d(obb)
    {
    }

    /** \details
      Destructor for the OdGsCullingBBox class.
    */
    ~OdGsCullingOBBox()
    {
    }

    /** \details
      Assignment operator for the OdGsCullingOBBox class. Assigns origin and direction vectors of the passed bounding block.
      
      \param bb  [in]  Bounding block.
    */
    OdGsCullingOBBox &operator =(const OdGeBoundBlock3d &bb)
    {
      OdGePoint3d base; OdGeVector3d side1, side2, side3;
      bb.get(base, side1, side2, side3);
      set(base, side1, side2, side3);
      return *this;
    }

    /** \details
      Assignment operator for the OdGsCullingOBBox class. Assigns origin and direction vectors of the passed oriented bounding box.
      
      \param obb  [in]  Oriented bounding box.
    */
    OdGsCullingOBBox &operator =(const OdGsCullingOBBox &obb)
    {
      OdGePoint3d base; OdGeVector3d side1, side2, side3;
      obb.get(base, side1, side2, side3);
      set(base, side1, side2, side3);
      return *this;
    }

    // OdGsCullingPrimitive overrides

    /** \details
      Returns culling primitive type.
    */
    PrimitiveType primitiveType() const
    {
      return kPrimOBBox;
    }
};

// Projection culling primitives:
// Ortho (Orthogonal culling box merged with optimization for 2d mode)
// Frustum (Perspective culling box without near and far culling planes)

/** \details
    Base class for orthogonal and perspective culling volumes.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsCullingVolume : public OdRxObject
{
  public:
    /** \details
      Represents the projection type of the culling volume.
    */
    enum ProjectionType
    {
      /** Orthogonal projection.*/
      kProjOrtho,
      /** Perspective projection.*/
      kProjPerspective
    };
    /** \details
      Represents the intersection status of the culling volume.
    */
    enum IntersectionStatus
    {
      /** No intersection detected.*/
      kIntersectNot,
      /** Intersects with projection boundary.*/
      kIntersectOk,
      /** Primitive completely inside projection boundary.*/
      kIntersectIn
    };
  public:
    ODRX_DECLARE_MEMBERS(OdGsCullingVolume);

    /** \details
        Returns type of projection for this culling volume object.

        \remarks
        <table>
        Name               Value    Description
        kProjOrtho         0        Orthogonal projection.
        kProjPerspective   1        Perspective projection.
        </table>
    */
    virtual ProjectionType projectionType() const = 0;

    /** \details
        Optimized intersection test between culling volume and culling primitive.
        \param prim [in]  Input primitive for culling detection.
        \returns
        true if the provided primitive intersects with this culling volume, false otherwise.
    */
    virtual bool intersectWithOpt(const OdGsCullingPrimitive &prim) const = 0;
    /** \details
        Intersection test between culling volume and culling primitive (returns complete and incomplete
        intersection statuses).
        
        \param prim [in]  Input primitive for culling detection.        
        \returns
        one of the following intersection statuses:
        <table>
        Name               Value    Description
        kIntersectNot      0        No intersection detected.
        kIntersectOk       1        Intersects with culling volume boundary.
        kIntersectIn       2        Culling volume primitive is completely inside culling volume boundary.
        </table>
    */
    virtual IntersectionStatus intersectWith(const OdGsCullingPrimitive &prim) const = 0;

    /** \details
        Transform culling volume by transformation matrix.
        \param xfm [in]  Xform matrix.
    */
    virtual void transformBy(const OdGeMatrix3d& xfm) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsCullingVolume object pointers.
*/
typedef OdSmartPtr<OdGsCullingVolume> OdGsCullingVolumePtr;

/** \details
    Orthogonal culling volume.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsOrthoCullingVolume : public OdGsCullingVolume
{
  public:
    ODRX_DECLARE_MEMBERS(OdGsOrthoCullingVolume);

    /** \details
        Initialize frustum culling volume by new values.
        \param position [in]  View position.
        \param direction [in]  View direction.
        \param upVector [in]  View up vector.
        \param volumeWidth [in]  Horizontal field of view.
        \param volumeHeight [in]  Vertical field of view.
    */
    virtual void init(const OdGePoint3d &position, const OdGeVector3d &direction, const OdGeVector3d &upVector,
                      double volumeWidth, double volumeHeight) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsOrthoCullingVolume object pointers.
*/
typedef OdSmartPtr<OdGsOrthoCullingVolume> OdGsOrthoCullingVolumePtr;

/** \details
    Perspective culling volume.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsFrustumCullingVolume : public OdGsCullingVolume
{
  public:
    ODRX_DECLARE_MEMBERS(OdGsFrustumCullingVolume);

    /** \details
        Initialize frustum culling volume by new values.
        \param position [in]  View position.
        \param direction [in]  View direction.
        \param upVector [in]  View up vector.
        \param fovY [in]  Horizontal field of view in radians.
        \param aspect [in]  Aspect ratio between horizontal and vertical field of view (aspect = h / v).
        \param nPlanes [in]  Number of culling planes to test intersection (1,2,3,4 - enable only side planes, 5 - enable nearZ plane, 6 - enable farZ plane).
        \param nearZ [in]  Near Z-axis culling plane (must be greater than zero).
        \param farZ [in]  Far Z-axis culling plane.
    */
    virtual void init(const OdGePoint3d &position, const OdGeVector3d &direction, const OdGeVector3d &upVector,
                      double fovY, double aspect, OdUInt32 nPlanes = 4, double nearZ = 1.0, double farZ = 1e20) = 0;
    /** \details
        Initialize frustum culling volume by new values.
        \param position [in]  View position.
        \param direction [in]  View direction.
        \param upVector [in]  View up vector.
        \param fovX [in]  Horizontal field of view in radians.
        \param aspect [in]  If enabled - then fovY is aspect ratio (fovY = h / v), elsewhere fovY is vertical field of view in radians.
        \param fovY [in]  Aspect ratio or vertical field of view in radians (see aspect argument description).
        \param nPlanes [in]  Number of culling planes to test intersection (1,2,3,4 - enable only side planes, 5 - enable nearZ plane, 6 - enable farZ plane).
        \param nearZ [in]  Near Z-axis culling plane (must be greater than zero).
        \param farZ [in]  Far Z-axis culling plane.
    */
    virtual void init(const OdGePoint3d &position, const OdGeVector3d &direction, const OdGeVector3d &upVector,
                      double fovX, bool aspect, double fovY, OdUInt32 nPlanes = 4, double nearZ = 1.0, double farZ = 1e20) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsFrustumCullingVolume object pointers.
*/
typedef OdSmartPtr<OdGsFrustumCullingVolume> OdGsFrustumCullingVolumePtr;

#include "TD_PackPop.h"

#endif // _ODGSCULLINGVOLUME_H_INCLUDED_
