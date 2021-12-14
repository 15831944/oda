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
// OdqViewSystem.h
//

#ifndef ODA_QT_VIEW_SYSTEM_H_
#define ODA_QT_VIEW_SYSTEM_H_

#include <QMultiMap>
#include <QPointer>

QT_BEGIN_NAMESPACE
  class QMdiArea;
  class QMdiSubWindow;
QT_END_NAMESPACE

#include "OdaCommon.h"
#include "DbEntity.h"
#include "Gs/Gs.h"
class OdDbStub; //#include "OdDbStub.h"

#include "OdqInterfaces.h"
class OdqView;

//////////////////////////////////////////////////////////////////////////

class OdqViewSystem : public QObject
                    , public IqViewSystem
{
  Q_OBJECT
  Q_INTERFACES(IqViewSystem)

  OdqViewSystem(); 
public:
  static OdqViewSystem* create();
  virtual ~OdqViewSystem();

  // ----- IqViewSystem -----

  virtual OdGsModulePtr getGsModule(OdRxObject* pRxDatabase);

  virtual IqView* createViews(OdRxObject* pRxDatabase);
  virtual IqView* getView(OdRxObject* pRxDatabase, bool bDoActive = false, bool* pIsSingle = NULL);
  virtual QList<IqView*> getOpenViews(OdRxObject* pRxDatabase);
  // return NULL if point out of view widget
  virtual IqView* getViewByScreenPoint(QPoint& point, // in[screen] // out[view] 
                                       bool bDoActive = false,
                                       bool bDwgOnly = true); 
  // repaint
  virtual void updateGL(OdRxObject* pRxDatabase, bool bActiveOrFirstOnly = false);
  QMdiArea* getMdiArea();
  
  virtual bool updateViews(OdRxObject* pRxDatabase);
  virtual bool closeViews(OdRxObject* pRxDatabase);
  virtual bool closeAllViews(QList<OdRxObject*>* pLstExceptDatabases = NULL);
  virtual bool markToCloseAppByLastView(); // return false if opened views are missing
  virtual void checkMarkToCloseApp();

  // add / remove :NN suffix in window titles
  virtual void updateViewTitles(OdRxObject* pRxDatabase,
                                bool bForce = false);
  virtual bool setActiveViewbyTitle(const OdString& sTitle);

  virtual bool isMaximized(); // state of QMdiSubWindow

  // ----- OdqViewSystem -----

  bool isValid();
  void saveSettings();

  // executed via close event of OdqView
  // and execute closeDatabase by last view of database
  bool removeView(OdqView* pView);

  virtual bool updateObject(OdDbStub* id,
                            OdRxObject* pRxDatabase = NULL); // via baseDatabaseBy(id)
  virtual bool highlightObject(bool bValue, OdDbStub* id, const OdDbBaseFullSubentPath* pPath = NULL,
                               OdRxObject* pRxDatabase = NULL); // via baseDatabaseBy(id)
  virtual bool onModifiedGrip(OdGiDrawablePtr pGripData, 
                              OdRxObject* pRxDatabase);
  virtual bool highlightGripPoint(bool bValue, OdGiDrawablePtr pGripData,
                                  OdRxObject* pRxDatabase);
  virtual bool highlightSnapPoint(bool bValue, OdDbStub* id, const OdDbBaseFullSubentPath* pPath = NULL,
                                  OdDb::OsnapMode osm = OdDb::OsnapMode(0), 
                                  const OdGePoint3d& point = OdGePoint3d(),
                                  OdRxObject* pRxDatabase = NULL); // via baseDatabaseBy(id)

protected:
  OdqView* createView(OdRxObject* pRxDatabase, OdDbStub* idLayoutView = NULL, const OdGsDCRect* pRect = NULL);
  bool postCreateView(OdRxObject* pRxDatabase, OdDbStub* idLayoutView, const OdGsDCRect* pRect);

  void updateViewTitles(OdRxObject* pRxDatabase,
                        QList<QPointer<OdqView> >& lst,
                        bool bForce = false);

public slots:
  void postOperationsSlot();
private slots:
  void activateMdiSubWindow(QMdiSubWindow* pWindow);
  bool deleteView();

signals:
  void postOperations();

private:
  // return false to keep operation in query (usable for enPO_PostCreateView)
  bool doOperations(const class PostOperation& operation, bool bUpdateGl = true);

  bool m_bGlGsChecked;
  OdGsModulePtr m_pGlGs;
  bool m_bGlesGsChecked;
  OdGsModulePtr m_pGlesGs;
  bool m_bCausticGsChecked;
  OdGsModulePtr m_pCausticGs; // OpenRL realtime render gs (for models only)

  OdString m_sDeviceReduction;
  OdGsModulePtr m_pDeviceGs;

  QPointer<QMdiArea> m_pMdiArea;

  typedef QMultiMap<class OdRxObject*,
                  //OdSharedPtr<class OdqView> > TViewMap;
                    QPointer<OdqView> > TViewMap;
  TViewMap m_mapViews;
  bool m_markToCloseAppByLastView;
};

#endif // ODA_QT_VIEW_SYSTEM_H_
