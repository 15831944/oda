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
/* ExCustObject.cpp: implementation of the ExCustObject class, a custom */
/* OdDbObject                                                           */                                 
/************************************************************************/
#include "OdaCommon.h"
#include "ExCustObject.h"
#include "DbFiler.h"
#include "DbCommandContext.h"
#include "DbDatabase.h"
#include "DbDictionary.h"
#include "DbUserIO.h"


ODRX_DXF_DEFINE_MEMBERS(ExCustObject,                                                 // ClassName
                        OdDbObject,                                                   // ParentClass
                        DBOBJECT_CONSTR,                                              // DOCREATE
                        OdDb::vAC15,                                                  // DwgVer
                        OdDb::kMRelease0,                                             // MaintVer
                        0,                                                            // nProxyFlags
                        EXCUSTOBJECT,                                                 // DxfName
                        ExCustObjs|Description: Teigha Run-time Extension Example)    // AppName

const int ExCustObject::lastKnownVersion = 2;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ExCustObject::ExCustObject()
  : m_nCustValue(0)
{
}

ExCustObject::~ExCustObject()
{

}
/************************************************************************/
/* Properties unique to ExCustObj                                       */
/************************************************************************/
int ExCustObject::getVersion()
{
  return lastKnownVersion;
}

void ExCustObject::setValue(int n)
{
  assertWriteEnabled();
  m_nCustValue = n;
}

int ExCustObject::getValue() const
{
  assertReadEnabled();
  return m_nCustValue;
}

void ExCustObject::setString(const OdString& str)
{
  assertWriteEnabled();
  m_string = str;
}

OdString ExCustObject::getString() const
{
  assertReadEnabled();
  return m_string;
}

/************************************************************************/
/* Reads the DWG format data of this object from the specified file.    */
/************************************************************************/
OdResult ExCustObject::dwgInFields(OdDbDwgFiler* pFiler)
{
  /**********************************************************************/
  /* Read the fields common to all OdDbObjects                          */
  /**********************************************************************/
  OdDbObject::dwgInFields(pFiler);
  /**********************************************************************/
  /* Read the version of the ExCustObj, and indicate create if a proxy  */
  /* is required.                                                       */
  /**********************************************************************/
  int nVersion = pFiler->rdInt16();
  if (nVersion > lastKnownVersion)
    return eMakeMeProxy;

  /**********************************************************************/
  /* Read and set the remaining properties unique to ExCustObj.         */
  /**********************************************************************/
  m_nCustValue = pFiler->rdInt32();
  if (nVersion > 1)
    m_string = pFiler->rdString();
  else
    m_string.empty();
  return eOk;
}

/************************************************************************/
/* Writes the DWG format data of this object to the specified file.     */
/************************************************************************/
void ExCustObject::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  /**********************************************************************/
  /* Write the fields common to all OdDbObjects                        */
  /**********************************************************************/
  OdDbObject::dwgOutFields(pFiler);
  /**********************************************************************/
  /* Write the properties unique to ExCustObj.                          */
  /**********************************************************************/
  OdInt16 nVer = (m_string.isEmpty() ? 1 : 2);
  pFiler->wrInt16(nVer);
  pFiler->wrInt32(m_nCustValue);
  if (nVer > 1)
    pFiler->wrString(m_string);
}

/************************************************************************/
/* Reads the DXF format data of this object from the specified file.    */
/************************************************************************/
OdResult ExCustObject::dxfInFields(OdDbDxfFiler* pFiler)
{
  /**********************************************************************/
  /* Read the fields common to all OdDbObjects                          */
  /**********************************************************************/
  OdResult res = OdDbObject::dxfInFields(pFiler);
  if (res != eOk)
    return res;
  if (!pFiler->atSubclassData(desc()->name()))
    return eBadDxfSequence;

  /**********************************************************************/
  /* Read and Set the remaining properties unique to ExCustomObj.       */
  /* Do not depend on a particular DXF file order                       */
  /**********************************************************************/
  while(!pFiler->atEOF())
  {
    int nCode = pFiler->nextItem();
    switch (nCode)
    {
    case 70:
      {
      int nVersion = pFiler->rdInt16();
      if (nVersion > lastKnownVersion)
        return eMakeMeProxy;
      }
      break;
    case 90:
      m_nCustValue = pFiler->rdInt32();
      break;
    case 1:
      m_string = pFiler->rdString();
      break;
    }
  }
  return eOk;
}

/************************************************************************/
/* Writes the DXF format data of this object to the specified file.     */
/************************************************************************/
void ExCustObject::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  /**********************************************************************/
  /* Write the fields common to all OdDbObjects                         */
  /**********************************************************************/
  OdDbObject::dxfOutFields(pFiler);
  pFiler->wrSubclassMarker(desc()->name());
  OdInt16 nVer = (m_string.isEmpty() ? 1 : 2);
  pFiler->wrInt16(70, nVer);
  pFiler->wrInt32(90, m_nCustValue);
  if (nVer > 1)
    pFiler->wrString(1, m_string);
}

