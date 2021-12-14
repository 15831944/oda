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

/************************************************************************/
/* ExCustEntity.cpp: implementation of the ExCustEntity class           */
/*                                                                      */
/* ExCustEntity is derived from OdDbCircle, but includes an m_nCount    */
/* member which determines the display characteristics of the           */ 
/* ExCustEntity.                                                        */
/*                                                                      */
/* m_nCount  Display                                                    */
/*  <= 1     A circle, whose center is the center of the ExCustEntity,  */
/*           is solid hatched and displayed.                            */
/*  >1       A polar array of m_nCount circles, whose center is the     */
/*           of the ExCustEntity, is solid hatched and displayed        */
/*                                                                      */
/* In addition, any block inserts with attributes with tags of          */
/* "RADIUS" and "COUNT" will override the stored radius and             */
/* and m_nCount of the ExCustEntity.                                    */
/*                                                                      */                                
/************************************************************************/
#include "OdaCommon.h"
#include "ExCustEntity.h"
#include "DbFiler.h"
#include "Gi/GiWorldDraw.h"
#include "DbProxyEntity.h"
#include "DbHatch.h"
#include "Ge/GeCircArc2d.h"
#include "DbCommandContext.h"
#include "DbDatabase.h"
#include "DbBlockTableRecord.h"
#include "DbUserIO.h"
#include "Gi/GiPathNode.h"
#include "DbBlockReference.h"
#include "DbAttribute.h"
#include "OdDToStr.h"
#include "StaticRxObject.h"
#include "Gs/Gs.h"
#include "RxProperty.h"
#include "RxAttribute.h"
#include "RxCategory.h"
#define STL_USING_LIMITS
#include "OdaSTL.h"

// Usually ODRX_DEFINE_MEMBERS_EX is enough, but to declare custom properties 
// we use the most complicated class registration form

ODRX_DEFINE_INIT_MEMBERS(ExCustEntity,                                                  // ClassName
                        OdDbCircle,                                                     // ParentClass
                        ExCustEntity::pseudoConstructor,                                // DOCREATE
                        OdDb::vAC15,                                                    // DwgVer
                        OdDb::kMRelease0,                                               // MaintVer
                        OdDbProxyEntity::kAllButCloningAllowed,                         // NProxyFlags
                        L"ExCustEntity",                                                // DWG class name
                        L"EXCUSTENTITY",                                                // DxfName
                        L"ExCustObjs|Description: Teigha Run-time Extension Example",   // AppName
                        OdRx::kMTLoading | OdRx::kMTRender | OdRx::kMTRenderInBlock,    // Custom flags
                        ExCustEntity::propertyConstructor,0                             // Property constructor
);
ODRX_DEFINE_RTTI_MEMBERS(ExCustEntity, OdDbCircle)
ODRX_DEFINE_PSEUDOCONSTRUCTOR(ExCustEntity, DBOBJECT_CONSTR)

const OdInt16 ExCustEntity::lastKnownVersion = 1;

ExCustEntity::ExCustEntity()
  : m_nCount(4)
{
  setRadius(1.);
}

ExCustEntity::~ExCustEntity()
{
}

OdInt16 ExCustEntity::getVersion()
{
  return lastKnownVersion;
}
/************************************************************************/
/* Properties unique to ExCustEntity                                    */
/************************************************************************/
void ExCustEntity::setCount(OdInt16 n)
{
  assertWriteEnabled();
  m_nCount = n;
}

OdInt16 ExCustEntity::getCount() const
{
  assertReadEnabled();
  return m_nCount;
}


/************************************************************************/
/* Read the DWG format data of this object from the specified file.     */
/************************************************************************/
OdResult ExCustEntity::dwgInFields(OdDbDwgFiler* pFiler)
{
  /**********************************************************************/
  /* Read the fields common to OdDbCircle                               */
  /**********************************************************************/
  OdResult res = OdDbCircle::dwgInFields(pFiler);
  if (res != eOk)
  {
    return res;
  }
  /**********************************************************************/
  /* Read the version of the ExCustEntity, and indicate create if a     */
  /* proxy is required.                                                 */
  /**********************************************************************/
  int nVersion = pFiler->rdInt16();
  if (nVersion > lastKnownVersion)
  {
    return eMakeMeProxy;
  }

  /**********************************************************************/
  /* Read and set the remaining properties unique to ExCustEntity.      */
  /**********************************************************************/
  m_nCount = pFiler->rdInt16();
  return eOk;
}

/************************************************************************/
/* Write the DWG format data of this object to the specified file.      */
/************************************************************************/
void ExCustEntity::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  /**********************************************************************/
  /* Write the fields common to OdDbCircle                              */
  /**********************************************************************/
  OdDbCircle::dwgOutFields(pFiler);
  /**********************************************************************/
  /* Write the properties common to ExCustEntity.                       */
  /**********************************************************************/
  pFiler->wrInt16(getVersion());
  pFiler->wrInt16(m_nCount);
}

