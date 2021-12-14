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

#ifndef _AFX_ODADGNAPPDOC__INCLUDED_
#define _AFX_ODADGNAPPDOC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>
#include "DgDatabase.h"
#include "DgCommandContext.h"
#include "ExEdBaseIO.h"
#include "Ed/EdCommandStack.h"

#define STL_USING_SET
#include "OdaSTL.h"

class COdaDgnAppDoc;
class COdaDgnAppVectorizer;
class ExStringIO;
class CUserIOConsole;

class COdaDgnAppPrintSettings
{
public:

	enum OdDgPlotRotation
	{
		k0degrees = 0,   // No rotation
		k90degrees = 1,   // 90° CCW
		k180degrees = 2,   // Inverted
		k270degrees = 3    // 90° CW
	};

	enum OdDgPlotType
	{
		kView = 0,   // View
		kExtents = 1,   // Extents
		kSheetModel = 2,   // Sheet extents
		kFence = 3,   // Fence ( Not implemented yet )
	};

public:

	COdaDgnAppPrintSettings();

public:

	OdGsView::RenderMode m_uRenderMode;
	bool                 m_bPlotToBMP;
	OdString             m_strPrinterDriver;
	ODCOLORREF           m_clrBgColor;
	OdDgPlotRotation     m_uPlotRotation;
	OdDgPlotType         m_uPlotType;
	double               m_dPaperWidth;
	double               m_dPaperHeight;
	double               m_dLeftMargin;
	double               m_dRightMargin;
	double               m_dTopMargin;
	double               m_dBottomMargin;
	double               m_dPlotOffsetX;
	double               m_dPlotOffsetY;
	double               m_dScale;
	bool                 m_bCentered;
};

class OdDgDatabaseDoc : public OdDgDatabase
{
	static  COdaDgnAppDoc*  g_pDoc;
	mutable COdaDgnAppDoc*  m_pDoc;

	//using OdDgDatabase::operator new;
	//using OdDgDatabase::operator delete;
public:
	ODRX_DECLARE_MEMBERS(OdDgDatabaseDoc);

	OdDgDatabaseDoc();

	COdaDgnAppDoc* document() const;

	static void setDocToAssign(COdaDgnAppDoc* pDoc);
};

typedef OdSmartPtr<OdDgDatabaseDoc> OdDgDatabaseDocPtr;

class COdaDgnAppDocStaticRxObjects : //public OdDgLayoutManagerReactor, 
	public OdEdBaseIO
{
	ODRX_NO_HEAP_OPERATORS();
};


class COdaDgnAppDoc :
	public CDocument,
	protected OdStaticRxObject<COdaDgnAppDocStaticRxObjects>
{
	// Constructor/destructor
protected:
	// create from serialization only
	using CDocument::operator new;
	using CDocument::operator delete;
	void OnReToolbar(bool);
	COdaDgnAppDoc();
	DECLARE_DYNCREATE(COdaDgnAppDoc)

	COdaDgnAppVectorizer*       m_pVectorizer;
	OdDgCommandContextPtr       m_pCmdCtx;
	int                         m_nCmdActive;
	OdSmartPtr<CUserIOConsole>  m_pConsole;
	bool                        m_bConsole;
	bool                        m_bConsoleResponded;
	OdSmartPtr<ExStringIO>      m_pMacro;

	COdaDgnAppPrintSettings     m_printSettings;
	OdUInt16                    m_nActiveView;
	std::vector<OdDgElementId>  m_pViews;
	CToolBar*					  m_pToolBar;


	OdDgCommandContextPtr cmdCtx();
	OdEdBaseIO* cmdIO();
	OdString commandPrompt();
	OdString recentCmdName();
	CUserIOConsole* console();

	// OdEdBaseIO
	OdString getString(const OdString& prompt, int options, OdEdStringTracker* pTracker);
	void putString(const OdString& string);
	OdGePoint3d getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker);
	virtual OdUInt32 getKeyState();

	bool m_bDisableClearSel;

public:
	virtual ~COdaDgnAppDoc();

	// Attributes
public:
	OdDgDatabase* database() { return m_pDb; }
	OdDgElementId vectorizedViewId() { return m_vectorizedViewId; }
	OdDgElementId vectorizedModelId() { return m_vectorizedModelId; }

	// Operations
