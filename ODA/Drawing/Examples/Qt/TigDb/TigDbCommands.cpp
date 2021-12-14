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
// TigDbCommands.cpp
//

#include "OdaCommon.h"

#include "OdqInterfaces.h"
#include <QMainWindow>
#include <QDockWidget>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFile>

#include "TigDbModuleImpl.h"
#include "OdqRevSystem.h"
#include "RevisionController.h"

void OdqCmd_UiRevisionShow::execute(OdEdCommandContext*) // pCmdCtx
{
  IqRevSystem* iRS = getIRevSystem();
  ODA_ASSERT_ONCE(iRS);
  if (!iRS || iRS->isRevVisible())
    return;

  OdqRevSystem::getDockPalette()->setVisible(true);

  // maybe non-updated as invisible
  OdqRevSystem::getRevSystem().refresh("<event-refresh:revision_changed>", "");
}

void OdqCmd_UiRevisionHide::execute(OdEdCommandContext*) // pCmdCtx
{
  IqRevSystem* iRS = getIRevSystem();
  ODA_ASSERT_ONCE(iRS);
  if (!iRS || !iRS->isRevVisible())
    return;

  OdqRevSystem::getDockPalette()->setVisible(false);
}

void OdqCmd_UIRevisionsRefresh::execute(OdEdCommandContext*) // pCmdCtx
{
  IqRevSystem* iRS = getIRevSystem();
  ODA_ASSERT_ONCE(iRS);
  if (!iRS)
    return;

  if (!iRS->isRevVisible())
    OdqRevSystem::getDockPalette()->setVisible(true);

  OdqRevSystem::getRevSystem().refresh("<event-refresh:revision_changed>", "");
}

static OdqRevTab* getRevTab(OdRxObject* pRxDb)
{
  OdqRevSystem* pRS = (OdqRevSystem*)getIRevSystem();
  ODA_ASSERT_ONCE(pRS);
  if (!pRS)
    return NULL;

  return pRS->getRevTab(pRxDb);
}

static OdqRevisionController* controller()
{
  OdqRevSystem* pRS = (OdqRevSystem*)getIRevSystem();
  ODA_ASSERT_ONCE(pRS);
  return pRS->controller();
}

static void commitBranch(OdEdCommandContext* pCmdCtx, const OdString sTitle, 
                         OdCommitDescription* pInfoFrom = NULL,
                         const OdString sDescription = OdString::kEmpty)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  if (!pRxDb)
    throw OdError(eNoDatabase);
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdString sAuthor,
           sMessage;
  if (getIConsole()->isInputStringReady(pCmdCtx))
  {
    sAuthor = pIO->getString(L"author"),
    sMessage = pIO->getString(L"message");
  }
  else
  {
    QDialog dlg(getIApp()->getMainWindow());
    getIApp()->restoreWidgetPlacement(&dlg, "dlgcommit", 500, 200);
    
    dlg.setWindowTitle(getIApp()->toQString(sTitle));
    QGridLayout* pLayout = new QGridLayout();
    dlg.setLayout(pLayout);

    QLabel* pAuthorLabel = new QLabel(QObject::tr("Author:"));
    pLayout->addWidget(pAuthorLabel, 0, 0);
    QLineEdit* pAuthor = new QLineEdit();
    //pAuthor->setText("");
    pLayout->addWidget(pAuthor, 0, 1);
    pAuthorLabel->setBuddy(pAuthor);

    QLabel* pDescriptionLabel = new QLabel(QObject::tr("Description:"));
    pLayout->addWidget(pDescriptionLabel, 1, 0);
    QLineEdit* pDescription = new QLineEdit();
    if (!sDescription.isEmpty())
      pDescription->setText(getIApp()->toQString(sDescription));
    pLayout->addWidget(pDescription, 1, 1);
    pDescriptionLabel->setBuddy(pDescription);

    QDialogButtonBox* pButtonBox = new QDialogButtonBox();
    pButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    if (   !QObject::connect(pButtonBox, SIGNAL(accepted()), &dlg, SLOT(accept()), Qt::QueuedConnection)
        || !QObject::connect(pButtonBox, SIGNAL(rejected()), &dlg, SLOT(reject()), Qt::QueuedConnection))
    {
      ODA_FAIL_ONCE();
    }
    pLayout->addWidget(pButtonBox, 2, 1);

    int res = dlg.exec();
    getIApp()->saveWidgetPlacement(&dlg, "dlgcommit");
    if (res != QDialog::Accepted)
      return;
    sAuthor = getIApp()->toOdString(pAuthor->text());
    sMessage = getIApp()->toOdString(pDescription->text());
  }

  if (sMessage.isEmpty() || sAuthor.isEmpty())
    throw OdError(L"Wrong commit msg");

  OdString sError;
  if (controller()->commit(pRxDb, sAuthor, sMessage, pInfoFrom, &sError))
    OdqRevSystem::getRevSystem().refresh("<event-refresh:revision_changed>", "false");
  else if (!sError.isEmpty())
    getIConsole()->putString(sError);
}

