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
/* OdDbText entities.                                                 */
/************************************************************************/
#include "StdAfx.h"
#include "DbTextGripPoints.h"
#include "RxObjectImpl.h"
#include "Ge/GeLine3d.h"

#include "AbstractViewPE.h"
#include "DbViewportTableRecord.h"
#include "OdGripPointsModule.h"

//----------------------------------------------------------------------------------------------------------------------------
// Returns GripPoints

OdResult OdDbTextGripPointsPE::getGripPoints( const OdDbEntity* pEnt, OdGePoint3dArray& gripPoints )const
{
  OdDbTextPtr pText = pEnt;

  gripPoints.append(pText->position());               // left lower corner of Text

  if (!is_Justify_Left(pText))                        // if in AutoCad properties Justify != Left
    gripPoints.append(pText->alignmentPoint());       // OdDbText has two grippoints: position() and alignmentPoint

  double dThickness = pText->thickness();
  if (!OdZero(dThickness))
  {
    OdGeVector3d vExtrusion = pText->normal() * dThickness;
    gripPoints.append(pText->position() + vExtrusion);

    if (!is_Justify_Left(pText))
      gripPoints.append(pText->alignmentPoint() + vExtrusion);
  }

  return eOk;
}
//----------------------------------------------------------------------------------------------------------------------------
// Move text

OdResult OdDbTextGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& vOffset )
{
  const unsigned indicesSize = indices.size();
  if ( indicesSize == 0 )
    return eOk;

  OdDbTextPtr pText = pEnt;
  bool bMovePosition = false;
  bool bMoveAlignmentPoint = false;

  OdGeVector3d offset(vOffset);
  // Project offset on entity's plane in view direction
  bool bPlaneChanges = !projectOffset(pText->database(), pText->normal(), offset);

  if(is_Justify_Left(pText))
  {
    bMovePosition = true;
  }
  else if (is_Justify_Aligned(pText) || is_Justify_Fit(pText))
  {
    // Both points can be moved
    if (bPlaneChanges)
    { //Move both points
      bMovePosition = true;
      bMoveAlignmentPoint = true;
    }
    else
    {
      for (unsigned i = 0; i < indicesSize; ++i)
      {
        unsigned ind = indices[i];
        switch (ind)
        {
        case 0:
        case 2:
          bMovePosition = true;
          break;
        case 1:
        case 3:
          bMoveAlignmentPoint = true;
          break;
        }
      }
    }
  }
  else
  {
    bMoveAlignmentPoint = true;
  }

  try
  {
    if (bMovePosition)
    {
      pText->setPosition(pText->position() + offset);
    }
    if (bMoveAlignmentPoint)
    {
      pText->setAlignmentPoint(pText->alignmentPoint() + offset);
    }
    pText->adjustAlignment();
  }
  catch(const OdError& e)
  {
    return e.code();
  }
  return eOk;
}


//----------------------------------------------------------------------------------------------------------------------------
// Stretched

OdResult OdDbTextGripPointsPE::getStretchPoints( const OdDbEntity* pEnt, OdGePoint3dArray& stretchPoints ) const
{
  OdDbTextPtr pText = pEnt;
  double dThickness = pText->thickness();
  OdGeVector3d vExtrusion = pText->normal() * dThickness;

  if (is_Justify_Left(pText) || is_Justify_Aligned(pText) || is_Justify_Fit(pText))
  {
    stretchPoints.append(pText->position());       // left lower corner of Text

    if (!OdZero(dThickness))
      stretchPoints.append(pText->position() + vExtrusion);
  }

  if (!is_Justify_Left(pText))
  {
    stretchPoints.append(pText->alignmentPoint()); // OdDbText (except Justify_Left) always has stretchPoint alignmentPoint

    if (!OdZero(dThickness))
      stretchPoints.append(pText->alignmentPoint() + vExtrusion);
  }
  return eOk;
}

