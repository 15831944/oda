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
// convertor.h
//
// Combines the qt dependent functionality of
// ExSystemServices and ExHostAppServices
//

#ifndef  __TD_QT_CONVERTOR_H_
#define __TD_QT_CONVERTOR_H_

#include <QWidget>

QT_BEGIN_NAMESPACE
  class QString;
  class QDir;
  class QLineEdit;
  class QProgressBar;
QT_END_NAMESPACE

#include "OdaCommon.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"

//////////////////////////////////////////////////////////////////////////

OdString toOdString(const QString& qsStr);
QString toQString(const OdString& sStr);

//////////////////////////////////////////////////////////////////////////

class CConvertor : public ExSystemServices
                 , public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);

public:
  CConvertor();
	~CConvertor();
  void init();
  void uninit();

  bool isBusy();
  bool startConvertion(const QString& qsInputPath,
                       const QString& qsInputFileWildcard,
                       bool bRecurseFolders,
                       bool bAudit,
                       const QString& qsOutputPath,
                       OdDb::DwgVersion versionSaveAs,
                       OdDb::SaveType typeSaveAs,
                       QLineEdit* pStateBar = NULL,
                       QProgressBar* pProgressBar = NULL,
                       QProgressBar* pSpastic = NULL,
                       bool bSilent = false);
  void stopConvertion(bool bForce = false);

private:
  // overloads :
  virtual OdRxModule* loadModule(const OdString& moduleFileName, bool silent);
  virtual void start(const OdString& displayString = OdString::kEmpty);
  virtual void stop();
  virtual void meterProgress();
  virtual void setLimit(int max);
  virtual void warning(const char* warnVisGroup, const OdString& sMsg);

  void checkStop(int stepProgress = 0,
                 int stepSpasticProgress = 0);
  // part of checkStop
  void checkProgress(int stepProgress = 0,
                     int stepSpasticProgress = 0);

  void convertFile(const QString& qsInputFileName,
                   const QString& qsOutputFileName);

  //////////////////////////////////////////////////////////////////////////

  bool m_bInitialized,
       m_bBusy,
       m_bStopRequest,
       m_bSilent;

  QDir m_dirInput,
       m_dirOutput;
  bool m_bRecurseFolders,
       m_bAudit;
  OdDb::DwgVersion m_verSaveAs;
  OdDb::SaveType m_typeSaveAs;
  QPointer<QLineEdit> m_pStateBar;
  QPointer<QProgressBar> m_pProgressBar;
  QPointer<QProgressBar> m_pSpasticProgressBar;

  QString m_qsError;
};

#endif // __TD_QT_CONVERTOR_H_
