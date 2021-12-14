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

// Hoops ViewerView.cpp : implementation of the CHoopsViewerView class
//

#include "stdafx.h"
#include "HoopsViewer.h"

#include "HoopsViewerDoc.h"
#include "HoopsViewerView.h"
#include "MainFrm.h"
#include "SetLayoutDlg.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MOUSE_WHEEL_STEP 1.2

extern CHoopsViewerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHoopsViewerView

IMPLEMENT_DYNCREATE(CHoopsViewerView, CView)

BEGIN_MESSAGE_MAP(CHoopsViewerView, CView)
  //{{AFX_MSG_MAP(CHoopsViewerView)
  ON_WM_PAINT()
  ON_WM_SIZE()
  ON_COMMAND(ID_XY_PLANE, OnXYPlane)
  ON_COMMAND(ID_XZ_PLANE, OnXZPlane)
  ON_COMMAND(ID_YZ_PLANE, OnYZPlane)
  ON_COMMAND(ID_BOTTOM, OnBottomView)
  ON_COMMAND(ID_RIGHT, OnRightView)
  ON_COMMAND(ID_BACK, OnBackView)
  ON_COMMAND(ID_NE_ISOMETRIC, OnNEIsometric)
  ON_COMMAND(ID_NW_ISOMETRIC, OnNWIsometric)
  ON_COMMAND(ID_SE_ISOMETRIC, OnSEIsometric)
  ON_COMMAND(ID_SW_ISOMETRIC, OnSWIsometric)
  ON_COMMAND(ID_ORBIT, OnOrbit)
  ON_COMMAND(ID_ZOOM, OnZoom)
  ON_COMMAND(ID_PAN, OnPan)
  ON_COMMAND(ID_ZOOM_TO_EXTENTS, OnZoomToExtents)
  ON_COMMAND(ID_ZOOM_TO_WINDOW, OnZoomWindow)
  ON_COMMAND(ID_AXIS_ONOFF, OnAxisOnOff )
  ON_UPDATE_COMMAND_UI(ID_AXIS_ONOFF, OnUpdateAxisOnOff)
  ON_UPDATE_COMMAND_UI(ID_ORBIT, OnUpdateOrbit)
  ON_UPDATE_COMMAND_UI(ID_ZOOM, OnUpdateZoom)
  ON_UPDATE_COMMAND_UI(ID_PAN, OnUpdatePan)
  ON_COMMAND(ID_CHANGE_BG_COLOR, ChangeBgColor)
  ON_UPDATE_COMMAND_UI(ID_ZOOM_TO_EXTENTS, OnUpdateZoomToExtents)
  ON_UPDATE_COMMAND_UI(ID_ZOOM_TO_WINDOW, OnUpdateZoomWindow)
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_WM_MOUSEWHEEL()
  ON_COMMAND(ID_VIEW_WIREFRAME, OnViewWireframe)
  ON_UPDATE_COMMAND_UI(ID_VIEW_WIREFRAME, OnUpdateViewWireframe)
  ON_COMMAND(ID_VIEW_SHADED, OnViewShaded)
  ON_UPDATE_COMMAND_UI(ID_VIEW_SHADED, OnUpdateViewShaded)
  ON_COMMAND(ID_VIEW_HIDDENLINE, OnViewHiddenline)
  ON_UPDATE_COMMAND_UI(ID_VIEW_HIDDENLINE, OnUpdateViewHiddenline)
  ON_COMMAND(ID_TOOLS_SETLAYOUT, OnSetActiveLayout)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_SETLAYOUT, OnUpdateSetActiveLayout)
  ON_COMMAND(ID_SHOW_LINES, OnShowLines)
  ON_UPDATE_COMMAND_UI(ID_SHOW_LINES, OnUpdateShowLines)
  ON_COMMAND(ID_SHOW_TEXT, OnShowText)
  ON_UPDATE_COMMAND_UI(ID_SHOW_TEXT, OnUpdateShowText)
  //}}AFX_MSG_MAP
  // Standard printing commands
  ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHoopsViewerView construction/destruction

CHoopsViewerView::CHoopsViewerView()
{
  m_bFastPrint = true;
  m_bOperatorStarted = false;
  m_bClipboardPrinting = false;
  m_MetafileType = EMF;
  m_bClipboardTruecolor = true;
  m_lDriverKey = 0L;
  m_lSceneKey = 0L;
  m_pViewPalette = NULL;
  m_iViewMode = kOrbit;
  m_bZoomWindowOn = false;
  m_lRectangleKey = 0;
  m_lMarkerKey = 0;
  m_bResetColor = false;
  m_bResetVisibility = false;
  m_AxisWindowSize = 125;
  m_sDriverSegment[0] = 0;
}

CHoopsViewerView::~CHoopsViewerView()
{
  // delete the color palette, the driver instance and decrement the driver instance counter
  if( m_sDriverSegment[0] )
    HC_Delete_Segment(m_sDriverSegment);

  theApp.DecrementCounter();

  if (m_pViewPalette)
    delete m_pViewPalette;
}

BOOL CHoopsViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.lpszClass = AfxRegisterWndClass(CS_OWNDC|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW);
  return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CHoopsViewerView drawing

void CHoopsViewerView::OnDraw(CDC* /*pDC*/)
{
  CHoopsViewerDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CHoopsViewerView printing

BOOL CHoopsViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CHoopsViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add extra initialization before printing
}

void CHoopsViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CHoopsViewerView diagnostics

#ifdef _DEBUG
void CHoopsViewerView::AssertValid() const
{
  CView::AssertValid();
}