public:
	void OnCloseVectorizer(COdaDgnAppVectorizer* pVectorizer);
	void UpdateVectorizersColorPalette(const COLORREF* pColorPaletteRef);
	COdaDgnAppVectorizer* getVectorizer();
	void setVectorizer(COdaDgnAppVectorizer* pVectorizer);
	afx_msg void OnViewNumber(UINT nID);
	void OnSwitchView(const OdString& sVectorizerPath, UINT iElementNumber);
	void OnVectorize(const OdString& sVectorizerPath);
	OdDgSelectionSetPtr selectionSet() const;
	void ExecuteCommand(const OdString& sCmd, bool bEcho = true);
	void DeleteSelection();
	OdUInt16 getActiveView() { return m_nActiveView; }
	void setActiveView(OdUInt16 item) { m_nActiveView = item; }
	UINT getViewsCount() { return (UINT)(m_pViews.size()); }
	OdDgElementId getVeiwId(OdUInt16 item);
	//void SetViewsCount(UINT viewsCount) { m_ViewsCount = viewsCount; }
	void recountVeiws( const OdDgViewGroupPtr& pViewGroup );

  bool hideFence();
  void showFence();

	// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
  virtual BOOL CanCloseFrame(CFrameWnd* pFrame);

private:
	bool createCellLibraryPreview(OdDgDatabasePtr pDb, const OdString& strFileName, bool bModel2d, bool bSharedCell);
	void createCellTable2d(OdDgModelPtr& pModel, OdDgDatabasePtr pDbLib,
		OdArray<OdDgElementId> arrModelIds, bool bSharedCell, OdUInt32 uModelCount);
	void createCellTable3d(OdDgModelPtr& pModel, OdDgDatabasePtr pDbLib,
		OdArray<OdDgElementId> arrModelIds, bool bSharedCell, OdUInt32 uModelCount);


	// Generated message map functions
