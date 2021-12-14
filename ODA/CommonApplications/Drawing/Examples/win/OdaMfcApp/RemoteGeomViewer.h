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

#if !defined(AFX_CDEBUGGEOMVIEWER_H_)
#define AFX_CDEBUGGEOMVIEWER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DebugGeomViewer.h : header file
//

#include "DwgViewer.h"
#include "RemoteGeomProxy.h"
#include "RemoteGeomPropsDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDwgViewer view

class CRemoteGeomViewer : public CDwgViewer 
{
  DECLARE_DYNCREATE(CRemoteGeomViewer);

  HANDLE  m_hPipe;
  HANDLE  m_hThread;

  HANDLE m_hBufferWritten;
  HANDLE m_hBufferProcessed;

  void* m_pIPCBuffer;

  DWORD   m_nThreadId;
  static DWORD WINAPI _listener( LPVOID lpParameter );
  DWORD WINAPI listener( );

  OdRemoteGeomProxy::Opcode m_opcode;
  OdUInt8Array              m_buffer;

  

  struct RemoteGeomBlock
  {
    OdDbObjectId idBlock;
    OdDbObjectId idBlockRef;
  };

  typedef std::map<OdUInt32, RemoteGeomBlock> RemoteGeomBlocksCollection;
  
  OdDbObjectIdArray m_arrBlockStack;
  RemoteGeomBlocksCollection m_collRemoteBlocks;
  RemoteGeomBlocksCollection m_collDeletedRemoteBlocks;

  static OdUInt32 GenerateNewRemoteBlockKey()
  {
    static OdUInt32 uKey = 0;
    return uKey++;
  }

private:
  bool m_fOutputEnabled;

  CRemoteGeomPropsDlg m_dlgRemoteGeomProps;

private:
  OdDbObjectId AddBlockRefrenceToModelSpace(OdDbObjectId idBlock);
  
  OdDbBlockTableRecordPtr getBlockRecordToInsertRemoteGeometry();
  void appendRemoteGeometryToBlock(OdDbBlockTableRecordPtr pBlock, OdRxObjectPtrArray proxyGraphics);
  void parseRemoteGeometryEntities(OdRxObjectPtrArray& remoteGeometry);
  void parseAndDrawRemoteGeometry();
  
  // Attributes
public:
  CRemoteGeomViewer();

  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRemoteGeomViewer)
public:
  virtual void OnInitialUpdate();
  bool GetPreviewBlockData(OdUInt32 uBlockKey, PreviewBlockData& rData);
	//}}AFX_VIRTUAL

protected:
  virtual ~CRemoteGeomViewer();

  // Generated message map functions
protected:
  //{{AFX_MSG(CRemoteGeomViewer)
  afx_msg void OnDestroy();
  afx_msg void processRequest( );
  afx_msg void OnUpdateDebugGeometry( CCmdUI* pCmdUI );
  afx_msg LRESULT OnRgvChangeOutputState( WPARAM wParam, LPARAM lParam );
  afx_msg LRESULT OnRgvDeleteBlock( WPARAM wParam, LPARAM lParam );
  afx_msg LRESULT OnRgvUndoDeleteBlock( WPARAM wParam, LPARAM lParam );
  afx_msg LRESULT OnRgvPermanentEraseBlock( WPARAM wParam, LPARAM lParam );
	//}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDEBUGGEOMVIEWER_H_)
