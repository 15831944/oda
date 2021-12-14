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
// Preparation step
// We are implementing the constuctor and destructor of smiley object
// and are creating implementations for the functions declared by the 
// ODDB_DECLARE_MEMBERS macro in the class declaration.
//
ODRX_DXF_DEFINE_MEMBERS(AsdkSmiley,									// class name
						OdDbEntity,									// parent class name
						DBOBJECT_CONSTR,							// creation macro
						OdDb::kDHL_CURRENT,                         // dwg version
						OdDb::kMReleaseCurrent,                     // maintenance release version
						OdDbProxyEntity::kTransformAllowed |
                        OdDbProxyEntity::kColorChangeAllowed |
                        OdDbProxyEntity::kLayerChangeAllowed,       // proxy flags
						ASDKSMILEY,									// DXF name
  AsdkSmileyDb|Description: Teigha Run-time Extension Example)	// Application name

AsdkSmiley::AsdkSmiley()			// Constuctor of smiley object
{
  mnormal.set( 0,0,1 );
  setCenter( OdGePoint3d( 0,0,0 ) );
  setRadius( 1.0 );
  meyesapart=0.9;
  meyesheight=0.4;
  meyesize=0.15;

  OdGeVector3d vecstart( 0.9 * sin( 1.25 * kPi ), 0.9 * cos( 1.25 * kPi ), 0 ),
               vecend( 0.9 * cos( 1.75 * kPi ), 0.9 * sin( 1.75 * kPi ), 0 ),
               vecbottom( 0, -1.0, 0 );
  OdGePoint3d mouthcen( center() );
  mouthcen.y += 0.4;

  setMouth( mouthcen + vecstart, mouthcen + vecbottom, mouthcen + vecend );

  mbackcolor.setRGB(0,0,0);
  recordGraphicsModified();

#ifdef SMILEY_WITH_TEXT
  mTextField = mTextFieldValue;
#endif

}

AsdkSmiley::~AsdkSmiley() {}		// Destructor of smiley object
//
// To create a custom application, we should create a subclass of OdRxModule
// and implement 'init' and 'uninit' functions, that is registered/unregistered 
// the custom object types in this module.
//
#include "RxDynamicModule.h"

class SmileyModule : public OdRxModule
{
  public:
	void initApp()   {  AsdkSmiley::rxInit();  };
    void uninitApp() {  AsdkSmiley::rxUninit();  };
};
// Macro defines the entry point for this library.
//
ODRX_DEFINE_DYNAMIC_MODULE(SmileyModule);
//
// We should implement also a functions to set and to get a property of 
// smiley object. These function is located in the SmileyDbProperty.cpp.
//
