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
// Step 6
// We are implementing 'explode' function to destroy the smiley object on its 
// elements. Face and eyes are transmuted into circle entities, the mouth is  
// transmuted into arc entity and line entity. After exploding of custom object 
// the simple entity exists independently.
//
#include "DbCircle.h"
#include "DbLine.h"
#include "DbArc.h"
//
OdResult AsdkSmiley::subExplode(OdRxObjectPtrArray& entitySet) const
{
    assertReadEnabled();

	OdDbCirclePtr pFace = OdDbCircle::createObject();			// Create circle for face
	pFace->setPropertiesFrom(this);
	pFace->setNormal( normal() );
	pFace->setRadius( radius() );
	pFace->setCenter( center() );
    entitySet.append( pFace.get() );

	OdDbCirclePtr pLeftEye = OdDbCircle::createObject();		// Create circle for left eye
	pLeftEye->setPropertiesFrom(this);
	pLeftEye->setNormal( normal() );
	pLeftEye->setRadius( eyeSize() );
	pLeftEye->setCenter( leftEyeCenter() );
    entitySet.append( pLeftEye.get() );
    
	OdDbCirclePtr pRightEye = OdDbCircle::createObject();		// Create circle for right eye
	pRightEye->setPropertiesFrom(this);
	pRightEye->setNormal( normal() );
	pRightEye->setRadius( eyeSize() );
	pRightEye->setCenter( rightEyeCenter() );
    entitySet.append( pRightEye.get() );
	
	OdDbLinePtr pLine = OdDbLine::createObject();				// Create arc chord for mouth
	pLine->setPropertiesFrom(this);
	pLine->setNormal( normal() );
	pLine->setStartPoint( mouthLeft() );
	pLine->setEndPoint( mouthRight() );
	entitySet.append( pLine.get() );

    OdGeVector3d normvec( 1, 0, 0 );							// Create arc for mouth 
    double dStartAngle = 2 * kPi - (mouthLeft() - mouthCenter()).angleTo( normvec ),
           dEndAngle = 2 * kPi - (mouthRight() - mouthCenter()).angleTo( normvec );

	OdDbArcPtr pArc = OdDbArc::createObject();
    pArc->setPropertiesFrom(this);
    pArc->setNormal( normal() );
    pArc->setRadius( mouthRadius() );
    pArc->setCenter( mouthCenter() );
    pArc->setStartAngle( dStartAngle );
    pArc->setEndAngle( dEndAngle );
    entitySet.append( pArc.get() );

    return eOk;
}
