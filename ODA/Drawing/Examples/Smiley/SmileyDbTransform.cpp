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

#include "OdaCommon.h"
#include "DbProxyEntity.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeLineSeg3d.h"    // need for OdGeLineSeg3d class
#include "SmileyDb.h"
#include "DbCircle.h"
#include "DbArc.h"
#include "DbGrip.h"
#include "DbPolyline.h"

#define FACE_CENTER_GRIP_PT 1
#define FACE_RIGHT_GRIP_PT  2
#define FACE_TOP_GRIP_PT	3
#define FACE_LEFT_GRIP_PT	4
#define FACE_BOTTOM_GRIP_PT	5

#define EYE_CENTER_GRIP_PT  1
#define EYE_RIGHT_GRIP_PT   2
#define EYE_TOP_GRIP_PT		3
#define EYE_LEFT_GRIP_PT	4
#define EYE_BOTTOM_GRIP_PT	5

#define MOUTH_LEFT_GRIP_PT		1
#define MOUTH_RIGHT_GRIP_PT		2
#define MOUTH_MID_GRIP_PT		3
#define MOUTH_BOTTOM_GRIP_PT	4

#define LINE_LEFT_GRIP_PT		1
#define LINE_RIGHT_GRIP_PT		2

//
// Step 3
// We are implementing 'transformBy' function to move, to scale and  
// to rotate the smiley object. To stretch and constrict of smiley, 
// we have implemented the 'scaleRadius' function. This function will
// being used in following steps.
//
OdResult AsdkSmiley::subTransformBy(const OdGeMatrix3d& xform)
{
    assertWriteEnabled();

    // Transform the center point and get the translation vector
    OdGePoint3d oldCenter( center() ),
                newCenter( center() );
    newCenter.transformBy( xform );
    OdGeVector3d transVec = newCenter - center();

    // Get the equivalent transformation
    OdGeMatrix3d newXform;
    newXform.setToTranslation( transVec );

    // Only translate the face and mouth - do not transform!
    mfacecircle.transformBy( newXform );
    mmoutharc.transformBy( newXform );

    // Get the point at a quadrant, transform it..
    OdGePoint3d oldXquad = center() + OdGeVector3d( radius(), 0, 0 ),
                newXquad( oldXquad );
    newXquad.transformBy( xform );

    // ... then scale the Smiley accordingly
    if ( false == xform.isEqualTo( OdGeMatrix3d::kIdentity ))
        scaleRadius( radius() * newCenter.distanceTo( newXquad ) / oldCenter.distanceTo( oldXquad ));

    xDataTransformBy(xform);

    return eOk;
}

void AsdkSmiley::scaleRadius(const double r)     // stretch and constrict of smiley
{
    assertWriteEnabled();
    OdGePoint3d cen( center() );
    double rad = radius(),
           factor = r / rad;
    setEyesApart( factor * eyesApart() );
    setEyesHeight( factor * eyesHeight() );
    setEyeSize( factor * eyeSize() );
    setMouth( mouthLeft().scaleBy( factor, cen ), 
		      mouthBottom().scaleBy( factor, cen ), 
			  mouthRight().scaleBy( factor, cen ) );
    setRadius( r );
    recordGraphicsModified();
}
//
// Step 4
// We are implementing 'getGripPoints' function to create the array of 
// grip points for smiley object and are implementing 'moveGripPointsAt' 
// function to stretch and to move the face, the mouth and eyes of 
// smiley object using a grip points. To control the size of smiley at 
// changes of its elements, we have implemented the 'ensureRadius' functions.
//
OdResult AsdkSmiley::subGetGripPoints(OdGePoint3dArray& gripPoints) const
{
    assertReadEnabled();
    //										indices
	// Grip points to face
    OdGePoint3d cen( center() );
    OdGeVector3d xoff( radius(), 0, 0 ),
                 yoff( 0, radius(), 0 );	
    gripPoints.append( cen );			// 0
    gripPoints.append( cen + xoff );		// 1
    gripPoints.append( cen + yoff );		// 2
    gripPoints.append( cen - xoff );		// 3
    gripPoints.append( cen - yoff );		// 4
	// Grip points to mouth
    gripPoints.append( mouthLeft() );		// 5
    gripPoints.append( mouthRight() );		// 6
    gripPoints.append( mouthBottom() );		// 7
    gripPoints.append( OdGeLineSeg3d(mouthLeft(),mouthRight()).midPoint() );  // 8
    // Grip points to eye
    xoff.x = yoff.y = meyesize;
	// Left eye
	cen = leftEyeCenter();
    gripPoints.append( cen );			// 9
    gripPoints.append( cen + xoff );		// 10
    gripPoints.append( cen + yoff );		// 11
    gripPoints.append( cen - xoff );		// 12
    gripPoints.append( cen - yoff );		// 13
	// Left eye
	cen = rightEyeCenter();
    gripPoints.append( cen );			// 14
    gripPoints.append( cen + xoff );		// 15
    gripPoints.append( cen + yoff );		// 16
    gripPoints.append( cen - xoff );		// 17
    gripPoints.append( cen - yoff );		// 18

    return eOk;
}

