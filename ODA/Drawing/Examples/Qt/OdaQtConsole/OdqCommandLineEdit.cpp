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
// OdqCommandLineEdit.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif
#include <QStringList>
#include <QSettings>

#include "OdaCommon.h"
#include "OdqCommandLineEdit.h" 
#include "OdqConsole.h" // for command map

//////////////////////////////////////////////////////////////////////////
static QMap<THintType, OdSharedPtr<QStringList> > s_mapData;
static int s_sizeMaxHistory = 15;

QString hintTypeToString(THintType tp)
{
  switch (tp)
  {
  case enHintCommandHistory: return "command";
  case enHintString: return "string";
  case enHintTrackingString: return "text";
  case enHintKeyword: return "keyword";
  case enHintPoint: return "point";
  case enHintDistance: return "distance"; 
  case enHintAngle: return "angle";
  case enHintInt: return "int";
  case enHintReal: return "real";
  case enHintColor: return "color";
  }
  ODA_FAIL_ONCE(); //TODO
  return "unknown";
}

OdqCommandLineEdit::OdqCommandLineEdit(const TCmdMap& mapCmds,
                                       QWidget* pParent) // = NULL
  : QLineEdit(pParent)
  , m_mapCmds(mapCmds)
  , m_sizeCmdMapAtUpdate(0) // non-updated
  , m_mapData(s_mapData)
  , m_pCompleter(NULL) //(new QCompleter())
#if !defined(ANDROID)
  , m_mode(enHintMode_Popup)
#else
  , m_mode(enHintMode_None) // (enHintMode_Inline)
#endif
  , m_type(enHintNone)
  , m_keyPressed(0)
{
  if (s_mapData.isEmpty())
  {
    QSettings settings("ODA", 
                       QFileInfo(QCoreApplication::applicationFilePath()).baseName());
    settings.beginGroup(TD_SHORT_STRING_VER_S);

    s_sizeMaxHistory = settings.value("console/history/max", 15).toInt();
    int count = settings.value("console/history/count", 0).toInt() + enHintCommandHistory;
    if (count > enHintCommandHistory && s_sizeMaxHistory > 0)
    {
      for (THintType tp = enHintCommandHistory; tp < count && tp < enHintCount; tp = THintType(tp + 1))
      {
        for (int index = 0; index < count; index++)
        {
          QString qsText = settings.value(QString("console/history/%1/%2").arg(tp - enHintCommandHistory).arg(index), 
                                          "").toString();
          if (qsText.isEmpty())
            break;
          OdSharedPtr<QStringList>& pList = s_mapData[tp];
          if (pList.isNull())
            pList = new QStringList();
          
          *pList << qsText;
        }
      }
    }

    settings.endGroup();
  }

  if (m_mode == enHintMode_None)
    return;

  if (!connect(this, SIGNAL(textChanged(const QString &)),
               this, SLOT(changed(const QString&)), 
               Qt::QueuedConnection))
  {
    ODA_FAIL_ONCE();
  }
}

OdqCommandLineEdit::~OdqCommandLineEdit()
{
}

void OdqCommandLineEdit::saveSettings()
{
  if (s_mapData.size() <= enHintCommandHistory)
    return;
  int count = s_mapData.size();
  ODA_ASSERT_ONCE(count < enHintCount);
  while (count > 0 && s_mapData[THintType(count - 1)].isNull())
    count--; 

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S);
  settings.setValue("console/history/max", s_sizeMaxHistory);
  settings.setValue("console/history/count", count - enHintCommandHistory);

  for (THintType tp = enHintCommandHistory; tp < count && tp < enHintCount; tp = THintType(tp + 1))
  {
    settings.setValue(QString("console/history/%1/type").arg(tp - enHintCommandHistory), 
                      hintTypeToString(tp));
    int index = 0;
    OdSharedPtr<QStringList>& pList = s_mapData[tp];
    if (!pList.isNull())
    {
      foreach (QString qsText, *pList)
      {
        if (qsText.isEmpty() || index >= s_sizeMaxHistory)
          break;
        settings.setValue(QString("console/history/%1/%2").arg(tp - enHintCommandHistory).arg(index++), 
                          qsText);
      }
    }
    settings.setValue(QString("console/history/%1/%2").arg(tp - enHintCommandHistory).arg(index), 
                      "");
  }

  settings.endGroup();
}

void OdqCommandLineEdit::keyPressEvent(QKeyEvent* pEvent)
{
  QString qsTxt = text();
  switch (m_keyPressed = pEvent->key())
  {
  // processHistory
  case Qt::Key_Enter:
  case Qt::Key_Return:
    //if (!qsTxt.isEmpty())
      addToHistory(qsTxt);
    //else
#ifdef Q_OS_LINUX
      clearFocus(); // reset compliter
#endif
    break;
  //processArrowKeys
  case Qt::Key_Up:
  case Qt::Key_Down:
    if (qsTxt.isEmpty())
      changed("");
    break;
  }

  QLineEdit::keyPressEvent(pEvent);
}

