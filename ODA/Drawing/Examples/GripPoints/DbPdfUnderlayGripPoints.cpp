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
/* This class is an implementation of the OdDbGripPointsPE class for    */
/* OdDbPdfUnderlay entities                                              */
/************************************************************************/

#include "StdAfx.h"
#include "DbPdfUnderlayGripPoints.h"
#include "DbUnderlayReference.h"
#include "RxObjectImpl.h"
#include "Gi/GiViewport.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiDummyGeometry.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeCircArc3d.h"
#include "DbGrip.h"
#include "TDVersion.h"
#include "StaticRxObject.h"
#include "../Extensions/PdfUnderlayCommon/PdfUnderlay.h"
#include "DbUnderlayDefinition.h"

OdResult OdDbPdfUnderlayGripPointsPE::getOsnapPoints(const OdDbEntity* ent, 
                                             OdDb::OsnapMode osnapMode, 
                                             OdGsMarker gsSelectionMark, 
                                             const OdGePoint3d& pickPoint,
                                             const OdGePoint3d& lastPoint, 
                                             const OdGeMatrix3d& xWorldToEye, 
                                             OdGePoint3dArray& snapPoints) const
{
  if (osnapMode != OdDb::kOsModeEnd)
    return eNotImplemented;

  // FLYSDK version
  //PdfUnderlayModulePtr pModule = odrxDynamicLinker()->loadModule(OdPdfModuleVIModuleName);
  //
  PdfUnderlayModulePtr pModule = OdDbPdfDefinition::loadPdfUnderlayModule();
  if (pModule.isNull())
    return eTxError;

  OdResult res = OdDbUnderlayGripPointsPE::getOsnapPoints(ent, osnapMode, gsSelectionMark, pickPoint, lastPoint, xWorldToEye, snapPoints);
  if (eOk == res)
  {
    class PdfSnapGrabberImpl : public OdStaticRxObject<OdGiDummyGeometry<OdGiGeometry> >
    {
      OdGePoint3dArray& snapPoints;
      OdGeMatrix3d mx;
    public:
      PdfSnapGrabberImpl(OdGePoint3dArray& sp, const OdGeMatrix3d& m) : snapPoints(sp), mx(m) {}

      void polyline(OdInt32 numVertices,
        const OdGePoint3d* vertexList,
        const OdGeVector3d* /*pNormal*/,
        OdGsMarker /*baseSubEntMarker*/)
      {
        for (int f = 0; f < numVertices; ++f)
        {
          OdGePoint3d p = vertexList[f];
          snapPoints.push_back(p.transformBy(mx));
        }
      }
      void nurbs(const OdGeNurbCurve3d& nurbsCurve)
      {
        OdGePoint3d point;
        if (nurbsCurve.hasStartPoint(point))
        {
          snapPoints.push_back(point.transformBy(mx));
        }
        if (nurbsCurve.hasEndPoint(point))
        {
          snapPoints.push_back(point.transformBy(mx));
        }
      }

      void circularArc(const OdGePoint3d& start, const OdGePoint3d&, const OdGePoint3d& end, OdGiArcType)
      {
        OdGePoint3d p = start;
        snapPoints.push_back(p.transformBy(mx));
        p = end;
        snapPoints.push_back(p.transformBy(mx));
      }

      void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal)
      {
        OdGeCircArc3d arc(center, normal, radius);

        OdGePoint3d point;
        if (arc.hasStartPoint(point))
        {
          snapPoints.push_back(point.transformBy(mx));
        }
      }
    };

    OdDbUnderlayReferencePtr pPdfRef = OdDbUnderlayReference::cast(ent);
    OdGeMatrix3d transformRef;
    PdfSnapGrabberImpl PdfSnapGrabber(snapPoints, transformRef); //transformation is not needed because it has been applied to pickPoint (and will be applied to snap points) on app level
    return pModule->getSnapGeometry(pPdfRef, &PdfSnapGrabber, &pickPoint);
  }
  return res;
}
