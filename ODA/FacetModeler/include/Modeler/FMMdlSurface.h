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

#ifndef __FMMDL_SURFACE_H__
#define __FMMDL_SURFACE_H__

#include "Modeler/FMMdlEntity.h"
#include "Ge/GeLine3d.h"

namespace FacetModeler
{

  /** \details
     Represents facet body surface objects.
  */
  class FMGEOMETRY_API Surface : public Entity
  {
  public:
    /** \details
       Default constructor.
    */
    Surface();

    /** \details
       Creates a copy of the surface to the specified body.

       \param to [in] Destination body.
       \returns Pointer to the newly created surface.
    */
    virtual Surface* copy(Body& to) const;

    /** \details
       Defines surface object types.
    */
    enum Type {
      /** Undefined surface */
      kUndefined = 0,

      /** Cylindrical surface */
      kCylinder = 1,

      /** Conical surface */
      kCone = 2,

      /** Spherical surface */
      kSphere = 3,

      /** Toroidal surface */
      kTorus = 4,

      /** Used for shaded rendering in Architecture to mark continuous surfaces */
      kShaded = 5
    };

    /** \details
       Gets the surface type.

       \returns Type of the surface as a Type enumeration value.
    */
    virtual Type type() const;

    /** \details
       Gets the normal at the specified vertex on the specified face.

       \param atV [in] The vertex at which the normal is computed.
       \param atF [in] The face for which the normal is computed.
       \returns Normal at the specified vertex on the specified face.

       \remarks
       Child surfaces do not use the second input parameter.
    */
    virtual OdGeVector3d normal(const Vertex& atV, const Face& atF) const;

    /** \details
       Transforms the surface with the given transformation matrix.

       \param x [in] Given transformation matrix.

       \remarks
       Base class has no data to transform.
    */
    virtual void transform(const OdGeMatrix3d& x);

    /** \details
       Compares surfaces with the specified tolerance.

       \param pOther [in] Other surface to compare with.
       \param eps    [in] Comparison tolerance.
       \returns
       * 0 - if the surfaces are equal,
       * 1 - if a parameter of this surface is greater than the corresponding parameter of another surface,
       * -1 - otherwise.

       \remarks
       The main purpose of the method is to sort surfaces.
    */
    virtual int compare(const Surface* pOther, double eps) const;

    /** \details
       Gets the next surface in the body.

       \returns Pointer to the next surface in the body.
    */
    Surface* next() const;

    /** \details
       Sets the pointer to the next item in the list of surfaces.

       \param s [in] Next surface pointer to set.
    */
    void setNext(Surface* s);
    
    /** \details 
      Destructor.
    */
    ~Surface();

  protected:

    /** \details 
      Compares this surface with other by types.
    
      \param pOther [in] Other surface pointer.
      \param eps   [out] Epsilon.
    
      \returns 
        * 1 - if this surface type > other surface type
        * -1 - otherwise.
    */
    int surface_compare(const Surface* pOther, double eps) const;

    /** \details
       Creates a clone of the surface in the specified body.

       \param to [in] Destination body.
       \returns Pointer to the cloned object.

       \remarks
       Saves object binding.
    */
    virtual Surface* clone(Body& to) const;

  private:

    /** \details 
      Copy constructor.
    */
    Surface(const Surface&);

    /** \details 
      Copies data from other Surface.
    
      \param other [in] Surface to copy data from.
    
      \returns Reference to this object.
    */
    Surface& operator=(const Surface& other);

    Surface* m_pNext; // Next Surface pointer.

    //DOM-IGNORE-BEGIN
    friend class Body;
    friend class BodyImpl;
    friend class BodyCustom;
    //DOM-IGNORE-END
  };

  /** \details
    Used to mark shaded surfaces in Architecture rendering. Vertex normals will be calculated on faces marked with the same 
    ShadedSurface instance
  */
  class FMGEOMETRY_API ShadedSurface : public Surface {
  public:
    /** \details
    Constructor. Creates a ShadedSurface

    \param creaseAngle [in] The angle between faces to count the edge "smooth".
    */
    ShadedSurface(double creaseAngle = 0);