OdResult ExCustEntity::dxfInFields(OdDbDxfFiler* pFiler)
{
  /**********************************************************************/
  /* Read the fields common to OdDbCircle                               */
  /**********************************************************************/
  OdResult res = OdDbCircle::dxfInFields(pFiler);
  if (res != eOk)
    return res;
  if (!pFiler->atSubclassData(desc()->name()))
    return eBadDxfSequence;

  /**********************************************************************/
  /* Read and Set the remaining properties unique to ExCustEntity.      */
  /**********************************************************************/
  while(!pFiler->atEOF())
  {
    int nCode = pFiler->nextItem();
    switch (nCode)
    {
    case 70:
      {
      int nVersion = pFiler->rdInt16();
      if (nVersion > lastKnownVersion)
      {
        return eMakeMeProxy;
      }
      }
      break;
    case 71:
      m_nCount = pFiler->rdInt16();
      break;
    }
  }
  return eOk;
}

/************************************************************************/
/* Write the DXF format data of this object to the specified file.      */
/************************************************************************/
void ExCustEntity::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  /**********************************************************************/
  /* Write the fields common to OdDbCircle  .                           */
  /**********************************************************************/
  OdDbCircle::dxfOutFields(pFiler);
  /**********************************************************************/
  /* Write the properties unique to ExCustEntity.                       */
  /**********************************************************************/
  pFiler->wrSubclassMarker(desc()->name());
  pFiler->wrInt16(70, getVersion());
  pFiler->wrInt16(71, m_nCount);
}

/************************************************************************/
/* Creates a viewport-independent geometric representation of the       */
/* ExCustEntity.                                                        */
/************************************************************************/
bool ExCustEntity::subWorldDraw(OdGiWorldDraw * pWd) const
{
  assertReadEnabled();
  OdGePoint3d ptCenter(center());
  OdGeVector3d vNormal(normal());
  double dRadius = radius();
  int nCount = m_nCount;

  /**********************************************************************/
  /* Override dRadius and nCount from block reference(s) with           */
  /* attributes with tags of "RADIUS" and "COUNT" respectively.         */
  /**********************************************************************/  
  const OdGiPathNode* pPath = pWd->geometry().currentGiPath();
  if(pPath)
  {
    while(pPath)
    {
      /******************************************************************/
      /* Is this a block reference?                                     */
      /******************************************************************/
      OdDbBlockReferencePtr pInsert = OdDbBlockReference::cast(OdDbObjectId(pPath->persistentDrawableId()).openObject());
      if(pInsert.get())
      {
        OdDbObjectIteratorPtr pIter = pInsert->attributeIterator();
        /****************************************************************/
        /* Iterate through the associated attributes                    */
        /****************************************************************/
        while(!pIter->done())
        {
          OdDbAttributePtr pAttrib = pIter->objectId().safeOpenObject();
          if(pAttrib->tag()==OD_T("RADIUS"))
          {
            dRadius = odStrToD(pAttrib->textString());
          }
          else if(pAttrib->tag()==OD_T("COUNT"))
          {
            nCount = odStrToInt(pAttrib->textString());
          }
          pIter->step();
        }
        break;
      }
      pPath = pPath->parent();
    }
  }

  /**********************************************************************/
  /* Create a hatch entity                                              */
  /**********************************************************************/
  OdDbHatchPtr pHatch = OdDbHatch::createObject();
  pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("SOLID"));
  pHatch->setHatchStyle(OdDbHatch::kNormal);

  /**********************************************************************/
  /* Inherit the properties of this ExCustEntity                        */
  /**********************************************************************/
  pHatch->setPropertiesFrom(this);
  pHatch->setAssociative(false);
  pHatch->setNormal(vNormal);
  pHatch->setElevation(ptCenter.z);

  EdgeArray edgePtrs;

  if (nCount <= 1)
  {
    /********************************************************************/
    /* Vectorize the circle                                             */
    /********************************************************************/  
    pWd->geometry().circle(ptCenter, dRadius, vNormal);
    OdGeCircArc2d *cirArc = new OdGeCircArc2d(OdGePoint2d(ptCenter.x, ptCenter.y), dRadius);
    /********************************************************************/
    /* Use the circle as a hatch boundary                               */
    /********************************************************************/  
    edgePtrs.append(cirArc);
    pHatch->appendLoop(OdDbHatch::kDefault, edgePtrs);
  }
  else
  {
    OdGeVector3d vDisp(radius(), 0., 0.);
    double step = Oda2PI / nCount;
    while (nCount--)
    {
      OdGePoint3d ptC(ptCenter + vDisp);
      /******************************************************************/
      /* Vectorize a circle                                             */
      /******************************************************************/  
      pWd->geometry().circle(ptC, dRadius, vNormal);
      vDisp.rotateBy(step, vNormal);
      /******************************************************************/
      /* Use the circle as a hatch boundary                             */
      /******************************************************************/  
      OdGeCircArc2d *cirArc = new OdGeCircArc2d(OdGePoint2d(ptC.x, ptC.y), dRadius);
      edgePtrs.resize(0);
      edgePtrs.append(cirArc);
      pHatch->appendLoop(OdDbHatch::kDefault, edgePtrs);
    }
  }
  pHatch->worldDraw(pWd);
  return true;
}
/************************************************************************/
/* Create ExCustEntity command                                          */
/************************************************************************/
void _ExCreateCustomEntity_func(OdEdCommandContext* pCmdCtx)
{
  /**********************************************************************/
  /* Establish command context, database, and UserIO                    */
  /**********************************************************************/
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();

  /**********************************************************************/
  /* Open Model Space for writing                                       */
  /**********************************************************************/
  OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Create the ExCustEntity                                            */
  /**********************************************************************/
  ExCustEntityPtr pMyEnt = ExCustEntity::createObject();

  /**********************************************************************/
  /* Set the default properties common all entities                     */
  /**********************************************************************/
  pMyEnt->setDatabaseDefaults(pDb);

  /**********************************************************************/
  /* Set the center of the ExCustEntity                                 */
  /**********************************************************************/
  pMyEnt->setCenter( pIO->getPoint(OD_T("Enter circle center:"), true ) );
  
  /**********************************************************************/
  /* Define a RadiusTracker to drag the radius                          */
  /**********************************************************************/
  struct RadiusTracker : OdStaticRxObject<OdEdRealTracker>
  {
    OdDbCirclePtr _ent;
    RadiusTracker(const OdGePoint3d& center,OdDbDatabase* pDb)
    {
      /******************************************************************/
      /* Use a circle to drag the radius.                               */
      /******************************************************************/
      _ent = OdDbCircle::createObject();
      _ent->setDatabaseDefaults(pDb);
      _ent->setCenter(center);
    }

    /********************************************************************/
    /* Set the radius of the circle to the tracked real value           */
    /********************************************************************/
    virtual void setValue(double r){_ent->setRadius(r);}

    /********************************************************************/
    /* Add the dragged circle to the view                               */
    /********************************************************************/
    virtual int addDrawables(OdGsView* pView)
    {
      pView->add(_ent, 0);
      return 1;
    }

    /********************************************************************/
    /* Remove the dragged circle from the view                          */
    /********************************************************************/
    virtual void removeDrawables(OdGsView* pView)
    {
      pView->erase(_ent);
    }
  }
  /**********************************************************************/
  /* Initialize the tracker                                             */
  /**********************************************************************/
  tracker(pMyEnt->center(),pDb);
  
  /**********************************************************************/
  /* Get the radius                                                     */
  /**********************************************************************/
  pMyEnt->setRadius( pIO->getDist(OD_T("\nSpecify radius of circle: "), OdEd::kGdsFromLastPoint, 0.0, OdString::kEmpty, &tracker ) );

  /**********************************************************************/
  /* Get the count                                                     */
  /**********************************************************************/
  pMyEnt->setCount( (OdInt16)pIO->getInt(OD_T("\nSpecify number of circle circles: ")) );

  /**********************************************************************/
  /* Add the ExCustEntity to Model Space                                */
  /**********************************************************************/
  pMS->appendOdDbEntity(pMyEnt);
}

