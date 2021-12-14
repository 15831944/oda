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
// PropSystemCommands.cpp
//

#include <QDockWidget>
#include <QTabWidget>
#include <QTabBar>

#include "OdaCommon.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"

#include "OdqPropSystem.h"
#include "PropServices.h"
#include "PropSourcesImpl.h"
#include "PropSystemModule.h"

#include "DbDatabase.h"
#include "DbAssocManager.h"

//////////////////////////////////////////////////////////////////////////

void OdqCmd_UiPropertyShow::execute(OdEdCommandContext*) // pCmdCtx
{
  IqPropSystem* iPS = getIPropSystem();
  ODA_ASSERT_ONCE(iPS);
  if (!iPS || iPS->isPropVisible())
    return;

  OdqPropSystem::getDockPalette()->setVisible(true);
  
  // maybe non-updated as invisible
  OdqPropSystem::getPropSystem().refresh("<event-refresh:selection_changed>", ""); 
}

void OdqCmd_UiPropertyHide::execute(OdEdCommandContext*) // pCmdCtx
{
  IqPropSystem* iPS = getIPropSystem();
  ODA_ASSERT_ONCE(iPS);
  if (!iPS || !iPS->isPropVisible())
    return;

  OdqPropSystem::getDockPalette()->setVisible(false);
}

// internal - is used created undo with set value vis property palette
void OdqCmd_SetFilteredSelectionPropValue::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  OdqPropTab* pTab = OdqPropSystem::getPropTab(pRxDb);
  ODA_ASSERT_ONCE(pTab);
  if (!pTab)
    return;
  ObjectPropSourceImpl* pObjectPropSource = pTab->getObjectSource();
  ODA_ASSERT_ONCE(pObjectPropSource);
  if (!pObjectPropSource)
    return;

  OdString sPath,
           sValue;
  try 
  {
    sPath = pCmdCtx->userIO()->getString(OD_T("Enter property path"), OdEd::kInpThrowEmpty);
  }
  catch (const OdEdEmptyInput&)
  {
  }
  try 
  {
    sValue = pCmdCtx->userIO()->getString(OD_T("Enter property value"),  OdEd::kGstAllowSpaces | OdEd::kInpThrowEmpty);
    if (sValue.find(L'&') >= 0)
      sValue = OdPropContext::decodeFromXmlAttrib(sValue); // CORE-13627
  }
  catch (const OdEdEmptyInput&)
  {
  }
  OdPropSource::Action action = OdPropSource::enAction_None;
  if (   !pObjectPropSource->setPropertyValueDirect(sPath, sValue, action)
      || GETBIT(action, OdPropSource::enAction_Cancel))
    throw OdEdCancel(); // restore via undo

  if (GETBIT(action, OdPropSource::enAction_evaluateTopLevelNetwork))
  {
    OdDbDatabase* pDb = OdDbDatabase::cast(pRxDb).get();
    ODA_ASSERT_ONCE(pDb);
    if (pDb && ::odrxDynamicLinker()->getModule(DbConstraintsModuleName).get())
      OdDbAssocManager::evaluateTopLevelNetwork(pDb);
  }

  bool bRecreateWidgets = true; 
  // TODO
  //bForceToWidget = bool(action & 4); // OdPropSource::enAction_RecreateWidgets
  pTab->update(pObjectPropSource, bRecreateWidgets);
  getIApp()->refreshQObjetcts("<event-refresh:setprop_ended>");
  IqViewSystem* iVS = getIViewSystem();
  if (iVS)
    iVS->updateViews(pCmdCtx->baseDatabase());
}

