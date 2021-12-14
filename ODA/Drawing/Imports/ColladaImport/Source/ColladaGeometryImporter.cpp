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
#include "COLLADAFWTypes.h"
#include "COLLADAFWGeometry.h"
#include "COLLADAFWTriangles.h"
#include "COLLADAFWTristrips.h"
#include "COLLADAFWTrifans.h"
#include "COLLADAFWPolygons.h"
#include "COLLADAFWUniqueId.h"
#include "ColladaDocumentImporter.h"
#include "ColladaGeometryImporter.h"


namespace TD_COLLADA_IMPORT
{
  bool isSupportedMeshPrimitive(const COLLADAFW::MeshPrimitive* meshPrimitive)
  {
    COLLADAFW::MeshPrimitive::PrimitiveType type = meshPrimitive->getPrimitiveType();
    return    (type == COLLADAFW::MeshPrimitive::TRIANGLES) 
      || (type == COLLADAFW::MeshPrimitive::TRIANGLE_FANS) 
      || (type == COLLADAFW::MeshPrimitive::TRIANGLE_STRIPS) 
      || (type == COLLADAFW::MeshPrimitive::POLYGONS) 
      || (type == COLLADAFW::MeshPrimitive::POLYLIST); 
  }

  GeometryImporter::GeometryImporter( DocumentImporter* documentImporter )
    : ImporterBase(documentImporter)
  {}

  GeometryImporter::~GeometryImporter()
  {}

  bool GeometryImporter::import(const COLLADAFW::Geometry* pGeometry)
  {
    bool bSuccess = true;
    if (pGeometry->getType() == COLLADAFW::Geometry::GEO_TYPE_MESH)
    {
      bSuccess = importMesh(static_cast<const COLLADAFW::Mesh*>(pGeometry));
    }
    return bSuccess;
  }

  bool GeometryImporter::importMesh(const COLLADAFW::Mesh* pMesh)
  {
    bool bSuccess = true;
    if (const_cast<COLLADAFW::Mesh*>(pMesh)->getPolygonsPolygonCount() > 0 ||
      const_cast<COLLADAFW::Mesh*>(pMesh)->getTrianglesTriangleCount() > 0 ||
      const_cast<COLLADAFW::Mesh*>(pMesh)->getTristripsTriangleCount() > 0 ||
      const_cast<COLLADAFW::Mesh*>(pMesh)->getTrifansTriangleCount() > 0)
    {
      bSuccess = importDbSubDMesh(pMesh);
    }

    return bSuccess;
  }

