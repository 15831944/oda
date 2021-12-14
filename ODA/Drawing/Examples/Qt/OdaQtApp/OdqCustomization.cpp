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
// OdqCustomization.cpp
//

#include <QFile>
#include <QDateTime>
#include <QDomDocument>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QBitmap>
#include <QSettings>
#include <QStack>
#include <QTextStream>
#include <QDir>
#include <QStyle>

#include "OdaCommon.h"
#include "DbBaseHostAppServices.h"
#include "DbDatabase.h"
#include "TDVersion.h"

#include "OdqCustomization.h" 
#include "OdqMainWindow.h"
#include "ExtDbModule.h"

//////////////////////////////////////////////////////////////////////////

static bool parseExpression(QDomElement& elem, 
                            QString& qsTagExpression, QStack<QVariant>& values)
{
  if (elem.isNull() || qsTagExpression.isEmpty())
    return false; // syntax error if qsTagExpression is non-empty

  QStringList lstPatterns;
  lstPatterns << "(@)([A-Z]*[A-Za-z0-9_]+)"
              << "(')(.*)(')"
              << "(\")(.*)(\")"
              << "!=" << "=" << "\\bnot\\b" << "\\band\\b" << "\\bor\\b"; 
  int index = -1;
  foreach (const QString& qsPattern, lstPatterns)
  {
    index++;
    QRegExp rx(qsPattern);
    int pos = rx.indexIn(qsTagExpression);
    if (pos)
      continue;

    qsTagExpression = qsTagExpression.mid(rx.matchedLength()).trimmed();

    bool bVal;
    QString qsVal;
    switch (index)
    {
    case 0: // "(@)([A-Z]*[A-Za-z0-9_]+)"
      qsVal = rx.cap(2);
      qsVal = elem.attribute(qsVal);
      values.push(qsVal);
      break;
    case 1: // "(')(.*)(')"
    case 2: // "(\")(.*)(\")"
      qsVal = rx.cap(2);
      values.push(qsVal);
      break;
    case 3: // "!="
      if (!parseExpression(elem, qsTagExpression, values) && !qsTagExpression.isEmpty())
        return false;
      ODA_ASSERT_ONCE(values.count() > 1);
      qsVal = values.pop().toString();
      bVal = (qsVal != values.pop().toString());
      values.push(bVal);
      break;
    case 4: // "="
      if (!parseExpression(elem, qsTagExpression, values) && !qsTagExpression.isEmpty())
        return false;
      ODA_ASSERT_ONCE(values.count() > 1);
      qsVal = values.pop().toString();
      bVal = (qsVal == values.pop().toString());
      values.push(bVal);
      break;
    case 5: // "\\bnot\\b" 
      if (!parseExpression(elem, qsTagExpression, values) && !qsTagExpression.isEmpty())
        return false;
      ODA_ASSERT_ONCE(values.count());
      bVal = !values.pop().toBool();
      values.push(bVal);
      break;
    case 6: // "\\band\\b"
      if (!parseExpression(elem, qsTagExpression, values) && !qsTagExpression.isEmpty())
        return false;
      ODA_ASSERT_ONCE(values.count() > 1);
      bVal = values.pop().toBool();
      bVal = bVal && values.pop().toBool();
      values.push(bVal);
      break;
    case 7: // "\\bor\\b"
      if (!parseExpression(elem, qsTagExpression, values) && !qsTagExpression.isEmpty())
        return false;
      ODA_ASSERT_ONCE(values.count() > 1);
      bVal = values.pop().toBool();
      bVal = bVal || values.pop().toBool();
      values.push(bVal);
      break;
    default:
      ODA_FAIL_ONCE(); // TODO
      break;
    }
    return !qsTagExpression.isEmpty();
  }
  return false; // syntax error
}

static bool checkExpression(QDomElement elem, const QString& cqsTagExpression)
{
  if (elem.isNull())
    return false;
  if (cqsTagExpression.isEmpty())
    return true;

  QString qsTagExpression = cqsTagExpression.trimmed();
  QStack<QVariant> values;

  while (parseExpression(elem, qsTagExpression, values))
  {
  }
  if (!qsTagExpression.isEmpty())
  {
    ODA_FAIL_ONCE();
    OdString sMsg = getIApp()->toOdString(
    QObject::tr("Syntax error or unsupported xpath expression \"%1\" at pos %2")
      .arg(cqsTagExpression)
      .arg(cqsTagExpression.length() - qsTagExpression.length()));
    getIApp()->getBaseHostAppServices()->warning(sMsg);
    return false;
  }
  if (values.count() != 1)
  {
    ODA_FAIL_ONCE();
    return false;
  }
  QVariant val = values.pop();
  return val.toBool();
}

static QList<QDomElement> getElements(QDomNode node, const QString& cqsPath,
                                      bool bSingleOnly = false)
{
  ODA_ASSERT_ONCE(!node.isNull() && !cqsPath.isEmpty());
  QString qsPath = cqsPath.trimmed();

  // possible TODO is using QXmlQuery etc 
  // (now xmlpatterns does not compiled via msvc2005)

  QList<QDomElement> lstRes;

  QString qsTagName,
          qsTagExpression;
  int pos = qsPath.indexOf("/");
  if (!pos)
  {
    ODA_FAIL_ONCE(); // unsupported now // TODO
    return lstRes;
  }
  if (pos > 0)
  {
    qsTagName = qsPath.left(pos).trimmed();
    qsPath = qsPath.mid(++pos);
  }
  else
  {
    qsTagName = qsPath;
    qsPath = "";
  }

  pos = qsTagName.indexOf("[");
  if (!pos)
  {
    ODA_FAIL_ONCE(); // unsupported now // TODO
    return lstRes;
  }
  if (pos > 0)
  {
    qsTagExpression = qsTagName.mid(pos + 1).trimmed();
    qsTagName = qsTagName.left(pos).trimmed();
    pos = qsTagExpression.lastIndexOf("]");
    if (pos >= 0)
    {
      ODA_ASSERT_ONCE(pos && pos == (qsTagExpression.length() - 1));
      qsTagExpression = qsTagExpression.left(pos).trimmed();
      ODA_ASSERT_ONCE(!qsTagExpression.isEmpty());
    }
    else
      ODA_FAIL_ONCE(); // Syntax error
  }

  if (qsTagName.isEmpty())
  {
    ODA_FAIL_ONCE();
    return lstRes;
  }

  QRegExp rx;
  if (qsTagName.indexOf("*") >= 0)
  {
    rx.setPatternSyntax(QRegExp::Wildcard);
    rx.setPattern(qsTagName);
    ODA_ASSERT_ONCE(rx.isValid());
    qsTagName = "";
  }

  for (QDomElement elem = node.firstChildElement(qsTagName);
       !elem.isNull(); elem = elem.nextSiblingElement(qsTagName))
  {
    if (qsTagName.isEmpty())
    {
      QString qsName = elem.nodeName();
      ODA_ASSERT_ONCE(!qsName.isEmpty());

      if (!rx.exactMatch(qsName))
        continue;
    }

    if (checkExpression(elem, qsTagExpression))
    {
      if (!qsPath.isEmpty())
      {
        QList<QDomElement> lst = getElements(elem, qsPath, bSingleOnly);
        if (lst.count())
        {
          lstRes.append(lst);
          if (bSingleOnly)
            break;
        }
      }
      else
      {
        lstRes.append(elem);
        if (bSingleOnly)
          break;
      }
    }
  }

  return lstRes;
}

static QDomElement getSingleElement(QDomNode node, const QString& qsPath)
{
  QList<QDomElement> lst = getElements(node, qsPath,
                                       true); // single
  if (!lst.count())
    return QDomElement();
    
  ODA_ASSERT_ONCE(lst.count() == 1);
  return lst.first();
}

static bool saveXml(const QDomDocument& docXml, const QString& qsPath)
{
  ODA_ASSERT_ONCE(!qsPath.isEmpty());
  
  QFile file(qsPath);
  if (!file.open(QFile::WriteOnly | QFile::Text)) 
    return false;

  QTextStream out(&file);
  docXml.save(out, 2); // IndentSize
  return true;
}

//////////////////////////////////////////////////////////////////////////

OdqMenuMacro::OdqMenuMacro(const QString& qsCommand,
                           const QString& qsIconPath)
  : m_qsCommand(qsCommand)
  , m_qsIconPath(qsIconPath)
{
}

OdqMenuMacro::~OdqMenuMacro()
{
}

QAction* OdqMenuMacro::action() const
{
  return m_pAction;
}

