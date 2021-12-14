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
//
//////////////////////////////////////////////////////////////////////

#ifndef _PDF_2D_EXPORT_VIEW_INCLUDED_
#define _PDF_2D_EXPORT_VIEW_INCLUDED_

#if _MSC_VER > 1000
#pragma once 
#endif // _MSC_VER > 1000

#include "2dExportDeviceForPdfExport.h"
#include "RxObjectImpl.h"
#include "OdPlatform.h"
#include "Gi/GiPlotGenerator.h"
#include "PdfExportClipper.h"
#include "Pdf2dExportDevice.h"

using namespace TD_PDF;

//////////////////////////////////////////////////////////////////////

/** \details
  Contains declarations for 2D PDF Export.
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT
{
struct ImageData;
struct HyperlinkData;

/** \details
  This class implements the two-dimensional export view.
*/
class PDF2dExportView : public Od2dExportView
{
//DOM-IGNORE-BEGIN
  typedef Od2dExportView super;
protected:
  void exportTraits();
  virtual void endMetafileRecording() ODRX_OVERRIDE;
  void beginMetafile(OdRxObject* pMetafile) ODRX_OVERRIDE;

  /** \details
  Retrieves a hyperlink data if it exists.
  \param pDrawable  [in] A raw pointer to the Gi drawable object.
  \param dataToFill [out] A placeholder for hyperlink data.
  \returns true if the method successfully returns the hyperlink data; otherwise, the method returns false (for example, if no hyperlink data exists).
  */
  bool getHyperlink(const OdGiDrawable* pDrawable, HyperlinkData& dataToFill) const;

  /** \details
    Plays (draws) the specified Metafile (cache) object.
    \param pMetafile [in]  Pointer to the metafile.
  */
  void playMetafile(const OdRxObject* pMetafile) ODRX_OVERRIDE;

  /** \details
    Service method to process layers for inserts.

    \param pLayer [in]  Pointer to a layer.
    \param bOpen  [in]  flag if layer should be opened or closed.
  */
  bool processLayerFromMf(OdDbStub* layer, bool bOpen);

  /** \details
  Passes a hyperlink to the meta-filer.
  */
  void passHyperlinkToMetafiler();

  /** \details
  Adds a text annotation if it is necessary.
  \param pDrawable [in] A raw pointer to the Gi drawable entity.
  \param layerName [in] A string that contains the layer name where the annotation should be allocated.
  */
  void processSearchableText(const OdGiDrawable* pDrawable, const OdString &layerName = OdString());

  /** \details
  Passes a text annotation to the meta-filer.
  \param layerName [in] A string that contains the layer name where the annotation is allocated.
  */
  void passTextAnnotationToMetafiler(const OdString &layerName);

  /** \details
  Processes PRC content from a specified Gi drawing entity.
  \param pDrawable [in] A raw pointer to the Gi drawable entity.
  */
  void processPRC(const OdGiDrawable* pDrawable);

  /** \details
  Pushes a clip to the viewport.
  */
  void pushViewportClip();
  
  /** \details
  Pops a clip from the viewport.
  */
  void popViewportClip();

  /** \details
  Retrieves the layer name from an identifier.
  \param layer [in] A raw pointer to a stub object that represents the identifier.
  \returns A string that contains the layer name.
  */
  OdString layerName(OdDbStub* layer);

  /** \details
  Views extended specified block.
  \param extents [in]  A reference to the OdGeBoundBlock3d class instance that must be viewed.
  \returns
  Returns true if the view is successfully done or false in the other case.
  */
  bool viewExtents(OdGeBoundBlock3d& extents) const;

  void setAnnotationText(const OdChar* msg, OdInt32 length, const OdGiTextStyle* pStyle);
  bool drawHatchAsBitmap(const OdGeExtents3d& extents, OdUInt32 clipStat,
    OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize,
    const OdInt32* faceList, const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData);
  void drawHatchAsPaths(OdDbBaseHatchPE& hatchPE, OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize,
    const OdInt32* faceList, const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData);
  void drawImages(const ImageData* aImg, OdUInt32 nImg);

