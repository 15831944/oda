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
#include "DbGrip.h"
#include "Gi/GiViewportDraw.h"
#include "DbAttribute.h"
#include "Ge/GeCircArc3d.h"
#include "DynamicBlocks/DbBlockRepresentation.h"
#include "DynamicBlocks/DbBlockActions.h"
#include "DynamicBlocks/DbBlockConstraintParameters.h"
#include "DynamicBlocks/DbBlockPropertiesTable.h"
#include "DynamicBlocks/DbBlockVisibilityParameter.h"
#include "DbDynBlockReference.h"
#include "GripPoints.h"
#include "DbBlockTableRecord.h"
#include "DbTransactionWrapper.h"

ODRX_CONS_DEFINE_MEMBERS(OdDbSBAppData, OdRxObject, RXIMPL_CONSTR);

static double getGripSize(OdGiViewportDraw* pWd, const OdGePoint3d& eyePt, int gripSize)
{
  OdGePoint2d ptDim;
  OdGePoint3d wcsPt(eyePt); wcsPt.transformBy(pWd->viewport().getEyeToWorldTransform());
  pWd->viewport().getNumPixelsInUnitSquare(wcsPt, ptDim);
  OdGeVector3d v(gripSize / ptDim.x, 0, 0);
  v.transformBy(pWd->viewport().getWorldToEyeTransform());
  return v.length();
}

static void drawFlipArrow(OdGiViewportDraw* pWd, const OdGePoint3d& p, int gripSize, const OdGeVector3d& orient)
{
  OdGeVector3d v(orient);
  v.normalize();
  double dGripSize = getGripSize(pWd, p, gripSize);
  v *= dGripSize;
  OdGeVector3d n(v);
  n.rotateBy(OdaPI2, OdGeVector3d::kZAxis);
  n *= 0.75;
  OdGePoint3d pp[7];
  pp[0] = p + n * 0.5;
  pp[1] = p + n;
  pp[3] = p - n;
  pp[4] = p - n * 0.5;
  pp[2] = p + v;
  pp[5] = p - v - n * 0.5;;
  pp[6] = p - v + n * 0.5;;
  pWd->geometry().polygonEye(7, pp);
}

static void drawTriangle(OdGiViewportDraw* pWd, const OdGePoint3d& p, int gripSize, const OdGeVector3d& orient)
{
  OdGeVector3d v(orient);
  v.normalize();
  double dGripSize = getGripSize(pWd, p, gripSize);
  v *= dGripSize;
  OdGePoint3d pp[3];
  pp[0] = p + v;
  v.rotateBy(OdaPI * 0.75, OdGeVector3d::kZAxis);
  pp[1] = p + v;
  v.rotateBy(OdaPI2, OdGeVector3d::kZAxis);
  pp[2] = p + v;
  pWd->geometry().polygonEye(3, pp);
}

static void drawSquare(OdGiViewportDraw* pWd, const OdGePoint3d& p, int gripSize)
{
  double dGripSize = getGripSize(pWd, p, gripSize);
  OdGePoint3d pp[4];
  pp[0] = OdGePoint3d(p.x - dGripSize, p.y - dGripSize, p.z);
  pp[1] = OdGePoint3d(p.x - dGripSize, p.y + dGripSize, p.z);
  pp[2] = OdGePoint3d(p.x + dGripSize, p.y + dGripSize, p.z);
  pp[3] = OdGePoint3d(p.x + dGripSize, p.y - dGripSize, p.z);
  pWd->geometry().polygonEye(4, pp);
}

static void drawCircle(OdGiViewportDraw* pWd, const OdGePoint3d& p, int gripSize)
{
  double dGripSize = getGripSize(pWd, p, gripSize);
  OdGeCircArc3d c(p, OdGeVector3d::kZAxis, dGripSize);
  OdGePoint3dArray pp;
  c.getSamplePoints(8, pp);
  pWd->geometry().polygonEye(pp.size(), pp.asArrayPtr());
}

