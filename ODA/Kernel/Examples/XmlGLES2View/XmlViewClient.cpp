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
// GLES2 Xml viewer for Windows

#include "OdaCommon.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"
#include "DynamicLinker.h"

#include "XmlGlesParser.h"
#include "BinaryGlesParser.h"

#include "TrVisRenderClient.h"
#include "TrRndLocalViewer.h"
#include "TrRndLocalRenditionDefaultClient.h"
#include "TrRndLocalTransient.h"
#include "TrRndRenderModule.h"
#include "TrVisRenditionRecorder.h"
#include "TrVisIdGenerator.h"
#include "AppClient.h"
#include "AnimationHost.h"
#include "ExtBar.h"

class OdGLES2LocalTracker : public OdTrRndLocalTransient
{
  protected:
    OdGePoint3d m_pt;
    OdGePoint3d m_lastPt;
  public:
    OdGLES2LocalTracker() {}

    void setPoint(const OdGePoint3d &pt) { m_pt = m_lastPt = pt; }

    virtual void updateTracker(const OdGePoint3d &pt)
    {
      m_lastPt = pt;
      invalidate();
    }

    virtual void getRect(OdGePoint3d &pt1, OdGePoint3d &pt2)
    {
      pt1 = m_pt; pt2 = m_lastPt;
    }

    typedef OdSmartPtr<OdGLES2LocalTracker> Ptr;
};

class OdGLES2LocalSelectionTracker : public OdGLES2LocalTracker
{
  public:
    OdGLES2LocalSelectionTracker() {}

    virtual void draw(OdTrVisMetafileBuilderPtr mfBuilder, const OdTrRndLocalRenderer* pRenderer)
    {
      OdTrVisGeometryDraw* geom = mfBuilder->scsGeometry();
      const OdGeVector3d hDiff(m_lastPt.x - m_pt.x, 0.0, 0.0);
      const OdGeVector3d vDiff(0.0, m_lastPt.y - m_pt.y, 0.0);
      const OdGePoint3d plPt[5] = { m_pt, m_pt + hDiff, m_pt + hDiff + vDiff, m_pt + vDiff, m_pt };
      OdTrVisColorRGBA cl;
      if (hDiff.x < 0.0) cl.set(0, 255, 0, 128);
      else               cl.set(0, 0, 255, 128);
      mfBuilder->setColor(cl);
      geom->polygon(4, plPt);
      cl.set_a(255);
      mfBuilder->setColor(cl);
      geom->polyline(5, plPt, OdTrVisGeometryDraw::kLineStrip);
    }

    static OdGLES2LocalTracker::Ptr create(const OdGePoint3d &pt)
    {
      OdSmartPtr<OdGLES2LocalSelectionTracker> pTracker = OdRxObjectImpl<OdGLES2LocalSelectionTracker>::createObject();
      pTracker->setPoint(pt);
      return pTracker;
    }
};

class OdGLES2LocalSecPlaneTracker : public OdGLES2LocalTracker
{
  public:
    OdGLES2LocalSecPlaneTracker() {}

    virtual void draw(OdTrVisMetafileBuilderPtr mfBuilder, const OdTrRndLocalRenderer* pRenderer)
    {
      OdTrRndLocalViewerPtr pViewer(pRenderer);
      const double dS = pViewer->renderer()->getOverlayExtents(pViewer->activeViewportId(), kTrVisMainOverlayId).minPoint().distanceTo(
                        pViewer->renderer()->getOverlayExtents(pViewer->activeViewportId(), kTrVisMainOverlayId).maxPoint());
      OdTrVisViewportId vpId = pViewer->activeViewportId();
      OdTrVisGeometryDraw* geom = mfBuilder->wcsGeometry();
      for (OdUInt32 nPlane = 0; nPlane < pViewer->sectionPlanesNumber(vpId); nPlane++)
      {
        const OdTrVisSectionPlane *pPlane = pViewer->getSectionPlane(vpId, nPlane);
        const OdGeVector3d hDiff = pPlane->normal().perpVector() * dS;
        const OdGeVector3d vDiff = pPlane->normal().crossProduct(hDiff).normal() * dS;
        m_pt = pPlane->position() - hDiff / 2.0 - vDiff / 2.0;
        const OdGePoint3d plPt[5] = {
          m_pt, m_pt + hDiff, m_pt + hDiff + vDiff, m_pt + vDiff, m_pt
        };
        OdTrVisColorRGBA cl;
        cl.set( 128, 128, 128, 128 );
        mfBuilder->setColor( cl );
        geom->polygon(4, plPt);
      }
    }

    static OdGLES2LocalTracker::Ptr create()
    {
      OdSmartPtr<OdGLES2LocalSecPlaneTracker> pTracker = OdRxObjectImpl<OdGLES2LocalSecPlaneTracker>::createObject();
      return pTracker;
    }
};

static const OdUInt32 ExCmdElem_Background  = 100;
static const OdUInt32 ExCmdElem_Extents     = 101;
static const OdUInt32 ExCmdElem_RenderMode  = 102;
static const OdUInt32 ExCmdElem_LayerSel    = 103;
static const OdUInt32 ExCmdElem_LayerChange = 104;
static const OdUInt32 ExCmdElem_AddPlane    = 105;
static const OdUInt32 ExCmdElem_RemovePlane = 106;
static const OdUInt32 ExCmdElem_RotateT     = 107;
static const OdUInt32 ExCmdElem_RotateL     = 108;
static const OdUInt32 ExCmdElem_RotateR     = 109;
static const OdUInt32 ExCmdElem_RotateB     = 110;

