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
// OdqView.h
//

#ifndef ODA_QT_VIEW_H_
#define ODA_QT_VIEW_H_


#include <QMdiSubWindow>
#include <QScrollArea>
#include <QGroupBox>
#include <QGLWidget>
#include <QPointer>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtWidgets>
#include <QGesture>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
#include <QOpenGLWidget>
#endif

QT_BEGIN_NAMESPACE
  class QGroupBox;
  class QTabBar; //class QTabWidget;
  class QDialog;
QT_END_NAMESPACE

#include "OdaCommon.h"
#include "DbEntity.h"
#include "Gs/Gs.h"
#include "Ed/EdUserIO.h"

#include "DbUserIO.h" // TODO DbBaseDatabaseIO.h like DbBaseDatabase.h //#include "Ed/EdUserIO.h"
#define OdDbBaseDatabaseIO OdDbUserIO
typedef OdSmartPtr<OdDbBaseDatabaseIO> OdDbBaseDatabaseIOPtr;

class OdDbStub; //#include "OdDbStub.h"

#include "OdqInterfaces.h"

//////////////////////////////////////////////////////////////////////////

typedef enum {
  enBusy_UninitializedGL = 0 // step 1 - init
, enBusy_InitializeGL
, enBusy_UnresizedGL     // step 2 - resize
, enBusy_ResizeGL
, enBusy_UnpaintGL       // step 3 - paint
, enBusy_PaintGL
, enBusy_Free            // ready to repaint
, enBusy_Closed
} TEnViewBusy;

class OdqBaseRenderArea
{
public:
  OdqBaseRenderArea(QWidget* pParent, bool bMaximized = false, TEnViewBusy step = enBusy_UninitializedGL);
  virtual ~OdqBaseRenderArea();
  static OdqBaseRenderArea* renderArea(QWidget* pWidget);

  virtual void init(class OdqView* iView);
  virtual QWidget* widget() = 0;
  virtual void resetContext(OdGsModule* pGs) = 0;
  virtual bool setWinDeviceProperties() { return false; }

  OdqView* getView();
  void applySize();

protected:
  QSize size_hint(int width, int heigth) const;

  QPointer<OdqView> m_pView;
  TEnViewBusy m_enBusy; friend class OdqView; friend class _AutoUnlocker;
  bool m_bMaximized,
       m_bResized; // set on (after resize) by paint gl with already valid gs device 

  bool m_isLastResize,
       m_bFillAreaByPaint;
  int m_widthLastResize,
      m_heightLastResize;
};

//////////////////////////////////////////////////////////////////////////

class OdqGlRenderArea : public QGLWidget, public OdqBaseRenderArea
{
  Q_OBJECT
public:
  OdqGlRenderArea(QWidget* pParent, bool bMaximized = false, TEnViewBusy step = enBusy_UninitializedGL);
  virtual ~OdqGlRenderArea();

  virtual void init(OdqView* iView);
  virtual QWidget* widget();
  virtual void resetContext(OdGsModule* pGs);

  //OdqView* getView(); 
  QImage& getImageFrame();
  bool isImageFrameUsed() const;
  bool isResized() const;

protected:

  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  static QGLFormat getQGLFormat();

  virtual void mouseMoveEvent(QMouseEvent* pEvent);
  virtual void mousePressEvent(QMouseEvent* pEvent);
  virtual void mouseReleaseEvent(QMouseEvent* pEvent);
  virtual void mouseDoubleClickEvent(QMouseEvent* pEvent);
  virtual void wheelEvent(QWheelEvent* pEvent);
  // keyPressEvent and keyReleaseEvent are used via OdqView

  virtual bool event(QEvent* pEvent);
  virtual void resizeEvent(QResizeEvent* pEvent);

  //QPointer<OdqView> m_pView;
  bool m_bUseImageFrame;
  GLuint m_idTexture;
  QImage m_imageFrame; // used for Caustic device only