void CHoopsViewerView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CHoopsViewerDoc* CHoopsViewerView::GetDocument() const // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHoopsViewerDoc)));
  return (CHoopsViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHoopsViewerView message handlers

void CHoopsViewerView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();

  CHoopsViewerDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);

  // Setup the view to be associated with a HOOPS driver segment.
  CMainFrame * pAppFrame = (CMainFrame *)AfxGetMainWnd();

  if (!pAppFrame->SharedPalette())
    InitializePalette ();

  CPalette * pPalette = GetPalette();

  int count = theApp.IncrementCounter();

  sprintf (m_sDriverSegment, "/driver/%s/window%d", theApp.GetDriverType(), count);

  m_lDriverKey = HC_KOpen_Segment (m_sDriverSegment);

  char        szTemp[256];
  long        debug_flags = DEBUG_NO_WINDOWS_HOOK;

  sprintf (szTemp, "disable input, use window id = %u, debug = %u", m_hWnd, debug_flags);

  HC_Set_Driver_Options (szTemp);

  HC_Set_Handedness("right");

  sprintf (szTemp, "use colormap id = %u", (long)(HPALETTE)(*pPalette));
  HC_Set_Driver_Options (szTemp);


  if( !strcmp (((CHoopsViewerApp *)AfxGetApp())->GetDriverType(), "opengl")) 
  {
    HC_Set_Rendering_Options ("hsr algorithm = hardware z-buffer,  technology = standard");
    HC_Set_Driver_Options("double-buffering");
  }
  else 
  {
    HC_Set_Rendering_Options ("hsra = software z buffer, technology = sfb");
    HC_Set_Driver_Options("fixed colors = 125");
  }

  HC_Set_Driver_Options("anti-alias = 4");

  COLORREF clrBgColor = ((CHoopsViewerApp*)AfxGetApp())->m_clrBgColor;
  CString strColor;
  strColor.Format(L"windows=(R = %f G = %f B = %f)", GetRValue(clrBgColor) / 255.0,
    GetGValue(clrBgColor) / 255.0, GetBValue(clrBgColor) / 255.0 );
  char* pchData = new char[strColor.GetLength() + 1] ;
  wcstombs( pchData, strColor.GetBuffer(strColor.GetLength()), strColor.GetLength() );
  strColor.ReleaseBuffer();
  pchData[strColor.GetLength()] = '\0';
  HC_Set_Color(pchData);
  delete[] pchData;
  HC_Set_Window_Frame ("off");

  m_lSceneKey = HC_KOpen_Segment ("Scene");
  m_AxisTriadKey = HC_KCreate_Segment("axis");

  HC_Include_Segment(pDoc->GetModelSegment());

  HC_KEY lModelKey = HC_KOpen_Segment( pDoc->GetModelSegment() );
  HPoint cameraPosition;
  HPoint cameraTarget;
  HPoint cameraUp;
  float  cameraWidth, cameraHeight;
  char   cameraProj[512];

  HC_UnSet_Visibility();

  if( (!pDoc->m_bMultyLayouts && pDoc->m_bLoadDWG) || pDoc->m_bLoadDGN )
  {
    if( HC_Show_Existence("camera") )
    {
      HC_Show_Camera( &cameraPosition, &cameraTarget, &cameraUp, &cameraWidth, &cameraHeight, cameraProj);
      HC_UnSet_Camera();
    }
    else
    {
      cameraPosition.x = 0;
      cameraPosition.y = 0;
      cameraPosition.z = 1;
      cameraTarget.x   = 0;
      cameraTarget.y   = 0;
      cameraTarget.z   = 0;
      cameraUp.x       = 0;
      cameraUp.y       = 1;
      cameraUp.z       = 0;
    }

    CDWGLayoutInfo tmp;
    tmp.lSegmentKey = lModelKey;
    tmp.strSegmentName = pDoc->GetModelSegment();
    tmp.ptCameraPosition = cameraPosition;
    tmp.ptCameraTarget   = cameraTarget;
    tmp.vrCameraUp       = cameraUp;

    pDoc->m_layoutInfoArr.push_back(tmp);
    pDoc->m_iCurLayoutRecord = 0;
  }
  else if( pDoc->m_bLoadDWG )
  {
    char pchSegmentName[4096];

    HC_Begin_Segment_Search("*");

    while( HC_Find_Segment(pchSegmentName) )
    {
      HC_KEY lLayoutKey = HC_KOpen_Segment(pchSegmentName);

      if( HC_Show_Existence("camera") )
      {
        HC_Show_Camera( &cameraPosition, &cameraTarget, &cameraUp, &cameraWidth, &cameraHeight, cameraProj);
        HC_UnSet_Camera();
      }
      else
      {
        cameraPosition.x = 0;
        cameraPosition.y = 0;
        cameraPosition.z = 1;
        cameraTarget.x   = 0;
        cameraTarget.y   = 0;
        cameraTarget.z   = 0;
        cameraUp.x       = 0;
        cameraUp.y       = 1;
        cameraUp.z       = 0;
      }

      CDWGLayoutInfo tmp;
      tmp.lSegmentKey      = lLayoutKey;
      tmp.strSegmentName   = pchSegmentName;
      tmp.ptCameraPosition = cameraPosition;
      tmp.ptCameraTarget   = cameraTarget;
      tmp.vrCameraUp       = cameraUp;

      pDoc->m_layoutInfoArr.push_back(tmp);

      CString strSegmentName = tmp.strSegmentName;
      strSegmentName.MakeUpper();
      strSegmentName.Replace('\\','/');

      if( strSegmentName.ReverseFind('/') != -1 )
          strSegmentName = strSegmentName.Right( strSegmentName.GetLength() - strSegmentName.ReverseFind('/') - 1);

      HC_UnSet_Visibility();

      if( strSegmentName == L"MODEL" )
      {
        pDoc->m_iCurLayoutRecord = pDoc->m_layoutInfoArr.size() - 1;
        HC_Set_Heuristics("exclude bounding = off");
      }
      else
      {
        HC_Set_Visibility("geometry = off");
        HC_Set_Heuristics("exclude bounding = on");
      }

      HC_Close_Segment();
    }

    HC_End_Segment_Search();

    if( pDoc->m_iCurLayoutRecord < 0 )
      pDoc->m_iCurLayoutRecord = 0;
  }
  else
  {
    if( pDoc->m_layoutInfoArr.isEmpty() )
    {
      CDWGLayoutInfo tmp;
      tmp.lSegmentKey      = lModelKey;
      tmp.strSegmentName   = pDoc->GetModelSegment();
      tmp.ptCameraPosition.x = 0;
      tmp.ptCameraPosition.y = 0;
      tmp.ptCameraPosition.z = 1;
      tmp.ptCameraTarget.x   = 0;
      tmp.ptCameraTarget.y   = 0;
      tmp.ptCameraTarget.z   = 0;
      tmp.vrCameraUp.x       = 0;
      tmp.vrCameraUp.y       = 1;
      tmp.vrCameraUp.z       = 0;

      pDoc->m_layoutInfoArr.push_back(tmp);
      pDoc->m_iCurLayoutRecord = 0;
    }
  }

  HC_Close_Segment();

  cameraPosition = pDoc->m_layoutInfoArr[pDoc->m_iCurLayoutRecord].ptCameraPosition;
  cameraTarget   = pDoc->m_layoutInfoArr[pDoc->m_iCurLayoutRecord].ptCameraTarget;
  cameraUp       = pDoc->m_layoutInfoArr[pDoc->m_iCurLayoutRecord].vrCameraUp;

  HC_Set_Camera_Position( cameraPosition.x, cameraPosition.y, cameraPosition.z);
  HC_Set_Camera_Target( cameraTarget.x, cameraTarget.y, cameraTarget.z );
  HC_Set_Camera_Up_Vector( cameraUp.x, cameraUp.y, cameraUp.z );

  HC_Set_Visibility("vertices=off, edges = off, lines = on, text = on, markers=off");
  //HC_Set_Rendering_Options ("shadow map = on, simple shadow = on");
  FitWorld();

  // Insert a light
  HPoint	position, target, up;
  float	width, height;
  char    projection[16];
  HC_Show_Net_Camera (&position, &target, &up, &width, &height, projection);
  HPoint vector1, vector2, new_light;
  vector1.x = position.x - target.x;
  vector1.y = position.y - target.y;
  vector1.z = position.z - target.z;
  HC_Compute_Normalized_Vector(&vector1, &vector1);
  HC_Compute_Normalized_Vector(&up, &up);
  HC_Compute_Cross_Product(&vector1, &up, &vector2);

  // offset the light from the camera vector so that flats objects facing the screen
  // do not appear washed out
  new_light.x = 1.5f*vector1.x+vector2.x;
  new_light.y = 1.5f*vector1.y+vector2.y;
  new_light.z = 1.5f*vector1.z+vector2.z;
  m_lLightKey = HC_KInsert_Distant_Light (new_light.x, new_light.y, new_light.z);

  SetAxis();

  HC_Close_Segment ();

  HC_Close_Segment ();
  HC_Update_Display();
}


void CHoopsViewerView::FitWorld() 
{
  float min[3];
  float max[3];

  HC_Compute_Circumcuboid (".", &min, &max);

  //order the points
  if (min[0] > max[0]) {
    float tmp;
    tmp = min[0]; min[0] = max[0]; max[0] = tmp;
  }
  if (min[1] > max[1]) {
    float tmp;
    tmp = min[1]; min[1] = max[1]; max[1] = tmp;
  }
  if (min[2] > max[2]) {
    float tmp;
    tmp = min[2]; min[2] = max[2]; max[2] = tmp;
  }

  // compute the length of the bbox diagonal vector
  // this will be used to make the camera field a bit larger
  // to ensure that objects between the camera position and target
  // will still not get clipped (this is for perspective viewing mode)

  float diagonalVector[3];
  float center[3];
  float diagonalLength;

  diagonalVector[0] = max[0] - min[0];
  diagonalVector[1] = max[1] - min[1];
  diagonalVector[2] = max[2] - min[2];

  diagonalLength = (float) sqrt(
    diagonalVector[0] * diagonalVector[0] +
    diagonalVector[1] * diagonalVector[1] +
    diagonalVector[2] * diagonalVector[2]);

  center[0] = 0.5F * (min[0] + max[0]);
  center[1] = 0.5F * (min[1] + max[1]);
  center[2] = 0.5F * (min[2] + max[2]);

  float position[3], target[3], up[3];
  float fieldx, fieldy;
  char proj[32];

  HC_Show_Net_Camera(&position, &target, &up, &fieldx, &fieldy, proj);

  // now we determine the new camera definition
  float new_position[3], new_target[3], new_up[3];
  float new_fieldx, new_fieldy;

  if( diagonalLength != 0 ) {

    new_target[0] = center[0];
    new_target[1] = center[1];
    new_target[2] = center[2];

    float viewingVector[3];
    viewingVector[0] = position[0] - target[0];
    viewingVector[1] = position[1] - target[1];
    viewingVector[2] = position[2] - target[2];

    HC_Compute_Normalized_Vector (&viewingVector, &viewingVector);

    // reset camera position to lie on the viewing vector, at a distance of
    // 2.5 * diagonal length away from the center of the scene centroid
    new_position[0] = center[0] + 2.5f * viewingVector[0] * diagonalLength;
    new_position[1] = center[1] + 2.5f * viewingVector[1] * diagonalLength;
    new_position[2] = center[2] + 2.5f * viewingVector[2] * diagonalLength;

    // set the camera field to be the diagonal length of the scene's bounding box
    new_fieldx = diagonalLength;
    new_fieldy = diagonalLength;

    new_up[0] = up[0];
    new_up[1] = up[1];
    new_up[2] = up[2];
  }
  else
  {
    // in this case only change the target
    new_target[0] = center[0];
    new_target[1] = center[1];
    new_target[2] = center[2];

    new_position[0] = position[0];
    new_position[1] = position[1];
    new_position[2] = position[2];

    new_up[0] = up[0];
    new_up[1] = up[1];
    new_up[2] = up[2];

    new_fieldx = fieldx;
    new_fieldy = fieldy;
  }


  HC_Set_Camera(&new_position, &new_target, &new_up, new_fieldx, new_fieldy, proj);


  return;
}

void CHoopsViewerView::OnPaint() 
{
  CPaintDC dc(this); // device context for painting

  HC_Control_Update (m_sDriverSegment, "redraw everything");
  HC_Update_Display ();
}


