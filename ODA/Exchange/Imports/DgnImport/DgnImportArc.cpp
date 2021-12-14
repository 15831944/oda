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
#include "DgnImportArc.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include <DgArc.h>
#include <DbCircle.h>
#include <DbArc.h>
#include <DbEllipse.h>
#include <DbLine.h>

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

void OdDgnArc2dImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgArc2d* arc = (OdDgArc2d*)e;
  double a1 = arc->getPrimaryAxis();
  double a2 = arc->getSecondaryAxis();
  OdGePoint2d o = arc->getOrigin();
  double startAng = arc->getStartAngle();
  double sweep = arc->getSweepAngle();
  OdDbEntityPtr dbEnt;
  if (OdEqual(a1, a2))
  {
    if (OdEqual(fabs(sweep), Oda2PI))
    {
      OdDbCirclePtr dbArc = OdDbCircle::createObject();
      dbArc->setCenter(OdGePoint3d(o.x, o.y, 0.0));
      dbArc->setRadius(a1);
      dbArc->setThickness(arc->getThickness());
      dbArc->setNormal(OdGeVector3d::kZAxis);
      dbEnt = dbArc;
    }
    else
    {
      OdDbArcPtr dbArc = OdDbArc::createObject();
      dbArc->setCenter(OdGePoint3d(o.x, o.y, 0.0));
      dbArc->setRadius(a1);
      dbArc->setThickness(arc->getThickness());
      double a = arc->getRotationAngle();
      if (sweep < 0)
      {
        dbArc->setStartAngle(startAng + sweep + a);
        dbArc->setEndAngle(startAng + a);
      }
      else
      {
        dbArc->setStartAngle(startAng + a);
        dbArc->setEndAngle(startAng + sweep + a);
      }
      dbArc->setNormal(OdGeVector3d::kZAxis);
      dbEnt = dbArc;
    }
  }
  else
  {
    if (odmax(a1, a2) < 1e-6) // DB ellipse cannot accept axis shorter than 1e-6
	    return;

    if (OdZero(a1) || OdZero(a2))
    {
      OdGeEllipArc2d ellipArc = arc->getEllipArc();
      OdDbLinePtr dbLine = OdDbLine::createObject();
      OdGePoint2d  ptCenter2d = ellipArc.center();
      OdGeVector2d vrMajor2d = ellipArc.majorAxis()*ellipArc.majorRadius();
      OdGePoint3d  ptCenter3d(ptCenter2d.x, ptCenter2d.y, 0.0);
      OdGeVector3d vrMajor3d(vrMajor2d.x, vrMajor2d.y, 0.0);
      dbLine->setStartPoint(ptCenter3d + vrMajor3d);
      dbLine->setEndPoint(ptCenter3d - vrMajor3d);
      dbEnt = dbLine;
    }
    else if (a2 > a1)
    {
      OdGeVector3d major = OdGeVector3d::kYAxis;
      major.rotateBy(arc->getRotationAngle(), OdGeVector3d::kZAxis);
      major *= a2;
      OdDbEllipsePtr dbArc = OdDbEllipse::createObject();
      if (sweep > 0)
      {
        dbArc->set(OdGePoint3d(o.x, o.y, 0.0), OdGeVector3d::kZAxis, major, 
         OdZero(a1) ? 1.0 : (a1 / a2), startAng - OdaPI2, startAng + sweep - OdaPI2);
      }
      else
      {
        dbArc->set(OdGePoint3d(o.x, o.y, 0.0), OdGeVector3d::kZAxis, major, 
          OdZero(a1) ? 1.0 : (a1 / a2), startAng + sweep - OdaPI2, startAng - OdaPI2);
      }
      dbEnt = dbArc;
    }
    else
    {
      OdGeVector3d major = OdGeVector3d::kXAxis;
      major.rotateBy(arc->getRotationAngle(), OdGeVector3d::kZAxis);
      major *= a1;
      OdDbEllipsePtr dbArc = OdDbEllipse::createObject();
      if (sweep > 0)
      {
        dbArc->set(OdGePoint3d(o.x, o.y, 0.0), OdGeVector3d::kZAxis, major, 
          OdZero(a2) ? 1.0 : (a2 / a1), startAng, startAng + sweep);
      }
      else
      {
        dbArc->set(OdGePoint3d(o.x, o.y, 0.0), OdGeVector3d::kZAxis, major, 
          OdZero(a2) ? 1.0 : (a2 / a1), startAng + sweep, startAng);
      }
      dbEnt = dbArc;
    }
  }
  dbEnt->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(dbEnt, owner->objectId());
  copyEntityProperties(arc, dbEnt);
  owner->appendOdDbEntity(dbEnt);

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( dbEnt->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( arc->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------


void OdDgnArc3dImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgArc3d* arc = (OdDgArc3d*)e;
  double a1 = arc->getPrimaryAxis();
  double a2 = arc->getSecondaryAxis();

  OdDbEntityPtr dbEnt;

  if( (a1 < 0) || (a2 < 0) )
  {
    OdGeEllipArc3d ellipArc = arc->getEllipArc();

    if( ellipArc.isCircular() && ellipArc.isClosed())
    {
      OdDbCirclePtr dbArc = OdDbCircle::createObject();
      dbArc->setCenter(ellipArc.center());
      dbArc->setRadius(ellipArc.majorRadius());
      dbArc->setThickness(arc->getThickness());
      dbArc->setNormal(arc->getNormal());
      dbEnt = dbArc;
    }
    else
    {
      OdDbEllipsePtr dbArc = OdDbEllipse::createObject();
      dbArc->setFromOdGeCurve(ellipArc);
      dbEnt = dbArc;
    }
  }
  else
  {
    OdGePoint3d o = arc->getOrigin();
    double startAng = arc->getStartAngle();
    double sweep = arc->getSweepAngle();

    if (OdEqual(a1, a2))
    {
      if (OdEqual(fabs(sweep), Oda2PI))
      {
        OdDbCirclePtr dbArc = OdDbCircle::createObject();
        dbArc->setCenter(o);
        dbArc->setRadius(a1);
        dbArc->setThickness(arc->getThickness());
        dbArc->setNormal(arc->getNormal());
        dbEnt = dbArc;
      }
      else
      {
        OdDbArcPtr dbArc = OdDbArc::createObject();
        dbArc->setCenter(o);
        dbArc->setRadius(a1);
        dbArc->setThickness(arc->getThickness());
        OdGeVector3d normal = arc->getNormal();

        OdGePlane p(o, normal);
        OdGeVector3d refvector = normal.perpVector();
        OdGeVector3d refvector1 = OdGeVector3d::kXAxis;
        arc->getRotation().rotate(refvector1);
        double a = refvector1.angleOnPlane(p) - refvector.angleOnPlane(p);
        if (sweep < 0)
        {
          dbArc->setStartAngle(startAng + sweep + a);
          dbArc->setEndAngle(startAng + a);
        }
        else
        {
          dbArc->setStartAngle(startAng + a);
          dbArc->setEndAngle(startAng + sweep + a);
        }
        dbArc->setNormal(normal);
        dbEnt = dbArc;
      }
    }
    else
    {
      if (odmax(a1, a2) < 1e-6) // DB ellipse cannot accept axis shorter than 1e-6
        return;

      if( OdZero(a1) || OdZero(a2) )
      {
        OdGeEllipArc3d ellipArc = arc->getEllipArc();
        OdDbLinePtr dbLine = OdDbLine::createObject();
        dbLine->setStartPoint(ellipArc.center() + ellipArc.majorAxis()*ellipArc.majorRadius());
        dbLine->setEndPoint(ellipArc.center() - ellipArc.majorAxis()*ellipArc.majorRadius());
        dbEnt = dbLine;
      }
      else
      {
        OdDbEllipsePtr dbArc = OdDbEllipse::createObject();
        dbArc->setFromOdGeCurve(arc->getEllipArc());
        dbEnt = dbArc;
      }
    }
  }

  dbEnt->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(dbEnt, owner->objectId());
  copyEntityProperties(arc, dbEnt);
  owner->appendOdDbEntity(dbEnt);

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( dbEnt->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( arc->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

}