static struct XmlViewRenderClient : public AppClient, protected OdTrRndLocalRenderer::SelectionReactor
{
  enum CaptureState
  {
    kNoCapture = 0,
    kPanning,
    kOrbit,
    kSelection
  };

  AppHandle m_appHandle;
  CaptureState m_capState;

  OdTrRndLocalViewerPtr ViewRenderer;
  OdGLES2LocalTracker::Ptr Tracker;

  OdTrVisGenericIdGenerator HlBranchIdGen;

  OdXmlGlesIdMapper m_idMapper;

  AppHandle appHandle() const { return m_appHandle; }

  OdTrRndLocalViewerPtr createRenderer() {
    OdTrRndRenderModulePtr pRenderModule = ::odrxDynamicLinker()->loadModule(OdTrGL2ModuleName, false);
    return pRenderModule->createLocalViewer(OdTrRndLocalRenditionDefaultClient::createObject()); }
  OdTrRndLocalViewer *viewer() { return ViewRenderer.get(); }
  OdTrRndLocalRenderer *renderer() { return viewer()->renderer(); }
  OdTrVisRendition *rendition() { return renderer()->rendition(); }

  struct RenderClientImpl : public OdTrVisRenderClientPropImpl
  {
    XmlViewRenderClient *m_pAppClient;
    OdAnsiString m_lastWarn;

    RenderClientImpl() : m_pAppClient(NULL)
    {
      setProperty(OD_T("DoubleBufferEnabled"), OdRxVariantValue(true));
      setProperty(OD_T("UseSceneGraph"), OdRxVariantValue(true));
      setProperty(OD_T("BlendingMode"), OdRxVariantValue((OdUInt32)OdTrVisRenderClient::kBlendingSinglePassOIT));
    }

    virtual bool usePartialUpdate() const
    { // This property isn't currently configurable through properties dictionary.
      return true;
    }

    // Output window size
    virtual int outputWindowWidth() const
    {
      OdGsDCRect clientRect;
      m_pAppClient->getSize(m_pAppClient->appHandle(), clientRect);
      return clientRect.m_max.x - clientRect.m_min.x;
    }
    virtual int outputWindowHeight() const
    {
      OdGsDCRect clientRect;
      m_pAppClient->getSize(m_pAppClient->appHandle(), clientRect);
      return clientRect.m_min.y - clientRect.m_max.y;
    }

    // Rendering process handlers
    virtual void emitError(const char *pError)
    {
      m_pAppClient->displayError(m_pAppClient->appHandle(), pError);
      throw pError;
    }

    virtual void emitWarning(const char *pWarn)
    {
      if (pWarn != m_lastWarn)
      {
        m_pAppClient->displayWarning(m_pAppClient->appHandle(), pWarn);
        m_lastWarn = pWarn;
      }
    }
  } ViewRenderClient;

  void setAppHandle(AppHandle appHandle)
  {
    m_appHandle = appHandle;
    ViewRenderClient.setProperty(OD_T("WindowHWND"), OdRxVariantValue((OdIntPtr)appHandle));
  }

  XmlViewRenderClient()
    : m_appHandle(NULL)
    , m_capState(kNoCapture)
  {
    AppClient::registerAppClient(this);
    ViewRenderClient.m_pAppClient = this;
  }
  ~XmlViewRenderClient()
  {
    AppClient::unregisterAppClient(this);
  }

  virtual const char *appClientName() const { return "OdTrRndLocalViewer"; }
  virtual bool isDefaultAppClient() const { return true; }

  virtual void onCreate(AppHandle appHandle)
  {
    setAppHandle(appHandle);
    ViewRenderer = createRenderer();
    // Fill Extensions Bar
    getExtBar(appHandle).createButton(ExCmdElem_Background)->setText(L"Background Color");
    getExtBar(appHandle).createButton(ExCmdElem_Extents)->setText(L"Recompute Extents");
    ExtBar::GroupElement *pGroup = getExtBar(appHandle).createGroup(ExCmdElem_RenderMode + 100);
    ExtBar::TextElement::cast(pGroup)->setText(L"Render Mode");
    ExtBar::ListElement *pList = getExtBar(appHandle).createComboBox(ExCmdElem_RenderMode, pGroup);
    pList->addString(L"2dWireframe"); pList->addString(L"3dWireframe"); pList->addString(L"HiddenLine");
    pList->addString(L"FlatShaded"); pList->addString(L"GouraudShaded");
    pList->addString(L"FlatShadedWithWireframe"); pList->addString(L"GouraudShadedWithWireframe");
    pGroup = getExtBar(appHandle).createGroup(ExCmdElem_LayerSel + 100);
    ExtBar::TextElement::cast(pGroup)->setText(L"Layers");
    pList = getExtBar(appHandle).createListBox(ExCmdElem_LayerSel, pGroup);
    getExtBar(appHandle).createCheckBox(ExCmdElem_LayerChange, pGroup)->setText(L"Visible Layer");
    getExtBar(appHandle).getElement(ExCmdElem_LayerChange)->setDisabled(true);
    pGroup = getExtBar(appHandle).createGroup(ExCmdElem_AddPlane + 100);
    ExtBar::TextElement::cast(pGroup)->setText(L"Section Plane");
    getExtBar(appHandle).createButton(ExCmdElem_AddPlane, pGroup)->setText(L"Add Plane");
    getExtBar(appHandle).createButton(ExCmdElem_RemovePlane, pGroup)->setText(L"Remove Plane");
    getExtBar(appHandle).createPlaceholder(pGroup);
    //
    getExtBar(appHandle).createPlaceholder(pGroup, ExtBar::k2d8); getExtBar(appHandle).createPlaceholder(pGroup, ExtBar::k1d8);
    getExtBar(appHandle).createButton(ExCmdElem_RotateT, pGroup, ExtBar::k2d8)->setText(L"^");
    getExtBar(appHandle).createPlaceholder(pGroup, ExtBar::k2d8); getExtBar(appHandle).createPlaceholder(pGroup, ExtBar::k1d8);
    //
    getExtBar(appHandle).createPlaceholder(pGroup, ExtBar::k1d8);
    getExtBar(appHandle).createButton(ExCmdElem_RotateL, pGroup, ExtBar::k2d8)->setText(L"<");
    getExtBar(appHandle).createPlaceholder(pGroup, ExtBar::k2d8);
    getExtBar(appHandle).createButton(ExCmdElem_RotateR, pGroup, ExtBar::k2d8)->setText(L">");
    getExtBar(appHandle).createPlaceholder(pGroup, ExtBar::k1d8);
    //
    getExtBar(appHandle).createPlaceholder(pGroup, ExtBar::k2d8); getExtBar(appHandle).createPlaceholder(pGroup, ExtBar::k1d8);
    getExtBar(appHandle).createButton(ExCmdElem_RotateB, pGroup, ExtBar::k2d8)->setText(L"_");
    getExtBar(appHandle).createPlaceholder(pGroup, ExtBar::k2d8); getExtBar(appHandle).createPlaceholder(pGroup, ExtBar::k1d8);
    updateSectionIface(appHandle);
    // Complete Extensions Bar
    updateExtBar(appHandle);
  }
  virtual void onDestroy(AppHandle /*appHandle*/)
  {
    Tracker.release();
    ViewRenderer.release();
  }

