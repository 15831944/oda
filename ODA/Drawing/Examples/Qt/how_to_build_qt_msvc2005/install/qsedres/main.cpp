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
// qsedres_main.cpp
//

#include <windows.h>

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QStringList>


//inline LPCWSTR lpLibFileName
//wchar_t toOdString(const QString& qsStr)
//{
//  std::wstring wstr(qsStr.toStdWString());
//  OdString sText(wstr.c_str());
//  return sText;
//}
//
//inline QString toQString(const OdString& sStr)
//{
//  std::wstring wstr(sStr.c_str());
//  QString qsStr(QString::fromStdWString(wstr));
//  return qsStr;
//}

static QTextStream* s_pStreamErr = NULL;
static void editString(QString& qsValue); // in /out

#pragma pack( push )
#pragma pack( 2 )

typedef struct  {
  unsigned short len;
  unsigned short unknown;
} TVersHeader;

#pragma pack( pop )

static BOOL CALLBACK EnumResNameProc(HMODULE hModule,
                                     LPCTSTR pszType,
                                     LPTSTR pszName,
                                     LONG_PTR lParam)
{
  HANDLE hUpdate  = (HANDLE) lParam;    

  if (pszType == RT_VERSION)
  {
    HRSRC hRes =  FindResource(hModule, pszName, pszType);  
    int nSizeOfRes = SizeofResource(hModule,hRes);  
    HGLOBAL hResLoaded = LoadResource(hModule, hRes);   
    void* pData = LockResource(hResLoaded);   
    TVersHeader* pVersHeader = (TVersHeader*) pData;

    QByteArray btsInput((char*) pData, nSizeOfRes),
               btsOutput;
    int index = 0,
        pos = 4, 
        posPrev = 4;
    for (; (pos = btsInput.indexOf((const char*) L"\0", posPrev)) >= 0; index++, posPrev = pos)
    {
      if (pos == posPrev)
      {
        //skip en empty
        pos += 2;
        continue;
      }
      QByteArray btsValue = btsInput.mid(posPrev, pos);
      QString qsValue = QString::fromStdWString((LPTSTR) btsValue.data());
      pos = pos; // brk
    }

    //foreach(QString qsInfoKey, QStringList() << "Build Comments" 
    //                                         << "Comments" 
    //                                         << "CompanyName" 
    //                                         << "FileDescription" 
    //                                         << "InternalName" 
    //                                         << "OriginalFilename")
    //{
    //  std::wstring wsInfoKey(qsInfoKey.toStdWString());
    //  const char* pszInfoKey = (const char*) wsInfoKey.c_str();
    ////int pos = btsValue.indexOf("FileDescription");
    ////if (pos < 0)
    //  //int pos = btsValue.indexOf((const char*) L"FileDescription");
    //  int pos = btsValue.indexOf(pszInfoKey);
    //  pos = pos;
    //}

    /*
    BYTE* pInfo = (BYTE*) pData; // GetFileVersionInfo
    UINT nLen = 0;
    LPVOID lpBuff = NULL;
    QString qsBlock = "\\StringFileInfo\\Translation";
    std::wstring wstr(qsBlock.toStdWString());
    //::VerQueryValue(pInfo, (LPTSTR) wstr.c_str(), &lpBuff, &nLen);
    ::VerQueryValue(pInfo, (LPTSTR) L"\\StringFileInfo\\Translation", &lpBuff, &nLen);
    struct LANGANDCODEPAGE {
      WORD wLanguage;
      WORD wCodePage;
    } *lpTranslate;
    lpTranslate = (LANGANDCODEPAGE*) lpBuff;

    //CString block;
    //block.Format(_T("\\StringFileInfo\\%04x%04x\\%s"), 
    //  lpTranslate->wLanguage, lpTranslate->wCodePage, szInfoKey);
    ///::VerQueryValue(pInfo, (LPTSTR)block.GetString(), &lpBuff, &nLen);
    //
    foreach(QString qsInfoKey, QStringList() << "FileDescription" << "InternalName" << "OriginalFilename")
    {
      if (lpTranslate)
        qsBlock = QString("\\StringFileInfo\\%1%2\\%3")
                              .arg(lpTranslate->wLanguage, 4, 16)
                              .arg(lpTranslate->wCodePage, 4, 16)
                              .arg(qsInfoKey);
      else
        qsBlock = QString("\\StringFileInfo\\%1").arg(qsInfoKey);
      wstr = std::wstring (qsBlock.toStdWString());

      ::VerQueryValue(pInfo, (LPTSTR) wstr.c_str(), &lpBuff, &nLen);

      if (nLen <= 0)
        continue;

      std::wstring wsValue((TCHAR*) lpBuff);
      QString qsValue = QString::fromStdWString(wsValue);
      editString(qsValue);

      if (qsValue == QString::fromStdWString(wsValue))
        continue;

      // TODO
      //if (!hUpdate)
      //  hUpdate = BeginUpdateResource(szExePath, FALSE);

      //UpdateResource(hUpdate,szType,szName,MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),pData,nSizeOfRes);

    } // foreach
    */

    UnlockResource(hResLoaded);   
    FreeResource(hResLoaded);   
    return TRUE;
  }

  if (pszType == RT_STRING)
  {
    DWORD idRes = (DWORD) pszName; 
    DWORD idStart = (idRes -1) * 16;
    DWORD idEnd = idStart + 16;  
    for (DWORD id = idStart ; id < idEnd; id++) 
    {   
      TCHAR buffer[2048];

      if (!LoadString(hModule, id, buffer, sizeof(buffer)/sizeof(TCHAR)))
        continue;

      std::wstring wsOut(buffer);
      QString qsValue = QString::fromStdWString(wsOut);
      editString(qsValue);

      if (qsValue == QString::fromStdWString(wsOut))
        continue;

      // TODO
      static bool bFirst = true;
      if (bFirst)
      {
        bFirst = false;
        (*s_pStreamErr) << QObject::tr("Not implemented yet");
        s_pStreamErr->flush();
        return FALSE;
      }
    }
    return TRUE;
  }

  return TRUE;
}

