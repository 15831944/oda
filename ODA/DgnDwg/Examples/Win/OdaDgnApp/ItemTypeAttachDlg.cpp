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
#include "ItemTypeAttachDlg.h"
#include "DgTable.h"
#include "DgItemTypeLibrary.h"
#include "OdSetLocale.h"
#include "ItemTypeLibsDlg.h"
#include "DgElement.h"

// CItemTypeAttachDlg dialog

#define UPDATE_PROPERTY_TIMER 156
#define UPDATE_ARRAY_TIMER    157

IMPLEMENT_DYNAMIC(CItemTypeAttachDlg, CDialog)

CItemTypeAttachDlg::CItemTypeAttachDlg(CWnd* pParent, OdDgDatabase* pDb, const OdDgElementId& idElm )
	: CDialog(CItemTypeAttachDlg::IDD, pParent)
{
  m_pDb           = pDb;
  m_idElement     = idElm;
  m_idItemTypeLib = NULL;
  m_iItemType     = -1;
  m_iPropIndex    = -1;
  m_iArrIndex     = -1;
}

//=================================================================================

CItemTypeAttachDlg::~CItemTypeAttachDlg()
{
}

//=================================================================================

void CItemTypeAttachDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_CBIndex( pDX, IDC_LIB_COMBO, m_iItemTypeLib );
  DDX_CBIndex( pDX, IDC_ITEM_TYPE_COMBO, m_iItemType );
  DDX_Text( pDX, IDC_VALUE_EDIT, m_strSimpleVal );
  DDX_Text( pDX, IDC_ARR_VALUE_EDIT, m_strArrVal );
}

//=================================================================================

BEGIN_MESSAGE_MAP(CItemTypeAttachDlg, CDialog)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_PROPERTIES_LIST, CItemTypeAttachDlg::OnLvnItemchangedPropertiesList )
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_ARRAY_LIST, CItemTypeAttachDlg::OnLvnItemchangedArrayList )
  ON_CBN_SELCHANGE(IDC_LIB_COMBO, CItemTypeAttachDlg::OnItemLibComboSelchange )
  ON_CBN_SELCHANGE(IDC_ITEM_TYPE_COMBO, CItemTypeAttachDlg::OnItemTypeComboSelchange )
  ON_BN_CLICKED(IDC_ATTACH_BUTTON, CItemTypeAttachDlg::OnAttachBtnClicked )
  ON_BN_CLICKED(IDC_DETACH_BUTTON, CItemTypeAttachDlg::OnDetachBtnClicked )
  ON_BN_CLICKED(IDC_OPENLIB_BUTTON, CItemTypeAttachDlg::OnLibBtnClicked )  
  ON_BN_CLICKED(IDC_ARR_ADD_BUTTON, CItemTypeAttachDlg::OnArrAddBtnClicked )
  ON_BN_CLICKED(IDC_ARR_DEL_BUTTON, CItemTypeAttachDlg::OnArrDelBtnClicked )
  ON_BN_CLICKED(IDC_ARR_EDIT_BUTTON, CItemTypeAttachDlg::OnEditBtnClicked )
  ON_WM_TIMER()
END_MESSAGE_MAP()

//=================================================================================

void CItemTypeAttachDlg::OnLvnItemchangedPropertiesList(NMHDR *pNMHDR, LRESULT *pResult)
{
  UNUSED_ALWAYS( pNMHDR );

  KillTimer(UPDATE_PROPERTY_TIMER);
  SetTimer(UPDATE_PROPERTY_TIMER, 500, NULL );

  *pResult = 0;
}

//=================================================================================

void CItemTypeAttachDlg::OnLvnItemchangedArrayList(NMHDR *pNMHDR, LRESULT *pResult)
{
  UNUSED_ALWAYS( pNMHDR );

  KillTimer(UPDATE_ARRAY_TIMER);
  SetTimer(UPDATE_ARRAY_TIMER, 500, NULL );

  *pResult = 0;
}

//=================================================================================

void CItemTypeAttachDlg::OnTimer( UINT_PTR nIDEvent )
{
  if( nIDEvent == UPDATE_PROPERTY_TIMER )
  {
    CListCtrl* pList = (CListCtrl*)(GetDlgItem(IDC_PROPERTIES_LIST));

    if( pList )
    {
      int iItemSelected = pList->GetSelectionMark();

      if( (m_iPropIndex != iItemSelected) && (iItemSelected >= 0) )
      {
        m_iPropIndex = iItemSelected;
        updatePropertyCtrls();
      }
    }

    KillTimer(UPDATE_PROPERTY_TIMER);
  }
  else if( nIDEvent == UPDATE_ARRAY_TIMER )
  {
    CListCtrl* pList = (CListCtrl*)(GetDlgItem(IDC_ARRAY_LIST));

    if( pList )
    {
      int iItemSelected = pList->GetSelectionMark();

      if( (m_iArrIndex != iItemSelected) && (iItemSelected >= 0) )
      {
        m_iArrIndex = iItemSelected;
        m_strArrVal = pList->GetItemText(m_iArrIndex, 0);
        GetDlgItem(IDC_ARR_DEL_BUTTON)->EnableWindow(TRUE);
        GetDlgItem(IDC_ARR_EDIT_BUTTON)->EnableWindow(TRUE);
        UpdateData(FALSE);
      }
    }

    KillTimer(UPDATE_ARRAY_TIMER);
  }
  else
  {
    CDialog::OnTimer( nIDEvent );
  }
}

//=================================================================================

OdDgECPropertyInstance getSubPropertyByIndex(const OdDgECClassInstancePtr& pClassInstance, OdUInt32 uIndex)
{
  OdDgECPropertyInstance retVal;

  if( !pClassInstance.isNull() )
  {
    OdUInt32 uCurIndex = 0;

    for (OdUInt32 j = 0; j < pClassInstance->getPropertyInstanceCount(); j++)
    {
      OdDgECPropertyInstance subProperty = pClassInstance->getPropertyInstance(j);

      if ((subProperty.getType() == OdDgECPropertyInstance::kECClassInstance) ||
          (subProperty.getType() == OdDgECPropertyInstance::kECClassInstanceArray)
        )
      {
        uCurIndex++;
        continue;
      }

      if (uCurIndex == uIndex)
      {
        retVal = pClassInstance->getPropertyInstance(uCurIndex);
        break;
      }

      uCurIndex++;
    }
  }

  return retVal;
}

//=================================================================================

void CItemTypeAttachDlg::valueDataArrToStrArr(const OdDgECPropertyInstance& value,
                                                OdUInt32 uItemIndex,
                                                  OdArray<CString>& arrStrValues)
{
  OdDgECPropertyInstance curVal = value;

  if( curVal.getType() == OdDgECPropertyInstance::kECClassInstance )
    curVal = getSubPropertyByIndex(value.getClassInstanceValue(),uItemIndex);

  OdDgECPropertyInstance::OdDgECPropertyInstanceType type = curVal.getType();

  CString strData;

  switch( type )
  {
    case OdDgECPropertyInstance::kBoolArray:
    {
      OdArray<bool> arrBool = curVal.getBoolArrayValue();

      for( OdUInt32 i = 0; i < arrBool.size(); i++ )
      {
        arrBool[i] ? strData = L"true" : strData = L"false";
        arrStrValues.push_back(strData);
      }
    } break;

    case OdDgECPropertyInstance::kByteArray:
    {
      OdArray<OdInt8> arrByte = curVal.getByteArrayValue();

      for( OdUInt32 i = 0; i < arrByte.size(); i++ )
      {
        strData.Format(L"%d", arrByte[i] );
        arrStrValues.push_back(strData);
      }
    } break;

    case OdDgECPropertyInstance::kShortArray:
    {
      OdArray<OdInt16> arrShort = curVal.getShortArrayValue();

      for( OdUInt32 i = 0; i < arrShort.size(); i++ )
      {
        strData.Format(L"%d", arrShort[i] );
        arrStrValues.push_back(strData);
      }
    } break;

    case OdDgECPropertyInstance::kInt32Array:
    {
      OdArray<OdInt32> arrInt = curVal.getIntArrayValue();

      for( OdUInt32 i = 0; i < arrInt.size(); i++ )
      {
        strData.Format(L"%d", arrInt[i] );
        arrStrValues.push_back(strData);
      }
    } break;

    case OdDgECPropertyInstance::kInt64Array:
    {
      OdArray<OdInt64> arrInt64 = curVal.getInt64ArrayValue();

      for( OdUInt32 i = 0; i < arrInt64.size(); i++ )
      {
        strData.Format(L"%u", arrInt64[i] );
        arrStrValues.push_back(strData);
      }
    } break;

    case OdDgECPropertyInstance::kDoubleArray:
    {
      OdArray<double> arrDouble = curVal.getDoubleArrayValue();

      for( OdUInt32 i = 0; i < arrDouble.size(); i++ )
      {
        strData.Format(L"%f", arrDouble[i] );
        arrStrValues.push_back(strData);
      }
    } break;

    case OdDgECPropertyInstance::kStringArray:
    {
      OdArray<OdString> arrStr = curVal.getStringArrayValue();

      for( OdUInt32 i = 0; i < arrStr.size(); i++ )
      {
        strData = arrStr[i].c_str();
        arrStrValues.push_back(strData);
      }
    } break;

    case OdDgECPropertyInstance::kPoint3dArray:
    {
      OdArray<OdGePoint3d> arrPts = curVal.getPoint3dArrayValue();

      for( OdUInt32 i = 0; i < arrPts.size(); i++ )
      {
        strData.Format(L"(%f,%f,%f)", arrPts[i].x, arrPts[i].y, arrPts[i].z );
        arrStrValues.push_back(strData);
      }
    } break;

    case OdDgECPropertyInstance::kDateTimeArray:
    {
      OdArray<OdTimeStamp> arrTime = curVal.getDateTimeArrayValue();

      for( OdUInt32 i = 0; i < arrTime.size(); i++ )
      {
        OdString strVal;
        arrTime[i].strftime(L"%d.%m.%Y %H:%M:%S", strVal);
        strData = strVal.c_str();
        arrStrValues.push_back(strData);
      }
    } break;
  }
}

