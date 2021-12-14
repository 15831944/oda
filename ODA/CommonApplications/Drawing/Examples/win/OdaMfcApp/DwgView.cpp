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

// DwgView.cpp : implementation of the CDwgView class
//

#include "stdafx.h"
#include "OdString.h"

#include "OdaMfcApp.h"
#include "OdaMfcAppDoc.h"
#include "DwgView.h"
#include "DbSymbolTableRecord.h"
#include "MainFrm.h"
#include "TreeFiller.h"
#include "ListFiller.h"
#include "DbDictionary.h"
#include "ClassesDlg.h"
#include "FindObjectDlg.h"
#include "DbEntity.h"
#include "DbSafeTransaction.h"
#include "DbSymbolTable.h"
#include "DbDimension.h"
#include "DbBlockTableRecord.h"
#include "DbOle2Frame.h"
#include "DbBlockReference.h"
#include "MaterialsDlg.h"
#include "LightsDlg.h"
#include "FileDependencyDlg.h"

#include "DbRegion.h"
#include "DbBody.h"
#include "Db3dSolid.h"

#include "ObjPropDlg.h"
#include "AcisSaveDialog.h"
#include "DbSSet.h"
//#include "Ed/EdCommandStack.h"
#include "ExDbCommandContext.h"
#include "DbBlockTable.h"
#include "OdRound.h"
#include "DbDimStyleTableRecord.h"

#if _MSC_VER < 1400
#include <atlbase.h>
#include <atlwin.h>
#endif
extern COdaMfcAppApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDwgView

IMPLEMENT_DYNCREATE(CDwgView, CFormView)

BEGIN_MESSAGE_MAP(CDwgView, CFormView)
	//{{AFX_MSG_MAP(CDwgView)
	ON_NOTIFY(TVN_SELCHANGING, IDC_OWNERSHIPTREE, OnSelchangingOwnershiptree)
	ON_NOTIFY(TVN_KEYDOWN, IDC_OWNERSHIPTREE, OnKeydownOwnershiptree)
	ON_COMMAND(ID_VIEW_CLASSESINFO, OnViewClassesinfo)
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_COMMAND(ID_VIEW_SYSTEMVARIABLES, OnViewSystemvariables)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_OWNERSHIPTREE, OnItemexpandingOwnershiptree)
	ON_NOTIFY(NM_RCLICK, IDC_OWNERSHIPTREE, OnRclickOwnershiptree)
	ON_COMMAND(ID_EDIT_RECOMPUTEDIMBLOCK, OnEditRecomputeDimBlock)
	ON_COMMAND(ID_VIEW_RENDER_MATERIALS, OnViewRenderMaterials)
	ON_COMMAND(ID_VIEW_RENDER_LIGHT, OnViewRenderLight)
	ON_COMMAND(ID_EDIT_ACIS_CONVERSION, OnEditAcisConversion)
	ON_COMMAND(ID_VIEW_FILEDEPENDENCY, OnViewFiledependency)
	ON_COMMAND(ID_EDIT_ACIS_SAT_SAVE, OnEditAcisSatSave)
	ON_COMMAND(ID_EDIT_REDUCE, OnEditReduce)
	ON_COMMAND(ID_EDIT_CLEARSELECTION, OnEditClearSelection)
	ON_UPDATE_COMMAND_UI(ID_EXPLODE_ALL, OnUpdateExplode)
	//}}AFX_MSG_MAP
  ON_NOTIFY(NM_CLICK, IDC_OWNERSHIPTREE, OnNMClickOwnershiptree)
  ON_COMMAND(ID_EDIT_FINDNEXTSELECTED, OnEditFindnextselected)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDwgView construction/destruction

CDwgView::CDwgView()
	: CFormView(CDwgView::IDD)
  , m_nCurFound(0)
{
	//{{AFX_DATA_INIT(CDwgView)
	//}}AFX_DATA_INIT
}

CDwgView::~CDwgView()
{
}

void CDwgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDwgView)
	DDX_Control(pDX, IDC_OWNERSHIPTREE, m_ObjectTree);
	DDX_Control(pDX, IDC_OBJECTDATA, m_ObjectList);
	//}}AFX_DATA_MAP
}

BOOL CDwgView::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.style |= WS_MAXIMIZE;
	return CFormView::PreCreateWindow(cs);
}
void CDwgView::SetControlFont()
{
  LOGFONT lf = {0};
  m_ObjectTree.GetFont()->GetLogFont(&lf);
  CString strFont = AfxGetApp()->GetProfileString(_T("options"), _T("List window font typeface"), _T("Microsoft Sans Serif"));
  _tcscpy(lf.lfFaceName, (LPCTSTR)strFont);
  m_font.CreateFontIndirect(&lf);

  // apply the font to the controls
  m_ObjectTree.SetFont(&m_font);
  m_ObjectList.SetFont(&m_font);
}

void CDwgView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();
  SetControlFont();

	m_ObjectList.SetExtendedStyle(m_ObjectList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	ResizeParentToFit();

	m_ObjectList.InsertColumn(0, _T("Groupcode"), LVCFMT_LEFT, 80);
	m_ObjectList.InsertColumn(1, _T("Type"), LVCFMT_LEFT, 120);
	m_ObjectList.InsertColumn(2, _T("Value"), LVCFMT_LEFT, 231);
	m_nTotalObjects = 0;
}


#define round(a) ((int)((a) + 0.5))

