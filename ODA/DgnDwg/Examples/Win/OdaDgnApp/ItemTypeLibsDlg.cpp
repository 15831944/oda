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
// ViewFlagsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "ItemTypeLibsDlg.h"
#include "DgTable.h"
#include "DgItemTypeLibrary.h"
#include "OdSetLocale.h"

// CItemTypeLibsDlg dialog

#define UPDATE_PROPERTY_TIMER 156
#define UPDATE_ITEMTYPE_TIMER 157

IMPLEMENT_DYNAMIC(CItemTypeLibsDlg, CDialog)

CItemTypeLibsDlg::CItemTypeLibsDlg(CWnd* pParent, OdDgDatabase* pDb)
	: CDialog(CItemTypeLibsDlg::IDD, pParent)
{
  m_pDb = pDb;
  m_iItemType = -1;
  m_iPropIndex = -1;
  m_bArray = false;
  m_idItemTypeLib = NULL;
}

//=================================================================================

CItemTypeLibsDlg::~CItemTypeLibsDlg()
{
}

//=================================================================================

void CItemTypeLibsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_CBIndex( pDX, IDC_ITEM_LIB_COMBO, m_iItemTypeLib );
  DDX_Text( pDX, IDC_LIB_NAME_EDIT, m_strItemTypeLibName );
  DDX_Text( pDX, IDC_ITEM_TYPE_NAME_EDIT, m_strItemTypeName );
  DDX_Text( pDX, IDC_PROP_TYPE_NAME_EDIT, m_strPropertyTypeName );
  DDX_Text( pDX,  IDC_PROP_TYPE_COMBO, m_strPropType);
  DDX_Text( pDX,  IDC_PROP_NAME_EDIT, m_strPropName);
  DDX_Text( pDX,  IDC_VALUE_EDIT, m_strPropValue);
  DDX_Check(pDX, IDC_ARRAY_CHECK, m_bArray);
}

//=================================================================================

BEGIN_MESSAGE_MAP(CItemTypeLibsDlg, CDialog)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_PROPERTY_LIST, CItemTypeLibsDlg::OnLvnItemchangedPropertyList)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEM_TYPE_LIST, CItemTypeLibsDlg::OnLvnItemchangedItemTypeList)
  ON_CBN_SELCHANGE(IDC_ITEM_LIB_COMBO, CItemTypeLibsDlg::OnItemLibComboSelchange )
  ON_EN_CHANGE(IDC_PROP_NAME_EDIT, CItemTypeLibsDlg::OnEditPropName )
  ON_EN_CHANGE(IDC_ITEM_TYPE_NAME_EDIT, CItemTypeLibsDlg::OnEditItemOrPropName )
  ON_EN_CHANGE(IDC_PROP_TYPE_NAME_EDIT, CItemTypeLibsDlg::OnEditItemOrPropName )
  ON_EN_CHANGE(IDC_LIB_NAME_EDIT, CItemTypeLibsDlg::OnEditLibName )
  ON_BN_CLICKED(IDC_ADD_PROP_BUTTON, CItemTypeLibsDlg::OnAddProperty )
  ON_BN_CLICKED(IDC_EDIT_PROP_BUTTON, CItemTypeLibsDlg::OnModifyProperty )
  ON_BN_CLICKED(IDC_DEL_PROP_BUTTON, CItemTypeLibsDlg::OnDeleteProperty )
  ON_BN_CLICKED(IDC_ADD_ITEM_TYPE_BUTTON, CItemTypeLibsDlg::OnAddItemType )
  ON_BN_CLICKED(IDC_ADD_PROP_TYPE_BUTTON, CItemTypeLibsDlg::OnAddPropertyType )
  ON_BN_CLICKED(IDC_DEL_ITEM_TYPE_BUTTON, CItemTypeLibsDlg::OnDeleteItemType )
  ON_BN_CLICKED(IDC_ADDLIB_BUTTON, CItemTypeLibsDlg::OnAddLibrary )
  ON_BN_CLICKED(IDC_DEL_LIB_BUTTON, CItemTypeLibsDlg::OnDeleteLibrary )
  ON_WM_TIMER()
END_MESSAGE_MAP()

//=================================================================================

void CItemTypeLibsDlg::updateLibrariesCtrls()
{
  UpdateData(TRUE);
  CWnd* pDelLibBtn = GetDlgItem(IDC_DEL_LIB_BUTTON);
  CWnd* pAddLibBtn = GetDlgItem(IDC_ADDLIB_BUTTON);
  CComboBox* pLibCombo = (CComboBox*)(GetDlgItem(IDC_ITEM_LIB_COMBO));

  bool bEnableAdd = true;

  if( m_strItemTypeLibName.IsEmpty() )
    bEnableAdd = false;
  else
  {
    int iNum = pLibCombo->FindString(0,m_strItemTypeLibName);

    if( iNum >= 0 )
    {
      CString strText;
      pLibCombo->GetLBText(iNum, strText);
      bEnableAdd = strText != m_strItemTypeLibName;
    }
  }

  pDelLibBtn->EnableWindow( m_iItemTypeLib >= 0 );
  pAddLibBtn->EnableWindow( bEnableAdd );
}

//=================================================================================