//=================================================================================

void CItemTypeAttachDlg::updatePropertyCtrls()
{
  UpdateData(TRUE);

  bool bSimpleProp = false;
  bool bArrayProp  = false;

  if( (m_iPropIndex >= 0) && (m_iPropIndex < (int)m_arrListDescriptor.size()) )
  {
    OdDgECPropertyInstance::OdDgECPropertyInstanceType curType = m_arrListDescriptor[m_iPropIndex].m_value.getType();

    if (curType == OdDgECPropertyInstance::kECClassInstance)
    {
      OdDgECPropertyInstance curProp = getSubPropertyByIndex(m_arrListDescriptor[m_iPropIndex].m_value.getClassInstanceValue(), m_arrListDescriptor[m_iPropIndex].m_uSubpropIndex);
      curType = curProp.getType();
    }
    
    switch( curType )
    {
      case OdDgECPropertyInstance::kBool:
      case OdDgECPropertyInstance::kByte:
      case OdDgECPropertyInstance::kShort:
      case OdDgECPropertyInstance::kInt32:
      case OdDgECPropertyInstance::kInt64:
      case OdDgECPropertyInstance::kDouble:
      case OdDgECPropertyInstance::kString:
      case OdDgECPropertyInstance::kPoint3d:
      case OdDgECPropertyInstance::kDateAndTime:
      {
        bSimpleProp = true;
        m_strSimpleVal = valueDataToStr(m_arrListDescriptor[m_iPropIndex].m_value, m_arrListDescriptor[m_iPropIndex].m_uSubpropIndex);
        m_strArrVal.Empty();
        m_iArrIndex = -1;
      } break;

      case OdDgECPropertyInstance::kBoolArray:
      case OdDgECPropertyInstance::kByteArray:
      case OdDgECPropertyInstance::kShortArray:
      case OdDgECPropertyInstance::kInt32Array:
      case OdDgECPropertyInstance::kInt64Array:
      case OdDgECPropertyInstance::kDoubleArray:
      case OdDgECPropertyInstance::kStringArray:
      case OdDgECPropertyInstance::kPoint3dArray:
      case OdDgECPropertyInstance::kDateTimeArray:
      {
        m_strSimpleVal.Empty();
        OdArray<CString> arrStrValues;
        valueDataArrToStrArr(m_arrListDescriptor[m_iPropIndex].m_value, m_arrListDescriptor[m_iPropIndex].m_uSubpropIndex, arrStrValues );

        CListCtrl* pListCtrl = (CListCtrl*)(GetDlgItem(IDC_ARRAY_LIST));
        pListCtrl->DeleteAllItems();

        for( OdUInt32 i = 0; i < arrStrValues.size(); i++ )
          pListCtrl->InsertItem(i, arrStrValues[i]);

        if( arrStrValues.size() > 0 )
        {
          m_strArrVal = arrStrValues[0];
          m_iArrIndex = 0;
        }
        else
        {
          m_strArrVal.Empty();
          m_iArrIndex = -1;
        }

        pListCtrl->SetSelectionMark(m_iArrIndex);

        bArrayProp = true;
      } break;

      default:
      {
        m_strSimpleVal.Empty();
        m_strArrVal.Empty();
        m_iArrIndex = -1;
      } break;
    }
  }
  else
  {
    m_strSimpleVal.Empty();
    m_strArrVal.Empty();
    m_iArrIndex = -1;
  }

  if( !bArrayProp )
  {
    CListCtrl* pListCtrl = (CListCtrl*)(GetDlgItem(IDC_ARRAY_LIST));
    pListCtrl->DeleteAllItems();
  }

  GetDlgItem(IDC_VALUE_EDIT)->EnableWindow(bSimpleProp);
  GetDlgItem(IDC_VALUE_STATIC)->EnableWindow(bSimpleProp);
  GetDlgItem(IDC_ARRAY_LIST)->EnableWindow(bArrayProp);
  GetDlgItem(IDC_ARRAY_STATIC)->EnableWindow(bArrayProp);
  GetDlgItem(IDC_ARR_VALUE_EDIT)->EnableWindow(bArrayProp);
  GetDlgItem(IDC_ARR_ADD_BUTTON)->EnableWindow(bArrayProp);
  GetDlgItem(IDC_ARR_DEL_BUTTON)->EnableWindow(bArrayProp && (m_iArrIndex >= 0) );
  GetDlgItem(IDC_ARR_EDIT_BUTTON)->EnableWindow(bSimpleProp || (bArrayProp && m_iArrIndex >= 0) );

  UpdateData(FALSE);
}

//=================================================================================

void CItemTypeAttachDlg::OnItemLibComboSelchange()
{
  int iOldData = m_iItemTypeLib;
  UpdateData(TRUE);

  if( iOldData != m_iItemTypeLib )
  {
    if( (m_iItemTypeLib >= 0) && ((int)m_arrLibIds.size() > m_iItemTypeLib) )
      m_idItemTypeLib = m_arrLibIds[m_iItemTypeLib];
    else
      m_idItemTypeLib = NULL;

    m_iItemType = 0;
    setItemTypeCombo();
  }

  UpdateData(FALSE);
}

//=================================================================================

void CItemTypeAttachDlg::OnItemTypeComboSelchange()
{
  int iOldType = m_iItemType;
  UpdateData(TRUE);

  if( iOldType != m_iItemType )
    updateItemTypeCtrls();

  UpdateData(FALSE);
}
//=================================================================================

BOOL CItemTypeAttachDlg::OnInitDialog()
{
  if( !m_pDb || m_idElement.isNull())
    return FALSE;

  CListCtrl* pPropList = (CListCtrl*)GetDlgItem(IDC_PROPERTIES_LIST);

  pPropList->InsertColumn(0, L"Library", LVCFMT_LEFT, 85);
  pPropList->InsertColumn(1, L"Item Type", LVCFMT_LEFT, 85);
  pPropList->InsertColumn(2, L"Property", LVCFMT_LEFT, 133);
  pPropList->InsertColumn(3, L"Type", LVCFMT_LEFT, 70);
  pPropList->InsertColumn(4, L"Value", LVCFMT_LEFT, 90);
  pPropList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

  CListCtrl* pListCtrl = (CListCtrl*)(GetDlgItem(IDC_ARRAY_LIST));
  pListCtrl->InsertColumn(0, L"Values", LVCFMT_LEFT, 80 );

  initListDescriptor();
  updatePropertyListData();
  updatePropertyCtrls();
  initItemTypeLibraryCombo();

  return CDialog::OnInitDialog();
}

//=================================================================================

