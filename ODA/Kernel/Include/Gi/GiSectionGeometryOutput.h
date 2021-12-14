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

#ifndef __ODGISECTIONGEOMETRYOUTPUT_H__
#define __ODGISECTIONGEOMETRYOUTPUT_H__

#include "Gi/GiConveyorPassingImpl.h"
#include "Gi/GiDummyGeometry.h"

#include "TD_PackPush.h"

// Forward declarations
class OdGiClippedGeometryOutput;

/** \details
    This class provides default callback functionality for OdGiClippedGeometryOutput interface.

    <group OdGi_Classes>
    \sa
    <link gi_geometry_sectioning_callback, Work with Geometry Sectioning Callback>
*/
class ODGI_EXPORT OdGiClippedGeometryOutputCallback : public OdRxObject
{
  public:
    /** \details
      Default constructor for the OdGiClippedGeometryOutputCallback class.
    */
    OdGiClippedGeometryOutputCallback() { }

    /** details
        This callback method will be called for each sectionable object.
        \param pGeomOutput [in]  Current geometry output for reconfiguration.
        \param pDrawContext [in]  Pointer to the drawing context.
        \returns
        true to process upcoming geometry or false to disable geometry processing output.
    */
    virtual bool clippedGeometryOutputCallbackProc(OdGiClippedGeometryOutput &pGeomOutput, const OdGiConveyorContext &pDrawContext) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiClippedGeometryOutputCallback object pointers.
*/
typedef OdSmartPtr<OdGiClippedGeometryOutputCallback> OdGiClippedGeometryOutputCallbackPtr;

/** \details
    This class is an default implementation of OdGiConveyorGeometry that provides generic output of clipped geometry.

    <group OdGi_Classes>
    \sa
    <link gi_geometry_sectioning_callback, Work with Geometry Sectioning Callback>
*/
class ODGI_EXPORT OdGiClippedGeometryOutput : public OdRxObject, public OdGiConveyorPassingImpl<OdGiClippedGeometryOutput>
{
  public:
    /** \details
        This class provides data for OdGiClippedGeometryOutput and inherited classes.

        <group OdGi_Classes>
    */
    struct ODGI_EXPORT ClippedGeometryOutputInterface
    {
      /** \details
          Returns destination geometry interface.
          \returns
          destination geometry interface as OdGiConveyorGeometry instance.
      */
      virtual OdGiConveyorGeometry &destinationGeometry() = 0;
      /** \details
          Returns conveyor context interface.
          \returns
          a reference to the conveyor context interface.
      */
      virtual OdGiConveyorContext &conveyorContext() = 0;
    };
  protected:
    enum Flags
    {
      kProcessingEnabled          = 1,
      kNonSectionableGeometryClip = 2,

      kLastGenericFlag = kNonSectionableGeometryClip
    };
  protected:
    ClippedGeometryOutputInterface      *m_pIface;        // Conveyor geometry output interface.
    OdGiClippedGeometryOutputCallbackPtr m_pDrawCallback; // Optional drawable callback.
    OdUInt32                             m_flags;         // Generic flags storage.
    OdUInt32                             m_overrideFlags; // Traits override flags storage.
    OdGiSubEntityTraitsData              m_overrides;     // Traits override values.
    OdGiSubEntityTraitsData              m_traits;        // Actual traits storage.
  public:
    /** \details
      Retrieves the destination geometry.
      \returns
      a reference to the destination geometry or throws an error if there is no associated conveyor geometry output interface or it is NULL.
    */
    OdGiConveyorGeometry& destGeometry() { if (!m_pIface) throw OdError(eInvalidContext); return m_pIface->destinationGeometry(); }
    
    /** \details
      Retrieves the drawing context.
      \returns
      a reference to the drawing context or throws an error if there is no associated conveyor geometry output interface or it is NULL.
    */
    OdGiConveyorContext& drawContext() { if (!m_pIface) throw OdError(eInvalidContext); return m_pIface->conveyorContext(); }
  protected:
    class WrapTraitsProc
    { OdGiClippedGeometryOutput &m_this; OdGiConveyorContext &m_drawContext;
      public: WrapTraitsProc(OdGiClippedGeometryOutput *pThis) : m_this(*pThis), m_drawContext(pThis->drawContext())
                        { m_this.setupTraits(m_drawContext); }
      ~WrapTraitsProc() { m_this.releaseTraits(m_drawContext); }
    };
  public:
    /** \details
      Default constructor for the OdGiClippedGeometryOutput class. Sets the associated clipped geometry output interface to NULL,
      sets kProcessingEnabled and kNonSectionableGeometryClip flags.
    */
    OdGiClippedGeometryOutput() : m_pIface(NULL), m_flags(kProcessingEnabled | kNonSectionableGeometryClip), m_overrideFlags(0) {}