OdResult AsdkSmiley::subMoveGripPointsAt(const OdGePoint3dArray& gripPoints, 
									  const OdIntArray& indices)
{
  assertWriteEnabled();
  int idx, count = indices.length();
  OdGePoint3d idxpoint;

  for( int i = 0; i < count ; i++ )		// a few grip points can be selected concurrently
  {
	  idx = indices[i];
    idxpoint = gripPoints[idx];

    if( idx == 0 )						// Move the smiley center
	  { 
	    setCenter( idxpoint );
	  }
	  else if( idx >= 1 && idx <= 4 )		// Stretch the smiley radius
	  {    
		  scaleRadius( idxpoint.distanceTo( center() ));
	  }
	  else if( idx == 5 )					// Stretch the left edge of mouth
	  {
      setMouth( idxpoint, mouthBottom(), mouthRight() );
      ensureRadiusMouth();
	  }
	  else if( idx == 6 )					// Stretch the right edge of mouth
	  {
      setMouth( mouthLeft(), mouthBottom(), idxpoint );
      ensureRadiusMouth();
	  }
	  else if( idx == 7 )					// Stretch the bottom edge of mouth
	  {		
		  setMouth( mouthLeft(), idxpoint, mouthRight() );
      ensureRadiusMouth();
	  }
	  else if( idx == 8 )					// Move the mouth
	  {           
		  moveMouthToPoint( idxpoint );
		  ensureRadiusMouth();
	  }
	  else if( idx == 9 || idx == 14 )		// Move the center of eyes 
	  {
		  setEyesApart( (idxpoint.x - center().x) * 2 );					// Apart >= 2*eyeSize

      if( eyesApart() < 2 * eyeSize() ) setEyesApart( 2 * eyeSize() );

		  setEyesHeight( idxpoint.y - center().y );						// Height >= eyeSize

		  if( eyesHeight() < eyeSize() ) setEyesHeight( eyeSize() );

		  ensureRadiusEyes();
    }
	  else if( (idx >= 10 && idx <= 13) || (idx >= 15 && idx <= 18))	// Stretch the radius of eyes 
	  {
		  setEyeSize( idxpoint.distanceTo( (idx < 14) ? leftEyeCenter() : rightEyeCenter() ) );

		  if( 2 * eyeSize() > eyesApart() ) setEyeSize( eyesApart() / 2 );
		  ensureRadiusEyes();
	  };
  }
	return eOk;
}

void AsdkSmiley::moveMouthToPoint( const OdGePoint3d point )
{
        OdGeLineSeg3d lineSeg (mouthLeft(),mouthRight());
	OdGePoint3d middle( lineSeg.midPoint() );
	OdGeVector3d offset( point.x - middle.x, point.y - middle.y, point.z - middle.z );
	setMouth( mouthLeft() + offset, mouthBottom() + offset, mouthRight() + offset );
}

void AsdkSmiley::ensureRadiusMouth()
{
	double d;
	OdGePoint3d cen( center() );
	if((d = cen.distanceTo( mouthLeft() )) > radius() / 1.1) setRadius( 1.1 * d );
	if((d = cen.distanceTo( mouthRight() )) > radius() / 1.1) setRadius( 1.1 * d );
	if((d = cen.distanceTo( mouthBottom() )) > radius() / 1.1) setRadius( 1.1 * d );
}

