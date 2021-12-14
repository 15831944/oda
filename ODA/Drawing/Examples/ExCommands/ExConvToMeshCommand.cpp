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
#include "ExConvToMeshCommand.h"
#include "StaticRxObject.h"
#include "DbUserIO.h"
#include "DbCommandContext.h"
#include "DbEntity.h"
#include "Gi/GiGeometry.h"
#include "DbSubDMesh.h"
#include "DbBlockTableRecord.h"
#include "Wr/wrTriangulationParams.h"
#include "OdDToStr.h"

ConvToMeshCommand::ConvToMeshCommand()
{
  wrTriangulationParams params(true);
  m_faceter.faceterDevNormal = params.normalTolerance;
  m_faceter.faceterDevSurface = params.surfaceTolerance;
  m_faceter.faceterGridRatio = params.gridAspectRatio;
  m_faceter.faceterMaxEdgeLength = params.maxFacetEdgeLength;
  m_faceter.faceterMaxGrid = params.maxNumGridLines;
  //m_faceter.faceterMeshType = 
}

const OdString ConvToMeshCommand::globalName() const
{
  return OD_T("ConvToMesh");
}

const OdString ConvToMeshCommand::groupName() const
{
  return OD_T("SubDMesh commands");
}

void ConvToMeshCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  if (pIO.isNull())
    return;

  OdString tmp;
  unsigned int convertedCount = 0,
    selectedCount = 0,
    candidatesCount = 0;

  try
  {
    OdDbSelectionSetPtr pSet = pIO->select(OD_T("Select objects for converting to SubDMeshes:"), OdEd::kSelAllowSubentsAlways, 0, OdString::kEmpty);
    selectedCount = pSet->numEntities();
    if (selectedCount == 0)
    {
      pIO->putString(OD_T("Nothing selected."));
      return;
    }
    else
    {
      tmp.format(OD_T("%d Entities selected"), selectedCount);
      pIO->putString(tmp);
    }

    // surface_tol
    tmp.format(OD_T("SurfaceTol : %s"), odDToStr(m_faceter.faceterDevSurface, 'f', 6).c_str());
    m_faceter.faceterDevSurface = pIO->getReal(tmp, OdEd::kInpNonZero, m_faceter.faceterDevSurface);
    // normal_tol
    tmp.format(OD_T("NormalTol : %s"), odDToStr(m_faceter.faceterDevNormal, 'f', 6).c_str());
    m_faceter.faceterDevNormal = pIO->getReal(tmp, OdEd::kInpNonNeg | OdEd::kInpNonZero, m_faceter.faceterDevNormal);
    // max_edge_length
    tmp.format(OD_T("MaxEdgeLength : %s"), odDToStr(m_faceter.faceterMaxEdgeLength, 'f', 6).c_str());
    m_faceter.faceterMaxEdgeLength = pIO->getReal(tmp, OdEd::kInpNonNeg | OdEd::kInpNonZero, m_faceter.faceterMaxEdgeLength);
    // grid_aspect_ratio
    tmp.format(OD_T("GridAspectRatio : %s"), odDToStr(m_faceter.faceterGridRatio, 'f', 6).c_str());
    m_faceter.faceterGridRatio = pIO->getReal(tmp, OdEd::kInpNonNeg | OdEd::kInpNonZero, m_faceter.faceterGridRatio);
    // max_grid_lines
    tmp.format(OD_T("MaxGridLines : %d"), m_faceter.faceterMaxGrid);
    m_faceter.faceterMaxGrid = pIO->getInt(tmp, OdEd::kInpNonNeg | OdEd::kInpNonZero, m_faceter.faceterMaxGrid);

    OdDbSelectionSetIteratorPtr pIt = pSet->newIterator();
    unsigned int surfaceCount = 0;
    while (!pIt->done())
    {
      OdDbObjectPtr pObj = pIt->objectId().safeOpenObject(OdDb::kForWrite);

      OdDbEntityPtr pEntity = OdDbEntity::cast(pObj);
      if (pEntity.isNull())
      {
        pIO->putString(OD_T("Unsupported entity."));
        pIt->next();
        continue;
      }

      OdGePoint3dArray vertexArray;
      OdInt32Array faceArray;
      OdGiFaceData* faceData = NULL;
      OdDbFaceterSettings *pFaceter;
      pFaceter = &m_faceter;
      //pFaceter = NULL;

      OdDbSubDMeshPtr pMesh;
      OdResult res = oddbGetObjectMesh(pEntity, pFaceter, vertexArray, faceArray, faceData);

      if (res != eOk)
      {
        OdString tmp;
        tmp.format(OD_T("Error : %s\n"), OdError(res).description().c_str());
        pIO->putString(tmp);
        pIt->next();
        continue;
      }

      if (vertexArray.isEmpty() || faceArray.isEmpty())
      {
        OdString tmp;
        tmp.format(OD_T("Error: Empty data returned!\n"));
        pIO->putString(tmp);
        pIt->next();
        continue;
      }

      candidatesCount++;

      pMesh = OdDbSubDMesh::createObject();
      res = pMesh->setSubDMesh(vertexArray, faceArray, 0);
      if (res != eOk)
      {
        OdString tmp;
        tmp.format(OD_T("Error : %s\n"), OdError(res).description().c_str());
        pIO->putString(tmp);
        pIt->next();
        continue;
      }

      pMesh->setDatabaseDefaults(pDb);
      pMesh->setPropertiesFrom(pEntity);

      // Set SubDMesh Face color/material
      if (faceData)
      {
        const OdCmEntityColor* trueColors = faceData->trueColors();
        OdDbStub**             materials = faceData->materials();
        const OdGiMapper*      mappers = faceData->mappers();

        OdInt32 facesCnt;
        pMesh->numOfFaces(facesCnt);

        OdDbSubentId subId;
        OdCmColor clr;
        subId.setType(OdDb::kFaceSubentType);
        for (int i = 0; i < facesCnt; ++i)
        {
          subId.setIndex(i);
          if (trueColors)
          {
            if (trueColors[i] != OdCmEntityColor::kNone)
            {
              clr.setColor(trueColors[i].color());
              res = pMesh->setSubentColor(subId, clr);
              if (res != eOk)
              {
                OdString tmp;
                tmp.format(OD_T("Error : %s\n"), OdError(res).description().c_str());
                pIO->putString(tmp);
                pIt->next();
                continue;
              }
            }
          }
          if (materials)
          {
            if (materials[i])
            {
              res = pMesh->setSubentMaterial(subId, materials[i]);
              if (res != eOk)
              {
                OdString tmp;
                tmp.format(OD_T("Error : %s\n"), OdError(res).description().c_str());
                pIO->putString(tmp);
                pIt->next();
                continue;
              }
            }
          }
          if (mappers)
          {
            res = pMesh->setSubentMaterialMapper(subId, mappers[i]);
            if (res != eOk)
            {
              OdString tmp;
              tmp.format(OD_T("Error : %s\n"), OdError(res).description().c_str());
              pIO->putString(tmp);
              pIt->next();
              continue;
            }
          }
        }

        if (trueColors)
          delete trueColors;
        if (materials)
          delete materials;
        if (mappers)
          delete mappers;
        if (faceData)
          delete faceData;
      }

      // Get owner of entity for insertion
      OdDbObjectId ownerId = pEntity->ownerId();
      if (ownerId && ownerId.database())
      {
        // Open owner for writing
        OdDbBlockTableRecordPtr pBlock = ownerId.openObject(OdDb::kForWrite);
        // Append newly created entity
        pBlock->appendOdDbEntity(pMesh);
        // Open object for deletion
        pObj = pIt->objectId().safeOpenObject(OdDb::kForWrite);
        pEntity->erase();
        // Increment counter
        convertedCount++;
      }
      pIt->next();
    }

    tmp.format(OD_T("%d of %d Objects converted"), convertedCount, candidatesCount);
    pIO->putString(tmp);
  }
  catch (...)
  {
    pIO->putString(OD_T("Create SubDMesh Failed!"));
  }

}
