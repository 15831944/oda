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

#include <OdaCommon.h>
#include "CenterObjectsGrips.h"
#include "DbSmartCenterActionBody.h"
#include "RxDynamicModule.h"
#include "DbBlockReference.h"
#include "DbCenterMarkActionBody.h"
#include "Ge/GePoint3dArray.h"
#include "Ge/GeCircArc3d.h"
#include "Gi/GiViewportDraw.h"

ODRX_CONS_DEFINE_MEMBERS(OdDbCmAppData, OdRxObject, RXIMPL_CONSTR);

inline bool isOverruleApplicable(const OdRxObject* pOverruledSubject, const OdRxClass* pClass)
{
  OdDbBlockReference* pBlkRef = OdDbBlockReference::cast(pOverruledSubject);
  if (!pBlkRef)
    return false;

  OdDbObjectId idActionBody = OdDbSmartCenterActionBody::getSmartCenterActionBody(pBlkRef);

  OdDbObjectPtr pActionBody = idActionBody.openObject();
  return !pActionBody.isNull() && pActionBody->isA() == pClass;
}

bool CenterMarkGripsOverrule::isApplicable(const OdRxObject* pOverruledSubject) const
{
  return isOverruleApplicable(pOverruledSubject, OdDbCenterMarkActionBody::desc());
}

static double getGripSize(OdGiViewportDraw* pWd, const OdGePoint3d& eyePt, int gripSize)
{
  OdGePoint2d ptDim;
  OdGePoint3d wcsPt(eyePt); wcsPt.transformBy(pWd->viewport().getEyeToWorldTransform());
  pWd->viewport().getNumPixelsInUnitSquare(wcsPt, ptDim);
  OdGeVector3d v(gripSize / ptDim.x, 0, 0);
  v.transformBy(pWd->viewport().getWorldToEyeTransform());
  return v.length();
}

static void drawTriangle(OdGiViewportDraw* pWd, const OdGePoint3d& pOrigPoint, int gripSize, const OdGeVector3d& orient, const OdGeVector3d& normal)
{
  OdGeVector3d v(orient);
  OdGePoint3d p = pOrigPoint;
  v.normalize();
  double dGripSize = getGripSize(pWd, p, gripSize);
  v *= dGripSize * 2;
  p.transformBy(pWd->viewport().getEyeToWorldTransform());
  OdGePoint3d pp[3];
  pp[0] = p + v;
  v.rotateBy(OdaPI * 5. / 6., normal);
  pp[1] = p + v;
  v.rotateBy(OdaPI / 3., normal);
  pp[2] = p + v;
  for (int i = 0; i < 3; i++)
    pp[i].transformBy(pWd->viewport().getWorldToEyeTransform());
  pWd->geometry().polygonEye(3, pp);
}

static void drawSquare(OdGiViewportDraw* pWd, const OdGePoint3d& pOrigPoint, int gripSize, const OdGeVector3d& orient, const OdGeVector3d& normal)
{
  OdGeVector3d v(orient);
  OdGePoint3d p = pOrigPoint;
  v.normalize();
  double dGripSize = getGripSize(pWd, p, gripSize);
  v *= dGripSize * 1.41;
  p.transformBy(pWd->viewport().getEyeToWorldTransform());
  OdGePoint3d pp[4];
  v.rotateBy(OdaPI * 0.25, normal);
  for (int i = 0; i < 4; i++)
  {
    pp[i] = p + v;
    pp[i].transformBy(pWd->viewport().getWorldToEyeTransform());
    v.rotateBy(OdaPI2, normal);
  }
  pWd->geometry().polygonEye(4, pp);
}

void CmGripOpStatus(OdDbGripData* pThis, OdDbStub*, OdDbGripOperations::GripStatus status)
{
  if (pThis->appDataOdRxClass() == OdDbCmAppData::desc() && (status == OdDbGripOperations::kGripEnd || status == OdDbGripOperations::kGripAbort))
  {
    ((OdDbCmAppData*)pThis->appData())->release();
    pThis->setAppData(0);
  }
}

