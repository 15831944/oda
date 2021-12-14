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
//
// convertor.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#include <QMessageBox>
#else
#include <QtWidgets>
#endif
#include <QDir>

#include "convertor.h" 
#include "DbAudit.h"
#include "OdFileBuf.h"
#include "diagnostics.h"
#include "RxDynamicModule.h"

#if defined(Q_OS_LINUX)
 #include <QProcess>
 #include <signal.h>
#endif

//////////////////////////////////////////////////////////////////////////

OdString toOdString(const QString& qsStr)
{
  std::wstring wstr(qsStr.toStdWString());
  OdString sText(wstr.c_str());
  return sText;
}

QString toQString(const OdString& sStr)
{
  std::wstring wstr((wchar_t*)sStr.c_str());
  QString qsStr(QString::fromStdWString(wstr));
  return qsStr;
}

static void saveTextToFile(const QString& qsText, const QString& qsFilePathName)
{
  ODA_ASSERT_ONCE(!qsText.isEmpty() && !qsFilePathName.isEmpty());

  QFile file(qsFilePathName);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    ODA_FAIL_ONCE();
    return;
  }
  QTextStream out(&file);
  out << qsText << endl;
}

//////////////////////////////////////////////////////////////////////////
//
// Unfortunately Q_ASSERT is inconvenient to debugging
// (does the abort the application - see at end of function qt_message_output)
//

#if defined(Q_CC_MSVC) && defined(_CRT_ERROR)


static void myAssert(const char* pExpression, const char* pFileName, int nLineNo)
{
#ifdef _DEBUG 
  int iReport = _CrtDbgReport(_CRT_ASSERT, //_CRT_ERROR
                              pFileName, nLineNo, 
                              NULL, "%s\nType: %s", pExpression,
                              !qstricmp(pExpression, "Invalid Execution.")
                                  ? "ODA_FAIL" : "ODA_ASSERT");
  if (iReport == 1)
    _CrtDbgBreak(); // _asm { int 3 }
#endif
}

#else // elif defined(Q_OS_MAC) // defined(__GNUC__) && defined(__APPLE__)

inline int td_dbg_report(const char *title, const char *msg, ...)
{
  static const int QT_BUFFER_LENGTH = 8192; // internal buffer length
  char buf[QT_BUFFER_LENGTH];
  buf[QT_BUFFER_LENGTH - 1] = '\0';
  va_list ap;
  va_start(ap, msg); // use variable arg list
  qvsnprintf(buf, QT_BUFFER_LENGTH - 1, msg, ap);
  va_end(ap);

  if (QMessageBox::Retry == QMessageBox::question(
          QApplication::activeWindow(), // qApp->activeWindow()
          QString(title), // caption
          QString(buf), // text
          QMessageBox::Retry | QMessageBox::Ignore,
          QMessageBox::Ignore))
  {
    return 1;
  }
  return 0;
}

#include <signal.h>

static void myAssert(const char* pExpression, const char* pFileName, int nLineNo)
{
  int iReport = td_dbg_report("Assertion failed!",
                              "Expression: \"%s\"\nFile: \"%s\"\nLine: %d",
                              pExpression, pFileName, nLineNo);
  if (iReport == 1)
    ::raise(SIGTRAP);
}

#endif

static bool myCheckAssertGroup(const char* pGroup)
{
  return !pGroup || !*pGroup; // asserts of base group are enabled only (default)
  //return true; // asserts of all groups are enabled
}

static void myGeError(OdResult res)
{
  ODA_FAIL_ONCE(); // TODO
  //OdString message;
  //message.format(L"\n!!! Ge error: \"%s\"\n", OdError(res).description().c_str());
  //odPrintConsoleString(message);
}

//////////////////////////////////////////////////////////////////////////

// Define an AuditInfo class that prints no errors

class dwgAudit : public OdDbAuditInfo
{
  void printError(const OdString& /*strName*/,
                  const OdString& /*strValue*/,
                  const OdString& /*strValidation*/,
                  const OdString& /*strDefaultValue*/)
  {
    //ODA_FAIL_ONCE(); // TODO
  }
};

#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExFieldEvaluatorModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,   ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdExFieldEvaluatorModuleName, ExFieldEvaluatorModule)
ODRX_END_STATIC_MODULE_MAP()

#endif

CConvertor::CConvertor()
  : m_bInitialized(false)
  , m_bBusy(true)
  , m_bStopRequest(false)
  , m_bSilent(false)
  , m_bAudit(false)
  , m_typeSaveAs(OdDb::kDwg)
  , m_verSaveAs(OdDb::vAC24)
{
}