/************************************************************************/
/* Create ExCustObj command                                             */
/*                                                                      */
/* The NamedObjects dictionary store keys that reference the ExCustObj  */
/* objects.                                                             */
/************************************************************************/
void _ExCreateCustomObject_func(OdEdCommandContext* pCmdCtx)
{
  /**********************************************************************/
  /* Establish command context, database, and UserIO                    */
  /**********************************************************************/
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdEdUserIO* pIO = pDbCmdCtx->userIO();
  
  /**********************************************************************/
  /* Link to NamedObjectsDictionary                                     */
  /**********************************************************************/
  OdDbDictionaryPtr pNamedObjectsDic = pDb->getNamedObjectsDictionaryId().safeOpenObject();

  /**********************************************************************/
  /* Open the custom dictionary                                         */
  /**********************************************************************/
  const char* ExCustomObjectsDicName = "EXCUSTOMOBJECTS";
  OdDbDictionaryPtr pExCustomObjectsDic = 
    (OdDbDictionaryPtr) pNamedObjectsDic->getAt(ExCustomObjectsDicName,OdDb::kForRead);
  if (!pExCustomObjectsDic.get())
  {
    /********************************************************************/
    /* Create a custom dictionary                                       */
    /********************************************************************/
    pExCustomObjectsDic = OdDbDictionary::createObject();
    pNamedObjectsDic->upgradeOpen();
    pNamedObjectsDic->setAt(ExCustomObjectsDicName, pExCustomObjectsDic);
    pNamedObjectsDic->downgradeOpen();
  }
  
  ExCustObjectPtr pMyObj;
  bool bDone = false;
  OdString sKey;
  OdString sDef;
  do
  {
    /********************************************************************/
    /* Get the Default Key                                              */
    /********************************************************************/
    sDef.format(OD_T("MyObject%d"), pExCustomObjectsDic->numEntries());
    
    /********************************************************************/
    /* Get the Prompt, with default key in <>                           */
    /********************************************************************/
    sKey.format(OD_T("Enter object key <%ls>:"), sDef.c_str());

    /********************************************************************/
    /* Get a key with a default                                         */
    /********************************************************************/
    sKey = pIO->getString(sKey, OdEd::kGstAllowSpaces, sDef);
    
    /********************************************************************/
    /* Return a pointer to object at the key.                            */
    /********************************************************************/
    OdDbObjectPtr pObj = pExCustomObjectsDic->getAt(sKey, OdDb::kForRead);
    
    if(pObj.get()) // key exists
    {
      /******************************************************************/
      /* Key exists                                                     */
      /******************************************************************/
      pMyObj = ExCustObject::cast(pExCustomObjectsDic->getAt(sKey, OdDb::kForRead));
      if(pMyObj.get()) // object of a kind OD_T("ExCustObject") exists
      {
        switch(pIO->getKeyword(OD_T("Key exists. [Replace/Edit existing/erase All existing/Reenter]?:"),
                               OD_T("Replace Edit eraseAll Reenter"))
              )
        {
        case 0: // Replace
          /**************************************************************/
          /* Replace the object for the specified key                   */
          /**************************************************************/
          pMyObj = ExCustObject::createObject();
          break;
        case 1: //Edit
          /**************************************************************/
          /* Edit the object for the specified key                      */
          /**************************************************************/
          pMyObj->upgradeOpen();
          break;
        case 2: //eraseAll
          {
            /************************************************************/
            /* Erase all objects                                        */
            /************************************************************/
            OdDbDictionaryIteratorPtr pObjects = pExCustomObjectsDic->newIterator();
            while(!pObjects->done())
            {
              int n = 0;
              pMyObj = ExCustObject::cast(pObjects->object());
              if(pMyObj.get())
              {
                pMyObj->upgradeOpen();
                pMyObj->erase();
                ++n;
              }
              pObjects->next();
              OdString s;
              s.format(OD_T("%d object(s) erased."));
              pIO->putString(s);
            }
            /************************************************************/
            /* Create a new object at the specified key                 */
            /************************************************************/
            pMyObj = ExCustObject::createObject();
          }
          break;
        case 3: //Reenter
          /**************************************************************/
          /* Enter a different key                                      */
          /**************************************************************/
          continue;
        default:
          /**************************************************************/
          /* Do nothing                                                 */
          /**************************************************************/
          break;
        }
        bDone = true;
      }
      else
      {
        /****************************************************************/
        /* The key exists, but no object corresponds to it              */
        /****************************************************************/
        pIO->putString(OD_T("Key exists."));
      }
    }
    else
    {
      /******************************************************************/
      /* Key does not exist                                             */
      /******************************************************************/
      pMyObj = ExCustObject::createObject();
      bDone = true;
    }
  }
  while(!bDone);

  /**********************************************************************/
  /* Set the properties                                                 */
  /**********************************************************************/

  /**********************************************************************/
  /* Set the value to the specified integer or a random number          */
  /**********************************************************************/
  try
  {
    pMyObj->setValue(pIO->getInt(OD_T("Enter integer or [Random]:"), 0, 0, OD_T("Random")));
  }
  catch(const OdEdKeyword&)
  {
    pMyObj->setValue(rand());
  }

  /**********************************************************************/
  /* Set the string                                                     */
  /**********************************************************************/
  pMyObj->setString(pIO->getString(OD_T("Enter string <\"\">:"), OdEd::kGstAllowSpaces, OD_T("")));

  /**********************************************************************/
  /* If the object is not already database resident, add it to the      */
  /* NamedObjects dictionary, which makes it so                         */
  /**********************************************************************/
  if(!pMyObj->isDBRO())
  {
    pExCustomObjectsDic->upgradeOpen();
    pExCustomObjectsDic->setAt(sKey, pMyObj);
  }
}
