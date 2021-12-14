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

#ifndef __ODGICOMMONDRAW_H__
#define __ODGICOMMONDRAW_H__

#include "TD_PackPush.h"

#include "RxObject.h"
#include "Gi/Gi.h"

class OdGiGeometry;
class OdGiContext;
class OdGiSubEntityTraits;
class OdDbStub;
typedef OdRxObject OdDbBaseDatabase;
class OdGePoint3d;
class OdGiDrawable;
class OdGiPathNode;
class OdGiSelectionStyle;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGiDrawable object pointers.
*/
typedef OdSmartPtr<OdGiDrawable> OdGiDrawablePtr;

/** \details
    Viewport regeneration modes. 
*/
typedef enum
{
  eOdGiRegenTypeInvalid         = 0,  // Invalid regen type. 
  kOdGiStandardDisplay          = 2,  // Regenerating for 2D wireframe display. 
  kOdGiHideOrShadeCommand       = 3,  // Regenerating for legacy hide or shade commands. 
  kOdGiRenderCommand            = 4,  // Regenerating for rendered display.
  kOdGiForExplode               = 5,  // Regenerating for explode.  
  kOdGiSaveWorldDrawForProxy    = 6,  // Regenerating for proxy. 
  kOdGiForExtents               = 7   // Regenerating for extents. 
} OdGiRegenType;

/** \details
    Deviation types used for tessellation.
*/
typedef enum
{
  kOdGiMaxDevForCircle      = 0,  // Set maximum deviation for circles.
  kOdGiMaxDevForCurve       = 1,  // Set maximum deviation for curves.
  kOdGiMaxDevForBoundary    = 2,  // Set maximum deviation for boundaries.
  kOdGiMaxDevForIsoline     = 3,  // Set maximum deviation for isolines.
  kOdGiMaxDevForFacet       = 4   // Set maximum deviation for facets.
} OdGiDeviationType;

class OdGiCommonDraw;
class OdGiTextStyle;
class OdPsPlotStyleData;
class OdGiConveyorGeometry;

/** \details
    Text vectorization flags.
*/
enum
{
  kOdGiIncludeScores    = 2,  // Include scores during text vectorization.
  kOdGiRawText          = 4,  // Vectorize text in one raw.
  kOdGiIncludePenups    = 8,  // Include penups during text vectorization.
  kOdGiDrawShape        = 16, // Draw shapes during text vectorization.
  kOdGiIgnoreMIF        = 32,  // Ignore MIF during text vectorization.
  kOdGiLastPosOnly      = 64   // Calculate last position only (for internal use only)
};

class OdGiSectionGeometryManager;
/** \details
  A data type that represents a smart pointer to an <link OdGiSectionGeometryManager, OdGiSectionGeometryManager> object.
*/
typedef OdSmartPtr<OdGiSectionGeometryManager> OdGiSectionGeometryManagerPtr;