protected:
	//{{AFX_MSG(COdaMfcAppDoc)
	afx_msg void OnUpdateView1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateView2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateView3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateView4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateView5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateView6(CCmdUI* pCmdUI);
	afx_msg void OnUpdateView7(CCmdUI* pCmdUI);
	afx_msg void OnUpdateView8(CCmdUI* pCmdUI);
  afx_msg void OnRasterAttach();
	afx_msg void OnVectorize();
  afx_msg void OnFileCompress();
	afx_msg void OnExportStl();
	afx_msg void OnExportSvg();
	afx_msg void OnExportHoops();
  afx_msg void OnExportObj();
	afx_msg void OnExportPdf();
	afx_msg void OnUpdateVectorize(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchModel(CCmdUI*pCmdUI);
  afx_msg void OnSwitchModel();	
  afx_msg void OnUpdateItemTypeLibs(CCmdUI*pCmdUI);
  afx_msg void OnItemTypeLibs();
	afx_msg void OnViewSumminfo();
	afx_msg void OnTestsViewcamera();
	afx_msg void OnTestsCellLibraryPreview();
	afx_msg void OnTestsLoadSmartSolid();
  afx_msg void OnTestsLoadLineStyles();
  afx_msg void OnTestsMergeModels();
  afx_msg void OnTestsMergeXRef();
	afx_msg void OnEditRecentcommand();
	afx_msg void OnUpdateEditRecentcommand(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnBmpExport();
	afx_msg void OnUpdateBmpExport(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditSelectall();
	afx_msg void OnEditClearselection();
	afx_msg void OnEditSelectionReactor();
	afx_msg void OnFilePageSetup();
	afx_msg void OnUpdateEditSelectall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateClearselection(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelectionReactor(CCmdUI* pCmdUI);
  afx_msg void OnOptionsApplyGlobalOrigin();
  afx_msg void OnUpdateOptionsApplyGlobalOrigin(CCmdUI* pCmdUI);

	// Implementation
public:
	BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
	void OnToolsOptionsBgColor();
  void OnToolOptionsLineStyleScale();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	OdDgDatabasePtr                   m_pDb;
  OdDgElementId                     m_idFence;
	std::set< COdaDgnAppVectorizer* > m_pVectorizers;

	OdDgElementId m_vectorizedModelId;
	OdDgElementId m_vectorizedViewId;

	int m_SaveAsVer;
	int m_SaveAsType;
public:
	afx_msg void OnExportDwf();
	afx_msg void OnRenderLights();
	afx_msg void OnRenderMaterials();

	afx_msg void OnToolsLevelManager();
	
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
  static OdDgElementIdArray selectEntities(OdDgUserIO* pIO, const OdString& strCaption);
};

/********************************************************************************************/
/* Specialization of the OdEdCommand class to add a COPY_LINE_STYLES_FROM_RSC command       */
/********************************************************************************************/
struct lineStyleDefInfoForCopyCmd
{
	OdUInt32                                m_uEntryId;
	OdDgLineStyleResource::OdLsResourceType m_rsType;
	OdDgElementId                           m_idDef;
};

struct lineStyleElementPairForCopyCmd
{
	OdDgElementId                           m_idOldElement;
	OdDgElementId                           m_idElement;
};

struct lineStyleNamePairForCopyCmd
{
	OdString                                m_strLSName;
	OdDgElementId                           m_idElement;
};

class Cmd_COPY_LINE_STYLES_FROM_RSC : public OdEdCommand
{
public:

	static const OdString name();
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCommandContext);
	const OdRxModule* commandApp() const;
	void              commandUndef(bool undefIt);
	OdInt32           commandFlags() const;

private:

	OdDgElementId addRscDefinition(OdDgLineStyleDefTablePtr& pTable, const OdString& strName);
	OdDgElementId addRscDefinition(OdDgLineStyleDefTablePtr& pTable, OdUInt32 entryId, OdDgLineStyleResource::OdLsResourceType rsType);

private:

	OdArray<lineStyleDefInfoForCopyCmd>     m_arrLineStyleDef;
	OdArray<lineStyleElementPairForCopyCmd> m_arrCellDef;
	OdArray<lineStyleNamePairForCopyCmd>    m_arrNameDef;
};

/************************************************************************/
/* Specialization of the OdEdCommand class to add a ERASE command       */
/************************************************************************/
class Cmd_ERASE : public OdEdCommand
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
/* Specialization of the OdEdCommand class to add a Scale2 command       */
/************************************************************************/
class Cmd_Scale2 : public OdEdCommand
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
/* Specialization of the OdEdCommand class to add a RotateXY30 command       */
/************************************************************************/
class Cmd_RotateXY30 : public OdEdCommand
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
/* Specialization of the OdEdCommand class to add a ERASE command       */
/************************************************************************/
class Cmd_ERASE_Invisible : public OdEdCommand
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
/* Specialization of the OdEdCommand class to add a CraeteDefaultInst command       */
/************************************************************************/
class Cmd_CraeteDefaultInst : public OdEdCommand
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

/***************************************************************************************/
/* Specialization of the OdEdCommand class to add a ConvertToExtGraphics command       */
/***************************************************************************************/
class Cmd_ConvertToExtGraphics : public OdEdCommand
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

/***************************************************************************************/
/* Specialization of the OdEdCommand class to add a ConvertToMesh command              */
/***************************************************************************************/
class Cmd_ConvertToMesh : public OdEdCommand
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

/***************************************************************************************/
/* Specialization of the OdEdCommand class to add a ConvertToMesh command              */
/***************************************************************************************/
class Cmd_SetGlobalOrigin : public OdEdCommand
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
/* Specialization of the OdEdCommand class to add a UNDO command       */
/************************************************************************/
class Cmd_UNDO : public OdEdCommand
{
public:
	Cmd_UNDO()
	{
//		SETBIT_1(flags(), OdEdCommand::kNoUndoMarker);
	}
	static const OdString name();
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCommandContext);
	virtual OdInt32 flags() const
	{
		return OdEdCommand::flags() | OdEdCommand::kNoUndoMarker;
	}
};

/************************************************************************/
/* Specialization of the OdEdCommand class to add a LINES command       */
/************************************************************************/
class Cmd_LINE : public OdEdCommand
{
public:
	static const OdString name();
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCommandContext);
};

/************************************************************************/
/* Specialization of the OdEdCommand class to add a ARC command       */
/************************************************************************/
class Cmd_ARC : public OdEdCommand
{
public:
	static const OdString name();
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCommandContext);
};