void OdqMenuMacro::setAction(QAction* pAction)
{
  m_pAction = pAction;
  if (!pAction)
    return;

  if (!connect(pAction, SIGNAL(triggered()), this, SLOT(execute())))
  {
    ODA_FAIL_ONCE();
  }

  //QIcon icn = getIcon();
  //if (!icn.isNull())
  //  pAction->setIcon(icn);
  updateIconToAction();
}

//#define USE_CHECK_ON_OFF_AS_MARKER_TO_SET_CHECKED

void OdqMenuMacro::updateIconToAction()
{
  QAction* pAction = action();
  ODA_ASSERT_ONCE(pAction); // test
  if (!pAction)
    return;

  QString qsIconPath = getIconPath();
  //ODA_ASSERT_ONCE(!qsIconPath.isEmpty()); // test
  if (qsIconPath.isEmpty())
    return;

#if defined(USE_CHECK_ON_OFF_AS_MARKER_TO_SET_CHECKED)
  int pos = qsIconPath.indexOf("check_o");
  if (pos >= 0)
  {
    if (!pAction->isCheckable())
    {
      ??? // TODO // reconnect
    }
    bool isOn = (qsIconPath.at(pos + 7) == 'n');
    ODA_ASSERT_ONCE(isOn || (qsIconPath.at(pos + 7) == 'f'));
    pAction->setCheckable(true);
    pAction->setChecked(isOn);
    return;
  }
#endif

  QIcon icn = getIcon();
  if (!icn.isNull())
    pAction->setIcon(icn);
}

QString OdqMenuMacro::getCommand(bool bEvaluated) const // = true
{
  if (m_qsCommand.isEmpty() || !bEvaluated)
    return m_qsCommand;
  QString qsEvaluatedCmd = getIConsole()->evaluateDieselExpression(m_qsCommand);
  return qsEvaluatedCmd;
}

bool OdqMenuMacro::isReferenced() const
{
  return !m_pAction.isNull();
}

bool OdqMenuMacro::hasIconPathDiesel() const
{
  if (m_qsIconPath.indexOf("$(") < 0)
    return false;
  return true;
}

QString OdqMenuMacro::getIconPath(bool bEvaluated) const // = true
{
  QString qsIconPath = m_qsIconPath;
  if (bEvaluated && !qsIconPath.isEmpty())
    qsIconPath = getIConsole()->evaluateDieselExpression(qsIconPath);
  if (qsIconPath.isEmpty())
    return qsIconPath;
  
  // QFile does not work with "qrc:/..." prefix (should be ":/...")
  qsIconPath.replace("qrc:/", ":/");

  bool bExists = QFileInfo(qsIconPath).exists();
  ODA_ASSERT_ONCE(bExists || qsIconPath.indexOf(".png", 0, Qt::CaseInsensitive) < 0);
  if (!bExists)
    qsIconPath.clear(); // image file was not found

  return qsIconPath;
}

QIcon OdqMenuMacro::getIcon() const
{
  QString qsIconPath = getIconPath();
  if (qsIconPath.isEmpty())
    return QIcon();

  QPixmap pixmap(qsIconPath);
  if (pixmap.isNull())
    return QIcon();

  pixmap.setMask(pixmap.createHeuristicMask());
  return QIcon(pixmap);
}

// slot
void OdqMenuMacro::execute() const
{
  QString qsCommand = getCommand();
  if (qsCommand.isEmpty())
    return;
  if (action()->isCheckable() && !action()->isChecked())
    action()->setChecked(true); // restore state

  getIConsole()->executeCommand(qsCommand);
}

//////////////////////////////////////////////////////////////////////////

OdqMenuControlMacro::OdqMenuControlMacro(const QString& qsEventKeys) // = ""
  : OdqMenuMacro("", "")
  , m_qsEventKeys(qsEventKeys)
{
  if (!m_qsEventKeys.isEmpty())
    getIApp()->registerQObject(m_qsEventKeys, this);
}

OdqMenuControlMacro::~OdqMenuControlMacro()
{
  if (!m_qsEventKeys.isEmpty())
    getIApp()->unregisterQObject(m_qsEventKeys, this);
}

QString OdqMenuControlMacro::getEventKey(int indexKey) const // = 0
{
  QStringList lst = m_qsEventKeys.split('|', QString::SkipEmptyParts);
  if (indexKey < 0 || indexKey >= lst.size())
  {
    ODA_FAIL_ONCE();
    return "";
  }

  return lst[indexKey].trimmed();
}

bool OdqMenuControlMacro::isMenu() const
{
  return false;
}

bool OdqMenuControlMacro::isVisible() const
{
  return isMenu();
}

bool OdqMenuControlMacro::isEnabled() const
{
  return true;
}

// slot
void OdqMenuControlMacro::refresh(const QString& qsKey, const QString& qsArg) // refresh states
{
  ODA_ASSERT_ONCE(m_qsEventKeys.indexOf(qsKey) >= 0 || qsKey == "<event-refresh:database_changed>");

  QAction* pAction = action();
  ODA_ASSERT_ONCE(pAction);
  if (!pAction)
    return;

  bool bVisible = isVisible(),
       bEnabled = isEnabled();

  if (pAction->isEnabled() != bEnabled)
    pAction->setEnabled(bEnabled);
  if (!bVisible || pAction->isVisible() != bVisible)
    pAction->setVisible(bVisible);

  //QString qsText = text();
  //if (pAction->text() != qsText)
  //  pAction->setText(qsText);

  //QString qsIconPath;
  //if (!(qsIconPath = getIconPath()).isEmpty() && qsIconPath != getIconPath(false))
  //  pAction->setIcon(getIcon());
  updateIconToAction();

  refreshData(qsKey, qsArg);
}

//////////////////////////////////////////////////////////////////////////

class OdqMenuControlDrawingHistory : public OdqMenuControlMacro
{
  //Q_OBJECT
private:
  QPointer<OdqCustomization> m_pCustomization;
  QList<OdSharedPtr<OdqMenuMacro> > m_lstDrawingHistoryMacros;

  Q_DISABLE_COPY(OdqMenuControlDrawingHistory) 
public:
  OdqMenuControlDrawingHistory(OdqCustomization* pCustomization)
    : OdqMenuControlMacro("<event-refresh:drawing_history_update>")
    , m_pCustomization(pCustomization)
  {
    ODA_ASSERT_ONCE(!m_pCustomization.isNull());
  }

  virtual void refreshData(const QString&, const QString&)
  {
    //refreshDrawingHistory
    QWidget* pParent = qobject_cast<QWidget*>(action()->parent());
    if (!pParent)
    {
      ODA_FAIL_ONCE(); // test
      return;
    }

    QList<QAction*> lstHist = pParent->findChildren<QAction*>("history");
    foreach (QAction* pHistAction, lstHist)
      pParent->removeAction(pHistAction);
    m_lstDrawingHistoryMacros.clear();

    foreach (QString qsPath, m_pCustomization->m_lstDrawingHistory)
    {
      QAction* pHist = new QAction(qsPath, pParent);
      pHist->setObjectName("history");
      pParent->insertAction(action(), pHist);

      OdSharedPtr<OdqMenuMacro> pMenuMacro 
        = new OdqMenuMacro(QString("open \"%1\"").arg(qsPath));
      pMenuMacro->setAction(pHist);
      m_lstDrawingHistoryMacros.push_back(pMenuMacro);
    }
  }
};

//////////////////////////////////////////////////////////////////////////

class OdqMenuControlWindows : public OdqMenuControlMacro
{
  //Q_OBJECT
private:
  QHash<QString, OdSharedPtr<OdqMenuMacro> > m_mapWndMacro;

  Q_DISABLE_COPY(OdqMenuControlWindows) 
public:
  OdqMenuControlWindows()
    : OdqMenuControlMacro("<event-refresh:window_added>"
                          "|<event-refresh:window_removed>"
                          "|<event-refresh:window_activated>")
  {
  }