/** \details
    This class defines common operations and properties that are used in the
    ODA Platform vectorization process.
    
    \remarks
    An instance of an OdGiContext subclass is normally created as a preliminary step in the vectorization process, which is to be
    used throughout the vectorization.

    Most of the virtual functions in this class (the ones that are not pure virtual) are no-ops, serving only to define an interface.

    Corresponding C++ library: TD_Gi
    
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiContext : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiContext);

  /** \details
    Retrieves the database that is currently being vectorized.
    
    \returns
    pointer to the database being vectorized.
  */
  virtual OdDbBaseDatabase* database() const = 0;

  /** \details
    Opens for reading the specified drawable object that belongs to the database associated with this Context object.
    
    \param drawableId [in]  Object ID of the drawable to be opened.
    
    \returns
    Retrieves a SmartPointer to the drawable object.
  */
  virtual OdGiDrawablePtr openDrawable(
    OdDbStub* drawableId) = 0;

  /** \details
      Retrieves the default lineweight associated with this Context object.
      
      \returns
      the values from the OdDb::LineWeight enumeration that represent lineweight configurations.
  */
  virtual OdDb::LineWeight defaultLineWeight() const;

  /** \details
    Retrieves the common linetype scale for this Context object.
    
    \returns
    common scale of linetypes.
  */
  virtual double commonLinetypeScale() const;

  /** \details
    Retrieves the default text style associated with this Context object.
    
    \param textStyle [out]  Receives the TextStyle object.
  */
  virtual void getDefaultTextStyle(
    OdGiTextStyle& textStyle);

  /** \details
    Vectorizes the specified shape to the specified interface object.

    \param pDraw [in]  Pointer to the CommonDraw object.
    \param position [in]  Position of the shape.
    \param shapeNumber [in]  Shape number.
    \param pTextStyle [in]  Pointer to the TextStyle for the shape.
  */
  virtual void drawShape(
    OdGiCommonDraw* pDraw, 
    OdGePoint3d& position, 
    int shapeNumber, 
    const OdGiTextStyle* pTextStyle);

  /** \details
    Vectorizes the specified shape.
    
    \param pDest [in]  Pointer to the Conveyor object.
    \param position [in]  Position of a shape.
    \param direction [in]  Baseline direction for the text.
    \param upVector [in]  Up vector for the text.
    \param shapeNumber [in]  Number of shape to draw. Shape is a single character of a special SHX font. This function returns the number of that character.
    \param pTextStyle [in]  Pointer to the text style applied to a shape.
    \param pExtrusion [in]  Pointer to the Extrusion vector for the text.
  */
  virtual void drawShape(
    OdGiConveyorGeometry* pDest,
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    int shapeNumber, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion);

  /** \details
    Vectorizes the specified text string to the supplied CommonDraw object.

    \param pDraw [in]  Pointer to the CommonDraw object.
    \param position [in]  Position of the text.
    \param msg [in]  Text string.
    \param numBytes [in]  Number of bytes in msg (not including the optional null byte).
    \param pTextStyle [in]  Pointer to the TextStyle for msg.
    \param flags [in]  Vectorization flags.
    
    \remarks
    msg must be null terminated if numBytes is not supplied.
  */
  virtual void drawText(
    OdGiCommonDraw* pDraw, 
    OdGePoint3d& position,
    const OdChar* msg, 
    OdInt32 numBytes,
    const OdGiTextStyle* pTextStyle, 
    OdUInt32 flags = 0);

  /** \details
    Vectorizes the specified text string to the supplied CommonDraw object.
    \param pDraw [in]  Pointer to the CommonDraw object.
    \param position [in]  Position of the text.
    \param height [in]  Height of the text.
    \param width [in]  Width of the text.
    \param oblique [in]  Oblique angle of the text.
    \param msg [in]  Text message.
      
    \remarks
    All angles are expressed in radians.
    
    As currently implemented, this function ignores width and oblique.
    They will be fully implemented in a future release.
  */
  virtual void drawText(
    OdGiCommonDraw* pDraw, 
    OdGePoint3d& position,
    double height, 
    double width, 
    double oblique, 
    const OdString& msg);

  /** \details
    Vectorizes the specified text string to the supplied CommonDraw object.
    
    \param pDest [in]  Pointer to the Conveyor object.
    \param position [in]  Position of the text.
    \param direction [in]  Baseline direction for the text.
    \param upVector [in]  Up vector for the text.
    \param msg [in]  Text string.
    \param numBytes [in]  Number of bytes in msg (not including the optional null byte).
    \param raw [in]  If and only if true, escape sequences, such as %%P, will not be converted to special characters.
    \param pTextStyle [in]  Pointer to the TextStyle for msg.
    \param pExtrusion [in]  Pointer to the extrusion vector for the text.
  */
  virtual void drawText(
    OdGiConveyorGeometry* pDest,
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    const OdChar* msg, 
    OdInt32 numBytes, 
    bool raw,
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion);

  /** \details
    Retrieves the extents box for the specified text.
    
    \param textStyle [in]  TextStyle for msg.
    \param msg [in]  Pointer to an array of characters.
    \param nLength [in]  Message length.
    \param flags [in]  Vectorization flags.
    \param min [out]  Receives the lower-left corner of the extents box.
    \param max [out]  Receives the upper-right corner of the extents box.
    \param pEndPos [out]  If non-NULL, receives the end position of the text string.

  */
  virtual void textExtentsBox(
    const OdGiTextStyle& textStyle, 
    const OdChar* msg, 
    int nLength,
    OdUInt32 flags, 
    OdGePoint3d& min, 
    OdGePoint3d& max, 
    OdGePoint3d* pEndPos = 0);

  /** \details
    Retrieves the extents box for the specified shape.
    
    \param textStyle [in]  TextStyle for the shape.
    \param shapeNumber [in]  Number of a shape. Shape is a single character of a special SHX font. This function returns the number of that character.
    \param min [out]  Receives the lower-left corner of the extents box.
    \param max [out]  Receives the upper-right corner of the extents box.
  */
  virtual void shapeExtentsBox(
    const OdGiTextStyle& textStyle, 
    int shapeNumber, 
    OdGePoint3d& min, 
    OdGePoint3d& max);

  /** \details
    Retrieves the circle zoom percent for this vectorization process.
    
    \param viewportId [in]  Pointer to the Object ID of the Viewport object to be queried.
    
    \returns
    Retrieves a value in the range [1,20000]. 100 is the default.
  */
  virtual unsigned int circleZoomPercent(
    OdDbStub* viewportId) const;

  /** \details
    Checks whether this vectorization is intended for hard copy output.
    
    \returns
    true if this vectorization is intended for hard copy output, false otherwise.
  */
  virtual bool isPlotGeneration() const;

  /** \details
    Retrieves the palette background color.
    
    \returns
    palette background color as ODCOLORREF definition.
  */
  virtual ODCOLORREF paletteBackground() const;

  /** \details
    Checks whether TrueType text should be filled during this vectorization.
    
    \returns
    true if TrueType text should be filled during this vectorization, false otherwise.
  */
  virtual bool fillTtf() const;

  /** \details
    Retrieves the number of isolines to be drawn on surfaces during this vectorization.
    
    \returns
    number of isolines as OdUInt32 value.
  */
  virtual OdUInt32 numberOfIsolines() const;

  /** \details
    Checks whether shell/mesh geometry primitives should be filled during this vectorization.
    
    \returns
    true if shell/mesh geometry primitives should be filled, false otherwise.
  */
  virtual bool fillMode() const;

  /** \details
    Retrieves true if and only if quick text mode is enabled for this vectorization process.
    
    \returns
    true if quick text mode is enabled for this vectorization process, false otherwise.
  */
  virtual bool quickTextMode() const;

  /** \details
    Retrieves the text quality percent for this vectorization process.

    \returns
    value in the range [0,100]. 50 is the default.
    \sa
    <link gi_ttf_render_quality, Adjusting the Quality of TrueType Font Text>
  */
  virtual OdUInt32 textQuality() const;

  /** \details
    Retrieves true if and only if triangle cache for ttf fonts is enabled for this vectorization process.
    
    \returns
    true if triangle cache for ttf fonts is enabled for this vectorization process.
  */
  virtual bool useTtfTriangleCache() const;

  /** \details
    Image quality types.
  */
  enum ImageQuality
  {
    kImageQualityDraft = 0,  // Draft quality.
    kImageQualityHigh  = 1   // High quality.
  };

  /** \details
    Retrieves the image quality for this vectorization process.
    
    \returns
    Image quality as a value of ImageQuality enumeration.
  */
  virtual ImageQuality imageQuality() const;

  /** \details
    Raster image flags.
  */
  enum ImageSelectionFlags
  {
    kImageHighlight = (1 << 0), // Highlight contents of a raster image.
    kImageSelect    = (1 << 1)  // Select contents of a raster image.
  };

  /** \details
    Retrieves a set of flags that describe image behavior during selection.

    \returns
    Set of ImageSelectionFlags enumeration bit flags.
  */
  virtual OdUInt32 imageSelectionBehavior() const;

  /** \details
    Fading types.
  */
  enum FadingType
  {
    kLockedLayerFade = 0,  // Fading objects on locked layers.
    kXrefFade        = 1,  // Fading xref objects.
    kRefEditFade     = 2   // Fading references being edited.
  };

  /** \details
    Retrieves Fading intensity percentage.
    
    \param fadingType [in]  Fading type.
    
    \returns
    50 if fading type isn't kRefEditFade, 70 is returned otherwise.
  */
  virtual OdUInt32 fadingIntensityPercentage(FadingType fadingType) const;

  /** \details
    Glyph types.
  */
  enum GlyphType
  {
    kLightGlyph  = 0,  // Light glyph.
    kCameraGlyph = 1   // Camera glyph.
  };

  /** \details
    Retrieves the glyph size for the specified glyph type.

    \param glyphType [in]  Type of glyph to return glyph size for.

    \returns
    Retrieves a value in the range [0,100].

    The default return values are the following:
    <table>
    Name         Value
    kLightGlyph  0
    kCameraGlyph 50
    </table>
  */
  virtual OdUInt32 glyphSize(GlyphType glyphType) const;
  
  /** \details
    Lineweight styles.
  */
  enum LineWeightStyle
  {
    kPointLineWeight = 0,  // Point lineweight style.
    kLineCapStyle    = 1,  // Line cap style.
    kLineJoinStyle   = 2   // Line join style.
  };

  /** \details
    Retrieves the lineweight display style configuration.

    \param styleEntry [in]  Lineweight style entry for which the current setting will be returned.

    \returns
    <ul>
    * For kPointLineWeight style, 0 will be returned if point lineweight is disabled or 1 will be returned if enabled.
    * For kLineCapStyle style, entry will be returned as a member of the OdPs::LineEndStyle enumeration.
    * For kLineJoinStyle style, entry will be returned as a member of the OdPs::LineJoinStyle enumeration.
    </ul>
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  virtual OdUInt32 lineWeightConfiguration(LineWeightStyle styleEntry) const;
  
  /** \details
    Retrieves the selection styles for entities highlighting.

    \param nStyle [in]  Selection style index.
    \param selStyle [out]  Receives the selection style data.

    \returns
    Number of selection styles provided by this context object.

    \remarks
    This method should return at least single selection style for 0 index.
  */
  virtual OdUInt32 selectionStyle(OdUInt32 nStyle, OdGiSelectionStyle &selStyle) const;

  /** \details
    Coordinate systems.
  */
  enum CoordinatesSystem
  {
    kWCS = 0, // World Coordinates System.
    kECS = 1, // Eye Coordinates System.
    kNDC = 2, // Normalized Device Coordinates (in 0-1 range).
    kDCS = 3  // Device Coordinates System (in pixels).
  };

  /** \details
    Specifies behavior of OdGiViewportGeometry::polylineEye/polygonEye/polylineDc/polygonDc methods.

    \param csType [in]  If specified kECS - return coordinates system for polylineEye/polygonEye methods.
                        If specified kNDC or kDCS - return coordinates system for polylineDc/polygonDc methods.
                        Other argument types ingored and return coordinates system as is (kWCS for kWCS for example).

    \returns
    Coordinates system for specified input coordinates system type.
  */
  virtual CoordinatesSystem customViewportGeometryCS(CoordinatesSystem csType) const;

  /** \details
    Input flags of drawable filter.
  */
  enum DrawableFilterInputFlags 
  { 
    kNestedDrawable = 0x1000000  // Nested drawable flag.
  };
  
  /** \details
    Output flags of drawable filter.
  */
  enum DrawableFilterOutputFlags 
  { 
    kSkipVectorization = 1  // Skip vectorization flag.
  };
  
  /** \details
    Ranges of drawable filter.
  */
  enum DrawableFilterRanges 
  { 
	  kDrawableFilterAppRangeStart = 0x1000000                    // Start of the drawable filter range.
#if defined(OD_SWIGCSHARP)
    , kDrawableFilterAppRangeMask          = (uint)0xFF000000     // Drawable filter range mask for C#.
#elif defined(OD_SWIGJAVA)
    , kDrawableFilterAppRangeMask          = (long)0xFF000000     // Drawable filter range mask for Java.
#else
    , kDrawableFilterAppRangeMask          = (OdInt32)0xFF000000  // Drawable filter range mask for other languages.
#endif
  };

  /** \details
    Retrieves internal drawable filtration function ID. Must be implemented in derived classes.
    
    \param viewportId [in]  Pointer to the Object ID of the Viewport object to be queried.
    
    \returns
    0.
  */
  virtual OdIntPtr drawableFilterFunctionId(OdDbStub* viewportId) const;

  /** \details
    Retrieves the set of flags for the drawable filtration function (will be called for each drawable vectorization).

    \param functionId [in]  Internal drawable filtration function ID.
    \param pDrawable [in]  Currently vectorizing drawable pointer.
    \param nFlags [in]  Set of input flags.
    
    \returns
    0.
  */
  virtual OdUInt32 drawableFilterFunction(OdIntPtr functionId, const OdGiDrawable *pDrawable, OdUInt32 nFlags);

  /** \details
    Checks whether smooth contours information output is used for TrueType fonts.
    
    \returns
    true if smooth contours information output is used for TrueType fonts (OdCharacterProperties::kBezierCurves flag), false otherwise.
    \sa
    <link gi_ttf_render_quality, Adjusting the Quality of TrueType Font Text>
  */
  virtual bool ttfPolyDraw() const;

  /** \details
    Checks whether this vectorization process should be aborted.
    
    \returns
    true if this vectorization process should be aborted.
  */
  virtual bool regenAbort() const;

  /** \details
    Plot style types.
  */
  enum PStyleType
  {
    kPsNone           = 0,  // Style is disabled.
    kPsByColor        = 1,  // "By color" style.
    kPsByName         = 2   // "By name" style.
  };
  
  /** \details
    Retrieves the plot style type of this Context object.
    
    \returns
    returns one of the following:
    
    <table>
    Name          Value
    kPsNone       0
    kPsByColor    1
    kPsByName     2
    </table>
  */
  virtual PStyleType plotStyleType() const;
  
  /** \details
    Retrieves the PaperSpace PlotStyle data for this vectorization.

    \param penNumber [in]  Pen number.
    \param plotStyleData [out]  Receives the PlotStyle data.
    \param objectId [in]  Object ID of plot style.
  */
  virtual void plotStyle(
    int penNumber, 
    OdPsPlotStyleData& plotStyleData) const;

  /** \details
    Retrieves the PaperSpace PlotStyle data for this vectorization.

    \param objectId [in]  Object ID of plot style.
    \param plotStyleData [out]  Receives the PlotStyle data.
  */
  virtual void plotStyle(
    OdDbStub* objectId, 
    OdPsPlotStyleData& plotStyleData) const;

  /** \details
    Converts an object handle into an Object ID.

    \param persistentId [in]  Object handle.
    
    \returns
    converted object identifier.
  */
  virtual OdDbStub* getStubByID( OdUInt64 persistentId ) const;

  /** \details
    Converts an Object ID to an object handle (database persistent ID).

    \param objectId [in]  Object ID.
  */
  virtual OdUInt64 getIDByStub( OdDbStub* objectId ) const;

  /** \details
    Retrieves the database of an Object ID.

    \param objectId [in]  Object ID.
    
    \returns
    pointer to the database of the specified object identifier.
  */
  virtual OdDbBaseDatabase *getDatabaseByStub( OdDbStub* objectId ) const;

  /** \details
    Retrieves the Owner Object ID of the specified Object ID.

    \param objectId [in]  Object ID.
    
    \returns
    pointer to the identifier of owner object.
  */
  virtual OdDbStub* getOwnerIDByStub( OdDbStub* objectId ) const;

  /** \details
    Converts a material name into an Object ID.

    \param pBaseDb [in]  Pointer to a database where to search for material name.
    \param strMatName [in]  Material name.
    
    \returns
    pointer to the identifier of owner object.
  */
  virtual OdDbStub* getStubByMatName( OdDbBaseDatabase *pBaseDb, const OdString& strMatName ) const;

  /** \details
  Converts a material id into an Object ID. This method may check that converted Object ID is really material.

  \param pBaseDb [in]  Pointer to a database.
  \param materialId [in]  Material identifier.
  
  \returns
  pointer to a converted object identifier.
  */
  virtual OdDbStub* getStubByMaterialId(OdDbBaseDatabase *pBaseDb, const OdUInt64& materialId) const;

  /** \details
    Silhouette types.
  */
  enum SilhouetteType
  {
    kNoSilhouettes      = 0,        // Disables all types of silhouettes.
    kMeshSilhouettes    = (1 << 0), // Generates silhouettes for mesh primitives on Gi side.
    kModelerSilhouettes = (1 << 1), // Generates silhouettes for Brep's on modeler geometry side.
    kAllSilhouettes     = kMeshSilhouettes | kModelerSilhouettes // Enables all types of silhouettes.
  };

  /** \details
    Retrieves a silhouette type.
    
    \returns
    silhouette type as OdUInt32 value.
  */
  virtual OdUInt32 displaySilhouettes() const;
  /** \details
    Checks whether generation of silhouettes is enabled for mesh primitives on Gi side.
    
    \returns
    true if generation of silhouettes is enabled for mesh primitives on Gi side, false otherwise.
  */
  inline bool displayMeshSilhouettes() const    { return GETBIT(displaySilhouettes(), kMeshSilhouettes); }
  /** \details
    Checks whether generation of silhouettes is enabled for modeler geometry.
    
    \returns
    true if generation of silhouettes is enabled for modeler geometry.
  */
  inline bool displayModelerSilhouettes() const { return GETBIT(displaySilhouettes(), kModelerSilhouettes); }

  /** \details
    Retrieves the interface for accessing section geometry functionality.
    
    \returns
    smart pointer to the OdGiSectionGeometryManager instance.
  */
  virtual OdGiSectionGeometryManagerPtr getSectionGeometryManager();

  /** \details
    Receives rendering anti-aliasing settings.
    
    \returns
    0.
  */
  virtual OdUInt32 antiAliasingMode() const;

  /** \details
    Controls overriding of Xref properties.
  */
  virtual bool xrefPropertiesOverride() const;

  /** \details
    Controls processing of line type scales.
  */
  virtual bool multiplyByBlockLinetypeScales() const;
};