void CItemTypeLibsDlg::updateLibrariesCombo()
{
  UpdateData(TRUE);
  CString strActiveLibraryName;

  CComboBox* pLibCombo = (CComboBox*)(GetDlgItem(IDC_ITEM_LIB_COMBO));

  if( m_iItemTypeLib != -1 )
    pLibCombo->GetLBText(m_iItemTypeLib,strActiveLibraryName);

  while( pLibCombo->GetCount() )
    pLibCombo->DeleteString(0);

  OdDgElementIdArray arrIds;

  if( m_pDb )
  {
    OdDgTagDefinitionSetTablePtr pTagSetTable = m_pDb->getTagDefinitionSetTable(OdDg::kForRead);

    if( !pTagSetTable.isNull() )
    {
      OdDgElementIteratorPtr pIter = pTagSetTable->createIterator();

      for(; !pIter->done(); pIter->step() )
      {
        OdDgElementPtr pElm = pIter->item().openObject(OdDg::kForRead);

        if( pElm->isKindOf(OdDgItemTypeLibrary::desc()) && !pElm->isKindOf(OdDgXMLSchemaContainer::desc()) )
        {
          OdDgItemTypeLibraryPtr pItemTypeLib = pElm;
          pLibCombo->InsertString(pLibCombo->GetCount(), convertECSchemaItemNameToString(pItemTypeLib->getName()) );
          arrIds.push_back(pElm->elementId());
        }
      }
    }
  }

  if( !strActiveLibraryName.IsEmpty() )
    m_iItemTypeLib = pLibCombo->FindString(0, strActiveLibraryName );

  if( m_iItemTypeLib < 0 && pLibCombo->GetCount() > 0 )
    m_iItemTypeLib = 0;

  if( m_iItemTypeLib >= 0 )
  {
    pLibCombo->GetLBText(m_iItemTypeLib,strActiveLibraryName);
    m_strItemTypeLibName = strActiveLibraryName;
    m_idItemTypeLib = arrIds[m_iItemTypeLib];
  }
  else
    m_idItemTypeLib = NULL;

  UpdateData(FALSE);

  updateLibrariesCtrls();
}

//=================================================================================

void CItemTypeLibsDlg::updateItemTypesCtrls()
{
  UpdateData(TRUE);
  CListCtrl* pItemTypesList = (CListCtrl*)GetDlgItem(IDC_ITEM_TYPE_LIST);

  if( m_iItemType >= 0 )
  {
    if( m_iItemType < pItemTypesList->GetItemCount() )
    {
      CString strItemName   = m_strItemTypeName;
      CString strItemNameEx = L"-[" + m_strItemTypeName + L"]-";
      CString strPropName   = m_strPropertyTypeName;
      CString strPropNameEx = L"-[" + m_strPropertyTypeName + L"]-";

      if( strPropName.IsEmpty() )
        strPropNameEx.Empty();

      if( strItemName.IsEmpty() )
        strItemNameEx.Empty();

      if( strItemName.IsEmpty() )
      {
        GetDlgItem(IDC_ADD_ITEM_TYPE_BUTTON)->EnableWindow(FALSE);
      }
      else
      {
        bool bFindStr = false;
        LVFINDINFO fndInfo;
        fndInfo.flags = LVFI_STRING;
        fndInfo.psz = strItemName.GetBuffer(strItemName.GetLength());
        bFindStr = pItemTypesList->FindItem( &fndInfo ) != -1;

        if( !bFindStr )
        {
          fndInfo.flags = LVFI_STRING;
          fndInfo.psz = strItemNameEx.GetBuffer(strItemNameEx.GetLength());
          bFindStr = pItemTypesList->FindItem( &fndInfo ) != -1;
        }

        GetDlgItem(IDC_ADD_ITEM_TYPE_BUTTON)->EnableWindow(!bFindStr);
        strItemName.ReleaseBuffer();
        strItemNameEx.ReleaseBuffer();
      }

      if( strPropName.IsEmpty() )
      {
        GetDlgItem(IDC_ADD_PROP_TYPE_BUTTON)->EnableWindow(FALSE);
      }
      else
      {
        bool bFindStr = false;
        LVFINDINFO fndInfo;
        fndInfo.flags = LVFI_STRING;
        fndInfo.psz = strPropName.GetBuffer(strPropName.GetLength());
        bFindStr = pItemTypesList->FindItem( &fndInfo ) != -1;

        if( !bFindStr )
        {
          fndInfo.flags = LVFI_STRING;
          fndInfo.psz = strPropNameEx.GetBuffer(strPropNameEx.GetLength());
          bFindStr = pItemTypesList->FindItem( &fndInfo ) != -1;
        }

        GetDlgItem(IDC_ADD_PROP_TYPE_BUTTON)->EnableWindow(!bFindStr);
        strItemName.ReleaseBuffer();
        strItemNameEx.ReleaseBuffer();
      }

      GetDlgItem(IDC_DEL_ITEM_TYPE_BUTTON)->EnableWindow(m_iItemType >= 0);
    }
    else
    {
      m_iItemType = pItemTypesList->GetItemCount() > 0 ? 0 : 1;
    }
  }
  else
  {
    GetDlgItem(IDC_DEL_ITEM_TYPE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_ADD_ITEM_TYPE_BUTTON)->EnableWindow(!m_strItemTypeName.IsEmpty());
    GetDlgItem(IDC_ADD_PROP_TYPE_BUTTON)->EnableWindow(!m_strPropertyTypeName.IsEmpty());
  }

  UpdateData(FALSE);
}

//=================================================================================

