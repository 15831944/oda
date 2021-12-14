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

// ExCircle.cpp: implementation of the ExCircle function.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DbCircle.h"
#include "DbText.h"
#include "Ge/GeCircArc3d.h"


//--------------------------------------------------------------------------------------------------------------------
struct CircleTracker
{
  OdDbCircle*  m_pCircle;                                  
  OdDbTextPtr  m_pText;
    
  CircleTracker(OdDbCircle* pCircle)
    : m_pCircle(pCircle)
  {
    m_pText = OdDbText::createObject();
    m_pText->setDatabaseDefaults(pCircle->database());
    m_pText->setNormal(pCircle->normal());
    OdCmColor Color;
    Color.setColorMethod(OdCmEntityColor::kByColor);
    Color.setGreen(200);
    m_pText->setColor(Color);
  }

  void update()
  {
    double dRadius =  m_pCircle->radius();
    OdString str = m_pCircle->database()->formatter().formatLinear(dRadius);
    m_pText->setTextString(str);

    OdGeExtents3d Extents;
    m_pText->getGeomExtents(Extents);
    double w = Extents.maxPoint().x - Extents.minPoint().x;
    m_pText->setPosition(m_pCircle->center() + OdGeVector3d::kXAxis * (dRadius - w)/2);
  }

  int addDrawables(OdGsView* pView)              // Add the dragged circle to the view
  {
    pView->add(m_pCircle, 0);
    pView->add(m_pText,   0);

    OdGePoint2d pixelDensity;
    pView->getNumPixelsInUnitSquare(OdGePoint3d(), pixelDensity);
    m_pText->setHeight(10. / pixelDensity.x);
    return 1;
  };

  void removeDrawables(OdGsView* pView)   // Remove drawables from the view
  {
    pView->erase(m_pCircle);
    pView->erase(m_pText);
  }
};


//--------------------------------------------------------------------------------------------------------------------
void trackRadius(OdSmartPtr<OdDbUserIO> pIO, CircleTracker* pCrTr)
{
  struct RadiusTracker : OdStaticRxObject<OdEdRealTracker> //--- use to drag the radius  -----------------------------
  {
    CircleTracker* m_pCrTr;
    bool m_bDiameter;

    RadiusTracker(CircleTracker* pCrTr)
      : m_pCrTr(pCrTr)
      , m_bDiameter(false)
    {}

    virtual void setValue(double dRadius)        // callback function, call OnMouseMove,
    {
      if (m_bDiameter)
        dRadius /= 2.;
      m_pCrTr->m_pCircle->setRadius(dRadius);
      m_pCrTr->update();
    }
    virtual int addDrawables(OdGsView* pView)
    {
      return m_pCrTr->addDrawables(pView);
    }

    virtual void removeDrawables(OdGsView* pView)
    {
      m_pCrTr->removeDrawables(pView);
    }
  }
  tracker(pCrTr);
  try
  {
    pIO->getDist( L"Specify radius of circle or [Diameter]: ",   // allows drawing the dragged circle and radius 
                OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand | OdEd::kInpThrowEmpty,
                1., 
                L"Diameter", 
                &tracker );
  }
  catch(const OdEdKeyword &kwd)
  {
    if (kwd.keywordIndex() != 0)
    {
      ODA_FAIL();
    }
    tracker.m_bDiameter = true;
    pIO->getDist( L"Specify diameter of circle: ",
                OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand | OdEd::kInpThrowEmpty,
                1., 
                OdString::kEmpty, 
                &tracker );
  }
}

