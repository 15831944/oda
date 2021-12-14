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

#if !defined(AFX_PSTBEDITFORMVIEWPG_H__0D67E57C_0ED0_4998_A9E8_A605D65325E7__INCLUDED_)
#define AFX_PSTBEDITFORMVIEWPG_H__0D67E57C_0ED0_4998_A9E8_A605D65325E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPsTbEditFormViewPage.h : header file
//

#include <vector>
using namespace std;

#include "BitmapPickerCombo.h"
#include "resource.h"
class OdPsPlotStyleTable;

#define  PS_COLOR_MAX_NAME        25
#define  PS_COMBO_COLOR_POSITION  8
#define  PS_SPIN_MAX_PEN          32
#define  PS_SPIN_MAX_VIRTPEN      255
#define  PS_SPIN_MAX_SCREENING    100

#define MMTOINCH(mm) (double(mm)  / kMmPerInch)
#define INCHTOMM(inch) (double(inch) * kMmPerInch)

  // plotstyle's linetype
static OdString StringLineType[] =
{
  _T("Solid"),
  _T("Dashed"),
  _T("Dotted"),
  _T("Dash Dot"),
  _T("Short Dash"),
  _T("Medium Dash"),
  _T("Long Dash"),
  _T("Short Dash X2"),
  _T("Medium Dash X2"),
  _T("Long Dash X2"),
  _T("Medium Long Dash"),
  _T("Medium Dash Short Dash Short Dash"),
  _T("Long Dash Short Dash"),
  _T("Long Dash Dot Dot"),
  _T("Long Dash Dot"),
  _T("Medium Dash Dot Short Dash Dot"),
  _T("Sparse Dot"),
  _T("ISO Dash"),
  _T("ISO Dash Space"),
  _T("ISO Long Dash Dot"),
  _T("ISO Long Dash Double Dot"),
  _T("ISO Long Dash Triple Dot"),
  _T("ISO Dot"),
  _T("ISO Long Dash Short Dash"),
  _T("ISO Long Dash Double Short Dash"),
  _T("ISO Dash Dot"),
  _T("ISO Double Dash Dot"),
  _T("ISO Dash Double Dot"),
  _T("ISO Double Dash Double Dot"),
  _T("ISO Dash Triple Dot"),
  _T("ISO Double Dash Triple Dot"),
  _T("Use object linetype")
  };


static OdString StringFillStyle[] =
  {
    _T("Solid"),
    _T("Checkerboard"),
    _T("Crosshatch"),
    _T("Diamonds"),
    _T("HorizontalBars"),
    _T("SlantLeft"),
    _T("SlantRight"),
    _T("SquareDots"),
    _T("VerticalBars"),
    _T("Use object fill style")
  };

static OdString StringLineEndStyle[] =
  {
    _T("Butt"),
    _T("Square"),
    _T("Round"),
    _T("Diamond"),
    _T("Use object end style")
  };
  
static OdString StringLineJoinStyle[] =
  {
    _T("Miter"),
    _T("Bevel"),
    _T("Round"),
    _T("Diamond"),
    _T("Use object join style")
  };


struct DIBCOLOR
{
    BYTE b;
    BYTE g;
    BYTE r;
    BYTE reserved;
    DIBCOLOR(BYTE ar,BYTE ag,BYTE ab):
    r(ar), g(ag), b(ab), reserved(0) {}
    DIBCOLOR(COLORREF color):
    r(GetRValue(color)), g(GetGValue(color)), b(GetBValue(color)), reserved(0) {}
    operator DWORD() {return *reinterpret_cast<DWORD*>(this);}
};

class CBitmapColorInfo
{
public:
  BYTE        m_iItem;
  COLORREF    m_color;
  CBitmap     m_bitmap;
  TCHAR       m_name[PS_COLOR_MAX_NAME];


  CBitmapColorInfo(const CBitmap *pBitmap, COLORREF color, const TCHAR* name);
  CBitmapColorInfo(const CBitmap *pBitmap, COLORREF color, BYTE cColorItem, int colorIndex = -1);
  CBitmapColorInfo(LPCTSTR lpszResourceName, const TCHAR* name);

// Implementation
protected:
  void SetBitmapPixels(CBitmap &Bmp, DIBCOLOR *pPixels);
  DIBCOLOR *GetBitmapPixels(CBitmap &Bmp, int &W, int &H);
  void GetBitmapSizes(CBitmap &Bmp, int &W, int &H);

public:
  CBitmap* CloneBitmap(const CBitmap* pBmpSource, CBitmap* pBmpClone);
  void PaintBitmap(CBitmap &Bmp, COLORREF color);
  const bool IsColor(COLORREF color, BYTE item);
  const OdCmEntityColor GetColor();

};

typedef OdArray<CBitmapColorInfo*> OdBitmapColorInfoArray;

class CPsListStyleData
{

  OdPsPlotStyle*          m_pPlotStyles;
  OdBitmapColorInfoArray* m_pPublicBitmapList;
  CBitmapColorInfo*       m_pBitmapColorInfo; 
  int                     m_iActiveListIndex;

protected:
  const int getPublicArrayIndexByColor(COLORREF color);

public:
  CPsListStyleData(OdPsPlotStyle* pPs, OdBitmapColorInfoArray* pPublicBitmapList, const char item);
  