CConvertor::~CConvertor() 
{
  uninit();
}

void CConvertor::init()
{
  ::odSetAssertFunc(myAssert);
  ::odSetCheckAssertGroupFunc(myCheckAssertGroup);
  OdGeContext::gErrorFunc = myGeError;

  ODA_ASSERT(m_bBusy && !m_bInitialized);

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif
  // Initialize the Drawings SDK

  ::odInitialize(this);

  // preload all modules with "ObjectDBX Classes"
  //::odrxDynamicLinker()->loadModule(OdDynBlocksModuleName);
  //::odrxDynamicLinker()->loadModule(DbConstraintsModuleName);

  m_bInitialized = true;
  m_bBusy = false;
}

void CConvertor::uninit()
{
  if (m_bInitialized)
  {
    m_bInitialized = false;

    // Uninitialize the Drawings SDK
    ::odUninitialize();

#if defined(Q_OS_LINUX) && !defined(QT_DEBUG)
    // for ubuntu 10.10 
    // TODO debug the reason of segmentation fault during this destructor
    // QApplication via libfontconfig.so.1 (debug Qt version is needed)
    // Suspicion to confluct of our libTD_FT.a (old vers of freetype) with 
    // standard freetype6 (v >=2.4 - used by Qt)
    //
    bool isUbuntu = false;
    {
      QProcess proc;
      proc.start("uname -a");
      proc.waitForFinished();
      QString qsOut = QString::fromLocal8Bit(proc.readAllStandardOutput());
      if (qsOut.toLower().indexOf("ubuntu") >= 0)
        isUbuntu = true;
    }
    if (isUbuntu)
      ::raise(SIGQUIT);
#endif
  }
}

OdRxModule* CConvertor::loadModule(const OdString & sModuleFileName, bool bSilent)
{
  OdString sModulePathName(sModuleFileName);

  static QString qsAppDrxPath;
  if (qsAppDrxPath.isEmpty())
  {
    qsAppDrxPath = QDir(qApp->applicationDirPath()).absolutePath();

   #if defined(__GNUC__) && defined(__APPLE__) && !defined(__IPHONE_OS_VERSION_MIN_REQUIRED)// GNU compiler MacOS X

    // redirect to destination folder (if application is executed under xCode)
    QString qsXCodeRedirectDebug = "/Examples/Qt/build/Debug",
            qsXCodeRedirectRelease = "/Examples/Qt/build/Release";
    int pos = qsAppDrxPath.indexOf(qsXCodeRedirectDebug);
    if (pos >= 0)
    {
      qsAppDrxPath.replace(qsXCodeRedirectDebug, "");
    }
    else if ((pos = qsAppDrxPath.indexOf(qsXCodeRedirectRelease)) >= 0)
    {
      qsAppDrxPath.replace(qsXCodeRedirectRelease, "");
    }

    QDir dirAppDrx(qsAppDrxPath);
    if (dirAppDrx.dirName() == "MacOS") 
    {
      qsAppDrxPath.clear();
      dirAppDrx.cdUp();
      ODA_ASSERT_ONCE(dirAppDrx.dirName() == "Contents");
      // find possible sub-folder in Contents folder (DEF_MODULE_EXTENSION is useless here)
      foreach (QString qsDrx, QStringList() << "tx" << "Tx" << "TX" << "drx" << "Drx" << "DRX")
      {
        if (dirAppDrx.exists(qsDrx))
        {
          dirAppDrx.cd(qsDrx);
          qsAppDrxPath = dirAppDrx.absolutePath();
          break;
        }
      }

      if (qsAppDrxPath.isEmpty())
      {
        dirAppDrx.cdUp();
        dirAppDrx.cdUp();
        // at OdaFileConverter.app folder
        qsAppDrxPath = dirAppDrx.absolutePath();
      }

      //QMessageBox::question(
      //  QApplication::activeWindow(), 
      //  QObject::tr("ODA File Converter"), // caption
      //  QObject::tr("qsAppDrxPath = \"%1\"").arg(qsAppDrxPath), // text
      //  QMessageBox::Yes,
      //  QMessageBox::Yes);
    }
    else
      ODA_FAIL_ONCE();

   #endif
  }

  QString qsModulePathName = toQString(sModuleFileName);
 #if defined(_TOOLKIT_IN_DLL_) // !defined(ANDROID)
  if (   !qsAppDrxPath.isEmpty() 
      && qsModulePathName.indexOf("/") < 0
      && qsModulePathName.indexOf("\\") < 0)
  {
    QDir dir(qsAppDrxPath);
    ODA_ASSERT_ONCE(dir.exists());

    qsModulePathName = dir.absoluteFilePath(qsModulePathName);
    sModulePathName = toOdString(qsModulePathName);
  }
 #endif

  OdRxModule *pModule = OdDbSystemServices::loadModule(sModulePathName, bSilent);
  return pModule;
}