  virtual const OdChar *supportFormats(AppHandle /*appHandle*/, OdString &defExt)
  {
    defExt = L"xml";
    return L"Xml Files (*.xml)\0*.xml\0"
           L"GS Binary Files (*.tgs)\0*.tgs\0"
           L"All Files (*.*)\0*.*\0\0";
  }
  bool onOpenImpl(AppHandle /*appHandle*/, const OdString &fileName, OdTrVisRendition *pRendition)
  {
    OdString sFileName(fileName),
      sExt = L".xml";
    int pos = sFileName.reverseFind('.');
    if (pos > 0)
      sExt = sFileName.mid(pos).makeLower();

    // Load rendering data
    OdSharedPtr<OdBaseGlesParser> pXmlParser;
    if (sExt == L".tgs")
      pXmlParser = new OdBinaryGlesParser( &m_idMapper );
    else
      pXmlParser = new OdXmlGlesParser( &m_idMapper );
    if (!pXmlParser->parse(sFileName, pRendition))
      return false;
    return true;
  }
  void finalizeOpen(AppHandle appHandle, OdTrRndLocalViewer *pRenderer)
  {
    // Set currently rendering rendition
    if (viewer() != pRenderer)
      ViewRenderer = pRenderer;
    // Set rendering area size
    OdGsDCRect rArea;
    getSize(appHandle, rArea);
    viewer()->resize(rArea);
    // Configure data representation
    renderer()->setViewportBorderProps(OdTrRndLocalRenderer::kVpBorderMS,
      OdTrRndLocalRenderer::ViewportBorderProps(OdTrRndLocalRenderer::kVpBorderEnabled, ODRGBA(255, 255, 255, 255), 2));
    renderer()->setViewportBorderProps(OdTrRndLocalRenderer::kVpBorderMSActive, renderer()->getViewportBorderProps(OdTrRndLocalRenderer::kVpBorderMS));
    renderer()->setViewportBorderProps(OdTrRndLocalRenderer::kVpBorderPSActive,
      OdTrRndLocalRenderer::ViewportBorderProps(OdTrRndLocalRenderer::kVpBorderEnabled, ODRGBA(0, 0, 0, 255), 2));
    // Apply extension bar data
    ExtBar::ListElement::cast(getExtBar(appHandle).getElement(ExCmdElem_RenderMode))->setSelString(viewer()->renderMode(viewer()->activeViewportId()));
    { // Fill layers list
      OdTrRndLocalViewer::LayersList layers;
      viewer()->getLayersList(layers);
      ExtBar::ListElement *pList = ExtBar::ListElement::cast(getExtBar(appHandle).getElement(ExCmdElem_LayerSel));
      pList->clear(); getExtBar(appHandle).getElement(ExCmdElem_LayerChange)->setDisabled(true);
      OdUInt32 nLayers = layers.size();
      const OdTrVisLayerId *pLayers = layers.getPtr();
      for (OdUInt32 nLayer = 0; nLayer < nLayers; nLayer++)
        pList->addString(viewer()->getLayerName(pLayers[nLayer]));
    }
  }

  class RestoreRenderer
  {
    OdTrRndLocalViewerPtr pOldRenderer;
    OdTrRndLocalViewerPtr &pActiveRenderer;
    OdTrVisRenderClient *pRenderClient;
    public:
      RestoreRenderer(OdTrRndLocalViewerPtr &pCurRenderer, OdTrRndLocalViewer *pNewRenderer, OdTrVisRenderClient *_pRenderClient)
        : pOldRenderer(pCurRenderer), pActiveRenderer(pCurRenderer), pRenderClient(_pRenderClient)
      {
        pActiveRenderer = pNewRenderer;
        if (!pNewRenderer) pOldRenderer.release();
        else pOldRenderer->renderer()->detachWindowContext(),
          pNewRenderer->renderer()->rendition()->onDeviceUpdateBegin(pRenderClient);
        
      }
      void acceptSubstitution() { pOldRenderer.release(); }
      ~RestoreRenderer()
      {
        pActiveRenderer->renderer()->rendition()->onDeviceUpdateEnd(pRenderClient);
        if (!pOldRenderer.isNull())
          pActiveRenderer = pOldRenderer,
          pActiveRenderer->renderer()->attachWindowContext(pRenderClient);
      }
  };