void CItemTypeAttachDlg::initItemTypeLibraryCombo()
{
  UpdateData(TRUE);
  CString strActiveLibraryName;

  CComboBox* pLibCombo = (CComboBox*)(GetDlgItem(IDC_LIB_COMBO));

  if( !m_idItemTypeLib.isNull() )
  {
    OdDgElementPtr pItem = m_idItemTypeLib.openObject(OdDg::kForRead);

    if( !pItem.isNull() )
    {
      if( pItem->isKindOf(OdDgItemTypeLibrary::desc()) )
      {
        OdDgItemTypeLibraryPtr pItemTypeLib = pItem;
        strActiveLibraryName = convertECSchemaItemNameToString(pItemTypeLib->getName()).c_str();
      }
      else if (pItem->isKindOf(OdDgECSchema::desc()))
      {
        OdDgECSchemaPtr pItemTypeLib = pItem;
        strActiveLibraryName = convertECSchemaItemNameToString(pItemTypeLib->getName()).c_str();
      }
      else if (pItem->isKindOf(OdDgXMLCustomSchemaContainer::desc()))
      {
        OdDgXMLCustomSchemaContainerPtr pItemTypeLib = pItem;
        strActiveLibraryName = convertECSchemaItemNameToString(pItemTypeLib->getName()).c_str();
      }
    }
  }

  m_idItemTypeLib = NULL;

  while( pLibCombo->GetCount() )
    pLibCombo->DeleteString(0);

  m_arrLibIds.clear();

  if( m_pDb )
  {
    OdDgTagDefinitionSetTablePtr pTagSetTable = m_pDb->getTagDefinitionSetTable(OdDg::kForRead);

    if( !pTagSetTable.isNull() )
    {
      OdDgElementIteratorPtr pIter = pTagSetTable->createIterator();

      for(; !pIter->done(); pIter->step() )
      {
        OdDgElementPtr pElm = pIter->item().openObject(OdDg::kForRead);

        if (!pElm.isNull())
        {
          if (pElm->isKindOf(OdDgItemTypeLibrary::desc()))
          {
            OdDgItemTypeLibraryPtr pItemTypeLib = pElm;
            pLibCombo->InsertString(pLibCombo->GetCount(), convertECSchemaItemNameToString(pItemTypeLib->getName()));
            m_arrLibIds.push_back(pElm->elementId());
          }
          else if (pElm->isKindOf(OdDgECSchema::desc()))
          {
            OdDgECSchemaPtr pItemTypeLib = pElm;
            pLibCombo->InsertString(pLibCombo->GetCount(), convertECSchemaItemNameToString(pItemTypeLib->getName()));
            m_arrLibIds.push_back(pElm->elementId());
          }
          else if (pElm->isKindOf(OdDgXMLCustomSchemaContainer::desc()))
          {
            OdDgXMLCustomSchemaContainerPtr pItemTypeLib = pElm;
            pLibCombo->InsertString(pLibCombo->GetCount(), convertECSchemaItemNameToString(pItemTypeLib->getName()));
            m_arrLibIds.push_back(pElm->elementId());
          }
        }
      }
    }
  }

  int iLib = -1;

  if( !strActiveLibraryName.IsEmpty() )
  {
    iLib = pLibCombo->FindString(0, strActiveLibraryName );

    if( iLib >= 0 )
      m_idItemTypeLib = m_arrLibIds[iLib];
  }

  if( m_idItemTypeLib.isNull() && pLibCombo->GetCount() > 0 )
  {
    m_idItemTypeLib = m_arrLibIds[0];
    iLib = 0;
  }

  if( iLib >= 0 )
  {
    m_iItemTypeLib = iLib;
    pLibCombo->SetCurSel(iLib);
  }

  UpdateData(FALSE);

  setItemTypeCombo();
}

//=================================================================================

void CItemTypeAttachDlg::setItemTypeCombo()
{
  UpdateData(TRUE);

  CComboBox* pItemTypeCombo = (CComboBox*)(GetDlgItem(IDC_ITEM_TYPE_COMBO));

  while( pItemTypeCombo->GetCount() )
    pItemTypeCombo->DeleteString(0);

  m_iItemType = -1;

  if( m_pDb && !m_idItemTypeLib.isNull() )
  {
    OdDgElementPtr pElm = m_idItemTypeLib.openObject(OdDg::kForRead);

    if (!pElm.isNull())
    {
      if (pElm->isKindOf(OdDgItemTypeLibrary::desc()))
      {
        OdDgItemTypeLibraryPtr pItemTypeLib = m_idItemTypeLib.openObject(OdDg::kForRead, true);

        if (!pItemTypeLib.isNull())
        {
          for (int i = 0; i < (int)pItemTypeLib->getItemTypeCount(); i++)
          {
            OdDgItemTypePtr pItemType = pItemTypeLib->getItemType(i);
            CString strText = convertECSchemaItemNameToString(pItemType->getName()).c_str();

            if (pItemType->getCategory() != OdDgItemType::kPropertyType)
              pItemTypeCombo->AddString(strText);

            if (pItemTypeCombo->GetCount() > 0)
              m_iItemType = 0;
          }
        }

        if (m_iItemType >= 0)
          pItemTypeCombo->SetCurSel((int)m_iItemType);
      }
      else if (pElm->isKindOf(OdDgECSchema::desc()))
      {
        OdDgECSchemaPtr pItemTypeLib = m_idItemTypeLib.openObject(OdDg::kForRead, true);

        if (!pItemTypeLib.isNull())
        {
          for (int i = 0; i < (int)pItemTypeLib->getClassCount(); i++)
          {
            OdDgECClassPtr pItemType = pItemTypeLib->getClass(i);

            if (pItemType->isKindOf(OdDgECRelationshipClass::desc()))
              continue;

            CString strText = convertECSchemaItemNameToString(pItemType->getName()).c_str();
            pItemTypeCombo->AddString(strText);

            if (pItemTypeCombo->GetCount() > 0)
              m_iItemType = 0;
          }
        }

        if (m_iItemType >= 0)
          pItemTypeCombo->SetCurSel((int)m_iItemType);
      }
      else if (pElm->isKindOf(OdDgXMLCustomSchemaContainer::desc()))
      {
        OdDgXMLCustomSchemaContainerPtr pItemTypeLib = m_idItemTypeLib.openObject(OdDg::kForRead, true);

        if (!pItemTypeLib.isNull())
        {
          for (int i = 0; i < (int)pItemTypeLib->getCustomItemCount(); i++)
          {
            OdDgCustomItemTypePtr pItemType = pItemTypeLib->getCustomItem(i);

            CString strText = convertECSchemaItemNameToString(pItemType->getName()).c_str();
            pItemTypeCombo->AddString(strText);

            if (pItemTypeCombo->GetCount() > 0)
              m_iItemType = 0;
          }
        }

        if (m_iItemType >= 0)
          pItemTypeCombo->SetCurSel((int)m_iItemType);
      }
    }
  }

  UpdateData(FALSE);

  updateItemTypeCtrls();
}

//=================================================================================

void CItemTypeAttachDlg::updateItemTypeCtrls()
{
  UpdateData(true);

  OdString strLibName      = OdString::kEmpty;
  OdString strItemTypeName = OdString::kEmpty;

  if( !m_idItemTypeLib.isNull() )
  {
    OdDgElementPtr pItem = m_idItemTypeLib.openObject(OdDg::kForRead);

    if (pItem->isKindOf(OdDgItemTypeLibrary::desc()))
    {
      OdDgItemTypeLibraryPtr pItemTypeLib = pItem;

      if (!pItemTypeLib.isNull())
        strLibName = convertECSchemaItemNameToString(pItemTypeLib->getName()).c_str();
    }
    else if (pItem->isKindOf(OdDgECSchema::desc()))
    {
      OdDgECSchemaPtr pItemTypeLib = pItem;

      if (!pItemTypeLib.isNull())
        strLibName = convertECSchemaItemNameToString(pItemTypeLib->getName()).c_str();
    }
    else if (pItem->isKindOf(OdDgXMLCustomSchemaContainer::desc()))
    {
      OdDgXMLCustomSchemaContainerPtr pItemTypeLib = pItem;

      if (!pItemTypeLib.isNull())
        strLibName = convertECSchemaItemNameToString(pItemTypeLib->getName()).c_str();
    }

    if( m_iItemType >= 0 )
    {
      CComboBox* pItemTypeCombo = (CComboBox*)(GetDlgItem(IDC_ITEM_TYPE_COMBO));
      CString strType;
      pItemTypeCombo->GetLBText( m_iItemType, strType );
      strItemTypeName = convertStringToECSchemaItemName(strType.GetBuffer(strType.GetLength()));
      strType.ReleaseBuffer();
    }
  }

  bool bEnableAttach = false;
  bool bEnableDetach = false;

  if( !strLibName.isEmpty() && !strItemTypeName.isEmpty() )
  {
    bEnableAttach = true;
    bEnableDetach = false;

    for( OdUInt32 i = 0; i < m_arrListDescriptor.size(); i++ )
    {
      if( (strLibName == m_arrListDescriptor[i].m_classRef.getSchemaName()) && (strItemTypeName == m_arrListDescriptor[i].m_classRef.getClassName()))
      {
        bEnableAttach = false;
        bEnableDetach = true;
        break;
      }
    }
  }

  GetDlgItem(IDC_ATTACH_BUTTON)->EnableWindow(bEnableAttach);
  GetDlgItem(IDC_DETACH_BUTTON)->EnableWindow(bEnableDetach);
}

//=================================================================================

