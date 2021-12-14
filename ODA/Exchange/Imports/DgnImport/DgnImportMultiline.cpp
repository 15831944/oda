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

#include <OdaCommon.h>
#include "DgnImportMultiline.h"
#include "DgnImportCommon.h"
#include "DgMultiline.h"
#include <DbMline.h>
#include "DgView.h"
#include "DgnImportContext.h"

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

Mline::MlineJustification convertMLineOffset( OdDgMultiline::OffsetMode uMode )
{
  Mline::MlineJustification uRet = Mline::kZero;

  switch( uMode )
  {
    case OdDgMultiline::kByWorkLine:
    case OdDgMultiline::kByCenter:
    {
      uRet = Mline::kZero;
    } break;

    case OdDgMultiline::kByMaximum:
    {
      uRet = Mline::kTop;
    } break;

    case OdDgMultiline::kByMinimum:
    {
      uRet = Mline::kBottom;
    } break;
  }

  return uRet;
}

//---------------------------------------------------------------------------------------------------

OdDbObjectId getMLineStyle( const OdDgMultiline* pMLine, OdDbDatabase* pDb )
{
  OdDbObjectId idRet;

  // Create descriptor

  OdDgMultilineSymbology startCap;
  OdDgMultilineSymbology endCap;
  OdDgMultilineSymbology middleCap;

  pMLine->getOriginCap(startCap);
  pMLine->getEndCap(endCap);
  pMLine->getMiddleCap( middleCap );

  bool      bFillMLine = false;
  OdUInt32  uFillColor = 0;

  OdDgViewPtr pActiveView = OdDgnImportContext::getActiveView();

  if( pActiveView.isNull() || pActiveView->getShowFillsFlag() )
  {
    if( pMLine->getFillType() == OdDg::kFillColor )
    {
      uFillColor = pMLine->getFillColorIndex();
      bFillMLine = true;
    }
  }

  OdDgnImportMultilineDescriptor mlineDesc;
  mlineDesc.m_bShowJoints          = middleCap.getCapLineFlag();
  mlineDesc.m_bShowStartCapLine    = startCap.getCapLineFlag();
  mlineDesc.m_bShowStartCapInArc   = startCap.getCapInArcFlag();
  mlineDesc.m_bShowStartCapOutArc  = startCap.getCapOutArcFlag();
  mlineDesc.m_bShowEndCapLine      = endCap.getCapLineFlag();
  mlineDesc.m_bShowEndCapInArc     = endCap.getCapInArcFlag();
  mlineDesc.m_bShowEndCapOutArc    = endCap.getCapOutArcFlag();
  mlineDesc.m_dStartCapAngle       = pMLine->getOriginCapAngle();
  mlineDesc.m_dEndCapAngle         = pMLine->getEndCapAngle();
  mlineDesc.m_bShowFill            = bFillMLine;
  mlineDesc.m_uJustification       = pMLine->getOffsetMode();
  mlineDesc.m_uFillColor           = uFillColor;
  mlineDesc.m_uMLineStyle          = pMLine->getMultilineStyleId();
  mlineDesc.m_bTableMLineStyle     = false;

  for( OdUInt32 i = 0; i < pMLine->getProfilesCount(); i++ )
  {
    OdDgMultilineProfile curProfile;
    pMLine->getProfile( i, curProfile );
    mlineDesc.m_arrOffsets.push_back( curProfile.getDistance() );

    if( curProfile.symbology().getUseColorFlag() )
      mlineDesc.m_arrColorIndexes.push_back( curProfile.symbology().getColorIndex() );
    else
      mlineDesc.m_arrColorIndexes.push_back(pMLine->getColorIndex());

    if( curProfile.symbology().getUseStyleFlag() )
      mlineDesc.m_arrLineStyles.push_back( curProfile.symbology().getLineStyleEntryId() );
    else
      mlineDesc.m_arrLineStyles.push_back(pMLine->getLineStyleEntryId());
  }

  idRet = OdDgnImportContext::getMultilineStyle( mlineDesc );

  if( idRet.isNull() )
  {
    idRet = OdDgnImportContext::createMultilineStyle( mlineDesc, pDb, pMLine->database() );
  }

  OdDgnImportContext::setResourceUsage(idRet);

  return idRet;
}

