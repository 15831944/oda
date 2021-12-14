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

// ExClipUnderlayCommand.cpp: implementation of the ClipUnderlay commands.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ExClipUnderlayCommand.h"
#include "Ge/GeExtents3d.h"
#include "Db3dSolid.h"
#include "DbPolyline.h"
#include "DbUnderlayReference.h"

template <class Underlay>
class UnderelaySourceTracker : public OdStaticRxObject<OdEdSSetTracker>
{

public:
  UnderelaySourceTracker()
  {}
  virtual int addDrawables(OdGsView*) { return 0; }
  virtual void removeDrawables(OdGsView*) {}

  virtual bool append(const OdDbObjectId& entId, const OdDbSelectionMethod*)
  {
    OdDbObjectPtr obj = entId.openObject();
    return !obj.isNull() && obj->isKindOf(Underlay::desc());
  }
  virtual bool remove(const OdDbObjectId&, const OdDbSelectionMethod*)
  {
    return true;
  }
};

class ClipUnderlayBasePointTracker : public OdStaticRxObject<OdEdPointTracker>
{
protected:

  OdDbDatabase*  m_pDb;
  OdGePoint3d    m_FirstPt;
  OdGeExtents3d  m_Ext;

public:
  ClipUnderlayBasePointTracker(OdDbDatabase* pDb, const OdGePoint3d& first_point, const OdGeExtents3d& ext)
    : m_pDb(pDb)
    , m_FirstPt(first_point)
    , m_Ext(ext)
  {
    fixPoint(m_FirstPt);
  }

  static OdGeMatrix3d getUcsToWcsMatrix(OdDbDatabase* pDb)
  {
    OdGeMatrix3d ucsToWcsMatrix;
    ucsToWcsMatrix.setCoordSystem(pDb->getUCSORG(), pDb->getUCSXDIR(), pDb->getUCSYDIR(),
      pDb->getUCSXDIR().crossProduct(pDb->getUCSYDIR()));
    return ucsToWcsMatrix;
  }

  void fixPoint(OdGePoint3d& point)
  {
    point.x = odmax(point.x, m_Ext.minPoint().x);
    point.x = odmin(point.x, m_Ext.maxPoint().x);
    point.y = odmax(point.y, m_Ext.minPoint().y);
    point.y = odmin(point.y, m_Ext.maxPoint().y);
  }

};

class ClipUnderlayRectPointTracker : public ClipUnderlayBasePointTracker
{
  OdDb3dSolidPtr m_ent;
  OdGePoint3d    m_SecondPt;
  double         m_xLen;
  double         m_yLen;

public:
  ClipUnderlayRectPointTracker(OdDbDatabase* pDb, const OdGePoint3d& first_point, const OdGeExtents3d& ext, OdDb3dSolidPtr ent)
    : ClipUnderlayBasePointTracker(pDb, first_point, ext)
    , m_ent(ent)
    , m_SecondPt(first_point)
    , m_xLen(0.001)
    , m_yLen(0.001)
  {

  }

  virtual int addDrawables(OdGsView* pView)
  {
    pView->add(m_ent, 0);
    return 1;
  }

  virtual void removeDrawables(OdGsView* pView)
  {
    pView->erase(m_ent);
  }

  virtual void setValue(const OdGePoint3d& value)
  {
    m_SecondPt = value;
    fixPoint(m_SecondPt);
    OdGePoint3d center = (m_FirstPt + m_SecondPt.asVector()) / 2.0;
    OdGeVector3d vect = m_SecondPt - m_FirstPt;
    m_xLen = odmax(0.001, fabs(vect.x));
    m_yLen = odmax(0.001, fabs(vect.y));
    m_ent->createBox(m_xLen, m_yLen, 0.001);
    m_ent->transformBy(OdGeMatrix3d::translation(center.asVector()));
    m_ent->transformBy(getUcsToWcsMatrix(m_pDb));
  }

  OdGePoint3d  getFirstPoint() const
  {
    return m_FirstPt;
  }

  OdGePoint3d getSecondPoint() const
  {
    return m_SecondPt;
  }

};

class ClipUnderlayPolyPointTracker : public ClipUnderlayBasePointTracker
{
  OdDbPolylinePtr   m_ent;
  OdGePoint3d       m_Point;

public:
  ClipUnderlayPolyPointTracker(OdDbDatabase* pDb, const OdGePoint3d& first_point, const OdGeExtents3d& ext, OdDbPolylinePtr ent)
    : ClipUnderlayBasePointTracker(pDb, first_point, ext)
    , m_ent(ent)
    , m_Point(first_point)
  {
    m_ent->addVertexAt(0, m_FirstPt.convert2d());
    m_ent->addVertexAt(1, m_FirstPt.convert2d());
  }

  virtual int addDrawables(OdGsView* pView)
  {
    m_ent->addVertexAt(m_ent->numVerts() - 2, m_Point.convert2d());
    pView->add(m_ent, 0);
    return 1;
  }

  virtual void removeDrawables(OdGsView* pView)
  {
    pView->erase(m_ent);
  }

  virtual void setValue(const OdGePoint3d& value)
  {
    m_Point = value;
    fixPoint(m_Point);
    m_ent->setPointAt(m_ent->numVerts() - 2, m_Point.convert2d());
  }
};