void CItemTypeLibsDlg::updateItemTypesList()
{
  UpdateData(TRUE);

  bool bEnableCtrls = false;

  if( m_iItemTypeLib != -1 )
  {
    CListCtrl* pItemTypesList = (CListCtrl*)GetDlgItem(IDC_ITEM_TYPE_LIST);
    CComboBox* pLibCombo = (CComboBox*)(GetDlgItem(IDC_ITEM_LIB_COMBO));

    CString strCurItem;

    if( m_iItemType >= 0 )
      strCurItem = pItemTypesList->GetItemText(m_iItemType,0);

    pItemTypesList->DeleteAllItems();

    CString strActiveLibraryName;
    pLibCombo->GetLBText(m_iItemTypeLib,strActiveLibraryName);
    OdString strFullActiveLibName = convertStringToECSchemaItemName(strActiveLibraryName.GetBuffer(strActiveLibraryName.GetLength()));
    strActiveLibraryName.ReleaseBuffer();

    if( m_pDb && !m_idItemTypeLib.isNull() )
    {
      OdDgItemTypeLibraryPtr pItemTypeLib = m_idItemTypeLib.openObject(OdDg::kForRead, true);

      if( !pItemTypeLib.isNull() )
      {
        for( int i = 0; i < (int)pItemTypeLib->getItemTypeCount(); i++ )
        {
          OdDgItemTypePtr pItemType = pItemTypeLib->getItemType(i);
          CString strText = convertECSchemaItemNameToString(pItemType->getName()).c_str();

          if( pItemType->getCategory() == OdDgItemType::kPropertyType )
            strText = L"-[" + strText + L"]-";

          pItemTypesList->InsertItem(i, strText);
        }

        if( !strCurItem.IsEmpty() )
        {
          LVFINDINFO fndInfo;
          fndInfo.flags = LVFI_STRING;
          fndInfo.psz = strCurItem.GetBuffer(strCurItem.GetLength());
          m_iItemType = pItemTypesList->FindItem( &fndInfo );
          strCurItem.ReleaseBuffer();
        }

        if( pItemTypesList->GetItemCount() > 0 && m_iItemType < 0 )
          m_iItemType = 0;
        else if( pItemTypesList->GetItemCount() == 0 )
          m_iItemType = -1;

        bEnableCtrls = true;
      }
    }

    if( m_iItemType >= 0 )
    {
      pItemTypesList->SetSelectionMark(m_iItemType);
      pItemTypesList->SetItemState(m_iItemType,LVIS_SELECTED,(UINT)-1);

      CString strText = pItemTypesList->GetItemText(m_iItemType, 0);

      if( strText.GetLength() > 2 && strText.Left(2) == L"-[" )
      {
        m_strPropertyTypeName = strText.Mid(2, strText.GetLength()-4);
        m_strItemTypeName.Empty();
      }
      else
      {
        m_strPropertyTypeName.Empty();
        m_strItemTypeName = strText;
      }
    }
  }

  if( m_iItemType < 0 )
  {
    m_strPropertyTypeName.Empty();
    m_strItemTypeName.Empty();
  }

  GetDlgItem(IDC_ITEM_TYPE_LIST)->EnableWindow(bEnableCtrls);
  GetDlgItem(IDC_ITEM_TYPE_NAME_EDIT)->EnableWindow(bEnableCtrls);
  GetDlgItem(IDC_PROP_TYPE_NAME_EDIT)->EnableWindow(bEnableCtrls);
  GetDlgItem(IDC_ADD_ITEM_TYPE_BUTTON)->EnableWindow(bEnableCtrls);
  GetDlgItem(IDC_ADD_PROP_TYPE_BUTTON)->EnableWindow(bEnableCtrls);
  GetDlgItem(IDC_DEL_ITEM_TYPE_BUTTON)->EnableWindow(bEnableCtrls);

  UpdateData(FALSE);

  updateItemTypesCtrls();
}

//=================================================================================

OdDgItemTypePropertyPtr CItemTypeLibsDlg::curProperty()
{
  OdDgItemTypePropertyPtr retVal;

  if( !m_idItemTypeLib.isNull() && m_iItemType >= 0 && m_iPropIndex >= 0 )
  {
    OdDgItemTypeLibraryPtr pLibrary = m_idItemTypeLib.openObject(OdDg::kForRead, true);

    if( !pLibrary.isNull() && pLibrary->getItemTypeCount() > (OdUInt32)m_iItemType )
    {
      OdDgItemTypePtr pItemType = pLibrary->getItemType(m_iItemType);

      if( !pItemType.isNull() && pItemType->getPropertyCount() > (OdUInt32)m_iPropIndex )
      {
        retVal = pItemType->getProperty(m_iPropIndex);
      }
    }
  }

  return retVal;
}

//=================================================================================

CString CItemTypeLibsDlg::propertyTypeToStr( const OdDgItemTypePropertyPtr& pProp )
{
  CString strRet = L"";

  switch( pProp->getType() )
  {
    case OdDgItemTypeProperty::kString:   strRet = L"kString"; break;
    case OdDgItemTypeProperty::kDouble:   strRet = L"kDouble"; break;
    case OdDgItemTypeProperty::kInteger:  strRet = L"kInteger"; break;
    case OdDgItemTypeProperty::kBoolean:  strRet = L"kBoolean"; break;
    case OdDgItemTypeProperty::kDateTime: strRet = L"kDateTime"; break;
    case OdDgItemTypeProperty::kPoint3d:  strRet = L"kPoint3d"; break;
    case OdDgItemTypeProperty::kCustom:   strRet = convertECSchemaItemNameToString(pProp->getTypeName()).c_str(); break;
  }

  return strRet;
}

