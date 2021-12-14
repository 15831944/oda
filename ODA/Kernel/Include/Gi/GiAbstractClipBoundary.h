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

#ifndef _GI_ABSTRACT_CLIP_BOUNDARY_H_INCLUDED_
#define _GI_ABSTRACT_CLIP_BOUNDARY_H_INCLUDED_

#include "TD_PackPush.h"

#include "GiClipBoundary.h"
#include "GiSectionGeometryOutput.h"
#include "IntArray.h"

/** \details
    Base class for pass additional clip boundary information from OdGiDrawable objects to the underlying OdGi system.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
*/
class OdGiAbstractClipBoundary
{
  public:
    /** \details
    Represents clip boundary types.
    */
    enum BoundaryType
    {
      kNormal = 0, // Default clip boundary (not pass additional data, clip outside contour)
      kInverted,   // Inverted clip boundary (pass inverted clip array, clip inside contour)
      kExtended,   // Extended clip boundary (pass clipping contours, clip by sorted contours array)
      kComplex,    // Complex clip boundary (extended clip boundary which requires additional preprocessing)
      kPlanar      // Planar clip boundary (clipping by set of 3d planes with ability to output geometry sections)
    };
  public:
    ODRX_HEAP_OPERATORS();

    /** \details
    Default constructor for the OdGiAbstractClipBoundary class.
    */
    OdGiAbstractClipBoundary() { }
    
    /** \details
    Destructor for the OdGiAbstractClipBoundary class.
    */
    virtual ~OdGiAbstractClipBoundary() { }

    /** \details
        Returns type of the inherited boundary.
      
      \returns
      boundary type as a OdGiAbstractClipBoundary::BoundaryType value which is the OdGiAbstractClipBoundary::kNormal for this object.
    */
    virtual BoundaryType type() const { return kNormal; }

    /** \details
        Returns cloned object.
        
      \returns
      pointer to a new OdGiAbstractClipBoundary instance.
    */
    virtual OdGiAbstractClipBoundary *clone() const { return new OdGiAbstractClipBoundary(); }
};

/** \details
    Class to pass inverted clip boundary information from OdGiDrawable objects to the underlying OdGi system.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
*/
class OdGiInvertedClipBoundary : public OdGiAbstractClipBoundary
{
  protected:
    OdGePoint2dArray m_Points; // Array of points that defines the inverted clip boundary.
  public:
    /** \details
        Default constructor for the OdGiInvertedClipBoundary class.
    */
    OdGiInvertedClipBoundary() { }
    
    /** \details
        Destructor for the OdGiInvertedClipBoundary class.
    */
    ~OdGiInvertedClipBoundary() { }

    /** \details
        Returns type of the inherited boundary.
        
      \returns
      boundary type as a OdGiAbstractClipBoundary::BoundaryType value which is the OdGiAbstractClipBoundary::kInverted for this object.
    */
    BoundaryType type() const { return kInverted; }

    /** \details
        Returns inverted clipping boundary.
        
      \returns
      array of clipping points of the inverted clipping boundary as the OdGePoint2dArray object.
    */
    const OdGePoint2dArray &invertedClipBoundary() const { return m_Points; }
    /** \details
        Setup inverted clipping boundary.
        \param pPoints [in]  Input points array.
    */
    void setInvertedClipBoundary(const OdGePoint2dArray &pPoints) { m_Points = pPoints; }

    /** \details
        Returns cloned object.
        
      \returns
      pointer to the cloned OdGiInvertedClipBoundary object.
    */
    OdGiAbstractClipBoundary *clone() const {
      OdGiInvertedClipBoundary *pNew = new OdGiInvertedClipBoundary();
      pNew->setInvertedClipBoundary(invertedClipBoundary());
      return pNew;
    }
};

/** \details
    Class to pass extended clip boundary information from OdGiDrawable objects to the underlying OdGi system.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
*/
class OdGiExtendedClipBoundary : public OdGiAbstractClipBoundary
{
  protected:
    OdIntArray       m_Counts; // Array of contour points counts.
    OdGePoint2dArray m_Points; // Array of points that defines the extended clip boundary contours.
  public:
    /** \details
        Default constructor for the OdGiExtendedClipBoundary class.
    */
    OdGiExtendedClipBoundary() { }
    
    /** \details
        Destructor for the OdGiExtendedClipBoundary class.
    */
    ~OdGiExtendedClipBoundary() { }

    /** \details
        Returns type of the inherited boundary.
        
      \returns
      boundary type as a OdGiAbstractClipBoundary::BoundaryType value which is the OdGiAbstractClipBoundary::kExtended for this object.
    */
    BoundaryType type() const { return kExtended; }

