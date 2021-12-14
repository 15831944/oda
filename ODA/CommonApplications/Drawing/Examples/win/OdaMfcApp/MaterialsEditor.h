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

#if !defined(AFX_MATERIALSEDITOR_H__F10E7051_5C9B_43B8_BAC8_C31A8A7921AA__INCLUDED_)
#define AFX_MATERIALSEDITOR_H__F10E7051_5C9B_43B8_BAC8_C31A8A7921AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaterialsEditor.h : header file
//

#include "ExSystemServices.h"
#include "ExHostAppServices.h"

#include "GiContextForDbDatabase.h"

#include "PsTbEditFormViewPg.h"
#include "SnapshotDlg.h"

#include "Gi/GiMaterialItem.h"
#include "Gi/GiRasterWrappers.h"

// CCmEntityColorPreview
class CCmEntityColorPreview : public CStatic
{
	DECLARE_DYNAMIC(CCmEntityColorPreview)
  protected:
    OdCmEntityColor m_color;
  public:
	  CCmEntityColorPreview();
	  virtual ~CCmEntityColorPreview();

    void SetColor(const OdCmEntityColor &color);
    const OdCmEntityColor &GetColor() { return m_color; }
    COLORREF GetCref();
  protected:
	  DECLARE_MESSAGE_MAP()
  public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
};

/////////////////////////////////////////////////////////////////////////////
// CDbLightsDlg dialog

class OdDbDatabase;

class CDbLightsDlg : public CDialog
{
// Construction
public:
	CDbLightsDlg(OdDbDatabase* pDb, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDbLightsDlg)
	enum { IDD = IDD_RENDER_DBLIGHTS };
	CListCtrl m_LightsList;
  CCmEntityColorPreview m_Color;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbLightsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDbLightsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  int m_nItem;

  void UpdateProperties(int nItem);
private:
  OdDbDatabase* m_pDb;
public:
  afx_msg void OnLvnItemchangedLightsList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedCbon();
  afx_msg void OnBnClickedCbtargeted();
  afx_msg void OnBnClickedBhighlight();
  afx_msg void OnEnChangeEintensity();
  afx_msg void OnBnClickedBcolor();
  afx_msg void OnEnChangeEhotspot();
  afx_msg void OnEnChangeEfalloff();
};

// CMaterialPreview
class CMaterialPreview : public CStatic, public ExSystemServices, public ExHostAppServices
{
	DECLARE_DYNAMIC(CMaterialPreview)
  protected:
    OdDbDatabase *m_pHostDb;
    CDC *m_hDc;
    OdDbDatabasePtr m_pDb;
    OdGiContextForDbDatabasePtr m_pCtx;
    OdGsDevicePtr m_pDevice;
    CString m_devName;
    bool m_bDiagBkgnd;
  public:
	  CMaterialPreview(OdDbDatabase *pHostDb);
	  virtual ~CMaterialPreview();

    void addRef() { }
    void release() { }
    ODRX_USING_HEAP_OPERATORS(ExSystemServices);

    void SetMaterial(const OdDbObjectId &mtlId);
    OdDbObjectId GetMaterial();

    void ResetDevice(const CString &devName, bool bMaterial);

    bool SetDiagnosticBackground(bool bSet);

    void UpdatePreview();

    OdString findFile(const OdString& file, OdDbBaseDatabase* pDb,
                      OdDbBaseHostAppServices::FindFileHint hint);

    const OdGiContextForDbDatabase* giContext();
  protected:
    void PreparePreviewDatabase();
    void PreparePreviewDevice();
  protected:
	  DECLARE_MESSAGE_MAP()
  public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
};

/////////////////////////////////////////////////////////////////////////////
// CDbMaterialsDlg dialog

//Common tools
class OdTabCtrl : public CTabCtrl
{
public:
  OdTabCtrl();
  void addTabPage( const OdString& caption, CDialog* pDlg );
protected:
  OdArray< CDialog* > m_pages;
  OdUInt32 m_curPage;

  void adjustTabPageSize( CDialog* pDlg, bool bMakeVisible );
  void showTab( OdUInt32 nTab );

  afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
  DECLARE_MESSAGE_MAP()
};

class OdDialogLayoutManager
{
public:
  OdDialogLayoutManager();
  void initBySize( CWnd* pWnd, OdUInt32 width = 16, OdUInt32 height = 13, OdUInt32 offset = 4 );
  bool placeItem( CRect* pRect, OdUInt32 nRow, OdUInt32 nCol, OdUInt32 numRows = 1, OdUInt32 numCols = 1 );