//---------------------------------------------------------------------------------------------------

void craeteIndexPairArray(const OdDbMlinePtr& pDbMLine,
                            const OdDgMultiline* pDgMLine,
                              OdArray<OdUInt32>& arrIndexes)
{
  OdMLSegmentArray arrSegments;
  pDbMLine->getParametersAt(0, arrSegments);
  arrIndexes.resize(pDgMLine->getProfilesCount());

  OdDoubleArray arrDgnProfileOffsets;
  OdUInt32Array arrDgnIndexes;
  OdDoubleArray arrDwgSegmentOffsets;
  OdUInt32Array arrDwgIndexes;

  OdUInt32 l;

  for (l = 0; l < arrSegments.size(); l++)
  {
    arrDwgSegmentOffsets.push_back(arrSegments[l].m_SegParams[0]);
    arrDwgIndexes.push_back(l);
  }

  for(l = 0; l < pDgMLine->getProfilesCount(); l++)
  {
    OdDgMultilineProfile dgProfile;
    pDgMLine->getProfile(l, dgProfile);
    arrDgnProfileOffsets.push_back(dgProfile.getDistance());
    arrDgnIndexes.push_back(l);
  }

  for (l = 0; l < arrDwgSegmentOffsets.size(); l++)
  {
    for( OdUInt32 m = l + 1; m < arrDwgSegmentOffsets.size(); m++ )
    {
      if(arrDwgSegmentOffsets[l] > arrDwgSegmentOffsets[m])
      { 
        double dTmp = arrDwgSegmentOffsets[m];
        arrDwgSegmentOffsets[m] = arrDwgSegmentOffsets[l];
        arrDwgSegmentOffsets[l] = dTmp;

        OdUInt32 uTmp = arrDwgIndexes[m];
        arrDwgIndexes[m] = arrDwgIndexes[l];
        arrDwgIndexes[l] = uTmp;
      }
    }
  }

  for (l = 0; l < arrDgnProfileOffsets.size(); l++)
  {
    for (OdUInt32 m = l + 1; m < arrDgnProfileOffsets.size(); m++)
    {
      if (arrDgnProfileOffsets[l] > arrDgnProfileOffsets[m])
      {
        double dTmp = arrDgnProfileOffsets[m];
        arrDgnProfileOffsets[m] = arrDgnProfileOffsets[l];
        arrDgnProfileOffsets[l] = dTmp;

        OdUInt32 uTmp = arrDgnIndexes[m];
        arrDgnIndexes[m] = arrDgnIndexes[l];
        arrDgnIndexes[l] = uTmp;
      }
    }
  }

  for( l = 0; l < arrDgnIndexes.size(); l++ )
    arrIndexes[arrDgnIndexes[l]] = arrDwgIndexes[l];
}

//---------------------------------------------------------------------------------------------------

void addSegment(OdGeDoubleArray& arrSegments, double dValue)
{
  bool bAddToEnd = true;

  for( OdInt32 i = arrSegments.size() - 1; i >= 0; i-- )
  {
    if (arrSegments[i] < dValue)
    {
      bAddToEnd = false;
      arrSegments.insert(arrSegments.begin() + i + 1, dValue);
      break;
    }
  }

  if( bAddToEnd )
    arrSegments.push_back(dValue);
}

//---------------------------------------------------------------------------------------------------

void addBreak( OdUInt32 uIndex, OdArray<OdGeDoubleArray>& arrParams, double dBreakOffset, double dBreakLength )
{
  double dStartOfBreak = dBreakOffset;
  double dEndOfBreak   = dBreakOffset + dBreakLength;
  addSegment(arrParams[uIndex], dStartOfBreak);
  addSegment(arrParams[uIndex], dEndOfBreak);
}

//---------------------------------------------------------------------------------------------------