void OdqCmd_Options::execute(OdEdCommandContext* pCmdCtx)
{
  OdPropServicesPtr pPropServices = OdPropServices::cast(pCmdCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());

  bool bPaletteAsContent = true; // TODO = false;
  OdString sDlgKey = pPropServices->openDialog(L"%ls Options", NULL,
                                               true, // via "<event-refresh:setprop_ended>"
                                               bPaletteAsContent ? OdPropServices::enSepCatPlace_None 
                                                                 : OdPropServices::enSepCatPlace_Left);
  ODA_ASSERT_ONCE(!sDlgKey.isEmpty());
  if (sDlgKey.isEmpty())
    return;
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  if (!pRxDb)
    throw OdError(eNoDatabase); // TODO for AppSettings

  //m_pToolSource = new ToolPropSourceImpl(m_pCtx);
  //pPropServices->addSource(m_sDlgKey, m_pToolSource, 
  //                         OD_T("Tools"), OD_T("Properties"),
  //                         OD_T("alignment=top"));
  //                       //OD_T("alignment=top:title="));
  
  int flags = getIAppProps()->toInt(L"Vars/optpropsrc", 4);
  OdSelectionSetPtr pIds = ToolPropSourceImpl::createSelectionSet(pRxDb);
  if (pIds.isNull())
    throw OdError(eNotApplicable);
  pIds->append((OdDbStub*)NULL);
  OdSharedPtr<OdPropSource> pObjectSource = new ObjectPropSourceImpl(pCmdCtx, flags, pIds);
  const OdChar* pcszWidgetAttribs = bPaletteAsContent ? OD_T("type=palette") : OD_T("type=dialog"); 
  pPropServices->addSource(sDlgKey, pObjectSource,
                           L"Merged", L"Properties", pcszWidgetAttribs);
  //if (pToolSource.get())
  //  pToolSource->setEntitySource(m_sDlgKey, m_pEntitySource.get()); // is used for setMergeFilter
  pPropServices->setMergeFilter(sDlgKey, pObjectSource, 
                                OdString::kEmpty, // empty for all
                                L"Filter", L"Entity");

  OdqPropTab* pTab = OdqPropSystem::getPropTab(pRxDb);
  if (pTab)
    pTab->setModalDialogObjectSourceToSet((ObjectPropSourceImpl*) pObjectSource.get());

  pPropServices->executeDialog(sDlgKey);
  pPropServices->closeDialog(sDlgKey);

  if (pTab)
    pTab->setModalDialogObjectSourceToSet(NULL);
}

void OdqCmd_PropPal::execute(OdEdCommandContext* pCmdCtx)
{
  OdPropServicesPtr pPropServices = OdPropServices::cast(pCmdCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());

  OdString sDlgKey = pPropServices->openDialog(L"%ls Properties", NULL,
                                               true); // via "<event-refresh:setprop_ended>"
  ODA_ASSERT_ONCE(!sDlgKey.isEmpty());
  if (sDlgKey.isEmpty())
    return;

  OdSharedPtr<ToolPropSourceImpl> pToolSource = new ToolPropSourceImpl(pCmdCtx);
  pPropServices->addSource(sDlgKey, pToolSource, 
                           L"Tools", L"Properties", OD_T("alignment=top")); // OD_T(L"alignment=top:title="));

  int flags = getIAppProps()->toInt(L"Vars/dlgpropsrc", 4);
  OdSharedPtr<EntityPropSourceImpl> pEntitySource = new EntityPropSourceImpl(pCmdCtx, flags, pToolSource);
  const OdChar* pcszWidgetAttribs = OD_T("type=palette"); // = L"type=palette_simple";
  pPropServices->addSource(sDlgKey, pEntitySource,
                           L"Merged", L"Properties", pcszWidgetAttribs);
  pToolSource->setEntitySource(sDlgKey, pEntitySource.get()); // is used for setMergeFilter
  pPropServices->setMergeFilter(sDlgKey, pEntitySource.get(), 
                                OdString::kEmpty, // empty for all
                                L"Filter", L"Entity");

  OdqPropTab* pTab = OdqPropSystem::getPropTab(pCmdCtx->baseDatabase());
  if (pTab)
    pTab->setModalDialogObjectSourceToSet((ObjectPropSourceImpl*) pEntitySource.get());

  pPropServices->executeDialog(sDlgKey);
  pPropServices->closeDialog(sDlgKey);

  if (pTab)
    pTab->setModalDialogObjectSourceToSet(NULL);
}

void OdqCmd_PropDlg::execute(OdEdCommandContext* pCmdCtx)
{
  OdPropServicesPtr pPropServices = OdPropServices::cast(pCmdCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());

  OdString sDlgKey = pPropServices->openDialog(L"%ls Properties", NULL, 
                                               true, // via "<event-refresh:setprop_ended>"
                                               OdPropServices::enSepCatPlace_Left);
  ODA_ASSERT_ONCE(!sDlgKey.isEmpty());
  if (sDlgKey.isEmpty())
    return;

  //OdSharedPtr<ToolPropSourceImpl> pToolSource;
  //pToolSource = new ToolPropSourceImpl(pCmdCtx);
  //pPropServices->addSource(sDlgKey, pToolSource, 
  //                         L"Tools", L"Properties", OD_T("alignment=top")); // OD_T(L"alignment=top:title="));

  int flags = getIAppProps()->toInt(L"Vars/dlgpropsrc", 4);
  OdSharedPtr<EntityPropSourceImpl> pEntitySource = new EntityPropSourceImpl(pCmdCtx, flags); // , pToolSource);

  const OdChar* pcszWidgetAttribs = OD_T("type=dialog");
  pPropServices->addSource(sDlgKey, pEntitySource,
                           L"Merged", L"Properties", pcszWidgetAttribs);
  //if (pToolSource.get())
  //  pToolSource->setEntitySource(sDlgKey, pEntitySource.get()); // is used for setMergeFilter
  pPropServices->setMergeFilter(sDlgKey, pEntitySource.get(), 
                                OdString::kEmpty, // empty for all
                                L"Filter", L"Entity");

  OdqPropTab* pTab = OdqPropSystem::getPropTab(pCmdCtx->baseDatabase());
  if (pTab)
    pTab->setModalDialogObjectSourceToSet((ObjectPropSourceImpl*) pEntitySource.get());

  pPropServices->executeDialog(sDlgKey);
  pPropServices->closeDialog(sDlgKey);

  if (pTab)
    pTab->setModalDialogObjectSourceToSet(NULL);
}

