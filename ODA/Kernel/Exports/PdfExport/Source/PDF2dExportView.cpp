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


#include "PdfExportCommon.h"

#include "Pdf2dExportView.h"
#include "OdRound.h"

#include "AbstractViewPE.h"
#include "RxVariantValue.h"
#include "ColorMapping.h"

#include "Gi/GiRasterWrappers.h"
#include "Ge/GeScale3d.h"
#include "Gi/GiDrawableImpl.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeCircArc2d.h"
#include "Ge/GeEllipArc3d.h"

#include "OleItemHandler.h"
#include "RxRasterServices.h"
#include "MemoryStream.h"

#include "PrcContextForPdfExport.h"
#include "Pdf2PrcExportParams.h"

#include "Ge/GeExtents3d.h"
#include "Gs/GsDbRootLinkage.h"
#include "Gi/GiPathNode.h"
#include "Gi/GiShmDataStorage.h"
#include "DynamicLinker.h"
#include "DbHyperlink.h"

#include "Gs/GsBlockReferenceNode.h"
#include "Gs/GsLayerNode.h"

#include "Pdf2dExportGsCacheSupport.h"
#include "PdfExport.h"
#include "PdfExportPE.h"


#if defined(ODA_WINDOWS) && !defined(_WINRT)  && !defined(_WIN32_WCE)
#include "../../../Extensions/ExRender/win/GdiBmp.h"
#else
#define drawWithGdiBmp(pDrawable, pVect, origin, u, v, bitmapWidth, bitmapHeight, rect, bMonochrome)
#endif