    /** \details
    Gets the surface type.

    \always returns kShaded for ShadedSurface
    */
    virtual Type type() const;

    /** \details
    Creates a copy of the surface to the specified body.

    \param to [in] Destination body.
    \returns Pointer to the newly created surface.
    */
    virtual Surface* copy(Body& to) const;

    /** \details
    Finds sharp edges laying on this surface.

    \param body [in] Current body.
    \param angleTol [in] The angle threshold to cound edge "sharp".
    */
    void findSharpEdges(const Body& body, double angleTol);

    /** \details
    Returns true if the vertex belongs to sharp edge.

    \param vertex [in] The vertex to check.
    */
    bool belongsToSharpEdge(const Vertex* vertex) const;

    /** \details
    Returns the number of sharp edges for this surface.
    */
    int nSharpEdges() const;

    /** \details
    Returns the crease angle of this surface.
    */
    double creaseAngle() const;

  protected:
    /** \details
    The set of vertexes laying on sharp edges. Sharp edges will not be rendered "smooth" by FMDrawBody.
    */
    std::set<const Vertex*> m_setSharpEdge;

    /** \details
    The angle between faces to count the edge "smooth".
    */
    double m_creaseAngle;
  };

  /** \details
     Represents facet body revolution objects.
  */
  class FMGEOMETRY_API Revolution : public Surface {
  public:
    /** \details
       Constructor. Creates a revolution surface with the specified axis of rotation and the start direction vector.
      
       \param axis     [in] Axis of rotation line.
       \param startDir [in] Start direction vector.
    */
    Revolution(const OdGeLine3d& axis, const OdGeVector3d& startDir);

    /** \details
       Transforms the surface with the given transformation matrix.

       \param x [in] Given transformation matrix.
    */
    void transform(const OdGeMatrix3d& x);

    /** \details
       Gets the axis of the revolution surface.

       \returns Axis as an OdGeLine3d object.
    */
    const OdGeLine3d& axis() const;

    /** \details
       Gets the start direction of the revolution surface.

       \returns Start direction vector as an OdGeVector3d object.
    */
    const OdGeVector3d& startDir() const;

    /** \details
       Compares surfaces with the specified tolerance.

       \param pOther [in] Other surface to compare with.
       \param eps    [in] Comparison tolerance.
       \returns
       * 0 - if the surfaces are equal,
       * 1 - if a parameter of this surface is greater than the corresponding parameter of another surface,
       * -1 - otherwise.

       \remarks
       The main purpose of the method is to sort surfaces.
    */
    int compare(const Surface* pOther, double eps) const;

  protected:
    OdGeLine3d m_axis;       // Rotation axis vector.
    OdGeVector3d m_startDir; // Start direction vector.
  };

  /** \details
     Represents facet body spherical objects.
  */
  class FMGEOMETRY_API Sphere : public Revolution {
  public:
    /** \details
       Constructor. Creates a sphere with the specified center point, radius and approximation.
      
       \param center [in] Sphere center coordinates.
       \param radius [in] Radius value.
       \param approx [in] Approximation.
    */
    Sphere(const OdGePoint3d& center, double radius, OdUInt32 approx);

    /** \details
       Gets the surface type.

       \returns Type of the surface as a Type enumeration value.
    */
    Type type() const;

    /** \details
       Gets the sphere center coordinates.

       \returns Sphere center point as an OdGePoint3d object.
    */
    const OdGePoint3d& center() const;

    /** \details
       Gets the radius value.

       \returns Value of the radius.
    */
    double radius() const;

    /** \details
       Gets the approximation.

       \returns Approximation as OdUInt32.
    */
    OdUInt32 approximation() const;

    /** \details
       Gets the normal at the specified vertex on the specified face.

       \param atV [in] The vertex at which the normal is computed.
       \param atF [in] The face for which the normal is computed.
       \returns Normal at the specified vertex on the specified face.

       \remarks
       Parameter atF is unused.
    */
    OdGeVector3d normal(const Vertex& atV, const Face& atF) const;

    /** \details
       Transforms the surface with the given transformation matrix.

       \param x [in] Given transformation matrix.
    */
    void transform(const OdGeMatrix3d& x);