void CDwgView::fillTree()
{
  OdDbObjectId oldObjId;
  OdDbSelectionSetPtr pSSet = GetDocument()->selectionSet();
  HTREEITEM h = m_ObjectTree.GetSelectedItem();
  if( h )
  {
    if (( m_ObjectTree.GetItemState( h, TVIF_STATE ) & TVIS_BOLD )==0 )
    {
      oldObjId = reinterpret_cast<OdDbStub*>(m_ObjectTree.GetItemData(h));
      pSSet->remove(oldObjId);
    }
    else if (m_ObjectTree.GetItemState( h, TVIF_STATE ) & TVIS_SELECTED )
    {
      oldObjId = reinterpret_cast<OdDbStub*>(m_ObjectTree.GetItemData(h));
    }
  }

  m_ObjectTree.DeleteAllItems();
  // TEMPORARY IMPLEMENTATION

  TreeFiller tf(GetDocument()->m_pDb, &m_ObjectTree, GetDocument()->selectionSet() );
  try
  {
    HTREEITEM hItem = tf.addObject2Tree(GetDocument()->m_pDb->objectId());
    if(hItem)
      m_ObjectTree.Expand(hItem, TVE_EXPAND);
  }
  catch(OdError& e)
  {
    theApp.reportError(_T("Error reading DB..."), e);
  }
  if (oldObjId.isValid())
  {
    Find(oldObjId);
  }
}

/////////////////////////////////////////////////////////////////////////////
// CDwgView diagnostics

#ifdef _DEBUG
void CDwgView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDwgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

COdaMfcAppDoc* CDwgView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COdaMfcAppDoc)));
	return (COdaMfcAppDoc*)m_pDocument;
}

#endif //_DEBUG

OdDbObjectId CDwgView::curObjectId() 
{
  HTREEITEM h = m_ObjectTree.GetSelectedItem();
  if(h)
    return reinterpret_cast<OdDbStub*>(m_ObjectTree.GetItemData(h));
  OdDbSelectionSetIteratorPtr pIter = GetDocument()->selectionSet()->newIterator();
  if(pIter->done())
    return OdDbObjectId::kNull;
  return pIter->objectId();
}

bool isAECObjectOrEntity( const OdRxClass* pClass );
class AECOwnershipTreeExpandPE;
extern AECOwnershipTreeExpandPE* g_pTreeExpandPE;

const OdRxObject* AECObject( const OdDbObject* pObj, const CString& s ) {
  int addr;
  if( g_pTreeExpandPE && _stscanf((LPCTSTR)s, _T("{0x%08X"), &addr ) == 1 ) {
    return (const OdRxObject*)OdIntToPtr(addr);
  }
  else if ( isAECObjectOrEntity( pObj->isA() ) ) {
    return pObj;
  }
  return 0;
}

void CDwgView::UpdateFieldDisplay() {
  LRESULT resultStub;
  OnSelchangingOwnershiptree((NMHDR*)&m_savedLast, &resultStub);
}

