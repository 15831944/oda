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

#include "StdAfx.h"

#include "DbPolyline.h"
#include "DbLine.h"
#include "DbCircle.h"
#include "DbEllipse.h"
#include "DbArc.h"
#include "DbPolyline.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeEllipArc3d.h"

#include "GiContextForDbDatabase.h"
#include "Gi/GiBaseVectorizer.h"
#include "Gi/GiConveyorNode.h"
#include "Gi/GiConveyorEntryPoint.h"

class TextContourExplode : public OdStaticRxObject<OdGiConveyorNode>, private OdGiEmptyGeometry,
                           public OdGiConveyorInput, public OdGiConveyorOutput
{
  OdDbDatabasePtr m_pDb;
  OdGiBaseVectorizer *m_pVectorizer;
  int m_iplCount;

protected:
  void addSourceNode(OdGiConveyorOutput& sourceNode)
  {
    sourceNode.setDestGeometry(*this);
  }
  void removeSourceNode(OdGiConveyorOutput& /*sourceNode*/)
  {
  }
  void setDestGeometry(OdGiConveyorGeometry& destGeometry)
  {
  }
  OdGiConveyorGeometry& destGeometry() const
  {
    return OdGiEmptyGeometry::kVoid;
  }
  OdGiConveyorInput& input() { return *this; }
  OdGiConveyorOutput& output() { return *this; }
public:
  TextContourExplode(const OdDbDatabase *pDb, int iplCount)
    : m_pDb(pDb)
    , m_pVectorizer(NULL)
    , m_iplCount(iplCount)
  {
  }
  void setVectorizer(OdGiBaseVectorizer *pVect) { m_pVectorizer = pVect; }

  void appendEntity(OdDbEntity * pEnt)
  {
    // Set Properties
    const OdGiSubEntityTraitsData& traits = m_pVectorizer->effectiveTraits();

    pEnt->setLayer(traits.layer());

    //Color
    OdCmColor cmColor;
    cmColor.setColor(traits.trueColor().color());
    pEnt->setColor(cmColor);

    pEnt->setLinetype(traits.lineType(), false);
    pEnt->setLinetypeScale(traits.lineTypeScale(), false);
    pEnt->setLineWeight(traits.lineWeight(), false);
    pEnt->setPlotStyleName(traits.plotStyleNameType(), traits.plotStyleNameId(), false);
    pEnt->setMaterial(traits.material(), false);
    pEnt->setMaterialMapper(traits.mapper(), false);

    double dThickness = traits.thickness();
    if (!OdZero(dThickness))
    {
      OdDbPolylinePtr pPline = OdDbPolyline::cast(pEnt);
      if (!pPline.isNull())
      {
        pPline->setThickness(dThickness);
      }
      else
      {
        OdDb2dPolylinePtr pPline = OdDb2dPolyline::cast(pEnt);
        if (!pPline.isNull())
        {
          pPline->setThickness(dThickness);
        }
        else
        {
          OdDbCirclePtr pCirc = OdDbCircle::cast(pEnt);
          if (!pCirc.isNull())
          {
            pCirc->setThickness(dThickness);
          }
          else
          {
            OdDbLinePtr pLine = OdDbLine::cast(pEnt);
            if (!pLine.isNull())
            {
              pLine->setThickness(dThickness);
            }
            else
            {
              OdDbArcPtr pArc = OdDbArc::cast(pEnt);
              if (!pArc.isNull())
              {
                pArc->setThickness(dThickness);
              }
            }
          }
        }
      }
    }
    OdDbBlockTableRecord::cast(m_pDb->getActiveLayoutBTRId().openObject(OdDb::kForWrite))->appendOdDbEntity(pEnt);
  }

  void polylineProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal,
    const OdGeVector3d* pExtrusion, OdGsMarker baseSubEntMarker)
  {
    OdDbEntityPtr pEnt;
    if (numPoints == 2)
    {
      OdDbLinePtr pLine = OdDbLine::createObject();
      pLine->setStartPoint(*vertexList++);
      pLine->setEndPoint(*vertexList);
      if (pNormal)
      {
        pLine->setNormal(*pNormal);
      }
      pEnt = pLine;
    } else
    {
      bool bClosed = (*vertexList == vertexList[numPoints-1]);
      if (bClosed)
      {
        --numPoints;
      }
      OdGeVector3d vNormal;
      if (pNormal)
        vNormal = *pNormal;
      else if (geCalculateNormal(vertexList, numPoints, &vNormal) != OdGe::kOk)
      {
        ODA_FAIL(); // shoul not be 3d polyline for text
        return;
      }
      OdGeMatrix3d matrix;
      if (vNormal.isParallelTo(OdGeVector3d::kZAxis))
      {
        vNormal = OdGeVector3d::kZAxis;
      }
      matrix.setToWorldToPlane(vNormal);

      OdDbPolylinePtr pLine = OdDbPolyline::createObject();
      pEnt = pLine;

      pLine->setNormal(vNormal);
      OdGePoint3d pnt = matrix * vertexList[0];
      pLine->setElevation(pnt.z);
      matrix.setTranslation(OdGeVector3d(0., 0., -pnt.z));

      for(OdInt32 f=0; f<numPoints; ++f)
      {
        pnt = matrix * vertexList[f];
        ODA_ASSERT(OdZero(pnt.z / 2.));
        pLine->addVertexAt(f, OdGePoint2d(pnt.x, pnt.y));
      }
      if (bClosed)
      {
        pLine->setClosed(true);
      }
    }
    appendEntity(pEnt);
  }

  virtual void circleProc(
    const OdGePoint3d& center, 
    double radius, 
    const OdGeVector3d& normal, 
    const OdGeVector3d* pExtrusion)
  {
    OdDbCirclePtr pCirc = OdDbCircle::createObject();
    pCirc->setCenter(center);
    pCirc->setRadius(radius);
    pCirc->setNormal(normal);
    appendEntity(pCirc);
  }
  void circularArcProc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType,
    const OdGeVector3d* pExtrusion)
  {
    OdGeCircArc3d arc(firstPoint, secondPoint, thirdPoint);
    OdDbArcPtr pArc = OdDbArc::createObject();
    pArc->setFromOdGeCurve(arc, (OdGeVector3d*)pExtrusion);
    appendEntity(pArc);
  }

  void ellipArcProc(const OdGeEllipArc3d& ellipse,
                                     const OdGePoint3d* , OdGiArcType , const OdGeVector3d* pExtrusion)
  {
    OdDbEntityPtr pEnt;
    if (ellipse.isCircular())
    {
      OdGeCircArc3d arc(ellipse.center(), ellipse.normal(), ellipse.majorAxis(), ellipse.majorRadius(),
        ellipse.startAng(), ellipse.endAng());
      if (arc.isClosed())
      {
        OdDbCirclePtr pCirc = OdDbCircle::createObject();
        pCirc->setFromOdGeCurve(arc);
        pEnt = pCirc;
      }
      else
      {
        OdDbArcPtr pArc = OdDbArc::createObject();
        pArc->setFromOdGeCurve(arc, (OdGeVector3d*)pExtrusion);
        pEnt = pArc;
      }
    }
    else
    {
      OdDbEllipsePtr pArc = OdDbEllipse::createObject();
      pArc->setFromOdGeCurve(ellipse, (OdGeVector3d*)pExtrusion);
      pEnt = pArc;
    }
    appendEntity(pEnt);
  }

  void shellProc(
    OdInt32 /*numVertices*/,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* /*pEdgeData*/ = 0,
    const OdGiFaceData* /*pFaceData*/ = 0,
    const OdGiVertexData* /*pVertexData*/ = 0)
  {
    OdInt32 it = 0;
    while (it < faceListSize)
    {
      OdDb2dPolylinePtr pPl = OdDb2dPolyline::createObject();
      for (OdInt32 id = 1; id < abs(faceList[it]) + 1; id++)
      {
        OdDb2dVertexPtr pVert = OdDb2dVertex::createObject();
        pVert->setPosition(vertexList[faceList[id + it]]);
        pPl->appendVertex(pVert);
      }
      OdDbBlockTableRecord::cast(m_pDb->getActiveLayoutBTRId().openObject(OdDb::kForWrite))->appendOdDbEntity(pPl);
      it += Od_abs(faceList[it]) + 1;
    }
  }
  enum pdFlags
  {
    kpdCloseFigure = 0x01,
    kpdLineTo      = 0x02,
    kpdBezierTo    = 0x04,
    kpdMoveTo      = 0x06
  };
  void ttfPolyDrawProc(OdInt32 numVertices, const OdGePoint3d* vertexList,
                       OdInt32 /*faceListSize*/, const OdInt32* /*faceList*/,
                       const OdUInt8* pBezierTypes, const OdGiFaceData* /*pFaceData*/ = 0)
  {
    OdGeVector3d vNormal;
    if (geCalculateNormal(vertexList, numVertices, &vNormal) != OdGe::kOk)
    {
      ODA_FAIL(); // shoul not be 3d polyline for text
      return;
    }
    OdGeMatrix3d matrix;
    if (vNormal.isParallelTo(OdGeVector3d::kZAxis))
    {
      vNormal = OdGeVector3d::kZAxis;
    }
    matrix.setToWorldToPlane(vNormal);
    OdGePoint3d pnt = matrix * vertexList[0];
    double dElevation = pnt.z;
    matrix.setTranslation(OdGeVector3d(0., 0., -dElevation));
    int nVerts = 0;

    OdDbPolylinePtr pPl;
    int bzCnt = 0;
    OdGePoint3d bzSpl[4];
    for (OdInt32 i = 0; i < numVertices; i++)
    {
      if (pBezierTypes[i] == kpdMoveTo)
      {
        pPl = OdDbPolyline::createObject();
        pPl->setNormal(vNormal);
        pPl->setElevation(dElevation);
        nVerts = 0;
        pnt = matrix * vertexList[i];
        ODA_ASSERT(OdZero(pnt.z / 2.));
        pPl->addVertexAt(nVerts++, OdGePoint2d(pnt.x, pnt.y));
      }
      else
      {
        if (GETBIT(pBezierTypes[i], kpdLineTo))
        {
          pnt = matrix * vertexList[i];
          ODA_ASSERT(OdZero(pnt.z / 2.));
          pPl->addVertexAt(nVerts++, OdGePoint2d(pnt.x, pnt.y));
        }
        else if (GETBIT(pBezierTypes[i], kpdBezierTo))
        {
          bzCnt++;
          bzSpl[bzCnt] = vertexList[i];
          if (bzCnt == 3)
          {
            const int nSegs = m_iplCount;
            // This is sequence of 3 bezierTo points + previous point, so when we collect 3rd bezierTo,
            // we could contruct cubic spline based on 4 points.
            pPl->getEndPoint(bzSpl[0]);
            double d = 1. / nSegs;
            for (int n = 1; n < nSegs; n++)
            {
              double t = d * n;
              double u = 1.0 - t;
              double tt = t * t, uu = u * u;
              double uuu = uu * u, ttt = tt * t;
              double uut3 = uu * t * 3, utt3 = u * tt * 3;
          
              pnt = matrix * OdGePoint3d(
                        bzSpl[0].x * uuu + bzSpl[1].x * uut3 + bzSpl[2].x * utt3 + bzSpl[3].x * ttt,
                        bzSpl[0].y * uuu + bzSpl[1].y * uut3 + bzSpl[2].y * utt3 + bzSpl[3].y * ttt,
                        bzSpl[0].z * uuu + bzSpl[1].z * uut3 + bzSpl[2].z * utt3 + bzSpl[3].z * ttt);
              ODA_ASSERT(OdZero(pnt.z / 2.));
              pPl->addVertexAt(nVerts++, OdGePoint2d(pnt.x, pnt.y));
            }

            pnt = matrix * vertexList[i];
            ODA_ASSERT(OdZero(pnt.z / 2.));
            pPl->addVertexAt(nVerts++, OdGePoint2d(pnt.x, pnt.y));
            bzCnt = 0;
          }
        }
      }
      if (GETBIT(pBezierTypes[i], kpdCloseFigure))
      {

        appendEntity(pPl);

        pPl = OdDbPolyline::createObject();
        pPl->setNormal(vNormal);
        pPl->setElevation(dElevation);
        nVerts = 0;

        OdGePoint3d ptPos(matrix * vertexList[i]);
        ODA_ASSERT(OdZero(ptPos.z / 2.));
        pPl->addVertexAt(nVerts++, OdGePoint2d(ptPos.x, ptPos.y));
      }
    }
  }
  virtual void textProc(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    const OdChar* msg, 
    OdInt32 numBytes, 
    bool raw, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0)
  {
    m_pVectorizer->context()->drawText(this, position, direction, upVector, msg, numBytes, raw, pTextStyle, pExtrusion);
  }
};