void OdqCmd_Commit::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  if (!pRxDb)
    throw OdError(eNoDatabase);

  OdString sCurrentBranch = controller()->currentBranchName(pRxDb);
  if (sCurrentBranch.isEmpty())
    throw OdError("Database is not under revision control");

  commitBranch(pCmdCtx, OdString().format(L"Commit to branch \"%ls\"", sCurrentBranch.c_str()));
}

void OdqCmd_CreateBranch::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  if (!pRxDb)
    throw OdError(eNoDatabase);
  OdEdUserIO* pIO = pCmdCtx->userIO();
  
  OdString sCurrentBranch = controller()->currentBranchName(pRxDb);
  if (sCurrentBranch.isEmpty())
    throw OdError("Database is not under revision control");

  OdString sBranch;
  if (getIConsole()->isInputStringReady(pCmdCtx))
    sBranch = pIO->getString("Branch name to switch to");
  else
  {
    QDialog dlg(getIApp()->getMainWindow());
    getIApp()->restoreWidgetPlacement(&dlg, "dlgcreatebranch", 500, 200);
    
    dlg.setWindowTitle("Create new branch");
    QGridLayout* pLayout = new QGridLayout();
    dlg.setLayout(pLayout);

    QLabel* pBranchLabel = new QLabel(QObject::tr("Branch name:"));
    pLayout->addWidget(pBranchLabel, 0, 0);
    QLineEdit* pName = new QLineEdit();
    //pName->setText("");
    pLayout->addWidget(pName, 0, 1);
    pBranchLabel->setBuddy(pName);

    QDialogButtonBox* pButtonBox = new QDialogButtonBox();
    pButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    if (   !QObject::connect(pButtonBox, SIGNAL(accepted()), &dlg, SLOT(accept()), Qt::QueuedConnection)
        || !QObject::connect(pButtonBox, SIGNAL(rejected()), &dlg, SLOT(reject()), Qt::QueuedConnection))
    {
      ODA_FAIL_ONCE();
    }
    pLayout->addWidget(pButtonBox, 1, 1);

    int res = dlg.exec();
    getIApp()->saveWidgetPlacement(&dlg, "dlgcreatebranch");
    if (res != QDialog::Accepted)
      return;
    sBranch = getIApp()->toOdString(pName->text());
  }
  
  if (sBranch.isEmpty())
    throw OdError("Branch name cannot be empty");
  if (sCurrentBranch == sBranch)
    return;

  OdString sError;
  if (controller()->createBranch(pRxDb, sBranch, getRevTab(pRxDb)->checkedOutRevision(), &sError))
  {
    //getRevTab(pRxDb)->setLastCreatedBranchName(sBranch);

    OdqRevSystem::getRevSystem().refresh("<event-refresh:revision_changed>", "false");
    pIO->putString(L"Branch was created");
  }
  else if (!sError.isEmpty())
    getIConsole()->putString(sError);
}

