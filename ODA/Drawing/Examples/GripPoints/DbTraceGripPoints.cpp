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

#include "StdAfx.h"
#include "DbTrace.h"
#include "DbSolid.h"
#include "DbTraceGripPoints.h"

class _DbTraceGPRedir
{
  public:
    virtual void getPointAt(int nPt, OdGePoint3d &pPt) const = 0;
    virtual void setPointAt(int nPt, const OdGePoint3d &pPt) = 0;
    virtual double thickness() const = 0;
    virtual OdGeVector3d normal() const = 0;
};

template <class classPtr>
class _DbTraceGPRedirImpl : public _DbTraceGPRedir
{
  classPtr pTrace;
  public:
    _DbTraceGPRedirImpl(const OdDbEntity *pEnt)
      : pTrace(pEnt)
    { }
    _DbTraceGPRedirImpl(OdDbEntity *pEnt)
      : pTrace(pEnt)
    { }

    void getPointAt(int nPt, OdGePoint3d &pPt) const
    {
      pTrace->getPointAt(nPt, pPt);
    }
    void setPointAt(int nPt, const OdGePoint3d &pPt)
    {
      pTrace->setPointAt(nPt, pPt);
    }
    double thickness() const
    {
      return pTrace->thickness();
    }
    OdGeVector3d normal() const
    {
      return pTrace->normal();
    }
};

OdResult OdDbTraceGripPointsPE_Base::getGripPoints( const _DbTraceGPRedir* pTrace, OdGePoint3dArray& gripPoints ) const
{
  gripPoints.resize(4);
  int nPt;
  for (nPt = 0; nPt < 4; nPt++)
    pTrace->getPointAt(nPt, gripPoints[nPt]);
  if (OdNonZero(pTrace->thickness()))
  {
    OdGeVector3d thkPath(pTrace->normal().normal() * pTrace->thickness());
    gripPoints.resize(8);
    for (nPt = 4; nPt < 8; nPt++)
      gripPoints[nPt] = gripPoints[nPt - 4] + thkPath;
  }
  return eOk;
}

OdResult OdDbTraceGripPointsPE_Base::moveGripPointsAt( _DbTraceGPRedir* pTrace, const OdIntArray& indices, const OdGeVector3d& offset )
{
  OdGeVector3d planarBasis[3];
  double planarCoefs[3]; int nPt;
  planarBasis[2] = pTrace->normal();
  planarBasis[0] = planarBasis[2].perpVector().normal();
  planarBasis[1] = planarBasis[2].crossProduct(planarBasis[0]).normal();
  for (nPt = 0; nPt < 3; nPt++)
    planarCoefs[nPt] = planarBasis[nPt].dotProduct(offset);
  if (OdNonZero(planarCoefs[0]) || OdNonZero(planarCoefs[1]))
  {
    bool bMarkIds[4] = { false, false, false, false };
    OdGeVector3d delta = planarBasis[0] * planarCoefs[0] + planarBasis[1] * planarCoefs[1];
    for (nPt = 0; nPt < (int)indices.size(); nPt++)
      bMarkIds[indices[nPt] % 4] = true;
    for (nPt = 0; nPt < 4; nPt++)
    {
      if (bMarkIds[nPt])
      {
        OdGePoint3d movePt;
        pTrace->getPointAt(nPt, movePt);
        movePt += delta;
        pTrace->setPointAt(nPt, movePt);
      }
    }
  }
  if (OdNonZero(planarCoefs[2]))
  {
    OdGeVector3d delta = planarBasis[2] * planarCoefs[2];
    for (nPt = 0; nPt < 4; nPt++)
    {
      OdGePoint3d movePt;
      pTrace->getPointAt(nPt, movePt);
      movePt += delta;
      pTrace->setPointAt(nPt, movePt);
    }
  }
  return eOk;
}

OdResult OdDbTraceGripPointsPE_Base::getStretchPoints( const _DbTraceGPRedir* pTrace, OdGePoint3dArray& stretchPoints ) const
{
  return getGripPoints( pTrace, stretchPoints );
}

OdResult OdDbTraceGripPointsPE_Base::moveStretchPointsAt( _DbTraceGPRedir* pTrace, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( pTrace, indices, offset );
}

