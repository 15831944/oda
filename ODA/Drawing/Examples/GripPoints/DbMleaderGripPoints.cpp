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
/* OdDbMleader entities.                                                */
/************************************************************************/
#include "StdAfx.h"
#include "DbMleaderGripPoints.h"
#include "DbMLeader.h"
#include "RxObjectImpl.h"
#include "DbGrip.h"
#include "Ge/GeLineSeg3d.h"
#include "DbBaseDatabase.h"
#include "Gi/GiAnnoScaleSet.h"

#include "DbAnnotativeObjectPE.h"
#include "DbObjectContextInterface.h"
#include "DbBlockTableRecord.h"
#include "DbLayout.h"
#include "DbViewport.h"
#include "DbSymUtl.h"
#include "DbObjectContextPE.h" // moved from Source/database/Objects

//#define  SUBENTITY_TEST 1

const int START_GRIP  = 1;

const int DOGLEG_START_GRIP   = START_GRIP;
const int DOGLEG_CENTER_GRIP  = START_GRIP + 1;
const int DOGLEG_END_GRIP     = START_GRIP + 2;
const int TEXT_POS_GRIP       = START_GRIP + 3;
const int BLOCK_POS_GRIP      = START_GRIP + 4;
const int LINE_START_GRIP     = START_GRIP + 5;

static OdGePoint3d ProjectPointToLine( const OdGePoint3d ptLineStart, 
                    const OdGePoint3d ptLineEnd, 
                    const OdGePoint3d ptBase )
{
  double a = ptBase.distanceTo( ptLineStart );
  double b = ptBase.distanceTo( ptLineEnd );
  double c = ptLineStart.distanceTo( ptLineEnd );

  if( c < 1e-8 )
    return ptLineStart;

  double d = (a*a+c*c-b*b)/(2*c);

  OdGePoint3d ptRet;

  ptRet.x = ptLineStart.x + (ptLineEnd.x - ptLineStart.x) * d / c;
  ptRet.y = ptLineStart.y + (ptLineEnd.y - ptLineStart.y) * d / c;
  ptRet.z = ptLineStart.z + (ptLineEnd.z - ptLineStart.z) * d / c;

  return ptRet;
}

static bool IsOnSegment( const OdGePoint3d ptSegStart, const OdGePoint3d ptSegEnd, const OdGePoint3d ptCheck )
{
  OdGeLineSeg3d pLineSeg( ptSegStart, ptSegEnd );
  return pLineSeg.isOn( ptCheck, 0);
}

static bool IsDoglegEnabled(OdDbMLeader* pMLeader, 
                            bool* pbConnectedAtDogLeg = NULL, // "Horizontal Landing" in ACAD property
                            bool* pbUseDogLegCenterGrip = NULL, // use DOGLEG_CENTER_GRIP
                            bool* pbSkipForLastVertex = NULL)
{
  if (pbConnectedAtDogLeg)
    *pbConnectedAtDogLeg = true;
  if (pbUseDogLegCenterGrip)
    *pbUseDogLegCenterGrip = false; // = true // old way
  if (pbSkipForLastVertex)
    *pbSkipForLastVertex = true; // last vertex will be moved via DOGLEG_END_GRIP (is a duplicate of last grip point on arrow)
  ODA_ASSERT_ONCE_X(MLEADER, pMLeader);
  if (!pMLeader)
    return false;

  bool bEnableDogleg = pMLeader->enableDogleg();
  OdDbMLeaderStyle::TextAttachmentDirection tp = pMLeader->textAttachmentDirection();
  if (tp == OdDbMLeaderStyle::kAttachmentVertical || pMLeader->leaderLineType() == OdDbMLeaderStyle::kSplineLeader)
    bEnableDogleg = false;
  if (!bEnableDogleg)
  {
    switch (pMLeader->contentType())
    {
    case OdDbMLeaderStyle::kBlockContent:
      if (!pMLeader->blockContentId().isNull() && pMLeader->blockConnectionType() == OdDbMLeaderStyle::kConnectBase)
      {
        if (pbConnectedAtDogLeg)
          *pbConnectedAtDogLeg = false;
        //if (pbSkipForLastVertex)
        //  *pbSkipForLastVertex = false;
      }
      break;
    case OdDbMLeaderStyle::kMTextContent:
      switch (tp)
      {
      case OdDbMLeaderStyle::kAttachmentHorizontal:
        if (bEnableDogleg) 
          break;
        // remove useless grip point if dogleg is disabled // CORE-12183
      case OdDbMLeaderStyle::kAttachmentVertical:
        if (pbConnectedAtDogLeg)
          *pbConnectedAtDogLeg = false;
        if (pbSkipForLastVertex)
          *pbSkipForLastVertex = false;
        break;
      }
      break;
    }
  }
  return bEnableDogleg;
}

static OdInt32 GripTypeByIndex(const OdDbEntity* pEnt, int iIndex)
{
  OdDbMLeader* pMLeader = OdDbMLeader::cast(pEnt).get();
  ODA_ASSERT_ONCE_X(MLEADER, iIndex >= 0);

  bool bConnectedAtDogLeg = false;
  int index = 0;
  OdIntArray LeaderIndexes;

  if (pMLeader->leaderLineType() != OdDbMLeaderStyle::kInVisibleLeader) // #5591 // CORE-15557 grips are not displayed when leader type is none
  {
    bool bUseDogLegCenterGrip = true,
         bSkipForLastVertex = false,
         bEnableDogleg = IsDoglegEnabled(pMLeader, &bConnectedAtDogLeg, &bUseDogLegCenterGrip, &bSkipForLastVertex);

    pMLeader->getLeaderIndexes(LeaderIndexes);
    for (unsigned int idx = 0, sz = LeaderIndexes.size(); idx < sz; idx++)
    {
      if (bEnableDogleg)
      {
        switch (iIndex - index)
        {
        case 0: return DOGLEG_START_GRIP;
        case 1: return bUseDogLegCenterGrip ? DOGLEG_CENTER_GRIP : DOGLEG_END_GRIP;
        case 2: if (bUseDogLegCenterGrip) return DOGLEG_END_GRIP;
        }
        index += bUseDogLegCenterGrip ? 3 : 2;
      }
      else if (bConnectedAtDogLeg)
      {
        if (iIndex == index)
          return DOGLEG_END_GRIP;
        index++;
      }

      OdIntArray LeaderLineIndexes;
      pMLeader->getLeaderLineIndexes(LeaderIndexes[idx], LeaderLineIndexes);
      for (unsigned int idxLn = 0, szLn = LeaderLineIndexes.size(); idxLn < szLn; idxLn++) // INT-6499 problem with attachment of CORE-8843
      {
        int nVertices = 0;
        pMLeader->numVertices(LeaderLineIndexes[idxLn], nVertices);
        if (bSkipForLastVertex)
          nVertices--;
        if (iIndex < (index + nVertices))
          return LINE_START_GRIP; // + (iIndex - index);
        index += nVertices;
      }
    } // for
  }

  if (   pMLeader->contentType() == OdDbMLeaderStyle::kMTextContent
      && ((pMLeader->mtext().get() && !pMLeader->mtext()->contents().isEmpty()) || !LeaderIndexes.isEmpty()))
  {
    if (iIndex == index)
      return TEXT_POS_GRIP;
    index++;
  }
  else if (pMLeader->contentType() == OdDbMLeaderStyle::kBlockContent && !bConnectedAtDogLeg)
  {
    if (iIndex == index)
      return BLOCK_POS_GRIP;
    index++;
  }
  
  ODA_ASSERT_ONCE_X(MLEADER, iIndex == index); // test
  return 0; // none
}