// initialize this view's unique palette (meaning that we aren't sharing a palette among
// all views, which should be quite rare)  
void CHoopsViewerView::InitializePalette()
{
  if (m_pViewPalette != NULL) {
    delete m_pViewPalette;
    m_pViewPalette = NULL;
  }

  // Setup the Palette

  CHoopsViewerDoc * pDoc = GetDocument();
  ASSERT_VALID(pDoc);

  CMainFrame * pAppFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
  ASSERT(pAppFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));

  CClientDC docDC(pAppFrame);

  int rc = docDC.GetDeviceCaps(RASTERCAPS);

  if (rc & RC_PALETTE) {

    ASSERT (GetSystemPaletteUse (docDC.GetSafeHdc()) == SYSPAL_STATIC );

    m_pViewPalette = new CPalette;

    LPLOGPALETTE 		Palette;
    int		            nStaticColors;
    DWORD	            *dword_ptr;
    int		            nColors;
    int                 i;

    // Get the static colors from the system palette 
    nStaticColors = docDC.GetDeviceCaps(NUMCOLORS);
    nColors = docDC.GetDeviceCaps(SIZEPALETTE);

    Palette = (LPLOGPALETTE) malloc (sizeof(LOGPALETTE) + sizeof (PALETTEENTRY) * nColors);
    Palette->palVersion = 0x300;
    Palette->palNumEntries = (WORD)nColors;


    nColors = GetSystemPaletteEntries (docDC.GetSafeHdc(), 0, nColors, Palette->palPalEntry);

    for (i=0; i<nColors; i++) {
      Palette->palPalEntry[i].peFlags = PC_NOCOLLAPSE;
    }


    // Set the peFlags of the lower static colors to reserved and the
    // logical color array to the system colors

    nStaticColors = nStaticColors / 2;
    for (i = 0; i < nStaticColors; i++) {
      dword_ptr = (DWORD *)&Palette->palPalEntry[i];
      *dword_ptr = i;
      Palette->palPalEntry[i].peFlags = PC_EXPLICIT;
    }

    // Set the peFlags of the upper static colors to reserved 
    for (i = nColors - nStaticColors; i < nColors; i++) {
      dword_ptr = (DWORD *)&Palette->palPalEntry[i];
      *dword_ptr = i;
      Palette->palPalEntry[i].peFlags = PC_EXPLICIT;
    }
    m_pViewPalette->CreatePalette(Palette);
    free (Palette);

    m_pViewPalette->UnrealizeObject();
    docDC.SelectPalette (m_pViewPalette, FALSE);
    docDC.RealizePalette ();
  }
}

CPalette* CHoopsViewerView::GetPalette()
{
  CMainFrame* pAppFrame = (CMainFrame *)AfxGetMainWnd();

  if (pAppFrame->SharedPalette())
    return (pAppFrame->GetSharedPalette());
  else
    return m_pViewPalette;
}

void CHoopsViewerView::OnEditCopy() 
{
  BeginWaitCursor();

  // Clean clipboard of contents and copy.
  if (OpenClipboard()) {
    EmptyClipboard();
    CloseClipboard();
  }

  CDC * hDC = GetDC();

  CMetaFileDC     hMfDC;

  hMfDC.CreateEnhanced(hDC, NULL, NULL, NULL);
  ReleaseDC (hDC);

  char driver[256];
  sprintf(driver, "/driver/msw/clipboard");

  HC_Open_Segment (driver);
  // first let's do the generic print driver set up
  char szTemp[256];

  sprintf (szTemp, "use window ID = %u", m_hWnd);
  HC_Set_Driver_Options (szTemp);

  sprintf (szTemp, "use window ID2 = %u", hMfDC.GetSafeHdc());
  HC_Set_Driver_Options (szTemp);

  long debug_flags = 0;
  debug_flags |= DEBUG_CLIPBOARD_MODE;
  debug_flags |= DEBUG_FORCE_FULL_COLOR;

  if (GetRenderMode() == SHADED)
    debug_flags |= DEBUG_SFB_COPY_TO_CLIPBOARD;

  sprintf (szTemp, "debug = %u", debug_flags);
  HC_Set_Driver_Options (szTemp);

  sprintf (szTemp, "special events, disable input=all, subscreen stretching, subscreen = (-1.0, 1.0, -1.0, 1.0)");
  HC_Set_Driver_Options ( szTemp);

  HPoint	window_color;
  window_color.x = 1.0;
  window_color.y = 1.0;
  window_color.z = 1.0;

  HC_Set_Color_By_Value("windows", "RGB", window_color.x, window_color.y, window_color.z);

  // now depending on the rendermode we take three different approaches to printing
  int renderMode = GetRenderMode();
  if (renderMode == SHADED)
  {
    HC_Set_Rendering_Options("hsra = szb, technology = sfb");
  }
  else if (renderMode == HIDDENLINE) //non rasterOutput path
  {
    char string[1024], hlr[2048];
    HC_Show_One_Net_Rendering_Optio("hidden line removal options", string);
    sprintf(hlr, "hsra=hidden line, hlro=(%s)", string);
    HC_Set_Rendering_Options("hsra=hidden line, hlro=(visibility=off)");
  }
  else if (renderMode == WIREFRAME) 
  {
    HC_Set_Heuristics("no hidden surfaces");
  }

  HC_Include_Segment_By_Key(m_lSceneKey);

  HC_Close_Segment();

  HC_Control_Update (driver, "redraw everything");
  HC_Update_Display ();
  HC_Delete_Segment (driver);


  HENHMETAFILE    hEmf;

  hEmf = hMfDC.CloseEnhanced ();

  if (hEmf == NULL) 
  {
    MessageBox (_T("Error creating enhanced metafile for clipboard"), _T("Copy to Clipboard Error"), MB_OK);
    EndWaitCursor();
    return;	
  }

  if (OpenClipboard()) 
  {
    SetClipboardData (CF_ENHMETAFILE, hEmf);
    CloseClipboard();
  }

  DeleteEnhMetaFile(hEmf);
  EndWaitCursor();

  return;
}
void CHoopsViewerView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/) 
{
  char driver[256];
  sprintf(driver, "/driver/msw/printer");

  HC_Open_Segment (driver);
  // first let's do the generic print driver set up
  char szTemp[256];

  sprintf (szTemp, "use window ID = %u", ::GetDesktopWindow());
  HC_Set_Driver_Options (szTemp);

  HC_Set_Window_Frame ("off");
  sprintf (szTemp, "use window ID2 = %u", pDC->m_hAttribDC);
  HC_Set_Driver_Options (szTemp);

  sprintf (szTemp, "output format = %u", pDC->m_hDC);
  HC_Set_Driver_Options (szTemp);

  long debug_flags = 0;
  debug_flags |= DEBUG_PRINTING_MODE;
  debug_flags |= DEBUG_PRINT_NOT_FRAMEBUFFER_MODE;
  debug_flags |= DEBUG_PRINT_NOT_RASTER_REDUCTION;

  sprintf (szTemp, "debug = %u", debug_flags);
  HC_Set_Driver_Options (szTemp);

  float SubscreenXmin, SubscreenXmax, SubscreenYmin, SubscreenYmax;
  SubscreenXmin = -1.0f;
  SubscreenXmax = 1.0f;
  SubscreenYmin =  -1.0f;
  SubscreenYmax = 1.0f;
  sprintf (szTemp, "special events, disable input=all, subscreen stretching, subscreen = (%f, %f, %f, %f)",  SubscreenXmin, SubscreenXmax, SubscreenYmin, SubscreenYmax);
  HC_Set_Driver_Options ( szTemp);

  int renderMode = GetRenderMode();

  // now depending on the rendermode we take three different approaches to printing
  if (renderMode == SHADED)
  {
    char					buffer[1024];
    HPoint					window_color;
    HC_KEY					image_key;

    //let's determine the size of the page we are printing to so that we can create an
    //approriately sized image
    int w=pDC->GetDeviceCaps(HORZRES);
    int h=pDC->GetDeviceCaps(VERTRES);
    //float aspect_ratio = (float)h/(float)w;

    float resolution_w = (float)pDC->GetDeviceCaps(LOGPIXELSX);
    float page_width = (float)w/resolution_w;
    float resolution_h = (float)pDC->GetDeviceCaps(LOGPIXELSY);
    float page_height = (float)h/resolution_h;

    //let's our stretch blit image 100 dpi
    int image_dpi=100;
    int iwidth = (int) (page_width*image_dpi*0.5*(SubscreenXmax-SubscreenXmin)) ;
    int iheight =(int)(page_height*image_dpi*0.5*(SubscreenYmax-SubscreenYmin));

    //prepare image for rendering
    HC_Open_Segment ("?driver/null/1");
    image_key = HC_KInsert_Image(0.0, 0.0, 0.0, "rgb, name=background", iwidth, iheight, NULL);
    HC_Close_Segment ();

    window_color.x = 1.0;
    window_color.y = 1.0;
    window_color.z = 1.0;

    // prepare data to render
    HC_Open_Segment ("?driver/image/1");
    HC_Set_Color_By_Value("windows", "RGB", window_color.x, window_color.y, window_color.z);
    sprintf (buffer, "use window id = %ld", image_key);
    HC_Set_Driver_Options (buffer);
    HC_Set_Rendering_Options("hsra = szb");
    HC_Set_Window_Frame("off");
    HC_Include_Segment_By_Key (m_lSceneKey);
    HC_Set_Visibility("lines=off, text=off");
    HC_Set_Rendering_Options("attribute lock= (visibility=(lines, text))");
    HC_Close_Segment ();

    //create the image
    HC_Update_Display ();

    // put this image in the background
    HC_Set_Color("windows=background");
    HC_Set_Rendering_Options("hsra=hidden line, hlro=(visibility=off)");
  }
  else if (renderMode == HIDDENLINE) //non rasterOutput path
  {
    char string[1024], hlr[2048];
    HC_Show_One_Net_Rendering_Optio("hidden line removal options", string);
    sprintf(hlr, "hsra=hidden line, hlro=(%s)", string);
    HC_Set_Rendering_Options("hsra=hidden line, hlro=(visibility=off)");
  }
  else if (renderMode == WIREFRAME) 
  {
    HC_Set_Heuristics("no hidden surfaces");
  }

  HC_Include_Segment_By_Key(m_lSceneKey);


  HC_Close_Segment();

  HC_Control_Update (driver, "redraw everything");
  HC_Update_Display ();
  HC_Delete_Segment (driver);

  if (renderMode == SHADED)
  {
    HC_UnDefine_Texture("background");
    HC_Delete_Segment ("?driver/null/1");
    HC_Delete_Segment ("?driver/image/1");
  }
}

void CHoopsViewerView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);

  // TODO: Add your message handler code here

}


