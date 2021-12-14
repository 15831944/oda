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
#include "ExDgnHostAppServices.h"
#include "ExDgnServices.h"
#include "DgGiContext.h"
#include "SSet.h"
#include "DgElementId.h"
#include "DgElementIdArray.h"
#include "Gs/Gs.h"
#include "DgUserIO.h"
#include "DgAttributeLinkage.h"

 //CCmEntityColorPreview
class CCmEntityColorPreview : public CStatic
{
	DECLARE_DYNAMIC(CCmEntityColorPreview)
  protected:
    OdCmEntityColor m_color;
  public:
	  CCmEntityColorPreview();
	  virtual ~CCmEntityColorPreview();

    void SetColor(const OdCmEntityColor color);
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
	CDbLightsDlg(OdDgDatabase* pDb, CWnd* pParent = NULL);   // standard constructor

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
  OdDgDatabase* m_pDb;
public:
  afx_msg void OnLvnItemchangedLightsList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedCbon();
  afx_msg void OnEnChangeEintensity();
  afx_msg void OnBnClickedBcolor();
};

class CMaterialPreview : public CStatic, public OdExDgnSystemServices, public OdExDgnHostAppServices
{
	DECLARE_DYNAMIC(CMaterialPreview)
  protected:
    OdDgDatabase *m_pHostDb;
    CDC *m_hDc;
    OdDgDatabasePtr m_pDb;
	OdGiContextForDgDatabasePtr m_pCtx;
    OdGsDevicePtr m_pDevice;
    CString m_devName;
    bool m_bDiagBkgnd;
  public:
	  CMaterialPreview(OdDgDatabase *pHostDb);
	  virtual ~CMaterialPreview();

    void addRef() { }
    void release() { }
    ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);

	void SetMaterial(const OdDgElementId &mtlId, CString sMaterialString, OdDgDatabasePtr pDb);
	void recountVeiws(std::vector<OdDgElementId> &pViews, OdDgDatabasePtr pDb);
	OdDgElementId GetMaterial();

    void ResetDevice(const CString &devName, bool bMaterial);

    void UpdatePreview();

    OdString findFile(const OdString& file, OdDbBaseDatabase* pDb,
                      OdDgHostAppServices::FindFileHint hint);
  protected:
    void PreparePreviewDatabase( OdDgHostAppServices* pAppServices );
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

class CDbMaterialsDlg : public CDialog
{
// Construction
public:
	CDbMaterialsDlg(OdDgDatabasePtr pDb, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDbLightsDlg)
	enum { IDD = IDD_RENDER_DBMATERIALS };
	CListBox m_MaterialsList;
	OdUInt16 m_nItem;
  CMaterialPreview m_Material;
  CCmEntityColorPreview m_Diffuse;
  CCmEntityColorPreview m_Specular;
  CCmEntityColorPreview m_BrightnessColor;
  CComboBox m_Device;
  CSliderCtrl m_Gloss;

  CSliderCtrl m_Opacity;
  //CSliderCtrl m_SelfIllum;
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  void UpdateMaterial();
  void CheckChanges();
private:
  OdDgDatabase* m_pDb;
  bool m_bChanged;
public:
  afx_msg void OnLbnSelchangeLbmaterials();
  afx_msg void OnCbnSelchangeCbdevice();
  afx_msg void OnBnClickedBdiffuse();
  afx_msg void OnBnClickedBspecular();
  afx_msg void OnBnClickedCheck2();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnBnClickedOk();
};

/////////////////////////////////////////////////////////////////////////////
// Initialization routines
void rxInitMaterialsEditorObjects();
void rxUninitMaterialsEditorObjects();

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MATERIALSEDITOR_H__F10E7051_5C9B_43B8_BAC8_C31A8A7921AA__INCLUDED_)
