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
#include "ExExtrudeConnectedFacesSubDMesh.h"
#include "StaticRxObject.h"
#include "DbUserIO.h"
#include "DbCommandContext.h"
#include "DbBlockTableRecord.h"
#include "DbSubDMesh.h"

class MeshFilter : public OdStaticRxObject<OdEdSSetTracker>
{
public:

  MeshFilter()
  {}

  int addDrawables(OdGsView*) { return 0; }
  void removeDrawables(OdGsView*) { }

  bool check(const OdDbObjectId& entId)
  {
    return true;
  }

  bool append(const OdDbObjectId& entId, const OdDbSelectionMethod*)
  {
    return check(entId);
  }

  bool remove(const OdDbObjectId& entId, const OdDbSelectionMethod*)
  {
    return check(entId);
  }
};

const OdString ExtrudeConnectedFacesCommand::globalName() const
{
  return OD_T("extrudeconnectedfaces");
}

const OdString ExtrudeConnectedFacesCommand::groupName() const
{
  return OD_T("MODELER GEOMETRY");
}

void ExtrudeConnectedFacesCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  if (pIO.isNull())
    return;

  try
  {
    MeshFilter filter;
    OdDbSelectionSetPtr pSet = pIO->select(OD_T("Select object for extrusion:"), OdEd::kSelSingleEntity, 0, OdString::kEmpty, &filter);
    if (pSet->numEntities() == 0)
    {
      pIO->putString(OD_T("Nothing selected."));
      return;
    }

    bool  iDirZero = pIO->getInt(OD_T("Is direction zero? default false"),
      OdEd::kInpDefault,
      0);

    double length = pIO->getReal(OD_T("Enter extrusion length(default 1)"),
      OdEd::kInpDefault,
      1);

    OdInt32 countFaces = pIO->getInt(OD_T("Enter number faces (default 1)"),
      OdEd::kInpDefault,
      1);

    OdDbSelectionSetIteratorPtr pIt = pSet->newIterator();
    unsigned int surfaceCount = 0;

    while (!pIt->done())
    {
      OdDbObjectPtr pObj = pIt->objectId().safeOpenObject(OdDb::kForWrite);

      OdDbEntityPtr pEntity = OdDbEntity::cast(pObj);
      if (pEntity.isNull() || !pEntity->isKindOf(OdDbSubDMesh::desc()))
      {
        pIO->putString(OD_T("Unsupported entity."));
        return;
      }

      OdDbSubDMesh* pMesh = static_cast<OdDbSubDMesh*>(pEntity.get());
      OdInt32 numOfFaces;
      pMesh->numOfFaces(numOfFaces);
      OdDbFullSubentPathArray pathArray;
      for(OdInt32 t = 0; t < numOfFaces; ++t)
      {
        OdGeVector3d dir;
        for (OdInt32 t1 = 0; t1 < countFaces; ++t1, ++t)
        {
          pathArray.append(OdDbFullSubentPath(OdDb::kFaceSubentType, t));
          OdGePlane plane;
          pMesh->getFacePlane(OdDbSubentId(OdDb::kFaceSubentType, t), plane);
          dir = plane.normal();
        }
        const OdResult res = pMesh->extrudeConnectedFaces(pathArray, length, iDirZero ? OdGeVector3d() : dir, 0);
        pathArray.clear();
        if (res != eOk)
        {
          OdString tmp;
          tmp.format(OD_T("Error : %s"), OdError(res).description().c_str());
          pIO->putString(tmp);
          return;
        }
      }
      

      pIt->next();
    }
    OdString tmp;
    tmp.format(OD_T("Extrude is done"));
    pIO->putString(tmp);
  }
  catch (...)
  {
    pIO->putString(OD_T("Extrude is Failed!"));
  }

}

