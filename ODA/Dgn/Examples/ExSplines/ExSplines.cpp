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
/*************************************************************************/
/* The application creates SplineExample.dgn with some popularisation of */
/* the splines elements                                                  */
/*************************************************************************/

#include "OdaCommon.h"
#include "StaticRxObject.h"
#include "DynamicLinker.h"
#include "RxInit.h"
#include "DgDatabase.h"
#include "DgModel.h"
#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"
#include "DgText.h"
#include "DgBSplineCurve.h"
#include "DgBSplineSurface.h"
#include "DgLine.h"
#include "RxDynamicModule.h"

#include <stdio.h>
#include <locale.h>

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifndef _tcout
#ifdef _UNICODE
#define _tcout wcout
#else
#define _tcout cout
#endif
#endif

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif


void                  DrawTable(OdDgModel* pModel);
void                  DrawText( OdDgModel* pModel, const OdString& text, OdGePoint3d const& position );
OdDgBSplineCurve3dPtr DrawZigzagSpline( OdDgModel* pModel, OdGePoint3d const& position, OdUInt8 order, bool isClose, bool bRational, const OdGeDoubleArray& arrWeights );
OdDgBSplineCurve3dPtr DrawNiceSpline( OdDgModel* pModel, OdGePoint3d const& position );


/************************************************************************/
/* Define a Custom Services class.                                      */
/*                                                                      */
/* Combines the platform dependent functionality of                     */
/* OdExDgnSystemServices and OdExDgnHostAppServices                     */ 
/************************************************************************/
class MyServices : public OdExDgnSystemServices, public OdExDgnHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);
};


#ifndef _TOOLKIT_IN_DLL_
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
ODRX_END_STATIC_MODULE_MAP()
#endif


/************************************************************************/
/* Main                                                                 */
/************************************************************************/
#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  int nRes = 0; // Return value for main

  /**********************************************************************/
  /* Set command line defaults                                          */
  /**********************************************************************/
  bool disableOutput = false;

#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  setlocale(LC_TIME, ""); 

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /********************************************************************/
  /* Create a custom Services instance.                               */
  /********************************************************************/
  OdStaticRxObject<MyServices> svcs;

  /**********************************************************************/
  /* Display the Product and Version that created the executable        */
  /**********************************************************************/
  STD(cout) << "\nExSplines sample program. Copyright (c) 2019, Open Design Alliance\n";

  /**********************************************************************/
  /* Parse Command Line inputs                                          */
  /**********************************************************************/
  bool bInvalidArgs = (argc < 2);
  if (bInvalidArgs)
  {
    bInvalidArgs = true;
    nRes  = 1;
  }
  /**********************************************************************/
  /* Disable progress meter                                             */
  /**********************************************************************/
  if (argc >= 3)
  {
    OdString argv2(argv[2]);
    if (!odStrICmp(argv2, OD_T("-DO")))
    {
      disableOutput = true;
    }
  }

  if (bInvalidArgs)    
  {
    STD(cout)  << "\n\n\tusage: ExSplines <filename> [-DO]";
    STD(cout)  << "\n\t-DO disables progress meter output.\n";
    return nRes;
  }

  /************************************************************************/
  /* Disable/Enable progress meter                                       */
  /************************************************************************/
  svcs.disableProgressMeterOutput( disableOutput );

  /**********************************************************************/
  /* Initialize Runtime Extension environment                           */
  /**********************************************************************/
  odrxInitialize( &svcs );

  OdString fileName(argv[1]);

  try
  {
    /**********************************************************************/
    /* Initialize ODA Drawings API for DGN                                */
    /**********************************************************************/
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);

    /********************************************************************/
    /* Create a default OdDgDatabase object                             */
    /********************************************************************/
    OdDgDatabasePtr pDb = svcs.createDatabase();
    OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
    pModel->setWorkingUnit( OdDgModel::kWuMasterUnit );

    /********************************************************************/
    /* Fill the pDb                                                */
    /********************************************************************/
    DrawTable(pModel);
    pModel->fitToView();
    pModel = 0;

    /********************************************************************/
    /* Write the pDb                                               */
    /********************************************************************/
    pDb->writeFile( fileName );
  }
  catch (...)
  {
    STD(cout) << "\n\nUnexpected error.";
    nRes = -1;
    throw;
  }

  /**********************************************************************/
  /* Uninitialize Runtime Extension environment                         */
  /**********************************************************************/
  ::odrxUninitialize();

  if (nRes)
  {
     STD(cout) << "\n\nPress ENTER to continue...";
     STD(cin).get();
  }
  STD(cout) << "\n\n";
  return nRes;
}


/************************************************************************/
/* Other functions                                                      */
/************************************************************************/