  //TEnViewBusy m_enBusy; 
  friend class OdqView; friend class _AutoUnlocker;
  //bool m_bMaximized,
  //     m_bResized; // set on (after resize) by paint gl with already valid gs device 
  //bool m_bSkipFirstResizeGL;
  //bool m_isLastResizeGL;
  //int m_widthLastResize,
  //    m_heightLastResize,
  //    m_levelLastResize;
  //qreal totalScaleFactor;

  // QMutex works between threads and is useless here after it
};

//////////////////////////////////////////////////////////////////////////

class OdqWidgetRenderArea : public QWidget, public OdqBaseRenderArea
{
  Q_OBJECT
public:
  OdqWidgetRenderArea(QWidget* pParent, bool bMaximized = false, TEnViewBusy step = enBusy_UninitializedGL);
  virtual ~OdqWidgetRenderArea();

  virtual void init(OdqView* iView);
  virtual QWidget* widget();
  virtual void resetContext(OdGsModule* pGs);
  virtual bool setWinDeviceProperties();

  public slots:
  virtual void updateGL() {}
protected:

  //void initializeGL();
  //void paintGL();
  //void resizeGL(int width, int height);

  //virtual void mouseMoveEvent(QMouseEvent* pEvent);
  //virtual void mousePressEvent(QMouseEvent* pEvent);
  //virtual void mouseReleaseEvent(QMouseEvent* pEvent);
  //virtual void mouseDoubleClickEvent(QMouseEvent* pEvent);
  //virtual void wheelEvent(QWheelEvent* pEvent);
  //// keyPressEvent and keyReleaseEvent are used via OdqView

  //virtual bool event(QEvent* pEvent);
  virtual void resizeEvent(QResizeEvent* pEvent);
  virtual void paintEvent(QPaintEvent* pEvent);

  //bool m_bUseImageFrame;
  //GLuint m_idTexture;
  //QImage m_imageFrame; // used for Caustic device only

  friend class OdqView; friend class _AutoUnlocker;
};

//////////////////////////////////////////////////////////////////////////

class OdqViewGroupBox : public QGroupBox
{
  Q_OBJECT
public:
  OdqViewGroupBox(class OdqView* pView, QWidget* pParent = NULL)
    : QGroupBox(pParent), m_pView(pView)
  {
  }
protected:
  virtual QSize minimumSizeHint() const;
  virtual QSize sizeHint() const;
  OdqView* m_pView;
};

//////////////////////////////////////////////////////////////////////////