static QStringList getOtherBranches(OdRxObject* pRxDb, OdString& sCurrentBranch)
{
  if (!pRxDb)
    throw OdError(eNoDatabase);

  QStringList lstOthers;
  if ((sCurrentBranch = controller()->currentBranchName(pRxDb)).isEmpty())
    return lstOthers;

  //OdStringArray branches; controller()->loadBranches(pRxDb, branches, OdCommitDescriptionList());
  OdStringArray branches = getRevTab(pRxDb)->branchNames();
  OdUInt32 sz = branches.size();
  if (sz <= 1)
    return lstOthers;

  for (OdUInt32 idx = 0; idx < sz; idx++)
  {
    OdString sName = branches[idx];
    if (sName == sCurrentBranch)
      continue;
    lstOthers << getIApp()->toQString(sName);
  }
  ODA_ASSERT_ONCE(lstOthers.size() >= 1);

  return lstOthers;
}

void OdqCmd_Merge::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  OdEdUserIO* pIO = pCmdCtx->userIO();
  
  OdString sCurrentBranch;
  QStringList lstOthers = getOtherBranches(pRxDb, sCurrentBranch);
  if (sCurrentBranch.isEmpty())
    throw OdError("Database is not under revision control");
  if (!lstOthers.size())
  {
    pIO->putString("There is no other branches to merge.");
    return;
  }

  //static bool s_bNoFastForward = false; // = true;
  //bool bNoFastForward = s_bNoFastForward;
  OdString sBranch;

  if (getIConsole()->isInputStringReady(pCmdCtx))
  {
    OdString sFirst = getIApp()->toOdString(lstOthers[0]);
    sBranch = pIO->getString(OdString().format(L"Merge from<%ls>", sFirst.c_str()), OdEd::kInpDefault, sFirst);
  }
  else
  {
    QDialog dlg(getIApp()->getMainWindow());
    getIApp()->restoreWidgetPlacement(&dlg, "dlgmerge", 500, 200);
    
    QString qsCurrentBranch = getIApp()->toQString(sCurrentBranch);
    dlg.setWindowTitle(QString("Merge into \"%1\"").arg(qsCurrentBranch));
    QGridLayout* pLayout = new QGridLayout();
    dlg.setLayout(pLayout);

    QLabel* pBranchLabel = new QLabel(QObject::tr("Merge from branch:"));
    pLayout->addWidget(pBranchLabel, 0, 0);
    QComboBox* pBranchCombo = new QComboBox();
    pBranchCombo->addItems(lstOthers);
    pBranchCombo->setCurrentIndex(0);
    pLayout->addWidget(pBranchCombo, 0, 1);
    pBranchLabel->setBuddy(pBranchCombo);

    //QLabel* pCheckLabel = new QLabel(QObject::tr("No fast forward:"));
    //pLayout->addWidget(pCheckLabel, 1, 0);
    //QCheckBox* pNoFastForward = new QCheckBox();
    //pNoFastForward->setCheckState(bNoFastForward ? Qt::Checked : Qt::Unchecked);
    //pLayout->addWidget(pNoFastForward, 1, 1);
    //pCheckLabel->setBuddy(pNoFastForward);

    QDialogButtonBox* pButtonBox = new QDialogButtonBox();
    pButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    if (   !QObject::connect(pButtonBox, SIGNAL(accepted()), &dlg, SLOT(accept()), Qt::QueuedConnection)
        || !QObject::connect(pButtonBox, SIGNAL(rejected()), &dlg, SLOT(reject()), Qt::QueuedConnection))
    {
      ODA_FAIL_ONCE();
    }
    //pLayout->addWidget(pButtonBox, 2, 1);
    pLayout->addWidget(pButtonBox, 1, 1);

    int res = dlg.exec();
    getIApp()->saveWidgetPlacement(&dlg, "dlgmerge");
    //s_bNoFastForward = bNoFastForward = pNoFastForward->checkState() == Qt::Checked;
    if (res != QDialog::Accepted)
      return;
    sBranch = getIApp()->toOdString(pBranchCombo->currentText());
    if (sBranch.isEmpty())
      throw OdError("Branch name cannot be empty");
  }

  OdString sError;
  //if (controller()->merge(pRxDb, sBranch, bNoFastForward, &sError))
  if (controller()->merge(pRxDb, sBranch, false, &sError))
  {
    OdCommitDescription* pInfoFrom = getRevTab(pRxDb)->topRevisionpOf(sBranch);
    OdString sDescr;
    sDescr.format(L"Merge branch \"%ls\" into \"%ls\"", sBranch.c_str(), sCurrentBranch.c_str());
    commitBranch(pCmdCtx, L"Merge commit", pInfoFrom, sDescr);
    pIO->putString(L"Merge commit was created");
    //getRevTab(pRxDb)->setLastCreatedBranchName("");
    OdqRevSystem::getRevSystem().refresh("<event-refresh:revision_changed>", "false");
  }
  else if (!sError.isEmpty())
    getIConsole()->putString(sError);
  else
    getIConsole()->putString(L"Merge was fast forward (no merge commit necessary).");
}

