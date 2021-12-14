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

#include "RxDictionary.h"

#include "DgEllipse.h"
#include "DgLineString.h"
#include "DgModel.h"
#include "DgArc.h"
#include "DgCone.h"
#include "DgLine.h"
#include "DgText.h"
#include "DgTextNode.h"
#include "DgShape.h"
#include "DgDimOptions.h"
#include "DgDimension.h"
#include "DgCurve.h"
#include "DgConic.h"
#include "DgPointString.h"
#include "DgComplexString.h"
#include "DgDatabase.h"
#include "DgMultiline.h"
#include "DgComplexShape.h"
#include "DgBSplineCurve.h"
#include "DgBSplineSurface.h"
#include "Dg3DObject.h"
#include "DgRaster.h"
#include "DgRasterAttach.h"
#include "DgTagElement.h"
#include "DgMesh.h"
#include "DgReferenceAttach.h"
#include "DgLevelMask.h"
#include "DgSharedCellDefinition.h"
#include "DgSharedCellReference.h"
#include "DgBRepEntityPE.h"
#include "DgCellHeader.h"
#include "DgTextStyleTableRecord.h"
#include "DgAttributeLinkage.h"
#include "DgHostAppServices.h"
#include "DgDimStyleTableRecord.h"
#include "DgLight.h"
#include "DgLevelFilterTableRecord.h"
#include "DgLineStyleTableRecord.h"

#include "DgSummaryInfo.h"
#include "TGVersion.h"

#include "MemoryStream.h"

#include "ExDgnCellTransformFiller.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a
#ifndef _tcout
#ifdef TD_UNICODE
#define _tcout wcout
#else
#define _tcout cout
#endif
#endif

//========================================================================

const double   OdDgnCellTransformFiller::boxItemSize              = 10.0;
const OdUInt32 OdDgnCellTransformFiller::iBoxColorIndex           = 181;
const OdUInt32 OdDgnCellTransformFiller::iControlLineColorIndex   = 128;
const OdUInt32 OdDgnCellTransformFiller::iCenterMarkColorIndex    = 163;
const OdUInt32 OdDgnCellTransformFiller::iCellColorIndex          = 2;
const OdUInt32 OdDgnCellTransformFiller::iUnrotatedCellColorIndex = 224;
const OdUInt32 OdDgnCellTransformFiller::iBoxLStyleId             = 0;
const OdUInt32 OdDgnCellTransformFiller::iControlLineLStyleId     = 5;

//========================================================================

OdDgnCellTransformFiller::OdDgnCellTransformFiller( OdDgDatabase* pDb )
{
  m_pDb = pDb;

  m_idModel3d = pDb->getActiveModelId();

  if( !m_idModel3d.isNull() )
  {
    m_pModel3d = m_idModel3d.safeOpenObject(OdDg::kForWrite);
    m_pModel3d->setName(OD_T("Model for 3D elements") );
    m_pModel3d->setWorkingUnit( OdDgModel::kWuMasterUnit );
  }

  // create additional 2d Model
  OdDgModelTablePtr pModelTable = pDb->getModelTable();
  m_pModel2d = OdDgModel::createObject();
  m_pModel2d->setModelIs3dFlag(false);
  m_pModel2d->setName(OD_T("Model for 2D elements") );
  m_pModel2d->setWorkingUnit( OdDgModel::kWuMasterUnit );
  pModelTable->add( m_pModel2d );

  OdGeVector3d vrAngleEnd(1,1,1);
  vrAngleEnd.normalize();
  OdGeVector3d vrAngle1 = vrAngleEnd;
  vrAngle1.z = 0;
  vrAngle1.x = sqrt(1 - vrAngle1.y*vrAngle1.y);

  double dZAngle = OdGeVector3d::kXAxis.angleTo(vrAngle1, OdGeVector3d::kZAxis);

  OdGeMatrix3d matZRot;
  OdGeMatrix3d matYRot;
  OdGeMatrix3d matEndRot;

  matZRot.setToRotation( dZAngle, OdGeVector3d::kZAxis);
  matYRot.setToRotation( -OdaPI4, OdGeVector3d::kYAxis);
  matEndRot.setToRotation( OdaPI4,  vrAngleEnd);

  m_matRotation3d = matEndRot * matYRot * matZRot;

  m_matRotation2d.setToRotation( OdaPI4 );
}

//========================================================================

OdDgnCellTransformFiller::~OdDgnCellTransformFiller()
{

}

//========================================================================

void OdDgnCellTransformFiller::create2DBoxes( OdUInt32 nRows, OdUInt32 nCols )
{
  for( OdUInt32 i = 0; i < nCols; i++ )
  {
    for( OdUInt32 j = 0; j < nRows; j++ )
    {
      OdDgLineString2dPtr boxLine = OdDgLineString2d::createObject();

      boxLine->setColorIndex( iBoxColorIndex );
      boxLine->setLineStyleEntryId( iBoxLStyleId );

      boxLine->addVertex( OdGePoint2d( i*boxItemSize, j*boxItemSize) );
      boxLine->addVertex( OdGePoint2d( (i + 1)*boxItemSize, j*boxItemSize) );
      boxLine->addVertex( OdGePoint2d( (i + 1)*boxItemSize, (j + 1)*boxItemSize) );
      boxLine->addVertex( OdGePoint2d( i*boxItemSize, (j + 1)*boxItemSize) );
      boxLine->addVertex( OdGePoint2d( i*boxItemSize, j*boxItemSize) );

      OdDgLine2dPtr ctrlLine = OdDgLine2d::createObject();

      ctrlLine->setColorIndex(iControlLineColorIndex);
      ctrlLine->setLineStyleEntryId(iControlLineLStyleId);

      ctrlLine->setStartPoint(OdGePoint2d( i*boxItemSize, j*boxItemSize));
      ctrlLine->setEndPoint(OdGePoint2d( (i + 1)*boxItemSize, (j + 1)*boxItemSize));

      OdDgEllipse2dPtr centerMark = OdDgEllipse2d::createObject();

      centerMark->setColorIndex( iCenterMarkColorIndex );

      centerMark->setPrimaryAxis( boxItemSize/30.0 );
      centerMark->setSecondaryAxis( boxItemSize/30.0 );
      centerMark->setRotationAngle( 0.0 );
      centerMark->setOrigin( OdGePoint2d( (i + 0.5)*boxItemSize, (j + 0.5)*boxItemSize ) );

      m_pModel2d->addElement( boxLine );
      m_pModel2d->addElement( ctrlLine );
      m_pModel2d->addElement( centerMark );

      centerMark->setFillColor( iCenterMarkColorIndex );
    }
  }
}

//========================================================================

void OdDgnCellTransformFiller::create3DBoxes( OdUInt32 nRows, OdUInt32 nCols )
{
  for( OdUInt32 i = 0; i < nCols; i++ )
  {
    for( OdUInt32 j = 0; j < nRows; j++ )
    {
      OdDgLineString3dPtr boxTopLine = OdDgLineString3d::createObject();

      boxTopLine->setColorIndex( iBoxColorIndex );
      boxTopLine->setLineStyleEntryId( iBoxLStyleId );

      boxTopLine->addVertex( OdGePoint3d( i*boxItemSize, j*boxItemSize, boxItemSize/2.0) );
      boxTopLine->addVertex( OdGePoint3d( (i + 1)*boxItemSize, j*boxItemSize, boxItemSize/2.0) );
      boxTopLine->addVertex( OdGePoint3d( (i + 1)*boxItemSize, (j + 1)*boxItemSize, boxItemSize/2.0) );
      boxTopLine->addVertex( OdGePoint3d( i*boxItemSize, (j + 1)*boxItemSize, boxItemSize/2.0) );
      boxTopLine->addVertex( OdGePoint3d( i*boxItemSize, j*boxItemSize, boxItemSize/2.0) );

      OdDgLineString3dPtr boxBottomLine = OdDgLineString3d::createObject();

      boxBottomLine->setColorIndex( iBoxColorIndex );
      boxBottomLine->setLineStyleEntryId( iBoxLStyleId );

      boxBottomLine->addVertex( OdGePoint3d( i*boxItemSize, j*boxItemSize, -boxItemSize/2.0) );
      boxBottomLine->addVertex( OdGePoint3d( (i + 1)*boxItemSize, j*boxItemSize, -boxItemSize/2.0) );
      boxBottomLine->addVertex( OdGePoint3d( (i + 1)*boxItemSize, (j + 1)*boxItemSize, -boxItemSize/2.0) );
      boxBottomLine->addVertex( OdGePoint3d( i*boxItemSize, (j + 1)*boxItemSize, -boxItemSize/2.0) );
      boxBottomLine->addVertex( OdGePoint3d( i*boxItemSize, j*boxItemSize, -boxItemSize/2.0) );

      OdDgLine3dPtr boxZLine1 = OdDgLine3d::createObject();

      boxZLine1->setColorIndex( iBoxColorIndex );
      boxZLine1->setLineStyleEntryId( iBoxLStyleId );

      boxZLine1->setStartPoint( OdGePoint3d( i*boxItemSize, j*boxItemSize, boxItemSize/2.0) );
      boxZLine1->setEndPoint( OdGePoint3d( i*boxItemSize, j*boxItemSize, -boxItemSize/2.0) );

      OdDgLine3dPtr boxZLine2 = OdDgLine3d::createObject();

      boxZLine2->setColorIndex( iBoxColorIndex );
      boxZLine2->setLineStyleEntryId( iBoxLStyleId );

      boxZLine2->setStartPoint( OdGePoint3d( (i+1)*boxItemSize, j*boxItemSize, boxItemSize/2.0) );
      boxZLine2->setEndPoint( OdGePoint3d( (i+1)*boxItemSize, j*boxItemSize, -boxItemSize/2.0) );

      OdDgLine3dPtr boxZLine3 = OdDgLine3d::createObject();

      boxZLine3->setColorIndex( iBoxColorIndex );
      boxZLine3->setLineStyleEntryId( iBoxLStyleId );

      boxZLine3->setStartPoint( OdGePoint3d( (i+1)*boxItemSize, (j+1)*boxItemSize, boxItemSize/2.0) );
      boxZLine3->setEndPoint( OdGePoint3d( (i+1)*boxItemSize, (j+1)*boxItemSize, -boxItemSize/2.0) );

      OdDgLine3dPtr boxZLine4 = OdDgLine3d::createObject();

      boxZLine4->setColorIndex( iBoxColorIndex );
      boxZLine4->setLineStyleEntryId( iBoxLStyleId );

      boxZLine4->setStartPoint( OdGePoint3d( i*boxItemSize, (j+1)*boxItemSize, boxItemSize/2.0) );
      boxZLine4->setEndPoint( OdGePoint3d( i*boxItemSize, (j+1)*boxItemSize, -boxItemSize/2.0) );

      OdDgLine3dPtr ctrlLine = OdDgLine3d::createObject();

      ctrlLine->setColorIndex(iControlLineColorIndex);
      ctrlLine->setLineStyleEntryId(iControlLineLStyleId);

      ctrlLine->setStartPoint(OdGePoint3d( i*boxItemSize, j*boxItemSize, -boxItemSize/2.0));
      ctrlLine->setEndPoint(OdGePoint3d( (i + 1)*boxItemSize, (j + 1)*boxItemSize, boxItemSize/2.0));

      OdDgEllipse3dPtr centerMark = OdDgEllipse3d::createObject();

      centerMark->setColorIndex( iCenterMarkColorIndex );

      centerMark->setPrimaryAxis( boxItemSize/30.0 );
      centerMark->setSecondaryAxis( boxItemSize/30.0 );
      centerMark->setOrigin( OdGePoint3d( (i + 0.5)*boxItemSize, (j + 0.5)*boxItemSize, 0 ) );

      m_pModel3d->addElement( boxTopLine );
      m_pModel3d->addElement( boxBottomLine );
      m_pModel3d->addElement( boxZLine1 );
      m_pModel3d->addElement( boxZLine2 );
      m_pModel3d->addElement( boxZLine3 );
      m_pModel3d->addElement( boxZLine4 );
      m_pModel3d->addElement( ctrlLine );
      m_pModel3d->addElement( centerMark );

      centerMark->setFillColor( iCenterMarkColorIndex );
    }
  }
}

