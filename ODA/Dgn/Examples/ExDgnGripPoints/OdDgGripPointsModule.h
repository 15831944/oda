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

/****************************************************************************/
/* Main header file for the DLL                                             */
/*                                                                          */
/* Sample implementation of the OdGeGripPointsPE interface.                 */
/*                                                                          */
/* ODA Platform attempts to use this interface for grip points operations;  */
/* OdDgElement::getGripPoints, etc.                                         */
/*                                                                          */
/****************************************************************************/
#if !defined(DGGRIPPOINTSMODULE__INCLUDED_)
#define DGGRIPPOINTSMODULE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RxModule.h"
#include "StaticRxObject.h"
#include "DgElementGripPoints.h"
#include "DgLineGripPoints.h"
#include "DgArcGripPoints.h"
#include "DgMultilineGripPoints.h"
#include "DgEllipseGripPoints.h"
#include "DgLineStringGripPoints.h"
#include "DgShapeGripPoints.h"
#include "DgBSplineCurveGripPoints.h"
#include "DgCurveGripPoints.h"
#include "DgConeGripPoints.h"
#include "DgTextGripPoints.h"
#include "DgDimensionGripPoints.h"
#include "DgDetailingSymbolGripPoints.h"

/************************************************************************/
/* OdGripPointsModule                                                   */
/* See GripPoints.cpp for the implementation of this class              */
/************************************************************************/
class OdDgGripPointsModule : public OdRxModule
{
  OdStaticRxObject<OdDgElementGripPointsPE>       _egp;
  OdStaticRxObject<OdDgLineGripPointsPE>          _lngp;
  OdStaticRxObject<OdDgDetailMarker2dPointsPE>    _dm2dgp;
  OdStaticRxObject<OdDgDetailMarker3dPointsPE>    _dm3dgp;
  OdStaticRxObject<OdDgArrowMarker2dPointsPE>     _am2dgp;
  OdStaticRxObject<OdDgArrowMarker3dPointsPE>     _am3dgp;
  OdStaticRxObject<OdDgSectionMarker2dPointsPE>   _sm2dgp;
  OdStaticRxObject<OdDgSectionMarker3dPointsPE>   _sm3dgp;
  OdStaticRxObject<OdDgTitleText2dPointsPE>       _tt2dgp;
  OdStaticRxObject<OdDgTitleText3dPointsPE>       _tt3dgp;
  OdStaticRxObject<OdDgArcGripPointsPE>           _a2dgp;
  OdStaticRxObject<OdDgArcGripPointsPE>           _a3dgp;
  OdStaticRxObject<OdDgEllipseGripPointsPE>       _el2dgp;
  OdStaticRxObject<OdDgEllipseGripPointsPE>       _el3dgp;
  OdStaticRxObject<OdDgMultilineGripPointsPE>     _mlngp;
  OdStaticRxObject<OdDgLineStringGripPointsPE>    _ls2dgp;
  OdStaticRxObject<OdDgLineStringGripPointsPE>    _ls3dgp;
  OdStaticRxObject<OdDgCurveGripPointsPE>         _cv2dgp;
  OdStaticRxObject<OdDgCurveGripPointsPE>         _cv3dgp;
  OdStaticRxObject<OdDgConeGripPointsPE>          _conegp;
  OdStaticRxObject<OdDgShapeGripPointsPE>         _shp2dgp;
  OdStaticRxObject<OdDgShapeGripPointsPE>         _shp3dgp;
  OdStaticRxObject<OdDgBSplineCurveGripPointsPE>  _bscrv2dgp;
  OdStaticRxObject<OdDgBSplineCurveGripPointsPE>  _bscrv3dgp;
  OdStaticRxObject<OdDgTextGripPointsPE>          _text2dgp;
  OdStaticRxObject<OdDgTextGripPointsPE>          _text3dgp;
  OdStaticRxObject<OdDgDimensionGripPointsPE>     _dimengp;
  
protected:
	OdDgGripPointsModule();
  void initApp();
  void uninitApp();
public:
	~OdDgGripPointsModule();
};

typedef OdSmartPtr< OdDgGripPointsModule > OdDgGripPointsModulePtr;


#endif // !defined(DGGRIPPOINTSMODULE__INCLUDED_)
