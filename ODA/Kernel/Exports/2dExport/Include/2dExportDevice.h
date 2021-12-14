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

//////////////////////////////////////////////////////////////////////
//
// 2dExportDevice.h: definition of the Od2dExportDevice, Od2dExportDevice classes.
//                    These are base classes for 2d export.
//
//////////////////////////////////////////////////////////////////////

#ifndef _2DEXPORTDEVICE_INCLUDED_
#define _2DEXPORTDEVICE_INCLUDED_

#if _MSC_VER > 1000
#pragma once 
#endif // _MSC_VER > 1000

#include "Gs/GsBaseInclude.h"
#include "Gi/GiGeometrySimplifier.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiSpatialFilter.h"
#include "Gi/GiPerspectivePreprocessor.h"
#include "Gi/GiHLRemover.h"
#include "Gi/GiPlaneProjector.h"
#include "Gi/GiOrthoClipperEx.h"
#include "Gi/GiMetafiler.h"
#include "../Extensions/ExRender/MetafileTransformStack.h"
#include "BoolArray.h"

/** \details
  <group OdExport_Classes> 
*/
namespace TD_2D_EXPORT
{

/** \details

  This class implements the font parameters for two-dimensional export.
*/
class Od2dFont
{
public:
  Od2dFont()
    : m_bold(false)
    , m_italic(false)
    , m_underline(false)
    , m_charset(1)
    , m_pitch(0)
    , m_family(0)
    , m_height(1.0)
    , m_rotation(0)
    , m_width_scale(1.0)
    , m_oblique(0)
    , m_spacing(1024)
    , m_flags(0)
  {}

  OdString      m_font_name;        // Font name
  bool          m_bold;             // Boldface option
  bool          m_italic;           // Italics option
  bool          m_underline;        // Underline option
  int           m_charset;          // Character set code as defined
  int           m_pitch;            // Pitch bitmask as defined 
  int           m_family;           // Font family as defined 
  double        m_height;
  double        m_rotation;
  double        m_width_scale;
  double        m_oblique;
  double        m_spacing;

  enum
  {
      VERTICAL        = 0x0001      // TVERT
    , MIRROR_X        = 0x0002      // TXMIR
    , MIRROR_Y        = 0x0004      // TYMIR
    , UNDERSCORE      = 0x0008      // TUNDER
    , OVERSCORE       = 0x0010      // TOVER
    , MTEXT_START     = 0x0020      // TMSTART
    , MTEXT_END       = 0x0040      // TMEND
    , MTEXT           = 0x0080      // TMTEXT
    , GREEK_TEXT      = 0x1000      // Whip and GDI font engine only
    , PATH_TEXT       = 0x2000      // Whip and GDI font engine only
    , OUTLINE_TEXT    = 0x4000      // Outline Text Only
  };

