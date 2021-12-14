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
#include "DgECSchema.h"

// CItemTypeAttachDlg dialog

struct CItemTypeValueDescriptor
{
  OdDgECClassReference m_classRef;
  OdUInt32 m_uSubpropIndex;
  OdDgECPropertyInstance m_value;
};

class CItemTypeAttachDlg : public CDialog
{
	DECLARE_DYNAMIC(CItemTypeAttachDlg)

public:
	CItemTypeAttachDlg(CWnd* pParent, OdDgDatabase* pDb, const OdDgElementId& idElm );   // standard constructor
	virtual ~CItemTypeAttachDlg();

// Dialog Data
	enum { IDD = IDD_ATTACH_ITEMTYPES };

private:
  OdDgDatabase* m_pDb;
  OdDgElementId m_idElement;
  OdDgElementId m_idItemTypeLib;
  int           m_iItemType;
  int           m_iItemTypeLib;
  int           m_iPropIndex;
  int           m_iArrIndex;
  OdDgElementIdArray m_arrLibIds;
  OdArray<CItemTypeValueDescriptor> m_arrListDescriptor;
  CString       m_strSimpleVal;
  CString       m_strArrVal;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnItemLibComboSelchange();
  afx_msg void OnItemTypeComboSelchange();
  afx_msg void OnAttachBtnClicked();
  afx_msg void OnDetachBtnClicked();
  afx_msg void OnLibBtnClicked();
  afx_msg void OnArrAddBtnClicked();
  afx_msg void OnArrDelBtnClicked();
  afx_msg void OnEditBtnClicked();
  afx_msg void OnLvnItemchangedPropertiesList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnLvnItemchangedArrayList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnTimer( UINT_PTR nIDEvent );

private:
  void initListDescriptor();
  void initItemTypeLibraryCombo();
  void setItemTypeCombo();
  void updateItemTypeCtrls();
  void updatePropertyListData();
  void updatePropertyCtrls();
  void convertDescriptionsToInstances( OdDgDatabase* pDb, const OdArray<CItemTypeValueDescriptor>& arrDescriptors, OdArray<OdDgECClassInstancePtr>& arrInsts );
  void createDefaultInstance( const OdDgElementPtr& pLib, const OdDgItemTypePropertyPtr& pProp, OdDgECPropertyInstance& inst );

  CString createPropName(const OdString& strPropName, const OdDgECPropertyInstance& value, OdUInt32 uItemIndex );
  CString valueTypeToStr(const OdDgECPropertyInstance& value, OdUInt32 uItemIndex);
  CString valueDataToStr(const OdDgECPropertyInstance& value, OdUInt32 uItemIndex);
  void    valueDataArrToStrArr(const OdDgECPropertyInstance& value, OdUInt32 uItemIndex, OdArray<CString>& arrStrValues);
  bool    setItemPropertyValue( const CString& strValue, OdDgECPropertyInstance& value, OdUInt32 uSubPropertyIndex, int iArrayIndex );
  bool    delItemPropertyArrayValue( OdDgECPropertyInstance& value, OdUInt32 uSubPropertyIndex, int iArrayIndex );
  void    synchronizeComplexPropDesc( OdArray<CItemTypeValueDescriptor>& arrDescriptors, OdUInt32 iPropIndex );
};