void AsdkSmiley::ensureRadiusEyes()
{
    double d = center().distanceTo(leftEyeCenter()) + eyeSize();
	if(d > radius() / 1.1) setRadius( 1.1 * d );
}
//
// Step 5
// We are implementing 'getOsnapPoints' function to create the array of 
// snap points for smiley object. Snap points are being used for joining of
// smiley object with other objects at drawing. At testing, appropriate snap
// mode must be ON.
//
OdResult AsdkSmiley::subGetOsnapPoints(OdDb::OsnapMode osnapMode,
									OdGsMarker   /*gsSelectionMark*/,
									const OdGePoint3d& /*pickPoint*/,
									const OdGePoint3d& /*lastPoint*/,
									const OdGeMatrix3d& /*xfm*/,
									const OdGeMatrix3d& /*ucs*/,
									OdGePoint3dArray& snapPoints ) const
{
    assertReadEnabled();
    switch( osnapMode ) 
	{
	   case OdDb::kOsModeCen:
			snapPoints.append( center() );			// Osnap center to the face's center
			snapPoints.append( leftEyeCenter() );
			snapPoints.append( rightEyeCenter() );
			return eOk;

       case OdDb::kOsModeQuad:						// Osnap quad to the face's quad points
       case OdDb::kOsModeNear:
	      { OdGeVector3d xoff(radius(),0,0),yoff(0,radius(),0);
			OdGePoint3d cen( center() );	
			snapPoints.append( cen + xoff );
			snapPoints.append( cen + yoff );
			snapPoints.append( cen - xoff );
			snapPoints.append( cen - yoff );
		  }
			return eOk;

       case OdDb::kOsModeMid:
       case OdDb::kOsModeEnd:
       case OdDb::kOsModeNode:
       case OdDb::kOsModeIns:
       case OdDb::kOsModePerp:
       case OdDb::kOsModeTan:
       default: break;
    }
    return eInvalidInput;
}

//==================================================================================================//

OdResult AsdkSmiley::subGetGsMarkersAtSubentPath( const OdDbFullSubentPath& subPath, 
                                               OdGsMarkerArray& gsMarkers ) const
{
  assertReadEnabled();

  gsMarkers.clear();

  if( subPath.objectIds().length() == 0 )
    return eNotApplicable;

  gsMarkers.push_back( subPath.subentId().index() );

  return eOk;
}

//==================================================================================================//

OdResult AsdkSmiley::subGetSubentPathsAtGsMarker(OdDb::SubentType  type , 
                                              OdGsMarker  gsMark , 
                                              const OdGePoint3d& /* pickPoint */,
                                              const OdGeMatrix3d& /* xfm */, 
                                              OdDbFullSubentPathArray&  subentPaths , 
                                              const OdDbObjectIdArray* /* pEntAndInsertStack */) const
{
  assertReadEnabled();

  OdDbFullSubentPath pPaths;
  pPaths.subentId().setIndex(gsMark);
  pPaths.subentId().setType(OdDb::kClassSubentType);
  pPaths.objectIds().append(this->objectId());
  subentPaths.append(pPaths);
  return eOk;
}

//==================================================================================================//

OdResult AsdkSmiley::subGetSubentClassId( const OdDbFullSubentPath& path, void* clsId ) const
{
  assertReadEnabled();

  OdDbEntityPtr pEnt = subSubentPtr( path );

  if( !pEnt.isNull() )
  {
    pEnt->getClassID( clsId );
    return eOk;
  }

  return eInvalidInput;
}

//==================================================================================================//

OdResult AsdkSmiley::subGetSubentPathGeomExtents( const OdDbFullSubentPath& path, OdGeExtents3d& extents ) const
{
  assertReadEnabled();

  OdDbEntityPtr pEnt = subSubentPtr( path );

  if( !pEnt.isNull() )
  {
    pEnt->getGeomExtents( extents );
    return eOk;
  }

  return eInvalidInput;
}

//==================================================================================================//