  virtual void onOpen(AppHandle appHandle, const OdString &fileName)
  {
    // Create new rendition
    RestoreRenderer _rr(ViewRenderer, createRenderer(), &ViewRenderClient);
    m_idMapper.reset();
    // Open file
    if (!onOpenImpl(appHandle, fileName, ViewRenderer->renderer()->rendition()))
    {
      ViewRenderClient.emitError("Can't parse input file.");
      return;
    }
    // Complete opening
    _rr.acceptSubstitution();
    finalizeOpen(appHandle, ViewRenderer);
  }
  virtual bool isSupportMultiOpen(AppHandle /*appHandle*/) const { return true; }
  virtual void onMultiOpen(AppHandle appHandle, const OdString &directory, OdUInt32 nFiles, const OdString *pFiles, bool bResetContents)
  {
    // Create new rendition
    RestoreRenderer _rr(ViewRenderer, (bResetContents) ? createRenderer().get() : NULL, &ViewRenderClient);
    if (bResetContents)
      m_idMapper.reset();
    // Open files
    for (OdUInt32 nFile = 0; nFile < nFiles; nFile++)
    {
      OdString fileName = directory + pFiles[nFile];
      if (!onOpenImpl(appHandle, fileName, ViewRenderer->renderer()->rendition()))
      {
        OdString fmtStr;
        fmtStr.format(OD_T("Can't parse input file: %s."), pFiles[nFile].c_str());
        ViewRenderClient.emitError(fmtStr);
        return;
      }
    }
    // Complete opening
    finalizeOpen(appHandle, ViewRenderer);
  }

  class XmlAnimationClient : public AnimationClient, public RestoreRenderer, public OdStaticRxObject<OdTrVisRenditionRedir>
  {
    protected:
      XmlViewRenderClient *m_pXmlRender;
      AnimationHost *m_pAnimHost;
      OdString m_fileName;
      OdStaticRxObject<OdTrVisRenditionRecorder> m_recorder;
      OdStaticRxObject<OdTrVisRenditionDef> m_skipper;
      enum Mode
      {
        kPlaying, kPaused, kSkipping
      } m_mode;
      bool m_bResult;
    public:
      XmlAnimationClient(XmlViewRenderClient *pRenderClient, const OdString &fileName, AnimationHost *pAnimHost)
        : RestoreRenderer(pRenderClient->ViewRenderer, pRenderClient->createRenderer(), &pRenderClient->ViewRenderClient)
        , m_pXmlRender(pRenderClient), m_pAnimHost(pAnimHost), m_fileName(fileName), m_mode(kPlaying), m_bResult(false)
      { setRedirection(&m_recorder); setInformationalRedirection(pRenderClient->ViewRenderer->renderer()->rendition());
        onDeviceUpdateEnd(&pRenderClient->ViewRenderClient) ;}
      ~XmlAnimationClient()
      {
        if (!m_bResult)
          m_pXmlRender->ViewRenderClient.emitError("Can't parse input file.");
        else
        {
          acceptSubstitution();
          m_pXmlRender->finalizeOpen(m_pXmlRender->appHandle(), m_pXmlRender->ViewRenderer);
        }
      }

      virtual bool process(AnimationHost *pAnimHost)
      {
        pAnimHost->updateStatus("Parsing");
        m_bResult = m_pXmlRender->onOpenImpl(m_pXmlRender->appHandle(), m_fileName, this);
        return false;
      }
      void processFrame(AnimationHost *pAnimHost)
      {
        pAnimHost->updateStatus("Applying");
        onDeviceUpdateBegin(&m_pXmlRender->ViewRenderClient);
        m_recorder.playRecords(getInformationalRedirection(), true);
        onDeviceUpdateEnd(&m_pXmlRender->ViewRenderClient);
        pAnimHost->updateStatus("Waiting");
        if (m_mode == kPlaying)
          pAnimHost->openGate();
      }

      virtual void finalize(AnimationHost *pAnimHost)
      {
        pAnimHost->updateStatus("Finalizing");
        onDeviceUpdateBegin(&m_pXmlRender->ViewRenderClient);
        m_recorder.playRecords(getInformationalRedirection(), true);
        delete this;
      }

      virtual void play(AnimationHost *pAnimHost)
      {
        m_mode = kPlaying;
        pAnimHost->openGate();
      }
      virtual void pause(AnimationHost * /*pAnimHost*/)
      {
        m_mode = kPaused;
      }
      virtual void stop(AnimationHost *pAnimHost)
      {
        m_mode = kSkipping;
        pAnimHost->openGate(true);
      }
      virtual void forward(AnimationHost *pAnimHost)
      {
        pAnimHost->openGate(true);
      }

      virtual void onFrameSwitchInfo(OdUInt32 nInterval)
      {
        OdTrVisRenditionRedir::onFrameSwitchInfo(nInterval);
        if (m_mode == kSkipping)
        {
          m_pAnimHost->updateStatus("Skipping");
          if (getRedirection() != &m_skipper)
            setRedirection(&m_skipper);
          return;
        }
        struct ProcessFrame : public AnimationClient
        {
          XmlAnimationClient *m_pClient;
          ProcessFrame(XmlAnimationClient *pClient)
            : m_pClient(pClient) { }
          virtual bool process(AnimationHost *pAnimHost)
          {
            m_pClient->processFrame(pAnimHost);
            delete this;
            return false;
          }
        };
        m_pAnimHost->processThreadCallback(new ProcessFrame(this));
        m_pAnimHost->closeGate(nInterval);
      }

