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
/* Main header file for the DLL                                         */
/*                                                                      */
/* Sample implementation of the OdGeGripPointsPE interface.             */
/*                                                                      */
/* Drawings SDK attempts to use this interface for                      */
/* grip points operations;                                              */
/* OdDbEntity::getGripPoints, etc.                                      */
/*                                                                      */
/* This implementation supports only arcs, dimensions, lines, and       */
/* polylines. Support for additional entity types may be added by the   */
/* client                                                               */
/************************************************************************/
#if !defined(AFX_CODA1_13EXAMPLESGRIPPOINTSODGRIPPOINTSMODULE_H__A5F8E743_CFD3_4CDD_A179_02EA2B8B02E5__INCLUDED_)
#define AFX_CODA1_13EXAMPLESGRIPPOINTSODGRIPPOINTSMODULE_H__A5F8E743_CFD3_4CDD_A179_02EA2B8B02E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RxModule.h"
#include "StaticRxObject.h"
#include "DbArcGripPoints.h"
#include "DbLineGripPoints.h"
#include "DbPolylineGripPoints.h"
#include "DbEntityGripPoints.h"
#include "DbDimGripPoints.h"
#include "DbMlineGripPoints.h"
#include "DbBlockReferenceGripPoints.h"
#include "DbMleaderGripPoints.h"
#include "DbPolygonMeshGripPoints.h"
#include "DbPdfUnderlayGripPoints.h"
#include "DbViewportGripPoints.h"
#include "Db2dPolylineGripPoints.h"
#include "DbRasterImageGripPoints.h"
#include "DbTraceGripPoints.h"
#include "Db3dPolylineGripPoints.h"
#include "DbCameraGripPoints.h"
#include "DbCircleGripPoints.h"
#include "DbEllipseGripPoints.h"
#include "DbTextGripPoints.h"
#include "DbGeoPositionMarkerGripPoints.h"
#include "DbDgnUnderlayGripPoints.h"
#include "DbOleGripPoints.h"
#include "DbWipeOutGripPoints.h"
#include "DbFaceGripPoints.h"

/************************************************************************/
/* OdGripPointsModule                                                   */
/* See GripPoints.cpp for the implementation of this class              */
/************************************************************************/

class OdGripPointsModule : public OdRxModule
{
  OdStaticRxObject<OdDbLineGripPointsPE> _lgp;
  OdStaticRxObject<OdDbMlineGripPointsPE> _mlgp;
  OdStaticRxObject<OdDbMleaderGripPointsPE> _mleadgp;
  OdStaticRxObject<OdDbPolygonMeshGripPointsPE> _mmeshdgp;
  OdStaticRxObject<OdDbArcGripPointsPE> _agp;
  OdStaticRxObject<OdDbPolylineGripPointsPE> _plgp;
  OdStaticRxObject<OdDbEntityGripPointsPE> _egp;
  OdStaticRxObject<OdDbRotatedDimGripPointsPE> _rdgp;
  OdStaticRxObject<OdDbAlignedDimGripPointsPE> _adgp;
  OdStaticRxObject<OdDbRadialDimGripPointsPE> _rcdgp;
  OdStaticRxObject<OdDbDiametricDimGripPointsPE> _dcdgp;
  OdStaticRxObject<OdDb3PointAngularDimGripPointsPE> _3padgp;
  OdStaticRxObject<OdDbOrdinateDimGripPointsPE> _odgp;
  OdStaticRxObject<OdDb2LineAngularDimGripPointsPE> _2ladgp;
  OdStaticRxObject<OdDbArcDimGripPointsPE> _arcdgp;
  OdStaticRxObject<OdDbRadialDimLargeGripPointsPE> _rdlgp;
  OdStaticRxObject<OdDbBlockReferenceGripPointsPE> _insertgp;
  OdStaticRxObject<OdDbPdfUnderlayGripPointsPE> _pdfunderlaygp;
  OdStaticRxObject<OdDbViewportGripPointsPE> _vptgp;
  OdStaticRxObject<OdDb2dPolylineGripPointsPE> _2dplgp;
  OdStaticRxObject<OdDbRasterImageGripPointsPE> _rimggp;
  OdStaticRxObject<OdDbTraceGripPointsPE> _trcgp;
  OdStaticRxObject<OdDbSolidGripPointsPE> _sldgp;
  OdStaticRxObject<OdDb3dPolylineGripPointsPE> _3dplgp;
  OdStaticRxObject<OdDbCameraGripPointsPE> _camgp;
  OdStaticRxObject<OdDbCircleGripPointsPE> _cgp;
  OdStaticRxObject<OdDbEllipseGripPointsPE> _elgp;
  OdStaticRxObject<OdDbTextGripPointsPE> _txtgp;
  OdStaticRxObject<OdDbGeoPositionMarkerPE> _geoposmarkgp;
  OdStaticRxObject<OdDbDgnUnderlayGripPointsPE> _dgnunderlaygp;
  OdStaticRxObject<OdDbOleGripPointsPE> _olegp;
  OdStaticRxObject<OdDbWipeOutGripPointsPE> _wipeoutgp;
  OdStaticRxObject<OdDbFaceGripPointsPE> _fgp;

protected:
  OdGripPointsModule();
  void initApp();
  void uninitApp();
public:
  ~OdGripPointsModule();
};

////////////////////////////////////////////////////////////////////////////////////
// For 2D object projects given vOffset vector on object's plane defined by vNormal
// in current view direction.
// Returns true on success.
//
// If current view direction is perpendicular to vNormal returns false and does not modify vOffset
//
bool projectOffset(const OdDbDatabase* pDb, const OdGeVector3d& vNormal, OdGeVector3d& vOffset);


#endif // !defined(AFX_CODA1_13EXAMPLESGRIPPOINTSODGRIPPOINTSMODULE_H__A5F8E743_CFD3_4CDD_A179_02EA2B8B02E5__INCLUDED_)