OdDbEntityPtr AsdkSmiley::subSubentPtr( const OdDbFullSubentPath& id ) const
{
  assertReadEnabled();

  OdGsMarker iMark = id.subentId().index();

  if( iMark == kFaceMark )
  {
    OdDbCirclePtr pCircle = OdDbCircle::createObject();
    pCircle->setCenter( center() );
    pCircle->setNormal( mnormal );
    pCircle->setRadius( radius() );
    return pCircle;
  }
  else if( iMark == kLeftEyeMark || iMark == kRightEyeMark )
  {
    OdGePoint3d  ptEyeCenter;

    if( iMark == kLeftEyeMark )
      ptEyeCenter = leftEyeCenter();
    else
      ptEyeCenter = rightEyeCenter();

    OdDbCirclePtr pCircle = OdDbCircle::createObject();
    pCircle->setCenter( ptEyeCenter );
    pCircle->setNormal( mnormal );
    pCircle->setRadius( meyesize );
    return pCircle;
  }
  else if( iMark == kMouthMark )
  {
    OdGePoint3d smilecen( mouthCenter());
    OdGePoint3d startpt( mouthLeft() );
    OdGePoint3d endpt( mouthRight() );
    OdGeVector3d normvec( 1, 0, 0 );
    OdGeVector3d startvec = startpt - smilecen;
    OdGeVector3d endvec = endpt - smilecen;
    double startang = 2 * kPi - startvec.angleTo( normvec );
    double endang = 2 * kPi - endvec.angleTo( normvec );
    OdDbPolylinePtr pLine = OdDbPolyline::createObject();

    OdGeVector3d vrCenterStart = startpt   - smilecen;
    OdGeVector3d vrCenterEnd   = endpt     - smilecen;
    vrCenterStart /= vrCenterStart.length();
    vrCenterEnd   /= vrCenterEnd.length();
    OdGeVector3d vrBase = vrCenterStart + vrCenterEnd;
    vrBase /= vrBase.length();

    if( startang - endang > OdaPI )
      vrBase *= -1;

    OdGePoint3d ptBulge = smilecen + vrBase * smilecen.distanceTo( endpt );
    OdGeLineSeg3d segStartEnd( startpt, endpt );

    double dBulge = segStartEnd.midPoint().distanceTo( ptBulge ) / segStartEnd.midPoint().distanceTo( startpt );
    pLine->addVertexAt( 0, OdGePoint2d( startpt.x, startpt.y), dBulge );
    pLine->addVertexAt( 1, OdGePoint2d( endpt.x, endpt.y), 0 );
    pLine->setClosed( true );
    return pLine;
  }

  return OdDbEntityPtr();
}

//==================================================================================================//