      virtual void onSurfaceChanged(const OdTrVisSurfaceDef &pDef)
      { OdTrVisSurfaceDef correctedDef(pDef);
        m_pXmlRender->getSize(m_pXmlRender->appHandle(), correctedDef.m_surfaceSize);
        OdTrVisRenditionRedir::onSurfaceChanged(correctedDef);
      }
  };
  virtual bool isSupportAnimation(AppHandle /*appHandle*/) const { return true; }
  virtual void onOpenAnimation(AppHandle appHandle, const OdString &fileName, AnimationHost *pAnimHost)
  {
    m_idMapper.reset();
    pAnimHost->runAnimationLoop(new XmlAnimationClient(this, fileName, pAnimHost));
  }

  virtual void onDraw(AppHandle /*appHandle*/)
  {
    try
    {
      viewer()->update(&ViewRenderClient);
    }
    catch (const char*)
    {
      close(appHandle());
    }
  }
  virtual void onSize(AppHandle /*appHandle*/, const OdGsDCRect &size)
  {
    viewer()->resize(size);
  }

  virtual void onLMouseButtonDown(AppHandle /*appHandle*/, int x, int y, bool bDoubleClick)
  {
    if (viewer()->activeViewportId() != kTrVisNegativeId)
    {
      if (!bDoubleClick)
      {
        if (m_capState == kNoCapture)
        {
          capture(appHandle());
          m_capState = kSelection;
          Tracker = OdGLES2LocalSelectionTracker::create(OdGePoint3d((double)x, (double)y, 0.0));
          renderer()->transientManager()->attachTransient(viewer()->activeSelectionViewportId(), Tracker.get());
        }
      }
      else
      {
        if (m_capState == kNoCapture)
        {
          OdTrVisViewportId vpId = viewer()->pickViewport(x, y);
          if (vpId != kTrVisNegativeId)
          {
            viewer()->setActiveViewport(vpId);
            ExtBar::ListElement::cast(getExtBar(appHandle()).getElement(ExCmdElem_RenderMode))->setSelString(viewer()->renderMode(viewer()->activeViewportId()));
            redraw(appHandle());
          }
        }
      }
    }
  }
  virtual void onLMouseButtonUp(AppHandle /*appHandle*/, int /*x*/, int /*y*/)
  {
    if (m_capState == kSelection)
    {
      OdGePoint3d pt1, pt2;
      Tracker->getRect(pt1, pt2);
      OdGsDCRect rect; OdTrRndLocalRenderer::SelectionType selType = OdTrRndLocalRenderer::kSelCrossing;
      bool selOnce = false;
      if (pt1 == pt2)
      {
        rect = OdGsDCRect(int(pt1.x) - 5, int(pt1.x) + 5, int(pt1.y) + 5, int(pt1.y) - 5);
        selOnce = true;
      }
      else
      {
        int ptLastX = (int)pt2.x, ptLastY = (int)pt2.y;
        int ptX = (int)pt1.x, ptY = (int)pt1.y;
        if (ptX > ptLastX) { int tmp = ptX; ptX = ptLastX; ptLastX = tmp; selType = OdTrRndLocalRenderer::kSelWindow; }
        if (ptY > ptLastY) { int tmp = ptY; ptY = ptLastY; ptLastY = tmp; }
        if (ptX == ptLastX) ptLastX++;
        if (ptY == ptLastY) ptLastY++;
        rect = OdGsDCRect(ptX, ptLastX, ptLastY, ptY);
      }
      OdUInt16 selFlags = 0 /*OdTrRndLocalRenderer::kSelInserts*/;
      if (selOnce)
      {
        selFlags |= OdTrRndLocalRenderer::kSelOnce /*| OdTrRndLocalRenderer::kSelInvertHlt*/;
        if (renderer()->getViewportData(viewer()->activeViewportId()) && !renderer()->getViewportData(viewer()->activeViewportId())->viewportShading().renderMode().is_2d())
          selFlags |= OdTrRndLocalRenderer::kSelTop;
        else
          selFlags |= OdTrRndLocalRenderer::kSelLast;
      }
      OdUInt32 nKeyStates = getKeyStates(appHandle());
      if (!GETBIT(nKeyStates, kShiftKey)) selFlags |= OdTrRndLocalRenderer::kSelEnableHlt;
      if (GETBIT(nKeyStates, kCtrlKey))   selFlags |= OdTrRndLocalRenderer::kSelSubEnts;
      renderer()->select(viewer()->activeViewportId(), kTrVisMainOverlayId, rect, OdTrRndLocalRenderer::SelectionInfo(selType, selFlags), this);
      renderer()->transientManager()->detachTransient(viewer()->activeSelectionViewportId(), Tracker.get());
      redraw(appHandle());
      releaseCapture(appHandle());
      m_capState = kNoCapture;
    }
  }

