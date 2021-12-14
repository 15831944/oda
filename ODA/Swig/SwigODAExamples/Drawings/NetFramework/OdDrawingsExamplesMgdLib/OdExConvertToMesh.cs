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

using System;
using System.Collections.Generic;
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib
{
  public static class OdExConvertToMesh
  {
    public static void ConvertToMesh(OdDbDatabase db)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      try
      {
        OdDbBlockTableRecord btr = db.getModelSpaceId().openObject() as OdDbBlockTableRecord;
        OdDbObjectIterator it = btr.newIterator();
        for (it.start(); !it.done(); it.step())
        {
          OdDbEntity entity = OdDbEntity.cast(it.entity());
          if (entity != null)
            EntityConvertToMesh(db, entity);
        }
      }
      catch (Exception err)
      {
        throw err;
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }
    }

    private static void EntityConvertToMesh(OdDbDatabase db, OdDbEntity obj)
    {
      if (obj == null || db == null)
        return;

      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        wrTriangulationParams fParams = new wrTriangulationParams(true);
        MeshFaceterSettings faceter = new MeshFaceterSettings();
        faceter.faceterDevNormal = fParams.normalTolerance;
        faceter.faceterGridRatio = fParams.gridAspectRatio;
        faceter.faceterMaxEdgeLength = fParams.maxFacetEdgeLength;
        faceter.faceterMaxGrid = (ushort)fParams.maxNumGridLines;
        faceter.faceterDevSurface = fParams.surfaceTolerance;

        OdGePoint3dArray vertexArray = new OdGePoint3dArray();
        OdInt32Array faceArray = new OdInt32Array();
        OdGiFaceData faceData = null;

        OdResult res = TD_Db.oddbGetObjectMesh(obj, faceter, vertexArray, faceArray, out faceData);

        if (res != OdResult.eOk)
          throw new OdError(res);

        if (vertexArray.Count == 0 || faceArray.Count == 0)
          throw new OdError(String.Format("Empty data returned!"));

        OdDbSubDMesh pMesh = OdDbSubDMesh.createObject();
        res = pMesh.setSubDMesh(vertexArray, faceArray, 0);
        if (res != OdResult.eOk)
          throw new OdError(res);

        pMesh.setDatabaseDefaults(db);
        pMesh.setPropertiesFrom(obj);

        // Set SubDMesh Face color/material
        if (faceData != null)
        {
          int facesCnt;
          pMesh.numOfFaces(out facesCnt);

          OdDbSubentId subId = new OdDbSubentId();
          OdCmColor clr = new OdCmColor();
          subId.setType(Teigha.Core.SubentType.kFaceSubentType);

          for (int i = 0; i < facesCnt; ++i)
          {
            subId.setIndex(new IntPtr(i));

            if (faceData.HasTrueColors())
            {
              if (faceData.TrueColor(i).colorMethod() != OdCmEntityColor.Items_ColorMethod.kNone)
              {
                clr.setColor(faceData.TrueColor(i).color());
                res = pMesh.setSubentColor(subId, clr);
                if (res != OdResult.eOk)
                  throw new OdError(res);
              }
            }

            if (faceData.HasMaterials())
            {
              if (faceData.Material(i) != null)
              {
                res = pMesh.setSubentMaterial(subId, new OdDbObjectId(faceData.Material(i)));
                if (res != OdResult.eOk)
                  throw new OdError(res);
              }
            }

            if (faceData.HasMappers())
            {
              res = pMesh.setSubentMaterialMapper(subId, faceData.Mapper(i));
              if (res != OdResult.eOk)
                throw new OdError(res);
            }
          }

          if (faceData.HasTrueColors())
            faceData.DeleteTrueColors();
          if (faceData.HasMaterials())
            faceData.DeleteMaterials();
          if (faceData.HasMappers())
            faceData.DeleteMappers();
        }

        // Get owner of entity for insertion
        OdDbObjectId ownerId = obj.ownerId();
        if (ownerId != null && ownerId.database() != null)
        {
          // Open owner for writing
          OdDbBlockTableRecord pBlock = (OdDbBlockTableRecord)ownerId.openObject(OpenMode.kForWrite);
          // Append newly created entity
          pBlock.appendOdDbEntity(pMesh);
          // Open object for deletion
          obj = obj.objectId().openObject(OpenMode.kForWrite) as OdDbEntity;
          // delete object
          obj.erase();
        }
      }
      catch (OdError err)
      {
        Console.WriteLine("Error: {0}", err.description());
      }
      catch (Exception err)
      {
        Console.WriteLine("Error: {0}", err.Message);
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }
    }
  }

}