static OdDbCmAppData* s_currentData = 0;
static int nMenuItemIndex = 0;
void CmContextMenuItemIndexPtr(unsigned itemIndex)
{
  nMenuItemIndex = 0;
  if (s_currentData)
  {
    nMenuItemIndex = itemIndex;
    if (itemIndex == 1)
      s_currentData->bCenterStretchMode = true;
    else if (itemIndex == 2)
      s_currentData->bCenterStretchMode = false;
  }
  s_currentData = 0;

}

OdResult CmGripHoverFunc(OdDbGripData* pThis, OdDbStub* entId, int iContextFlags)
{
  OdResult res = eOk;
#if !defined(ODA_UNIXOS) && !(defined(ODA_WINDOWS) && !defined(OD_WINDOWS_DESKTOP))

  OdDbCmAppData* appdata = (OdDbCmAppData*)pThis->appData();

  OdString menuName;
  ODHMENU menu = 0;
  ContextMenuItemIndexPtr cb = 0;

  #if defined(WINVER) && (WINVER >= 0x0500) && defined(OD_WINDOWS_DESKTOP)

  /* // && !defined(_WIN32_WCE) :
  Windows CE introduced a new set of functions for creating and
  manipulating top-level menus. And then Pocket PC introduced yet
  another set, with a special resource as well.
  */

  OdStringArray s_Values;
  s_Values.append("Stretch");
  s_Values.append("Change extension length");
  cb = CmContextMenuItemIndexPtr;
  s_currentData = appdata;
  HMENU hm = ::CreatePopupMenu();
  for (unsigned j = 0; j < s_Values.size(); ++j)
  {
    MENUITEMINFO mi = { sizeof(MENUITEMINFO), 0 };
    mi.fMask = MIIM_STRING | MIIM_ID;
    mi.wID = j + 1;
  #ifdef _UNICODE
    mi.dwTypeData = (LPTSTR)s_Values[j].c_str();
    mi.cch = s_Values[j].getLength() + 1;
  #else
    mi.dwTypeData = (const char*)s_Values[j];
    mi.cch = s_Values[j].getLengthA() + 1;
  #endif
    ::InsertMenuItem(hm, j, TRUE, &mi);
  }
  menu = (ODHMENU)hm;
  #endif // WINVER

  OdGePoint3d pCenter = pThis->gripPoint();
  LONG x = pCenter.x;
  LONG y = pCenter.x;

  HWND wnd = ::GetActiveWindow();
  POINT pt;
  GetCursorPos(&pt);
  //::ClientToScreen(wnd, &pt);
  (*cb)(::TrackPopupMenu((HMENU)menu,
    TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_NOANIMATION, pt.x, pt.y, 0, wnd, 0));
  if (nMenuItemIndex)
    res = eGripOpGetNewGripPoints;
  ::DestroyMenu((HMENU)menu);

#endif // ODA_UNIXOS
  return res;
}

static void CmGripViewportDraw(OdDbGripData* pThis, OdGiViewportDraw* pWd, OdDbStub* entId,
  OdDbGripOperations::DrawType type, OdGePoint3d* imageGripPoint, int gripSize)
{
  ODA_ASSERT(pThis->appDataOdRxClass() == OdDbCmAppData::desc());
  OdGePoint3d p = imageGripPoint == 0 ? pThis->gripPoint() : *imageGripPoint;
  p.transformBy(pWd->viewport().getWorldToEyeTransform());
  int nIndex = ((OdDbCmAppData*)pThis->appData())->nGripIndex;

  switch (type)
  {
  case OdDbGripOperations::kWarmGrip:
    pWd->subEntityTraits().setColor(OdCmEntityColor::kACIBlue);
    break;
  case OdDbGripOperations::kHoverGrip:
    pWd->subEntityTraits().setColor(OdCmEntityColor::kACIMagenta);
    break;
  case OdDbGripOperations::kHotGrip:
    pWd->subEntityTraits().setColor(OdCmEntityColor::kACIRed);
    break;
  case OdDbGripOperations::kDragImageGrip:
    pWd->subEntityTraits().setColor(OdCmEntityColor::kACIRed);
    break;
  }
  pWd->subEntityTraits().setFillType(kOdGiFillAlways);
  
  if ((nIndex > 3) && (nIndex < 8))
    drawTriangle(pWd, p, gripSize, ((OdDbCmAppData*)pThis->appData())->vOrient, ((OdDbCmAppData*)pThis->appData())->vNormal);
  else
    drawSquare(pWd, p, gripSize, ((OdDbCmAppData*)pThis->appData())->vOrient, ((OdDbCmAppData*)pThis->appData())->vNormal);
}