void CDwgView::OnSelchangingOwnershiptree(NMHDR* pNMHDR, LRESULT* pResult)
{
  m_ObjectList.DeleteAllItems();
  *pResult = 0;
  NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
  m_savedLast = *pNMTreeView;
  OdDbObjectId oldObjId = reinterpret_cast<OdDbStub*>(pNMTreeView->itemOld.lParam);
  OdDbObjectId newObjId = reinterpret_cast<OdDbStub*>(pNMTreeView->itemNew.lParam);
  
  OdDbSelectionSetPtr pSSet = GetDocument()->selectionSet();
  HTREEITEM h = pNMTreeView->itemOld.hItem;
  if( h && ( m_ObjectTree.GetItemState( h, TVIF_STATE ) & TVIS_BOLD )==0 )
  {
    pSSet->remove(oldObjId);
  }

  try
  {
    OdDbObjectPtr pObj = newObjId.safeOpenObject();
    OdDbDatabase* pDb = GetDocument()->m_pDb;
    if (pNMHDR) {
      h = pNMTreeView->itemNew.hItem;

      if (GetDocument()->m_bPartial)
      {
        HTREEITEM hParent = m_ObjectTree.GetParentItem(pNMTreeView->itemNew.hItem);
        if(hParent && (DWORD_PTR)pNMTreeView->itemNew.lParam == m_ObjectTree.GetItemData(hParent))
        {  // "click to load"
          TreeFiller tf(pDb, &m_ObjectTree, GetDocument()->selectionSet());
          tf.outSubObjects2Tree(pObj, hParent);
          m_ObjectTree.DeleteItem(pNMTreeView->itemNew.hItem);
          *pResult = 1;
        }
      }
      CString s;
      s.Format(_T("%d subitems"), ChildrenCounter(pDb).countChildren(pObj));
      theApp.setStatusText(2, s);

      if((::GetKeyState(VK_CONTROL) & 0x80000000)!=0 || (::GetKeyState(VK_SHIFT) & 0x80000000)!=0)
      {
        // change state
        m_ObjectTree.SetItemState( h, ( m_ObjectTree.GetItemState( h, TVIS_BOLD ) ^ TVIS_BOLD ), TVIS_BOLD );
      }

      // MFCApp just started, File New, Render, STLOut -> "is not an entity".
      // Selection set has 1 element = DatabaseDoc.
      if ((OdUInt64)newObjId.getHandle() != 0)
      {
        pSSet->append(newObjId);
      }
      pSSet.release();
    }

    const OdRxObject* pAECObj = AECObject( pObj, m_ObjectTree.GetItemText( h ) );
    COdaMfcAppApp::DisplayFields displayFields = COdaMfcAppApp::DisplayFields(((COdaMfcAppApp*)AfxGetApp())->m_displayFields);
    if (displayFields!=COdaMfcAppApp::kDxfFields)
    {
      ListFillerDwg lf(pDb, &m_ObjectList);
      if(pObj->isErased())
        lf.wrString(_T("ERASED"));
      if( pAECObj ) {
        g_pTreeExpandPE->outFields( pAECObj, &lf );
      }
      else {
        lf.listDwgFields(pObj, displayFields==COdaMfcAppApp::kSchemaFields);
      }
    }
    else
    {
      ListFiller lf(pDb, &m_ObjectList);
      OdString s;
      s.format(OD_T("database: 0x%08X %s"), pObj->database(), pObj->database()->getFilename().c_str());
      lf.wrString(999, s);
      pAECObj = AECObject( pObj, m_ObjectTree.GetItemText( h ) );
      if( pAECObj ) {
        g_pTreeExpandPE->outFields( pAECObj, &lf );
      }
      else {
        pObj->dxfOut(&lf);
      }

      OdResult err;

      if (pObj->isKindOf(OdDbEntity::desc())
        || pObj->isKindOf(OdDbBlockTableRecord::desc())
        || pObj->isKindOf(OdDbLayout::desc()))
      {
        OdGeExtents3d extents;
        err = pObj->getGeomExtents(extents);
        if (err == eOk)
        {
          lf.wrString(10, OD_T("getGeomExtents()"));
          lf.wrPoint3d(111, extents.minPoint());
          lf.wrPoint3d(111, extents.maxPoint());
        }
        else
        {
          OdString tmp2 = pObj->database()->appServices()->getErrorDescription(err);
          OdString tmp(OD_T("getGeomExtents() : "));
          lf.wrString(10, tmp + tmp2);
        }
      }
      if (pObj->isKindOf(OdDbEntity::desc()))
      {
        if (pObj->isKindOf(OdDbBlockReference::desc()))
        {
          lf.wrString(10, OD_T("------------ OdDbBlockReference ------------"));
          lf.wrString(10, OD_T("position()"));
          lf.wrPoint3d(111, OdDbBlockReferencePtr(pObj)->position());
        }

        if (pObj->isKindOf(OdDbRegion::desc()))
        {

          OdGePlane regPlane;
          OdDb::Planarity planarity;
          err = OdDbRegionPtr(pObj)->getPlane(regPlane, planarity);
          if (err == eOk)
          {
            OdGePoint3d origin;
            OdGeVector3d xAxis;
            OdGeVector3d yAxis;

            regPlane.get(origin, xAxis, yAxis);

            double perimeter, area;
            OdGePoint2d centroid;
            double momInertia[2];
            double prodInertia;
            double prinMoments[2];
            OdGeVector2d prinAxes[2];
            double radiiGyration[2];
            OdGePoint2d extentsLow, extentsHigh;

            err = OdDbRegionPtr(pObj)->getAreaProp(origin, xAxis, yAxis, perimeter, area, centroid,
              momInertia, prodInertia, prinMoments, prinAxes,
              radiiGyration, extentsLow, extentsHigh );

            if (err == eOk)
            {
              lf.wrString(10, _T("getAreaProp()"));
              lf.wrDouble(40, perimeter);
              lf.wrDouble(40, area);
              lf.wrPoint2d(111, centroid);
              lf.wrDouble(40, momInertia[0]);
              lf.wrDouble(40, momInertia[1]);
              lf.wrDouble(40, prodInertia);;
              lf.wrDouble(40, prinMoments[0]);
              lf.wrDouble(40, prinMoments[1]);
              lf.wrVector2d(111, prinAxes[0]);
              lf.wrVector2d(111, prinAxes[1]);
              lf.wrDouble(40, radiiGyration[0]);
              lf.wrDouble(40, radiiGyration[1]);
              lf.wrPoint2d(111, extentsLow);
              lf.wrPoint2d(111, extentsHigh);
            }
          }
          if (err != eOk)
          {
            OdString tmp2 = pObj->database()->appServices()->getErrorDescription(err);
            OdString tmp(_T("getAreaProp() : "));
            lf.wrString(10, tmp + tmp2);
          }
        }
      }
      
      OdDbOle2FramePtr pOle = OdDbOle2Frame::cast(pObj);
      if(pOle.get())
      {
        lf.wrString(10, OD_T("------------ OdDbOle2Frame ------------"));
        
        lf.wrString(10, OD_T("himetric size:"));
        lf.wrInt16(70, pOle->unhandled_himetricWidth());
        lf.wrInt16(71, pOle->unhandled_himetricHeight());
        
        OdRectangle3d rect;
        pOle->position(rect);
        lf.wrString(10, OD_T("position:"));
        lf.wrPoint3d(111, rect.upLeft);
        lf.wrPoint3d(111, rect.upRight);
        lf.wrPoint3d(111, rect.lowRight);
        lf.wrPoint3d(111, rect.lowLeft);
        
        if(pOle->getType()==OdDbOle2Frame::kLink)
        {
          OdString tmp;
          tmp.format(OD_T("LinkName: \"%s\""), pOle->getLinkName().c_str());
          lf.wrString(1, tmp);
          tmp.format(OD_T("LinkPath: \"%s\""), pOle->getLinkPath().c_str());
          lf.wrString(1, tmp);
          COleClientItem * pCOle = pOle->getOleClientItem();
          if (pCOle)
          {
            OLEUPDATE update = pCOle->GetLinkUpdateOptions();
            switch (update)
            {
            case OLEUPDATE_ALWAYS:
              tmp = L"OLEUPDATE_ALWAYS";
              break;
            case OLEUPDATE_ONCALL:
              tmp = L"OLEUPDATE_ONCALL";
              break;
            default:
              tmp = L"Update Unknown";
            }
            lf.wrString(1, tmp);
          }
        }
      }
    }
  }
  catch(const OdError& e)
  {
    int n = m_ObjectList.InsertItem(m_ObjectList.GetItemCount(), _T("OdError:"));
    m_ObjectList.SetItemText(n, 1, e.description());
  }
  catch(...)
  {
    int n = m_ObjectList.InsertItem(m_ObjectList.GetItemCount(), _T("ERROR:"));
    m_ObjectList.SetItemText(n, 1, _T("..."));
  }
}