  virtual void refreshData(const QString& qsKey, const QString& qsArg)
  {
    QWidget* pParent = qobject_cast<QWidget*>(action()->parent());
    if (!pParent)
    {
      ODA_FAIL_ONCE(); // test
      return;
    }

    if (!qsArg.isEmpty())
    {
      if (qsKey == "<event-refresh:window_added>")
      {
        QAction* pWndAction = new QAction(qsArg, pParent);
        pWndAction->setCheckable(true);
        pWndAction->setChecked(true);
        pWndAction->setObjectName("window");
        pParent->addAction(pWndAction);

        OdSharedPtr<OdqMenuMacro> pMenuMacro 
          = new OdqMenuMacro(QString("\'syswindow \"%1\"").arg(qsArg));
        pMenuMacro->setAction(pWndAction);
        m_mapWndMacro[qsArg] = pMenuMacro;
        return;
      }
      if (qsKey == "<event-refresh:window_removed>")
      {
        QList<QAction*> lstWndActions = pParent->findChildren<QAction*>("window");
        foreach (QAction* pWndAction, lstWndActions)
        {
          if (qsArg != pWndAction->text())
            continue;

          OdSharedPtr<OdqMenuMacro> pMenuMacro = m_mapWndMacro[qsArg];
          pParent->removeAction(pWndAction);
          m_mapWndMacro.erase(m_mapWndMacro.find(qsArg));
          if (pMenuMacro.get() && pMenuMacro->action())
          {
            ODA_ASSERT_ONCE(pWndAction == pMenuMacro->action());
            delete pWndAction;
          }
          return;
        }
        ODA_FAIL_ONCE();
        return;
      }
      if (qsKey == "<event-refresh:window_activated>")
      {
        QList<QAction*> lstWndActions = pParent->findChildren<QAction*>("window");
        foreach (QAction* pWndAction, lstWndActions)
        {
          bool bChecked = (qsArg == pWndAction->text());
          if (bChecked != pWndAction->isChecked())
            pWndAction->setChecked(bChecked);
        }
        return;
      }
    }
    ODA_ASSERT_ONCE(qsArg.isEmpty()); // TODO

    QList<QAction*> lstWndActions = pParent->findChildren<QAction*>("window");
    foreach (QAction* pWndAction, lstWndActions)
    {
      pParent->removeAction(pWndAction);
      //delete pWndAction;
    }
    m_mapWndMacro.clear();

    IqViewSystem* iVS = getIViewSystem();
    QList<OdRxObject*> lstRxDatabases = getIConsole()->getOpenDatabases();
    if (!iVS || !lstRxDatabases.size())
      return;

    foreach (OdRxObject* pRxDb, lstRxDatabases)
    {
      ODA_ASSERT_ONCE(pRxDb);

      iVS->updateViewTitles(pRxDb, true);
    }
    iVS->updateViewTitles(NULL, true); // for "<event-refresh:window_activated>"
  }
};

//////////////////////////////////////////////////////////////////////////

inline bool caseInsensitiveLessThan(const QString& s1, const QString& s2)
{
  return s1.toLower() < s2.toLower();
}

class OdqMenuControlRegisteredCommands : public OdqMenuControlMacro
{
  //Q_OBJECT
private:
  Q_DISABLE_COPY(OdqMenuControlRegisteredCommands)

  QHash<QString, QPointer<QMenu> > m_mapMenuByGroup;
  QHash<QString, OdSharedPtr<OdqMenuMacro> > m_mapMenuMacro;
public:
  OdqMenuControlRegisteredCommands()
    : OdqMenuControlMacro("<event-refresh:command_added>"
                          "|<event-refresh:command_removed>")
  {
  }

  virtual bool isMenu() const
  {
    return true;
  }

  virtual void refreshData(const QString& qsKey, const QString& qsArg)
  {
    QMenu* pMenu = qobject_cast<QMenu*>(action()->parent());
    if (!pMenu)
    {
      ODA_FAIL_ONCE(); // test
      return;
    }

    if (!qsArg.isEmpty() && !m_mapMenuByGroup.isEmpty())
    {
      QStringList lst = qsArg.split(cqchCmdGroupDelim, QString::SkipEmptyParts);
      ODA_ASSERT_ONCE(lst.size() == 2);
      if (lst.size() != 2)
        return;
      QString qsGroup = lst.first().trimmed(),
              qsCommand = lst.at(1).trimmed(),
              qsCommandLow = qsCommand.toLower();
      ODA_ASSERT_ONCE(!qsGroup.isEmpty() && !qsCommand.isEmpty());

      if (qsKey == "<event-refresh:command_added>")
      {
        if (m_mapMenuByGroup.find(qsGroup) == m_mapMenuByGroup.end())
          m_mapMenuByGroup[qsGroup] = pMenu->addMenu(qsGroup);
        
        QPointer<QMenu> pSubMenu = m_mapMenuByGroup[qsGroup];
        ODA_ASSERT_ONCE(!pSubMenu.isNull());
        if (pSubMenu.isNull())
          return;
        QAction* pActionBefore = NULL;
        foreach (QAction* pAct, pSubMenu->actions())
        {
          ODA_ASSERT_ONCE(pAct);
          QString qsCmd = pAct->text().toLower();
          if (qsCmd <= qsCommandLow)
            continue;
          pActionBefore = pAct;
          break;
        }

        QAction* pCmdAction = new QAction(qsCommand, pSubMenu);
        pCmdAction->setObjectName("command");
        if (pActionBefore)
          pSubMenu->insertAction(pActionBefore, pCmdAction);
        else
          pSubMenu->addAction(pCmdAction);

        OdSharedPtr<OdqMenuMacro> pMenuMacro = new OdqMenuMacro(qsCommand.toLower());
        pMenuMacro->setAction(pCmdAction);
        m_mapMenuMacro[qsCommandLow] = pMenuMacro;
        return;
      }
      ODA_ASSERT_ONCE(qsKey == "<event-refresh:command_removed>");

      if (m_mapMenuByGroup.find(qsGroup) == m_mapMenuByGroup.end())
      {
        ODA_FAIL_ONCE();
        return;
      }
      QPointer<QMenu> pSubMenu = m_mapMenuByGroup[qsGroup];
      ODA_ASSERT_ONCE(!pSubMenu.isNull());
      if (pSubMenu.isNull())
        return;
      OdSharedPtr<OdqMenuMacro> pMenuMacro = m_mapMenuMacro[qsCommandLow];
      ODA_ASSERT_ONCE(!pMenuMacro.isNull());
      if (pMenuMacro.isNull())
        return;
      m_mapMenuMacro[qsCommandLow] = NULL;
      QAction* pCmdAction = pMenuMacro->action();
      pSubMenu->removeAction(pCmdAction);
      QList<QAction*> lstActions =  pSubMenu->actions();
      int cntActions = lstActions.size();
      if (cntActions)
        return;
      // remove an empty Sub Menu
      pMenu->removeAction(pSubMenu->menuAction());
      return;
    }

    pMenu->clear();
    m_mapMenuByGroup.clear();
    m_mapMenuMacro.clear();
    const TCmdMap& mapCmds = getIConsole()->getCommandMap();

    foreach (QString qsGroups, mapCmds.values())
    {
      QStringList lstGroups = qsGroups.split(cqchCmdGroupDelim, QString::SkipEmptyParts);
      ODA_ASSERT_ONCE(!lstGroups.isEmpty());
      if (lstGroups.isEmpty())
        continue;
      QString qsGroup = lstGroups.first().trimmed();
      ODA_ASSERT_ONCE(!qsGroup.isEmpty());
      if (qsGroup.isEmpty())
        continue;
      if (m_mapMenuByGroup.find(qsGroup) != m_mapMenuByGroup.end())
        continue;

      m_mapMenuByGroup[qsGroup] = pMenu->addMenu(qsGroup);
    }

    QStringList lstCommands = mapCmds.keys();
    qSort(lstCommands.begin(), lstCommands.end(), caseInsensitiveLessThan);

    foreach (QString qsCommand, lstCommands)
    {
      QString qsCommandLow = qsCommand.toLower();
      QString qsGroups = mapCmds[qsCommandLow];
      QStringList lstGroups = qsGroups.split(cqchCmdGroupDelim, QString::SkipEmptyParts);
      ODA_ASSERT_ONCE(!lstGroups.isEmpty());
      if (lstGroups.isEmpty())
        continue;
      QString qsGroup = lstGroups.first().trimmed();
      ODA_ASSERT_ONCE(!qsGroup.isEmpty());
      if (qsGroup.isEmpty())
        continue;
      ODA_ASSERT_ONCE(m_mapMenuByGroup.find(qsGroup) != m_mapMenuByGroup.end());
      QPointer<QMenu> pSubMenu = m_mapMenuByGroup[qsGroup];
      ODA_ASSERT_ONCE(!pSubMenu.isNull());
      if (pSubMenu.isNull())
        continue;

      QAction* pCmdAction = new QAction(qsCommand, pSubMenu);
      pCmdAction->setObjectName("command");
      pSubMenu->addAction(pCmdAction);

      OdSharedPtr<OdqMenuMacro> pMenuMacro = new OdqMenuMacro(qsCommand.toLower());
      pMenuMacro->setAction(pCmdAction);
      m_mapMenuMacro[qsCommandLow] = pMenuMacro;
    }
  }
};

//////////////////////////////////////////////////////////////////////////