void OdqCmd_Switch::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdString sCurrentBranch;
  QStringList lstOthers = getOtherBranches(pRxDb, sCurrentBranch);
  if (sCurrentBranch.isEmpty())
    throw OdError("Database is not under revision control");
  if (!lstOthers.size())
  {
    pIO->putString("There is nothing to switch");
    return;
  }

  OdString sBranch;
  if (getIConsole()->isInputStringReady(pCmdCtx))
  {
    OdString sFirst = getIApp()->toOdString(lstOthers[0]);
    sBranch = pIO->getString(OdString().format(L"Switch to branch<%ls>:", sFirst.c_str()), OdEd::kInpDefault, sFirst);
  }
  else
  {
    QDialog dlg(getIApp()->getMainWindow());
    getIApp()->restoreWidgetPlacement(&dlg, "dlgswitch", 500, 200);

    dlg.setWindowTitle("Switch to branch");
    QGridLayout* pLayout = new QGridLayout();
    dlg.setLayout(pLayout);

    QLabel* pBranchLabel = new QLabel(QObject::tr("Switch into:"));
    pLayout->addWidget(pBranchLabel, 0, 0);

    QComboBox* pBranchCombo = new QComboBox();
    pBranchCombo->addItems(lstOthers);
    pBranchCombo->setCurrentIndex(0);
    pLayout->addWidget(pBranchCombo, 0, 1);
    pBranchLabel->setBuddy(pBranchCombo);

    QDialogButtonBox* pButtonBox = new QDialogButtonBox();
    pButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    if (!QObject::connect(pButtonBox, SIGNAL(accepted()), &dlg, SLOT(accept()), Qt::QueuedConnection)
      || !QObject::connect(pButtonBox, SIGNAL(rejected()), &dlg, SLOT(reject()), Qt::QueuedConnection))
    {
      ODA_FAIL_ONCE();
    }
    pLayout->addWidget(pButtonBox, 1, 1);

    int res = dlg.exec();
    getIApp()->saveWidgetPlacement(&dlg, "dlgswitch");
    if (res != QDialog::Accepted)
      return;
    sBranch = getIApp()->toOdString(pBranchCombo->currentText());
    if (sBranch.isEmpty())
      throw OdError("Branch name cannot be empty");
  }

  OdString sError;
  if (controller()->switchToBranch(pRxDb, sBranch, &sError))
    OdqRevSystem::getRevSystem().refresh("<event-refresh:revision_changed>", "false");
  else if (!sError.isEmpty())
    pIO->putString(sError);
}