//=================================================================================

CString CItemTypeLibsDlg::propertyValueToStr( const OdDgItemTypePropertyValue& value )
{
  CString strRet = L"";

  switch( value.getPropertyType() )
  {
    case OdDgItemTypeProperty::kString:   strRet = value.getStringValue().c_str(); break;
    case OdDgItemTypeProperty::kDouble:   strRet.Format(L"%f",value.getDoubleValue()); break;
    case OdDgItemTypeProperty::kInteger:  strRet.Format(L"%d",value.getIntegerValue()); break;
    case OdDgItemTypeProperty::kBoolean:  strRet = value.getBooleanValue() ? L"true" : L"false"; break;
    case OdDgItemTypeProperty::kDateTime: 
    {
      OdString strTime;
      value.getDateTimeValue().strftime(L"%d.%m.%Y %H:%M:%S", strTime);
      strRet = strTime.c_str();
    } break;
    case OdDgItemTypeProperty::kPoint3d:
    {
      OdGePoint3d pt3d = value.getPointValue();
      strRet.Format(L"(%f,%f,%f)", pt3d.x, pt3d.y, pt3d.z );
    } break;
  }

  return strRet;
}

//=================================================================================

void CItemTypeLibsDlg::updatePropertiesList()
{
  bool bEnableCtrls = false;

  CListCtrl* pPropList = (CListCtrl*)GetDlgItem(IDC_PROPERTY_LIST);

  CString strPropName = L"";

  if( m_iPropIndex >= 0 && m_iPropIndex < pPropList->GetItemCount() )
    strPropName = pPropList->GetItemText(m_iPropIndex, 0);

  pPropList->DeleteAllItems();

  if( !m_idItemTypeLib.isNull() && m_iItemType >= 0 )
  {
    OdDgItemTypeLibraryPtr pLibrary = m_idItemTypeLib.openObject(OdDg::kForRead, true);

    if( !pLibrary.isNull() && pLibrary->getItemTypeCount() > (OdUInt32)m_iItemType )
    {
      OdDgItemTypePtr pItemType = pLibrary->getItemType(m_iItemType);
      
      for( int i = 0; i < (int)pItemType->getPropertyCount(); i++ )
      {
        OdDgItemTypePropertyPtr pProperty = pItemType->getProperty(i);
        pPropList->InsertItem(i, convertECSchemaItemNameToString(pProperty->getName()) );
        pPropList->SetItemText(i, 1, propertyTypeToStr(pProperty) );
        pPropList->SetItemText(i, 2, propertyValueToStr(pProperty->getDefaultValue()) );
      }

      if( !strPropName.IsEmpty() )
      {
        LVFINDINFO fndInfo;
        fndInfo.flags = LVFI_STRING;
        fndInfo.psz = strPropName.GetBuffer(strPropName.GetLength());
        m_iPropIndex = pPropList->FindItem( &fndInfo );
        strPropName.ReleaseBuffer();
      }
      else
        m_iPropIndex = -1;

      if( m_iPropIndex == -1 && pPropList->GetItemCount() > 0 )
        m_iPropIndex = 0;
      else if( pPropList->GetItemCount() == 0 )
        m_iPropIndex = -1;

      if( m_iPropIndex >= 0 )
      {
        pPropList->SetSelectionMark(m_iPropIndex);
        pPropList->SetItemState(m_iPropIndex,LVIS_SELECTED,(UINT)-1);

        OdDgItemTypePropertyPtr pProp = curProperty();

        m_strPropName  = convertECSchemaItemNameToString(pProp->getName()).c_str();
        m_strPropType  = propertyTypeToStr(pProp);
        m_strPropValue = propertyValueToStr(pProp->getDefaultValue());
        m_bArray       = pProp->getArrayFlag() ? 1 : 0;

        CComboBox* pTypesCombo    = (CComboBox*)GetDlgItem(IDC_PROP_TYPE_COMBO);
        pTypesCombo->SetCurSel( pTypesCombo->FindString(0,m_strPropType));
      }

      bEnableCtrls = true;
    }
  }
  else
    m_iPropIndex = -1;

  if( m_iPropIndex < 0 )
  {
    m_strPropName.Empty();
    m_strPropType.Empty();
    m_strPropValue.Empty();
    m_bArray = FALSE;
    CComboBox* pTypesCombo    = (CComboBox*)GetDlgItem(IDC_PROP_TYPE_COMBO);
    pTypesCombo->SetCurSel(-1);
  }

  GetDlgItem(IDC_PROPERTY_LIST)->EnableWindow(bEnableCtrls);
  GetDlgItem(IDC_PROP_NAME_EDIT)->EnableWindow(bEnableCtrls);
  GetDlgItem(IDC_PROP_TYPE_COMBO)->EnableWindow(bEnableCtrls);
  GetDlgItem(IDC_VALUE_EDIT)->EnableWindow(bEnableCtrls);
  GetDlgItem(IDC_ARRAY_CHECK)->EnableWindow(bEnableCtrls);
  GetDlgItem(IDC_ADD_PROP_BUTTON)->EnableWindow(bEnableCtrls);
  GetDlgItem(IDC_DEL_PROP_BUTTON)->EnableWindow(bEnableCtrls);
  GetDlgItem(IDC_EDIT_PROP_BUTTON)->EnableWindow(bEnableCtrls);

  UpdateData(FALSE);

  updatePropertyCtrls();
}