OdqToolbarControlMacro::OdqToolbarControlMacro(const QString& qsButtonText, // tooltip
                                               const QString& qsCommand, 
                                               const QString& qsIconPath,
                                               const QString& qsEventKeys) // = ""
 : OdqMenuMacro(qsCommand, qsIconPath)
 , m_qsButtonText(qsButtonText)
 , m_qsEventKeys(qsEventKeys)
{
  if (!m_qsEventKeys.isEmpty())
    getIApp()->registerQObject(m_qsEventKeys, this);
}

OdqToolbarControlMacro::~OdqToolbarControlMacro()
{
  if (!m_qsEventKeys.isEmpty())
    getIApp()->unregisterQObject(m_qsEventKeys, this);
}

QString OdqToolbarControlMacro::text(bool bEvaluated) const //  = true
{
  if (!bEvaluated)
    return m_qsButtonText;
  QString qsText = getIConsole()->evaluateDieselExpression(m_qsButtonText);
  if (!qsText.indexOf("~"))
    qsText = qsText.mid(1);
  return qsText;
}

bool OdqToolbarControlMacro::isVisible() const
{
  if (text().isEmpty())
    return false;

  return true;
}

bool OdqToolbarControlMacro::isEnabled() const
{
  QString qsText = getIConsole()->evaluateDieselExpression(m_qsButtonText);
  if (qsText.isEmpty() || !qsText.indexOf("~"))
    return false;
  return true;
}

// slot
void OdqToolbarControlMacro::refresh(const QString& qsKey, const QString&) // refresh states
{
  //ODA_ASSERT_ONCE(   qsKey == "<event-refresh:database_changed>"
  //                || qsKey == "<event-refresh:cui_changed>"
  //                || qsKey == "<event-refresh:dwg_cvport_changed>"
  //                || qsKey == "<event-refresh:appvar_changed>");

  QAction* pAction = action();
  ODA_ASSERT_ONCE(pAction);
  if (!pAction)
    return;

  bool bVisible = isVisible(),
       bEnabled = isEnabled();

  if (pAction->isEnabled() != bEnabled)
    pAction->setEnabled(bEnabled);
  if (!bVisible || pAction->isVisible() != bVisible)
    pAction->setVisible(bVisible);

  QString qsText = text();
  if (pAction->text() != qsText)
    pAction->setText(qsText);

  //QString qsIconPath;
  //if (!(qsIconPath = getIconPath()).isEmpty() && qsIconPath != getIconPath(false))
  //  pAction->setIcon(getIcon());
  updateIconToAction();
}

//////////////////////////////////////////////////////////////////////////

class OdqToolbarControlUndo : public OdqToolbarControlMacro
{
private: Q_DISABLE_COPY(OdqToolbarControlUndo) //Q_OBJECT
public:
  OdqToolbarControlUndo()
    : OdqToolbarControlMacro(QObject::tr("Undo (Ctrl+Z)"), // Reverses the most recent action
                             "_u", ":/images/undo.png",
                             "<event-refresh:database_changed>"
                             "|<event-refresh:dwg_layout_changed>"
                             "|<event-refresh:undo_state_changed>")
  {
  }

  virtual bool isEnabled() const
  {
    OdRxObject* pRxDb = getIConsole()->getCurrentDatabase();
    if (!pRxDb)
      return false;
    if (OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDb).get())
      return pDwgDb->hasUndo();
    if (OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDb))
      return pExtDb->hasUndo(pRxDb);
    return false;
  }
};

class OdqToolbarControlRedo : public OdqToolbarControlMacro
{
private: Q_DISABLE_COPY(OdqToolbarControlRedo) //Q_OBJECT
public:
  OdqToolbarControlRedo()
    : OdqToolbarControlMacro(QObject::tr("Redo (Ctrl+Y)"), // Reverses the effects of the previous UNDO
                             "_mredo 1", ":/images/redo.png",
                             "<event-refresh:database_changed>"
                             "|<event-refresh:dwg_layout_changed>"
                             "|<event-refresh:redo_state_changed>")
  {
  }

  virtual bool isEnabled() const
  {
    OdRxObject* pRxDb = getIConsole()->getCurrentDatabase();
    if (!pRxDb)
      return false;
    if (OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDb).get())
      return pDwgDb->hasRedo();
    if (OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDb))
      return pExtDb->hasRedo(pRxDb);
    return false;
  }
};

//////////////////////////////////////////////////////////////////////////

// static
OdSharedPtr<OdqMenuMacro> OdqMenuMacro::createMenuControlMacro(const QString& qsUID, 
                                                               OdqCustomization* pCustomization)
{
  OdSharedPtr<OdqMenuMacro> pMacro;

  if (qsUID == "ID_MRU")
    pMacro = new OdqMenuControlDrawingHistory(pCustomization);
  else if (qsUID == "ID_SYSWINDOWS")
    pMacro = new OdqMenuControlWindows();
  else if (qsUID == "ID_REGCOMMANDS")
    pMacro = new OdqMenuControlRegisteredCommands();
  else
  {
    ODA_FAIL_ONCE(); // unknown control
  }

  return pMacro;
}

// static
OdSharedPtr<OdqMenuMacro> OdqMenuMacro::createToolbarControlMacro(const QString& qsUID,
                                                                  const QString& qsControl)
{
  OdSharedPtr<OdqMenuMacro> pMacro;
  if (qsControl == "DimStyleControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0001");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "OPTColorControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0002"); 
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "LineTypeControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0003");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "LineWeightControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0004");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "PlotStyleControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0005");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "LayerControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0006");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "TxtStyleControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0007");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "DimStyleControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0008");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "TblStyleControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0009");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "MldStyleControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0035" || qsUID == "TBC_0034");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "ViewControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0010");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "RefblknameControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0011");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "UndoSkinnyButtonControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBCU_0001" || qsUID == "TBC_0012"); 
    pMacro = new OdqToolbarControlUndo();
  }
  else if (qsControl == "RedoSkinnyButtonControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBCU_0002" || qsUID == "TBC_0013");
    pMacro = new OdqToolbarControlRedo();
  }
  else if (qsControl == "UCSControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0014");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "WorkspacesControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0015");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "NamedViewControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0016");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "ViewportScaleControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0017");
    ODA_FAIL_ONCE(); // TODO
  }
  else if (qsControl == "FindTextControl")
  {
    ODA_ASSERT_ONCE(qsUID == "TBC_0036");
    ODA_FAIL_ONCE(); // TODO
  }
  else
  {
    ODA_FAIL_ONCE(); // unknown control
  }

  return pMacro;
}

//////////////////////////////////////////////////////////////////////////

class OdqCustomGroup
{
public:
  OdqCustomGroup(const QString& qsSrcFilePath)
    : m_isOn(true)
  {
    QFileInfo info(qsSrcFilePath);
    ODA_ASSERT_ONCE(info.exists());
    // used for isEqualByFileAttribs
    m_qsSrcFilePath = QDir::toNativeSeparators(info.canonicalFilePath());
    m_sizeSrcFile = info.size();
    m_timeSrcFile = info.lastModified();

    m_docXml.setContent(QString("<?xml version=\"1.0\"?>")); // set minimal header
  }

  virtual ~OdqCustomGroup()
  {
  }

  bool isOn() const
  {
    return m_isOn;
  }

  void setOn(bool bValue = true)
  {
    m_isOn = bValue;
  }

  bool isEqualByFileAttribs(const QString& qsSrcFilePath) const
  {
    QFileInfo info(qsSrcFilePath);
    ODA_ASSERT_ONCE(info.exists());

    bool bRes =    m_qsSrcFilePath == QDir::toNativeSeparators(info.canonicalFilePath())
                && m_sizeSrcFile == info.size()
                && m_timeSrcFile == info.lastModified();
    return bRes;
  }

  QDomDocument& getXmlDoc()
  {
    return m_docXml;
  }
  OdSharedPtr<OdqMenuMacro>& getMenuMacro(const QString& qsUID)
  {
    return m_mapMenuMacros[qsUID];
  }
  OdSharedPtr<OdqMenuMacro> getMenuMacro(const QString& qsUID, const QString& qsToolbarControl)
  {
    if (m_mapMenuMacros.find(qsUID) != m_mapMenuMacros.end())
      return m_mapMenuMacros[qsUID];

    OdSharedPtr<OdqMenuMacro> pMacro = OdqMenuMacro::createToolbarControlMacro(qsUID, qsToolbarControl);

    m_mapMenuMacros[qsUID] = pMacro;
    return pMacro;
  }

