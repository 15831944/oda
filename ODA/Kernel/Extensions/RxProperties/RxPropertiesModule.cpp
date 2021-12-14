///////////////////////////////////////////////////////////////////////////////
// THIS FILE IS MACHINE GENERATED!!!!                                        //
///////////////////////////////////////////////////////////////////////////////

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
#include "RxProperty.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "RxProperties.h"
#include "Gi/GiViewportTraits.h"
#include "RxObject.h"

FIRSTDLL_EXPORT void odrxSetMemberConstructor(OdRxClass* pClass, OdRxMemberCollectionConstructorPtr pc, void* data = 0);

class OdRxPropertiesModule : public OdRxModule
{
  static OdArray<OdRxMemberPtr> properties;
  static void constructOdRxObjectProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdRxObjectProperties(properties, b, data);
  };
  static void constructOdGiPhotographicExposureParametersProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdGiPhotographicExposureParametersProperties(properties, b, data);
  };
  static void constructOdGiToneOperatorParametersProperties(OdRxMemberCollectionBuilder& b, void* data)
  {
    createOdGiToneOperatorParametersProperties(properties, b, data);
  };
public:
  void initApp()
  {
    ::odrxSetMemberConstructor(OdRxObject::desc(), constructOdRxObjectProperties);
    ::odrxSetMemberConstructor(OdGiPhotographicExposureParameters::desc(), constructOdGiPhotographicExposureParametersProperties);
    ::odrxSetMemberConstructor(OdGiToneOperatorParameters::desc(), constructOdGiToneOperatorParametersProperties);
  }
  void uninitApp()
  {
    for (unsigned i = 0; i < properties.size(); ++i)
      OdRxMember::deleteMember(properties[i]);
    properties.clear();
  }
};
OdArray<OdRxMemberPtr> OdRxPropertiesModule::properties;

ODRX_DEFINE_DYNAMIC_MODULE(OdRxPropertiesModule);
DISABLE_THREAD_LIBRARY_CALLS()