/************************************************************************/
/* Specialization of the OdEdCommand class to add a CIRCLE command       */
/************************************************************************/
class Cmd_CIRCLE : public OdEdCommand
{
public:
	static const OdString name();
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCommandContext);
};

/************************************************************************/
/* Specialization of the OdEdCommand class to add a TESTUNDOPL command       */
/************************************************************************/
class Cmd_TESTUNDOPL : public OdEdCommand
{
public:
	static const OdString name();
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCommandContext);
};

/************************************************************************/
/* Specialization of the OdEdCommand class to add a CONE command       */
/************************************************************************/
class Cmd_CONE : public OdEdCommand
{
public:
	static const OdString name();
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCommandContext);
};

/************************************************************************/
/* Specialization of the OdEdCommand class to add a TEXT command       */
/************************************************************************/
class Cmd_TEXT : public OdEdCommand
{
public:
	static const OdString name();
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCommandContext);
};

/*************************************************************************************/
/* Specialization of the OdEdCommand class to add a CREATE_VIEW_GROUPS command       */
/*************************************************************************************/
class Cmd_CREATE_VIEW_GROUPS : public OdEdCommand
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

/************************************************************************************/
/* Specialization of the OdEdCommand class to add a ERASE_ALL_BY_TYPE command       */
/************************************************************************************/
class Cmd_ERASE_ALL_BY_TYPE : public OdEdCommand
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


/************************************************************************************/
/* Specialization of the OdEdCommand class to add a ISOLATE command                 */
/************************************************************************************/
class Cmd_ISOLATE : public OdEdCommand
{
public:
	static const OdString name();
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCommandContext);
	const OdRxModule* commandApp() const;
	void              commandUndef(bool undefIt);
	OdInt32           commandFlags() const;

private:
	void addElementIdToSet(std::set<OdDgElementId>& setSelected, const OdDgElementId& idElement);
	void addComplexIdToSet(std::set<OdDgElementId>& setSelected, OdDgElementIteratorPtr& pIter);
	void extendSelectionArray(const OdDgElementIdArray& arrSelected, std::set<OdDgElementId>& arrFullSet);
	OdDgElementId      getTopContainerId(const OdDgElementPtr& pElm);
	OdDgElementIteratorPtr getElementIterator(const OdDgElementPtr& pElm);
	void isolateElements(OdDgElementIteratorPtr& pIter, const std::set<OdDgElementId>& setSelected);
	bool isElementSelected(const OdDgElementId& idElement, const std::set<OdDgElementId>& setSelected);
};

/********************************************************************************************/
/* Specialization of the OdEdCommand class to add a ERASE_UNUSED_SHARED_CELLS command       */
/********************************************************************************************/
class Cmd_ERASE_UNUSED_SHARED_CELLS : public OdEdCommand
{
public:
	static const OdString name();
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCommandContext);
	const OdRxModule* commandApp() const;
	void              commandUndef(bool undefIt);
	OdInt32           commandFlags() const;

private:
	void markSharedCells(OdDgElementIteratorPtr pElmIter, OdArray<OdDgElementId> arrCellIds,
		OdArray<bool>& arrCellUsage, OdDgDatabasePtr pDb);
	void markNestedSharedCells(OdArray<OdDgElementId> arrCellIds, OdArray<bool>& arrCellUsage, OdDgDatabasePtr pDb);
};

class Cmd_Fence : public OdEdCommand
{
public:
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCmdCtx);
};


/************************************************************************/
/* Specialization of the OdEdCommand class to add a ZOOM command       */
/************************************************************************/
class Cmd_ZOOM : public OdEdCommand
{
public:
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCmdCtx);
};

class OdDgEx3dOrbitCmd : public OdEdCommand
{
public:
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCmdCtx);
};

class OdDgExDollyCmd : public OdEdCommand
{
public:
	const OdString groupName() const;
	const OdString globalName() const;
	void execute(OdEdCommandContext* pCmdCtx);
};

struct Cmd_REDUCE : OdEdCommand
{
	virtual const OdString groupName() const ODRX_OVERRIDE { return L"ODDB_COMMANDS"; }
	virtual const OdString globalName() const ODRX_OVERRIDE { return L"REDUCE"; }
	virtual void execute(OdEdCommandContext* pCommandContext) ODRX_OVERRIDE;
};


#endif // _AFX_ODADGNAPPDOC__INCLUDED_