  void clearUnreferencedMenuMacros()
  {
    QStringList keysToDelete;
    foreach (QString qsUID, m_mapMenuMacros.keys())
    {
      OdSharedPtr<OdqMenuMacro>& pMacro = m_mapMenuMacros[qsUID];
      if (pMacro.isNull() || !pMacro->isReferenced())
        keysToDelete << qsUID;
    }

    foreach (QString qsUID, keysToDelete)
      m_mapMenuMacros.remove(qsUID);
  }

  void refreshIconStates() // conditional only
  {
    QStringList keysToDelete;
    foreach (QString qsUID, m_mapMenuMacros.keys())
    {
      OdSharedPtr<OdqMenuMacro>& pMacro = m_mapMenuMacros[qsUID];
      if (pMacro.isNull() || !pMacro->isReferenced())
        continue;
      if (qobject_cast<OdqMenuControlMacro*>(pMacro.get())) // (qsUID == "ID_MRU") // skip drawing history
        continue;

      if (!pMacro->hasIconPathDiesel())
        continue; // an empty or not conditional
      QString qsIconPath = pMacro->getIconPath();
      QAction* pAction = pMacro->action();
      ODA_ASSERT_ONCE(pAction || qsIconPath.isEmpty());
      if (!pAction)
        continue;

#    if defined(USE_CHECK_ON_OFF_AS_MARKER_TO_SET_CHECKED)
      int pos = qsIconPath.indexOf("check_o");
      if (pos >= 0)
      {
        bool isOn = (qsIconPath.at(pos + 7) == 'n');
        ODA_ASSERT_ONCE(isOn || (qsIconPath.at(pos + 7) == 'f'));
        pAction->setCheckable(true);
        pAction->setChecked(isOn);
        return;
      }
#    endif
      QIcon icn = pMacro->getIcon();
      pAction->setIcon(icn);
    }
  }

private:
  bool m_isOn;
  QDomDocument m_docXml;
  QHash<QString, OdSharedPtr<OdqMenuMacro> > m_mapMenuMacros;

  // used for isEqualByFileAttribs only
  QString m_qsSrcFilePath;  
  qint64 m_sizeSrcFile;
  QDateTime m_timeSrcFile;
};

//////////////////////////////////////////////////////////////////////////

OdqCustomization::OdqCustomization()
{
  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);

  m_sizeMaxDrawingHistory = settings.value("history/max", 10).toInt();
  int count = settings.value("history/count", 0).toInt();
  if (count < 0 || count > m_sizeMaxDrawingHistory)
    count = 0;

  for (int index = 0; index < count; index++)
  {
    QString qsPath = settings.value(QString("history/path_%1").arg(index), "").toString();
    if (qsPath.isEmpty() || !QFileInfo(qsPath).exists() || m_lstDrawingHistory.contains(qsPath.replace('\\', '/')))
      continue;
   
    m_lstDrawingHistory << qsPath;
  }

  settings.endGroup();

  getIApp()->registerQObject("<event-refresh:database_changed>"
                             "|<event-refresh:dwg_layout_changed>"
                             "|<event-refresh:dwg_cvport_changed>"
                             "|<event-refresh:appvar_changed>"
                             "|<event-refresh:drawing_history_changed>"
                             "|<event-refresh:toolbarsize_changed>", this);
}

OdqCustomization::~OdqCustomization()
{
  // now call via OdqMainWindow::closeEvent() // saveSettings();
}

void OdqCustomization::saveSettings(bool bToolbarsOnly) // = false
{
  if (!bToolbarsOnly)
  {
    getIApp()->unregisterQObject("<event-refresh:database_changed>"
                                 "|<event-refresh:dwg_layout_changed>"
                                 "|<event-refresh:dwg_cvport_changed>"
                                 "|<event-refresh:appvar_changed>"
                                 "|<event-refresh:drawing_history_changed>"
                                 "|<event-refresh:toolbarsize_changed>", this);
    int count = m_lstDrawingHistory.size();
    if (count)
    {
      ODA_ASSERT_ONCE(count <= m_sizeMaxDrawingHistory);

      QSettings settings("ODA", 
                         QFileInfo(QCoreApplication::applicationFilePath()).baseName());
      settings.beginGroup(TD_SHORT_STRING_VER_S);

      settings.setValue("history/max", m_sizeMaxDrawingHistory);
      settings.setValue("history/count", count);
      int index = 0;
      foreach (QString qsPath, m_lstDrawingHistory)
        settings.setValue(QString("history/path_%1").arg(index++), qsPath);

      settings.endGroup();
    }
  }

  bool hasToolbars = false;
  foreach (QPointer<QToolBar> pToolbar, getIApp()->getMainWindow()->findChildren<QToolBar*>()) 
  {
    ODA_ASSERT_ONCE(!pToolbar.isNull());
    if (pToolbar.isNull())
      continue;
    QString qsTollbarNameBase = pToolbar->objectName();
    ODA_ASSERT_ONCE(!qsTollbarNameBase.isEmpty());
    if (qsTollbarNameBase.isEmpty())
      continue;
    hasToolbars = true;

    // save properties
    if (!qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow())
            ->saveQObjectProperties(pToolbar,
                                    "enabled visible"))
    {
      ODA_FAIL_ONCE();
    }
  }
  if (hasToolbars)
  {
    QSettings settings("ODA", 
                       QFileInfo(QCoreApplication::applicationFilePath()).baseName());
    settings.setValue(TD_SHORT_STRING_VER_S 
                      "/objectProps/QToolBar/windowState", getIApp()->getMainWindow()->saveState());
  }
}

QStringList OdqCustomization::getCustomGroups()
{
  QStringList lstGroups;
  foreach (QString qsGroup, m_lstCustomGroups) // in loading order // m_mapCustomGroups.keys())
  {
    OdSharedPtr<OdqCustomGroup>& pGroup = m_mapCustomGroups[qsGroup];
    if (!pGroup.isNull() && pGroup->isOn())
      lstGroups << qsGroup;
  }

  return lstGroups;
}

QString OdqCustomization::getLoadedCuiCustomGroupNameByFileName(const QString& cqsCuiFileName) const
{
  QFileInfo info(cqsCuiFileName);
  QString qsCuiFileName = info.completeBaseName();
#if defined(TD_DLL_VERSION_SUFFIX_STR)
  qsCuiFileName.remove(getIApp()->toQString(TD_DLL_VERSION_SUFFIX_STR));
#endif
  qsCuiFileName += ".cui";

  if (m_mapGroupNameByFilePath.find(qsCuiFileName) == m_mapGroupNameByFilePath.end())
    return "";
  return m_mapGroupNameByFilePath[qsCuiFileName];
}

bool OdqCustomization::unloadCustomGroup(const QString& qsGroupName)
{
  ODA_ASSERT_ONCE(!qsGroupName.isEmpty());

  OdSharedPtr<OdqCustomGroup>& pGroup = m_mapCustomGroups[qsGroupName];
  if (pGroup.isNull() || !pGroup->isOn())
    return false;
  
  pGroup->setOn(false);
  return true;
}