  ~CPsListStyleData();

  OdPsPlotStyle* GetOdPsPlotStyle() const { return m_pPlotStyles; };
  CBitmapColorInfo*  GetBitmapColorInfo() const { return m_pBitmapColorInfo; };
  const int GetActiveListIndex() const { return m_iActiveListIndex; };
  const bool ReplaceBitmapColorInfo(COLORREF color, const int item);
  const bool SetActiveListIndex(const int index, const bool bBmpInfo = false);
  const OdCmEntityColor GetColor();
  OdPsPlotStyle* GetOdPsPlotStyle(){ return m_pPlotStyles; };
};

/////////////////////////////////////////////////////////////////////////////
// CPsTbEditFormViewPage dialog

class CPsTbEditFormViewPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPsTbEditFormViewPage)

	void mtHideHelpBtn();

  CImageList               m_imageList;
  OdPsPlotStyleTable*      m_pPlotStyleTable;
  OdPsPlotStyle*           m_pPlotStyleActive;
  OdBitmapColorInfoArray   m_bitmapList;
  OdString                 m_sFileBufPath;
  bool                     m_bEditChanging;

// Construction
public:
	CPsTbEditFormViewPage();
	~CPsTbEditFormViewPage();

// Dialog Data
	//{{AFX_DATA(CPsTbEditFormViewPage)
	enum { IDD = IDD_PS_DLG_FORMVIEW };
  	CComboBox	           m_comboDither;
  	CComboBox	           m_comboGrayscale;
    CComboBox            m_comboLinetype;
    CComboBox            m_comboLineweight;
    CComboBox            m_comboLineendstyle;
    CComboBox            m_comboLinejoinstyle;
    CComboBox            m_comboFillstyle;
    CBitmapPickerCombo   m_comboColor;
  	CEdit	               m_editDescription;
    CEdit                m_editPen;
    CEdit                m_editVirtpen;
    CEdit                m_editScreening;
    CComboBox            m_comboAdaptive;
    CSpinButtonCtrl      m_spinPen;
    CSpinButtonCtrl      m_spinVirtpen;
    CSpinButtonCtrl      m_spinScreening;
    CListCtrl	           m_listStyles;
    CButton              m_btnAddstyle;
    CButton              m_btnDelstyle;
    CButton              m_btnLineweight;
    CButton              m_btnSave;

		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPsTbEditFormViewPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  void initBitmapList();
  void initAdaptiveComboBox();
  void initGrayscaleComboBox();
  void initDitherComboBox();
  void initLinetypeComboBox();
  void initLineweightComboBox();
  void initLineendstyleComboBox();
  void initLinejoinstyleComboBox();
  void initFillstyleComboBox();
  void initColorComboBox();
  void initListCtrl();
  const int insertItem(int index);
  HICON initColorIcon(int width,int height, COLORREF color);
  void initImageList();

  const int deleteCustomColor();
  const int appendCustomColor(const int item);
  const int replaceCustomColor(COLORREF color, const int item);

public:
  const bool SetPlotStyleTable(OdPsPlotStyleTable* pPlotStyleTable);
  BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags);
  void SetFileBufPath(const OdString sFilePath);
  void AddNewPlotStyle(LPCTSTR lpStyleName);
  const OdPsPlotStyleTable *GetPlotStyleTable() const { return m_pPlotStyleTable; };

protected:
	// Generated message map functions
	//{{AFX_MSG(CPsTbEditFormViewPage)
		// NOTE: the ClassWizard will add member functions here
      afx_msg void OnLineweightBtn();
      afx_msg void OnAddBtnStyle();
      afx_msg void OnSaveBtn();
    	afx_msg void OnDelBtnStyle();
      afx_msg void OnUpdateEditDescription();
      afx_msg void OnChangeEditDescription();
    	afx_msg void OnChangeEditPen();
    	afx_msg void OnChangeEditVirtPen();
      afx_msg void OnChangeEditScreening();
    	virtual BOOL OnInitDialog();
    	afx_msg void OnItemchangedListStyles(NMHDR* pNMHDR, LRESULT* pResult);
    	afx_msg void OnItemchangingListStyles(NMHDR* pNMHDR, LRESULT* pResult);
    	afx_msg void OnDeltaposSpinPen(NMHDR* pNMHDR, LRESULT* pResult);
    	afx_msg void OnSelchangeComboColor();
    	afx_msg void OnSelendokComboColor();
      afx_msg void OnSelendokComboDither();
      afx_msg void OnSelendokComboGrayScale();
      afx_msg void OnSelendokComboLineType();
      afx_msg void OnSelendokComboAdaptive();
      afx_msg void OnSelendokComboLineWeight();
      afx_msg void OnSelendokComboLineEndStyle();
      afx_msg void OnSelendokComboLineJoinStyle();
      afx_msg void OnSelendokComboFillStyle();
      afx_msg void OnDestroy( );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSTBEDITFORMVIEWPG_H__0D67E57C_0ED0_4998_A9E8_A605D65325E7__INCLUDED_)
