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

#ifndef _ODDBDIMGRIPPOINTS_INCLUDED
#define _ODDBDIMGRIPPOINTS_INCLUDED

#include "DbGripPoints.h"
#include "DbDimension.h"

/************************************************************************/
/* This class is an implementation of the OdDbGripPointsPE class for    */
/* entities derived from OdDbDimension.                                 */
/************************************************************************/
class OdDbDimGripPointsPE : public OdDbGripPointsPE
{
protected:
    const bool isAngleDirectionBetweenVectors( OdGeVector3d v1, OdGeVector3d v2 );

public:
  virtual OdResult getGripPoints( const OdDbEntity* /*pEntity*/, OdGePoint3dArray& /*gripPoints*/ ) const
  {
      return eOk;
  };
  OdResult moveGripPointsAt( OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset )
  {
      if ( indices.empty() ) return eOk;
      try
      {
        OdGePoint3dArray gripPoints;
        getGripPoints(pEntity, gripPoints);
        for (unsigned i = 0; i < indices.size(); ++i)
        {
          if (indices[i] < (int)gripPoints.size())
            gripPoints[indices[i]] += offset;
        }

         moveGripPoint( pEntity, gripPoints, indices, false );
         OdDbDimensionPtr pDimPtr = pEntity;
         pDimPtr->recomputeDimBlock();
      }
      catch(const OdError& e)
      {
          return e.code();
      }
      return eOk;
  }
  virtual OdResult moveGripPoint(OdDbEntity* /*pEntity*/, const OdGePoint3dArray& /*gripPoints*/, const OdIntArray& /*indices */, bool bStretch) = 0;
  virtual OdResult getStretchPoints( const OdDbEntity* pEntity, OdGePoint3dArray& stretchPoints ) const;
  virtual OdResult moveStretchPointsAt( OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset );
  virtual OdResult getOsnapPoints( 
    const OdDbEntity* pEntity, 
    OdDb::OsnapMode osnapMode, 
    OdGsMarker gsSelectionMark, 
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint, 
    const OdGeMatrix3d& xWorldToEye, 
    OdGePoint3dArray& snapPoints) const;
};

/************************************************************************/
/* This class is an specialization of the OdDbGripPointsPE class for    */
/* OdDbRotatedDimension entities.                                       */
/************************************************************************/
class OdDbRotatedDimGripPointsPE : public OdDbDimGripPointsPE
{
public:
  virtual OdResult getGripPoints( const OdDbEntity* pEntity, OdGePoint3dArray& gripPoints ) const;
  virtual OdResult moveGripPoint( OdDbEntity* pEntity, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch);
//  virtual OdResult moveGripPointsAt( OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset );

};

/************************************************************************/
/* This class is an specialization of the OdDbGripPointsPE class for    */
/* OdDbAlignedDimension entities.                                       */
/************************************************************************/
class OdDbAlignedDimGripPointsPE : public OdDbDimGripPointsPE
{
  virtual OdResult getGripPoints( const OdDbEntity* pEntity, OdGePoint3dArray& gripPoints ) const;
  virtual OdResult moveGripPoint( OdDbEntity* pEntity, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch);
//  virtual OdResult moveGripPointsAt( OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset );
};


/************************************************************************/
/* This class is an specialization of the OdDbGripPointsPE class for    */
/* OdDbRadialDimension entities.                                        */
/************************************************************************/
class OdDbRadialDimGripPointsPE : public OdDbDimGripPointsPE
{
public:
  virtual OdResult getGripPoints( const OdDbEntity* pEntity, OdGePoint3dArray& gripPoints ) const;
  virtual OdResult moveGripPoint( OdDbEntity* pEntity, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch);
//virtual OdResult moveGripPointsAt( OdDbEntity* pEntity, const OdGePoint3dArray& gripPoints, const OdIntArray& indices );
};



/** Description:
  This class is an specialization of the OdDbDimGripPointsPE class for OdDbDiametricDimension entities.
*/
/************************************************************************/
/* This class is an specialization of the OdDbGripPointsPE class for    */
/* OdDbDiametricDimension entities.                                     */
/************************************************************************/
class OdDbDiametricDimGripPointsPE : public OdDbDimGripPointsPE
{
public:
  virtual OdResult getGripPoints( const OdDbEntity* pEntity, OdGePoint3dArray& gripPoints ) const;
  virtual OdResult moveGripPoint( OdDbEntity* pEntity, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch);
//  virtual OdResult moveGripPointsAt( OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset );
};

/************************************************************************/
/* This class is an specialization of the OdDbGripPointsPE class for    */
/* OdDbAngularDimension entities.                                       */
/************************************************************************/
class OdDb3PointAngularDimGripPointsPE : public OdDbDimGripPointsPE
{
public:
  virtual OdResult getGripPoints( const OdDbEntity* pEntity, OdGePoint3dArray& gripPoints ) const;
  virtual OdResult moveGripPoint( OdDbEntity* pEntity, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch);
//  virtual OdResult moveGripPointsAt( OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset );
};


/************************************************************************/
/* This class is an specialization of the OdDbGripPointsPE class for    */
/* OdDbOrdinateDimension entities.                                      */
/************************************************************************/
class OdDbOrdinateDimGripPointsPE : public OdDbDimGripPointsPE
{
public:
  virtual OdResult getGripPoints( const OdDbEntity* pEntity, OdGePoint3dArray& gripPoints ) const;
  virtual OdResult moveGripPoint( OdDbEntity* pEntity, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch);
//  virtual OdResult moveGripPointsAt( OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset );
};


/************************************************************************/
/* This class is an specialization of the OdDbGripPointsPE class for    */
/* OdDb2LineAngularDimension entities.                                  */
/************************************************************************/
class OdDb2LineAngularDimGripPointsPE : public OdDbDimGripPointsPE
{
public:
  virtual OdResult getGripPoints( const OdDbEntity* pEntity, OdGePoint3dArray& gripPoints ) const;
  virtual OdResult moveGripPoint( OdDbEntity* pEntity, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch);
//  virtual OdResult moveGripPointsAt( OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset );
};


/************************************************************************/
/* This class is an specialization of the OdDbGripPointsPE class for    */
/* OdDbArcDimension entities.                                           */
/************************************************************************/
class OdDbArcDimGripPointsPE : public OdDbDimGripPointsPE
{
public:
  virtual OdResult getGripPoints( const OdDbEntity* pEntity, OdGePoint3dArray& gripPoints ) const;
  virtual OdResult moveGripPoint( OdDbEntity* pEntity, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch);
//  virtual OdResult moveGripPointsAt( OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset );
};

/************************************************************************/
/* This class is an specialization of the OdDbGripPointsPE class for    */
/* OdDbRadialDimensionLarge entities.                                   */
/************************************************************************/
class OdDbRadialDimLargeGripPointsPE : public OdDbDimGripPointsPE
{
public:
  virtual OdResult getGripPoints( const OdDbEntity* pEntity, OdGePoint3dArray& gripPoints ) const;
  virtual OdResult moveGripPoint( OdDbEntity* pEntity, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch);
//  virtual OdResult moveGripPointsAt( OdDbEntity* pEntity, const OdIntArray& indices, const OdGeVector3d& offset );
};


#endif //_ODDBDIMGRIPPOINTS_INCLUDED
