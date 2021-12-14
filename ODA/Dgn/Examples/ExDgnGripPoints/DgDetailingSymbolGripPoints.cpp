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
#include "DgDetailingSymbolGripPoints.h"
#include "DgDetailingSymbol.h"
#include "RxObjectImpl.h"
#include "Ge/GeLine3d.h"

OdResult OdDgDetailMarker2dPointsPE::getGripPoints( const OdDgElement* pEnt, OdGePoint3dArray& gripPoints )const
{
  if( pEnt )
  {
    OdDgDetailMarker2dPtr pDetailMarker2d = pEnt;

    OdArray<OdGePoint2d> arrGripPts2d;
    OdGePoint2d          ptOrigin = pDetailMarker2d->getOrigin();

    arrGripPts2d.push_back( ptOrigin );

    OdGeVector2d vrDir;

    if( pDetailMarker2d->getLeaderPointsCount() > 0 )
    {
      vrDir = pDetailMarker2d->getLeaderPoint(0) - ptOrigin;
    }
    else
    {
      vrDir = pDetailMarker2d->getBubblePoint() - ptOrigin;
    }

    if( !OdZero(vrDir.length()) )
    {
      vrDir.normalize();
    }
    else
    {
      vrDir = OdGeVector2d::kXAxis;
    }

    OdGePoint2d ptDetail = ptOrigin + vrDir * pDetailMarker2d->getDetailAreaRadius();

    arrGripPts2d.push_back( ptDetail );

    for( OdUInt32 i = 0; i < pDetailMarker2d->getLeaderPointsCount(); i++ )
    {
      arrGripPts2d.push_back( pDetailMarker2d->getLeaderPoint(i) );
    }

    OdGePoint2d          ptBubble = pDetailMarker2d->getBubblePoint();

    arrGripPts2d.push_back( ptBubble );

    for( OdUInt32 j = 0; j < arrGripPts2d.size(); j++ )
    {
      gripPoints.push_back( OdGePoint3d( arrGripPts2d[j].x, arrGripPts2d[j].y, 0) );
    }
  }
  return eOk;
}

OdResult OdDgDetailMarker2dPointsPE::moveGripPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();

  if ( size == 0 )
    return eOk;

  if( pEnt )
  {
    OdDgDetailMarker2dPtr pDetailMarker2d = pEnt;

    OdGePoint3dArray oldGripPts;
    OdGePoint3dArray newGripPts;

    getGripPoints( pEnt, oldGripPts );

    newGripPts = oldGripPts;

    for( OdUInt32 i = 0; i < indices.size(); i++ )
    {
      if( newGripPts.size() > (OdUInt32)indices[i] )
      {
        newGripPts[indices[i]] += offset;
      }
    }

    for( OdUInt32 j = 0; j < newGripPts.size(); j++ )
    {
      if( OdZero(oldGripPts[j].distanceTo(newGripPts[j]) ) )
      {
        continue;
      }

      if( j == 0 )
      {
        pDetailMarker2d->setOrigin( OdGePoint2d(newGripPts[j].x, newGripPts[j].y) );
      }
      else if( j == 1 )
      {
        pDetailMarker2d->setDetailAreaRadius( newGripPts[j].distanceTo(newGripPts[j-1] ) );
      }
      else if( j == newGripPts.size() - 1 )
      {
        pDetailMarker2d->setBubblePoint( OdGePoint2d(newGripPts[j].x, newGripPts[j].y) );
      }
      else
      {
        pDetailMarker2d->setLeaderPoint( j - 2, OdGePoint2d(newGripPts[j].x, newGripPts[j].y) );
      }
    }
  }

  return eOk;
}

OdResult OdDgDetailMarker2dPointsPE::getStretchPoints( const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  OdResult res = getGripPoints( pEnt, stretchPoints );

  if( res == eOk )
  {
    stretchPoints.resize( stretchPoints.size() - 1 );
  }

  return res;
}


OdResult OdDgDetailMarker2dPointsPE::moveStretchPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( pEnt, indices, offset );
}