void CConvertor::checkProgress(int stepProgress, // = 0
                               int stepSpasticProgress) // = 0
{
  int val;
  if (stepProgress && !m_pProgressBar.isNull())
  {
    val = m_pProgressBar->value() + stepProgress;
    if (val > m_pProgressBar->maximum())
      val = m_pProgressBar->maximum();
    m_pProgressBar->setValue(val);
  }
  if (stepSpasticProgress && !m_pSpasticProgressBar.isNull())
  {
    val = m_pSpasticProgressBar->value() + stepSpasticProgress;
    if (val > m_pSpasticProgressBar->maximum())
      val = m_pSpasticProgressBar->maximum();
    m_pSpasticProgressBar->setValue(val);
  }

  qApp->processEvents(); // show
}

void  CConvertor::checkStop(int stepProgress, // = 0
                            int stepSpasticProgress) // = 0
{
  checkProgress(stepProgress, stepSpasticProgress);

  if (m_bStopRequest)
  {
    m_bStopRequest = false;
    if (   m_bSilent
        || QMessageBox::Yes == QMessageBox::question(
                  QApplication::activeWindow(), 
                  QObject::tr("ODA File Converter"), // caption
                  QObject::tr("Are you sure you want to terminate conversion?"), // text
                  QMessageBox::Yes | QMessageBox::No,
                  QMessageBox::Yes))
    {
      throw OdError(eUserBreak);
    }
  }
}

// Returns true if and only if a conversion is in progress
bool CConvertor::isBusy()
{
  return m_bBusy;
}

