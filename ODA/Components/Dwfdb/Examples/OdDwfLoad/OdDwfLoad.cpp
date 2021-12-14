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

#include "OdaCommon.h"
#include "DwfHostAppServices.h"
#include "StaticRxObject.h"
#include "DwfDatabase.h"
#include "DwfObjectImpl.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "ExSystemServices.h"
#include "Gs/GsBaseInclude.h"
#include "diagnostics.h"

#include "dwfcore/String.h"
#include "dwfcore/paging/MemoryManager.h"

#include "ExPrintConsole.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
//#define  STD(a)  std:: a

/************************************************************************/
/* Define a module map for statically linked modules                    */
/************************************************************************/
#if !defined(_TOOLKIT_IN_DLL_)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDbRootModuleObject);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DwfDbModule);
ODRX_BEGIN_STATIC_MODULE_MAP()
ODRX_DEFINE_STATIC_APPMODULE(OdDbRootModuleName, OdDbRootModuleObject)
ODRX_DEFINE_STATIC_APPMODULE(OdDwfDbModuleName, DwfDbModule)
ODRX_END_STATIC_MODULE_MAP() 
#endif

class OdExDwfSystemServices : public ExSystemServices
{
public:
	OdExDwfSystemServices() {}
};

class MyDwfServices : public OdExDwfSystemServices, public OdDwfHostAppServices
{
protected:
	ODRX_USING_HEAP_OPERATORS(OdExDwfSystemServices);
};

#if 0
static class MemLeakCatcher
{
public:
  MemLeakCatcher()
  {
    //_CrtSetBreakAlloc(354);
    // marker to see mem leaks
    char* pLeakMarker = (char *) odrxAlloc(100);
    memset(pLeakMarker, 0, 100);
    strcpy(pLeakMarker, "Marker_Start");

    long allocReqNum;
    _CrtIsMemoryBlock(pLeakMarker, 100, &allocReqNum, NULL, NULL);
    
    //return;
    //long num = allocReqNum + 1;
    ////test for check memleak catcher
    //_CrtSetBreakAlloc(num); // num in {} in dump of _CrtDumpMemoryLeaks() (num < 453(by MarkerStart) are useless to check)

    //pLeakMarker = (char *)odrxAlloc(200);
    //memset(pLeakMarker, 0, 200);
    //strcpy(pLeakMarker, "Marker_1");
  }
  ~MemLeakCatcher()
  {
    // unrecommnded (uses to remove Dwf Toolkit internal memory leaks at end of application only)
    DWFCore::DWFStringTable::Purge();
    //
    _CrtDumpMemoryLeaks();
  }
} s_catcher;
#endif

static void DumpObjectProps(OdDwfObjPropsPtr pProps, const OdString& csIndent = OdString::kEmpty)
{
  if (pProps.isNull())
    return;
  OdString sIndent = csIndent;
  if (!pProps->id().isEmpty()) 
    odPrintConsoleString(L"%ls    (id=%ls)\r\n", sIndent.c_str(), pProps->id().c_str());
  OdStringArray cats = pProps->categories();
  if (cats.isEmpty())
    cats.push_back(L"");
  for (OdUInt32 idx = 0; idx < cats.size(); idx++)
  {
    sIndent = csIndent;
    OdString sCat = cats[idx];
    odPrintConsoleString(L"%ls    [%ls]:\r\n", sIndent.c_str(), sCat.c_str());
    sIndent += L"  ";

    OdStringArray ks = pProps->keys(sCat);
    for (OdUInt32 idxKs = 0; idxKs < ks.size(); idxKs++)
    {
      OdString key = ks[idxKs];
      odPrintConsoleString(L"%ls    %ls=%ls\r\n", sIndent.c_str(), 
                           pProps->propertyName(key).c_str(), pProps->property(key).c_str());
    }
  }
}

static bool DumpObjectTree(OdDwfViewPtr pView, OdDwfObjectId id, const OdString& sIndent = OdString::kEmpty)
{
  OdDwfObjectPtr pObj = id.openObject();
  if (pObj.isNull())
    return false;

  int nodeId = 0;
  OdDwfSegment* pSeg = OdDwfSegment::cast(pObj).get();
  OdDwfEntity* pEnt = NULL;
  if (pSeg)
    nodeId = pSeg->tagIndex();
  else if ((pEnt = OdDwfEntity::cast(pObj).get()) != NULL)
    nodeId = pEnt->tagIndex();
  OdDwfObjPropsPtr pProps;
  OdString sLabel;
  if (nodeId > 0)
  {
    sLabel = pView->label(nodeId);
    pProps = pView->props(nodeId);

    if (pEnt && OdDwfSegmentReference::cast(pEnt).get()) // useless node in dump tree of rac_basic_sample_project.dwf.
      return false;
  }
  if (!sLabel.isEmpty())
    odPrintConsoleString(L"%ls+ %ls:\r\n", sIndent.c_str(), sLabel.c_str());
  if (pSeg)
  {
    OdString sName = pSeg->getName(),
             sBy = (pSeg->getMatrix() != OdGeMatrix3d::kIdentity) ? L"by_xform_seg" : L"by_seg";
    if (!sName.isEmpty())
      odPrintConsoleString(L"%ls  %ls{%ls}:\r\n", sIndent.c_str(), sBy.c_str(), sName.c_str());
  }
  if (pProps.get())
    DumpObjectProps(pProps, sIndent);

  // for kids
  if (OdDwfView::cast(pObj).get())
    for (OdDwfObjectIteratorPtr pItr = pView->objectIterator(); !pItr->done(); pItr->step())
      DumpObjectTree(pView, pItr->item(), sIndent + L"  ");
  else if (pSeg)
    for (OdDwfObjectIteratorPtr pItr = pSeg->objectIterator(); !pItr->done(); pItr->step())
      if (!DumpObjectTree(pView, pItr->item(), sIndent + L"  "))
        break;
  return true;
}