void OdqCmd_Cda::execute(OdEdCommandContext* pCmdCtx)
{
  OdEdUserIO* pIO = pCmdCtx->userIO();
  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  if (!pRxDb)
  {
    pIO->putString(L"  use next variables to attune properties:\n"
                   L"    for command   property: [3]\n"
                   L"      propsrcr\n"
                   L"        (16 - Show CAD Weight / 8 - Skip None Cat / 4 - CDA / 2 - RegistredXmlSrc / 1 - XmlPattern)\n"
                   L"    for command   proppal [12]\n"
                   L"      dlgpropsrc\n"
                   L"       (16 - Show CAD Weight / 8 - Skip None Cat / 4 - CDA / 2 - RegistredXmlSrc / 1 - XmlPattern)\n"
                   L"    for command   options [4]\n"
                   L"      optpropsrc\n"
                   L"        (16 - Show CAD Weight / 8 - Skip None Cat / 4 - CDA)\n");
    throw OdError(eNoDatabase);
  }
  ODA_ASSERT_ONCE(module());  // or via ::odrxDynamicLinker()->loadModule(OD_T("OdaQtPropSystem.dll")).get()

  OdSelectionSet* pSSet = getIConsole()->workingSelectionSet(pRxDb);
  if (pSSet && pSSet->numEntities())
    static_cast<OdqPropSystemModule*>(const_cast<OdRxModule*>(module()))->m_cmd_PropPal.execute(pCmdCtx);
  else
    static_cast<OdqPropSystemModule*>(const_cast<OdRxModule*>(module()))->m_cmd_Options.execute(pCmdCtx);
}

#ifdef _DEBUG //QT_DEBUG
void OdqCmd_UiPropertyTab::execute(OdEdCommandContext* pCmdCtx)
{
  // change visibility and placement of tabbar
  QList<QTabBar*> widgets = OdqPropSystem::getDockPalette()->widget()->findChildren<QTabBar*>();
  if (widgets.count() != 1)
    throw OdError(eNotApplicable);

  QTabWidget* pTabWidget = qobject_cast<QTabWidget*>(OdqPropSystem::getDockPalette()->widget());
  ODA_ASSERT_ONCE(pTabWidget);

  OdString sPrompt,
           sKeywords;
  int indexDefault;

  bool bVisible = widgets.last()->isVisible();
  if (!bVisible)
  {
    sPrompt = OD_T("Enter console tab state [East/West/North/South/On/Off]<Off>");
    sKeywords = OD_T("East West North South On Off");
    indexDefault = 5;
  }
  else  
  {
    sPrompt += OD_T("Enter console tab state [East/West/North/South/Off]");
    sKeywords += OD_T("East West North South Off");

    QTabWidget::TabPosition pos = pTabWidget->tabPosition();
    switch (pos)
    {
    default:
      ODA_FAIL_ONCE();
    case QTabWidget::East:
      sPrompt += OD_T("<East>");
      indexDefault = 0;
      break;
    case QTabWidget::West:
      sPrompt += OD_T("<West>");
      indexDefault = 1;
      break;
    case QTabWidget::North:
      sPrompt += OD_T("<North>");
      indexDefault = 2;
      break;
    case QTabWidget::South:
      sPrompt += OD_T("<South>");
      indexDefault = 3;
      break;
    }
  }

  int indexKeyword = pCmdCtx->userIO()
                      ->getKeyword(sPrompt, sKeywords, indexDefault, OdEd::kGstNoEmpty);
                                   // OdEd::kInpDefault | OdEd::kGstNoEmpty);
  switch (indexKeyword)
  {
  case 0: // East
    pTabWidget->setTabPosition(QTabWidget::East);
    widgets.last()->setVisible(true);
    return;
  case 1: // West
    pTabWidget->setTabPosition(QTabWidget::West);
    widgets.last()->setVisible(true);
    return;
  case 2: // North
    pTabWidget->setTabPosition(QTabWidget::North);
    widgets.last()->setVisible(true);
    return;
  case 3: // South
    pTabWidget->setTabPosition(QTabWidget::South);
    widgets.last()->setVisible(true);
    return;
  case 5:
    if (!bVisible)
      return; // current
    break;
  }

  widgets.last()->setVisible(!bVisible);
}
#endif

///////////////////////////////////////////////////////////////////////////////