/** \details
    This class is the base class for entity-level vectorization within the ODA Platform.

    \sa
    TD_Gi

    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiCommonDraw : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiCommonDraw);

  /** \details
    Retrieves the regeneration type of the current vectorization process.

    \returns
    one of the following regeneration types:
    
    <table>
    Name                            Value 
    eOdGiRegenTypeInvalid           0
    kOdGiStandardDisplay            2
    kOdGiHideOrShadeCommand         3
    kOdGiRenderCommand              4
    kOdGiForExplode                 5
    kOdGiSaveWorldDrawForProxy      6
    kOdGiForExtents                 7
    </table>
  */
  virtual OdGiRegenType regenType() const = 0;

  /** \details
    Checks whether this vectorization process should be aborted.
    
    \returns
    true if this vectorization process should be aborted.
  */
  virtual bool regenAbort() const = 0;

  /** \details
    Provides access to this object's subentity traits.

    \returns
    sub-entity traits.

    \remarks
    This allows the modification of the vectorization attributes such as color, linetype, etc.
  */
  virtual OdGiSubEntityTraits& subEntityTraits() const = 0;

  /** \details
    Provides access to this object's "drawing interface."
    
    \remarks
    The "drawing interface" is a set of geometry functions used during the vectorization process.
  */
  virtual OdGiGeometry& rawGeometry() const = 0;

  /** \details
    Checks whether this vectorization process is the result of a "drag" operation.
    
    \returns
    true if this vectorization process is the result of a "drag" operation, false otherwise.
  */
  virtual bool isDragging() const = 0;
  
  /** \details
    Retrieves the recommended maximum deviation of the
    current vectorization for the specified point on a curve.

    \param deviationType [in]  Deviation type.
    \param pointOnCurve [in]  Point on a curve.
        
    \returns
    recommended maximum difference (with respect to the current active viewport) between the actual curve or surface, 
    and the tessellated curve or surface. 
    
    \remarks
    deviationType must be one of the following:
    
    <table>
    Name                       Value
    kOdGiMaxDevForCircle       0      
    kOdGiMaxDevForCurve        1      
    kOdGiMaxDevForBoundary     2      
    kOdGiMaxDevForIsoline      3
    kOdGiMaxDevForFacet        4
    </table>

    \remarks
    This method uses circle zoom percent as appropriate.  */
  virtual double deviation(
    const OdGiDeviationType deviationType, 
    const OdGePoint3d& pointOnCurve) const = 0;

  /** \details
    Retrieves the number of isolines to be drawn on surfaces during this vectorization.
    
    \returns
    number of isolines.
    \remarks
    Isolines are lines or curves which are used to represent the entity. 
    For example, if an entity is a sphere and this method returns 3, the 
    sphere should be displayed as 3 circles with a common center and planes that are
    perpendicular to each other.
  */
  virtual OdUInt32 numberOfIsolines() const = 0;

  /** \details
    Retrieves context associated with this object.
    
    \returns
    Pointer to the associated context.
  */
  virtual OdGiContext* context() const = 0;

  /** \details
    Retrieves the current drawable nesting graph.
    
    \returns
    current drawable nesting graph.
  */
  virtual const OdGiPathNode* currentGiPath() const;

  /** \details
    Checks whether silhouette curves should be generated (basically should return OdDbDatabase::getDISPSILH,
    but the behavior could be specialized).
    
    \returns
    true if silhouette curves should be generated, false otherwise.
  */
  virtual bool displayModelerSilhouettes() const;
};