OdResult OdDgDetailMarker2dPointsPE::getOsnapPoints(const OdDgElement* pEnt, 
                                                      OdDgElement::OsnapMode osnapMode, 
                                                        OdGsMarker gsSelectionMark, 
                                                          const OdGePoint3d& pickPoint,
                                                            const OdGePoint3d& lastPoint, 
                                                              const OdGeMatrix3d& xWorldToEye, 
                                                                OdGePoint3dArray& snapPoints
                                                   ) const
{
  return eOk;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

OdResult OdDgDetailMarker3dPointsPE::getGripPoints( const OdDgElement* pEnt, OdGePoint3dArray& gripPoints )const
{
  if( pEnt )
  {
    OdDgDetailMarker3dPtr pDetailMarker3d = pEnt;

    OdArray<OdGePoint3d> arrGripPts3d;
    OdGePoint3d          ptOrigin = pDetailMarker3d->getOrigin();

    arrGripPts3d.push_back( ptOrigin );

    OdGeVector3d vrDir;

    if( pDetailMarker3d->getLeaderPointsCount() > 0 )
    {
      vrDir = pDetailMarker3d->getLeaderPoint(0) - ptOrigin;
    }
    else
    {
      vrDir = pDetailMarker3d->getBubblePoint() - ptOrigin;
    }

    if( !OdZero(vrDir.length()) )
    {
      vrDir.normalize();
    }
    else
    {
      vrDir = OdGeVector3d::kXAxis;
    }

    OdGePoint3d ptDetail = ptOrigin + vrDir * pDetailMarker3d->getDetailAreaRadius();

    arrGripPts3d.push_back( ptDetail );

    for( OdUInt32 i = 0; i < pDetailMarker3d->getLeaderPointsCount(); i++ )
    {
      arrGripPts3d.push_back( pDetailMarker3d->getLeaderPoint(i) );
    }

    OdGePoint3d          ptBubble = pDetailMarker3d->getBubblePoint();

    arrGripPts3d.push_back( ptBubble );

    for( OdUInt32 j = 0; j < arrGripPts3d.size(); j++ )
    {
      gripPoints.push_back( arrGripPts3d[j] );
    }
  }

  return eOk;
}

OdResult OdDgDetailMarker3dPointsPE::moveGripPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();

  if ( size == 0 )
    return eOk;

  if( pEnt )
  {
    OdDgDetailMarker3dPtr pDetailMarker3d = pEnt;

    OdGePoint3dArray oldGripPts;
    OdGePoint3dArray newGripPts;

    getGripPoints( pEnt, oldGripPts );

    newGripPts = oldGripPts;

    for( OdUInt32 i = 0; i < indices.size(); i++ )
    {
      if( newGripPts.size() > (OdUInt32)indices[i] )
      {
        newGripPts[indices[i]] += offset;
      }
    }

    for( OdUInt32 j = 0; j < newGripPts.size(); j++ )
    {
      if( OdZero(oldGripPts[j].distanceTo(newGripPts[j]) ) )
      {
        continue;
      }

      if( j == 0 )
      {
        pDetailMarker3d->setOrigin( newGripPts[j] );
      }
      else if( j == 1 )
      {
        pDetailMarker3d->setDetailAreaRadius( newGripPts[j].distanceTo(newGripPts[j-1] ) );
      }
      else if( j == newGripPts.size() - 1 )
      {
        pDetailMarker3d->setBubblePoint( newGripPts[j] );
      }
      else
      {
        pDetailMarker3d->setLeaderPoint( j - 2, newGripPts[j] );
      }
    }
  }

  return eOk;
}

OdResult OdDgDetailMarker3dPointsPE::getStretchPoints( const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  OdResult res = getGripPoints( pEnt, stretchPoints );

  if( res == eOk )
  {
    stretchPoints.resize( stretchPoints.size() - 1 );
  }

  return res;
}


OdResult OdDgDetailMarker3dPointsPE::moveStretchPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( pEnt, indices, offset );
}


OdResult OdDgDetailMarker3dPointsPE::getOsnapPoints(const OdDgElement* pEnt, 
                                                    OdDgElement::OsnapMode osnapMode, 
                                                    OdGsMarker gsSelectionMark, 
                                                    const OdGePoint3d& pickPoint,
                                                    const OdGePoint3d& lastPoint, 
                                                    const OdGeMatrix3d& xWorldToEye, 
                                                    OdGePoint3dArray& snapPoints
                                                    ) const
{
  return eOk;
}

/////////////////////////////////////////////////////////////////////////////////////////