  long                        m_flags;
};

//////////////////////////////////////////////////////////////////////   {filename:Od2dExportDevice_Definition}


/** \details

  This class implements the device for two-dimensional export.
*/
class Od2dExportDevice : public OdGsBaseVectorizeDevice, 
                         public OdGiGeometrySimplifier  // Receives 3d data from the ODA 
                                                        //  visualization framework. 
{
  // Members
  //
  struct RenderDeviceInfo
  {
    OdGsDevicePtr m_pDevice;
    OdGsDevicePtr m_pLayout;
    RenderDeviceInfo() { }
  };
  OdArray<RenderDeviceInfo> m_renderDevices;
  bool                      m_bUseHLR;

public:
  enum ImageOrg
  {
      orgBitonal = 1      // 1 bit per pixel
    , orgMapped  = 8      // 8 bits per pixel, paletted
    , orgRGB     = 24     // 24 bits. Red-green-blue
    , orgBGRA    = 32     // 32 bits. Blue-green-red-alpha
  };

  enum DeviceType
  {
    kSupportNothing     = 0,

    kFirstFlag          = 1,

    kSupport2dPolyline  = kFirstFlag << 0,
    kSupport2dCircle    = kFirstFlag << 1,
    kSupport2dEllipse   = kFirstFlag << 2,
    kSupportNrcClip     = kFirstFlag << 3, // nrc clip supported
    kUnicodeOnly        = kFirstFlag << 4,
    kSupportContourFill = kFirstFlag << 5,

    kLastFlag           = kSupportContourFill,

    kDWFDevice          = kSupport2dPolyline|kSupport2dCircle|kSupport2dEllipse,
    kPDFDevice          = kSupport2dPolyline|kUnicodeOnly|kSupportNrcClip|kSupportContourFill
  };

  /** \details
  Shaded viewport export policy
  */
  enum ShadedViewportExportMode
  {
    /** Shaded Viewports will be exported as one-piece image with low resolution*/
    kOnePiece = 0,
    /** Shaded Viewports will be exported as tiled images according to vector resolution (geometry DPI)*/
    kAsTiles = 1,
    /** Shaded Viewports will be exported as striped images according to vector resolution (geometry DPI).*/
    kAsStrips = 2
  };

protected:
  //update() helper
  virtual void calculateRenderDeviceClipping(OdGsDevice *pRenderDevice, OdGeExtents3d& ext2d, OdGeExtents3d& extDevice,
                                             OdIntArray& nrcCounts, OdGsDCPointArray& nrcPoints, int nDevice, bool& bDrawPicture) const;
  virtual bool transformImage(OdGiRasterImagePtr pRastIn, OdGiRasterImagePtr& pRastOut, ODCOLORREF backgroundColor) const;
  int unionNrcClips(int nrcContours1, const int* pNrcCounts1, const OdGsDCPointArray &nrcPoints1, int nrcContours2,
                    const int* pNrcCounts2, const OdGsDCPointArray &nrcPoints2, OdIntArray &nrcCounts, OdGsDCPointArray &nrcPoints) const;

  OdUInt32 m_type;
  OdGiSubEntityTraitsData m_effTraits;
  std::map<OdDbStub *, OdUInt32> m_arrViewports; //viewport Stub, index of device

  ODRX_USING_HEAP_OPERATORS(OdGsBaseVectorizeDevice);
public:

  /** \details
     Constructor.
  */
  Od2dExportDevice(OdUInt32 t);
  ~Od2dExportDevice();

  /** \details
    Sets the deviation.
  */
  virtual void setupSimplifier( const OdGiDeviation* dev )
  {
    OdGiGeometrySimplifier::setDeviation( dev );
  }

  /** \details
    Sets the device support primitives type.
  */
  void setDeviceType(OdUInt32 type) { m_type = type; }

  /** \details
    Returns the device support primitives type as 32-bit value.
  */
  OdUInt32 getDeviceType() const { return m_type; }

  // Set the target data stream and the type.  
  //static OdGsDevicePtr createObject(DeviceType type = /*Od2dExportDevice::*/k3dDevice);

  /** \details
    Pseudo-constructor for device. Returns the smart pointer to the device instance.
  */
  static OdGsDevicePtr createObject();

  /** \details
    Sets the render devices for this device.
  */
  virtual void setRenderDevice(OdGsDevice* pRenderDev = NULL, bool bEnable = true);
  /** \details
    Create render device compatible with specified OdGsView.
  */
  virtual OdGsDevicePtr createRenderDevice(OdGsView* pView);
  /** \details
    Is render device enabled to be created for specified OdGsView.
  */
  virtual bool isEnabledToAddRenderDevice(const OdArray<OdGsViewPtr>& views, int indexView) const;
  /** \details
    Returns number of render devices.
  */
  virtual int numRenderDevices() { return static_cast<int>(m_renderDevices.size()); }
  /** \details
    Returns render device at specified index.
  */
  virtual OdGsDevice* renderDeviceAt(int i) { return m_renderDevices[i].m_pDevice; }
  /** \details
    Returns layout helper for render device at specified index.
  */
  virtual OdGsDevice* renderLayoutHelperAt(int i)
  {
    return m_renderDevices[i].m_pLayout;
  }
  /** \details
    Sets layout helper for render device at specified index.
  */
  virtual void setRenderLayoutHelperAt(int i, OdGsDevice* pLayoutHelper)
  {
    m_renderDevices[i].m_pLayout = pLayoutHelper;
  }

  /** \details
    Checks whether the hidden line remove mode is on.
  */
  virtual bool isUseHLR() const { return m_bUseHLR; } // can be overriden in derived devices

  /** \details
    Switches the hidden line remove mode.
  */
  void setUseHLR(bool val) { m_bUseHLR = val; }

  /** \details
    #9843 : Override it to modify output rendering area dimensions for shaded viewports rendering precision
  */
  virtual double renderAreaMultiplier(OdUInt32 width, OdUInt32 height, ShadedViewportExportMode& mode) const;

  // Attention: there is a need to call own device methods for setting of context 
  //            before setupActiveLayoutViews() or setupLayoutViews() not after! 
  //            Only base interface methods could be called after them.

  // Called by the ODA visualization framework before rendering this view.
  // The override should call the parent version of this function, OdGsBaseVectorizeDevice::update().
  /*!DOM*/ 
  virtual void update(OdGsDCRect* pUpdatedRect) ODRX_OVERRIDE;
  
  // output into device context

  /** \details
    Outputs a point into device.
  */
  virtual void dc_polyline(OdUInt32 , const OdGePoint2d* );

  /** \details
    Outputs a polygon into device.
  */
  virtual void dc_polygon(OdUInt32 , const OdGePoint2d*  );

  /** \details
    Outputs a circle into device.
  */
  virtual void dc_circle( const OdGeEllipArc3d& ellipse  );

  /** \details
    Outputs an ellipse into device.
  */
  virtual void dc_ellipse(const OdGeEllipArc3d& ellipse  );

  /** \details
    Outputs a polytriangle into device.
  */
  virtual void dc_gouraud_polytriangle(
                          const OdGePoint3d* pVertexList,
                          OdInt32 faceListSize,
                          const OdInt32* pFaceList,
                          const OdCmEntityColor* pColors );  
  
  /** \details
    Outputs a raster image into device.
  */
  virtual void dc_raster_image( OdGiRasterImagePtr pRaster, 
                                const OdGeExtents2d& imgExts);

  /** \details
    Outputs a color into device.
  */
  virtual void dc_color( ODCOLORREF );

  /** \details
    Outputs a layer into device.
  */
  virtual void dc_layer( int, const OdString& );

  /** \details
    Pushes a clip into device.
  */
  virtual void dc_pushClip(int nrcContours, const int* pNrcCounts, const OdGsDCPointArray &nrcPoints);

  /** \details
    Pops a clip from device.
  */
  virtual void dc_popClip();

  /** \details
    Returns the supported number of bits per pixel.
  */
  virtual int getSupportedColorBits( int nColorBits, bool bTransparency ) const;

  // ----- The Overrides of OdGiGeometrySimplifier (OdGiConveyorGeometry) -----
  //
  
  /** \details
    Returns a triangle.
    
    
  */
  virtual void triangleOut(const OdInt32* vertices,
    const OdGeVector3d* pNormal);

  /** \details
    Outputs a polyline into device.
  */
  virtual void polylineOut(OdInt32 numPoints, const OdGePoint3d* pVertexList);

  void circleProc2(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    const OdGeVector3d* pExtrusion = 0);

  void circleProc(
            const OdGePoint3d& center, 
            double radius, 
            const OdGeVector3d& normal, 
            const OdGeVector3d* pExtrusion = 0);
  
  void circleProc(
            const OdGePoint3d&, 
            const OdGePoint3d&, 
            const OdGePoint3d&, 
            const OdGeVector3d* pExtrusion = 0);
  
  void circularArcProc(
            const OdGePoint3d& center,
            double radius,
            const OdGeVector3d& normal,
            const OdGeVector3d& startVector,
            double sweepAngle,
            OdGiArcType arcType = kOdGiArcSimple, 
            const OdGeVector3d* pExtrusion = 0);
  
  void circularArcProc(
            const OdGePoint3d& start,
            const OdGePoint3d& point,
            const OdGePoint3d& end,
            OdGiArcType arcType = kOdGiArcSimple, 
            const OdGeVector3d* pExtrusion = 0);
  
  void polygonProc(
            OdInt32 nbPoints, 
            const OdGePoint3d* pVertexList,
            const OdGeVector3d* pNormal = 0,
            const OdGeVector3d* pExtrusion = 0);

  void textProc(
            const OdGePoint3d& position,
            const OdGeVector3d& u, 
            const OdGeVector3d& v,
            const OdChar* msg, 
            OdInt32 length, 
            bool raw, 
            const OdGiTextStyle* pTextStyle,
            const OdGeVector3d* pExtrusion = 0);
  
  void xlineProc(
            const OdGePoint3d&, 
            const OdGePoint3d&);
  
  void rayProc(
            const OdGePoint3d&, 
            const OdGePoint3d&);
  
  void ellipArcProc(
            const OdGeEllipArc3d& arc,
            const OdGePoint3d* pEndPointsOverrides,
            OdGiArcType arcType,
            const OdGeVector3d* pExtrusion);

  // Utilitary functions
  virtual void putEllipse(OdGeEllipArc3d& ellipse);
  virtual void shxText( const OdGePoint3d& /*position*/,
                        const OdGeVector3d& /*u*/,
                        const OdGeVector3d& /*v*/,
                        const OdChar* /*msg*/,
                        OdInt32 /*length*/,
                        bool /*raw*/,
                        const OdGiTextStyle* /*pStyle*/,
                        const OdGeVector3d* /*pExtrusion*/ )
  {
    // Here is the place for additional processing of SHX text in case of a need
  }
  bool textProcessing( const OdGePoint3d& position,
                       const OdGeVector3d& u,
                       const OdGeVector3d& v,
                       const OdChar* msg,
                       OdInt32 length,
                       bool raw,
                       const OdGiTextStyle* pStyle,
                       OdString& pProcessedText,
                       OdArray<OdUInt16>& arrayUnicode,
                       bool bASCII,
                       OdBoolArray *isInBigFont = NULL,
                       OdGePoint2dArray *pAdvances = NULL);

  virtual void onTraitsModified(const OdGiSubEntityTraitsData& traits);

private:
  //update() helper
  void exportAsPicture(OdGsDevice *pRenderDevice, const OdGeExtents3d& ext2d, const OdGeExtents3d& extDevice,
                       const OdIntArray& nrcCounts, const OdGsDCPointArray& nrcPoints, OdUInt32 wholeWidth, OdUInt32 wholeHeight, ShadedViewportExportMode exMode);

  bool viewHasMatchingRenderDevice(const OdGsView* pView, OdUInt32 deviceIdx) const;
  void setupViewsVisibility();
}; // end Od2dExportDevice

/** \details

   This class implements the view for two-dimensional export.
*/
class Od2dExportView :   public OdGsBaseVectorizeViewDef
{
protected:
  OdGiMetafilerPtr           m_pMetafiler;
  OdGiXformPtr               m_pMfPlay;
  OdGiConveyorConnector      m_mfPlayEntryPoint;
  MetafileTransformStack     m_mfTransformStack;

public:
  OdGiOrthoClipperExPtr   m_pPerspPrepClip;
  OdGiOrthoClipperExPtr   m_pViewportClip;
  OdGiXYProjectorPtr      m_pXYProjector;

