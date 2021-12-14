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
// OdqCommandLineEdit.h
//

#ifndef ODA_QT_COMMAND_LINE_EDIT_H_
#define ODA_QT_COMMAND_LINE_EDIT_H_

#include <QObject>
#include <QLineEdit>
#include <QPointer>
#include <QCompleter>

QT_BEGIN_NAMESPACE
  class QCompleter;
  class QStringList;
QT_END_NAMESPACE

#include "OdaCommon.h"
#include "OdqInterfaces.h"
#include "SharedPtr.h"

//////////////////////////////////////////////////////////////////////////

typedef enum {
  enHintMode_None
, enHintMode_Popup
, enHintMode_Inline
} THintMode;

typedef enum {
  enHintNone = -1
, enHintCommand = 0
, enHintCommandHistory
, enHintString
, enHintTrackingString
, enHintKeyword
, enHintPoint
, enHintDistance 
, enHintAngle
, enHintInt
, enHintReal
, enHintColor
  // etc
, enHintCount
} THintType;

class OdqCommandLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  OdqCommandLineEdit(const TCmdMap& mapCmds, QWidget *parent = NULL);
  virtual ~OdqCommandLineEdit();
  static void saveSettings();

  THintMode setMode(THintMode mode);
  // return prev value
  THintType setType(THintType type);
  THintType type(bool bSkipCommandHistoty = true) const;
  void addToHistory(const QString& qsData);

protected:
  virtual void keyPressEvent(QKeyEvent* pEvent);
  virtual void keyReleaseEvent(QKeyEvent* pEvent);

public slots:
  void clear();
  void changed(const QString &);
  void clearFocus();

private:
  bool updateCompleter();

  const TCmdMap& m_mapCmds;
  int m_sizeCmdMapAtUpdate;

  QPointer<QCompleter> m_pCompleter; // OdSharedPtr<QCompleter>
  QMap<THintType, OdSharedPtr<QStringList> >& m_mapData;

  THintMode m_mode;
  THintType m_type;
  int m_keyPressed;
};

#endif // ODA_QT_COMMAND_LINE_EDIT_H_
