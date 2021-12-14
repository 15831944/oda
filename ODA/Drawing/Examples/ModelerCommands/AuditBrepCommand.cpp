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
#include "AuditBrepCommand.h"
#include "../Extensions/BrepBuilderFiller/Include/BaseMaterialAndColorHelper.h"
#include "../Extensions/BrepBuilderFiller/Include/BrepBuilderFillerModule.h"
#include "Br/BrBrep.h"
#include "BrepBuilder/BrepBuilder.h"
#include "CmColorBase.h"
#include "Db3dSolid.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "DbBody.h"
#include "DbCommandContext.h"
#include "DbDatabase.h"
#include "DbDictionary.h"
#include "DbHostAppServices.h"
#include "DbMaterial.h"
#include "DbNurbSurface.h"
#include "DbObjectId.h"
#include "DbPolyline.h"
#include "DbSurface.h"
#include "Ed/EdUserIO.h"
#include "Ge/GeSurfacePtrArray.h"
#include "ModelerGeometry.h"
#include "OdErrorContext.h"
#include "OdString.h"
#include "RxDynamicModule.h"
#include "StaticRxObject.h"

class ValidBrepFilter : public OdStaticRxObject<OdEdSSetTracker>
{
  bool m_bModelSpaceOnly;
public:

  ValidBrepFilter(bool bModelSpaceOnly = false)
  {}

  int addDrawables(OdGsView*) { return 0; }
  void removeDrawables(OdGsView*) { }

  bool check(const OdDbObjectId& entId)
  {
    OdDbEntityPtr pEnt = entId.openObject();
    return pEnt->isKindOf(OdDbRegion::desc()) || pEnt->isKindOf(OdDb3dSolid::desc()) || pEnt->isKindOf(OdDbSurface::desc()) || pEnt->isKindOf(OdDbBody::desc());
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

const OdString AuditBrepCommand::groupName() const
{
  return OD_T("MODELER GEOMETRY");
}

const OdString AuditBrepCommand::globalName() const
{
  return OD_T("auditBREP");
}

inline bool checkEntity(OdEdCommandContext* pCmdCtx, OdDbEntityPtr pEntity)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdString sBuf;
  OdBrBrep brep;
  BrepType bbType = kOpenShell;
  OdBrepBuilder bbuilder;
  if (pEntity->isKindOf(OdDbSurface::desc()))
  {
    OdDbSurfacePtr pBody = static_cast<OdDbSurfacePtr>(pEntity);
    pBody->brep(brep);
  }
  else if (pEntity->isKindOf(OdDbRegion::desc()))
  {
    OdDbRegionPtr pBody = static_cast<OdDbRegionPtr>(pEntity);
    pBody->brep(brep);
  }
  else if (pEntity->isKindOf(OdDb3dSolid::desc()))
  {
    OdDb3dSolidPtr pBody = static_cast<OdDb3dSolidPtr>(pEntity);
    pBody->brep(brep);
    bbType = kSolid;
  }
  else if (pEntity->isKindOf(OdDbBody::desc()))
  {
    OdDbBodyPtr pBody = static_cast<OdDbBodyPtr>(pEntity);
    pBody->brep(brep);
  }
  else
  {
    return true;
  }
  try
  {
    pDb->appServices()->brepBuilder(bbuilder, bbType);
    OdBrepBuilderFiller BBFiller;
    BBFiller.params().setupFor(OdBrepBuilderFillerParams::kBrepAcisDwg, OdBrepBuilderFillerParams::kBrepAcisDwg);
    if (BBFiller.initFrom(bbuilder, brep) == eOk)
    {
      OdRxObjectPtr pObj = bbuilder.finish();
    }
    else
    {
      throw OdError(OdString(OD_T("Can't get BREP data for processing (brep filler error)")));
    }
  }
  catch (OdError &err)
  {
    pIO->putString(sBuf.format(OD_T("Error : %s"), err.description().c_str()));
    return false;
  }
  return true;
}

inline OdDbObjectIdArray getTableRecords(OdDbObjectId tableId)
{
  OdDbSymbolTablePtr pTable = tableId.safeOpenObject();
  OdDbSymbolTableIteratorPtr pIter = pTable->newIterator();
  OdDbObjectIdArray ids;
  for (; !pIter->done(); pIter->step())
  {
    ids.append(pIter->getRecordId());
  }
  return ids;
}

void AuditBrepCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdString sBuf;
  OdUInt32 invalidCount = 0;
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  if (pIO.isNull())
    return;