  OdGiXformPtr                   m_pPerspXform;
  OdGiPerspectivePreprocessorPtr m_pPerspPrepr;

  OdGiHLRemoverPtr m_pHLRemover;

public:
  // Returns the ExGsSimpleDevice instance that owns this view. 
  Od2dExportDevice* device()
  { 
    return (Od2dExportDevice*)OdGsBaseVectorizeView::device();
  }

  // Returns the ExGsSimpleDevice instance that owns this view. 
  Od2dExportDevice* device() const
  {
    return (Od2dExportDevice*)OdGsBaseVectorizeView::device();
  }

  // Constructor. 
  Od2dExportView();

  friend class Od2dExportDevice;

public:
  OdGiConveyorOutput& screenOutput() { return m_pXYProjector->output(); }

  // to update conveyor if view is perspective
  virtual void setView(const OdGePoint3d & position,
    const OdGePoint3d& target,
    const OdGeVector3d& upVector,
    double fieldWidth,
    double fieldHeight,
    Projection projection = kParallel);

  virtual void update();

  // Called by the ODA visualization framework to give the 
  // client application a chance to terminate the current 
  // rendering process.  Returning true from this function will 
  // stop the current rendering operation. 
  bool regenAbort() const;

  // --------  Od2dExportView  --------
  //
  virtual void onTraitsModified();