//=================================================================================

void CItemTypeLibsDlg::updatePropertyCtrls()
{
  UpdateData(TRUE);

  CListCtrl* pPropList = (CListCtrl*)GetDlgItem(IDC_PROPERTY_LIST);

  if( m_strPropName.IsEmpty() )
  {
    GetDlgItem(IDC_ADD_PROP_BUTTON)->EnableWindow(FALSE);
  }
  else
  {
    LVFINDINFO fndInfo;
    fndInfo.flags = LVFI_STRING;
    fndInfo.psz = m_strPropName.GetBuffer(m_strPropName.GetLength());
    GetDlgItem(IDC_ADD_PROP_BUTTON)->EnableWindow(pPropList->FindItem( &fndInfo ) == -1);
    m_strPropName.ReleaseBuffer();
  }

  GetDlgItem(IDC_EDIT_PROP_BUTTON)->EnableWindow(m_iPropIndex >= 0);
  GetDlgItem(IDC_DEL_PROP_BUTTON)->EnableWindow(m_iPropIndex >= 0);

  UpdateData(FALSE);
}

//=================================================================================

void CItemTypeLibsDlg::setPropertyTypesComboValues()
{
  UpdateData(TRUE);

  CListCtrl* pItemTypesList = (CListCtrl*)GetDlgItem(IDC_ITEM_TYPE_LIST);
  CComboBox* pTypesCombo    = (CComboBox*)GetDlgItem(IDC_PROP_TYPE_COMBO);

  while( pTypesCombo->GetCount() )
    pTypesCombo->DeleteString(0);

  pTypesCombo->InsertString(0, L"kString");
  pTypesCombo->InsertString(1, L"kDouble");
  pTypesCombo->InsertString(2, L"kInteger");
  pTypesCombo->InsertString(3, L"kBoolean");
  pTypesCombo->InsertString(4, L"kDateTime");
  pTypesCombo->InsertString(5, L"kPoint3d");

  OdDgItemTypeLibraryPtr pLibrary = m_idItemTypeLib.openObject(OdDg::kForRead, true);

  if( !pLibrary.isNull() && pLibrary->getItemTypeCount() > (OdUInt32)m_iItemType )
  {
    OdDgItemTypePtr pItemType = pLibrary->getItemType(m_iItemType);

    if( pItemType->getCategory() == OdDgItemType::kItemType )
    {
      for( int i = 0; i < pItemTypesList->GetItemCount(); i++ )
      {
        CString strText = pItemTypesList->GetItemText(i, 0);

        if( strText.GetLength() > 2 && strText.Left(2) == L"-[" )
          pTypesCombo->InsertString( pTypesCombo->GetCount(), strText.Mid(2, strText.GetLength() - 4) );
      }
    }
  }

  if( pTypesCombo->FindString(0, m_strPropType) != -1 )
    pTypesCombo->SetCurSel( pTypesCombo->FindString(0, m_strPropType) );

  UpdateData(FALSE);
}

//=================================================================================

BOOL CItemTypeLibsDlg::OnInitDialog()
{
  if( !m_pDb )
    return FALSE;

  CListCtrl* pItemTypesList = (CListCtrl*)GetDlgItem(IDC_ITEM_TYPE_LIST);
  CListCtrl* pPropList      = (CListCtrl*)GetDlgItem(IDC_PROPERTY_LIST);

  pItemTypesList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
  pPropList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

  pItemTypesList->InsertColumn(0,L"Name", LVCFMT_LEFT, 140, 1);
  pPropList->InsertColumn(0, L"Name", LVCFMT_LEFT, 78);
  pPropList->InsertColumn(1, L"Type", LVCFMT_LEFT, 70);
  pPropList->InsertColumn(2, L"Default Value", LVCFMT_LEFT, 100);

  updateLibrariesCombo();
  updateItemTypesList();
  setPropertyTypesComboValues();
  updatePropertiesList();

  return CDialog::OnInitDialog();
}

//=================================================================================

void CItemTypeLibsDlg::OnLvnItemchangedPropertyList(NMHDR *pNMHDR, LRESULT *pResult)
{
  UNUSED_ALWAYS( pNMHDR );

  KillTimer(UPDATE_PROPERTY_TIMER);
  SetTimer( UPDATE_PROPERTY_TIMER, 500, NULL );

  *pResult = 0;
}

//=================================================================================

void CItemTypeLibsDlg::OnLvnItemchangedItemTypeList(NMHDR *pNMHDR, LRESULT *pResult)
{
  UNUSED_ALWAYS( pNMHDR );

  KillTimer(UPDATE_ITEMTYPE_TIMER);
  SetTimer( UPDATE_ITEMTYPE_TIMER, 500, NULL );

  *pResult = 0;
}

//=================================================================================