void OdqCmd_SaveAsDsf::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  if (!pRxDb)
    throw OdError(eNoDatabase);
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdString sFileName = getIConsole()->getFilePathName(pRxDb);
  int pos = sFileName.reverseFind(L'.');
  if (pos > 0)
    sFileName = sFileName.left(++pos) + L"dsf";
  sFileName = pIO->getFilePath(L"Save as DSF", OdEd::kGfpForSave, L"Save as DSF",
    L"dsf", sFileName, "Drawings Stream Format files (*.dsf)|*.dsf");
  if (sFileName.isEmpty())
    return;
  if ((pos = sFileName.find(L".dsf")) < 0)
    sFileName += L".dsf";

  QFile info(getIApp()->toQString(sFileName));
  if (info.exists())
  {
    if (!info.remove())
    {
      pIO->putString(OdString().format(L"Error: Failed to delete old version of \"%ls\".", sFileName.c_str()));
      return;
    }
  }

  //if (::odrxGetSchemaManager()->getSchemaPath().isEmpty())
  //{
  //  bool usePrefixes = getIAppProps()->toInt(L"Vars/tigprefix", false);
  //  controller()->setPrefixDelimiter(usePrefixes ? L'\\' : L'\0');
  //}

  OdString sError;
  if (controller()->import(pRxDb, sFileName, &sError))
  {
    IqViewSystem* iVS = getIViewSystem();
    if (iVS)
      iVS->closeViews(pRxDb);

    QString qsCmd = QString("open \"%1\"").arg(getIApp()->toQString(sFileName));
    getIConsole()->postponeCommand(qsCmd);
  }
  else if (!sError.isEmpty())
    pIO->putString(sError);
}

void OdqCmd_SaveAsTig::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  if (!pRxDb)
    throw OdError(eNoDatabase);
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdString sFileName = getIConsole()->getFilePathName(pRxDb);
  int pos = sFileName.reverseFind(L'.');
  if (pos > 0)
    sFileName = sFileName.left(++pos) + L"tig";
  sFileName = pIO->getFilePath(L"Save as TIG", OdEd::kGfpForSave, L"Save as TIG",
                               L"tig", sFileName, "ODA SDK Format files (*.tig)|*.tig");
  if (sFileName.isEmpty())
    return;
  if ((pos = sFileName.find(L".tig")) < 0)
    sFileName += L".tig";

  QFile info(getIApp()->toQString(sFileName));
  if (info.exists())
  {
    if (!info.remove())
    {
      pIO->putString(OdString().format(L"Error: Failed to delete old version of \"%ls\".", sFileName.c_str()));
      return;
    }
  }

  //if (::odrxGetSchemaManager()->getSchemaPath().isEmpty())
  //{
  //  bool usePrefixes = getIAppProps()->toInt(L"Vars/tigprefix", false);
  //  controller()->setPrefixDelimiter(usePrefixes ? L'\\' : L'\0');
  //}

  OdString sError;
  if (controller()->import(pRxDb, sFileName, &sError))
  {
    IqViewSystem* iVS = getIViewSystem();
    if (iVS)
      iVS->closeViews(pRxDb);

    QString qsCmd = QString("open \"%1\"").arg(getIApp()->toQString(sFileName));
    getIConsole()->postponeCommand(qsCmd);
  }
  else if (!sError.isEmpty())
    pIO->putString(sError);
}

void OdqCmd_ExportToDwg::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  if (!pRxDb)
    throw OdError(eNoDatabase);
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdString sFileName = getIConsole()->getFilePathName(pRxDb);
  sFileName.replace(L".tig", L"");
  sFileName.replace(L".dsf", L"");
  int pos;
  if ((pos  = sFileName.find(L".dwg")) < 0)
    sFileName += L".dwg";
  sFileName = pIO->getFilePath(L"Export to DWG", OdEd::kGfpForSave, L"Export to DWG",
                               L"dwg", sFileName, "DWG files (*.dwg)|*.dwg");
  if (sFileName.isEmpty())
    return;
  if ((pos = sFileName.find(L".dwg")) < 0)
    sFileName += L".dwg";

  QFile info(getIApp()->toQString(sFileName));
  if (info.exists())
  {
    if (!info.remove())
    {
      pIO->putString(OdString().format(L"Error: Failed to delete old version of \"%ls\".", sFileName.c_str()));
      return;
    }
  }

  OdString sError;
  if (!controller()->exportToDwg(pRxDb, sFileName, OdDb::kDHL_MC0_0, &sError) && !sError.isEmpty())
    pIO->putString(sError);
}

//////////////////////////////////////////////////////////////////////////
