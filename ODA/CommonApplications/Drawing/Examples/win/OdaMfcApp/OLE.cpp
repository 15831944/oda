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

// OLE.cpp : OLE item related code
//

#include "stdafx.h"
#include "OdaMfcAppDoc.h"
#include "afxodlgs.h"
#include "DbOle2Frame.h"
#include "DbBlockTableRecord.h"
#include "OleItemHandlerBase.h"
#include "MemoryStream.h"
#include "Ge/GeScale3d.h"
#include "OdaMfcApp.h"
#include "ThreadsCounter.h"
#include "ExEmbedRaster.h"

class OleDwgItem : public COleClientItem, public OdOleItemHandler
{
  ODRX_HEAP_OPERATORS();

  OdSmartPtr<OdOleItemSimplestHandler> m_pRawData;

  class File : public CFile
  {
    OdStreamBuf&  m_stream;
  public:
    File(OdStreamBuf& stream)
      : m_stream(stream)
    {
      m_strFileName = "OdDbOle2Frame binary data.";
      m_bCloseOnDelete = false;
      m_hFile = 0;
    }
    UINT Read(void* lpBuf, UINT nCount)
    {
      OdUInt32 nBytesLeft = OdUInt32(m_stream.length()-m_stream.tell());
      if(nBytesLeft < nCount)
        nCount = nBytesLeft;
      m_stream.getBytes(lpBuf, nCount);
      return nCount;
    }
    void Write(const void* lpBuf, UINT nCount)
    {
      m_stream.putBytes(lpBuf, nCount);
    }
  };

public:
  OdDbObjectId m_frameId;
  OdDbDatabaseDoc* m_pDbDoc;

public:
  ODRX_DECLARE_MEMBERS(OleDwgItem);

  ~OleDwgItem()
  {
  }
  void addRef()
  {
    COleClientItem::ExternalAddRef();
  }
  void release()
  {
    COleClientItem::ExternalRelease();
  }

  static OdRxObjectPtr createNew()
  {
    OdRxObjectPtr pRes;
    pRes.attach(new OleDwgItem());
    static_cast<OleDwgItem*>(pRes.get())->m_pDbDoc = NULL;
    return pRes;
  }