void getMLineSegmentOffsetAndDistances(const OdDgMultiline* pDgnMLine,
                                         OdUInt32 uSegmentIndex,
                                           bool bClosedMLine,
                                            OdDoubleArray& arrStartOffsets,
                                              OdDoubleArray& arrSegmentDistances )
{
  OdGePoint3d ptPrevStartBase, ptStartBase, ptEndBase, ptNextEndBase;
  OdDgMultilinePoint ptMLine;
  pDgnMLine->getPoint(uSegmentIndex, ptMLine);
  ptMLine.getPoint(ptStartBase);
  pDgnMLine->getPoint(uSegmentIndex + 1, ptMLine);
  ptMLine.getPoint(ptEndBase);

  OdGeVector3d vrNormal = pDgnMLine->getZVector();
  vrNormal.normalize();

  OdGeVector3d vrDir, vrPrevDir, vrNextDir;

  vrDir = ptEndBase - ptStartBase;

  if( !vrDir.isZeroLength() )
    vrDir.normalize();

  if( uSegmentIndex > 0 )
  {
    pDgnMLine->getPoint(uSegmentIndex - 1, ptMLine);
    ptMLine.getPoint(ptPrevStartBase);

    vrPrevDir = ptStartBase - ptPrevStartBase;

    if (!vrPrevDir.isZeroLength())
      vrPrevDir.normalize();
  }
  else if( bClosedMLine )
  {
    pDgnMLine->getPoint( pDgnMLine->getPointsCount() - 1, ptMLine );
    ptMLine.getPoint(ptPrevStartBase);

    if( ptStartBase.isEqualTo(ptPrevStartBase) )
    {
      pDgnMLine->getPoint(pDgnMLine->getPointsCount() - 2, ptMLine);
      ptMLine.getPoint(ptPrevStartBase);
    }

    vrPrevDir = ptStartBase - ptPrevStartBase;

    if (!vrPrevDir.isZeroLength())
      vrPrevDir.normalize();
  }

  if( uSegmentIndex < pDgnMLine->getPointsCount() - 2 )
  {
    pDgnMLine->getPoint(uSegmentIndex + 2, ptMLine);
    ptMLine.getPoint(ptNextEndBase);

    vrNextDir = ptNextEndBase - ptEndBase;

    if (!vrNextDir.isZeroLength())
      vrNextDir.normalize();
  }
  else if( bClosedMLine )
  {
    pDgnMLine->getPoint(0, ptMLine);
    ptMLine.getPoint(ptNextEndBase);

    if( ptNextEndBase.isEqualTo(ptEndBase) )
    {
      pDgnMLine->getPoint(1, ptMLine);
      ptMLine.getPoint(ptNextEndBase);
    }

    vrNextDir = ptNextEndBase - ptEndBase;

    if (!vrNextDir.isZeroLength())
      vrNextDir.normalize();
  }

  if( !vrDir.isZeroLength() )
  {
    for (OdUInt32 i = 0; i < pDgnMLine->getProfilesCount(); i++)
    {
      OdDgMultilineProfile curProfile;
      pDgnMLine->getProfile(i, curProfile);

      if( OdZero(curProfile.getDistance()) )
      {
        arrStartOffsets.push_back(0.0);
        arrSegmentDistances.push_back(ptStartBase.distanceTo(ptEndBase));
        continue;
      }

      OdGePoint3d ptSegmentStart, ptSegmentEnd, ptPrevEnd, ptNextStart;

      OdGeVector3d vrPerpDir = vrDir;
      vrPerpDir.rotateBy(OdaPI2, vrNormal);

      ptSegmentStart = ptStartBase + vrPerpDir * curProfile.getDistance();
      ptSegmentEnd   = ptEndBase + vrPerpDir * curProfile.getDistance();

      if( (uSegmentIndex > 0) || bClosedMLine )
      {
        OdGePoint3d ptPrevSegmentStart, ptPrevSegmentEnd;

        if( vrPrevDir.isZeroLength() )
        {
          ptPrevEnd = ptSegmentStart;
        }
        else
        {
          OdGeVector3d vrPrevPerpDir = vrPrevDir;
          vrPrevPerpDir.rotateBy(OdaPI2, vrNormal);

          ptPrevSegmentStart = ptPrevStartBase + vrPrevPerpDir * curProfile.getDistance();
          ptPrevSegmentEnd = ptStartBase + vrPrevPerpDir * curProfile.getDistance();

          OdGeLine3d linePrev(ptPrevSegmentStart, ptPrevSegmentEnd);
          OdGeLine3d lineCur(ptSegmentStart, ptSegmentEnd);

          if (!linePrev.intersectWith(lineCur, ptPrevEnd))
            ptPrevEnd = ptSegmentStart;
        }
      }
      else
      {
        double dCapAngle = pDgnMLine->getOriginCapAngle();

        if( OdZero( sin(dCapAngle) ) )
          ptPrevEnd = ptSegmentStart;
        else
        {
          OdGeVector3d vrPerpDir = vrDir;
          vrPerpDir.rotateBy(dCapAngle, vrNormal);

          double dDist = curProfile.getDistance() / sin(dCapAngle);
          ptPrevEnd = ptStartBase + vrPerpDir * dDist;
        }
      }

      if( (uSegmentIndex < pDgnMLine->getPointsCount() - 2) || bClosedMLine )
      {
        OdGePoint3d ptNextSegmentStart, ptNextSegmentEnd;

        if( vrNextDir.isZeroLength() )
        {
          ptNextStart = ptSegmentEnd;
        }
        else
        {
          OdGeVector3d vrNextPerpDir = vrNextDir;
          vrNextPerpDir.rotateBy(OdaPI2, vrNormal);

          ptNextSegmentStart = ptEndBase + vrNextPerpDir * curProfile.getDistance();
          ptNextSegmentEnd = ptNextEndBase + vrNextPerpDir * curProfile.getDistance();

          OdGeLine3d lineNext(ptNextSegmentStart, ptNextSegmentEnd);
          OdGeLine3d lineCur(ptSegmentStart, ptSegmentEnd);

          if( !lineNext.intersectWith(lineCur, ptNextStart) )
            ptNextStart = ptSegmentEnd;
        }
      }
      else
      {
        double dCapAngle = pDgnMLine->getEndCapAngle();

        if (OdZero(sin(dCapAngle)))
          ptNextStart = ptSegmentEnd;
        else
        {
          OdGeVector3d vrPerpDir = vrDir;
          vrPerpDir.rotateBy(dCapAngle, vrNormal);

          double dDist = curProfile.getDistance() / sin(dCapAngle);
          ptNextStart = ptEndBase + vrPerpDir * dDist;
        }
      }

      if (ptPrevEnd.isEqualTo(ptSegmentStart))
        arrStartOffsets.push_back(0.0);
      else
      {
        OdGeVector3d vrOffset = ptSegmentStart - ptPrevEnd;
        vrOffset.normalize();

        if (vrOffset.isCodirectionalTo(vrDir))
          arrStartOffsets.push_back(ptSegmentStart.distanceTo(ptPrevEnd));
        else
          arrStartOffsets.push_back(-ptSegmentStart.distanceTo(ptPrevEnd));
      }

      arrSegmentDistances.push_back(ptPrevEnd.distanceTo(ptNextStart));
    }
  }
  else
  {
    for (OdUInt32 i = 0; i < pDgnMLine->getProfilesCount(); i++)
    {
      arrStartOffsets.push_back(0.0);
      arrSegmentDistances.push_back(0.0);
    }
  }
}

