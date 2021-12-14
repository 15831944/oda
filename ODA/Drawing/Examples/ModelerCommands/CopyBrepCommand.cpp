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
#include "CopyBrepCommand.h"
#include "RxDynamicModule.h"
#include "Ed/EdUserIO.h"
#include "DbCommandContext.h"
#include "StaticRxObject.h"
#include "OdErrorContext.h"
#include "DbBlockTableRecord.h"
#include "DbHostAppServices.h"
#include "Db3dSolid.h"
#include "DbSurface.h"
#include "DbNurbSurface.h"

#include "Br/BrBrep.h"
#include "ModelerGeometry.h"
#include "BrepBuilderFiller/Include/BrepBuilderFillerModule.h"
#include "BrepBuilder/BrepBuilder.h"

#include "BrepBuilderFiller/Include/BaseMaterialAndColorHelper.h"
#include "DbPolyline.h"
#include "DbBody.h"

class ValidBrepFilter : public OdStaticRxObject<OdEdSSetTracker>
{
public:

  ValidBrepFilter()
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

const OdString CopyBrepCommand::groupName() const
{
  return OD_T("MODELER GEOMETRY");
}

const OdString CopyBrepCommand::globalName() const
{
  return OD_T("copyBREP");
}

void CopyBrepCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  if (pIO.isNull())
    return;
  OdArray<OdDbEntityPtr> arrCreatedEntities;
  ValidBrepFilter filter;
  OdDbSelectionSetPtr pSet = pIO->select(OD_T("Select objects that will be copied:"), OdEd::kSelDefault, 0, OdString::kEmpty, &filter);
  OdDbSelectionSetPtr ss = OdDbSelectionSet::createObject(pDb);
  if (pSet->numEntities() == 0)
  {
    pIO->putString(OD_T("Nothing selected."));
    return;
  }

  bool bEnableBrepAudit = pIO->getKeyword(OD_T("Enable brep builder audit? <N>"), OD_T("N Y"), 0, OdEd::kInpDefault) == 1;

  OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  OdDbSelectionSetIteratorPtr pIt = pSet->newIterator();
  for (bool bItRes = true; bItRes; bItRes = pIt->next())
  {
    OdResult res = eOk;
    OdBrBrep brep;
    BrepType bbType = kOpenShell;
    OdBrepBuilder bbuilder;
    OdDbEntityPtr pRetEntity;
    EntityType enType;
    OdDbEntityPtr pEntity(pIt->objectId().safeOpenObject());
    if (pEntity->isKindOf(OdDbNurbSurface::desc()))
    {
      OdDbNurbSurfacePtr pBody = static_cast<OdDbNurbSurfacePtr>(pEntity);
      pBody->brep(brep);
      pRetEntity = OdDbNurbSurface::createObject();
      enType = kNurbSurface;
    }
    else if (pEntity->isKindOf(OdDbSurface::desc()))
    {
      OdDbSurfacePtr pBody = static_cast<OdDbSurfacePtr>(pEntity);
      pBody->brep(brep);
      pRetEntity = OdDbSurface::createObject();
      enType = kSurface;
    }
    else if (pEntity->isKindOf(OdDbRegion::desc()))
    {
      OdDbRegionPtr pBody = static_cast<OdDbRegionPtr>(pEntity);
      pBody->brep(brep);
      pRetEntity = OdDbRegion::createObject();
      enType = kRegion;
    }
    else if (pEntity->isKindOf(OdDb3dSolid::desc()))
    {
      OdDb3dSolidPtr pBody = static_cast<OdDb3dSolidPtr>(pEntity);
      pBody->brep(brep);
      pRetEntity = OdDb3dSolid::createObject();
      enType = k3dSolid;
      bbType = kSolid;
    }
    else if (pEntity->isKindOf(OdDbBody::desc()))
    {
      OdDbBodyPtr pBody = static_cast<OdDbBodyPtr>(pEntity);
      pBody->brep(brep);
      pRetEntity = OdDbBody::createObject();
    }
    else
    {
      pIO->putString(OD_T("Unsupported entity type."));
      continue;
    }
    try
    {
      pDb->appServices()->brepBuilder(bbuilder, bbType);
      bbuilder.enableValidator(bEnableBrepAudit);
      OdGeMatrix3d mx;
      bool bTransformed = brep.getTransformation(mx);

      OdBaseMaterialAndColorHelper materialHelper(pEntity->materialId());
      OdBrepBuilderFiller BBFiller;
      BBFiller.params().setupFor(pDb, pDb);
      if (BBFiller.initFrom(bbuilder, brep, &materialHelper) == eOk)
      {
        switch (enType)
        {
        case kSurface:
        {
          OdDbSurface* pSurf = (OdDbSurface*)(pRetEntity.get());
          pSurf->setBody(bbuilder.finish());
        }
        break;
        case kRegion:
        {
          OdDbRegion* pReg = (OdDbRegion*)(pRetEntity.get());
          pReg->setBody(bbuilder.finish());
        }
        break;
        case k3dSolid:
        {
          OdDb3dSolid* pSol = (OdDb3dSolid*)(pRetEntity.get());
          pSol->setBody(bbuilder.finish());
        }
        break;
        case kBody:
        {
          OdDbBody* pBody = (OdDbBody*)(pRetEntity.get());
          pBody->setBody(bbuilder.finish());
        }
        break;
        default:
          break;
        }
        if (bTransformed)
        {
          pRetEntity->transformBy(mx);
        }
        OdDbObjectId idNewEntity = pMs->appendOdDbEntity(pRetEntity);
        arrCreatedEntities.append(pRetEntity);
        OdCmColor clr =  pEntity->color();
        pRetEntity->setColor(clr,false);
        pRetEntity->setLayer(pEntity->layerId());
        OdDbObjectId matId = pEntity->materialId();
        pRetEntity->setMaterial(matId);
      }
      else
      {
        pIO->putString(OD_T("Can't get BREP data for processing (brep filler error"));
      }
    }
    catch (OdError &err)
    {
      OdString tmp;
      tmp.format(OD_T("Error : %s"), err.description().c_str());
      pIO->putString(tmp);
      continue;
    }
    // catch  all exceptions
    catch (...)
    {
      pIO->putString(OD_T("Unknown error was found"));
    }
  }

  struct MouseMoveTracker : OdStaticRxObject<OdEdPointTracker>
  {
    OdArray<OdDbEntityPtr> m_arrCreatedEntities;
    OdDbDatabasePtr m_pDb;
    OdGePoint3d m_ptFirst;
    OdGsView* m_pGsView;

    MouseMoveTracker(OdDbDatabasePtr pDb, OdArray<OdDbEntityPtr>& entitites) : m_pGsView(NULL)
    {
      m_arrCreatedEntities.append(entitites);
      OdGeExtents3d ext3d;
      for (unsigned int i = 0; i < m_arrCreatedEntities.length(); i++)
      {
        OdGeExtents3d ext3dCurrent;
        m_arrCreatedEntities[i]->getGeomExtents(ext3dCurrent);
        ext3d.addExt(ext3dCurrent);
      }
      OdGePoint3d pntCenter = ext3d.center();
      m_ptFirst = pntCenter;
    }

    virtual void setValue(const OdGePoint3d & val)
    {
      OdGeMatrix3d mx(OdGeMatrix3d::translation(val - m_ptFirst));
      for (unsigned int i = 0; i < m_arrCreatedEntities.length(); i++)
      {
        m_arrCreatedEntities[i]->transformBy(mx);
      }
      m_ptFirst = val;
    }

    virtual int addDrawables(OdGsView* pView)
    {
      m_pGsView = pView;
      for (unsigned int i = 0; i < m_arrCreatedEntities.length(); i++)
      {
        pView->add(m_arrCreatedEntities[i], 0);
      }
      return 0;
    }

    virtual void removeDrawables(OdGsView* pView)
    {
      for (unsigned int i = 0; i < m_arrCreatedEntities.length(); i++)
      {
        pView->erase(m_arrCreatedEntities[i]);
      }
    }
  } MmTracker(pDb, arrCreatedEntities);
  OdGePoint3d ptEnd = pIO->getPoint(L"Move to...", 0, 0, OdString::kEmpty, &MmTracker);

}