OdResult CenterMarkGripsOverrule::getGripPoints(const OdDbEntity* pSubject,
                                                OdDbGripDataPtrArray& gripsData,
                                                const double curViewUnitSize,
                                                const int gripSize,
                                                const OdGeVector3d& curViewDir,
                                                const int bitFlags)
{
//  return eNotImplemented;
  OdDbBlockReference* pBlkRef = OdDbBlockReference::cast(pSubject);
  if (!pBlkRef)
    return eNotApplicable;

  OdDbObjectId idActionBody = OdDbSmartCenterActionBody::getSmartCenterActionBody(pBlkRef);
  OdDbCenterMarkActionBodyPtr pActionBody = OdDbCenterMarkActionBody::cast(idActionBody.openObject());
  if (pActionBody.isNull())
    return eNullObjectId;

  OdGePoint3dArray arrPoints;
  OdResult res = pActionBody->getAllExtensionPoints(arrPoints);
  if (res != eOk)
    return res;

  res = pActionBody->getAllOvershootPoints(arrPoints);
  if (res != eOk)
    return res;

  OdGePoint3d ptCenter;
  res = pActionBody->getCenterPoint(ptCenter);
  if (res != eOk)
    return res;

  int nSize = arrPoints.size();
  int nIndex = 0;
  for (int i = 0; i < nSize; i++)
  {
    OdDbGripDataPtr aGrip(new OdDbGripData());
    aGrip->setViewportDraw(CmGripViewportDraw);
    aGrip->setGizmosEnabled(true);
    aGrip->setGripOpStatFunc(CmGripOpStatus);
    aGrip->setGripPoint(arrPoints[i]);
    aGrip->setAppDataOdRxClass(OdDbCmAppData::desc());
    OdSmartPtr<OdDbCmAppData> originAppData(OdRxObjectImpl<OdDbCmAppData>::createObject());
    originAppData->nGripIndex = nIndex;
    originAppData->vOrient = arrPoints[i] - ptCenter;
    originAppData->vNormal = pBlkRef->normal();
    aGrip->setAppData(originAppData.detach());
    gripsData.append(aGrip);
    nIndex++;
  }

  // center grip with hoverFunc()

  OdDbGripDataPtr aGrip(new OdDbGripData());
  aGrip->setViewportDraw(CmGripViewportDraw);
  aGrip->disableModeKeywords(true);
  aGrip->setGizmosEnabled(true);
  aGrip->setGripOpStatFunc(CmGripOpStatus);
  aGrip->setHoverFunc(CmGripHoverFunc);
  aGrip->setGripPoint(ptCenter);
  aGrip->setAppDataOdRxClass(OdDbCmAppData::desc());
  OdSmartPtr<OdDbCmAppData> originAppData(OdRxObjectImpl<OdDbCmAppData>::createObject());
  originAppData->vOrient = arrPoints[0] - ptCenter; // for center square applicable previous orientation
  originAppData->vNormal = pBlkRef->normal();
  originAppData->nGripIndex = nIndex;
  aGrip->setAppData(originAppData.detach());
  gripsData.append(aGrip);

  return eOk;
}