void DrawTable(OdDgModel* pModel)
{
  //lines
  {
    OdDgLine3dPtr line;
    
    line = OdDgLine3d::createObject();
    line->setStartPoint( OdGePoint3d( 0., 5., 0. ) );
    line->setEndPoint( OdGePoint3d( 60., 5., 0. ) );
    pModel->addElement( line );
    line = OdDgLine3d::createObject();
    line->setStartPoint( OdGePoint3d( 0., 0., 0. ) );
    line->setEndPoint( OdGePoint3d( 0., -30., 0. ) );
    pModel->addElement( line );
  }

  //text for the table
  {
    char  string[ 10 ];
    int   i;

    DrawText( pModel, "Order", OdGePoint3d( 0., 6., 0. ) );
    DrawText( pModel, "Rational", OdGePoint3d( -15., -5.5, 0. ) );
    DrawText( pModel, "Open", OdGePoint3d( -15., -15.5, 0. ) );
    DrawText( pModel, "Closed", OdGePoint3d( -15., -25.5, 0. ) );

    for( i = 0; i < 6; i++ )
    {
      sprintf( string, "%d", i + 3 );
      DrawText( pModel, string, OdGePoint3d( i * 10. + 4.5, 2., 0. ) );
    }
  }

  //splines in the table
  {
    OdUInt8               i, j;
    OdDgBSplineCurve3dPtr curve;

    for( i = 0; i < 6; i++ )
    {
      OdGeDoubleArray arrEmptyWeights;
      OdGeDoubleArray arrWeights;
      for( j = 0; j < 12; j++ )
      {
        arrWeights.push_back((j % 3 ) / 3. + .3333 );
      }
      //rational
      curve = DrawZigzagSpline( pModel, OdGePoint3d( i * 10. + 5., -5., 0. ), i + 3, false, true, arrWeights );

      //open
      DrawZigzagSpline( pModel, OdGePoint3d( i * 10. + 5., -15., 0. ), i + 3, false, false, arrEmptyWeights );

      //close
      curve = DrawZigzagSpline( pModel, OdGePoint3d( i * 10. + 5., -25., 0. ), i + 3, true, false, arrEmptyWeights );
    }
  }

  //splines with visible parts
  {
    OdDgBSplineCurve3dPtr curve;

    curve = DrawNiceSpline( pModel, OdGePoint3d( 80., -10., 0. ) );
    curve = DrawNiceSpline( pModel, OdGePoint3d( 100., -10., 0. ) );
    curve->setPolygonDisplayFlag( true );
    curve->setCurveDisplayFlag( false );
    curve = DrawNiceSpline( pModel, OdGePoint3d( 120., -10., 0. ) );
    curve->setPolygonDisplayFlag( true );

    DrawText( pModel, "Display mode for spline/polygon", OdGePoint3d( 84., 5., 0. ) );
  }
}


void DrawText( OdDgModel* pModel, const OdString& text, OdGePoint3d const& position )
{
  OdDgText3dPtr textElement;

  textElement = OdDgText3d::createObject();
  textElement->setText( text );
  textElement->setOrigin( position );
  textElement->setRotation( OdGeQuaternion( 1., 0., 0., 0. ) );

  textElement->setHeightMultiplier( 1. );
  textElement->setLengthMultiplier( 1. );

  pModel->addElement( textElement );
}


OdDgBSplineCurve3dPtr DrawZigzagSpline( OdDgModel* pModel, 
                                          OdGePoint3d const& position, 
                                            OdUInt8 order, 
                                              bool isClose, 
                                                bool bRational, 
                                                  const OdGeDoubleArray& arrWeights )
{
  OdDgBSplineCurve3dPtr curve = OdDgBSplineCurve3d::createObject();

  OdGePoint3dArray arrCtrlPts;
  arrCtrlPts.push_back( position + OdGeVector3d( -2.,  3., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d(  2.,  3., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d(  2.,  1., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d( -2.,  1., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d( -2., -1., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d(  2., -1., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d(  2., -3., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d( -2., -3., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d( -2., -1., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d(  2., -1., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d(  2.,  1., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d( -2.,  1., 0. ) );

  OdGeKnotVector arrUniformKnots;

  curve->setNurbsData( order, bRational, isClose, arrCtrlPts, arrUniformKnots, arrWeights );
  pModel->addElement( curve );

  return curve;
}


OdDgBSplineCurve3dPtr DrawNiceSpline( OdDgModel* pModel, OdGePoint3d const& position )
{
  OdDgBSplineCurve3dPtr curve = OdDgBSplineCurve3d::createObject();

  OdGePoint3dArray arrCtrlPts;
  arrCtrlPts.push_back( position + OdGeVector3d( -2., -5., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d( -4., -5., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d( -4., -2., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d(  0., -2., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d(  0., -5., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d(  4., -5., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d(  4.,  0., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d( -3.,  0., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d(  2.,  2., 0. ) );
  arrCtrlPts.push_back( position + OdGeVector3d( -1.,  7., 0. ) );

  OdGeKnotVector  arrUniformKnots;
  OdGeDoubleArray arrEmptyWeights;

  curve->setNurbsData( 4, false, false, arrCtrlPts, arrUniformKnots, arrEmptyWeights );

  //add all poles
  pModel->addElement( curve );

  return curve;
}