// return name of loaded custom group
QString OdqCustomization::loadCustomGroup(const QString& qsXmlFilePath)
{
  QFile file(qsXmlFilePath);
  if (qsXmlFilePath.isEmpty() || !file.open(QFile::ReadOnly | QFile::Text))
    return ""; // file was not found

  QDomDocument docXml;

  QString qsErrorMsg;
  int iErrorLine = 0,
      iErrorColumn = 0;
  if (!docXml.setContent(&file, &qsErrorMsg, &iErrorLine, &iErrorColumn))
  {
    OdString sMsg = getIApp()->toOdString(
                      QObject::tr("%1 - at line %2 column %3 of XML file \"%4\" ")
                          .arg(qsErrorMsg)
                          .arg(iErrorLine)
                          .arg(iErrorColumn)
                          .arg(qsXmlFilePath));
    getIApp()->getBaseHostAppServices()->warning(sMsg);
    return "";
  }

  QDomElement elemCustomGroup = getSingleElement(docXml, "CustSection/MenuGroup[@Name!='']");
  if (elemCustomGroup.isNull())
  {
    ODA_FAIL_ONCE(); // unsupported format
    return "";
  }

  QString qsGroupName = elemCustomGroup.attribute("Name");

  OdSharedPtr<OdqCustomGroup>& pCustomGroup = m_mapCustomGroups[qsGroupName];
  if (!pCustomGroup.isNull())
  {
    if (pCustomGroup->isOn())
    {
      OdString sMsg = getIApp()->toOdString(
                        QObject::tr("Custom group \"%1\" already loaded").arg(qsGroupName));
      getIApp()->getBaseHostAppServices()->warning(sMsg);
      return qsGroupName;
    }

    if (pCustomGroup->isEqualByFileAttribs(qsXmlFilePath))
    {
      pCustomGroup->setOn();
      return qsGroupName;
    }
    //pCustomGroup = NULL;
  }
  else
    m_lstCustomGroups << qsGroupName;

  pCustomGroup = new OdqCustomGroup(qsXmlFilePath);

  QString qsCuiFileName = QFileInfo(qsXmlFilePath).completeBaseName();
#if defined(TD_DLL_VERSION_SUFFIX_STR)
  qsCuiFileName.remove(getIApp()->toQString(TD_DLL_VERSION_SUFFIX_STR));
#endif
  qsCuiFileName += ".cui";
  m_mapGroupNameByFilePath[qsCuiFileName] = qsGroupName;

  // parse MenuMacro records to action map
  QList<QDomElement> lst = getElements(elemCustomGroup, 
                                       "MacroGroup[@Name='User']/MenuMacro[@UID!='']");
  foreach (QDomElement elem, lst)
  {
    ODA_ASSERT_ONCE(!elem.isNull());

    QString qsUID = elem.attribute("UID");

    QString qsCommand; 
    QDomElement elemCmd = getSingleElement(elem, "Macro/Command");
    ODA_ASSERT_ONCE(!elemCmd.isNull());
    if (elemCmd.isNull())
      continue;

    qsCommand = elemCmd.text();

    QString qsIconPath;
    QDomElement elemImage = getSingleElement(elem, "Macro/LargeImage[@Name!='']");
    if (elemImage.isNull())
      elemImage = getSingleElement(elem, "Macro/SmallImage[@Name!='']");
    if (!elemImage.isNull())
      qsIconPath = elemImage.attribute("Name");

    OdSharedPtr<OdqMenuMacro> pMacro;
    if (!qsCommand.isEmpty()) 
      pMacro = new OdqMenuMacro(qsCommand, qsIconPath);
    else
    {
      ODA_ASSERT_ONCE(qsIconPath.isEmpty()); // TODO
      pMacro = OdqMenuMacro::createMenuControlMacro(qsUID, this);
    }

    if (!pMacro.isNull())
    {
      OdSharedPtr<OdqMenuMacro>& pMacro_ = pCustomGroup->getMenuMacro(qsUID); 
      ODA_ASSERT_ONCE(pMacro_.isNull()); // check for duplicates

      pMacro_ = pMacro;
    }
  }

  QDomElement elemDestRoot = pCustomGroup->getXmlDoc().appendChild(
                               pCustomGroup->getXmlDoc().createElement("MenuRoot"))
                                 .toElement();
  foreach(QString qsNode, QStringList() << "PopMenuRoot" << "ToolbarRoot")
  {
    QDomElement elem = getSingleElement(elemCustomGroup, 
                                        QString("MenuRoot/%1").arg(qsNode));
    ODA_ASSERT_ONCE(!elem.isNull());
    if (!elem.isNull())
      elemDestRoot.appendChild(pCustomGroup->getXmlDoc().importNode(elem, true));
  }
  // test
  //ODA_ASSERT_ONCE(saveXml(pCustomGroup->getXmlDoc(),
  //                        QDir(qApp->applicationDirPath())
  //                          .absoluteFilePath("_last_cust_group.xml")));    
  return qsGroupName;
}

//slot 
void OdqCustomization::refresh(const QString& qsKey, const QString& cqsArg)
{
  if (qsKey == "<event-refresh:drawing_history_changed>")
  {
    ODA_ASSERT(m_sizeMaxDrawingHistory > 0 && !cqsArg.isEmpty());
    // qsArg is the lastest path
    if (cqsArg.isEmpty())
      return;
    QString qsArg = cqsArg;
    qsArg.replace('\\', '/');

    for (QStringList::iterator itr = m_lstDrawingHistory.begin(); 
         itr != m_lstDrawingHistory.end();)
    {
      if (qsArg == *itr)
        itr = m_lstDrawingHistory.erase(itr);
      else
        itr++;
    }
    m_lstDrawingHistory.push_front(qsArg);
    if (m_lstDrawingHistory.size() > m_sizeMaxDrawingHistory)
      m_lstDrawingHistory.erase(--m_lstDrawingHistory.end());
    getIApp()->refreshQObjetcts("<event-refresh:drawing_history_update>");
    return;
  }
  if (qsKey == "<event-refresh:toolbarsize_changed>")
  {
    int nSize = 0; // undef
    bool bLarge = getIAppProps()->toBool(OD_T("Vars/largetoolbars"), 
                                         true); // is default for Qt too
    foreach (QPointer<QToolBar> pToolbar, getIApp()->getMainWindow()->findChildren<QToolBar*>()) 
    {
      ODA_ASSERT_ONCE(!pToolbar.isNull());
      if (pToolbar.isNull())
        continue;
      if (!nSize)
      {
        if (!bLarge)
          nSize = pToolbar->style()->pixelMetric(QStyle::PM_SmallIconSize); // = 24
        else
          nSize = pToolbar->style()->pixelMetric(QStyle::PM_LargeIconSize); // = 48 // is default
        if (!nSize)
          return;
      }
      pToolbar->setIconSize(QSize(nSize, nSize));
    }
    return;
  }

  //ODA_ASSERT_ONCE(   qsKey == "<event-refresh:database_changed>"
  //                || qsKey == "<event-refresh:cui_changed>");

  if (!m_mapConditionalTexts.isEmpty())
  {
    // refresh states of widgets (now for menu only)

    foreach (QPointer<QObject> pObject, m_mapConditionalTexts.keys())
    {
      ODA_ASSERT_ONCE(!pObject.isNull()); // test
      if (pObject.isNull())
        continue;
      QMenu* pMenu = qobject_cast<QMenu*>(pObject);
      OdqToolbarControlMacro* pToolbarMacro = NULL;
      QAction* pAction = NULL;
      if (!pMenu)
      {
        pToolbarMacro = qobject_cast<OdqToolbarControlMacro*>(pObject);
        if (pToolbarMacro)
          pAction = pToolbarMacro->action();
        else
          pAction = qobject_cast<QAction*>(pObject);
        ODA_ASSERT_ONCE(pAction);
        if (!pAction)
          continue;
      }

      QString qsTextBase = m_mapConditionalTexts[pObject];
      ODA_ASSERT_ONCE(!qsTextBase.isEmpty());
      if (qsTextBase.isEmpty())
        continue;
      QString qsText = getIConsole()->evaluateDieselExpression(qsTextBase);

      if (pMenu)
      {
        bool bEnabled = true,
             bVisible = true;
        if (!qsText.indexOf("~"))
        {
          qsText = qsText.mid(1);
          bEnabled = false;
          // maybe better :
          //if (pMenu->parent() == getIApp()->getMainWindow()->menuBar())
          //  bVisible = false;
        }
        if (qsText.isEmpty())
        {
          bEnabled = false;
          bVisible = false;
        }
        if (pMenu->isEnabled() != bEnabled)
          pMenu->setEnabled(bEnabled);
        if (!bVisible || pMenu->isVisible() != bVisible)
          pMenu->menuAction()->setVisible(bVisible);
        
        if (pMenu->title() != qsText)
          pMenu->setTitle(qsText);
        continue;
      }
      ODA_ASSERT_ONCE(pAction);

      bool bVisible = true,
           bEnabled = true;
      if (pToolbarMacro)
      {
        bVisible = pToolbarMacro->isVisible();
        bEnabled = pToolbarMacro->isEnabled();
      }
      else
      {
        if (!qsText.indexOf("~"))
        {
          bEnabled = false;
          qsText = qsText.mid(1);
        }
        if (qsText.isEmpty())
        {
          bEnabled = false;
          bVisible = false;
        }
      }

      if (pAction->isEnabled() != bEnabled)
        pAction->setEnabled(bEnabled);
      if (!bVisible || pAction->isVisible() != bVisible)
        pAction->setVisible(bVisible);

      if (pAction->text() != qsText)
        pAction->setText(qsText);

    } // foreach 
  } // if

  if (!m_mapCustomGroups.isEmpty())
  {
    foreach (QString qsGroupName, getCustomGroups())
    {
      OdSharedPtr<OdqCustomGroup>& pCustomGroup = m_mapCustomGroups[qsGroupName];
      ODA_ASSERT_ONCE(!pCustomGroup.isNull() && pCustomGroup->isOn());
      if (pCustomGroup.isNull() || !pCustomGroup->isOn())
        continue;

      pCustomGroup->refreshIconStates();
    }
  }
}

