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

#include "stdafx.h"
#include "OdaMfcAppDoc.h"
#include "OdaMfcApp.h"
#include "OdaMfcExportImpl.h"
#include "DwgViewer.h"

#ifdef ODAMFC_EXPORT_SYMBOL

ODRX_CONS_DEFINE_MEMBERS( OdApplicationReactor, OdRxObject, RXIMPL_CONSTR );
ODRX_NO_CONS_DEFINE_MEMBERS( OdApDocument, OdRxObject );

OdSmartPtr< OdApDocumentImpl > OdApDocumentImpl::createObject( CDocument* pDoc )
{
  OdSmartPtr< OdApDocumentImpl > pRes = OdRxObjectImpl< OdApDocumentImpl >::createObject();
  pRes->m_pImp = new MfcObjectWrapper< COdaMfcAppDoc >( (COdaMfcAppDoc*) pDoc );
  return pRes;
}  

OdApDocumentImpl::~OdApDocumentImpl()
{
  delete m_pImp;
}

OdString OdApDocumentImpl::fileName() const
{
  return (const wchar_t*) (*m_pImp)->GetPathName();
}

CDocument* OdApDocumentImpl::cDoc() const
{
  return (*m_pImp).get();
}

//AcTransactionManager* transactionManager() const = 0;

OdDbDatabasePtr OdApDocumentImpl::database() const
{
  return (*m_pImp)->m_pDb;
}

void OdApDocumentImpl::lockMode(bool bIncludeMyLocks ) const
{
}

void OdApDocumentImpl::myLockMode() const
{
}

bool OdApDocumentImpl::isQuiescent() const
{
  return false;
}

void* OdApDocumentImpl::contextPtr() const
{
  return 0;
}

OdEdBaseIO* OdApDocumentImpl::cmdIO()
{
  return (*m_pImp)->cmdIO();
}

OdDbCommandContextPtr OdApDocumentImpl::cmdCtx()
{
  return (*m_pImp)->cmdCtx();
}

void OdApDocumentImpl::ExecuteCommand(const OdString& sCmd, bool bEcho)
{
  (*m_pImp)->ExecuteCommand(sCmd, bEcho);
}

OdString OdApDocumentImpl::recentCmd()
{
  return (*m_pImp)->recentCmd();
}

OdDbSelectionSetPtr OdApDocumentImpl::selectionSet() const
{
  return (*m_pImp)->selectionSet();
}

OdApDocumentPtr odGetAppDocument( CDocument* pDoc )
{
  return static_cast< COdaMfcAppDoc* >( pDoc )->m_pRefDocument;
}

void odAddAppReactor( OdApplicationReactor* pReactor )
{
  COdaMfcAppApp* pApp = (COdaMfcAppApp*) AfxGetApp();
  pApp->AddReactor( pReactor );
}

OdGsLayoutHelperPtr odGetDocDevice( CDocument* pDoc )
{
  POSITION pos = pDoc->GetFirstViewPosition();
  while ( pos != 0 )
  {
    CView* pView = pDoc->GetNextView( pos );
    if ( pView->IsKindOf( RUNTIME_CLASS( CDwgViewer ) ) )
    {
      CDwgViewer* pViewer = (CDwgViewer*) pView;
      return pViewer->m_pDevice;
    }
  }
  return OdGsLayoutHelperPtr();
}

bool odGetDocOsnapPoint( CDocument* pDoc, OdGePoint3d& pt )
{
  POSITION pos = pDoc->GetFirstViewPosition();
  while ( pos != 0 )
  {
    CView* pView = pDoc->GetNextView( pos );
    if ( pView->IsKindOf( RUNTIME_CLASS( CDwgViewer ) ) )
    {
      CDwgViewer* pViewer = (CDwgViewer*) pView;
      return pViewer->editorObject().snap( pt, 0 );
    }
  }
  return false;
}

#endif // ODAMFC_EXPORT_SYMBOL