    /** \details
       Compares surfaces with the specified tolerance.

       \param pOther [in] Other surface to compare with.
       \param eps    [in] Comparison tolerance.
       \returns
       * 0 - if the surfaces are equal,
       * 1 - if a parameter of this surface is greater than the corresponding parameter of another surface,
       * -1 - otherwise.

       \remarks
       The main purpose of the method is to sort surfaces.
    */
    int compare(const Surface* pOther, double eps) const;

    /** \details
       Creates a copy of the surface to the specified body.

       \param to [in] Destination body.
       \returns Pointer to the newly created surface.
    */
    Surface* copy(Body& to) const;

  protected:
    OdGePoint3d m_center;     // Center point coordinates.
    double m_radius;          // Radius value.
    OdUInt32 m_approximation; // Approximation.
  };

  /** \details
     Represents facet body cylindrical objects.
  */
  class FMGEOMETRY_API Cylinder : public Revolution {
  public:
    /** \details
       Creates a cylinder with the specified axis of rotation, start direction of rotation, radius and approximation.
      
       \param axis     [in] Axis of the cylinder.
       \param startDir [in] Start direction of rotation.
       \param dRadius  [in] Radius value.
       \param approx   [in] Approximation.
    */
    Cylinder(const OdGeLine3d& axis, const OdGeVector3d& startDir,
      double dRadius, OdUInt32 approx);

    /** \details
       Gets the surface type.

       \returns Type of the surface as a Type enumeration value.
    */
    Type type() const;

    /** \details
       Gets the radius value.

       \returns Value of the radius.
    */
    double radius() const;

    /** \details
       Gets the approximation.

       \returns Approximation as OdUInt32.
    */
    OdUInt32 approximation() const;

    /** \details
       Gets the normal at the specified vertex on the specified face.

       \param atV [in] The vertex at which the normal is computed.
       \param atF [in] The face for which the normal is computed.
       \returns Normal at the specified vertex on the specified face.

       \remarks
       Parameter atF is unused.
    */
    OdGeVector3d normal(const Vertex& atV, const Face& atF) const;

    /** \details
       Transforms the surface with the given transformation matrix.

       \param x [in] Given transformation matrix.
    */
    void transform(const OdGeMatrix3d& x);

    /** \details
       Compares surfaces with the specified tolerance.

       \param pOther [in] Other surface to compare with.
       \param eps    [in] Comparison tolerance.
       \returns
       * 0 - if the surfaces are equal,
       * 1 - if a parameter of this surface is greater than the corresponding parameter of another surface,
       * -1 - otherwise.

       \remarks
       The main purpose of the method is to sort surfaces.
    */
    int compare(const Surface* pOther, double eps) const;

    /** \details
       Creates a copy of the surface to the specified body.

       \param to [in] Destination body.
       \returns Pointer to the newly created surface.
    */
    Surface* copy(Body& to) const;

  protected:
    double m_radius; // Radius value.
    OdUInt32 m_approximation; // Approximation.
  };

  /** \details
     Represents facet body conical objects.
  */
  class FMGEOMETRY_API Cone : public Revolution {
  public:
    /** \details
       Constructor. Creates a cone with the specified axis, start direction of rotation, radius of the bottom circle, radius of the top circle and approximation.
      
       \param axis      [in] Axis of the cone.
       \param startDir  [in] Start direction of rotation.
       \param bottomRadius [in] Radius of the bottom circle.
       \param topRadius [in] Radius of the top circle.
       \param approx    [in] Approximation.
    */
    Cone(const OdGeLine3d& axis, const OdGeVector3d& startDir,
      double bottomRadius, double topRadius, OdUInt32 approx);

    /** \details
       Gets the surface type.

       \returns Type of the surface as a Type enumeration value.
    */
    Type type() const;

    /** \details
       Gets the radius value.

       \returns Value of the radius.
    */
    double radius() const;

    /** \details
       Calculates the radius value on the cone top.

       \returns Value of the radius.
    */
    double topRadius() const;

    /** \details
       Gets the tangent angle value.
      
       \returns Value of the tangent angle.
    */
    double tanAngle() const;

    /** \details
       Gets the approximation.

       \returns Approximation as OdUInt32.
    */
    OdUInt32 approximation() const;