//========================================================================

OdGePoint2d OdDgnCellTransformFiller::get2dBoxCenter( OdUInt32 iRow, OdUInt32 iCol )
{
  return OdGePoint2d( (iCol + 0.5)*boxItemSize, (iRow + 0.5)*boxItemSize );
}

//========================================================================

OdGePoint3d OdDgnCellTransformFiller::get3dBoxCenter( OdUInt32 iRow, OdUInt32 iCol )
{
  return OdGePoint3d( (iCol + 0.5)*boxItemSize, (iRow + 0.5)*boxItemSize , 0 );
}

//========================================================================

void OdDgnCellTransformFiller::addLine3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
     iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgLine3dPtr pLineTop = OdDgLine3d::createObject();

  pLineTop->setColorIndex( iColor );

  OdGePoint3d ptStartTop = ptCenter - OdGeVector3d::kXAxis * boxItemSize / 3.0 + OdGeVector3d::kYAxis * boxItemSize / 4.0;
  OdGePoint3d ptEndTop   = ptCenter + OdGeVector3d::kXAxis * boxItemSize / 3.0 + OdGeVector3d::kYAxis * boxItemSize / 4.0;

  pLineTop->setStartPoint( ptStartTop );
  pLineTop->setEndPoint( ptEndTop );

  OdDgLine3dPtr pLineBottom = OdDgLine3d::createObject();

  pLineBottom->setColorIndex( iColor );

  OdGePoint3d ptStartBottom = ptCenter - OdGeVector3d::kXAxis * boxItemSize / 3.0 - OdGeVector3d::kYAxis * boxItemSize / 4.0;
  OdGePoint3d ptEndBottom   = ptCenter + OdGeVector3d::kXAxis * boxItemSize / 3.0 - OdGeVector3d::kYAxis * boxItemSize / 4.0;

  pLineBottom->setStartPoint( ptStartBottom );
  pLineBottom->setEndPoint( ptEndBottom );

  pCell->add( pLineTop );
  pCell->add( pLineBottom );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addLine2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgLine2dPtr pLineTop = OdDgLine2d::createObject();

  pLineTop->setColorIndex( iColor );

  OdGePoint2d ptStartTop = ptCenter - OdGeVector2d::kXAxis * boxItemSize / 3.0 + OdGeVector2d::kYAxis * boxItemSize / 4.0;
  OdGePoint2d ptEndTop   = ptCenter + OdGeVector2d::kXAxis * boxItemSize / 3.0 + OdGeVector2d::kYAxis * boxItemSize / 4.0;

  pLineTop->setStartPoint( ptStartTop );
  pLineTop->setEndPoint( ptEndTop );

  OdDgLine2dPtr pLineBottom = OdDgLine2d::createObject();

  pLineBottom->setColorIndex( iColor );

  OdGePoint2d ptStartBottom = ptCenter - OdGeVector2d::kXAxis * boxItemSize / 3.0 - OdGeVector2d::kYAxis * boxItemSize / 4.0;
  OdGePoint2d ptEndBottom   = ptCenter + OdGeVector2d::kXAxis * boxItemSize / 3.0 - OdGeVector2d::kYAxis * boxItemSize / 4.0;

  pLineBottom->setStartPoint( ptStartBottom );
  pLineBottom->setEndPoint( ptEndBottom );

  pCell->add( pLineTop );
  pCell->add( pLineBottom );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addLineString3d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgLineString3dPtr pLineStr = OdDgLineString3d::createObject();

  pLineStr->setColorIndex( iColor );

  OdGePoint3d ptsLine[7];
  
  ptsLine[0] = ptCenter - OdGeVector3d::kXAxis * boxItemSize / 3.0 + OdGeVector3d::kYAxis * boxItemSize / 4.0;
  ptsLine[1] = ptCenter + OdGeVector3d::kXAxis * boxItemSize / 3.0 + OdGeVector3d::kYAxis * boxItemSize / 4.0;
  ptsLine[2] = ptCenter + OdGeVector3d::kXAxis * boxItemSize *(1.0/9.0 );
  ptsLine[3] = ptCenter + OdGeVector3d::kXAxis * boxItemSize / 3.0 - OdGeVector3d::kYAxis * boxItemSize / 4.0;
  ptsLine[4] = ptCenter - OdGeVector3d::kXAxis * boxItemSize / 3.0 - OdGeVector3d::kYAxis * boxItemSize / 4.0;
  ptsLine[5] = ptCenter - OdGeVector3d::kXAxis * boxItemSize *(1.0/9.0 );
  ptsLine[6] = ptsLine[0];

  pLineStr->addVertex(ptsLine[0]);
  pLineStr->addVertex(ptsLine[1]);
  pLineStr->addVertex(ptsLine[2]);
  pLineStr->addVertex(ptsLine[3]);
  pLineStr->addVertex(ptsLine[4]);
  pLineStr->addVertex(ptsLine[5]);
  pLineStr->addVertex(ptsLine[6]);

  pCell->add( pLineStr );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addLineString2d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgLineString2dPtr pLineStr = OdDgLineString2d::createObject();

  pLineStr->setColorIndex( iColor );

  OdGePoint2d ptsLine[7];

  ptsLine[0] = ptCenter - OdGeVector2d::kXAxis * boxItemSize / 3.0 + OdGeVector2d::kYAxis * boxItemSize / 4.0;
  ptsLine[1] = ptCenter + OdGeVector2d::kXAxis * boxItemSize / 3.0 + OdGeVector2d::kYAxis * boxItemSize / 4.0;
  ptsLine[2] = ptCenter + OdGeVector2d::kXAxis * boxItemSize *(1.0/9.0 );
  ptsLine[3] = ptCenter + OdGeVector2d::kXAxis * boxItemSize / 3.0 - OdGeVector2d::kYAxis * boxItemSize / 4.0;
  ptsLine[4] = ptCenter - OdGeVector2d::kXAxis * boxItemSize / 3.0 - OdGeVector2d::kYAxis * boxItemSize / 4.0;
  ptsLine[5] = ptCenter - OdGeVector2d::kXAxis * boxItemSize *(1.0/9.0 );
  ptsLine[6] = ptsLine[0];

  pLineStr->addVertex(ptsLine[0]);
  pLineStr->addVertex(ptsLine[1]);
  pLineStr->addVertex(ptsLine[2]);
  pLineStr->addVertex(ptsLine[3]);
  pLineStr->addVertex(ptsLine[4]);
  pLineStr->addVertex(ptsLine[5]);
  pLineStr->addVertex(ptsLine[6]);

  pCell->add( pLineStr );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addArc3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgArc3dPtr pArc = OdDgArc3d::createObject();

  pArc->setColorIndex( iColor );
  pArc->setPrimaryAxis( boxItemSize/3.0 );
  pArc->setSecondaryAxis( boxItemSize/5.0 );
  pArc->setStartAngle( OdaPI4 );
  pArc->setSweepAngle( 3*OdaPI/2.0 );
  pArc->setOrigin( ptCenter );

  pCell->add( pArc );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addArc2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgArc2dPtr pArc = OdDgArc2d::createObject();

  pArc->setColorIndex( iColor );
  pArc->setPrimaryAxis( boxItemSize/3.0 );
  pArc->setSecondaryAxis( boxItemSize/5.0 );
  pArc->setStartAngle( OdaPI4 );
  pArc->setSweepAngle( 3*OdaPI/2.0 );
  pArc->setOrigin( ptCenter );

  pCell->add( pArc );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addEllipse3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgEllipse3dPtr pEllipse = OdDgEllipse3d::createObject();

  pEllipse->setColorIndex( iColor );
  pEllipse->setPrimaryAxis( boxItemSize/3.0 );
  pEllipse->setSecondaryAxis( boxItemSize/5.0 );
  pEllipse->setOrigin( ptCenter );

  pCell->add( pEllipse );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addEllipse2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgEllipse2dPtr pEllipse = OdDgEllipse2d::createObject();

  pEllipse->setColorIndex( iColor );
  pEllipse->setPrimaryAxis( boxItemSize/3.0 );
  pEllipse->setSecondaryAxis( boxItemSize/5.0 );
  pEllipse->setOrigin( ptCenter );

  pCell->add( pEllipse );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addText3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgText3dPtr pText = OdDgText3d::createObject();

  pCell->add( pText );

  pText->setColorIndex( iColor );
  pText->setText(OD_T("Simple text"));
  pText->setHeightMultiplier( boxItemSize/12.0 );
  pText->setLengthMultiplier( boxItemSize/12.0 );
  pText->setJustification( OdDg::kCenterCenter );
  pText->setPosition( ptCenter );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addText2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgText2dPtr pText = OdDgText2d::createObject();

  pCell->add( pText );

  pText->setColorIndex( iColor );
  pText->setText(OD_T("Simple text"));
  pText->setHeightMultiplier( boxItemSize/12.0 );
  pText->setLengthMultiplier( boxItemSize/12.0 );
  pText->setJustification( OdDg::kCenterCenter );
  pText->setPosition( ptCenter );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addTextNode3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgTextNode3dPtr pTextNode = OdDgTextNode3d::createObject();

  pTextNode->setColorIndex( iColor );
  pTextNode->setHeightMultiplier( boxItemSize/14.0 );
  pTextNode->setLengthMultiplier( boxItemSize/14.0 );
  pTextNode->setOrigin( ptCenter - OdGeVector3d::kXAxis * boxItemSize/2.1 - OdGeVector3d::kYAxis * boxItemSize/3.0 );

  pCell->add( pTextNode );

  OdDgText3dPtr pStr1 = OdDgText3d::createObject();

  pTextNode->add( pStr1 );

  pStr1->setColorIndex( iColor );
  pStr1->setText(OD_T("First string"));
  pStr1->setHeightMultiplier( boxItemSize/14.0 );
  pStr1->setLengthMultiplier( boxItemSize/14.0 );
  pStr1->setJustification( OdDg::kCenterCenter );
  pStr1->setPosition( ptCenter + OdGeVector3d::kYAxis * boxItemSize / 4.0 );

  OdDgText3dPtr pStr2 = OdDgText3d::createObject();

  pTextNode->add( pStr2 );

  pStr2->setColorIndex( iColor );
  pStr2->setText(OD_T("Second string"));
  pStr2->setHeightMultiplier( boxItemSize/14.0 );
  pStr2->setLengthMultiplier( boxItemSize/14.0 );
  pStr2->setJustification( OdDg::kCenterCenter );
  pStr2->setPosition( ptCenter - OdGeVector3d::kYAxis * boxItemSize / 4.0 );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addTextNode2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgTextNode2dPtr pTextNode = OdDgTextNode2d::createObject();

  pTextNode->setColorIndex( iColor );
  pTextNode->setHeightMultiplier( boxItemSize/16.0 );
  pTextNode->setLengthMultiplier( boxItemSize/16.0 );
  pTextNode->setOrigin( ptCenter - OdGeVector2d::kXAxis * boxItemSize/2.1 - OdGeVector2d::kYAxis * boxItemSize/3.0 );

  pCell->add( pTextNode );

  OdDgText2dPtr pStr1 = OdDgText2d::createObject();

  pTextNode->add( pStr1 );

  pStr1->setColorIndex( iColor );
  pStr1->setText(OD_T("First string"));
  pStr1->setHeightMultiplier( boxItemSize/16.0 );
  pStr1->setLengthMultiplier( boxItemSize/16.0 );
  pStr1->setJustification( OdDg::kCenterCenter );
  pStr1->setPosition( ptCenter + OdGeVector2d::kYAxis * boxItemSize / 4.0 );

  OdDgText2dPtr pStr2 = OdDgText2d::createObject();

  pTextNode->add( pStr2 );

  pStr2->setColorIndex( iColor );
  pStr2->setText(OD_T("Second string"));
  pStr2->setHeightMultiplier( boxItemSize/16.0 );
  pStr2->setLengthMultiplier( boxItemSize/16.0 );
  pStr2->setJustification( OdDg::kCenterCenter );
  pStr2->setPosition( ptCenter - OdGeVector2d::kYAxis * boxItemSize / 4.0 );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addAttribute3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgTagElementPtr pTag = OdDgTagElement::createObject();

  pCell->add( pTag );

  pTag->setColorIndex( iColor );
  pTag->setDataType( OdDgTagDefinition::kChar );
  pTag->setString(OD_T("Tag element"));
  pTag->setHeightMultiplier( boxItemSize/12.0 );
  pTag->setLengthMultiplier( boxItemSize/12.0 );
  pTag->setJustification( OdDg::kCenterCenter );
  pTag->setOrigin( ptCenter );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addAttribute2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgTagElementPtr pTag = OdDgTagElement::createObject();

  pCell->add( pTag );

  pTag->setColorIndex( iColor );
  pTag->setDataType( OdDgTagDefinition::kChar );
  pTag->setString(OD_T("Tag element"));
  pTag->setHeightMultiplier( boxItemSize/12.0 );
  pTag->setLengthMultiplier( boxItemSize/12.0 );
  pTag->setJustification( OdDg::kCenterCenter );
  pTag->setOrigin( OdGePoint3d(ptCenter.x,ptCenter.y,0) );
  pTag->setRotation2d( 0 );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addDimension3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  OdDgDimPoint ptDim;

  pCell->add( pDim );

  pDim->setColorIndex( iColor );

  ptDim.setPoint( OdGePoint3d( ptCenter - OdGeVector3d::kXAxis * boxItemSize / 3.0 - OdGeVector3d::kYAxis * boxItemSize / 4.0) );
  ptDim.setOffsetToDimLine( boxItemSize / 2.0 );
  ptDim.setJustification( OdDgDimTextInfo::kCenterLeftText );
  pDim->addPoint( ptDim );

  ptDim.setPoint( OdGePoint3d( ptCenter + OdGeVector3d::kXAxis * boxItemSize / 3.0 - OdGeVector3d::kYAxis * boxItemSize / 4.0) );
  ptDim.setOffsetToDimLine( boxItemSize / 2.0 );
  ptDim.setJustification( OdDgDimTextInfo::kCenterLeftText );
  pDim->addPoint( ptDim );

  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal4 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal4 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion(1.0, 0.0, 0.0, 0.0 ) );

  OdDgDimGeometry geom;
  geom.setDefaultValues();
  pDim->setDimGeometry( geom );

  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setFontEntryId(0);
  info.setColorIndex( iColor );
  pDim->setDimTextInfo( info );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addDimension2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRoatate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  OdDgDimPoint ptDim;

  pCell->add( pDim );

  pDim->setColorIndex( iColor );

  OdGePoint3d ptCenter3d;

  ptCenter3d.x = ptCenter.x;
  ptCenter3d.y = ptCenter.y;
  ptCenter3d.z = 0;

  ptDim.setPoint( OdGePoint3d( ptCenter3d - OdGeVector3d::kXAxis * boxItemSize / 3.0 - OdGeVector3d::kYAxis * boxItemSize / 4.0) );
  ptDim.setOffsetToDimLine( boxItemSize / 2.0 );
  ptDim.setJustification( OdDgDimTextInfo::kCenterLeftText );
  pDim->addPoint( ptDim );

  ptDim.setPoint( OdGePoint3d( ptCenter3d + OdGeVector3d::kXAxis * boxItemSize / 3.0 - OdGeVector3d::kYAxis * boxItemSize / 4.0) );
  ptDim.setOffsetToDimLine( boxItemSize / 2.0 );
  ptDim.setJustification( OdDgDimTextInfo::kCenterLeftText );
  pDim->addPoint( ptDim );

  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal4 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal4 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeMatrix2d::kIdentity );

  OdDgDimGeometry geom;
  geom.setDefaultValues();
  pDim->setDimGeometry( geom );

  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setFontEntryId(0);
  info.setColorIndex( iColor );
  pDim->setDimTextInfo( info );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRoatate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addShape3d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();

  pShape->setColorIndex( iColor );

  OdGePoint3d ptsLine[7];

  ptsLine[0] = ptCenter - OdGeVector3d::kXAxis * boxItemSize *(1.0/9.0) + OdGeVector3d::kYAxis * boxItemSize / 4.0;
  ptsLine[1] = ptCenter + OdGeVector3d::kXAxis * boxItemSize *(1.0/9.0) + OdGeVector3d::kYAxis * boxItemSize / 4.0;
  ptsLine[2] = ptCenter + OdGeVector3d::kXAxis * boxItemSize / 3.0;
  ptsLine[3] = ptCenter + OdGeVector3d::kXAxis * boxItemSize *(1.0/9.0) - OdGeVector3d::kYAxis * boxItemSize / 4.0;
  ptsLine[4] = ptCenter - OdGeVector3d::kXAxis * boxItemSize *(1.0/9.0) - OdGeVector3d::kYAxis * boxItemSize / 4.0;
  ptsLine[5] = ptCenter - OdGeVector3d::kXAxis * boxItemSize / 3.0;
  ptsLine[6] = ptsLine[0];

  pShape->addVertex(ptsLine[0]);
  pShape->addVertex(ptsLine[1]);
  pShape->addVertex(ptsLine[2]);
  pShape->addVertex(ptsLine[3]);
  pShape->addVertex(ptsLine[4]);
  pShape->addVertex(ptsLine[5]);
  pShape->addVertex(ptsLine[6]);

  pCell->add( pShape );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addShape2d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgShape2dPtr pShape = OdDgShape2d::createObject();

  pShape->setColorIndex( iColor );

  OdGePoint2d ptsLine[7];

  ptsLine[0] = ptCenter - OdGeVector2d::kXAxis * boxItemSize *(1.0/9.0) + OdGeVector2d::kYAxis * boxItemSize / 4.0;
  ptsLine[1] = ptCenter + OdGeVector2d::kXAxis * boxItemSize *(1.0/9.0) + OdGeVector2d::kYAxis * boxItemSize / 4.0;
  ptsLine[2] = ptCenter + OdGeVector2d::kXAxis * boxItemSize / 3.0;
  ptsLine[3] = ptCenter + OdGeVector2d::kXAxis * boxItemSize *(1.0/9.0) - OdGeVector2d::kYAxis * boxItemSize / 4.0;
  ptsLine[4] = ptCenter - OdGeVector2d::kXAxis * boxItemSize *(1.0/9.0) - OdGeVector2d::kYAxis * boxItemSize / 4.0;
  ptsLine[5] = ptCenter - OdGeVector2d::kXAxis * boxItemSize / 3.0;
  ptsLine[6] = ptsLine[0];

  pShape->addVertex(ptsLine[0]);
  pShape->addVertex(ptsLine[1]);
  pShape->addVertex(ptsLine[2]);
  pShape->addVertex(ptsLine[3]);
  pShape->addVertex(ptsLine[4]);
  pShape->addVertex(ptsLine[5]);
  pShape->addVertex(ptsLine[6]);

  pCell->add( pShape );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addComplexShape3d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgComplexShapePtr pComplexShape = OdDgComplexShape::createObject();

  pComplexShape->setColorIndex( iColor );

  pCell->add( pComplexShape );

  OdDgArc3dPtr pArc1 = OdDgArc3d::createObject();

  pArc1->setColorIndex( iColor );
  pArc1->setOrigin( ptCenter - OdGeVector3d::kXAxis * boxItemSize / 5.0 );
  pArc1->setPrimaryAxis( boxItemSize / 4.0 );
  pArc1->setSecondaryAxis( boxItemSize / 4.0 );
  pArc1->setStartAngle( OdaPI2 );
  pArc1->setSweepAngle( OdaPI );

  OdDgArc3dPtr pArc2 = OdDgArc3d::createObject();

  pArc2->setColorIndex( iColor );
  pArc2->setOrigin( ptCenter + OdGeVector3d::kXAxis * boxItemSize / 5.0 );
  pArc2->setPrimaryAxis( boxItemSize / 4.0 );
  pArc2->setSecondaryAxis( boxItemSize / 4.0 );
  pArc2->setStartAngle( -OdaPI2 );
  pArc2->setSweepAngle( OdaPI );

  OdDgLineString3dPtr pLine1 = OdDgLineString3d::createObject();

  pLine1->setColorIndex( iColor );

  OdGePoint3d ptsLine[3];

  ptsLine[0] = ptCenter - OdGeVector3d::kXAxis * boxItemSize / 5.0 - OdGeVector3d::kYAxis * boxItemSize / 4.0;
  ptsLine[1] = ptCenter - OdGeVector3d::kYAxis * boxItemSize / 9.0;
  ptsLine[2] = ptCenter + OdGeVector3d::kXAxis * boxItemSize / 5.0 - OdGeVector3d::kYAxis * boxItemSize / 4.0;

  pLine1->addVertex(ptsLine[0]);
  pLine1->addVertex(ptsLine[1]);
  pLine1->addVertex(ptsLine[2]);

  OdDgLineString3dPtr pLine2 = OdDgLineString3d::createObject();

  pLine2->setColorIndex( iColor );

  ptsLine[0] = ptCenter + OdGeVector3d::kXAxis * boxItemSize / 5.0 + OdGeVector3d::kYAxis * boxItemSize / 4.0;
  ptsLine[1] = ptCenter + OdGeVector3d::kYAxis * boxItemSize / 9.0;
  ptsLine[2] = ptCenter - OdGeVector3d::kXAxis * boxItemSize / 5.0 + OdGeVector3d::kYAxis * boxItemSize / 4.0;

  pLine2->addVertex(ptsLine[0]);
  pLine2->addVertex(ptsLine[1]);
  pLine2->addVertex(ptsLine[2]);

  pComplexShape->add( pArc1 );
  pComplexShape->add( pLine1 );
  pComplexShape->add( pArc2 );
  pComplexShape->add( pLine2 );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addComplexShape2d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgComplexShapePtr pComplexShape = OdDgComplexShape::createObject();

  pComplexShape->setColorIndex( iColor );

  pCell->add( pComplexShape );

  OdDgArc2dPtr pArc1 = OdDgArc2d::createObject();

  pArc1->setColorIndex( iColor );
  pArc1->setOrigin( ptCenter - OdGeVector2d::kXAxis * boxItemSize / 5.0 );
  pArc1->setPrimaryAxis( boxItemSize / 4.0 );
  pArc1->setSecondaryAxis( boxItemSize / 4.0 );
  pArc1->setStartAngle( OdaPI2 );
  pArc1->setSweepAngle( OdaPI );

  OdDgArc2dPtr pArc2 = OdDgArc2d::createObject();

  pArc2->setColorIndex( iColor );
  pArc2->setOrigin( ptCenter + OdGeVector2d::kXAxis * boxItemSize / 5.0 );
  pArc2->setPrimaryAxis( boxItemSize / 4.0 );
  pArc2->setSecondaryAxis( boxItemSize / 4.0 );
  pArc2->setStartAngle( -OdaPI2 );
  pArc2->setSweepAngle( OdaPI );

  OdDgLineString2dPtr pLine1 = OdDgLineString2d::createObject();

  pLine1->setColorIndex( iColor );

  OdGePoint2d ptsLine[3];

  ptsLine[0] = ptCenter - OdGeVector2d::kXAxis * boxItemSize / 5.0 - OdGeVector2d::kYAxis * boxItemSize / 4.0;
  ptsLine[1] = ptCenter - OdGeVector2d::kYAxis * boxItemSize / 9.0;
  ptsLine[2] = ptCenter + OdGeVector2d::kXAxis * boxItemSize / 5.0 - OdGeVector2d::kYAxis * boxItemSize / 4.0;

  pLine1->addVertex(ptsLine[0]);
  pLine1->addVertex(ptsLine[1]);
  pLine1->addVertex(ptsLine[2]);

  OdDgLineString2dPtr pLine2 = OdDgLineString2d::createObject();

  pLine2->setColorIndex( iColor );

  ptsLine[0] = ptCenter + OdGeVector2d::kXAxis * boxItemSize / 5.0 + OdGeVector2d::kYAxis * boxItemSize / 4.0;
  ptsLine[1] = ptCenter + OdGeVector2d::kYAxis * boxItemSize / 9.0;
  ptsLine[2] = ptCenter - OdGeVector2d::kXAxis * boxItemSize / 5.0 + OdGeVector2d::kYAxis * boxItemSize / 4.0;

  pLine2->addVertex(ptsLine[0]);
  pLine2->addVertex(ptsLine[1]);
  pLine2->addVertex(ptsLine[2]);

  pComplexShape->add( pArc1 );
  pComplexShape->add( pLine1 );
  pComplexShape->add( pArc2 );
  pComplexShape->add( pLine2 );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addComplexString3d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgComplexStringPtr pComplexString = OdDgComplexString::createObject();

  pComplexString->setColorIndex( iColor );

  pCell->add( pComplexString );

  OdDgArc3dPtr pArc1 = OdDgArc3d::createObject();

  pArc1->setColorIndex( iColor );
  pArc1->setOrigin( ptCenter - OdGeVector3d::kXAxis * boxItemSize * 2.0 / 5.0 );
  pArc1->setPrimaryAxis( boxItemSize / 4.0 );
  pArc1->setSecondaryAxis( boxItemSize / 4.0 );
  pArc1->setStartAngle( OdaPI2 );
  pArc1->setSweepAngle( -OdaPI );

  OdDgArc3dPtr pArc2 = OdDgArc3d::createObject();

  pArc2->setColorIndex( iColor );
  pArc2->setOrigin( ptCenter + OdGeVector3d::kXAxis * boxItemSize * 2.0 / 5.0 );
  pArc2->setPrimaryAxis( boxItemSize / 4.0 );
  pArc2->setSecondaryAxis( boxItemSize / 4.0 );
  pArc2->setStartAngle( -OdaPI2 );
  pArc2->setSweepAngle( -OdaPI );

  OdDgLineString3dPtr pLine1 = OdDgLineString3d::createObject();

  pLine1->setColorIndex( iColor );

  OdGePoint3d ptsLine[3];

  ptsLine[0] = ptCenter - OdGeVector3d::kXAxis * boxItemSize * 2.0 / 5.0 - OdGeVector3d::kYAxis * boxItemSize / 4.0;
  ptsLine[1] = ptCenter - OdGeVector3d::kYAxis * boxItemSize / 9.0;
  ptsLine[2] = ptCenter + OdGeVector3d::kXAxis * boxItemSize * 2.0 / 5.0 - OdGeVector3d::kYAxis * boxItemSize / 4.0;

  pLine1->addVertex(ptsLine[0]);
  pLine1->addVertex(ptsLine[1]);
  pLine1->addVertex(ptsLine[2]);

  OdDgLineString3dPtr pLine2 = OdDgLineString3d::createObject();

  pLine2->setColorIndex( iColor );

  ptsLine[0] = ptCenter + OdGeVector3d::kXAxis * boxItemSize * 2.0 / 5.0 + OdGeVector3d::kYAxis * boxItemSize / 4.0;
  ptsLine[1] = ptCenter + OdGeVector3d::kYAxis * boxItemSize / 9.0;
  ptsLine[2] = ptCenter - OdGeVector3d::kXAxis * boxItemSize * 2.0 / 5.0 + OdGeVector3d::kYAxis * boxItemSize / 4.0;

  pLine2->addVertex(ptsLine[0]);
  pLine2->addVertex(ptsLine[1]);
  pLine2->addVertex(ptsLine[2]);

  pComplexString->add( pArc1 );
  pComplexString->add( pLine1 );
  pComplexString->add( pArc2 );
  pComplexString->add( pLine2 );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addComplexString2d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgComplexStringPtr pComplexString = OdDgComplexString::createObject();

  pComplexString->setColorIndex( iColor );

  pCell->add( pComplexString );

  OdDgArc2dPtr pArc1 = OdDgArc2d::createObject();

  pArc1->setColorIndex( iColor );
  pArc1->setOrigin( ptCenter - OdGeVector2d::kXAxis * boxItemSize * 2.0 / 5.0 );
  pArc1->setPrimaryAxis( boxItemSize / 4.0 );
  pArc1->setSecondaryAxis( boxItemSize / 4.0 );
  pArc1->setStartAngle( OdaPI2 );
  pArc1->setSweepAngle( -OdaPI );

  OdDgArc2dPtr pArc2 = OdDgArc2d::createObject();

  pArc2->setColorIndex( iColor );
  pArc2->setOrigin( ptCenter + OdGeVector2d::kXAxis * boxItemSize * 2.0 / 5.0 );
  pArc2->setPrimaryAxis( boxItemSize / 4.0 );
  pArc2->setSecondaryAxis( boxItemSize / 4.0 );
  pArc2->setStartAngle( -OdaPI2 );
  pArc2->setSweepAngle( -OdaPI );

  OdDgLineString2dPtr pLine1 = OdDgLineString2d::createObject();

  pLine1->setColorIndex( iColor );

  OdGePoint2d ptsLine[3];

  ptsLine[0] = ptCenter - OdGeVector2d::kXAxis * boxItemSize * 2.0 / 5.0 - OdGeVector2d::kYAxis * boxItemSize / 4.0;
  ptsLine[1] = ptCenter - OdGeVector2d::kYAxis * boxItemSize / 9.0;
  ptsLine[2] = ptCenter + OdGeVector2d::kXAxis * boxItemSize * 2.0 / 5.0 - OdGeVector2d::kYAxis * boxItemSize / 4.0;

  pLine1->addVertex(ptsLine[0]);
  pLine1->addVertex(ptsLine[1]);
  pLine1->addVertex(ptsLine[2]);

  OdDgLineString2dPtr pLine2 = OdDgLineString2d::createObject();

  pLine2->setColorIndex( iColor );

  ptsLine[0] = ptCenter + OdGeVector2d::kXAxis * boxItemSize * 2.0 / 5.0 + OdGeVector2d::kYAxis * boxItemSize / 4.0;
  ptsLine[1] = ptCenter + OdGeVector2d::kYAxis * boxItemSize / 9.0;
  ptsLine[2] = ptCenter - OdGeVector2d::kXAxis * boxItemSize * 2.0 / 5.0 + OdGeVector2d::kYAxis * boxItemSize / 4.0;

  pLine2->addVertex(ptsLine[0]);
  pLine2->addVertex(ptsLine[1]);
  pLine2->addVertex(ptsLine[2]);

  pComplexString->add( pArc1 );
  pComplexString->add( pLine1 );
  pComplexString->add( pArc2 );
  pComplexString->add( pLine2 );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addCellHeader3d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgCellHeader3dPtr pSubCell1 = OdDgCellHeader3d::createObject();

  pSubCell1->setColorIndex( iColor );

  OdDgCellHeader3dPtr pSubCell2 = OdDgCellHeader3d::createObject();

  pSubCell1->setColorIndex( iColor );

  pCell->add( pSubCell1 );
  pCell->add( pSubCell2 );

  OdDgEllipse3dPtr pEllip1 = OdDgEllipse3d::createObject();
  pEllip1->setColorIndex( iColor );
  pEllip1->setPrimaryAxis( boxItemSize / 20.0 );
  pEllip1->setSecondaryAxis( boxItemSize / 20.0 );
  pEllip1->setOrigin( ptCenter - OdGeVector3d::kXAxis * boxItemSize / 8.0 );

  OdDgEllipse3dPtr pEllip2 = OdDgEllipse3d::createObject();
  pEllip2->setColorIndex( iColor );
  pEllip2->setPrimaryAxis( boxItemSize / 20.0 );
  pEllip2->setSecondaryAxis( boxItemSize / 20.0 );
  pEllip2->setOrigin( ptCenter - OdGeVector3d::kXAxis * boxItemSize *3.0 / 8.0 );

  OdDgLine3dPtr pLine1 = OdDgLine3d::createObject();

  pLine1->setColorIndex( iColor );
  pLine1->setStartPoint( ptCenter - OdGeVector3d::kXAxis * boxItemSize / 4.0 - OdGeVector3d::kYAxis * boxItemSize / 6.0 );
  pLine1->setEndPoint( ptCenter - OdGeVector3d::kXAxis * boxItemSize / 4.0 + OdGeVector3d::kYAxis * boxItemSize / 6.0 );

  pSubCell1->add( pEllip1 );
  pSubCell1->add( pEllip2 );
  pSubCell1->add( pLine1 );

  pSubCell1->setOrigin( ptCenter - OdGeVector3d::kXAxis * boxItemSize / 4.0 );
  pSubCell1->setTransformation( OdGeMatrix3d::rotation(OdaPI4, OdGeVector3d::kZAxis)  );

  OdDgEllipse3dPtr pEllip3 = OdDgEllipse3d::createObject();
  pEllip3->setColorIndex( iColor );
  pEllip3->setPrimaryAxis( boxItemSize / 20.0 );
  pEllip3->setSecondaryAxis( boxItemSize / 20.0 );
  pEllip3->setOrigin( ptCenter + OdGeVector3d::kXAxis * boxItemSize / 8.0 );

  OdDgEllipse3dPtr pEllip4 = OdDgEllipse3d::createObject();
  pEllip4->setColorIndex( iColor );
  pEllip4->setPrimaryAxis( boxItemSize / 20.0 );
  pEllip4->setSecondaryAxis( boxItemSize / 20.0 );
  pEllip4->setOrigin( ptCenter + OdGeVector3d::kXAxis * boxItemSize *3.0 / 8.0 );

  OdDgLine3dPtr pLine2 = OdDgLine3d::createObject();

  pLine2->setColorIndex( iColor );
  pLine2->setStartPoint( ptCenter + OdGeVector3d::kXAxis * boxItemSize / 4.0 - OdGeVector3d::kYAxis * boxItemSize / 6.0 );
  pLine2->setEndPoint( ptCenter + OdGeVector3d::kXAxis * boxItemSize / 4.0 + OdGeVector3d::kYAxis * boxItemSize / 6.0 );

  pSubCell2->add( pEllip3 );
  pSubCell2->add( pEllip4 );
  pSubCell2->add( pLine2 );

  pSubCell2->setOrigin( ptCenter + OdGeVector3d::kXAxis * boxItemSize / 4.0 );
  pSubCell2->setTransformation( OdGeMatrix3d::rotation(-OdaPI4, OdGeVector3d::kZAxis)  );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addCellHeader2d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgCellHeader2dPtr pSubCell1 = OdDgCellHeader2d::createObject();

  pSubCell1->setColorIndex( iColor );

  OdDgCellHeader2dPtr pSubCell2 = OdDgCellHeader2d::createObject();

  pSubCell1->setColorIndex( iColor );

  pCell->add( pSubCell1 );
  pCell->add( pSubCell2 );

  OdDgEllipse2dPtr pEllip1 = OdDgEllipse2d::createObject();
  pEllip1->setColorIndex( iColor );
  pEllip1->setPrimaryAxis( boxItemSize / 20.0 );
  pEllip1->setSecondaryAxis( boxItemSize / 20.0 );
  pEllip1->setOrigin( ptCenter - OdGeVector2d::kXAxis * boxItemSize / 8.0 );

  OdDgEllipse2dPtr pEllip2 = OdDgEllipse2d::createObject();
  pEllip2->setColorIndex( iColor );
  pEllip2->setPrimaryAxis( boxItemSize / 20.0 );
  pEllip2->setSecondaryAxis( boxItemSize / 20.0 );
  pEllip2->setOrigin( ptCenter - OdGeVector2d::kXAxis * boxItemSize *3.0 / 8.0 );

  OdDgLine2dPtr pLine1 = OdDgLine2d::createObject();

  pLine1->setColorIndex( iColor );
  pLine1->setStartPoint( ptCenter - OdGeVector2d::kXAxis * boxItemSize / 4.0 - OdGeVector2d::kYAxis * boxItemSize / 6.0 );
  pLine1->setEndPoint( ptCenter - OdGeVector2d::kXAxis * boxItemSize / 4.0 + OdGeVector2d::kYAxis * boxItemSize / 6.0 );

  pSubCell1->add( pEllip1 );
  pSubCell1->add( pEllip2 );
  pSubCell1->add( pLine1 );

  pSubCell1->setOrigin( ptCenter - OdGeVector2d::kXAxis * boxItemSize / 4.0 );
  pSubCell1->setTransformation( OdGeMatrix2d::rotation(OdaPI4)  );

  OdDgEllipse2dPtr pEllip3 = OdDgEllipse2d::createObject();
  pEllip3->setColorIndex( iColor );
  pEllip3->setPrimaryAxis( boxItemSize / 20.0 );
  pEllip3->setSecondaryAxis( boxItemSize / 20.0 );
  pEllip3->setOrigin( ptCenter + OdGeVector2d::kXAxis * boxItemSize / 8.0 );

  OdDgEllipse2dPtr pEllip4 = OdDgEllipse2d::createObject();
  pEllip4->setColorIndex( iColor );
  pEllip4->setPrimaryAxis( boxItemSize / 20.0 );
  pEllip4->setSecondaryAxis( boxItemSize / 20.0 );
  pEllip4->setOrigin( ptCenter + OdGeVector2d::kXAxis * boxItemSize *3.0 / 8.0 );

  OdDgLine2dPtr pLine2 = OdDgLine2d::createObject();

  pLine2->setColorIndex( iColor );
  pLine2->setStartPoint( ptCenter + OdGeVector2d::kXAxis * boxItemSize / 4.0 - OdGeVector2d::kYAxis * boxItemSize / 6.0 );
  pLine2->setEndPoint( ptCenter + OdGeVector2d::kXAxis * boxItemSize / 4.0 + OdGeVector2d::kYAxis * boxItemSize / 6.0 );

  pSubCell2->add( pEllip3 );
  pSubCell2->add( pEllip4 );
  pSubCell2->add( pLine2 );

  pSubCell2->setOrigin( ptCenter + OdGeVector2d::kXAxis * boxItemSize / 4.0 );
  pSubCell2->setTransformation( OdGeMatrix2d::rotation(-OdaPI4)  );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addSolid( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgSolidPtr solid = OdDgSolid::createObject();
  OdDg3dObjectHelper helper( solid );

  solid->setColorIndex( iColor );

  pCell->add( solid );

  //make some solid
  {
    OdDgShape3dPtr shape;
    OdDgLine3dPtr line;

    double sx = boxItemSize/3.0;
    double sy = boxItemSize/3.0;
    double sz = boxItemSize/3.0;

    //boundary #1
    shape = OdDgShape3d::createObject();
    shape->setColorIndex( iColor );
    shape->addVertex( ptCenter + OdGeVector3d( 0., sy, sz ) * .8 );
    shape->addVertex( ptCenter + OdGeVector3d( 0., -sy, sz ) * .8 );
    shape->addVertex( ptCenter + OdGeVector3d( 0., -sy, -sz ) * .8 );
    shape->addVertex( ptCenter + OdGeVector3d( 0., sy, -sz ) * .8 );
    shape->addVertex( ptCenter + OdGeVector3d( 0., sy, sz ) * .8 );
    helper.addToBoundary( shape );

    //boundary #2
    shape = OdDgShape3d::createObject();
    shape->setColorIndex( iColor );
    shape->addVertex( ptCenter + OdGeVector3d( sx, sy, sz ) * .3 );
    shape->addVertex( ptCenter + OdGeVector3d( sx, -sy, sz ) * .3 );
    shape->addVertex( ptCenter + OdGeVector3d( sx, -sy, -sz ) * .3 );
    shape->addVertex( ptCenter + OdGeVector3d( sx, sy, -sz ) * .3 );
    shape->addVertex( ptCenter + OdGeVector3d( sx, sy, sz ) * .3 );
    helper.addToBoundary( shape );

    //rule #1
    line = OdDgLine3d::createObject();
    line->setColorIndex( iColor );
    line->setStartPoint( ptCenter + OdGeVector3d( 0., sy, sz ) * .8 );
    line->setEndPoint( ptCenter + OdGeVector3d( sx, sy, sz ) * .3 );
    helper.addToRule( line );

    //rule #2
    line = OdDgLine3d::createObject();
    line->setColorIndex( iColor );
    line->setStartPoint( ptCenter + OdGeVector3d( 0., -sy, sz ) * .8 );
    line->setEndPoint( ptCenter + OdGeVector3d( sx, -sy, sz ) * .3 );
    helper.addToRule( line );

    //rule #3
    line = OdDgLine3d::createObject();
    line->setColorIndex( iColor );
    line->setStartPoint( ptCenter + OdGeVector3d( 0, -sy, -sz ) * .8 );
    line->setEndPoint( ptCenter + OdGeVector3d( sx, -sy, -sz ) * .3 );
    helper.addToRule( line );

    //rule #4
    line = OdDgLine3d::createObject();
    line->setColorIndex( iColor );
    line->setStartPoint( ptCenter + OdGeVector3d( 0., sy, -sz ) * .8 );
    line->setEndPoint( ptCenter + OdGeVector3d( sx, sy, -sz ) * .3 );
    helper.addToRule( line );
  }

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addSurface( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgSurfacePtr surface = OdDgSurface::createObject();
  OdDg3dObjectHelper helper( surface );

  surface->setColorIndex( iColor );

  pCell->add( surface );

  double sx = boxItemSize / 3.0;
  double sy = boxItemSize / 3.0;
  double sz = boxItemSize / 3.0;

  OdDgEllipse3dPtr ellipse;
  OdDgLine3dPtr line;
  const double bias = .8;

  //boundary #1
  ellipse = OdDgEllipse3d::createObject();
  ellipse->setColorIndex( iColor );
  ellipse->setPrimaryAxis( sx );
  ellipse->setSecondaryAxis( sy );
  ellipse->setOrigin( ptCenter );
  helper.addToBoundary( ellipse );

  //boundary #2
  ellipse = OdDgEllipse3d::createObject();
  ellipse->setColorIndex( iColor );
  ellipse->setPrimaryAxis( sx * bias );
  ellipse->setSecondaryAxis( sy * bias );
  ellipse->setOrigin( ptCenter + OdGeVector3d( 0., 0., sz ) );
  helper.addToBoundary( ellipse );

  //rule #1
  line = OdDgLine3d::createObject();
  line->setColorIndex( iColor );
  line->setStartPoint( ptCenter + OdGeVector3d( -sx, 0., 0. ) );
  line->setEndPoint( ptCenter + OdGeVector3d( -sx * bias, 0., sz ) );
  helper.addToRule( line );

  //rule #2
  line = OdDgLine3d::createObject();
  line->setColorIndex( iColor );
  line->setStartPoint( ptCenter + OdGeVector3d( 0., sy, 0. ) );
  line->setEndPoint( ptCenter + OdGeVector3d( 0., sy * bias, sz ) );
  helper.addToRule( line );

  //rule #3
  line = OdDgLine3d::createObject();
  line->setColorIndex( iColor );
  line->setStartPoint( ptCenter + OdGeVector3d( sx, 0., 0. ) );
  line->setEndPoint( ptCenter + OdGeVector3d( sx * bias, 0., sz ) );
  helper.addToRule( line );

  //rule #4
  line = OdDgLine3d::createObject();
  line->setColorIndex( iColor );
  line->setStartPoint( ptCenter + OdGeVector3d( 0., -sy, 0. ) );
  line->setEndPoint( ptCenter + OdGeVector3d( 0., -sy * bias, sz ) );
  helper.addToRule( line );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addBSplineSurface( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgBSplineSurfacePtr surface = OdDgBSplineSurface::createObject();

  double sx = boxItemSize /6.0;
  double sy = boxItemSize /6.0;

  pCell->add( surface );

  surface->setColorIndex(iColor);

  OdGePoint3dArray arrCtrlPts;
  OdGeDoubleArray  arrEmptyWeights;
  OdGeKnotVector   arrEmptyKnots; // to set uniform knots.

  arrCtrlPts.push_back( ptCenter + OdGeVector3d( -2. * sx, -2. * sy, 0. ) ); // 0,0
  arrCtrlPts.push_back( ptCenter + OdGeVector3d( -1. * sx, -1. * sy, 0. ) ); // 1,0
  arrCtrlPts.push_back( ptCenter + OdGeVector3d(  1. * sx, -1. * sy, 0. ) ); // 2,0
  arrCtrlPts.push_back( ptCenter + OdGeVector3d(  2. * sx, -2. * sy, 0. ) ); // 3,0
  arrCtrlPts.push_back( ptCenter + OdGeVector3d( -1. * sx, -1. * sy, 0. ) ); // 0,1
  arrCtrlPts.push_back( ptCenter + OdGeVector3d( -1. * sx, -1. * sy, 2. ) ); // 1,1
  arrCtrlPts.push_back( ptCenter + OdGeVector3d(  1. * sx, -1. * sy, 2. ) ); // 2,1
  arrCtrlPts.push_back( ptCenter + OdGeVector3d(  1. * sx, -1. * sy, 0. ) ); // 3,1
  arrCtrlPts.push_back( ptCenter + OdGeVector3d( -1. * sx,  1. * sy, 0. ) ); // 0,2
  arrCtrlPts.push_back( ptCenter + OdGeVector3d( -1. * sx,  1. * sy, 2. ) ); // 1,2
  arrCtrlPts.push_back( ptCenter + OdGeVector3d(  1. * sx,  1. * sy, 2. ) ); // 2,2
  arrCtrlPts.push_back( ptCenter + OdGeVector3d(  1. * sx,  1. * sy, 0. ) ); // 3,2
  arrCtrlPts.push_back( ptCenter + OdGeVector3d( -2. * sx,  2. * sy, 0. ) ); // 0,3
  arrCtrlPts.push_back( ptCenter + OdGeVector3d( -1. * sx,  1. * sy, 0. ) ); // 1,3
  arrCtrlPts.push_back( ptCenter + OdGeVector3d(  1. * sx,  1. * sy, 0. ) ); // 2,3
  arrCtrlPts.push_back( ptCenter + OdGeVector3d(  2. * sx,  2. * sy, 0. ) ); // 3,3

  surface->set( 4, 4, false, false, false, 4, 4, arrCtrlPts, arrEmptyWeights, arrEmptyKnots, arrEmptyKnots );
  surface->setNumberOfSpansInU(10);
  surface->setNumberOfSpansInV(10);

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addSmartSolid( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  const OdUInt8 pX_B[] = {
    0x50, 0x53, 0x00, 0x00, 0x00, 0x33, 0x3A, 0x20, 0x54, 0x52, 0x41, 0x4E, 0x53, 0x4D, 0x49, 0x54 , 0x20, 0x46, 0x49, 0x4C, 0x45, 0x20, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x62, 0x79
    , 0x20, 0x6D, 0x6F, 0x64, 0x65, 0x6C, 0x6C, 0x65, 0x72, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F , 0x6E, 0x20, 0x31, 0x38, 0x30, 0x30, 0x31, 0x39, 0x37, 0x00, 0x00, 0x00, 0x11, 0x53, 0x43, 0x48
    , 0x5F, 0x31, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5F, 0x31, 0x32, 0x30, 0x30, 0x36, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x0C, 0x00, 0x02, 0x00, 0x00, 0x00, 0x17, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01
    , 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x40, 0x8F, 0x40, 0x00, 0x00, 0x00, 0x00, 0x01, 0x3E, 0x45 , 0x79, 0x8E, 0xE2, 0x30, 0x8C, 0x3A, 0x00, 0x01, 0x00, 0x04, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01
    , 0x01, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x01, 0x00, 0x08, 0x00, 0x09, 0x00, 0x01, 0x00 , 0x46, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00
    , 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0A, 0x00 , 0x0A, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x0D, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01
    , 0x00, 0x02, 0x00, 0x01, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x01, 0x00, 0x0C, 0x00, 0x01, 0x00, 0x33 , 0x00, 0x06, 0x00, 0x00, 0x00, 0x12, 0x00, 0x01, 0x00, 0x0D, 0x00, 0x0E, 0x00, 0x01, 0x00, 0x01
    , 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0xBF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x00, 0x1F, 0x00, 0x07, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x09, 0x00, 0x0F, 0x00
    , 0x01, 0x00, 0x01, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0xF0, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02
    , 0x00, 0x0C, 0x00, 0x01, 0x00, 0x10, 0x56, 0x00, 0x10, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0C, 0x00 , 0x11, 0xC2, 0xBC, 0x92, 0x8F, 0x99, 0x6E, 0x00, 0x00, 0x00, 0x12, 0x00, 0x01, 0x00, 0x13, 0x00
    , 0x07, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x51, 0x00, 0x00, 0x00, 0x01, 0x00, 0x11, 0x00 , 0x00, 0x00, 0x16, 0x00, 0x14, 0x00, 0x09, 0x00, 0x01, 0x00, 0x01, 0x00, 0x15, 0x00, 0x16, 0x00
    , 0x17, 0x00, 0x11, 0x00, 0x12, 0x00, 0x01, 0x00, 0x18, 0x00, 0x12, 0x00, 0x12, 0x00, 0x01, 0x00 , 0x19, 0x00, 0x09, 0x00, 0x01, 0x00, 0x01, 0x2B, 0x00, 0x10, 0x00, 0x13, 0x00, 0x00, 0x00, 0x06
    , 0x00, 0x16, 0xC2, 0xBC, 0x92, 0x8F, 0x99, 0x6E, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x09, 0x00, 0x01 , 0x00, 0x0F, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x51, 0x00, 0x00, 0x00, 0x01, 0x00, 0x16
    , 0x00, 0x00, 0x00, 0x17, 0x00, 0x14, 0x00, 0x13, 0x00, 0x01, 0x00, 0x01, 0x00, 0x11, 0x00, 0x01 , 0x00, 0x1B, 0x00, 0x11, 0x00, 0x1A, 0x00, 0x01, 0x00, 0x1C, 0x00, 0x1A, 0x00, 0x1A, 0x00, 0x01
    , 0x00, 0x1D, 0x00, 0x13, 0x00, 0x01, 0x00, 0x01, 0x2B, 0x00, 0x1F, 0x00, 0x0F, 0x00, 0x00, 0x00 , 0x0A, 0x00, 0x01, 0x00, 0x13, 0x00, 0x01, 0x00, 0x07, 0x00, 0x01, 0x2B, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0xF0, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x1C , 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00, 0x1A, 0x00, 0x0D, 0x00, 0x01, 0x00, 0x11, 0x00, 0x1D
    , 0x00, 0x01, 0x00, 0x1E, 0x00, 0x1D, 0x00, 0x1D, 0x00, 0x01, 0x00, 0x1A, 0x00, 0x13, 0x00, 0x01 , 0x00, 0x01, 0x2D, 0x00, 0x0F, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x08, 0x00, 0x01, 0x00, 0x1D, 0x00
    , 0x1F, 0x00, 0x01, 0x00, 0x0E, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x09, 0x00, 0x20, 0xC2, 0xBC, 0x92 , 0x8F, 0x99, 0x6E, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x0B, 0x00, 0x1E, 0x00, 0x05, 0x00, 0x21, 0x2B
    , 0x00, 0x01, 0x00, 0x01, 0x00, 0x0D, 0x00, 0x0B, 0x00, 0x10, 0x00, 0x51, 0x00, 0x00, 0x00, 0x01 , 0x00, 0x20, 0x00, 0x00, 0x00, 0x14, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x01, 0x00, 0x01, 0x00, 0x22
    , 0x00, 0x15, 0x00, 0x23, 0x00, 0x0E, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x02, 0x00, 0x15, 0xC2, 0xBC , 0x92, 0x8F, 0x99, 0x6E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x00, 0x18, 0x00, 0x05, 0x00, 0x06
    , 0x2B, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x1F, 0x00, 0x10, 0x00, 0x0E, 0x00, 0x0B, 0x00 , 0x00, 0x00, 0x0F, 0x00, 0x22, 0xC2, 0xBC, 0x92, 0x8F, 0x99, 0x6E, 0x00, 0x00, 0x00, 0x1F, 0x00
    , 0x01, 0x00, 0x24, 0x00, 0x05, 0x00, 0x0E, 0x2B, 0x00, 0x01, 0x00, 0x01, 0x00, 0x1F, 0x00, 0x01 , 0x00, 0x10, 0x00, 0x32, 0x00, 0x21, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x1F, 0x00, 0x01
    , 0x00, 0x0E, 0x00, 0x01, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0xF0, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00 , 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00
    , 0x01, 0x00, 0x08, 0x00, 0x0B, 0x00, 0x32, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x11, 0x00, 0x01, 0x00 , 0x0B, 0x00, 0x21, 0x00, 0x06, 0x00, 0x01, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x51, 0x00, 0x00, 0x00, 0x01, 0x00, 0x22, 0x00, 0x00, 0x00, 0x13, 0x00, 0x14, 0x00, 0x0B , 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x20, 0x00, 0x25, 0x00, 0x0F, 0x00, 0x24, 0x00, 0x00
    , 0x00, 0x0E, 0x00, 0x01, 0x00, 0x19, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x11, 0x00, 0x19, 0x00, 0x01 , 0x00, 0x24, 0x00, 0x19, 0x00, 0x19, 0x00, 0x01, 0x00, 0x12, 0x00, 0x09, 0x00, 0x01, 0x00, 0x01
    , 0x2D, 0x00, 0x50, 0x00, 0x00, 0x00, 0x01, 0x00, 0x14, 0x00, 0x26, 0x00, 0x27, 0x00, 0x00, 0x23 , 0x28, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x52, 0x00, 0x00, 0x00, 0x02, 0x00, 0x25 , 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x4F, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x27
    , 0x42, 0x53, 0x49, 0x5F, 0x45, 0x6E, 0x74, 0x69, 0x74, 0x79, 0x49, 0x64, 0x00, 0x51, 0x00, 0x00 , 0x00, 0x01, 0x00, 0x15, 0x00, 0x00, 0x00, 0x15, 0x00, 0x14, 0x00, 0x0D, 0x00, 0x01, 0x00, 0x01
    , 0x00, 0x20, 0x00, 0x11, 0x00, 0x28, 0x00, 0x0F, 0x00, 0x18, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x01 , 0x00, 0x12, 0x00, 0x0D, 0x00, 0x1C, 0x00, 0x52, 0x00, 0x00, 0x00, 0x02, 0x00, 0x28, 0x00, 0x00
    , 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x52, 0x00, 0x00, 0x00, 0x02, 0x00, 0x23, 0x00, 0x00 , 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x52, 0x00, 0x00, 0x00, 0x02, 0x00, 0x1B, 0x00, 0x00
    , 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x52, 0x00, 0x00, 0x00, 0x02, 0x00, 0x17, 0x00, 0x00 , 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x13, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x05, 0x00, 0x01
    , 0x00, 0x02, 0x00, 0x01, 0x00, 0x08, 0x00, 0x05, 0x53, 0x00, 0x4A, 0x00, 0x00, 0x00, 0x14, 0x00 , 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x16, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00
    , 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00 , 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00
    , 0x01, 0x00, 0x01 };

  OdStreamBufPtr pBuf = OdMemoryStream::createNew();
  pBuf->putBytes(pX_B, sizeof(pX_B));
  pBuf->rewind();

  OdDgCellHeader3dPtr smartSolidCell = OdDgCellHeader3d::createObject();
  smartSolidCell->setColorIndex( iColor );

  pCell->add( smartSolidCell );

  OdGeMatrix3d transformation;

  transformation.setToScaling( 2.0 );
  transformation *= OdGeMatrix3d::rotation( OdaPI2, OdGeVector3d::kYAxis );

  smartSolidCell->setOrigin( ptCenter );
  smartSolidCell->setTransformation( transformation );

  OdDgBRepEntityPEPtr(smartSolidCell)->fillSmartSolid(*smartSolidCell, *pBuf);

  OdDgElementIteratorPtr iterator = smartSolidCell->createIterator();

  for ( ; !iterator->done(); iterator->step() )
  {
    OdDgGraphicsElementPtr pItem = dynamic_cast<OdDgGraphicsElement*>(iterator->item().openObject(OdDg::kForWrite).get());

    if( !pItem.isNull() )
    {
      pItem->setColorIndex(iColor);
    }
  }

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addBSplineCurve3d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgBSplineCurve3dPtr curve = OdDgBSplineCurve3d::createObject();
  pCell->add( curve );

  double minor = boxItemSize / 6.0;
  double major = boxItemSize / 6.0;

  OdGePoint3dArray arrCtrlPts;
  arrCtrlPts.push_back(ptCenter + OdGeVector3d( -1. * major,  1. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d( -1. * major,  2. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d( -2. * major,  2. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d( -2. * major,  1. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d( -1. * major,  1. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d(  1. * major,  1. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d(  2. * major,  1. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d(  2. * major,  2. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d(  1. * major,  2. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d(  1. * major,  1. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d(  1. * major, -1. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d(  1. * major, -2. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d(  2. * major, -2. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d(  2. * major, -1. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d(  1. * major, -1. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d( -1. * major, -1. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d( -2. * major, -1. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d( -2. * major, -2. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d( -1. * major, -2. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d( -1. * major, -1. * minor, 0. ));
  arrCtrlPts.push_back(ptCenter + OdGeVector3d( -1. * major,  1. * minor, 0. ));

  OdGeKnotVector  vrEmptyKnot;
  OdGeDoubleArray arrEmptyWeights;

  curve->setNurbsData( 4, false, true, arrCtrlPts, vrEmptyKnot, arrEmptyWeights );
  curve->setColorIndex( iColor );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addBSplineCurve2d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgBSplineCurve2dPtr curve = OdDgBSplineCurve2d::createObject();
  pCell->add( curve );

  double minor = boxItemSize / 6.0;
  double major = boxItemSize / 6.0;

  OdGePoint2dArray arrCtrlPts;
  arrCtrlPts.push_back(ptCenter + OdGeVector2d( -1. * major,  1. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d( -1. * major,  2. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d( -2. * major,  2. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d( -2. * major,  1. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d( -1. * major,  1. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d(  1. * major,  1. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d(  2. * major,  1. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d(  2. * major,  2. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d(  1. * major,  2. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d(  1. * major,  1. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d(  1. * major, -1. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d(  1. * major, -2. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d(  2. * major, -2. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d(  2. * major, -1. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d(  1. * major, -1. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d( -1. * major, -1. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d( -2. * major, -1. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d( -2. * major, -2. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d( -1. * major, -2. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d( -1. * major, -1. * minor ));
  arrCtrlPts.push_back(ptCenter + OdGeVector2d( -1. * major,  1. * minor ));

  OdGeKnotVector  vrEmptyKnot;
  OdGeDoubleArray arrEmptyWeights;

  curve->setNurbsData( 4, false, true, arrCtrlPts, vrEmptyKnot, arrEmptyWeights );
  curve->setColorIndex( iColor );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addMesh( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdGeMatrix3d toBox;
  {
    //scale the axises
    toBox.setToIdentity();
    toBox[ 0 ][ 0 ] = boxItemSize / 100.;
    toBox[ 1 ][ 1 ] = boxItemSize / 100.;

    //shift to the center
    toBox.setTranslation( ptCenter.asVector() );
  }

  //face loops
  {
    OdDgMeshFaceLoopsPtr mesh = OdDgMeshFaceLoops::createObject();
    pCell->add( mesh );
    mesh->setColorIndex( iColor );

    mesh->setPointsNumber( 8 );
    mesh->setPoint( 0, OdGePoint3d( -35., -35., 0. ).transformBy( toBox ) );
    mesh->setPoint( 1, OdGePoint3d( -35., 35., 0. ).transformBy( toBox ) );
    mesh->setPoint( 2, OdGePoint3d( -20., -20., 0. ).transformBy( toBox ) );
    mesh->setPoint( 3, OdGePoint3d( -20., 20., 0. ).transformBy( toBox ) );
    mesh->setPoint( 4, OdGePoint3d( 20., -20., 0. ).transformBy( toBox ) );
    mesh->setPoint( 5, OdGePoint3d( 20., 20., 0. ).transformBy( toBox ) );
    mesh->setPoint( 6, OdGePoint3d( 35., -35., 0. ).transformBy( toBox ) );
    mesh->setPoint( 7, OdGePoint3d( 35., 35., 0. ).transformBy( toBox ) );

    OdDgMeshFaceLoops::VerticesArray vertices;
    vertices.resize( 4 );

    mesh->setFacesNumber( 4 );
    vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 3; vertices[ 3 ].m_pointIndex = 2;
    vertices[ 0 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_impliesVisibleEdge = false; 
    mesh->setFace( 0, vertices );
    vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 2; vertices[ 2 ].m_pointIndex = 4; vertices[ 3 ].m_pointIndex = 6;
    vertices[ 0 ].m_impliesVisibleEdge = false;
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    mesh->setFace( 1, vertices );
    vertices[ 0 ].m_pointIndex = 3; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 7; vertices[ 3 ].m_pointIndex = 5;
    vertices[ 0 ].m_impliesVisibleEdge = false; 
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    mesh->setFace( 2, vertices );
    vertices[ 0 ].m_pointIndex = 6; vertices[ 1 ].m_pointIndex = 4; vertices[ 2 ].m_pointIndex = 5; vertices[ 3 ].m_pointIndex = 7;
    vertices[ 0 ].m_impliesVisibleEdge = false;
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    mesh->setFace( 3, vertices );
  }

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addSharedCell3d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgSharedCellDefinitionPtr cellDef;
  OdDgSharedCellDefinitionTablePtr table = m_pModel3d->database()->getSharedCellDefinitionTable(OdDg::kForWrite);

  cellDef = OdDgSharedCellDefinition::createObject();
  cellDef->setName( OD_T("3d cell definition") );
  table->add( cellDef );

  OdDgEllipse3dPtr ellipse;

  ellipse = OdDgEllipse3d::createObject();
  ellipse->setPrimaryAxis( 1. );
  ellipse->setSecondaryAxis( 1. );
  ellipse->setLineWeight(3);
  cellDef->add( ellipse );

  ellipse = OdDgEllipse3d::createObject();
  ellipse->setPrimaryAxis( .7 );
  ellipse->setSecondaryAxis( .7 );
  cellDef->add( ellipse );

  OdDgLine3dPtr pLine = OdDgLine3d::createObject();
  pLine->setStartPoint( OdGePoint3d( -0.5, 0.0, 0.0 ) );
  pLine->setEndPoint( OdGePoint3d( 0.5, 0.0, 0.0 ) );
  cellDef->add( pLine );

  pLine = OdDgLine3d::createObject();
  pLine->setStartPoint( OdGePoint3d( 0.0, -0.5, 0.0 ) );
  pLine->setEndPoint( OdGePoint3d( 0.0, 0.5, 0.0 ) );
  cellDef->add( pLine );

  OdDgSharedCellReferencePtr reference;
  OdGeMatrix3d matScale;

  matScale.setToScaling( boxItemSize / 2.5 );

  reference = OdDgSharedCellReference::createObject();
  reference->setDefinitionName( OD_T("3d cell definition") );
  reference->setOrigin( ptCenter );
  reference->setTransformation( matScale );
  reference->setColorIndex( iColor );
  reference->setColorOverrideFlag( true );
  pCell->add( reference );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addSharedCell2d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgSharedCellDefinitionPtr cellDef;
  OdDgSharedCellDefinitionTablePtr table = m_pModel2d->database()->getSharedCellDefinitionTable(OdDg::kForWrite);

  cellDef = OdDgSharedCellDefinition::createObject();
  cellDef->setName( OD_T("2d cell definition") );
  table->add( cellDef );

  OdDgEllipse2dPtr ellipse;

  ellipse = OdDgEllipse2d::createObject();
  ellipse->setPrimaryAxis( 1. );
  ellipse->setSecondaryAxis( 0.8 );
  ellipse->setLineWeight(3);
  cellDef->add( ellipse );

  ellipse = OdDgEllipse2d::createObject();
  ellipse->setPrimaryAxis( .7 );
  ellipse->setSecondaryAxis( 0.5 );
  cellDef->add( ellipse );

  OdDgLine2dPtr pLine = OdDgLine2d::createObject();
  pLine->setStartPoint( OdGePoint2d( -0.5, 0.0 ) );
  pLine->setEndPoint( OdGePoint2d( 0.5, 0.0 ) );
  cellDef->add( pLine );

  pLine = OdDgLine2d::createObject();
  pLine->setStartPoint( OdGePoint2d( 0.0, -0.5 ) );
  pLine->setEndPoint( OdGePoint2d( 0.0, 0.5 ) );
  cellDef->add( pLine );

  OdDgSharedCellReferencePtr reference;
  OdGeMatrix3d matScale;

  matScale.setToScaling( boxItemSize / 2.5 );

  reference = OdDgSharedCellReference::createObject();
  reference->setDefinitionName( OD_T("2d cell definition") );
  reference->setOrigin( OdGePoint3d( ptCenter.x, ptCenter.y, 0) );
  reference->setTransformation( matScale );
  reference->setColorIndex( iColor );
  reference->setColorOverrideFlag( true );
  pCell->add( reference );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addRasterHeader3d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgRasterHeader3dPtr raster = OdDgRasterHeader3d::createObject();
  double sx = boxItemSize / 4.;
  double sy = boxItemSize / 4.;

  raster->setXExtent( 8 );
  raster->setYExtent( 8 );
  raster->setFormat( OdDgRaster::kBitmap );
  raster->setHorizontalDataFlag( true );
  raster->setScale( sy / 8 );
  raster->setOrigin( ptCenter - OdGeVector3d( sx/2.0,-sy/2.0, 0 ) );
  raster->setForeground( OdUInt8(iColor + 1) );

  pCell->add( raster );

  //add rows
  {
    OdDgRasterComponentPtr  component;
    OdUInt8                 byte;

    for( OdUInt16 i = 0; i < 8; i++ )       // MKU 23/12/09 - ('warning' issue)
    {
      component = OdDgRasterComponent::createObject();
      component->setFormat( OdDgRaster::kBitmap );
      component->setOffsetX( 0 );
      component->setOffsetY( i );
      component->setNumberOfPixels( 8 );

      switch( i )
      {
      case 0 : byte = 0x8E; break;
      case 1 : byte = 0xB3; break;
      case 2 : byte = 0xC3; break;
      case 3 : byte = 0xE6; break;
      case 4 : byte = 0x58; break;
      case 5 : byte = 0x4C; break;
      case 6 : byte = 0x87; break;
      case 7 : byte = 0x83; break;
      }

      component->setData( &byte, 1 );
      raster->add( component );
    }
  }

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d * OdGeMatrix3d::scaling( 2.0 ) );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addRasterHeader2d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgRasterHeader2dPtr raster = OdDgRasterHeader2d::createObject();
  double sx = boxItemSize / 4.;
  double sy = boxItemSize / 4.;

  raster->setXExtent( 8 );
  raster->setYExtent( 8 );
  raster->setFormat( OdDgRaster::kBitmap );
  raster->setHorizontalDataFlag( true );
  raster->setScale( sy / 8 );
  raster->setOrigin( OdGePoint3d( ptCenter.x - sx/2.0, ptCenter.y + sy/2.0, 0 ) );
  raster->setForeground( OdUInt8(iColor + 1) );

  pCell->add( raster );

  //add rows
  {
    OdDgRasterComponentPtr  component;
    OdUInt8                 byte;

    for( OdUInt16 i = 0; i < 8; i++ )       // MKU 23/12/09 - ('warning' issue)
    {
      component = OdDgRasterComponent::createObject();
      component->setFormat( OdDgRaster::kBitmap );
      component->setOffsetX( 0 );
      component->setOffsetY( i );
      component->setNumberOfPixels( 8 );

      switch( i )
      {
      case 0 : byte = 0x8E; break;
      case 1 : byte = 0xB3; break;
      case 2 : byte = 0xC3; break;
      case 3 : byte = 0xE6; break;
      case 4 : byte = 0x58; break;
      case 5 : byte = 0x4C; break;
      case 6 : byte = 0x87; break;
      case 7 : byte = 0x83; break;
      }

      component->setData( &byte, 1 );
      raster->add( component );
    }
  }

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation2d * OdGeMatrix2d::scaling( 2.0 ) );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addCone( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  OdDgConePtr pCone = OdDgCone::createObject();

  pCell->add( pCone );

  pCone->setColorIndex( iColor );

  OdGePoint3d ptCenter1 = ptCenter - OdGeVector3d(boxItemSize/3.0,0,0);
  OdGePoint3d ptCenter2 = ptCenter + OdGeVector3d(boxItemSize/3.0,0,0);

  OdGeQuaternion rotAngle;
  OdGeMatrix3d   rotMatrix;
  rotMatrix.setToRotation(OdaPI2, OdGeVector3d::kYAxis);
  rotAngle.set( rotMatrix );

  pCone->setConeType( OdDgCone::kGeneralCone );
  pCone->setCenter1(ptCenter1);
  pCone->setCenter2(ptCenter2);
  pCone->setRadius1(boxItemSize/3.0);
  pCone->setRadius2(boxItemSize/4.0);
  pCone->setRotation(rotAngle);

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addMultiline3d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint3d ptCenter = get3dBoxCenter(iRow, iCol);

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  double      minor = boxItemSize;
  double      major = boxItemSize;

  OdDgMultilinePtr multiline = OdDgMultiline::createObject();

  //add some profiles
  {
    OdDgMultilineProfile    profile;
    OdDgMultilineSymbology  symbology;

    symbology.setLineWeight( 1 );
    symbology.setColorIndex( iColor );
    symbology.setUseColorFlag( true );
    profile.setSymbology( symbology );
    profile.setDistance( 0. );
    multiline->addProfile( profile );

    symbology.setColorIndex( iColor );
    profile.setSymbology( symbology );
    profile.setDistance( .1 );
    multiline->addProfile( profile );

    symbology.setColorIndex( iColor );
    symbology.setUseStyleFlag( true );
    symbology.setLineStyleEntryId( 3 );
    profile.setSymbology( symbology );
    profile.setDistance( -.1 );
    multiline->addProfile( profile );
  }

  //define caps
  {
    OdDgMultilineSymbology symbology;
    symbology.setColorIndex( iColor );
    symbology.setUseColorFlag( true );

    multiline->setOriginCapAngle( 90. );
    multiline->setEndCapAngle( 90. );

    symbology.setCapLineFlag( true );
    multiline->setOriginCap( symbology );
    multiline->setEndCap( symbology );
  }

  //add some points
  {
    OdDgMultilinePoint point;
    point.setPoint( ptCenter + OdGeVector3d( -major / 3., minor / 3., 0. ) );
    multiline->addPoint( point );
    point.setPoint( ptCenter + OdGeVector3d( major / 3., 0., 0. ) );
    multiline->addPoint( point );
    point.setPoint( ptCenter + OdGeVector3d( -major / 3., -minor / 3., 0. ) );
    multiline->addPoint( point );
  }

  multiline->setLineWeight(4);

  pCell->add( multiline );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation3d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::addMultiline2d( OdUInt32 iRow, OdUInt32 iCol, bool bRotate )
{
  OdGePoint2d ptCenter = get2dBoxCenter(iRow, iCol);

  OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();

  m_pModel2d->addElement( pCell );

  OdUInt32 iColor = iCellColorIndex;

  if( !bRotate )
  {
    iColor = iUnrotatedCellColorIndex;
  }

  //------------------------------

  double      minor = boxItemSize;
  double      major = boxItemSize;

  OdDgMultilinePtr multiline = OdDgMultiline::createObject();

  //add some profiles
  {
    OdDgMultilineProfile    profile;
    OdDgMultilineSymbology  symbology;

    symbology.setLineWeight( 1 );
    symbology.setColorIndex( iColor );
    symbology.setUseColorFlag( true );
    profile.setSymbology( symbology );
    profile.setDistance( 0. );
    multiline->addProfile( profile );

    symbology.setColorIndex( iColor );
    profile.setSymbology( symbology );
    profile.setDistance( .1 );
    multiline->addProfile( profile );

    symbology.setColorIndex( iColor );
    symbology.setUseStyleFlag( true );
    symbology.setLineStyleEntryId( 3 );
    profile.setSymbology( symbology );
    profile.setDistance( -.1 );
    multiline->addProfile( profile );
  }

  //define caps
  {
    OdDgMultilineSymbology symbology;
    symbology.setColorIndex( iColor );
    symbology.setUseColorFlag( true );

    multiline->setOriginCapAngle( 90. );
    multiline->setEndCapAngle( 90. );

    symbology.setCapLineFlag( true );
    multiline->setOriginCap( symbology );
    multiline->setEndCap( symbology );
  }

  //add some points
  {
    OdDgMultilinePoint point;
    point.setPoint( OdGePoint3d( ptCenter.x - major / 3., ptCenter.y + minor / 3., 0. ) );
    multiline->addPoint( point );
    point.setPoint( OdGePoint3d( ptCenter.x + major / 3., ptCenter.y, 0. ) );
    multiline->addPoint( point );
    point.setPoint( OdGePoint3d( ptCenter.x - major / 3., ptCenter.y - minor / 3., 0. ) );
    multiline->addPoint( point );
  }

  multiline->setLineWeight(4);

  pCell->add( multiline );

  //------------------------------

  pCell->setOrigin(  ptCenter );

  if( bRotate )
  {
    pCell->setTransformation( m_matRotation2d );
  }
}

//========================================================================

void OdDgnCellTransformFiller::fillModel2d()
{
  create2DBoxes(4,4);

  addLine2d(0,0);
  addLine2d(0,0, false);

  addLineString2d(0,1);
  addLineString2d(0,1, false);

  addArc2d(0,2);
  addArc2d(0,2, false);

  addEllipse2d(0,3);
  addEllipse2d(0,3, false);

  addText2d(1,0);
  addText2d(1,0, false);

  addTextNode2d(1,1);
  addTextNode2d(1,1, false);

  addAttribute2d(1,2);
  addAttribute2d(1,2, false);

  addDimension2d(1,3);
  addDimension2d(1,3, false);

  addShape2d(2,0);
  addShape2d(2,0, false);

  addComplexShape2d(2,1);
  addComplexShape2d(2,1, false);

  addComplexString2d(2,2);
  addComplexString2d(2,2, false);

  addCellHeader2d(2,3);
  addCellHeader2d(2,3, false);

  addBSplineCurve2d(3,0);
  addBSplineCurve2d(3,0, false);

  addSharedCell2d(3,1);
  addSharedCell2d(3,1, false);

  addRasterHeader2d(3,2);
  addRasterHeader2d(3,2, false);

  addMultiline2d(3,3);
  addMultiline2d(3,3, false);

  m_pModel2d->fitToView();
}

//========================================================================

void OdDgnCellTransformFiller::fillModel3d()
{
  create3DBoxes(5,5);

  addLine3d(0,0);
  addLine3d(0,0, false);

  addLineString3d(0,1);
  addLineString3d(0,1, false);

  addArc3d(0,2);
  addArc3d(0,2, false);

  addEllipse3d(0,3);
  addEllipse3d(0,3, false);

  addText3d(0,4);
  addText3d(0,4, false);

  addTextNode3d(1,0);
  addTextNode3d(1,0, false);

  addAttribute3d(1,1);
  addAttribute3d(1,1, false);

  addDimension3d(1,2);
  addDimension3d(1,2, false);

  addShape3d(1,3);
  addShape3d(1,3, false);

  addComplexShape3d(1,4);
  addComplexShape3d(1,4, false);

  addComplexString3d(2,0);
  addComplexString3d(2,0, false);

  addCellHeader3d(2,1);
  addCellHeader3d(2,1, false);

  addSolid(2,2);
  addSolid(2,2, false);

  addSurface(2,3);
  addSurface(2,3, false);

  addBSplineSurface(2,4);
  addBSplineSurface(2,4, false);

  addSmartSolid(3,0);
  addSmartSolid(3,0, false);

  addBSplineCurve3d(3,1);
  addBSplineCurve3d(3,1, false);

  addMesh(3,2);
  addMesh(3,2, false);

  addSharedCell3d(3,3);
  addSharedCell3d(3,3, false);

  addRasterHeader3d(3,4);
  addRasterHeader3d(3,4, false);

  addCone(4,0);
  addCone(4,0, false);

  addMultiline3d(4,1);
  addMultiline3d(4,1, false);

  m_pModel3d->fitToView();
}

//========================================================================