OdResult CenterMarkGripsOverrule::getGripPoints( const OdDbEntity* pSubject, OdGePoint3dArray& gripPoints )
{
  return eNotImplemented;
/*  OdDbBlockReference* pBlkRef = OdDbBlockReference::cast(pSubject);
  if (!pBlkRef)
    return eNotApplicable;

  OdDbObjectId idActionBody = OdDbSmartCenterActionBody::getSmartCenterActionBody(pBlkRef);
  OdDbCenterMarkActionBodyPtr pActionBody = OdDbCenterMarkActionBody::cast(idActionBody.openObject());
  if (pActionBody.isNull())
    return eNullObjectId;

  OdGePoint3dArray arrPoints;
  OdResult res = pActionBody->getAllExtensionPoints(arrPoints);
  if (res !=eOk)
    return res;

  res = pActionBody->getAllOvershootPoints(arrPoints);
  if (res != eOk)
    return res;

  gripPoints.append(arrPoints);

  OdGePoint3d ptCenter;

  res = pActionBody->getCenterPoint(ptCenter);
  if (res != eOk)
    return res;

  gripPoints.push_back(ptCenter);

  return eOk;*/
}

OdResult CenterMarkGripsOverrule::moveGripPointsAt(OdDbEntity* pSubject,
                                                  const OdDbVoidPtrArray& grips,
                                                  const OdGeVector3d& offset,
                                                  int bitFlags )
{
//  return eNotImplemented;
  OdResult res;
  if (grips.size() == 0)
    return eOk;
  if (grips.size() > 1)
    return pSubject->transformBy(OdGeMatrix3d::translation(offset));

  OdDbBlockReference* pBlkRef = OdDbBlockReference::cast(pSubject);
  if (!pBlkRef)
    return eNotApplicable;

  OdDbObjectId idActionBody = OdDbSmartCenterActionBody::getSmartCenterActionBody(pBlkRef);
  OdDbCenterMarkActionBodyPtr pActionBody = OdDbCenterMarkActionBody::cast(idActionBody.openObject(OdDb::kForWrite));
  if (pActionBody.isNull())
    return eNullObjectId;
  
  if (((OdDbCmAppData*)grips[0])->nGripIndex != -1)
  {
    OdGePoint3dArray arrPoints;
    res = pActionBody->getAllExtensionPoints(arrPoints);
    if (res != eOk)
      return res;

    res = pActionBody->getAllOvershootPoints(arrPoints);
    if (res != eOk)
      return res;

    OdGePoint3d ptCenter;
    res = pActionBody->getCenterPoint(ptCenter);
    if (res != eOk)
      return res;
    arrPoints.push_back(ptCenter);

    int indexToMove = ((OdDbCmAppData*)grips[0])->nGripIndex;

    OdGeVector3d vOldVector, vNewVector;
    vOldVector = arrPoints[indexToMove] - ptCenter;
    vNewVector = vOldVector + offset;
    if (!vOldVector.isZeroLength())
      vOldVector.normalize();
    double dNewLength = vOldVector.dotProduct(vNewVector);
    double dExtension = -1.0;
        
    switch (indexToMove)
    {
      // Extension points
    case 0:
      res = pActionBody->setHorizontalStartExtension(dNewLength);
      break;
    case 1:
      res = pActionBody->setHorizontalEndExtension(dNewLength);
      break;
    case 2:
      res = pActionBody->setVerticalStartExtension(dNewLength);
      break;
    case 3:
      res = pActionBody->setVerticalEndExtension(dNewLength);
      break;
      // Overshoot points
    case 4:
      dExtension = pActionBody->horizontalStartExtension();
      if ((dExtension != -1.0) && ((dNewLength - dExtension) >= 0))
      {
        res = pActionBody->setHorizontalStartOvershoot(dNewLength - dExtension);
      }
      else
        res = pActionBody->setHorizontalStartOvershoot(0.12);
      break;

    case 5:
      dExtension = pActionBody->horizontalEndExtension();
      if ((dExtension != -1.0) && ((dNewLength - dExtension) >= 0))
      {
        res = pActionBody->setHorizontalEndOvershoot(dNewLength - dExtension);
      }
      else
        res = pActionBody->setHorizontalEndOvershoot(0.12);
      break;

    case 6:
      dExtension = pActionBody->verticalStartExtension();
      if ((dExtension != -1.0) && ((dNewLength - dExtension) >= 0))
      {
        res = pActionBody->setVerticalStartOvershoot(dNewLength - dExtension);
      }
      else
        res = pActionBody->setVerticalStartOvershoot(0.12);
      break;

    case 7:
      dExtension = pActionBody->verticalEndExtension();
      if ((dExtension != -1.0) && ((dNewLength - dExtension) >= 0))
      {
        res = pActionBody->setVerticalEndOvershoot(dNewLength - dExtension);
      }
      else
        res = pActionBody->setVerticalEndOvershoot(0.12);
      break;

      // Center point
    case 8:

      if (((OdDbCmAppData*)grips[0])->bCenterStretchMode)   // "Stretch" - move center mark
        res = pActionBody->setCenterPoint(ptCenter + offset);
      
      else  // change all overshoots lengths
      {
        // looking for nearest to grip CenterMark axis 
        OdGeCircArc3d refCircle;
        OdResult res = pActionBody->getRefCircle(refCircle);
        if (res != eOk)
          return res;

        double dRotation = pActionBody->rotation();
        if (res != eOk)
          dRotation = 0.;

        OdGeVector3dArray arrVAxes;
        OdGeVector3d vNormal = refCircle.normal();
        double dRadius = refCircle.radius();

        OdGeVector3d vAxis(-dRadius, 0., 0.); // horizontal start
        vAxis.rotateBy(dRotation, vNormal);
        arrVAxes.append(vAxis);

        vAxis.set(dRadius, 0., 0.); // horizontal end
        vAxis.rotateBy(dRotation, vNormal);
        arrVAxes.append(vAxis);

        vAxis.set(0., -dRadius, 0.); // vertical start
        vAxis.rotateBy(dRotation, vNormal);
        arrVAxes.append(vAxis);

        vAxis.set(0., dRadius, 0.); // vertical end
        vAxis.rotateBy(dRotation, vNormal);
        arrVAxes.append(vAxis);

        int nAxis = 0;
        double dAngel = OdaPI;
        for (int i = 0; i < 4; i++)
        {
          if (offset.angleTo(arrVAxes[i]) < dAngel)
          {
            nAxis = i;
            dAngel = offset.angleTo(arrVAxes[i]);
          }
        }

        // calculating new length
        arrVAxes[nAxis].normalize();
        dNewLength = arrVAxes[nAxis].dotProduct(offset);

        // geting current extension length
        switch (nAxis)
        {
        case 0:
          dExtension = pActionBody->horizontalStartExtension();
          break;
        case 1:
          dExtension = pActionBody->horizontalEndExtension();
          break;
        case 2:
          dExtension = pActionBody->verticalStartExtension();
          break;
        case 3:
          dExtension = pActionBody->verticalEndExtension();
          break;
        }
        
        if (dExtension == -1.0)
          dExtension = dRadius;

        if (dExtension <= dNewLength) // changing overshoot only if offset point farther then extension point from center
          res = pActionBody->setAllOvershoots(dNewLength - dExtension);
      }
      break;
    }
  }
  return res;
}