void OdqCommandLineEdit::keyReleaseEvent(QKeyEvent* pEvent)
{
  m_keyPressed = 0;
  QLineEdit::keyReleaseEvent(pEvent);
}

QString extractCommandPrefix(QString& qsCmd) // in / out
{
  // prefix characters : 
  //  "'" transparency prefix (invokes a command during execution of another command - e.g. '_HELP) 
  //  "-" commandline prefix (invokes a non-dialog version of a command - e.g. _-LAYER) 
  //  "_" non-localized command prefix (original english command names in a localized version of AutoCAD - e.g. _LINE) 
  //  "." non-redefined command prefix (e.g. for commands undefined using _UNDEFINE, example: ._LINE or _.LINE) 
  //  "+" dialog tab prefix (for selecting a particular dialog tab - e.g. '._+DSETTINGS), or a special command mode (e.g. +PUBLISH) 
  //
  // To list all commands which can be prefixed with "+", type "+" on the commandline 
  //  and press the TAB key repeatedly (since version 2006).

  QString qsPrefix;

  while (!qsCmd.isEmpty())
  {
    QChar ch = qsCmd[0];
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    switch (ch.toAscii())
#else
    switch (ch.toLatin1())
#endif
    {
    default:
      return qsPrefix;
    case ' ': case '\t':
      break;
    case '\'': case '-': case '_': case '.': case '+':
      qsPrefix += ch;
      break;
    }
    qsCmd = qsCmd.mid(1);
  }
  return qsPrefix;
}

// slot
void OdqCommandLineEdit::changed(const QString& qsTxt)
{
  bool isTypeCheckedByText = false;
  bool isCompleterVisible =    !m_pCompleter.isNull() && m_pCompleter->popup() 
                            && m_pCompleter->popup()->isVisible();

  if (isCompleterVisible)
  {
    QString qsPrev = text();

   //#ifdef QT_DEBUG
   // if (!qsTxt.indexOf("s"))
   //   qsPrev = qsPrev; // brk
   //#endif

    if (isCompleterVisible && (qsTxt.isEmpty() || qsTxt != qsPrev))
      return;
    if (isCompleterVisible && m_type == enHintCommand)
      return;
  }
  if (!m_pCompleter.isNull() && !qsTxt.isEmpty()) // isCompleterVisible
  {
    QString qsCurr = m_pCompleter->currentCompletion().toLower();
    //if (!qsCurr.indexOf(qsTxt))
    //  return;
    if (qsCurr.indexOf(qsTxt))
    {
      OdSharedPtr<QStringList>& pList = m_mapData[m_type];
      if (pList.isNull())
        return;

      int idx = -1,
          index = -1;
      foreach (QString qsText, *pList)
      {
        idx++;
        if (qsText.toLower().indexOf(qsTxt))
          continue;
        index = idx;
        break;
      }
      if (index >= 0)
        return;
      
      switch (m_type)
      {
      case enHintCommand:
        return;
      case enHintCommandHistory:
        setType(enHintCommand);
        break;
      default:
        if (isCompleterVisible)
          updateCompleter();
        break;
      }
    }
    isTypeCheckedByText = true;
  }

  if (m_keyPressed == Qt::Key_Enter || m_keyPressed == Qt::Key_Return)
    return;

  if (!isTypeCheckedByText)
  {
    if (!qsTxt.isEmpty())
    {
      ODA_ASSERT_ONCE(m_keyPressed != Qt::Key_Up && m_keyPressed != Qt::Key_Down); // test
      //if (m_keyPressed == Qt::Key_Up || m_keyPressed == Qt::Key_Down)
      //  return;

      if (m_type == enHintNone || m_type == enHintCommandHistory || m_pCompleter.isNull())
        setType(enHintCommand); // updateCompleter();
    }
    else
    {
      if (m_type == enHintCommandHistory && m_pCompleter.isNull())
        setType(enHintCommandHistory); // updateCompleter();

      if (m_type == enHintNone || m_type == enHintCommand)
      {
        if (m_keyPressed != Qt::Key_Up && m_keyPressed != Qt::Key_Down)
          return;
        setType(enHintCommandHistory); // updateCompleter();
      }
    }
  }
  if (!m_pCompleter.isNull())
  {
    // force show
    //emit m_pCompleter->activated(qsTxt);
    if (m_type == enHintCommand) // || !qsTxt.isEmpty())
    {
      //if (m_mode == enHintMode_Inline)
      //  m_pCompleter->setCompletionMode(QCompleter::InlineCompletion);
      //else
      //  m_pCompleter->setCompletionMode(QCompleter::PopupCompletion);
      m_pCompleter->setCompletionPrefix(qsTxt);
    }
    else // if (qsTxt.isEmpty())
    {
      //m_pCompleter->setCompletionPrefix("");
      //m_pCompleter->setModelSorting(QCompleter::UnsortedModel);
      m_pCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    }

    ODA_ASSERT_ONCE(!qsTxt.isEmpty() || m_pCompleter->completionCount());
    m_pCompleter->complete(); 
  }
}