// for menu and toolbars only
void OdqCustomization::refreshCustomGroupWidgets(bool bSkipSaveToolbars) // = false
{
  QMainWindow* pMainWindow = getIApp()->getMainWindow();
  QMenuBar* pMenuBar = pMainWindow->menuBar();
  pMenuBar->clear();

  if (!bSkipSaveToolbars)
    saveSettings(true); // bToolbarsOnly
  // clear all toolbars
  foreach (QPointer<QToolBar> pToolbar, getIApp()->getMainWindow()->findChildren<QToolBar*>()) 
  {
    ODA_ASSERT_ONCE(!pToolbar.isNull());
    if (pToolbar.isNull())
      continue;
    getIApp()->getMainWindow()->removeToolBar(pToolbar);
    if (!pToolbar.isNull())
      delete pToolbar;
  }

  m_mapConditionalTexts.clear();
  QMenu* pVrtMenuBefore = NULL; // is referenced to horizontal menu item "Window" if it was already created
  bool hasToolbars = false;
  foreach (QString qsGroupName, getCustomGroups())
  {
    OdSharedPtr<OdqCustomGroup>& pCustomGroup = m_mapCustomGroups[qsGroupName];
    ODA_ASSERT_ONCE(!pCustomGroup.isNull() && pCustomGroup->isOn());
    if (pCustomGroup.isNull() || !pCustomGroup->isOn())
      continue;

    QDomDocument& docXml = pCustomGroup->getXmlDoc();
    QMenu* pVrtMenuBefore_ = pVrtMenuBefore;

    // add menu items
    QList<QDomElement> lst 
      = getElements(docXml, "MenuRoot/PopMenuRoot/PopMenu");
    foreach (QDomElement elemHor, lst)
    {
      ODA_ASSERT_ONCE(!elemHor.isNull());
      if (getSingleElement(elemHor, "Alias").isNull())
        continue; // skip as vertical referenced menu

      QDomElement elemHorName = getSingleElement(elemHor, "Name");
      QString qsHorNameBase; 
      if (elemHorName.isNull() || (qsHorNameBase = elemHorName.text()).isEmpty())
      {
        ODA_FAIL_ONCE();
        continue;
      }
      QString qsHorName = getIConsole()->evaluateDieselExpression(qsHorNameBase);
      bool bEnabled = true,
           bVisible = true;
      if (!qsHorName.indexOf("~"))
      {
        qsHorName = qsHorName.mid(1);
        bEnabled = false;
      }
      if (qsHorName.isEmpty())
      {
        bEnabled = false,
        bVisible = false;
      }

      QMenu* pVrtMenu = NULL;
      if (!pVrtMenuBefore)
        pVrtMenu = pMenuBar->addMenu(qsHorName);
      else
        pMenuBar->insertMenu(pVrtMenuBefore->menuAction(), (pVrtMenu = new QMenu(qsHorName)));
      if (!pVrtMenuBefore_ && elemHor.attribute("UID") == "ID_MnWindow")
          pVrtMenuBefore_ = pVrtMenu;

      if (!bEnabled)
        pVrtMenu->setEnabled(false);
      if (!bVisible)
        pVrtMenu->menuAction()->setVisible(false);

      if (qsHorName != qsHorNameBase)
        m_mapConditionalTexts[(QObject*) pVrtMenu] = qsHorNameBase;

      fillPopMenuItems(pVrtMenu, elemHor, pCustomGroup);

    } //foreach elemHor
    if (pVrtMenuBefore != pVrtMenuBefore_)
      pVrtMenuBefore = pVrtMenuBefore_;

    // add toolbars
    QSettings settings("ODA", 
                       QFileInfo(QCoreApplication::applicationFilePath()).baseName());
    settings.beginGroup(TD_SHORT_STRING_VER_S);
    lst = getElements(docXml, "MenuRoot/ToolbarRoot/Toolbar");
    foreach (QDomElement elemToolbar, lst)
    {
      ODA_ASSERT_ONCE(!elemToolbar.isNull());
      QString qsVis = elemToolbar.attribute("ToolbarVisible"),
              qsOrient = elemToolbar.attribute("ToolbarOrient");
      bool bEnabled = true,
           bVisible = true;
      enum Qt::ToolBarArea area = Qt::TopToolBarArea;
      if (qsVis == "hide")
      {
        // is used via by Alias
        ODA_ASSERT_ONCE(qsOrient == "floating"); // xval, yval, rows
        QDomElement elemAlias = getSingleElement(elemToolbar, "Alias");
        ODA_ASSERT_ONCE(!elemAlias.isNull());
        if (elemAlias.isNull())
          continue;
        //QString qsAlias = elemAlias.text();
        bVisible = false;
      }
      else
      {
        ODA_ASSERT_ONCE(qsVis == "show");
      }
      if (qsOrient == "top")
        area = Qt::TopToolBarArea;
      else if (qsOrient == "bottom")
        area = Qt::BottomToolBarArea;
      else if (qsOrient == "left")
        area = Qt::LeftToolBarArea;
      else if (qsOrient == "right")
        area = Qt::RightToolBarArea;
      else
      {
        ODA_ASSERT_ONCE(qsOrient == "floating"); // && qsVis == "hide");
      }

      QDomElement elemToolbarName = getSingleElement(elemToolbar, "Name");
      QString qsTollbarNameBase; 
      if (elemToolbarName.isNull() || (qsTollbarNameBase = elemToolbarName.text()).isEmpty())
      {
        ODA_FAIL_ONCE();
        continue;
      }
      QString qsTollbarName = getIConsole()->evaluateDieselExpression(qsTollbarNameBase);
      if (!qsTollbarName.indexOf("~"))
      {
        qsTollbarName = qsTollbarName.mid(1);
        bEnabled = false;
      }
      if (qsTollbarName.isEmpty())
      {
        bEnabled = false,
        bVisible = false;
      }
      hasToolbars = true;

      QToolBar* pToolbar = getIApp()->getMainWindow()->findChild<QToolBar*>(qsTollbarNameBase);
      if (!pToolbar)
      {
        pToolbar = new QToolBar();
        pToolbar->setWindowTitle(qsTollbarName);
        pToolbar->setObjectName(qsTollbarNameBase); // is not empty

        int nSize = 0; // undef
        bool bLarge = getIAppProps()->toBool(OD_T("Vars/largetoolbars"), 
                                             true); // is default for Qt too
        if (!bLarge)
          nSize = pToolbar->style()->pixelMetric(QStyle::PM_SmallIconSize); // = 24
        else
          nSize = pToolbar->style()->pixelMetric(QStyle::PM_LargeIconSize); // = 48 // is default
        if (nSize)
          pToolbar->setIconSize(QSize(nSize, nSize));

        //getIApp()->getMainWindow()->addToolBarBreak(Qt::TopToolBarArea); // do next toolbar on next line
        getIApp()->getMainWindow()->addToolBar(area, pToolbar);
      }
      if (!qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow())
             ->restoreQObjectProperties(pToolbar, "enabled"
            // The default is Qt::Horizontal.
            //  This function should not be used when the toolbar is managed by QMainWindow.
            //You can use QMainWindow::addToolBar() or QMainWindow::insertToolBar() if you wish
            // to move a toolbar (that is already added to a main window) to another Qt::ToolBarArea.
                                                //" orientation"
                                                  " visible"))
      {
        if (!bEnabled)
          pToolbar->setEnabled(false);
        if (!bVisible)
          pToolbar->setVisible(false);
      }

      if (qsTollbarName != qsTollbarNameBase)
        m_mapConditionalTexts[(QObject*) pToolbar] = qsTollbarNameBase;

      fillToolbarButtons(pToolbar, elemToolbar, pCustomGroup);

    } //foreach elemToolbar
    settings.endGroup();

    pCustomGroup->clearUnreferencedMenuMacros();

  } // foreach qsGroupName

  if (hasToolbars)
  {
    QSettings settings("ODA", 
                       QFileInfo(QCoreApplication::applicationFilePath()).baseName());
    QByteArray baWindowState = settings.value(TD_SHORT_STRING_VER_S 
                                              "/objectProps/QToolBar/windowState").toByteArray();
    if (baWindowState.size())
      getIApp()->getMainWindow()->restoreState(baWindowState);
  }

  refresh("<event-refresh:cui_changed>", "");
}