OdResult CenterMarkGripsOverrule::moveGripPointsAt(OdDbEntity* pSubject, 
                                                  const OdIntArray& indices,
                                                  const OdGeVector3d& offset)
{
  return eNotImplemented;
  /*OdDbBlockReference* pBlkRef = OdDbBlockReference::cast(pSubject);
  if (!pBlkRef)
    return eNotApplicable;

  OdDbObjectId idActionBody = OdDbSmartCenterActionBody::getSmartCenterActionBody(pBlkRef);
  OdDbCenterMarkActionBodyPtr pActionBody = OdDbCenterMarkActionBody::cast(idActionBody.openObject(OdDb::kForWrite));
  if (pActionBody.isNull())
    return eNullObjectId;

  OdGePoint3dArray arrPoints;
  OdResult res = getGripPoints(pSubject, arrPoints);
  if (res !=eOk)
    return res;

  OdGeCircArc3d refCircle;
  res = pActionBody->getRefCircle(refCircle);
  OdGePoint3d ptCenter = refCircle.center();

  int nPoints = indices.size();
  OdGeVector3d vOldVector, vNewVector;
  for (int i = 0; i < nPoints; ++i)
  {
    vOldVector = arrPoints[indices[i]] - ptCenter;
    vNewVector = vOldVector + offset;
    vOldVector.normalize();
    double dNewLength = vOldVector.dotProduct(vNewVector);
    double dExtension = -1.0;
    switch (indices[i])
    {
      // Extension points
    case 0:
      res = pActionBody->setGeometryParam(dNewLength, L"HorizontalStartExtension");
      break;
    case 1:
      res = pActionBody->setGeometryParam(dNewLength, L"HorizontalEndExtension");
      break;
    case 2:
      res = pActionBody->setGeometryParam(dNewLength, L"VerticalStartExtension");
      break;
    case 3:
      res = pActionBody->setGeometryParam(dNewLength, L"VerticalEndExtension");
      break;
      // Overshoot points
    case 4:
      dExtension = pActionBody->horizontalStartExtension();
      if ((dExtension != -1.0) && ((dNewLength - dExtension) >= 0))
      {
        res = pActionBody->setHorizontalStartOvershoot(dNewLength - dExtension);
      }
      else 
        res = pActionBody->setHorizontalStartOvershoot(0.12);
      break;
    
    case 5:
      dExtension = pActionBody->horizontalEndExtension();
      if ((dExtension != -1.0) && ((dNewLength - dExtension) >= 0))
      {
        res = pActionBody->setHorizontalEndOvershoot(dNewLength - dExtension);
      }
      else
        res = pActionBody->setHorizontalEndOvershoot(0.12);
      break;
    
    case 6:
      dExtension = pActionBody->verticalStartExtension();
      if ((dExtension != -1.0) && ((dNewLength - dExtension) >= 0))
      {
        res = pActionBody->setVerticalStartOvershoot(dNewLength - dExtension);
      }
      else
        res = pActionBody->setVerticalStartOvershoot(0.12);
      break;
    
    case 7:
      dExtension = pActionBody->verticalEndExtension();
      if ((dExtension != -1.0) && ((dNewLength - dExtension) >= 0))
      {
        res = pActionBody->setVerticalEndOvershoot(dNewLength - dExtension);
      }
      else
        res = pActionBody->setVerticalEndOvershoot(0.12);
      break;
    }
    if (res != eOk)
      return res;
  }*/
}

///////////////////////////////////////////////////////////////////////////////////////////
void CenterObjectsGripsModule::initApp()
{
  m_pConstraintsModule = odrxDynamicLinker()->loadModule(L"DbConstraints");
  OdRxOverrule::setIsOverruling(true);
  OdRxOverrule::addOverrule(OdDbBlockReference::desc(), &m_CenterMarkGripsOverrule);
}

void CenterObjectsGripsModule::uninitApp()
{
  OdRxOverrule::removeOverrule(OdDbBlockReference::desc(), &m_CenterMarkGripsOverrule);
}

// define TX module entry point
ODRX_DEFINE_DYNAMIC_MODULE(CenterObjectsGripsModule);

// the stuff below is irrelevant to the sample
#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER)

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID )
{
  switch ( dwReason )
  {
  case DLL_PROCESS_ATTACH:
    // remove this if you need per-thread initialization
    DisableThreadLibraryCalls( (HMODULE)hInstance );
    break;
  }
  return TRUE;
}
#endif //_TOOLKIT_IN_DLL_
