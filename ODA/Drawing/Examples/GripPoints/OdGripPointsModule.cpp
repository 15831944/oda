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
#include "OdGripPointsModule.h"
#include "RxDynamicModule.h"
#include "DbLine.h"
#include "DbMline.h"
#include "DbArc.h"
#include "DbPolyline.h"
#include "DbEntityGripPoints.h"
#include "DbRotatedDimension.h"
#include "DbAlignedDimension.h"
#include "DbRadialDimension.h"
#include "DbDiametricDimension.h"
#include "Db3PointAngularDimension.h"
#include "DbOrdinateDimension.h"
#include "Db2LineAngularDimension.h"
#include "DbRadialDimensionLarge.h"
#include "DbArcDimension.h"
#include "DbCircle.h"
#include "DbBlockReference.h"
#include "DbMLeader.h"
#include "DbPolygonMesh.h"
#include "DbUnderlayReference.h"
#include "DbViewport.h"
#include "DbRasterImage.h"
#include "DbTrace.h"
#include "DbSolid.h"
#include "Db3dPolyline.h"
#include "DbCamera.h"
#include "AbstractViewPE.h"
#include "DbGeoPositionMarker.h"
#include "DbWipeout.h"
#include "DbFace.h"

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

ODRX_DEFINE_DYNAMIC_MODULE(OdGripPointsModule);

OdGripPointsModule::OdGripPointsModule()
{
}

OdGripPointsModule::~OdGripPointsModule()
{
}

void OdGripPointsModule::initApp()
{
  if ( !OdDbEntity::desc() )
  {
    throw OdError(eNotInitializedYet);
  }
  ::odrxDynamicLinker()->loadModule(OdDbModuleName, false)->addRef();

  OdDbEntity::desc()->addX( OdDbGripPointsPE::desc(), &_egp );
  OdDbLine::desc()->addX( OdDbGripPointsPE::desc(), &_lgp );
  OdDbMline::desc()->addX( OdDbGripPointsPE::desc(), &_mlgp );
  OdDbArc::desc()->addX( OdDbGripPointsPE::desc(), &_agp );
  OdDbPolyline::desc()->addX( OdDbGripPointsPE::desc(), &_plgp );
  OdDbRotatedDimension::desc()->addX( OdDbGripPointsPE::desc(), &_rdgp );
  OdDbAlignedDimension::desc()->addX( OdDbGripPointsPE::desc(), &_adgp );
  OdDbRadialDimension::desc()->addX( OdDbGripPointsPE::desc(), &_rcdgp );
  OdDbDiametricDimension::desc()->addX( OdDbGripPointsPE::desc(), &_dcdgp );
  OdDb3PointAngularDimension::desc()->addX( OdDbGripPointsPE::desc(), &_3padgp );
  OdDbOrdinateDimension::desc()->addX( OdDbGripPointsPE::desc(), &_odgp );
  OdDb2LineAngularDimension::desc()->addX( OdDbGripPointsPE::desc(), &_2ladgp );
  OdDbArcDimension::desc()->addX( OdDbGripPointsPE::desc(), &_arcdgp );
  OdDbRadialDimensionLarge::desc()->addX( OdDbGripPointsPE::desc(), &_rdlgp );
  OdDbBlockReference::desc()->addX( OdDbGripPointsPE::desc(), &_insertgp );
  OdDbMLeader::desc()->addX( OdDbGripPointsPE::desc(), &_mleadgp );
  OdDbPolygonMesh::desc()->addX( OdDbPolygonMeshGripPointsPE::desc(), &_mmeshdgp );
  OdDbPdfReference::desc()->addX( OdDbPdfUnderlayGripPointsPE::desc(), &_pdfunderlaygp );
  OdDbViewport::desc()->addX( OdDbViewportGripPointsPE::desc(), &_vptgp );
  OdDb2dPolyline::desc()->addX( OdDbGripPointsPE::desc(), &_2dplgp );
  OdDbRasterImage::desc()->addX( OdDbRasterImageGripPointsPE::desc(), &_rimggp );
  OdDbTrace::desc()->addX( OdDbTraceGripPointsPE::desc(), &_trcgp );
  OdDbSolid::desc()->addX( OdDbSolidGripPointsPE::desc(), &_sldgp );
  OdDb3dPolyline::desc()->addX( OdDbGripPointsPE::desc(), &_3dplgp );
  OdDbCamera::desc()->addX( OdDbGripPointsPE::desc(), &_camgp );
  OdDbCircle::desc()->addX( OdDbGripPointsPE::desc(), &_cgp );
  OdDbEllipse::desc()->addX(OdDbGripPointsPE::desc(), &_elgp);
  OdDbText::desc()->addX( OdDbGripPointsPE::desc(), &_txtgp );
  OdDbGeoPositionMarker::desc()->addX( OdDbGeoPositionMarkerPE::desc(), &_geoposmarkgp );
  OdDbUnderlayReference::desc()->addX( OdDbDgnUnderlayGripPointsPE::desc(), &_dgnunderlaygp );
  OdDbOle2Frame::desc()->addX( OdDbOleGripPointsPE::desc(), &_olegp );
  OdDbWipeout::desc()->addX( OdDbWipeOutGripPointsPE::desc(), &_wipeoutgp );
  OdDbFace::desc()->addX(OdDbGripPointsPE::desc(), &_fgp);
  OdDbBlockGripAppData::rxInit();
}