void CItemTypeAttachDlg::initListDescriptor()
{
  m_arrListDescriptor.clear();

  if( !m_idElement.isNull() && m_pDb )
  {
    OdDgGraphicsElementPtr pElm = m_idElement.openObject(OdDg::kForRead);

    OdDgECClassInstanceElementPEPtr pItemTypePE = pElm;

    OdUInt32 nItems = pItemTypePE->getECClassInstanceCount(pElm);

    if( !pItemTypePE.isNull() && (nItems > 0) )
    {
      for( OdUInt32 i = 0; i < nItems; i++ )
      {
        OdDgECClassInstancePtr pCurItemType = pItemTypePE->getECClassInstance( pElm, i );

        if (pCurItemType.isNull())
          continue;

        CString strLibName;
        CString strItemName = convertECSchemaItemNameToString(pCurItemType->getClassName()).c_str();

        if( pCurItemType->getECSchemaId() != 0 )
        {
          OdDgElementId idLib = m_pDb->getElementId(OdDbHandle(pCurItemType->getECSchemaId()));

          if( !idLib.isNull())
          {
            OdDgElementPtr pItem = idLib.openObject(OdDg::kForRead);

            if( pItem.isNull() )
              continue;

            if (pItem->isKindOf(OdDgItemTypeLibrary::desc()))
            {
              OdDgItemTypeLibraryPtr pItemTypeLib = idLib.openObject(OdDg::kForRead);

              if (!pItemTypeLib.isNull())
                strLibName = convertECSchemaItemNameToString(pItemTypeLib->getName()).c_str();
            }
            else if (pItem->isKindOf(OdDgECSchema::desc()))
            {
              OdDgECSchemaPtr pItemTypeLib = idLib.openObject(OdDg::kForRead);

              if (!pItemTypeLib.isNull())
                strLibName = convertECSchemaItemNameToString(pItemTypeLib->getName()).c_str();
            }
            else if (pItem->isKindOf(OdDgXMLCustomSchemaContainer::desc()))
            {
              OdDgXMLCustomSchemaContainerPtr pItemTypeLib = idLib.openObject(OdDg::kForRead);

              if (!pItemTypeLib.isNull())
                strLibName = convertECSchemaItemNameToString(pItemTypeLib->getName()).c_str();
            }
          }
        }

        if( strLibName.IsEmpty() )
          strLibName = convertECSchemaItemNameToString(pCurItemType->getSchemaName()).c_str();

        for( OdUInt32 j = 0; j < pCurItemType->getPropertyInstanceCount(); j++ )
        {
          OdDgECPropertyInstance curProp = pCurItemType->getPropertyInstance(j);

          if( curProp.getType() == OdDgECPropertyInstance::kECClassInstance )
          {
            CItemTypeValueDescriptor tmp;
            tmp.m_classRef    = pCurItemType->getECClassReference();
            tmp.m_value       = curProp;

            OdDgECClassInstancePtr pComplexProp = curProp.getClassInstanceValue();

            if( !pComplexProp.isNull() )
            {
              for( OdUInt32 k = 0; k < pComplexProp->getPropertyInstanceCount(); k++ )
              {
                OdDgECPropertyInstance subProperty = pComplexProp->getPropertyInstance(k);

                if( (subProperty.getType() != OdDgECPropertyInstance::kECClassInstance) &&
                    (subProperty.getType() != OdDgECPropertyInstance::kECClassInstanceArray)
                  )
                {
                  tmp.m_uSubpropIndex = k;
                  m_arrListDescriptor.push_back(tmp);
                }
              }
            }
          }
          else
          {
            CItemTypeValueDescriptor tmp;
            tmp.m_classRef      = pCurItemType->getECClassReference();
            tmp.m_uSubpropIndex = 0;
            tmp.m_value = curProp;

            m_arrListDescriptor.push_back(tmp);
          }
        }
      }
    }
  }
}

CString CItemTypeAttachDlg::createPropName(const OdString& strPropName, const OdDgECPropertyInstance& value, OdUInt32 uItemIndex )
{
  CString strRet = strPropName.c_str();

  if( value.getType() == OdDgECPropertyInstance::kECClassInstance )
  {
    const OdDgECClassInstancePtr pCplxProp = value.getClassInstanceValue();

    strRet += L"->";
    strRet += convertECSchemaItemNameToString(pCplxProp->getPropertyInstance(uItemIndex).getName()).c_str();
  }

  return strRet;
}

CString CItemTypeAttachDlg::valueTypeToStr(const OdDgECPropertyInstance& value, OdUInt32 uItemIndex)
{
  CString strRet = L"kUndefined";
  OdDgECPropertyInstance::OdDgECPropertyInstanceType type = value.getType();

  if( type == OdDgECPropertyInstance::kECClassInstance )
    type = value.getClassInstanceValue()->getPropertyInstance(uItemIndex).getType();

  switch( type )
  {
    case OdDgECPropertyInstance::kBool: strRet = L"kBool"; break;
    case OdDgECPropertyInstance::kBoolArray: strRet = L"kBoolArray"; break;
    case OdDgECPropertyInstance::kByte: strRet = L"kByte"; break;
    case OdDgECPropertyInstance::kByteArray: strRet = L"kByteArray"; break;
    case OdDgECPropertyInstance::kShort: strRet = L"kShort"; break;
    case OdDgECPropertyInstance::kShortArray: strRet = L"kShortArray"; break;
    case OdDgECPropertyInstance::kInt32: strRet = L"kInt32"; break;
    case OdDgECPropertyInstance::kInt32Array: strRet = L"kInt32Array"; break;
    case OdDgECPropertyInstance::kInt64: strRet = L"kInt64"; break;
    case OdDgECPropertyInstance::kInt64Array: strRet = L"kInt64Array"; break;
    case OdDgECPropertyInstance::kDouble: strRet = L"kDouble"; break;
    case OdDgECPropertyInstance::kDoubleArray: strRet = L"kDoubleArray"; break;
    case OdDgECPropertyInstance::kString: strRet = L"kString"; break;
    case OdDgECPropertyInstance::kStringArray: strRet = L"kStringArray"; break;
    case OdDgECPropertyInstance::kBinary: strRet = L"kBinary"; break;
    case OdDgECPropertyInstance::kBinaryArray: strRet = L"kStringArray"; break;
    case OdDgECPropertyInstance::kPoint3d: strRet = L"kPoint3d"; break;
    case OdDgECPropertyInstance::kPoint3dArray: strRet = L"kPoint3dArray"; break;
    case OdDgECPropertyInstance::kDateAndTime: strRet = L"kDateAndTime"; break;
    case OdDgECPropertyInstance::kDateTimeArray: strRet = L"kDateTimeArray"; break;
    case OdDgECPropertyInstance::kECClassInstance: strRet = L"kECClassInstance"; break;
    case OdDgECPropertyInstance::kECClassInstanceArray: strRet = L"kECClassInstanceArray"; break;
  }

  return strRet;
}

CString CItemTypeAttachDlg::valueDataToStr(const OdDgECPropertyInstance& value, OdUInt32 uItemIndex)
{
  CString strRet = L"...";

  OdDgECPropertyInstance curVal = value;
  OdDgECPropertyInstance::OdDgECPropertyInstanceType type = value.getType();

  if( type == OdDgECPropertyInstance::kECClassInstance )
  {
    curVal = value.getClassInstanceValue()->getPropertyInstance(uItemIndex);
    type   = curVal.getType();
  }

  switch( type )
  {
    case OdDgECPropertyInstance::kBool:
    {
      curVal.getBoolValue() ? strRet = L"true" : strRet = L"false";
    } break;

    case OdDgECPropertyInstance::kByte:
    {
      strRet.Format(L"%d", curVal.getByteValue() );
    } break;

    case OdDgECPropertyInstance::kShort:
    {
      strRet.Format(L"%d", curVal.getShortValue() );
    } break;

    case OdDgECPropertyInstance::kInt32:
    {
      strRet.Format(L"%d", curVal.getIntValue() );
    } break;

    case OdDgECPropertyInstance::kInt64:
    {
      strRet.Format(L"%u", curVal.getInt64Value() );
    } break;

    case OdDgECPropertyInstance::kDouble:
    {
      strRet.Format(L"%f", curVal.getDoubleValue() );
    } break;

    case OdDgECPropertyInstance::kString:
    {
      strRet = curVal.getStringValue().c_str();
    } break;

    case OdDgECPropertyInstance::kPoint3d:
    {
      OdGePoint3d pt3d = curVal.getPoint3dValue();
      strRet.Format(L"(%f,%f,%f)", pt3d.x, pt3d.y, pt3d.z );
    } break;
    
    case OdDgECPropertyInstance::kDateAndTime:
    {
      OdString strVal;
      curVal.getDateTimeValue().strftime(L"%d.%m.%Y %H:%M:%S", strVal);
      strRet = strVal.c_str();
    } break;
  }

  return strRet;
}

//=================================================================================