OdResult AsdkSmiley::subGetGripPointsAtSubentPath( 
  const OdDbFullSubentPath& path, 
  OdDbGripDataPtrArray& grips,
  const double curViewUnitSize, 
  const int gripSize,
  const OdGeVector3d& curViewDir, 
  const OdUInt32 bitflags) const
{
  assertReadEnabled();

  OdGsMarker iMark = path.subentId().index();

  if( iMark == kFaceMark )
  {
    OdGePoint3d _center( center() );
    OdGeVector3d xoff( radius(), 0, 0 );
    OdGeVector3d yoff( 0, radius(), 0 );

    OdDbGripDataPtr pGrip1 = new OdDbGripData();
    pGrip1->setAppData((void *)(FACE_CENTER_GRIP_PT));
    pGrip1->setGripPoint(_center);
    grips.append(pGrip1);

    OdDbGripDataPtr pGrip2 = new OdDbGripData();
    pGrip2->setAppData((void *)(FACE_RIGHT_GRIP_PT));
    pGrip2->setGripPoint(_center + xoff);
    grips.append(pGrip2);

    OdDbGripDataPtr pGrip3 = new OdDbGripData();
    pGrip3->setAppData((void *)(FACE_TOP_GRIP_PT));
    pGrip3->setGripPoint(_center + yoff);
    grips.append(pGrip3);

    OdDbGripDataPtr pGrip4 = new OdDbGripData();
    pGrip4->setAppData((void *)(FACE_LEFT_GRIP_PT));
    pGrip4->setGripPoint(_center - xoff);
    grips.append(pGrip4);

    OdDbGripDataPtr pGrip5 = new OdDbGripData();
    pGrip5->setAppData((void *)(FACE_BOTTOM_GRIP_PT));
    pGrip5->setGripPoint(_center - yoff);
    grips.append(pGrip5);
  }
  else if( iMark == kLeftEyeMark || iMark == kRightEyeMark )
  {
    OdGePoint3d eyecen;
    OdGeVector3d xoff( meyesize, 0, 0 );
    OdGeVector3d yoff( 0, meyesize, 0 );
    
    if( iMark == kLeftEyeMark )
      eyecen = leftEyeCenter();
    else
      eyecen = rightEyeCenter();

    OdDbGripDataPtr pGrip1 = new OdDbGripData();
    pGrip1->setAppData((void *)(EYE_CENTER_GRIP_PT));
    pGrip1->setGripPoint(eyecen);
    grips.append(pGrip1);

    OdDbGripDataPtr pGrip2 = new OdDbGripData();
    pGrip2->setAppData((void *)(EYE_RIGHT_GRIP_PT));
    pGrip2->setGripPoint(eyecen + xoff);
    grips.append(pGrip2);

    OdDbGripDataPtr pGrip3 = new OdDbGripData();
    pGrip3->setAppData((void *)(EYE_TOP_GRIP_PT));
    pGrip3->setGripPoint(eyecen + yoff);
    grips.append(pGrip3);

    OdDbGripDataPtr pGrip4 = new OdDbGripData();
    pGrip4->setAppData((void *)(EYE_LEFT_GRIP_PT));
    pGrip4->setGripPoint(eyecen - xoff);
    grips.append(pGrip4);

    OdDbGripDataPtr pGrip5 = new OdDbGripData();
    pGrip5->setAppData((void *)(EYE_BOTTOM_GRIP_PT));
    pGrip5->setGripPoint(eyecen - yoff);
    grips.append(pGrip5);
  }
  else if( iMark == kMouthMark )
  {
    OdGeLineSeg3d chord( mouthLeft(), mouthRight() );

    OdDbGripDataPtr pGrip1 = new OdDbGripData();
    pGrip1->setAppData((void *)(MOUTH_LEFT_GRIP_PT));
    pGrip1->setGripPoint(mouthLeft());
    grips.append(pGrip1);

    OdDbGripDataPtr pGrip2 = new OdDbGripData();
    pGrip2->setAppData((void *)(MOUTH_RIGHT_GRIP_PT));
    pGrip2->setGripPoint(mouthRight());
    grips.append(pGrip2);

    OdDbGripDataPtr pGrip3 = new OdDbGripData();
    pGrip3->setAppData((void *)(MOUTH_MID_GRIP_PT));
    pGrip3->setGripPoint(chord.midPoint());
    grips.append(pGrip3);

    OdDbGripDataPtr pGrip4 = new OdDbGripData();
    pGrip4->setAppData((void *)(MOUTH_BOTTOM_GRIP_PT));
    pGrip4->setGripPoint(mouthBottom());
    grips.append(pGrip4);
  }

  return eOk;
}

//==================================================================================================//