BOOL CHoopsViewerView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
  dwStyle = (dwStyle | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED);
  return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CHoopsViewerView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  CView::OnLButtonDown(nFlags, point);

  m_bOperatorStarted=true;

  m_ptFirst.x = (float)point.x;
  m_ptFirst.y = (float)point.y;
  m_ptFirst.z = 0;

  m_ptRealPoint.x = (float) point.x;
  m_ptRealPoint.y = (float) point.y;
  m_ptRealPoint.z = 0.0;

  HC_Open_Segment_By_Key(m_lSceneKey);
  HC_Compute_Coordinates(".", "local pixels", &m_ptRealPoint, "world", &m_ptWorldFirst);
  HC_Close_Segment();

  HC_Open_Segment(m_sDriverSegment);
  HC_Compute_Coordinates(".", "local pixels", &m_ptRealPoint, "local window", &m_ptRealPoint);
  HC_Close_Segment();

  if( m_iViewMode == kZoom )
  {
    HPoint target;
    HC_Open_Segment_By_Key(m_lSceneKey);
    HC_Show_Net_Camera_Target(&m_ptCamera.x,&m_ptCamera.y,&m_ptCamera.z);
    HC_Show_Net_Camera_Position(&target.x,&target.y,&target.z);	
    m_ptDist.x = target.x-m_ptCamera.x;
    m_ptDist.y = target.y - m_ptCamera.y;
    m_ptDist.z = target.z - m_ptCamera.z;
    m_fLength = (float)HC_Compute_Vector_Length(&m_ptDist);
    HC_Compute_Normalized_Vector(&m_ptDist,&m_ptDist);
    HC_Show_Net_Camera_Field(&m_Width, &m_Height);
    HC_Close_Segment();
  }
  else if( m_bZoomWindowOn )
  {
    HC_Open_Segment_By_Key(m_lSceneKey);

    if( HC_Show_Existence( "color" ) )
    {
      HC_Show_Color( m_pSavedColor );
      m_bResetColor = true;
    }
    if( HC_Show_Existence( "visibility" ) )
    {
      HC_Show_Visibility( m_pSavedVisibility );
      m_bResetVisibility = true;
    }

    HC_Close_Segment();
  }

  float tmp,vl;

  // map screen mouse points to sphere mouse points
  tmp = (m_ptRealPoint.x * m_ptRealPoint.x + m_ptRealPoint.y * m_ptRealPoint.y);
  vl = (float)sqrt(tmp);

  // remember screen mouse position
  m_ptRealOld.x = m_ptRealPoint.x;
  m_ptRealOld.y = m_ptRealPoint.y;

  if (vl > 1.0f) 
  {
    m_ptRealPoint.x /= vl;
    m_ptRealPoint.y /= vl;
    m_ptRealPoint.z = 0.0;
  }
  else
    m_ptRealPoint.z = (float) sqrt (1.0f - tmp);

  m_ptFirstReal = m_ptRealPoint;

  m_ptRectangle[0] = m_ptRectangle[1] = m_ptFirst;

  return;
}



void CHoopsViewerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  CView::OnLButtonUp(nFlags, point);
  m_bOperatorStarted = false;

  if( m_bZoomWindowOn )
  {
    HC_Open_Segment_By_Key(m_lSceneKey);

    if( m_lRectangleKey != 0 )
      HC_Delete_By_Key(m_lRectangleKey);

    if( m_lMarkerKey != 0 )
      HC_Delete_By_Key(m_lMarkerKey);

    m_lRectangleKey = 0;
    m_lMarkerKey    = 0;

    if( m_bResetColor )
    {
      HC_UnSet_Color();
      HC_Set_Color (m_pSavedColor);
    }

    if( m_bResetVisibility )
    {
      HC_UnSet_Visibility();
      HC_Set_Visibility (m_pSavedVisibility);
    }

    HC_Close_Segment();

    m_bResetColor = false;
    m_bResetVisibility = false;

    if( fabs(m_ptRectangle[0].x - m_ptRectangle[1].x) > 1 && fabs(m_ptRectangle[0].y - m_ptRectangle[1].y) > 1 )
      ZoomWindow();

    m_bZoomWindowOn = false;

    HC_Update_Display();
  }

  return;
}

void CHoopsViewerView::OnMouseMove(UINT nFlags, CPoint point) 
{
  CView::OnMouseMove(nFlags, point);

  if (m_bOperatorStarted)
  {
    if( m_bZoomWindowOn )
    {
      HPoint newPoint;

      newPoint.x = (float) point.x;
      newPoint.y = (float) point.y;
      newPoint.z = 0.0;

      OnZoomWindowMouseMove( newPoint );
    }
    else
    {
      switch( m_iViewMode )
      {
      case kOrbit:
        {
          HPoint newPoint, real_new;

          newPoint.x = (float) point.x;
          newPoint.y = (float) point.y;
          newPoint.z = 0.0;

          HC_Open_Segment(m_sDriverSegment);
          HC_Compute_Coordinates(".", "local pixels", &newPoint, "local window", &newPoint);
          HC_Close_Segment();

          // remember the real mouse positions
          real_new.x = newPoint.x;
          real_new.y = newPoint.y;

          OnOrbitMouseMove( newPoint, real_new );

          // update sphere space mouse
          m_ptRealPoint.x = newPoint.x;
          m_ptRealPoint.y = newPoint.y;
          m_ptRealPoint.z = newPoint.z;

          // update screen space mouse
          m_ptRealOld.x = real_new.x;
          m_ptRealOld.y = real_new.y;

        } break;

      case kZoom:
        {
          HPoint newPoint;

          newPoint.x = (float) point.x;
          newPoint.y = (float) point.y;
          newPoint.z = 0.0;

          HC_Open_Segment(m_sDriverSegment);
          HC_Compute_Coordinates(".", "local pixels", &newPoint, "local window", &newPoint);
          HC_Close_Segment();

          OnZoomMouseMove( newPoint ); break;

        } break;

      case kPan:
        {
          HPoint newPoint;
          newPoint.x = (float) point.x;
          newPoint.y = (float) point.y;
          newPoint.z = 0.0;

          HC_Open_Segment_By_Key(m_lSceneKey);
          HC_Compute_Coordinates(".", "local pixels", &newPoint, "world", &newPoint);
          HC_Close_Segment();

          OnPanMouseMove( newPoint ); break;
        } break;
      }
    }

    HC_Update_Display();

  }

  return;
}

BOOL CHoopsViewerView::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint point)
{
  HPoint ptBase;
  HPoint ptScreen;
  HPoint cameraPos;
  HPoint cameraTarget;
  HPoint ptDist;
  float Width, Height;
  double fLen  = 1;
  char   proj[64];

  CRect wndRect;

  GetWindowRect( &wndRect );

  point.x -= wndRect.left;
  point.y -= wndRect.top;

  ptBase.x = (float)point.x;
  ptBase.y = (float)point.y;
  ptBase.z = 0;

  HC_Open_Segment_By_Key( m_lSceneKey );
  HC_Compute_Coordinates(".", "local pixels", &ptBase, "world", &ptBase);
  HC_Show_Net_Camera_Position (&cameraPos.x, &cameraPos.y, &cameraPos.z);
  HC_Show_Net_Camera_Target (&cameraTarget.x, &cameraTarget.y, &cameraTarget.z);
  HC_Show_Net_Camera_Field(&Width, &Height);
  HC_Show_Net_Camera_Projection(proj);

  double zoom_factor = 1;

  if( zDelta < 0 )
    zoom_factor = MOUSE_WHEEL_STEP;
  else
    zoom_factor = 1.0/MOUSE_WHEEL_STEP;

  ptDist.x = cameraPos.x - cameraTarget.x;
  ptDist.y = cameraPos.y - cameraTarget.y;
  ptDist.z = cameraPos.z - cameraTarget.z;
  fLen = HC_Compute_Vector_Length(&ptDist);
  HC_Compute_Normalized_Vector(&ptDist,&ptDist);

  HC_Set_Camera_Position( cameraTarget.x + ptDist.x * fLen*zoom_factor,
    cameraTarget.y + ptDist.y * fLen*zoom_factor,
    cameraTarget.z + ptDist.z * fLen*zoom_factor);

  if( strcmp(proj, "orthographic"))
    HC_Set_Camera_Field(Width * zoom_factor, Height * zoom_factor);

  HC_Show_Net_Camera_Position (&cameraPos.x, &cameraPos.y, &cameraPos.z);
  HC_Show_Net_Camera_Target (&cameraTarget.x, &cameraTarget.y, &cameraTarget.z);

  ptScreen.x = (float)point.x;
  ptScreen.y = (float)point.y;
  ptScreen.z = 0;

  HC_Compute_Coordinates(".", "local pixels", &ptScreen, "world", &ptScreen);

  double dX = ptBase.x - ptScreen.x;
  double dY = ptBase.y - ptScreen.y;
  double dZ = ptBase.z - ptScreen.z;

  // Pan

  HC_Set_Camera_Target   (cameraTarget.x + dX, cameraTarget.y + dY, cameraTarget.z + dZ);
  HC_Set_Camera_Position (cameraPos.x + dX, cameraPos.y + dY, cameraPos.z + dZ);

  ptBase.x = (float)point.x;
  ptBase.y = (float)point.y;
  ptBase.z = 0;

  HC_Compute_Coordinates(".", "local pixels", &ptBase, "world", &ptBase);

  HC_Close_Segment();

  UpdateLamp();

  HC_Update_Display();

  return TRUE;
}

