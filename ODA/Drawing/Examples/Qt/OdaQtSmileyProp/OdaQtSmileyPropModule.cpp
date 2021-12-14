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
//
// OdeSmileyPropModule.cpp
//

#include "OdaCommon.h"
#include "RxDynamicModule.h"
#include "DbEntity.h"
#include "Ge/GeCircArc3d.h"
#include "SmileyDb.h"
#include "PropServices.h"

class OdDbXmlPropSmiley : public OdPropBase
{
public:
  virtual bool getPropertyValues(OdPropContext& ctx)
  {
    OdDbEntityPtr pEnt = object();
    AsdkSmiley* pSmiley = static_cast<AsdkSmiley*>(pEnt.get());

    OdPropContext ctxSmiley = ctx.addCategory(L"Smiley");
    
    OdCmColor clr; clr.setColor(pSmiley->backColor().color());
    OdString sClr = propServ()->colorBaseToStr(baseDatabase(), clr);
    ctxSmiley.addProperty(L"back color", sClr, tOdColor).comment(L"Specifies smiley back color");
    ctxSmiley.addProperty(L"radius", pSmiley->radius(), tDistance);
    ctxSmiley.addProperty(L"eye size", pSmiley->eyeSize(), tDistance);
    OdGePoint3d left = pSmiley->mouthLeft(), bottom = pSmiley->mouthBottom(),
                right = pSmiley->mouthRight();
    if (left.x != right.x)
    {
      double dState = fabs(left.y - bottom.y) / fabs(right.x - left.x);
      ctxSmiley.addProperty(L"mouth state", OdString(dState > 0.1 ? L"open" : L"close"), tCombo);
    }

    return true;
  }

  virtual bool setPropertyValue(const OdString& sPath, const OdString& sValue, Action& action)
  {
    OdString sName = sPath, sCat = extractFirst(sName);
    if (sCat == L"Smiley")
    {
      OdDbEntityPtr pEnt = object(true);
      AsdkSmiley* pSmiley = static_cast<AsdkSmiley*>(pEnt.get());
      double dRadius = pSmiley->mouthRadius();
      if (sName == L"back color")
      {
        OdCmColor clrPrev, clr; clrPrev.setColor(pSmiley->backColor().color());
        if (propServ()->colorBaseByStr(baseDatabase(), sValue, clr) && clr != clrPrev)
          pSmiley->setBackColor(clr.entityColor());
      }
      else if (sName == L"radius")
      {
        double dRadius = toDistance(sValue);
        if (!OdEqual(dRadius, pSmiley->radius()))
          pSmiley->setRadius(dRadius);
      }
      else if (sName == L"eye size")
      {
        double dSize = toDistance(sValue);
        if (!OdEqual(dSize, pSmiley->eyeSize()))
          pSmiley->setEyeSize(dSize);
      }
      else if (sName == L"mouth state" && !OdZero(dRadius))
      {
        OdGePoint3d left = pSmiley->mouthLeft(), bottom = pSmiley->mouthBottom(), 
                    right = pSmiley->mouthRight();
        if (left.x != right.x)
        {
          double dState = fabs(left.y - bottom.y) / fabs(right.x - left.x);
          bool statePrev = dState > 0.1, state = (sValue == L"open");
          if (state != statePrev)
          {
            bottom.y = (left.y = right.y) - fabs(right.x - left.x) * (state ? 0.3 : 0.05);
            pSmiley->setMouth(left, bottom, right);
          }
        }
      }
      else
        return false;
      return true;
    }
    return false;
  }

  virtual bool getPropertyPossibleValues(const OdString& sPath,
                                         OdStringArray& values, OdStringArray&) // iconPaths
  {
    OdString sName = sPath, sCat = extractFirst(sName);
    if (sCat == L"Smiley" && sName == L"mouth state")
    {
      values.push_back(L"open"); values.push_back(L"close");
      return true;
    }
    return false;
  }

  virtual bool overrideClassName(OdString& sClassName) const
  {
    sClassName = L"Smiley Custom Entity";
    return true;
  }
};

class OdeSmileyPropModule : public OdRxModule
{
  OdRxModulePtr m_pSmileyModule;
public:
  void initApp();
  void uninitApp();
};

// Macro defines the entry point for this library.
ODRX_DEFINE_DYNAMIC_MODULE(OdeSmileyPropModule);

#if defined(ODA_PROP_SERVICES_UNLINKED_RXINIT) && defined(_TOOLKIT_IN_DLL_)
ODRX_NO_CONS_DEFINE_MEMBERS(OdPropServices, OdRxObject);
#endif
static bool s_bUnlinkedWithPropServices = false;

static OdPropServices* propServices()
{
  OdPropServicesPtr pPropServices = ::odrxSysRegistry()->getAt(OD_PROP_SERVICES);
  ODA_ASSERT_ONCE(!pPropServices.isNull()); // is loaded out of ODE
  return pPropServices.get();
}

void OdeSmileyPropModule::initApp()
{
  m_pSmileyModule = ::odrxDynamicLinker()->loadModule(OdAsdkSmileyDbModuleName);

  s_bUnlinkedWithPropServices = (!OdPropServices::g_pDesc);
  if (s_bUnlinkedWithPropServices)
  {
    OdRxDictionary* pClassDict = ::odrxClassDictionary().get();
    OdRxClass* pOdPropServicesClass = OdRxClass::cast(pClassDict->getAt(OD_PROP_SERVICES)).get();
    ODA_ASSERT_ONCE(pOdPropServicesClass);
    OdPropServices::g_pDesc = pOdPropServicesClass;
  }

  if (OdPropServices* pPropSrv = propServices())
    pPropSrv->registerXmlProp(L"AsdkSmiley", // AsdkSmiley::desc()->name() - is inaccessible as undeclared as external
                              &OdRxObjectImpl<OdDbXmlPropSmiley, OdPropBase>::createObject);
}

void OdeSmileyPropModule::uninitApp()
{ 
  if (OdPropServices* pPropSrv = propServices())
    pPropSrv->unregisterXmlProp(L"AsdkSmiley"); // (AsdkSmiley::desc()->name());

  if (s_bUnlinkedWithPropServices)
    OdPropServices::g_pDesc = NULL;

  m_pSmileyModule = NULL;
}

