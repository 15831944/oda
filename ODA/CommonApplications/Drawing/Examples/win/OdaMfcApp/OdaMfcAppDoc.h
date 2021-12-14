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

/************************************************************************/
/* Defines the interface of the COdaMfcAppDoc class                     */
/************************************************************************/
#if !defined(AFX_ODAMFCAPPDOC_H__BFCBB1B3_DB28_48F5_B96E_35E4EE840454__INCLUDED_)
#define AFX_ODAMFCAPPDOC_H__BFCBB1B3_DB28_48F5_B96E_35E4EE840454__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DbDatabase.h"
#include "DbCommandContext.h"
#include "Ed/EdCommandStack.h"
#include "ExDbCommandContext.h"
#include "StaticRxObject.h"
#include "SharedPtr.h"
#include "DbLayoutManager.h"
#ifdef ENABLE_VBA_SUPPORT
#include "resource.h"
#include "OdaX.h"
#endif // ENABLE_VBA_SUPPORT

#ifdef ODAMFC_EXPORT_SYMBOL
# include "OdaMfcExportImpl.h"
#endif // ODAMFC_EXPORT_SYMBOL

#include "DbAssocManager.h"

#ifdef MECHANICAL_MODE 
  #include "OdDbDatabaseMech.h"
  #define PARAMDATABASE OdDbDatabaseMech
#else 
  #define PARAMDATABASE OdDbDatabase
#endif

class CDwgViewer;
class OdEdCommand;
class COdaMfcAppDoc;
class CUserIOConsole;
class ExStringIO;

/************************************************************************/
/* Specialization of the OdEdCommand class to add a VIEW command        */
/************************************************************************/
class Cmd_VIEW : public OdEdCommand
{
public:
  static const OdString name();
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCommandContext);
  const OdRxModule* commandApp() const;
  void              commandUndef(bool undefIt);
  OdInt32           commandFlags() const;
};

/************************************************************************/
/* Specialization of the OdEdCommand class to add a SELECT command        */
/************************************************************************/
class Cmd_SELECT : public OdEdCommand
{
public:
  static const OdString name();
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCommandContext);
  const OdRxModule* commandApp() const;
  void              commandUndef(bool undefIt);
  OdInt32           commandFlags() const;
};


class Cmd_DISPLAY_DIFFS : public OdEdCommand
{
public:
  static const OdString name();
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCommandContext);
  const OdRxModule* commandApp() const;
  void              commandUndef(bool undefIt);
  OdInt32           commandFlags() const;
};

/************************************************************************/
/* Specialization of the OdEdCommand class to add a SAVE command        */
/************************************************************************/
class Cmd_SAVE : public OdEdCommand
{
public:
  static const OdString name();
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCommandContext);
};

/************************************************************************/
/* Specialization of the OdEdCommand class to add a SAVEAS command      */
/************************************************************************/
class Cmd_SAVEAS : public OdEdCommand
{
public:
  static const OdString name();
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCommandContext);
};


class OdDbDatabaseDoc : public PARAMDATABASE
{
  static  COdaMfcAppDoc*  g_pDoc;
  mutable COdaMfcAppDoc*  m_pDoc;
public:
  ODRX_DECLARE_MEMBERS(OdDbDatabaseDoc);
  
  OdDbDatabaseDoc();

  COdaMfcAppDoc* document() const;

  static void setDocToAssign(COdaMfcAppDoc* pDoc);
};

typedef OdSmartPtr<OdDbDatabaseDoc> OdDbDatabaseDocPtr;

class COdaMfcAppDocStaticRxObjects : public OdDbLayoutManagerReactor
                                   , public OdEdBaseIO
{
  ODRX_NO_HEAP_OPERATORS();
};

#ifdef ENABLE_VBA_SUPPORT
using namespace MSAPC;
#endif // ENABLE_VBA_SUPPORT




class COdaMfcAppDoc : 
#ifdef ENABLE_VBA_SUPPORT
  public CApcDualDyn<CApcDocument<COdaMfcAppDoc,COleDocument>, Document, IOdaMfcDoc,&IID_IOdaMfcDoc>,
#else
  public COleDocument,