  /** \details
  Set fill rule for polygons.
  \param rule [in] A rule for polygons.
  */
  void setFillRule(PDF2dExportGeometry::PolygonFillRules);
  void setDrawingContour(bool bStart);
  void fillContour();
  void enableViewportClip(bool bEnable);
  void processFrozenLayer(const OdString& name = L""); //if name == "", then close layer

  OdInt32 calculateTextLen(const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pStyle);

  bool isBlockRef(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable);
//DOM-IGNORE-END

public:

  /** \details
  Creates a new export view object with default parameters.
  */
  PDF2dExportView() 
    : m_bHatchToBitmapMode(false)
    , m_bPRCMode(false)
    , m_bAdditionalClip(false)
    , m_annotationText("")
    , m_bRestartClipper(false)
    , m_bDisableColorMerge(false)
    , m_bValidMfToDeviceMatrix(false)
    , m_bTextGeomConnected(false)
    , m_curHl(NULL)
  {
    setDrawContextFlags(OdGiConveyorContext::kPlineAllowArcProc, true);
  }

  /** \details
  Retrieves the geometry for the current view.
  \returns A raw pointer to the PDF2dExportGeometry object for the current view.
  */
  PDF2dExportGeometry* deviceGeom() { return (PDF2dExportGeometry*)m_deviceGeom; }

  /** \details
  Determines whether the meta-file is playing for the display.
  \returns true if the metafile is playing for the display; otherwise, the method returns false.
  */
  bool isPlayingMfForDisplay() const { return metafilePlayMode() == kMfDisplay; }

  /** \details
  Retrieves the current Gs note entity for the view.
  \returns A raw pointer to the OdGsEntityNode object that represents the current node object for the view.
  */
  const OdGsEntityNode* mfPlayNode() const { return m_MfPlayNode; }

  /** \details
  Retrieves a matrix for meta-file to display transformation.
  \returns An OdGeMatrix3d object that represents the meta-file to display transformation.
  */
  OdGeMatrix3d metafileToDeviceMatrix() const;

  /** \details
    Plays (redraws) the specified Nested metafile (cache) object.
    \param pPortion [in]  Pointer to the geometry portion.
    \param pContext [in]  Pointer to the Gs context.
    \param node     [in]  Reference to the Gs node.
    \remarks
    GsMetafiles are used to record vectorization, and rapidly 'redraw' them to a display device.
  */
  virtual bool playNestedMetafile(const OdGsGeomPortion* pPortion, OdGsBaseContext* pContext, OdGsEntityNode& node) ODRX_OVERRIDE;

  /** \details
  Traits modifying handler for the export view.
  \remarks
  Process modified traits with specified data for the view.
  */
  void onTraitsModified();

  /** \details
  Creates a new view object.
  \returns A smart pointer to the created view object.
  */
  static OdGsViewPtr createObject()
  {
    return OdRxObjectImpl<PDF2dExportView, OdGsView>::createObject();
  }

  /** \details
  Retrieves the export device associated with the export view.
  \returns A raw pointer to the PDF2dExportDevice object that represents the export device associated with the export view.
  */
   PDF2dExportDevice* device()
  { 
    return (PDF2dExportDevice*)Od2dExportView::device();
  }

   /** \details
  Retrieves the export device associated with the export view.
  \returns A raw pointer to the PDF2dExportDevice object that represents the export device associated with the export view.
  */
   PDF2dExportDevice* device() const
   {
     return (PDF2dExportDevice*)Od2dExportView::device();
   }

   /** \details
   Retrieves a plot generator for the export view.
   \returns a raw pointer to the OdGiPlotGenerator that represents the plot generator for the export view. 
   */
   OdGiPlotGenerator& plotGenerator();

  // --------  Od2dExportView  --------
  
  /** \details
  Starts the vectorization of the export view.
  */
  virtual void beginViewVectorization() ODRX_OVERRIDE;
  
  /** \details
  Updates the vectorization of the export view.
  */
  virtual void update() ODRX_OVERRIDE;
  