void CHoopsViewerView::OnViewWireframe() 
{
  HC_Open_Segment_By_Key(m_lDriverKey);
  HC_Set_Heuristics("no hidden surfaces");
  HC_Close_Segment();

  CHoopsViewerDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);

  HC_Open_Segment_By_Key(m_lSceneKey);
  if( pDoc->IsBrepGeometry() )
    HC_Set_Visibility("faces = off, edges=off, lines = on, text = on, lights=off, markers=off");
  else
    HC_Set_Visibility("faces = off, edges=on, text = on, lines = on, lights=off, markers=off");
  HC_Close_Segment();

  HC_Update_Display();

}

void CHoopsViewerView::OnUpdateViewWireframe(CCmdUI* pCmdUI) 
{
  int render_mode = GetRenderMode();

  if (render_mode == WIREFRAME)
    pCmdUI->SetCheck(true);
  else
    pCmdUI->SetCheck(false);

}

void CHoopsViewerView::OnViewShaded() 
{
  HC_Open_Segment_By_Key(m_lDriverKey);
  if (!strcmp (theApp.GetDriverType(), "opengl")) {
    HC_Set_Rendering_Options ("hsr algorithm = hardware z-buffer");
  }
  else {
    HC_Set_Rendering_Options ("hsra = software z buffer");
  }
  HC_Set_Heuristics("hidden surfaces");
  HC_Close_Segment();

  HC_Open_Segment_By_Key(m_lSceneKey);
  HC_Set_Visibility("faces = on, edges=off, lights=on, markers=off");

  if( GetDocument()->m_bShowLines )
    HC_Set_Visibility("lines = on");
  else
    HC_Set_Visibility("lines = off");

  if( GetDocument()->m_bShowText )
    HC_Set_Visibility("text = on");
  else
    HC_Set_Visibility("text = off");

  HC_Close_Segment();

  HC_Update_Display();
}

void CHoopsViewerView::OnUpdateViewShaded(CCmdUI* pCmdUI) 
{
  int hlr = GetRenderMode();

  if (hlr == SHADED)
    pCmdUI->SetCheck(true);
  else
    pCmdUI->SetCheck(false);
}

void CHoopsViewerView::OnViewHiddenline() 
{
  HC_Open_Segment_By_Key(m_lDriverKey);
  HC_Set_Rendering_Options ("hsr algorithm = hidden line, hlro=(visibility=off)");
  HC_Set_Heuristics("hidden surfaces");
  HC_Set_Visibility("faces=on");
  HC_Close_Segment();

  CHoopsViewerDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);

  HC_Open_Segment_By_Key(m_lSceneKey);
  if( pDoc->IsBrepGeometry() )
    HC_Set_Visibility("faces = on, lines = on, text = on, lights=off, markers=off, edges=(everything=off, interior silhouettes=on)");
  else
    HC_Set_Visibility("faces = on, lines = on, text = on, lights=off, markers=off, edges=on");
  HC_Close_Segment();

  pDoc->m_bShowLines = true;
  pDoc->m_bShowText  = true;

  HC_Update_Display();

}

void CHoopsViewerView::OnUpdateViewHiddenline(CCmdUI* pCmdUI) 
{
  int hlr = GetRenderMode();

  if (hlr == HIDDENLINE)
    pCmdUI->SetCheck(true);
  else
    pCmdUI->SetCheck(false);
}

void CHoopsViewerView::OnAxisOnOff()
{
  ((CHoopsViewerApp*)(AfxGetApp()))->m_bShowAxis = !((CHoopsViewerApp*)(AfxGetApp()))->m_bShowAxis;

  RotateAxisOnUpdate();
  HC_Update_Display();
}

void CHoopsViewerView::OnUpdateAxisOnOff(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck( ((CHoopsViewerApp*)(AfxGetApp()))->m_bShowAxis );
}

int CHoopsViewerView::GetRenderMode()
{
  // This function uses the following logic to determine what render mode
  // we are in.  If hidden surfaces are turned off then we are in wireframe
  // mode.  If hidden surfaces are turned on, and the hsra is hidden line
  // then we are in 'hidden line' mode.  If the hsra has a 'z buffer' in it
  // then it is either hardware z buffer or software z buffer both of which
  // are shaded modes

  char hsralg[1024], hs[512];
  int hsra = WIREFRAME;

  HC_Open_Segment(m_sDriverSegment);
  HC_Show_One_Net_Heuristic("hidden surfaces", hs);

  if ((strstr (hs, "no") || strstr (hs, "off")))
    hsra = WIREFRAME;
  else
  {
    HC_Show_One_Net_Rendering_Optio("hidden surface removal algorithm", hsralg);
    if (strstr(hsralg, "hidden line"))
      hsra = HIDDENLINE;
    else if (strstr(hsralg, "z buffer"))
      hsra=SHADED;
  }
  HC_Close_Segment();

  return hsra;
}


void CHoopsViewerView::OnOrbit()
{
  m_iViewMode = kOrbit;
}

void CHoopsViewerView::OnZoom()
{
  m_iViewMode = kZoom;
}

void CHoopsViewerView::OnPan()
{
  m_iViewMode = kPan;
}

void CHoopsViewerView::OnZoomWindow()
{
  m_bZoomWindowOn = true;
}

void CHoopsViewerView::OnZoomToExtents()
{
  HC_Open_Segment_By_Key(m_lSceneKey);
  FitWorld();
  HC_Close_Segment();
  HC_Update_Display();
}

void CHoopsViewerView::OnUpdateOrbit(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck( m_iViewMode == kOrbit );
}

void CHoopsViewerView::OnUpdateZoom(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck( m_iViewMode == kZoom );
}

void CHoopsViewerView::OnUpdatePan(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck( m_iViewMode == kPan );
}

void CHoopsViewerView::OnUpdateZoomToExtents(CCmdUI* /*pCmdUI*/)
{

}

void CHoopsViewerView::OnUpdateZoomWindow(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck( m_bZoomWindowOn );
}

void CHoopsViewerView::OnOrbitMouseMove( HPoint& newPoint, HPoint& real_new )
{
  HPoint axis, vtmp;	
  float theta, dist, tmp, vl, angle1, angle2, angle3;

  // map screen mouse points to sphere mouse points
  tmp = newPoint.x * newPoint.x + newPoint.y * newPoint.y;
  vl = (float)sqrt(tmp);

  if (vl > 1.0f) 
  {
    newPoint.x /= vl;
    newPoint.y /= vl;
    newPoint.z = 0.0f;
  }
  else
    newPoint.z = (float)sqrt(1.0f - tmp);

  // get the axis of rotation
  HC_Compute_Cross_Product (&m_ptRealPoint, &newPoint, &axis);

  // this is for screen mouse based movement
  vtmp.x = real_new.x - m_ptRealOld.x;
  vtmp.y = real_new.y - m_ptRealOld.y;
  dist = (float)sqrt(vtmp.x * vtmp.x + vtmp.y * vtmp.y) * 90.0f;

  if ((axis.x != 0.0 || axis.y != 0.0 || axis.z != 0)){

    HC_Compute_Normalized_Vector(&axis, &axis);
    HC_Open_Segment_By_Key(m_lSceneKey);

    // project axis of rotation onto yz plane 
    vtmp.x = 0.0;
    vtmp.y = axis.y;
    vtmp.z = axis.z;

    // calculate angle of x orbit
    tmp = (float)HC_Compute_Dot_Product(&axis, &vtmp);
    if(fabs(tmp) > 1.001f || fabs(tmp) < 0.999f)		
      theta = (float)acos(tmp);
    else
      theta = 0.0f;

    if (axis.x < 0.0)
      angle2= -theta * dist;
    else
      angle2= theta * dist;

    // project axis of rotation onto xz plane 
    vtmp.x = axis.x;
    vtmp.y = 0.0;
    vtmp.z = axis.z;

    // calculate angle of y orbit
    tmp = (float)HC_Compute_Dot_Product(&axis, &vtmp);
    if (fabs(tmp) > 1.001f || fabs(tmp) < 0.999f)		
      theta = (float)acos(tmp);
    else
      theta = 0.0f;

    if (axis.y < 0.0)
      angle1=-theta * dist;
    else
      angle1=theta * dist;

    // project axis of rotation onto xy plane 
    vtmp.x = axis.x;
    vtmp.y = axis.y;
    vtmp.z = 0.0;

    // calculate angle of z orbit
    tmp = (float)HC_Compute_Dot_Product(&axis, &vtmp);
    if(fabs(tmp) > 1.001f || fabs(tmp) < 0.999f)		
      theta = (float)acos(tmp);
    else
      theta = 0.0f;

    if (axis.z < 0.0)
      angle3=-theta * dist;
    else
      angle3=theta * dist;

    HC_Orbit_Camera(angle1,0);
    HC_Orbit_Camera(0,angle2);
    HC_Roll_Camera(angle3);

    HC_Close_Segment();

    UpdateLamp();
    RotateAxisOnUpdate();
  }
}