OdResult AsdkSmiley::subMoveGripPointsAtSubentPaths( 
  const OdDbFullSubentPathArray& paths, 
  const OdDbVoidPtrArray& gripAppData, 
  const OdGeVector3d& offset, 
  const OdUInt32 bitflags )
{
  assertWriteEnabled();

  OdGePoint3d oldquad, newquad;

  for( unsigned int i = 0; i < gripAppData.length(); i++ ) 
  {
    OdGsMarker iMark = paths[i].subentId().index();

    OdInt32 idx = OdPtrToInt32(gripAppData.at(i));	

    if( iMark == kFaceMark )
    {
      switch( idx )
      {
      case FACE_CENTER_GRIP_PT:
        {
          setCenter( center() + offset );
        } break;

      case FACE_RIGHT_GRIP_PT:
        {
          oldquad = center() + OdGeVector3d( radius(), 0, 0 );
          newquad = oldquad + offset;
          scaleRadius( newquad.distanceTo( center() ));
        } break;

      case FACE_TOP_GRIP_PT:
        {
          oldquad = center() + OdGeVector3d( 0, radius(), 0 );
          newquad = oldquad + offset;
          scaleRadius( newquad.distanceTo( center() ));
        } break;

      case FACE_LEFT_GRIP_PT:
        {
          oldquad = center() - OdGeVector3d( radius(), 0, 0 );
          newquad = oldquad + offset;
          scaleRadius( newquad.distanceTo( center() ));
        } break;

      case FACE_BOTTOM_GRIP_PT:
        {
          oldquad = center() - OdGeVector3d( 0, radius(), 0 );
          newquad = oldquad + offset;
          scaleRadius( newquad.distanceTo( center() ));
        } break;
      }
    }
    else if( iMark == kLeftEyeMark || iMark == kRightEyeMark )
    {
      OdGePoint3d eyecen;
      OdGeVector3d xoff( meyesize, 0, 0 );
      OdGeVector3d yoff( 0, meyesize, 0 );

      if( iMark == kLeftEyeMark )
        eyecen = leftEyeCenter();
      else
        eyecen = rightEyeCenter();

      switch( idx )
      {
        case EYE_CENTER_GRIP_PT:
        {
          if( meyesheight+offset.x < 0 )
            setEyesHeight( 0 );
          else
            setEyesHeight( meyesheight + offset.y );

          if( eyecen.x < center().x ) // left eye
          { 
            if( meyesapart - ( offset.x * 2 ) < meyesize * 2 )
              setEyesApart( meyesize * 2 );
            else
              setEyesApart( meyesapart - ( offset.x * 2 ));
          }
          else // right eye
          {                         
            if( meyesapart + ( offset.x * 2 ) < meyesize * 2 )
              setEyesApart( meyesize * 2 );
            else
              setEyesApart( meyesapart + ( offset.x * 2));
          }

          ensureRadiusEyes();
        } break;

        case EYE_RIGHT_GRIP_PT:
        {
          oldquad = eyecen + OdGeVector3d( meyesize, 0, 0 );
          newquad = oldquad + offset;

          if( newquad.distanceTo( eyecen ) > meyesapart / 2 )
            setEyeSize( meyesapart / 2 );
          else
            setEyeSize( newquad.distanceTo( eyecen ));

          ensureRadiusEyes();
        } break;

        case EYE_TOP_GRIP_PT:
        {
          oldquad = eyecen - OdGeVector3d( meyesize, 0, 0 );
          newquad = oldquad + offset;

          if( newquad.distanceTo( eyecen ) > meyesapart / 2 )
            setEyeSize( meyesapart / 2 );
          else
            setEyeSize( newquad.distanceTo( eyecen ));

          ensureRadiusEyes();
        } break;

        case EYE_LEFT_GRIP_PT:
        {
          oldquad = eyecen - OdGeVector3d( meyesize, 0, 0 );
          newquad = oldquad + offset;

          if( newquad.distanceTo( eyecen ) > meyesapart / 2 )
            setEyeSize( meyesapart / 2 );
          else
            setEyeSize( newquad.distanceTo( eyecen ));

          ensureRadiusEyes();
        } break;

        case EYE_BOTTOM_GRIP_PT:
        {
          oldquad = eyecen - OdGeVector3d( 0, meyesize, 0 );
          newquad = oldquad + offset;

          if( newquad.distanceTo( eyecen ) > meyesapart / 2 )
            setEyeSize( meyesapart / 2 );
          else
            setEyeSize( newquad.distanceTo( eyecen ));

          ensureRadiusEyes();
        } break;
      }
    }
    else if( iMark == kMouthMark )
    {
      switch( idx )
      {
        case MOUTH_LEFT_GRIP_PT:
        {
          setMouth( mouthLeft() + offset, mouthBottom(), mouthRight() );
          ensureRadiusMouth();
        } break;

        case MOUTH_RIGHT_GRIP_PT:
        {
          setMouth( mouthLeft(), mouthBottom(), mouthRight() + offset );
          ensureRadiusMouth();
        } break;

        case MOUTH_MID_GRIP_PT:
        {
          setMouth( mouthLeft() + offset, mouthBottom() + offset, mouthRight() + offset );
          ensureRadiusMouth();
        } break;

        case MOUTH_BOTTOM_GRIP_PT:
        {
          setMouth( mouthLeft(), mouthBottom() + offset, mouthRight() );
          ensureRadiusMouth();
        } break;
      }
    }
  }
  return eOk;
}