static void drawMenuTriangle(OdGiViewportDraw* pWd, const OdGePoint3d& p, int gripSize)
{
  double dGripSize = getGripSize(pWd, p, gripSize);
  OdGePoint3d pp[4];
  pp[0] = OdGePoint3d(p.x - dGripSize, p.y - dGripSize/6, p.z);
  pp[1] = OdGePoint3d(p.x - dGripSize, p.y + dGripSize/6, p.z);
  pp[2] = OdGePoint3d(p.x + dGripSize, p.y + dGripSize/6, p.z);
  pp[3] = OdGePoint3d(p.x + dGripSize, p.y - dGripSize/6, p.z);
  pWd->geometry().polygonEye(4, pp);
  OdGePoint3d pp1[3];
  pp1[0] = OdGePoint3d(p.x - dGripSize, p.y - 3*dGripSize/6, p.z);
  pp1[1] = OdGePoint3d(p.x + dGripSize, p.y - 3*dGripSize/6, p.z);
  pp1[2] = OdGePoint3d(p.x, p.y - dGripSize*2, p.z);
  pWd->geometry().polygonEye(3, pp1);
}

static void SbGripViewportDraw(OdDbGripData* pThis, OdGiViewportDraw* pWd, OdDbStub* entId,
  OdDbGripOperations::DrawType type, OdGePoint3d* imageGripPoint, int gripSize)
{
  ODA_ASSERT(pThis->appDataOdRxClass() == OdDbSBAppData::desc());
  OdGePoint3d p = imageGripPoint == 0 ? pThis->gripPoint() : *imageGripPoint;
  p.transformBy(pWd->viewport().getWorldToEyeTransform());
  OdDbBlockGripPtr grip = ((OdDbSBAppData*)pThis->appData())->m_pGrip;
  switch (type)
  {
  case OdDbGripOperations::kWarmGrip:
    if (grip.isNull())
      pWd->subEntityTraits().setColor(OdCmEntityColor::kACIBlue);
    else
      pWd->subEntityTraits().setColor(OdCmEntityColor::kACICyan);
    break;
  case OdDbGripOperations::kHoverGrip:
    pWd->subEntityTraits().setColor(OdCmEntityColor::kACIRed);
    break;
  case OdDbGripOperations::kHotGrip:
    pWd->subEntityTraits().setColor(OdCmEntityColor::kACIMagenta);
    break;
  case OdDbGripOperations::kDragImageGrip:
    pWd->subEntityTraits().setColor(OdCmEntityColor::kACIBlue);
    break;
  }
  pWd->subEntityTraits().setFillType(kOdGiFillAlways);
  if (grip.isNull())
    drawSquare(pWd, p, gripSize);
  else if (grip->isKindOf(OdDbBlockRotationGrip::desc()))
    drawCircle(pWd, p, gripSize);
  else if (grip->isKindOf(OdDbBlockLinearGrip::desc()))
  {
    OdGeVector3d orient = ((OdDbBlockLinearGrip*)grip.get())->orientation();
    OdDbBlockReferencePtr br = OdDbObjectId(entId).safeOpenObject();
    orient.transformBy(br->blockTransform());
    orient.transformBy(pWd->viewport().getWorldToEyeTransform());
    drawTriangle(pWd, p, gripSize, orient);
  }
  else if (grip->isKindOf(OdDbBlockVisibilityGrip::desc())||grip->isKindOf(OdDbBlockLookupGrip::desc()))
    drawMenuTriangle(pWd, p, gripSize);
  else if (grip->isKindOf(OdDbBlockFlipGrip::desc()))
    drawFlipArrow(pWd, p, gripSize, OdDbBlockFlipGripPtr(grip)->orientation());
  else // polar, XY
    drawSquare(pWd, p, gripSize);
}

OdResult updateGraph(OdDbBlockRepresentationContext* ctx, OdDbEvalNodeId id);
static OdDbSBAppData* s_currentData = 0;
void SbContextMenuItemIndexPtr( unsigned itemIndex )
{
  if (s_currentData)
  {
    if (itemIndex != 0)
    {
      OdDbTransactionWrapper tm(s_currentData->m_pContext->getGraph()->database());
      if (s_currentData->m_pParameter->isKindOf(OdDbBlockPropertiesTable::desc()))
        s_currentData->m_pParameter->setPropertyValue(s_currentData->m_sPropertyName, OdResBuf::newRb(OdResBuf::kDxfInt16, OdInt16(itemIndex - 1)));
      else
        s_currentData->m_pParameter->setPropertyValue(s_currentData->m_sPropertyName, OdResBuf::newRb(0, s_currentData->m_sValues[itemIndex - 1]));
      updateGraph(s_currentData->m_pContext, s_currentData->m_pParameter->nodeId());
      tm.endTransaction();
    }
  }
  s_currentData = 0;
}