OdResult OdDgArrowMarker2dPointsPE::getGripPoints( const OdDgElement* pEnt, OdGePoint3dArray& gripPoints )const
{
  if( pEnt )
  {
    OdDgArrowMarker2dPtr pArrowMarker2d = pEnt;

    OdArray<OdGePoint2d> arrGripPts2d;
    OdGePoint2d          ptOrigin = pArrowMarker2d->getOrigin();

    arrGripPts2d.push_back( ptOrigin );

    OdGeVector2d vrDir;

    for( OdUInt32 i = 0; i < pArrowMarker2d->getLeaderPointsCount(); i++ )
    {
      arrGripPts2d.push_back( pArrowMarker2d->getLeaderPoint(i) );
    }

    for( OdUInt32 j = 0; j < arrGripPts2d.size(); j++ )
    {
      gripPoints.push_back( OdGePoint3d( arrGripPts2d[j].x, arrGripPts2d[j].y, 0) );
    }
  }
  return eOk;
}

OdResult OdDgArrowMarker2dPointsPE::moveGripPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();

  if ( size == 0 )
    return eOk;

  if( pEnt )
  {
    OdDgArrowMarker2dPtr pArrowMarker2d = pEnt;

    OdGePoint3dArray oldGripPts;
    OdGePoint3dArray newGripPts;

    getGripPoints( pEnt, oldGripPts );

    newGripPts = oldGripPts;

    for( OdUInt32 i = 0; i < indices.size(); i++ )
    {
      if( newGripPts.size() > (OdUInt32)indices[i] )
      {
        newGripPts[indices[i]] += offset;
      }
    }

    for( OdUInt32 j = 0; j < newGripPts.size(); j++ )
    {
      if( OdZero(oldGripPts[j].distanceTo(newGripPts[j]) ) )
      {
        continue;
      }

      if( j == 0 )
      {
        pArrowMarker2d->setOrigin( OdGePoint2d(newGripPts[j].x, newGripPts[j].y) );
      }
      else
      {
        pArrowMarker2d->setLeaderPoint( j - 1, OdGePoint2d(newGripPts[j].x, newGripPts[j].y) );
      }
    }
  }

  return eOk;
}

OdResult OdDgArrowMarker2dPointsPE::getStretchPoints( const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  OdResult res = getGripPoints( pEnt, stretchPoints );

  if( res == eOk )
  {
    stretchPoints.resize( stretchPoints.size() - 1 );
  }

  return res;
}


OdResult OdDgArrowMarker2dPointsPE::moveStretchPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( pEnt, indices, offset );
}


OdResult OdDgArrowMarker2dPointsPE::getOsnapPoints(const OdDgElement* pEnt, 
                                                    OdDgElement::OsnapMode osnapMode, 
                                                    OdGsMarker gsSelectionMark, 
                                                    const OdGePoint3d& pickPoint,
                                                    const OdGePoint3d& lastPoint, 
                                                    const OdGeMatrix3d& xWorldToEye, 
                                                    OdGePoint3dArray& snapPoints
                                                    ) const
{
  return eOk;
}

/////////////////////////////////////////////////////////////////////////////////////////

OdResult OdDgArrowMarker3dPointsPE::getGripPoints( const OdDgElement* pEnt, OdGePoint3dArray& gripPoints )const
{
  if( pEnt )
  {
    OdDgArrowMarker3dPtr pArrowMarker3d = pEnt;

    OdArray<OdGePoint3d> arrGripPts3d;
    OdGePoint3d          ptOrigin = pArrowMarker3d->getOrigin();

    gripPoints.push_back( ptOrigin );

    OdGeVector2d vrDir;

    for( OdUInt32 i = 0; i < pArrowMarker3d->getLeaderPointsCount(); i++ )
    {
      gripPoints.push_back( pArrowMarker3d->getLeaderPoint(i) );
    }
  }

  return eOk;
}

OdResult OdDgArrowMarker3dPointsPE::moveGripPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();

  if ( size == 0 )
    return eOk;

  if( pEnt )
  {
    OdDgArrowMarker3dPtr pArrowMarker3d = pEnt;

    OdGePoint3dArray oldGripPts;
    OdGePoint3dArray newGripPts;

    getGripPoints( pEnt, oldGripPts );

    newGripPts = oldGripPts;

    for( OdUInt32 i = 0; i < indices.size(); i++ )
    {
      if( newGripPts.size() > (OdUInt32)indices[i] )
      {
        newGripPts[indices[i]] += offset;
      }
    }

    for( OdUInt32 j = 0; j < newGripPts.size(); j++ )
    {
      if( OdZero(oldGripPts[j].distanceTo(newGripPts[j]) ) )
      {
        continue;
      }

      if( j == 0 )
      {
        pArrowMarker3d->setOrigin( newGripPts[j] );
      }
      else
      {
        pArrowMarker3d->setLeaderPoint( j - 1, newGripPts[j] );
      }
    }
  }

  return eOk;
}

