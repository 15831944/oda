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
#include "ExToPolylineCommand.h"
#include "StaticRxObject.h"
#include "DbUserIO.h"
#include "DbCommandContext.h"
#include "DbEntity.h"
#include "DbBlockTableRecord.h"
#include "DbSpline.h"

const OdString ToPolylineCommand::globalName() const
{
  return OD_T("ToPolyline");
}

const OdString ToPolylineCommand::groupName() const
{
  return OD_T("Spline");
}

void ToPolylineCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  if (pIO.isNull())
    return;

  OdString tmp;
  unsigned int convertedCount = 0, 
    selectedCount = 0, 
    splinesCount = 0;

  try
  {
    // Select entities
    OdDbSelectionSetPtr pSet = pIO->select(
      OD_T("Select objects for conversation to polylines:"), 
      OdEd::kSelAllowSubentsAlways, 0, OdString::kEmpty);
    selectedCount = pSet->numEntities();
    if (selectedCount == 0)
    {
      pIO->putString(OD_T("Nothing selected."));
      return;
    }
    else
    {
      tmp.format(OD_T("%d Entities selected"), convertedCount);
      pIO->putString(tmp);
    }

    // Get precision
    tmp.format(OD_T("Specify a precision <%d>"), iPrecision);
    iPrecision = pIO->getInt(tmp, OdEd::kInpNonNeg|OdEd::kInpDefault, iPrecision, OdString::kEmpty);
    if (iPrecision > 99)
      iPrecision = 99;

    // Iterate through selection
    OdDbSelectionSetIteratorPtr pIt;
    for (pIt = pSet->newIterator(); !pIt->done(); pIt->next())
    {
      OdDbObjectPtr pObj = pIt->objectId().safeOpenObject(OdDb::kForRead);

      // Cast to spline
      OdDbSplinePtr pEntity = OdDbSpline::cast(pObj);
      if (pEntity.isNull())
      {
        pIO->putString(OD_T("Selected entity is not a spline\n"));
        continue;
      }
      

      splinesCount++;

      // Convert to polyline
      OdDbCurve* curve = NULL;
      OdResult res = pEntity->toPolyline(curve, iPrecision);
      if (res != eOk)
      {
        pIO->putString(OdString().format(OD_T("Error : %s\n"), OdError(res).description().c_str()));
        continue;
      }

      // Break when no result curve returned
      if (!curve)
      {
        pIO->putString(OD_T("No polyline returned\n"));
        continue;
      }

      // Copy properties from old entity
      OdDbCurvePtr pPolyline = curve;
      pPolyline->setDatabaseDefaults(pDb);
      pPolyline->setPropertiesFrom(pEntity);

      // Get owner of entity for polyline insertion
      OdDbObjectId ownerId = pEntity->ownerId();
      if (ownerId && ownerId.database())
      {
        // Open owner for writing
        OdDbBlockTableRecordPtr pBlock = ownerId.openObject(OdDb::kForWrite);
        // Append newly created polyline
        pBlock->appendOdDbEntity(pPolyline);
        // Open object for deletion
        pObj = pIt->objectId().safeOpenObject(OdDb::kForWrite);
        pEntity->erase();
        // Increment counter
        convertedCount++;
      }
    }

    tmp.format(OD_T("%d of %d Splines converted"), convertedCount, splinesCount);
    pIO->putString(tmp);
  }
  catch (...)
  {
    pIO->putString(OD_T("Create Polyline Failed!"));
  }

}
