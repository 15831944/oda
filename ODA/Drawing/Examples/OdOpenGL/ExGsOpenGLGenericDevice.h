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

#ifndef ODEXGSOPENGLGENERICDEVICE
#define ODEXGSOPENGLGENERICDEVICE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GsOpenGLVectorizer.h"
#include "GsOpenGLStreamVectorizer.h" // Comment it if you want use non-streaming version of vectorizer
#include "ExGsGenericDevice.h"

#ifdef OD_OGL_ENABLEMETASTREAM
typedef OdGsOpenGLStreamVectorizeDevice ExGsOpenGLVectorizeDeviceBase;
typedef OdGsOpenGLStreamVectorizeView ExGsOpenGLVectorizeViewBase;
#else
typedef OdGsOpenGLVectorizeDevice ExGsOpenGLVectorizeDeviceBase;
typedef OdGsOpenGLVectorizeView ExGsOpenGLVectorizeViewBase;
#endif

class ExGsOpenGLGenericDevice : public ExGsOpenGLVectorizeDeviceBase, public ExGsGenericDevice
{
protected:

  ODRX_HEAP_OPERATORS();

  ExGsOpenGLGenericDevice();

  virtual ~ExGsOpenGLGenericDevice();

  void update(OdGsDCRect* pUpdatedRect);

  OdRxDictionaryPtr properties() { return (OdRxDictionary*)this; }

  void clear();  

  virtual void createContext();

  virtual void deleteContext();
};


class ExGsOpenGLGenericView : public ExGsOpenGLVectorizeViewBase
{
  ExGsOpenGLGenericDevice* device()
  {
    return static_cast<ExGsOpenGLGenericDevice*>(ExGsOpenGLGenericView::device());
  }
public:
  void polylineOut(OdInt32 nPoints, const OdGePoint3d* pPoints);
};


#endif //ODEXGSOPENGLVECTORIZEDEVICE
