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
#include "DbAlignedDimension.h"



void _DimAligned_func(OdEdCommandContext* pCmdCtx) {
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
	OdDbDatabasePtr pDb = pDbCmdCtx->database();
	OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

	OdGePoint3d start = pIO->getPoint("set start point");
	OdGePoint3d end   = pIO->getPoint("set end point");
	OdGePoint3d dimLn = pIO->getPoint("set dimension point");

	OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
	OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

	OdDbAlignedDimensionPtr pDimension = OdDbAlignedDimension::createObject();
	pDimension->setDatabaseDefaults(pDb);

	pSpace->appendOdDbEntity(pDimension);

	pDimension->setXLine1Point(start);
	pDimension->setXLine2Point(end);
	pDimension->setDimLinePoint(dimLn);
	pDimension->useDefaultTextPosition();
	pDimension->setJogSymbolHeight(1.5);
}