  struct OleItemClientData
  {
    LPVIEWOBJECT m_lpViewObject;
    DVASPECT m_nDrawAspect;
    void *hdc;
    RECTL rectl;
  };
  static void drawByMainThread(void *pArg)
  {
    OleItemClientData *pData = reinterpret_cast<OleItemClientData*>(pArg);

    int nPrevMode = ::GetStretchBltMode((HDC) pData->hdc);
    int nMode = COLORONCOLOR; // or HALFTONE
    if (nMode != nPrevMode)
      ::SetStretchBltMode((HDC) pData->hdc, nMode); // PROD-63

#ifdef _DEBUG
    HRESULT hr = 
#endif
      pData->m_lpViewObject->Draw(pData->m_nDrawAspect, -1, NULL, NULL, NULL, (HDC)pData->hdc, &(pData->rectl), NULL, NULL, 0);

    if (nMode != nPrevMode)
      ::SetStretchBltMode((HDC) pData->hdc, nPrevMode);

#ifdef _DEBUG
    if (FAILED(hr))
    {
      ODA_TRACE1("[OleDwgItem::draw] m_lpViewObject->Draw() returned %x\n", hr);
      if(HRESULT_FACILITY(hr) == FACILITY_WINDOWS)
        hr = HRESULT_CODE(hr);
      OdChar *szError;
      if (::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                           FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr,
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           (LPTSTR)&szError, 0, NULL) != 0)
      {
        ODA_TRACE1("[OleDwgItem::draw] Decoded HRESULT: %s", szError);
        ::LocalFree(szError);
      }
    }
#endif
  }

  bool draw(const OdGiCommonDraw& drawObj, void* hdc, const OdGsDCRect& rect) const
  {
    if (m_pRawData.get())
      return m_pRawData->draw(drawObj, hdc, rect);

    ODA_ASSERT_ONCE(m_lpViewObject);
    if (!m_lpViewObject)
      return false;
    ::FillRect((HDC)hdc, (LPRECT)&rect, ::GetSysColorBrush(COLOR_WINDOW));
    RECTL rectl = { rect.m_min.x, rect.m_max.y, rect.m_max.x, rect.m_min.y };
    OleItemClientData cliData = { m_lpViewObject, m_nDrawAspect, hdc, rectl };
    odExecuteMainThreadAction(drawByMainThread, &cliData);
    return true;
  }

  void load(OdStreamBuf& stream)
  {
    COleClientItem::Release();

    COleDocument* pOleDocument = 0;
    if (stream.isKindOf(OdOleItemInitStream::desc()))
    {
      m_frameId = OdOleItemInitStreamPtr(&stream)->frameId();
      OdDbDatabaseDocPtr pDb = OdDbDatabaseDoc::cast(m_frameId.database());
      if (pDb.get())
        pOleDocument = pDb->document();
    }
    else if (m_pDbDoc)
      pOleDocument = m_pDbDoc->document();

    if(pOleDocument)
    {
      m_pRawData.release();

      File file(stream);
      CArchive archive(&file, CArchive::load|CArchive::bNoFlushOnDelete);
      archive.m_pDocument = pOleDocument;
      
      // Maintain reference balance:
      // COleDocument calls COleClientItem::InternalRelease() in its DeleteContents(),
      // but it does not add reference in COleDocument::AddItem();
      if(!m_pDocument)
      {
        pOleDocument->AddItem(this);
        addRef();
      }

      TRY
      {
        COleClientItem::Serialize(archive);
        if (type() == OT_LINK)
        { // External file may be modified
          COleClientItem::UpdateLink();
        }
      }
      CATCH(CException, pException)
      {
        CString msg;
        pException->GetErrorMessage(msg.GetBuffer(256), 256);
        OdString msg2;
#ifdef _UNICODE
        msg2.format(L"%ls : \"%ls\"", (LPCTSTR)CString(pException->GetRuntimeClass()->m_lpszClassName), (LPCTSTR)msg);
#else
        msg2.format(L"%s : \"%s\"", pException->GetRuntimeClass()->m_lpszClassName, (LPCTSTR)msg);
#endif        
        // remove invalid object from OLE document
        m_pDocument->RemoveItem(this);
        release();
        archive.Abort();
        throw OdError(msg2);
      }
      END_CATCH
    }
    else
    {
      if (m_pRawData.isNull())
      {
        ODA_ASSERT_ONCE(OdOleItemHandler::desc()->constructor() == OleDwgItem::createNew);
        static OdPseudoConstructorType s_pCostrInXref = NULL;
        if (s_pCostrInXref && ::odrxDynamicLinker()->getModule(OdOleItemHandlerModuleName).isNull())
          s_pCostrInXref = NULL;
        if (::odrxDynamicLinker()->loadModule(OdOleItemHandlerModuleName).get())
        {
          if (!s_pCostrInXref)
            s_pCostrInXref = OdOleItemHandler::desc()->constructor();
          OdOleItemHandler::desc()->setConstructor(OleDwgItem::createNew); // restore for commands OLEOPEN by non-Xref-ed OLE item handlers later 
          if (s_pCostrInXref)
            m_pRawData = (*s_pCostrInXref)(); // CORE-13319 OLE in Nested Xref
        }
      }
      // loading database is not associated with COdaMfcApp (COleDocument)
      // so do not load OLE object - just store its data.
      if(m_pRawData.isNull())
        m_pRawData = OdRxObjectImpl<OdOleItemSimplestHandler>::createObject();
      m_pRawData->load(stream);
    }
  }
  
  void save(OdStreamBuf& stream) const
  {
    if(m_pRawData.get())
    {
      m_pRawData->save(stream);
      return;
    }

    if(m_lpObject)
    {
      File file(stream);
      CArchive archive(&file, CArchive::store);
      
      const_cast<OleDwgItem*>(this)->COleClientItem::Serialize(archive);
    }
  }
  
  OdOleItemHandler::Type type() const
  {
    if(m_pRawData.get())
      return m_pRawData->type();

    return Type(COleClientItem::GetType());
  }
  
  OdOleItemHandler::DvAspect drawAspect() const
  {
    if(m_pRawData.get())
      return m_pRawData->drawAspect();

    return DvAspect(COleClientItem::GetDrawAspect());
  }

  PlotQuality outputQuality() const
  {
    if (m_pRawData.get())
      return m_pRawData->outputQuality();

    OdDbOle2FramePtr pOleFrame = m_frameId.openObject(OdDb::kForRead);
    if (pOleFrame.get())
    {
      return (OdOleItemHandler::PlotQuality)pOleFrame->outputQuality();
    }

    return OdOleItemHandler::kLowGraphics;
  }
  void setOutputQuality(PlotQuality quality)
  {
    if (m_pRawData.get())
      m_pRawData->setOutputQuality(quality);
  }
  
  void setDrawAspect(DvAspect drawAspect)
  {
    if(m_pRawData.get())
    {
      m_pRawData->setDrawAspect(drawAspect);
      return ;
    }
    if (m_pDocument)
      COleClientItem::SetDrawAspect(DVASPECT(drawAspect));
  }
  
  OdString linkName() const
  {
    return linkPath();
  }
  
  OdString linkPath() const
  {
    if(m_pRawData.get())
      return m_pRawData->linkName();

    IOleLinkPtr pOleLink(m_lpObject);
    if(pOleLink)
    {
      LPOLESTR displayName = 0;
      if(SUCCEEDED(pOleLink->GetSourceDisplayName(&displayName)))
      {
        _bstr_t res(displayName);
        IMallocPtr pMalloc;
        ::CoGetMalloc(1, &pMalloc);
        if(pMalloc!=0)
          pMalloc->Free(displayName);
        return (const wchar_t*)res;
      }
    }
    return OdString::kEmpty;
  }
  
  OdString userType() const
  {
    if(m_pRawData.get())
      return m_pRawData->userType();

    if(m_lpObject)
    {
      CString res;
      const_cast<OleDwgItem*>(this)->COleClientItem::GetUserType(USERCLASSTYPE_FULL, res);
      return (const OdChar*)OdString(res);
    }
    return OdString::kEmpty;
  }
  
  OdOleItemHandler::DvAspect adviseType() const
  {
    if(m_pRawData.get())
      return m_pRawData->adviseType();

    if(!m_lpViewObject)
      return kContent;

    DWORD dwAspect;
    IAdviseSinkPtr pAdviseSink;
    VERIFY(m_lpViewObject->GetAdvise(&dwAspect, NULL, &pAdviseSink) == S_OK);
    return DvAspect(dwAspect);
  }
  
  bool monikerAssigned() const
  {
    if(m_pRawData.get())
      return m_pRawData->monikerAssigned();

    return (m_bMoniker!=0);
  }

  OdUInt32 getCompoundDocumentDataSize() const
  {
    if(m_pRawData.get())
      return m_pRawData->getCompoundDocumentDataSize();

    OdMemoryStreamPtr pStream = OdMemoryStream::createNew();
    save(*pStream.get());
    pStream->rewind();
    OdSmartPtr<OdOleItemSimplestHandler> pData = OdRxObjectImpl<OdOleItemSimplestHandler>::createObject();
    pData->load(*pStream.get());
    return pData->getCompoundDocumentDataSize();
  }
  
  void getCompoundDocument(OdStreamBuf& stream) const
  {
    if(m_pRawData.get())
    {
      m_pRawData->getCompoundDocument(stream);
      return;
    }
    OdMemoryStreamPtr pStream = OdMemoryStream::createNew();
    save(*pStream.get());
    pStream->rewind();
    OdSmartPtr<OdOleItemSimplestHandler> pData = OdRxObjectImpl<OdOleItemSimplestHandler>::createObject();
    pData->load(*pStream.get());
    pData->getCompoundDocument(stream);
  }
  
  void setCompoundDocument(OdUInt32 nDataSize, OdStreamBuf& stream)
  {
    if(m_pRawData.get())
    {
      m_pRawData->setCompoundDocument(nDataSize, stream);
      return;
    }
    OdSmartPtr<OdOleItemSimplestHandler> pRawData = OdRxObjectImpl<OdOleItemSimplestHandler>::createObject();
    pRawData->setCompoundDocument(nDataSize, stream);
    OdMemoryStreamPtr pStream = OdMemoryStream::createNew();
    pRawData->save(*pStream.get());
    pStream->rewind();
    load(*pStream.get());
  }

  void OnChange( OLE_NOTIFICATION nCode, DWORD dwParam )
  {
    COleClientItem::OnChange(nCode, dwParam);

    // suppress boring save dialog on exit...
    m_pDocument->SetModifiedFlag(FALSE);

    OdDbOle2FramePtr pOleFrame = m_frameId.openObject(OdDb::kForWrite);
    if(pOleFrame.get())
    {
      CSize newSize;
      if (GetExtent(&newSize))
      {
        CSize oldSize(pOleFrame->unhandled_himetricWidth(), pOleFrame->unhandled_himetricHeight());
        if(oldSize != CSize(0,0) && newSize != oldSize)
        {
          OdGeScale3d s;
          pOleFrame->unhandled_setHimetricSize(OdUInt16(s.sx = newSize.cx), OdUInt16(s.sy = newSize.cy));
          s.sz = 1.0;
          s.sx /= oldSize.cx;
          s.sy /= oldSize.cy;
          
          OdRectangle3d r3d;
          pOleFrame->position(r3d);
          
          OdGeMatrix3d xScaling = OdGeMatrix3d::scaling(s, r3d.upLeft);
          r3d.lowLeft.  transformBy(xScaling);
          r3d.upLeft.   transformBy(xScaling);
          r3d.upRight.  transformBy(xScaling);
          r3d.lowRight. transformBy(xScaling);
          pOleFrame->setPosition(r3d);
        }
      }
    }
  }

  COleClientItem* getOleClientItem(void) const
  {
    if(m_pRawData.get())
    {
      return NULL;
    }
    return (COleClientItem*)this;
  }

  bool embedRaster(OdGiRasterImagePtr pImage,
                   OdRxObject* pRxDb) // = NULL
  {
    if (m_pRawData.get())
      m_pRawData = NULL; //TODO //return exEmbedRaster(m_pRawData, pImage);
    m_pDbDoc = OdDbDatabaseDoc::cast(pRxDb).get();
    ODA_ASSERT_ONCE(m_pDbDoc);
    if (!exEmbedRaster(this, pImage))
      return false;
    ODA_ASSERT_ONCE(m_lpViewObject);
    return true;
  }

  OdGiRasterImagePtr getRaster(bool) const
  {
    return OdGiRasterImagePtr();  // TODO
  }
};