  bool GeometryImporter::importDbSubDMesh(const COLLADAFW::Mesh* pMesh)
  {
    OdResult res = eOk;
    OdDbBlockTableRecordPtr pBTR = this->getDocumentImporter()->getGeometryTmpBTR();

    OdGePoint3dArray positionsMeshArray;
    importMeshPositions(pMesh, positionsMeshArray);
    ODA_ASSERT(positionsMeshArray.size() > 0);
    if (positionsMeshArray.size() == 0)
      return false;

    OdGeVector3dArray vertexNormalsMeshArray;
    importMeshNormals(pMesh, vertexNormalsMeshArray);

    //TODO: should we read colors?

    OdGePoint3dArray vertexTextureMeshArray;
    int sourceIndex = 0; //It should be tested when several images are in a material.
    importMeshUVCoords(pMesh, sourceIndex, vertexTextureMeshArray);

    std::vector<OdUInt32> usedPositionsIdx;

    const COLLADAFW::MeshPrimitiveArray& meshPrimitiveArray = pMesh->getMeshPrimitives();
    size_t primitiveMeshCount = meshPrimitiveArray.getCount();
    for (size_t idxPrimitiveMesh = 0; idxPrimitiveMesh < primitiveMeshCount; ++idxPrimitiveMesh)
    {
      const COLLADAFW::MeshPrimitive* meshPrimitive = meshPrimitiveArray[idxPrimitiveMesh];
      if (!meshPrimitive)
        continue;
      OdDbSubDMeshPtr pSubDMesh = OdDbSubDMesh::createObject();
      pSubDMesh->setDatabaseDefaults(pBTR->database());
      OdDbObjectId objId = pBTR->appendOdDbEntity(pSubDMesh);

      COLLADAFW::MeshPrimitive::PrimitiveType type = meshPrimitive->getPrimitiveType();
      //TODO: implement other cases, e.g. LINES
      if (type == COLLADAFW::MeshPrimitive::TRIANGLE_STRIPS || type == COLLADAFW::MeshPrimitive::TRIANGLE_FANS)
      {
        ODA_FAIL(); //detect this case.
      }
      else if (type == COLLADAFW::MeshPrimitive::TRIANGLES || type == COLLADAFW::MeshPrimitive::POLYGONS)
      {
        const COLLADAFW::UIntValuesArray& positionIndices = meshPrimitive->getPositionIndices();
        const COLLADAFW::UIntValuesArray& normalIndices = meshPrimitive->getNormalIndices();
        const COLLADAFW::IndexListArray& uvIndexArray = meshPrimitive->getUVCoordIndicesArray();
        const COLLADAFW::UIntValuesArray* uvIndices = NULL;
        if (uvIndexArray.getCount() > 0)
          uvIndices = &uvIndexArray[sourceIndex]->getIndices();

        size_t faceCount = meshPrimitive->getFaceCount();

        //prepare arrays for OdDbSubDMesh
        //all of them are either copies of the global corresponding array
        //or are created from the global array through indices
        OdInt32Array faceArray;
        faceArray.reserve(OdUInt32(faceCount));
        OdGePoint3dArray vertexArray;
        if (positionIndices.getCount() > 0)
        {
          vertexArray.reserve(OdUInt32(odmin(positionsMeshArray.size(), positionIndices.getCount())));
        }
        else if (primitiveMeshCount == 1)
        {
          vertexArray = positionsMeshArray;
        }
        else
        {
          //can there be multiple primitive meshes and no indices?
          ODA_FAIL();
          return false;
        }

        OdGeVector3dArray vertexNormalsArray;
        if (vertexNormalsMeshArray.size() > 0)
        {
          //indices may be present only for some of primitive meshes
          if (normalIndices.getCount() > 0)
          {
            vertexNormalsArray.reserve(OdUInt32(vertexArray.size()));
          }
          else if (primitiveMeshCount == 1)
          {
            //this is theoretical case where indices are implicitly 0, 1, 2, ...
            vertexNormalsArray = vertexNormalsMeshArray;
          }
        }

        OdGePoint3dArray textureUvArray;
        if (vertexTextureMeshArray.size() > 0)
        {
          //indices may be present only for some of primitive meshes
          if (uvIndices)
          {
            textureUvArray.reserve(OdUInt32(vertexArray.size()));
            textureUvArray.clear();
          }
          else if (primitiveMeshCount == 1)
          {
            //this is theoretical case where indices are implicitly 0, 1, 2, ...
            textureUvArray = vertexTextureMeshArray;
          }
        }

        const static OdUInt32 INVALID_POS = ~1;
        usedPositionsIdx.assign(positionsMeshArray.size(), INVALID_POS);

        const COLLADAFW::IntValuesArray* faceVertexCountArray = NULL;
        if (type == COLLADAFW::MeshPrimitive::POLYGONS)
          faceVertexCountArray = &static_cast<const COLLADAFW::Polygons*>(meshPrimitive)->getGroupedVerticesVertexCountArray();

        //fill OdDbSubDMesh arrays
        size_t idxPrimitiveMeshVertex = 0;
        for (size_t idxCurrentFace = 0; idxCurrentFace < faceCount; ++idxCurrentFace)
        {
          //holes are marked with negative numbers
          int faceVertexCount = faceVertexCountArray ? Od_abs((*faceVertexCountArray)[idxCurrentFace]) : 3;
          faceArray.append(faceVertexCount);
          for (int idxFaceVertex = 0; idxFaceVertex < faceVertexCount; ++idxFaceVertex, ++idxPrimitiveMeshVertex)
          {
            //set vertex positions
            OdUInt32 positionIdx = positionIndices[idxPrimitiveMeshVertex];
            bool newVertex = true;
            OdUInt32 vertexIdx;
            if (positionIndices.getCount() > 0)
            {
              if (usedPositionsIdx[positionIdx] != INVALID_POS)
              {
                vertexIdx = usedPositionsIdx[positionIdx];
                newVertex = false;
              }
              else
              {
                vertexIdx = vertexArray.append(positionsMeshArray[positionIdx]);
                usedPositionsIdx[positionIdx] = vertexIdx;
              }
              faceArray.append(vertexIdx);
            }
            else
              faceArray.append(OdUInt32(idxPrimitiveMeshVertex));

            //set normals
            if (vertexNormalsMeshArray.size() > 0 && normalIndices.getCount() > 0)
            {
              OdUInt32 normalIdx = normalIndices[idxPrimitiveMeshVertex];
              const OdGeVector3d& normal = vertexNormalsMeshArray[normalIdx];
              if (newVertex)
                vertexNormalsArray.append(normal);
              else
              {
                OdGeVector3d newNormal = vertexNormalsArray[vertexIdx] + normal;
                //OdDbSubDMesh doesn't support face normals. Vertex normals can sometimes be zero if face normals are opposed
                //Thus, there may be problems with 2 sided planes...
                if (!newNormal.isZeroLength())
                  vertexNormalsArray[vertexIdx] = newNormal;
              }
            }

            //read and set texture uv coordinates
            //I assume that if vertex is met multiple times, its texture uvCoords are the same, so I take only first uvCoord
            if (newVertex && vertexTextureMeshArray.size() > 0 && uvIndices)
            {
              OdUInt32 uvIdx = (*uvIndices)[idxPrimitiveMeshVertex];
              textureUvArray.append(vertexTextureMeshArray[uvIdx]);
            }
          }
        }
        OdResult res = pSubDMesh->setSubDMesh(vertexArray, faceArray, 0);
        if (eOk == res && vertexNormalsArray.size() > 0)
        {
          OdGeVector3d* vertexNormalsPtr = vertexNormalsArray.asArrayPtr();
          //OdDbSubDMesh should normalize vectors but it doesn't, so we do it ourselves.
          //After .dwg -> .dae export by ODA normals can become zero (why?) so we should avoid exceptions here
          for (OdUInt32 idxNormal = 0; idxNormal < vertexNormalsArray.size(); ++idxNormal)
            vertexNormalsPtr[idxNormal].normalizeGetLength();
          res = pSubDMesh->setVertexNormalArray(vertexNormalsArray);
        }
        if (eOk == res && textureUvArray.size() > 0)
        {
          res = pSubDMesh->setVertexTextureArray(textureUvArray);
        }

        if (eOk == res)
        {
          handleObjectReferences(meshPrimitive, objId, meshPrimitive->getMaterialId());
          addUniqueIdSubUniqueIdPair(pMesh->getUniqueId(), meshPrimitive->getUniqueId());
        }
        else
        {
          return false;
        }
      }
    }
    return true;
  }