OdResult SbGripRtClkHandler(OdDbGripDataArray& hotGrips, const OdDbStubPtrArray& ents, OdString& menuName, 
  ODHMENU& menu, ContextMenuItemIndexPtr& cb )
{
  for (unsigned i = 0; i < hotGrips.size(); ++i)
  {
    ODA_ASSERT(hotGrips[i].appDataOdRxClass() == OdDbSBAppData::desc());
    if ((((OdDbSBAppData*)hotGrips[i].appData())->m_pParameter->isKindOf(OdDbBlockPropertiesTable::desc())))
    {
      OdStringArray& values = ((OdDbSBAppData*)hotGrips[i].appData())->m_sValues;
      if (values.isEmpty())
      {
        OdDbBlockPropertiesTablePtr pt = ((OdDbSBAppData*)hotGrips[i].appData())->m_pParameter;
        // Here we ignore the fact that the values in the first column may be not unique.
        // Resolving such collisions would require e.g. adding sub-menus with values 
        // from the next columns (which would add no clarity to this example)
        for (int i = 0; i < pt->numberOfRows(); ++i)
        {
          OdString value;
          pt->getCellValueString(0, i, value);
          values.append(value);
        }
      }
    }
    if (!((OdDbSBAppData*)hotGrips[i].appData())->m_sValues.isEmpty())
    {
      s_currentData = (OdDbSBAppData*)hotGrips[i].appData();
      cb = SbContextMenuItemIndexPtr;
#if defined(WINVER) && (WINVER >= 0x0500) && defined(OD_WINDOWS_DESKTOP)
      HMENU hm = ::CreatePopupMenu();
      for (unsigned j = 0; j < s_currentData->m_sValues.size(); ++j)
      {
        MENUITEMINFO mi = { sizeof(MENUITEMINFO), 0 };
        mi.fMask = MIIM_STRING|MIIM_ID;
        mi.wID = j + 1;
        mi.dwTypeData = (LPTSTR)s_currentData->m_sValues[j].c_str();
        mi.cch = s_currentData->m_sValues[j].getLength() + 1;
        ::InsertMenuItem(hm, j, TRUE, &mi);
      }
      menu = (ODHMENU)hm;
#endif
    }
  }
  return eOk;
}

void SbGripOpStatus(OdDbGripData* pThis, OdDbStub*, OdDbGripOperations::GripStatus status)
{
  if (pThis->appDataOdRxClass() == OdDbSBAppData::desc() && (status == OdDbGripOperations::kGripEnd || status == OdDbGripOperations::kGripAbort))
  {
    ((OdDbSBAppData*)pThis->appData())->release();
    pThis->setAppData(0);
  }
}

OdResult SbHotGrip(OdDbGripData* pThis, OdDbStub* entId, int status)
{
  if (status & OdDbGripOperations::kSharedGrip)
    return eOk;
  OdDbSBAppData* appdata = (OdDbSBAppData*)pThis->appData();
  if (appdata->m_pGrip.isNull() || !appdata->m_pGrip->isKindOf(OdDbBlockFlipGrip::desc()))
    return eOk;
  OdDbTransactionWrapper tm(appdata->m_pContext->getGraph()->database());
  OdDbBlockFlipGripPtr fg = appdata->m_pGrip;
  fg->setUpdatedFlipState( fg->updatedFlipState() == OdDbBlockFlipParameter::NotFlipped ? OdDbBlockFlipParameter::Flipped : OdDbBlockFlipParameter::NotFlipped);
  OdResult res = updateGraph(appdata->m_pContext, appdata->m_pGrip->nodeId());
  tm.endTransaction();
  if (eOk == res)
    return eGripOpGetNewGripPoints;
  else
    return res;
}

static bool isActiveGrip(OdDbBlockGrip* p)
{
  if (!p)
    return false;
  OdDbBlockParameterPtr param = p->getAssociatedParameter();
  if (param.isNull())
    return false;
  if (p->isKindOf(OdDbBlockVisibilityGrip::desc()) || p->isKindOf(OdDbBlockLookupGrip::desc()) || p->isKindOf(OdDbBlockPropertiesTableGrip::desc()))
    return true; // always active
  if (!param->isMemberOfCurrentVisibilitySet())
    return false;
  if (param->isKindOf(OdDbBlockConstraintParameter::desc())) // may drive no actions
    return true;
  OdDbEvalEdgeInfoArray outEdges;
  OdDbEvalGraphPtr gr = param->getGraph();
  gr->getOutgoingEdges(param->nodeId(), outEdges);
  bool is2ptParam = param->isKindOf(OdDbBlock2PtParameter::desc()) && !param->isKindOf(OdDbBlockFlipParameter::desc());
  for (unsigned int i = 0; i < outEdges.size(); ++i)
  {
    OdDbEvalExprPtr ex = gr->getNode(outEdges[i].to());
    if (!ex.isNull() && ex->isKindOf(OdDbBlockAction::desc()))
    {
      if (is2ptParam)
      {
        if (OdDbBlock2PtParameterPtr(param)->parameterComponentHasAction(p->getAssociatedParameterComponent(), (const OdDbBlockAction*)ex.get()))
          return true;
      }
      else
        return true;
    }
  }
  return false;
}