void CItemTypeLibsDlg::OnTimer( UINT_PTR nIDEvent )
{
  if( nIDEvent == UPDATE_PROPERTY_TIMER )
  {
    CListCtrl* pList = (CListCtrl*)(GetDlgItem(IDC_PROPERTY_LIST));

    if( pList )
    {
      int iItemSelected = pList->GetSelectionMark();

      if( (m_iPropIndex != iItemSelected) && (iItemSelected >= 0) )
      {
        m_iPropIndex = iItemSelected;
        OdDgItemTypePropertyPtr pProp = curProperty();

        m_strPropName  = convertECSchemaItemNameToString(pProp->getName()).c_str();
        m_strPropType  = propertyTypeToStr(pProp);
        m_strPropValue = propertyValueToStr(pProp->getDefaultValue());
        m_bArray       = pProp->getArrayFlag() ? 1 : 0;

        CComboBox* pTypesCombo    = (CComboBox*)GetDlgItem(IDC_PROP_TYPE_COMBO);
        pTypesCombo->SetCurSel( pTypesCombo->FindString(0,m_strPropType));

        UpdateData(FALSE);
        updatePropertyCtrls();
      }
    }

    KillTimer(UPDATE_PROPERTY_TIMER);
  }
  else if( nIDEvent == UPDATE_ITEMTYPE_TIMER )
  {
    CListCtrl* pList = (CListCtrl*)(GetDlgItem(IDC_ITEM_TYPE_LIST));

    if( pList )
    {
      int iItemSelected = pList->GetSelectionMark();

      if( (m_iItemType != iItemSelected) && (iItemSelected >= 0) )
      {
        m_iItemType  = iItemSelected;
        m_iPropIndex = -1;

        CString strText = pList->GetItemText(m_iItemType, 0);

        if( strText.GetLength() > 2 && strText.Left(2) == L"-[" )
        {
          m_strPropertyTypeName = strText.Mid(2, strText.GetLength()-4);
          m_strItemTypeName.Empty();
        }
        else
        {
          m_strPropertyTypeName.Empty();
          m_strItemTypeName = strText;
        }
        UpdateData(FALSE);

        updateItemTypesCtrls();
        setPropertyTypesComboValues();
        updatePropertiesList();
      }
    }

    KillTimer(UPDATE_ITEMTYPE_TIMER);
  }
  else
  {
    CDialog::OnTimer( nIDEvent );
  }
}

//=================================================================================

void CItemTypeLibsDlg::OnItemLibComboSelchange()
{
  int iOldLibIndex = m_iItemTypeLib;
  UpdateData(TRUE);

  if( iOldLibIndex != m_iItemTypeLib )
  {
    m_idItemTypeLib = NULL;

    CComboBox* pLibCombo = (CComboBox*)(GetDlgItem(IDC_ITEM_LIB_COMBO));

    if( m_iItemTypeLib >= 0 && m_pDb)
    {
      OdDgTagDefinitionSetTablePtr pTagSetTable = m_pDb->getTagDefinitionSetTable(OdDg::kForRead);

      if( !pTagSetTable.isNull() )
      {
        OdDgElementIteratorPtr pIter = pTagSetTable->createIterator();

        OdUInt32 uIndex = 0;

        for(; !pIter->done(); pIter->step() )
        {
          OdDgElementPtr pElm = pIter->item().openObject(OdDg::kForRead);

          if( pElm->isKindOf(OdDgItemTypeLibrary::desc()) && !pElm->isKindOf(OdDgXMLSchemaContainer::desc()) )
          {
            if( uIndex == m_iItemTypeLib )
            {
              m_idItemTypeLib = pElm->elementId();
              pLibCombo->GetLBText(m_iItemTypeLib,m_strItemTypeLibName);
              break;
            }
            else
              uIndex++;
          }
        }
      }
    }

    UpdateData(FALSE);

    updateLibrariesCtrls();
    updateItemTypesList();
    setPropertyTypesComboValues();
    updatePropertiesList();
  }
}

//=================================================================================

void CItemTypeLibsDlg::OnEditPropName()
{
  UpdateData(TRUE);
  updatePropertyCtrls();
}

//=================================================================================

void CItemTypeLibsDlg::OnEditItemOrPropName()
{
  UpdateData(TRUE);
  updateItemTypesCtrls();
}

//=================================================================================

void CItemTypeLibsDlg::OnEditLibName()
{
  UpdateData(TRUE);
  updateLibrariesCtrls();
}

//=================================================================================

void CItemTypeLibsDlg::setItemPropertyType( const CString& strTypeName, OdDgItemTypePropertyPtr& pProperty )
{
  OdString strType = strTypeName;
  pProperty->setUsePropertyTypeFlag(false);

  if( strTypeName == L"kString" || strTypeName.IsEmpty() )
    pProperty->setType( OdDgItemTypeProperty::kString );
  else if( strTypeName == L"kDouble" )
    pProperty->setType( OdDgItemTypeProperty::kDouble );
  else if( strTypeName == L"kInteger" )
    pProperty->setType( OdDgItemTypeProperty::kInteger );
  else if( strTypeName == L"kBoolean" )
    pProperty->setType( OdDgItemTypeProperty::kBoolean );
  else if( strTypeName == L"kDateTime" )
    pProperty->setType( OdDgItemTypeProperty::kDateTime );
  else if( strTypeName == L"kPoint3d" )
    pProperty->setType( OdDgItemTypeProperty::kPoint3d );
  else
  {
    pProperty->setTypeName(strType);
    pProperty->setType(OdDgItemTypeProperty::kCustom);
  }
}

//=================================================================================

OdString CItemTypeLibsDlg::getPartOfDate( OdString& strValue )
{
  OdString retVal;

  if( strValue.isEmpty() )
    retVal = L"1";
  else if( strValue.find('.') != -1 )
  {
    retVal = strValue.left(strValue.find('.'));
    strValue = strValue.right(strValue.getLength() - retVal.getLength() - 1 );
  }
  else
  {
    retVal = strValue;
    strValue = OdString::kEmpty;
  }

  return retVal;
}