  // Retrieves the current rendering traits (color, linewidth, etc.) 
  //  and sets these properties in this device. 

  virtual void beginViewVectorization();
  virtual void endViewVectorization();
  virtual void loadViewport();

  virtual bool isTransparency(OdGeVector3d screenU, OdGeVector3d screenV);

  virtual void ownerDrawDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiSelfGdiDrawable* pDrawable,
    bool bDcAligned = true,
    bool bAllowClipping = false);

  virtual void setText( const OdGePoint3d& /*position*/,
                        const OdGeVector3d& /*u*/,
                        const OdGeVector3d& /*v*/,
                        const OdChar* /*msg*/, OdInt32 /*length*/, bool /*raw*/,
                        const OdGiTextStyle* /*pTextStyle*/,
                        bool  /*bBox*/,
                        const Od2dFont* /*pFont*/ = NULL,
                        const OdArray<OdUInt16>* /*pUnicode*/ = NULL )
  {}

  //////////////////////////////////////////////////////////////////////////caching
  /** \details
  Creates a new GsMetafile (cache) object.
  \remarks
  Returns a SmartPointer to the newly created object.
  */
  virtual OdRxObjectPtr newGsMetafile();
  /** \details
    Begins vectorization to the specified GsMetafile (cache) object.
    \param pMetafile [in]  Pointer to the metafile.
  */
  virtual void beginMetafile(OdRxObject* pMetafile);
  /** \details
    Ends vectorization to the specified GsMetafile (cache) object.
    \param pMetafile [in]  Pointer to the metafile.
  */
  virtual void endMetafile(OdRxObject* pMetafile);
  /** \details
    Plays (draws) the specified Metafile (cache) object.
    \param pMetafile [in]  Pointer to the metafile.
  */
  virtual void playMetafile(const OdRxObject* pMetafile);

