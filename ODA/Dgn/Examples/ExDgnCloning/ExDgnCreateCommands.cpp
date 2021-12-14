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
/* ExDgnCreateCommands.cpp: Defines commands fro creating custom objects */
/* with overridden subDeepClone() and subWblockClone() methods          */
/************************************************************************/
#include "OdaCommon.h"
//#include "ExDgnObject.h"
#include "ExDgnEntity.h"
#include "DgCommandContext.h"
#include "DgDatabase.h"
//#include "DbDictionary.h"
#include "DgUserIO.h"
#include "StaticRxObject.h"
//#include "DbBlockTableRecord.h"
#include "Gs/Gs.h"

const OdChar* ExDgnObjectsDicName = OD_T("EXDGNDEEPCLONINGPROPERTIES");

/************************************************************************/
/* Create ExDcObject command                                            */
/*                                                                      */
/* The NamedObjects dictionary store keys that reference the ExDcObject */
/* objects.                                                             */
/************************************************************************/
//void _EXDC_CREATEOBJECT_func(OdEdCommandContext* pCmdCtx)
//{
//  /**********************************************************************/
//  /* Establish command context, database, and UserIO                    */
//  /**********************************************************************/
//  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
//  OdDbDatabasePtr pDb = pDbCmdCtx->database();
//  OdEdUserIO* pIO = pDbCmdCtx->userIO();
//  
//  /**********************************************************************/
//  /* Link to NamedObjectsDictionary                                     */
//  /**********************************************************************/
//  OdDbDictionaryPtr pNamedObjectsDic = pDb->getNamedObjectsDictionaryId().safeOpenObject();
//
//  /**********************************************************************/
//  /* Open the custom dictionary                                         */
//  /**********************************************************************/
//  OdDbDictionaryPtr pExCustomObjectsDic = 
//    pNamedObjectsDic->getAt(ExDcObjectsDicName,OdDb::kForRead);
//  if (!pExCustomObjectsDic.get())
//  {
//    /********************************************************************/
//    /* Create a custom dictionary                                       */
//    /********************************************************************/
//    pExCustomObjectsDic = OdDbDictionary::createObject();
//    pNamedObjectsDic->upgradeOpen();
//    pNamedObjectsDic->setAt(ExDcObjectsDicName, pExCustomObjectsDic);
//    pNamedObjectsDic->downgradeOpen();
//  }
//  
//  ExDcObjectPtr pMyObj;
//  bool bDone = false;
//  OdString sKey;
//  OdString sDef;
//  do
//  {
//    /********************************************************************/
//    /* Get the Default Key                                              */
//    /********************************************************************/
//    sDef.format(OD_T("Property%d"), pExCustomObjectsDic->numEntries());
//    
//    /********************************************************************/
//    /* Get the Prompt, with default key in <>                           */
//    /********************************************************************/
//    sKey.format(OD_T("Enter object key <%ls>:"), sDef.c_str());
//
//    /********************************************************************/
//    /* Get a key with a default                                         */
//    /********************************************************************/
//    sKey = pIO->getString(sKey, OdEd::kGstAllowSpaces, sDef);
//    
//    /********************************************************************/
//    /* Return a pointer to object at the key.                            */
//    /********************************************************************/
//    OdDbObjectPtr pObj = pExCustomObjectsDic->getAt(sKey, OdDb::kForRead);
//    
//    if(pObj.get()) // key exists
//    {
//      /******************************************************************/
//      /* Key exists                                                     */
//      /******************************************************************/
//      pMyObj = ExDcObject::cast(pExCustomObjectsDic->getAt(sKey, OdDb::kForRead));
//      if(pMyObj.get()) // object of a kind OD_T("ExDcObject") exists
//      {
//        switch(pIO->getKeyword(OD_T("Key exists. [Replace/Edit existing/erase All existing/Reenter]?:"),
//                               OD_T("Replace Edit eraseAll Reenter"))
//              )
//        {
//        case 0: // Replace
//          /**************************************************************/
//          /* Replace the object for the specified key                   */
//          /**************************************************************/
//          pMyObj = ExDcObject::createObject();
//          break;
//        case 1: //Edit
//          /**************************************************************/
//          /* Edit the object for the specified key                      */
//          /**************************************************************/
//          pMyObj->upgradeOpen();
//          break;
//        case 2: //eraseAll
//          {
//            /************************************************************/
//            /* Erase all objects                                        */
//            /************************************************************/
//            OdDbDictionaryIteratorPtr pObjects = pExCustomObjectsDic->newIterator();
//            while(!pObjects->done())
//            {
//              int n = 0;
//              pMyObj = ExDcObject::cast(pObjects->object());
//              if(pMyObj.get())
//              {
//                pMyObj->upgradeOpen();
//                pMyObj->erase();
//                ++n;
//              }
//              pObjects->next();
//              OdString s;
//              s.format(OD_T("%d object(s) erased."));
//              pIO->putString(s);
//            }
//            /************************************************************/
//            /* Create a new object at the specified key                 */
//            /************************************************************/
//            pMyObj = ExDcObject::createObject();
//          }
//          break;
//        case 3: //Reenter
//          /**************************************************************/
//          /* Enter a different key                                      */
//          /**************************************************************/
//          continue;
//        default:
//          /**************************************************************/
//          /* Do nothing                                                 */
//          /**************************************************************/
//          break;
//        }
//        bDone = true;
//      }
//      else
//      {
//        /****************************************************************/
//        /* The key exists, but no object corresponds to it              */
//        /****************************************************************/
//        pIO->putString(OD_T("Key exists."));
//      }
//    }
//    else
//    {
//      /******************************************************************/
//      /* Key does not exist                                             */
//      /******************************************************************/
//      pMyObj = ExDcObject::createObject();
//      bDone = true;
//    }
//  }
//  while(!bDone);
//
//  /**********************************************************************/
//  /* Set the string property                                            */
//  /**********************************************************************/
//  pMyObj->setString(pIO->getString(OD_T("Enter string <\"\">:"), OdEd::kGstAllowSpaces, OD_T("")));
//
//  /**********************************************************************/
//  /* If the object is not already database resident, add it to the      */
//  /* NamedObjects dictionary, which makes it so                         */
//  /**********************************************************************/
//  if(!pMyObj->isDBRO())
//  {
//    pExCustomObjectsDic->upgradeOpen();
//    pExCustomObjectsDic->setAt(sKey, pMyObj);
//  }
//}
//
///************************************************************************/
///* Create ExDcEntity command                                            */
///************************************************************************/
//void _EXDC_CREATEENTITY_func(OdEdCommandContext* pCmdCtx)
//{
//  /**********************************************************************/
//  /* Establish command context, database, and UserIO                    */
//  /**********************************************************************/
//  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
//  OdDbDatabasePtr pDb = pDbCmdCtx->database();
//  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();
//
//  /**********************************************************************/
//  /* Open Model Space for writing                                       */
//  /**********************************************************************/
//  OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject(OdDb::kForWrite);
//
//  /**********************************************************************/
//  /* Create the ExDcEntity                                            */
//  /**********************************************************************/
//  ExDcEntityPtr pMyEnt = ExDcEntity::createObject();
//
//  /**********************************************************************/
//  /* Set the default properties common all entities                     */
//  /**********************************************************************/
//  pMyEnt->setDatabaseDefaults(pDb);
//
//  /**********************************************************************/
//  /* Set the center of the ExDcEntity                                 */
//  /**********************************************************************/
//  pMyEnt->setCenter( pIO->getPoint(OD_T("Enter circle center:"), true ) );
//  
//  /**********************************************************************/
//  /* Define a RadiusTracker to drag the radius                          */
//  /**********************************************************************/
//  struct RadiusTracker : OdStaticRxObject<OdEdRealTracker>
//  {
//    OdDbCirclePtr _ent;
//    RadiusTracker(const OdGePoint3d& center,OdDbDatabase* pDb)
//    {
//      /******************************************************************/
//      /* Use a circle to drag the radius.                               */
//      /******************************************************************/
//      _ent = OdDbCircle::createObject();
//      _ent->setDatabaseDefaults(pDb);
//      _ent->setCenter(center);
//    }
//
//    /********************************************************************/
//    /* Set the radius of the circle to the tracked real value           */
//    /********************************************************************/
//    virtual void setValue(double r){_ent->setRadius(r);}
//
//    /********************************************************************/
//    /* Add the dragged circle to the view                               */
//    /********************************************************************/
//    virtual int addDrawables(OdGsView* pView)
//    {
//      pView->add(_ent, 0);
//      return 1;
//    }
//
//    /********************************************************************/
//    /* Remove the dragged circle from the view                          */
//    /********************************************************************/
//    virtual void removeDrawables(OdGsView* pView)
//    {
//      pView->erase(_ent);
//    }
//  }
//  /**********************************************************************/
//  /* Initialize the tracker                                             */
//  /**********************************************************************/
//  tracker(pMyEnt->center(),pDb);
//  
//  /**********************************************************************/
//  /* Get the radius                                                     */
//  /**********************************************************************/
//  pMyEnt->setRadius( pIO->getDist(OD_T("\nSpecify radius of circle: "), OdEd::kGdsFromLastPoint, 0.0, OdString::kEmpty, &tracker ) );
//
//  /**********************************************************************/
//  /* Set owned objects                                                  */
//  /**********************************************************************/
//  int nOwnedIds = pIO->getInt(OD_T("\nSpecify number of properties: "));
//  for (int i = 0; i < nOwnedIds; ++i)
//  {
//    /**********************************************************************/
//    /* Create a new owned object and set the string                       */
//    /**********************************************************************/
//    OdString prompt;
//    prompt.format(OD_T("Enter property string (%d) <\"\">:"), i);
//    ExDcObjectPtr pOwnedObj = ExDcObject::createObject();
//    pOwnedObj->setString(pIO->getString(prompt, OdEd::kGstAllowSpaces, OD_T("")));
//    pMyEnt->addOwnedId(pDb->addOdDbObject(pOwnedObj));
//  }
//
//  /**********************************************************************/
//  /* Set a referenced object                                            */
//  /**********************************************************************/
//  OdDbDictionaryPtr pMainDic = pDb->getNamedObjectsDictionaryId().safeOpenObject();
//  OdDbDictionaryPtr pDic = pMainDic->getAt(ExDcObjectsDicName,OdDb::kForRead);
//  if (pDic.get())
//  {
//    OdString propName = pIO->getString(OD_T("Enter global property name <\"Property0\">:"), OdEd::kGstAllowSpaces, OD_T("Property0"));
//    if (!propName.isEmpty())
//    {
//      pMyEnt->setRefId(pDic->getAt(propName));
//    }
//  }
//
//
//  /**********************************************************************/
//  /* Add the ExDcEntity to Model Space                                */
//  /**********************************************************************/
//  pMS->appendOdDbEntity(pMyEnt);
//}
