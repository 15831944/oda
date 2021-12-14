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
#include "SmileyDb.h"
//
// Source contains functions to get and to set a property of smiley object.
//
OdCmEntityColor AsdkSmiley::backColor()	const		// get back color of smiley
{
	assertReadEnabled();
	return mbackcolor;
}
													// set back color of smiley
void AsdkSmiley::setBackColor(const OdCmEntityColor& color)
{
	assertWriteEnabled();
	mbackcolor=color;
}

OdGeVector3d AsdkSmiley::normal() const				// get smiley normal
{
    assertReadEnabled();
    return mnormal;
}

void AsdkSmiley::setNormal(const OdGeVector3d v)	// set smiley normal
{
    assertWriteEnabled();
    mnormal = v;
    recordGraphicsModified();
}
//
// ------ Functions of face ------
//
OdGePoint3d AsdkSmiley::center() const				// get center of face
{
    assertReadEnabled();
    return mfacecircle.center();
}

void AsdkSmiley::setCenter(const OdGePoint3d c)		// set center of face
{
    assertWriteEnabled();
    mmoutharc.translateBy( c - mfacecircle.center() );
    mfacecircle.setCenter( c );
    recordGraphicsModified();
} 

double AsdkSmiley::radius() const					// get radius of face
{
    assertReadEnabled();
    return mfacecircle.radius();
}

void AsdkSmiley::setRadius(const double r)     		// set radius of face
{
    assertWriteEnabled();
    mfacecircle.setRadius( r );
}
//
// ------ Functions of eyes ------
//
double AsdkSmiley::eyesApart() const				// get eyes apart
{													// It is a distance between centers of eyes
    assertReadEnabled();
    return meyesapart;
}

void AsdkSmiley::setEyesApart(const double a)		// set eyes apart
{
    assertWriteEnabled();
    meyesapart = a;
    recordGraphicsModified();
}

double AsdkSmiley::eyesHeight() const				// get eyes height
{													// It is a distance from center of eyes
    assertReadEnabled();							// to middle line of face
    return meyesheight;
}

void AsdkSmiley::setEyesHeight(const double h)      // set eyes height
{
    assertWriteEnabled();
	meyesheight = h;
    recordGraphicsModified();
}

double AsdkSmiley::eyeSize() const					// get eyes size
{													// It is a radius of eyes
    assertReadEnabled();
    return meyesize;
}

void AsdkSmiley::setEyeSize(const double s)			// set eyes size
{
    assertWriteEnabled();
    meyesize = s;
    recordGraphicsModified();
}

OdGePoint3d AsdkSmiley::leftEyeCenter() const		// get center of left eye
{
    assertReadEnabled();
	OdGeVector3d offset(-meyesapart/2, meyesheight, 0);
    return mfacecircle.center() + offset;
}

OdGePoint3d AsdkSmiley::rightEyeCenter() const		// get center of right eye
{
    assertReadEnabled();
	OdGeVector3d offset(meyesapart/2, meyesheight, 0);
    return mfacecircle.center() + offset;
}
//
// ------ Functions of mouth ------
//
OdGePoint3d AsdkSmiley::mouthCenter() const			// get center of mouth arc
{
    assertReadEnabled();
    return mmoutharc.center();
}

double AsdkSmiley::mouthRadius() const				// get radius of mouth arc
{
    assertReadEnabled();
    return mmoutharc.radius();
}

OdGePoint3d AsdkSmiley::mouthLeft() const     		// get left mouth point
{													// It is a first point of mouth arc
    assertReadEnabled();
    return mmoutharc.startPoint();					
}

OdGePoint3d AsdkSmiley::mouthRight() const     		// get right mouth point
{													// It is a end point of mouth arc
    assertReadEnabled();
    return mmoutharc.endPoint();
}
													// get bottom mouth point
OdGePoint3d AsdkSmiley::mouthBottom() const    		// It is calculated as point of  
{													// intersection of the mouth arc with
    assertReadEnabled();							// line from mouth center to the face 
	int countIntersect;								// bottom point on border.
    OdGePoint3d faceBottom( center() - OdGeVector3d(0,radius(),0) ),
		        FirstIntersect,SecondIntersect;
	OdGeLine3d  line( mouthCenter(), faceBottom );
    
	mmoutharc.intersectWith(line,countIntersect,FirstIntersect,SecondIntersect);
    return FirstIntersect;
}

void AsdkSmiley::setMouth( const OdGePoint3d& left, const OdGePoint3d& bottom, const OdGePoint3d& right )
{
    assertWriteEnabled();
    OdGePoint3d realLeft( left ), realBottom( bottom ), realRight( right );

    double topY = center().y + meyesheight - meyesize;
    double diff = 0.0;								// Mouth must be under eyes always

    if (( left.y - topY ) > diff ){
        diff = left.y - topY;
    }
    if (( bottom.y - topY ) > diff ){
        diff = bottom.y - topY;
    }
    if (( right.y - topY ) > diff ){
        diff = right.y - topY;
    }
    
    OdGeVector3d vec( 0, diff, 0 );
    mmoutharc.set( realLeft - vec, realBottom - vec, realRight - vec );
    recordGraphicsModified();
}