OdResult OdDgArrowMarker3dPointsPE::getStretchPoints( const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  OdResult res = getGripPoints( pEnt, stretchPoints );

  if( res == eOk )
  {
    stretchPoints.resize( stretchPoints.size() - 1 );
  }

  return res;
}


OdResult OdDgArrowMarker3dPointsPE::moveStretchPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( pEnt, indices, offset );
}


OdResult OdDgArrowMarker3dPointsPE::getOsnapPoints(const OdDgElement* pEnt, 
                                                   OdDgElement::OsnapMode osnapMode, 
                                                   OdGsMarker gsSelectionMark, 
                                                   const OdGePoint3d& pickPoint,
                                                   const OdGePoint3d& lastPoint, 
                                                   const OdGeMatrix3d& xWorldToEye, 
                                                   OdGePoint3dArray& snapPoints
                                                   ) const
{
  return eOk;
}

/////////////////////////////////////////////////////////////////////////////////////////

OdResult OdDgSectionMarker2dPointsPE::getGripPoints( const OdDgElement* pEnt, OdGePoint3dArray& gripPoints )const
{
  if( pEnt )
  {
    OdDgSectionMarker2dPtr pSectionMarker2d = pEnt;

    OdGePoint2d          ptOrigin = pSectionMarker2d->getOrigin();

    gripPoints.push_back( OdGePoint3d(ptOrigin.x, ptOrigin.y,0) );
  }

  return eOk;
}

OdResult OdDgSectionMarker2dPointsPE::moveGripPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();

  if ( size == 0 )
    return eOk;

  if( pEnt )
  {
    OdDgSectionMarker2dPtr pSectionMarker2d = pEnt;

    OdGePoint2d          ptOrigin = pSectionMarker2d->getOrigin();

    OdGePoint3d          ptOrigin3d( ptOrigin.x, ptOrigin.y, 0 );

    ptOrigin3d = ptOrigin3d + offset;

    pSectionMarker2d->setOrigin( OdGePoint2d( ptOrigin3d.x, ptOrigin3d.y ) );
  }

  return eOk;
}

OdResult OdDgSectionMarker2dPointsPE::getStretchPoints( const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  OdResult res = getGripPoints( pEnt, stretchPoints );

  if( res == eOk )
  {
    stretchPoints.resize( stretchPoints.size() - 1 );
  }

  return res;
}


OdResult OdDgSectionMarker2dPointsPE::moveStretchPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( pEnt, indices, offset );
}


OdResult OdDgSectionMarker2dPointsPE::getOsnapPoints(const OdDgElement* pEnt, 
                                                   OdDgElement::OsnapMode osnapMode, 
                                                   OdGsMarker gsSelectionMark, 
                                                   const OdGePoint3d& pickPoint,
                                                   const OdGePoint3d& lastPoint, 
                                                   const OdGeMatrix3d& xWorldToEye, 
                                                   OdGePoint3dArray& snapPoints
                                                   ) const
{
  return eOk;
}

/////////////////////////////////////////////////////////////////////////////////////////

OdResult OdDgSectionMarker3dPointsPE::getGripPoints( const OdDgElement* pEnt, OdGePoint3dArray& gripPoints )const
{
  if( pEnt )
  {
    OdDgSectionMarker3dPtr pSectionMarker3d = pEnt;

    OdGePoint3d          ptOrigin = pSectionMarker3d->getOrigin();

    gripPoints.push_back( ptOrigin );
  }

  return eOk;
}

OdResult OdDgSectionMarker3dPointsPE::moveGripPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();

  if ( size == 0 )
    return eOk;

  if( pEnt )
  {
    OdDgSectionMarker3dPtr pSectionMarker3d = pEnt;

    OdGePoint3d          ptOrigin3d = pSectionMarker3d->getOrigin();

    ptOrigin3d = ptOrigin3d + offset;

    pSectionMarker3d->setOrigin( ptOrigin3d );
  }

  return eOk;
}

OdResult OdDgSectionMarker3dPointsPE::getStretchPoints( const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  OdResult res = getGripPoints( pEnt, stretchPoints );

  if( res == eOk )
  {
    stretchPoints.resize( stretchPoints.size() - 1 );
  }

  return res;
}