namespace TD_PDF_2D_EXPORT
{

OdUInt32 clipStatus(const OdGeExtents3d& extents, OdGiConveyorGeometry& geom)
{
  OdGeBoundBlock3d bbChk(extents.minPoint(), extents.maxPoint());
  OdUInt32 bbStatus = 0;
  geom.conveyorBoundaryInfoProc(bbChk, bbStatus);
  return bbStatus;
}

bool isFullyClipped(const OdGeExtents3d& extents, OdGiConveyorGeometry& geom)
{
  return GETBIT(clipStatus(extents, geom), OdGiConveyorGeometry::kBoundaryProcClipFull);
}

bool isPartiallyClipped(const OdGeExtents3d& extents, OdGiConveyorGeometry& geom)
{
  return GETBIT(clipStatus(extents, geom), OdGiConveyorGeometry::kBoundaryProcClip);
}

OdGeMatrix3d PDF2dExportView::getTransformForMetafileGeometry() const
{
  return metafileTransform();
  /*OdGeMatrix3d m;
  m_pMfPlay->transform(m);
  return m;*/
}

OdString PDF2dExportView::layerName(OdDbStub* layer)
{
  if(!layer)
    return OdString();
  OdRxObjectPtr pLayer(userGiContext()->openDrawable(layer));
  if(pLayer.isNull())
    return OdString();
  OdString strLayerName = OdDbBaseLayerPEPtr(pLayer)->name(pLayer);
  return strLayerName;
}

void PDF2dExportView::onTraitsModified()
{
  Od2dExportView::onTraitsModified();
  if(m_pMetafiler->metafile())
  {
    const OdGiSubEntityTraitsData& entityTraits = effectiveTraits();
    if(entityTraits.selectionGeom() && !m_pDetachedOutput) //do not put the selection geometry into metafile
    {
      ODA_ASSERT(!m_pDetachedOutput);
      m_pDetachedOutput = &(output().destGeometry());
      output().setDestGeometry(OdGiEmptyGeometry::kVoid);
      return;
    }
    else if(!entityTraits.selectionGeom() && m_pDetachedOutput)
    {
      ODA_ASSERT(!!m_pDetachedOutput);
      output().setDestGeometry(*m_pDetachedOutput);
      m_pDetachedOutput = NULL;
    }

    ODA_ASSERT(!deviceGeom()); //no geometry, recording metafile
    m_bDisableColorMerge = GETBIT(OdGiBaseVectorizer::drawableAttributes(), OdGiDrawable::kDrawableMergeControlOff);
    if(m_bDisableColorMerge)
    {
      m_pMetafiler->add(new RecDisableColorMerge());
      m_bDisableColorMerge = false;
    }
    m_pMetafiler->saveTraits(effectiveTraits(), byBlockTraits());
    //m_pMetafiler->flush();
    // All next code is used during display only. It's redundant to call it if we process metafile.
    return;
  }
  if(deviceGeom())
    exportTraits();
}

void PDF2dExportView::passHyperlinkToMetafiler()
{
  if(m_pMetafiler->metafile())
    m_pMetafiler->add(new RecHyperlink(*m_curHl));
}

void PDF2dExportView::beginMetafile(OdRxObject* pMetafile)
{
  super::beginMetafile(pMetafile);
}

void PDF2dExportView::endMetafileRecording()
{
  if(deviceGeom() || !m_curHl)
  {
    super::endMetafileRecording();
    return;
  }

  if(!m_pMetafiler->metafile() && gsWriter().isRecordingMetafile())
  {
    onTraitsModified(); //forces creation of a new geometry portion
    ODA_ASSERT(m_pMetafiler->metafile());
  }
  if(m_pMetafiler->metafile())
  {
    if(m_curHl)
    {
      passHyperlinkToMetafiler();
      m_curHl = NULL; //this indicates that the data have been processed
    }
  }
  super::endMetafileRecording();
}

void PDF2dExportView::playMetafile(const OdRxObject* pMetafile)
{
  //reset disable color merge flag, the next entity will set it, if necessary
  if(GETBIT(vectorizingFlags(), kDisplaying))
    m_bDisableColorMerge = false;
  super::playMetafile(pMetafile);
}

bool PDF2dExportView::playNestedMetafile(const OdGsGeomPortion* pPortion, OdGsBaseContext* pContext, OdGsEntityNode &node)
{
  if(!pPortion)
    return false;

  OdGsLayerNode* pLayer = pPortion->m_pLayer;
  bool bRes = (pLayer != 0);
  bool bPlayed = false;

  if(bRes)
    bRes = processLayerFromMf(pLayer->underlyingDrawableId(), true);
  bPlayed = super::playNestedMetafile(pPortion, pContext, node);
  if(bRes)
    processLayerFromMf(pLayer->underlyingDrawableId(), false);

  return bPlayed;
}

bool PDF2dExportView::processLayerFromMf(OdDbStub* layer, bool bOpen)
{
  if(!layer)
    return false;

  const bool isOffLayersEnabled = device()->isLayersSupported() && device()->isOffLayersEnabled();
  if(!isOffLayersEnabled || !device()->frozenLayerSimulationEnabled())
    return false;

  OdString name = layerName(layer);
  if(!device()->isFrozenLayer(name))
      return false;

  if(bOpen)
    name += PDF_FROZEN_LAYER;
  else
    name.empty();
  processFrozenLayer(name);
  return true;
}

void PDF2dExportView::exportTraits()
{
  ODA_ASSERT(deviceGeom()); //it is either direct output or playing cache
  //////////////////////////////////////////////////////////////////////////
  if (!deviceGeom()->recordingEnabled() && deviceGeom()->currentPdfPage())
    deviceGeom()->enableRecording();

  const OdPsPlotStyleData& plotStyle = effectivePlotStyle();
  const OdGiSubEntityTraitsData& entityTraits = effectiveTraits();

  OdCmTransparency tr = entityTraits.transparency();
  OdUInt8 alpha = tr.alpha();
  double  alphaPercent = tr.alphaPercent();
  if (alphaPercent == 0)
  {
    deviceGeom()->enableRecording(false, false); //we shouldn't plot completely transparent entities
    return;
  }

  // color
  if (userGiContext()->isPlotGeneration())
  {
    if (plotStyle.color().isByColor())
      deviceGeom()->set_Color(ODTOCOLORREF(plotStyle.color()));
    else if (plotStyle.color().colorMethod() != OdCmEntityColor::kNone)
      deviceGeom()->set_ColorIndex(plotStyle.color().colorIndex());
    //else continue export with previous color
  }
  else
  {
    if (entityTraits.trueColor().isByColor())
      deviceGeom()->set_Color(ODTOCOLORREF(entityTraits.trueColor()));
    else if (entityTraits.trueColor().colorMethod() != OdCmEntityColor::kNone)
      deviceGeom()->set_ColorIndex(entityTraits.color());
    //else continue export with previous color
  }

  deviceGeom()->set_Layer(entityTraits.layer(), userGiContext());

  bool bLwDisplay = view().hasLweights() || view().lineweightToDcScale() != 0.0 || hasLineweightOverride();
  double lw = OdDb::kLnWt000;

  if (userGiContext()->isPlotGeneration() && userGiContext()->plotStyleType() != OdGiContext::kPsNone)
  {
    //LineWeight
    lw = lineweightToPixels(plotStyle.lineweight());

    OdPsPlotStyleData zeroLwPlotStyle = plotStyle;
    zeroLwPlotStyle.setLineweight(0);
    plotGenerator().setPlotStyle(zeroLwPlotStyle);
    plotGenerator().enable(true);

    OdPs::LineEndStyle  lnEndStyle = plotStyle.endStyle();
    OdPs::LineJoinStyle lnJoinStyle = plotStyle.joinStyle();
    if (hasLinestyleOverride())
    {
      lnEndStyle  = currentLineweightOverride()->endStyleOverride();
      lnJoinStyle = currentLineweightOverride()->joinStyleOverride();
    }
    TD_PDF::PDFLineCap  uLineCap = kRoundCap;
    TD_PDF::PDFLineJoin uLineJoin = kRoundJoin;
    if (lnEndStyle == OdPs::kLesSquare)
      uLineCap = kProjectingSquareCap;
    else if (lnEndStyle == OdPs::kLesButt)
      uLineCap = kButtCap;
    if (lnJoinStyle == OdPs::kLjsMiter)
      uLineJoin = kMiterJoin;
    else if (lnJoinStyle == OdPs::kLjsBevel)
      uLineJoin = kBevelJoin;
    deviceGeom()->setCapJoin(uLineCap, uLineJoin);
  }
  else
  {
    lw = lineweightToPixels(entityTraits.lineWeight());
    plotGenerator().enable(false);
  }

  if (deviceGeom()->isDrawingContour()) //if (!m_pSolidHatch.isNull())
    lw = OdDb::kLnWt000; //for hatch we will draw just contour, so there is no need for lw, even if we won't stroke the path (kFillStrokeCloseRule or other rule)

  if (deviceGeom()->exportParams().archived() != PDFExportParams::kPDFA_1b)
  {
    const bool bPlotTransparency = isPlotTransparency();
    if ((!m_bDisableColorMerge) && !isPlayingMetafile())
      m_bDisableColorMerge = GETBIT(OdGiBaseVectorizer::drawableAttributes(), OdGiDrawable::kDrawableMergeControlOff);
    const bool bMergeLines = GETBIT(deviceGeom()->exportParams().exportFlags(), PDFExportParams::kMergeLines);

    if (bMergeLines)
      deviceGeom()->dc_transparency(1, 255, PDF2dExportGeometry::kDarken); //cannot cache blend mode and transparency groups now, because they are stackable

    if (bPlotTransparency/* && !(bDisableColorMerge && alpha == 255 && bMergeLines)*/)
    {
      if (bMergeLines && !m_bDisableColorMerge && alpha == 255)
        deviceGeom()->dc_transparency(1, 255, PDF2dExportGeometry::kRestoreDarken);
      else
        deviceGeom()->dc_transparency(alphaPercent, alpha, PDF2dExportGeometry::kTransparency);
    }
    else if (bMergeLines)
    {
      if (m_bDisableColorMerge)
        deviceGeom()->dc_transparency(1, 255, PDF2dExportGeometry::kTransparency);
      else
        deviceGeom()->dc_transparency(1, 255, PDF2dExportGeometry::kRestoreDarken);
    }
    m_bDisableColorMerge = false;
  }

  if (bLwDisplay)
    deviceGeom()->set_LineWeight(OdRoundToLong(lw));
  else
    deviceGeom()->set_LineWeight(OdDb::kLnWt000); // Avoid dynamic sublists creation if 'lwdisplay' disabled, but now regeneration needed if 'lwdisplay' sysvar changed.
  
  // secondary color
  if( GETBIT(entityTraits.drawFlags(), OdGiSubEntityTraits::kDrawContourFill) )
  {
    OdPsPlotStyleData plotStyleSecondary;

    if( userGiContext()->isPlotGeneration() )
    {
      OdCmEntityColor oldColor = m_effectiveEntityTraitsData.trueColor();
      m_effectiveEntityTraitsData.setTrueColor(entityTraits.secondaryTrueColor());
      plotStyleSecondary = effectivePlotStyle();
      m_effectiveEntityTraitsData.setTrueColor(oldColor);
    }

    if( userGiContext()->isPlotGeneration() )
    {
      if (plotStyleSecondary.color().isByColor())
        deviceGeom()->set_SecondaryColor(ODTOCOLORREF(plotStyleSecondary.color()));
      else if (plotStyleSecondary.color().colorMethod() != OdCmEntityColor::kNone)
        deviceGeom()->set_SecondaryColorIndex(plotStyleSecondary.color().colorIndex());
      //else continue export with previous color
    }
    else
    {
      if (entityTraits.secondaryTrueColor().isByColor())
        deviceGeom()->set_SecondaryColor(ODTOCOLORREF(entityTraits.secondaryTrueColor()));
      else if (entityTraits.secondaryTrueColor().colorMethod() != OdCmEntityColor::kNone)
        deviceGeom()->set_SecondaryColorIndex(entityTraits.secondaryTrueColor().colorIndex());
      //else continue export with previous color
    }
  }

  if(userGiContext()->isPlotGeneration() && userGiContext()->plotStyleType() != OdGiContext::kPsNone)
  {
    // CORE-16404 fix
    OdCmEntityColor oldColor = m_effectiveEntityTraitsData.trueColor();
    m_effectiveEntityTraitsData.setTrueColor(plotStyle.color());
    deviceGeom()->onTraitsModified(m_effectiveEntityTraitsData);
    m_effectiveEntityTraitsData.setTrueColor(oldColor);
  }
}

class TextOnlyGeometry: public OdGiEmptyGeometry
{
  OdGiConveyorGeometry& m_passGeom;
public:
  TextOnlyGeometry(OdGiConveyorGeometry& passGeom): m_passGeom(passGeom) {}
  void textProc(const OdGePoint3d& position,
    const OdGeVector3d& direction, const OdGeVector3d& upVector,
    const OdChar* msg, OdInt32 numChars, bool raw, const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0)
  {
    m_passGeom.textProc(position, direction, upVector, msg, numChars, raw, pTextStyle, pExtrusion);
  }
  static void connect(OdGiConveyorConnector& ep, OdGiConveyorOutput& o)
  {
    //NS: replace destination geometry by TextOnlyGeometry
    OdGiConveyorGeometry* textGeom = new TextOnlyGeometry(o.destGeometry());
    ep.addSourceNode(o);
    ep.setDestGeometry(*textGeom);
  }
  static void disconnect(OdGiConveyorConnector& ep, OdGiConveyorOutput& o)
  {
    //NS: restore conveyor and delete TextOnlyGeometry
    OdGiConveyorGeometry* pTextOnlyGeom = &ep.destGeometry();
    ODA_ASSERT(dynamic_cast<TextOnlyGeometry*>(pTextOnlyGeom));
    ep.removeSourceNode(o);
    delete pTextOnlyGeom;
  }
};

void PDF2dExportView::switchTextOnlyGeomForDisplay(bool bOn)
{
  if(!exportSearchableTextOnly())
    return;

  if(bOn)
  {
    ODA_ASSERT(!m_bTextGeomConnected);
    TextOnlyGeometry::connect(m_textOnlyEntryPointForDisplay, plotGenerator().output());
    m_bTextGeomConnected = true;
  }
  else if(m_bTextGeomConnected)
  {
    TextOnlyGeometry::disconnect(m_textOnlyEntryPointForDisplay, plotGenerator().output());
    m_bTextGeomConnected = false;
  }
}

static const int cMfDefaultOptions = OdGiMetafiler::kTextAsText | OdGiMetafiler::kNurbsAsNurbs
| OdGiMetafiler::kPlineAsPline | OdGiMetafiler::kDisableImageConversion;

void PDF2dExportView::beginViewVectorization()
{
  m_bDisableColorMerge = false;
  const bool bDisplaying = GETBIT(vectorizingFlags(), kDisplaying);
  if(bDisplaying)
  {
    setDeviceGeom(device()->getGeomForOutput());
    //if the view is used only for the hidden text output then set this flag to geometry once,
    //from the beginning
    deviceGeom()->setExportHiddenText(exportSearchableTextOnly());
    deviceGeom()->setFillRule(PDF2dExportGeometry::defaultFillRule());
    if(isGsCacheEnabled())
    {
      m_pViewportClip->setTextsNoClip(true); //disable text clipping with viewport when text is stored in the metafiles
      m_pViewportClip->setSkipExtentsCheck(true);
    }
  }
  else
  {
    m_pMetafiler->setOptions(cMfDefaultOptions);
  }

  Od2dExportView::beginViewVectorization();
  if(bDisplaying)
  {
    ODA_ASSERT(deviceGeom()); //it is either direct output or playing cache
    deviceGeom()->m_bRasterClipping = false;
  }

  const OdUInt16 geomDPI = device()->exportParams().getGeomDPI(); //NS: non-modifying access

  double scale = 72./(double)geomDPI;
  if (bDisplaying && GETBIT(device()->exportParams().exportFlags(), PDFExportParams::kZoomToExtentsMode) != 0 && deviceGeom()->m_LwToDcScale != 0 && !hasLweights()) //We need to calculate LW without plotSettings in Z2E mode (CORE-11936)
    setLineweightToDcScale(deviceGeom()->m_LwToDcScale);

  if(bDisplaying)
  {
    if(giContext().isPlotGeneration() && !updateExtentsOnly()) // In extents mode destGeometry() should be empty. Code below change destGeometry(). So we don't use plot generator in the extents mode.
    {
      OdGiPlotGenerator& pgen = plotGenerator();
      pgen.setDrawContext(drawContext());
      OdInt32 dpi = 72;
      if(!device()->properties().isNull() && device()->properties()->has(OD_T("DPI"))) 
        dpi = OdRxVariantValue(device()->properties()->getAt(OD_T("DPI")))->getInt32();
      pgen.dot_per_inch(dpi);
      pgen.input().addSourceNode(screenOutput());
      pgen.output().setDestGeometry(*deviceGeom());

      switchTextOnlyGeomForDisplay(true);

      // BR : rev6501
      // PDF page size is set in mm, therefore deviation was calculated too rough.
      // Now we take into account dpi of PDF during calculation deviation.
      const OdGiDeviation& deviationObj = m_pModelToEyeProc->eyeDeviation();
      OdGeDoubleArray values(5);

    // WE should use the same deviation as we have for 'system' PDF printing.
    // No extra deviation adjustment is needed since v4.03
    pgen.dot_per_inch(geomDPI);
#if 0
      values.append(deviationObj.deviation(kOdGiMaxDevForCircle,   OdGePoint3d::kOrigin)/pgen.dot_per_mm());
      values.append(deviationObj.deviation(kOdGiMaxDevForCurve,    OdGePoint3d::kOrigin)/pgen.dot_per_mm());
      values.append(deviationObj.deviation(kOdGiMaxDevForBoundary, OdGePoint3d::kOrigin)/pgen.dot_per_mm());
      values.append(deviationObj.deviation(kOdGiMaxDevForIsoline,  OdGePoint3d::kOrigin)/pgen.dot_per_mm());
      values.append(deviationObj.deviation(kOdGiMaxDevForFacet,    OdGePoint3d::kOrigin)/pgen.dot_per_mm());
      pgen.setDeviation(values);
#endif
    }

    //////////////////////////////////////////////////////////////////////////
    OdGeDoubleArray dev(5);
    dev.append(calcScaledDeviation(kOdGiMaxDevForCircle, OdGePoint3d::kOrigin));
    dev.append(calcScaledDeviation(kOdGiMaxDevForCurve, OdGePoint3d::kOrigin));
    dev.append(calcScaledDeviation(kOdGiMaxDevForBoundary, OdGePoint3d::kOrigin));
    dev.append(calcScaledDeviation(kOdGiMaxDevForIsoline, OdGePoint3d::kOrigin));
    dev.append(calcScaledDeviation(kOdGiMaxDevForFacet, OdGePoint3d::kOrigin));
    deviceGeom()->setDeviation(dev);
    //////////////////////////////////////////////////////////////////////////

    deviceGeom()->StartOptimizator();
    OdUInt32  uCapStyle = giContext().lineWeightConfiguration(OdGiContext::kLineCapStyle);
    OdUInt32  uJointStyle = giContext().lineWeightConfiguration(OdGiContext::kLineJoinStyle);

    TD_PDF::PDFLineCap  uLineCap = kRoundCap;
    TD_PDF::PDFLineJoin uLineJoin = kRoundJoin;

    if(uCapStyle == 0)
      uLineCap = kButtCap;
    else if(uCapStyle == 1)
      uLineCap = kProjectingSquareCap;

    if(uJointStyle == 0)
      uLineJoin = kMiterJoin;
    else if(uJointStyle == 1)
      uLineJoin = kBevelJoin;

    deviceGeom()->setCapJoin(uLineCap, uLineJoin);

    // Init GsPdfView class members ...
    // m_curRGB = device()->getBackgroundColor();
    //device()->set_Color(device()->getBackgroundColor());

    // m_lineWeight = OdDb::LineWeight(-100);
    deviceGeom()->set_LineWeight(0);

    setViewportBorderVisibility(false);

    deviceGeom()->set_Color(subEntityTraits().trueColor().color());
    deviceGeom()->processModifiedTraits();
    deviceGeom()->dc_mx();
    pushViewportClip();
  }
  else
  {
    setViewportBorderVisibility(false);
    if (mode() > kHiddenLine && device()->exportParams().forceDisableGsDevice() && device()->exportParams().useGsCache())
      m_regenerationType = kOdGiStandardDisplay; //to provide wires in metafile
  }
  setDrawContextFlags(OdGiConveyorContext::kSpatialFilterSimplPline, false);

  //notify caller about start of vectorization, to give a chance to set vectorization parameters specific to viewport,
  if(GETBIT(vectorizingFlags(), kVectorizing) && (threadIndex() == 0))
    //NS: beginViewVectorization() is called in the main thread first
  {
    if(device()->exportParams().exportReactor())
      device()->exportParams().exportReactor()->beginViewVectorization(*this);
  }
}

void PDF2dExportView::endViewVectorization()
{
  const bool bDisplaying = GETBIT(vectorizingFlags(), kDisplaying);
  if(bDisplaying)
  {
    ODA_ASSERT(deviceGeom()); //it is either direct output or playing cache
    if(!deviceGeom()->recordingEnabled() && deviceGeom()->currentPdfPage())
      deviceGeom()->enableRecording();
  }

  super::endViewVectorization();
  if(bDisplaying && giContext().isPlotGeneration())
  {
    switchTextOnlyGeomForDisplay(false);
    plotGenerator().input().removeSourceNode(screenOutput());
    screenOutput().setDestGeometry(*deviceGeom());
  }
  if(bDisplaying)
  {
    if(isGsCacheEnabled())
    {
      //restore default settings values
      m_pViewportClip->setTextsNoClip(false);
      m_pViewportClip->setSkipExtentsCheck(false);
    }
    deviceGeom()->StopOptimizator();
    popViewportClip();

    //reset blend params
    if (GETBIT(device()->exportParams().exportFlags(), PDFExportParams::kMergeLines) && device()->exportParams().archived() != PDFExportParams::kPDFA_1b)
      deviceGeom()->dc_transparency(1, 255, PDF2dExportGeometry::kUndefined);
    deviceGeom()->setExportHiddenText(false);
    setDeviceGeom(NULL);
  }
  if(GETBIT(vectorizingFlags(), kVectorizing))
  {
    //notify caller about end of vectorization
    if (device()->exportParams().exportReactor() && (threadIndex() == 0))
      //NS: endViewVectorization() is called in the main thread last
      device()->exportParams().exportReactor()->endViewVectorization(*this);
  }
}

void PDF2dExportView::loadViewport()
{
  ODA_ASSERT(deviceGeom()); //it is either direct output or playing cache
  ODA_ASSERT(&(m_pXYProjector->output()) == &(screenOutput()));

  OdGiConveyorGeometry& xyProjectorDestGeom = screenOutput().destGeometry();
  Od2dExportView::loadViewport();

  OdGsDCPoint min, max;
  screenRect(min, max);
  //////////////////////////////////////////////////////////////////////////
  OdGsDCRect rr = OdGsDCRect(min, max);
  rr.normalize();
  OdGsDCRect tmp = m_screenRect;
  tmp.intersectWith(rr);
  if (!tmp.is_null())
    m_screenRect = tmp;
  
  const int nrcCountsOut = 4;
  OdGsDCPointArray nrcPointsOut;
  OdGeExtents2d devPaperClip;
  nrcPointsOut.resize(4);
  nrcPointsOut[0] = m_screenRect.m_min;
  nrcPointsOut[1].x = m_screenRect.m_max.x;
  nrcPointsOut[1].y = m_screenRect.m_min.y;
  nrcPointsOut[2] = m_screenRect.m_max;
  nrcPointsOut[3].x = m_screenRect.m_min.x;
  nrcPointsOut[3].y = m_screenRect.m_max.y;

  devPaperClip.set(OdGePoint2d(double(m_screenRect.m_min.x), double(m_screenRect.m_min.y)),
    OdGePoint2d(OdRoundToLong(double(m_screenRect.m_max.x)), OdRoundToLong(double(m_screenRect.m_max.y))));

  deviceGeom()->m_paperClip = devPaperClip;
  deviceGeom()->dc_pushClip(1, &nrcCountsOut, nrcPointsOut);

  //expand clip box because of OdGiOrthoClipper clips the entities on the border
  if (m_screenRect.m_min.x > 0)
    m_screenRect.m_min.x--;
  if (m_screenRect.m_min.y > 0)
    m_screenRect.m_min.y--;
  m_screenRect.m_max.x++;
  m_screenRect.m_max.y++;
  //////////////////////////////////////////////////////////////////////////

  const OdGePoint2d pts[4] = { OdGePoint2d(m_screenRect.m_min.x, m_screenRect.m_min.y),
    OdGePoint2d((double)m_screenRect.m_max.x, (double)m_screenRect.m_min.y),
    OdGePoint2d(m_screenRect.m_max.x, m_screenRect.m_max.y),
    OdGePoint2d((double)m_screenRect.m_min.x, (double)m_screenRect.m_max.y) };
  const int ptsCounts = 4;
  m_pViewportClip->popClipStage();

  if (!view().isPerspective()) //workaround till CORE-11417
  {
    OdGeMatrix3d xToDc = view().eyeToScreenMatrix();

    setEyeToOutputTransform(xToDc);

    OdGeVector3d zAxis = OdGeVector3d::kZAxis;
    OdGeVector3d dcFront(0., 0., frontClip());
    OdGeVector3d dcBack(0., 0., backClip());
    dcFront.transformBy(xToDc);
    dcBack.transformBy(xToDc);
    zAxis.transformBy(xToDc);

    if (zAxis.z > 0.)
    {
      m_pViewportClip->set(ptsCounts, pts, isBackClipped(), dcBack.z, isFrontClipped(), dcFront.z);
    }
    else
    {
      m_pViewportClip->set(ptsCounts, pts, isFrontClipped(), dcFront.z, isBackClipped(), dcBack.z);
    }
    m_pXYProjector->enable();
  }
  else
  {
    m_pViewportClip->pushClipStage(1, &ptsCounts, 4, pts);
  }

  OdGeDoubleArray dev(5);
  dev.append(calcScaledDeviation(kOdGiMaxDevForCircle, OdGePoint3d::kOrigin));
  dev.append(calcScaledDeviation(kOdGiMaxDevForCurve, OdGePoint3d::kOrigin));
  dev.append(calcScaledDeviation(kOdGiMaxDevForBoundary, OdGePoint3d::kOrigin));
  dev.append(calcScaledDeviation(kOdGiMaxDevForIsoline, OdGePoint3d::kOrigin));
  dev.append(calcScaledDeviation(kOdGiMaxDevForFacet, OdGePoint3d::kOrigin));
  m_pViewportClip->setDeviation(dev);
  //m_pViewportClip->disable();
  if(view().isPerspective())
  {
    if(giContext().isPlotGeneration() && &xyProjectorDestGeom != deviceGeom()
      && &xyProjectorDestGeom != &(OdGiEmptyGeometry::kVoid))
    {
      // PDF2dExportView::beginViewVectorization() sets the destination geometry of the XY projector output
      // to the plot generator via pgen.input().addSourceNode(screenOutput()).
      // Od2dExportView::loadViewport() sets the destination geometry of the XY projector output
      // to Od2dExportDevice via m_pXYProjector->output().setDestGeometry(*device()).
      // Thus it is necessary to restore here the destination geometry of the XY projector output
      // to the plot generator to not skip the plot generator during vectorization.
      ODA_ASSERT(&(screenOutput().destGeometry()) == deviceGeom());

      OdGiPlotGenerator& pgen = plotGenerator();

      pgen.input().removeSourceNode(screenOutput());

      ODA_ASSERT(&(screenOutput().destGeometry()) == &(OdGiEmptyGeometry::kVoid));

      pgen.input().addSourceNode(screenOutput());

      ODA_ASSERT(&(screenOutput().destGeometry()) != deviceGeom());
      ODA_ASSERT(&(screenOutput().destGeometry()) != &(OdGiEmptyGeometry::kVoid));
    }

    m_pXYProjector->disable();
  }  
}

void savePalleteFromDeviceToParams(PDF2PRCExportParams &params, const OdGsBaseVectorizeDevice &device)
{
  // get color pallet from rendering device
  int numcolors;
  OdUInt64 ppallete = (OdUInt64) device.getLogicalPalette(numcolors);
  params.m_extraOptions->putAt("Pallet", OdRxVariantValue(ppallete));
  params.m_extraOptions->putAt("NumColors", OdRxVariantValue((OdUInt32)numcolors));
}

long OdSafeRoundToLong(double a)
{
  if (a >= 0.)
  {
    a += .5;
    if (a > double(LONG_MAX))
      return LONG_MAX;
    return long(a);
  }
  else
  {
    a -= .5;
    if (a < double(LONG_MIN))
      return LONG_MIN;
    return long(a);
  }
}

void PDF2dExportView::pushViewportClip()
{
  long min_x, max_x, min_y, max_y;
  OdGsDCPoint wtMin, wtMax;
  screenRect(wtMin, wtMax);
  m_screenRect = OdGsDCRect(wtMin, wtMax);
  m_screenRect.normalize();
  OdGsDCRect outRect = device()->outputRect();
  outRect.normalize();
  m_screenRect.intersectWith(outRect);

  if (device()->m_clipPoints.size() == 4) //for now there is always rectangular clipping
  {
    OdGePoint2dArray clipPoints = device()->m_clipPoints;
    int nrcCountsOut = 4;
    //find min & max points of clip rectangle
    min_x = clipPoints[0].x, max_x = clipPoints[0].x, min_y = clipPoints[0].y, max_y = clipPoints[0].y;
    for (int f = 1; f < nrcCountsOut - 1; ++f)
    {
      if (clipPoints[f].x < min_x)
        min_x = clipPoints[f].x;
      else if (clipPoints[f].x > max_x)
        max_x = clipPoints[f].x;
      if (clipPoints[f].y < min_y)
        min_y = clipPoints[f].y;
      else if (clipPoints[f].y > max_y)
        max_y = clipPoints[f].y;
    }
    OdGsDCRect clipRect(OdGsDCPoint(min_x, min_y), OdGsDCPoint(max_x, max_y));
    OdGePoint2d crMinP(min_x, min_y), crMaxP(max_x, max_y), srMinP(m_screenRect.m_min.x, m_screenRect.m_min.y), srMaxP(m_screenRect.m_max.x, m_screenRect.m_max.y);
    //if difference between these points is just 1 unit (distanceTo <= sqrt(2)), then additional clip is not needed
    if (!crMinP.isEqualTo(srMinP, sqrt(2.)) || !crMaxP.isEqualTo(srMaxP, sqrt(2.)))
      m_screenRect.intersectWith(clipRect);
    if (m_screenRect.is_null()) //screen rect is not intersecting with clip rect
      m_screenRect = OdGsDCRect(0, 0, 1, 1);
  }

  if(isNonRectClipped())
  {
    OdGsDCPointArray nrcPoints;
    OdIntArray nrcCounts;
    viewportClipRegion(nrcCounts, nrcPoints);
    deviceGeom()->dc_pushClip(nrcCounts.size(), nrcCounts.getPtr(), nrcPoints);
    m_bAdditionalClip = true;
  }

  //temporarily fix for screen rect (till translation matrix (cm) is not added)
  OdUInt16 geomDPI = device()->exportParams().getGeomDPI();
  double scale = 72. / (double)geomDPI;
  m_screenRect.m_min.x = scale * (double)m_screenRect.m_min.x;
  m_screenRect.m_min.y = scale * (double)m_screenRect.m_min.y;
  if (m_screenRect.m_max.x > 0)
    m_screenRect.m_max.x = OdSafeRoundToLong(scale * (double)m_screenRect.m_max.x) + 1;   //we have to expand clipping borders a bit
  if (m_screenRect.m_max.y > 0)
    m_screenRect.m_max.y = OdSafeRoundToLong(scale * (double)m_screenRect.m_max.y) + 1;

  if (m_screenRect.m_min.x > 0)
    m_screenRect.m_min.x -= 1;
  if (m_screenRect.m_min.y > 0)
    m_screenRect.m_min.y -= 1;

  m_screenRect.m_min.x = (double)m_screenRect.m_min.x / scale;
  m_screenRect.m_min.y = (double)m_screenRect.m_min.y / scale;
  m_screenRect.m_max.x = (double)m_screenRect.m_max.x / scale;
  m_screenRect.m_max.y = (double)m_screenRect.m_max.y / scale;

// The code below was moved in loadViewport() to provide more accurate clipping 
//   const int nrcCountsOut = 4;
//   OdGsDCPointArray nrcPointsOut;
//   OdGeExtents2d devPaperClip;
//   nrcPointsOut.resize(4);
//   nrcPointsOut[0]   = m_screenRect.m_min;
//   nrcPointsOut[1].x = m_screenRect.m_max.x;
//   nrcPointsOut[1].y = m_screenRect.m_min.y;
//   nrcPointsOut[2]   = m_screenRect.m_max;
//   nrcPointsOut[3].x = m_screenRect.m_min.x;
//   nrcPointsOut[3].y = m_screenRect.m_max.y;
// 
//   devPaperClip.set(OdGePoint2d(double(m_screenRect.m_min.x), double(m_screenRect.m_min.y)),
//                    OdGePoint2d(OdRoundToLong(double(m_screenRect.m_max.x)), OdRoundToLong(double(m_screenRect.m_max.y))));
// 
//   device()->m_paperClip = devPaperClip;
//   device()->dc_pushClip(1, &nrcCountsOut, nrcPointsOut);
}

void PDF2dExportView::popViewportClip()
{
  if (m_bAdditionalClip)
  {
    deviceGeom()->dc_popClip();
    m_bAdditionalClip = false;
  }
  deviceGeom()->dc_popClip();
  deviceGeom()->m_paperClip = OdGeExtents2d();
}

OdGiPlotGenerator& PDF2dExportView::plotGenerator()
{
  ODA_ASSERT(deviceGeom()); //it is either direct output or playing cache
  if(m_pPlotstyler.isNull())
  {
    m_pPlotstyler = OdGiPlotGenerator::createObject();
    OdGeDoubleArray devs(5,1);
    devs.insert(devs.end(), 5, .5);
    m_pPlotstyler->setDeviation(devs);
    m_pPlotstyler->setExternalEllipseOut(deviceGeom());
  }
  return *m_pPlotstyler;
}

bool PDF2dExportView::viewExtents(OdGeBoundBlock3d& extents) const
{
 
  //see CORE-9754:
  //Geometry located at Frozen layer shouldn't be included to "extents".
  //in case if some geometry out of pdf paper area, this geometry will not be exported to .pdf.
  bool bFrozenLayersExist = device()->isLayersSupported() && device()->isOffLayersEnabled() && device()->frozenLayerSimulationEnabled();

  if(isGsCacheEnabled() && !bFrozenLayersExist)
  { // #CORE-16421 : Generate cache for current viewport if it is not already available.
    PDF2dExportDevice *pDevice = (PDF2dExportDevice*)device();
    if(pDevice)
    { // Actually we can avoid hiding of additional views and generate entire graphics cache in result, but
      // typically extents acquired for single view, so in this case we will require rebuilding of entire cache
      // graphics, which is redundant.
      OdIntArray unhideViews;
      for(int nView = 0; nView < pDevice->numViews(); nView++)
      {
        if(pDevice->viewAt(nView) != this && pDevice->viewAt(nView)->isVisible())
          unhideViews.push_back(nView), pDevice->viewAt(nView)->hide();
      }
      // Initialize MtRegen and call updateGeometry().
      OdSaveStateFlag(OdUInt32, eEnableUpdateExtentsOnly, _ssVectorizationOnly, pDevice->m_vectPerfData.m_options, true);
      //pDevice->OdGsBaseVectorizeDevice::update(NULL);

      // Here we can enable parallel vectorization, like this is done in OdGsBaseVectorizeDevice::update, if it will be required in future.
      pDevice->doUpdateGeometry();

      // Show previously hidden views
      for(unsigned nHide = 0; nHide < unhideViews.size(); nHide++)
        pDevice->viewAt(unhideViews.getPtr()[nHide])->show();
    }
    // Now we can run standard viewExtents, which now will be able to get extents from already pre-built graphics cache.
  }

  bool b = Od2dExportView::viewExtents(extents);
  return b;
}

void invalidateViewModelChanges(OdGsViewImpl& view)
{
  if(view.cachedDrawables() != 0)
  { // Get list of models
    OdGsModelArray models = view.getModelList();
    for(OdUInt32 nModel = 0; nModel < models.size(); nModel++)
    { // For each model we can check aware flags difference from previous cache building pass and invalidate
      // parts of cache, which is influenced by view changes.
      OdGsBaseModel *pModel = static_cast<OdGsBaseModel*>(models.at(nModel));
      if(pModel->isViewPropsValid(view.localViewportId(pModel)))
      {
        ViewProps viewProps; viewProps.set(view);
        const OdUInt32 propsDiff = viewProps.difference(pModel->viewProps(view.localViewportId(pModel)));
        if(propsDiff)
          pModel->invalidate(view, propsDiff);
      }
    }
  }
}

#define ENABLE_HATCH_AS_BITMAP
//#ifdef ENABLE_HATCH_AS_BITMAP

static void createTransparentImage(OdGiRasterImagePtr pCopyImg, OdGiRasterImagePtr pRas1)
{
  OdUInt32 srcScanLineSz = pCopyImg->scanLineSize();
  OdUInt32 srcScanLineSz2 = pRas1->scanLineSize();
  OdUInt8Array srcScLnBuf2;
  srcScLnBuf2.resize(srcScanLineSz2, 0);
  OdUInt8 *pSrcScLnBuf2 = srcScLnBuf2.asArrayPtr();

  OdUInt32 dstY, dstX, dstHeight = pCopyImg->pixelHeight(), dstWidth = pCopyImg->pixelWidth();

  OdUInt8Array tmpScanline4Copy;
  const OdUInt8 *pScanline4Copy = pCopyImg->scanLines();
  if (!pScanline4Copy)
  {
    tmpScanline4Copy.resize(srcScanLineSz * pCopyImg->pixelHeight());
    pCopyImg->scanLines(tmpScanline4Copy.asArrayPtr(), 0, pCopyImg->pixelHeight());
    pScanline4Copy = tmpScanline4Copy.getPtr();
  }

  for(dstY = 0; dstY < dstHeight; dstY ++)
  {
    OdUInt8 *pRGBA =(OdUInt8 *)pScanline4Copy + dstY*srcScanLineSz;

    pRas1->scanLines(pSrcScLnBuf2, dstY);
    OdUInt8 *pRGB  = pSrcScLnBuf2;

    dstX = 0;
    for(; dstX < dstWidth; dstX ++)
    {
      if (pRGB[0]==0 && pRGB[1]==0 && pRGB[2]==0 &&
        pRGBA[0]==255 && pRGBA[1]==255 && pRGBA[2]==255)
      {
        pRGBA[3] = 0; // transparent
      }
      else
      {
        pRGBA[3] = 255;
      }

      pRGB += 3;
      pRGBA += 4;
    }
  }
}

static OdGiRasterImagePtr convertToRGBA(OdGiRasterImagePtr pRas1)
{
  OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
  pDesc->setPixelWidth(pRas1->pixelWidth());
  pDesc->setPixelHeight(pRas1->pixelHeight());
  pDesc->setColorDepth(32);
  pDesc->pixelFormat().setRGBA();
  pDesc->setScanLinesAlignment(4);

  return pRas1->convert(true, 50, 50, 0, ODRGB(255,255,255), false, false, false, pDesc, true);
}

static OdGsDevicePtr getRenderingDevice()
{
  OdGsModulePtr pGs;
  try
  {
    pGs = odrxDynamicLinker()->loadModule(OdWinBitmapModuleName, false);
  }
  catch (...)
  {
  }  
  if (pGs.isNull())
    return OdGsDevicePtr();

  OdGsDevicePtr pDevice = pGs->createBitmapDevice();
  // AMark : enable precision triangles rendering mode for bitmap device
  OdRxDictionaryPtr pDevProps = pDevice->properties();
  if (!pDevProps.isNull())
  {
    const OdString pm(OD_T("PrecisionMode"));
    const OdString bpp(OD_T("BitPerPixel"));
    if (pDevProps->has(pm))
      pDevProps->putAt(pm, OdRxVariantValue(true));
    if(pDevProps->has(bpp))
      pDevice->properties()->putAt(bpp, OdRxVariantValue(OdUInt32(24)));
  }
  ODCOLORREF bg = ODRGB(255, 255, 255); // palette background
  const ODCOLORREF* palette = odcmAcadPalette(bg);
  ODGSPALETTE pPalCpy;
  pPalCpy.insert(pPalCpy.begin(), palette, palette + 256);
  pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
  return pDevice;
}

class ShellDrawable : public OdGiDrawableImpl<OdGiDrawable>
{
public:
#if (defined(_MSC_VER) && (_MSC_VER > 1700)) || (defined(__cplusplus) && (__cplusplus > 199711L))
  ShellDrawable(const OdGiSubEntityTraitsData& traits,
    OdGsBaseVectorizer& vect, OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData):
    m_numVertices(numVertices),
    m_vertexList(vertexList),
    m_faceListSize(faceListSize),
    m_faceList(faceList),
    m_pEdgeData(pEdgeData),
    m_pFaceData(pFaceData),
    m_pVertexData(pVertexData),
    m_traits(traits)
  {
    if (vect.view().userGiContext()->isPlotGeneration() &&
      vect.view().userGiContext()->plotStyleType() != OdGiContext::kPsNone)
    {
      const OdPsPlotStyleData& ps = vect.effectivePlotStyle();
      if (ps.color().isByColor())
        m_traits.setTrueColor(ps.color());
      else
        m_traits.setColor(ps.color().colorIndex());
    }
  }
#else
  void init(const OdGiSubEntityTraitsData& traits,
    const OdGsBaseVectorizer& vect,
    OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData)
  {
    m_numVertices = numVertices;
    m_vertexList = vertexList;
    m_faceListSize = faceListSize;
    m_faceList = faceList;
    m_pEdgeData = pEdgeData;
    m_pFaceData = pFaceData;
    m_pVertexData = pVertexData;
    m_traits = traits;
    if (vect.view().userGiContext()->isPlotGeneration() &&
      vect.view().userGiContext()->plotStyleType() != OdGiContext::kPsNone)
    {
      const OdPsPlotStyleData& ps = vect.effectivePlotStyle();
      if (ps.color().isByColor())
        m_traits.setTrueColor(ps.color());
      else
        m_traits.setColor(ps.color().colorIndex());
    }
  }
#endif