  virtual OdTrRndLocalRenderer::SelectionFeedback selected(OdTrVisViewportId viewportId, OdTrVisOverlayId /*overlayId*/, const OdTrRndLocalRenderer::SelectionResult *pRes,
                                                           OdUInt32 nInserts, OdTrVisGsMarker selMarker, const OdTrRndLocalRenderer::SelectionInfo &selectionInfo,
                                                           const OdTrRndLocalRenderer &renderer) const
  {
    OdTrRndLocalRenderer &rnd = const_cast<OdTrRndLocalRenderer&>(renderer);
    if (!nInserts && !::isValidMarker(selMarker))
    { // Top level metafile selection
      if (pRes->hasGroup())
        rnd.highlightMetafilesGroup(pRes->m_grId, selectionInfo);
      else
        rnd.highlightMetafile(pRes->m_mfId, selectionInfo);
    }
    else
    { // Nested metafile selection
      // @@@TODO: this is experimental implementation. actually logic isn't precis and buggy here.
      OdUInt32 nPopulated = 0;
      OdUInt32 nInsert = nInserts + 1;
      if (selectionInfo.isSelectHighlightingInversionEnabled() || selectionInfo.isSelectHighlightingEnabled())
      {
        OdTrVisStateBranchId prevBranchId = kTrVisNegativeId, attachBranchId = kTrVisNegativeId;
        while (nInsert)
        { enum { kTop = 1, kLast = 2, kTopAndLast = 3 };
          const OdUInt8 bClass = ((nInsert == (nInserts + 1)) ? kTop : 0) | ((nInsert == 1) ? kLast : 0);
          OdTrVisStateBranchId curBranchId = kTrVisNegativeId;
          if (GETBIT(bClass, kTop))
            curBranchId = attachBranchId = (pRes[nInsert - 1].hasGroup()) ? renderer.getGroupHlBranch(pRes[nInsert - 1].m_grId)
                                                                          : renderer.getMetafileHlBranch(pRes[nInsert - 1].m_mfId);
          else if (prevBranchId != kTrVisNegativeId)
            curBranchId = renderer.getNestedStateBranch(prevBranchId, (pRes[nInsert - 1].hasGroup()) ? renderer.getGroupOwner(pRes[nInsert - 1].m_grId)
                                                                                                     : renderer.getMetafileOwner(pRes[nInsert - 1].m_mfId));
          if (curBranchId == kTrVisNegativeId)
          {
            nPopulated++;
            curBranchId = HlBranchIdGen.generateId();
            if (GETBIT(bClass, kTop))
              attachBranchId = curBranchId;
            if (GETBIT(bClass, kLast) || ((prevBranchId != kTrVisNegativeId) && !renderer.getStateBranchDef(curBranchId)))
            {
              OdTrVisStateBranchDef lastDef;
              if (GETBIT(bClass, kLast) && ::isValidMarker(selMarker))
                lastDef.m_markers.push_back(selMarker);
              rnd.rendition()->onStateBranchAdded(curBranchId, lastDef);
              rnd.rendition()->onStateBranchPropertyAttached(curBranchId,
                OdTrVisOwningPropertyDef((pRes[nInsert - 1].hasGroup()) ? renderer.getGroupOwner(pRes[nInsert - 1].m_grId) : renderer.getMetafileOwner(pRes[nInsert - 1].m_mfId)));
            }
            if (prevBranchId != kTrVisNegativeId)
            {
              OdTrVisStateBranchDef *pPrev = renderer.getStateBranchDef(prevBranchId);
              OdTrVisStateBranchDef hlDef;
              if (pPrev)
                hlDef = *pPrev;
              hlDef.m_childs.push_back(curBranchId);
              if (pPrev)
                rnd.rendition()->onStateBranchModified(prevBranchId, hlDef, true, false);
              else
              {
                rnd.rendition()->onStateBranchAdded(prevBranchId, hlDef);
                rnd.rendition()->onStateBranchPropertyAttached(prevBranchId,
                  OdTrVisOwningPropertyDef((pRes[nInsert].hasGroup()) ? renderer.getGroupOwner(pRes[nInsert].m_grId) : renderer.getMetafileOwner(pRes[nInsert].m_mfId)));
              }
            }
            if (GETBIT(bClass, kLast))
            {
              if (pRes[nInserts].hasGroup())
                rnd.rendition()->onGroupPropertyAttached(pRes[nInserts].m_grId, OdTrVisStateBranchPropertyDef(attachBranchId));
              else
                rnd.rendition()->onMetafilePropertyAttached(pRes[nInserts].m_mfId, OdTrVisStateBranchPropertyDef(attachBranchId));
            }
          }
          else if (GETBIT(bClass, kLast))
          {
            OdTrVisStateBranchDef *pDef = renderer.getStateBranchDef(curBranchId);
            if (::isValidMarker(selMarker))
            {
              if (!pDef->m_markers.contains(selMarker))
              {
                pDef->m_markers.push_back(selMarker);
                nPopulated++;
              }
            }
            else
            {
              pDef->m_markers.clear();
            }
          }
          prevBranchId = curBranchId;
          nInsert--;
        }
      }
      if (!selectionInfo.isSelectHighlightingEnabled() && (!selectionInfo.isSelectHighlightingInversionEnabled() || !nPopulated))
      {
        nInsert = nInserts + 1;
        OdTrVisStateBranchId nextBranchId = kTrVisNegativeId;
        while (nInsert)
        {
          const bool bTop = nInsert == (nInserts + 1);
          const bool bLast = nInsert == 1;
          OdTrVisStateBranchId curBranchId = kTrVisNegativeId;
          if (bTop)
            curBranchId = (pRes[nInsert - 1].hasGroup()) ? renderer.getGroupHlBranch(pRes[nInsert - 1].m_grId) : renderer.getMetafileHlBranch(pRes[nInsert - 1].m_mfId);
          else
            curBranchId = nextBranchId;
          if (curBranchId != kTrVisNegativeId)
          {
            if (!bLast)
              nextBranchId = renderer.getNestedStateBranch(curBranchId, (pRes[nInsert - 2].hasGroup()) ? renderer.getGroupOwner(pRes[nInsert - 2].m_grId)
                                                                                                    : renderer.getMetafileOwner(pRes[nInsert - 2].m_mfId));
            OdTrVisStateBranchDef &nDef = *renderer.getStateBranchDef(curBranchId);
            if (!bLast)
              nDef.m_childs.remove(nextBranchId);
            if (bLast && ::isValidMarker(selMarker))
              nDef.m_markers.remove(selMarker);
            if (nDef.m_childs.isEmpty() && nDef.m_markers.isEmpty())
            {
              rnd.rendition()->onStateBranchDeleted(curBranchId);
              HlBranchIdGen.killId(curBranchId);
              if (bTop)
              {
                if (pRes[nInsert - 1].hasGroup())
                  rnd.rendition()->onGroupPropertyDetached(pRes[nInsert - 1].m_grId, OdTrVisPropertyDef::kHlBranchProperty);
                else
                  rnd.rendition()->onMetafilePropertyDetached(pRes[nInsert - 1].m_mfId, OdTrVisPropertyDef::kHlBranchProperty);
              }
            }
          }
          nInsert--;
        }
      }
    }
    return OdTrRndLocalRenderer::kSelContinue;
  }