void CItemTypeAttachDlg::updatePropertyListData()
{
  int iOldIndex = m_iPropIndex;

  CListCtrl* pPropList = (CListCtrl*)GetDlgItem(IDC_PROPERTIES_LIST);
  pPropList->DeleteAllItems();

  m_iPropIndex = -1;

  if( !m_arrListDescriptor.isEmpty() )
  {
    for( int i = 0; i < (int)m_arrListDescriptor.size(); i++ )
    {
      pPropList->InsertItem(i, convertECSchemaItemNameToString(m_arrListDescriptor[i].m_classRef.getSchemaName()) );
      pPropList->SetItemText(i, 1, convertECSchemaItemNameToString(m_arrListDescriptor[i].m_classRef.getClassName()) );
      pPropList->SetItemText(i, 2, createPropName(convertECSchemaItemNameToString(m_arrListDescriptor[i].m_value.getName()), m_arrListDescriptor[i].m_value, m_arrListDescriptor[i].m_uSubpropIndex) );
      pPropList->SetItemText(i, 3, valueTypeToStr(m_arrListDescriptor[i].m_value, m_arrListDescriptor[i].m_uSubpropIndex));
      pPropList->SetItemText(i, 4, valueDataToStr(m_arrListDescriptor[i].m_value, m_arrListDescriptor[i].m_uSubpropIndex) );
    }

    if( iOldIndex >= 0 && iOldIndex < pPropList->GetItemCount() )
    {
      m_iPropIndex = iOldIndex;
      pPropList->SetSelectionMark(iOldIndex);
    }
    else if( pPropList->GetItemCount() > 0 )
      m_iPropIndex = 0;
    else
      m_iPropIndex = -1;
  }
}

//=================================================================================

void CItemTypeAttachDlg::createDefaultInstance( const OdDgElementPtr& pLib,
                                                  const OdDgItemTypePropertyPtr& pProp, 
                                                    OdDgECPropertyInstance& inst )
{
  inst.setByDefault(pProp, pLib->database());
}

//=================================================================================

void CItemTypeAttachDlg::OnAttachBtnClicked()
{
  if( m_idItemTypeLib.isNull() || (m_iItemType < 0) )
    return;

  if( m_idElement.isNull() || !m_pDb )
    return;

  OdDgGraphicsElementPtr pElm = m_idElement.openObject(OdDg::kForRead);
  OdDgECClassInstanceElementPEPtr pItemTypePE = pElm;

  OdDgElementPtr pItem = m_idItemTypeLib.openObject(OdDg::kForRead);

  if (!pItem->isKindOf(OdDgItemTypeLibrary::desc()))
    return;

  OdDgItemTypeLibraryPtr pItemTypeLib = m_idItemTypeLib.openObject(OdDg::kForRead);
  CComboBox* pItemTypeCombo = (CComboBox*)(GetDlgItem(IDC_ITEM_TYPE_COMBO));
  CString strCItemTypeName;
  pItemTypeCombo->GetLBText(m_iItemType, strCItemTypeName );
  OdString strItemName = convertStringToECSchemaItemName(strCItemTypeName.GetBuffer(strCItemTypeName.GetLength()));
  strCItemTypeName.ReleaseBuffer();

  OdDgItemTypePtr pItemType = pItemTypeLib->getItemType(strItemName);

  if( !pItemType.isNull() && (pItemType->getPropertyCount() > 0) )
  {
    OdDgECClassInstancePtr pItemInst = OdDgECClassInstance::createObject(pItemType->getClassReference(), pItemTypeLib->database(), false);

    for( OdUInt32 i = 0; i < pItemType->getPropertyCount(); i++ )
    {
      OdDgItemTypePropertyPtr pProp = pItemType->getProperty(i);
      OdDgECPropertyInstance inst;
      createDefaultInstance( pItemTypeLib, pProp, inst );
      pItemInst->addPropertyInstance( inst );
    }

    pItemTypePE->addECClassInstance(pElm, pItemInst);
  }

  initListDescriptor();
  updatePropertyListData();
  updateItemTypeCtrls();
  updatePropertyCtrls();
}

//=================================================================================

void CItemTypeAttachDlg::convertDescriptionsToInstances( OdDgDatabase* pDb,
                                                           const OdArray<CItemTypeValueDescriptor>& arrDescriptors, 
                                                             OdArray<OdDgECClassInstancePtr>& arrInsts 
                                                        )
{
  if( !pDb )
    return;

  OdDgTagDefinitionSetTablePtr pTagSetTable = pDb->getTagDefinitionSetTable(OdDg::kForRead);

  for( OdUInt32 i = 0; i < arrDescriptors.size(); i++ )
  {
    if( arrDescriptors[i].m_uSubpropIndex > 0 )
      continue;

    OdDgElementId idItemLib = OdDgECSchema::getECSchemaIdByReference( arrDescriptors[i].m_classRef.getSchemaReference(), pDb);

    bool bCreateNewItem = true;

    for( OdUInt32 j = 0; j < arrInsts.size(); j++ )
    {
      if( ( (arrInsts[j]->getSchemaName() == arrDescriptors[i].m_classRef.getSchemaName() ) ||
            (!idItemLib.isNull() && ( arrInsts[j]->getECSchemaId() == idItemLib.getHandle() ))
          ) && (arrInsts[j]->getClassName() == arrDescriptors[i].m_classRef.getClassName())
        )
      {
        bCreateNewItem = false;

        bool bAddProperty = true;

        for( OdUInt32 k = 0; k < arrInsts[j]->getPropertyInstanceCount(); k++ )
        {
          OdDgECPropertyInstance prop = arrInsts[j]->getPropertyInstance(k);

          if( prop.getName() == arrDescriptors[i].m_value.getName() )
          {
            bAddProperty = false;
            break;
          }
        }

        if( bAddProperty )
        {
          arrInsts[j]->addPropertyInstance( arrDescriptors[i].m_value );
        }

        break;
      }
    }

    if( bCreateNewItem )
    {
      OdDgECClassInstancePtr pNewItem = OdDgECClassInstance::createObject(arrDescriptors[i].m_classRef, pDb, false);
      pNewItem->addPropertyInstance( arrDescriptors[i].m_value );

      arrInsts.push_back( pNewItem );
    }
  }
}

//=================================================================================

void CItemTypeAttachDlg::OnDetachBtnClicked()
{
  if( m_idItemTypeLib.isNull() || (m_iItemType < 0) )
    return;

  OdDgElementPtr pItem = m_idItemTypeLib.openObject(OdDg::kForRead);

  if (!pItem->isKindOf(OdDgItemTypeLibrary::desc()))
    return;

  OdDgItemTypeLibraryPtr pItemTypeLib = m_idItemTypeLib.openObject(OdDg::kForRead);
  OdString strLibName  = pItemTypeLib->getReferenceToSchema().getName();
  CComboBox* pItemTypeCombo = (CComboBox*)(GetDlgItem(IDC_ITEM_TYPE_COMBO));
  CString strCItemTypeName;
  pItemTypeCombo->GetLBText(m_iItemType, strCItemTypeName );
  OdString strItemName = convertStringToECSchemaItemName(strCItemTypeName.GetBuffer(strCItemTypeName.GetLength()));
  strCItemTypeName.ReleaseBuffer();

  // Update description list.

  OdUInt32 iCount = 0;

  while( iCount < m_arrListDescriptor.size() )
  {
    if( (m_arrListDescriptor[iCount].m_classRef.getSchemaName() == strLibName) &&
        (m_arrListDescriptor[iCount].m_classRef.getClassName() == strItemName )
      )
      m_arrListDescriptor.removeAt(iCount);
    else
      iCount++;
  }

  // Update X-Attribute.

  if( !m_idElement.isNull() && m_pDb )
  {
    OdDgGraphicsElementPtr pElm = m_idElement.openObject(OdDg::kForRead);

    OdDgECClassInstanceElementPEPtr pItemTypePE = pElm;

    // Remove All item types

    while( pItemTypePE->getECClassInstanceCount(pElm) )
      pItemTypePE->deleteECClassInstance(pElm, 0);

    // Restore item types from description.

    OdArray<OdDgECClassInstancePtr> arrInsts;

    convertDescriptionsToInstances( m_pDb, m_arrListDescriptor, arrInsts );

    for( OdUInt32 i = 0; i < arrInsts.size(); i++ )
      pItemTypePE->addECClassInstance( pElm, arrInsts[i] );
  }

  // Update Controls.

  updatePropertyListData();
  updateItemTypeCtrls();
}

//=================================================================================

void CItemTypeAttachDlg::OnLibBtnClicked()
{
  CItemTypeLibsDlg dlg(NULL,m_pDb);
  dlg.DoModal();

  initListDescriptor();
  initItemTypeLibraryCombo();
}

#undef  ERASE_ARRAY_VALUE
#define ERASE_ARRAY_VALUE(typeName, name, valueName)            \
{                                                               \
OdArray<typeName> arrData = valueName.get##name##ArrayValue();  \
if( iArrayIndex <= (int)arrData.size() )                        \
{                                                               \
  arrData.removeAt(iArrayIndex);                                \
  bRet = true;                                                  \
}                                                               \
valueName.set##name##ArrayValue( arrData );                     \
}

//=================================================================================