  void resetFaces(OdInt32 faceListSize, const OdInt32* faceList, const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData)
  {
    m_faceListSize = faceListSize;
    m_faceList = faceList;
    m_pEdgeData = pEdgeData;
    m_pFaceData = pFaceData;
  }

  OdInt32 numVertices() const { return m_numVertices; }
  const OdGePoint3d* vertexList() const { return m_vertexList; }
  OdInt32 faceListSize() const { return m_faceListSize; }
  const OdInt32* faceList() const { return m_faceList; }
  const OdGiEdgeData* edgeData() const { return m_pEdgeData; }
  const OdGiFaceData* faceData() const { return m_pFaceData; }

private:
  OdUInt32 subSetAttributes(OdGiDrawableTraits * traits) const
  {
    return 0;
  }

  virtual bool subWorldDraw(OdGiWorldDraw * wd) const
  {
    wd->subEntityTraits().setTrueColor(m_traits.trueColor());
    wd->subEntityTraits().setFillType(m_traits.fillType());
    wd->geometry().shell(m_numVertices, m_vertexList, m_faceListSize, m_faceList, m_pEdgeData, m_pFaceData, m_pVertexData);
    return false;
  }

private:
  OdInt32 m_numVertices;
  const OdGePoint3d* m_vertexList;
  OdInt32 m_faceListSize;
  const OdInt32* m_faceList;
  const OdGiEdgeData* m_pEdgeData;
  const OdGiFaceData* m_pFaceData;
  const OdGiVertexData* m_pVertexData;
  OdGiSubEntityTraitsData m_traits;
};

static OdGeNurbCurve3d* convert2dto3d(const OdGeNurbCurve2d* pNc2d)
{
  OdGePoint3dArray ctrlPts;
  ctrlPts.resize(pNc2d->numControlPoints());
  OdGeDoubleArray  weights;
  weights.resize(pNc2d->numWeights());
  ODA_ASSERT(weights.size()==0 || weights.size()==ctrlPts.size());
  for(OdInt32 i = 0; i < pNc2d->numControlPoints(); i ++)
  {
    ctrlPts[i].x = pNc2d->controlPointAt(i).x;
    ctrlPts[i].y = pNc2d->controlPointAt(i).y;
    ctrlPts[i].z = 0.0;
    if(pNc2d->numWeights())
      weights[i] = pNc2d->weightAt(i);
  }
  return new OdGeNurbCurve3d(pNc2d->degree(), pNc2d->knots(), ctrlPts, weights);
}

static OdGeNurbCurve3d* ellipse2dtoNurbs3d(const OdGeEllipArc2d& arc)
{
  OdGeEllipArc3d arc3d;
  OdGePoint3d center(arc.center().x, arc.center().y, 0.);
  OdGeVector3d axisMajor(arc.majorAxis().x, arc.majorAxis().y, 0.);
  OdGeVector3d axisMinor(arc.minorAxis().x, arc.minorAxis().y, 0.);
  arc3d.set(center, axisMajor, axisMinor, arc.majorRadius(), arc.minorRadius(), arc.startAng(), arc.endAng());
  return new OdGeNurbCurve3d(arc3d);
}

struct Shell
{
  void addShell(const Shell& shell);
  void addEdgeData(const OdGiEdgeData& edgeData, OdInt32 edgeIndex);
  void addEdgeData(const OdGiEdgeDataStorage& edgeData);