void track3Points(OdSmartPtr<OdDbUserIO> pIO, CircleTracker* pCrTr)
{
  struct Cr3PointTracker : OdStaticRxObject<OdEdPointTracker>
  {
    OdGePoint3d m_pt1;
    OdGePoint3d m_pt2;
    CircleTracker* m_pCrTr;

    Cr3PointTracker(CircleTracker* pCrTr)
      : m_pCrTr(pCrTr)
    {}

    virtual void setValue(const OdGePoint3d& pt3)
    {
      OdGeCircArc3d geArc;
      // Check if arc changed direction. Normal should not be flipped.
      OdGeVector3d v1 = m_pt2 - m_pt1;
      OdGeVector3d v2 = pt3 - m_pt2;
      OdGeVector3d vNewNormal = v1.crossProduct(v2);
      if (vNewNormal == OdGeVector3d::kIdentity)
        return;

      if (vNewNormal.isCodirectionalTo(m_pCrTr->m_pCircle->normal()))
      { // OK
        geArc.set(m_pt1, m_pt2, pt3);
      }
      else
      {
        geArc.set(pt3, m_pt2, m_pt1);
      }
      geArc.setAngles(0., Oda2PI);
      m_pCrTr->m_pCircle->setFromOdGeCurve(geArc);

      m_pCrTr->update();
    }
    virtual int addDrawables(OdGsView* pView)
    {
      return m_pCrTr->addDrawables(pView);
    }

    virtual void removeDrawables(OdGsView* pView)
    {
      m_pCrTr->removeDrawables(pView);
    }
  }
  tracker(pCrTr);

  tracker.m_pt1 = pIO->getPoint(L"Specify first point on circle");
  tracker.m_pt2 = pIO->getPoint(L"Specify second point on circle",
                                OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand);
     
  pIO->getPoint(L"Specify third point on circle",
                  OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand,
                  NULL, OdString::kEmpty, &tracker);
}

void track2Points(OdSmartPtr<OdDbUserIO> pIO, CircleTracker* pCrTr)
{
  struct Cr3PointTracker : OdStaticRxObject<OdEdPointTracker>
  {
    OdGePoint3d m_pt1;
    CircleTracker* m_pCrTr;

    Cr3PointTracker(CircleTracker* pCrTr)
      : m_pCrTr(pCrTr)
    {}

    virtual void setValue(const OdGePoint3d& pt2)
    {
      OdGeVector3d vC = (pt2 - m_pt1) / 2.;
      double dRadius = vC.length();
      if (!OdZero(dRadius))
      {
        m_pCrTr->m_pCircle->setCenter(m_pt1 + vC);
        m_pCrTr->m_pCircle->setRadius(vC.length());
      }

      m_pCrTr->update();
    }
    virtual int addDrawables(OdGsView* pView)
    {
      return m_pCrTr->addDrawables(pView);
    }

    virtual void removeDrawables(OdGsView* pView)
    {
      m_pCrTr->removeDrawables(pView);
    }
  }
  tracker(pCrTr);

  tracker.m_pt1  = pIO->getPoint(L"Specify first end point of circle's diameter: ");
     
  pIO->getPoint(L"Specify second end point of circle's diameter: ",
                  OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand,
                  NULL, OdString::kEmpty, &tracker);
}


//---------------------------------------------------------------------------------------------------------------------

void _Circle_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr  pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr        pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdGeVector3d vNormal(ExCommandsUtils::ucsNormal(pDb));
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(pDb);
  pCircle->setNormal(vNormal);

  CircleTracker trCircle(pCircle);

  OdGePoint3d ptCenter;
  int nVar = -1;
  try
  {
    ptCenter = pIO->getPoint(L"Specify center point for circle or [3P/2P]: ",
                              OdEd::kGptDefault, NULL, L"3P 2P");  
  }
  catch(const OdEdKeyword &kwd)
  {
    nVar = kwd.keywordIndex();
  }
  
  switch (nVar)
  {
  case -1:
    pCircle->setCenter(ptCenter);
    trackRadius(pIO, &trCircle);
    break;
  case 0:
    track3Points(pIO, &trCircle);
    break;
  case 1:
    track2Points(pIO, &trCircle);
    break;
  default:
    ODA_FAIL();
  }

  OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
  OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);
  pSpace->appendOdDbEntity(pCircle);
}