/** \details
    Draw flags helper.
    
    \remarks
    Modifies draw flags and restores them back in the destructor.

    Corresponding C++ library: TD_Gi
    
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiDrawFlagsHelper
{
  protected:
    OdGiSubEntityTraits *m_pTraits;
    OdUInt32 m_prevFlags;
  public:
    
    /** \details
    Constructor for the OdGiDrawFlagsHelper class.

    \param pTraits [in/out]  Sub-entity traits to set. Draw flags of this OdGiSubEntityTraits object may change inside the constructor.
    \param addFlags [in]  Draw flags to add.
    \param delFlags [in]  Flags to remove.
  */
    OdGiDrawFlagsHelper(OdGiSubEntityTraits &pTraits, OdUInt32 addFlags, OdUInt32 delFlags = 0)
      : m_pTraits(NULL)
    {
      if (addFlags || delFlags)
      {
        m_prevFlags = pTraits.drawFlags();
        OdUInt32 newFlags = (m_prevFlags & ~delFlags) | addFlags;
        if (m_prevFlags != newFlags)
        {
          pTraits.setDrawFlags(newFlags);
          m_pTraits = &pTraits;
        }
      }
    }
    
    /** \details
    Destructor for the OdGiDrawFlagsHelper class. Sets previous draw flags.
  */
    ~OdGiDrawFlagsHelper()
    {
      if (m_pTraits)
        m_pTraits->setDrawFlags(m_prevFlags);
    }
};

class OdGiSelectionGeomState
{
protected:
  OdGiSubEntityTraits* m_pTraits;
  bool m_bSaved;
public:
  OdGiSelectionGeomState(OdGiSubEntityTraits* pTraits, bool bOn) : m_pTraits(pTraits)
  {
    if (m_pTraits)
    {
      m_bSaved = m_pTraits->selectionGeom();
      m_pTraits->setSelectionGeom(bOn);
    }
  }
  ~OdGiSelectionGeomState()
  {
    if (m_pTraits)
      m_pTraits->setSelectionGeom(m_bSaved);
  }
};
#include "TD_PackPop.h"

#endif