  virtual void onRMouseButtonDown(AppHandle /*appHandle*/, int /*x*/, int /*y*/, bool bDoubleClick)
  {
    if (!bDoubleClick)
    {
      if (m_capState == kNoCapture)
      {
        capture(appHandle());
        m_capState = kOrbit;
      }
    }
    else
    {
      viewer()->zoomExtents();
      redraw(appHandle());
    }
  }
  virtual void onRMouseButtonUp(AppHandle /*appHandle*/, int /*x*/, int /*y*/)
  {
    if (m_capState == kOrbit)
    {
      releaseCapture(appHandle());
      m_capState = kNoCapture;
    }
  }

  virtual void onMMouseButtonDown(AppHandle /*appHandle*/, int x, int y, bool bDoubleClick)
  {
    if (!bDoubleClick)
    {
      if (m_capState == kNoCapture)
      {
        capture(appHandle());
        m_capState = kPanning;
      }
    }
  }

  virtual void onMMouseButtonUp(AppHandle /*appHandle*/, int /*x*/, int /*y*/)
  {
    if (m_capState == kPanning)
    {
      releaseCapture(appHandle());
      m_capState = kNoCapture;
    }
  }

  virtual void onMouseMove(AppHandle /*appHandle*/, int dx, int dy)
  {
    switch (m_capState)
    {
      case kPanning:
        viewer()->pan(dx, dy);
      break;
      case kOrbit:
        {
          OdGsDCRect rArea; int lpX, lpY;
          getSize(appHandle(), rArea);
          getLastPoint(appHandle(), lpX, lpY);
          OdGePoint3d center(OdGePoint3d::kOrigin + OdGeVector3d::kXAxis * double((rArea.m_max.x - rArea.m_min.x) / 2) +
                                                    OdGeVector3d::kYAxis * double((rArea.m_min.y - rArea.m_max.y) / 2));
          OdGePoint3d point = OdGePoint3d(double(lpX), double(lpY), 0.0);
          double len = (point - center).length();
          double minLen = double(odmin(rArea.m_max.x - rArea.m_min.x, rArea.m_min.y - rArea.m_max.y)) * 0.5;
          if (len < minLen)
            viewer()->orbit(dx, dy);
          else
          {
            double angle = (point - center).angleTo((point + OdGeVector3d::kXAxis * double(dx) + OdGeVector3d::kYAxis * double(dy)) - center, OdGeVector3d::kZAxis);
            viewer()->orbit(dx, dy, OdTrRndLocalViewer::kOrbitEye, angle);
          }
        }
      break;
      case kSelection:
        {
          int lpX, lpY;
          getLastPoint(appHandle(), lpX, lpY);
          Tracker->updateTracker(OdGePoint3d(double(lpX + dx), double(lpY + dy), 0.0));
        }
      break;
      default: break;
    }
    if (m_capState != kNoCapture)
      redraw(appHandle());
  }

  virtual void onMouseWheel(AppHandle /*appHandle*/, float delta, int x, int y)
  {
    if (!isCaptured(appHandle()))
    {
      viewer()->zoom(x, y, (delta <= 0.0) ? (1.0f / 0.9f) : 0.9f);
      redraw(appHandle());
    }
  }

  virtual void onKey(AppHandle /*appHandle*/, OdUInt32 charCode, bool down, int cRepeat, OdUInt32 /*flags*/)
  {
    if (down && (cRepeat < 2))
    {
      switch (charCode)
      {
        case '1': case '2': case '3': case '4': case '5': case '6': case '7':
          viewer()->setRenderMode(viewer()->activeViewportId(), (OdGsView::RenderMode)(charCode - '1' + OdGsView::k2DOptimized));
          ExtBar::ListElement::cast(getExtBar(appHandle()).getElement(ExCmdElem_RenderMode))->setSelString(viewer()->renderMode(viewer()->activeViewportId()));
          redraw(appHandle());
        break;
        case ' ':
          // Deselect all
          renderer()->selectAll(viewer()->activeViewportId(), kTrVisMainOverlayId, this);
          HlBranchIdGen.clean();
          redraw(appHandle());
        break;
      }
    }
  }