  void addFaceData(const OdGiFaceData& faceData, OdInt32 faceIndex);
  void addFaceData(const OdGiFaceDataStorage& faceData);

public:
  OdInt32Vector m_faces;
  OdGiEdgeDataStorage m_edgeData;
  OdGiFaceDataStorage m_faceData;
  OdGeExtents3d m_extents;
};

typedef OdVector<Shell> ShellArray;


inline void Shell::addShell(const Shell& shell)
{
  m_faces.append(shell.m_faces);

  addEdgeData(shell.m_edgeData);
  addFaceData(shell.m_faceData);

  m_extents.addExt(shell.m_extents);
}

template<class T, class TArray>
static inline void addData(TArray& destArray, const T* pSourceArray, OdInt32 index)
{
  if(pSourceArray != NULL)
    destArray.append(pSourceArray[index]);
}

inline void Shell::addEdgeData(const OdGiEdgeData& edgeData, OdInt32 edgeIndex)
{
  addData(m_edgeData.colorsArray(), edgeData.colors(), edgeIndex);
  addData(m_edgeData.trueColorsArray(), edgeData.trueColors(), edgeIndex);
  addData(m_edgeData.layerIdsArray(), edgeData.layerIds(), edgeIndex);
  addData(m_edgeData.linetypeIdsArray(), edgeData.linetypeIds(), edgeIndex);
  addData(m_edgeData.selectionMarkersArray(), edgeData.selectionMarkers(), edgeIndex);
  addData(m_edgeData.visibilityArray(), edgeData.visibility(), edgeIndex);
}

inline void Shell::addEdgeData(const OdGiEdgeDataStorage& edgeData)
{
  m_edgeData.colorsArray().append(edgeData.colorsArray());
  m_edgeData.trueColorsArray().append(edgeData.trueColorsArray());
  m_edgeData.layerIdsArray().append(edgeData.layerIdsArray());
  m_edgeData.linetypeIdsArray().append(edgeData.linetypeIdsArray());
  m_edgeData.selectionMarkersArray().append(edgeData.selectionMarkersArray());
  m_edgeData.visibilityArray().append(edgeData.visibilityArray());
}

inline void Shell::addFaceData(const OdGiFaceData& faceData, OdInt32 faceIndex)
{
  addData(m_faceData.colorsArray(), faceData.colors(), faceIndex);
  addData(m_faceData.trueColorsArray(), faceData.trueColors(), faceIndex);
  addData(m_faceData.layerIdsArray(), faceData.layerIds(), faceIndex);
  addData(m_faceData.selectionMarkersArray(), faceData.selectionMarkers(), faceIndex);
  addData(m_faceData.visibilityArray(), faceData.visibility(), faceIndex);
  addData(m_faceData.normalsArray(), faceData.normals(), faceIndex);
  addData(m_faceData.materialsArray(), faceData.materials(), faceIndex);
  addData(m_faceData.mappersArray(), faceData.mappers(), faceIndex);
  addData(m_faceData.transparencyArray(), faceData.transparency(), faceIndex);
}

inline void Shell::addFaceData(const OdGiFaceDataStorage& faceData)
{
  m_faceData.colorsArray().append(faceData.colorsArray());
  m_faceData.trueColorsArray().append(faceData.trueColorsArray());
  m_faceData.layerIdsArray().append(faceData.layerIdsArray());
  m_faceData.selectionMarkersArray().append(faceData.selectionMarkersArray());
  m_faceData.visibilityArray().append(faceData.visibilityArray());
  m_faceData.normalsArray().append(faceData.normalsArray());
  m_faceData.materialsArray().append(faceData.materialsArray());
  m_faceData.mappersArray().append(faceData.mappersArray());
  m_faceData.transparencyArray().append(faceData.transparencyArray());
}


static void getSubshells(ShellArray& subshells, OdInt32 numVtx
  , const OdGePoint3d* pVertices, OdInt32 facesSize, const OdInt32* pFaces
  , const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData)
{
  ODA_ASSERT(pVertices != NULL && pFaces != NULL && subshells.isEmpty());
  for(OdInt32 i = 0, edgeIndex = 0, faceIndex = 0; i < facesSize;)
  {
    ODA_ASSERT(pFaces[i] > 0);
    Shell* pSubshell = subshells.append();
    const OdInt32& numFaceVtx = pFaces[i];
    pSubshell->m_faces.append(numFaceVtx);
    ++i;
    for(OdInt32 j = 0; j < numFaceVtx; ++j, ++i, ++edgeIndex)
    {
      pSubshell->m_faces.append(pFaces[i]);
      if(pEdgeData != NULL)
        pSubshell->addEdgeData(*pEdgeData, edgeIndex);
      pSubshell->m_extents.addPoint(pVertices[pFaces[i]]);
    }

    if(pFaceData != NULL)
      pSubshell->addFaceData(*pFaceData, faceIndex);

    // add holes
    for(++faceIndex; i < facesSize && pFaces[i] < 0;)
    {
      const OdInt32& numHoleFaceVtx = pFaces[i];
      pSubshell->m_faces.append(numHoleFaceVtx);
      ++i;
      for(OdInt32 j = 0; j > numHoleFaceVtx; --j, ++i, ++edgeIndex)
      {
        pSubshell->m_faces.append(pFaces[i]);
        if(pEdgeData != NULL)
          pSubshell->addEdgeData(*pEdgeData, edgeIndex);
      }
    }

    for(OdInt32 j = subshells.length() - 2; j >= 0; --j)
    {
      if(!pSubshell->m_extents.isDisjoint(subshells[j].m_extents))
      {
        subshells[j].addShell(*pSubshell);
        for(OdInt32 k = j - 1; k >= 0; --k)
        {
          if(!subshells.last().m_extents.isDisjoint(subshells[k].m_extents))
          {
            subshells[k].addShell(subshells[j]);
            subshells.removeAt(j);
            j = k;
          }
        }

        ODA_ASSERT(subshells.length() > 1);
        subshells.setLogicalLength(subshells.length() - 1);
        break;
      }
    }
  }
}

static inline void generateImage(ImageArray& images, OdGsDevice* pDevice
  , OdGiContext* pUserGiContext, const PDF2dExportDevice* pExportDevice
  , const OdGeExtents3d& extents, const double& fieldWidth, const double& fieldHeight
  , long imageWidth, long imageHeight)
{
  OdGsDCRect UpdatedRect(0, imageWidth, 0, imageHeight);
  pDevice->onSize(UpdatedRect);

  if(pExportDevice->getPalette())
    pDevice->setLogicalPalette(pExportDevice->getPalette(), 256);

  pDevice->setUserGiContext(pUserGiContext);

  // create mask 1 with white background
  pDevice->setBackgroundColor(ODRGB(255, 255, 255));
  pDevice->update();
  const OdString ri(OD_T("RasterImage"));
  OdGiRasterImagePtr pRas1 = OdGiRasterImagePtr(pDevice->properties()->getAt(ri));

  // convert from RGB to RGBA
  OdGiRasterImagePtr pCopyImg = convertToRGBA(pRas1);

  // create mask 2 with black background
  pDevice->setBackgroundColor(ODRGB(0, 0, 0));
  pDevice->update();
  pRas1 = OdGiRasterImagePtr(pDevice->properties()->getAt(ri));

  // Merge both images to create Bitmap with transparency
  createTransparentImage(pCopyImg, pRas1);

  ImageData* pImage = images.append();
  pImage->m_origin = extents.minPoint() + OdGeVector3d::kYAxis*fieldHeight;
  pImage->m_uVec = OdGeVector3d::kXAxis*fieldWidth / imageWidth;
  pImage->m_vVec = -OdGeVector3d::kYAxis*fieldHeight / imageHeight;
  pImage->m_pImage = pCopyImg;
}

static inline bool isValidImageSize(long imageWidth, long imageHeight)
{
  return (OdUInt64((OdUInt64)imageWidth*(OdUInt64)imageHeight) <= 8000 * 8000 // check that image is not too big to be created
    && imageHeight <= 65500); // workaround, because FreeImage_SaveToHandle() throws an exception in FreeImageWriter::write()
                              // for JPEG free image format if the height of the image is greater than 65500
}
#ifdef _DEBUG
static void checkMetafile(OdGiGeometryMetafile* pMf, int& contourCnt)
{
  for(OdGiGeometryMetafile::Record* p = pMf->firstRecord(); p; p = p->tail())
  {
    RecDrawingContour* pCont = dynamic_cast<RecDrawingContour*>(p);
    if(pCont)
    {
      if(contourCnt == 0)
      {
        ODA_ASSERT(pCont->isStart());
        contourCnt++;
      }
      else
      {
        ODA_ASSERT(!pCont->isStart());
        contourCnt--;
      }
    }
  }
}

static void checkJointMetafile(OdGiGeometryMetafile* pMf1, OdGiGeometryMetafile* pMf2)
{
  int contourCnt = 0;
  checkMetafile(pMf1, contourCnt);
  if(pMf2 && (pMf2 != pMf1))
  {
    ODA_ASSERT(false);
    checkMetafile(pMf2, contourCnt);
  }
  ODA_ASSERT(!contourCnt);
}
#endif

static OdGeExtents3d calcExtents(OdInt32 numVertices, const OdGePoint3d* vertexList)
{
  OdGeExtents3d extents;
  for(OdInt32 f = 0; f < numVertices; ++f)
    extents.addPoint(vertexList[f]);
  return extents;
}

bool PDF2dExportView::processSolidShell(
  const OdGeExtents3d& extents,
  OdUInt32 clipStat,
  OdInt32 numVertices,
  const OdGePoint3d* vertexList,
  OdInt32 faceListSize,
  const OdInt32* faceList,
  const OdGiEdgeData* pEdgeData,
  const OdGiFaceData* pFaceData,
  const OdGiVertexData* pVertexData)
{
  //////////////////////////////////////////////////////////////////////////
  if(GETBIT(clipStat, OdGiConveyorGeometry::kBoundaryProcClip))//Partially clipped
  {
    if(!deviceGeom())
      return false;

    bool restartVpClip = false;
    if(!m_pViewportClip.isNull() && m_pViewportClip->enabled())
    {
      m_pViewportClip->disable(); //disable VpClip to check if the text clipped by it or by clipping on the conveyor
      restartVpClip = true;
    }

    if(restartVpClip)
      clipStat = clipStatus(extents, m_modelEntryPoint.geometry());

    if(GETBIT(clipStat, OdGiConveyorGeometry::kBoundaryProcClip))//Partially clipped
      //on conveyor so it cannot be correctly exported as pdfPaths
    {
      if(restartVpClip)
        m_pViewportClip->enable();
      return false;
    }
  }
  OdDbBaseHatchPE* pH = OdGsDbRootLinkage::getDbBaseHatchPE(m_pSolidHatch);
  ODA_ASSERT(pH);
#ifdef _DEBUG
  if(m_pMetafiler->metafile())
    checkJointMetafile(m_pMetafiler->metafile(), nullptr);
#endif
  drawHatchAsPaths(*pH, numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
  return true;
}

void PDF2dExportView::shell(OdInt32 numVertices,
           const OdGePoint3d* vertexList,
           OdInt32 faceListSize,
           const OdInt32* faceList,
           const OdGiEdgeData* pEdgeData,
           const OdGiFaceData* pFaceData,
           const OdGiVertexData* pVertexData)
{
  if(m_bHatchToBitmapMode)
  {
    ODA_ASSERT(isGsCacheEnabled() || !updateExtentsOnly());
    const OdGeExtents3d extents = calcExtents(numVertices, vertexList);

    if (updateExtentsOnly())
    {
      //When we calculate extents with GsCache we just need to add extents to extAccum, because of:
      //1. There is almost absolute probability, that the hatch will need to be regenerated on vectorization stage, because of changed deviation, so there is no need to spend resources on its vectorization
      //2. In HatchAsBitmap mode, the generated picture can affect the extents calculation, when it will be sent to conveyor, because of objectToDeviceMatrix cannot be calculated properly on extents calculation stage
      OdGiBaseVectorizerImpl::setExtents((OdGePoint3d*)&extents);
      m_awareFlags.setFlag(kEntityRegenDraw);
      return;
    }
    
    const OdUInt32 clipStat = clipStatus(extents, m_modelEntryPoint.geometry());
    if(GETBIT(clipStat, OdGiConveyorGeometry::kBoundaryProcClipFull))
      return;//Fully clipped, vectorization can be skipped
    //////////////////////////////////////////////////////////////////////////
    if(m_pSolidHatch.get())
    {
      if(processSolidShell(extents, clipStat, numVertices,
        vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData))
        return;
    }
    else if(drawHatchAsBitmap(extents, clipStat, numVertices,
      vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData))
      return;
  }
  super::shell(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
}

void PDF2dExportView::setFillRule(PDF2dExportGeometry::PolygonFillRules rule)
{
  if(m_pMetafiler->metafile())
  {
    m_pMetafiler->flush();
    m_pMetafiler->add(new RecFillRule(rule));
  }
  else
    deviceGeom()->setFillRule(rule);
}

void PDF2dExportView::fillContour()
{
  if(!m_pMetafiler->metafile())
    return deviceGeom()->dc_fill();
  m_pMetafiler->flush();
  m_pMetafiler->add(new RecFillContour());
}

void PDF2dExportView::setDrawingContour(bool bStart)
{
  if(deviceGeom())
  {
    if(bStart)
      deviceGeom()->startDrawingContour();
    else
      deviceGeom()->endDrawingContour();
    return;
  }
  if(m_pMetafiler->metafile())
  {
    m_pMetafiler->flush();
    m_pMetafiler->add(new RecDrawingContour(bStart));
  }
}

void PDF2dExportView::enableViewportClip(bool bEnable)
{
  if(deviceGeom())
  {
    if(bEnable)
      m_pViewportClip->enable();
    else
      m_pViewportClip->disable();
    return;
  }
  if(m_pMetafiler->metafile())
  {
    m_pMetafiler->flush();
    m_pMetafiler->add(new RecEnableClipper(bEnable));
  }
}

static double getHatchDeviation(OdGiWorldDraw* pWd)
{
  double dev(1e-3);
  if(pWd)
  {
    dev = pWd->deviation(kOdGiMaxDevForCurve, OdGePoint3d::kOrigin);
    if(OdZero(dev))
      dev = 1e-3;
    else
    {
      //Temporarily enable kDrawHatchGroup flag to inform Gs that cache shouldn't be marked as ModelTfDependent during getModelToWorldTransform call.
      OdGiDrawFlagsHelper _dd(pWd->subEntityTraits(), OdGiSubEntityTraits::kDrawHatchGroup);
      dev /= pWd->geometry().getModelToWorldTransform().scale();
    }
  }
  return dev;
}

 void PDF2dExportView::drawHatchAsPaths(OdDbBaseHatchPE& hatchPE, OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList, const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData)
 {
   const int nLoops = hatchPE.numLoops(m_pSolidHatch);
   if(!nLoops || !effectivelyVisible())
     return;
   if(m_pMetafiler.get())
   {
     setLineWeight(OdDb::kLnWt000);
     onTraitsModified();
     m_pMetafiler->flush(true);
   }
#ifdef _DEBUG
   OdGiGeometryMetafilePtr pMf = m_pMetafiler->metafile();
#endif

   if(m_bRestartClipper)
     enableViewportClip(false);
   setFillRule(PDF2dExportGeometry::kNoFill);
   try
   {
     {
       const double dev = getHatchDeviation(this);
       for(int l = 0; l < nLoops; ++l)
       {
         bool bEmptyContour = true;
         bool bContourStarted = false;
         OdInt32 type = hatchPE.loopTypeAt(m_pSolidHatch, l);
         OdDbBaseHatchPE::HatchStyle hs = hatchPE.hatchStyle(m_pSolidHatch);
         if(
           (hs == OdDbBaseHatchPE::kIgnore && (type & OdDbBaseHatchPE::kOutermost)) ||
           (hs == OdDbBaseHatchPE::kOuter && ((~type & OdDbBaseHatchPE::kOutermost) && (~type & OdDbBaseHatchPE::kExternal))) ||
           type & OdDbBaseHatchPE::kDuplicate ||
           type & OdDbBaseHatchPE::kTextbox || type & OdDbBaseHatchPE::kTextIsland || // TextBox + TextIsland loops are ignored by ACAD
           ((~type & OdDbBaseHatchPE::kPolyline) && (type & OdDbBaseHatchPE::kNotClosed))
           )
         {
           goto nextloop;
           //continue;
         }
 
         setDrawingContour(true);
         bContourStarted = true;
         if((type & OdDbBaseHatchPE::kPolyline) != 0)
         {
           OdGePoint2dArray vertices;
           OdGeDoubleArray bulges;
           hatchPE.getLoopAt(m_pSolidHatch, l, vertices, bulges);
           const bool bClosed = !(type & OdDbBaseHatchPE::kNotClosed);
 
           OdGePoint3dArray pnts3d;
           const unsigned int n = bClosed ? vertices.size() : vertices.size() - 1;
           for(unsigned i = 0; i < n; ++i)
           {
             const OdGePoint2d& pt = vertices[i];
             const double bulge = bulges.isEmpty() ? 0. : bulges[i];
             if(OdZero(bulge))
             {
               pnts3d.push_back(OdGePoint3d(pt.x, pt.y, 0.));
               continue;
             }
 
             if(!pnts3d.isEmpty())
             {
               pnts3d.push_back(OdGePoint3d(pt.x, pt.y, 0.));
               polyline(pnts3d.logicalLength(), pnts3d.asArrayPtr());
               pnts3d.clear();
               bEmptyContour = false;
             }
 
             const OdGePoint2d ptNext = vertices[(i + 1) % n];
             if(pt.isEqualTo(ptNext))
             {
               continue;
             }
 
             OdGeCircArc2d arc(pt, ptNext, bulge, false);
             OdSharedPtr<OdGeNurbCurve3d> nurb3d = ellipse2dtoNurbs3d(arc);
             nurbs(*nurb3d);
             bEmptyContour = false;
           }
           if(!pnts3d.isEmpty())
           {
             pnts3d.push_back(bClosed ? OdGePoint3d(vertices[0].x, vertices[0].y, 0.) : OdGePoint3d(vertices.last().x, vertices.last().y, 0.));
             polyline(pnts3d.logicalLength(), pnts3d.asArrayPtr());
             bEmptyContour = false;
           }
         }
         else
         {
           OdArray<OdGeCurve2d*> edgePtrs;
           hatchPE.getLoopAt(m_pSolidHatch, l, edgePtrs);
 
           OdGePoint2d pLast;
           for(unsigned f = 0; f < edgePtrs.size(); ++f)
           {
             OdGeCurve2d* cv = edgePtrs.getAt(f);
 
             // try to get nurbs
             OdSharedPtr<OdGeNurbCurve3d> nurb3d = NULL;
            switch(cv->type())
            {
            case OdGe::kNurbCurve2d:
              nurb3d = convert2dto3d((OdGeNurbCurve2d*)cv);
              break;
            case OdGe::kEllipArc2d:
              nurb3d = ellipse2dtoNurbs3d(*((OdGeEllipArc2d*)cv));
              break;
            case OdGe::kCircArc2d:
              nurb3d = ellipse2dtoNurbs3d(*((OdGeCircArc2d*)cv));
              break;
            }

            if(nurb3d != NULL)
             {
               nurbs(*nurb3d);
               //CORE-11119 PdfExport: Export solid hatch regression - hatch w/o one edge
               OdGeInterval intvl;
               nurb3d->getInterval(intvl);
               double param1 = intvl.lowerBound();
               double param2 = intvl.upperBound();
               OdGePoint3dArray pnts;
               nurb3d->appendSamplePoints(param1, param2, dev, pnts);
               //OdGePoint2d p1 = OdGePoint2d(pnts[0].x, pnts[0].y);
               pLast = OdGePoint2d(pnts.last().x, pnts.last().y);
               //////////////////////////////////////////////////////////////////////////
               bEmptyContour = false;
             }
             else
             {
               OdGeInterval intvl;
               cv->getInterval(intvl);
               double param1 = intvl.lowerBound();
               double param2 = intvl.upperBound();
               OdGePoint2dArray pnts;
               cv->appendSamplePoints(param1, param2, dev, pnts);
 
               /*const*/ unsigned int nPt = pnts.logicalLength();
 
               if(nPt > 0)
               {
                 OdGePoint3dArray pnts3d;
                 //CORE-11119 PdfExport: Export solid hatch regression - hatch w/o one edge
                 OdUInt32 k = 0, kk = 0;
                 if(!pLast.isEqualTo(pnts[0], 1.e-6) && f)
                 {
                   pnts3d.setLogicalLength(++nPt);
                   pnts3d[k].set(pLast.x, pLast.y, 0.);
                   k++;
                 }
                 else
                 {
                   pnts3d.setLogicalLength(nPt);
                 }
                 //////////////////////////////////////////////////////////////////////////
                 for(k; k < nPt; ++k)
                 {
                   pLast = pnts[kk];
                   pnts3d[k].set(pnts[kk].x, pnts[kk].y, 0.);
                   ++kk;
                 }
                 polyline(nPt, pnts3d.asArrayPtr());
                 bEmptyContour = false;
               }
             }
           }
         }
 
       nextloop:
         if(l == nLoops - 1)
           setFillRule(PDF2dExportGeometry::kFillStrokeCloseRule); //Bug 22740. It was PDF2dExportGeometry::kFillEvenOddRule before
 
         // last path will be closed by fill command
         if(l == nLoops - 1 && bEmptyContour)
           fillContour();
 
         if(bContourStarted)
           setDrawingContour(false);
       } // loops                
     }
     setFillRule(PDF2dExportGeometry::defaultFillRule()); //restore previous rule

     if(m_bRestartClipper)
       enableViewportClip(true);
#ifdef _DEBUG
     if(m_pMetafiler->metafile())
       checkJointMetafile(pMf, m_pMetafiler->metafile());
#endif
     return;
   }
   catch(...)
   {
     setDrawingContour(false);
     setFillRule(PDF2dExportGeometry::defaultFillRule()); //restore previous rule
     if(m_bRestartClipper)
       enableViewportClip(true);
#ifdef _DEBUG
     if(m_pMetafiler->metafile())
       checkJointMetafile(pMf, m_pMetafiler->metafile());
#endif
     super::shell(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
   }
 }

 static inline void calcImageSize(long& imageWidth, long& imageHeight
   , const double& fieldWidth, const double& fieldHeight, const double& coef
   , const double& dpiScale)
 {
   if(fieldHeight < fieldWidth)
   {
     imageHeight = long(odmax((fieldHeight / coef + 0.5)*dpiScale, 1.0));
     imageWidth = long(odmax(double(imageHeight) / fieldHeight * fieldWidth + 0.5, 1.0));
   }
   else
   {
     imageWidth = long(odmax((fieldWidth / coef + 0.5)*dpiScale, 1.0));
     imageHeight = long(odmax(double(imageWidth) / fieldWidth * fieldHeight + 0.5, 1.0));
   }
 }

void getAsImages(bool bFromMf, ShellDrawable* pDrw, const OdGeExtents3d& extents,
  PDF2dExportView& vect, OdGsDevice* pRenderDevice, OdGsView& newView, ImageArray& images)
{
  OdGeVector3d vec(1.0, 1.0, 0.0);
  OdGeMatrix3d mx = bFromMf ? vect.metafileToDeviceMatrix() : vect.objectToDeviceMatrix();
  vec.transformBy(mx.invert());
  // eugenea, #9217
  OdGeMatrix3d m;
  m.setToScaling((double)vect.device()->exportParams().getGeomDPI() / 72.);
  vec.transformBy(m);

  const double coef = fabs(vec.x);
  const OdUInt16 hatchDPI = vect.device()->getHatchDPI();
  const double dpiScale = double(hatchDPI) / 72.0;
  const OdGeVector3d dir = OdGeVector3d::kZAxis;
  const OdGeVector3d upVec = OdGeVector3d::kYAxis;
  double fieldWidth = extents.maxPoint().x - extents.minPoint().x;
  double fieldHeight = extents.maxPoint().y - extents.minPoint().y;
  long imageWidth, imageHeight;
  calcImageSize(imageWidth, imageHeight, fieldWidth, fieldHeight, coef, dpiScale);
  if(!isValidImageSize(imageWidth, imageHeight))
  {
    ShellArray subshells;
    getSubshells(subshells, pDrw->numVertices(), pDrw->vertexList(), pDrw->faceListSize(), pDrw->faceList(), pDrw->edgeData(), pDrw->faceData());
    const int numSubshell = subshells.length();
    images.reserve(numSubshell);
    const bool bEdgeData = !!pDrw->edgeData();
    const bool bFaceData = !!pDrw->faceData();
    for(int i = 0; i < numSubshell; ++i)
    {
      Shell& subshell = subshells[i];
      pDrw->resetFaces(subshell.m_faces.length(), subshell.m_faces.asArrayPtr()
        , bEdgeData ? &(subshell.m_edgeData) : NULL
        , bFaceData ? &(subshell.m_faceData) : NULL);

      fieldWidth = subshell.m_extents.maxPoint().x - subshell.m_extents.minPoint().x;
      fieldHeight = subshell.m_extents.maxPoint().y - subshell.m_extents.minPoint().y;

      calcImageSize(imageWidth, imageHeight, fieldWidth, fieldHeight, coef, dpiScale);
      if(!isValidImageSize(imageWidth, imageHeight))
      {
        images.setLogicalLength(0);
        break;
      }

      const OdGePoint3d target = (subshell.m_extents.minPoint() + subshell.m_extents.maxPoint().asVector())*0.5;
      OdAbstractViewPEPtr(&newView)->setView(&newView, target, dir, upVec, fieldWidth, fieldHeight, false);
      generateImage(images, pRenderDevice, vect.view().userGiContext(), vect.device(),
        subshell.m_extents, fieldWidth, fieldHeight, imageWidth, imageHeight);
    }
  }
  else
  {
    const OdGePoint3d target = (extents.minPoint() + extents.maxPoint().asVector())*0.5;
    OdAbstractViewPEPtr(&newView)->setView(&newView, target, dir, upVec, fieldWidth, fieldHeight, false);
    generateImage(images, pRenderDevice, vect.view().userGiContext(), vect.device(), extents,
      fieldWidth, fieldHeight, imageWidth, imageHeight);
  }
}

void PDF2dExportView::drawImages(const ImageData* aImg, OdUInt32 nImg)
{
  if(deviceGeom())
  {
    deviceGeom()->setHatchDrawingMode(true);
    for(OdUInt32 i = 0; i < nImg; ++i)
      rasterImageDc(aImg[i].m_origin, aImg[i].m_uVec, aImg[i].m_vVec, aImg[i].m_pImage, 0, 0, true);
    deviceGeom()->setHatchDrawingMode(false);
  }
  else
  {
    onTraitsModified();
    m_pMetafiler->flush(true);
    m_pMetafiler->add(new RecHatchDrawingMode(true));

    const OdUInt16 savedColorBitmapDPI = device()->getColorBimapDPI();
    const OdUInt16 hatchDPI = device()->getHatchDPI();
    if(hatchDPI != savedColorBitmapDPI)
      m_pMetafiler->add(new RecColorBitmapDPI(hatchDPI));

    OdGePoint2d uvBoundary[4]; //set boundary to avoid clipping in metafile
    uvBoundary[0].set(-0.5, -0.5);

    for(OdUInt32 i = 0; i < nImg; ++i)
    {
      OdUInt32 imgWidth = aImg[i].m_pImage->pixelWidth();
      OdUInt32 imgHeight = aImg[i].m_pImage->pixelHeight();
      uvBoundary[2].set(imgWidth + 0.5, imgHeight + 0.5);
      uvBoundary[1].set(uvBoundary[0].x, uvBoundary[2].y);
      uvBoundary[3].set(uvBoundary[2].x, uvBoundary[0].y);
      rasterImageDc(aImg[i].m_origin, aImg[i].m_uVec, aImg[i].m_vVec, aImg[i].m_pImage, uvBoundary, 4, true);
    }
    if(hatchDPI != savedColorBitmapDPI)
      m_pMetafiler->add(new RecColorBitmapDPI(savedColorBitmapDPI));

    m_pMetafiler->add(new RecHatchDrawingMode(false));
  }
}

OdGsDevicePtr PDF2dExportView::getBitmapDevice()
{
  if(!m_bitmapDevice.get())
    m_bitmapDevice = getRenderingDevice();
  return m_bitmapDevice;
}

void getShellAsImages(bool bFromMf, const OdGeExtents3d& extents,
  PDF2dExportView& vect, ImageArray& images,
  OdInt32 numVertices, const OdGePoint3d* vertexList,
  OdInt32 faceListSize, const OdInt32* faceList,
  const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData,
  const OdGiVertexData* pVertexData)
{
  OdGsDevicePtr pRenderDevice = vect.getBitmapDevice();
  if(!pRenderDevice.get())
    return;
  vect.onTraitsModified();

#if (defined(_MSC_VER) && (_MSC_VER > 1700)) || (defined(__cplusplus) && (__cplusplus > 199711L))
  OdSmartPtr<ShellDrawable> myTmpDrawable = OdRxObjectImpl2<ShellDrawable>::createObject(vect.effectiveTraits(),
    vect, numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
#else
  OdSmartPtr<ShellDrawable> myTmpDrawable = OdRxObjectImpl<ShellDrawable>::createObject();
  myTmpDrawable->init(vect.effectiveTraits(), vect, numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
#endif

  OdGsViewPtr pNewView = pRenderDevice->createView();
  pNewView->add(myTmpDrawable, 0);
  pRenderDevice->addView(pNewView);
  getAsImages(bFromMf, myTmpDrawable, extents, vect, pRenderDevice, *pNewView, images);
  for(unsigned i = 0; i < images.size(); ++i)
    images[i].m_pImage = images[i].m_pImage->clone();
  pRenderDevice->eraseView(pNewView.get());
}

class ShellGeometry : public OdStaticRxObject<OdGiConveyorNode>, public OdGiConveyorInput, public OdGiConveyorOutput, private OdGiEmptyGeometry
{
public:
  ShellGeometry(OdGiConveyorInput& inp, OdGiConveyorOutput& outp) :
    m_input(inp), m_output(outp), m_pResult(0)
  {
    m_input.removeSourceNode(m_output);
    this->addSourceNode(m_output);
  }
  ~ShellGeometry()
  {
    delete m_pResult;
    this->removeSourceNode(m_output);
    m_input.addSourceNode(m_output);
  }

  void addSourceNode(OdGiConveyorOutput& sourceNode)
  {
    sourceNode.setDestGeometry(*this);
  }
  void removeSourceNode(OdGiConveyorOutput& /*sourceNode*/)
  {
  }
  void setDestGeometry(OdGiConveyorGeometry& destGeometry)
  {
  }
  OdGiConveyorGeometry& destGeometry() const
  {
    return OdGiEmptyGeometry::kVoid;
  }
  OdGiConveyorInput& input() { return *this; }
  OdGiConveyorOutput& output() { return *this; }

  OdGiConveyorGeometry* optionalGeometry() { return this; }

  void shellProc(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData) ODRX_OVERRIDE
  {
    ODA_ASSERT(!m_pResult);
    if(!m_pResult)
      m_pResult = new RecHatchAsBitmap(numVertices, vertexList,
        faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
  }
  OdGiGeometryMetafile::Record* getResult()
  {
    OdGiGeometryMetafile::Record* pRec = m_pResult;
    m_pResult = NULL;
    return pRec;
  }

private:
  OdGiConveyorInput& m_input;
  OdGiConveyorOutput& m_output;
  OdGiGeometryMetafile::Record* m_pResult;
};

void PDF2dExportView::saveHatchToMetafiler(OdInt32 numVertices, const OdGePoint3d* vertexList,
  OdInt32 faceListSize, const OdInt32* faceList,
  const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData,
  const OdGiVertexData* pVertexData)
{
  onTraitsModified();
  m_pMetafiler->flush(true); //this is important. 

  ShellGeometry geom(m_pMetafiler->input(), output());
  super::shell(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
  OdGiGeometryMetafile::Record* pRec = geom.getResult();
  if(pRec)
    m_pMetafiler->add(pRec);
}

bool PDF2dExportView::drawHatchAsBitmap(
  const OdGeExtents3d& extents,
  OdUInt32 clipStat,
  OdInt32 numVertices, const OdGePoint3d* vertexList,
  OdInt32 faceListSize, const OdInt32* faceList,
  const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData)
{
  try
  {
    OdGsDevicePtr pRenderDevice = getBitmapDevice();
    if(!pRenderDevice.get())
      return false;

    if((!GETBIT(clipStat, OdGiConveyorGeometry::kBoundaryProcClip)) && !deviceGeom())
    {
      saveHatchToMetafiler(numVertices, vertexList,
        faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
      OdGiBaseVectorizerImpl::setExtents((const OdGePoint3d*)&extents);
      return true;
    }

#if (defined(_MSC_VER) && (_MSC_VER > 1700)) || (defined(__cplusplus) && (__cplusplus > 199711L))
    OdSmartPtr<ShellDrawable> myTmpDrawable = OdRxObjectImpl2<ShellDrawable>::createObject(effectiveTraits(),
      *this, numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
#else
    OdSmartPtr<ShellDrawable> myTmpDrawable = OdRxObjectImpl<ShellDrawable>::createObject();
    myTmpDrawable->init(effectiveTraits(), *this, numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
#endif

    OdGsViewPtr pNewView = pRenderDevice->createView();
    pNewView->add(myTmpDrawable, 0);
    pRenderDevice->addView(pNewView);

    ImageArray images;
    getAsImages(false, myTmpDrawable, extents, *this, pRenderDevice, *pNewView, images);
    pRenderDevice->eraseView(pNewView.get());
    if(images.isEmpty())
      return false;
    drawImages(images.asArrayPtr(), images.size());
  }
  catch(...)
  {
    return false;
  }
  return true;
}

bool PDF2dExportView::getHyperlink(const OdGiDrawable* pDrawable, HyperlinkData& dataToFill) const
{
  if(!GETBIT(device()->exportParams().exportFlags(), PDFExportParams::kExportHyperlinks))
    return false;
  if(!pDrawable || OdGsDbRootLinkage::isLayoutDrawable(pDrawable))
    return false;

  OdDbHyperlinkCollectionPtr collection;
  try
  {
    OdDbEntityHyperlinkPEPtr phPE = OdDbEntityHyperlinkPE::cast(pDrawable);
    if(phPE.get())
      collection = phPE->getHyperlinkCollection(pDrawable, true);
  }
  catch(const OdError&)
  {
    return false;
  }
  if(collection.isNull() || !collection->count())
    return false;

  OdString hLink = collection->item(0)->name();
  OdString subLocation = collection->item(0)->subLocation();
  if(hLink.isEmpty() && subLocation.isEmpty())
    return false;

  if(!hLink.isEmpty())
  {
    const bool looksLikeWebPage = hLink.getLength() > 4 && (hLink.left(4).iCompare(L"http") == 0 || hLink.left(4).iCompare(L"www.") == 0);
    if(looksLikeWebPage || subLocation.isEmpty())
    {
      hLink.replace('\\', '/');
      dataToFill.m_link = hLink;
      dataToFill.m_bInternal = false;
      return true;
    }
  }

  const PDFExportParams& params = device()->exportParams();
  const OdStringArray layouts = params.layouts();
  if(layouts.size() > 1 && !subLocation.isEmpty())
  {
    int idx = subLocation.find(L",");
    if(idx != -1)
      subLocation.deleteChars(0, idx + 1);
    if(layouts.contains(subLocation))
    {
      OdInt32 pageIndex(-1);
      if(device()->dbLayoutToPage())
        pageIndex = device()->dbLayoutToPage()->findPage(hLink, subLocation, params.database());
      if(pageIndex == -1) //still not found
      {
        for(OdUInt32 i = 0; i < layouts.size(); i++)
        {
          if(!layouts.at(i).compare(subLocation) && (params.databases().size() == 0 || params.databases().at(i).get() == params.database()))
          {
            pageIndex = i;
            break;
          }
        }
      }
      if(hLink.isEmpty() || pageIndex != -1)
      {
        // page number must be found for specified locations 
        // otherwise it means the given location is not exported and the link should be omitted
        dataToFill.m_link = subLocation;
        dataToFill.m_bInternal = true;
        dataToFill.m_page = pageIndex;
        return true;
      }
    }
  }
  return false;
}

OdGeMatrix3d PDF2dExportView::metafileToDeviceMatrix() const
{
  //NS, TODO: set m_bValidMfToDeviceMatrix flag in push/pop/setTransformForMetafileGeometry
  /*if(m_bValidMfToDeviceMatrix)
  {
    ODA_ASSERT((gsView()->worldToDeviceMatrix()*getTransformForMetafileGeometry()).isEqualTo(m_mfToDeviceMatrix));
  }
  else*/
  {
    m_mfToDeviceMatrix.setToProduct(gsView()->worldToDeviceMatrix(), getTransformForMetafileGeometry());
    m_bValidMfToDeviceMatrix = true;
  }
  return m_mfToDeviceMatrix;
}

static void textExtentsTo2d(PDF2dExportView& vect, const OdGeExtents3d& ext, bool bFromMf, OdGeExtents2d& res)
{
  ODA_ASSERT(vect.deviceGeom());
  OdGeMatrix3d mx = bFromMf ? vect.metafileToDeviceMatrix() : vect.objectToDeviceMatrix();
  OdGeExtents3d extents(ext);
  extents.transformBy(mx);
  //////////////////////////////////////////////////////////////////////////
  OdGeMatrix3d m;
  //unscale extents because of annotation coordinates are not in the content stream
  m.setToScaling(72. / (double)vect.device()->exportParams().getGeomDPI());
  extents.transformBy(m);
  //////////////////////////////////////////////////////////////////////////
  res.set((const OdGePoint2d&)(extents.minPoint()), (const OdGePoint2d&)(extents.maxPoint()));
}

void PDF2dExportView::passHyperlinkToGeometry(const HyperlinkData& data, const OdGeExtents3d& ext, bool bFromMf)
{
  ODA_ASSERT(deviceGeom());
  OdGeExtents2d rect;
  textExtentsTo2d(*this, ext, bFromMf, rect);

  if(!bFromMf && isFullyClipped(ext, m_modelEntryPoint.geometry())) //Check if entity with hyperlink is clipped
      return;

  OdString layer;
  if(deviceGeom()->isLayersSupported())
    layer = data.m_layer.isEmpty() ? this->layerName(subEntityTraits().layer()) : data.m_layer;
  deviceGeom()->dc_hyperlink(rect, data.m_link, layer, data.m_bInternal, data.m_page);
}

void PDF2dExportView::passTextAnnotationToGeometry(const OdString& sText, const OdString &layerName, const OdGeExtents3d& ext, bool bFromMf)
{
  ODA_ASSERT(deviceGeom());
  OdGeExtents2d rect;
  textExtentsTo2d(*this, ext, bFromMf, rect);
  OdString sTitle(OD_T("Text"));
  OdString layer;
  if(deviceGeom()->isLayersSupported())
    layer = layerName.isEmpty() ? this->layerName(subEntityTraits().layer()) : layerName;
  deviceGeom()->dc_textAnnot(rect, sTitle, sText, layerName);
}

OdInt32 PDF2dExportView::calculateTextLen(const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pStyle)
{
  OdDbBaseDatabase* pDb = drawContext()->giContext().database();
  OdBaseTextIteratorPtr itText = OdDbBaseDatabasePEPtr(pDb)->createTextIterator(pDb, msg, length, raw, pStyle);
  OdInt32 res = 0;

  for(bool bLast = false; !bLast; bLast = itText->currProperties().bLastChar)
  {
    OdUInt16 ch = itText->nextChar();
    if(ch == 0)
      break;
    else
      res++;
  }
  return res;
}

//Draws text in one or two passes if necessary for the purposes of Ctrl+F searchable text
void PDF2dExportView::doDrawText(const OdGePoint3d& position,
  const OdGeVector3d& normal,
  const OdGeVector3d& direction,
  const OdChar* msg,
  OdInt32 length,
  bool raw,
  const OdGiTextStyle* pTextStyle,
  bool bAsGeom)
{
  super::text(position, normal, direction, msg, length, raw, pTextStyle);
  if(!deviceGeom())
    return;
  if(exportSearchableTextOnly())
  {
    ODA_ASSERT(deviceGeom()->exportHiddenText()); //already set in beginViewViewctorization()
    return;
  }
  if(!bAsGeom)
    return;

  if(!device()->exportSearchableTextAsHiddenText())
    return;

  bool restartVpClip = false;
  if(!m_pViewportClip.isNull() && m_pViewportClip->enabled())
  {
    m_pViewportClip->disable();
    restartVpClip = true;
  }
  OdSaveStateFunc<PDF2dExportGeometry, bool> ssEHT(deviceGeom(),
    &PDF2dExportGeometry::exportHiddenText,
    &PDF2dExportGeometry::setExportHiddenText,
    true);
  super::text(position, normal, direction, msg, length, raw, pTextStyle);
  if(restartVpClip)
    m_pViewportClip->enable();
}

bool PDF2dExportView::isSearchableText(const OdGiTextStyle* pStyle) const
{
  return (pStyle->isTtfFont() && GETBIT(device()->exportParams().searchableTextType(), PDFExportParams::kTTF) ||
    pStyle->isShxFont() && GETBIT(device()->exportParams().searchableTextType(), PDFExportParams::kSHX));
}

static const void* lastRecord(const OdGiGeometryMetafile* pMf)
{
  return pMf ? pMf->lastRecord() : NULL;
}

void PDF2dExportView::setAnnotationText(const OdChar* msg, OdInt32 length, const OdGiTextStyle* pStyle)
{
  const bool bTextAsGeom = (pStyle->isTtfFont() && device()->exportParams().isTTFTextAsGeometry() ||
    pStyle->isShxFont() && device()->exportParams().isSHXTextAsGeometry());
  if(bTextAsGeom && !device()->exportSearchableTextAsHiddenText() && !exportSearchableTextOnly()
    && isSearchableText(pStyle))
    m_annotationText += OD_T(" ") + (length < 0 ? OdString(msg) : OdString(msg, length)); //export as annotation
}

void PDF2dExportView::text(const OdGePoint3d& position,
  const OdGeVector3d& normal,
  const OdGeVector3d& direction,
  const OdChar* msg,
  OdInt32 length,
  bool raw,
  const OdGiTextStyle* pStyle)
{
  if(!effectivelyVisible() || regenAbort())
    return;

  bool bTextAsGeom = (pStyle->isTtfFont() && device()->exportParams().isTTFTextAsGeometry() ||
      pStyle->isShxFont() && device()->exportParams().isSHXTextAsGeometry());

  OdGeVector3d u, v;
  ::odgiCalculateTextBasis(u, v, normal, direction, pStyle->textSize(), pStyle->xScale(), pStyle->obliquingAngle(), pStyle->isBackward(), pStyle->isUpsideDown());
  onTextProcessing(position, u, v);
  OdGePoint3d pMin, pMax, endPos;

  device()->userGiContext()->textExtentsBox(*pStyle, msg, length, 0, pMin, pMax, &endPos);
  if(!OdGeExtents3d(pMin, pMax).isValidExtents())
    return;// NS ??

  OdGeVector3d uNorm(u.normal()), vNorm(v.normal());
  OdGeBoundBlock3d bbChk(position + uNorm * pMin.x + vNorm * pMin.y, uNorm * (endPos.x - pMin.x), vNorm * (pMax.y - pMin.y), OdGePoint3d::kOrigin.asVector());
  OdUInt32 bbStatus = 0;
  m_modelEntryPoint.geometry().conveyorBoundaryInfoProc(bbChk, bbStatus);

  if(GETBIT(bbStatus, OdGiConveyorGeometry::kBoundaryProcClipFull)) // Text is fully clipped, skip vectorization
    return;

  if(gsWriter().isRecordingMetafile())
  {
    onTraitsModified();
    const void* pLastRec = lastRecord(m_pMetafiler->metafile());
    
    if(bTextAsGeom && !exportSearchableTextOnly())
      m_pMetafiler->setOptions(cMfDefaultOptions & ~OdGiMetafiler::kTextAsText);
    else
      m_pMetafiler->setOptions(cMfDefaultOptions);
    super::text(position, normal, direction, msg, length, raw, pStyle);
    m_pMetafiler->flush(); //if text is transformed into geometry there can be primitives just cached, but not added into MF, so we need to flush them

    if(pLastRec != lastRecord(m_pMetafiler->metafile()))
    {
      //passed through geometry conveyor and some geometry has been added to the metafile
      setAnnotationText(msg, length, pStyle);
    }

    if(GETBIT(bbStatus, OdGiConveyorGeometry::kBoundaryProcClip))
      return; //text will be clipped in conveyor and transformed to geometry, so it cannot be exported as hidden

    //process searchable text as hidden text
    if(bTextAsGeom && device()->exportSearchableTextAsHiddenText() && !exportSearchableTextOnly() &&
      isSearchableText(pStyle))
    {
      m_pMetafiler->add(new RecHiddenText(true));
      m_pMetafiler->setOptions(cMfDefaultOptions);
      super::text(position, normal, direction, msg, length, raw, pStyle);
      m_pMetafiler->add(new RecHiddenText(false));
    }
    return;
  }

  if(length > 0) //length can be < 0 for missing xrefs
  {
    onTraitsModified();
    setAnnotationText(msg, length, pStyle);

    if(GETBIT(bbStatus, OdGiConveyorGeometry::kBoundaryProcClip))
    { // Text is partially clipped, so we do not clip it at all
      bool restartVpClip = false;

      if(!m_pViewportClip.isNull() && m_pViewportClip->enabled())
      {
        m_pViewportClip->disable(); //disable VpClip to check if the text clipped by it or by clipping on the conveyor
        restartVpClip = true;
      }

      bbStatus = 0;
      m_modelEntryPoint.geometry().conveyorBoundaryInfoProc(bbChk, bbStatus);

      if(bTextAsGeom && !exportSearchableTextOnly() && restartVpClip) //restore disabled clipping to clip invisible geometry and save pdf size
      {
        m_pViewportClip->enable();
        restartVpClip = false;
      }

      if(GETBIT(bbStatus, OdGiConveyorGeometry::kBoundaryProcClip))
      { //if text is still clipped with disabled viewport clip, then there is the conveyor clip and the text will be transformed to geometry
        if(exportSearchableTextOnly())
        {
          if (restartVpClip)
            m_pViewportClip->enable();
          return; //if the text will be transformed to geometry, it is useless to export it as searchable, because of it won't be searchable anyway
        }
        if (bTextAsGeom && device()->exportSearchableTextAsHiddenText())
          bTextAsGeom = false; //set TextAsGeom to false, to prevent its export, because of it will be transformed in to geometry by conveyor clipper anyway
      }

      doDrawText(position, normal, direction, msg, length, raw, pStyle, bTextAsGeom);
      if(restartVpClip)
        m_pViewportClip->enable();
    }
    else
      doDrawText(position, normal, direction, msg, length, raw, pStyle, bTextAsGeom);
    return;
  }
  onTraitsModified();
  return doDrawText(position, normal, direction, msg, length, raw, pStyle, bTextAsGeom);
}

void PDF2dExportView::processPRC(const OdGiDrawable* pDrawable)
{
  //This method is called only when Gs cache is NOT used
  ODA_ASSERT((device()->exportParams().getPRCMode() != PDFExportParams::kDisabled));
  ODA_ASSERT(deviceGeom());
  m_bPRCMode = false;

  // apply filter to find you whether entity should be exported or not
  bool canExportToPRC = false;
  OdUInt32 viewportidx = -1;
  const OdGiPathNode* pGiPath = currentGiPath();

  if(!updateExtentsOnly() && device()->exportParams().getPRCContext())
    canExportToPRC = device()->exportParams().getPRCContext()->shouldExportAsPRC(pDrawable, pGiPath, &m_viewInfo, viewportidx);

  if(!canExportToPRC)
    return;

  // setup
  PDF2PRCExportParams prcParams;
  prcParams.m_arrDeviation.resize(5);
  prcParams.m_arrDeviation[kOdGiMaxDevForCircle] = getDeviation(kOdGiMaxDevForCircle, OdGePoint3d::kOrigin, true);
  prcParams.m_arrDeviation[kOdGiMaxDevForCurve] = getDeviation(kOdGiMaxDevForCurve, OdGePoint3d::kOrigin, true);
  prcParams.m_arrDeviation[kOdGiMaxDevForBoundary] = getDeviation(kOdGiMaxDevForBoundary, OdGePoint3d::kOrigin, true);
  prcParams.m_arrDeviation[kOdGiMaxDevForIsoline] = getDeviation(kOdGiMaxDevForIsoline, OdGePoint3d::kOrigin, true);
  prcParams.m_arrDeviation[kOdGiMaxDevForFacet] = getDeviation(kOdGiMaxDevForFacet, OdGePoint3d::kOrigin, true);

  // brep or mesh mode
  prcParams.m_exportAsBrep = device()->exportParams().getPRCMode() == PDFExportParams::kAsBrep;

  //get traits data
  prcParams.m_pTraitsData = &effectiveTraits();
  prcParams.m_pByBlockTraitsData = byBlockTraits();

  // compression
  PDF3D_ENUMS::PRCCompressionLevel compressLev;
  bool hasBrepCompression = device()->exportParams().hasPrcBrepCompression(compressLev);
  prcParams.setPRCCompression(compressLev, hasBrepCompression, device()->exportParams().hasPrcTessellationCompression());

  prcParams.m_bodyTransformationMatr = modelToWorldTransform();

  OdGeExtents3d extents;
  OdGeVector3d cameraVector = viewDir();
  OdGeVector3d normalToCamera = upVector();
  OdGeVector3d third = normalToCamera.crossProduct(cameraVector);
  OdGeMatrix3d cameraView;
  // using OdGePoint3d::kOrigin instead of position() is correct
  cameraView.setCoordSystem(OdGePoint3d::kOrigin, third, normalToCamera, cameraVector);

  device()->exportParams().getPRCContext()->getExtents(pDrawable, m_viewInfo.viewportObjectId, &(drawContext())->giContext(), cameraView, modelToWorldTransform(), extents);
  if(!extents.isValidExtents())
  {
    m_bPRCMode = false;
    return;
  }
  m_bPRCMode = true;

  // user context
  prcParams.m_extraOptions->putAt("user data", deviceGeom()->exportParams().getPRCContext()->userData());

  // export drawable to prc file and/or stream
  int viewIdx = 0;
  for(int i = 0; i < device()->numViews(); ++i)
  {
    if(device()->viewAt(i) == this)
    {
      viewIdx = i;
      break;
    }
  }

  std::map< OdUInt32, std::pair<OdRxObjectPtr, ViewExtents> > ::iterator it = deviceGeom()->m_prc_context.find(viewportidx);
  if(it == deviceGeom()->m_prc_context.end())
  {
    // no extents for this viewport yet => create new ones
    it = deviceGeom()->m_prc_context.insert(
      std::pair<OdUInt32, std::pair<OdRxObjectPtr, ViewExtents> >(viewportidx, std::pair<OdRxObjectPtr, ViewExtents>((OdRxObject *)NULL, ViewExtents()))
    ).first;

    //is this code correct? (m_viewIdx sets one time per every viewport)
    it->second.second.m_viewIdx = viewIdx;
  }
  it->second.second.m_extents.addExt(extents);
  prcParams.m_viewportIdx = viewportidx;
  prcParams.m_viewIdx = viewIdx;
  savePalleteFromDeviceToParams(prcParams, *device());
  OdResult result = deviceGeom()->dc_prc(pDrawable, prcParams);
  ODA_ASSERT(result == ::eOk);
}

void PDF2dExportView::processFrozenLayer(const OdString& name /*= L""*/)
{
  if(deviceGeom())
  {
    if(!name.isEmpty())
      deviceGeom()->open_Frozen_Layer(name);
    else
      deviceGeom()->close_Frozen_Layer();
  }
}

bool PDF2dExportView::doDrawImp(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable, const OdString &layerName)
{
  m_annotationText.empty();
  HyperlinkData curHl;
  OdSaveState<HyperlinkData*> hlss(m_curHl, getHyperlink(pDrawable, curHl) ? &curHl : NULL);
  if(m_curHl)
    m_curHl->m_layer = layerName;
  bool bRes = super::doDraw(drawableFlags, pDrawable);

  if(deviceGeom())
  {
    if(m_curHl || !m_annotationText.isEmpty())
    {
      OdGeExtents3d extents;
      if(pDrawable->getGeomExtents(extents) != ::eOk)
        return bRes;
      if(m_curHl)
        passHyperlinkToGeometry(curHl, extents, false);
      if(!m_annotationText.isEmpty())
      {
        passTextAnnotationToGeometry(m_annotationText, layerName, extents, false);
        m_annotationText.empty();
      }
    }
    return bRes;
  }
  if(m_curHl)
    passHyperlinkToMetafiler();

  if(!m_annotationText.isEmpty())
  {
    passTextAnnotationToMetafiler(layerName);
    m_annotationText.empty();
  }
  return bRes;
}

bool PDF2dExportView::isBlockRef(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable)
{
  if(!isGsCacheEnabled())
    return OdGsDbRootLinkage::isBlockRefDrawable(pDrawable);

  OdGsBlockReferenceNode *pNode = NULL;
  if(GETBIT(drawableFlags, OdGiDrawable::kDrawableIsCompoundObject))
  {
    OdGsNode *pCsh = NULL;
    // 1) Try to use cache attached to drawable (actual for top-level entities)
    if(gsWriter().gsModel())
      pCsh = static_cast<OdGsNode*>(gsWriter().gsModel()->drawableGsNode(pDrawable));
    if(pCsh && (pCsh->nodeType() == kEntityNode))
    {
      OdGsEntityNode *pEntNode = static_cast<OdGsEntityNode*>(pCsh);
      if(pEntNode->isReference())
        pNode = static_cast<OdGsBlockReferenceNode*>(pEntNode);
    }
    else // 2) Try to use queryX (actual for non top-level entities)
      pNode = static_cast<OdGsBlockReferenceNode*>(pDrawable->queryX(OdGsBlockReferenceNode::desc()));
  }
  return pNode != NULL;
}

bool PDF2dExportView::doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable)
{
  if(updateExtentsOnly() && !isGsCacheEnabled())
    return super::doDraw(drawableFlags, pDrawable);

  // simulation of frozen BlockRefs
  const bool isOffLayersEnabled = device()->isLayersSupported() && device()->isOffLayersEnabled();
  if(isOffLayersEnabled && device()->frozenLayerSimulationEnabled())
  {
    // start rendering of BlockReference
    if(isBlockRef(drawableFlags, pDrawable))
    {
      OdString layerName = this->layerName(subEntityTraits().layer());
      bool isFrozen = device()->isFrozenLayer(layerName);
      if(isFrozen)
      {
        layerName += PDF_FROZEN_LAYER;
        processFrozenLayer(layerName);
      }
      bool bRet = doDrawImp(drawableFlags, pDrawable, layerName);
      if(bRet && isFrozen)
        processFrozenLayer();
      return bRet;
    }
  }

  OdDbBaseHatchPE* pH = OdGsDbRootLinkage::isHatchDrawable(pDrawable) ?
    OdGsDbRootLinkage::getDbBaseHatchPE(pDrawable) : NULL;

  const PDFExportParams::PRCSupport pramsMode = device()->exportParams().getPRCMode();
  const bool bExportPrc = device()->exportParams().getPRCMode() != PDFExportParams::kDisabled;
  bool canExportHatchToPrc = bExportPrc && pH;

  if(!pH || canExportHatchToPrc) //(pH.isNull() || m_bExtentsMode /*|| !hatchPE.isGradient()*/)
  {
    bool canProcessPRC = bExportPrc;
    if(canProcessPRC && m_drawableFilterFunction)
    {
      const OdUInt32 filterFlags = giContext().drawableFilterFunction(m_drawableFilterFunction, pDrawable, drawableFlags);
      if(GETBIT(filterFlags, OdGiContext::kSkipVectorization))
        canProcessPRC = false;
    }
    if(canProcessPRC)
      processPRC(pDrawable);

    return m_bPRCMode ? true : doDrawImp(drawableFlags, pDrawable, OdString::kEmpty);
  }
  return drawHatch(drawableFlags, pDrawable, *pH);
}

bool PDF2dExportView::drawHatch(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable, OdDbBaseHatchPE& hatchPE)
{
  OdSaveState<bool> ssHb(m_bHatchToBitmapMode);
  m_bHatchToBitmapMode = (device()->exportParams().otherHatchesExportType() == PDFExportParams::kBitmap);
  return hatchPE.isSolidFill(pDrawable) ?
    drawSolidHatch(drawableFlags, pDrawable, hatchPE) : doDrawImp(drawableFlags, pDrawable, OdString::kEmpty);
}

bool PDF2dExportView::drawSolidHatch(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable, OdDbBaseHatchPE& hatchPE)
{
  // export non solid hatch using bitmap||vectorizer
  OdSaveState<OdGiDrawablePtr> ssSh(m_pSolidHatch);
  OdSaveState<bool> ssRc(m_bRestartClipper);

  if(hatchPE.isGradient(pDrawable))
  {
    m_bHatchToBitmapMode = (device()->exportParams().gradientHatchesExportType() == PDFExportParams::kBitmap);
  }
  else
  {
    m_bHatchToBitmapMode = true;
    switch(device()->exportParams().solidHatchesExportType())
    {
    case PDFExportParams::kPdfPaths:
    case PDFExportParams::kPolygons:
    {
      m_pSolidHatch = pDrawable;
      OdGeExtents3d extents;
      if((!gsWriter().isRecordingMetafile()) && (::eOk == pDrawable->getGeomExtents(extents)))
      {
        OdGeMatrix3d mx = objectToDeviceMatrix();
        extents.transformBy(mx);
        if(m_pViewportClip->checkExtentsVisibility(extents) == OdGiOrthoClipperEx::kStatusClipped)
          m_bRestartClipper = true;
      }
    }
    break;
    case PDFExportParams::kDrawing:
      m_bHatchToBitmapMode = false;
      break;
    }
  }
  return doDrawImp(drawableFlags, pDrawable, OdString::kEmpty);
}

void PDF2dExportView::passTextAnnotationToMetafiler(const OdString &layer)
{
  ODA_ASSERT(!deviceGeom());
  if(m_pMetafiler->metafile())
  {
    m_pMetafiler->flush();
    m_pMetafiler->add(new RecTextAnnot(m_annotationText, layer));
  }
  m_annotationText.empty();
}

void PDF2dExportView::update()
{
  Od2dExportView::update();
}

//====================================================================
//

//////////////////////////////////////////////////////////////////
double PDF2dExportView::calcScaledDeviation(const OdGiDeviationType type, const OdGePoint3d& pt) const
{
  if(!OdEqual(dcScreenMax().x, dcScreenMin().x) &&
    !OdEqual(dcScreenMax().y, dcScreenMin().y))
  {
    OdGePoint2d pixelsPerUnit;
    OdGsBaseVectorizer::getNumPixelsInUnitSquare(pt, pixelsPerUnit, false);
    pixelsPerUnit.x = fabs(pixelsPerUnit.x);
    pixelsPerUnit.y = fabs(pixelsPerUnit.y);
    if(type == kOdGiMaxDevForCircle)
    {
      unsigned int viewres = userGiContext()->circleZoomPercent(m_viewInfo.viewportObjectId);
      if(viewres < 1 || viewres > 20000)
        viewres = 100;
      double cft = double(viewres) / 100;
      pixelsPerUnit.x *= cft;
      pixelsPerUnit.y *= cft;
    }
    double deviation = 0.5 / pow(odmax(pixelsPerUnit.x, pixelsPerUnit.y), 1. / 64.);
    //calculate deviation this way gives us the same result as Autocad export
    //because the increased deviation will be compensated with increased radius of arcs
    if(OdGsBaseVectorizer::isPerspective())
    {
      OdGePoint3d eyePt(pt);
      eyePt.transformBy(viewingMatrix());
      deviation *= 1.0 - eyePt.z / focalLength();
      if(deviation < 0.0)
        deviation = -deviation;
    }
    return deviation;
  }
  return 1e100;
}

double PDF2dExportView::deviation(const OdGiDeviationType type, const OdGePoint3d& pt) const
{
  //Former kVpMaxDevForBoundary and kVpMaxDevForIsoline are used for other purposes now
  switch(type)
  {
  case kOdGiMaxDevForCircle:
    m_awareFlags.setFlag(kVpMaxDevForCircle); break;
  case kOdGiMaxDevForCurve:
  case kOdGiMaxDevForBoundary:
    m_awareFlags.setFlag(kVpMaxDevForCurve); break;
  default:
    m_awareFlags.setFlag(kVpMaxDevForFacet); break;
  }
  return view().getDeviation(type, pt, true/*!GETBIT(m_flags, kModelCache)*/); //recalculate always to provide the same result either with GsCache or w/o it
}

void PDF2dExportView::ownerDrawDc(const OdGePoint3d& _origin,
  const OdGeVector3d& _u,
  const OdGeVector3d& _v,
  const OdGiSelfGdiDrawable* pDrawable,
  bool bDcAligned,
  bool bAllowClipping)
{
  if(mode() != OdGsView::k2DOptimized)
    return;

  OdOleItemHandlerPtr pOLE = OdOleItemHandler::cast(pDrawable);
  if(pOLE.isNull()) {
    Od2dExportView::ownerDrawDc(_origin, _u, _v, pDrawable, bDcAligned, bAllowClipping);
    return;
  }

  OdOleItemHandler::PlotQuality plotQuality = pOLE->outputQuality();
  bool bMonochrome = false;
  double iScale = 1.;
  if(plotQuality == OdOleItemHandler::kMonochrome) {
    iScale = 72. / (double)device()->exportParams().bwImagesDPI();
    bMonochrome = true;
  }
  else
    iScale = 72. / (double)device()->exportParams().colorImagesDPI();

  OdGeMatrix3d x = getModelToEyeTransform();

  OdGePoint3d origin(x * _origin);
  OdGeVector3d u(x * _u), v(x * _v);

  OdGeExtents3d ext(origin, origin);
  ext.addPoint(origin + u);
  ext.addPoint(origin + v);
  ext.addPoint(origin + u + v);

  OdGePoint3d imgMin = ext.minPoint();
  OdGePoint3d imgMax = ext.maxPoint();

  OdGePoint3d origin2;
  OdGeVector3d u2;
  OdGeVector3d v2;
  origin2 = imgMin;
  v2 = u2 = imgMax - imgMin;
  u2.z = u2.y = 0.;
  v2.z = v2.x = 0.;

  x = view().eyeToScreenMatrix();

  origin2.transformBy(x);
  u2.transformBy(x);
  v2.transformBy(x);

  OdGeVector3d u2Tmp = (u2 / ((double)device()->exportParams().getGeomDPI() / 72.)) / iScale; //downscale vectors because of geomDPI (see SUP-716)
  OdGeVector3d v2Tmp = (v2 / ((double)device()->exportParams().getGeomDPI() / 72.)) / iScale;
  //u2 /= iScale;
  //v2 /= iScale;

  OdUInt64 rasterWidth, rasterHeight;
  try
  {
    rasterWidth = OdRoundToLong(fabs(u2Tmp.x));
    rasterHeight = OdRoundToLong(fabs(v2Tmp.y));
    if(rasterWidth * rasterHeight > 48 * 1024 * 1024) // CreateDibBitmap limitation (huge rasters are scaled anyway)
    {
      double scale = sqrt((((double)(rasterWidth) / 1024.) * ((double)(rasterHeight) / 1024.)) / 48.);//that is equal to (rasterWidth * rasterHeight) / (48 * 1024 * 1024);
      if(OdLessOrEqual(scale, 1.))
        scale = 1.1;
      while(rasterWidth * rasterHeight > 48 * 1024 * 1024)
      {
        rasterWidth = OdRoundToLong((double)rasterWidth / scale);
        rasterHeight = OdRoundToLong((double)rasterHeight / scale);
      }
    }
  }
  catch(const OdError&)
  {
    // #10225 : Can't render if arithmetic overflow thrown
    return;
  }
  //u2 *= iScale;
  //v2 *= iScale;

  OdGsDCRect rect; // RECT 
  rect.m_min.x = 0; // left
  rect.m_min.y = rasterHeight; // top
  rect.m_max.x = rasterWidth; // right
  rect.m_max.y = 0; // bottom 

  OdSmartPtr<OdGiSelfGiDrawablePE> pOdGiSelfGiDrawablePE = OdGiSelfGiDrawablePE::cast(pDrawable);
  if(pOdGiSelfGiDrawablePE.get() && pOdGiSelfGiDrawablePE->draw(const_cast<OdGiSelfGdiDrawable*>(pDrawable), &output().destGeometry(), origin2, u2, v2, rect, true))
    return;

  OdGeMatrix3d m2w;
  if(isGsCacheEnabled())
  {
    //do not need to transform when metafile is active. Transformations will be applied on metafile playing
    //we just need to recalculate u and v to handle if OLE has negative width or height
    OdGeExtents3d ext(_origin, _origin);
    ext.addPoint(_origin + _u);
    ext.addPoint(_origin + _v);
    ext.addPoint(_origin + _u + _v);

    OdGePoint3d imgMin = ext.minPoint();
    OdGePoint3d imgMax = ext.maxPoint();

    origin2 = imgMin;
    v2 = u2 = imgMax - imgMin;
    u2.z = u2.y = 0.;
    v2.z = v2.x = 0.;
    //////////////////////////////////////////////////////////////////////////
    m2w = m_pModelToEyeProc->modelToWorldTransform();
  }
  drawWithGdiBmp(pDrawable, this, m2w*origin2, m2w*u2, m2w*v2, rasterWidth, rasterHeight, rect, bMonochrome);
}

void PDF2dExportView::nurbs(const OdGeNurbCurve3d& nurbs)
{
  if(isGsCacheEnabled())
  {
    const bool bSavedFlag = GETBIT(drawContextFlags(), kEmbranchmentSimplNurbs);
    try
    {
      setDrawContextFlags(kEmbranchmentSimplNurbs, false);
      super::nurbs(nurbs);
      setDrawContextFlags(kEmbranchmentSimplNurbs, bSavedFlag);
    }
    catch(...)
    {
      setDrawContextFlags(kEmbranchmentSimplNurbs, bSavedFlag);
    }
  }
  else
    super::nurbs(nurbs);
}

double PDF2dExportView::linetypeGenerationCriteria() const
{
  m_awareFlags.setFlag(kVpViewport);
  return super::linetypeGenerationCriteria();
}

}
