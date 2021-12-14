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
#include "StaticRxObject.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "DynamicLinker.h"
#include "diagnostics.h"
#include "RxDynamicModule.h"

#include "DbDatabase.h"
#include "DbSecurity.h"

#include "DbBlockTableRecord.h"
#include "DbLine.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

#define MAX_PATH_LENGTH 1024

//////////////////////////////////////////////////////////////////////////

class MyServices : public ExSystemServices, public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);
};

//////////////////////////////////////////////////////////////////////////

class ExampleApp
{
public:
  void InitInstance(MyServices* pSvcs);

  void FileOpen(const OdString& filePath);
  void CreateTempAndOpen();

  void IncSave();
  void IncSaveToTemp();
    
  void setISavePercent(int newValue);

  void AddLines1();
  void AddLines2();
  void AddLines3();
  void AddLines4();
  void AddLines5();
  
  MyServices* m_pServices;
  OdDbDatabasePtr m_pDb;
  
  OdString m_filePath;//original file path

  OdStreamBufPtr m_pTempFile;//temp file stream
};

void ExampleApp::InitInstance(MyServices* pSvcs)
{
  m_pServices = pSvcs;
}

void ExampleApp::setISavePercent(int newValue)
{
  if (!m_pDb.isNull())
  {    
    m_pDb->appServices()->setISAVEPERCENT(newValue);
  }  
}

void ExampleApp::AddLines1()
{
  if (!m_pDb.isNull())
  {
    OdDbBlockTableRecordPtr pModelSpace = m_pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    OdGePoint3d point;
    OdGeVector3d toStart(1000.0, 0.0, 0.0);
    for (int i = 0; i < 12; i++)
    {
      OdDbLinePtr pLine = OdDbLine::createObject();
      pLine->setDatabaseDefaults(pModelSpace->database());
      pModelSpace->appendOdDbEntity(pLine);
      pLine->setStartPoint(point + toStart);
      pLine->setEndPoint(point + toStart.rotateBy(OdaToRadian(160.0), OdGeVector3d::kZAxis));
    }
  }
}

void ExampleApp::AddLines2()
{
  if (!m_pDb.isNull())
  {
    OdDbBlockTableRecordPtr pModelSpace = m_pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    OdGePoint3d point;
    OdGeVector3d toStart(2000.0, 0.0, 0.0);
    for (int i = 0; i < 12; i++)
    {
      OdDbLinePtr pLine = OdDbLine::createObject();
      pLine->setDatabaseDefaults(pModelSpace->database());
      pModelSpace->appendOdDbEntity(pLine);
      pLine->setStartPoint(point + toStart);
      pLine->setEndPoint(point + toStart.rotateBy(OdaToRadian(160.0), OdGeVector3d::kZAxis));
    }
  }
}

void ExampleApp::AddLines3()
{
  if (!m_pDb.isNull())
  {
    OdDbBlockTableRecordPtr pModelSpace = m_pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    OdGePoint3d point;
    OdGeVector3d toStart(3000.0, 0.0, 0.0);
    for (int i = 0; i < 12; i++)
    {
      OdDbLinePtr pLine = OdDbLine::createObject();
      pLine->setDatabaseDefaults(pModelSpace->database());
      pModelSpace->appendOdDbEntity(pLine);
      pLine->setStartPoint(point + toStart);
      pLine->setEndPoint(point + toStart.rotateBy(OdaToRadian(160.0), OdGeVector3d::kZAxis));
    }
  }
}

void ExampleApp::AddLines4()
{
  if (!m_pDb.isNull())
  {
    OdDbBlockTableRecordPtr pModelSpace = m_pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    OdGePoint3d point;
    OdGeVector3d toStart(4000.0, 0.0, 0.0);
    for (int i = 0; i < 12; i++)
    {
      OdDbLinePtr pLine = OdDbLine::createObject();
      pLine->setDatabaseDefaults(pModelSpace->database());
      pModelSpace->appendOdDbEntity(pLine);
      pLine->setStartPoint(point + toStart);
      pLine->setEndPoint(point + toStart.rotateBy(OdaToRadian(160.0), OdGeVector3d::kZAxis));
    }
  }
}

