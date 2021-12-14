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
// Services.h
//
// Combines the qt dependent functionality of
// ExSystemServices and ExHostAppServices
//

#ifndef  ODQ_SERVICES_H_
#define ODQ_SERVICES_H_

#include "OdaCommon.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"

class OdqServices : public ExSystemServices
                  , public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);

public:
  OdqServices();
	~OdqServices();
  void init();
  void uninit();

  OdStringArray resetRegVariables(); // Restore default values of registry variables
  
  OdStringArray getExtDbModuleNames(OdRxObject* pRxDb = NULL,
                                    bool bNamesOnly = true);
  void updateExtDbModuleNames(OdRxModule* pModule);

  // is needed to add path for findFile set
  bool importFromFile(OdExtDbModule* pExtDb, OdEdCommandContext* pCmdCtx,
                      const OdString& sFileName, ODCOLORREF& clrBackground);
private:
  // overloads :
  virtual OdRxModule* loadModule(const OdString& moduleFileName, bool silent);
  virtual void setLimit(int max);
  virtual void start(const OdString& displayString = OdString::kEmpty);
  virtual void stop();
  virtual void meterProgress();
  virtual void warning(const char* warnVisGroup, const OdString& sMsg);

  virtual OdDbDatabasePtr readFile(const OdString& sFileName,
                                         bool allowCPConversion = false,
                                         bool partialLoad = false,
                                         Oda::FileShareMode shareMode = Oda::kShareDenyNo,
                                         const OdPassword& password = OdPassword());

  virtual OdString findFile(const OdString& filename,
                            OdDbBaseDatabase* pDb = 0,
                            FindFileHint hint = kDefault);

  virtual OdString fileDialog(int flags,
                              const OdString& sDialogCaption = OdString::kEmpty,
                              const OdString& sDefExt = OdString::kEmpty,
                              const OdString& sDefFilename = OdString::kEmpty,
                              const OdString& sFilter = OdString::kEmpty);

  virtual OdGsDevicePtr gsBitmapDevice(OdRxObject* pViewObj = NULL,
                                       OdDbBaseDatabase* pDb = NULL,
                                       OdUInt32 flags = 0);

  virtual bool getPassword(const OdString& sDwgPathName,
                           bool isXref,
                           OdPassword& password);

  ///** { Secret } */
  // TODO //virtual OdPwdCachePtr getPasswordCache();

  virtual OdDbUndoControllerPtr newUndoController();

  virtual void auditPrintReport(OdAuditInfo* pAuditInfo, 
                                const OdString& sLine, 
                                int nPrintDest) const;

  virtual OdDbPlotSettingsValidator* plotSettingsValidator();

  virtual bool ttfFileNameByDescriptor(const OdTtfDescriptor& description,
                                       OdString& sFileName);

  //virtual OdCodePageId systemCodePage() const;

  //////////////////////////////////////////////////////////////////////////

  bool m_bInitialized;
  OdStringArray m_namesExtDb,
                m_pathsExtDb;
  //QString m_qsError;
};

#endif // ODQ_SERVICES_H_