OdResult OdDbTextGripPointsPE::moveStretchPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt(pEnt, indices, offset);
}

//----------------------------------------------------------------------------------------------------------------------------
// Return snap Points into snapPoints, depending on type sonapMode

OdResult OdDbTextGripPointsPE::getOsnapPoints(const OdDbEntity* pEnt, 
                                              OdDb::OsnapMode sonapMode,           
                                              OdGsMarker gsSelectionMark, 
                                              const OdGePoint3d& pickPoint,        // Point, which moves
                                              const OdGePoint3d& lastPoint,        // Point, from which draw line
                                              const OdGeMatrix3d& xWorldToEye, 
                                              OdGePoint3dArray& snapPoints) const
{
  OdDbTextPtr pText = pEnt;

  double dThickness = pText->thickness();
  OdGeVector3d vExtrusion = pText->normal() * dThickness;

  switch(sonapMode)
  {
    case OdDb::kOsModeNode:                  // Node: cursor as cross in a square

      if (is_Justify_Aligned(pText) || is_Justify_Fit(pText))
      {
        snapPoints.append( pText->alignmentPoint() );
        if (!OdZero(dThickness))
          snapPoints.append( pText->alignmentPoint() + vExtrusion);
      }
      else
      if (!is_Justify_Left(pText))
      {
        snapPoints.append( pText->position() );
        if (!OdZero(dThickness))
          snapPoints.append( pText->position() + vExtrusion);
      }
      break;

    case OdDb::kOsModeIns:                   // Insertion: cursor as intersection in a square

      if (is_Justify_Left(pText) || is_Justify_Aligned(pText) || is_Justify_Fit(pText))
      {
        snapPoints.append( pText->position() );
        if (!OdZero(dThickness))
          snapPoints.append( pText->position() + vExtrusion);
      }
      else
      {
        snapPoints.append( pText->alignmentPoint() );
        if (!OdZero(dThickness))
          snapPoints.append( pText->alignmentPoint() + vExtrusion);
      }
      break;

    case OdDb::kOsModeEnd:                   // Endpoint: cursor as square
    case OdDb::kOsModeCen:                   // Center: draw cross
    case OdDb::kOsModeQuad:                  // Quadrant: cursor as square turned on 45 degrees
    case OdDb::kOsModePerp:                  // Perpendicular: cursor as angle 90
    case OdDb::kOsModeNear:                  // Nearest: cursor ~ hourglasses
    case OdDb::kOsModeTan:                   // Tangent
    case OdDb::kOsModeMid:                   // Midpoint:     cursor as triangle
    case OdDb::kOsModeIntersec:              // Intersection: cursor as intersection in a square
    case OdDb::kOsModePar:                   // Parallel:
    case OdDb::kOsModeApint:                 // Apparent intersection:
      break;                                 //               isn't necessary to do

    default:
      break;
  }

  return eOk;
}
//----------------------------------------------------------------------------------------------------------------------------
// Additional service functions

bool OdDbTextGripPointsPE::is_Justify_Left(const OdDbText* pText) const
{
  return pText->horizontalMode() == OdDb::kTextLeft &&   // in AutoCad properties Justify = Left,
         pText->verticalMode()   == OdDb::kTextBase;     // text has only one gripPoint
}

bool OdDbTextGripPointsPE::is_Justify_Aligned(const OdDbText*  pText) const
{
  return pText->horizontalMode() == OdDb::kTextAlign &&  // Justify = Aligned,
         pText->verticalMode()   == OdDb::kTextBase;     // we have to do stretch text
}

bool OdDbTextGripPointsPE::is_Justify_Fit(const OdDbText*  pText) const
{
  return pText->horizontalMode() == OdDb::kTextFit &&    // Justify = Fit,
         pText->verticalMode()   == OdDb::kTextBase;     // we have to do stretch text
}
//----------------------------------------------------------------------------------------------------------------------------
