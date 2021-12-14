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

// CRemoteGeomViewer.cpp : implementation file
//
#include "stdafx.h"
#include "RemoteGeomViewer.h"
#include "OdaMfcApp.h"
#include "OdaMfcAppDoc.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTable.h"
#include "DbBlockReference.h"
#include "remotegeomproxy.h"

#define ID_PROCESS_CMD _APS_NEXT_COMMAND_VALUE

/////////////////////////////////////////////////////////////////////////////
// CRemoteGeomViewer

IMPLEMENT_DYNCREATE(CRemoteGeomViewer, CDwgViewer)

BEGIN_MESSAGE_MAP(CRemoteGeomViewer, CDwgViewer)
	//{{AFX_MSG_MAP(CRemoteGeomViewer)
	ON_WM_DESTROY()
  ON_COMMAND(ID_PROCESS_CMD, processRequest)
  ON_UPDATE_COMMAND_UI(ID_DEBUG_GEOMETRY, OnUpdateDebugGeometry)
  ON_MESSAGE(WM_RGV_CHANGE_OUTPUT_STATE, OnRgvChangeOutputState)
  ON_MESSAGE(WM_RGV_DELETE_BLOCK, OnRgvDeleteBlock)
  ON_MESSAGE(WM_RGV_UNDO_DELETE_BLOCK, OnRgvUndoDeleteBlock)
  ON_MESSAGE(WM_RGV_PERMANENT_ERASE_BLOCK, OnRgvPermanentEraseBlock)
    
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CRemoteGeomViewer::CRemoteGeomViewer()
  : m_hPipe(INVALID_HANDLE_VALUE)
{
  m_pIPCBuffer = NULL;
  m_fOutputEnabled = true;
}

CRemoteGeomViewer::~CRemoteGeomViewer()
{
}

TCHAR szPipeName[] = _T("\\\\.\\pipe\\OdaRemoteGeometryViewer");
/////////////////////////////////////////////////////////////////////////////
// CDwgViewer message handlers
void CRemoteGeomViewer::OnInitialUpdate() 
{
	CDwgViewer::OnInitialUpdate( );
  GetDocument()->setVectorizer( this );

  m_hBufferWritten = ::CreateEvent(NULL, FALSE, FALSE, OD_T("OdaRemoteGeometryBufferWrittenEvent"));
  m_hBufferProcessed = ::CreateEvent(NULL, FALSE, FALSE, OD_T("OdaRemoteGeometryBufferProcessedEvent"));

  ::ResetEvent(m_hBufferWritten);
  ::SetEvent(m_hBufferProcessed);

  m_hPipe = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 10000 * 1024, OD_T("OdaRemoteGeometryViewerIPCBuffer"));
  if( m_hPipe != NULL ) {
    m_pIPCBuffer = ::MapViewOfFile(m_hPipe, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    m_hThread = ::CreateThread( NULL, 0, _listener, this, 0, &m_nThreadId );
  } 

  m_dlgRemoteGeomProps.Create(IDD_REMOTE_GEOMETRY_PROPS, GetParent());
  m_dlgRemoteGeomProps.SetNotificationWindow(this);
}

void CRemoteGeomViewer::OnDestroy() 
{
	CDwgViewer::OnDestroy();
  if( m_hPipe!=INVALID_HANDLE_VALUE ) {
    if( m_hThread!=INVALID_HANDLE_VALUE ) {
      ::PostThreadMessage( m_nThreadId, WM_QUIT, 0, 0 );

      if( ::WaitForSingleObjectEx( m_hThread, 2000, FALSE )==WAIT_TIMEOUT ) {
        ::TerminateThread( m_hThread, 100 );
      }
      ::CloseHandle( m_hThread );
    }
    ::UnmapViewOfFile(m_pIPCBuffer);
    ::CloseHandle( m_hPipe );
    ::CloseHandle( m_hBufferWritten );
    ::CloseHandle( m_hBufferProcessed );
  }
}

DWORD WINAPI CRemoteGeomViewer::_listener( LPVOID pThis ) {
  return ((CRemoteGeomViewer*)pThis)->listener();
}

DWORD CRemoteGeomViewer::listener( )
{
  OdUInt32 n = 0;
  OdUInt32 nBytesRead = 0;
  BOOL bQuit = FALSE;
  MSG msg;
  // Force the system to create the message queue for this thread
  ::PeekMessage( &msg, NULL, WM_USER, WM_USER, PM_NOREMOVE );
  
  do 
  {
    //::SignalObjectAndWait(m_hBufferProcessed, m_hBufferWritten, INFINITE, FALSE);
    ::WaitForSingleObject(m_hBufferWritten, INFINITE);

    byte* pBuffer = reinterpret_cast<byte*>(m_pIPCBuffer);
    
    memcpy(&n, pBuffer, sizeof(n));
    pBuffer += sizeof(n);

    m_opcode = (OdRemoteGeomProxy::Opcode)n;

    memcpy(&n, pBuffer, sizeof(n));
    pBuffer += sizeof(n);

    m_buffer.resize( n );
    memcpy(m_buffer.asArrayPtr(), pBuffer, n);

    SendMessage( WM_COMMAND, ID_PROCESS_CMD, 0 );
    ::SetEvent(m_hBufferProcessed);

    bQuit = ::PeekMessage( &msg, NULL, WM_QUIT, WM_QUIT, PM_NOREMOVE );   
  }
  while ( !bQuit );
  return 0;
}