static QStringList s_lstExpressions; 

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  QTextStream streamErr(stderr);
  s_pStreamErr = &streamErr;

  enum {
    enSt_Normal // inputFileName or -option
  , enSt_Expression
  , enSt_CommandsFromFile
  } state = enSt_Normal;
  
  QString qsExecutableFilePathName;

  QStringList lstArgs = QCoreApplication::arguments();
  lstArgs.removeFirst();
  
  foreach (QString qsArg, lstArgs)
  {
    switch (state)
    {
    case enSt_Normal:
      if (!qsArg.indexOf("-"))
      {
        qsArg = qsArg.mid(1);
        if (qsArg.length() == 1)
        {
          switch (qsArg.toAscii().at(0))
          {
          case 'e':
            state = enSt_Expression;
            continue;
          case 'f':
            state = enSt_CommandsFromFile;
            continue;
          }
        }
        streamErr << QObject::tr("Unsupported option \"-%1\".").arg(qsArg);
        streamErr.flush();
        return 1;
      }
      qsExecutableFilePathName = qsArg;
      break;
    case enSt_Expression:
      s_lstExpressions << qsArg;
      state = enSt_Normal;
      break;
    case enSt_CommandsFromFile:
      {
        QFile dataCmd(qsArg);
        if (   qsArg.isEmpty()
            || !dataCmd.exists()
            || !dataCmd.open(QIODevice::ReadOnly))
        {
          streamErr << QObject::tr(
              "File \"%1\" was not found or inaccessible"
            ).arg(qsArg);
          streamErr.flush();
          return 1;
        }

        QTextStream inputCmd(&dataCmd);
        while (!inputCmd.atEnd())
        {
          QString qsLine = inputCmd.readLine().trimmed();
          if (!qsLine.isEmpty())
            s_lstExpressions << qsLine;
        }
      }
      state = enSt_Normal;
      break;
    } // end switch
  } // end foreach

  if (qsExecutableFilePathName.isEmpty())
  {
    // http://en.wikipedia.org/wiki/Sed
    streamErr << QObject::tr(
      "String editor in resources usage:\n"
      "  qsedres -e \"s/oldstuff/newstuff\" input_outpu_executable_file_name\n"
      "   options:\n"
      "       -e      indicates that the sed expression follows\n"
      "       -f      to read the commands from the file\n"
    );
    streamErr.flush();
    return 1;
  }

  std::wstring wsExecutableFilePathName(qsExecutableFilePathName.toStdWString());
	HMODULE hMod = LoadLibraryEx(wsExecutableFilePathName.c_str(), NULL, LOAD_LIBRARY_AS_DATAFILE);
	if (!hMod)
  {
    streamErr << QObject::tr("File \"%1\" was not found or inaccessible").arg(qsExecutableFilePathName);
    streamErr.flush();
    return 1;
  }

  HANDLE hUpdate = NULL; //LONG_PTR lParam = NULL;
  // will set be first changes // hUpdate = BeginUpdateResource(szExePath, FALSE);  

  EnumResourceNames(hMod, RT_VERSION, EnumResNameProc, (LONG_PTR) &hUpdate);
  EnumResourceNames(hMod, RT_STRING, EnumResNameProc, (LONG_PTR) &hUpdate);
  
  if (hUpdate)
    EndUpdateResource(hUpdate, FALSE);

  FreeLibrary(hMod);  
  return 0;
}