//=================================================================================

void CItemTypeLibsDlg::setItemPropertyDefaultValue( const CString& strValue, 
                                                      OdDgItemTypePropertyPtr& pProperty )
{
  if( strValue.IsEmpty() || pProperty->getType() == OdDgItemTypeProperty::kCustom )
    return;

  SET_LOCALE( LC_NUMERIC, "C" );

  OdString strVal = strValue;

  OdDgItemTypePropertyValue val;

  switch( pProperty->getType() )
  {
    case OdDgItemTypeProperty::kString:
    {
      val.setStringValue( strVal );
    } break;

    case OdDgItemTypeProperty::kDouble:
    {
      strVal.replace(L',', L'.');
      OdChar* pNextDouble;
      double dValue = wcstod( strVal.c_str(), &pNextDouble );
      val.setDoubleValue( dValue );
    } break;

    case OdDgItemTypeProperty::kInteger:
    {
      OdInt32 iValue = wcstol( strVal.c_str(), 0, 10 );
      val.setIntegerValue( iValue );
    } break;

    case OdDgItemTypeProperty::kBoolean:
    {
      strVal.makeUpper();
      
      if( strVal == L"TRUE" )
        val.setBooleanValue(true);
      else
        val.setBooleanValue(false);
    } break;

    case OdDgItemTypeProperty::kDateTime:
    {
      strVal.trimLeft();
      strVal.trimRight();
      strVal.replace(L',', L'.');
      strVal.replace(L'/', L'.');
      strVal.replace(L'\\', L'.');
      strVal.replace(L':', L'.');
      strVal.replace(L' ', L'.');

      OdString strDay, strMonth, strYear, strHour, strMinute, strSec;

      strDay    = getPartOfDate( strVal );
      strMonth  = getPartOfDate( strVal );
      strYear   = getPartOfDate( strVal );
      strHour   = getPartOfDate( strVal );
      strMinute = getPartOfDate( strVal );
      strSec    = getPartOfDate( strVal );

      short uDay    = wcstol( strDay.c_str(), 0, 10 );
      short uMonth  = wcstol( strMonth.c_str(), 0, 10 );
      short uYear   = wcstol( strYear.c_str(), 0, 10 );
      short uHour   = wcstol( strHour.c_str(), 0, 10 );
      short uMinute = wcstol( strMinute.c_str(), 0, 10 );
      short uSec    = wcstol( strSec.c_str(), 0, 10 );

      OdTimeStamp timeVal;
      timeVal.setDate( uMonth, uDay, uYear );
      timeVal.setTime( uHour, uMinute, uSec, 0 );
      val.setDateTimeValue( timeVal );
    } break;

    case OdDgItemTypeProperty::kPoint3d:
    {
      strVal.trimLeft();
      strVal.trimLeft('(');
      strVal.trimRight();
      strVal.trimRight(')');
      strVal.replace(L',', L' ');
      strVal.replace(L';', L' ');
      OdChar* pNextDouble;
      double dX   = wcstod( strVal.c_str(), &pNextDouble );
      double dY   = wcstod( pNextDouble, &pNextDouble );
      double dZ   = wcstod( pNextDouble, &pNextDouble );
      val.setPointValue( OdGePoint3d(dX, dY, dZ) );
    } break;
  }

  pProperty->setDefaultValue( val );

  RESET_LOCALE();
}

//=================================================================================

void CItemTypeLibsDlg::OnAddProperty()
{
  UpdateData(TRUE);

  OdString strName = m_strPropName;
  OdDgItemTypePropertyPtr pNewProperty = OdDgItemTypeProperty::createObject();
  pNewProperty->setName( strName );
  setItemPropertyType( m_strPropType, pNewProperty );
  setItemPropertyDefaultValue( m_strPropValue, pNewProperty );
  pNewProperty->setArrayFlag( m_bArray ? true : false);
  pNewProperty->setDisplayLabel(strName);
  pNewProperty->setUsePropertyTypeFlag( pNewProperty->getType() == OdDgItemTypeProperty::kCustom );

  if( !m_idItemTypeLib.isNull() && m_iItemType >= 0 )
  {
    OdDgItemTypeLibraryPtr pLibrary = m_idItemTypeLib.openObject(OdDg::kForRead, true);

    if( !pLibrary.isNull() && pLibrary->getItemTypeCount() > (OdUInt32)m_iItemType )
    {
      OdDgItemTypePtr pItemType = pLibrary->getItemType(m_iItemType);
      pItemType->addProperty( pNewProperty );
      m_iPropIndex = pItemType->getPropertyCount() - 1;
    }
  }

  int iProp = m_iPropIndex;
  updatePropertiesList();

  CListCtrl* pPropList = (CListCtrl*)GetDlgItem(IDC_PROPERTY_LIST);
  pPropList->SetSelectionMark(iProp);
  pPropList->SetItemState(iProp,LVIS_SELECTED,(UINT)-1);
}

//=================================================================================

void CItemTypeLibsDlg::OnModifyProperty()
{
  UpdateData(TRUE);

  OdDgItemTypePropertyPtr pCurProp = curProperty();
  OdString strName = m_strPropName;
  pCurProp->setName( strName );
  setItemPropertyType( m_strPropType, pCurProp );
  setItemPropertyDefaultValue( m_strPropValue, pCurProp );
  pCurProp->setArrayFlag( m_bArray ? true : false);
  pCurProp->setDisplayLabel(strName);
  pCurProp->setUsePropertyTypeFlag( pCurProp->getType() == OdDgItemTypeProperty::kCustom );

  updatePropertiesList();
}

