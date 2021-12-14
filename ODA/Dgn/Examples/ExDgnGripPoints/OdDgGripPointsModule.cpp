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
/* Defines entry points for the DLL                                     */
/************************************************************************/
//#include "stdafx.h"
#include "OdaCommon.h"
#include "OdDgGripPointsModule.h"
#include "RxDynamicModule.h"
#include "DgElementGripPoints.h"
#include "DgLine.h"
#include "DgArc.h"
#include "DgEllipse.h"
#include "DgMultiline.h"
#include "DgCurve.h"
#include "DgBSplineCurve.h"
#include "DgCone.h"
#include "DgShape.h"
#include "DgLineString.h"
#include "DgText.h"
#include "DgDetailingSymbol.h"
#include "DgDimension.h"
#include "DgDetailingSymbolGripPoints.h"
#include "DgReferenceAttach.h"

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER) //  MacOS X dynamic library loading. //#ifdef _TOOLKIT_IN_DLL_


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

#endif //_TOOLKIT_IN_DLL_ //  MacOS X dynamic library loading.

ODRX_DEFINE_DYNAMIC_MODULE(OdDgGripPointsModule);

OdDgGripPointsModule::OdDgGripPointsModule()
{
}

OdDgGripPointsModule::~OdDgGripPointsModule()
{
}

void OdDgGripPointsModule::initApp()
{
  if ( !OdDgElement::desc() )
  {
    throw OdError(eNotInitializedYet);
  }

  OdDgElement::desc()->module()->addRef();

  OdDgElement::desc()->addX( OdDgGripPointsPE::desc(), &_egp );
  OdDgReferenceAttachmentHeader::desc()->addX(OdDgGripPointsPE::desc(), &_egp);
  OdDgLine2d::desc()->addX( OdDgGripPointsPE::desc(), &_lngp );
  OdDgLine3d::desc()->addX( OdDgGripPointsPE::desc(), &_lngp );
  OdDgArc2d::desc()->addX(OdDgGripPointsPE::desc(), &_a2dgp);
  OdDgArc3d::desc()->addX(OdDgGripPointsPE::desc(), &_a3dgp);
  OdDgEllipse2d::desc()->addX(OdDgGripPointsPE::desc(), &_el2dgp);
  OdDgEllipse3d::desc()->addX(OdDgGripPointsPE::desc(), &_el3dgp);
  OdDgLineString2d::desc()->addX(OdDgGripPointsPE::desc(), &_ls2dgp);
  OdDgLineString3d::desc()->addX(OdDgGripPointsPE::desc(), &_ls3dgp);
  OdDgMultiline::desc()->addX(OdDgGripPointsPE::desc(), &_mlngp);
  OdDgCurve2d::desc()->addX(OdDgGripPointsPE::desc(), &_cv2dgp);
  OdDgCurve3d::desc()->addX(OdDgGripPointsPE::desc(), &_cv3dgp);
  OdDgCone::desc()->addX(OdDgGripPointsPE::desc(), &_conegp);
  OdDgShape2d::desc()->addX(OdDgGripPointsPE::desc(), &_shp2dgp);
  OdDgShape3d::desc()->addX(OdDgGripPointsPE::desc(), &_shp3dgp);
  OdDgBSplineCurve2d::desc()->addX(OdDgGripPointsPE::desc(), &_bscrv2dgp);
  OdDgBSplineCurve3d::desc()->addX(OdDgGripPointsPE::desc(), &_bscrv3dgp);
  OdDgText2d::desc()->addX(OdDgGripPointsPE::desc(), &_text2dgp);
  OdDgText3d::desc()->addX(OdDgGripPointsPE::desc(), &_text3dgp);
  OdDgDimension::desc()->addX(OdDgGripPointsPE::desc(), &_dimengp);
  OdDgDetailMarker2d::desc()->addX( OdDgGripPointsPE::desc(), &_dm2dgp );
  OdDgDetailMarker3d::desc()->addX( OdDgGripPointsPE::desc(), &_dm3dgp );
  OdDgArrowMarker2d::desc()->addX( OdDgGripPointsPE::desc(), &_am2dgp );
  OdDgArrowMarker3d::desc()->addX( OdDgGripPointsPE::desc(), &_am3dgp );
  OdDgSectionMarker2d::desc()->addX( OdDgGripPointsPE::desc(), &_sm2dgp );
  OdDgSectionMarker3d::desc()->addX( OdDgGripPointsPE::desc(), &_sm3dgp );
  OdDgTitleText2d::desc()->addX( OdDgGripPointsPE::desc(), &_tt2dgp );
  OdDgTitleText3d::desc()->addX( OdDgGripPointsPE::desc(), &_tt3dgp );
  
}

void OdDgGripPointsModule::uninitApp()
{
  OdDgTitleText3d::desc()->delX( OdDgGripPointsPE::desc() );
  OdDgTitleText2d::desc()->delX( OdDgGripPointsPE::desc() );
  OdDgSectionMarker3d::desc()->delX( OdDgGripPointsPE::desc() );
  OdDgSectionMarker2d::desc()->delX( OdDgGripPointsPE::desc() );
  OdDgArrowMarker3d::desc()->delX( OdDgGripPointsPE::desc() );
  OdDgArrowMarker2d::desc()->delX( OdDgGripPointsPE::desc() );
  OdDgDetailMarker2d::desc()->delX( OdDgGripPointsPE::desc() );
  OdDgDetailMarker3d::desc()->delX( OdDgGripPointsPE::desc() );
  OdDgLine3d::desc()->delX( OdDgGripPointsPE::desc() );
  OdDgLine2d::desc()->delX( OdDgGripPointsPE::desc() );
  OdDgArc2d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgArc3d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgCurve2d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgCurve3d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgCone::desc()->delX(OdDgGripPointsPE::desc());
  OdDgEllipse2d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgEllipse3d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgLineString2d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgLineString3d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgMultiline::desc()->delX(OdDgGripPointsPE::desc());
  OdDgShape2d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgShape3d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgBSplineCurve2d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgBSplineCurve3d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgText2d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgText3d::desc()->delX(OdDgGripPointsPE::desc());
  OdDgDimension::desc()->delX(OdDgGripPointsPE::desc());
  OdDgReferenceAttachmentHeader::desc()->delX(OdDgGripPointsPE::desc());
  OdDgElement::desc()->delX( OdDgGripPointsPE::desc() );
  OdDgElement::desc()->module()->release();
}