    /** \details
       Gets the normal at the specified vertex on the specified face.

       \param atV [in] The vertex at which the normal is computed.
       \param atF [in] The face for which the normal is computed.
       \returns Normal at the specified vertex on the specified face.

       \remarks
       Parameter atF is unused.
    */
    OdGeVector3d normal(const Vertex& atV, const Face& atF) const;

    /** \details
       Transforms the surface with the given transformation matrix.

       \param x [in] Given transformation matrix.
    */
    void transform(const OdGeMatrix3d& x);

    /** \details
       Compares surfaces with the specified tolerance.

       \param pOther [in] Other surface to compare with.
       \param eps    [in] Comparison tolerance.
       \returns
       * 0 - if the surfaces are equal,
       * 1 - if a parameter of this surface is greater than the corresponding parameter of another surface,
       * -1 - otherwise.

       \remarks
       The main purpose of the method is to sort surfaces.
    */
    int compare(const Surface* pOther, double e) const;

    /** \details
       Creates a copy of the surface to the specified body.

       \param to [in] Destination body.
       \returns Pointer to the newly created surface.
    */
    Surface* copy(Body& to) const;

  protected:
    double m_radius;   // Radius value.
    double m_tanAngle; // Angle tangent value.
    OdUInt32 m_approximation; // Approximation.
  };

  /** \details
     Represents facet body toroidal objects.
  */
  class FMGEOMETRY_API Torus : public Revolution {
  public:
    /** \details
       Constructor. Creates a torus with the specified axis line, start direction, major and minor radii, and major and minor approximations.
      
       \param axis        [in] Axis of the torus.
       \param startDir    [in] Start direction of rotation.
       \param majorRadius [in] Major radius value.
       \param minorRadius [in] Minor radius value.
       \param majorApprox [in] Major approximation.
       \param minorApprox [in] Minor approximation.
    */
    Torus(const OdGeLine3d& axis, const OdGeVector3d& startDir,
      double majorRadius, double minorRadius,
      OdUInt32 majorApprox, OdUInt32 minorApprox);

    /** \details
       Gets the surface type.

       \returns Type of the surface as a Type enumeration value.
    */
    Type type() const;

    /** \details
       Gets the major radius value.

       \returns Value of the major radius.
    */
    double majorRadius() const;

    /** \details
       Gets the minor radius value.

       \returns Value of the minor radius.
    */
    double minorRadius() const;

    /** \details
       Gets the major approximation.

       \returns Major approximation as OdUInt32.
    */
    OdUInt32 majorApproximation() const;

    /** \details
       Gets the minor approximation.

       \returns Minor approximation as OdUInt32.
    */
    OdUInt32 minorApproximation() const;

    /** \details
       Gets the normal at the specified vertex on the specified face.

       \param atV [in] The vertex at which the normal is computed.
       \param atF [in] The face for which the normal is computed.
       \returns Normal at the specified vertex on the specified face.

       \remarks
       Parameter atF is unused.
    */
    OdGeVector3d normal(const Vertex& atV, const Face& atF) const;

    /** \details
       Transforms the surface with the given transformation matrix.

       \param x [in] Given transformation matrix.
    */
    void transform(const OdGeMatrix3d& x);

    /** \details
       Compares surfaces with the specified tolerance.

       \param pOther [in] Other surface to compare with.
       \param eps    [in] Comparison tolerance.
       \returns
       * 0 - if the surfaces are equal,
       * 1 - if a parameter of this surface is greater than the corresponding parameter of another surface,
       * -1 - otherwise.

       \remarks
       The main purpose of the method is to sort surfaces.
    */
    int compare(const Surface* pOther, double e) const;

    /** \details
       Creates a copy of the surface to the specified body.

       \param to [in] Destination body.
       \returns Pointer to the newly created surface.
    */
    Surface* copy(Body& to) const;

  protected:
    double m_majorRadius;   // Major radius value.
    double m_minorRadius;   // Minor radius value.
    OdUInt32 m_majorApprox; // Major approximation.
    OdUInt32 m_minorApprox; // Minor approximation.
  };

}

#endif //__FMMDL_SURFACE_H__