bool OdDbDynBlockReferenceGripPointsOverrule::isApplicable(const OdRxObject* pOverruledSubject) const
{
  OdDbBlockReferencePtr br(pOverruledSubject);
  if (br.isNull())
    return false;
  return OdDbDynBlockReference(br).isDynamicBlock();
}


static void getDynamicGrips( OdDbDynBlockReference &dr, OdDbBlockReferencePtr br, OdDbGripDataPtrArray &grips );

OdResult OdDbDynBlockReferenceGripPointsOverrule::getGripPoints( const OdDbEntity* pEntity,
  OdDbGripDataPtrArray& grips, const double curViewUnitSize, const int gripSize,
  const OdGeVector3d& curViewDir, const int bitFlags )
{
  OdDbBlockReferencePtr br(pEntity);
  if (!br.isNull())
  {
    OdDbDynBlockReference dinBr = OdDbDynBlockReference(br);
    getDynamicGrips(dinBr, br, grips);
  }
  return OdDbGripOverrule::getGripPoints(pEntity, grips, curViewUnitSize, gripSize, curViewDir, bitFlags);
}

static bool updateArrays( OdDbEvalGraph* gr, const OdDbEvalNodeIdArray& activated, const OdDbEvalContext* ctx )
{
  OdDbEvalNodeIdArray arrays;
  OdDbEvalNodeIdArray nodes; gr->getAllNodes(nodes);
  for ( unsigned i = 0; i < nodes.size(); i++ )
  {
    if ( !OdDbBlockArrayAction::cast( gr->getNode( nodes[i] ) ).isNull() 
      && !activated.contains( nodes[i] ) )
    {
      arrays.append( nodes[i] );
    }
  }

  if ( !arrays.isEmpty() )
    return gr->evaluate( ctx, &arrays );

  return true;
}
#undef ODA_NON_TRACING

OdResult updateGraph(OdDbBlockRepresentationContext* ctx, OdDbEvalNodeId id)
{
  ctx->createRepresentation();
  OdDbEvalGraphPtr gr = ctx->getGraph();
  OdDbEvalNodeIdArray a; a.append( id );
  OdDbEvalNodeIdArray activated;
  ODA_VERIFY( gr->activate(a, &activated) );
  
  OdRxObjectImpl<OdDbEvalContext> ectx;
  ectx.insertAt( OdDbEvalContextPair(L"DD_ENHANCEDBLOCK_EVALUATE", ctx));
  ectx.insertAt(OdDbEvalContextPair(L"DD_ENHANCEDBLOCK_EVALUATE_START", reinterpret_cast<void*>(static_cast<OdIntPtr>(id))));
  bool res = gr->evaluate(&ectx);
  ODA_ASSERT_ONCE( res );
  if (!res)
    return eInvalidInput;
  res &= updateArrays( gr, activated, &ectx );
  ODA_ASSERT_ONCE( res );
  if (!res)
    return eInvalidInput;
  ctx->updateRepresentation();
  return eOk;
}

OdResult OdDbDynBlockReferenceGripPointsOverrule::moveGripPointsAt(OdDbEntity* pEntity, const OdDbVoidPtrArray& grips,
  const OdGeVector3d& offset, int bitFlags )
{
  if (grips.size() == 1 && grips[0] != 0 && ((OdRxObject*)grips[0])->isA() == OdDbSBAppData::desc())
  {
    OdDbTransactionWrapper tm(pEntity->database());
    OdDbBlockReferencePtr br(pEntity);
    OdSmartPtr<OdDbBlockRepresentationContext> ctx = OdDbBlockRepresentationContext::getRepresentationContext(br);
    OdDbBlockGripPtr g = ((OdDbSBAppData*)grips[0])->m_pGrip;
    OdGeVector3d localOffset(offset);
    localOffset.transformBy(br->blockTransform().invert());
    OdGePoint3d p = g->updatedLocation();
    g->setUpdatedLocation(p + localOffset);
    OdResult res = updateGraph(ctx, g->nodeId());
    if (res != eOk)
    {
      g->setUpdatedLocation(p);
    }
    else
    {
      ctx->addHistoryRecord(L"GRIPLOC", OdResBuf::newRb(11, offset), g->nodeId());
      tm.endTransaction();
    }
    return res;
  }
  else
    return OdDbGripOverrule::moveGripPointsAt(pEntity, grips, offset, bitFlags);
}