static double getScale(OdDbMLeader* pMLeader)
{
  ODA_ASSERT_ONCE(pMLeader);
  double dScale = pMLeader->scale();
  if (dScale < 1e-8) // is zero for annotative (ObjectArx) 
  {
    // INT-6499 problem with attachment of CORE-10120 via annotative scale. 
    dScale = 1.0;
    OdDbAnnotativeObjectPEPtr pAnno = OdDbAnnotativeObjectPE::cast(pMLeader);
    bool isAnno = pAnno.get() && pAnno->annotative(pMLeader);
    ODA_ASSERT_ONCE_X(MLEADER, isAnno);
    OdGiAnnoScaleSet res;
    OdDbBaseDatabasePE* pDbPE = OdDbBaseDatabasePE::cast(pMLeader->database()).get();
    if (pDbPE && !pDbPE->getAnnoScaleSet(pMLeader->objectId(), res))
      isAnno = false; // CORE-12141
    if (isAnno)
    {
      OdDbDatabase* pDb = pMLeader->database();
      ODA_ASSERT_ONCE_X(MLEADER, pDb);
      OdDbAnnotationScalePtr pScale;
      OdDbAnnotScaleObjectContextDataPtr pScaleCtx;
      if (pDb->getActiveLayoutBTRId() == pDb->getModelSpaceId())
        pScale = pDb->getCANNOSCALE();
      else
      {
        OdDbBlockTableRecordPtr pBtr = OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().safeOpenObject());
        OdDbLayoutPtr pLayout = OdDbLayout::cast(pBtr->getLayoutId().safeOpenObject(OdDb::kForRead));
        OdDbViewportPtr pVpt = OdDbViewport::cast(pLayout->activeViewportId().safeOpenObject());
        pScale = pVpt->annotationScale();
      }

      OdDbObjectContextInterfacePtr op(pMLeader);
      if (!op->hasContext(pMLeader, *pScale))
      {
        // is accessible via OdDbObjectContextPE only
        //if (OdDbObjectContextDataManager* pMgr = OdDbSystemInternals::getImpl(pMLeader)->contextDataManager())
        //  if (OdDbContextDataSubManager* pSub = pMgr->getSubManager(ODDB_ANNOTATIONSCALES_COLLECTION))
        //    pScale = OdDbAnnotationScale::cast(pSub->getDefaultContextData());
        //
        OdDbObjectContextPEPtr pCtxPE = op; // = OdDbObjectContextInterface::cast(pMLeader);
        OdDbObjectContextDataPtr pCtxDef = pCtxPE->getDefaultContextData(pMLeader, ODDB_ANNOTATIONSCALES_COLLECTION);
        pScaleCtx = OdDbAnnotScaleObjectContextData::cast(pCtxDef);
        ODA_ASSERT_ONCE_X(MLEADER, pScaleCtx.get());
        if (pScaleCtx.get())
          pScale = NULL;
      }

      OdResult res = eInvalidContext;
      if (pScale.get())
        res = pScale->getScale(dScale);
      else if (pScaleCtx.get())
        res = pScaleCtx->getScale(dScale);
      ODA_ASSERT_ONCE_X(MLEADER, res == eOk && !OdZero(dScale));
      if (res == eOk && !OdZero(dScale))
        dScale = 1.0 / dScale;
      else
        dScale = 1.0;
    }
  }
  return dScale;
}

static bool GetConnectionData(OdDbMLeader* pMLeader, int leaderIndex, 
                              bool bEnableDogleg, bool bConnectedAtDogLeg,
                              OdGePoint3d& ptConnect, OdGeVector3d& vrDoglegDir, // out
                              double& dDoglegLength, double* pdScale = NULL) // out
{
  vrDoglegDir = OdGeVector3d();
  ptConnect = OdGePoint3d();
  dDoglegLength = 0;
  double dScale = getScale(pMLeader);
  if (pdScale)
    *pdScale = dScale;
  OdDbMLeaderStyle::TextAttachmentDirection tp = pMLeader->textAttachmentDirection();

  if (!bEnableDogleg && !bConnectedAtDogLeg) // vertical
  {
    ODA_ASSERT_ONCE_X(MLEADER,    pMLeader->contentType() != OdDbMLeaderStyle::kMTextContent 
                               || tp == OdDbMLeaderStyle::kAttachmentVertical || !bEnableDogleg);
    // there is no reason in connection date for such mleader
    return true;
  }

  ODA_ASSERT_ONCE_X(MLEADER, tp != OdDbMLeaderStyle::kAttachmentVertical);
  if (tp == OdDbMLeaderStyle::kAttachmentVertical)
    return true;
  
  pMLeader->getDoglegDirection(leaderIndex, vrDoglegDir);
  vrDoglegDir = - vrDoglegDir;
  if (pMLeader->connectionPoint(vrDoglegDir, ptConnect) != eOk) // via text location point
    return false;

  OdGePoint3d ptCnt = ptConnect;
  if (pMLeader->connectionPoint(OdGeVector3d::kIdentity, // unused now
                                ptCnt, tp) == eOk && ptCnt != ptConnect) // via current value of connection point
  {
    //ODA_FAIL_ONCE_X(MLEADER);
    ptConnect = ptCnt; // way for broken setDogLegLength_acad.dwg created by ARX subtest setDogLegLength
  }

  dDoglegLength = pMLeader->doglegLength(leaderIndex) * dScale;

  if (pMLeader->contentType() != OdDbMLeaderStyle::kMTextContent || !bEnableDogleg)
    return true;

  OdIntArray leaderLineIndexes;
  pMLeader->getLeaderLineIndexes(leaderIndex, leaderLineIndexes);
  if (leaderLineIndexes.isEmpty())
    return false;

  OdGePoint3d ptConnectOpposite;
  if (pMLeader->getLastVertex(leaderLineIndexes[0], ptConnectOpposite) != eOk)
    return true;
  OdGePoint3d ptConnectAlt = ptConnectOpposite - vrDoglegDir * dDoglegLength;
  if (ptConnect == ptConnectAlt)
    return true;

  // this offset (exists in some files) will disappear after recomputing
  OdGeVector3d vOffs = ptConnectAlt - ptConnect;
  double dProjX = vOffs.dotProduct(vrDoglegDir),
         dProjAbsX = fabs(dProjX),
         dProjY = vOffs.dotProduct(vrDoglegDir.crossProduct(OdGeVector3d::kZAxis)),
         dProjAbsY = fabs(dProjY),
         dLen = vOffs.length();
  if ((dProjAbsX * 100.0) < dLen) // if (OdEqual(ptConnect.x, ptConnectAlt.x))
    ptConnect = ptConnectAlt;    //   ptConnect.y = ptConnectAlt.y; //shrt_10917.dwg 
  else if ((dProjAbsY * 25.0) < dLen) // if (OdEqual(ptConnect.y, ptConnectAlt.y))
    ptConnect += vrDoglegDir * dProjX;  //   ptConnect.x = ptConnectAlt.x; // 581902_shrt_acad_res_10738.dwg
  //else
  //{
  //  ODA_FAIL_X(MLEADER); // TODO
  //}
  return true;
}