#endif // ENABLE_VBA_SUPPORT
  protected OdStaticRxObject<COdaMfcAppDocStaticRxObjects>
{
protected:
#ifdef ENABLE_VBA_SUPPORT
  using CApcDualDyn<CApcDocument<COdaMfcAppDoc,COleDocument>,    Document, IOdaMfcDoc,&IID_IOdaMfcDoc>::operator new;
  using CApcDualDyn<CApcDocument<COdaMfcAppDoc,COleDocument>,    Document, IOdaMfcDoc,&IID_IOdaMfcDoc>::operator delete;
#else
  using COleDocument::operator new;
  using COleDocument::operator delete;
#endif // ENABLE_VBA_SUPPORT
  CDwgViewer*         m_pViewer;
  bool                m_bConsole;
  bool                m_bConsoleResponded;
  int                 m_nCmdActive;

  bool                m_bTryIncSave;
  CUserIOConsole* console();

  class DataSource : COleDataSource
  {
    friend class COdaMfcAppDoc;
    OdString m_tmpPath;
  public:
    DataSource();
    void Create(COdaMfcAppDoc* pDoc, const OdGePoint3d& p2 = OdGePoint3d::kOrigin);
    bool DoDragDrop();
    void Empty();
    ~DataSource();
  };

  template<class TChar>
  struct AcadClipData
  {
    void init()
    {
      memset(this, 0, sizeof(AcadClipData<TChar>));
    }
    void read(CFile* pFile)
    {
      pFile->Read( this, sizeof(AcadClipData<TChar>) );
    }

    TChar _tempFileName[0x104];   // name of the temp dwg file, where dragged entities are
    TChar _origFileName[0x104];   // original file name
    TChar _version[4];            // version of the original file, e.g. 'R15'
    int _one1;                    // seem to be always 1
    double _x, _y, _z;            // pick point
    int _zero1;                   // seem to be always zero
    int _one2;                    // seem to be always 1
    int _unk[4];                  //
    int _zero2[4];                // seem to be always zero
  };

  template<class TChar>
  struct AcadClipDataConstr : public AcadClipData<TChar>
  {
    AcadClipDataConstr(const OdString& tempFileName,const OdString& origFileName, const OdGePoint3d& pickPoint)
    {
      init();
      _one1 = 1;
      _one2 = 1;
      _version[0] = 'R';
      _version[1] = '1';
      _version[2] = '5';
      _version[3] = 0;
      _x = pickPoint.x;
      _y = pickPoint.y;
      _z = pickPoint.z;
      memcpy( _tempFileName, (const TChar*)tempFileName, odmin((0x100*sizeof(TChar)),((tempFileName.getLength()+1)*sizeof(TChar))) );
      memcpy( _origFileName, (const TChar*)origFileName, odmin((0x100*sizeof(TChar)),((origFileName.getLength()+1)*sizeof(TChar))) );
    }
    AcadClipDataConstr()
    {
      init();
    }
  };
  typedef AcadClipDataConstr<char> AcadClipDataR15;
  typedef AcadClipDataConstr<OdChar> AcadClipDataR21;

public:

  class ClipboardData
  {
  public:
    static unsigned short m_FormatR15;
    static unsigned short m_FormatR16;
    static unsigned short m_FormatR17;
    static unsigned short m_FormatR18;
    static unsigned short m_FormatR19;
    static bool isAcadDataAvailable(COleDataObject* pDataObject, bool bAttach = false)
    {
      if (bAttach && !pDataObject->AttachClipboard())
      {
        return false;
      }
      return pDataObject->IsDataAvailable(m_FormatR15) 
        || pDataObject->IsDataAvailable(m_FormatR16)
        || pDataObject->IsDataAvailable(m_FormatR17)
        || pDataObject->IsDataAvailable(m_FormatR18)
        || pDataObject->IsDataAvailable(m_FormatR19);
    }
    static OdSharedPtr<ClipboardData> get(COleDataObject* pDataObject, bool bAttach = false)
    {
      if (bAttach && !pDataObject->AttachClipboard())
        return 0;

      OdSharedPtr<ClipboardData> pData = new ClipboardData();
      if (pData->read(pDataObject))
        return pData;
      return 0;
    }

    ClipboardData()
      : _isR15format(false)
    {
    }

    bool read(COleDataObject* pDataObject)
    {
      OdSharedPtr<CFile> pFile = 0;
      if ( (pFile = pDataObject->GetFileData(m_FormatR15)).get()
        || (pFile = pDataObject->GetFileData(m_FormatR16)).get() )
      {
        _isR15format = true;
        _data._r15.read(pFile);
        return true;
      }
      else if ( (pFile = pDataObject->GetFileData(m_FormatR17)).get()
             || (pFile = pDataObject->GetFileData(m_FormatR18)).get() 
             || (pFile = pDataObject->GetFileData(m_FormatR19)).get()
             )
      {
        _isR15format = false;
        _data._r21.read(pFile);
        return true;
      }
      else
      {
        return false;
      }
    }

    OdString tempFileName() { return _isR15format ? OdString(_data._r15._tempFileName) : OdString(_data._r21._tempFileName); }
    OdGePoint3d pickPoint() { return _isR15format ? OdGePoint3d(_data._r15._x, _data._r15._y, _data._r15._z) : OdGePoint3d(_data._r21._x, _data._r21._y, _data._r21._z); }

  private:
    union Data
    {
      AcadClipData<char>   _r15;
      AcadClipData<OdChar> _r21;
      Data() { _r21.init(); }
    } _data;
    bool _isR15format;
  };

protected:
  COdaMfcAppDoc();
  DECLARE_DYNCREATE(COdaMfcAppDoc)

  BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
  // Attributes
  OdDbCommandContextPtr       m_pCmdCtx;
  OdSmartPtr<CUserIOConsole>  m_pConsole;
  OdSmartPtr<ExStringIO>      m_pMacro;

  OdDbCommandContextPtr cmdCtx();
  OdEdBaseIO* cmdIO();
  OdString commandPrompt();
  OdString recentCmd();
  OdString recentCmdName();
  
  // OdEdBaseIO
  OdString getString(const OdString& prompt, int options, OdEdStringTracker* pTracker);
  void putString(const OdString& string);
  OdGePoint3d getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker);
  virtual OdUInt32 getKeyState();

  // OdDbLayoutManagerReactor
  bool m_bLayoutSwitchable;
  void layoutSwitched(const OdString& name, const OdDbObjectId& id);
  bool m_bDisableClearSel;