void CDwgView::OnKeydownOwnershiptree(NMHDR* pNMHDR, LRESULT* pResult)
{
  TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
#ifndef ODA_FILE_FIXER
	switch( pTVKeyDown->wVKey )
  {
  case VK_DELETE:
    GetDocument()->DeleteSelection(true);
    break;
  }
#endif
	*pResult = 0;
}


void CDwgView::OnViewClassesinfo()
{
	CClassesDlg cd(GetDocument()->m_pDb, this);
  cd.DoModal();
}

void CDwgView::OnProperties()
{
#if defined(_TOOLKIT_IN_DLL_) && (_MSC_VER > 1310)
  try
  {
    CObjPropDlg pd(curObjectId(), this);
    pd.DoModal();
  }
  catch (HRESULT& /*hr*/)
#endif //#ifdef _TOOLKIT_IN_DLL_
  {
    MessageBox(_T("Can not create properties dialog."));
  }
}

class Traverser : public OdDbDwgFiler {
  OdDbObjectIdArray m_queuePage[2];
  OdDbObjectIdArray* m_pQueuePage[2];
  OdDbDatabase* m_pDb;
  OdDbObjectPtr m_pObj;
  int m_nEmbeddedSubobjectFollowState;
public:
  class Doer;
private:
  Doer* m_pDoer;
public:
  Traverser() {
    m_queuePage[0] =
    m_queuePage[1] = OdDbObjectIdArray( 256, -100 );
    m_pQueuePage[0] = &m_queuePage[0];
    m_pQueuePage[1] = &m_queuePage[1];
    m_nEmbeddedSubobjectFollowState = 0;
  }
  class Doer {
  public:
    virtual bool action( const OdDbObject* pObj, const OdRxObject* pSubObj ) = 0;
  };
  void traverse( OdDbDatabase* pDb, Doer& doer ) {
    if ( doer.action( pDb, 0 ) ) {
      m_pDoer = &doer;
      m_pDb = pDb;
      pDb->dwgOut( this );
      do {
        std::swap( m_pQueuePage[0], m_pQueuePage[1] );
        int n = m_pQueuePage[1]->size();
        for( int i=0; i<n; ++i ) {
          m_pObj = m_pQueuePage[1]->at( i ).openObject();
          if (m_pObj.isNull()) // erased
            continue;
          if ( !doer.action( m_pObj, 0 ) ) {
            m_pQueuePage[0] = 0;
            break;
          }
          m_pObj->dwgOut( this );
        }
        m_pQueuePage[1]->clear();
      }
      while( m_pQueuePage[0] && !m_pQueuePage[0]->empty() );
    }
  }
  OdDbDatabase* database() const { return const_cast<OdDbDatabase*>(m_pDb); }
protected:
  FilerType filerType() const { return kIdFiler; }

  void seek( OdInt64 , OdDb::FilerSeekType ) {}
  OdUInt64 tell() const { return 0; }
  bool rdBool() { return false; }
  OdString rdString() { return OdString::kEmpty; }
  OdInt8 rdInt8() { return 0; }
  OdUInt8 rdUInt8() { return 0; }
  OdInt16 rdInt16() { return 0; }
  OdInt32 rdInt32() { return 0; }
  OdInt64 rdInt64() { return 0; }
  void* rdAddress() { return 0; }
  double rdDouble() { return 0; }
  OdDbHandle rdDbHandle() { return 0; }
  OdDbObjectId rdSoftOwnershipId() { return 0; }
  OdDbObjectId rdHardOwnershipId() { return 0; }
  OdDbObjectId rdHardPointerId() { return 0; }
  OdDbObjectId rdSoftPointerId() { return 0; }
  OdGePoint2d rdPoint2d() { return OdGePoint2d::kOrigin; }
  OdGePoint3d rdPoint3d() { return OdGePoint3d::kOrigin; }
  OdGeVector2d rdVector2d() { return OdGeVector2d::kIdentity; }
  OdGeVector3d rdVector3d() { return OdGeVector3d::kIdentity; }
  OdGeScale3d rdScale3d() { return OdGeScale3d::kIdentity; }
  void rdBytes( void* , OdUInt32 ) {}
  void wrBool(bool ) { }
  void wrString(const OdString &) { }
  void wrBytes(const void* , OdUInt32 ) { }
  void wrInt8(OdInt8 ) { }
  void wrUInt8(OdUInt8 ) { }
  void wrInt16(OdInt16 ) { }
  void wrInt32(OdInt32 ) { }
  void wrInt64(OdInt64 ) { }
  void wrDouble(double ) { }
  void wrDbHandle(const OdDbHandle& ) { }
  void wrSoftPointerId(const OdDbObjectId& ) { }
  void wrHardPointerId(const OdDbObjectId& ) { }
  void wrPoint2d(const OdGePoint2d& ) { }
  void wrPoint3d(const OdGePoint3d& ) { }
  void wrVector2d(const OdGeVector2d& ) { }
  void wrVector3d(const OdGeVector3d& ) { }
  void wrScale3d(const OdGeScale3d& ) { }
  bool usesReferences() { return true; }

  void wrSoftOwnershipId( const OdDbObjectId& id ) {
    if( m_pQueuePage[0] && !id.isNull() ) {
      m_pQueuePage[0]->append( id );
    }
  }
  void wrHardOwnershipId( const OdDbObjectId& id ) {
    Traverser::wrSoftOwnershipId( id );
  }
  void addReference( OdDbObjectId id, OdDb::ReferenceType rt ) {
    if( GETBIT(rt, 2) ) {// OwnershipRef
      Traverser::wrSoftOwnershipId( id );
    }
  }
  void wrAddress(const void* val) {
    switch( m_nEmbeddedSubobjectFollowState ) {
      case 0:
        // "this" pointer used as an identifier that embedded subobject follows
        // Caller calls: pFiler->wrAddress( pFiler ) and then immediately - pFiler->wrAddress( pEmbeddedSubobject )
        if( val == this )
          m_nEmbeddedSubobjectFollowState = 1;
        break;
      case 1:
        if ( !m_pDoer->action( m_pObj, (const OdRxObject*)val ) ) {
          m_pQueuePage[0] = 0;
        }
        m_nEmbeddedSubobjectFollowState = 2;
        break;
      case 2:
        m_nEmbeddedSubobjectFollowState = 0;
        break;
    }
  }
};