OdResult OdDbTraceGripPointsPE_Base::getOsnapPoints(  const _DbTraceGPRedir* pTrace,
                                                      OdDb::OsnapMode osnapMode,
                                                      OdGsMarker /*gsSelectionMark*/,
                                                      const OdGePoint3d& /*pickPoint*/,
                                                      const OdGePoint3d& /*lastPoint*/,
                                                      const OdGeMatrix3d& /*xWorldToEye*/,
                                                      OdGePoint3dArray& snapPoints  ) const
{
  switch (osnapMode)
  {
    case OdDb::kOsModeEnd:
      {
        OdUInt32 nSnaps = snapPoints.size();
        snapPoints.resize(nSnaps + 4);
        for (int nPt = 0; nPt < 4; nPt++)
          pTrace->getPointAt(nPt, snapPoints[nSnaps + nPt]);
      }
    break;
    default:
      break;
  }
  return eOk;
}

// TracePE

OdResult OdDbTraceGripPointsPE::getGripPoints( const OdDbEntity* pEnt, OdGePoint3dArray& gripPoints ) const
{
  _DbTraceGPRedirImpl<OdDbTracePtr> redir(pEnt);
  return OdDbTraceGripPointsPE_Base::getGripPoints(&redir, gripPoints);
}

OdResult OdDbTraceGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  _DbTraceGPRedirImpl<OdDbTracePtr> redir(pEnt);
  return OdDbTraceGripPointsPE_Base::moveGripPointsAt(&redir, indices, offset);
}

OdResult OdDbTraceGripPointsPE::getStretchPoints( const OdDbEntity* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  _DbTraceGPRedirImpl<OdDbTracePtr> redir(pEnt);
  return OdDbTraceGripPointsPE_Base::getStretchPoints(&redir, stretchPoints);
}

OdResult OdDbTraceGripPointsPE::moveStretchPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  _DbTraceGPRedirImpl<OdDbTracePtr> redir(pEnt);
  return OdDbTraceGripPointsPE_Base::moveStretchPointsAt(&redir, indices, offset);
}

OdResult OdDbTraceGripPointsPE::getOsnapPoints(const OdDbEntity* pEnt, 
                                               OdDb::OsnapMode osnapMode, 
                                               OdGsMarker gsSelectionMark, 
                                               const OdGePoint3d& pickPoint,
                                               const OdGePoint3d& lastPoint,
                                               const OdGeMatrix3d& viewXform,
                                               OdGePoint3dArray& snapPoints ) const
{
  _DbTraceGPRedirImpl<OdDbTracePtr> redir(pEnt);
  return OdDbTraceGripPointsPE_Base::getOsnapPoints(&redir, osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints);
}

// SolidPE

OdResult OdDbSolidGripPointsPE::getGripPoints( const OdDbEntity* pEnt, OdGePoint3dArray& gripPoints ) const
{
  _DbTraceGPRedirImpl<OdDbSolidPtr> redir(pEnt);
  return OdDbTraceGripPointsPE_Base::getGripPoints(&redir, gripPoints);
}

OdResult OdDbSolidGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  _DbTraceGPRedirImpl<OdDbSolidPtr> redir(pEnt);
  return OdDbTraceGripPointsPE_Base::moveGripPointsAt(&redir, indices, offset);
}

OdResult OdDbSolidGripPointsPE::getStretchPoints( const OdDbEntity* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  _DbTraceGPRedirImpl<OdDbSolidPtr> redir(pEnt);
  return OdDbTraceGripPointsPE_Base::getStretchPoints(&redir, stretchPoints);
}

OdResult OdDbSolidGripPointsPE::moveStretchPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  _DbTraceGPRedirImpl<OdDbSolidPtr> redir(pEnt);
  return OdDbTraceGripPointsPE_Base::moveStretchPointsAt(&redir, indices, offset);
}

OdResult OdDbSolidGripPointsPE::getOsnapPoints(const OdDbEntity* pEnt, 
                                               OdDb::OsnapMode osnapMode, 
                                               OdGsMarker gsSelectionMark, 
                                               const OdGePoint3d& pickPoint,
                                               const OdGePoint3d& lastPoint,
                                               const OdGeMatrix3d& viewXform,
                                               OdGePoint3dArray& snapPoints ) const
{
  _DbTraceGPRedirImpl<OdDbSolidPtr> redir(pEnt);
  return OdDbTraceGripPointsPE_Base::getOsnapPoints(&redir, osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints);
}