void CHoopsViewerView::OnZoomMouseMove( HPoint newPoint )
{
  float tmp = newPoint.x * newPoint.x + newPoint.y * newPoint.y;
  float vl = (float)sqrt(tmp);

  if (vl > 1.0f) 
  {
    newPoint.x /= vl;
    newPoint.y /= vl;
    newPoint.z = 0.0f;
  }
  else
    newPoint.z = (float)sqrt(1.0f - tmp);

  HC_Open_Segment_By_Key(m_lSceneKey);

  float w, h, zoom_factor = newPoint.y - m_ptFirstReal.y;

  HPoint target, position, diff; 
  char proj[64];

  HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);
  HC_Show_Net_Camera_Field(&w, &h);
  HC_Show_Net_Camera_Projection(proj);

  // we only want to zoom in if we are still further out than
  // the maximum zoom level and we are actually zooming in
  float maxZoomLevel = 0.000001f;

  if( (w > maxZoomLevel) || (h > maxZoomLevel) || (zoom_factor<0.0f) )
  {

    diff.x = position.x - target.x;
    diff.y = position.y - target.y;
    diff.z = position.z - target.z;

    if( zoom_factor < 0.99 )
      HC_Set_Camera_Position( m_ptCamera.x + m_ptDist.x * (m_fLength-(m_fLength*zoom_factor)),
      m_ptCamera.y + m_ptDist.y * (m_fLength-(m_fLength*zoom_factor)),
      m_ptCamera.z + m_ptDist.z * (m_fLength-(m_fLength*zoom_factor)));

    if( strcmp(proj, "orthographic"))
    {
      if( !(zoom_factor > 0.99) )
        HC_Set_Camera_Field(m_Width - (m_Width * zoom_factor), m_Height - (m_Height * zoom_factor));
    }

    UpdateLamp();
  }

  HC_Close_Segment();
}

void CHoopsViewerView::OnPanMouseMove( HPoint newPoint )
{
  HPoint delta;
  delta.x = newPoint.x - m_ptWorldFirst.x;
  delta.y = newPoint.y - m_ptWorldFirst.y;
  delta.z = newPoint.z - m_ptWorldFirst.z;

  HC_Open_Segment_By_Key( m_lSceneKey );
  HPoint camera, target;

  HC_Show_Net_Camera_Target (&target.x, &target.y, &target.z);
  HC_Show_Net_Camera_Position (&camera.x, &camera.y, &camera.z);

  HC_Set_Camera_Target (target.x - delta.x, target.y - delta.y, target.z - delta.z);
  HC_Set_Camera_Position (camera.x - delta.x, camera.y - delta.y, camera.z - delta.z);

  HC_Close_Segment();

  UpdateLamp();
}

void CHoopsViewerView::OnZoomWindowMouseMove( HPoint newPoint )
{
  HC_Open_Segment_By_Key(m_lSceneKey);

  HC_Open_Segment( "SelWindow" );

  if( m_lRectangleKey != 0 )
    HC_Delete_By_Key(m_lRectangleKey);

  if( m_lMarkerKey != 0 )
    HC_Delete_By_Key(m_lMarkerKey);

  HPoint  pts[5];
  HPoint center;

  m_ptRectangle[0] = m_ptFirst;
  m_ptRectangle[1] = newPoint;

  center.x = 0.5F * (m_ptFirst.x + newPoint.x);
  center.y = 0.5F * (m_ptFirst.y + newPoint.y);
  center.z = 0;

  pts[0].x = m_ptFirst.x;   pts[0].y = m_ptFirst.y;   pts[0].z = 0;
  pts[1].x = m_ptFirst.x;   pts[1].y = newPoint.y;   pts[1].z = 0;
  pts[2].x = newPoint.x;   pts[2].y = newPoint.y;   pts[2].z = 0;
  pts[3].x = newPoint.x;   pts[3].y = m_ptFirst.y;   pts[3].z = 0;
  pts[4].x = m_ptFirst.x;   pts[4].y = m_ptFirst.y;   pts[4].z = 0;

  HC_Open_Segment_By_Key(m_lSceneKey);
  HC_Compute_Coordinates(".", "local pixels", &pts[0], "world", &pts[0]);
  HC_Compute_Coordinates(".", "local pixels", &pts[1], "world", &pts[1]);
  HC_Compute_Coordinates(".", "local pixels", &pts[2], "world", &pts[2]);
  HC_Compute_Coordinates(".", "local pixels", &pts[3], "world", &pts[3]);
  HC_Compute_Coordinates(".", "local pixels", &pts[4], "world", &pts[4]);
  HC_Compute_Coordinates(".", "local pixels", &center, "world", &center);
  HC_Close_Segment();

  m_lRectangleKey = HC_KInsert_Polyline(5,pts); 

  m_lMarkerKey = HC_KInsert_Marker (center.x, center.y, center.z);
  HC_Set_Visibility ("lines = on, markers = on");	

  HC_Close_Segment();

  HC_Close_Segment();
}

void CHoopsViewerView::ZoomWindow()
{
  HPoint ptCenter;
  float  zoom_factor = 1;

  CRect wndRect;

  GetWindowRect( &wndRect );

  CRect  clientRect;
  CRect  targetRect;
  GetClientRect( &clientRect );
  targetRect.left  = 0;
  targetRect.right = (LONG)(fabs( m_ptRectangle[0].x - m_ptRectangle[1].x ));
  targetRect.top = 0;
  targetRect.bottom = (LONG)(fabs( m_ptRectangle[0].y - m_ptRectangle[1].y ));

  float xFactor = (float)(targetRect.Width() * 1.0 / clientRect.Width());
  float yFactor = (float)(targetRect.Height() * 1.0 / clientRect.Height());

  if( xFactor > yFactor )
    zoom_factor = xFactor;
  else
    zoom_factor = yFactor;

  ptCenter.x = (m_ptRectangle[0].x + m_ptRectangle[1].x)*0.5f;
  ptCenter.y = (m_ptRectangle[0].y + m_ptRectangle[1].y)*0.5f;
  ptCenter.z = 0;

  HC_Open_Segment_By_Key( m_lSceneKey );

  HC_Compute_Coordinates(".", "local pixels", &ptCenter, "world", &ptCenter);

  HPoint camera;
  HPoint target;
  float Width, Height;

  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);
  HC_Show_Net_Camera_Position (&camera.x, &camera.y, &camera.z);
  HC_Show_Net_Camera_Field(&Width, &Height);

  HC_Set_Camera_Target (ptCenter.x, ptCenter.y, ptCenter.z);
  HC_Set_Camera_Position (camera.x + ptCenter.x - target.x, camera.y + ptCenter.y - target.y, camera.z + ptCenter.z - target.z);

  if( !(zoom_factor > 0.99) )
    HC_Set_Camera_Field(Width * zoom_factor, Height * zoom_factor);

  HC_Close_Segment();

  UpdateLamp();
}

void CHoopsViewerView::OnXYPlane()
{
  HC_Open_Segment_By_Key(m_lSceneKey);

  HPoint target, position; 

  HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);

  float fLength = sqrt((position.x - target.x)*(position.x - target.x) + 
    (position.y - target.y)*(position.y - target.y) +
    (position.z - target.z)*(position.z - target.z));

  HC_Set_Camera_Position (target.x, target.y, target.z + fLength);
  HC_Set_Camera_Up_Vector( 0, 1, 0 );

  HC_Close_Segment();

  UpdateLamp();
  RotateAxisOnUpdate();

  HC_Update_Display();
}

void CHoopsViewerView::OnXZPlane()
{
  HC_Open_Segment_By_Key(m_lSceneKey);

  HPoint target, position; 

  HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);

  float fLength = sqrt((position.x - target.x)*(position.x - target.x) + 
    (position.y - target.y)*(position.y - target.y) +
    (position.z - target.z)*(position.z - target.z));

  HC_Set_Camera_Position (target.x, target.y - fLength, target.z);
  HC_Set_Camera_Up_Vector( 0, 0, 1 );

  HC_Close_Segment();

  UpdateLamp();
  RotateAxisOnUpdate();

  HC_Update_Display();
}

void CHoopsViewerView::OnYZPlane()
{
  HC_Open_Segment_By_Key(m_lSceneKey);

  HPoint target, position; 

  HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);

  float fLength = sqrt((position.x - target.x)*(position.x - target.x) + 
    (position.y - target.y)*(position.y - target.y) +
    (position.z - target.z)*(position.z - target.z));

  HC_Set_Camera_Position (target.x - fLength, target.y, target.z);
  HC_Set_Camera_Up_Vector( 0, 0, 1 );

  HC_Close_Segment();

  UpdateLamp();
  RotateAxisOnUpdate();

  HC_Update_Display();
}

void CHoopsViewerView::OnBottomView()
{
  HC_Open_Segment_By_Key(m_lSceneKey);

  HPoint target, position; 

  HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);

  float fLength = sqrt((position.x - target.x)*(position.x - target.x) + 
    (position.y - target.y)*(position.y - target.y) +
    (position.z - target.z)*(position.z - target.z));

  HC_Set_Camera_Position (target.x, target.y, target.z - fLength);
  HC_Set_Camera_Up_Vector( 0, 1, 0 );

  HC_Close_Segment();

  UpdateLamp();
  RotateAxisOnUpdate();

  HC_Update_Display();
}

void CHoopsViewerView::OnRightView()
{
  HC_Open_Segment_By_Key(m_lSceneKey);

  HPoint target, position; 

  HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);

  float fLength = sqrt((position.x - target.x)*(position.x - target.x) + 
    (position.y - target.y)*(position.y - target.y) +
    (position.z - target.z)*(position.z - target.z));

  HC_Set_Camera_Position (target.x + fLength, target.y, target.z);
  HC_Set_Camera_Up_Vector( 0, 0, 1 );

  HC_Close_Segment();

  UpdateLamp();
  RotateAxisOnUpdate();

  HC_Update_Display();
}

void CHoopsViewerView::OnBackView()
{
  HC_Open_Segment_By_Key(m_lSceneKey);

  HPoint target, position; 

  HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);

  float fLength = sqrt((position.x - target.x)*(position.x - target.x) + 
    (position.y - target.y)*(position.y - target.y) +
    (position.z - target.z)*(position.z - target.z));

  HC_Set_Camera_Position (target.x, target.y + fLength, target.z);
  HC_Set_Camera_Up_Vector( 0, 0, 1 );

  HC_Close_Segment();

  UpdateLamp();
  RotateAxisOnUpdate();

  HC_Update_Display();
}

