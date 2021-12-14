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
#ifndef __COLLADA_GEOMETRYIMPORTER_H__
#define __COLLADA_GEOMETRYIMPORTER_H__

#include "ColladaImporterBase.h"
#include "COLLADAFWMeshPrimitive.h"
#include "COLLADAFWMesh.h"
#include "COLLADAFWMeshVertexData.h"
#include "DbSubDMesh.h"
#include "COLLADAFWGeometry.h"


/** \details
  <group OdImport_Classes> 
*/
namespace TD_COLLADA_IMPORT
{
  /** \details
    This class implements the geometry importer.
  */
  class GeometryImporter : public ImporterBase
  {
    friend class ImporterBase;
  public:
    /** Constructor. 
    */
    GeometryImporter(DocumentImporter* documentImporter);

    /** Destructor. 
    */
    virtual ~GeometryImporter();

    bool import(const COLLADAFW::Geometry* pGeometry);

    bool importMesh(const COLLADAFW::Mesh* pMesh);

    bool importDbSubDMesh(const COLLADAFW::Mesh* pMesh);

    void importMeshPositions(const COLLADAFW::Mesh* pMesh, OdGePoint3dArray& positionsArray);

    void importMeshUVCoords(const COLLADAFW::Mesh* pMesh, int iSourceIndex, OdGePoint3dArray& vertexTextureArray);

    void importMeshNormals(const COLLADAFW::Mesh* pMesh, OdGeVector3dArray& vertexNormalsArray);

    template<class NumberArray> 
    void setPolygonMeshUVVertices(const NumberArray& uvArray, size_t stride, size_t startPosition, OdUInt32 vertsCount, OdGePoint3dArray& vertexTextureArray );

    void fillPolygonMeshMapPerSet(const COLLADAFW::MeshVertexData& uvCoordinates, const COLLADAFW::MeshVertexData::InputInfosArray& inputInfos,
                                  size_t sourceIndex, OdGePoint3dArray& vertexTextureArray);
  };
}

#endif // __COLLADA_GEOMETRYIMPORTER_H__
