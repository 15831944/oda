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

// ExLine.cpp: implementation of the ExLine function.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DbLine.h"

void _Line_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
  OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

  OdGeVector3d vNormal(ExCommandsUtils::ucsNormal(pDb));

  OdString strNext_U(L"Specify next point or [Undo]:");
  OdString strKeywords_U(L"Undo");

  OdString strNext_CU(L"Specify next point or [Undo/Close]:");
  OdString strKeywords_CU(L"Undo Close");
  OdGePoint3d ptFirst, ptStart, ptEnd;
  int nLines = 0;

  for (bool bFinished = false; !bFinished;)
  {
    ptStart = ptFirst = pIO->getPoint(L"Specify first point:");

    for (;;)
    {
      try
      {
        if (nLines >= 2)
        {
          ptEnd = pIO->getPoint(strNext_CU, OdEd::kGptRubberBand|OdEd::kInpThrowEmpty, NULL, strKeywords_CU);
        }
        else
        {
          ptEnd = pIO->getPoint(strNext_U, OdEd::kGptRubberBand|OdEd::kInpThrowEmpty, NULL, strKeywords_U);
        }
      }
      catch (const OdEdKeyword &kwd)
      {
        if (kwd.keywordIndex() == 1)
        { //Close
          ptEnd = ptFirst;
          bFinished = true;
        }
        else
        { //Undo
          if (nLines == 0)
          { //Undo first point
            break;
          }
          OdDbObjectId idLast = oddbEntLast(pDb);
          OdDbLinePtr pLine = idLast.openObject(OdDb::kForWrite);
          if (!pLine.isNull())
          {
            ptStart = pLine->startPoint();
            pLine->erase();
            pIO->setLASTPOINT(ptStart);
            --nLines;
          }
          else
          {
            ODA_FAIL();
          }
          continue;
        }
      }
      catch (const OdEdCancel& )
      {
        return;
      }

      OdDbLinePtr pNewLine = OdDbLine::createObject();
      pNewLine->setDatabaseDefaults(pDb);
      pNewLine->setNormal(vNormal);
      pNewLine->setStartPoint(ptStart);
      pNewLine->setEndPoint(ptEnd);
      pSpace->appendOdDbEntity(pNewLine);
      ptStart = ptEnd;
      ++nLines;

      if (bFinished)
        return;
    }
  }
}