  void GeometryImporter::importMeshPositions(const COLLADAFW::Mesh* pMesh, OdGePoint3dArray& positionsArray)
  {
    const COLLADAFW::MeshVertexData& meshPositions = pMesh->getPositions();
    OdUInt32 positionsCount = OdUInt32(meshPositions.getValuesCount());
    positionsArray.resize(positionsCount / 3);
    OdGePoint3d* positionsArrayData = positionsArray.asArrayPtr();
    size_t i = 0;
    if (meshPositions.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE)
    {
      const COLLADAFW::DoubleArray* meshPositionsArray = meshPositions.getDoubleValues();
      for (OdUInt32 iArr = 0; i < positionsCount; ++iArr, i += 3)
      {
        positionsArrayData[iArr].x = (*meshPositionsArray)[i];
        positionsArrayData[iArr].y = (*meshPositionsArray)[i + 1];
        positionsArrayData[iArr].z = (*meshPositionsArray)[i + 2];
      }
    }
    else
    {
      const COLLADAFW::FloatArray* meshPositionsArray = meshPositions.getFloatValues();
      for (OdUInt32 iArr = 0; i < positionsCount; ++iArr, i += 3)
      {
        positionsArrayData[iArr].x = (*meshPositionsArray)[i];
        positionsArrayData[iArr].y = (*meshPositionsArray)[i + 1];
        positionsArrayData[iArr].z = (*meshPositionsArray)[i + 2];
      }
    }
  }

  template<class NumberArray> 
  void GeometryImporter::setPolygonMeshUVVertices(const NumberArray& uvArray, size_t stride, size_t startPosition, OdUInt32 vertsCount, OdGePoint3dArray& vertexTextureArray)
  {
    OdGePoint3d* vertexTextureArrayData = vertexTextureArray.asArrayPtr();
    size_t uvIndex = startPosition;
    switch (stride)
    {
    case 1:
      {
        for (OdUInt32 i = 0; i < vertsCount; ++i)
        {
          vertexTextureArrayData[i].x = uvArray[uvIndex++];
          vertexTextureArrayData[i].y = 0.0f;
          vertexTextureArrayData[i].z = 0.0f;
        }
       break;
      }
    case 2:
      {
        for (OdUInt32 i = 0; i < vertsCount; ++i)
        {
          vertexTextureArrayData[i].x = uvArray[uvIndex++];
          vertexTextureArrayData[i].y = 1.0f - uvArray[uvIndex++]; // Ac uses inverted coord
          vertexTextureArrayData[i].z = 0.0f;
        }
        break;
      }
    case 3:
      {
        for (OdUInt32 i = 0; i < vertsCount; ++i)
        {
          vertexTextureArrayData[i].x = uvArray[uvIndex++];
          vertexTextureArrayData[i].y = 1.0f - uvArray[uvIndex++]; // Ac uses inverted coord
          vertexTextureArrayData[i].z = uvArray[uvIndex++];
        }
        break;
      }
    case 4:
      {
        for (OdUInt32 i = 0; i < vertsCount; ++i)
        {
          vertexTextureArrayData[i].x = uvArray[uvIndex++];
          vertexTextureArrayData[i].y = 1.0f - uvArray[uvIndex++]; // Ac uses inverted coord
          vertexTextureArrayData[i].z = uvArray[uvIndex++];
          uvIndex++;
        }
        break;
      }
    default:
      ODA_ASSERT(false);
    }
  }