void OdGripPointsModule::uninitApp()
{
  OdDbBlockGripAppData::rxUninit();
  OdDbFace::desc()->delX(OdDbFaceGripPointsPE::desc());
  OdDbWipeout::desc()->delX( OdDbWipeOutGripPointsPE::desc() );
  OdDbOle2Frame::desc()->delX( OdDbOleGripPointsPE::desc() );
  OdDbGeoPositionMarker::desc()->delX( OdDbGeoPositionMarkerPE::desc() );
  OdDbGeoPositionMarker::desc()->delX( OdDbGeoPositionMarkerPE::desc() );
  OdDbText::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbEllipse::desc()->delX(OdDbGripPointsPE::desc());
  OdDbCircle::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbCamera::desc()->delX( OdDbGripPointsPE::desc() );
  OdDb3dPolyline::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbSolid::desc()->delX( OdDbSolidGripPointsPE::desc() );
  OdDbTrace::desc()->delX( OdDbTraceGripPointsPE::desc() );
  OdDbViewport::desc()->delX( OdDbViewportGripPointsPE::desc() );
  OdDbPdfReference::desc()->delX( OdDbPdfUnderlayGripPointsPE::desc() );
  OdDbPolyline::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbLine::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbMline::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbArc::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbEntity::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbRotatedDimension::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbAlignedDimension::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbRadialDimension::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbDiametricDimension::desc()->delX( OdDbGripPointsPE::desc() );
  OdDb3PointAngularDimension::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbOrdinateDimension::desc()->delX( OdDbGripPointsPE::desc() );
  OdDb2LineAngularDimension::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbArcDimension::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbRadialDimensionLarge::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbBlockReference::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbMLeader::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbPolygonMesh::desc()->delX( OdDbPolygonMeshGripPointsPE::desc() );
  OdDb2dPolyline::desc()->delX( OdDbGripPointsPE::desc() );
  OdDbRasterImage::desc()->delX( OdDbRasterImageGripPointsPE::desc() );
  OdDbUnderlayReference::desc()->delX( OdDbDgnUnderlayGripPointsPE::desc() );
  ::odrxDynamicLinker()->loadModule(OdDbModuleName, false)->release();
}

bool projectOffset(const OdDbDatabase* pDb, const OdGeVector3d& vNormal, OdGeVector3d& vOffset)
{
  OdDbObjectId idVp = pDb->activeViewportId();
  OdDbObjectPtr pVp(idVp.openObject());
  OdAbstractViewPEPtr pAVp(pVp);
  if (!pAVp.isNull())
  {
    OdGeVector3d vViewDir = pAVp->direction(pVp);
    if (!vViewDir.isPerpendicularTo(vNormal))
    {
      OdGePlane pln(OdGePoint3d::kOrigin, vNormal);
      OdGeMatrix3d mProj;
      mProj.setToProjection(pln, vViewDir);
      vOffset.transformBy(mProj);
      return true;
    }
  }
  return false;
}
