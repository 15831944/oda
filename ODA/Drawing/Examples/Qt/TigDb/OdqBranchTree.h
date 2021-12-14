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
// OdqBranchTree.h
//

#ifndef ODA_QT_BRANCH_TREE_H_
#define ODA_QT_BRANCH_TREE_H_

#include <QTreeWidget>
//QT_BEGIN_NAMESPACE
//  class QPainter;
//QT_END_NAMESPACE

#include "OdaCommon.h"
#include "RevisionController.h"

//class OdqBranchModel : public QAbstractItemModel
//{
//  Q_OBJECT
//
//public:
//  OdqBranchModel(const QString& qsRootPath = "");
//  virtual ~OdqBranchModel();
//
//  //const QModelIndex& activeIndex();    
//  QModelIndex modelIndex(const QString& qsPathName);
//  QModelIndex rootIndex() const; // is base path
//
//  //void setWidget(QWidget* pWidget);
//  //QWidget* widget() const;
//
//private:
//  Q_DISABLE_COPY(OdqBranchModel)
//};

class OdqBranchTree : public QTreeWidget  // : public QTreeView
{
  Q_OBJECT

public:
  OdqBranchTree(QWidget* pParent = NULL);
  virtual ~OdqBranchTree();
  void saveSettings();
  void restoreSettings();

  QPoint mousePos() const;
  void updateGraphData(OdStringArray* pLstBranchNames, OdCommitDescriptionList* pLstRevisions, const OdString& sBranchOnly);

  //void setModel(QAbstractItemModel* pModel);
  //int necessaryHeight(const QStyleOptionViewItem& options, 
  //                    const QModelIndex& index) const;

protected:
  virtual void drawRow(QPainter* pPainter, 
                       const QStyleOptionViewItem& options, 
                       const QModelIndex& index) const;
  //virtual void drawBranches(QPainter* pPainter,
  //                          const QRect& rect,
  //                          const QModelIndex& index) const;

  //virtual void mouseMoveEvent(QMouseEvent* pEvent);
  virtual void mouseReleaseEvent(QMouseEvent* pEvent);
  virtual void mousePressEvent(QMouseEvent* pEvent);
  //virtual bool event(QEvent* pEvent);

private:
  //QModelIndex m_idxPicture;

  QPoint m_posMouse; //QPoint m_mousePos;
  OdStringArray* m_pLstBranchNames;
  OdString m_sBranchOnly; // for all if it is an empty
  OdCommitDescriptionList* m_pLstRevisions;
  QList<QPixmap> m_pixmaps;

  //bool m_bTracking;

  Q_DISABLE_COPY(OdqBranchTree)
};

#endif // ODA_QT_BRANCH_TREE_H_