  /** \details
  Draws the specified drawable entity with specified flags.
  \param drawableFlags [in] A value that contains drawable flags.
  \param pDrawable     [in] A raw pointer to the drawable entity.
  \returns true if the drawing was successful; otherwise the method returns false.
  */
  virtual bool doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable) ODRX_OVERRIDE;
  
  /** \details
  Loads the viewport.
  */
  virtual void loadViewport() ODRX_OVERRIDE;
  
  /** \details
  Ends the vectorization of the export view.
  */
  virtual void endViewVectorization() ODRX_OVERRIDE;
  
  /** \details
  Retrieves the transformation for the meta-file geometry.
  \returns An OdGeMatrix3d object that represents the transformation.
  */
  virtual OdGeMatrix3d getTransformForMetafileGeometry() const ODRX_OVERRIDE;

  /** \details
  Creates a shell object.
  \param numVertices  [in] A number of the shell vertices.
  \param vertexList   [in] A raw pointer to the vertices array.
  \param faceListSize [in] A number of the shell faces.
  \param faceList     [in] A raw pointer to the face indexes.
  \param pEdgeData    [in] A raw pointer to the edge data.
  \param pFaceData    [in] A raw pointer to the face data.
  \param pVertexData  [in] A raw pointer to the vertex data.
  */
  virtual void shell(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData) ODRX_OVERRIDE;

  /** \details
  Creates a text object.
  \param position   [in] A point that defines the text position.
  \param normal     [in] A vector that determines the text normal vector.
  \param direction  [in] A vector that determines the text direction.
  \param msg        [in] A raw pointer to the char array that contains the related message text.
  \param length     [in] A length of the message text.
  \param raw        [in] A raw flag value.
  \param pTextStyle [in] A raw pointer to the text style object.
  */
  virtual void text(const OdGePoint3d& position,
    const OdGeVector3d& normal,
    const OdGeVector3d& direction,
    const OdChar* msg,
    OdInt32 length,
    bool raw,
    const OdGiTextStyle* pTextStyle) ODRX_OVERRIDE;

  /** \details
  Passes the object to the visualization framework to render itself. 
  \param origin         [in] The object origin.
  \param u              [in] The object width vector.
  \param v              [in] The object length vector.
  \param pDrawable      [in] A raw pointer the drawable entity. 
  \param bDcAligned     [in] A reserved. You can omit this parameter when calling the method.
  \param bAllowClipping [in] A reserved. You can omit this parameter when calling the method.
  */
  virtual void ownerDrawDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiSelfGdiDrawable* pDrawable,
    bool bDcAligned = true,
    bool bAllowClipping = false) ODRX_OVERRIDE;

  virtual void nurbs(const OdGeNurbCurve3d& nurbs) ODRX_OVERRIDE;

  virtual double linetypeGenerationCriteria() const ODRX_OVERRIDE;

  /** \details
  Determines whether the geometry can be processed.
  \returns true if the geometry can be processed; otherwise, the method returns false.
  \remarks
  Call this method to request a preliminary loop for viewing and processing of current settings without geometry processing.
  */
  bool isProcessGeom() { return true; }

  /** \details
  Calculate the deviation of the specified type for the specified point.
  \param type [in] A deviation type (an instance of the OdGiDeviationType class).	
  \param pt   [in] A point for which the deviation is needed to be calculated represented with an instance of the OdGePoint3d class.
  \returns true if the deviation was successfully calculated; otherwise, the method returns false.
  */
  double calcScaledDeviation(const OdGiDeviationType type, const OdGePoint3d& pt) const;
  
  /** \details
  Retrieves the deviation of the specified type for the specified point.
  \param type [in] A deviation type (an instance of the OdGiDeviationType class).	
  \param pt   [in] A point for which the deviation is needed to be calculated represented with an instance of the OdGePoint3d class.
  \returns The current deviation value for the specified point.
  */
  virtual double deviation(const OdGiDeviationType type, const OdGePoint3d& pt) const;

  /** \details
  Handles the viewport clipping (used for texts and hatches as pdf paths).
  \param bEnable [in] A flag value that enables (if equals to true) or disables (if equals to false) the clipping.
  */
  void enableClipper(bool bEnable)
  {
    if (bEnable)
      m_pViewportClip->enable();
    else
      m_pViewportClip->disable();
  }

  /** \details
  Disables the color merge (used to export texts wipeouts via metafile).
  */
  void disableColorMerge()
  {
    m_bDisableColorMerge = true;
  }

  /** \details
  Passes the hyperlink to the device geometry (pdf).
  \param data [in] A hyperlink data.
  \param ext  [in] An extents object.
  \param bFromMf [in] A flag value that determines whether the hyperlink is passed from the meta-file.
  */
  void passHyperlinkToGeometry(const HyperlinkData& data, const OdGeExtents3d& ext, bool bFromMf);

  /** \details
  Passes the text annotation to the device geometry (pdf).
  \param sText     [in] A string that contains the annotation text.
  \param layerName [in] A string that contains the layer name.
  \param ext       [in] An extents object.
  \param bFromMf   [in] A flag value that determines whether the text annotation is passed from the meta-file.
  */
  void passTextAnnotationToGeometry(const OdString& sText, const OdString &layerName, const OdGeExtents3d& ext, bool bFromMf);

  /** \details
  Checks whether the displaying stage of the export process is in progress.
  \returns true if the printing to a PDF string is in progress; otherwise, the method returns false.
  */
  bool isDisplaying() const
  {
    return GETBIT(vectorizingFlags(), kDisplaying);
  }

  /** \details
  Check whether the entity is clipped by viewport at the displaying stage.
  \param extents [in] An OdGeExtents3d object that represents the entity extents.
  \returns A OdGiOrthoClipperEx::VisibilityStatus value.
  */
  OdGiOrthoClipperEx::VisibilityStatus checkViewportClipping(const OdGeExtents3d &extents) const
  {
    if (!m_pViewportClip.isNull())
      return m_pViewportClip->checkExtentsVisibility(extents);
    else
      return OdGiOrthoClipperEx::kStatusVisible;
  }

  /** \details
  Returns or creates GsDevice
  \returns pointer to GsDevice.
  */
  OdGsDevicePtr getBitmapDevice();