OdResult OdDgSectionMarker3dPointsPE::moveStretchPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( pEnt, indices, offset );
}


OdResult OdDgSectionMarker3dPointsPE::getOsnapPoints(const OdDgElement* pEnt, 
                                                     OdDgElement::OsnapMode osnapMode, 
                                                     OdGsMarker gsSelectionMark, 
                                                     const OdGePoint3d& pickPoint,
                                                     const OdGePoint3d& lastPoint, 
                                                     const OdGeMatrix3d& xWorldToEye, 
                                                     OdGePoint3dArray& snapPoints
                                                     ) const
{
  return eOk;
}

/////////////////////////////////////////////////////////////////////////////////////////

OdResult OdDgTitleText2dPointsPE::getGripPoints( const OdDgElement* pEnt, OdGePoint3dArray& gripPoints )const
{
  if( pEnt )
  {
    OdDgTitleText2dPtr pTitleText2d = pEnt;

    OdGePoint2d          ptOrigin = pTitleText2d->getOrigin();

    gripPoints.push_back( OdGePoint3d(ptOrigin.x, ptOrigin.y,0) );
  }

  return eOk;
}

OdResult OdDgTitleText2dPointsPE::moveGripPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();

  if ( size == 0 )
    return eOk;

  if( pEnt )
  {
    OdDgTitleText2dPtr pTitleText2d = pEnt;

    OdGePoint2d          ptOrigin = pTitleText2d->getOrigin();

    OdGePoint3d          ptOrigin3d( ptOrigin.x, ptOrigin.y, 0 );

    ptOrigin3d = ptOrigin3d + offset;

    pTitleText2d->setOrigin( OdGePoint2d( ptOrigin3d.x, ptOrigin3d.y ) );
  }

  return eOk;
}

OdResult OdDgTitleText2dPointsPE::getStretchPoints( const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  OdResult res = getGripPoints( pEnt, stretchPoints );

  if( res == eOk )
  {
    stretchPoints.resize( stretchPoints.size() - 1 );
  }

  return res;
}


OdResult OdDgTitleText2dPointsPE::moveStretchPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( pEnt, indices, offset );
}


OdResult OdDgTitleText2dPointsPE::getOsnapPoints(const OdDgElement* pEnt, 
                                                     OdDgElement::OsnapMode osnapMode, 
                                                     OdGsMarker gsSelectionMark, 
                                                     const OdGePoint3d& pickPoint,
                                                     const OdGePoint3d& lastPoint, 
                                                     const OdGeMatrix3d& xWorldToEye, 
                                                     OdGePoint3dArray& snapPoints
                                                     ) const
{
  return eOk;
}

/////////////////////////////////////////////////////////////////////////////////////////

OdResult OdDgTitleText3dPointsPE::getGripPoints( const OdDgElement* pEnt, OdGePoint3dArray& gripPoints )const
{
  if( pEnt )
  {
    OdDgTitleText3dPtr pTitleText3d = pEnt;

    OdGePoint3d          ptOrigin = pTitleText3d->getOrigin();

    gripPoints.push_back( ptOrigin );
  }

  return eOk;
}

OdResult OdDgTitleText3dPointsPE::moveGripPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();

  if ( size == 0 )
    return eOk;

  if( pEnt )
  {
    OdDgTitleText3dPtr pTitleText3d = pEnt;

    OdGePoint3d          ptOrigin3d = pTitleText3d->getOrigin();

    ptOrigin3d = ptOrigin3d + offset;

    pTitleText3d->setOrigin( ptOrigin3d );
  }

  return eOk;
}

OdResult OdDgTitleText3dPointsPE::getStretchPoints( const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  OdResult res = getGripPoints( pEnt, stretchPoints );

  if( res == eOk )
  {
    stretchPoints.resize( stretchPoints.size() - 1 );
  }

  return res;
}


OdResult OdDgTitleText3dPointsPE::moveStretchPointsAt( OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( pEnt, indices, offset );
}


OdResult OdDgTitleText3dPointsPE::getOsnapPoints(const OdDgElement* pEnt, 
                                                     OdDgElement::OsnapMode osnapMode, 
                                                     OdGsMarker gsSelectionMark, 
                                                     const OdGePoint3d& pickPoint,
                                                     const OdGePoint3d& lastPoint, 
                                                     const OdGeMatrix3d& xWorldToEye, 
                                                     OdGePoint3dArray& snapPoints
                                                     ) const
{
  return eOk;
}