static int LoadTeighaDWFx(const OdString& szSource, bool bLoadObjectProperties = false)
{
  int nRes = 0;
  odPrintConsoleString(L"\r\nOdDwfLoad sample program\r\n");

  /********************************************************************/
  /* Create a custom Services instance.                               */
  /********************************************************************/
  OdStaticRxObject<MyDwfServices> svcs;

#if !defined(_TOOLKIT_IN_DLL_) 
  ODRX_INIT_STATIC_MODULE_MAP();
#endif
  /**********************************************************************/
  /* Initialize ODA Drawings SDK                                        */
  /**********************************************************************/
  odrxInitialize(&svcs);
  odgsInitialize();
  //::odrxDynamicLinker()->loadModule(OdDbRootModuleName, false); // workaround
  ::odrxDynamicLinker()->loadModule(OdDwfDbModuleName, false);
  /**********************************************************************/

  try {
    svcs.enable3dObjectPropsLoading(bLoadObjectProperties); // Initially is disabled to improve performance

    OdDwfDatabasePtr pDb = svcs.readFile(szSource);

    if (!pDb.isNull()) {

      OdDwfObjectId objId = pDb->currentLayoutId();
      nRes++;

      odPrintConsoleString(L"Got layout id: %ls\r\n", objId.getHandle().ascii().c_str());

      OdDwfViewPtr pView = OdDwfView::cast(objId.openObject());

      // obsolete but working way
      //OdDwfModelViewPtr pModelView = OdDwfModelView::cast(pView);
      //if (pModelView.get())
      //{
      //  odPrintConsoleString(L"Dump model view properties:\r\n");
      //  OdStringArray keys = pModelView->propertyKeys();
      //  for (int idx = 0, sz = (int) keys.size(); idx < sz; idx++)
      //    odPrintConsoleString(L"  prop: name = \"%ls\", value = \"%ls\"\r\n", 
      //                         pModelView->propertyName(keys[idx]).c_str(),
      //                         pModelView->property(keys[idx]).c_str());
      //}
      
      if (pView.get())
      {
        OdDwfObjPropsPtr pProps = pView->props();
        if (pProps.get())
        {
          odPrintConsoleString(L"Dump model view properties:\r\n");
          DumpObjectProps(pProps, L"  ");
          odPrintConsoleString(L"\r\n");
        }

        pView->checkLoad();

        if (pView->hasPropData())
        {
          OdString sRootLabel = pView->label(0);
          if (!sRootLabel.isEmpty())
            odPrintConsoleString(L"  + %ls:\r\n", sRootLabel.c_str());

          DumpObjectTree(pView, pView->objectId());
        }
      }
    }
  }
  catch (const OdError& e) {
    odPrintConsoleString(L"\r\nException Caught: %ls\r\n", e.description().c_str());
  }
  catch (...)
  {
    odPrintConsoleString(L"\r\nUnknown Exception Caught\r\n");
  }

  // unrecommnded (uses to remove Dwf Toolkit internal memory leaks at end of application only)
  DWFCore::DWFStringTable::Purge();
  //_CrtDumpMemoryLeaks();

  /**********************************************************************/
  /* Uninitialize Initialize ODA Drawings SDK                           */
  /**********************************************************************/

  ::odrxDynamicLinker()->unloadUnreferenced();
  odgsUninitialize();
  ::odrxUninitialize();

  return nRes;
}

////////////////////////////////////////////////////////////////////////////////////

static void MyAssert(const char* expr, const char* fileName, int nLine)
{
  OdString sMsg, sExpr(expr), sFileName(fileName);
  sMsg.format(L"assertion expression: \"%ls\"\r\nfile: \"%ls\"\r\nline: %d", sExpr.c_str(), sFileName.c_str(), nLine);
  odPrintConsoleString(sMsg.c_str());
}

static bool MyCheckAssertGroup(const char* pGroup)
{
  return !pGroup || !*pGroup; // asserts of base group are enabled only (default)
                              //return true; // asserts of all groups are enabled
}

#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  /**********************************************************************/
  /* Set customized assert function                                     */
  /**********************************************************************/
  odSetAssertFunc(MyAssert);
  odSetCheckAssertGroupFunc(MyCheckAssertGroup);

  /**********************************************************************/
  /* Parse Command Line strings                                         */
  /**********************************************************************/

  int nRes = 0;
  if (argc >= 2)
  {
    bool bLoadObjectProperties = false;
    for (int idx = 1; idx < argc; idx++)
    {
      OdString sFile(argv[idx]);
      if (sFile == L"-d" || sFile == L"-D" || sFile == L"/d" || sFile == L"/D")
      {
        bLoadObjectProperties = true;
        continue;
      }

      if ((nRes = LoadTeighaDWFx(sFile, bLoadObjectProperties)) != 0)
        break;
    }
  }
  else
    odPrintConsoleString(L"Use as: OdDwfLoad [-d] file_path.dwf\r\n"
                         L"         -d  means dump of 3d Object Properties\r\n");

  //odPrintConsoleString(L"\r\n\r\nPress ENTER to continue...\r\n");
  //std::cin.get(); // system("pause");

  return nRes;
}
