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
// ExAppServicesImpl.cpp
//

#include "OdaCommon.h"
#include "StaticRxObject.h"
#include "DbBaseHostAppServices.h"
#include "TDVersion.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QSettings>

#include "OdqInterfaces.h"
#include "ExAppServicesImpl.h"
#include "RxSysRegistry.h"

///////////////////////////////////////////////////////////////////////////////

#if defined(ODA_EX_APP_SERVICES_UNLINKED_RXINIT)
  ODRX_NO_CONS_DEFINE_MEMBERS(ExAppServices, OdRxObject);
#endif

///////////////////////////////////////////////////////////////////////////////

class ExAppServicesImpl : public ExAppServices
{
public:
  ODRX_DECLARE_MEMBERS(ExAppServicesImpl);

  virtual void executeCommand(const OdString& sCmd,
                              bool bEcho = true,
                              OdRxObject* pRxDatabase = NULL,
                              OdVariant* pvRes = NULL);
  virtual void postponeCommand(const OdString& sCmd,
                               bool bEcho = true,
                               OdRxObject* pRxDatabase = NULL, // current
                               int timeStep = 0); // milliseconds

  virtual bool readRegistryValue(const OdString& sKey, 
				                         OdSysVarType typeRb, void* pValue);
  virtual void writeRegistryValue(const OdString& sKey, 
				                          OdSysVarType typeRb, const void* pValue);

  virtual bool isNetAssembly(OdString& sPath, // in / out(normalized as side effect)
                             OdString* psVersion = NULL);
};
//typedef OdSmartPtr<ExAppServicesImpl> ExAppServicesImplPtr;

ODRX_NO_CONS_DEFINE_MEMBERS(ExAppServicesImpl, ExAppServices);

void ExAppServicesImpl::executeCommand(const OdString& sCmd,
                                       bool bEcho, // = true
                                       OdRxObject* pRxDatabase, // = NULL // current
                                       OdVariant* pvRes) // = NULL
{
  OdVariant vRes = getIConsole()->executeCommand(getIApp()->toQString(sCmd), bEcho, pRxDatabase);
  if (pvRes)
    *pvRes = vRes;
}

void ExAppServicesImpl::postponeCommand(const OdString& sCmd,
                                        bool bEcho, // = true
                                        OdRxObject* pRxDatabase, // = NULL // current
                                        int timeStep) // = 0 milliseconds
{
  getIConsole()->postponeCommand(getIApp()->toQString(sCmd), bEcho, pRxDatabase, timeStep);
}