class TraverseDoer : public Traverser::Doer {
public:
  const CDwgView::Condition* m_pCondition;
  OdDbObjectIdArray m_found;
  OdRxObjectRawPtrArray m_foundSub;

  bool action( const OdDbObject* pObj, const OdRxObject* pSubObj ) {
    if( m_pCondition->check( pObj, pSubObj ) ) {
      m_found.append( pObj->objectId() );
      if( pSubObj ) {
        m_foundSub.resize( m_found.size()-1, 0 );
        m_foundSub.append( pSubObj );
      }
    }
    return true;
  }
};

void CDwgView::Find( const CDwgView::Condition& condition )
{
  m_found.clear();
  m_foundSub.clear();
  OdStaticRxObject<Traverser> traverser;
  TraverseDoer doer;
  doer.m_pCondition = &condition;
  traverser.traverse( GetDocument()->m_pDb, doer );
  m_found = doer.m_found;
  m_foundSub = doer.m_foundSub;
  m_nCurFound = 0;
}

void CDwgView::Find(const OdDbObjectId& targetId, const OdRxObject* pSubObj )
{
  // Building parentship queue
  OdDbObjectIdArray parents;
  OdDbObjectId id = targetId;
  HTREEITEM hRoot = 0;
  while(!id.isNull() && id!=id.originalDatabase()->objectId())
  {
    OdDbObjectPtr pObj = id.openObject();
    if (!pObj.isNull())
    {
      parents.push_back(id);
      id = pObj->ownerId();
    }
    else
      break;
  }
  
  // Opening...
  HTREEITEM item = m_ObjectTree.GetChildItem( m_ObjectTree.GetRootItem() );
  int num = parents.size()-1;
  while( num >= 0 && item )
  {
    for(; item != NULL; item = m_ObjectTree.GetNextItem(item, TVGN_NEXT))
    {
      OdHWnd data = m_ObjectTree.GetItemData(item);
      if ( data == (OdHWnd)((OdDbStub*)parents[num]) )
        break;
    }
    if ( !item )
    {
      ODA_FAIL();
      break;
    }
    if( !pSubObj )
      m_ObjectTree.SelectItem( item );
    hRoot = item;
    if ( num-- )
    {
      m_ObjectTree.Expand( item, TVE_EXPAND );
      item = m_ObjectTree.GetChildItem( item );
    }
  }
  if( pSubObj ) {
    CString sub;
    sub.Format( _T("0x%08X"), pSubObj );
    CDwgView::Find( hRoot, sub );
  }
}

bool CDwgView::Find( HTREEITEM hRoot, const CString& sub )
{
  CString sItem = m_ObjectTree.GetItemText( hRoot );
  if( sItem.Find(sub)==-1 ) {
    HTREEITEM hItem = m_ObjectTree.GetChildItem( hRoot );
    if( hItem==0 && m_ObjectTree.ItemHasChildren( hRoot ) ) {
      m_ObjectTree.Expand( hRoot, TVE_EXPAND );
      m_ObjectTree.Expand( hRoot, TVE_COLLAPSE );
      hItem = m_ObjectTree.GetChildItem( hRoot );
    }
    while ( hItem ) {
      if( Find( hItem, sub ) ) {
        m_ObjectTree.Expand( hRoot, TVE_EXPAND );
        return true;
      }
      hItem = m_ObjectTree.GetNextItem( hItem, TVGN_NEXT );
    }
  }
  else {
    m_ObjectTree.SetItemState( hRoot, TVIS_SELECTED, TVIS_SELECTED );
    return true;
  }
  return false;
}

class ClassNameCond : public CDwgView::Condition {
  const OdRxClass* m_pClassToFind;
public:
  OdDbObjectId id;
  ClassNameCond( const OdRxClass* pClass ) {
    m_pClassToFind = pClass;
  }
  bool check( const OdDbObject* pObj, const OdRxObject* pSubObj ) const {
    const OdRxClass* pClass = ( pSubObj ? pSubObj->isA() : ( pObj ? pObj->isA() : 0 ) );
    if( pClass==m_pClassToFind ) {
      return true;
    }
    return false;
  }
};
#include <OdUtilAds.h>
class StringCond : public CDwgView::Condition
{
  OdString _value;
public:
  StringCond(const TCHAR* s) : _value(s) {}
  virtual bool check( const OdDbObject* pObj, const OdRxObject* ) const
  {
    OdResBufPtr data = oddbEntGet(pObj);
    for (OdResBufPtr rb = data; !rb.isNull(); rb = rb->next())
    {
      if (OdDxfCode::_getType(rb->restype()) == OdDxfCode::String)
      {
        if (odutWcMatch(rb->getString(), _value))
          return true;
      }
    }
    return false;
  }
};