DBIO_EXPORT void oddbExplodeProxyGraphics( OdDbDatabase* pDb,  const OdUInt8Array& graphics, OdRxObjectPtrArray& entitySet );

OdDbBlockTableRecordPtr CRemoteGeomViewer::getBlockRecordToInsertRemoteGeometry()
{
  OdDbDatabasePtr pDb = GetDocument()->m_pDb;
  OdDbBlockTableRecordPtr pBlock;
  
  if(m_arrBlockStack.isEmpty())
  {
    pBlock = pDb->getModelSpaceId().safeOpenObject( OdDb::kForWrite );
  }
  else
  {
    OdDbObjectId idLastBlock = m_arrBlockStack.last();
    pBlock = idLastBlock.safeOpenObject( OdDb::kForWrite );  
  } 

  return pBlock;
}

void CRemoteGeomViewer::appendRemoteGeometryToBlock(OdDbBlockTableRecordPtr pBlock, OdRxObjectPtrArray proxyGraphics)
{
  if(pBlock.isNull())
    return;

  OdRxObjectPtrArray::iterator pIt = proxyGraphics.begin();
  OdRxObjectPtrArray::iterator pItEnd = proxyGraphics.end();

  for(;pIt != pItEnd; ++pIt)
  {
    OdDbEntity* pEnt = static_cast<OdDbEntity*>( pIt->get() );
    pBlock->appendOdDbEntity( pEnt );
    // Force subclose and appending subentities to database
    // so that they get handle values
    pEnt->downgradeOpen();
  }        
}

void CRemoteGeomViewer::parseRemoteGeometryEntities(OdRxObjectPtrArray& remoteGeometry)
{
  OdDbDatabasePtr pDb = GetDocument()->m_pDb;  
  ::oddbExplodeProxyGraphics( pDb, m_buffer, remoteGeometry );
}

void CRemoteGeomViewer::parseAndDrawRemoteGeometry()
{
  ODA_ASSERT_ONCE(m_arrBlockStack.size() > 0);

  OdRxObjectPtrArray remoteGeometry;
  parseRemoteGeometryEntities(remoteGeometry);

  OdDbBlockTableRecordPtr pBlock = getBlockRecordToInsertRemoteGeometry();
  appendRemoteGeometryToBlock(pBlock, remoteGeometry);  
}

void CRemoteGeomViewer::processRequest( )
{
  if(!m_fOutputEnabled)
    return;

  OdDbDatabase* pDb = GetDocument()->m_pDb;
  switch( m_opcode ) {
    case OdRemoteGeomProxy::kGraphics:
      {
        parseAndDrawRemoteGeometry();        
      }
      break;
    case OdRemoteGeomProxy::kStringToExecute:
      {
        OdString s((const OdChar*)m_buffer.getPtr(), m_buffer.size()/2);

        if(s == g_strGeomBlockBegin)
        {
          OdDbBlockTableRecordPtr pNewBlock = OdDbBlockTableRecord::createObject();
          pNewBlock->setName(OD_T("*R"));
          OdDbBlockTablePtr pBlockTable = pDb->getBlockTableId().safeOpenObject( OdDb::kForWrite );
          pBlockTable->add(pNewBlock);

          m_arrBlockStack.push_back(pNewBlock->objectId());
        }
        else if( s == g_strGeomBlockEnd )
        {
          if(m_arrBlockStack.isEmpty())
            break;

          if(m_arrBlockStack.length() == 1)
          {
            pDb->startUndoRecord();
            
            OdDbObjectId idNewBlock = m_arrBlockStack.last();
            m_arrBlockStack.removeLast();

            OdDbObjectId idBlockRef = AddBlockRefrenceToModelSpace(idNewBlock);
            if(!idBlockRef.isNull())
            {
              RemoteGeomBlock rNewBlock;
			        rNewBlock.idBlock = idNewBlock;
			        rNewBlock.idBlockRef = idBlockRef;
              OdUInt32 uNewBlockKey = GenerateNewRemoteBlockKey();

              m_collRemoteBlocks.insert( std::make_pair(uNewBlockKey, rNewBlock) );
              m_dlgRemoteGeomProps.OnNewRemoteBlockAdded(uNewBlockKey);      
            }            
          }
          else
          {
            OdDbObjectId idLastBlock = m_arrBlockStack.last();
            m_arrBlockStack.removeLast();

            OdDbBlockReferencePtr pInsert = OdDbBlockReference::createObject();
            pInsert->setDatabaseDefaults(pDb);
            pInsert->setBlockTableRecord(idLastBlock);

            OdDbObjectId idPrevBlock = m_arrBlockStack.last();
            OdDbBlockTableRecordPtr pPrevBlock = idPrevBlock.safeOpenObject(OdDb::kForWrite);

            pPrevBlock->appendOdDbEntity( pInsert );
          }
        }        
      }
      break;
  }
}

