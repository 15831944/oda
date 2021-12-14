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
#pragma once

#include "DgDatabase.h"
#include "DgItemTypeLibrary.h"

// CItemTypeLibsDlg dialog

class CItemTypeLibsDlg : public CDialog
{
	DECLARE_DYNAMIC(CItemTypeLibsDlg)

public:
	CItemTypeLibsDlg(CWnd* pParent, OdDgDatabase* pDb );   // standard constructor
	virtual ~CItemTypeLibsDlg();

// Dialog Data
	enum { IDD = IDD_ITEMTYPE_LIBS };

private:
  OdDgDatabase* m_pDb;
  int           m_iItemTypeLib;
  CString       m_strItemTypeLibName;
  CString       m_strItemTypeName;
  CString       m_strPropertyTypeName;
  CString       m_strPropName;
  int           m_iItemType;
  CString       m_strPropType;
  int           m_iPropIndex;
  CString       m_strPropValue;
  int           m_bArray;
  OdDgElementId m_idItemTypeLib;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLvnItemchangedPropertyList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnLvnItemchangedItemTypeList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnItemLibComboSelchange();
  afx_msg void OnEditPropName();
  afx_msg void OnEditItemOrPropName();
  afx_msg void OnEditLibName();
  afx_msg void OnAddProperty();
  afx_msg void OnModifyProperty();
  afx_msg void OnDeleteProperty();
  afx_msg void OnAddItemType();
  afx_msg void OnAddPropertyType();
  afx_msg void OnDeleteItemType();
  afx_msg void OnAddLibrary();
  afx_msg void OnDeleteLibrary();
  afx_msg void OnTimer( UINT_PTR nIDEvent );

  static OdString getPartOfDate( OdString& strValue );

private:
  void updateLibrariesCombo();
  void updateLibrariesCtrls();
  void updateItemTypesList();
  void updateItemTypesCtrls();
  void updatePropertiesList();
  void updatePropertyCtrls();
  void setPropertyTypesComboValues();
  OdDgItemTypePropertyPtr curProperty();
  CString propertyTypeToStr( const OdDgItemTypePropertyPtr& pProp );
  CString propertyValueToStr( const OdDgItemTypePropertyValue& value );
  void setItemPropertyType( const CString& strTypeName, OdDgItemTypePropertyPtr& pProperty );
  void setItemPropertyDefaultValue( const CString& strValue, OdDgItemTypePropertyPtr& pProperty );
};