    /** \details
        Returns extended clipping boundary contour vertices counts.
        
      \returns
      array of contour points counts.
    */
    const OdIntArray &clipBoundaryCounts() const { return m_Counts; }
    /** \details
        Setup extended boundary contour vertices counts.
        \param pCounts [in]  Input counts array.
    */
    void setClipBoundaryCounts(const OdIntArray &pCounts) { m_Counts = pCounts; }

    /** \details
        Returns extended clipping boundary points.
        
      \returns
      array of points that define the extended clip boundary contours.
    */
    const OdGePoint2dArray &clipBoundaryPoints() const { return m_Points; }
    /** \details
        Setup extended clipping boundary points.
        \param pPoints [in]  Input points array.
    */
    void setClipBoundaryPoints(const OdGePoint2dArray &pPoints) { m_Points = pPoints; }

    /** \details
        Returns cloned object.
        
      \returns
      pointer to the cloned OdGiExtendedClipBoundary object.
    */
    OdGiAbstractClipBoundary *clone() const { return copyExtendedData(new OdGiExtendedClipBoundary()); }
  protected:
    OdGiAbstractClipBoundary *copyExtendedData(OdGiExtendedClipBoundary *pNew) const {
      pNew->setClipBoundaryCounts(clipBoundaryCounts());
      pNew->setClipBoundaryPoints(clipBoundaryPoints());
      return pNew;
    }
};

/** \details
    Class to pass extended clip boundary information which requires preprocessing from OdGiDrawable objects to the underlying OdGi system.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
*/
class OdGiComplexClipBoundary : public OdGiExtendedClipBoundary
{
  public:
    /** \details
        Default constructor for the OdGiComplexClipBoundary class.
    */
    OdGiComplexClipBoundary() { }
    
    /** \details
        Destructor for the OdGiComplexClipBoundary class.
    */
    ~OdGiComplexClipBoundary() { }

    /** \details
        Returns type of the inherited boundary.
    */
    BoundaryType type() const { return kComplex; }

    /** \details
        Returns cloned object.
        
      \returns
      pointer to the cloned OdGiComplexClipBoundary object.
    */
    OdGiAbstractClipBoundary *clone() const { return copyExtendedData(new OdGiComplexClipBoundary()); }
};

/** \details
    Class to pass planar clip boundary information from OdGiDrawable objects to the underlying OdGi system.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
    \sa
    <link tv_planarClip, Work with Planar Clipping>
*/
class OdGiPlanarClipBoundary : public OdGiAbstractClipBoundary
{
  public:
    /** \details
      Describes a clipping plane.
    */
    struct ClipPlane
    {
      /**Plane's origin point.*/
      OdGePoint3d m_origin;
      /**Plane's normal vector.*/
      OdGeVector3d m_normal;

      /** \details
        Default constructor for the ClipPlane struct.
      */
      ClipPlane() {}
      
      /** \details
        Constructor for the ClipPlane struct. Creates structure with the specified origin point and normal to the clipping plane.
        
        \param origin [in]  Origin point.
        \param normal [in]  Normal to the clipping plane.
      */
      ClipPlane(const OdGePoint3d &origin, const OdGeVector3d &normal)
        : m_origin(origin), m_normal(normal) {}
      
      /** \details
        Sets origin point for this clipping plane.
        
        \param origin [in]  Origin point.
        
        \returns
        reference to a clipping plane.
      */
      ClipPlane &setOrigin(const OdGePoint3d &origin) { m_origin = origin; return *this; }
      
      /** \details
        Retrieves the origin point of this clipping plane.

        \returns
        origin point.
      */
      const OdGePoint3d &origin() const { return m_origin; }
      
      /** \details
        Sets normal vector for this clipping plane.
        
        \param normal [in]  Normal to this clipping plane.
        
        \returns
        reference to a clipping plane.
      */
      ClipPlane &setNormal(const OdGeVector3d &normal) { m_normal = normal; return *this; }
      
      /** \details
        Retrieves the normal to this clipping plane.

        \returns
        normal to this clipping plane.
      */
      const OdGeVector3d &normal() const { return m_normal; }
    };
    /** \details
      Defines the type of array of clipping planes.
    */
    typedef OdArray<ClipPlane> ClipPlaneArray;
  protected:
    ClipPlaneArray                       m_ClipPlanes;     // Array of clipping planes.
    mutable OdGiSectionGeometryOutputPtr m_pSectionOutput; // Optional geometry sections output.
    mutable OdGiCuttedGeometryOutputPtr  m_pCuttedOutput;  // Optional cutted geometry output.
  public:
    /** \details
        Default constructor for the OdGiPlanarClipBoundary class.
    */
    OdGiPlanarClipBoundary() { }
    
    /** \details
        Destructor for the OdGiPlanarClipBoundary class.
    */
    ~OdGiPlanarClipBoundary() { }

    /** \details
        Returns type of the inherited boundary.
        
      \returns
      boundary type as a OdGiAbstractClipBoundary::BoundaryType value which is the OdGiAbstractClipBoundary::kPlanar for this object.
    */
    BoundaryType type() const { return kPlanar; }

    /** \details
        Returns array of clipping planes.
        
      \returns
      array that may contain multiple clipping planes.
    */
    const ClipPlaneArray &clipPlanes() const { return m_ClipPlanes; }
    /** \details
        Setup array of clipping planes.
        \param pClipPlanes [in]  Input array of clipping planes.
    */
    void setClipPlanes(const ClipPlaneArray &pClipPlanes) { m_ClipPlanes = pClipPlanes; }

    /** \details
        Returns optional clipping section geometry output.
        
      \returns
      optional geometry sections output.
    */
    OdGiSectionGeometryOutput *sectionGeometryOutput() const { return m_pSectionOutput; }
    /** \details
        Setup optional clipping section geometry output.
        \param pSectionOutput [in]  Input pointer onto section geometry output.
    */
    void setSectionGeometryOutput(OdGiSectionGeometryOutput *pSectionOutput) { m_pSectionOutput = pSectionOutput; }

    /** \details
        Returns optional cutted geometry output.
        
      \returns
      optional cut geometry output.
    */
    OdGiCuttedGeometryOutput *cuttedGeometryOutput() const { return m_pCuttedOutput; }
    /** \details
        Setup optional cutted geometry output.
        \param pCuttedOutput [in]  Input pointer onto cutted geometry output.
    */
    void setCuttedGeometryOutput(OdGiCuttedGeometryOutput *pCuttedOutput) { m_pCuttedOutput = pCuttedOutput; }

    /** \details
        Returns cloned object.
        
      \returns
      pointer to a new OdGiAbstractClipBoundary instance.
    */
    OdGiAbstractClipBoundary *clone() const {
      OdGiPlanarClipBoundary *pNew = new OdGiPlanarClipBoundary();
      pNew->setClipPlanes(clipPlanes());
      pNew->setSectionGeometryOutput(sectionGeometryOutput());
      pNew->setCuttedGeometryOutput(cuttedGeometryOutput());
      return pNew;
    }
};

/** \details
    Container for OdGiClipBoundary and OdGiAbstractClipBoundary.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
struct OdGiClipBoundaryWithAbstractData : public OdGiClipBoundary
{
  /** \details
    Abstract data.
  */
  OdGiAbstractClipBoundary *m_pAbstractData;

  /** \details
    Default constructor for the OdGiClipBoundaryWithAbstractData class. Creates an object with no underlying abstract data.
  */
  OdGiClipBoundaryWithAbstractData() : m_pAbstractData(NULL) { }
  
  /** \details
    Constructor for the OdGiClipBoundaryWithAbstractData class. Creates an object with specified clipping boundary and abstract data.
    
    \param pBoundary [in]  Clipping boundary.
    \param pAbsData [in]  Abstract data.
  */
  OdGiClipBoundaryWithAbstractData(const OdGiClipBoundary &pBoundary, const OdGiAbstractClipBoundary *pAbsData = NULL)
    : OdGiClipBoundary(pBoundary), m_pAbstractData(NULL)
  {
    if (pAbsData) m_pAbstractData = pAbsData->clone();
  }
  
  /** \details
    Constructor for the OdGiClipBoundaryWithAbstractData class. Creates an object with specified clipping boundary and no abstract data.
    
    \param pBoundary [in]  Clipping boundary.
  */
  OdGiClipBoundaryWithAbstractData(const OdGiClipBoundaryWithAbstractData &pBoundary)
    : OdGiClipBoundary(pBoundary), m_pAbstractData(NULL)
  {
    if (pBoundary.m_pAbstractData) m_pAbstractData = pBoundary.m_pAbstractData->clone();
  }
  
  /** \details
    Destructor for the OdGiClipBoundaryWithAbstractData class.
  */
  ~OdGiClipBoundaryWithAbstractData() {
    if (m_pAbstractData) delete m_pAbstractData;
  }
};

#include "TD_PackPop.h"

#endif // _GI_ABSTRACT_CLIP_BOUNDARY_H_INCLUDED_