//#ifdef SUBENTITY_TEST
//OdResult OdDbMleaderGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
//{
//
//  OdDbMLeader* pMLeader = OdDbMLeader::cast(ent);
//
//  if( GetAsyncKeyState( VK_SHIFT) )
//  {
//    int nPaths = 0;
//    int iMarker = 0;
//    int iMinMarker = 0;
//    int iMaxMarker = 16000;
//
//    for( iMarker = iMinMarker; iMarker < iMaxMarker; iMarker++ )
//    {
//      OdGePoint3d  ptTmp;
//      OdGeMatrix3d matTmp;
//      OdGeVector3d vrTmp;
//      OdDbFullSubentPathArray PathArr;
//
//      OdResult es = pMLeader->getSubentPathsAtGsMarker( OdDb::kClassSubentType, iMarker, ptTmp, matTmp, PathArr );
//
//      if( es == eOk && PathArr.size() > 0 )
//      {
//        OdDbEntityPtr pNewEnt = pMLeader->subentPtr( PathArr[0] );
//
//        if( !pNewEnt.isNull() )
//        {
//          OdDbDatabase* pDb = pMLeader->database();
//          OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
//          pMs->appendOdDbEntity(pNewEnt);
//        }
//      }
//    }
//
//    return eOk;
//  }
//
//  OdIntArray leaderIndexes;
//  OdIntArray leaderLineIndexes;
//  pMLeader->getLeaderIndexes(leaderIndexes);
//
//  if( leaderIndexes.size() > 0 )
//    pMLeader->getLeaderLineIndexes( leaderIndexes[0], leaderLineIndexes);
//
//  OdDbGripDataPtrArray pGrips;
//
//  OdGePoint3d  ptTmp;
//  OdGeMatrix3d matTmp;
//  OdGeVector3d vrTmp;
//  OdDbFullSubentPathArray PathArr;
//
//  bool bEnableDogleg = IsDoglegEnabled(pMLeader);
//  if( bEnableDogleg )
//  {
//    pMLeader->getSubentPathsAtGsMarker( OdDb::kClassSubentType, OdDbMLeader::kDoglegMark, ptTmp, matTmp,PathArr);
//
//    if( PathArr.size() > 0 )
//    {
//      OdResult iRes = getGripPointsAtSubentPath( ent, PathArr[0], pGrips, 0, 0, vrTmp, 0);
//
//      if( iRes == eOk )
//        for( unsigned int i = 0; i < pGrips.size(); i++ )
//          gripPoints.append( pGrips[i]->gripPoint() );
//    }
//  }
//  else
//  {
//    pMLeader->getSubentPathsAtGsMarker( OdDb::kClassSubentType, OdDbMLeader::kLeaderLineMark, ptTmp, matTmp,PathArr);
//
//    if( PathArr.size() > 0 )
//    {
//      OdResult iRes = getGripPointsAtSubentPath( ent, PathArr[0], pGrips, 0, 0, vrTmp, 0);
//
//      if( iRes == eOk && pGrips.size())
//        gripPoints.append( pGrips[0]->gripPoint() );
//    }
//  }
//
//  for( OdUInt32 j = 0; j < leaderLineIndexes.size(); j++ )
//  {
//    PathArr.clear();
//    pGrips.clear();
//
//    pMLeader->getSubentPathsAtGsMarker( OdDb::kClassSubentType, OdDbMLeader::kLeaderLineMark + leaderLineIndexes[j], ptTmp, matTmp,PathArr);
//
//    if( PathArr.size() > 0 )
//    {
//      OdResult iRes = getGripPointsAtSubentPath( ent, PathArr[0], pGrips, 0, 0, vrTmp, 0);
//
//      if( iRes == eOk )
//        for( unsigned int i = 0; i < pGrips.size(); i++ )
//        {
//          if( i == 0 )
//            continue;
//
//          gripPoints.append( pGrips[i]->gripPoint() );
//        }
//    }
//  }
//
//  if( pMLeader->contentType() == OdDbMLeaderStyle::kMTextContent )
//  {
//    PathArr.clear();
//    pGrips.clear();
//
//    pMLeader->getSubentPathsAtGsMarker( OdDb::kClassSubentType, OdDbMLeader::kMTextMark, ptTmp, matTmp,PathArr);
//
//    if( PathArr.size() > 0 )
//    {
//      OdResult iRes = getGripPointsAtSubentPath( ent, PathArr[0], pGrips, 0, 0, vrTmp, 0);
//
//      if( iRes == eOk )
//        for( unsigned int i = 0; i < pGrips.size(); i++ )
//          gripPoints.append( pGrips[i]->gripPoint() );
//    }
//  }
//
//  return eOk;
//}
//#else
OdResult OdDbMleaderGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints ) const
{
  OdDbMLeader* pMLeader = OdDbMLeader::cast(ent);
  bool bConnectedAtDogLeg = false;
  OdIntArray leaderIndexes;
  ODA_ASSERT_VAR(OdInt32 idxGrip = 0;)
  ODA_ASSERT_VAR(OdInt32 iGripType = 0;)

  if (pMLeader->leaderLineType() != OdDbMLeaderStyle::kInVisibleLeader) // #5591 // CORE-15557 grips are not displayed when leader type is none
  {
    bool bUseDogLegCenterGrip = true,
         bSkipForLastVertex = false,
         bEnableDogleg = IsDoglegEnabled(pMLeader, &bConnectedAtDogLeg, &bUseDogLegCenterGrip, &bSkipForLastVertex);

    pMLeader->getLeaderIndexes(leaderIndexes);

    for (OdUInt32 i = 0, sz = leaderIndexes.size(); i < sz; i++)
    {
      OdGePoint3d  ptConnect;
      OdGeVector3d vrDoglegDir;
      double dDoglegLength = 0;
      bool bRes = GetConnectionData(pMLeader, leaderIndexes[i], bEnableDogleg, bConnectedAtDogLeg,
                                    ptConnect, vrDoglegDir, dDoglegLength); // out
      if (bRes)
      {
        if( bEnableDogleg )
        {
          ODA_ASSERT_ONCE_X(MLEADER, (iGripType = GripTypeByIndex(ent, idxGrip++)) == DOGLEG_START_GRIP);
          gripPoints.append( ptConnect );
          if (bUseDogLegCenterGrip)
          {
            OdGePoint3d tmpPt1( ptConnect + vrDoglegDir * dDoglegLength/2);
            ODA_ASSERT_ONCE_X(MLEADER, (iGripType = GripTypeByIndex(ent, idxGrip++)) == DOGLEG_CENTER_GRIP);
            gripPoints.append( tmpPt1 );
          }
          OdGePoint3d tmpPt2( ptConnect + vrDoglegDir * dDoglegLength);
          ODA_ASSERT_ONCE_X(MLEADER, (iGripType = GripTypeByIndex(ent, idxGrip++)) == DOGLEG_END_GRIP);
          gripPoints.append( tmpPt2 );
        }
        else if (bConnectedAtDogLeg)
        {
          OdGePoint3d tmpPt2( ptConnect + vrDoglegDir * dDoglegLength);
          ODA_ASSERT_ONCE_X(MLEADER, (iGripType = GripTypeByIndex(ent, idxGrip++)) == DOGLEG_END_GRIP);
          gripPoints.append( tmpPt2 );
        }

        OdIntArray leaderLineIndexes;
        pMLeader->getLeaderLineIndexes(leaderIndexes[i], leaderLineIndexes);

        for (OdUInt32 k = 0, lsz = leaderLineIndexes.size(); k < lsz; k++)
        {
          int nVertices = 0;
          if (pMLeader->numVertices(leaderLineIndexes[k], nVertices) == eOk)
          {
            for( int j = 0; j < (bSkipForLastVertex ? (nVertices - 1) : nVertices); j++ )
            {
              OdGePoint3d ptVertex;
              pMLeader->getVertex(leaderLineIndexes[k], j, ptVertex);
              ODA_ASSERT_ONCE_X(MLEADER, (iGripType = GripTypeByIndex(ent, idxGrip++)) == LINE_START_GRIP);
              gripPoints.append( ptVertex );
            }
          }
        }
      }
    } // for
  }

  if (pMLeader->contentType() == OdDbMLeaderStyle::kBlockContent && !bConnectedAtDogLeg)
  {
    ODA_ASSERT_ONCE_X(MLEADER, (iGripType = GripTypeByIndex(ent, idxGrip++)) == BLOCK_POS_GRIP);
    OdGePoint3d ptBlockPos;
    pMLeader->getBlockPosition( ptBlockPos );
    gripPoints.append( ptBlockPos );
  }
  else if(pMLeader->contentType() == OdDbMLeaderStyle::kMTextContent)
  {
    if ((pMLeader->mtext().get() && !pMLeader->mtext()->contents().isEmpty()) || !leaderIndexes.isEmpty())
    {
      ODA_ASSERT_ONCE_X(MLEADER, (iGripType = GripTypeByIndex(ent, idxGrip++)) == TEXT_POS_GRIP);
      OdGePoint3d ptTextPos;
      pMLeader->getTextLocation(ptTextPos);
      gripPoints.append(ptTextPos);
    }
  }
  ODA_ASSERT_ONCE_X(MLEADER, (iGripType = GripTypeByIndex(ent, idxGrip)) == 0);

  return eOk;
}
//#endif