bool CItemTypeAttachDlg::delItemPropertyArrayValue( OdDgECPropertyInstance& value, 
                                                      OdUInt32 uSubPropertyIndex, 
                                                        int iArrayIndex 
                                                  )
{
  bool bRet = false;

  if( value.getType() == OdDgECPropertyInstance::kECClassInstanceArray || iArrayIndex < 0 )
    return bRet;

  bool bComplexProp = false;

  OdDgECPropertyInstance::OdDgECPropertyInstanceType valType = value.getType();

  if( (valType == OdDgECPropertyInstance::kECClassInstance) &&
      (value.getClassInstanceValue()->getPropertyInstanceCount() > uSubPropertyIndex)
    )
  {
    valType = value.getClassInstanceValue()->getPropertyInstance(uSubPropertyIndex).getType();
    bComplexProp = true;
  }

  if( bComplexProp )
  {
    OdDgECClassInstancePtr pSubClassInstance = value.getClassInstanceValue();

    OdDgECPropertyInstance curVal = pSubClassInstance->getPropertyInstance(uSubPropertyIndex);

    switch( valType )
    {
      case OdDgECPropertyInstance::kBoolArray:     ERASE_ARRAY_VALUE(bool,Bool,curVal); break;
      case OdDgECPropertyInstance::kByteArray:     ERASE_ARRAY_VALUE(OdInt8,Byte,curVal); break;
      case OdDgECPropertyInstance::kShortArray:    ERASE_ARRAY_VALUE(OdInt16,Short,curVal); break;
      case OdDgECPropertyInstance::kInt32Array:    ERASE_ARRAY_VALUE(OdInt32,Int,curVal); break;
      case OdDgECPropertyInstance::kInt64Array:    ERASE_ARRAY_VALUE(OdInt64,Int64,curVal); break;
      case OdDgECPropertyInstance::kStringArray:   ERASE_ARRAY_VALUE(OdString,String,curVal); break;
      case OdDgECPropertyInstance::kDoubleArray:   ERASE_ARRAY_VALUE(double,Double,curVal); break;
      case OdDgECPropertyInstance::kDateTimeArray: ERASE_ARRAY_VALUE(OdTimeStamp,DateTime,curVal); break;
      case OdDgECPropertyInstance::kPoint3dArray:  ERASE_ARRAY_VALUE(OdGePoint3d,Point3d,curVal); break;
    }

    pSubClassInstance->setPropertyInstance(uSubPropertyIndex, curVal);
    value.setClassInstanceValue( pSubClassInstance );
  }
  else
  {
    switch( valType )
    {
      case OdDgECPropertyInstance::kBoolArray:     ERASE_ARRAY_VALUE(bool,Bool,value); break;
      case OdDgECPropertyInstance::kByteArray:     ERASE_ARRAY_VALUE(OdInt8,Byte,value); break;
      case OdDgECPropertyInstance::kShortArray:    ERASE_ARRAY_VALUE(OdInt16,Short,value); break;
      case OdDgECPropertyInstance::kInt32Array:    ERASE_ARRAY_VALUE(OdInt32,Int,value); break;
      case OdDgECPropertyInstance::kInt64Array:    ERASE_ARRAY_VALUE(OdInt64,Int64,value); break;
      case OdDgECPropertyInstance::kStringArray:   ERASE_ARRAY_VALUE(OdString,String,value); break;
      case OdDgECPropertyInstance::kDoubleArray:   ERASE_ARRAY_VALUE(double,Double,value); break;
      case OdDgECPropertyInstance::kDateTimeArray: ERASE_ARRAY_VALUE(OdTimeStamp,DateTime,value); break;
      case OdDgECPropertyInstance::kPoint3dArray:  ERASE_ARRAY_VALUE(OdGePoint3d,Point3d,value); break;
    }
  }

  return bRet;
}

//=================================================================================

