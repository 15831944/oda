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
// OdqSnapManager.cpp
//

#include "OdaCommon.h"

#include "OdqSnapManager.h"
#include "OdqConsoleTab.h"
#include "DbHostAppServices.h"

//////////////////////////////////////////////////////////////////////////

ODRX_NO_CONS_DEFINE_MEMBERS(OdqSnapManager, OdBaseSnapManager);

//static 
OdqSnapManagerPtr OdqSnapManager::createObject(OdRxObject* pRxDb)
{
  OdqSnapManagerPtr pSnapManager = OdRxObjectImpl<OdqSnapManager>::createObject();
  pSnapManager->init(pRxDb);
  return pSnapManager;
}

//static 
bool OdqSnapManager::isSupported(OdRxObject* pRxDb)
{
  ODA_ASSERT_ONCE(pRxDb);
  if (OdDbDatabase::cast(pRxDb).get())
    return true;
  return false;
}

OdqSnapManager::OdqSnapManager()
  : m_pRxDb(NULL)
  , m_pBasePt(NULL)
{
}

OdqSnapManager::~OdqSnapManager()
{
}

void OdqSnapManager::init(OdRxObject* pRxDb)
{
  ODA_ASSERT_ONCE(pRxDb);
  m_pRxDb = pRxDb;
  if (!Set_Entity_centers(pRxDb))
  {
    ODA_FAIL_ONCE(); // TODO for non-DWG databases
  }
}

unsigned OdqSnapManager::snapModes() const
{ 
  unsigned nSnapModes = (unsigned) getIAppProps()->toInt(L"Vars/osmode", 4133);
  //if (m_pRxDb)
  //{
  //  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(m_pRxDb);
  //  if (pDwgDb.get() && nSnapModes != (unsigned) pDwgDb->appServices()->getOSMODE())
  //  {
  //    ODA_FAIL_ONCE();
  //    pDwgDb->appServices()->setOSMODE((OdInt16) nSnapModes);
  //    getIApp()->refreshQObjetcts("<event-refresh:appvar_changed>", "osmode");
  //  }
  //}
  return nSnapModes;
}

OdCmEntityColor OdqSnapManager::snapTrueColor() const
{
  ODCOLORREF color = getIAppProps()->toColorref(L"Options/Display/SnapColor", ODRGB(1, 152, 1));
  return OdCmEntityColor(ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color));
}

const OdGePoint3d* OdqSnapManager::basePtRef() const
{
  return m_pBasePt;
}

void OdqSnapManager::setBasePtRef(const OdGePoint3d* pBasePt)
{
  m_pBasePt = pBasePt;
}