void CRemoteGeomViewer::OnUpdateDebugGeometry(CCmdUI* pCmdUI)
{
  pCmdUI->Enable( FALSE );
}

LRESULT CRemoteGeomViewer::OnRgvChangeOutputState( WPARAM wParam, LPARAM lParam )
{
  m_fOutputEnabled = wParam != 0;
  return 0;
}

LRESULT CRemoteGeomViewer::OnRgvDeleteBlock( WPARAM wParam, LPARAM lParam )
{
  OdUInt32 uBlockKey = (OdUInt32)wParam;

  RemoteGeomBlocksCollection::iterator it = m_collRemoteBlocks.find(uBlockKey);
  if(it == m_collRemoteBlocks.end())
    return 0;
  
  RemoteGeomBlock& rData = it->second;
  OdDbObjectId idRef = rData.idBlockRef;

  OdDbBlockReferencePtr ptrRef = idRef.safeOpenObject(OdDb::kForWrite);
  if(!ptrRef.isNull())
    ptrRef->erase();
  
  m_collDeletedRemoteBlocks.insert( std::make_pair(uBlockKey, rData) );
  m_collRemoteBlocks.erase(it);
  
  OnViewerRegen();
  return 0;
}

OdDbObjectId CRemoteGeomViewer::AddBlockRefrenceToModelSpace(OdDbObjectId idBlock)
{
  OdDbDatabasePtr pDb = GetDocument()->m_pDb;
  OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().safeOpenObject( OdDb::kForWrite );

  OdDbBlockReferencePtr pInsert = OdDbBlockReference::createObject();
  pInsert->setDatabaseDefaults(pDb);
  pInsert->setBlockTableRecord(idBlock);            

  OdGeExtents3d ext;
  pInsert->getGeomExtents(ext);
  if(!ext.isValidExtents())
    return OdDbObjectId();

  pMS->appendOdDbEntity( pInsert );

  return pInsert->objectId();
}

LRESULT CRemoteGeomViewer::OnRgvUndoDeleteBlock( WPARAM wParam, LPARAM lParam )
{
  if(m_collDeletedRemoteBlocks.empty())
    return 1;

  RemoteGeomBlocksCollection::iterator it = m_collDeletedRemoteBlocks.find(OdUInt32(wParam));
  OdUInt32 uKey = it->first;
  RemoteGeomBlock& rData = it->second;

  rData.idBlockRef = AddBlockRefrenceToModelSpace(rData.idBlock);
  if(rData.idBlockRef.isNull())
    return 1;

  m_collRemoteBlocks.insert( std::make_pair(uKey, rData) );
  m_dlgRemoteGeomProps.OnNewRemoteBlockAdded(uKey);

  m_collDeletedRemoteBlocks.erase(it);
  
  OnViewerRegen();
  return 0;
}

LRESULT CRemoteGeomViewer::OnRgvPermanentEraseBlock( WPARAM wParam, LPARAM lParam )
{
  OdUInt32 uBlockKey = (OdUInt32)wParam;

  RemoteGeomBlocksCollection::iterator it = m_collRemoteBlocks.find(uBlockKey);
  if(it == m_collRemoteBlocks.end())
    return 0;

  RemoteGeomBlock& rData = it->second;
  OdDbObjectId idRef = rData.idBlockRef;

  OdDbBlockReferencePtr ptrRef = idRef.safeOpenObject(OdDb::kForWrite);
  if(!ptrRef.isNull())
    ptrRef->erase();

  m_collRemoteBlocks.erase(it);
  OnViewerRegen();
  return 0;
}

bool CRemoteGeomViewer::GetPreviewBlockData(OdUInt32 uBlockKey, PreviewBlockData& rData)
{
  RemoteGeomBlocksCollection::iterator it = m_collRemoteBlocks.find(uBlockKey);
  if(it == m_collRemoteBlocks.end())
    return false;

  RemoteGeomBlock& rBlockData = it->second;

  rData.pDb = GetDocument()->m_pDb;
  rData.idBlock = rBlockData.idBlock;
  return true;
}