void ExampleApp::AddLines5()
{
  if (!m_pDb.isNull())
  {
    OdDbBlockTableRecordPtr pModelSpace = m_pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    OdGePoint3d point;
    OdGeVector3d toStart(5000.0, 5000.0, 0.0);
    for (int i = 0; i < 12; i++)
    {
      OdDbLinePtr pLine = OdDbLine::createObject();
      pLine->setDatabaseDefaults(pModelSpace->database());
      pModelSpace->appendOdDbEntity(pLine);
      pLine->setStartPoint(point + toStart);
      pLine->setEndPoint(point + toStart.rotateBy(OdaToRadian(160.0), OdGeVector3d::kZAxis));
    }
  }
}

void CopyOneStreamToAnother(const OdString& srcPath, const OdString& targetPath)
{
#ifdef _MSC_VER
  OdString cmdString("copy \"" + srcPath + "\" \"" + targetPath + "\"");
#else
  OdString cmdString("cp \"" + srcPath + "\" \"" + targetPath + "\"");
#endif
  system(cmdString);
}

void ExampleApp::FileOpen(const OdString& filePath)
{
  m_pDb = m_pServices->readFile(filePath);
  m_filePath = filePath;
}

void ExampleApp::CreateTempAndOpen()
{
  if (!m_filePath.isEmpty())
  {
    OdString tempPath = m_filePath + OD_T(".tmp");

    CopyOneStreamToAnother(m_filePath, tempPath);

    m_pTempFile = odSystemServices()->createFile(tempPath, 
      (Oda::FileAccessMode)(Oda::kFileRead|Oda::kFileWrite), 
      Oda::kShareDenyReadWrite, Oda::kOpenAlways);
  }
}

void ExampleApp::IncSave()
{
  m_pDb->setFilename(m_filePath);
  m_pDb->save(m_filePath);  
}

void ExampleApp::IncSaveToTemp()
{
  if (!m_pTempFile.isNull())
  {
    m_pDb->save(m_pTempFile);
  }
}

//////////////////////////////////////////////////////////////////////////

#ifndef _TOOLKIT_IN_DLL_
    ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DbCryptModule);
    ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);

    ODRX_BEGIN_STATIC_MODULE_MAP()
      ODRX_DEFINE_STATIC_APPMODULE(DbCryptModuleName,            DbCryptModule)
      ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
    ODRX_END_STATIC_MODULE_MAP()
#endif

static const char cUsage[] =
{
  "IncrementalSaveEx sample program. Copyright (C) " TD_COPYRIGHT_START_YEAR_S  TD_COPYRIGHT_END_S "\n"
  "Usage: IncrementalSaveEx filepath\n"
  "  filepath - full path to .dwg file.\n"
  "\nPress ENTER to continue...\n"
};

//////////////////////////////////////////////////////////////////////////

#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  if (argc < 2) 
  {
    STD(cout) << cUsage;
    STD(cin).get();
    return 1;
  }

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  OdStaticRxObject<MyServices> svcs;

  /**********************************************************************/
  /* Initialize Runtime Extension environment                           */
  /**********************************************************************/
  ::odInitialize(&svcs);
  ::odrxDynamicLinker()->loadModule(DbCryptModuleName);
  
  /**********************************************************************/
  /* File processing example                                            */
  /**********************************************************************/
  {
    ExampleApp theApp;
    theApp.InitInstance(&svcs);
    theApp.setISavePercent(100);

    OdString filePath(argv[1]);
    
    theApp.FileOpen(filePath);
    theApp.CreateTempAndOpen();

    theApp.AddLines1();
    theApp.IncSave();

    theApp.AddLines2();
    theApp.IncSave();

    theApp.AddLines3();
    theApp.IncSaveToTemp();//TO TEMP

    theApp.AddLines4();
    theApp.IncSaveToTemp();//TO TEMP

    theApp.AddLines5();
    theApp.IncSave();
  } 

  /**********************************************************************/
  /* Uninitialize Runtime Extension environment                         */
  /**********************************************************************/
  ::odUninitialize();

	return 0;
}