OdResult OdDbMleaderGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();

  if ( size == 0 )
    return eOk;

  OdDbMLeaderPtr pMLeader = OdDbMLeader::cast(pEnt);

//#ifdef SUBENTITY_TEST
//  if( GetAsyncKeyState(VK_CONTROL) )
//  {
//    OdGePoint3d  ptTmp;
//    OdGeMatrix3d matTmp;
//    OdGeVector3d vrTmp;
//    OdDbFullSubentPathArray PathArr;
//    pMLeader->getSubentPathsAtGsMarker( OdDb::kClassSubentType, OdDbMLeader::kLeaderLineMark, ptTmp, matTmp,PathArr);
//
//    if( PathArr.size() > 0 )
//      pMLeader->deleteSubentPaths( PathArr );
//
//    return eOk;
//  }
//#endif

  bool bConnectedAtDogLeg = true,
       bUseDogLegCenterGrip = true,
       bSkipForLastVertex = false,
       bEnableDogleg = IsDoglegEnabled(pMLeader, &bConnectedAtDogLeg, &bUseDogLegCenterGrip, &bSkipForLastVertex);

  OdIntArray leaderIndexes;
  pMLeader->getLeaderIndexes(leaderIndexes);

  for( unsigned iPt = 0; iPt < size; iPt++ )
  {
    int iIndex = indices[iPt];
//#ifdef SUBENTITY_TEST
//
//    OdIntArray leaderLineIndexes;
//
//    if( leaderIndexes.size() > 0 )
//      pMLeader->getLeaderLineIndexes( leaderIndexes[0], leaderLineIndexes);
//
//    OdDbGripDataPtrArray pGrips;
//
//    OdGePoint3d  ptTmp;
//    OdGeMatrix3d matTmp;
//    OdGeVector3d vrTmp;
//    OdDbFullSubentPathArray PathArr;
//    OdDbVoidPtrArray pGripData;
//
//    int iCurIndex = 0;
//
//    if( bEnableDogleg && (iIndex < 3) )
//    {
//      pMLeader->getSubentPathsAtGsMarker( OdDb::kClassSubentType, OdDbMLeader::kDoglegMark, ptTmp, matTmp,PathArr);
//
//      if( PathArr.size() > 0 )
//      {
//        OdResult iRes = getGripPointsAtSubentPath( pEnt, PathArr[0], pGrips, 0, 0, vrTmp, 0);
//
//        if( iRes == eOk )
//        {
//          pGripData.append( pGrips[iIndex]->appData() );
//          iRes = moveGripPointsAtSubentPaths( pEnt, PathArr, pGripData, offset, 0 );
//        }
//      }
//    }
//    else if( (iIndex == 0) && !bEnableDogleg )
//    {
//      pMLeader->getSubentPathsAtGsMarker( OdDb::kClassSubentType, OdDbMLeader::kLeaderLineMark, ptTmp, matTmp,PathArr);
//
//      if( PathArr.size() > 0 )
//      {
//        OdResult iRes = getGripPointsAtSubentPath( pEnt, PathArr[0], pGrips, 0, 0, vrTmp, 0);
//
//        if( iRes == eOk && pGrips.size() )
//        {
//          pGripData.append( pGrips[0]->appData() );
//          iRes = moveGripPointsAtSubentPaths( pEnt, PathArr, pGripData, offset, 0 );
//        }
//      }
//    }
//    else
//    {
//      bool bMoveGripPt = false;
//
//      if( bEnableDogleg )
//        iCurIndex += 3;
//      else
//        iCurIndex++ ;
//
//      OdUInt32 nVertices = 0;
//
//      for( unsigned int k = 0; k < leaderLineIndexes.size(); k++ )
//      {
//        OdResult res = pMLeader->numVertices( leaderLineIndexes[k], nVertices );
//
//        if( iCurIndex + nVertices <= iIndex )
//          iCurIndex += nVertices;
//        else
//        {
//          pMLeader->getSubentPathsAtGsMarker( OdDb::kClassSubentType, OdDbMLeader::kLeaderLineMark + leaderLineIndexes[k], ptTmp, matTmp,PathArr);
//
//          if( PathArr.size() > 0 )
//          {
//            OdResult iRes = getGripPointsAtSubentPath( pEnt, PathArr[0], pGrips, 0, 0, vrTmp, 0);
//
//            if( iRes == eOk && pGrips.size() == nVertices + 1 )
//            {
//              pGripData.append( pGrips[ iIndex - iCurIndex + 1 ]->appData() );
//              iRes = moveGripPointsAtSubentPaths( pEnt, PathArr, pGripData, offset, 0 );
//            }
//          }
//
//          bMoveGripPt = true;
//          break;
//        }
//      }
//
//      if( !bMoveGripPt )
//      {
//        if( pMLeader->contentType() == OdDbMLeaderStyle::kMTextContent )
//        {
//          pMLeader->getSubentPathsAtGsMarker( OdDb::kClassSubentType, OdDbMLeader::kMTextMark, ptTmp, matTmp,PathArr);
//
//          if( PathArr.size() > 0 )
//          {
//            OdResult iRes = getGripPointsAtSubentPath( pEnt, PathArr[0], pGrips, 0, 0, vrTmp, 0);
//
//            if( iRes == eOk )
//            {
//              pGripData.append( pGrips[0]->appData() );
//              iRes = moveGripPointsAtSubentPaths( pEnt, PathArr, pGripData, offset, 0 );
//            }
//          }
//        }
//      }
//    }
//
//#else
    int iCurIndex = 0;

    bool bMoveGripPt = false;

    for( OdUInt32 i = 0; i < leaderIndexes.size(); i++ )
    {
      OdGePoint3d  ptConnect;
      OdGeVector3d vrDoglegDir;
      double dDoglegLength = 0, dScale = 1.0;
      bool bRes = GetConnectionData(pMLeader, leaderIndexes[i], bEnableDogleg, bConnectedAtDogLeg,
                                    ptConnect, vrDoglegDir, dDoglegLength, &dScale); // out
      if (bRes)
      {
        if( bEnableDogleg )
        {
          if (iIndex - iCurIndex < (bUseDogLegCenterGrip ? 3 : 2))
          {
            // Dogleg

            OdGePoint3d StartDoglegPt = ptConnect;
            OdGePoint3d EndDoglegPt( ptConnect + vrDoglegDir * dDoglegLength);

            if( iIndex == iCurIndex )
            {
              if( dDoglegLength < 1e-8 )
                break;

              OdGePoint3d tmpPt = ptConnect;
              tmpPt += offset;

              OdGePoint3d tmpNewPt = ProjectPointToLine(StartDoglegPt, EndDoglegPt, tmpPt );

              double dNewLength = tmpNewPt.distanceTo(EndDoglegPt);

              bool bZeroLength = true;

              if( IsOnSegment( StartDoglegPt, EndDoglegPt, tmpNewPt) || 
                ( StartDoglegPt.distanceTo(tmpNewPt) < EndDoglegPt.distanceTo(tmpNewPt) )
                )
              {
                if( dNewLength > 1e-8 )
                  bZeroLength = false;
              }

              if( !bZeroLength )
              {
                ODA_ASSERT_VAR(OdInt32 iGripType = GripTypeByIndex(pEnt, iIndex);)
                ODA_ASSERT_ONCE_X(MLEADER, iGripType == DOGLEG_START_GRIP)
                OdGeVector3d vOffset = - vrDoglegDir * (dNewLength - dDoglegLength);

                OdDbMLeaderStyle::ContentType ctype = pMLeader->contentType();
                switch (ctype)
                {
                case OdDbMLeaderStyle::kNoneContent:
                  break;
                case OdDbMLeaderStyle::kMTextContent:
                  break; // CORE-11916/CORE-14319 subtest setDogLegLength
                //{
                //  // CORE-11630
                //  OdGePoint3d loc;
                //  pMLeader->getTextLocation(loc);
                //  loc += vOffset;
                //  pMLeader->setTextLocation(loc);
                //  break;
                //}
                default:
                  pMLeader->moveMLeader(vOffset, OdDbMLeader::kMoveAllExceptArrowHeaderPoints); //INT-6499
                  break;
                }
                pMLeader->setDoglegLength(leaderIndexes[i], dNewLength / dScale);
                if (!pMLeader->isDBRO())
                  OdDbObject::cast(pMLeader)->subClose(); // force recompute of clone object
              }
            }
            else if (bUseDogLegCenterGrip && iIndex == iCurIndex + 1)
            {
              ODA_ASSERT_VAR(OdInt32 iGripType = GripTypeByIndex(pEnt, iIndex);)
              ODA_ASSERT_ONCE_X(MLEADER, iGripType == DOGLEG_CENTER_GRIP)
              pMLeader->moveMLeader(offset, OdDbMLeader::kMoveContentAndDoglegPoints);
            }
            else
            {
              if( dDoglegLength < 1e-8 )
                break;

              OdGePoint3d tmpPt = EndDoglegPt;
              tmpPt += offset;

              OdGePoint3d tmpNewPt = ProjectPointToLine(StartDoglegPt, EndDoglegPt, tmpPt );

              OdGeVector3d tmpOffset = tmpNewPt - EndDoglegPt;

              //double dNewLength = tmpNewPt.distanceTo(StartDoglegPt);
              //bool bZeroLength = true;
              //if( IsOnSegment( StartDoglegPt, EndDoglegPt, tmpNewPt) || 
              //  ( StartDoglegPt.distanceTo(tmpNewPt) > EndDoglegPt.distanceTo(tmpNewPt) )
              //  )
              //{
              //  if( dNewLength > 1e-8 )
              //    bZeroLength = false;
              //}
              //if( !bZeroLength )
              {
                ODA_ASSERT_VAR(OdInt32 iGripType = GripTypeByIndex(pEnt, iIndex);)
                ODA_ASSERT_ONCE_X(MLEADER, iGripType == DOGLEG_END_GRIP)

                ////pMLeader->moveMLeader( tmpOffset, OdDbMLeader::kMoveContentAndDoglegPoints, false);
                ////pMLeader->setDoglegLength(leaderIndexes[i], dNewLength / dScale);
                //pMLeader->moveMLeader(tmpOffset, OdDbMLeader::kMoveAllExceptArrowHeaderPoints); // INT-6499
                pMLeader->moveMLeader(offset, OdDbMLeader::kMoveContentAndDoglegPoints); // INT-6499
              }
            }

            bMoveGripPt = true;
            break;
          }
          else
            iCurIndex += bUseDogLegCenterGrip ? 3 : 2;
        }
        else if (bConnectedAtDogLeg)
        {
          if( iIndex == iCurIndex )
          {
            ODA_ASSERT_VAR(OdInt32 iGripType = GripTypeByIndex(pEnt, iIndex);)
            ODA_ASSERT_ONCE_X(MLEADER, iGripType == DOGLEG_END_GRIP);
            pMLeader->moveMLeader(offset, OdDbMLeader::kMoveContentAndDoglegPoints);
            bMoveGripPt = true;
            break;
          }
          else
            iCurIndex++;
        }
      }

      if( bMoveGripPt )
        break;

      OdIntArray leaderLineIndexes;
      pMLeader->getLeaderLineIndexes(leaderIndexes[i], leaderLineIndexes);

      for( OdUInt32 k = 0; k < leaderLineIndexes.size(); k++ )
      {
        int nVertices = 0;
        if (pMLeader->numVertices(leaderLineIndexes[k], nVertices) == eOk)
        {
          for( int j = 0; j < (bSkipForLastVertex ? (nVertices - 1) : nVertices); j++ )
          {
            if( iIndex == iCurIndex )
            {
              bMoveGripPt = true;
              ODA_ASSERT_VAR(OdInt32 iGripType = GripTypeByIndex(pEnt, iIndex);)
              ODA_ASSERT_ONCE_X(MLEADER, iGripType == LINE_START_GRIP);
              if (j == (nVertices - 1))
              {
                if (   !bEnableDogleg && !bConnectedAtDogLeg
                    && (   pMLeader->contentType() == OdDbMLeaderStyle::kMTextContent
                     // || pMLeader->contentType() == OdDbMLeaderStyle::kBlockContent
                       ))
                {
                  // 577_bmp_recomputed_.dwg // INT-7735
                  pMLeader->moveMLeader(offset, OdDbMLeader::kMoveContentAndDoglegPoints, false);
                  bMoveGripPt = true;
                  return eOk;
                }
                // INT-6499 
                // last vertex will be moved via DOGLEG_END_GRIP (is a duplicate of last grip point on arrow) 
                ODA_FAIL_X(MLEADER);
                break;
              }
              // line pt
              OdGePoint3d ptVertex;
              pMLeader->getVertex(leaderLineIndexes[k],j,ptVertex);
              ptVertex += offset;
              pMLeader->setVertex(leaderLineIndexes[k],j,ptVertex);
              //bMoveGripPt = true;
              break;
            }
            else
              iCurIndex++;
          }

          if( bMoveGripPt )
            break;
        }

        if( bMoveGripPt )
          break;
      }

      if( bMoveGripPt )
          break;
    }

    if( bMoveGripPt )
      continue;

    if (pMLeader->contentType() == OdDbMLeaderStyle::kBlockContent && !bConnectedAtDogLeg)
    {
      if( iIndex == iCurIndex )
      {
        ODA_ASSERT_VAR(OdInt32 iGripType = GripTypeByIndex(pEnt, iIndex);)
        ODA_ASSERT_ONCE_X(MLEADER, iGripType == BLOCK_POS_GRIP);
        // block pt
        pMLeader->moveMLeader(offset, OdDbMLeader::kMoveContentAndDoglegPoints);
        bMoveGripPt = true;
        break;
      }
      else
        iCurIndex++;
    }
    else if( pMLeader->contentType() == OdDbMLeaderStyle::kMTextContent )
    {
      if( iIndex == iCurIndex )
      {
        ODA_ASSERT_VAR(OdInt32 iGripType = GripTypeByIndex(pEnt, iIndex);)
        ODA_ASSERT_ONCE_X(MLEADER, iGripType == TEXT_POS_GRIP);
        pMLeader->moveMLeader(offset, OdDbMLeader::kMoveContentAndDoglegPoints);
        bMoveGripPt = true;
        break;
      }
      else
        iCurIndex++;
    }
//#endif
  }

  return eOk;
}

