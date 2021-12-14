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
// CSwitchModel.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "CSwitchModel.h"


// CSwitchModel dialog

IMPLEMENT_DYNAMIC(CSwitchModel, CDialog)

CSwitchModel::CSwitchModel(CWnd* pParent /*=NULL*/)
	: CDialog(CSwitchModel::IDD, pParent)
{

}

CSwitchModel::~CSwitchModel()
{
}

void CSwitchModel::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LIST2, m_listOfViewGroups);
}

CSwitchModel::CSwitchModel( OdDgDatabasePtr database, CWnd* pParent )
: CDialog( CSwitchModel::IDD, pParent ),
  m_database( database )
{
  if( m_database.isNull() )
  {
    throw OdError( eInvalidInput );
  }
}

BOOL CSwitchModel::OnInitDialog( )
{
  CDialog::OnInitDialog();

  //set the style with columns
  m_listOfViewGroups.SetExtendedStyle( LVS_REPORT );

  //create the correct columns
  {
    RECT rectangle;
    m_listOfViewGroups.GetClientRect( &rectangle );
    m_listOfViewGroups.InsertColumn( 0, L"View Group", LVCFMT_LEFT, rectangle.right / 2 );
    m_listOfViewGroups.InsertColumn( 1, L"Model", LVCFMT_LEFT, rectangle.right / 2, 1 );
  }

  //fill it with the correct information
  {
    OdDgViewGroupTablePtr viewGroupTable = m_database->getViewGroupTable();
    if( viewGroupTable.isNull() )
    {
      return TRUE;
    }

    OdDgElementIteratorPtr iterator = viewGroupTable->createIterator();
    OdDgViewGroupPtr viewGroup;
    OdDgElementId viewGroupId, modelId, activeViewGroupId = m_database->getActiveViewGroupId();
    OdDgModelPtr model;
    OdString viewGroupName, modelName;
    int itemIndex, activeIndex = -1;

    for( itemIndex = 0; !iterator->done(); iterator->step(), itemIndex++ )
    {
      viewGroupId = iterator->item();
      viewGroup = viewGroupId.openObject( OdDg::kForRead );
      viewGroupName = viewGroup->getName();

      modelId = viewGroup->getModelId();
      if( modelId.isNull() )
      {
        continue;
      }
      model = modelId.openObject( OdDg::kForRead );
      if( !model.isNull() )
      {
        modelName = model->getName();

        m_listOfViewGroups.InsertItem( itemIndex, viewGroupName.c_str() );
        m_listOfViewGroups.SetItemText( itemIndex, 1, modelName.c_str() );

        //check if this item is the current view group
        if( viewGroupId ==  activeViewGroupId )
        {
          activeIndex = itemIndex;
        }
      }
    }

    if( activeIndex != -1 )
    {
       m_listOfViewGroups.SetItemState( activeIndex, LVIS_SELECTED, LVIS_SELECTED );
    }
  }

  return TRUE;
}

BEGIN_MESSAGE_MAP(CSwitchModel, CDialog)
  ON_BN_CLICKED(IDOK, CSwitchModel::OnBnClickedOk)
END_MESSAGE_MAP()

void CSwitchModel::OnBnClickedOk()
{
  POSITION position = m_listOfViewGroups.GetFirstSelectedItemPosition();
  if( position )
  {
    int item = m_listOfViewGroups.GetNextSelectedItem( position );

    //try to change the active view group & model
    OdDgElementIteratorPtr iterator = m_database->getViewGroupTable()->createIterator();
    for( int i = 0; i < item; iterator->step(), i++ );
    OdDgViewGroupPtr viewGroup = iterator->item().openObject( OdDg::kForRead );

    m_database->setActiveViewGroupId( iterator->item() );
    m_database->setActiveModelId( viewGroup->getModelId() );
  }

  OnOK();
}
