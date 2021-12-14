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
// main.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif

#include "widget.h"
#include "convertor.h"

#if defined(ANDROID)
extern void initMbWcConv();
inline void attuneLocale()
{
  //char* pLocal = setlocale(LC_CTYPE, "C"); // (LC_ALL, "C");
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  initMbWcConv();
#endif
}
#else
#define attuneLocale()
#endif

int main(int argc, char *argv[])
{
  attuneLocale();

  QApplication app(argc, argv);
  
  Window window(app.arguments()); // argv as unicode strings
  OdStaticRxObject<CConvertor> convertor;
  
  convertor.init(); // after using OdStaticRxObject should be out of constructor / destructor
  window.setConvertor(&convertor);

  int res = 0;
  if (window.isValid())
  {
    window.show();
    res = app.exec();
  }
 
  convertor.uninit(); // after using OdStaticRxObject should be out of constructor / destructor
  return res;
}