OdResult OdDbMleaderGripPointsPE::getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const
{
  return getGripPoints( ent, stretchPoints );
}

OdResult OdDbMleaderGripPointsPE::moveStretchPointsAt( OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset )
{
  return moveGripPointsAt( ent, indices, offset );
}

OdResult OdDbMleaderGripPointsPE::getOsnapPoints(const OdDbEntity* ent, 
                         OdDb::OsnapMode osnapMode, 
                         OdGsMarker gsSelectionMark, 
                         const OdGePoint3d& pickPoint,
                         const OdGePoint3d& lastPoint, 
                         const OdGeMatrix3d& xWorldToEye, 
                         OdGePoint3dArray& snapPoints) const
{
  OdResult res;
  OdRxObjectPtrArray arrExploded;
  res = ent->explode(arrExploded);
  if (res != eOk)
    return res;

  for (unsigned int i = 0; i < arrExploded.size(); ++i)
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(arrExploded[i]);
    if (!pEnt.isNull())
    {
      pEnt->getOsnapPoints( osnapMode, gsSelectionMark,
        pickPoint, lastPoint, xWorldToEye, snapPoints);
    }
  }
  return eOk;
}

OdResult OdDbMleaderGripPointsPE::getGripPointsAtSubentPath(  const OdDbEntity* pEntity, 
                                const OdDbFullSubentPath& path, 
                                OdDbGripDataPtrArray& grips,
                                const double curViewUnitSize, 
                                const int gripSize,
                                const OdGeVector3d& curViewDir, 
                                const OdUInt32 bitflags) const
{
  OdDbMLeader* pMLeader = OdDbMLeader::cast(pEntity);
  int gsMarker = (int)path.subentId().index();

  if( gsMarker < OdDbMLeader::kLeaderLineMark || gsMarker >= OdDbMLeader::kBlockAttribute )
    return eOk;

  bool bConnectedAtDogLeg = true,
       bUseDogLegCenterGrip = true,
       bSkipForLastVertex = false,
       bEnableDogleg = IsDoglegEnabled(pMLeader, &bConnectedAtDogLeg, &bUseDogLegCenterGrip, &bSkipForLastVertex);

  OdIntArray leaderIndexes;
  pMLeader->getLeaderIndexes(leaderIndexes);

  if( bEnableDogleg && gsMarker >= OdDbMLeader::kDoglegMark && gsMarker < OdDbMLeader::kMTextMark )
  {
    for( OdUInt32 i = 0; i < leaderIndexes.size(); i++ )
    {
      if( gsMarker != OdDbMLeader::kDoglegMark + leaderIndexes[i] )
        continue;

      OdGePoint3d  ptConnect;
      OdGeVector3d vrDoglegDir;
      double dDoglegLength = 0;
      bool bRes = GetConnectionData(pMLeader, leaderIndexes[i], bEnableDogleg, bConnectedAtDogLeg,
                                    ptConnect, vrDoglegDir, dDoglegLength); // out
      if (bRes)
      {
        OdDbGripData *pGrip1 = new OdDbGripData();
        pGrip1->setAppData(OdIntToPtr(DOGLEG_START_GRIP));
        pGrip1->setGripPoint(ptConnect);
        grips.append(pGrip1);

        if (bUseDogLegCenterGrip)
        {
          OdGePoint3d tmpPt1( ptConnect + vrDoglegDir * dDoglegLength/2);
          OdDbGripData *pGrip2 = new OdDbGripData();
          pGrip2->setAppData(OdIntToPtr(DOGLEG_CENTER_GRIP));
          pGrip2->setGripPoint(tmpPt1);
          grips.append(pGrip2);
        }

        OdGePoint3d tmpPt2( ptConnect + vrDoglegDir * dDoglegLength);
        OdDbGripData *pGrip3 = new OdDbGripData();
        pGrip3->setAppData(OdIntToPtr(DOGLEG_END_GRIP));
        pGrip3->setGripPoint(tmpPt2);
        grips.append(pGrip3);
      }

      break;
    }
  }
  else if( gsMarker >= OdDbMLeader::kLeaderLineMark && gsMarker < OdDbMLeader::kDoglegMark )
  {
    bool bAddGrips = false;

    for( OdUInt32 i = 0; i < leaderIndexes.size(); i++ )
    {
      OdGePoint3d  ptConnect, ptFirst;
      OdGeVector3d vrDoglegDir;
      double dDoglegLength = 0;
      bool bRes = GetConnectionData(pMLeader, leaderIndexes[i], bEnableDogleg, bConnectedAtDogLeg,
                                    ptConnect, vrDoglegDir, dDoglegLength); // out
      if (bRes)
      {
        if (bEnableDogleg)
          ptFirst =  ptConnect;
        else
          ptFirst =  ptConnect + vrDoglegDir * dDoglegLength;

        OdIntArray leaderLineIndexes;
        pMLeader->getLeaderLineIndexes(leaderIndexes[i], leaderLineIndexes);

        for( OdUInt32 k = 0; k < leaderLineIndexes.size(); k++ )
        {
          if( gsMarker != OdDbMLeader::kLeaderLineMark + leaderLineIndexes[k] )
            continue;

          int nVertices = 0;
          if (pMLeader->numVertices(leaderLineIndexes[k], nVertices) == eOk)
          {
            OdDbGripData *pGrip = new OdDbGripData();
            pGrip->setAppData(OdIntToPtr(LINE_START_GRIP));
            pGrip->setGripPoint(ptFirst);
            grips.append(pGrip);

            for( int j = 0; j < (bSkipForLastVertex ? (nVertices - 1) : nVertices); j++ )
            {
              OdGePoint3d ptVertex;
              pMLeader->getVertex(leaderLineIndexes[k], j, ptVertex);

              OdDbGripData *pGrip1 = new OdDbGripData();
              pGrip1->setAppData(OdIntToPtr(LINE_START_GRIP + j + 1));
              pGrip1->setGripPoint(ptVertex);
              grips.append(pGrip1);
            }
          }

          bAddGrips = true;
          break;
        }
      }

      if( bAddGrips )
        break;
    }
  }
  else if( gsMarker >= OdDbMLeader::kMTextMark )
  {
    if (pMLeader->contentType() == OdDbMLeaderStyle::kMTextContent)
    {
      if( gsMarker == OdDbMLeader::kMTextMark )
      {
        OdGePoint3d ptTextPos;
        pMLeader->getTextLocation( ptTextPos );
        OdDbGripData *pGrip = new OdDbGripData();
        pGrip->setAppData(OdIntToPtr(TEXT_POS_GRIP));
        pGrip->setGripPoint(ptTextPos);
        grips.append(pGrip);
      }
    }
    //else if (pMLeader->contentType() == OdDbMLeaderStyle::kBlockContent && !bConnectedAtDogLeg)
    //{
    //  if( gsMarker == OdDbMLeader::kBlockMark )
    //  {
    //    //OdGePoint3d ptBlockPos;
    //    //pMLeader->getBlockPosition( ptBlockPos );
    //    //OdDbGripData *pGrip = new OdDbGripData();
    //    //pGrip->setAppData(reinterpret_cast<void*>(BLOCK_POS_GRIP));
    //    //pGrip->setGripPoint(ptBlockPos);
    //    //grips.append(pGrip);
    //  }
    //}
  }

  return eOk;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

OdResult OdDbMleaderGripPointsPE::moveGripPointsAtSubentPaths( OdDbEntity* pEntity,
                                 const OdDbFullSubentPathArray& paths, 
                                 const OdDbVoidPtrArray& gripAppData,
                                 const OdGeVector3d& offset, 
                                 const OdUInt32 bitflags)
{
  OdDbMLeader* pMLeader = OdDbMLeader::cast(pEntity);
  bool bConnectedAtDogLeg = true,
       bUseDogLegCenterGrip = true,
       bSkipForLastVertex = false,
       bEnableDogleg = IsDoglegEnabled(pMLeader, &bConnectedAtDogLeg, &bUseDogLegCenterGrip, &bSkipForLastVertex);

  OdIntArray leaderIndexes;
  pMLeader->getLeaderIndexes(leaderIndexes);

  for( OdUInt32 i = 0; i < paths.size(); i++ )
  {
    OdDbObjectIdArray pArr = paths[i].objectIds();

    OdDbObjectId CurId = pArr[ pArr.size() - 1 ];
    OdDbObjectId ObjId = pMLeader->id();

    if( CurId != ObjId )
      continue;

    int gsMarker  = (int)paths[i].subentId().index();
    OdInt32 iGripType = OdPtrToInt32(gripAppData.at(i));

    if( gsMarker < OdDbMLeader::kLeaderLineMark || gsMarker >= OdDbMLeader::kBlockAttribute )
      continue;

    if( gsMarker >= OdDbMLeader::kDoglegMark && gsMarker < OdDbMLeader::kMTextMark )
    {
      for( OdUInt32 i = 0; i < leaderIndexes.size(); i++ )
      {
        if( gsMarker != OdDbMLeader::kDoglegMark + leaderIndexes[i] )
          continue;

        OdGePoint3d  ptConnect;
        OdGeVector3d vrDoglegDir;
        double dDoglegLength = 0, dScale = 1.0;
        bool bRes = GetConnectionData(pMLeader, leaderIndexes[i], bEnableDogleg, bConnectedAtDogLeg,
                                      ptConnect, vrDoglegDir, dDoglegLength, &dScale); // out
        if (bRes)
        {
          if( bEnableDogleg )
          {
            OdGePoint3d StartDoglegPt = ptConnect;
            OdGePoint3d EndDoglegPt( ptConnect + vrDoglegDir * dDoglegLength);

            if( iGripType == DOGLEG_START_GRIP )
            {
              if( dDoglegLength < 1e-8 )
                break;

              OdGePoint3d tmpPt = ptConnect;
              tmpPt += offset;

              OdGePoint3d tmpNewPt = ProjectPointToLine(StartDoglegPt, EndDoglegPt, tmpPt );

              double dNewLength = tmpNewPt.distanceTo(EndDoglegPt);

              bool bZeroLength = true;

              if( IsOnSegment( StartDoglegPt, EndDoglegPt, tmpNewPt) || 
                ( StartDoglegPt.distanceTo(tmpNewPt) < EndDoglegPt.distanceTo(tmpNewPt) )
                )
              {
                if( dNewLength > 1e-8 )
                  bZeroLength = false;
              }

              if( !bZeroLength )
              {
                OdGeVector3d vrNewOffset = tmpNewPt - ptConnect;
                pMLeader->moveMLeader( vrNewOffset, OdDbMLeader::kMoveContentAndDoglegPoints, false);
                pMLeader->setDoglegLength(leaderIndexes[i], dNewLength / dScale);
              }
            }
            else if (bUseDogLegCenterGrip && iGripType == DOGLEG_CENTER_GRIP)
            {
              pMLeader->moveMLeader(offset, OdDbMLeader::kMoveContentAndDoglegPoints, false);
            }
            else if( iGripType == DOGLEG_END_GRIP )
            {
              if( dDoglegLength < 1e-8 )
                break;

              OdGePoint3d tmpPt = EndDoglegPt;
              tmpPt += offset;

              OdGePoint3d tmpNewPt = ProjectPointToLine(StartDoglegPt, EndDoglegPt, tmpPt );

              OdGeVector3d tmpOffset = tmpNewPt - EndDoglegPt;

              double dNewLength = tmpNewPt.distanceTo(StartDoglegPt);
              bool bZeroLength = true;

              if( IsOnSegment( StartDoglegPt, EndDoglegPt, tmpNewPt) || 
                ( StartDoglegPt.distanceTo(tmpNewPt) > EndDoglegPt.distanceTo(tmpNewPt) )
                )
              {
                if( dNewLength > 1e-8 )
                  bZeroLength = false;
              }

              if( !bZeroLength )
              {
                ODA_ASSERT_ONCE_X(MLEADER, iGripType == DOGLEG_END_GRIP)
                pMLeader->setDoglegLength(leaderIndexes[i], dNewLength / dScale);
              }
            }
          }
        }
      }
    }
    else if( gsMarker >= OdDbMLeader::kLeaderLineMark && gsMarker < OdDbMLeader::kDoglegMark )
    {
      bool bMoveGripPt = false;

      for( OdUInt32 i = 0; i < leaderIndexes.size(); i++ )
      {
        OdIntArray leaderLineIndexes;
        pMLeader->getLeaderLineIndexes(leaderIndexes[i], leaderLineIndexes);

        for( OdUInt32 k = 0; k < leaderLineIndexes.size(); k++ )
        {
          if( gsMarker != OdDbMLeader::kLeaderLineMark + leaderLineIndexes[k] )
            continue;

          int nVertices = 0;
          if (pMLeader->numVertices(leaderLineIndexes[k], nVertices) == eOk)
          {
            if (bSkipForLastVertex)
              nVertices--;
            if( iGripType == LINE_START_GRIP && !bEnableDogleg)
            {
              pMLeader->moveMLeader( offset, OdDbMLeader::kMoveContentAndDoglegPoints, false);
            }
            else if( iGripType > LINE_START_GRIP && iGripType <= (OdInt32)(LINE_START_GRIP + nVertices) )
            {
              OdInt32 iVertex = iGripType - LINE_START_GRIP - 1;
              OdGePoint3d ptVertex;
              pMLeader->getVertex(leaderLineIndexes[k],iVertex,ptVertex);
              ptVertex += offset;
              pMLeader->setVertex(leaderLineIndexes[k],iVertex,ptVertex);
              bMoveGripPt = true;
              break;
            }
          }
        }

        if( bMoveGripPt )
          break;
      }
    }
    else if( gsMarker >= OdDbMLeader::kMTextMark )
    {
      if( pMLeader->contentType() == OdDbMLeaderStyle::kMTextContent )
      {
        if( gsMarker == OdDbMLeader::kMTextMark && iGripType == TEXT_POS_GRIP )
          pMLeader->moveMLeader( offset, OdDbMLeader::kMoveContentAndDoglegPoints, false);
      }
      //else if(pMLeader->contentType() == OdDbMLeaderStyle::kBlockContent && !bConnectedAtDogLeg)
      //{
      //  if( gsMarker == OdDbMLeader::kBlockMark && iGripType == BLOCK_POS_GRIP )
      //    pMLeader->moveMLeader( offset, OdDbMLeader::kMoveContentAndDoglegPoints, false);
      //}
    }
  }

  return eOk;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
