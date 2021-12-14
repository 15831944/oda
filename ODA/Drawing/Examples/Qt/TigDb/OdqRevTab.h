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
// OdqRevTab.h
//

#ifndef ODA_QT_REV_TAB_H_
#define ODA_QT_REV_TAB_H_

#include <QWidget> // <QObject>
#include <QPointer> 
QT_BEGIN_NAMESPACE
class QComboBox;
class QTreeWidget;
class QGroupBox;
class QPushButton;
class QSlider;
class QCheckBox;
class QBrush;
class QLabel;
class QTableWidget;
QT_END_NAMESPACE

#include "OdaCommon.h"
#include "SharedPtr.h"

#include "OdqInterfaces.h"
#include "OdqBranchTree.h"

#include "RevisionController.h"
#include "OdqRevDiffHelper.h"

//////////////////////////////////////////////////////////////////////////

class OdqRevTab : public QWidget
                , public IqRevDiff
{
  Q_OBJECT
  Q_INTERFACES(IqRevDiff)

  OdqRevTab(QWidget* pParent, OdRxObject* pRxDb);
  friend class OdqRevSystem; 
  
  void init();
public:
  void uninit();
  bool isInitialized() const;

  class OdqRevisionController* controller();

  virtual ~OdqRevTab();

  // ----- IqRevDiff -----

  virtual OdSmartPtr<OdGsDevice> setupLayoutDiff(OdRxObject* pRxDb, OdGsDevice* pDevice, OdGiDefaultContext* pGiCtx);
  virtual void syncView(OdRxObject* pRxDb, OdGsDevice* pDevice);
  virtual void closeDiffView(OdRxObject* pRxDb);
  virtual void setVisualStyle( const OdString& );

  // ----- 

  OdRxObject* getRxDatabase();
  //OdEdCommandContext* commandContext();

  void updateBranches(bool bKeepCurrent = true);
  void updateRevisions();
  void updateChanges(bool bSliderOnly = false);

  bool isResposibleDiffView(IqView* iDiffView);

  OdString currentBranchName(); // in combo (an empty for All)
  const OdStringArray& branchNames();

  OdCommitDescription* checkedOutRevision();
  OdCommitDescription* topRevisionpOf(const OdString& sBranch);

  const OdString& lastCreatedBranchName();
  void setLastCreatedBranchName(const OdString& sName);

public slots:
  void currentBranchIndexChanged(const QString& qsText);

  void currentTreeItemChanged(QTreeWidgetItem* pTreeItem);
  void currentTreeitemPressed(QTreeWidgetItem* pTreeItem, int column);
  void doubleClickedTreeItem(QTreeWidgetItem* pTreeItem, int column);
  void menuTriggered(QAction* pAction);

  void clickedBackButton();
  void diffOnlyChanged(bool checked);
  void diffSlidervalueChanged(int value);

  void clickedSwitchRevButton();
  void clickedPropertyButton();
  void clickedCommitButton();
  void clickedDiffButton();

private:
  bool getIndexesToDiff(OdInt32& idxFirst, OdInt32& idxSecond);

  QTreeWidgetItem* branchItemByIndex(OdInt32 index);
  OdTfDigest* digestByIndex(OdInt32 index);

  void setGearVisible(OdInt32 index, bool bVal);

  OdRxObject* m_pRxDb;
  // for diffs:
  OdDbObjectPtr m_pRxFirstDb,
                m_pRxSecondDb;

  OdStringArray m_lstBranchNames;
  OdCommitDescriptionList m_lstRevisions;

  QPointer<QLabel> m_pUnsuppotLabel;

  QPointer<QLabel> m_pBranchLabel;
  QPointer<QComboBox> m_pBranchCombo;
  QPointer<OdqBranchTree> m_pBranchTree;
  QPointer<QMenu> m_pMenu;

  QPointer<QGroupBox> m_pChangesGroup;
  QPointer<QPushButton> m_pBackButton,
                        m_pDiffButton;
  QPointer<QSlider> m_pDiffSlider;
  QPointer<QCheckBox> m_pDiffCheckBox;

  QPointer<QLabel> m_pChangesTitle;
  QPointer<QTableWidget> m_pChangesTable;

  OdInt32 m_curRevIndex, // checked out
          m_curSelIndex,
          m_curPropIndex, // by menu properties (compare with prev)
          m_curDiffIndex;
  bool m_isCompareMode; // by menu compare
  IqView* m_iDiffView;
  OdSharedPtr<QBrush> m_pDefBrush;
  OdString m_sLastCreatedBranch;

  OdqRevDiffHelper m_rdHelper;
};

#endif // ODA_QT_REV_TAB_H_
