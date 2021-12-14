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
// OdqPropTab.cpp
//
#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif

#include "OdqPropTab.h"
#include "PropServices.h"

//////////////////////////////////////////////////////////////////////////

OdqPropTab::OdqPropTab(QWidget* pParent,
                       OdEdCommandContext* pCtx)
  : QWidget(pParent)
  , m_pCtx(pCtx)
  , m_pModalDialogObjectSourceToSet(NULL)
{
  init();
}


OdqPropTab::~OdqPropTab()
{
  uninit();
}

void OdqPropTab::init()
{
  if (isInitialized())
    return;

  //if (qApp->activeWindow())
  //  setMinimumWidth(qApp->activeWindow()->width() / 15);

  OdPropServices* pPropServices = propServices();
  if (!pPropServices)
    return;
  
  QLayout* pLayout = new QVBoxLayout();
  ODA_ASSERT_ONCE(!layout());
  setLayout(pLayout);
  void* pLayoutQObjectToAddDlg = (QObject*) pLayout;

  m_sDlgKey = pPropServices->openDialog(OD_T("Property Dialog"), 
                                        pLayoutQObjectToAddDlg);
  ODA_ASSERT_ONCE(!m_sDlgKey.isEmpty());
  if (m_sDlgKey.isEmpty())
    return;

  m_pToolSource = new ToolPropSourceImpl(m_pCtx);
  pPropServices->addSource(m_sDlgKey, m_pToolSource, 
                           OD_T("Tools"), OD_T("Properties"),
                           OD_T("alignment=top"));
                         //OD_T("alignment=top:title="));
  
  int flags = getIAppProps()->toInt(L"Vars/propsrc", 7);
  flags = overridePropSourceFlagsForPalette(getRxDatabase(), flags);
  m_pEntitySource = new EntityPropSourceImpl(m_pCtx, flags, m_pToolSource);
  pPropServices->addSource(m_sDlgKey, m_pEntitySource, 
                         //OD_T("Entity"),
                           OD_T("Merged"), 
                           OD_T("Properties"),
                           OD_T("type=palette")); // OD_T("type=palette_simple"));
  m_pToolSource->setEntitySource(m_sDlgKey, m_pEntitySource.get()); // is used for setMergeFilter
  pPropServices->setMergeFilter(m_sDlgKey, m_pEntitySource, 
                                OdString::kEmpty, // empty for all
                                OD_T("Filter"), OD_T("Entity"));
}

void OdqPropTab::uninit()
{
  if (!isInitialized())
    return;
  if (!m_pCtx)
    return;
  OdPropServices* pPropServices = propServices();
  if (!pPropServices)
    return;
  pPropServices->closeDialog(m_sDlgKey);
}

bool OdqPropTab::isInitialized() const
{
  bool bVal = !m_sDlgKey.isEmpty();
  return bVal;
}

OdRxObject* OdqPropTab::getRxDatabase()
{
  ODA_ASSERT_ONCE(m_pCtx);
  if (!m_pCtx)
    return NULL;
  return m_pCtx->baseDatabase();
}

OdPropServices* OdqPropTab::propServices() 
{
  OdPropServices* pPropServices = OdPropServices::cast(m_pCtx).get();
  ODA_ASSERT_ONCE(pPropServices);
  return pPropServices;
}

ObjectPropSourceImpl* OdqPropTab::getObjectSource()
{
  if (m_pModalDialogObjectSourceToSet)
    return m_pModalDialogObjectSourceToSet; // is temp override for command setprop only
  return m_pEntitySource.get();
}

void OdqPropTab::setModalDialogObjectSourceToSet(ObjectPropSourceImpl* pObjectSource)
{
  ODA_ASSERT_ONCE(   !m_pModalDialogObjectSourceToSet || !pObjectSource 
                  || m_pModalDialogObjectSourceToSet == pObjectSource);
  m_pModalDialogObjectSourceToSet = pObjectSource;
}

void OdqPropTab::update(OdPropSource* pSourceOnly, // = NULL(for all if changed)
                        bool bRecreateWidgets, // = true
                        bool bForce) // = false // true - to ignore unchanged state
{
  if (!getRxDatabase() || m_sDlgKey.isEmpty())
    return;
  if (pSourceOnly && pSourceOnly == m_pModalDialogObjectSourceToSet)
    // subtitute override to source of palette to update content
    // TODO update of dialog content via "<event-refresh:setprop_ended>"
    pSourceOnly = m_pEntitySource.get();

  bool bChanged = false;
  if (!pSourceOnly || pSourceOnly == m_pToolSource.get())
  {
    m_pToolSource->collectFilters(&bChanged);
    propServices()->refreshSources(m_sDlgKey, m_pToolSource, bRecreateWidgets);
  }

  if (bChanged || bForce || pSourceOnly == m_pEntitySource.get())
    propServices()->refreshSources(m_sDlgKey, m_pEntitySource, bRecreateWidgets);
}

// for color palette via ExtDb
ODGSPALETTE& OdqPropTab::cacheColors() const
{
  if (m_pCacheColors.isNull())
    m_pCacheColors = new ODGSPALETTE();
  return *m_pCacheColors;
}
