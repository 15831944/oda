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

// ExLine.cpp: implementation of the ExLine function.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DbGeoPositionMarker.h"


struct MarkerTracker
{
  OdDbGeoPositionMarker*  m_pMarker;                                  
    
  MarkerTracker(OdDbGeoPositionMarker* pMarker)
    : m_pMarker(pMarker)
  {
  }
/*
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
*/
  int addDrawables(OdGsView* pView)              // Add the dragged circle to the view
  {
    pView->add(m_pMarker, 0);
    return 1;
  };

  void removeDrawables(OdGsView* pView)   // Remove drawables from the view
  {
    pView->erase(m_pMarker);
  }
};


void trackAnnotationLocation(OdSmartPtr<OdDbUserIO> pIO, MarkerTracker* pCrTr)
{
  struct AnnoLocationTracker : OdStaticRxObject<OdEdPointTracker>
  {
    MarkerTracker* m_pCrTr;

    AnnoLocationTracker(MarkerTracker* pCrTr)
      : m_pCrTr(pCrTr)
    {}

    virtual void setValue(const OdGePoint3d& pt)
    {
      OdDbMTextPtr pMText = m_pCrTr->m_pMarker->mtext();
      pMText->setLocation(pt);
      m_pCrTr->m_pMarker->setMText(pMText);
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

  pIO->getPoint(L"Specify annotation location",
                  OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand,
                  NULL, OdString::kEmpty, &tracker);
}


void trackAnnotationWidth(OdSmartPtr<OdDbUserIO> pIO, MarkerTracker* pCrTr)
{
  struct WidthTracker : OdStaticRxObject<OdEdRealTracker>
  {
    MarkerTracker* m_pCrTr;

    WidthTracker(MarkerTracker* pCrTr)
      : m_pCrTr(pCrTr)
    {}

    virtual void setValue(double dWidth)
    {
      OdDbMTextPtr pMText = m_pCrTr->m_pMarker->mtext();
      pMText->setWidth(dWidth);
      m_pCrTr->m_pMarker->setMText(pMText);
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

  // Turn on MText frame
  OdDbMTextPtr pMText = pCrTr->m_pMarker->mtext();
  pMText->setShowBorders(true);
  pCrTr->m_pMarker->setMText(pMText);

  pIO->getDist( L"Specify Annotation width: ",
                OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand | OdEd::kInpThrowEmpty,
                1., 
                OdString::kEmpty,
                &tracker );

  // Turn off MText frame
  pMText = pCrTr->m_pMarker->mtext();
  pMText->setShowBorders(false);
  pCrTr->m_pMarker->setMText(pMText);
}

void trackAnnotationText(OdSmartPtr<OdDbUserIO> pIO, MarkerTracker* pCrTr)
{
  struct TextTracker : OdStaticRxObject<OdEdStringTracker>
  {
    MarkerTracker* m_pCrTr;

    TextTracker(MarkerTracker* pCrTr)
      : m_pCrTr(pCrTr)
    {}

    virtual void setValue(const OdString& strText)
    {
      OdDbMTextPtr pMText = m_pCrTr->m_pMarker->mtext();
      pMText->setContents(strText);
      m_pCrTr->m_pMarker->setMText(pMText);
    }
    virtual int addDrawables(OdGsView* pView)
    {
      return m_pCrTr->addDrawables(pView);
    }

    virtual void removeDrawables(OdGsView* pView)
    {
      m_pCrTr->removeDrawables(pView);
    }
  } tracker(pCrTr);

  pIO->getString(L"Specify text string: ",
                                  OdEd::kGstAllowSpaces | OdEd::kGstNoEmpty,
                                  OdString::kEmpty, OdString::kEmpty, &tracker);
}


void _GeoMarkPosition_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
  OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

  OdGeVector3d vNormal(ExCommandsUtils::ucsNormal(pDb));

  OdGePoint3d ptPosition = pIO->getPoint(L"Specify Position:");
  OdDbGeoPositionMarkerPtr pMarker = OdDbGeoPositionMarker::createObject();
  pMarker->setDatabaseDefaults(pDb);
  pMarker->setPosition(ptPosition);
  OdDbMTextPtr pMText = OdDbMText::createObject();
  pMText->setDatabaseDefaults(pDb);
  pMText->setNormal(vNormal);
  pMText->setLocation(ptPosition);
  pMText->setWidth(pMarker->radius() * 10.);
  pMText->setContents(L"Enter annotation");
  pMarker->setMText(pMText);
  pMarker->setEnableFrameText(true);

  MarkerTracker trMarker(pMarker);
  trackAnnotationLocation(pIO, &trMarker);
  pMarker->setEnableFrameText(false);
  trackAnnotationWidth(pIO, &trMarker);
  trackAnnotationText(pIO, &trMarker);

  pSpace->appendOdDbEntity(pMarker);
}