//==================================================================================================//

OdResult AsdkSmiley::subMoveGripPointsAt( const OdIntArray& indices, const OdGeVector3d& offset )
{
  assertWriteEnabled();
  int idx, count = indices.length();
  OdGePoint3d idxpoint;
  OdGePoint3d oldquad, newquad;

  for( int i = 0; i < count ; i++ )		// a few grip points can be selected concurrently
  {
    idx = indices[i];

    switch( idx )
    {
      case 0:
      {
        setCenter( center() + offset );
      } break;

      case 1:
      {
        oldquad = center() + OdGeVector3d( radius(), 0, 0 );
        newquad = oldquad + offset;
        scaleRadius( newquad.distanceTo( center() ));
      } break;

      case 2:
      {
        oldquad = center() + OdGeVector3d( 0, radius(), 0 );
        newquad = oldquad + offset;
        scaleRadius( newquad.distanceTo( center() ));
      } break;

      case 3:
      {
        oldquad = center() - OdGeVector3d( radius(), 0, 0 );
        newquad = oldquad + offset;
        scaleRadius( newquad.distanceTo( center() ));
      } break;

      case 4:
      {
        oldquad = center() - OdGeVector3d( 0, radius(), 0 );
        newquad = oldquad + offset;
        scaleRadius( newquad.distanceTo( center() ));
      } break;
      case 5:
      {
        setMouth( mouthLeft() + offset, mouthBottom(), mouthRight() );
        ensureRadiusMouth();
      } break;

      case 6:
      {
        setMouth( mouthLeft(), mouthBottom(), mouthRight() + offset );
        ensureRadiusMouth();
      } break;

      case 7:
      {
        setMouth( mouthLeft(), mouthBottom() + offset, mouthRight() );
        ensureRadiusMouth();
      } break;

      case 8:
      {
        setMouth( mouthLeft() + offset, mouthBottom() + offset, mouthRight() + offset );
        ensureRadiusMouth();
      } break;

      case 9:
      case 14:
      {
        OdGePoint3d eyecen;

        if( idx == 9 )
          eyecen = leftEyeCenter();
        else
          eyecen = rightEyeCenter();

        if( meyesheight+offset.x < 0 )
          setEyesHeight( 0 );
        else
          setEyesHeight( meyesheight + offset.y );

        if( eyecen.x < center().x ) // left eye
        { 
          if( meyesapart - ( offset.x * 2 ) < meyesize * 2 )
            setEyesApart( meyesize * 2 );
          else
            setEyesApart( meyesapart - ( offset.x * 2 ));
        }
        else // right eye
        {                         
          if( meyesapart + ( offset.x * 2 ) < meyesize * 2 )
            setEyesApart( meyesize * 2 );
          else
            setEyesApart( meyesapart + ( offset.x * 2));
        }

        ensureRadiusEyes();
      } break;

      case 10:
      case 15:
      {
        OdGePoint3d eyecen;

        if( idx == 10 )
          eyecen = leftEyeCenter();
        else
          eyecen = rightEyeCenter();

        oldquad = eyecen + OdGeVector3d( meyesize, 0, 0 );
        newquad = oldquad + offset;

        if( newquad.distanceTo( eyecen ) > meyesapart / 2 )
          setEyeSize( meyesapart / 2 );
        else
          setEyeSize( newquad.distanceTo( eyecen ));

        ensureRadiusEyes();
      } break;

      case 11:
      case 16:
      {
        OdGePoint3d eyecen;

        if( idx == 11 )
          eyecen = leftEyeCenter();
        else
          eyecen = rightEyeCenter();

        oldquad = eyecen - OdGeVector3d( meyesize, 0, 0 );
        newquad = oldquad + offset;

        if( newquad.distanceTo( eyecen ) > meyesapart / 2 )
          setEyeSize( meyesapart / 2 );
        else
          setEyeSize( newquad.distanceTo( eyecen ));

        ensureRadiusEyes();
      } break;

      case 12:
      case 17:
      {
        OdGePoint3d eyecen;

        if( idx == 12 )
          eyecen = leftEyeCenter();
        else
          eyecen = rightEyeCenter();

        oldquad = eyecen - OdGeVector3d( meyesize, 0, 0 );
        newquad = oldquad + offset;

        if( newquad.distanceTo( eyecen ) > meyesapart / 2 )
          setEyeSize( meyesapart / 2 );
        else
          setEyeSize( newquad.distanceTo( eyecen ));

        ensureRadiusEyes();
      } break;

      case 13:
      case 18:
      {
        OdGePoint3d eyecen;

        if( idx == 18 )
          eyecen = leftEyeCenter();
        else
          eyecen = rightEyeCenter();

        oldquad = eyecen - OdGeVector3d( 0, meyesize, 0 );
        newquad = oldquad + offset;

        if( newquad.distanceTo( eyecen ) > meyesapart / 2 )
          setEyeSize( meyesapart / 2 );
        else
          setEyeSize( newquad.distanceTo( eyecen ));

        ensureRadiusEyes();
      } break;
    }
  }

  return eOk;
}