bool CItemTypeAttachDlg::setItemPropertyValue( const CString& strValue,
                                                 OdDgECPropertyInstance& value,
                                                   OdUInt32 uSubPropertyIndex,
                                                     int iArrayIndex
                                             )
{
  bool bRet = false;

  if( value.getType() == OdDgECPropertyInstance::kECClassInstanceArray )
    return bRet;

  SET_LOCALE( LC_NUMERIC, "C" );

  OdString strVal = strValue;
  bool bComplexProp = false;

  OdDgECPropertyInstance::OdDgECPropertyInstanceType valType = value.getType();

  if( (valType == OdDgECPropertyInstance::kECClassInstance) &&
      (value.getClassInstanceValue()->getPropertyInstanceCount() > uSubPropertyIndex)
    )
  {
    valType = value.getClassInstanceValue()->getPropertyInstance(uSubPropertyIndex).getType();
    bComplexProp = true;
  }

  switch( valType )
  {
    case OdDgECPropertyInstance::kByte:
    case OdDgECPropertyInstance::kShort:
    case OdDgECPropertyInstance::kInt32:
    case OdDgECPropertyInstance::kInt64:
    {
      try
      {
        OdInt64 iValue = wcstol( strVal.c_str(), 0, 10 );

        if( bComplexProp )
        {
          OdDgECClassInstancePtr pSubClassInstance = value.getClassInstanceValue();
          OdDgECPropertyInstance curVal = pSubClassInstance->getPropertyInstance(uSubPropertyIndex);

          switch( valType )
          {
            case OdDgECPropertyInstance::kByte:  curVal.setByteValue( (OdInt8)iValue ); break;
            case OdDgECPropertyInstance::kShort: curVal.setShortValue( (OdInt16)iValue ); break;
            case OdDgECPropertyInstance::kInt32: curVal.setIntValue( (OdInt32)iValue ); break;
            case OdDgECPropertyInstance::kInt64: curVal.setInt64Value( iValue ); break;
          }

          pSubClassInstance->setPropertyInstance(uSubPropertyIndex, curVal);
          value.setClassInstanceValue(pSubClassInstance);
        }
        else
        {
          switch( valType )
          {
            case OdDgECPropertyInstance::kByte:  value.setByteValue( (OdInt8)iValue ); break;
            case OdDgECPropertyInstance::kShort: value.setShortValue( (OdInt16)iValue ); break;
            case OdDgECPropertyInstance::kInt32: value.setIntValue( (OdInt32)iValue ); break;
            case OdDgECPropertyInstance::kInt64: value.setInt64Value( iValue ); break;
          }
        }

        bRet = true;
      }
      catch(...)
      {
        bRet = false;
      }
    } break;

    case OdDgECPropertyInstance::kByteArray:
    case OdDgECPropertyInstance::kShortArray:
    case OdDgECPropertyInstance::kInt32Array:
    case OdDgECPropertyInstance::kInt64Array:
    {
      try
      {
        OdInt64 iValue = wcstol( strVal.c_str(), 0, 10 );

        if( bComplexProp )
        {
          OdDgECClassInstancePtr pSubClassInstance = value.getClassInstanceValue();
          OdDgECPropertyInstance curVal = pSubClassInstance->getPropertyInstance(uSubPropertyIndex);

          switch( valType )
          {
            case OdDgECPropertyInstance::kByteArray:
            {
              OdArray<OdInt8> byteArray = curVal.getByteArrayValue();

              if( iArrayIndex < 0 || iArrayIndex >= (int)byteArray.size() )
                byteArray.push_back(OdInt8(iValue));
              else
                byteArray[iArrayIndex] = OdInt8(iValue);

              curVal.setByteArrayValue(byteArray);
            } break;

            case OdDgECPropertyInstance::kShortArray:
            {
              OdArray<OdInt16> shortArray = curVal.getShortArrayValue();

              if( iArrayIndex < 0 || iArrayIndex >= (int)shortArray.size() )
                shortArray.push_back(OdInt16(iValue));
              else
                shortArray[iArrayIndex] = OdInt16(iValue);

              curVal.setShortArrayValue(shortArray);
            } break;

            case OdDgECPropertyInstance::kInt32Array:
            {
              OdArray<OdInt32> intArray = curVal.getIntArrayValue();

              if( iArrayIndex < 0 || iArrayIndex >= (int)intArray.size() )
                intArray.push_back(OdInt32(iValue));
              else
                intArray[iArrayIndex] = OdInt32(iValue);

              curVal.setIntArrayValue(intArray);
            } break;

            case OdDgECPropertyInstance::kInt64Array:
            {
              OdArray<OdInt64> int64Array = curVal.getInt64ArrayValue();

              if( iArrayIndex < 0 || iArrayIndex >= (int)int64Array.size() )
                int64Array.push_back(iValue);
              else
                int64Array[iArrayIndex] = iValue;

              curVal.setInt64ArrayValue(int64Array);
            } break;
          }

          pSubClassInstance->setPropertyInstance(uSubPropertyIndex, curVal);
          value.setClassInstanceValue(pSubClassInstance);
        }
        else
        {
          switch( valType )
          {
            case OdDgECPropertyInstance::kByteArray:
            {
              OdArray<OdInt8> byteArray = value.getByteArrayValue();

              if( iArrayIndex < 0 || iArrayIndex >= (int)byteArray.size() )
                byteArray.push_back(OdInt8(iValue));
              else
                byteArray[iArrayIndex] = OdInt8(iValue);

              value.setByteArrayValue(byteArray);
            } break;

            case OdDgECPropertyInstance::kShortArray:
            {
              OdArray<OdInt16> shortArray = value.getShortArrayValue();

              if( iArrayIndex < 0 || iArrayIndex >= (int)shortArray.size() )
                shortArray.push_back(OdInt16(iValue));
              else
                shortArray[iArrayIndex] = OdInt16(iValue);

              value.setShortArrayValue(shortArray);
            } break;

            case OdDgECPropertyInstance::kInt32Array:
            {
              OdArray<OdInt32> intArray = value.getIntArrayValue();

              if( iArrayIndex < 0 || iArrayIndex >= (int)intArray.size() )
                intArray.push_back(OdInt32(iValue));
              else
                intArray[iArrayIndex] = OdInt32(iValue);

              value.setIntArrayValue(intArray);
            } break;

            case OdDgECPropertyInstance::kInt64Array:
            {
              OdArray<OdInt64> int64Array = value.getInt64ArrayValue();

              if( iArrayIndex < 0 || iArrayIndex >= (int)int64Array.size() )
                int64Array.push_back(iValue);
              else
                int64Array[iArrayIndex] = iValue;

              value.setInt64ArrayValue(int64Array);
            } break;
          }
        }

        bRet = true;
      }
      catch(...)
      {
        bRet = false;
      }
    } break;

    case OdDgECPropertyInstance::kBool:
    {
      strVal.makeUpper();

      bool bValue = false;

      if( strVal == L"TRUE" )
        bValue = true;

      if( bComplexProp )
      {
        OdDgECClassInstancePtr pSubClassInstance = value.getClassInstanceValue();
        OdDgECPropertyInstance curVal = pSubClassInstance->getPropertyInstance(uSubPropertyIndex);

        curVal.setBoolValue(bValue);

        pSubClassInstance->setPropertyInstance(uSubPropertyIndex, curVal);
        value.setClassInstanceValue(pSubClassInstance);
      }
      else
        value.setBoolValue( bValue );

      bRet = true;
    } break;

    case OdDgECPropertyInstance::kBoolArray:
    {
      strVal.makeUpper();

      bool bValue = false;

      if( strVal == L"TRUE" )
        bValue = true;

      if( bComplexProp )
      {
        OdDgECClassInstancePtr pSubClassInstance = value.getClassInstanceValue();
        OdDgECPropertyInstance curVal = pSubClassInstance->getPropertyInstance(uSubPropertyIndex);

        OdArray<bool> boolArray = curVal.getBoolArrayValue();

        if( iArrayIndex < 0 || iArrayIndex >= (int)boolArray.size() )
          boolArray.push_back(bValue);
        else
          boolArray[iArrayIndex] = bValue;

        curVal.setBoolArrayValue(boolArray);

        pSubClassInstance->setPropertyInstance(uSubPropertyIndex, curVal);
        value.setClassInstanceValue(pSubClassInstance);
      }
      else
      {
        OdArray<bool> boolArray = value.getBoolArrayValue();

        if( iArrayIndex < 0 || iArrayIndex >= (int)boolArray.size() )
          boolArray.push_back(bValue);
        else
          boolArray[iArrayIndex] = bValue;

        value.setBoolArrayValue( boolArray );
      }

      bRet = true;
    } break;

    case OdDgECPropertyInstance::kString:
    {
      if( bComplexProp )
      {
        OdDgECClassInstancePtr pSubClassInstance = value.getClassInstanceValue();
        OdDgECPropertyInstance curVal = pSubClassInstance->getPropertyInstance(uSubPropertyIndex);

        curVal.setStringValue(strVal);

        pSubClassInstance->setPropertyInstance(uSubPropertyIndex, curVal);
        value.setClassInstanceValue(pSubClassInstance);
      }
      else
        value.setStringValue( strVal );

      bRet = true;
    } break;

    case OdDgECPropertyInstance::kStringArray:
    {
      if( bComplexProp )
      {
        OdDgECClassInstancePtr pSubClassInstance = value.getClassInstanceValue();
        OdDgECPropertyInstance curVal = pSubClassInstance->getPropertyInstance(uSubPropertyIndex);

        OdArray<OdString> strArray = curVal.getStringArrayValue();

        if( iArrayIndex < 0 || iArrayIndex >= (int)strArray.size() )
          strArray.push_back(strVal);
        else
          strArray[iArrayIndex] = strValue;

        curVal.setStringArrayValue(strArray);

        pSubClassInstance->setPropertyInstance(uSubPropertyIndex, curVal);
        value.setClassInstanceValue(pSubClassInstance);
      }
      else
      {
        OdArray<OdString> strArray = value.getStringArrayValue();

        if( iArrayIndex < 0 || iArrayIndex >= (int)strArray.size() )
          strArray.push_back(strVal);
        else
          strArray[iArrayIndex] = strValue;

        value.setStringArrayValue( strArray );
      }

      bRet = true;
    } break;

    case OdDgECPropertyInstance::kDouble:
    {
      try
      {
        strVal.replace(L',', L'.');
        OdChar* pNextDouble;
        double dValue = wcstod( strVal.c_str(), &pNextDouble );

        if( bComplexProp )
        {
          OdDgECClassInstancePtr pSubClassInstance = value.getClassInstanceValue();
          OdDgECPropertyInstance curVal = pSubClassInstance->getPropertyInstance(uSubPropertyIndex);

          curVal.setDoubleValue(dValue);

          pSubClassInstance->setPropertyInstance(uSubPropertyIndex, curVal);
          value.setClassInstanceValue(pSubClassInstance);
        }
        else
          value.setDoubleValue( dValue );

        bRet = true;
      }
      catch(...)
      {
        bRet = false;
      }
    } break;

    case OdDgECPropertyInstance::kDoubleArray:
    {
      try
      {
        strVal.replace(L',', L'.');
        OdChar* pNextDouble;
        double dValue = wcstod( strVal.c_str(), &pNextDouble );

        if( bComplexProp )
        {
          OdDgECClassInstancePtr pSubClassInstance = value.getClassInstanceValue();
          OdDgECPropertyInstance curVal = pSubClassInstance->getPropertyInstance(uSubPropertyIndex);

          OdArray<double> dblArray = curVal.getDoubleArrayValue();

          if( iArrayIndex < 0 || iArrayIndex >= (int)dblArray.size() )
            dblArray.push_back(dValue);
          else
            dblArray[iArrayIndex] = dValue;

          curVal.setDoubleArrayValue(dblArray);

          pSubClassInstance->setPropertyInstance(uSubPropertyIndex, curVal);
          value.setClassInstanceValue(pSubClassInstance);
        }
        else
        {
          OdArray<double> dblArray = value.getDoubleArrayValue();

          if( iArrayIndex < 0 || iArrayIndex >= (int)dblArray.size() )
            dblArray.push_back(dValue);
          else
            dblArray[iArrayIndex] = dValue;

          value.setDoubleArrayValue( dblArray );
        }

        bRet = true;
      }
      catch(...)
      {
      	bRet = false;
      }
    } break;

    case OdDgECPropertyInstance::kDateAndTime:
    case OdDgECPropertyInstance::kDateTimeArray:
    {
      try
      {
        strVal.trimLeft();
        strVal.trimRight();
        strVal.replace(L',', L'.');
        strVal.replace(L'/', L'.');
        strVal.replace(L'\\', L'.');
        strVal.replace(L':', L'.');
        strVal.replace(L' ', L'.');

        OdString strDay, strMonth, strYear, strHour, strMinute, strSec;

        strDay    = CItemTypeLibsDlg::getPartOfDate( strVal );
        strMonth  = CItemTypeLibsDlg::getPartOfDate( strVal );
        strYear   = CItemTypeLibsDlg::getPartOfDate( strVal );
        strHour   = CItemTypeLibsDlg::getPartOfDate( strVal );
        strMinute = CItemTypeLibsDlg::getPartOfDate( strVal );
        strSec    = CItemTypeLibsDlg::getPartOfDate( strVal );

        short uDay    = wcstol( strDay.c_str(), 0, 10 );
        short uMonth  = wcstol( strMonth.c_str(), 0, 10 );
        short uYear   = wcstol( strYear.c_str(), 0, 10 );
        short uHour   = wcstol( strHour.c_str(), 0, 10 );
        short uMinute = wcstol( strMinute.c_str(), 0, 10 );
        short uSec    = wcstol( strSec.c_str(), 0, 10 );

        OdTimeStamp timeVal;
        timeVal.setDate( uMonth, uDay, uYear );
        timeVal.setTime( uHour, uMinute, uSec, 0 );

        if( bComplexProp )
        {
          OdDgECClassInstancePtr pSubClassInstance = value.getClassInstanceValue();
          OdDgECPropertyInstance curVal = pSubClassInstance->getPropertyInstance(uSubPropertyIndex);

          if( valType == OdDgECPropertyInstance::kDateAndTime )
          {
            curVal.setDateTimeValue(timeVal);
          }
          else
          {
            OdArray<OdTimeStamp> timeArray = curVal.getDateTimeArrayValue();

            if( iArrayIndex < 0 || iArrayIndex >= (int)timeArray.size() )
              timeArray.push_back(timeVal);
            else
              timeArray[iArrayIndex] = timeVal;

            curVal.setDateTimeArrayValue( timeArray );
          }

          pSubClassInstance->setPropertyInstance(uSubPropertyIndex, curVal);
          value.setClassInstanceValue(pSubClassInstance);
        }
        else
        {
          if( valType == OdDgECPropertyInstance::kDateAndTime )
          {
            value.setDateTimeValue(timeVal);
          }
          else
          {
            OdArray<OdTimeStamp> timeArray = value.getDateTimeArrayValue();

            if( iArrayIndex < 0 || iArrayIndex >= (int)timeArray.size() )
              timeArray.push_back(timeVal);
            else
              timeArray[iArrayIndex] = timeVal;

            value.setDateTimeArrayValue( timeArray );
          }
        }

        bRet = true;
      }
      catch(...)
      {
        bRet = false;
      }
    } break;

    case OdDgECPropertyInstance::kPoint3d:
    case OdDgECPropertyInstance::kPoint3dArray:
    {
      try
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
        OdGePoint3d ptVal(dX,dY,dZ);

        if( bComplexProp )
        {
          OdDgECClassInstancePtr pSubClassInstance = value.getClassInstanceValue();
          OdDgECPropertyInstance curVal = pSubClassInstance->getPropertyInstance(uSubPropertyIndex);

          if( valType == OdDgECPropertyInstance::kPoint3d )
          {
            curVal.setPoint3dValue(ptVal);
          }
          else
          {
            OdArray<OdGePoint3d> ptsArray = curVal.getPoint3dArrayValue();

            if( iArrayIndex < 0 || iArrayIndex >= (int)ptsArray.size() )
              ptsArray.push_back(ptVal);
            else
              ptsArray[iArrayIndex] = ptVal;

            curVal.setPoint3dArrayValue( ptsArray );
          }

          pSubClassInstance->setPropertyInstance(uSubPropertyIndex, curVal);
          value.setClassInstanceValue(pSubClassInstance);
        }
        else
        {
          if( valType == OdDgECPropertyInstance::kPoint3d )
          {
            value.setPoint3dValue(ptVal);
          }
          else
          {
            OdArray<OdGePoint3d> ptsArray = value.getPoint3dArrayValue();

            if( iArrayIndex < 0 || iArrayIndex >= (int)ptsArray.size() )
              ptsArray.push_back(ptVal);
            else
              ptsArray[iArrayIndex] = ptVal;

            value.setPoint3dArrayValue( ptsArray );
          }
        }

        bRet = true;
      }
      catch(...)
      {
        bRet = false;
      }
    } break;
  }

  RESET_LOCALE();

  return bRet;
}