class OdqView : public QMdiSubWindow
              , public IqView
{
  Q_OBJECT
  Q_INTERFACES(IqView)
  OdqView(QMdiArea* pMdiArea,
          OdRxObject* pRxDatabase,
          OdDbStub* idLayoutView,
          const OdGsDCRect* pRect);
public:
  static OdqView* create(QMdiArea* pMdiArea, 
                         OdRxObject* pRxDatabase = NULL, 
                         OdDbStub* idLayoutView = NULL, // active if NULL
                         const OdGsDCRect* pRect = NULL);
  virtual ~OdqView();

  TEnViewBusy getBusyState();
  //void lockMaximize();

  virtual bool event(QEvent* pEvent);
  virtual void closeEvent(QCloseEvent *pEvent);
  virtual void changeEvent(QEvent* pEvent);

  // ----- IqView -----

  virtual OdRxObject* getRxDatabase();
  virtual void updateLayouts(bool bForce = false); // force means to renew render area widget);

  virtual bool emitClose();

  virtual OdGsDevice* getGsDevice(); // the device to render the active layout (is a OdGsLayoutHelper) 
  virtual OdGsView* getActiveGsView();
  virtual OdGsView* getActiveTopGsView();
  virtual OdGsModel* getGsModel(bool b2d = false);

  virtual QCursor cursor(); 
  virtual void setCursor(const QCursor& cursor); 

  bool getUcsPlane(OdGePlane& plane, const OdGeVector3d* pvHintDirToIntersect = NULL);
    // convert eyes point to UCS(by empty OdEd::kGptNoUCS flag in option) or to WCS in eye plane
  virtual bool eyesTo(const QPoint& ptEyes, // in 
                      int options, // in 
                      OdGePoint3d& ptOut, // out
                      const OdGePlane* pUcsPlane = NULL); // in
  virtual bool wcsToScreen(const OdGePoint3d& wcsPt,
                           OdGePoint2d& ptScr); // out

  virtual ODCOLORREF backgroundColor() const;
  virtual bool setBackgroundColor(ODCOLORREF clrBackground);
  virtual void setViewportBorderProperties();

  virtual QWidget* getWidget(); // // of render area
  virtual void updateGL(); // repaint
  virtual void setActive();
  // for Android only
  virtual bool isFullScreenMode(); 
  virtual bool isFullScreenActive();
  //bool isResizeActive();

  // -----

  static class OdqViewSystem* getViewSystem();

  void resetRenderArea(bool bForceRenew = false);
  void clearDevice();
  void resetDevice();
  bool isModelLayout();

  void saveSettings();

  void subscribeTimer(OdEdIntegerTracker* pTracker, int msec = 100);
  void unsubscribeTimer(OdEdIntegerTracker* pTracker);

  void updateObject(OdDbStub* id);
  void highlightObject(bool bValue, OdDbStub* id, const OdDbBaseFullSubentPath* pPath);
  void onModifiedGrip(OdGiDrawablePtr pGripData);
  void highlightGripPoint(bool bValue, OdGiDrawablePtr pGripData);
  void highlightSnapPoint(bool bValue, OdDbStub* id, const OdDbBaseFullSubentPath* pPath, 
                          OdDb::OsnapMode osm, const OdGePoint3d& point);

  QSize size_hint(int width, int heigth) const; // for m_pInternalWidget area

protected:
  virtual void keyPressEvent(QKeyEvent* pEvent);
  virtual void keyReleaseEvent(QKeyEvent* pEvent);

signals:
  void mouseMove(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void mousePress(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void mouseRelease(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void mouseDoubleClick(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void mouseWheel(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void keyPress(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void keyRelease(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void touchPressed(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void touchMove(IqView* iView, QEvent* pEvent, bool& bAccepted);
  void touchReleased(IqView* iView, QEvent* pEvent, bool& bAccepted);

private slots:
  void changeLayout(int index);
  void timerUpdate();

protected:
  OdRxObject* m_pRxDatabase;
  OdDbStub* m_idLayoutView; // used for DGN only
  bool m_bDoZoomExtents;

  // the device to render the active layout (is a OdGsLayoutHelper) 
  friend class OdqGlRenderArea; 
  friend class OdqOpenGlRenderArea;
  friend class OdqWidgetRenderArea;
  OdGsDevicePtr m_pGsDevice; 
  OdGsModelPtr m_pGsModel2d;

  OdqBaseRenderArea* m_pBaseRenderArea; // QPointer<OdqGlRenderArea> m_pRenderArea;
  QPointer<QWidget> m_pRenderAreaWidget; // OdqBaseRenderArea is not QObject
  OdqBaseRenderArea* renderArea()
  {
    if (m_pRenderAreaWidget.isNull())
      return NULL;
    ODA_ASSERT_ONCE(   m_pBaseRenderArea 
                    && m_pBaseRenderArea->widget() == m_pRenderAreaWidget.data());
    return m_pBaseRenderArea;
  }

  QPointer<OdqViewGroupBox> m_pInternalWidget;
  QPointer<QTabBar> m_pTabLayout;
  int m_indexTabLayoutPrev;

  QColor m_clrBack,
         m_clrBackSel,
         m_clrFore;

  OdEdIntegerTracker* m_pTrackerOnTimer;
  int m_intervalTimer;
  bool m_isByClickIfCloseEvent;

  friend class OdqBaseRenderArea; friend class OdqCmd_SysWindows;
  OdSharedPtr<OdGsDCRect> m_pRect;
  OdSharedPtr<QSize> m_pSize;
  //bool m_bLockMaximize;
};

#endif // ODA_QT_VIEW_H_