//==================================================================================================//

OdResult AsdkSmiley::subTransformSubentPathsBy( const OdDbFullSubentPathArray& paths, const OdGeMatrix3d& xform )
{
  assertWriteEnabled();

  for( unsigned int i = 0; i < paths.length(); i++ ) 
  {
    OdGsMarker iMark = paths[i].subentId().index();

    if( iMark == kFaceMark )
    {
      transformBy( xform );
    }
    else if( iMark == kRightEyeMark || iMark == kLeftEyeMark )
    {
      OdGePoint3d eyecen;
      OdGeVector3d xoff( meyesize, 0, 0 );
      OdGeVector3d yoff( 0, meyesize, 0 );

      if( iMark == kLeftEyeMark )
        eyecen = leftEyeCenter();
      else
        eyecen = rightEyeCenter();

      OdGePoint3d ptOldEyeCen = eyecen;

      OdGePoint3d iTopPt = eyecen;
      iTopPt.y += meyesize;
      OdGePoint3d iRightPt = eyecen;
      iRightPt.x += meyesize;

      eyecen.transformBy(xform);
      iTopPt.transformBy(xform);
      iRightPt.transformBy(xform);

      OdGeVector3d Offset = eyecen - ptOldEyeCen;
      double dMinD = iTopPt.distanceTo(eyecen) < iRightPt.distanceTo(eyecen) ? iTopPt.distanceTo(eyecen) : iRightPt.distanceTo(eyecen);

      if( meyesheight+Offset.y < 0 )
        setEyesHeight( 0 );
      else
        setEyesHeight( meyesheight + Offset.y );

      if( eyecen.x < center().x ) // left eye
      { 
        if( meyesapart - ( Offset.x * 2 ) < meyesize * 2 )
          setEyesApart( meyesize * 2 );
        else
          setEyesApart( meyesapart - ( Offset.x * 2 ));
      }
      else // right eye
      {                         
        if( meyesapart + ( Offset.x * 2 ) < meyesize * 2 )
          setEyesApart( meyesize * 2 );
        else
          setEyesApart( meyesapart + ( Offset.x * 2));
      }

      ensureRadiusEyes();

      if( dMinD > meyesapart / 2 )
        setEyeSize( meyesapart / 2 );
      else
        setEyeSize( dMinD );

      ensureRadiusEyes();
    }
    else if( iMark == kMouthMark )
    {
      OdGePoint3d ptMouthLeft = mouthLeft();
      OdGePoint3d ptMouthRight = mouthRight();
      OdGePoint3d ptMouthBottom = mouthBottom();

      ptMouthLeft.transformBy(xform);
      ptMouthRight.transformBy(xform);
      ptMouthBottom.transformBy(xform);

      OdGeVector3d vrLeftOffset  = ptMouthLeft - mouthLeft();
      OdGeVector3d vrRightOffset = ptMouthRight - mouthRight();
      OdGeVector3d vrBottomOffset   = ptMouthBottom - mouthBottom();
      setMouth( mouthLeft() + vrLeftOffset, mouthBottom() + vrBottomOffset, mouthRight() + vrRightOffset );
      ensureRadiusMouth();
    }
  }

  return eOk;
}
