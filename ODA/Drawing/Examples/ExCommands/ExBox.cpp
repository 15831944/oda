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
//
// ObjectsCommands.cpp
//

#include "StdAfx.h"
#include "Db3dSolid.h"

void _Box_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDb = pDbCmdCtx->database();
  OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  typedef enum 
  {
    enBox_FirstCorner
  , enBox_OtherCorner
  , enBox_CubeByFirst
  , enBox_Length
  , enBox_Width
  , enBox_Height
  , enBox_Height2Point

  , enBox_Center
  , enBox_Corner
  , enBox_CubeByCenter
  , enBox_LengthByCenter
  , enBox_WidthByCenter
  , enBox_HeightByCenter

  , enBox_Finish
  } TBoxState;
  OdDb3dSolidPtr pSolid;

  struct BoxTracker : public OdStaticRxObject<OdEdPointTracker>, 
                      public OdStaticRxObject<OdEdRealTracker>
  {
    OdDbDatabase* m_pDb;
    OdDb3dSolidPtr m_ent;
    OdGePoint3d m_ptFirst,
                m_ptOther,
                m_ptCenter;
    double m_xLen, 
           m_yLen, 
           m_zLen,
           m_angZ;
    TBoxState m_enState;

    BoxTracker()
      : m_pDb(NULL)
      , m_enState(enBox_Finish)
      , m_xLen(0.001)
      , m_yLen(0.001)
      , m_zLen(0.001)
      , m_angZ(0.0)
    {
    }

    static OdGeMatrix3d getUcsToWcsMatrix(OdDbDatabase* pDb)
    {
      OdGeMatrix3d ucsToWcsMatrix;
      ucsToWcsMatrix.setCoordSystem(pDb->getUCSORG(), pDb->getUCSXDIR(), pDb->getUCSYDIR(),
                                    pDb->getUCSXDIR().crossProduct(pDb->getUCSYDIR()));
      return ucsToWcsMatrix;
    }

    OdGeMatrix3d getUcsToWcsMatrix()
    {
      return getUcsToWcsMatrix(m_pDb);
    }

    void updateZAngle(const OdGeVector3d& vect)
    {
      OdGeVector3d vectNorm = vect;
      m_angZ = OD_ASIN(vectNorm.normalize().x);
      if (!OdZero(m_angZ))
      {
        if (vectNorm.y >= 0)
          m_angZ = -m_angZ;
      }
      else
        m_angZ = 0.0;
    }

    void updateValue(const OdGePoint3d& value_)
    {
      OdGePoint3d value = m_pDb->formatter().toUCS(value_);

      m_xLen = 0.001, 
      m_yLen = 0.001, 
      m_zLen = 0.001;
      m_angZ = 0.0;
      OdGeVector3d vect;
      switch (m_enState)
      {
      case enBox_FirstCorner:
        m_ptFirst = value;
        return;
      case enBox_OtherCorner:
        m_ptOther = value;
        m_ptCenter = (m_ptFirst + m_ptOther.asVector()) / 2.0;
        vect = m_ptOther - m_ptFirst;
        m_xLen = fabs(vect.x);
        m_yLen = fabs(vect.y);
        break;
      case enBox_CubeByFirst: 
        m_ptOther = value;
        vect = m_ptOther - m_ptFirst;
        updateZAngle(vect);
        m_zLen = m_yLen = m_xLen = vect.length();
        vect = - vect.normalize().crossProduct(OdGeVector3d::kZAxis) * m_yLen;
        m_ptOther += vect;
        m_ptCenter = (m_ptFirst + m_ptOther.asVector()) / 2.0;
        m_ptCenter.z = m_zLen / 2.0;
        break;
      case enBox_Length:
        m_ptOther = value;
        vect = m_ptOther - m_ptFirst;
        updateZAngle(vect);
        m_yLen = vect.length();
        m_ptCenter = (m_ptFirst + m_ptOther.asVector()) / 2.0;
        break;

      case enBox_Center:
        m_ptCenter = value;
        return;
      case enBox_Corner:
        m_ptOther = value;
        vect = 2.0 * (m_ptOther - m_ptCenter);
        m_ptFirst = m_ptOther - vect;
        m_xLen = fabs(vect.x);
        m_yLen = fabs(vect.y);
        break;
      case enBox_CubeByCenter:
        m_ptOther = value;
        vect = m_ptOther - m_ptCenter;
        updateZAngle(vect);
        m_zLen = m_yLen = m_xLen = vect.length() * 2.0;
        break;
      case enBox_LengthByCenter:
        m_ptOther = value;
        vect = m_ptOther - m_ptCenter;
        updateZAngle(vect);
        m_yLen = vect.length() * 2.0;
        break;
      default: // enBox_Width,enBox_WidthByCenter, enBox_Height, enBox_HeightByCenter
        ODA_FAIL_ONCE(); // test
        break;
     }

      if (m_xLen <= 1e-6 || m_yLen <= 1e-6 || m_zLen <= 1e-6)
        return;

      m_ent->createBox(m_xLen, m_yLen, m_zLen);
      if (!OdZero(m_angZ))
        m_ent->transformBy(OdGeMatrix3d::rotation(m_angZ, OdGeVector3d::kZAxis));
      m_ent->transformBy(OdGeMatrix3d::translation(m_ptCenter.asVector()));
      m_ent->transformBy(getUcsToWcsMatrix());
    }

    void updateValue(double value)
    {
      OdGeVector3d vect;
      switch (m_enState)
      {
      case enBox_Width:
        if ((m_xLen = fabs(value)) < 1e-6)
          m_xLen = 0.001;
        vect = m_ptOther - m_ptFirst;
        vect = - vect.normalize().crossProduct(OdGeVector3d::kZAxis) * m_xLen;
        if (value < 0)
          vect = - vect;
        m_ptCenter = (m_ptFirst + m_ptOther.asVector() + vect) / 2.0;
        break;
      case enBox_WidthByCenter:
        if ((m_xLen = 2.0 * fabs(value)) < 1e-6)
          m_xLen = 0.001;
        break;
      case enBox_Height:
        m_ptCenter.z = fabs(value) / 2.0;
        if (value < 0)
          m_ptCenter.z = - m_ptCenter.z;
      case enBox_HeightByCenter:
        if ((m_zLen = fabs(value)) < 1e-6)
          m_zLen = 0.001;
        break;
      }

      if (m_xLen <= 1e-6 || m_yLen <= 1e-6 || m_zLen <= 1e-6)
        return;

      m_ent->createBox(m_xLen, m_yLen, m_zLen);
      if (!OdZero(m_angZ))
        m_ent->transformBy(OdGeMatrix3d::rotation(m_angZ, OdGeVector3d::kZAxis));
      m_ent->transformBy(OdGeMatrix3d::translation(m_ptCenter.asVector()));
      m_ent->transformBy(getUcsToWcsMatrix());
    }

    virtual void setValue(const OdGePoint3d& value)
    {
      updateValue(value);
    }

    virtual void setValue(double value)
    {
      updateValue(value);
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

  } tracker;
  tracker.m_pDb = pDb;

  for (tracker.m_enState = enBox_FirstCorner; tracker.m_enState != enBox_Finish; )
  {
    OdGePoint3d pt;
    double dist = 0;
    try
    {
//TODO      FIX_LINUX_EXCEPTION_BEGIN

      switch (tracker.m_enState)
      {
      case enBox_FirstCorner:
        pt = pIO->getPoint(OD_T("Specify first corner or [Center]:"), 
                           OdEd::kGptDefault, 0, OD_T("Center"));
        tracker.updateValue(pt);
        pSolid = OdDb3dSolid::createObject();
        pSolid->setDatabaseDefaults(pDb);
        tracker.m_ent = pSolid;
        tracker.m_enState = enBox_OtherCorner;
        continue;
      case enBox_OtherCorner:
        pt = pIO->getPoint(OD_T("Specify other corner or [Cube/Length]:"),
                           OdEd::kGptDefault, 0, OD_T("Cube Length"), &tracker);
        tracker.updateValue(pt);
        tracker.m_enState = enBox_Height;
        continue;

      case enBox_Center:
        pt = pIO->getPoint(OD_T("Specify center:"));
        tracker.updateValue(pt);
        pSolid = OdDb3dSolid::createObject();
        pSolid->setDatabaseDefaults(pDb);
        tracker.m_ent = pSolid;
        tracker.m_enState = enBox_Corner;
        continue;
      case enBox_Corner:
        pt = pIO->getPoint(OD_T("Specify corner or [Cube/Length]:"),
                           OdEd::kGptDefault, 0, OD_T("Cube Length"), &tracker);
        tracker.updateValue(pt);
        tracker.m_enState = enBox_HeightByCenter;
        continue;

     case enBox_CubeByFirst: 
     case enBox_CubeByCenter:
        pt = pIO->getPoint(OD_T("Specify length:"),
                           OdEd::kGptDefault, 0, OdString::kEmpty, &tracker);
        tracker.updateValue(pt);
        tracker.m_enState = enBox_Finish;
        continue;

      case enBox_Length:
      case enBox_LengthByCenter:
        pt = pIO->getPoint(OD_T("Specify length:"),
                           OdEd::kGptDefault, 0, OdString::kEmpty, &tracker);
        tracker.updateValue(pt);
        tracker.m_enState = (tracker.m_enState == enBox_Length) ? enBox_Width : enBox_WidthByCenter;
        continue;

      case enBox_Width:
      case enBox_WidthByCenter:
        dist = pIO->getDist(OD_T("Specify width:"),
                            OdEd::kGdsFromLastPoint | OdEd::kGdsSignedDist, 
                            0.0, OdString::kEmpty, &tracker);
        tracker.updateValue(dist);
        tracker.m_enState = (tracker.m_enState == enBox_Width) ? enBox_Height : enBox_HeightByCenter;
        continue;

      case enBox_Height:
      case enBox_HeightByCenter:
        dist = pIO->getDist(OD_T("Specify height or [2Point]:"), 
                            OdEd::kGdsFromLastPoint | OdEd::kGdsSignedDist | OdEd::kGdsPerpDist, 
                            0.0, (tracker.m_enState == enBox_Height) ? OD_T("2Point") : OD_T(""), &tracker);
        tracker.updateValue(dist);
        tracker.m_enState = enBox_Finish;
        continue;
      case enBox_Height2Point:
        dist = pIO->getDist(OD_T("Specify first point:"), 
                            OdEd::kGdsSignedDist, 0.0, OdString::kEmpty, &tracker);
        tracker.updateValue(dist);
        tracker.m_enState = enBox_Finish;
        continue;
      }

//TODO      FIX_LINUX_EXCEPTION_END
    }
    catch (const OdEdKeyword& kw)
    {
      switch (tracker.m_enState)
      {
      case enBox_FirstCorner:
        if (!kw.keywordIndex()) // Center
        {
          tracker.m_enState = enBox_Center;
          continue;
        }
      case enBox_OtherCorner:
        switch (kw.keywordIndex())
        {
        case 0: // Cube
          tracker.m_enState = enBox_CubeByFirst;
          continue;
        case 1: // Length
          tracker.m_enState = enBox_Length;
          continue;
        }
        break;
      case enBox_Corner:
        switch (kw.keywordIndex())
        {
        case 0: // Cube
          tracker.m_enState = enBox_CubeByCenter;
          continue;
        case 1: // Length
          tracker.m_enState = enBox_LengthByCenter;
          continue;
        }
        break;
      case enBox_Height:
        if (!kw.keywordIndex()) // 2Point
        {
          tracker.m_enState = enBox_Height2Point;
          continue;
        }
      }
      
      pIO->putString(OD_T("Point or option keyword required."));
    }

  } // end while

  pMs->appendOdDbEntity(pSolid);
}
