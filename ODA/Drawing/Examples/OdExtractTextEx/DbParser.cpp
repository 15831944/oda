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
#include "ExProtocolExtension.h"
#include "DbEntity.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "DbParser.h"


void DbParser::parseEntity(const OdDbEntity* pEnt, int indent)
{
  if (pEnt == NULL)
  {
    ODA_FAIL(); // should not get here
    return;
  }
  /**********************************************************************/
  /* Retrieve the Protocol Extension registered for this object type    */
  /**********************************************************************/
  OdDbEntityParserPtr pEntParser = OdDbEntity_Parser::cast(pEnt);
  if (pEntParser.isNull())
  {
    ODA_FAIL(); // should not get here
    return;
  }
  /**********************************************************************/
  /* Parse the entity and extract text                                  */
  /**********************************************************************/
  pEntParser->extractText(pEnt, indent);
} 

/************************************************************************/
/* Parse the database                                                    */
/************************************************************************/
void DbParser::parse(const OdDbDatabase* pDb, int indent)
{
  /**********************************************************************/
  /* Get a SmartPointer to the BlockTable                               */
  /**********************************************************************/
  OdDbBlockTablePtr pTable = pDb->getBlockTableId().safeOpenObject();

  /**********************************************************************/
  /* Get a SmartPointer to a new SymbolTableIterator                    */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pBlkIter = pTable->newIterator();

  /**********************************************************************/
  /* Step through the BlockTable                                        */
  /**********************************************************************/
  for (pBlkIter->start(); ! pBlkIter->done(); pBlkIter->step())
  {
    /********************************************************************/
    /* Open the BlockTableRecord for Reading                            */
    /********************************************************************/
    OdDbBlockTableRecordPtr pBlock = pBlkIter->getRecord();
    /*********************************************************************/
    /* Omit Dimension blocks because they don't contains meaningful text */
    /*********************************************************************/
    if(pBlock->isAnonymous() && (pBlock->getName().left(2) == L"*D"))
    {
      continue;
    }
    OdDbObjectIteratorPtr pEntIter = pBlock->newIterator();
    /********************************************************************/
    /* Step through the BlockTableRecord                                */
    /********************************************************************/
    for (; !pEntIter->done(); pEntIter->step())
    {
      OdDbEntityPtr pEnt = pEntIter->entity();
      /********************************************************************/
      /* Parse the Entity                                                  */
      /********************************************************************/
      parseEntity(pEnt, indent+1);
    }
  }
}