bool OdqCommandLineEdit::updateCompleter()
{
  // !!! TODO
  //if (m_type == enHintNone)
  //  return false;

  THintType tp = m_type;
  if (tp == enHintNone)
    tp = enHintCommand;

  OdSharedPtr<QStringList>& pList = m_mapData[tp];
  if (tp == enHintCommand && m_sizeCmdMapAtUpdate != m_mapCmds.size())
  {
    m_sizeCmdMapAtUpdate = m_mapCmds.size();
    if (m_sizeCmdMapAtUpdate)
    {
      pList = new QStringList();
      for (TCmdMap::const_iterator itr = m_mapCmds.constBegin();
           itr != m_mapCmds.constEnd(); itr++)
      {
        QString qsVal = itr.key();
        *pList << qsVal;
      }
    }
  }

  if (!pList.isNull() && pList->size())
  {
    m_pCompleter = new QCompleter(*pList, NULL); //pParent);
    m_pCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    if (tp == enHintCommand)
    {
      if (m_mode == enHintMode_Inline)
        m_pCompleter->setCompletionMode(QCompleter::InlineCompletion);
    }
    else
    {
      m_pCompleter->setModelSorting(QCompleter::UnsortedModel);
      m_pCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    }

    // unpredictable crash via long input in ::clear method
    // (suspicion to side effect of = new QCompleter
    //
    // try to do it via QStringListModel or QStandardItemModel
    // http://doc.trolltech.com/4.3/tools-completer.html
    //
    // m_pCompleter->setModel(new QStringListModel(*pList, m_pCompleter));
    // m_pCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    // setCompleter(m_pCompleter);
  }
  else
    m_pCompleter = NULL;

  if (completer() == m_pCompleter)
    return false;
  setCompleter(m_pCompleter);
  return true;
}

void OdqCommandLineEdit::addToHistory(const QString& qsData)
{
  if (qsData.isEmpty())
    return;

  THintType tp = m_type;
  if (tp == enHintNone || tp == enHintCommand) // || tp == enHintDwgCommand || tp == enHintDgnCommand)
    tp = enHintCommandHistory;

  OdSharedPtr<QStringList>& pList = m_mapData[tp];
  if (pList.isNull())
    pList = new QStringList();
  
  if (   !pList->size()
      || qsData != pList->last()) // skip duplicates
  {
    int pos;
    while ((pos = pList->indexOf(qsData)) >= 0)
      pList->removeAt(pos);

    pList->push_front(qsData);
    if (pList->size() > s_sizeMaxHistory)
      pList->removeLast();
  }
}

void OdqCommandLineEdit::clear()
{
  //if (text().isEmpty())
  //  return;
  QString qsPrev = text();
  QLineEdit::clear(); //setText("");

  if (completer())
  {
    setCompleter(NULL);
    m_pCompleter = NULL;
  }
  if (!qsPrev.isEmpty() && !hasFocus())
    setFocus();
}

void OdqCommandLineEdit::clearFocus()
{
  //if (text().isEmpty())
  //  return;
  QString qsPrev = text();
 // QLineEdit::clear(); //setText("");

  if (completer())
  {
    setCompleter(NULL);
    m_pCompleter = NULL;
  }
  if (!qsPrev.isEmpty() && !hasFocus())
    setFocus();
}
THintMode OdqCommandLineEdit::setMode(THintMode mode)
{
  THintMode modePrev = m_mode;
  m_mode = mode;
  updateCompleter();
  return modePrev;
}

THintType OdqCommandLineEdit::setType(THintType type)
{
  THintType typePrev = m_type;
 #ifdef QT_DEBUG
  if (m_type != type && type == enHintCommand)
    m_type = m_type;
  if (m_type != type)
 #endif
    m_type = type;
  updateCompleter();
  return typePrev;
}

THintType OdqCommandLineEdit::type(bool bSkipCommandHistoty) const // = true
{
  if (bSkipCommandHistoty && m_type == enHintCommandHistory)
    return enHintCommand;
  return m_type;
}