  // Get mark mode from User
  bool bColoring(false);
  bColoring = pIO->getKeyword(OD_T("Select mode [Erasing/Coloring]:"),
    OD_T("Erasing Coloring"), 0, OdEd::kInpDefault) == 1;

  bool bRemove(false);
  if (bColoring == 0)
  {
    bRemove = pIO->getKeyword(OD_T("What to erase [Invalid/Valid]:"),
      OD_T("Invalid Valid"), 0, OdEd::kInpDefault) == 1;
  }

  // Get selection mode from User
  OdUInt32 nMode(0);
  nMode = pIO->getKeyword(OD_T("Choose selection mode [Database/ModelSpace/Selection]:"),
    OD_T("Database ModelSpace Selection"), 0, OdEd::kInpDefault);

  // Select Set of Entities
  ValidBrepFilter ioFilter;
  OdDbSelectionSetPtr pSet;
  OdDbObjectIdArray aRecords;
  switch (nMode)
  {
  case 0:
    aRecords = getTableRecords(pDb->getBlockTableId());
    break;
  case 1:
    aRecords.append(pDb->getModelSpaceId());
    break;
  case 2:
    pSet = pIO->select(OD_T("Select objects that will be audited:"), OdEd::kSelDefault, 0, OdString::kEmpty, &ioFilter);
    if (pSet->numEntities() == 0)
    {
      pIO->putString(OD_T("Nothing selected."));
      return;
    }
    break;
  }

  // For coloring mode
  OdCmColor colorInvalid;
  colorInvalid.setRGB(255, 0, 0);
  OdCmColor colorValid;
  colorValid.setRGB(0, 255, 0);
  if (!m_mtlId && bColoring)
  {
    OdDbMaterial mater;
    mater.setDiffuse(OdCmEntityColor(OdCmEntityColor()), OdGiMaterialMap());

    const OdConstString InvalidMaterialName(OD_T("InvalidEntitiesMaterial"));
    const OdConstString KeyMaterialName(OD_T("InvalidEntitiesMaterial"));
    OdDbMaterialPtr pMtl = OdDbMaterial::createObject();
    pMtl->setName(InvalidMaterialName);
    OdDbDictionaryPtr pDic = pDb->getMaterialDictionaryId().openObject(OdDb::kForWrite);
    m_mtlId = pDic->setAt(KeyMaterialName, pMtl);
  }

  switch (nMode)
  {
  case 0:
  case 1:
    for (unsigned i = 0; i < aRecords.size(); ++i)
    {
      OdDbBlockTableRecordPtr pRec = aRecords[i].safeOpenObject(OdDb::kForWrite);
      for (OdDbObjectIteratorPtr pItR = pRec->newIterator(); !pItR->done(); pItR->step())
      {
        OdDbEntityPtr pEntity(pItR->objectId().safeOpenObject(OdDb::kForWrite));
        bool isOk = checkEntity(pCmdCtx, pEntity);
        if (bColoring)
        {
          pEntity->setMaterial(m_mtlId);
          pEntity->setColor(isOk ? colorValid : colorInvalid);
        }
        else
        {
          if (bRemove == isOk)
          {
            pEntity->erase();
          }
        }
        if (!isOk)
        {
          invalidCount++;
        }
      }
    }
    break;
  case 2:
    OdDbSelectionSetIteratorPtr pIt = pSet->newIterator();
    for (bool bItRes = true; bItRes; bItRes = pIt->next())
    {
      OdDbEntityPtr pEntity(pIt->objectId().safeOpenObject(OdDb::kForWrite));
      bool isOk = checkEntity(pCmdCtx, pEntity);
      if (bColoring)
      {
        pEntity->setMaterial(m_mtlId);
        pEntity->setColor(isOk ? colorValid : colorInvalid);
      }
      else
      {
        if (bRemove == isOk)
        {
          pEntity->erase();
        }
      }
      if (!isOk)
      {
        invalidCount++;
      }
    }
    break;
  }
  pIO->putString(sBuf.format(OD_T("Invalid entities count: %d"), invalidCount));
}
