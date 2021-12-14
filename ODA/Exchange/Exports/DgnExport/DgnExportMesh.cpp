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
#include "DgnExportMesh.h"
#include "DgnExportCurve.h"
#include "DgnExportCommon.h"
#include "DgnExportContext.h"
#include <DbPolyFaceMesh.h>
#include <DgCellHeader.h>
#include <DgComplexShape.h>
#include <DgComplexString.h>
#include <DbFaceRecord.h>
#include <DbLayerTableRecord.h>
#include <DbPolyFaceMeshVertex.h>
#include <DbPolygonMesh.h>
#include <DbPolygonMeshVertex.h>
#include "DgnExport3dObjects.h"
#include <DbSubDMesh.h>
#include <Db3dSolid.h>
#include <DbSurface.h>

namespace TD_DGN_EXPORT {

  //===================================================================================================

  bool isEqualColors(const OdArray<OdDgCmColor>& arrFaceColors)
  {
    OdCmColor baseColor;

    if (!arrFaceColors.isEmpty())
      baseColor = arrFaceColors[0];
    else
      return true;

    for (OdUInt32 j = 0; j < arrFaceColors.size(); j++)
    {
      if (arrFaceColors[j] != baseColor)
        return false;
    }

    return true;
  }

  //===================================================================================================

  void createShellLoops(std::set<OdUInt64>& setEdges,
                          const OdGePoint3dArray& arrPts,
                            OdArray<OdGePoint3dArray>& arrLoops)
  {
    while( !setEdges.empty() )
    {
      OdUInt32 uStartIndex = 0;
      OdUInt32 uLastIndex  = 0;

      OdUInt64 uStartEdge = *(setEdges.begin());
      setEdges.erase(uStartEdge);

      uLastIndex  = (OdUInt32)uStartEdge;
      uStartIndex = (OdUInt32)(uStartEdge >> 32);

      OdGePoint3dArray arrLoop;
      arrLoop.push_back(arrPts[uStartIndex]);
      arrLoop.push_back(arrPts[uLastIndex]);

      bool bAddSegment = true;

      while( bAddSegment )
      {
        bAddSegment = false;

        std::set<OdUInt64>::iterator pIter = setEdges.begin();

        for (; pIter != setEdges.end(); pIter++)
        {
          OdUInt64 uCurEdge = *pIter;

          OdUInt32 uCurEndIndex   = (OdUInt32)uCurEdge;
          OdUInt32 uCurStartIndex = (OdUInt32)(uCurEdge >> 32);

          if( uCurStartIndex == uLastIndex )
          {
            arrLoop.push_back(arrPts[uCurEndIndex]);
            uLastIndex = uCurEndIndex;
            setEdges.erase(uCurEdge);
            bAddSegment = true;
          }
          else if( uCurEndIndex == uLastIndex )
          {
            arrLoop.push_back(arrPts[uCurStartIndex]);
            uLastIndex = uCurStartIndex;
            setEdges.erase(uCurEdge);
            bAddSegment = true;
          }

          if( bAddSegment )
          {
            if( uStartIndex == uLastIndex )
            {
              arrLoops.push_back(arrLoop);
              arrLoop.clear();
            }
            break;
          }
        }

        if( arrLoop.isEmpty() )
          break;
      }
    }
  }

  //===================================================================================================
  
  void OdDbPolyfaceMeshExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
  {
    OdDbPolyFaceMesh* pDbMesh = (OdDbPolyFaceMesh*)(pDbEntity);

    OdDgGraphicsElementPtr pDgMesh;

    OdGePoint3dArray  arrPts;
    OdArray<OdUInt32Array> arrFaces;
    OdArray<OdArray<bool> > arrEdgeVisibility;
    OdArray<OdDgCmColor> arrFaceColors;

    OdUInt32 i, j;

    OdDgCmColor meshColor = pDbMesh->color();
    OdDgCmColor baseColor;
    bool bInitBaseColor = false;

    if (meshColor.colorMethod() == OdCmEntityColor::kByLayer)
    {
      OdDbObjectId idLayer = pDbMesh->layerId();

      if (!idLayer.isNull())
      {
        OdDbLayerTableRecordPtr pLayer = idLayer.openObject(OdDb::kForRead);

        if (!pLayer.isNull())
        {
          if( pLayer->color().colorMethod() != OdCmEntityColor::kByColor )
            meshColor.setColorIndex(pLayer->colorIndex());
          else
          {
            OdCmColor layerColor = pLayer->color();
            meshColor.setRGB(layerColor.red(), layerColor.green(), layerColor.blue());
          }
        }
      }
    }

    OdDbObjectIteratorPtr pOIt = pDbMesh->vertexIterator();

    while (!pOIt->done())
    {
      OdDbFaceRecordPtr pFace = OdDbFaceRecord::cast(pOIt->entity());

      if( !pFace.isNull() )
      {
        OdUInt32Array arrVertices;
        OdArray<bool> arrEdgeVis;
        OdUInt32 uStartI = 1;

        for (i = 0; i < 4; i++)
        {
          if (pFace->getVertexAt(i) != 0)
          {
            arrVertices.push_back(abs(pFace->getVertexAt(i)) - 1);
            arrEdgeVis.push_back(pFace->isEdgeVisibleAt(i));
            uStartI = i + 1;
          }
        }

        for (i = uStartI; i < 4; i++)
        {
          if (pFace->getVertexAt(i) == 0)
            continue;

          OdInt16 uVertexIndex = (abs)(pFace->getVertexAt(i)) - 1;

          if (uVertexIndex != arrVertices.last())
          {
            arrVertices.push_back(uVertexIndex);
            arrEdgeVis.push_back(pFace->isEdgeVisibleAt(i));
          }
          else
            arrEdgeVis.last() = pFace->isEdgeVisibleAt(i);
        }

        OdDgCmColor faceColor = pFace->color();
        OdDgCmColor faceColorUpdated = faceColor;

        if (!bInitBaseColor)
        {
          baseColor = faceColor;
          bInitBaseColor = true;
        }

        if( faceColor.colorMethod() == OdCmEntityColor::kByLayer )
        {
          OdDbObjectId idLayer = pFace->layerId();

          if( !idLayer.isNull() )
          {
            OdDbLayerTableRecordPtr pLayer = idLayer.openObject(OdDb::kForRead);

            if (!pLayer.isNull())
            {
              if (pLayer->color().colorMethod() != OdCmEntityColor::kByColor)
                faceColorUpdated.setColorIndex(pLayer->colorIndex());
              else
              {
                OdCmColor layerColor = pLayer->color();
                faceColorUpdated.setRGB(layerColor.red(), layerColor.green(), layerColor.blue());
              }
            }
            else
              faceColorUpdated = meshColor;
          }
          else
            faceColorUpdated = meshColor;
        }
        else if( (faceColor.colorMethod() == OdCmEntityColor::kByBlock) || (faceColor.colorMethod() == OdCmEntityColor::kByPen) )
          faceColorUpdated = meshColor;

        arrFaceColors.push_back(faceColorUpdated);
        arrFaces.push_back(arrVertices);
        arrEdgeVisibility.push_back(arrEdgeVis);
      }
      else
      {
        OdDbPolyFaceMeshVertexPtr pVertex = OdDbPolyFaceMeshVertex::cast(pOIt->entity());

        if( !pVertex.isNull() )
          arrPts.push_back(pVertex->position());
      }

      pOIt->step();
    }

    bool bPlanar      = isPlanar(arrPts);
    bool bEqualColor  = isEqualColors(arrFaceColors);

    if (baseColor != meshColor)
      bEqualColor = false;

    if( bPlanar && bEqualColor )
    {
      std::set<OdUInt64> setEdges;

      for( i = 0; i < arrFaces.size(); i++ )
      {
        if( arrFaces[i].size() < 3 )
          continue;

        for( j = 0; j < arrFaces[i].size(); j++ )
        {
          OdUInt32 uStartIndex = arrFaces[i][j];
          OdUInt32 uEndIndex;

          if( j < arrFaces[i].size() - 1 )
            uEndIndex = arrFaces[i][j+1];
          else
            uEndIndex = arrFaces[i][0];

          if( uStartIndex > uEndIndex )
          {
            OdUInt32 uTmp = uEndIndex;
            uEndIndex = uStartIndex;
            uStartIndex = uTmp;
          }

          OdUInt64 uIndexKey = uStartIndex;
          uIndexKey = uIndexKey << 32;
          uIndexKey += uEndIndex;

          std::set<OdUInt64>::iterator pSetIter = setEdges.find(uIndexKey);

          if (pSetIter != setEdges.end())
            setEdges.erase(uIndexKey);
          else
            setEdges.insert(uIndexKey);
        }
      }

      OdArray<OdGePoint3dArray> arrLoops;
      createShellLoops(setEdges, arrPts, arrLoops);

      if( !arrLoops.isEmpty() )
      {
        if (arrLoops.size() == 1)
          pDgMesh = createDgShape(arrLoops[0], pDbEntity, owner->database());
        else
        {
          if (OdDgnExportContext::is3d())
            pDgMesh = OdDgCellHeader3d::createObject();
          else
            pDgMesh = OdDgCellHeader2d::createObject();

          pDgMesh->setDatabaseDefaults(owner->database());
          pDgMesh->setLevelEntryId(0);

          for (i = 0; i < arrLoops.size(); i++)
          {
            OdDgGraphicsElementPtr pDgLoop = createDgShape(arrLoops[i], pDbEntity, owner->database());
            pDgLoop->setHbitFlag(i != 0);
            copyElementProperties(pDgLoop, pDbMesh, owner->database());

            if( bInitBaseColor )
              pDgLoop->setColorIndex(getDgnColorIndex(baseColor, owner->database()));

            appendOdDgElementToOwner(pDgMesh, pDgLoop);
          }
        }
      }
    }
    else
    {
      OdDgMeshFaceLoopsPtr pDgnLoopsMesh = OdDgMeshFaceLoops::createObject();
      pDgnLoopsMesh->setDatabaseDefaults(owner->database());
      pDgnLoopsMesh->setPointsNumber(arrPts.size());

      for (i = 0; i < arrPts.size(); i++)
        pDgnLoopsMesh->setPoint(i, arrPts[i]);

      pDgnLoopsMesh->setUseColorTableIndexesFlag(!bEqualColor);

      for (i = 0; i < arrFaces.size(); i++)
      {
        if (arrFaces[i].size() < 3)
          continue;

        OdDgMeshFaceLoops::VerticesArray facePts;

        for( j = 0; j < arrFaces[i].size(); j++ )
        {
          OdDgMeshFaceLoops::FacePoint curPt;

          curPt.m_pointIndex          = arrFaces[i][j];
          curPt.m_impliesVisibleEdge  = arrEdgeVisibility[i][j];

          if( !bEqualColor )
            curPt.m_colorIndex = getDgnColorIndex(arrFaceColors[i], owner->database());

          facePts.push_back(curPt);
        }

        pDgnLoopsMesh->addFace(facePts);
      }

      pDgMesh = pDgnLoopsMesh;
    }

    if( !pDgMesh.isNull() )
    {
      addElementToDatabase(pDgMesh, owner);
      copyElementProperties(pDgMesh, pDbMesh, owner->database());
      appendOdDgElementToOwner(owner, pDgMesh);

      if( bEqualColor && bInitBaseColor )
        pDgMesh->setColorIndex(getDgnColorIndex(baseColor, owner->database()));

      OdDgnExportContext::addDgnElement(pDbMesh->objectId(), pDgMesh->elementId());
    }
  }