  void updateSectionIface(AppHandle appHandle)
  {
    if (viewer()->sectionPlanesNumber(viewer()->activeViewportId()))
    {
      getExtBar(appHandle).getElement(ExCmdElem_RemovePlane)->setDisabled(false);
      getExtBar(appHandle).getElement(ExCmdElem_RotateL)->setDisabled(false);
      getExtBar(appHandle).getElement(ExCmdElem_RotateT)->setDisabled(false);
      getExtBar(appHandle).getElement(ExCmdElem_RotateR)->setDisabled(false);
      getExtBar(appHandle).getElement(ExCmdElem_RotateB)->setDisabled(false);
      if (Tracker.isNull())
      {
        Tracker = OdGLES2LocalSecPlaneTracker::create();
        renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), Tracker.get());
      }
      else
        Tracker->updateTracker(OdGePoint3d::kOrigin);
    }
    else
    {
      getExtBar(appHandle).getElement(ExCmdElem_RemovePlane)->setDisabled(true);
      getExtBar(appHandle).getElement(ExCmdElem_RotateL)->setDisabled(true);
      getExtBar(appHandle).getElement(ExCmdElem_RotateT)->setDisabled(true);
      getExtBar(appHandle).getElement(ExCmdElem_RotateR)->setDisabled(true);
      getExtBar(appHandle).getElement(ExCmdElem_RotateB)->setDisabled(true);
      if (!Tracker.isNull())
      {
        renderer()->transientManager()->detachTransient(viewer()->activeViewportId(), Tracker.get());
        Tracker.release();
      }
    }
  }

  OdTrVisLayerId layerIdByName(const OdString &name)
  {
    OdTrRndLocalViewer::LayersList layers;
    viewer()->getLayersList(layers);
    OdUInt32 nLayers = layers.size();
    const OdTrVisLayerId *pLayers = layers.getPtr();
    for (OdUInt32 nLayer = 0; nLayer < nLayers; nLayer++)
    {
      if (viewer()->getLayerName(pLayers[nLayer]) == name)
        return pLayers[nLayer];
    }
    return kTrVisNegativeId;
  }

  virtual void onExtBar(AppHandle appHandle, ExtBar::Element *pElem)
  {
    switch (pElem->elementId())
    {
      case ExCmdElem_Background:
      {
        ODCOLORREF cr = viewer()->backgroundColor();
        if (getExtBar(appHandle).getColor(cr))
        {
          viewer()->setBackgroundColor(cr);
          redraw(appHandle);
        }
      } break;
      case ExCmdElem_Extents:
      {
        viewer()->recomputeSceneDepth(viewer()->activeViewportId());
        redraw(appHandle);
      } break;
      case ExCmdElem_RenderMode:
      {
        ExtBar::ListElement *pList = ExtBar::ListElement::cast(pElem);
        viewer()->setRenderMode(viewer()->activeViewportId(), (OdGsView::RenderMode)(pList->getSelString()));
        redraw(appHandle);
      } break;
      case ExCmdElem_LayerSel:
      {
        ExtBar::ListElement *pList = ExtBar::ListElement::cast(pElem);
        ExtBar::CheckElement *pCheck = ExtBar::CheckElement::cast(getExtBar(appHandle).getElement(ExCmdElem_LayerChange));
        int nCurSel = pList->getSelString();
        if (nCurSel == -1)
          pCheck->setDisabled(true);
        else
        {
          pCheck->setDisabled(false);
          OdTrVisLayerId layerId = layerIdByName(pList->getString(nCurSel));
          pCheck->setChecked(viewer()->getLayerDef(layerId)->m_props.isLayerVisible());
        }
        redraw(appHandle);
      } break;
      case ExCmdElem_LayerChange:
      {
        ExtBar::ListElement *pList = ExtBar::ListElement::cast(getExtBar(appHandle).getElement(ExCmdElem_LayerSel));
        ExtBar::CheckElement *pCheck = ExtBar::CheckElement::cast(pElem);
        int nCurSel = pList->getSelString();
        OdTrVisLayerId layerId = layerIdByName(pList->getString(nCurSel));
        if (pCheck->isChecked())
          viewer()->enableLayer(layerId);
        else
          viewer()->disableLayer(layerId);
        redraw(appHandle);
      } break;
      case ExCmdElem_AddPlane:
      {
        viewer()->addSectionPlane(viewer()->activeViewportId(), OdTrVisSectionPlane());
        updateSectionIface(appHandle);
        redraw(appHandle);
      } break;
      case ExCmdElem_RemovePlane:
      {
        viewer()->removeSectionPlane(viewer()->activeViewportId());
        updateSectionIface(appHandle);
        redraw(appHandle);
      } break;
      case ExCmdElem_RotateT:
      {
        viewer()->transformSectionPlanes(viewer()->activeViewportId(), OdGeMatrix3d::rotation(OdaPI / 8, OdGeVector3d::kXAxis));
        updateSectionIface(appHandle);
        redraw(appHandle);
      } break;
      case ExCmdElem_RotateB:
      {
        viewer()->transformSectionPlanes(viewer()->activeViewportId(), OdGeMatrix3d::rotation(-OdaPI / 8, OdGeVector3d::kXAxis));
        updateSectionIface(appHandle);
        redraw(appHandle);
      } break;
      case ExCmdElem_RotateL:
      {
        viewer()->transformSectionPlanes(viewer()->activeViewportId(), OdGeMatrix3d::rotation(OdaPI / 8, OdGeVector3d::kYAxis));
        updateSectionIface(appHandle);
        redraw(appHandle);
      } break;
      case ExCmdElem_RotateR:
      {
        viewer()->transformSectionPlanes(viewer()->activeViewportId(), OdGeMatrix3d::rotation(-OdaPI / 8, OdGeVector3d::kYAxis));
        updateSectionIface(appHandle);
        redraw(appHandle);
      } break;
    }
  }
} _XmlViewAppClient;

//