bool ExAppServicesImpl::readRegistryValue(const OdString& csKey, OdSysVarType typeRb,
                                          void* pValue)
{
  if (csKey.isEmpty())
  {
    ODA_FAIL_ONCE();
    return false;
  }

  OdString sKey(csKey);
  sKey.replace(__OD_T('\\'), __OD_T('/'));
  QString qsKey(TD_SHORT_STRING_VER_S);
  if (sKey.find(__OD_T('/')))
    qsKey += "/";
  qsKey += getIApp()->toQString(sKey);

  QSettings settings("ODA", QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  QString qsUndefMarker = "<<<$$$undefined$$$>>>";
  QString qsValue = settings.value(qsKey, qsUndefMarker).toString();
  if (qsValue == qsUndefMarker)
    return false;

  bool bOk = false;
  switch (typeRb)
  {
  case kRtBool:
    (*(bool *) pValue) = (qsValue.toLower() == "true" || qsValue == "1");
    return true;
  case kRtInt8:
    (*(OdInt8*) pValue) = (OdInt8) qsValue.toInt(&bOk);
    return bOk;
    break;
  case kRtInt16:
    (*(OdInt16*) pValue) = (OdInt16) qsValue.toInt(&bOk);
    return bOk;
  case kRtInt32:
    (*(OdInt32*) pValue) = (OdInt32) qsValue.toInt(&bOk);
    return bOk;
  case kRtDouble:
    (*(double*) pValue) = qsValue.toDouble(&bOk);
    return bOk;
  case kRtString:
    (*(OdString*) pValue) = getIApp()->toOdString(qsValue);
    return true;
  }

  ODA_FAIL_ONCE(); // TODO
  return false;
}

void ExAppServicesImpl::writeRegistryValue(const OdString& csKey, OdSysVarType typeRb,
                                           const void* pValue)
{
  if (csKey.isEmpty())
  {
    ODA_FAIL_ONCE();
    return;
  }

  OdString sKey(csKey);
  sKey.replace(__OD_T('\\'), __OD_T('/'));
  QString qsKey(TD_SHORT_STRING_VER_S);
  if (sKey.find(__OD_T('/')))
    qsKey += "/";
  qsKey += getIApp()->toQString(sKey);

  QString qsValue;
  switch (typeRb)
  {
  case kRtBool:
    qsValue = (*(const bool *) pValue) ? "true" : "false";
    break;
  case kRtInt8:
    qsValue = QString::number(*(const OdInt8*) pValue);
    break;
  case kRtInt16:
    qsValue = QString::number(*(const OdInt16*) pValue);
    break;
  case kRtInt32:
    qsValue = QString::number(*(const OdInt32*) pValue);
    break;
  case kRtDouble:
    qsValue = QString::number(*(const double*) pValue);
    break;
  case kRtString:
    qsValue = getIApp()->toQString(*(const OdString*) pValue);
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return;
  }

  QSettings settings("ODA", QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.setValue(qsKey, qsValue);
}

#if defined(Q_OS_WIN64) && defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER) // PROD-124

#define MKPTR(p1,p2) ((DWORD_PTR)(p1) + (DWORD_PTR)(p2))

typedef enum _pe_architecture {
  PE_ARCHITECTURE_UNKNOWN = 0x0000,
  PE_ARCHITECTURE_ANYCPU = 0x0001,
  PE_ARCHITECTURE_X86 = 0x010B,
  PE_ARCHITECTURE_x64 = 0x020B
} PE_ARCHITECTURE;

LPVOID GetOffsetFromRva(IMAGE_DOS_HEADER *pDos, IMAGE_NT_HEADERS *pNt, DWORD rva) 
{
  IMAGE_SECTION_HEADER* pSecHd = IMAGE_FIRST_SECTION(pNt);
  for (unsigned long i = 0; i < pNt->FileHeader.NumberOfSections; ++i, ++pSecHd) 
  {
    // Lookup which section contains this RVA so we can translate the VA to a file offset
    if (rva >= pSecHd->VirtualAddress && rva < (pSecHd->VirtualAddress + pSecHd->Misc.VirtualSize)) 
    {
      DWORD delta = pSecHd->VirtualAddress - pSecHd->PointerToRawData;
      return (LPVOID) MKPTR(pDos, rva - delta);
    }
  }
  return NULL;
}

PE_ARCHITECTURE GetImageArchitecture(void *pImageBase, bool& isAssembly) 
{
  isAssembly = false;
  // Parse and validate the DOS header
  IMAGE_DOS_HEADER *pDosHd = (IMAGE_DOS_HEADER*)pImageBase;
  if (IsBadReadPtr(pDosHd, sizeof(pDosHd->e_magic)) || pDosHd->e_magic != IMAGE_DOS_SIGNATURE)
    return PE_ARCHITECTURE_UNKNOWN;

  // Parse and validate the NT header
  IMAGE_NT_HEADERS *pNtHd = (IMAGE_NT_HEADERS*)MKPTR(pDosHd, pDosHd->e_lfanew);
  if (IsBadReadPtr(pNtHd, sizeof(pNtHd->Signature)) || pNtHd->Signature != IMAGE_NT_SIGNATURE)
    return PE_ARCHITECTURE_UNKNOWN;

  // First, naive, check based on the 'Magic' number in the Optional Header.
  PE_ARCHITECTURE architecture = (PE_ARCHITECTURE)pNtHd->OptionalHeader.Magic;

  // If the architecture is x86, there is still a possibility that the image is 'AnyCPU'
  
  if (architecture != PE_ARCHITECTURE_UNKNOWN)
  {
    IMAGE_DATA_DIRECTORY comDirectory = pNtHd->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR];
    if (comDirectory.Size)
    {
      IMAGE_COR20_HEADER *pClrHd = (IMAGE_COR20_HEADER*)GetOffsetFromRva(pDosHd, pNtHd, comDirectory.VirtualAddress);
      if (architecture == PE_ARCHITECTURE_X86)
      {
        // Check to see if the CLR header contains the 32BITONLY flag, if not then the image is actually AnyCpu
        if ((pClrHd->Flags & COMIMAGE_FLAGS_32BITREQUIRED) == 0)
          architecture = PE_ARCHITECTURE_ANYCPU;
      }
      if (pClrHd->Flags & COMIMAGE_FLAGS_ILONLY)
        isAssembly = true;
    }
  }
  return architecture;
}

#endif

static bool isManagedDll(OdString& sPath, // in / out // normalized as side effect
                         OdString* psVersion = NULL,
                          // see "winnt.h" :
                          // IMAGE_FILE_MACHINE_UNKNOWN(0x0)
                          //  IMAGE_FILE_MACHINE_I386(0x14c)
                          //  IMAGE_FILE_MACHINE_IA64(0x200)
                          //  IMAGE_FILE_MACHINE_AMD64(0x8664)
                          // ....
                         OdUInt32* pnTypeMachine = NULL)
{
  if (psVersion)
    psVersion->empty();

  OdDbBaseHostAppServices* pServices = getIApp()->getBaseHostAppServices();
  ODA_ASSERT_ONCE(pServices);
  OdString sFullPath = pServices->findFile(sPath, NULL, OdDbBaseHostAppServices::kTXApplication);
  if (!sFullPath.isEmpty())
    sPath = sFullPath;

 #if defined(TD_DLL_VERSION_SUFFIX_STR)
  else
  {
    sFullPath = sPath;
    int posSuffix = sFullPath.find(TD_DLL_VERSION_SUFFIX_STR);
    if (posSuffix > 0)
      return false;
    int posDot = sFullPath.reverseFind(__OD_T('.'));
    if (posDot > 0)
      sFullPath = sFullPath.left(posDot) + TD_DLL_VERSION_SUFFIX_STR + sFullPath.mid(posDot);
    else
      sFullPath += TD_DLL_VERSION_SUFFIX_STR;
    sFullPath = pServices->findFile(sFullPath, NULL, OdDbBaseHostAppServices::kTXApplication);
    if (sFullPath.isEmpty())
      return false;
    sPath = sFullPath;
  }
 #endif

#if defined(Q_OS_WIN64) && defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER) // PROD-124
  {
    // open file
    LPCWSTR lpFileName = (const wchar_t*) sPath.c_str();
    HANDLE file = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
      return false; // Could not read file

    // allocate heap
    DWORD fileSize = GetFileSize(file, NULL);
    LPVOID fileData = HeapAlloc(GetProcessHeap(), 0, fileSize);

    // read file bytes to memory
    DWORD bytesRead = NULL;
    ReadFile(file, fileData, fileSize, &bytesRead, NULL);

    // https://stackoverflow.com/questions/480696/how-to-find-if-a-native-dll-file-is-compiled-as-x64-or-x86

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)fileData;
    if (dosHeader)
    {
      bool isAssembly = false;
      PE_ARCHITECTURE arch = GetImageArchitecture(fileData, isAssembly);

      HeapFree(GetProcessHeap(), 0, fileData);
      CloseHandle(file);

      if (isAssembly && (arch == PE_ARCHITECTURE_ANYCPU || arch == PE_ARCHITECTURE_x64))
        return true;
    }
    return false;
  }
#elif defined(Q_OS_WIN32) && !defined(Q_OS_WIN64) && defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER)
  QString qsPath = getIApp()->toQString(sPath);
  QFile file(qsPath);
  if (!file.exists() || !file.open(QIODevice::ReadOnly))
    return false;

  // http://www.codeguru.com/forum/showthread.php?t=424454
  bool isAssembly = false;
  QByteArray data = file.read(2048),
             dataMarker("BSJB");
  for (int offset = 0; !data.isEmpty(); offset += data.size(), data = file.read(2048))
  {
    if (!offset)
    {
      // http://www.grimes.demon.co.uk/workshops/fusionWSCrackThree.htm
      // http://stackoverflow.com/questions/1366503/best-way-to-check-if-a-dll-is-an-clr-assembly-in-c
      // DOS header starts at 0x0, the DWORD at 0x3c contains a pointer to the PE signature 
      // (usually 0x80) which is 4 bytes, the next 20 bytes is the COFF header and then there
      // is the PE header (at 0x9. The PE header is 224 bytes and contains the data directory
      // (at 96 bytes into the PE header = 0xf. The 15th entry (at 0x16 is the CLR header
      // descriptor (sometimes called the COM descriptor, but this does not have anything to
      // do with COM). If this is empty (ie 0 in the 8 bytes from 0x168 to 0x16f) then the file
      // is not a .NET assembly. If you want to check if it is a COM DLL then you should look to
      // see if it exports GetClassObject.
      // via .Net : Use Module.GetPEKind method and check the PortableExecutableKinds Enumeration:
      // (ILOnly , Unmanaged32Bit The executable contains pure unmanaged code
      //
      OdUInt32 offsetPE = 0; // offset to Portable Executable Header
      if (data.size() > (0x3c + sizeof(OdUInt32)))
      {
        //offset to PE header is always at 0x3C
        offsetPE = *reinterpret_cast<OdUInt32*>(&data.data()[0x3c]);
        if (pnTypeMachine)
        {
          OdUInt32 headPE = 0;
          if (data.size() > int(offsetPE + 224))
          {
            headPE = *reinterpret_cast<OdUInt32*>(&data.data()[offsetPE]);
            //PE header starts with "PE\0\0" =  0x50 0x45 0x00 0x00   little-endian 
            if (headPE == 0x00004550)
              //followed by 2-byte machine type field (see document above for enum)
              *pnTypeMachine = *reinterpret_cast<OdUInt16*>(&data.data()[offsetPE + 4]);
            else
              *pnTypeMachine = 0; //IMAGE_FILE_MACHINE_UNKNOWN
          }
        }

        // COM header descriptor
        // 0xe8 = 0x168 - 0x80 = 224 + 8
        QByteArray dataClrDirectory = data.mid(0xe8 + offsetPE, 8); 
        ODA_ASSERT_ONCE(dataClrDirectory.size() == 8); // test
        
        static const char empty8[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        isAssembly = (dataClrDirectory != QByteArray::fromRawData(empty8, 8)); // attention QByteArray("\0", 8) is not same

        if (!psVersion)
          return isAssembly;
      }

      ////////////////////////
    }

    int pos = data.indexOf(dataMarker);
    if (pos < 0)
      continue;
    offset += pos;
    // TD_Mgd.dll : //ODA_ASSERT_ONCE(offset == 0x27e0); // test

    QString qsVersion(data.mid(pos + 0x10, 12));
    if (   qsVersion.indexOf("v")
        || (qsVersion = qsVersion.mid(1)).isEmpty())
    {
      ODA_FAIL_ONCE(); // test
      continue;
    }

    if (psVersion)
      *psVersion = getIApp()->toOdString(qsVersion);

    ODA_ASSERT_ONCE(isAssembly);
    return true;
  }
  ODA_ASSERT_ONCE(!isAssembly);

 #endif

  return false;
}

bool ExAppServicesImpl::isNetAssembly(OdString& sPath, // in / out(normalized as side effect)
                                      OdString* psVersion) // = NULL
{
  return isManagedDll(sPath, psVersion, NULL);
}

///////////////////////////////////////////////////////////////////////////////
static bool s_bExAppServicesInitialized = false;

void initExAppServices()
{
  static OdStaticRxObject<ExAppServicesImpl> s_ExAppServices;
  if (s_bExAppServicesInitialized)
    return;
  s_bExAppServicesInitialized = true;

 #if defined(ODA_EX_APP_SERVICES_UNLINKED_RXINIT)
  ExAppServices::rxInit();
 #endif
  ExAppServicesImpl::rxInit();
  OdDbBaseHostAppServices::desc()->addX(ExAppServices::desc(), &s_ExAppServices);
  ::odrxSysRegistry()->putAt(EX_APP_SERVICES, &s_ExAppServices);
}

void uninitExAppServices()
{
  if (!s_bExAppServicesInitialized) 
    return;
  s_bExAppServicesInitialized = false;

  ::odrxSysRegistry()->remove(EX_APP_SERVICES);
  OdDbBaseHostAppServices::desc()->delX(ExAppServices::desc());
  ExAppServicesImpl::rxUninit();
 #if defined(ODA_EX_APP_SERVICES_UNLINKED_RXINIT)
  ExAppServices::rxUninit();
 #endif
}
