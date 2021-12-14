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
#include "Ed/EdUserIO.h"
#include "Ed/EdCommandStack.h"
#include "DbCommandContext.h"
#include "DbDatabase.h"
#include "RxObject.h"

// IFC SDK includes
#include "IfcCore.h"
#include "IfcFile.h"
#include "IfcHostAppServices.h"
#include "sdai/daiObjectId.h"

using namespace OdIfc;

void _IfcExportEntity_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdString fileName = pIO->getString(OD_T("Enter IFC file name :"));
  if (fileName.isEmpty())
  {
    pIO->putString(OD_T("Empty filename entered"));
    return;
  }

  fileName.replace(OdChar('\\'), OdChar('/'));
  if (fileName.find(OD_T(".ifc")) == -1)
    fileName += ".ifc";

  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select entity to export in IFC file"));

  OdDbHandle contextId = pIO->getInt(OD_T("Enter target IfcGeometricRepresentationContext id : "), 0, 0);
  if ((OdUInt64)contextId <= 0)
  {
    pIO->putString(OD_T("Incorrect context id"));
    return;
  }

  OdDbHandle ownerId = pIO->getInt(OD_T("Enter target IfcProduct id : "), 0, 0);
  if ((OdUInt64)ownerId <= 0)
  {
    pIO->putString(OD_T("Incorrect context id"));
    return;
  }

  OdDbDatabase* pDb = pDbCmdCtx->database();

  OdResult res = eSubSelectionSetEmpty;
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

  odIfcInitialize(false, false);
  
  {
    // Ifc objects block. Don't remove brackets, cause after odIfcUninitialize 
    // some ifc objects destruction will be crushed.  

    OdString newFileName = pIO->getString(OD_T("Enter new IFC file name :"));
    if (newFileName.isEmpty())
    {
      pIO->putString(OD_T("Empty filename entered"));
      return;
    }

    newFileName.replace(OdChar('\\'), OdChar('/'));
    if (newFileName.find(OD_T(".ifc")) == -1)
      newFileName += ".ifc";

    OdStaticRxObject<OdIfcHostAppServices> svcs;
    OdIfcFilePtr pFile = svcs.createDatabase();
    if (OdResult res = pFile->readFile(fileName))
    {
      pIO->putString(OD_T("Failed to read IFC file"));
      return;
    }

    OdIfcModelPtr pModel = pFile->getModel(sdaiRW);
    if (pModel.isNull())
    {
      pIO->putString(OD_T("Failed to get IFC model"));
      return;
    }

    auto modelSchema = pModel->underlyingSchema();
    int shapeRepresentation = 1;

    if (!modelSchema->find("ifcpolygonalfaceset").isNull() || 
        !modelSchema->find("ifctriangulatedfacecset").isNull())
    { 
      shapeRepresentation = pIO->getInt("1-closed shell, 2-triangulated, 3-polygonal");
    }

    OdDAIObjectId shapeId;
    if (!pIter->done())
    {
      OdDbObjectPtr pObject = pIter->objectId().openObject();
      OdDAIObjectId daiShapeId = pModel->getEntityInstance(contextId);
      if (res = getDrawableShapeRepresentation(pObject,
                                               daiShapeId, 
                                               pModel, 
                                               shapeId, 
                                               shapeRepresentation))
      {
        pIO->putString(OD_T("Failed to import entity"));
        return;
      }
    }

    OdIfcEntityPtr pOwner = pFile->get(pModel->getEntityInstance(ownerId));
    if (pOwner.isNull() || !(pOwner->putAttr("representation", shapeId)))
    {
      pIO->putString(OD_T("Failed to put shape representation"));
      return;
    }

    pFile->writeFile(newFileName);
  
  }

  odIfcUninitialize();

  if (res == eOk)
  {
    pIO->putString(OD_T("Exported successfully."));
  }
  else
  {
    OdString tmp = OD_T("Export error: ") + OdError(res).description();
    pIO->putString(tmp);
  }
}