  /** \details
    Pushes transformation matrix into MetafileTransformStack.
    \param mtx [in]  matrix.
    \param uFlags [in]  MetafileTransformFlags.
  */
  virtual void pushMetafileTransform(const OdGeMatrix3d& mtx, OdUInt32 uFlags = 0);
  /** \details
    Pop transformation matrix from MetafileTransformStack.
    \param uFlags [in]  MetafileTransformFlags.
  */
  virtual void popMetafileTransform(OdUInt32 uFlags = 0);
  /** \details
  Returns transformation matrix from metafile.
  */
  virtual OdGeMatrix3d metafileTransform() const { return m_mfTransformStack.metafileTransform(); };
  virtual const OdGeMatrix3d &getMetafileTransform() const { return m_mfTransformStack.metafileTransform(); };

  /** \details
  Returns output for metafile.
  */
  virtual OdGiConveyorOutput& outputForMetafileGeometry() { return m_pMfPlay->output(); }
  /** \details
    Set transformation matrix for metafile output.
    \param mtx [in]  matrix.
  */
  virtual void setTransformForMetafileGeometry(const OdGeMatrix3d& mtx);
  /** \details
  Returns transformation matrix for metafile output.
  */
  virtual OdGeMatrix3d getTransformForMetafileGeometry() const;

  /** \details
  Returns if model cache is enabled.
  */
  inline bool isModelCacheEnabled() const
  {
    return (view().cachedDrawables() != 0 && device()->supportBlocks());
  };

  virtual bool useMetafileAsGeometry() const { return isModelCacheEnabled(); }
  
}; // end Od2dExportView 

} // TD_2D_EXPORT namespace

#endif  // _2DEXPORTDEVICE_INCLUDED_