ODRX_NO_CONS_DEFINE_MEMBERS(OleDwgItem, OdOleItemHandler);

#include "Ed/EdCommandStack.h"
#include "DbUserIO.h"
#include "DbSSet.h"

class OleCommand : public OdEdCommand
{
public:
  const OdString groupName() const { return OD_T("ODAMFCAPP"); }
};

class OLEOPEN_cmd : public OleCommand
{
public:
  const OdString globalName() const { return OD_T("OLEOPEN"); }
  void          execute(OdEdCommandContext* pCmdCtx)
  {
    OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
    OdDbDatabasePtr pDb = pDbCmdCtx->database();
    OdDbUserIOPtr pIO = pDbCmdCtx->userIO();

    OdDbOle2FramePtr pOle2Frame;
    OdSmartPtr<OleDwgItem> pItem;
    OdDbSelectionSetIteratorPtr pIter = pIO->select(OD_T("Select an object <done>:"))->newIterator();
    while(!pIter->done())
    {
      pOle2Frame = OdDbOle2Frame::cast(pIter->objectId().safeOpenObject(OdDb::kForWrite));
      if(pOle2Frame.get())
      {
        pItem = pOle2Frame->itemHandler();
        pItem->DoVerb(OLEIVERB_OPEN, NULL);
        return;
      }
      pIter->next();
    }
  }
};