//---------------------------------------------------------------------------------------------------

bool isPointMultiline(const OdDgMultiline* pMLine)
{
  bool bRet = true;

  OdDgMultilinePoint curPoint;
  OdGePoint3d ptFirst;
  OdGePoint3d ptCur;

  if (pMLine->getPointsCount() > 0)
  {
    pMLine->getPoint(0, curPoint);
    curPoint.getPoint(ptFirst);

    for (OdUInt32 i = 1; i < pMLine->getPointsCount(); i++)
    {
      pMLine->getPoint(i, curPoint);
      curPoint.getPoint(ptCur);

      if (!ptCur.isEqualTo(ptFirst))
      {
        bRet = false;
        break;
      }
    }
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

bool allowToApplyLineWeight(OdDgMultiline* pMLine, OdDb::LineWeight& mlineLineWeight)
{
  bool bRet = true;

  OdUInt32 nProfiles = pMLine->getProfilesCount();

  if (nProfiles == 0)
    return false;

  OdUInt32 uLineWeight = pMLine->getLineWeight();

  OdDgMultilineProfile curProfile;
  pMLine->getProfile(0, curProfile);

  if (curProfile.getSymbology().getUseWeightFlag())
    uLineWeight = curProfile.getSymbology().getLineWeight();

  for (OdUInt32 i = 1; i < nProfiles; i++)
  {
    pMLine->getProfile(i, curProfile);

    OdUInt32 uCurLineWeight = pMLine->getLineWeight();

    if (curProfile.getSymbology().getUseWeightFlag())
      uCurLineWeight = curProfile.getSymbology().getLineWeight();

    if (uCurLineWeight != uLineWeight)
    {
      bRet = false;
      break;
    }
  }

  if (bRet && nProfiles > 1 && !pMLine->getClosedFlag())
  {
    OdDgMultilineSymbology curSymbology;
    pMLine->getOriginCap(curSymbology);

    if( curSymbology.getCapLineFlag() || curSymbology.getCapOutArcFlag() || ((nProfiles > 3) && curSymbology.getCapInArcFlag()) )
    {
      OdUInt32 uCurLineWeight = pMLine->getLineWeight();

      if( curSymbology.getUseWeightFlag() )
        uCurLineWeight = curSymbology.getLineWeight();

      if( uCurLineWeight != uLineWeight )
        bRet = false;
    }
  }

  if (bRet && nProfiles > 1 && !pMLine->getClosedFlag())
  {
    OdDgMultilineSymbology curSymbology;
    pMLine->getEndCap(curSymbology);

    if (curSymbology.getCapLineFlag() || curSymbology.getCapOutArcFlag() || ((nProfiles > 3) && curSymbology.getCapInArcFlag()))
    {
      OdUInt32 uCurLineWeight = pMLine->getLineWeight();

      if (curSymbology.getUseWeightFlag())
        uCurLineWeight = curSymbology.getLineWeight();

      if (uCurLineWeight != uLineWeight)
        bRet = false;
    }
  }

  if (bRet && nProfiles > 1 )
  {
    OdDgMultilineSymbology curSymbology;
    pMLine->getMiddleCap(curSymbology);

    if( curSymbology.getCapLineFlag() )
    {
      OdUInt32 uCurLineWeight = pMLine->getLineWeight();

      if (curSymbology.getUseWeightFlag())
        uCurLineWeight = curSymbology.getLineWeight();

      if (uCurLineWeight != uLineWeight)
        bRet = false;
    }
  }

  if( bRet )
    mlineLineWeight = odDgnImportLineweightMapping(uLineWeight);

  return bRet;
}

//---------------------------------------------------------------------------------------------------

void OdDgnMultilineImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgMultiline* pDgnMLine = (OdDgMultiline*)(e);

  if( isPointMultiline(pDgnMLine) )
    return;

  OdDbMlinePtr pDbMLine = OdDbMline::createObject();
  owner->appendOdDbEntity( pDbMLine );

  pDbMLine->setStyle( getMLineStyle( pDgnMLine, owner->database() ) );

  bool bInitIndexArray = false;
  OdArray<OdUInt32> arrIndexes;
  arrIndexes.resize( pDgnMLine->getProfilesCount() );

  OdUInt32 i;

  bool bHasBreaks = false;

  OdUInt32 uPointCount = pDgnMLine->getPointsCount();

  if( pDgnMLine->getClosedFlag() )
  {
    OdDgMultilinePoint ptMLine;
    OdGePoint3d ptStartMLine;
    OdGePoint3d ptEndMLine;
    pDgnMLine->getPoint(0, ptMLine);
    ptMLine.getPoint(ptStartMLine);
    pDgnMLine->getPoint(pDgnMLine->getPointsCount()-1, ptMLine);
    ptMLine.getPoint(ptEndMLine);

    if (ptStartMLine.isEqualTo(ptEndMLine))
      uPointCount--;
  }

  for( i = 0; i < uPointCount; i++ )
  {
    OdDgMultilinePoint ptMLine;
    pDgnMLine->getPoint(i, ptMLine);
    OdGePoint3d pt3dMLine;
    ptMLine.getPoint(pt3dMLine);
    pDbMLine->appendSeg(pt3dMLine);

    if( ptMLine.getBreaksCount() > 0 )
      bHasBreaks = true;
  }

  pDbMLine->setClosedMline(pDgnMLine->getClosedFlag());

  if( bHasBreaks )
  {
    craeteIndexPairArray(pDbMLine, pDgnMLine, arrIndexes);

    OdUInt32 nPoints = pDgnMLine->getPointsCount() - 1;

    if( pDgnMLine->getClosedFlag() )
      nPoints++;

    for( i = 0; i < nPoints; i++ )
    {
      OdDgMultilinePoint ptMLine;
      pDgnMLine->getPoint(i, ptMLine);

      if( ptMLine.getBreaksCount() == 0 )
        continue;

      OdDoubleArray arrStartOffsets;
      OdDoubleArray arrSegmentDistances;

      getMLineSegmentOffsetAndDistances(pDgnMLine, i, pDgnMLine->getClosedFlag(), arrStartOffsets, arrSegmentDistances);

      OdArray<OdGeDoubleArray> arrParams;
      arrParams.resize(arrIndexes.size());

      OdUInt32 n;

      for( n = 0; n < pDgnMLine->getProfilesCount(); n++ )
      {
        OdGeDoubleArray arrSegments;
        arrSegments.push_back(0);
        arrParams[n] = arrSegments;
      }

      for( OdUInt32 j = 0; j < ptMLine.getBreaksCount(); j++ )
      {
        OdDgMultilineBreak curBreak;
        ptMLine.getBreak(j, curBreak);

        if( curBreak.getFlags() == OdDgMultilineBreak::kStandardByDistance )
        {
          for( OdUInt32 lineIndex = 0, currentBit = 1; lineIndex < arrIndexes.size(); lineIndex++, currentBit <<= 1 )
          {
            if (currentBit & curBreak.getLinesMask())
              addBreak(arrIndexes[lineIndex], arrParams, curBreak.getOffset() + arrStartOffsets[lineIndex], curBreak.getLength());
          }
        }
        else if(curBreak.getFlags() == OdDgMultilineBreak::kFromJoint )
        { 
          for( OdUInt32 lineIndex = 0, currentBit = 1; lineIndex < arrIndexes.size(); lineIndex++, currentBit <<= 1 )
          {
            if( currentBit & curBreak.getLinesMask() )
              addBreak(arrIndexes[lineIndex], arrParams, 0, curBreak.getLength());
          }
        }
        else if( curBreak.getFlags() == OdDgMultilineBreak::kToJoint )
        {
          for( OdUInt32 lineIndex = 0, currentBit = 1; lineIndex < arrIndexes.size(); lineIndex++, currentBit <<= 1 )
          {
            if (currentBit & curBreak.getLinesMask())
              addBreak(arrIndexes[lineIndex], arrParams, arrSegmentDistances[lineIndex] - curBreak.getLength(), curBreak.getLength());
          }
        }
      }

      OdMLSegmentArray arrSegments;
      pDbMLine->getParametersAt(i, arrSegments);

      for( n = 0; n < arrSegments.size(); n++ )
      {
        arrParams[n].insert(arrParams[n].begin(), arrSegments[n].m_SegParams[0]);
        arrSegments[n].m_SegParams = arrParams[n];
      }

      pDbMLine->setParametersAt(i, arrSegments);
    }
  }

  OdDgMultilineSymbology startCap;
  OdDgMultilineSymbology endCap;
  pDgnMLine->getOriginCap(startCap);
  pDgnMLine->getEndCap(endCap);

  pDbMLine->setJustification( convertMLineOffset(pDgnMLine->getOffsetMode()) );
  pDbMLine->setNormal( pDgnMLine->getZVector() );
  pDbMLine->setSupressStartCaps(!startCap.getCapOutArcFlag() && !startCap.getCapInArcFlag() && !startCap.getCapLineFlag());
  pDbMLine->setSupressEndCaps(!endCap.getCapOutArcFlag() && !endCap.getCapInArcFlag() && !endCap.getCapLineFlag());

  copyEntityProperties( pDgnMLine, pDbMLine );

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( pDbMLine->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( e->elementId(), dwgPath );

  OdDb::LineWeight mlineLineWeight = OdDb::kLnWt000;

  if( allowToApplyLineWeight(pDgnMLine, mlineLineWeight) )
    pDbMLine->setLineWeight(mlineLineWeight);
}

//---------------------------------------------------------------------------------------------------

}