void CDwgView::OnEditFind()
{
  CFindObjectDlg fd;
  if(fd.DoModal()==IDOK)
  {
    // Requesting ObjectId
    OdDbObjectId targetId;
    // Requesting subobject
    const OdRxObject* pSubObj = 0;

    
    if ( fd.m_sCriteria == _T("class name") ) 
    {
      OdRxClass* pRxClass = (OdRxClass*)odrxClassDictionary()->getAt((LPCTSTR)fd.m_objHandle).get();
      if (pRxClass)
      {
        CDwgView::Find( ClassNameCond( pRxClass ) );
        if( m_found.empty() ) {
          MessageBox(_T("No matching item found"), _T("Error"), MB_OK + MB_ICONSTOP);
          return;
        }
        targetId = m_found.first();
        pSubObj = m_foundSub.isEmpty() ? 0 : m_foundSub.first();
      }
    }
    else if (fd.m_sCriteria == _T("string"))
    {
      CDwgView::Find( StringCond( fd.m_objHandle ) );
      if( m_found.empty() ) {
        MessageBox(_T("No matching item found"), _T("Error"), MB_OK + MB_ICONSTOP);
        return;
      }
      targetId = m_found.first();
    }
    else if (fd.m_sCriteria == _T("handle"))
    {
      // Calculating handle
      OdDbHandle han((LPCTSTR)fd.m_objHandle);
      OdDbDatabase* pDb = GetDocument()->m_pDb;
      targetId = pDb->getOdDbObjectId(han);
      if(!targetId.isValid())
      {
        MessageBox(_T("Incorrect handle"), _T("Error"), MB_OK + MB_ICONSTOP);
        return;
      }
    }
    CDwgView::Find( targetId, pSubObj );
  }
}

void CDwgView::OnRefresh()
{
  m_ObjectTree.SelectItem(0);
  m_ObjectTree.DeleteAllItems( );
  fillTree();
}

class OdDatabaseVarDisplayer
{
public:
  inline OdDatabaseVarDisplayer(bool val, OdDbDwgFiler* pFiler)
  {
    // ??? pFiler->wrInt16(OdInt16(val ? 1 : 0));
    pFiler->wrBool(val);
  }
  inline OdDatabaseVarDisplayer(OdUInt8 val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(val);
  }
  inline OdDatabaseVarDisplayer(OdUInt16 val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(val);
  }
  inline OdDatabaseVarDisplayer(OdInt16 val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(val);
  }
  inline OdDatabaseVarDisplayer(OdInt32 val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt32(val);
  }
  inline OdDatabaseVarDisplayer(OdInt64 val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt64(val);
  }
  inline OdDatabaseVarDisplayer(OdCmColor val, OdDbDwgFiler* pFiler)
  {
    OdCmEntityColor::ColorMethod cm = val.colorMethod();
    OdString sCm;
    switch(cm)
    {
    case OdCmEntityColor::kByLayer:
      sCm += _T("ByLayer \"");
      break;
    case OdCmEntityColor::kByBlock:
      sCm += _T("ByBlock \"");
      break;
    case OdCmEntityColor::kByColor:
      {
        OdString sTmp;
        sTmp.format(OD_T("R=%d G=%d B=%d \""), val.red(), val.green(), val.blue());
        sCm += sTmp;
      }
      break;
    case OdCmEntityColor::kByACI:
      {
        OdString sTmp;
        sTmp.format(OD_T("index=%d \""), val.colorIndex());
        sCm += sTmp;
      }
      break;
    case OdCmEntityColor::kForeground:
      sCm += OD_T("Foreground \"");
      break;
    case OdCmEntityColor::kNone:
      sCm += OD_T("None \"");
      break;
    default:
      sCm.format(OD_T("Color method unknown - %H \""), cm);
    }
    sCm += val.colorName();
    sCm += OD_T("\" \"");
    sCm += val.bookName();
    sCm += OD_T("\"");
    pFiler->wrString(sCm);
  }
  inline OdDatabaseVarDisplayer(OdCmTransparency val, OdDbDwgFiler* pFiler)
  {
    OdString sVal;
    if (val.isByLayer())
      sVal = L"ByLayer";
    else if (val.isByBlock())
      sVal = L"ByBlock";
    else if (val.isByAlpha())
      sVal.format(L"%d", 100 - OdRoundToLong(val.alphaPercent() * 100.0));
    else if (val.isInvalid())
      sVal = L"Invalid";
    else
    {
      ODA_FAIL_ONCE();
    }
    pFiler->wrString(sVal);
  }
  inline OdDatabaseVarDisplayer(double val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrDouble(val);
  }
  inline OdDatabaseVarDisplayer(const OdString& val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrString(val);
  }
  inline OdDatabaseVarDisplayer(OdInt8 val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(val);
  }
  inline OdDatabaseVarDisplayer(OdDb::LineWeight val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(OdUInt16(val));
  }
  inline OdDatabaseVarDisplayer(const OdGePoint3d& val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrPoint3d(val);
  }
  inline OdDatabaseVarDisplayer(const OdGeVector3d& val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrPoint3d(OdGePoint3d::kOrigin + val);
  }
  inline OdDatabaseVarDisplayer(const OdGePoint2d& val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrPoint3d(OdGePoint3d(val.x,val.y,0.));
  }
  inline OdDatabaseVarDisplayer(const OdDbObjectId& val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrSoftPointerId(val);
  }
  inline OdDatabaseVarDisplayer(const OdDbDate& val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrDouble(val.julianFraction());
  }
  inline OdDatabaseVarDisplayer(OdDb::MeasurementValue val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(OdInt16(val));
  }
  inline OdDatabaseVarDisplayer(OdDb::EndCaps val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(OdInt16(val));
  }
  inline OdDatabaseVarDisplayer(OdDb::JoinStyle val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(OdInt16(val));
  }
  inline OdDatabaseVarDisplayer(OdDb::UnitsValue val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(OdInt16(val));
  }
  inline OdDatabaseVarDisplayer(OdDb::PlotStyleNameType val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(OdInt16(val));
  }
  inline OdDatabaseVarDisplayer(OdCodePageId val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(OdInt16(val));
  }
  inline OdDatabaseVarDisplayer(OdDb::TimeZone val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(OdUInt16(val));
  }
  inline OdDatabaseVarDisplayer(OdGiSubEntityTraits::ShadowFlags val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(OdUInt16(val));
  }
  inline OdDatabaseVarDisplayer(OdDb::LoftParamType val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(OdUInt16(val));
  }
  inline OdDatabaseVarDisplayer(OdDb::LoftNormalsType val, OdDbDwgFiler* pFiler)
  {
    pFiler->wrInt16(OdUInt16(val));
  }
  inline OdDatabaseVarDisplayer(OdDbAnnotationScalePtr val, OdDbDwgFiler* pFiler)
  {
    if (val.isNull())
      pFiler->wrString("");
    else
      pFiler->wrString(val->getName());
  }
};