void OdqCustomization::fillPopMenuItems(QMenu* pMenu, 
                                        QDomElement elemPopMenu,
                                        OdqCustomGroup* pCustomGroup)
{
  QList<QDomElement> lstPop = getElements(elemPopMenu, "PopMenu*"); // "PopMenuItem" or "PopMenuRef" 
  foreach (QDomElement elemPop, lstPop)
  {
    ODA_ASSERT_ONCE(!elemPop.isNull());

    QString qsTagName = elemPop.tagName(),
            qsPopNameBase,
            qsPopName;
    if (qsTagName  == "PopMenuRef")
    {
      QString qsUID = elemPop.attribute("pUID");
      QDomNode nodePopMenu;
      if (   qsUID.isEmpty() 
          || (nodePopMenu = elemPop.parentNode()).isNull())
      {
        ODA_FAIL_ONCE();
        continue;
      }

      elemPop = getSingleElement(nodePopMenu.parentNode(), // PopMenuRoot
                                 QString("PopMenu[@UID='%1']").arg(qsUID));
      if (   elemPop.isNull() 
          || !getSingleElement(elemPop, "Alias").isNull()
          || (elemPop = getSingleElement(elemPop, "Name")).isNull()
          || (qsPopNameBase = elemPop.text()).isEmpty()
          || (nodePopMenu = elemPop.parentNode()).isNull())
      {
        ODA_FAIL_ONCE();
        continue;
      }
      qsPopNameBase.replace("\\t", "\t");
      qsPopName = getIConsole()->evaluateDieselExpression(qsPopNameBase);

      bool bEnabled = true,
           bVisible = true;
      if (!qsPopName.indexOf("~"))
      {
        qsPopName = qsPopName.mid(1);
        bEnabled = false;
      }
      if (qsPopName.isEmpty())
      {
        bVisible = false;
        bEnabled = false;
      }

      QMenu* pSubMenu = pMenu->addMenu(qsPopName);
      if (!bEnabled)
        pSubMenu->setEnabled(false);
      if (!bVisible)
        pSubMenu->menuAction()->setVisible(false);
      
      if (qsPopName != qsPopNameBase)
        m_mapConditionalTexts[(QObject*) pSubMenu] = qsPopNameBase;

      fillPopMenuItems(pSubMenu, nodePopMenu.toElement(), pCustomGroup);
      continue;
    }
    ODA_ASSERT(qsTagName == "PopMenuItem");

    bool bSeparator = false;
    QString qsIsSeparator = elemPop.attribute("IsSeparator");
    if (!qsIsSeparator.isEmpty())
       bSeparator = QVariant(qsIsSeparator).toBool();
    else
      ODA_FAIL_ONCE();

    if (bSeparator)
    {
      pMenu->addSeparator();
      continue;
    }

    QDomElement elemPopName = getSingleElement(elemPop, "NameRef");
    if (   elemPopName.isNull()
        || (qsPopNameBase = elemPopName.text()).isEmpty())
    {
      ODA_FAIL_ONCE();
      continue;
    }
    qsPopNameBase.replace("\\t", "\t");
    qsPopName = getIConsole()->evaluateDieselExpression(qsPopNameBase);

    QDomElement elemMacroRef 
      = getSingleElement(elemPop, "MenuItem/MacroRef[@MenuMacroID!='']");
    if (elemMacroRef.isNull())
    {
      ODA_FAIL_ONCE();
      continue;
    }
    QString qsUID = elemMacroRef.attribute("MenuMacroID");

    OdSharedPtr<OdqMenuMacro>& pMacro = pCustomGroup->getMenuMacro(qsUID); 
    if (pMacro.isNull())
    {
      ODA_FAIL_ONCE();
      continue;
    }

    if (!pMacro->getCommand(false).isEmpty())
    {
      bool bEnabled = true,
           bVisible = true;
      if (!qsPopName.indexOf("~"))
      {
        qsPopName = qsPopName.mid(1);
        bEnabled = false;
      }
      if (qsPopName.isEmpty())
      {
        bVisible = false;
        bEnabled = false;
      }

      QAction* pAction = pMenu->addAction(qsPopName);
      pMacro->setAction(pAction);
      if (!bEnabled)
        pAction->setEnabled(false);
      if (!bVisible)
        pAction->setVisible(false);

      if (qsPopName != qsPopNameBase)
        m_mapConditionalTexts[(QObject*) pAction] = qsPopNameBase;
      continue;
    }

    OdqMenuControlMacro* pMenuControlMacro = qobject_cast<OdqMenuControlMacro*>(pMacro);
    ODA_ASSERT_ONCE(pMenuControlMacro);
    if (!pMenuControlMacro)
      continue;

    if (pMenuControlMacro->isMenu())
    {
      // registered commands
      QMenu* pSubMenu = pMenu->addMenu(qsPopName);
      pMacro->setAction(pSubMenu->menuAction());
    }
    else
    {
      // drawing history (qsUID == "ID_MRU"), windows etc
      QAction* pAction = pMenu->addAction(qsPopName);
      pMacro->setAction(pAction);
    }
    pMenuControlMacro->refresh(pMenuControlMacro->getEventKey(), ""); // refreshDrawingHistory();

  } // foreach elemPop
}

void OdqCustomization::fillToolbarButtons(QToolBar* pToolbar, 
                                          QDomElement elemToolbar,
                                          OdqCustomGroup* pCustomGroup)
{
  QList<QDomElement> lstButtons = getElements(elemToolbar, "Toolbar*"); // "ToolbarButton" or "ToolbarControl"
  foreach (QDomElement elemButton, lstButtons)
  {
    ODA_ASSERT_ONCE(!elemButton.isNull());

    QString qsTagName = elemButton.tagName(),
            qsUID = elemButton.attribute("UID");

    if (qsTagName == "ToolbarFlyout")
    {
      // references to other toolbars are unsupported now // TODO
      ODA_ASSERT_ONCE(elemButton.attribute("UseOwnIcon") == "false"); // TODO
      //QString qsAlias = elemButton.attribute("pTargetAlias");
      continue;
    }
    if (qsTagName == "ToolbarButton")
    {
      bool bSeparator = false;
      QString qsIsSeparator = elemButton.attribute("IsSeparator");
      if (!qsIsSeparator.isEmpty())
        bSeparator = QVariant(qsIsSeparator).toBool();
      else
        ODA_FAIL_ONCE();

      if (bSeparator)
      {
        pToolbar->addSeparator();
        continue;
      }

      QDomElement elemButtonName = getSingleElement(elemButton, "Name");
      QString qsButtonNameBase;
      if (   elemButtonName.isNull()
          || (qsButtonNameBase = elemButtonName.text()).isEmpty())
      {
        ODA_FAIL_ONCE();
        continue;
      }
      qsButtonNameBase.replace("\\t", "\t");
      QString qsButtonName = getIConsole()->evaluateDieselExpression(qsButtonNameBase);

      QDomElement elemMacroRef 
        = getSingleElement(elemButton, "MenuItem/MacroRef[@MenuMacroID!='']");
      if (elemMacroRef.isNull())
      {
        ODA_FAIL_ONCE();
        continue;
      }
      qsUID = elemMacroRef.attribute("MenuMacroID");

      OdSharedPtr<OdqMenuMacro>& pMacro = pCustomGroup->getMenuMacro(qsUID); 
      if (pMacro.isNull())
      {
        ODA_FAIL_ONCE();
        continue;
      }

      if (!pMacro->getCommand(false).isEmpty())
      {
        bool bEnabled = true,
             bVisible = true;
        if (!qsButtonName.indexOf("~"))
        {
          qsButtonName = qsButtonName.mid(1);
          bEnabled = false;
        }
        if (qsButtonName.isEmpty())
        {
          bVisible = false;
          bEnabled = false;
        }

        QAction* pAction = pToolbar->addAction(qsButtonName);
        pMacro->setAction(pAction);
        if (!bEnabled)
          pAction->setEnabled(false);
        if (!bVisible)
          pAction->setVisible(false);

        if (qsButtonName != qsButtonNameBase)
          m_mapConditionalTexts[(QObject*) pAction] = qsButtonNameBase;
        continue;
      }
    }

    if (qsTagName == "ToolbarControl")
    {
      QString qsControl = elemButton.attribute("Control");
      OdSharedPtr<OdqMenuMacro> pMacro = pCustomGroup->getMenuMacro(qsUID, qsControl);
      if (pMacro.isNull() || pMacro->getCommand(false).isEmpty())
        continue; // unsupported now or invalid
      OdqToolbarControlMacro* pToolbarMacro = qobject_cast<OdqToolbarControlMacro*>(pMacro.get());
      ODA_ASSERT_ONCE(pToolbarMacro);
      if (!pToolbarMacro)
        continue;
      QAction* pAction = pToolbar->addAction(pToolbarMacro->text());
      pMacro->setAction(pAction);
      if (!pToolbarMacro->isEnabled())
        pAction->setEnabled(false);
      if (!pToolbarMacro->isVisible())
        pAction->setVisible(false);

      // refresh directly via events //m_mapConditionalTexts[(QObject*) pMacro.get()] = pToolbarMacro->text(false); // always conditional
      continue;
    }

    ODA_FAIL_ONCE(); // test

  } // foreach elemButton
}