    /** details
        Copies the contents of the specified object into this object.
        \param pSource [in]  Pointer to the object from which to copy the data.
        /remarks
        If you create nested class, based onto OdGiClippedGeometryOutput, you should implement this method override,
        elsewhere it will not support multithreaded regeneration.
    */
    virtual void copyFrom(const OdRxObject* pSource)
    {
      const OdGiClippedGeometryOutput *pFrom = static_cast<const OdGiClippedGeometryOutput*>(pSource);
      if (!pFrom)
        throw OdError(eInvalidInput);
      m_pDrawCallback = pFrom->m_pDrawCallback;
      m_flags = pFrom->m_flags;
      m_overrideFlags = pFrom->m_overrideFlags;
      m_overrides = pFrom->m_overrides;
    }

    /** \details
        Retrieves conveyor geometry output interface.
        \returns
        pointer to the ClippedGeometryOutputInterface that represent geometry output interface for this object.
    */
    ClippedGeometryOutputInterface *getInterface() { return m_pIface; }
    /** \details
        Sets conveyor geometry output interface.
        \param pInterface [in]  Pointer onto conveyor geometry output interface.
        \remarks
        Called internally by OdGiOrthoClipperEx conveyor node.
    */
    void setInterface(ClippedGeometryOutputInterface *pInterface) { m_pIface = pInterface; }

    /** \details
        Retrieves geometry output callback interface.
        \returns
        pointer to the output geometry callback interface.
        \remarks
        Called internally by OdGiOrthoClipperEx conveyor node.
    */
    OdGiClippedGeometryOutputCallback *geometryProcessingCallback() { return m_pDrawCallback; }
    /** \details
        Setup geometry output callback interface.
        \param pCallback [in]  Geometry output callback interface pointer.
    */
    void setGeometryProcessingCallback(OdGiClippedGeometryOutputCallback *pCallback) { m_pDrawCallback = pCallback; }

    /** \details
        Checks whether processing of this geometry output is enabled.
        \returns true if processing of this geometry output is enabled, false otherwise.
    */
    bool isProcessingEnabled() const { return GETBIT(m_flags, kProcessingEnabled); }
    /** \details
        Enables or disables processing of this geometry output.
        \param bSet [in]  New processing state for this geometry output.
    */
    void setProcessingEnabled(bool bSet) { SETBIT(m_flags, kProcessingEnabled, bSet); }

    /** \details
        Checks whether clipping of non-sectionable geometry is enabled.
        \returns 
        true if clipping of non-sectionable geometry enabled, false otherwise.
    */
    bool isNonSectionableGeometryClipping() const { return GETBIT(m_flags, kNonSectionableGeometryClip); }
    /** \details
        Enables or disables clipping of non-sectionable geometry.
        \param bSet [in]  New state of non-sectionable geometry clipping.
    */
    void setNonSectionableGeometryClipping(bool bSet) { SETBIT(m_flags, kNonSectionableGeometryClip, bSet); }

    /** \details
        Setup traits override flags.
        \param nFlags [in]  Traits override flags.
    */
    void setTraitsOverrideFlags(OdUInt32 nFlags)
    {
      m_overrideFlags = nFlags;
    }
    /** \details
        Retrieves traits override flags.
        \returns
        a set of traits override flags as OdUInt32 value.
    */
    OdUInt32 traitsOverrideFlags() const
    {
      return m_overrideFlags;
    }

    /** \details
        Setup traits overrides.
        \param pData [in]  Traits overrides.
    */
    void setTraitsOverrides(const OdGiSubEntityTraitsData &pData)
    {
      m_overrides = pData;
    }
    /** \details
        Retrieves traits overrides (for changing).
        \returns
        subentity traits overrides as OdGiSubEntityTraitsData instance.
    */
    OdGiSubEntityTraitsData &traitsOverrides()
    {
      return m_overrides;
    }
    /** \details
        Returns traits overrides.
        \returns
        reference to subentity traits overrides.
    */
    const OdGiSubEntityTraitsData &traitsOverrides() const
    {
      return m_overrides;
    }

    /** \details
        Called by vectorizer to setup geometry output configuration for current conveyor drawable and state.
        \remarks
        Override to implement own settings processing.
    */
    virtual void setupDrawableProcessing()
    {
      if (!m_pDrawCallback.isNull())
        setProcessingEnabled(m_pDrawCallback->clippedGeometryOutputCallbackProc(*this, drawContext()));
    }