static void getDynamicGrips( OdDbDynBlockReference &dr, OdDbBlockReferencePtr br, OdDbGripDataPtrArray &grips )
{
  OdDbBlockTableRecordPtr dbtr = dr.dynamicBlockTableRecord().safeOpenObject();
  OdDbEvalGraphPtr gr = OdDbEvalGraph::getGraph(dbtr, L"ACAD_ENHANCEDBLOCK");
  if (gr.isNull())
    return;
  OdSmartPtr<OdDbBlockRepresentationContext> repContext = OdDbBlockRepresentationContext::getRepresentationContext(br);
  if (repContext.isNull())
  {
    repContext = OdRxObjectImpl<OdDbBlockRepresentationContext>::createObject();
    try
    {
      repContext->init(&dr, br, gr); // block may be invalid
    }
    catch (const OdError&)
    {
      return;
    }
  }
  OdGeMatrix3d bt(br->blockTransform());
  OdDbEvalNodeIdArray nodes; gr->getAllNodes( nodes );
  for ( unsigned i = 0; i < nodes.size(); i++ )
  {
    OdDbBlockGripPtr p = OdDbBlockGrip::cast(repContext->getRepresentationNode(nodes[i]));
    if (!isActiveGrip(p))
      continue;
    OdDbGripDataPtr data(new OdDbGripData());
    OdGePoint3d gripLoc(p->displayLocation());
    gripLoc.transformBy(bt);
    data->setGripPoint(gripLoc);
    data->setViewportDraw(SbGripViewportDraw);
    data->setGripOpStatFunc(SbGripOpStatus);
    data->setSkipWhenShared(true);
    data->disableModeKeywords(true);
    data->setAppDataOdRxClass(OdDbSBAppData::desc());
    OdSmartPtr<OdDbSBAppData> appData(OdRxObjectImpl<OdDbSBAppData>::createObject());
    appData->m_pGrip = p;
    appData->m_pContext = repContext.get();
    if (p->isKindOf(OdDbBlockVisibilityGrip::desc()) || p->isKindOf(OdDbBlockLookupGrip::desc()) || p->isKindOf(OdDbBlockPropertiesTableGrip::desc()))
    {
      data->disableRubberBandLine(true);
      data->setMapGripHotToRtClk(true);
      data->setRtClk(SbGripRtClkHandler);
      OdDbEvalEdgeInfoArray outNodes;
      repContext->getGraph()->getOutgoingEdges(p->nodeId(), outNodes);
      for (unsigned i = 0; i < outNodes.size(); ++i)
      {
        OdDbEvalExprPtr node = repContext->getGraph()->getNode(outNodes[i].to());
        if (node->isKindOf(OdDbBlockParameter::desc()))
        {
          OdDbBlockParameterPtr param(node);
          appData->m_pParameter = repContext->getRepresentationNode(param->nodeId());
          if (!param->isKindOf(OdDbBlockPropertiesTable::desc()))
          {
            OdDbBlkParamPropertyDescriptorArray props;
            param->getPropertyDescription(props);
            for (unsigned j = 0; j < props[0].m_pAllowedValues.size(); ++j)
            {
              appData->m_sValues.append(props[0].m_pAllowedValues[j]->getString());
            }
            if (p->isKindOf(OdDbBlockLookupGrip::desc()))
              appData->m_sPropertyName = L"lookupString";
            else
              appData->m_sPropertyName = L"VisibilityState";
            OdString current = appData->m_pParameter->getPropertyValue(appData->m_sPropertyName)->getString();
            if (!appData->m_sValues.contains(current))
              appData->m_sValues.append(current);
          }
          else
            appData->m_sPropertyName = L"currentRow";
          break;
        }
      }
    }
    else if (p->isKindOf(OdDbBlockFlipGrip::desc()))
    {
      data->setHotGripFunc(SbHotGrip);
    }
    data->setAppData(appData.detach());
    grips.append(data);
  }
}
