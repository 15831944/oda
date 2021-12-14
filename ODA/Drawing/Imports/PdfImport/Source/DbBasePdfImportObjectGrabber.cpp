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
#include "DbBasePdfImportObjectGrabber.h"
#include "DbHostAppProgressMeter.h"
#include "DbBaseHostAppServices.h"
#include "DbHostAppServices.h"

#include <algorithm>
#include <memory>
#include <vector>

PdfImportInfoHolder::PdfImportInfoHolder(OdPdfImport::ImportResult& result)
  :m_pProperties(PdfProperties::createObject())
  ,m_ImageError(result)
{

}

PdfImportInfoHolder::~PdfImportInfoHolder()
{
  if (NULL != m_pProgressMeter)
  {
    OdDbDatabasePtr pDb = OdDbDatabasePtr(m_pProperties->get_Database());
    if (!pDb.isNull())
    {
      OdDbHostAppServices *pServ = pDb->appServices();
      if (NULL != pServ)
      {
        pServ->releaseProgressMeter(m_pProgressMeter);
        m_pProgressMeter = NULL;
      }
    }
  }

}

void PdfImportInfoHolder::InitProgressMeter()
{
  if (NULL == m_pProgressMeter)
  {
    OdDbDatabasePtr pDb = OdDbDatabasePtr(m_pProperties->get_Database());
    if (!pDb.isNull())
    {
      OdDbHostAppServices *pServ = pDb->appServices();
      if (NULL != pServ)
        m_pProgressMeter = pServ->newProgressMeter();
    }
  }
}

void PdfImportInfoHolder::StartProgressMeter(OdString& displayString)
{
  if (NULL != m_pProgressMeter)
    m_pProgressMeter->start(displayString);
}

void PdfImportInfoHolder::StopProgressMeter()
{
  if (NULL != m_pProgressMeter)
    m_pProgressMeter->stop();
}

void PdfImportInfoHolder::ProgressMeterSetLimit()
{
  if (NULL != m_pProgressMeter)
    m_pProgressMeter->setLimit((int)m_ObjectCounter);
}

void PdfImportInfoHolder::MeterProgress()
{
  if (NULL != m_pProgressMeter)
    m_pProgressMeter->meterProgress();
}

OdDbBasePdfImportObjectGrabber::OdDbBasePdfImportObjectGrabber(PdfImportInfoHolderPtr info)
  :m_Info(info)
{

}

OdDbBasePdfImportObjectGrabber::~OdDbBasePdfImportObjectGrabber()
{

}

bool OdDbBasePdfImportObjectGrabber::needToAddGrathics(bool* ret)
{
  return m_Info->m_pProperties->get_ImportVectorGeometry();
}

bool OdDbBasePdfImportObjectGrabber::needToAddImage(bool* ret)
{
  return m_Info->m_pProperties->get_ImportRasterImages();
}

bool OdDbBasePdfImportObjectGrabber::needToAddText(bool* ret)
{
  if (m_Info->m_pProperties->get_ImportTrueTypeTextAsGeometry())
  {
    if (NULL != ret)
      *ret = false;
    return false;
  }
  return m_Info->m_pProperties->get_ImportTrueTypeText();
}

bool OdDbBasePdfImportObjectGrabber::needToAddShading(bool* ret)
{
  return m_Info->m_pProperties->get_ImportGradientFills();
}

float OdDbBasePdfImportObjectGrabber::getWidth()
{
  return m_Info->m_Width;
}

float OdDbBasePdfImportObjectGrabber::getHeight()
{
  return m_Info->m_Height;
}