  OdUInt32 numRows() const { return m_nRows; }
  OdUInt32 numCols() const { return m_nCols; }

protected:
  CWnd* m_pWnd;
  OdUInt32 m_nWidth;
  OdUInt32 m_nHeight;
  OdUInt32 m_nOffset;
  OdUInt32 m_nRows;
  OdUInt32 m_nCols;
};

//Tab page base class
class OdTabPage : public CDialog
{
public:
  enum 
  { 
    IDD = IDD_TABPAGE_DIALOG,
    MESSAGE_ID = WM_USER + 1
  };
  OdTabPage( CWnd* pParent = NULL );
  virtual ~OdTabPage();

  virtual void setupPage() = 0;
  virtual void enablePage( BOOL bEnable ) = 0;

protected:
  OdArray< CWnd* > m_items;
  void registerItem( CWnd*, bool bSetFont = true );
  void clearItems();

  int processScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
};

//Tab pages
class OdTabPage_Common : public OdTabPage
{
public:
  OdTabPage_Common( CWnd* pParent = NULL );
  void setupPage();
  void enablePage( BOOL bEnable );

  enum { DOUBLESIDED_CHECK = 1001 };

  bool getDoubleSided();
  void setDoubleSided( bool b );
  void setName( const OdString& str );
  void setDescription( const OdString& str );
protected:
  CStatic* m_nameWnd;
  CStatic* m_descrWnd;
  CButton* m_checkBtn;

  DECLARE_MESSAGE_MAP()
  afx_msg void OnDoubleSidedClicked();
};

class OdTabPage_Channel : public OdTabPage
{
public:
  enum Elements
  {
    kElementColor = 1 << 0,
    kElementSlider = 1 << 1,
    kElementTexturePreview = 1 << 2,
    kElementCheckbox = 1 << 3,
    //presets
    kElementColorPreviewCheckbox = kElementColor | kElementTexturePreview | kElementCheckbox,
    kElementSliderPreviewCheckbox = kElementSlider | kElementTexturePreview | kElementCheckbox,
    kElementAll = kElementColor | kElementSlider | kElementTexturePreview | kElementCheckbox
  };

  OdTabPage_Channel( CWnd* pParent = NULL );
  void setupPage();
  void enablePage( BOOL bEnable );

  void initPage( OdUInt8 elements, OdUInt8 nSliders = 1 );

  bool hasColor() const { return GETBIT( m_elements, kElementColor ); }
  bool hasSlider() const { return GETBIT( m_elements, kElementSlider ); }
  bool hasPreview() const { return GETBIT( m_elements, kElementTexturePreview ); }
  bool hasCheckbox() const { return GETBIT( m_elements, kElementCheckbox ); }

  OdUInt8 numSliders() const { return m_nSliders; }

  void setupColorElement( const OdString& caption, OdUInt16 message )
  {
    m_colorDescr.m_caption = caption;
    m_colorDescr.m_message = message;
  }

  void setupSliderElement( const OdString& str, OdUInt16 message, OdUInt8 index = 0 );

  void setupTexturePreview( OdUInt16 message )
  {
    m_texturePreviewDescr.m_message = message;
  }

  void setupCheckbox( const OdString& caption, OdUInt16 message )
  {
    m_checkboxDescr.m_caption = caption;
    m_checkboxDescr.m_message = message;
  }

  //data
  COLORREF getColor();
  void setColor( const OdCmEntityColor& cl );

  void setSliderValue( int value, OdUInt8 index = 0 );
  int getSliderValue( OdUInt8 index = 0 );

  void setTexturePreview( OdGiMaterialMap& matMap, const OdGiContext* pCtx );

  void setCheckboxChecked( bool b );
  bool isCheckboxChecked();

protected:
  OdUInt8 m_elements;
  OdUInt8 m_nSliders;

  enum Message
  {
    kMessageColorButtonPressed = 1008,
    kMessagePreviewButtonPressed = 1009,
    kMessageCheckboxPressed = 1010
  };

  struct ElementDescription
  {
    CStatic* m_pCaptionWnd;
    OdString m_caption;
    OdUInt16 m_message;
    CWnd* m_ownWnd;
    CWnd* m_extWnd;
    int m_curPos;
  };

  ElementDescription m_colorDescr;
  OdGiRasterImagePtr m_pCachedImage;

  ElementDescription m_texturePreviewDescr;

  ElementDescription m_checkboxDescr;

  OdArray< ElementDescription > m_sliders;

  DECLARE_MESSAGE_MAP()
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnColorButtonClicked();
  afx_msg void OnPreviewButtonClicked();
  afx_msg void OnCheckboxClicked();
};

class CTextureOptionsDialog : public CDialog
{
public:
  CTextureOptionsDialog( const OdGiMaterialMap& matMap, const OdGiContext* pCtx, CWnd* pParent = NULL );
  enum { IDD = IDD_TEXTURE_DIALOG };