public:
  bool              m_bPartial;
  OdDb::DwgVersion  m_SaveAsVer;
  OdDb::SaveType    m_SaveAsType;
  // Operations
public:
  OdDbSelectionSetPtr selectionSet() const;
  CDwgViewer* getViewer();
  void OnCloseVectorizer(CDwgViewer* pView);
  void setVectorizer(CDwgViewer* pView);
  void ExecuteCommand(const OdString& sCmd, bool bEcho = true);
  OdDbDatabasePtr m_pDb;
  void DeleteSelection(bool bForce);
  void startDrag(const OdGePoint3d& pt);

  bool getTryIncSave();
  void setTryIncSave(bool tryIncSave);

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COdaMfcAppDoc)
  public:
  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
  virtual BOOL OnNewDocument();
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  virtual void DeleteContents();
  virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~COdaMfcAppDoc();
  virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:

  // Generated message map functions
protected:
  void OnVectorize(const OdString& sVectorizerPath);

  //{{AFX_MSG(COdaMfcAppDoc)
  afx_msg void OnRefresh();
  afx_msg void OnEditAudit();
  afx_msg void OnViewSetactivelayout();
  afx_msg void OnViewConstrDisplay();
  afx_msg void OnUpdateViewConstrDisplay(CCmdUI* pCmdUI);
  afx_msg void OnViewNamedViews();
  afx_msg void OnEditUndo();
  afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
  afx_msg void OnEditRedo();
  afx_msg void OnEditZoomextents();
  afx_msg void OnEditIdeditlinetype();
  afx_msg void OnViewSumminfo();
  afx_msg void OnExportPdf();
  afx_msg void OnExportIfc();                                     // AMatvienko, exportToIfc
  afx_msg void OnExportHoops();                                   // RAV 12.26.08
  afx_msg void OnExportCollada();                                 // ET Feb 08, 2012
  afx_msg void OnExport();                                        // MKU 03.01.2003
  afx_msg void OnPublish();                                       // MKU 03.01.2003
  afx_msg void On3DPublish();                                     // MKU 09.08.2005
  afx_msg void OnBmpExport();                                     // AMark 28.12.2009
  afx_msg void OnUpdatePdfExport(CCmdUI* pCmdUI);
  afx_msg void OnUpdateHoopsExport(CCmdUI* pCmdUI);               // RAV 12.26.08
  afx_msg void OnUpdateColladaExport(CCmdUI* pCmdUI);             // ET Feb 08, 2012
  afx_msg void OnUpdateExport(CCmdUI* pCmdUI);
  afx_msg void OnUpdatePublish(CCmdUI* pCmdUI);
  afx_msg void OnUpdate3DPublish(CCmdUI* pCmdUI);
  afx_msg void OnUpdateBmpExport(CCmdUI* pCmdUI);                  // AMark 28.12.2009
  afx_msg void OnUpdateIfcExport(CCmdUI* pCmdUI);                  // AMatvienko, exportToIfc
  afx_msg void OnFormatLayer();                                    // AE 18.02.2003 
  afx_msg void OnFormatLinetype();                                 // OSHA 18.08.2005
  afx_msg void OnFormatMlstyle();                                  // OSHA 19.08.2005
  afx_msg void OnFormatBackgroundColor();                          // AE 08.04.2003 
  afx_msg void OnFormatLineweightsettings();
  afx_msg void OnInsertXrefmanager();
  afx_msg void OnInsertDwfUnderlay();
  afx_msg void OnInsertPdfUnderlay();
  afx_msg void OnInsertDgnUnderlay();                              // AMark 06.12.2007
  afx_msg void OnImportDwf();                                        // WVK 01.10.2003
  afx_msg void OnImportCollada();
  afx_msg void OnImportPdf();
  afx_msg void OnVectorize();
  afx_msg void OnRemoteGeomViewer();
  afx_msg void OnExportSvg();
  afx_msg void OnEditConsole();
  afx_msg void OnOptionsTriangulationParams();
  afx_msg void OnOptionsSetFACETRES();
  afx_msg void OnPageSetup();                                     // MKU 21.05.2004
  afx_msg void OnEditRecentcommand();
  afx_msg void OnUpdateEditRecentcommand(CCmdUI* pCmdUI);
  afx_msg void OnPurgeAll();
  afx_msg void OnUpdatePurge(CCmdUI* pCmdUI);
  afx_msg void OnPurgeBlocks();
  afx_msg void OnPurgeDimStyles();
  afx_msg void OnPurgeLayers();
  afx_msg void OnPurgeLtypes();
  afx_msg void OnPurgeTextStyles();
  afx_msg void OnInsertOleobject();
  afx_msg void OnFilePlotStyleManager();
  afx_msg void OnUpdateVectorize(CCmdUI* pCmdUI);
  afx_msg void OnEditExplode();
  afx_msg void OnEditEntget();
  afx_msg void OnOptionsDisableAnnotations();
  afx_msg void OnOptionsDisableFieldUpdate();
  afx_msg void OnViewRenderDbLights();
  afx_msg void OnViewRenderDbMaterials();
  afx_msg void OnViewRenderDbVisualStyles();
  afx_msg void OnViewRenderEnvironment();
  afx_msg void OnRevisionLog();
  afx_msg void OnRevisionLogUI(CCmdUI *pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEditSelectall();
  afx_msg void OnEditClearselection();
  afx_msg void OnEditReduce();
  afx_msg void OnEditPaste();
  afx_msg void OnEditCopy();
  afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
  afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
  afx_msg void OnOptionsDisableAnnotationsUI(CCmdUI *pCmdUI);
  afx_msg void OnOptionsDisableFieldUpdateUI(CCmdUI *pCmdUI);
#ifdef ODAMFC_EXPORT_SYMBOL
  OdSmartPtr< OdApDocumentImpl > m_pRefDocument;
#endif // ODAMFC_EXPORT_SYMBOL

#ifdef ENABLE_VBA_SUPPORT
  LPSTORAGE& getStorage(){ return m_lpRootStg;}
#endif // ENABLE_VBA_SUPPORT
};
#ifdef ENABLE_VBA_SUPPORT
class Document : 
public IDispatchImpl<IOdaMfcDoc, &IID_IOdaMfcDoc, &LIBID_OdaMfcAppLIB>, 
public ISupportErrorInfo,
public CComObjectRoot,
public CComCoClass<Document,&CLSID_Document>,
public IProvideClassInfo2Impl<&CLSID_Document, &IID_IOdaMfcDoc, &LIBID_OdaMfcAppLIB>,
public IApcDualImpl<COdaMfcAppDoc>
{
public:
  Document() {}
  virtual ~Document() {}
  
  BEGIN_COM_MAP(Document)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IOdaMfcDoc)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
    COM_INTERFACE_ENTRY(IApcDual)
    END_COM_MAP()
    //DECLARE_NOT_AGGREGATABLE(Document) 
    // Remove the comment from the line above if you don't want your object to 
    // support aggregation. 
    
    DECLARE_REGISTRY(Document, _T("OdaMfc.Document.1"), _T("OdaMfc.Document"), IDS_DOCUMENT_DESC, THREADFLAGS_BOTH)
    // ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
  
  // IOdaMfcDoc
public:
  STDMETHOD(Save)();
  STDMETHOD(Activate)();
  STDMETHOD(Close)(/*[in, optional]*/ MpSaveOptions Option);
  STDMETHOD(get_Saved)(/*[out, retval]*/ VARIANT_BOOL *pVal);
  STDMETHOD(put_Saved)(/*[in]*/ VARIANT_BOOL newVal);
  STDMETHOD(get_Collection)(/*[out, retval]*/ IOdaMfcDocs* *pVal);
  STDMETHOD(get_Parent)(/*[out, retval]*/ IOdaMfcApp* *pVal);
  STDMETHOD(get_Application)(/*[out, retval]*/ IOdaMfcApp* *pVal);
  STDMETHOD(get_Database)(IAcadDatabase** pDatabase);
  STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
};
#endif // ENABLE_VBA_SUPPORT
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODAMFCAPPDOC_H__BFCBB1B3_DB28_48F5_B96E_35E4EE840454__INCLUDED_)