//=================================================================================

void CItemTypeLibsDlg::OnDeleteProperty()
{
  if( !m_idItemTypeLib.isNull() && m_iItemType >= 0 )
  {
    OdDgItemTypeLibraryPtr pLibrary = m_idItemTypeLib.openObject(OdDg::kForRead, true);

    if( !pLibrary.isNull() && pLibrary->getItemTypeCount() > (OdUInt32)m_iItemType )
    {
      OdDgItemTypePtr pItemType = pLibrary->getItemType(m_iItemType);
      pItemType->deleteProperty( m_iPropIndex);
      m_iPropIndex = -1;
    }
  }

  updatePropertiesList();
}

//=================================================================================

void CItemTypeLibsDlg::OnAddItemType()
{
  UpdateData(TRUE);

  OdString strName = m_strItemTypeName;
  OdDgItemTypePtr pNewItemType = OdDgItemType::createObject();
  pNewItemType->setName( strName );
  pNewItemType->setCategory( OdDgItemType::kItemType );
  pNewItemType->setDisplayLabel( strName );
  pNewItemType->setUseNameForElementFlag(false);

  if( !m_idItemTypeLib.isNull() )
  {
    OdDgItemTypeLibraryPtr pLibrary = m_idItemTypeLib.openObject(OdDg::kForWrite, true);
    pLibrary->addItemType(pNewItemType);
  }

  m_iItemType = -1;
  updateItemTypesList();
  updatePropertiesList();

  CListCtrl* pItemList = (CListCtrl*)GetDlgItem(IDC_ITEM_TYPE_LIST);
  pItemList->SetSelectionMark(pItemList->GetItemCount()-1);
  pItemList->SetItemState(pItemList->GetItemCount()-1,LVIS_SELECTED,(UINT)-1);
}

//=================================================================================

void CItemTypeLibsDlg::OnAddPropertyType()
{
  UpdateData(TRUE);

  OdString strName = m_strPropertyTypeName;
  OdDgItemTypePtr pNewItemType = OdDgItemType::createObject();
  pNewItemType->setName( strName );
  pNewItemType->setCategory( OdDgItemType::kPropertyType );
  pNewItemType->setDisplayLabel( strName );
  pNewItemType->setUseNameForElementFlag(true);

  if( !m_idItemTypeLib.isNull() )
  {
    OdDgItemTypeLibraryPtr pLibrary = m_idItemTypeLib.openObject(OdDg::kForWrite, true);
    pLibrary->addItemType(pNewItemType);
  }

  m_iItemType = -1;
  updateItemTypesList();
  setPropertyTypesComboValues();
  updatePropertiesList();

  CListCtrl* pItemList = (CListCtrl*)GetDlgItem(IDC_ITEM_TYPE_LIST);
  pItemList->SetSelectionMark(pItemList->GetItemCount()-1);
  pItemList->SetItemState(pItemList->GetItemCount()-1,LVIS_SELECTED,(UINT)-1);
}

//=================================================================================

void CItemTypeLibsDlg::OnDeleteItemType()
{
  if( !m_idItemTypeLib.isNull() && m_iItemType >= 0 )
  {
    OdDgItemTypeLibraryPtr pLibrary = m_idItemTypeLib.openObject(OdDg::kForWrite, true);
    pLibrary->deleteItemType(m_iItemType);
  }

  m_iItemType = -1;
  updateItemTypesList();
  setPropertyTypesComboValues();
  updatePropertiesList();
}

//=================================================================================

void CItemTypeLibsDlg::OnAddLibrary()
{
  UpdateData(TRUE);

  OdDgItemTypeLibraryPtr pLibrary = OdDgItemTypeLibrary::createObject();
  OdString strName = m_strItemTypeLibName;
  pLibrary->setName( strName );
  pLibrary->setIsProxyFlag(false);
  pLibrary->setDisplayLabel(strName);

  if( m_pDb )
  {
    OdDgTagDefinitionSetTablePtr pTagSetTable = m_pDb->getTagDefinitionSetTable(OdDg::kForRead);

    if( !pTagSetTable.isNull() )
    {
      pTagSetTable->add( pLibrary );

      updateLibrariesCombo();
      CComboBox* pLibCombo = (CComboBox*)(GetDlgItem(IDC_ITEM_LIB_COMBO));
      pLibCombo->SetCurSel( pLibCombo->GetCount() - 1 );
      m_idItemTypeLib = pLibrary->elementId();

      UpdateData(FALSE);

      updateLibrariesCtrls();
      updateItemTypesList();
      setPropertyTypesComboValues();
      updatePropertiesList();
    }
  }
}

//=================================================================================

void CItemTypeLibsDlg::OnDeleteLibrary()
{
  UpdateData(TRUE);

  if( m_pDb && !m_idItemTypeLib.isNull() )
  {
    OdDgElementPtr pElm = m_idItemTypeLib.openObject(OdDg::kForWrite);
    pElm->erase(true);
    m_idItemTypeLib = NULL;
    m_iItemType = -1;
    UpdateData(FALSE);

    updateLibrariesCombo();
    updateItemTypesList();
    setPropertyTypesComboValues();
    updatePropertiesList();
  }
}

//=================================================================================



