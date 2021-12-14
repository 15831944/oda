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

#include "OdaCommon.h"
#include "OdString.h" // else undefined in rxvariantvalue.h (?)
#include "ExGsOpenGLGenericDevice.h"
#include "TD_GL.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiMetafile.h"
#include "OdRound.h"

#define ROUND(n) OdRoundToLong(n)

//Implementation of class ExGsOpenGLGenericDevice
//*******************************************************************************/
// Constructor
//*******************************************************************************/
ExGsOpenGLGenericDevice::ExGsOpenGLGenericDevice()
{
}

//*******************************************************************************/
// Destructor
//*******************************************************************************/
ExGsOpenGLGenericDevice::~ExGsOpenGLGenericDevice()
{
  deleteContext();
}

//*******************************************************************************/
//  createContext()
//*******************************************************************************/
void ExGsOpenGLGenericDevice::createContext()
{
  createPalette(&m_logPalette);
}

//*******************************************************************************/
// deleteContext()
//*******************************************************************************/
void ExGsOpenGLGenericDevice::deleteContext()
{
}

//*******************************************************************************/
//  setClearColor()
//*******************************************************************************/
void ExGsOpenGLGenericDevice::clear()
{
  glClearColor(1., 1., 0., 1.);
  glClear(GL_COLOR_BUFFER_BIT);  
}

//*******************************************************************************/
//  update()
//*******************************************************************************/
void ExGsOpenGLGenericDevice::update(OdGsDCRect* pUpdatedRect)
{
  createContext();

  if (!supportPartialUpdate())
  {
    if (!isValid())
      clear();
  }

  ExGsOpenGLVectorizeDeviceBase::update(pUpdatedRect);
  
  deleteContext();
}

//*******************************************************************************/
//  polylineProc()
//*******************************************************************************/
void ExGsOpenGLGenericView::polylineOut(OdInt32 nPoints, const OdGePoint3d* pPoints)
{
  ExGsOpenGLVectorizeViewBase::polylineOut(nPoints, pPoints);
}