  bool hasChanges() const { return m_bHasChanges; }
  OdGiMaterialMap getMap() { return m_map; }

protected:
  void SetDlgItemFloat(int nId, double nVal);
  double GetDlgItemFloat(int nId);

  void fillMatrix();
  OdGeMatrix3d readMatrix();

  BOOL updatePreview();

  void enableItems( BOOL bEnable );

  virtual BOOL OnInitDialog();

  OdGiMaterialMap m_map;
  bool m_bHasChanges;

  OdGiRasterImagePtr m_pImage;
  CPreviewGiRasterImage m_imgPreview;
  const OdGiContext* m_pCtx;

  DECLARE_MESSAGE_MAP()

  afx_msg void OnBnClickedApplyTransform();
  afx_msg void OnCbnSelchangeComboHTiling();
  afx_msg void OnCbnSelchangeComboVTiling();
  afx_msg void OnTexturePathClicked();
};

class CDbMaterialsDlg : public CDialog
{
// Construction
public:
	CDbMaterialsDlg(OdDbDatabase* pDb, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDbMaterialsDlg)
	enum { IDD = IDD_RENDER_DBMATERIALS };
  enum { TabCtrlIDD = 258 };
  enum 
  {
    MESSAGE_AMBIENTCOLOR_BTN = 1010,
    MESSAGE_DIFFUSECOLOR_BTN,
    MESSAGE_DIFFUSE_PREVIEW,
    MESSAGE_DIFFUSE_CHECK,
    MESSAGE_DIFFUSE_BLEND,
    MESSAGE_SPECULARCOLOR_BTN,
    MESSAGE_SPECULARCOLOR_GLOSS,
    MESSAGE_SPECULAR_PREVIEW,
    MESSAGE_SPECULAR_BLEND,
    MESSAGE_SPECULAR_CHECK,
    MESSAGE_OPACITY_OPACITY,
    MESSAGE_OPACITY_PREVIEW,
    MESSAGE_OPACITY_BLEND,
    MESSAGE_OPACITY_CHECK,
    MESSAGE_OPACITY_TRANSLUCENCE,
    MESSAGE_REFLECTION_REFLECTIVITY,
    MESSAGE_REFLECTION_PREVIEW,
    MESSAGE_REFLECTION_BLEND,
    MESSAGE_REFLECTION_CHECK,
    MESSAGE_REFRACTION_INDEX,
    MESSAGE_REFRACTION_BLEND,
    MESSAGE_REFRACTION_PREVIEW,
    MESSAGE_REFRACTION_CHECK,
    MESSAGE_BUMP_SCALE,
    MESSAGE_BUMP_BLEND,
    MESSAGE_BUMP_PREVIEW,
    MESSAGE_BUMP_CHECK,
    MESSAGE_NORMALMAP_STRENGTH,
    MESSAGE_NORMALMAP_BLEND,
    MESSAGE_NORMALMAP_PREVIEW,
    MESSAGE_NORMALMAP_CHECK
  };
	CListBox m_MaterialsList;
  CMaterialPreview m_Material;
  CComboBox m_Device;

  OdTabPage_Common m_commonPage;
  OdTabPage_Channel m_ambientPage;
  OdTabPage_Channel m_diffusePage;
  OdTabPage_Channel m_specularPage;
  OdTabPage_Channel m_opacityPage;
  OdTabPage_Channel m_reflectionPage;
  OdTabPage_Channel m_refractionPage;
  OdTabPage_Channel m_bumpPage;
  OdTabPage_Channel m_normalMapPage;
  OdTabCtrl  m_tabCtrl;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbMaterialsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDbMaterialsDlg)
	virtual BOOL OnInitDialog();
  virtual BOOL PreTranslateMessage( MSG* pMsg );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  void UpdateMaterial();
  void CheckChanges();
private:
  OdDbDatabase* m_pDb;
  bool m_bChanged;
public:
  afx_msg void OnLbnSelchangeLbmaterials();
  afx_msg void OnCbnSelchangeCbdevice();
  afx_msg void OnBnClickedCbdiagbkgnd();
  afx_msg void OnBnClickedBambient();
  afx_msg void OnBnClickedBdiffuse();
  afx_msg void OnBnClickedBspecular();
  afx_msg void OnSliderChanged( OdUInt32 id );
  afx_msg void OnEnableCheckboxClicked( OdUInt32 id );
  afx_msg void OnBnClickedCheck2();
  afx_msg void OnBnClickedExtractFbx();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnPreviewClicked( OdUInt32 id );
  virtual void OnOK();
};

