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

#ifndef __ODA_MFC_EXPORT_IMPL_H__
#define __ODA_MFC_EXPORT_IMPL_H__

#include "stdafx.h"
#include "OdaMfcExport.h"

class COdaMfcAppDoc;

template< class T >
class MfcObjectWrapper
{
  public:
    MfcObjectWrapper( T* obj )
      : m_pUnderlayObj( obj )
    {
    }

    T* operator->()
    {
      if ( m_pUnderlayObj == 0 )
        throw OdError( eNullObjectPointer );
      return m_pUnderlayObj;
    }

    const T* operator->() const
    {
      if ( m_pUnderlayObj == 0 )
        throw OdError( eNullObjectPointer );
      return m_pUnderlayObj;
    }

    const T* get() const
    {
      return m_pUnderlayObj;
    }

    T* get()
    {
      return m_pUnderlayObj;
    }

    void SetNull()
    {
      m_pUnderlayObj = 0;
    }

  private:
    T* m_pUnderlayObj;
};

class OdApDocumentImpl : public OdApDocument
{
public:
  static OdSmartPtr< OdApDocumentImpl > createObject( CDocument* pDoc );

  virtual ~OdApDocumentImpl();

  virtual OdString fileName() const;

  virtual CDocument* cDoc() const;

  //virtual AcTransactionManager* transactionManager() const;

  virtual OdDbDatabasePtr database() const;

  virtual void lockMode( bool bIncludeMyLocks ) const;

  virtual void myLockMode() const;

  virtual bool isQuiescent() const;

  virtual void* contextPtr() const;

  virtual OdEdBaseIO* cmdIO();

  virtual OdDbCommandContextPtr cmdCtx();

  virtual void ExecuteCommand( const OdString& sCmd, bool bEcho );

  virtual OdString recentCmd();

  virtual OdDbSelectionSetPtr selectionSet() const;

  MfcObjectWrapper< COdaMfcAppDoc >* m_pImp;
};

#endif // __ODA_MFC_EXPORT_IMPL_H__