    /** \details
        Called to setup output geometry traits.
        \param context [in]  Geometry conveyor context.
        \remarks
        Override to implement own traits processing.
    */
    virtual void setupTraits(OdGiConveyorContext &context)
    {
      if (m_overrideFlags)
      {
        // Copy traits settings from current state
        OdGiSubEntityTraitsToData dataAdaptor(m_traits);
        OdGiSubEntityTraitsToData::copyTraits(context.subEntityTraits(), dataAdaptor, m_overrideFlags);
        // Setup traits overrides
        dataAdaptor.setTraits(m_overrides);
        OdGiSubEntityTraitsToData::copyTraits(dataAdaptor, context.subEntityTraits(), m_overrideFlags);
        context.onTraitsModified();
      }
    }

    /** \details
        Called to release output geometry traits.
        \param context [in]  Geometry conveyor context.
        \remarks
        Override to implement own traits processing.
    */
    virtual void releaseTraits(OdGiConveyorContext &context)
    {
      if (m_overrideFlags)
      {
        // Copy modified traits back to current state
        OdGiSubEntityTraitsToData dataAdaptor(m_traits);
        OdGiSubEntityTraitsToData::copyTraits(dataAdaptor, context.subEntityTraits(), m_overrideFlags);
        //context.onTraitsModified(); // Don't need since we completed with section geometry
      }
    }

    // Conveyor geometry primitives implementation

    /** \details
      Processes OdGiPolyline data.

      \param polyline [in]  Polyline data to be processed.
      \param pXfm [in]  Pointer to a transformation matrix.
      \param fromIndex [in]  Index of the first segment to be processed.
      \param numSegs [in]  Number of segments to be processed.
      \remarks
      numSegs == 0 indicates all segments starting at fromIndex are to be processed. 
    */
    void plineProc(const OdGiPolyline& polyline, const OdGeMatrix3d* pXfm = 0, OdUInt32 fromIndex = 0, OdUInt32 numSegs = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().plineProc(polyline, pXfm, fromIndex, numSegs);
    }
    
    /** \details
      Processes simple polyline data.

      \param numPoints [in]  Number of points.
      \param vertexList [in]  Array of vertices.
      \param pNormal [in]  Pointer to the normal vector.
      \param pExtrusion [in]  Pointer to the extrusion vector.
      \param baseSubEntMarker [in]  Currently unused.
      \remarks
      The extrusion vector specifies the direction and distance of the extrusion.
  */
    void polylineProc(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal = 0,
                      const OdGeVector3d* pExtrusion = 0, OdGsMarker baseSubEntMarker = -1)
    { WrapTraitsProc _wtp(this);
      destGeometry().polylineProc(numPoints, vertexList, pNormal, pExtrusion, baseSubEntMarker);
    }
    
    /** \details
      Processes polygon data.

      \param numPoints [in]  Number of points.
      \param vertexList [in]  Array of vertices.
      \param pNormal [in]  Pointer to the normal vector.
      \param pExtrusion [in]  Pointer to the extrusion vector.
      \remarks
      The extrusion vector specifies the direction and distance of the extrusion.
  */
    void polygonProc(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal = 0, const OdGeVector3d* pExtrusion = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().polygonProc(numPoints, vertexList, pNormal, pExtrusion);
    }
    
    /** \details
      Processes Xline data.
      
      \param firstPoint [in]  First point.
      \param secondPoint [in]  Second point.
      \remarks
      An Xline is an infinite line passing through the 
      specified points.
    */
    void xlineProc(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint)
    { WrapTraitsProc _wtp(this);
      destGeometry().xlineProc(firstPoint, secondPoint);
    }
    
    /** \details
      Processes Ray data.
      
      \param basePoint [in]  Base point.
      \param throughPoint [in]  Through point     
      \remarks
      A Ray is a semi-infinite line that starts at the basePoint, and passes 
      through the throughPoint.
  */
    void rayProc(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint)
    { WrapTraitsProc _wtp(this);
      destGeometry().rayProc(basePoint, throughPoint);
    }
    
    /** \details
      Processes mesh data.

      \param numRows [in]  Number of rows.
      \param numColumns [in]  Number of columns.
      \param vertexList [in]  Array of vertices.
      \param pEdgeData [in]  Pointer to additional edge data.
      \param pFaceData [in]  Pointer to additional face data.
      \param pVertexData [in]  Pointer to additional vertex data. 
    */
    void meshProc(OdInt32 numRows, OdInt32 numColumns, const OdGePoint3d* vertexList, const OdGiEdgeData* pEdgeData = 0, const OdGiFaceData* pFaceData = 0,
                  const OdGiVertexData* pVertexData = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().meshProc(numRows, numColumns, vertexList, pEdgeData, pFaceData, pVertexData);
    }
    
    /** \details
      Processes shell data.

      \param numVertices [in]  Number of vertices.
      \param vertexList [in]  Array of vertices.
      \param faceListSize [in]  Number of entries in faceList.
      \param faceList [in]  Array of numbers that define the faces.
      \param pEdgeData [in]  Pointer to additional edge data.
      \param pFaceData [in]  Pointer to additional face data.
      \param pVertexData [in]  Pointer to additional vertex data.     
    */
    void shellProc(OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList, const OdGiEdgeData* pEdgeData = 0,
                   const OdGiFaceData* pFaceData = 0, const OdGiVertexData* pVertexData = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().shellProc(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
    }
    
    /** \details
      Process circle data. 

      \param center [in]  Center point.
      \param radius [in]  Radius.
      \param normal [in]  Normal vector.
      \param pExtrusion [in]  Pointer to the extrusion vector.
      \remarks
      The extrusion vector specifies the direction and distance of the extrusion.
    */
    void circleProc(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d* pExtrusion = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().circleProc(center, radius, normal, pExtrusion);
    }
    
    /** \details
      Process circle data. 

      \param center [in]  Center point.
      \param radius [in]  Radius.
      \param normal [in]  Normal vector.
      \param startVector [in]  Start vector of the arc. Not used.
      \param pExtrusion [in]  Pointer to the extrusion vector.
      \remarks
      The extrusion vector specifies the direction and distance of the extrusion.
    */
    void circleProc2(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d& startVector, const OdGeVector3d* pExtrusion = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().circleProc2(center, radius, normal, startVector, pExtrusion);
    }
    
    /** \details
      Process circle data.
      
      \param firstPoint [in]  First point of a 3-point circle.
      \param secondPoint [in]  Second point of a 3-point circle.
      \param thirdPoint [in]  Third point of a 3-point circle.
      \param pExtrusion [in]  Pointer to the extrusion vector.
      \remarks
      If firstPoint, secondPoint, and thirdPoint are specified, they
      cannot be colinear and no two can be coincident.
    */
    void circleProc(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint, const OdGeVector3d* pExtrusion = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().circleProc(firstPoint, secondPoint, thirdPoint, pExtrusion);
    }
    
    /** \details
    Processes circular arc data.

      \param center [in]  Center point.
      \param radius [in]  Radius.
      \param normal [in]  Normal vector.
      \param startVector [in]  Defines the start of this arc.
      \param sweepAngle [in]  Angle that defines the arc.
      \param arcType [in]  Arc type.
      \param pExtrusion [in]  Pointer to the extrusion vector.  
      \remarks
      arcType must be one of the following:
      
      <table>
      Name              Value     Description
      kOdGiArcSimple    0         Unfilled.
      kOdGiArcSector    1         Filled area bounded by the arc and its center.
      kOdGiArcChord     2         Filled area bounded by the arc and its end points
      </table>
      The extrusion vector specifies the direction and distance of the extrusion.
      \remarks
      All angles are expressed in radians.
    */
    void circularArcProc(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d& startVector,
                         double sweepAngle, OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().circularArcProc(center, radius, normal, startVector, sweepAngle, arcType, pExtrusion);
    }
    
    /** \details
      Processes circular arc data.

      \param firstPoint [in]  First point of a 3-point arc.
      \param secondPoint [in]  Second point of a 3-point arc.
      \param thirdPoint [in]  Third point of a 3-point arc.
      \param arcType [in]  Arc type.
      \param pExtrusion [in]  Pointer to the extrusion vector.
      
      \remarks
      If firstPoint, secondPoint, and thirdPoint are specified, they
      cannot be colinear and no two can be coincident.
    */
    void circularArcProc(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint, OdGiArcType arcType = kOdGiArcSimple,
                         const OdGeVector3d* pExtrusion = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().circularArcProc(firstPoint, secondPoint, thirdPoint, arcType, pExtrusion);
    }
    
    /** \details
      Processes OdGeEllipArc3d data.

      \param ellipArc [in]  Elliptical arc.
      \param endPointOverrides [in]  Array of points to be used as the first and last points of the vectorized arc.
      \param arcType [in]  Arc type.
      \param pExtrusion [in]  Pointer to the extrusion vector.       
      \remarks
      arcType must be one of the following:   
      
      <table>
      Name              Value     Description
      kOdGiArcSimple    0         Unfilled.
      kOdGiArcSector    1         Filled area bounded by the arc and its center.
      kOdGiArcChord     3         Filled area bounded by the arc and its end points
      </table>
      The extrusion vector specifies the direction and distance of the extrusion.
    */
    void ellipArcProc(const OdGeEllipArc3d& ellipArc, const OdGePoint3d* endPointOverrides = 0, OdGiArcType arcType = kOdGiArcSimple,
                      const OdGeVector3d* pExtrusion = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().ellipArcProc(ellipArc, endPointOverrides, arcType, pExtrusion);
    }
    
    /** \details
      Processes OdGeNurbCurve3d data.
      \param nurbsCurve [in]  NURBS curve data.
    */
    void nurbsProc(const OdGeNurbCurve3d& nurbsCurve)
    { WrapTraitsProc _wtp(this);
      destGeometry().nurbsProc(nurbsCurve);
    }
    
    /** \details
      Processes text data.

      \param position [in]  Position of the text.
      \param direction [in]  Baseline direction of the text.
      \param upVector [in]  Up vector for the text.
      \param msg [in]  Text string.
      \param numBytes [in]  Number of bytes in msg.
      \param raw [in]  If and only if true, escape sequences, such as %%P, will not be converted to special characters.
      \param pTextStyle [in]  Pointer to the TextStyle for the text.
      \param pExtrusion [in]  Pointer to the extrusion vector.

      \remarks
      The extrusion vector specifies the direction and distance of the extrusion.
    */
    void textProc(const OdGePoint3d& position, const OdGeVector3d& direction, const OdGeVector3d& upVector, const OdChar* msg,
                  OdInt32 numBytes, bool raw, const OdGiTextStyle* pTextStyle, const OdGeVector3d* pExtrusion = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().textProc(position, direction, upVector, msg, numBytes, raw, pTextStyle, pExtrusion);
    }
    
    /** \details
      Processes text data.

      \param position [in]  Position of the text.
      \param direction [in]  Baseline direction of the text.
      \param upVector [in]  Up vector for the text.
      \param msg [in]  Text string.
      \param numBytes [in]  Number of bytes in msg.
      \param raw [in]  Flag that specifies whether escape sequences, such as %%P, should not be converted to special characters.
      \param pTextStyle [in]  Pointer to the TextStyle for the text.
      \param pExtrusion [in]  Pointer to the extrusion vector.
      \remarks
      The extrusion vector specifies the direction and distance of the extrusion.
    */
    void textProc2(const OdGePoint3d& position, const OdGeVector3d& direction, const OdGeVector3d& upVector, const OdChar* msg,
                   OdInt32 numBytes, bool raw, const OdGiTextStyle* pTextStyle, const OdGeVector3d* pExtrusion = 0, const OdGeExtents3d* extentsBox = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().textProc2(position, direction, upVector, msg, numBytes, raw, pTextStyle, pExtrusion, extentsBox);
    }
    
    /** \details
      Processes shape data.

      \param position [in]  Position of the shape.
      \param direction [in]  Baseline direction of the shape.
      \param upVector [in]  Up vector for the shape.
      \param shapeNumber [in]  Shape number.
      \param pTextStyle [in]  Pointer to the TextStyle for the shape.
      \param pExtrusion [in]  Pointer to the extrusion vector.
      \remarks
      The extrusion vector specifies the direction and distance of the extrusion.
    */
    void shapeProc(const OdGePoint3d& position, const OdGeVector3d& direction, const OdGeVector3d& upVector, int shapeNumber,
                   const OdGiTextStyle* pTextStyle, const OdGeVector3d* pExtrusion = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().shapeProc(position, direction, upVector, shapeNumber, pTextStyle, pExtrusion);
    }
    
    /** \details
      Processes raster images data.

      \param origin [in]  Lower-left corner. 
      \param u [in]  Image width vector.
      \param v [in]  Image height vector.
      \param pImage [in]  Pointer to the RasterImage object.
      \param uvBoundary [in]  Array of image boundary points (may not be null).
      \param numBoundPts [in]  Number of boundary points.
      \param transparency [in]  True if and only if image transparency is on.
      \param brightness [in]  Image brightness [0.0 .. 100.0].
      \param contrast [in]  Image contrast [0.0 .. 100.0].
      \param fade [in]  Image fade value [0.0 .. 100.0].
    */
    void rasterImageProc(const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v, const OdGiRasterImage* pImage,
                         const OdGePoint2d* uvBoundary, OdUInt32 numBoundPts, bool transparency = false, double brightness = 50.0,
                         double contrast = 50.0, double fade = 0.0)
    { WrapTraitsProc _wtp(this);
      destGeometry().rasterImageProc(origin, u, v, pImage, uvBoundary, numBoundPts, transparency, brightness, contrast, fade);
    }
    
    /** \details
      Processes metafile data.

      \param origin [in]  Metafile origin.
      \param u [in]  Metafile width vector.
      \param v [in]  Metafile height vector.
      \param pMetafile [in]  Pointer to the metafile object.
      \param dcAligned [in]  reserved.
      \param allowClipping [in]  reserved.
    */
    void metafileProc(const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v, const OdGiMetafile* pMetafile,
                      bool dcAligned = true, bool allowClipping = false)
    { WrapTraitsProc _wtp(this);
      destGeometry().metafileProc(origin, u, v, pMetafile, dcAligned, allowClipping);
    }
    
    /** \details
      Processes Xline data.
      
      \param basePoint [in]  Base point.
      \param direction [in]  Direction of an xline.
      \remarks
      An Xline is an infinite line passing through the 
      specified basePoint along the passed direction.
    */
    void xlineProc2(const OdGePoint3d& basePoint, const OdGeVector3d& direction)
    { WrapTraitsProc _wtp(this);
      destGeometry().xlineProc2(basePoint, direction);
    }
    
    /** \details
      Processes Ray data.
      
      \param basePoint [in]  Base point.
      \param direction [in]  Direction of a ray.
      \remarks
      A Ray is a semi-infinite line that starts at the basePoint, and extends 
      through the specified direction.
    */
    void rayProc2(const OdGePoint3d& basePoint, const OdGeVector3d& direction)
    { WrapTraitsProc _wtp(this);
      destGeometry().rayProc2(basePoint, direction);
    }
    
    /** \details
      Processes TrueType font characters in dynamic polyDraw mode
      which is similar to shellProc() method, except here additional information about 
      bezier data is passed for processing.
      
      \param numVertices [in]  Number of vertices.
      \param vertexList [in]  Array of vertices.
      \param faceListSize [in]  Number of entries in faceList.
      \param faceList [in]  Array of numbers that define the faces.
      \param pBezierTypes [in]  Bezier types.
      \param pFaceData [in]  Pointer to additional face data.
      \param pVertexData [in]  Pointer to additional vertex data.
    */
    void ttfPolyDrawProc(OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList, 
                         const OdUInt8* pBezierTypes, const OdGiFaceData* pFaceData = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().ttfPolyDrawProc(numVertices, vertexList, faceListSize, faceList, pBezierTypes, pFaceData);
    }
    
    /** \details
      Processes an array of points.
      
      \param numPoints [in]  Number of points.
      \param vertexList [in]  Pointer to an array of vertices.
      \param pColors [in]  Pointer to point colors.
      \param pTransparency [in]  Pointer to point transparency.
      \param pNormals [in]  Array of normal vectors.
      \param pExtrusions [in]  Array of vectors that defines extrusions.
      \param pSubEntMarkers [in]  Array of sub-entity markers.
      \param nPointSize [in]  Points size.
    */
    void polypointProc(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors, const OdCmTransparency* pTransparency = 0,
                       const OdGeVector3d* pNormals = 0, const OdGeVector3d* pExtrusions = 0, const OdGsMarker* pSubEntMarkers = 0, OdInt32 nPointSize = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().polypointProc(numPoints, vertexList, pColors, pTransparency, pNormals, pExtrusions, pSubEntMarkers, nPointSize);
    }
    
    /** \details
      Processes a row of points.

      \param numPoints [in]  Number of points.
      \param startPoint [in]  First point to draw.
      \param dirToNextPoint [in]  Offset to next point.
    */
    void rowOfDotsProc(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint)
    { WrapTraitsProc _wtp(this);
      destGeometry().rowOfDotsProc(numPoints, startPoint, dirToNextPoint);
    }

    /** \details
      Processes point cloud object.

      \param pCloud [in]  Point cloud object.
      \param pFilter [in]  Optional set of point cloud object conveyor filters.
    */
    void pointCloudProc(const OdGiPointCloud &pCloud, const OdGiPointCloudFilter *pFilter)
    { WrapTraitsProc _wtp(this);
      destGeometry().pointCloudProc(pCloud, pFilter);
    }

    /** \details
      Processes hatch boundary.

      \param edges [in]  2d curves array.
      \param pXform [in]  Optional boundary transform matrix.
    */
    void edgeProc(const OdGiEdge2dArray& edges, const OdGeMatrix3d* pXform = 0)
    { WrapTraitsProc _wtp(this);
      destGeometry().edgeProc(edges, pXform);
    }
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiClippedGeometryOutput object pointers.
*/
typedef OdSmartPtr<OdGiClippedGeometryOutput> OdGiClippedGeometryOutputPtr;

/** \details
    This class is an default implementation of OdGiConveyorGeometry that provides output of section geometry.

    <group OdGi_Classes>
    \sa
    <link gi_geometry_sectioning_callback, Work with Geometry Sectioning Callback>
*/
class ODGI_EXPORT OdGiSectionGeometryOutput : public OdGiClippedGeometryOutput
{
  protected:
    enum Flags
    {
      kOutputClosedPrimitives      = kLastGenericFlag << 1,
      kOutputOpenedPrimitives      = kLastGenericFlag << 2,
      kClosedPrimitivesAsPolylines = kLastGenericFlag << 3,
      kToleranceOverrideEnabled    = kLastGenericFlag << 4
    };
  protected:
    double           m_tolOverride; // Section tolerance override.
    OdGePoint3dArray m_tmpPoints;   // Temporary vertexes array
  public:
    /** \details
      Default constructor for the OdGiSectionGeometryOutput class. Sets the tolerance for geometric operations
      to 1^-10 and sets kOutputClosedPrimitives flag.
    */
    OdGiSectionGeometryOutput() : m_tolOverride(1.e-10) { SETBIT_1(m_flags, kOutputClosedPrimitives); }

    ODRX_DECLARE_MEMBERS(OdGiSectionGeometryOutput);

    /** details
        Copies the contents of the specified object into this object.
        
        \param pSource [in]  Pointer to the object from which to copy the data.
        /remarks
        If you create nested class, based onto OdGiSectionGeometryOutput, you should implement this method override,
        elsewhere it will not support multithreaded regeneration.
    */
    virtual void copyFrom(const OdRxObject* pSource)
    {
      const OdGiSectionGeometryOutput *pFrom = OdGiSectionGeometryOutput::cast(pSource);
      if (!pFrom)
        throw OdError(eInvalidInput);
      OdGiClippedGeometryOutput::copyFrom(pSource);
      m_tolOverride = pFrom->m_tolOverride;
    }

    /** \details
        Checks whether closed sections output is enabled.
        \returns 
        true if closed sections output enabled, false otherwise.
    */
    bool isClosedSectionsOutputEnabled() const { return GETBIT(m_flags, kOutputClosedPrimitives); }
    /** \details
        Enables or disables closed sections output.
        \param bSet [in]  New state of closed sections output.
    */
    void enableClosedSectionsOutput(bool bSet) { SETBIT(m_flags, kOutputClosedPrimitives, bSet); }

    /** \details
        Checks whether opened sections output is enabled.
        \returns
        true if opened sections output enabled, false otherwise.
    */
    bool isOpenedSectionsOutputEnabled() const { return GETBIT(m_flags, kOutputOpenedPrimitives); }
    /** \details
        Enables or disables opened sections output.
        \param bSet [in]  New state of opened sections output.
    */
    void enableOpenedSectionsOutput(bool bSet) { SETBIT(m_flags, kOutputOpenedPrimitives, bSet); }

    /** \details
        Checks whether output of closed sections as polyline primitives is enabled.
        \returns
        true if output of closed sections as polyline primitives is enabled, false otherwise.
    */
    bool isOutputOfClosedSectionsAsPolylinesEnabled() const { return GETBIT(m_flags, kClosedPrimitivesAsPolylines); }
    /** \details
        Enables or disables output of closed sections as polyline primitives.
        \param bSet [in]  New state of closed sections output mode.
    */
    void enableOutputOfClosedSectionsAsPolylines(bool bSet) { SETBIT(m_flags, kClosedPrimitivesAsPolylines, bSet); }

    /** \details
        Checks whether sections generation tolerance override is enabled.
        \returns 
        true if sections generation tolerance override is enabled, false otherwise.
    */
    bool isSectionToleranceOverrideEnabled() const { return GETBIT(m_flags, kToleranceOverrideEnabled); }

    /** \details
        Returns sections generation tolerance override value.
    */
    double sectionToleranceOverride() const { return m_tolOverride; }

    /** \details
        Enables sections generation tolerance override and sets it's value.
        \param tolOverride [in]  Tolerance override for geometric operations.
    */
    void setSectionToleranceOverride(double tolOverride) { SETBIT_1(m_flags, kToleranceOverrideEnabled); m_tolOverride = tolOverride; }

    /** \details
        Disables sections generation tolerance override.
    */
    void resetSectionToleranceOverride() { SETBIT_0(m_flags, kToleranceOverrideEnabled); }

    /** \details
        Processes simple polyline data, called for opened sections.
        
        \param numPoints [in]  Number of points.
        \param vertexList [in]  Array of vertices.
        \param pNormal [in]  Pointer to the normal vector.
        \param pExtrusion [in]  Pointer to the extrusion vector.
        \param baseSubEntMarker [in]  Currently unused.
        \remarks
        The extrusion vector specifies the direction and distance of the extrusion.
        Redirects to destination geometry in default implementation.
    */
    void polylineProc(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal = 0,
                      const OdGeVector3d* pExtrusion = 0, OdGsMarker baseSubEntMarker = -1)
    {
      OdGiClippedGeometryOutput::polylineProc(numPoints, vertexList, pNormal, pExtrusion, baseSubEntMarker);
    }

    /** \details
        Processes shell data. Called for closed sections.
        
        \param numVertices [in]  Number of vertices.
        \param vertexList [in]  Array of vertices.
        \param faceListSize [in]  Number of entries in faceList.
        \param faceList [in]  Array of numbers that define the faces.
        \param pEdgeData [in]  Pointer to additional edge data.
        \param pFaceData [in]  Pointer to additional face data.
        \param pVertexData [in]  Pointer to additional vertex data. 
        \remarks
        Redirects to destination geometry in default implementation.
    */
    void shellProc(OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList,
                   const OdGiEdgeData* pEdgeData = 0, const OdGiFaceData* pFaceData = 0, const OdGiVertexData* pVertexData = 0)
    { WrapTraitsProc _wtp(this);
      if (!isOutputOfClosedSectionsAsPolylinesEnabled())
        destGeometry().shellProc(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
      else
        shellProcAsPolylineProc(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
    }
  protected:
    void shellProcAsPolylineProc(OdInt32 /*numVertices*/, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList,
                                 const OdGiEdgeData* /*pEdgeData*/, const OdGiFaceData* pFaceData, const OdGiVertexData* /*pVertexData*/)
    {
      OdInt32 nFace = 0, nFaceList = 0;
      OdGeVector3d extrusion;
      const bool bExtrusion = OdNonZero(drawContext().subEntityTraits().thickness());
      while (nFaceList < faceListSize)
      {
        const OdInt32 numVerts = Od_abs(faceList[nFaceList++]);
        const OdGeVector3d *pNormal = (pFaceData && pFaceData->normals()) ? (pFaceData->normals() + nFace) : NULL;
        if (pNormal && bExtrusion)
          extrusion = *pNormal * drawContext().subEntityTraits().thickness();
        m_tmpPoints.resize(numVerts + 1);
        OdGePoint3d *tmpPoints = m_tmpPoints.asArrayPtr();
        for (OdInt32 nVert = 0; nVert < numVerts; nVert++)
          tmpPoints[nVert] = vertexList[faceList[nFaceList++]];
        tmpPoints[numVerts] = tmpPoints[0];
        destGeometry().polylineProc(numVerts + 1, tmpPoints, pNormal, (pNormal && bExtrusion) ? &extrusion : NULL);
        nFace++;
      }
    }
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiSectionGeometryOutput object pointers.
*/
typedef OdSmartPtr<OdGiSectionGeometryOutput> OdGiSectionGeometryOutputPtr;

/** \details
    This class is an default implementation of OdGiConveyorGeometry that provides output of section geometry.

    <group OdGi_Classes>
    \sa
    <link gi_geometry_sectioning_callback, Work with Geometry Sectioning Callback>
*/
class ODGI_EXPORT OdGiCuttedGeometryOutput : public OdGiClippedGeometryOutput
{
  public:
    /** \details
      Default constructor for the OdGiCuttedGeometryOutput class.
    */
    OdGiCuttedGeometryOutput() { }

    ODRX_DECLARE_MEMBERS(OdGiCuttedGeometryOutput);

    /** details
        Copies the contents of the specified object into this object.
        
        \param pSource [in]  Pointer to the object from which to copy the data.
        /remarks
        If you create nested class, based onto OdGiSectionGeometryOutput, you should implement this method override,
        elsewhere it will not support multithreaded regeneration.
    */
    virtual void copyFrom(const OdRxObject* pSource)
    {
      const OdGiCuttedGeometryOutput *pFrom = OdGiCuttedGeometryOutput::cast(pSource);
      if (!pFrom)
        throw OdError(eInvalidInput);
      OdGiClippedGeometryOutput::copyFrom(pSource);
    }
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiCuttedGeometryOutput object pointers.
*/
typedef OdSmartPtr<OdGiCuttedGeometryOutput> OdGiCuttedGeometryOutputPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGISECTIONGEOMETRYOUTPUT_H__