void CHoopsViewerView::OnNEIsometric()
{
  HC_Open_Segment_By_Key(m_lSceneKey);

  HPoint target, position; 

  HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);

  float fLength = sqrt((position.x - target.x)*(position.x - target.x) + 
    (position.y - target.y)*(position.y - target.y) +
    (position.z - target.z)*(position.z - target.z));

  fLength = (float)(fLength/sqrt(3.0));

  HC_Set_Camera_Position (target.x + fLength, target.y + fLength, target.z + fLength);
  HC_Set_Camera_Up_Vector( 0, 0, 1 );

  HC_Close_Segment();

  UpdateLamp();
  RotateAxisOnUpdate();

  HC_Update_Display();
}

void CHoopsViewerView::OnSWIsometric()
{
  HC_Open_Segment_By_Key(m_lSceneKey);

  HPoint target, position; 

  HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);

  float fLength = sqrt((position.x - target.x)*(position.x - target.x) + 
    (position.y - target.y)*(position.y - target.y) +
    (position.z - target.z)*(position.z - target.z));

  fLength = (float)(fLength/sqrt(3.0));

  HC_Set_Camera_Position (target.x - fLength, target.y - fLength, target.z + fLength);
  HC_Set_Camera_Up_Vector( 0, 0, 1 );

  HC_Close_Segment();

  UpdateLamp();
  RotateAxisOnUpdate();

  HC_Update_Display();
}

void CHoopsViewerView::OnSEIsometric()
{
  HC_Open_Segment_By_Key(m_lSceneKey);

  HPoint target, position; 

  HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);

  float fLength = sqrt((position.x - target.x)*(position.x - target.x) + 
    (position.y - target.y)*(position.y - target.y) +
    (position.z - target.z)*(position.z - target.z));

  fLength = (float)(fLength/sqrt(3.0));

  HC_Set_Camera_Position (target.x + fLength, target.y - fLength, target.z + fLength);
  HC_Set_Camera_Up_Vector( 0, 0, 1 );

  HC_Close_Segment();

  UpdateLamp();
  RotateAxisOnUpdate();

  HC_Update_Display();
}

void CHoopsViewerView::OnNWIsometric()
{
  HC_Open_Segment_By_Key(m_lSceneKey);

  HPoint target, position; 

  HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);

  float fLength = sqrt((position.x - target.x)*(position.x - target.x) + 
    (position.y - target.y)*(position.y - target.y) +
    (position.z - target.z)*(position.z - target.z));

  fLength = (float)(fLength/sqrt(3.0));

  HC_Set_Camera_Position (target.x - fLength, target.y + fLength, target.z + fLength);
  HC_Set_Camera_Up_Vector( 0, 0, 1 );

  HC_Close_Segment();

  UpdateLamp();
  RotateAxisOnUpdate();

  HC_Update_Display();
}

void CHoopsViewerView::ChangeBgColor()
{
  HC_Open_Segment (m_sDriverSegment);

  COLORREF clrBgColor = ((CHoopsViewerApp*)AfxGetApp())->m_clrBgColor;

  CString strColor;
  strColor.Format(L"windows=(R = %f G = %f B = %f)", GetRValue(clrBgColor) / 255.0,
    GetGValue(clrBgColor) / 255.0, GetBValue(clrBgColor) / 255.0 );
  char* pchData = new char[strColor.GetLength() + 1] ;
  wcstombs( pchData, strColor.GetBuffer(strColor.GetLength()), strColor.GetLength() );
  strColor.ReleaseBuffer();
  pchData[strColor.GetLength()] = '\0';
  HC_Set_Color(pchData);
  delete[] pchData;
  HC_Close_Segment();
  HC_Update_Display();
}

void CHoopsViewerView::UpdateLamp()
{
  HPoint	position, target, up;
  float	width, height;
  char    projection[16];

  HC_Open_Segment_By_Key(m_lSceneKey);
  HC_Show_Net_Camera (&position, &target, &up, &width, &height, projection);
  HC_Close_Segment();

  HPoint vector1, vector2, new_light;
  vector1.x = position.x - target.x;
  vector1.y = position.y - target.y;
  vector1.z = position.z - target.z;
  HC_Compute_Normalized_Vector(&vector1, &vector1);
  HC_Compute_Normalized_Vector(&up, &up);
  HC_Compute_Cross_Product(&vector1, &up, &vector2);

  // offset the light from the camera vector so that flats objects facing the screen
  // do not appear washed out
  new_light.x = 1.5f*vector1.x+vector2.x;
  new_light.y = 1.5f*vector1.y+vector2.y;
  new_light.z = 1.5f*vector1.z+vector2.z;
  HC_Move_Distant_Light (m_lLightKey, new_light.x, new_light.y, new_light.z);
}

void CHoopsViewerView::SetAxis()
{
  HC_Open_Segment_By_Key(m_AxisTriadKey);  
  HC_Open_Segment("");
  // this forces the axis segment forward in the draw order
  // to get it visited early for timed updates
  HC_Set_Heuristics("quick moves");
  HC_Close_Segment();


  //things under the axis triad segment are too small to benefit from display lists
  HC_Set_Rendering_Options("no display lists"); 
  HC_Set_Rendering_Options("no force gray scale");
  HC_Set_Heuristics("no culling");
  HC_Set_Visibility("text = on");
  HC_Set_Visibility("no shadows");
  //this is just a dummy entry as the window needs to be adjusted outside the MVO with regard to the
  //outer window size
  HC_Set_Rendering_Options ("screen range=(0.1, 1.0, 0.1, 1.0)");
  HC_Set_Rendering_Options("depth range=(0,0.1)");
  HC_Set_Rendering_Options("no lod, no simple shadow, no simple reflection");
  HC_Set_Heuristics("exclude bounding");
  //the rendering options need to be set here explicitly so that they overwrite any inherited attributes 
  //from the view segment
  HC_Set_Visibility("markers=off,lines=off, edges=off,faces=on,lights=on, cutting planes = off");
  HC_Set_Rendering_Options("attribute lock=(visibility), hidden line removal options = (render faces = on)");
  HC_Set_Heuristics("no polygon handedness");
  HC_Set_Selectability("everything=off");				
  HC_Set_Camera_Projection("orthographic");
  HC_Set_Heuristics("hidden surfaces");
  HC_Set_Handedness("right");
  HC_Set_Text_Font("name=stroked");

  HC_Open_Segment("Z");
  HC_Set_Rendering_Options("attribute lock = (color=faces)");
  InsertArrow(0.5F, 0.1F, 0.05F, 0.1F,"Z", 0, 0, 10, 0.0F);
  HC_Set_Color("geometry = red");
  HC_Rotate_Object(90.0, 0.0, 0.0);
  HC_Close_Segment();

  HC_Open_Segment("Y");
  // need to set this for the fast hidden line mode
  HC_Set_Rendering_Options("attribute lock = (color=faces)");
  InsertArrow(0.5F, 0.1F, 0.05F, 0.1F,"Y", 0, 0, 10, 0.0F);
  HC_Set_Color("geometry = green");
  HC_Close_Segment();

  HC_Open_Segment("X");
  HC_Set_Rendering_Options("attribute lock = (color=faces)");
  InsertArrow(0.5F, 0.1F, 0.05F, 0.1F, "X", 0, 0, 10, 0.0F);
  HC_Set_Color("geometry = blue");
  HC_Rotate_Object(0.0, 0.0, -90.0);
  // need to set this for the fast hidden line mode
  HC_Close_Segment();

  AdjustAxisWindow();
  RotateAxisOnUpdate();
  HC_Close_Segment();
}

void		CHoopsViewerView::InsertArrow(float stick_length,
                                    float tip_length, 
                                    float stick_radius, 
                                    float tip_radius, 
                                    const char *text, 
                                    const char *stick_color, 
                                    const char *tip_color, 
                                    int num_sides, 
                                    float tip_falloff, 
                                    bool reverse)
{
  HPoint	bottom, top;	

  HPoint *stick_points = 0;
  HPoint *tip_points = 0;
  int *stick_faces_list = 0;
  int *tip_faces_list = 0;
  int i;

  stick_points= new HPoint[num_sides*2];				
  tip_points= new HPoint[num_sides+1];
  stick_faces_list= new int[num_sides*5+num_sides+1];
  tip_faces_list= new int[num_sides*4];

  bottom.x = 0.0;
  bottom.y = tip_length;
  bottom.z = 0.0;

  top.x = 0.0;
  top.y = stick_length + tip_length;
  top.z = 0.0;

  tip_points[num_sides].x = 0;
  tip_points[num_sides].y = 0;
  tip_points[num_sides].z = 0;

  GeneratePointsOnCircle(stick_points, top, stick_radius-tip_falloff,num_sides);
  GeneratePointsOnCircle(stick_points+num_sides, bottom, stick_radius,num_sides);
  GeneratePointsOnCircle(tip_points, bottom, tip_radius, num_sides);

  for (i=0;i<num_sides;i++)
  {
    stick_faces_list[i*5]=4;
    stick_faces_list[i*5+1]=i;
    stick_faces_list[i*5+2]=i+num_sides;
    stick_faces_list[i*5+3]=((i+1)%num_sides)+num_sides;
    stick_faces_list[i*5+4]=(i+1)%num_sides;

    tip_faces_list[i*4]=3;
    tip_faces_list[i*4+1]=i;
    tip_faces_list[i*4+2]=(i+1)%num_sides;
    tip_faces_list[i*4+3]=num_sides;

    stick_faces_list[num_sides*5+i+1]=i;		//this constructs the face for the bottom of
    //the cylinder (not really needed for the axis)
  }
  stick_faces_list[num_sides*5]=num_sides;

  HC_Open_Segment("inner"); 
  if (reverse)
  {
    HC_Rotate_Object(180,0,0);
    HC_Translate_Object(0,stick_length + tip_length,0);
  }
  if (stick_color)
  {
    HC_Open_Segment("");
    HC_Set_Color(stick_color);
    HC_Insert_Shell(num_sides*2,stick_points,num_sides*5+num_sides+1,stick_faces_list);
    HC_Close_Segment();

  }else
    HC_Insert_Shell(num_sides*2,stick_points,num_sides*5+num_sides+1,stick_faces_list);

  if (tip_color)
  {
    HC_Open_Segment("");	
    HC_Set_Color(tip_color);
    HC_Insert_Shell(num_sides+1,tip_points,num_sides*4,tip_faces_list);
    HC_Close_Segment();
  }else
    HC_Insert_Shell(num_sides+1,tip_points,num_sides*4,tip_faces_list);
  HC_Close_Segment();

  if (text)	
  {
    HC_Set_Text_Font("name=stroked,size=12px");
    HC_Insert_Text(0,top.y+0.1,0.0,text);
    HC_Set_Color("text = white");
  }

  if (stick_points)
    delete []stick_points;
  if (tip_points)
    delete []tip_points;
  if (stick_faces_list)
    delete []stick_faces_list;
  if (tip_faces_list)
    delete []tip_faces_list;
}