  void GeometryImporter::fillPolygonMeshMapPerSet(const COLLADAFW::MeshVertexData& uvCoordinates,
    const COLLADAFW::MeshVertexData::InputInfosArray& inputInfos,
    size_t sourceIndex,
    OdGePoint3dArray& vertexTextureArray)
  {
    ODA_ASSERT(inputInfos.getCount() == 1);
    const COLLADAFW::MeshVertexData::InputInfos* inputInfo = inputInfos[sourceIndex];

    size_t stride = inputInfo->mStride;
    OdUInt32 vertsCount = OdUInt32(inputInfo->mLength / stride);
    OdInt32 iNumVert = 0;

    // calculate first index position
    size_t startPosition = 0;
    for (size_t i = 0; i < sourceIndex; ++i)
    {
      const COLLADAFW::MeshVertexData::InputInfos* inputInfo = inputInfos[i];
      startPosition += inputInfo->mLength;
    }

    vertexTextureArray.resize(vertsCount);
    if (uvCoordinates.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE)
    {
      const COLLADAFW::DoubleArray& uvArray = *uvCoordinates.getDoubleValues();
      setPolygonMeshUVVertices(uvArray, stride, startPosition, vertsCount, vertexTextureArray);
    }
    else
    {
      const COLLADAFW::FloatArray& uvArray = *uvCoordinates.getFloatValues();
      setPolygonMeshUVVertices(uvArray, stride, startPosition, vertsCount, vertexTextureArray);
    }
  }

  void GeometryImporter::importMeshUVCoords(const COLLADAFW::Mesh* pMesh, int iSourceIndex, OdGePoint3dArray& vertexTextureArray)
  {
    if (!isImportTextures())
      return;
    const COLLADAFW::MeshVertexData& uvCoordinates = pMesh->getUVCoords();
    size_t iUVCoordinatesCount = uvCoordinates.getValuesCount();
    if (iUVCoordinatesCount > 0)
    {
      const COLLADAFW::MeshVertexData::InputInfosArray& uvInputInfos = uvCoordinates.getInputInfosArray();
      fillPolygonMeshMapPerSet( uvCoordinates, uvInputInfos, iSourceIndex, vertexTextureArray);
    }
  }

  void GeometryImporter::importMeshNormals(const COLLADAFW::Mesh* pMesh, OdGeVector3dArray& vertexNormalsArray)
  {
    if (!pMesh->hasNormals())
      return;

    // fill in the normals
    const COLLADAFW::MeshVertexData& meshNormals = pMesh->getNormals();
    size_t normalCount = meshNormals.getValuesCount() / 3;

    vertexNormalsArray.resize(OdUInt32(normalCount));
    if (meshNormals.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE)
    {
      const COLLADAFW::DoubleArray* normalsArray = meshNormals.getDoubleValues();
      for (OdUInt32 i = 0; i < normalCount; ++i)
      {
        vertexNormalsArray[i].x = (*normalsArray)[i * 3];
        vertexNormalsArray[i].y = (*normalsArray)[i * 3 + 1];
        vertexNormalsArray[i].z = (*normalsArray)[i * 3 + 2];
      }
    }
    else
    {
      const COLLADAFW::FloatArray* normalsArray = meshNormals.getFloatValues();
      for (OdUInt32 i = 0; i < normalCount; ++i)
      {
        vertexNormalsArray[i].x = (*normalsArray)[i * 3];
        vertexNormalsArray[i].y = (*normalsArray)[i * 3 + 1];
        vertexNormalsArray[i].z = (*normalsArray)[i * 3 + 2];
      }
    }
  }
}