//=================================================================================

void CItemTypeAttachDlg::OnArrAddBtnClicked()
{
  UpdateData(TRUE);

  if( m_iPropIndex >= 0 )
  {
    if( setItemPropertyValue(m_strArrVal, m_arrListDescriptor[m_iPropIndex].m_value, m_arrListDescriptor[m_iPropIndex].m_uSubpropIndex, -1) )
    {
      // Synchronize complex properties records.

      synchronizeComplexPropDesc( m_arrListDescriptor, m_iPropIndex );

      // Update Element Data.

      if( !m_idElement.isNull() && m_pDb )
      {
        OdDgGraphicsElementPtr pElm = m_idElement.openObject(OdDg::kForRead);

        OdDgECClassInstanceElementPEPtr pItemTypePE = pElm;

        // Remove All item types

        while( pItemTypePE->getECClassInstanceCount(pElm) )
          pItemTypePE->deleteECClassInstance(pElm, 0);

        // Restore item types from description.

        OdArray<OdDgECClassInstancePtr> arrInsts;

        convertDescriptionsToInstances( m_pDb, m_arrListDescriptor, arrInsts );

        for( OdUInt32 i = 0; i < arrInsts.size(); i++ )
          pItemTypePE->addECClassInstance( pElm, arrInsts[i] );
      }

      // Update Ctrl Data.
      
      CListCtrl* pList = (CListCtrl*)(GetDlgItem(IDC_ARRAY_LIST));
      pList->InsertItem( pList->GetItemCount(), m_strArrVal);
      pList->SetSelectionMark( pList->GetItemCount() - 1 );
    }
  }
}

//=================================================================================

void CItemTypeAttachDlg::OnArrDelBtnClicked()
{
  UpdateData(TRUE);

  if( m_iPropIndex >= 0 && m_iArrIndex >= 0 )
  {
    if( delItemPropertyArrayValue( m_arrListDescriptor[m_iPropIndex].m_value, m_arrListDescriptor[m_iPropIndex].m_uSubpropIndex, m_iArrIndex) )
    {
      // Synchronize complex properties records.

      synchronizeComplexPropDesc( m_arrListDescriptor, m_iPropIndex );

      // Update Element Data.

      if( !m_idElement.isNull() && m_pDb )
      {
        OdDgGraphicsElementPtr pElm = m_idElement.openObject(OdDg::kForRead);

        OdDgECClassInstanceElementPEPtr pItemTypePE = pElm;

        // Remove All item types

        while( pItemTypePE->getECClassInstanceCount(pElm) )
          pItemTypePE->deleteECClassInstance(pElm, 0);

        // Restore item types from description.

        OdArray<OdDgECClassInstancePtr> arrInsts;

        convertDescriptionsToInstances( m_pDb, m_arrListDescriptor, arrInsts );

        for( OdUInt32 i = 0; i < arrInsts.size(); i++ )
          pItemTypePE->addECClassInstance( pElm, arrInsts[i] );
      }

      // Update Ctrl Data.

      updatePropertyCtrls();
    }
  }
}

//=================================================================================

void CItemTypeAttachDlg::OnEditBtnClicked()
{
  UpdateData(TRUE);

  if( m_iPropIndex >= 0 )
  {
    CString strValue = m_strSimpleVal;

    switch( m_arrListDescriptor[m_iPropIndex].m_value.getType() )
    {
      case OdDgECPropertyInstance::kBoolArray:
      case OdDgECPropertyInstance::kByteArray:
      case OdDgECPropertyInstance::kShortArray:
      case OdDgECPropertyInstance::kInt32Array:
      case OdDgECPropertyInstance::kInt64Array:
      case OdDgECPropertyInstance::kDoubleArray:
      case OdDgECPropertyInstance::kStringArray:
      case OdDgECPropertyInstance::kPoint3dArray:
      case OdDgECPropertyInstance::kDateTimeArray:
        strValue = m_strArrVal;
      break;
    }

    if( setItemPropertyValue(strValue, m_arrListDescriptor[m_iPropIndex].m_value, m_arrListDescriptor[m_iPropIndex].m_uSubpropIndex, m_iArrIndex) )
    {
      // Synchronize complex properties records.

      synchronizeComplexPropDesc( m_arrListDescriptor, m_iPropIndex );

      // Update Element Data.

      if( !m_idElement.isNull() && m_pDb )
      {
        OdDgGraphicsElementPtr pElm = m_idElement.openObject(OdDg::kForWrite);

        OdDgECClassInstanceElementPEPtr pItemTypePE = pElm;

        // Remove All item types

        while( pItemTypePE->getECClassInstanceCount(pElm) )
          pItemTypePE->deleteECClassInstance(pElm, 0);

        // Restore item types from description.

        OdArray<OdDgECClassInstancePtr> arrInsts;

        convertDescriptionsToInstances( m_pDb, m_arrListDescriptor, arrInsts );

        for( OdUInt32 i = 0; i < arrInsts.size(); i++ )
          pItemTypePE->addECClassInstance( pElm, arrInsts[i] );
      }

      // Update Ctrl Data.

      updatePropertyListData();
      updatePropertyCtrls();
    }
  }
}

//=================================================================================

void    CItemTypeAttachDlg::synchronizeComplexPropDesc( OdArray<CItemTypeValueDescriptor>& arrDescriptors, 
                                                          OdUInt32 iPropIndex 
                                                      )
{
  if( arrDescriptors[iPropIndex].m_value.getType() != OdDgECPropertyInstance::kECClassInstance )
    return;

  for( OdUInt32 i = 0; i < arrDescriptors.size(); i++ )
  {
    if( i == iPropIndex )
      continue;

    if( (arrDescriptors[i].m_classRef.getClassName() == arrDescriptors[iPropIndex].m_classRef.getClassName()) && 
        (arrDescriptors[i].m_classRef.getSchemaName() == arrDescriptors[iPropIndex].m_classRef.getSchemaName())  && 
        (arrDescriptors[i].m_value.getName() == arrDescriptors[iPropIndex].m_value.getName()) 
      )
      arrDescriptors[i].m_value = arrDescriptors[iPropIndex].m_value;
  }
}

//=================================================================================
