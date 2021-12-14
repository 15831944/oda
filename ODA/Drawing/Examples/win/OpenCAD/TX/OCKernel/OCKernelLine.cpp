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

#include "StdAfx.h"
#include "OCKernelLine.h"
#include "DbLine.h"
#include "DbBlockTableRecord.h"

void COCKernelLine::execute(OdEdCommandContext* pCmdCtx)
{
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx); // downcast for database access
	if(pDbCmdCtx.isNull())
	{
		MessageBox(AfxGetMainWnd()->m_hWnd, _T("This command needs an active drawing"), _T("Command unavailable"), MB_ICONWARNING);
		return;
	}
	OdDbDatabasePtr pDatabase = pDbCmdCtx->database(); // Current database
	OdDbUserIOPtr pUserIO = pDbCmdCtx->userIO();

	OdGePoint3d Start, End;
	OdDbBlockTableRecordPtr pBlockTableRecord = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
try
{
	Start = pUserIO->getPoint(_T("From point: "));
	while(1)
	{
		End = pUserIO->getPoint(_T("To point: "), OdEd::kGptRubberBand);

		OdDbLinePtr pLine = OdDbLine::createObject();
		pLine->setDatabaseDefaults(pDatabase);

		pLine->setStartPoint(Start);
		pLine->setEndPoint(End);

		pBlockTableRecord->appendOdDbEntity(pLine);

		Start = End;
	}
}
catch(OdEdCancel& /*e*/)
{
	return;
}
}
