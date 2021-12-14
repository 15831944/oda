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

#ifndef __FMCONTOUR2D_H__
#define __FMCONTOUR2D_H__

#include "FMGeometry.h"
#include "FMContoursBase.h"
#include "FMSegment2D.h"

namespace FacetModeler
{

  class IContour2D;
  class IBulgeSeg2D;
  class Profile2D;

  /** \details
     Represents the class for handling traversing events.

     \remarks
     To use the traverse method, you must create an object of the heir of this class and override all methods.
  */
  class ODRX_ABSTRACT Contour2DTraversingReactor {
  public:
    /** \details
       Handles the situation when traversing has moved to the next line segment.
      
       \param nSegment [in] The number of the current segment.
       \returns false if traversing needs to be stopped; true if the event processed successfully.
    */
    virtual bool onLineSeg(int nSegment) = 0;

    /** \details
       Handles the situation when traversing has moved to the next arc segment.
      
       \param nSegment [in] The number of the current segment.
       \param ppDev    [in] The coordinates of the current point as OdGePoint2d.
       \returns false if traversing needs to be stopped; true if the event processed successfully.
    */
    virtual bool onArcSeg(int nSegment, DeviationParams** ppDev) = 0;

    /** \details
       Handles the situation when traversing has moved to the beginning of the next arc segment.
      
       \param nSegment  [in] The number of the current segment.
       \param pt        [in] The coordinates of the current point as OdGePoint2d.
       \param nTotalPts [in] The number of points on the current arc segment.
       \returns false if traversing needs to be stopped; true if the event processed successfully.
    */
    virtual bool onArcStartPoint(int nSegment, const OdGePoint2d& pt, int nTotalPts) = 0;

    /** \details
       Handles the situation when traversing has moved to the next arc point.
      
       \param nSegment [in] The number of the current segment.
       \param pt       [in] The coordinates of the current point as OdGePoint2d.
       \returns false if traversing needs to be stopped; true if the event processed successfully.
    */
    virtual bool onArcPoint(int nSegment, const OdGePoint2d& pt) = 0;

    /** \details
       Handles the situation when traversing has moved to the end of the next arc segment.
      
       \param nSegment [in] The number of the current segment.
       \param pt       [in] The coordinates of the current point as OdGePoint2d.
       \returns false if traversing needs to be stopped; true if the event processed successfully.
    */
    virtual bool onArcEndPoint(int nSegment, const OdGePoint2d& pt) = 0;

    /** \details
       Handles the situation when traversing has moved to the next coincident segment.
      
       \param nSegment [in] The number of the current segment.
       \returns false if traversing needs to be stopped; true if the event processed successfully.
    */
    virtual bool onCoincidentSeg(int nSegment) = 0;
  };

  /** \details
     Represents facet body 2D contour objects.

     \remarks
     Contour2D may contain linear and circular segments.

     There are three ways to pick (non-vertex) points on the contour:
     a) Use an integer index to get a segment and then
        use a parameter on the segment (GeLine or GeArc).
     b) Use a distance along the curve from the starting point.
     c) Use a "parameter" value.

     The last way is the fastest (and preferred in most cases) because of
     the "parameter" structure:
       dParam = startParam() + iIndex * (dOffsetOnSegment/dSegmentLength)

     So the point given by the parameter value can be found fast consistently.
  */
  class FMGEOMETRY_API Contour2D
  {
  public:
    /** \details
       Default constructor. Creates a contour of the eSimple implementation type.
    */
    Contour2D();

    /** \details
       Creates a contour of the specified implementation type.

       \param eImplClass [in] Implementation type.
    */
    explicit Contour2D(ContourImplClass eImplClass);

    /** \details
       Creates a contour of the eSimple implementation type by the specified segment.

       \param rSrcSeg    [in] Input segment to create the contour.
       \param eImplClass [in] Optional argument representing a Contour2D implementation type of the object to create. Default value is ecicSimple.
    */
    explicit Contour2D(const IBulgeSeg2D& rSrcSeg,
      ContourImplClass eImplClass = ecicSimple);

    /** \details
       Copy constructor. Creates a contour of the eSimple implementation type and initializes it with parameters of the specified contour.

       \param rSrcCont   [in] Contour2D object to copy.
       \param eImplClass [in] Optional argument representing a Contour2D implementation type of the object to create. Default value is ecicSimple.
    */
    Contour2D(const Contour2D& rSrcCont,
      ContourImplClass eImplClass = ecicSimple);

    /** \details
       Creates a contour of the eImplClass implementation type with the specified contour implementation object.

       \param rSrcCont   [in] Contour2D implementation to initialize.
       \param eImplClass [in] Optional argument representing a Contour2D implementation type of the object to create. Default value is ecicSimple.
    */
    explicit Contour2D(const IContour2D& rSrcCont,
      ContourImplClass eImplClass = ecicSimple);