/////////////////////////////////////////////////////////////////////////////
// COdCmEntityColorDlg dialog

class COdCmEntityColorDlg : public CDialog
{
  CButton m_bByLayer, m_bByBlock, m_bByColor, m_bByACI, m_bByPen, m_bForeground, m_bByDgnIndex, m_bNone;
  CCmEntityColorPreview m_cByColorPreview;
  CButton m_bChooseByColorColor;
  CBitmapPickerCombo m_cbByACIColor;
  OdCmEntityColor m_crColor;
  OdBitmapColorInfoArray m_aByACIColors;
public:
  COdCmEntityColorDlg(const OdCmEntityColor &crColor = OdCmEntityColor(0, 0, 0), CWnd* pParent = NULL);   // standard constructor
  virtual ~COdCmEntityColorDlg();

  // Dialog Data
  enum { IDD = IDD_RENDER_ODCMENTITYCOLOR };

  void SetColor(const OdCmEntityColor &crColor, bool invalidate = false);
  OdCmEntityColor GetColor() const;
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedColorMethod();
  afx_msg void OnDestroy();
  afx_msg void OnSelchangeComboColor();
  afx_msg void OnBnClickedSelColor();
};

// CVisualStylesPreview
class CVisualStylesPreview : public CStatic, public ExSystemServices, public ExHostAppServices
{
	DECLARE_DYNAMIC(CVisualStylesPreview)
  protected:
    OdDbDatabase *m_pHostDb;
    CDC *m_hDc;
    OdDbDatabasePtr m_pDb;
    OdGiContextForDbDatabasePtr m_pCtx;
    OdGsDevicePtr m_pDevice;
  public:
	  CVisualStylesPreview(OdDbDatabase *pHostDb);
	  virtual ~CVisualStylesPreview();

    void addRef() { }
    void release() { }
    ODRX_USING_HEAP_OPERATORS(ExSystemServices);

    void SetVisualStyle(const OdDbObjectId &vslId);
    OdDbObjectId GetVisualStyle();

    void UpdatePreview();

    OdString findFile(const OdString& file, OdDbBaseDatabase* pDb,
                      OdDbBaseHostAppServices::FindFileHint hint);
  protected:
    void PreparePreviewDatabase();
    void PreparePreviewDevice();
  protected:
	  DECLARE_MESSAGE_MAP()
  public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
};

/////////////////////////////////////////////////////////////////////////////
// CDbVisualStylesDlg dialog

class CDbVisualStylesDlg : public CDialog
{
// Construction
public:
  CDbVisualStylesDlg(OdDbDatabase* pDb, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDbVisualStylesDlg)
	enum { IDD = IDD_RENDER_DBVISUALSTYLES };
	CListBox m_VisualStylesList;
  CListCtrl m_PropsList;
  CVisualStylesPreview m_VisualStyle;
  CButton m_ShowInternalStyles;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbVisualStylesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDbVisualStylesDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  void UpdateVisualStyle();
private:
  OdDbDatabase* m_pDb;
public:
  afx_msg void OnLbnSelchangeLbvisualstyles();
  afx_msg void OnDblclkLbvisualstyles();
  afx_msg void OnClickListPropsList(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnShowInternalChanged();
  virtual void OnOK();
};

/////////////////////////////////////////////////////////////////////////////
// CDbRenderEnvironmentDlg dialog

class CDbRenderEnvironmentDlg : public CDialog
{
// Construction
public:
  CDbRenderEnvironmentDlg(OdDbDatabase* pDb, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDbRenderEnvironmentDlg)
	enum { IDD = IDD_RENDER_ENVIRONMENT };
  CPreviewGiRasterImage m_ctrlImagePreview;
  CCmEntityColorPreview m_ctrlFogColor;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbRenderEnvironmentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDbRenderEnvironmentDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  void SetDlgItemFloat(int nId, double nVal);
  double GetDlgItemFloat(int nId);

  void UpdateEnvImage();
private:
  OdDbDatabase* m_pDb;
public:
  virtual void OnOK();
  afx_msg void OnBnClickedEnvImageBrowse();
  afx_msg void OnBnClickedEnvFogColor();
  afx_msg void OnBnClickedEnvFogNearDist();
  afx_msg void OnBnClickedEnvFogFarDist();
  afx_msg void OnBnClickedEnvFogNearDens();
  afx_msg void OnBnClickedEnvFogFarDens();
};

/////////////////////////////////////////////////////////////////////////////
// Initialization routines
void rxInitMaterialsEditorObjects();
void rxUninitMaterialsEditorObjects();

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MATERIALSEDITOR_H__F10E7051_5C9B_43B8_BAC8_C31A8A7921AA__INCLUDED_)
