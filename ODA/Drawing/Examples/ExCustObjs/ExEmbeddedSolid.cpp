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

/************************************************************************/
/*ExEmbeddedSolid.cpp: example of implementation for a custom entity    */
/* with an embedded 3dSolid object                                      */
/************************************************************************/
#include "OdaCommon.h"
#include "ExEmbeddedSolid.h"
#include "Gi/GiWorldDraw.h"
#include "DbFiler.h"
#include "DbCommandContext.h"
#include "EmbeddedASMEntPE.h"
#include "DbProxyEntity.h"
#include "DbBlockTableRecord.h"

ODRX_DXF_DEFINE_MEMBERS(EmbeddedSolidWrapper,                                          // ClassName
                        OdDbEntity,                                                    // ParentClass
                        DBOBJECT_CONSTR,                                               // DOCREATE
                        OdDb::vAC27,                                                   // DwgVer
                        OdDb::kMRelease0,                                              // MaintVer
                        OdDbProxyEntity::kAllAllowedBits,                              // NProxyFlags
                        EmbeddedSolidWrapper,                                          // DxfName
                        ExCustObjs|Description: Teigha Run-time Extension Example)     // AppName


/************************************************************************/
/* A pointer to an OdDbEmbeddedASMEntPropsPE object implementing        */
/* protocol extension for embedded entity handling                      */
/************************************************************************/                        
static OdDbEmbeddedASMEntPropsPE* pPE = NULL;
static bool bInit = true;

/************************************************************************/
/* Get pointer to protocol extension object                             */
/************************************************************************/
static OdDbEmbeddedASMEntPropsPE* getEmbedPE()
{
  if (bInit)
  {
    bInit = false;
    OdRxClass* pCls = OdDb3dSolid::desc();
    OdRxClass* pPECls = OdRxClass::cast(odrxClassDictionary()->getAt(L"OdDbEmbeddedASMEntPropsPE"));
    if (pCls && pPECls)
    {
      pPE = (OdDbEmbeddedASMEntPropsPE*)pCls->getX(pPECls).get();
    }
  }
  return pPE;
}                        
                        
                        
                        
/************************************************************************/
/* Creates an instance of a custom entity with an embedded 3dSolid      */
/************************************************************************/
EmbeddedSolidWrapper::EmbeddedSolidWrapper()
{
  m_pSolid = OdDb3dSolid::createObject();
  OdDbEmbeddedASMEntPropsPE* pPE = getEmbedPE();
/************************************************************************/
/*Set embedded flag for the 3dSolid entity*/
/************************************************************************/
  if (pPE && !m_pSolid.isNull())
    pPE->setIsEmbeddedEnt(m_pSolid);
}

/************************************************************************/
/* Initialize embedded 3dSolid object with specified database default   */
/* properties and create sphere                                         */
/************************************************************************/
void EmbeddedSolidWrapper::initSolid(OdDbDatabase* pDb)
{
  if (!m_pSolid.isNull())
  {
    m_pSolid->setDatabaseDefaults(pDb);
    m_pSolid->createSphere(400);
  }
}

EmbeddedSolidWrapper::~EmbeddedSolidWrapper()
{
}

/************************************************************************/
/* Read the DWG format data of the object from the specified file       */
/************************************************************************/
OdResult EmbeddedSolidWrapper::dwgInFields(OdDbDwgFiler* filer)
{
/**********************************************************************/
  /* Determining whether an entity should be loaded as a proxy          */
  /* (when OdDbEmbeddedASMEntPropsPE is not available)                  */
  /**********************************************************************/
  if (!getEmbedPE() && filer->filerType() == OdDbFiler::kFileFiler)
    return eMakeMeProxy;

  /**********************************************************************/
  /* If the entity is opened for writing, read fields common to all     */
  /* entities                                                           */
  /**********************************************************************/
  assertWriteEnabled();
  OdResult es = OdDbEntity::dwgInFields(filer);
  if (es != eOk)
    return es;

  /**********************************************************************/
  /* Read data for 3dSolid object from the specified file               */
  /**********************************************************************/
  es = m_pSolid->dwgInFields(filer);
  return es;
}

/************************************************************************/
/* Write the DWG format data of the custom entity to the specified file */
/* through two steps: write common fields and write fields for the      */
/* embedded 3dSolid object                                              */
/************************************************************************/
void EmbeddedSolidWrapper::dwgOutFields(OdDbDwgFiler* filer) const
{
  assertReadEnabled();
  OdDbEntity::dwgOutFields(filer);
  m_pSolid->dwgOutFields(filer);
}

/************************************************************************/
/* Read the DXF format data of this object from the specified file      */
/************************************************************************/
OdResult EmbeddedSolidWrapper::dxfInFields (OdDbDxfFiler* filer)
{
  /**********************************************************************/
  /* Determining whether a proxy creating is required                   */
  /* (when OdDbEmbeddedASMEntPropsPE is not available)                  */
  /**********************************************************************/
  if (!getEmbedPE())
    return eMakeMeProxy;

  /**********************************************************************/
  /* If the entity is opened for writing, read fields common to all     */
  /* entities                                                           */
  /**********************************************************************/
  assertWriteEnabled();

  OdResult es = OdDbEntity::dxfInFields(filer);
  if (es != eOk)
    return es;

  if (filer->atEmbeddedObjectStart())
  {
    es = m_pSolid->dxfInFields(filer);
  }
  else
  {
    es = eBadDxfSequence;
  }
  return es;
}

/************************************************************************/
/* Write the DXF format data of this object to the specified file       */
/************************************************************************/
void EmbeddedSolidWrapper::dxfOutFields(OdDbDxfFiler* filer) const
{
  /************************************************************************/
  /* Checking whether the entity is opened for reading, then write common */
  /* fields                                                               */
  /************************************************************************/
  assertReadEnabled();
  OdDbEntity::dxfOutFields(filer);
  filer->wrEmbeddedObjectStart();
  /************************************************************************/
  /* Write fields for the embedded 3dSolid object                         */
  /************************************************************************/
  m_pSolid->dxfOutFields(filer);
}

/************************************************************************/
/* Creates a viewport-independent geometric representation of the       */
/* embedded 3dSolid object                                              */
/************************************************************************/
bool EmbeddedSolidWrapper::subWorldDraw(OdGiWorldDraw* pWd) const
{
  pWd->geometry().draw(m_pSolid);
  return true;
}

/************************************************************************/
/* Create EmbeddedSolidWrapper command                                       */
/************************************************************************/
void _EmbeddedSolidWrapper_func(OdEdCommandContext* pCmdCtx)
{
  if (!getEmbedPE())
    return;

  /**********************************************************************/
  /* Establish command context, database, and UserIO                    */
  /**********************************************************************/
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  
  OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject(OdDb::kForWrite);

  EmbeddedSolidWrapperPtr pWrapper = EmbeddedSolidWrapper::createObject();

  pWrapper->setDatabaseDefaults(pDb);
  pWrapper->initSolid(pDb);

  pMS->appendOdDbEntity(pWrapper);
}