// create output dir as side effect
static bool collectPaths(const QDir& dirInput,
                         const QStringList& filters,
                         bool bRecurseFolders,
                         const QDir& dirOutput,
                         const QString& qsOutputExtension, // ".dwg" ...
                         QStringList& pathsInput, // out
                         QStringList& pathsOutput) // out
{
  QFileInfoList infsFiles; 
  if (bRecurseFolders)
  {
    infsFiles = dirInput.entryInfoList(  QDir::Dirs  | QDir::Readable
                                       | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    foreach (QFileInfo info, infsFiles)
    {
      ODA_ASSERT_ONCE(info.isDir());

      QString qsFolder = info.fileName();
      QDir dirInputSub(dirInput);
      if (!dirInputSub.cd(qsFolder))
      {
        ODA_FAIL_ONCE();
        continue;
      }
      if (dirInputSub == dirOutput) // #9210
        continue; // skip output folder (maybe a subfolder of input)

      QDir dirOutputSub(dirOutput);
      if (!dirOutputSub.cd(qsFolder))
      {
        if (!dirOutput.mkdir(qsFolder))
        {
          ODA_FAIL_ONCE(); // folder is inaccesible to write 
          continue;
        }

        dirOutputSub = QDir(dirOutput);
        if (!dirOutputSub.cd(qsFolder))
        {
          ODA_FAIL_ONCE();
        }
      }

      if (!collectPaths(dirInputSub, filters, bRecurseFolders,
                        dirOutputSub, qsOutputExtension,
                        pathsInput, pathsOutput)) // out
      {
        return false;
      }
    }
  }

  infsFiles = dirInput.entryInfoList(filters, 
                                       QDir::Files | QDir::Readable
                                     | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  foreach (QFileInfo info, infsFiles)
  {
    ODA_ASSERT_ONCE(info.isFile());

    QString qsInputPath = info.absoluteFilePath();
    pathsInput.push_back(qsInputPath);

    QString qsOutputPath = info.completeBaseName() + qsOutputExtension;
    qsOutputPath = dirOutput.absoluteFilePath(qsOutputPath);
    pathsOutput.push_back(qsOutputPath);
  }
  return true;
}

#if defined(ANDROID) && defined(_DEBUG)

static QString s_qsLog;

OdString getStringFromHex(const unsigned int* pHex)
{
  OdString sText;
  OdChar* pToStart = sText.getBuffer(512);
  OdChar* pTo = pToStart;
  do 
  *pTo++ = *(const OdChar*) pHex++;
  while (*pHex);
  *pTo = OdChar('\0');
  sText.releaseBuffer();
  return sText;
}

#define odPrintConsoleString toLogFile
static void toLogFile(const OdChar* pszMgs)
{
  s_qsLog += toQString(pszMgs);
}

static void checkMakeLowerUpper(const OdChar* pszComment, const unsigned int* pSrcHex, 
                                const unsigned int* pLowerHex, const unsigned int* pUpperHex = NULL)
{
  OdString sComment(pszComment),
           sTxt = getStringFromHex(pSrcHex);
  QString qsTst = toQString(sTxt),
          qsOk;

  if (pSrcHex)
  {
    sTxt.makeLower();
    qsTst = toQString(sTxt);

    OdString sOk = getStringFromHex(pLowerHex);
    qsOk = toQString(sOk);

    if (sTxt == sOk)
      odPrintConsoleString(L"  " + sComment + L"  \"" + sTxt + L"\" == \"" + sOk + L"\" : OK\n");
    else
      odPrintConsoleString(L"  " + sComment + L"  \"" + sTxt + L"\" != \"" + sOk + L"\" : FAILED\n");
    ODA_ASSERT(sTxt == sOk);
  }
  if (pUpperHex)
  {
    sTxt.makeUpper();
    qsTst = toQString(sTxt);

    OdString sOk = getStringFromHex(pUpperHex);
    qsOk = toQString(sOk);

    if (sTxt == sOk)
      odPrintConsoleString(L"  " + sComment + L"  \"" + sTxt + L"\" == \"" + sOk + L"\" : OK\n");
    else
      odPrintConsoleString(L"  " + sComment + L"  \"" + sTxt + L"\" != \"" + sOk + L"\" : FAILED\n");
    ODA_ASSERT(sTxt == sOk);
  }
}

void GermanUmlauts_Mac_9860_YM()
{
  unsigned int hex0[] = { 0x41, 0x5a, 0x61, 0x7a, 0x20, 0x4a, 0x6f, 0x68, 0x6e, 0x20, 0x44, 0x6f, 0x65, 0x00 },
    hexLow0[] = { 0x61, 0x7a, 0x61, 0x7a, 0x20, 0x6a, 0x6f, 0x68, 0x6e, 0x20, 0x64, 0x6f, 0x65, 0x00 },
    hexUp0[] = { 0x41, 0x5a, 0x41, 0x5a, 0x20, 0x4a, 0x4f, 0x48, 0x4e, 0x20, 0x44, 0x4f, 0x45, 0x00 };
  checkMakeLowerUpper(L"en:", hex0, hexLow0, hexUp0);
  unsigned int hex1[] = { 0x0412, 0x0430, 0x0441, 0x044f, 0x20, 0x041f, 0x0443, 0x043f, 0x043a, 0x0438, 0x043d, 0x00 },
    hexLow1[] = { 0x0432, 0x0430, 0x0441, 0x044f, 0x20, 0x043f, 0x0443, 0x043f, 0x043a, 0x0438, 0x043d, 0x00 },
    hexUp1[] = { 0x0412, 0x0410, 0x0421, 0x042f, 0x20, 0x041f, 0x0423, 0x041f, 0x041a, 0x0418, 0x041d, 0x00 };
  checkMakeLowerUpper(L"rus:", hex1, hexLow1, hexUp1);
  unsigned int hex2[] = { 0xdc, 0xfc, 0xd6, 0xf6, 0x00 },
    hexLow2[] = { 0xfc, 0xfc, 0xf6, 0xf6, 0x00 },
    hexUp2[] = { 0xdc, 0xdc, 0xd6, 0xd6, 0x00 };
  checkMakeLowerUpper(L"de:", hex2, hexLow2, hexUp2);
  unsigned int hex3[] = { 0x30ea, 0x30f3, 0x30af, 0x00 };
  checkMakeLowerUpper(L"jap:", hex3, hex3, hex3);
}

void OdStrFormat_Mac_7324_YM()
{
  OdAnsiString aStr = "\\U+30EA\\U+30F3\\U+30AF";
  OdString sNameIn(aStr),
           sNameOut;
  sNameIn.getLength(); // force
  QString qsNameIn = toQString(sNameIn),
          qsNameOut;
  bool bOk; 

  odPrintConsoleString(L"in :" + sNameIn + L"\n");

  sNameOut.format(OD_T("%ls$0$"), sNameIn.c_str());
  //sNameOut.format(OD_T("%ls_123"), sNameIn.c_str());
  qsNameOut = toQString(sNameOut);

  odPrintConsoleString(L"out: " + sNameOut + L"\n");

  bOk = (sNameOut == sNameIn + L"$0$");
  //bOk = (sNameOut == sNameIn + L"_123");
  odPrintConsoleString(bOk ? OD_T("  Ok") : OD_T("  Failed"));

  ODA_ASSERT(bOk);

  /// scanf ///

  OdString sTstIn(sNameIn),
    sFmt = sNameIn;
  sTstIn += L" 123.456";
  sFmt += L" %lg";
  QString qsTstIn = toQString(sTstIn),
          qsFmt = toQString(sFmt);
  double dVal = 0.0;
  odSScanf(sTstIn, sFmt, &dVal);

  ODA_ASSERT(OdEqual(dVal, 123.456, 1e-5));
}

#define GETRAND() (unsigned int)(0x0FFFF * (rand() / ((double)(RAND_MAX) + (double)1.0)))
void testWChar()
{
  // once only
  static bool s_bFirst = true;
  if (!s_bFirst)
    return;
  s_bFirst = false;

  //if (s_bFirst) //TODO
    OdStrFormat_Mac_7324_YM();
  GermanUmlauts_Mac_9860_YM();

  int szWCharT = sizeof(wchar_t),
      szOdChar = sizeof(OdChar);

  wchar_t buf[150];
  buf[0] = L'\0';
  //OdChar buf[150];  // only 38 are needed, but increase to prevent buffer overrun in case of strange errors
  const wchar_t* fmt = L"{%d}"; // OD_T("{%d}")
  odSprintf(buf, 150, L"{}");
  odSprintf(buf, 150, L"%ls", L"54321");
  odSprintf(buf, 150, fmt, 12345);
  QString qsTst;
  OdString sTst(buf);
  qsTst = toQString(sTst);

  int index = 0;
  OdString sTmp = L"*Paper_Space",
           sName;
  //#define odPrintf(dest, size, szf, arglist) vswprintf(dest, size, szf, arglist)
  sName.format(L"%ls%d", sTmp.c_str(), index++); // OdPrintf
  qsTst = toQString(sName);
  ODA_ASSERT(qsTst == "*Paper_Space0");

  odSprintf(buf, 150, OD_T("{%d}"), 12345);
  qsTst = toQString(OdString(buf));
  ODA_ASSERT(qsTst == "{12345}");

  odSprintf(buf, 150, OD_T("{%4.4X}"), 1);
  qsTst = toQString(OdString(buf));
  int len = odStrLen(buf);
  ODA_ASSERT(len == 6);

  srand((unsigned)clock());
  unsigned int val0 = GETRAND(),
               val1 = GETRAND(),
               val2 = GETRAND(),
               val3 = GETRAND(),
               val4 = GETRAND(),
               val5 = GETRAND(),
               val6 = GETRAND(),
               val7 = GETRAND();
  odSprintf(buf, 150, OD_T("{%4.4X%4.4X-%4.4X-%4.4X-%4.4X-%4.4X%4.4X%4.4X}"), val0, val1, val2, val3, val4, val5, val6, val7);
  len = odStrLen(buf);
  ODA_ASSERT(len == 38);
  qsTst = toQString(OdString(buf));

  int iVal = 0;
  odSScanf(L"1", L"%i", &iVal);
  iVal = odStrToInt(L"123");
  ODA_ASSERT(iVal == 123);

  double dVal = 0.0;
  odSScanf(L"1.1", L"%lf", &dVal);
  ODA_ASSERT(OdEqual(dVal, 1.1, 1e-5));

  bool bVal = odStrCmp(OdString(L"test"), L"test") == 0;
  ODA_ASSERT(bVal);
  bVal = odStrCmp(L"TEST", L"test") == 0;
  ODA_ASSERT(!bVal);

  float fVal = odStrToF(L"123.456");
  ODA_ASSERT(OdEqual(fVal, 123.456, 1e-5));

  //use odStrStr
  sTst = L"aaa\\bbb\\ccc.ext";
  iVal = sTst.replace(L'\\', L'/');
  qsTst = toQString(sTst);

  std::wstring sTmpFileName(sTst.c_str());
  iVal = (int)sTmpFileName.rfind(L'.');
  if (iVal > 0)
    sTmpFileName = sTmpFileName.substr(0, iVal);
  sTmpFileName += L".tmp";
  qsTst = toQString(sTmpFileName.c_str());
  ODA_ASSERT(qsTst == "aaa/bbb/ccc.tmp");

  saveTextToFile(s_qsLog, "/sdcard/_Oda/convertor.log");
}
#else
  #define testWChar()
#endif

bool CConvertor::startConvertion(const QString& qsInputPath,
                                 const QString& qsInputFileWildcard,
                                 bool bRecurseFolders,
                                 bool bAudit,
                                 const QString& qsOutputPath,
                                 OdDb::DwgVersion verSaveAs,
                                 OdDb::SaveType typeSaveAs,
                                 QLineEdit* pStateBar, // = NULL
                                 QProgressBar* pProgressBar, // = NULL
                                 QProgressBar* pSpasticProgressBar, // = NULL
                                 bool bSilent) // = false
{
  if (m_bBusy)
    return false;
  m_bBusy = true;
  m_bSilent = bSilent;
  testWChar();

  ODA_ASSERT_ONCE(m_bInitialized);
  m_bStopRequest = false;

  if (qsInputPath.isEmpty() || !m_dirInput.cd(qsInputPath))
  {
    QMessageBox::warning(QApplication::activeWindow(), // qApp->activeWindow()
                         QObject::tr("Error"), // title
                         QObject::tr("Invalid or empty input path specified"));
    m_bBusy = false;
    return false;
  }

  if (qsOutputPath.isEmpty())
  {
    QMessageBox::warning(QApplication::activeWindow(), QObject::tr("Error"),
                         QObject::tr("Empty output path specified"));
    m_bBusy = false;
    return false;
  }
  if (!QDir().exists(qsOutputPath))
  {
    if (   !m_bSilent
        && QMessageBox::Yes != QMessageBox::question(
              QApplication::activeWindow(), 
              QObject::tr("ODA File Converter"), // caption
              QObject::tr("Directory %1 does not exist.\nDo you want to create it.")
                .arg(qsOutputPath), // text
              QMessageBox::Yes | QMessageBox::No,
              QMessageBox::Yes))
    {
      m_bBusy = false;
      return false;
    }

    if (!QDir().mkpath(qsOutputPath))
    {
      QMessageBox::warning(
              QApplication::activeWindow(), QObject::tr("Error"),
              QObject::tr("Unable to create output folder [%1]").arg(qsOutputPath));
      m_bBusy = false;
      return false;
    }
  }
  if (!m_dirOutput.cd(qsOutputPath))
  {
    QMessageBox::warning(QApplication::activeWindow(), QObject::tr("Error"),
                         QObject::tr("Invalid output path specified"));
    m_bBusy = false;
    return false;
  }

  m_verSaveAs = verSaveAs;
  m_typeSaveAs = typeSaveAs;

  if (   m_dirInput.absolutePath().toLower() == m_dirOutput.absolutePath().toLower()
      && (   (   m_typeSaveAs == OdDb::kDwg
              && qsInputFileWildcard.indexOf(".dwg", 0, Qt::CaseInsensitive) >= 0)
          || (   (m_typeSaveAs == OdDb::kDxf || m_typeSaveAs == OdDb::kDxb)
              && qsInputFileWildcard.indexOf(".dxf", 0, Qt::CaseInsensitive) >= 0)))
          //|| (   m_typeSaveAs == OdDb::kDxb
          //    && qsInputFileWildcard.indexOf(".dxb", 0, Qt::CaseInsensitive) >= 0)))
  {
    QMessageBox::warning(QApplication::activeWindow(), QObject::tr("Error"),
                         QObject::tr("Output folder must be different than input folder."));
    m_bBusy = false;
    return false;
  }

  m_bRecurseFolders = bRecurseFolders,
  m_bAudit = bAudit;
  m_pProgressBar = pProgressBar;
  m_pSpasticProgressBar = pSpasticProgressBar;
  m_pStateBar = pStateBar;

  int stepProgress = 1; // 2
  if (!m_pProgressBar.isNull())
    m_pProgressBar->setValue(0);

  QString qsOutputExtension = ".dwg";
  if (typeSaveAs == OdDb::kDxf || typeSaveAs == OdDb::kDxb) 
    qsOutputExtension = ".dxf";
  //else if (typeSaveAs == OdDb::kDxb)
  //  qsOutputExtension = ".dxb";
  else
    ODA_ASSERT_ONCE(typeSaveAs == OdDb::kDwg); 

  QStringList filters = QString(qsInputFileWildcard)
                          .replace("; ", ";").replace(" ;", ";")
                          .split(";", QString::SkipEmptyParts, Qt::CaseInsensitive),
              pathsInput,
              pathsOutput; 
  if (!collectPaths(m_dirInput, filters, m_bRecurseFolders, m_dirOutput, qsOutputExtension,
                    pathsInput, pathsOutput)) // out
  {
    QMessageBox::warning(QApplication::activeWindow(), QObject::tr("Error"),
                         QObject::tr("Output folder is inaccessible to write."));
    m_bBusy = false;
    return false;
  }

  if (!pathsInput.size())
  {
    QMessageBox::warning(QApplication::activeWindow(), QObject::tr("Error"),
                         QObject::tr("There is no matched files in input folder."));
    m_bBusy = false;
    return false;
  }

  ODA_ASSERT_ONCE(pathsInput.size() == pathsOutput.size()); 
  if (!m_pProgressBar.isNull())
  {
    m_pProgressBar->setRange(0, pathsInput.size() * stepProgress);
    m_pProgressBar->setValue(stepProgress > 1 ? 1 : 0); // step of files collecting 
  }

  QMessageBox::StandardButton buttonToAll = m_bSilent ? QMessageBox::Yes
                                                      : QMessageBox::NoButton;

  for (int index = 0; index < pathsInput.size(); index++)
  {
    QString qsInpitFilePath = pathsInput[index],
            qsOutputFilePath = pathsOutput[index];

    if (!m_pStateBar.isNull())
    {
      QString qsState = QFileInfo(qsInpitFilePath).fileName();
      m_pStateBar->setText(qsState);
    }

    QFileInfo info(qsOutputFilePath);
    if (info.exists())
    {
      QMessageBox::StandardButton button = buttonToAll;
      static QMessageBox::StandardButton buttonCurrent = QMessageBox::YesToAll;

      if (!info.size() || info.isDir())
        button = QMessageBox::Yes;

      if (buttonToAll == QMessageBox::NoButton)
      {
        button = QMessageBox::warning(
                    QApplication::activeWindow(), 
                    QObject::tr("ODA File Converter"), // caption
                    QObject::tr("Destination file \"%1\" already exists.\n"
                                "Do you want to replace it?").arg(qsOutputFilePath),
                    (  QMessageBox::Yes | QMessageBox::YesToAll 
                     | QMessageBox::NoToAll | QMessageBox::No),
                    buttonCurrent); // QMessageBox::YesToAll
        buttonCurrent = button;
        if (button == QMessageBox::YesToAll || button == QMessageBox::NoToAll)
          buttonToAll = button;
      }

      if (button == QMessageBox::NoToAll || button == QMessageBox::NoToAll)
      {
        checkProgress(stepProgress); // part of checkStop
        continue;
      }

      //overwrite
      if (info.isDir())
        QDir().rename(qsOutputFilePath, qsOutputFilePath +".back");
      else
        QFile::remove(qsOutputFilePath);
    }

    try 
    {
      convertFile(qsInpitFilePath, qsOutputFilePath);
    }
    catch (OdError& err)
    {
      ODA_ASSERT_ONCE(err.code() == eUserBreak);
      m_bBusy = false;
      return (err.code() == eUserBreak);
    }
    checkProgress(stepProgress); // part of checkStop
  }

  if (!m_pStateBar.isNull())
  {
    m_pStateBar->setText(QObject::tr("Done"));
  }

  m_bBusy = false;
  return true;
}

void CConvertor::stopConvertion(bool bForce) // = false
{
  m_bStopRequest = true;
  m_bSilent = bForce;
}

// Notification function called to initialize the progress meter

void CConvertor::start(const OdString&) // displayString = OdString::kEmpty)
{
  checkStop();
}

// Notification function called to reset the progress meter
void CConvertor::stop()
{
  checkStop();
}

// Notification function called to increment the progress meter

void CConvertor::meterProgress()
{
  checkStop(0, 1);
}

// Notification function called to specify the maximum number of times
// the ProgressMeter object will be incremented
void CConvertor::setLimit(int max)
{
  if (!m_pSpasticProgressBar.isNull())
  {
    m_pSpasticProgressBar->setRange(0, max);
    m_pSpasticProgressBar->setValue(0);
  }
  checkStop();
}

// Notification function called to display a warning message
void CConvertor::warning(const char* warnVisGroup, const OdString& sMsg)
{
  if (warnVisGroup && *warnVisGroup)
    return; 
  m_qsError += QObject::tr("ODA Warning: %1\n").arg(toQString(sMsg));
}

// Convert qsInputFileName to qsOutputFileName
void CConvertor::convertFile(const QString& qsInputFileName,
                             const QString& qsOutputFileName)
{
  OdString sInputFileName = toOdString(qsInputFileName);
  QString qsState; 
  OdDbDatabasePtr m_pDb;

  try 
  {
    // #8990 size of the file  with extension .err is changed, each time when a user attempts to convert bad dwg file
    m_qsError.clear();
    if (QFile::exists(qsOutputFileName + ".err"))
      QFile::remove(qsOutputFileName + ".err");

    if (!m_pStateBar.isNull())
    {
      qsState = QFileInfo(qsInputFileName).fileName() + QObject::tr(" reading");
      m_pStateBar->setText(qsState);
    }

    // Create a database and the file into it
    m_pDb = readFile(sInputFileName);

    if (!m_pStateBar.isNull())
    {
      qsState += QObject::tr(" done");
      m_pStateBar->setText(qsState);
    }
  }
  catch (OdError& err)
  {
    if (err.code() == eUserBreak)
    {
      if (!m_pStateBar.isNull())
        m_pStateBar->setText(QObject::tr("Terminated by user."));
      throw err;
    }
    m_qsError += QObject::tr("OdError thrown during readFile of drawing \"%1\" :\n  %2\n")
                                .arg(qsInputFileName)
                                .arg(toQString(err.description()));
    saveTextToFile(m_qsError, qsOutputFileName + ".err");
    return;
  }
  catch (...)
  {
    ODA_FAIL_ONCE();
    m_qsError += QObject::tr("General Exception thrown during readFile of drawing \"%1\"\n")
      .arg(qsInputFileName);
    //m_qsError += QObject::tr("  Test\"%1\"\n")
    //  .arg(toQString(sInputFileName.makeUpper()));

    saveTextToFile(m_qsError, qsOutputFileName + ".err");
	  return;
  }

  bool bAuditProgress = true;
  try 
  {
    // Create a output file
    //OdWrFileBuf fb; //OdRxObjectImpl<OdWrFileBuf> fb;
    //fb.open(toOdString(qsOutputFileName));

    // Audit the file, and fix any errors
    if (m_bAudit)
    {
      // Audit the file, and fix any errors
      dwgAudit ainfo;
      ainfo.setFixErrors(true);
      ainfo.setPrintDest(OdDbAuditInfo::kSilent);

      if (!m_pStateBar.isNull())
      {
        qsState = QFileInfo(qsInputFileName).fileName() + QObject::tr(" auditing");
        m_pStateBar->setText(qsState);
      }

      m_pDb->auditDatabase(&ainfo);

      if (!m_pStateBar.isNull())
      {
        qsState += QObject::tr(" done");
        m_pStateBar->setText(qsState);
      }
    }

    bAuditProgress = false;

    if (!m_pStateBar.isNull())
    {
      qsState = QFileInfo(qsOutputFileName).fileName() + QObject::tr(" writing");
      m_pStateBar->setText(qsState);
    }

    // Write the file
    // workaroud for crash on max os x release v3.8 //m_pDb->writeFile(&fb, m_typeSaveAs, m_verSaveAs, true);
    m_pDb->writeFile(toOdString(qsOutputFileName), m_typeSaveAs, m_verSaveAs, true);

    if (!m_pStateBar.isNull())
    {
      qsState += QObject::tr(" done");
      m_pStateBar->setText(qsState);
    }
  }
  catch (OdError& err)
  {
    if (err.code() == eUserBreak)
    {
      if (!m_pStateBar.isNull())
        m_pStateBar->setText(QObject::tr("Terminated by user."));
      throw err;
    }
    if (bAuditProgress)
      m_qsError += QObject::tr("OdError thrown during auditDatabase of \"%1\" :\n  %2\n")
                      .arg(qsInputFileName)
                      .arg(toQString(err.description()));
    else
      m_qsError += QObject::tr("OdError thrown during writeFile to \"%1\" :\n  %2\n")
                      .arg(qsOutputFileName)
                      .arg(toQString(err.description()));
    saveTextToFile(m_qsError, qsOutputFileName + ".err");
    return;
  }
  catch (...)
  {
    ODA_FAIL_ONCE();
    if (bAuditProgress)
      m_qsError += QObject::tr("General Exception thrown during auditDatabase of \"%1\"\n")
                      .arg(qsInputFileName);
    else
      m_qsError += QObject::tr("General Exception thrown during writeFile to \"%1\"\n")
                      .arg(qsOutputFileName);
    saveTextToFile(m_qsError, qsOutputFileName + ".err");
  }
  m_pDb.release();
}