template<>
OdString ClipUnderlayCommand<OdDbPdfReference>::moduleName() const
{
  return OD_T("PDF");
}

template<>
OdString ClipUnderlayCommand<OdDbDwfReference>::moduleName() const
{
  return OD_T("DWF");
}

template<>
OdString ClipUnderlayCommand<OdDbDgnReference>::moduleName() const
{
  return OD_T("DGN");
}

template<class Underlay>
void ClipUnderlayCommand<Underlay>::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbUserIOPtr pIO = OdDbCommandContextPtr(pCmdCtx)->dbUserIO();
  try
  {
    UnderelaySourceTracker<Underlay> trecker;
    OdDbSelectionSetPtr pSSet;
    do
    {
      pSSet = pIO->select(OdString().format(OD_T("Select %lsUnderlay:"), moduleName().c_str()), OdEd::kSelSingleEntity | OdEd::kSelSinglePass, NULL, OdString::kEmpty, &trecker);
    } while (!pSSet->numEntities());

    OdSmartPtr<Underlay> pRef = Underlay::cast(pSSet->objectIdArray().last().openObject(OdDb::kForWrite));
    if (pRef.isNull())
      return;

    OdUInt8 options = pIO->getKeyword(OD_T("Enter PDF clipping option [ON/OFF/Delete/New boundary]"), OD_T("ON OFF Delete New_boundary"), 3);

    switch (options)
    {
    case 0:
      pRef->setIsClipped(true);
      return;
      break;
    case 1:
      pRef->setIsClipped(false);
      return;
      break;
    case 2:
      pRef->setClipBoundary(NULL, 0);
      return;
      break;
    }

    const OdGePoint2dArray& old_boundary = pRef->clipBoundary();
    if (0 != old_boundary.size())
    {
      if (0 == pIO->getKeyword(OD_T("Delete old boundary? [Yes/No]"), OD_T("Yes No"), 0))
      {
        pRef->setClipBoundary(NULL, 0);
      }
      else
      {
        return;
      }
    }

    OdUInt8 sel_options = 2;
    bool is_clip_inverted = false;
    while (true)
    {
      sel_options = pIO->getKeyword(OD_T("Select [Polygonal/Rectangular/Invert clip]"), OD_T("Polygonal Rectangular Invert_clip"), 1);
      if (2 != sel_options)
        break;
      is_clip_inverted = !is_clip_inverted;
      if (is_clip_inverted)
        pIO->putString(OD_T("Inside mode - Objects inside boundary will be hidden."));
      else
        pIO->putString(OD_T("Outside mode - Objects outside boundary will be hidden."));
    }

    OdDbDatabase* pDb = OdDbCommandContextPtr(pCmdCtx)->database();
    OdGePoint2dArray clip_boundary;
    OdGeExtents3d ext;
    pRef->getGeomExtents(ext);
    OdGePoint3d pos = pRef->position();
    OdGePoint3d first_point = pos;
    if (1 == sel_options)
    {
      first_point = pIO->getPoint(OD_T("Specify first corner: "));
      OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
      pSolid->setDatabaseDefaults(pDb);
      ClipUnderlayRectPointTracker clip_tracker(pDb, first_point, ext, pSolid);
      pIO->getPoint(OD_T("Specify second corner: "), OdEd::kGptDefault, NULL, OD_T("second corner"), &clip_tracker);
      first_point = clip_tracker.getFirstPoint();
      OdGePoint3d second_point = clip_tracker.getSecondPoint();
      clip_boundary.append(first_point.convert2d() - pos.convert2d().asVector());
      clip_boundary.append(OdGePoint2d(first_point.x, second_point.y) - pos.convert2d().asVector());
      clip_boundary.append(second_point.convert2d() - pos.convert2d().asVector());
      clip_boundary.append(OdGePoint2d(second_point.x, first_point.y) - pos.convert2d().asVector());
    }
    else
    {
      first_point = pIO->getPoint(OD_T("Specify first point: "));
      clip_boundary.append(first_point.convert2d() - pos.convert2d().asVector());
      OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();
      pPolyline->setDatabaseDefaults(pDb);
      ClipUnderlayPolyPointTracker clip_tracker1(pDb, first_point, ext, pPolyline);
      bool resum = true;
      do
      {
        try
        {
          clip_boundary.append(pIO->getPoint(OD_T("Specify next point: "), OdEd::kInpThrowEmpty, NULL, OD_T("next point"), &clip_tracker1).convert2d() - pos.convert2d().asVector());
        }
        catch (const OdEdEmptyInput&)
        {
          resum = false;
        }
      } while (resum);
    }
    if (0 != clip_boundary.size())
    {
      pRef->setClipBoundary(clip_boundary.asArrayPtr(), clip_boundary.size());
      pRef->setClipInverted(is_clip_inverted);
      pIO->putString(OD_T("Underlay clipped"));
    }

  }
  catch (const OdEdKeyword&)
  {
  }
}

template class ClipUnderlayCommand<OdDbPdfReference>;
template class ClipUnderlayCommand<OdDbDwfReference>;
template class ClipUnderlayCommand<OdDbDgnReference>;