bool CHoopsViewerView::GeneratePointsOnCircle( HPoint* points, HPoint const & center,float radius,int numpoints)
{
  int i;
  bool success = true;
  float angle = 2.0f * 3.1415926f / (float)numpoints;

  for (i=0; i<numpoints; i++)
  {
    points[i].x = center.x + radius * (float)cos(angle*i);
    points[i].y = center.y;
    points[i].z = center.z + radius * (float)sin (angle*i);
  }

  return success;
}


void CHoopsViewerView::AdjustAxisWindow()
{
  int size = m_AxisWindowSize;

  HC_Open_Segment_By_Key(m_lDriverKey);
  char tmp[1024];
  HC_Show_Device_Info_By_Key(m_lDriverKey, "pixels", tmp);

  int h = 0;
  int w = 0;

  if (tmp[0] != '\0') 
  {
    int nx, ny;
    sscanf( tmp, "%d,%d", &nx, &ny);
    w = nx;
    h = ny;		
  }

  if( w>0 && h>0 ) 
  {
    HC_Open_Segment_By_Key(m_AxisTriadKey);
    char text[4096];			
    sprintf(text,"screen range=(%f, %f, %f, %f)",-1.0, -1.0 + 2.03125 * (float)size/(float)w, -1.0, -1.0 + 2.03125 * (float)size/(float)h);
    HC_Set_Rendering_Options (text);
    HC_Close_Segment();
  }  
  HC_Close_Segment();
}

void CHoopsViewerView::RotateAxisOnUpdate()
{
  if( !((CHoopsViewerApp*)(AfxGetApp()))->m_bShowAxis )
  {
    HC_Open_Segment_By_Key(m_AxisTriadKey);  
    HC_Set_Visibility("geometry = off");
    HC_Close_Segment();
    return;
  }
  else
  {
    HC_Open_Segment_By_Key(m_AxisTriadKey);  
    HC_Set_Visibility("markers=off,lines=off, edges=off,faces=on,text = on,lights=on, cutting planes = off");
    HC_Close_Segment();
  }

  HPoint position, target, up_vector;

  HC_Open_Segment_By_Key(m_lSceneKey);
  HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);
  HC_Show_Net_Camera_Up_Vector(&up_vector.x, &up_vector.y, &up_vector.z);
  HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
  HC_Close_Segment();

  HC_Open_Segment_By_Key(m_lSceneKey);
  HC_Open_Segment_By_Key(m_AxisTriadKey);
  HPoint oldposition, difference;
  HPoint old_up_vector, difference2;

  position.x -= target.x;
  position.y -= target.y;
  position.z -= target.z;

  HC_Compute_Normalized_Vector(&position, &position);	

  position.x = position.x*5.0f;
  position.y = position.y*5.0f;
  position.z = position.z*5.0f;

  HC_Show_Net_Camera_Position(&oldposition.x, &oldposition.y, &oldposition.z);

  difference.x = (float)fabs(oldposition.x - position.x);
  difference.y = (float)fabs(oldposition.y - position.y);
  difference.z = (float)fabs(oldposition.z - position.z);

  HC_Show_Net_Camera_Up_Vector(&old_up_vector.x, &old_up_vector.y, &old_up_vector.z);
  difference2.x = (float)fabs(old_up_vector.x - up_vector.x);
  difference2.y = (float)fabs(old_up_vector.y - up_vector.y);
  difference2.z = (float)fabs(old_up_vector.z - up_vector.z);

  if( difference.x + difference.y + difference.z > 0.01 || difference2.x + difference2.y + difference2.z > 0.01 )
  {  
    HC_Set_Camera_Target(0.0,0.0,0.0);
    HC_Set_Camera_Position(position.x, position.y, position.z);	 
    HC_Set_Camera_Up_Vector(up_vector.x, up_vector.y, up_vector.z);	
  }

  HC_Close_Segment();
  HC_Close_Segment();
}

void CHoopsViewerView::OnSetActiveLayout()
{
  CSetLayoutDlg dlgSetLayout;

  dlgSetLayout.m_iSelItem = GetDocument()->m_iCurLayoutRecord;

  for( unsigned int i = 0; i < GetDocument()->m_layoutInfoArr.size(); i++ )
  {
    CString strLayoutName =  GetDocument()->m_layoutInfoArr[i].strSegmentName;
    strLayoutName.Replace('\\','/');

    if( strLayoutName.ReverseFind('/') != -1 )
      strLayoutName = strLayoutName.Right(strLayoutName.GetLength() - strLayoutName.ReverseFind('/') - 1 );

    dlgSetLayout.m_strLayoutsArr.push_back( strLayoutName );
  }

  if( dlgSetLayout.DoModal() == IDOK && 
    GetDocument()->m_iCurLayoutRecord != dlgSetLayout.m_iSelItem &&
    dlgSetLayout.m_iSelItem >= 0
    )
  {
    GetDocument()->m_iCurLayoutRecord = dlgSetLayout.m_iSelItem;

    HC_Open_Segment( GetDocument()->GetModelSegment() );

    if( GetDocument()->m_bMultyLayouts )
    {
      char pchSegmentName[4096];

      HC_Begin_Segment_Search("*");

      HC_KEY iActiveKey = GetDocument()->m_layoutInfoArr[dlgSetLayout.m_iSelItem].lSegmentKey; 

      while( HC_Find_Segment(pchSegmentName) )
      {
        HC_KEY lLayoutKey = HC_KOpen_Segment(pchSegmentName);

        HC_UnSet_Visibility();

        if( iActiveKey != lLayoutKey )
        {
          HC_Set_Visibility("geometry = off");
          HC_Set_Heuristics("exclude bounding = on");
        }
        else
          HC_Set_Heuristics("exclude bounding = off");

        HC_Close_Segment();
      }

      HC_End_Segment_Search();
    }

    HC_Close_Segment();

    HPoint cameraPosition = GetDocument()->m_layoutInfoArr[GetDocument()->m_iCurLayoutRecord].ptCameraPosition;
    HPoint cameraTarget   = GetDocument()->m_layoutInfoArr[GetDocument()->m_iCurLayoutRecord].ptCameraTarget;
    HPoint cameraUp       = GetDocument()->m_layoutInfoArr[GetDocument()->m_iCurLayoutRecord].vrCameraUp;

    HC_Open_Segment_By_Key( m_lSceneKey );

    HC_Set_Camera_Position( cameraPosition.x, cameraPosition.y, cameraPosition.z);
    HC_Set_Camera_Target( cameraTarget.x, cameraTarget.y, cameraTarget.z );
    HC_Set_Camera_Up_Vector( cameraUp.x, cameraUp.y, cameraUp.z );

    FitWorld();

    HC_Close_Segment();

    UpdateLamp();
    RotateAxisOnUpdate();

    HC_Update_Display();
  }
}

void CHoopsViewerView::OnUpdateSetActiveLayout(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( GetDocument()->m_layoutInfoArr.size() > 1);
}

void CHoopsViewerView::OnShowText()
{
  GetDocument()->m_bShowText = !GetDocument()->m_bShowText;

  HC_Open_Segment_By_Key(m_lSceneKey);

  if( GetDocument()->m_bShowText )
    HC_Set_Visibility("text = on");
  else
    HC_Set_Visibility("text = off");

  HC_Close_Segment();

  HC_Update_Display();
}

void CHoopsViewerView::OnUpdateShowText(CCmdUI* pCmdUI)
{
  int hlr = GetRenderMode();
  pCmdUI->SetCheck( GetDocument()->m_bShowText );
  pCmdUI->Enable(hlr == SHADED);
}

void CHoopsViewerView::OnShowLines()
{
  GetDocument()->m_bShowLines = !GetDocument()->m_bShowLines;

  HC_Open_Segment_By_Key(m_lSceneKey);

  if( GetDocument()->m_bShowLines )
    HC_Set_Visibility("lines = on");
  else
    HC_Set_Visibility("lines = off");

  HC_Close_Segment();

  HC_Update_Display();
}

void CHoopsViewerView::OnUpdateShowLines(CCmdUI* pCmdUI)
{
  int hlr = GetRenderMode();
  pCmdUI->SetCheck( GetDocument()->m_bShowLines );
  pCmdUI->Enable(hlr == SHADED);
}