#include "DbUserIO.h"

class insertobj_cmd : public OleCommand
{
  struct TRACKER : OdEdRealTracker
  {
    OdSmartPtr<OdDbOle2Frame> _pOleFrame;
    OdGePoint3d   _orig;
    OdGeVector2d  _size;
    double        _dist;
    
    void setValue(double dist)
    {
      _dist = dist;
      OdRectangle3d r3d;

      OdGeVector2d size = _size.normal() * _dist;
      r3d.lowLeft .set(_orig.x,          _orig.y,          _orig.z);
      r3d.upLeft  .set(_orig.x,          _orig.y + size.y, _orig.z);
      r3d.upRight .set(_orig.x + size.x, _orig.y + size.y, _orig.z);
      r3d.lowRight.set(_orig.x + size.x, _orig.y,          _orig.z);

      _pOleFrame->setPosition(r3d);
    }

    int addDrawables(OdGsView* pView)
    {
      pView->add(_pOleFrame, 0);
      return 1;
    }
    void removeDrawables(OdGsView* pView)
    {
      pView->erase(_pOleFrame);
    }
  };
public:
  const OdString globalName() const { return OD_T("insertobj"); }
  void          execute(OdEdCommandContext* pCmdCtx)
  {
    OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
    OdDbDatabaseDocPtr pDb = pDbCmdCtx->database();
    OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

    // Invoke the standard Insert Object dialog box to obtain information
    //  for new CDrawItem object.
    COleInsertDialog dlg;
    if (dlg.DoModal() != IDOK)
      return;

    COleDocument* pOleDocument = pDb->document();

    OdStaticRxObject<TRACKER> tracker;

    OdSmartPtr<OdDbOle2Frame> pOleFrame = tracker._pOleFrame = OdDbOle2Frame::createObject();
    pOleFrame->setDatabaseDefaults(pDb);
    OdSmartPtr<OleDwgItem> pItem = pOleFrame->getItemHandler();

    pOleDocument->AddItem(pItem);
    pItem->addRef(); // DN: not sure about it, but otherwise it crashes on exit...

    ASSERT_VALID(pItem);

    // Now create the OLE object/item
    TRY
    {
      if (!dlg.CreateItem(pItem))
        AfxThrowMemoryException();

      // try to get initial presentation data
      pItem->UpdateLink();

      CSize size;
      if (!pItem->GetExtent(&size))
        size = CSize(1000, 1000);

      pOleFrame->unhandled_setHimetricSize(OdUInt16(size.cx), OdUInt16(size.cy));

      tracker._orig = pIO->getPoint(OD_T("Specify insertion point <0,0>:"));

      // Set initial OLE frame position to origin, and size in world units:
      // Convert MM_HIMETRIC (0.01 millimeter) to Space Units:
      double s = pDb->getMEASUREMENT()==OdDb::kEnglish ? 0.01 / 25.4 : 0.01;
      tracker._size.set(double(size.cx) * s, double(size.cy) * s);

      OdString sPmt;
      sPmt.format(OD_T("Specify size <%g>:"), tracker._size.length());
      tracker.setValue(pIO->getDist(sPmt, OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand, 0., OdString::kEmpty, &tracker));
      OdDbBlockTableRecordPtr pSpace = pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);
      pItem->m_frameId = pSpace->appendOdDbEntity(pOleFrame);

      // if insert new object -- initially show the object
      if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
        pItem->DoVerb(OLEIVERB_OPEN, NULL);

      // suppress boring save dialog on exit...
      pOleDocument->SetModifiedFlag(FALSE);
    }
    CATCH_ALL(e)
    {
      if(pOleFrame->isDBRO())
        pOleFrame->erase();

      // clean up item
      pItem->Delete();
      ::AfxMessageBox(_T("Failed to insert OLE object..."));
    }
    END_CATCH_ALL
  }
};
/////////////////////////////////////////////////////////////////////////////////
class OLELINKS_cmd : public OleCommand
{
public:
  const OdString globalName() const { return L"OLELINKS"; }
  void           execute(OdEdCommandContext* pCmdCtx)
  {
    OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
    OdDbDatabaseDocPtr pDb = pDbCmdCtx->database();

    COleDocument* pOleDocument = pDb->document();
    POSITION pos = pOleDocument->GetFirstViewPosition();
    CView *pView = pOleDocument->GetNextView(pos);
    CWnd *pParent = pView->GetParent();

    // Invoke the standard OLE Links dialog box
  	COleLinksDialog dlg(pOleDocument, pView, 0, pParent);
    dlg.DoModal();
    return;
  }
};






// Should be called just after odInitialize() to enable this implementation
void rxInit_COleClientItem_handler()
{
  OleDwgItem::rxInit();
  OdOleItemHandler::desc()->setConstructor(OleDwgItem::createNew);
  odedRegCmds()->addCommand(OdRxObjectImpl<OLEOPEN_cmd>::createObject());
  odedRegCmds()->addCommand(OdRxObjectImpl<insertobj_cmd>::createObject());
  odedRegCmds()->addCommand(OdRxObjectImpl<OLELINKS_cmd>::createObject());
}

// Should be called just before odUninitialize()
void rxUninit_COleClientItem_handler()
{
  odedRegCmds()->removeGroup(_T("ODAMFCAPP"));
  if(OdOleItemHandler::desc()->constructor() == OleDwgItem::createNew)
  {
    // OdOleItemHandler::desc()->setConstructor(OdOleItemHandler::pseudoConstructor);
  }
  OleDwgItem::rxUninit();
}

void COdaMfcAppDoc::OnInsertOleobject() 
{
  BeginWaitCursor();

  ExecuteCommand(_T("insertobj"));

  EndWaitCursor();
}