    /** \details
       Creates a rectangular contour with the specified extent points.

       \param ext [in] Rectangle as OdGeExtents2d.
    */
    Contour2D(const OdGeExtents2d& ext);

    /** \details
       Creates a copy of the specified contour.

       \param rSrcCont [in] Contour to copy.
       \returns Reference to the resulting object.

       \remarks
       The implementation type remains intact.
    */
    Contour2D& operator = (const Contour2D& rSrcCont);

    /** \details
       Creates a copy of the specified contour implementation to this object.

       \param rSrcCont [in] Contour implementation to copy.
       \returns Reference to the resulting object.
    */
    Contour2D& operator = (const IContour2D& rSrcCont);

    /** \details
       Clones the contents of the specified contour.

       \param rSrcCont [in] Contour implementation to clone.
       \returns Reference to the resulting object.

       \remarks
       The implementation type may change.
    */
    Contour2D& cloneFrom(const IContour2D& rSrcCont);

    /** \details
       Clears the contents of the contour.

       \param eImplClass [in] Implementation type.
       \returns Reference to the resulting object.

       \remarks
       By default the implementation type does not change.
       The contour is created open.
    */
    Contour2D& reset(ContourImplClass eImplClass = ecicUnknown);

    /** \details
       Gets the implementation type.

       \returns The implementation type.
    */
    ContourImplClass implClass() const;

    /** \details
       Swaps the contents of the contour with another one.

       \param rAnother [in] Contour to swap.
    */
    void swap(Contour2D& rAnother);

    /** \details
       Checks whether the contour is closed.

       \returns true if the contour is closed, or false otherwise.
    */
    bool isClosed() const;