#if defined(_MSC_VER) && defined(_TOOLKIT_IN_DLL_)
class __declspec(uuid("8D0E4AE8-D8B1-4C26-92E9-264D1C9D5C79")) CExCustEntityWrapper;
#endif

OdResult ExCustEntity::subGetClassID(void* pClsid) const
{
#if defined(_MSC_VER) && defined(_TOOLKIT_IN_DLL_)
  *(GUID*)pClsid = __uuidof(CExCustEntityWrapper);
  return eOk;
#else
  return eNotImplemented;
#endif
}

struct ExCustEntityCountProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<ExCustEntityCountProperty>::createObject();
    ((ExCustEntityCountProperty*)res.get())->init(L"Count", &OdRxValueType::Desc<int>::value(), owner);
    ((ExCustEntityCountProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(
      L"Polygon side count."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((ExCustEntityCountProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE
  {
    ExCustEntityPtr e = ExCustEntity::cast(pO);
    if (!e.isNull())
    {
      value = e->getCount();
      return eOk;
    }
    else
      return eNotApplicable;
  }
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE
  {
    if (const int* count = rxvalue_cast<int>(&value))
    {
      ExCustEntityPtr e = ExCustEntity::cast(pO);
      if (!e.isNull() && *count < std::numeric_limits<OdInt16>::max())
      {
        e->setCount((OdInt16)*count);
        return eOk;
      }
    }
    return eNotApplicable;
  }
};

void ExCustEntity::propertyConstructor(OdRxMemberCollectionBuilder& cb, void*)
{
  cb.add(ExCustEntityCountProperty::createObject(cb.owner()));
}