  //===================================================================================================

  void OdDbPolygonMeshExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
  {
    OdDbPolygonMesh* pDbMesh = (OdDbPolygonMesh*)(pDbEntity);

    OdDgGraphicsElementPtr pDgMesh;

    OdGePoint3dArray arrMeshPts;

    OdUInt32 i,j;

    OdUInt32 nRows = pDbMesh->mSize();
    OdUInt32 nColumns = pDbMesh->nSize();

    if (pDbMesh->polyMeshType() != OdDb::kSimpleMesh)
    {
      nRows = pDbMesh->mSurfaceDensity();
      nColumns = pDbMesh->nSurfaceDensity();
    }

    OdDbObjectIteratorPtr pOIt = pDbMesh->vertexIterator();

    while (!pOIt->done())
    {
      OdDbPolygonMeshVertexPtr pVertex = OdDbPolygonMeshVertex::cast(pOIt->entity());

      if( !pVertex.isNull() )
      {
        if( pDbMesh->polyMeshType() == OdDb::kSimpleMesh )
        {
          if( pVertex->vertexType() == OdDb::k3dSimpleVertex )
            arrMeshPts.push_back(pVertex->position());
        }
        else
        {
          if( pVertex->vertexType() != OdDb::k3dControlVertex )
            arrMeshPts.push_back(pVertex->position());
        }
      }

      pOIt->step();
    }

    if( arrMeshPts.size() == nRows*nColumns )
    {
      OdDgMeshFaceLoopsPtr pDgnLoopsMesh = OdDgMeshFaceLoops::createObject();
      pDgnLoopsMesh->setDatabaseDefaults(owner->database());
      pDgnLoopsMesh->setPointsNumber(arrMeshPts.size());

      for (i = 0; i < arrMeshPts.size(); i++)
        pDgnLoopsMesh->setPoint(i, arrMeshPts[i]);

      for( i = 0; i < nRows; i++ )
      {
        if((i == nRows - 1) && !pDbMesh->isMClosed())
          continue;

        for( j = 0; j < nColumns; j++ )
        {
          OdDgMeshFaceLoops::VerticesArray arrFacePts;

          if( (j == nColumns - 1) && !pDbMesh->isNClosed() )
            continue;

          OdDgMeshFaceLoops::FacePoint pt1, pt2, pt3, pt4;

          pt1.m_pointIndex = i * nColumns + j;

          if( (j == nColumns - 1) && pDbMesh->isNClosed() )
            pt2.m_pointIndex = i * nColumns;
          else
            pt2.m_pointIndex = i * nColumns + j + 1;

          if( (i == nRows - 1) && pDbMesh->isMClosed())
          {
            if ((j == nColumns - 1) && pDbMesh->isNClosed())
              pt3.m_pointIndex = 0;
            else
              pt3.m_pointIndex = j + 1;
          }
          else
          {
            if ((j == nColumns - 1) && pDbMesh->isNClosed())
              pt3.m_pointIndex = (i + 1) * nColumns;
            else
              pt3.m_pointIndex = (i + 1) * nColumns + j + 1;
          }

          if ((i == nRows - 1) && pDbMesh->isMClosed())
            pt4.m_pointIndex = j;
          else
            pt4.m_pointIndex = (i + 1) * nColumns + j;

          arrFacePts.push_back(pt1);
          arrFacePts.push_back(pt2);
          arrFacePts.push_back(pt3);
          arrFacePts.push_back(pt4);

          pDgnLoopsMesh->addFace(arrFacePts);
        }
      }

      pDgMesh = pDgnLoopsMesh;
    }

    if (!pDgMesh.isNull())
    {
      addElementToDatabase(pDgMesh, owner);
      copyElementProperties(pDgMesh, pDbMesh, owner->database());
      appendOdDgElementToOwner(owner, pDgMesh);
      OdDgnExportContext::addDgnElement(pDbMesh->objectId(), pDgMesh->elementId());
    }
  }

  //===================================================================================================

  void OdDbSubDMeshExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
  {
    OdDbSubDMesh* pDbMesh = (OdDbSubDMesh*)(pDbEntity);
    pDbMesh->upgradeOpen();
    pDbMesh->subdRefine();
    pDbMesh->downgradeOpen();

    OdDgnExportPE::subExportElement(pDbEntity, owner);
  }

  //===================================================================================================
}