// Sort the item in alphabetical order.
static int CALLBACK
CompareSysVarProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  // lParamSort contains a pointer to the list view control.
  // The lParam of an item is just its index.
  CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
  CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);
  CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);

  return _tcscmp(strItem1, strItem2);
}

void CDwgView::OnViewSystemvariables()
{
  m_ObjectList.DeleteAllItems();
  OdDbDatabase* pDb = GetDocument()->m_pDb;
  ListFillerDwg lf(pDb, &m_ObjectList);
  OdString sName;
  DWORD itemid;
#define VAR_DEF(type, name, def_val, metric_def_value, reserve1, reserve2)  \
  OdDatabaseVarDisplayer(pDb->get##name(), &lf);\
  itemid = m_ObjectList.GetItemCount()-1;\
  m_ObjectList.SetItemText(itemid, 0, _T(#name));\
  m_ObjectList.SetItemData(itemid, itemid);
#include "SysVarDefs.h"
#undef VAR_DEF

  OdDatabaseVarDisplayer(pDb->getTDCREATE(), &lf);
  itemid = m_ObjectList.GetItemCount()-1;
  m_ObjectList.SetItemText(itemid, 0, _T("TDCREATE"));
  m_ObjectList.SetItemData(itemid, itemid);
  OdDatabaseVarDisplayer(pDb->getTDUPDATE(), &lf);
  itemid = m_ObjectList.GetItemCount()-1;
  m_ObjectList.SetItemText(itemid, 0, _T("TDUPDATE"));
  m_ObjectList.SetItemData(itemid, itemid);

#define VAR_DEF(type, name, dxf, def_val, metric_def_value, reserve1, reserve2)  \
  sName = _T("DIM") _T(#name); sName.makeUpper();\
  OdDatabaseVarDisplayer(pDb->dim##name(), &lf);\
  itemid = m_ObjectList.GetItemCount()-1;\
  m_ObjectList.SetItemText(itemid, 0, sName);\
  m_ObjectList.SetItemData(itemid, itemid);
#include "DimVarDefs.h"
#include ".\dwgview.h"
#undef VAR_DEF
  m_ObjectList.SortItems(CompareSysVarProc, (LPARAM)&m_ObjectList);
}

void CDwgView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
  if(cx > 10 && cy > 10 && m_ObjectTree.m_hWnd)
  {
    CRect crc;
    GetClientRect(&crc);
    crc.DeflateRect(2,2,2,2);
    m_ObjectTree.SetWindowPos(NULL, crc.left, crc.top, crc.Width()/2-1, crc.Height(), SWP_NOZORDER);
    m_ObjectList.SetWindowPos(NULL, crc.left+crc.Width()/2+1, crc.top, crc.Width()/2-1, crc.Height(), SWP_NOZORDER);
  }
}

void CDwgView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
  fillTree();
}

void CDwgView::OnItemexpandingOwnershiptree(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

  // handle of selected item as tree member
  HTREEITEM hChild = m_ObjectTree.GetChildItem(pNMTreeView->itemNew.hItem);
  if(!hChild)
  {
    CMainFrame* pWnd = (CMainFrame*)theApp.GetMainWnd();
    pWnd->m_wndStatusBar.SetPaneText(0, _T("Filling tree..."));
    HCURSOR cursor = GetCursor();
    SetCursor(LoadCursor(NULL, IDC_WAIT));
    try
    {
      HTREEITEM h = pNMTreeView->itemNew.hItem;
      OdDbObjectPtr pObj = OdDbObjectId((OdDbStub*)m_ObjectTree.GetItemData( h )).safeOpenObject();
      TreeFiller treeFiller(GetDocument()->m_pDb, &m_ObjectTree, GetDocument()->selectionSet());
      const OdRxObject* pAECObj = AECObject( pObj, m_ObjectTree.GetItemText( h ) );
      if ( pAECObj )
        treeFiller.outEmbeddedSubObjects2Tree( pAECObj, h );
      else
        treeFiller.outSubObjects2Tree( pObj, h );
    }
    catch(const OdError& e)
    {
      theApp.reportError(_T("Filling tree..."), e);
    }
    catch(...)
    {
      theApp.reportError(_T("Filling tree..."), eOk);
    }
    SetCursor(cursor);
    pWnd->m_wndStatusBar.SetPaneText(0, _T("Ready"));
  }

	*pResult = 0;
}

void CDwgView::OnRclickOwnershiptree(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
  DWORD pos = GetMessagePos();
  CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
  m_ObjectTree.ScreenToClient( &pt );
  if ( HTREEITEM hItem = m_ObjectTree.HitTest( pt ) )
    m_ObjectTree.SelectItem( hItem );
  CMenu *pPopUpMenu = theApp.GetMainWnd()->GetMenu()->GetSubMenu(2);
  pPopUpMenu->TrackPopupMenu(TPM_LEFTALIGN, GET_X_LPARAM(pos), GET_Y_LPARAM(pos), this);
	*pResult = 1;
}

#include "DbLeader.h"
void CDwgView::OnEditRecomputeDimBlock()
{
  try
  {
    OdDbDatabase* pDb = GetDocument()->m_pDb;
    pDb->startUndoRecord();

    OdDbBlockTablePtr pBlockTbl = pDb->getBlockTableId().openObject();
    OdDbSymbolTableIteratorPtr  block_it(pBlockTbl->newIterator());
    while (!block_it->done())
    {
      OdDbBlockTableRecordPtr pBlock(block_it->getRecordId().safeOpenObject());
      block_it->step();
      if (!pBlock->isAnonymous())
      {
        OdDbObjectIteratorPtr pEntIter;
        for(pEntIter = pBlock->newIterator(); !pEntIter->done(); pEntIter->step())
        {
          OdDbDimensionPtr pDim = OdDbDimension::cast( pEntIter->entity(OdDb::kForWrite) );
          if (pDim.get())
          {
            pDim->recomputeDimBlock();
            pDim->recordGraphicsModified(false);
          } else 
          {
            OdDbLeaderPtr pLdr = OdDbLeader::cast( pEntIter->entity(OdDb::kForWrite) );
            if ( pLdr.get() )
            {
              pLdr->evaluateLeader();
              pLdr->recordGraphicsModified(false);
            }
          }
        }
      }
    }
  }
  catch(const OdError& e)
  {
    theApp.reportError(0, _T("recomputeDimBlock aborted"), e);
  }
}

void CDwgView::OnUpdateExplode(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(GetDocument()->selectionSet()->numEntities());
}

void CDwgView::OnViewRenderMaterials() 
{
  CMaterialsDlg matDlg(GetDocument()->m_pDb);
  matDlg.DoModal();
}

void CDwgView::OnViewRenderLight() 
{
  CLightsDlg ltDlg(GetDocument()->m_pDb);
  ltDlg.DoModal();
}

void CDwgView::OnEditAcisConversion() 
{
	// get current database
  OdDbDatabase* pDb = GetDocument()->m_pDb;
  pDb->startUndoRecord();

  // get Modelspace block table record
  OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject();

  // Step ower only model space block entities.
  OdDbObjectIteratorPtr pEntIter = pMS->newIterator();
  while(!pEntIter->done())
  {
    // next entity
    OdDbEntityPtr pEnt = pEntIter->entity(OdDb::kForWrite);
    
    // step should be here. Operations like handOverTo\erase is unsafe for iteration entity sequence.
    pEntIter->step();

    // proceed with SAT based entities only
    if (!pEnt.isNull() 
        && (pEnt->isKindOf(OdDbSurface::desc()) 
        ||  pEnt->isKindOf(OdDbRegion::desc())
        ||  pEnt->isKindOf(OdDbBody::desc())
        ||  pEnt->isKindOf(OdDb3dSolid::desc())))
    {
      bool exchangeXData;
      OdDbObjectId replaceId;

      // decomposeForSave with vAC12 input parameter return pointer to DbMesh entity or to Block
      OdDbObjectPtr pNewObj = pEnt->decomposeForSave(OdDb::vAC12, replaceId, exchangeXData);
      if (!pNewObj.isNull())
      {
        pEnt->handOverTo(pNewObj);
      }
    }
  }
  fillTree();
}

void CDwgView::OnViewFiledependency() 
{
  CFileDependencyDlg fd(GetDocument()->m_pDb, this);
  fd.DoModal();
}

void CDwgView::OnEditAcisSatSave() 
{
  try
  {
    CAcisSaveDialog pd(curObjectId(), this);
    pd.DoModal();
  }
  catch (HRESULT& /*hr*/)
  {
    MessageBox(_T("Can not create Acis Save To dialog."));
  }
}

void CDwgView::OnEditReduce()
{
  COdaMfcAppDoc* pDoc = GetDocument();
  OdDbSelectionSet& sset = *pDoc->selectionSet();
  OdDbObjectId selId;
  HTREEITEM hItem = m_ObjectTree.GetSelectedItem();
  if (hItem)
  {
    selId = (OdDbStub*)(m_ObjectTree.GetItemData(hItem));
    sset.append(selId);
  }
  else
  {
    selId = OdDbSelectionSetIterator::cast(sset.newIterator())->objectId();
  }
  pDoc->OnEditReduce();
  if (hItem)
    sset.remove(selId);
  Find(selId);
}

void CDwgView::OnNMClickOwnershiptree(NMHDR*, LRESULT *pResult)
{
  *pResult = 0;
  if ( ::GetKeyState(VK_CONTROL) & 0xf0000000 )
  {
    CPoint p; ::GetCursorPos(&p);
    m_ObjectTree.ScreenToClient(&p);
    UINT flags = 0; 
    HTREEITEM hItem = m_ObjectTree.HitTest(p,&flags);
    if ( flags & TVHT_ONITEM && m_ObjectTree.GetSelectedItem()==hItem )
    {
      // change state
      m_ObjectTree.SetItemState( hItem, ( m_ObjectTree.GetItemState( hItem, TVIS_BOLD ) ^ TVIS_BOLD ), TVIS_BOLD );
    }
  }
}

void CDwgView::OnEditFindnextselected()
{
  if( m_found.size() ) {
    ++m_nCurFound;
    if( m_nCurFound >= m_found.size() )
      m_nCurFound = 0;
    Find( m_found[m_nCurFound], m_foundSub[m_nCurFound] );
    return ;
  }
  OdDbSelectionSetIteratorPtr i = GetDocument()->selectionSet()->newIterator();
  if (HTREEITEM hItem = m_ObjectTree.GetSelectedItem())
  {
    OdDbObjectId id = (OdDbStub*)m_ObjectTree.GetItemData(hItem);
    if (GetDocument()->selectionSet()->isMember(id))
    {
      while ( i->objectId() != id )
        i->next();
      i->next();
    }
  }
  if ( !i->done() )
    Find( i->objectId() );
}

void CDwgView::OnEditClearSelection() 
{
  GetDocument()->OnEditClearselection();
  OnRefresh();
}