//DOM-IGNORE-BEGIN
  friend class PDF2dExportDevice;

private:
  bool doDrawImp(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable, const OdString &layerName);
  bool drawHatch(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable, OdDbBaseHatchPE& hatchPE);
  bool drawSolidHatch(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable, OdDbBaseHatchPE& hatchPE);

  void doDrawText(const OdGePoint3d& position,
    const OdGeVector3d& normal,
    const OdGeVector3d& direction,
    const OdChar* msg,
    OdInt32 length,
    bool raw,
    const OdGiTextStyle* pTextStyle,
    bool bAsGeom);
  void switchTextOnlyGeomForDisplay(bool bOn);
  bool isSearchableText(const OdGiTextStyle* pStyle) const;

  void saveHatchToMetafiler(OdInt32 numVertices, const OdGePoint3d* vertexList,
    OdInt32 faceListSize, const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData);

  bool processSolidShell(
    const OdGeExtents3d& extents,
    OdUInt32 clipStat,
    OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData);

  HyperlinkData* m_curHl; //hyperlink data of an innermost object for which doDraw() is executed (null if no hyperlink)
  OdString                m_annotationText; //collector for text annotation for searchable text

  OdGeMatrix3d            m_WorldToCameraMatrix; // matrix of transformation from world to camera coordinates

  OdGiPlotGeneratorPtr    m_pPlotstyler;  // Plot style generator.
  mutable OdGiDrawablePtr m_pSolidHatch;  // Smart pointer to /OdDbHatch/ class instance implementing solid hatch mode.
  OdGsDevicePtr           m_bitmapDevice; // Bitmap device to draw entities as images

  mutable bool            m_bPRCMode;
  mutable bool            m_bHatchToBitmapMode; // Determines if the view can be hatched to bitmap mode (*true*) or can not (*false*).
  bool                    m_bAdditionalClip;
  bool                    m_bRestartClipper;
  bool                    m_bDisableColorMerge; //Color merge flag
  bool                    m_bTextGeomConnected;
  mutable bool            m_bValidMfToDeviceMatrix;
  mutable OdGeMatrix3d    m_mfToDeviceMatrix;
  OdGsDCRect              m_screenRect;
  OdGiConveyorConnector   m_textOnlyEntryPointForDisplay;  //Support searchable text in rendered views
//DOM-IGNORE-END
};  // end PDF2dExportView

}
#endif // #ifndef _PDF_2D_EXPORT_VIEW_INCLUDED_