static void editString(QString& qsValue) // in /out
{
  QList<int> lstExpIndexes; 
  foreach (QString exp, s_lstExpressions)
  {
    lstExpIndexes << 0; 
  }
  QStringList lstCmds;
  lstCmds << "s" << "replace";

  int index = 0;
  foreach (QString qsExp, s_lstExpressions)
  {
    int& indexExp = lstExpIndexes[index++];
    if (indexExp >= qsExp.length())
      indexExp = 0;
    qsExp = qsExp.mid(indexExp);

    for (bool bRes = true; bRes;)
    {
      bRes = false;
      while (!qsExp.indexOf(" "))
      {
        qsExp = qsExp.mid(1);
        indexExp++;
      }
      if (qsExp.isEmpty())
        continue;

      int indexExpNext = indexExp;

      int indexCmd = -1;
      foreach (QString qsCmd, lstCmds)
      {
        indexCmd++;
        if (qsExp.indexOf(qsCmd))
          continue;
        qsExp = qsExp.mid(qsCmd.length());
        indexExpNext += qsCmd.length();
        switch (indexCmd)
        {
        case 0: case 1: // "s" "replace"
          {
            if (qsExp.isEmpty())
            {
              (*s_pStreamErr) << QObject::tr("Invalid argument of replace command");
              s_pStreamErr->flush();
              return;
            }

            QString qsDelim = qsExp[0];
            qsExp = qsExp.mid(1);
            indexExpNext++;
            int pos = qsExp.indexOf(qsDelim);
            if (pos >= 0)
            {
              QString qsFrom = qsExp.left(pos);
              qsExp = qsExp.mid(++pos);
              indexExpNext += pos;
              pos = qsExp.indexOf(qsDelim);
              if (pos >= 0)
              {
                QString qsTo = qsExp.left(pos);
                qsExp = qsExp.mid(++pos);
                indexExpNext += pos;

                if (!qsFrom.isEmpty())
                {
                  bRes = (qsValue.indexOf(qsFrom) >= 0);
                  qsValue.replace(qsFrom, qsTo);
                }
              }
            }
          }
          break;
        default:
          break;
        } // end switch

        break;

      } // end foreach

      if (bRes) 
        indexExp = indexExpNext;

    } // end for

  } // end foreach

  qsValue.trimmed();
}