void _IfcExportEntities_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdString fileName = pIO->getString(OD_T("Enter IFC file name :"));
  if (fileName.isEmpty())
  {
    pIO->putString(OD_T("Empty filename entered"));
    return;
  }
  fileName.replace(OdChar('\\'), OdChar('/'));
  if (fileName.find(OD_T(".ifc")) == -1)
    fileName += ".ifc";

  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select entities to export in IFC file"));

  OdDbHandle contextId = pIO->getInt(OD_T("Enter target IfcGeometricRepresentationContext id : "), 0, 0);
  if ((OdUInt64)contextId <= 0)
  {
    pIO->putString(OD_T("Incorrect context id"));
    return;
  }

  OdDbHandle ownerId = pIO->getInt(OD_T("Enter target IfcProduct id : "), 0, 0);
  if ((OdUInt64)ownerId <= 0)
  {
    pIO->putString(OD_T("Incorrect context id"));
    return;
  }

  OdDbDatabase* pDb = pDbCmdCtx->database();

  OdResult res = eSubSelectionSetEmpty;
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

  odIfcInitialize(false, false);

  {
    // Ifc objects block. Don't remove brackets, cause after odIfcUninitialize 
    // some ifc objects destruction will be crushed.  

    OdString newFileName = pIO->getString(OD_T("Enter new IFC file name :"));
    if (newFileName.isEmpty())
    {
      pIO->putString(OD_T("Empty filename entered"));
      return;
    }

    newFileName.replace(OdChar('\\'), OdChar('/'));
    if (newFileName.find(OD_T(".ifc")) == -1)
      newFileName += ".ifc";

    OdStaticRxObject<OdIfcHostAppServices> svcs;
    OdIfcFilePtr pFile = svcs.createDatabase();
    if (OdResult res = pFile->readFile(fileName))
    {
      pIO->putString(OD_T("Failed to read IFC file"));
      return;
    }

    OdIfcModelPtr pModel = pFile->getModel(sdaiRW);
    if (pModel.isNull())
    {
      pIO->putString(OD_T("Failed to get IFC model"));
      return;
    }

    auto modelSchema = pModel->underlyingSchema();
    int shapeRepresentation = 1;

    if (!modelSchema->find("ifcpolygonalfaceset").isNull() ||
      !modelSchema->find("ifctriangulatedfacecset").isNull())
    {
      shapeRepresentation = pIO->getInt("1-closed shell, 2-triangulated, 3-polygonal");
    }

    OdDAIObjectId shapeId;
    OdDAIObjectId daiShapeId = pModel->getEntityInstance(contextId);

    OdArray<OdGiDrawablePtr> entities;
    while (!pIter->done())
    {
      entities.append(pIter->objectId().openObject());
      pIter->next();
    }

    if (res = getDrawableShapeRepresentation(entities, daiShapeId, pModel, shapeId, shapeRepresentation))
    {
      pIO->putString(OD_T("Failed to import entities"));
      return;
    }

    OdIfcEntityPtr pOwner = pFile->get(pModel->getEntityInstance(ownerId));
    if (pOwner.isNull() || !(pOwner->putAttr("representation", shapeId)))
    {
      pIO->putString(OD_T("Failed to put shape representation"));
      return;
    }

    pFile->writeFile(newFileName);

  }

  odIfcUninitialize();

  if (res == eOk)
  {
    pIO->putString(OD_T("Exported successfully."));
  }
  else
  {
    OdString tmp = OD_T("Export error: ") + OdError(res).description();
    pIO->putString(tmp);
  }
}
/*
#else
void _IfcExportEntity_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

#ifdef _TOOLKIT_IN_DLL_
  pIO->putString(OD_T("IFC project should be enabled for export into IFC format..."));
#else
  pIO->putString(OD_T("Export into IFC format not enable for static build..."));
#endif
}

#endif
*/
