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
// OdqOpenGlRenderArea.h
//

#ifndef ODA_QT_OPEN_GL_RENDER_AREA_H_
#define ODA_QT_OPEN_GL_RENDER_AREA_H_

#include "OdqView.h"

//////////////////////////////////////////////////////////////////////////
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)

// It is in separate file only for linking problem with qt4 via moc incorrect generation
// (moc of qt4 unsupports ifdef s)

class OdqOpenGlRenderArea : public QOpenGLWidget, protected QOpenGLFunctions, public OdqBaseRenderArea
{
  Q_OBJECT
public:
  OdqOpenGlRenderArea(QWidget* pParent, bool bMaximized = false, TEnViewBusy step = enBusy_UninitializedGL);
  virtual ~OdqOpenGlRenderArea();

  static QSurfaceFormat getQSurfaceFormat(OdGsModule* pGs = NULL);

  virtual void init(OdqView* iView);
  virtual QWidget* widget();
  virtual void resetContext(OdGsModule* pGs);

  //virtual void resizeEvent(QResizeEvent * event);

  //QImage& getImageFrame();
  //bool isImageFrameUsed() const;
  //bool isResized() const;

  //QSize minimumSizeHint() const;
  //QSize sizeHint() const;

public slots:
  virtual void updateGL() {}
protected:
  //QSize sizeHint(int width, int heigth) const;

  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

  //virtual void mouseMoveEvent(QMouseEvent* pEvent);
  //virtual void mousePressEvent(QMouseEvent* pEvent);
  //virtual void mouseReleaseEvent(QMouseEvent* pEvent);
  //virtual void mouseDoubleClickEvent(QMouseEvent* pEvent);
  //virtual void wheelEvent(QWheelEvent* pEvent);
  //// keyPressEvent and keyReleaseEvent are used via OdqView

  //virtual bool event(QEvent* pEvent);
  //virtual void resizeEvent(QResizeEvent* pEvent);

  //bool m_bUseImageFrame;
  //GLuint m_idTexture;
  //QImage m_imageFrame; // used for Caustic device only

  friend class OdqView; friend class _AutoUnlocker;
  //bool m_bSkipFirstResizeGL;
};

#endif
//////////////////////////////////////////////////////////////////////////

#endif // ODA_QT_VIEW_H_