    /** \details
       Checks whether the start and the end of the contour are equal to the specified tolerance.

       \param gTol [in] Tolerance.
       \returns true if equal, or false otherwise.
    */
    bool isEndsEqual(const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Makes the contour closed (or open).

       \param bClosed [in] New closed state to set.
    */
    void setClosed(bool bClosed = true);

    /** \details
       Checks whether the contour is empty.

       \returns true if contour has no vertices, or false otherwise.
    */
    bool isEmpty() const;

    /** \details
       Gets the number of vertices.

       \returns Number of vertices as OdUInt32.
    */
    OdUInt32 numVerts() const;

    /** \details
       Gets the number of segments.

       \returns Number of segments as OdUInt32.

       \remarks
       The number of segments is less than or equal (if the contour is closed) to the number of vertices.
    */
    OdUInt32 numSegments() const;

    /** \details
       Checks whether the contour has arcs.

       \returns true if the contour contains at least one arc, or false otherwise.
    */
    bool hasArcs() const;

    /** \details
       Gets the type of segment that begins at the specified vertex.

       \param iIndex [in] Segment index.
       \returns Type of the segment: estArc, estLine, estCoincident, or estUnknown if the index is out of range.
    */
    SegmentType segmentType(OdUInt32 iIndex) const;

    /** \details
       Gets a segment by the specified index.

       \param iIndex    [in] Segment index.
       \param rSegment [out] Found segment.
       \returns eOk if the segment is found successfully, or a corresponding error type otherwise.
    */
    Result getSegmentAt(OdUInt32 iIndex,
      Segment2D& rSegment) const;

    /** \details
       Gets parameters of the segment specified by the index.

       \param iIndex   [in] Segment index.
       \param ptStart [out] Start point as OdGePoint2d.
       \param ptEnd   [out] End point as OdGePoint2d.
       \param dBulge  [out] Bulge value.
       \returns eOk if the segment is found successfully, or a corresponding error type otherwise.
    */
    Result getSegmentAt(OdUInt32 iIndex,
      OdGePoint2d& ptStart,
      OdGePoint2d& ptEnd,
      double& dBulge) const;

    /** \details
       Gets a linear segment by the specified index.

       \param iIndex  [in] Segment index.
       \param geLine [out] Resulting line segment as OdGeLineSeg2d.
       \returns eOk if the line segment is found successfully, or a corresponding error type otherwise.
    */
    Result getLineSegAt(OdUInt32 iIndex,
      OdGeLineSeg2d& geLine) const;

    /** \details
       Gets a circular arc segment by the specified index.

       \param iIndex [in] Segment index.
       \param geArc [out] Resulting arc segment as OdGeCircArc2d.
       \returns eOk if the arc segment is found successfully, or a corresponding error type otherwise.
    */
    Result getArcSegAt(OdUInt32 iIndex,
      OdGeCircArc2d& geArc) const;

    /** \details
       Gets a point by the specified index.

       \param iIndex   [in] Vertex index.
       \param ptPoint [out] Vertex point as OdGePoint2d.
       \returns eOk if the point is found successfully, or a corresponding error type otherwise.
    */
    Result getPointAt(OdUInt32 iIndex,
      OdGePoint2d& ptPoint) const;

    /** \details
       Gets a bulge value of a segment with the specified index.

       \param iIndex  [in] Segment index.
       \param dBulge [out] Bulge value.
       \returns eOk if the bulge is found successfully, or a corresponding error type otherwise.
    */
    Result getBulgeAt(OdUInt32 iIndex,
      double& dBulge) const;

    /** \details
       Gets data of a vertex with the specified index.

       \param iIndex      [in] Vertex index.
       \param pPoint     [out] Vertex point as OdGePoint2d.
       \param pBulge     [out] (Optional) Bulge value.
       \param pMetadata  [out] (Optional) Metadata attribute.
       \param puRawFlags [out] (Optional) Flags as a raw OdUInt32 value.
       \returns eOk if the vertex is found successfully, or a corresponding error type otherwise.
    */
    Result getVertexAt(OdUInt32 iIndex,
      OdGePoint2d* pPoint, double* pBulge = NULL,
      OdIntPtr* pMetadata = NULL, OdUInt32* puRawFlags = NULL) const;

    /** \details
       Gets an internal angle of a vertex with the specified index.

       \param iIndex  [in] Vertex index.
       \param dAngle [out] Internal angle value.
       \returns eOk if the internal angle is found successfully, or a corresponding error type otherwise.
    */
    Result getInternalAngleAt(OdUInt32 iIndex, double & dAngle) const;

    /** \details
       Sets a position of a vertex with the specified index.

       \param iIndex  [in] Vertex index.
       \param ptPoint [in] Vertex point to set.
       \returns eOk if the point is set successfully, or a corresponding error type otherwise.
    */
    Result setPointAt(OdUInt32 iIndex,
      const OdGePoint2d& ptPoint);

    /** \details
       Sets a bulge value for a vertex with the specified index.

       \param iIndex [in] Vertex index.
       \param dBulge [in] Bulge value to set.
       \returns eOk if the bulge is set successfully, or a corresponding error type otherwise.
    */
    Result setBulgeAt(OdUInt32 iIndex,
      double dBulge);

    /** \details
       Sets a location and a bulge value for a vertex with the specified index.

       \param iIndex  [in] Vertex index.
       \param ptPoint [in] Vertex point to set.
       \param dBulge  [in] Vertex bulge value to set.
       \returns eOk if the location and bulge are set successfully, or a corresponding error type otherwise.
    */
    Result setVertexAt(OdUInt32 iIndex,
      const OdGePoint2d& ptPoint, double dBulge);

    /** \details
       Sets the capacity of the contour data to the specified vertex number.

       \param iReservedSize [in] Vertex number to reserve.
    */
    void reserveVertices(OdUInt32 iReservedSize);

    /** \details
       Gets metadata for the specified segment.

       \param iIndex [in] Segment index.
       \returns Metadata for the specified segment.
    */
    OdIntPtr metadataAt(OdUInt32 iIndex) const;

    /** \details
       Sets metadata for the specified segment.

       \param iIndex   [in] Segment index.
       \param iNewData [in] New metadata to set.
       \returns eOk if the metadata is set successfully, or a corresponding error type otherwise.

       \remarks
       By default the metadata value is 0.
    */
    Result setMetadataAt(OdUInt32 iIndex,
      OdIntPtr iNewData);

    /** \details
       Sets metadata for all segments.

       \param iNewData      [in] New metadata to set.
       \param iBitsToModify [in] A bit mask that indicates which bits to change.
       \returns eOk if the metadata is set successfully, or a corresponding error type otherwise.
    */
    Result setAllMetadata(OdIntPtr iNewData,
      OdIntPtr iBitsToModify = (OdIntPtr)-1);

    /** \details
       Checks whether the segment with the specified index is hidden.

       \param iIndex [in] Segment index.
       \returns true if the segment with the specified index in hidden, or false otherwise.
    */
    bool isHiddenAt(OdUInt32 iIndex) const;

    /** \details
       Sets or clears the ecfHidden flag at the specified segment.

       \param iIndex  [in] Segment index.
       \param bHidden [in] Hidden state to set.
       \returns eOk if the segment hidden state is set successfully, or a corresponding error type otherwise.
    */
    Result setHiddenAt(OdUInt32 iIndex,
      bool bHidden = true);

    /** \details
       Gets the orientation of the segment with the specified index.

       \param iIndex [in] Segment index.
       \returns The segment orientation as a FaceOrientation value.
    */
    FaceOrientation orientationAt(OdUInt32 iIndex) const;

    /** \details
       Sets the orientation of the segment with the specified index.

       \param iIndex       [in] Segment index.
       \param eOrientation [in] Orientation state to set.
       \returns eOk if the segment orientation is set successfully, or a corresponding error type otherwise.
    */
    Result setOrientationAt(OdUInt32 iIndex,
      FaceOrientation eOrientation);

    /** \details
       Gets the "smooth" flag value of the segment with the specified index.

       \param iIndex [in] Segment index.
       \returns true if the "smooth" flag is set, or false otherwise.

       \remarks
       The flag is used to create smooth surfaces when generating an extrusion or revolution body.
    */
    bool threatAsSmoothCurveAt(OdUInt32 iIndex) const;

    /** \details
       Sets the "smooth" flag value.

       \param iIndex  [in] Segment index.
       \param bSmooth [in] New value of the "smooth" flag.

       \remark
       The flag is used to create smooth surfaces when generating an extrusion or revolution body.
    */
    void setThreatAsSmoothCurveAt(OdUInt32 iIndex, bool bSmooth);

    /** \details
       Gets the ecfExplodedArc flag value at the specified segment.

       \param iIndex [in] Segment index.
       \returns true if the segment with the specified index is part of an exploded arc.
    */
    bool isExplodedArcAt(OdUInt32 iIndex) const;

    /** \details
       Sets or clears the ecfExplodedArc flag at the specified segment.

       \param iIndex       [in] Segment index.
       \param bExplodedArc [in] New flag value.
       \returns eOk if the segment flag is set successfully, or a corresponding error type otherwise.
    */
    Result setExplodedArcAt(OdUInt32 iIndex,
      bool bExplodedArc = true);

    /** \details
       Returns all built-in flags (orientation, visibility etc.) for the specified segment.

       \param iIndex [in] Segment index.
       \returns Value of the flags as OdUInt32.

       \remarks
       By default the flag value is 0.
    */
    OdUInt32 rawFlagsAt(OdUInt32 iIndex) const;

    /** \details
       Updates all built-in flags (orientation, visibility etc.) for the specified segment.

       \param iIndex    [in] Vertex index to set.
       \param iRawFlags [in] Raw flags to set as OdUInt32.
       \returns eOk if the flags are set successfully, or a corresponding error type otherwise.
    */
    Result setRawFlagsAt(OdUInt32 iIndex, OdUInt32 iRawFlags);

    /** \details
       Adds a vertex at the specified index.

       \param iIndex    [in] Vertex index to append.
       \param ptStart   [in] Vertex point to append.
       \param dBulge    [in] (Optional) Arc segment bulge. Default value is 0.0.
       \param uMetadata [in] (Optional) Metadata to set. Default value is 0.
       \param uRawFlags [in] (Optional) Raw flags to set. Default value is 0.
       \returns eOk if the vertex appended successfully, or a corresponding error type otherwise.

       \remarks
       The index must be less than or equal to the number of vertices in the contour. 
    */
    Result addVertexAt(OdUInt32 iIndex,
      const OdGePoint2d& ptStart, double dBulge = 0.0,
      OdIntPtr uMetadata = 0, OdUInt32 uRawFlags = 0);

    /** \details
       Appends a new vertex to the end of the contour.

       \param ptStart   [in] Vertex point to append.
       \param dBulge    [in] (Optional) Arc segment bulge. Default value is 0.0.
       \param uMetadata [in] (Optional) Metadata to set. Default value is 0.
       \param uRawFlags [in] (Optional) Raw flags to set. Default value is 0.
       \returns eOk if the vertex appended successfully, or a corresponding error type otherwise.

       \remarks
       The bulge value relates to the segment starting at this point.
    */
    Result appendVertex(const OdGePoint2d& ptStart, double dBulge = 0.0,
      OdIntPtr uMetadata = 0, OdUInt32 uRawFlags = 0);

    /** \details
       Appends a new vertex to the end of the contour.

       \param ptStart   [in] Vertex point to append.
       \param ptMid     [in] Middle point of bulge to calculate.
       \param uMetadata [in] Metadata pointer to set.
       \param uRawFlags [in] Raw flags to set.
       \returns eOk if the vertex appended successfully, or a corresponding error type otherwise.
    */
    Result appendVertex(const OdGePoint2d& ptStart,
      const OdGePoint2d& ptMid, OdIntPtr uMetadata = 0, OdUInt32 uRawFlags = 0);

    /** \details
       Appends vertices to the end of this contour.

       \param vecSource [in] Array of vertices to append as OdGePoint2dArray.
       \returns eOk if the vertices array appended successfully, or a corresponding error type otherwise.
    */
    Result appendVertices(const OdGePoint2dArray & vecSource);

    /** \details 
       Appends vertices to the end of this contour.
    
       \param size        [in] Number of points to add.
       \param vecSource   [in] Pointer to an array of points.
       \param bulgeSource [in] (Optional) Pointer to an array of bulge values. The array must be the same size as vecSource.
       \return eOk if the vertices array appended successfully, or a corresponding error type otherwise.
    */
    Result appendVertices(OdUInt32 size, const OdGePoint2d* vecSource, const double* bulgeSource = NULL);

    /** \details
       Appends a segment to the end of this contour.

       \param rSeg        [in] Segment to add.
       \param bShiftToHit [in] (Optional) true to enable shift correction, or false for disable. Default value is false.
       \returns
       * eOk - If the segment is appended successfully, or a corresponding error type otherwise.
       * erPointNotOnThis - If bShiftToHit is false and rSeg.startPt() is not equal to the end of this (open) contour.
       * erWarnPointNotOnThis - If bShiftToHit is true and the rSeg is shifted.
    */
    Result appendSegment(const Segment2D& rSeg,
      bool bShiftToHit = false);

    /** \details
       Appends a contour to the end of this contour.

       \param rCont     [in] Contour to append.
       \param bCloseGap [in] If true, a segment connecting the points is inserted and the return code is erWarnPointNotOnThis.
       \param dMaxGap   [in] (Optional) The maximum allowable distance between the ends of the connected contours. Default value is 1e99.
       \returns
       * eOk - If the contour is appended successfully.
       * erInvalidArgs - If rCont is equal to this contour.
       * erPointNotOnThis - If rCont.startPoint is not equal to the end of this (open) contour.
       * erWarnPointNotOnThis - If the distance from rCont.startPoint to the end of this contour is greater than FMGeGbl :: gTol.equalPoint (), but less than dMaxGap.
    */
    Result appendContour(const Contour2D& rCont,
      bool bCloseGap = false, double dMaxGap = 1e99);

    /** \details
       Removes a vertex and corresponding segment.

       \param iIndex [in] Vertex index.
       \returns eOk if the vertex is removed successfully, or a corresponding result state otherwise.

       \remarks
       Bulge values of the other segments are left intact.
    */
    Result removeVertexAt(OdUInt32 iIndex);

    /** \details
       Gets the start point of the contour.

       \param ptPoint [out] Resulting start point as OdGePoint2d.
       \returns eOk if the point is calculated, or a corresponding result state otherwise.

       \remarks
       If the contour is empty, the contour has no start point.
    */
    Result getStartPoint(OdGePoint2d& ptPoint) const;

    /** \details
       Gets the end point of the contour.

       \param ptPoint [out] Resulting end point as OdGePoint2d.
       \returns eOk if the point is calculated, or a corresponding result state otherwise.

       \remarks
       If the contour is empty, it has no end point.
    */
    Result getEndPoint(OdGePoint2d & ptPoint) const;

    /** \details
       Gets a tangent to the contour at the specified parameter.

       \param dParam    [in] Parameter to calculate the tangent at.
       \param vTangent [out] Resulting tangent vector as OdGeVector2d.
       \returns eOk if the tangent is calculated, or a corresponding result state otherwise.
    */
    Result getTangent(double dParam,
      OdGeVector2d& vTangent) const;

    /** \details
       Gets a tangent at the distance along the contour starting from the start point.

       \param dDist     [in] Distance.
       \param vTangent [out] Resulting tangent vector as OdGeVector2d.
       \returns eOk if the tangent is calculated, or a corresponding result state otherwise.
    */
    Result getTangentAtDist(double dDist,
      OdGeVector2d& vTangent) const;

    /** \details
       Gets a point on the contour at the specified parameter.

       \param dParam   [in] Parameter.
       \param ptPoint [out] Resulting point.
       \returns eOk if the point is calculated, or a corresponding result state otherwise.
    */
    Result getPoint(double dParam,
      OdGePoint2d& ptPoint) const;

    /** \details
       Gets a point on the contour at the specified distance.

       \param dDist    [in] Distance.
       \param ptPoint [out] Resulting point.
       \returns eOk if the point is calculated, or a corresponding result state otherwise.
    */
    Result getPointAtDist(double dDist,
      OdGePoint2d& ptPoint) const;

    /** \details
       Gets a distance from the start point to the specified parameter.

       \param dParam [in] Parameter.
       \param dDist [out] Resulting distance.
       \returns Distance to the parameter.
    */
    Result getDistAtParam(double dParam,
      double& dDist) const;

    /** \details
       Gets a parameter at the specified distance from the start point.

       \param dDist   [in] Distance.
       \param dParam [out] Resulting param.
       \returns Parameter at the distance.
    */
    Result getParamAtDist(double dDist,
      double& dParam) const;

    /** \details
       Converts a parameter to the [start param, end param] range.

       \param dParam [in/out] Parameter to normalize.
       \returns eOk if dParam is valid, or a corresponding error type otherwise.

       \remarks
       Works for closed contours.
    */
    Result getNormalizedParam(double& dParam) const;

    /** \details
       Converts a distance to the [0, length] range.

       \param dDist [in/out] Distance to normalize.
       \returns eOk if dParam is valid, or a corresponding error type otherwise.

       \remarks
       Works for closed contours.
    */
    Result getNormalizedDist(double& dDist) const;

    /** \details
       Checks whether the specified parameters are equal depending on the closed state of the contour.

       \param dParam1   [in] First parameter.
       \param dParam2   [in] Second parameter.
       \param dParamTol [in] Tolerance.
       \returns true if the parameters are valid and equal (after normalization), or false otherwise.
    */
    bool areEqualParams(double dParam1,
      double dParam2, double dParamTol = 1e-10) const;

    /** \details
       Checks whether the specified distances are equal depending on the closed state of the contour.

       \param dDist1 [in] First distance.
       \param dDist2 [in] Second distance.
       \param dTol   [in] Tolerance.
       \returns true if the distances are equal (after normalization), or false otherwise.

       \remarks
       The result is useless unless both distances are valid.
    */
    bool areEqualDists(double dDist1,
      double dDist2, double dTol = 1e-6/*FMGeGbl::gTol.equalPoint()*/) const;

    /** \details
       Appends the extents of the contour to the specified extents structure.

       \param extExtents [in/out] Extents to add to.
       \returns Reference to this object.
    */
    Result addExtents(OdGeExtents2d& extExtents) const;

    /** \details
       Gets the start contour parameter.

       \returns Start contour parameter value.
    */
    inline double startParam() const { return 0.0; };

    /** \details
       Gets the end contour parameter.

       \returns End contour parameter value.
    */
    double endParam() const;

    // Returns true and a period value, if the contour is closed
    // Returns false otherwise
    //bool getPeriodParam( double & dPeriod ) const;

    /** \details
       Gets the length of the contour.

       \returns Contour length.
    */
    double length() const;

    /** \details
       Gets the non-negative area of the contour.

       \returns Non-negative area of the contour.

       \remarks
       For open contours, the first and last vertices are considered
       to be connected with a straight line.
    */
    double area() const;

    /** \details
       Gets the signed area of the contour.

       \returns Signed area of the contour
    */
    double signedArea() const;

    /** \details
       Checks whether the contour is convex.

       \returns true if the contour is convex, or false otherwise.
    */
    bool isConvex() const;

    /** \details
       Gets the orientation of the contour.

       \returns true if the contour has counter-clockwise orientation, or false otherwise.

       \remarks
       The area of the contour is positive for a counter-clockwise contour
       and negative for a clockwise contour.
    */
    bool isCCW() const;

    /** \details
       Makes the contour counter-clockwise or clockwise.

       \param bCCW [in] true for counter-clockwise, or false for clockwise.
    */
    void makeCCW(bool bCCW = true);

    /** \details
       Checks whether the 2D point is located on the contour with the given tolerance. 
       If the point is on the contour, the method calculates the parameter value at this point.

       \param ptTest  [in] Point to check.
       \param pParam [out] (Optional) The contour's resulting parameter at the point ptTest.
       \param gTol    [in] Tolerance.

       \returns true if the point lies on the contour with the specified tolerance, or false otherwise.
    */
    bool isOn(const OdGePoint2d& ptTest, double* pParam = NULL,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Gets the nearest parameter and (optional) point on the contour for the given 2D point.

       \param ptTest     [in] Point to check.
       \param ptNearest [out] (Optional) The nearest point on the contour for the given point ptTest.

       \returns Nearest parameter on contour.
    */
    double nearestParam(const OdGePoint2d& ptTest,
      OdGePoint2d* ptNearest = NULL) const;

    /** \details
       Checks whether a point is inside the contour, at its boundary, or outside.

       \param ptPoint    [in] Point to check.
       \param pOnBorder [out] (Optional) true if the point is located on the contour border.
       \param gTol       [in] Tolerance.

       \returns true if the point lies inside the contour with the specified tolerance, or false otherwise.
    */
    bool contains(const OdGePoint2d& ptPoint, bool* pOnBorder = NULL,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Checks whether the contour intersects itself.

       \param gTol          [in] Tolerance.
       \param bExcludeTouch [in] true for not considering a touch as self-intersection, or false otherwise.

       \returns
       true if and only if there exists a pair of intersecting segments
       and the shortest distance along the contour between the intersections is
       greater than the tolerance, or false otherwise.

       \remarks
       Runs in O( N * log(N) ) time at the average.

       E.g. The method returns true for an 8-shaped contour regardless of
            the central intersection type (touch or crossing).
       The method returns false for a O-shaped closed contour,
            but returns true for an open one.
    */
    bool isSelfIntersecting(const OdGeTol& gTol = FMGeGbl::gTol, bool bExcludeTouch = false) const;

    /** \details
       Checks whether this contour is closed and coincides with a directional
       boundary of an open 2D region.

       \param gTol [in] Tolerance.
       \returns
       * false for empty and non-closed contours.
       * false if there are coincident bounds with opposite directions (e.g., O-O shape).
       * false if there are sub-contours with incorrect orientation (e.g., 8 shape).
       * true otherwise.
    */
    bool isValidRegion(const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Checks whether this contour is inside the specified contour.

       \param c2dOuter           [in] Probable outer contour.
       \param fAllowBordersTouch [in] true to allow the contour to touch and overlap, or false to disallow.
       \param gTol               [in] Tolerance.
       \returns true if this contour is located inside the specified countour, false - otherwise.
    */
    bool isInsideContour(const Contour2D& c2dOuter,
      bool fAllowBordersTouch = false,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Calculates all intersections with the specified segment and appends the result to vecPoints.

       \param rSegB      [in] Segment this contour is intersected with.
       \param vecPoints [out] Intersection container the result is appended to.
       \param gTol       [in] Tolerance.

       \returns The number of intersections.
    */
    OdUInt32 intersect(const Segment2D& rSegB,
      std::vector< Intersection >& vecPoints,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Calculates all intersections with the specified contour and appends the result to vecPoints.

       \param rContB     [in] Other contour this contour is intersected with.
       \param vecPoints [out] Intersection container the result is appended to.
       \param gTol       [in] Tolerance.

       \returns The number of intersections.
    */
    OdUInt32 intersect(const Contour2D& rContB,
      std::vector< Intersection >& vecPoints,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Calculates all intersections with the specified line and appends the result to vecPoints.

       \param rLine      [in] Line this contour is intersected with.
       \param vecPoints [out] Intersection container the result is appended to.
       \param gTol       [in] Tolerance.
       \returns The number of intersections.
    */
    OdUInt32 intersectLine(const OdGeLine2d& rLine,
      std::vector< Intersection >& vecPoints,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Transforms the contour.

       \param mMatrix      [in] Transformation matrix.
       \param devDeviation [in] Deviation parameters.
       \returns eOk if the contour is transformed successfully, or a corresponding error type otherwise.
    */
    Result transformBy(const OdGeMatrix2d& mMatrix,
      const DeviationParams& devDeviation = FMGeGbl::gDefDev);

    /** \details
       Reverses the contour.
    */
    void reverse();

    /** \details
       Removes redundant segments with zero length.

       \param gTol [in] Tolerance.
    */
    void deleteCoincident(const OdGeTol& gTol = FMGeGbl::gTol);

    /** \details
       Merges adjacent segments.

       \param iMergeFlags [in] Combination of AECGe::FilletType flags (emfMergeArcs,emfIgnoreMetadata,emfMergeOrigin).
       \param gTol        [in] Tolerance.
    */
    void mergeSegments(int iMergeFlags = 0, const OdGeTol & gTol = FMGeGbl::gTol);

    /** \details
       Explodes all arc segments to lines.
       Sets the uArcMetadata flag on exploded segments.

       \param devDeviation [in] Deviation parameters.
       \param uArcMetadata [in] Metadata to set.
       \returns eOk if the contour is exploded successfully, or a corresponding error type otherwise.
    */
    Result explode(const DeviationParams& devDeviation = FMGeGbl::gDefDev, OdIntPtr uArcMetadata = 0);

    /** \details
       Explodes all arc segments to lines.
       Sets the uArcMetadata flag on exploded segments.

       \param rDestCont   [out] Destination contour for exploded lines.
       \param devDeviation [in] Deviation parameters.
       \param uArcMetadata [in] Metadata to set.
       \returns eOk if the contour is exploded successfully, or a corresponding error type otherwise.
    */
    Result explodeTo(Contour2D& rDestCont,
      const DeviationParams& devDeviation = FMGeGbl::gDefDev, OdIntPtr uArcMetadata = 0) const;

    /** \details
       Adds a vertex to the contour at the specified parameter value.

       \param dParam [in] Parameter to create a vertex.
       \param gTol   [in] Tolerance.
       \returns Index of created (or found) vertex.

       \remarks
       If a vertex at the specified parameter exists, the method does nothing.

       Insertion or removal of a vertex at dParam invalidates all parameters greater than floor(dParam).
    */
    OdUInt32 createVertexAt(double dParam,
      const OdGeTol& gTol = FMGeGbl::gTol);

    /** \details
       Adds a vertex to the contour at the specified distance.

       \param dDist [in] Distance on the contour to add the vertex at.
       \param gTol  [in] Tolerance.

       \returns Index of the created (or found) vertex.

       \remarks
       If a vertex at the specified parameter exists, the method does nothing.
    */
    OdUInt32 createVertexAtDist(double dDist,
      const OdGeTol & gTol = FMGeGbl::gTol);

    /** \details
       Adds vertices at the specified parameters into the contour.

       \param vecParams [in] Array of parameters.
       \param gTol      [in] Tolerance.
       \returns eOk if the vertices are added successfully, or a corresponding error type otherwise.

       \remarks
       If some vertices already exist, this method doesn't create them.
    */
    Result createVerticesAt(const std::vector<double> & vecParams,
      const OdGeTol& gTol = FMGeGbl::gTol);

    /** \details 
       Adds vertices at the specified parameters into the contour.
    
       \param size      [in] The number of elements in the parameter array.
       \param vecParams [in] Pointer to an array of parameters.
       \param gTol      [in] Tolerance.
       \return eOk if the vertices are added successfully, or a corresponding error type otherwise.

       \remarks
       If some vertices already exist, this method doesn't create them.
    */
    Result createVerticesAt(OdUInt32 size, const double* vecParams,
      const OdGeTol& gTol = FMGeGbl::gTol);

    /** \details
       Gets a sub-contour given by two parameters.

       \param dStartParam  [in] The lower bound on which to create a sub-contour.
       \param dEndParam    [in] The upper bound on which to create a sub-contour.
       \param rSubContour [out] Resulting sub-contour.
       \param gTol         [in] Tolerance.
       \returns eOk if the sub-contour is computed successfully, or a corresponding error type otherwise.

       \remarks
       If (dStartParam > dEndParam), the rSubContour has the opposite direction.
    */
    Result getSubContour(double dStartParam, double dEndParam,
      Contour2D & rSubContour, const OdGeTol & gTol = FMGeGbl::gTol) const;

    /** \details
       Gets the profile that is shifted in the normal direction from this closed contour.
       E.g., if dOffset is negative, a counter-clockwise circle is extended.
       If dOffset is positive and small enough, it is contracted,
       and becomes empty if dOffset > radius.

       \param dOffset     [in] Signed offset along the normal direction. A positive offset is towards the inside for CCW contours.
       \param rResult    [out] Resulting profile.
       \param eFilletType [in] Gap filling strategy for the segments sliding apart.
       \param gTol        [in] Tolerance.
       \returns eOk if the profile is computed successfully, or a corresponding error type otherwise.
    */
    Result getOffsetProfile(double dOffset, Profile2D& rResult,
      FilletType eFilletType = eftExtend,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Gets the profile that is shifted in the normal direction from this closed contour.

       \param adOffset [in] Array of the signed offset along the normal direction. A positive offset is inside for CCW contours.
       \param rResult [out] Resulting profile.
       \param gTol     [in] Tolerance.
       \returns eOk if the profile is computed successfully, or a corresponding error type otherwise.

       \sa
       getOffsetProfile
    */
    Result getProfileByOffsets(const std::vector< double >& adOffset,
      Profile2D& rResult,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Traverses the contour.

       \param reactor [in/out] Contour2DTraversingReactor object to traverse.
       \returns false if one of the reactor methods returns false, or true if traversing is successful.
    */
    bool traverse(Contour2DTraversingReactor& reactor) const;

    /** \details
       Gets the geometry extents.

       \param extens [out] Resulting extents as OdGeExtents2d.
    */
    void getGeomExtens(OdGeExtents2d& extens);

    /** \details
      Creates Contour2D as full circle with center point and radius specified.

      \param center [in] Coordinates of center.
      \param radius [in] Radius value.
      \returns Circular contour as Contour2D.
    */
    static Contour2D createCircle(const OdGePoint2d &center, double radius);

    /** \details
      Creates Contour2D as square with center point and side length specified.

      \param center [in] Coordinates of center.
      \param side [in] Side length value.
      \returns Square contour as Contour2D.
    */
    static Contour2D createSquare(const OdGePoint2d &center, double side);

    /** \details
       An empty static contour.
    */
    FMGEOMETRY_API_STATIC static const Contour2D kNull;

    /** \details 
      Destructor.
    */
    ~Contour2D();

  public:
    //DOM-IGNORE-BEGIN
    inline operator const IContour2D& () const { return *m_pImpl; }

    inline operator IContour2D& () { return *m_pImpl; }

    inline const IContour2D & impl() const { return *m_pImpl; }

    inline IContour2D & impl() { return *m_pImpl; }
    //DOM-IGNORE-END
  private:
    IContour2D* m_pImpl; // Pointer to the implementation object.
  };

};

#endif  //__FMCONTOUR2D_H__