class OdGiDrawTextForContourExplode : public OdGiBaseVectorizer
                                    , public OdGiContextForDbDatabase
{
  public:
    ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);

    OdGiDrawTextForContourExplode()
    {
      OdGiBaseVectorizer::m_flags |= (kDrawInvisibleEnts|kDrawLayerOff|kDrawLayerFrozen);
      setContext(this); 
      m_pModelToEyeProc->setDrawContext(OdGiBaseVectorizer::drawContext());
    }
    void attachOutputNode(OdGiConveyorNode *pNode)
    {
      pNode->input().addSourceNode(output());
    }
    OdGiRegenType regenType() const
    {
      return kOdGiForExplode;
    }
    void draw(const OdGiDrawable* pDrawable)
    {
      pDrawable->setAttributes(this);
      pDrawable->worldDraw(this);
    }
    const OdGiSubEntityTraitsData& effectiveTraits() const
    {
      return m_entityTraitsData;
    }
    void affectTraits(const OdGiSubEntityTraitsData* , OdGiSubEntityTraitsData& ) const
    {
    }
};

void _TxtExp_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbSelectionSetPtr pSel = pIO->select(L"Select texts to be exploded: ");
  if (pSel->numEntities() == 0)
  {
    pIO->putString(L"Nothing selected.");
    return;
  }
  OdDbSelectionSetIteratorPtr pIter = pSel->newIterator();
  int nPointsPerSplineSeg = pDb->getSPLINESEGS();

  if (nPointsPerSplineSeg < 0)
    nPointsPerSplineSeg = -nPointsPerSplineSeg;
  else if (nPointsPerSplineSeg == 0)
    nPointsPerSplineSeg = 1;

  TextContourExplode textExp(pDb, nPointsPerSplineSeg);
  OdStaticRxObject<OdGiDrawTextForContourExplode> vect;
  vect.setDatabase(pDb);
  textExp.setVectorizer(&vect);
  vect.attachOutputNode(&textExp);
  vect.setTtfPolyDrawMode(true);

  for ( ; !pIter->done(); pIter->next() )
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast( pIter->objectId().openObject() );
    vect.draw(pEnt);
    pEnt->upgradeOpen();
    pEnt->erase();
  }
}
