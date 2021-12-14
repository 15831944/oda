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

// HoopsViewer.h : main header file for the HoopsViewer application
//

#if !defined( _HOOPSVIEWER_INCLUDED)
#define _HOOPSVIEWER_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "RegistryCommon.h"
#include "hoops_stream/source/HStream.h"
#include "hoops_3dgs/source/hc.h"
#include "OdArray.h"

// some structures/macros we use 

struct HPoint
{
  float x; 
  float y; 
  float z;
};

#define streq(a,b)      (!strcmp(a,b))

#define ID_CHANGE_BG_COLOR WM_USER + 1


// CHoopsViewerApp:
// See HoopsViewer.cpp for the implementation of this class
//

class CHoopsViewerApp : public CWinApp
{
public:
	CHoopsViewerApp();
  const char	*GetDriverType()	{return m_sDriverType;};
  int	IncrementCounter() {return m_iCounter++;};
  int	DecrementCounter() {return m_iCounter--;};
  void InitSettingsFromRegistry();
  void AddACADDefaultPaths();
  void SaveSettingsToRegistry();

// Overrides
public:
	virtual BOOL InitInstance();
  virtual int ExitInstance();

// Variables
public:
  ODCOLORREF           m_clrBgColor;
  bool                 m_bFirstRun;
  bool                 m_bShowAxis;
  bool                 m_bImportAllLayouts;
  OdArray<OdString>    m_strFontDirArr;

private:
  char	m_sDriverType[16];
  int		m_iCounter;

// Implementation
  afx_msg void OnAppAbout();
  afx_msg void OnSetDirectories();
  afx_msg void OnBgColor();
  afx_msg void OnLoadActiveLayout();
  afx_msg void OnUpdateLoadActiveLayout(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

extern CHoopsViewerApp theApp;

#endif
