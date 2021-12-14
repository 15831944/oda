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
#ifndef ODGI_PROGRESSIVEMESH_H_INCLUDED
#define ODGI_PROGRESSIVEMESH_H_INCLUDED

#include "OdStreamBuf.h"
#include "OdVector.h"
#include "Ge/GeExtents3d.h"
#include "GiGeometry.h"
class OdGiViewport;
class OdGiProgressiveMesh;

/** \details
This structure represents Progressive Mesh vertex data.
\sa
<link gi_geometry_progressive_mesh_usage, Usage of the Progressive Meshes in the OdGi Library>
*/
struct ODGI_EXPORT OdGiProgressiveMeshVertexData
{
  /** \details
  Defines used vertex data fields
  */
  enum VertexDataFlags
  {
    /** Vertex data contains vertex normal information */
    kVertexDataNormals = 1,
    /** Vertex data contains vertex color information */
    kVertexDataColors = 1 << 1,
    /** Vertex data contains vertex UV information */
    kVertexDataUVs = 1 << 2
  };
  /** Vertex normals. */
  OdVector< OdGeVector3d > normals;
  /** Vertex colors. */
  OdVector< OdCmEntityColor > colors;
  /** Vertex UV coordinates. */
  OdVector< OdGePoint3d > UV;
};

/** \details
This structure represents Progressive Mesh face data.
\sa
<link gi_geometry_progressive_mesh_usage, Usage of the Progressive Meshes in the OdGi Library>
*/
struct ODGI_EXPORT OdGiProgressiveMeshFaceData
{
  /** \details
  Defines used face data fields
  */
  enum FaceDataFlags
  {
    /** Face data contains face material information */
    kFaceDataMaterials = 1,
    /** Face data contains face color information */
    kFaceDataColors = 1 << 1,
    /** Face data contains face normal information */
    kFaceDataNormals = 1 <<2
  };
  /** Face materials. */
  OdVector< OdDbStub* > materials;
  /** Face colors. */
  OdVector< OdCmEntityColor > colors;
  /** Face normals. */
  OdVector< OdGeVector3d > normals;
};

/** \details
This structure represents Progressive Mesh edge data.
\sa
<link gi_geometry_progressive_mesh_usage, Usage of the Progressive Meshes in the OdGi Library>
*/
struct ODGI_EXPORT OdGiProgressiveMeshEdgeData
{
  /** \details
  Defines used edge data fields
  */
  enum EdgeDataFlags
  {
    /** Edge data contains edge visibility information */
    kEdgeDataVisibilities = 1
  };
  /** Edge visibilities. */
  OdVector< OdUInt8 > visibilities;
};

/** \details
This class represents interface of ObjectId to Int converter
\sa
<link gi_geometry_progressive_mesh_usage, Usage of the Progressive Meshes in the OdGi Library>
*/
class ODGI_EXPORT OdGiProgressiveMeshObjectIdConverter
{
public:
  /** \details
  Converts OdDbStub to Int64
  */
  virtual OdInt64 dbStubToInt( OdDbStub* ) const = 0;
  /**
  Converts Int64 to OdDbStub
  */
  virtual OdDbStub* intToDbStub( OdInt64 ) const = 0;
};

/** \details
This class describes progressive mesh custom LOD select callback
*/
class ODGI_EXPORT OdGiProgressiveMeshAutoLODSelectCallback
{
public:
  /** \details
  Selects appropriate level of details
  \param pPM [in] Pointer to the progressive mesh
  \param pView [in] Pointer to the viewport
  \param pModelToWorldTransform [in] Pointer to the "model to world" transformation matrix, may be NULL
  \returns Returns appropriate level of details in interval [0, pPM->numLODs()]
  \remarks
  Any result out of interval [0, pPM->numLODs()] will be interpretated as error
  */
  virtual OdUInt32 selectLOD( const OdGiProgressiveMesh* pPM, const OdGiViewport* pView, const OdGeMatrix3d* pModelToWorldTransform ) const = 0;
};

/** \details
This class defines progressive mesh LOD auto select options
*/
class ODGI_EXPORT OdGiProgressiveMeshAutoLODSelectOptions
{
public:
  /** \details
  Constructs instance of OdGiProgressiveMeshAutoLODSelectOptions
  */
  OdGiProgressiveMeshAutoLODSelectOptions()
  {
    m_nMinPixels = 10;
    m_nMaxPixels = (OdUInt32)( 300.0 * sqrt( 3.0 ) );
  }
  /** \details
  Releases instance of OdGiProgressiveMeshAutoLODSelectOptions
  */
  ~OdGiProgressiveMeshAutoLODSelectOptions() {}
  /** \details
  Retrives extents diagonal size (in pixels) that treat as maximum for interpolation
  \returns Returns extents diagonal maximum size
  */
  OdUInt32 maxPixels() const { return m_nMaxPixels; }
  /** \details
  Specifies extents diagonal size (in pixels) that treat as maximum for interpolation
  \param nMax [in] Extents diagonal maximum size
  */
  void setMaxPixels( OdUInt32 nMax ) { m_nMaxPixels = nMax; }
  /** \details
  Retrives extents diagonal size (in pixels) that treat as minimum for interpolation
  \returns Returns extents diagonal minimum size
  */
  OdUInt32 minPixels() const { return m_nMinPixels; }
  /** \details
  Specifies extents diagonal size (in pixels) that treat as minimum for interpolation
  \param nMax [in] Extents diagonal minimum size
  */
  void setMinPixels( OdUInt32 nMin ) { m_nMinPixels = nMin; }
//DOM-IGNORE-BEGIN
protected:
  OdUInt32 m_nMinPixels;
  OdUInt32 m_nMaxPixels;
//DOM-IGNORE-END
};

/** \details
This class represents interface of Progressive Mesh
\sa
<link gi_geometry_progressive_mesh_usage, Usage of the Progressive Meshes in the OdGi Library>
*/
class ODGI_EXPORT OdGiProgressiveMesh : public OdRxObject
{
public:
  /** \details
  Defines Progressive Mesh stream version
  */
  enum ProgressiveMeshStreamVersion
  {
    kVersion1,
    kVersion2,
    kVersionActual = kVersion2
  };
  /** \details
  Enumerates level of details auto selection type
  */
  enum ProgressiveMeshAutoSelectLOD
  {
    kCustom = 0,            //Custom LOD select by callback
    kSqrInterpolation = 1,  //Square interpolation LOD select
    kSqrtInterpolation = 2  //Square root interpolation LOD select
  };
  ODRX_DECLARE_MEMBERS(OdGiProgressiveMesh);
  /** \details
  Obtains geometry representation of progressive mesh on current level of details
  \param pts [out]    OdVector for points
  \param faces [out]  OdVector for faces
  \returns
  Returns true if and only if geometry representation successfully obtained
  */
  virtual bool obtainShell( OdVector< OdGePoint3d >& pts, OdVector< OdInt32 >& faces ) const = 0;
  /** \details
  Obtains vertex data of progressive mesh on current level of details
  \param data [out]   OdGiProgressiveMeshVertexData instance to store results
  \param flags [in]   Set of OdGiProgressiveMeshVertexData::VertexDataFlags bits that specifies required vertex data fields
  \returns
  Returns set of OdGiProgressiveMeshVertexData::VertexDataFlags bits that specifies obtained vertex data fields
  */
  virtual OdUInt32 obtainVertexData( OdGiProgressiveMeshVertexData& data, OdUInt32 flags ) const = 0;
  /** \details
  Obtains face data of progressive mesh on current level of details
  \param data [out]   OdGiProgressiveMeshFaceData instance to store results
  \param flags [in]   Set of OdGiProgressiveMeshFaceData::FaceDataFlags bits that specifies required face data fields
  \returns
  Returns set of OdGiProgressiveMeshFaceData::FaceDataFlags bits that specifies obtained face data fields
  */
  virtual OdUInt32 obtainFaceData( OdGiProgressiveMeshFaceData& data, OdUInt32 flags ) const = 0;
  /** \details
  Obtains edge data of progressive mesh on current level of details
  \param data [out]   OdGiProgressiveMeshEdgeData instance to store results
  \param flags [in]   Set of OdGiProgressiveMeshEdgeData::EdgeDataFlags bits that specifies required edge data fields
  \returns
  Returns set of OdGiProgressiveMeshEdgeData::EdgeDataFlags bits that specifies obtained edge data fields
  */
  virtual OdUInt32 obtainEdgeData( OdGiProgressiveMeshEdgeData& data, OdUInt32 flags ) const = 0;
  /** \details
  Retunrs maximum level of details
  */
  virtual OdUInt32 numLODs() const = 0;
  /** \details
  Retunrs current level of details
  \remarks
  0 is a minimum level of details, numLODs() is a maximum level of details
  */
  virtual OdUInt32 currentLOD() const = 0;
  /** \details
  Switchs progressive mesh to the specified level of details
  */
  virtual bool setLOD( OdUInt32 ) = 0;
  /** \details
  Selects most appropriate level of details for specified view
  \param pView [in] Pointer to the view
  \param pModelToWorldTransform [in] Pointer to the "model to world" transformation matrix, may be NULL
  \returns Returns appropriate LOD value or OdUInt32(-1) if LOD selection fails
  \remarks This method do perform LOD switching, so return value may be used for monitoring purposes
  */
  virtual OdUInt32 selectLOD( ProgressiveMeshAutoSelectLOD lod, const OdGiViewport* pView, const OdGeMatrix3d* pModelToWorldTransform = NULL ) = 0;
  /** \details
  Retrives LOD auto select options
  \returns Returns LOD auto select options
  */
  virtual OdGiProgressiveMeshAutoLODSelectOptions& autoSelectLODOptions() = 0;
  /** \details
  Retrives LOD auto select options
  \returns Returns LOD auto select options
  */
  virtual const OdGiProgressiveMeshAutoLODSelectOptions& autoSelectLODOptions() const = 0;
  /** \details
  Specifies LOD auto select options
  \param options [in] LOD auto select options
  */
  virtual void setAutoSelectLODOptions( const OdGiProgressiveMeshAutoLODSelectOptions& options ) = 0;
  /** \details
  Specifies LOD auto select callback for ProgressiveMeshAutoSelectLOD::kCustom
  \param pCallback [in] LOD auto select callback
  */
  virtual void setCustomLODAutoSelectCallback( OdGiProgressiveMeshAutoLODSelectCallback* pCallback ) = 0;
  /** \details
  Retrives LOD auto select callback for ProgressiveMeshAutoSelectLOD::kCustom
  \returns Returns LOD auto select callback
  */
  virtual OdGiProgressiveMeshAutoLODSelectCallback* getCustomLODAutoSelectCallback() const = 0;
  /** \details
  Returns number of faces of progressive mesh on current level of details
  */
  virtual OdUInt32 numFaces() const = 0;
  /** \details
  Returns number of vertices of progressive mesh on current level of details
  */
  virtual OdUInt32 numVertices() const = 0;
  /** \details
  Returns extents of fully-detailed progressive mesh
  */
  virtual const OdGeExtents3d& extents() const = 0;
  /** \details
  Writes progressive mesh into the stream
  \remarks
  OdGiProgressiveMeshObjectIdConverter is required for writing DbStubs to the stream (materials).
  Converter may be NULL. In this case Data Flags with OdDbStub fileld will be reseted for writing. So, for example, face materials will be lost. 
  */
  virtual bool write( OdStreamBuf* pBuff, const OdGiProgressiveMeshObjectIdConverter* pConverter = NULL, ProgressiveMeshStreamVersion version = kVersionActual ) const = 0;
};

typedef OdSmartPtr<OdGiProgressiveMesh> OdGiProgressiveMeshPtr;

/** \details
This class presents mesh simplification options
\remarks
Some parameters are internal so they changing may be undesirable
Error (penalty) factors specifies how mush current error (penalty) affects on simplification step cost.
\sa
<link gi_geometry_progressive_mesh_usage, Usage of the Progressive Meshes in the OdGi Library>
*/
class ODGI_EXPORT OdGiProgressiveMeshGeneratorOptions
{
public:
  OdGiProgressiveMeshGeneratorOptions();
  ~OdGiProgressiveMeshGeneratorOptions();

  /** \details
  Returns minimum vertex count
  */
  OdUInt32 minVertices() const { return m_minVertices; }
  /** \details
  Specifies minimum vertex count, mey be 0
  \remarks 
  Mesh simplification stops if minimum vertex count is reached
  */
  void setMinVertices( OdUInt32 n ) { m_minVertices = n; }
  /** \details
  Returns minimum face count
  */
  OdUInt32 minFaces() const { return m_minFaces; }
  /** \details
  Specifies minimum face count, mey be 0
  \remarks 
  Mesh simplification stops if minimum face count is reached
  */
  void setMinFaces( OdUInt32 n ) { m_minFaces = n; }
  /** \details
  Returns worst diherial value
  */
  double worstDiherial() const { return m_worstDiherial; }
  /** \details
  Specifies worst diherial value
  \remarks
  Internal parameter.
  Diherial is used to check dihedral admissible mesh configuration.
  */
  void setWorstDiherial( double d ) { m_worstDiherial = d; }
  /** \details
  Returns smallest simplification step cost
  */
  double smallestCost() const { return m_minCost; }
  /** \details
  Specifies smallest simplification step cost
  \remarks
  Internal parameter.
  Smallest cost is used for prevent zero-comparison problems
  */
  void setSmallestCost( double d ) { m_minCost = d; }
  /** \details
  Returns worst simplification step cost
  */
  double worstCost() const { return m_worstCost; }
  /** \details
  Specifies worst simplification step cost
  \remarks
  Internal parameter.
  Mesh optimisation stops if all possible simplification steps cost more than worst cost
  */
  void setWorstCost( double d ) { m_worstCost = d; }
  /** \details
  Returns infinity value
  */
  double infinity() const { return m_infinity; }
  /** \details
  Specifies infinity value
  \remarks
  Internal parameter
  Infinity is used for prevent huge numbers comparison problems
  */
  void setInfinity( double d ) { m_infinity = d; }
  /** \details
  Returns true if mesh simplification should respect normal difference while estimating simplification step cost
  */
  bool getFitNormals() const { return GETBIT( m_flags, kFlag_FitNormals ); }
  /** \details
  Returns normal error factor
  */
  double normalErrorFactor() const { return m_normalErrorFactor; }
  /** \details
  Specifies should simplification respect normal difference or not
  \param bFit         [in]  True if simplification should respect normal difference
  \param errorFactor  [in]  Normal error factor
  \remarks
  Normal error factor is internal parameter.
  */
  void setFitNormals( bool bFit, double errorFactor )
  {
    SETBIT( m_flags, kFlag_FitNormals, bFit );
    m_normalErrorFactor = errorFactor;
  }
  /** \details
  Returns true if mesh simplification should respect color difference while estimating simplification step cost
  */
  bool getFitColors() const { return GETBIT( m_flags, kFlag_FitColors ); }
  /** \details
  Returns color error factor
  */
  double colorErrorFactor() const { return m_colorErrorFactor; }
  /** \details
  Specifies should simplification respect color difference or not
  \param bFit         [in]  True if simplification should respect color difference
  \param errorFactor  [in]  Color error factor
  \remarks
  Color error factor is internal parameter.
  */
  void setFitColors( bool bFit, double errorFactor )
  {
    SETBIT( m_flags, kFlag_FitColors, bFit );
    m_colorErrorFactor = errorFactor;
  }
  /** \details
  Returns scale number of sharp edge samples
  */
  double sharpEdgesScaleFactor() const { return m_sharpEdgesScaleFactor; }
  /** \details
  Specifies scale number of sharp edge samples
  \remarks
  Internal parameter.
  */
  void setSharpEdgesScaleFactor( double d ) { m_sharpEdgesScaleFactor = d; }

  /** \details
  Returns next cost threshold factor
  */
  double nextCostThresholdFactor() const { return m_nextCostThresholdFactor; }
  /** \details
  Specifies next cost threshold factor
  \remarks
  Internal parameter.
  thresCost = currentCost + (nextCost - currentCost)*nextCostThresholdFactor() + smallestCost()
  If thresCost is more than threshold the cost of current simplification step will be re-estimated
  */
  void setNextCostThresholdFactor( double d ) { m_nextCostThresholdFactor = d; }

  /** \details
  Returns true if mesh simplification should respect height of descendant tree while estimating simplification step cost
  */
  bool getDiffMaxPenalty() const { return GETBIT( m_flags, kFlag_DesH ); }
  /** \details
  Returns height of descendant tree penalty factor
  */
  double diffMaxPenaltyFactor() const { return m_desdFactor; }
  /** \details
  Specifies should simplification respect height of descendant tree or not
  \param bSet         [in]  True if simplification should respect height of descendant tree
  \param factor       [in]  Height of descendant tree penalty factor
  \remarks
  Height of descendant tree penalty factor is internal parameter.
  Height of descendant tree: For collapse v1-v2 -> v: DesH(v) = max( DesH( v1 ), DesH( v2 ) ) + 1
  */
  void setDiffMaxPenalty( bool bSet, double factor )
  {
    SETBIT( m_flags, kFlag_DesH, bSet );
    m_desdFactor = factor;
  }
  /** \details
  Returns true if mesh simplification should respect number of descendants while estimating simplification step cost
  */
  bool getSumPenalty() const { return GETBIT( m_flags, kFlag_DesN ); }
  /** \details
  Returns number of descendants penalty factor
  */
  double sumPenaltyFactor() const { return m_desnFactor; }
  /** \details
  Specifies should simplification respect number of descendants or not
  \param bSet         [in]  True if simplification should respect number of descendants
  \param factor       [in]  Height of descendant tree penalty factor
  \remarks
  Number of descendants penalty factor is internal parameter.
  Number of descendants: For collapse v1-v2 -> v: DesN (v) = DesN( v1 ) + DesN( v2 )
  */
  void setSumPenalty( bool bSet, double factor )
  {
    SETBIT( m_flags, kFlag_DesN, bSet );
    m_desnFactor = factor;
  }
  /** \details
  Returns possible curve topology type changes
  */
  OdUInt8 strictSharp() const { return m_strictSharp; }
  //Unsupported for now
  /*void setStrictSharp( OdUInt8 val ) 
  { 
    ODA_ASSERT( val < 3 );
    m_strictSharp = val;
  }*/
  /** \details
  Returns number of face sample points
  */
  OdUInt32 numFaceSamplePoints() const { return m_numFaceSamplePoints; }
  /** \details
  Specifies number of face sample points
  \remarks
  Internal parameter.
  */
  void setNumFaceSamplePoints( OdUInt32 n ) { m_numFaceSamplePoints = n; }

//DOM-IGNORE-BEGIN
private:
  enum OptionFlags
  {
    kFlag_FitNormals = 1,
    kFlag_FitColors = 1 << 1,
    kFlag_DesH = 1 << 2,
    kFlag_DesN = 1 << 3
  };
  OdUInt8 m_flags;
  OdUInt32 m_minVertices;
  OdUInt32 m_minFaces;
  double m_worstDiherial;
  double m_worstCost;
  double m_minCost;
  double m_infinity;
  double m_normalErrorFactor;     //normal error (0..2) to dist/diam rms, need to constant in front of normal error term
  double m_colorErrorFactor;      //color rms (0..sqrt(3)) to dist/diam rms, need to constant in front of color error term
  double m_sharpEdgesScaleFactor; //scale number of sharp edge samples
  double m_nextCostThresholdFactor;
  double m_desdFactor;            //penalize diff max #desc to balance tree
  double m_desnFactor;            //penalize sum #desc to balance tree
  OdUInt8 m_strictSharp;          //disallow disc. curve topo type changes {0, 1, 2}
  OdUInt32 m_numFaceSamplePoints; //number of face sample points, -1 means == face num
//DOM-IGNORE-END
};

/** \details
This class represents interface of Progressive Mesh generator
\remarks
Generation from geometry data (shell): 
  1) create instance
  2) (optional) fill attributes: materials, face data, vertex data, etc
  3) construct full-presented mesh by calling setupMesh()
  4) (optional) check status
  4) (optional) setup generation options
  5) generate progressive mesh by calling buildProgressiveMesh()
Generation from binary data (load):
  1) create instance
  2) create progressive mesh by calling createProgressiveMeshFrom()
  \sa
  <link gi_geometry_progressive_mesh_usage, Usage of the Progressive Meshes in the OdGi Library>
*/
class ODGI_EXPORT OdGiProgressiveMeshGenerator
{
public:
  /** \details
  Status of generated Progressive mesh
  */
  enum Status
  {
    kStatus_Ok = 0,                               //Progressive mesh is OK
    kStatus_DegenerateFacesIgnored = 1,           //Some faces from input are skipped since they are degenerative
    kStatus_NonManifoldFacesIgnored = 1 << 1,     //Some faces from input are skipped since they cause non-manifold
    kStatus_InvalidFacesIgnored = 1 << 2,         //Some faces from input are skipped since they are invalid (not 3 or 4 points, contains points out of input points range, etc)
    kStatus_ZeroNormalFacesIgnored = 1 << 3,      //Some faces from input are skipped since they have zero normal
    kStatus_NonTriangleFaceTriangulated = 1 << 4  //Some faces from input were not triangle and were triangulated
  };
  /** \details
  Creates instance of OdGiProgressiveMeshGenerator
  \remarks
  Created instance have to be manually deleted
  */
  static OdGiProgressiveMeshGenerator* createObject();
  /** \details
  Constructs full-presented mesh for progressive mesh generation
  \param nPoints      [in] A number of input points
  \param points       [in] A pointer to the input points array
  \param faceListSize [in] A size of face list
  \param faces        [in] A pointer to the faces list
  \returns
  Returns status of the operation: OdGiProgressiveMeshGenerator::kStatus_Ok if everything ok or set of OdGiProgressiveMeshGenerator::Status bits
  \remarks
  Faces list is a list of OdInt32 with the following sequence for each face: Number of points in face, Index of first point, Index of second point, etc
  After mesh is setuped it may have sence to check generator status that contaions information about mismatchs between input data and created mesh
  */
  virtual OdUInt8 setupInitialShell( OdInt32 nPoints, const OdGePoint3d * points, OdInt32 faceListSize, const OdInt32 * faces ) = 0;
  /** \details
  Generates progressive mesh from full-presented mesh
  \param pVertexData     [in] A pointer to the vertex data
  \param pFaceData       [in] A pointer to the face data
  \param pEdgeData       [in] A pointer to the edge data
  \remarks
  Progressive mesh generation stops when one of conditions are reached:
  1) Face count reaches minFaces
  2) Vertices reaches minVertices
  3) There are no more valid simplifications operations available
  */
  virtual OdGiProgressiveMeshPtr buildProgressiveMesh( const OdGiVertexData* pVertexData = NULL, const OdGiFaceData* pFaceData = NULL, const OdGiEdgeData* pEdgeData = NULL ) = 0;
  /** \details
  Creates progressive mesh from stream buffer
  \remarks
  OdGiProgressiveMeshObjectIdConverter is required for reading DbStubs from the stream (materials).
  Converter may be NULL. In this case Data Flags with OdDbStub fileld will be reseted. So, for example, face materials will be lost. 
  */
  virtual OdGiProgressiveMeshPtr createProgressiveMeshFrom( OdStreamBuf* pBuff, const OdGiProgressiveMeshObjectIdConverter* pConverter = NULL, 
    OdGiProgressiveMesh::ProgressiveMeshStreamVersion version = OdGiProgressiveMesh::kVersionActual ) const = 0;
  /** \details
  Returns mesh simplification options
  */
  virtual const OdGiProgressiveMeshGeneratorOptions& options() const = 0;
  /** \details
  Returns mesh simplification options
  */
  virtual OdGiProgressiveMeshGeneratorOptions& options() = 0;
  /** \details
  Specifies mesh simplification options
  */
  virtual void setOptions( const OdGiProgressiveMeshGeneratorOptions& ) = 0;
};

/** \details
This class represents interface of Progressive Mesh Extansion.
This object generates OdGiProgressiveMesh or keeps original data if generation fails.
*/
class ODGI_EXPORT OdGiProgressiveMeshEx : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiProgressiveMeshEx);
  /** \details
  Obtains geometry representation of progressive mesh on current level of details
  \param pts [out]    OdVector for points
  \param faces [out]  OdVector for faces
  \returns
  Returns true if and only if geometry representation successfully obtained
  \remarks
  Returns geometry representation from input data if progressive mesh generation fails
  */
  virtual bool obtainShell( OdVector< OdGePoint3d >& pts, OdVector< OdInt32 >& faces ) const = 0;
  /** \details
  Obtains vertex data of progressive mesh on current level of details
  \param data [out]   OdGiProgressiveMeshVertexData instance to store results
  \param flags [in]   Set of OdGiProgressiveMeshVertexData::VertexDataFlags bits that specifies required vertex data fields
  \returns
  Returns set of OdGiProgressiveMeshVertexData::VertexDataFlags bits that specifies obtained vertex data fields
  \remarks
  Returns vertex data from input data if progressive mesh generation fails
  */
  virtual OdUInt32 obtainVertexData( OdGiProgressiveMeshVertexData& data, OdUInt32 flags ) const = 0;
  /** \details
  Obtains face data of progressive mesh on current level of details
  \param data [out]   OdGiProgressiveMeshFaceData instance to store results
  \param flags [in]   Set of OdGiProgressiveMeshFaceData::FaceDataFlags bits that specifies required face data fields
  \returns
  Returns set of OdGiProgressiveMeshFaceData::FaceDataFlags bits that specifies obtained face data fields
  \remarks
  Returns face data from input data if progressive mesh generation fails
  */
  virtual OdUInt32 obtainFaceData( OdGiProgressiveMeshFaceData& data, OdUInt32 flags ) const = 0;
  /** \details
  Obtains edge data of progressive mesh on current level of details
  \param data [out]   OdGiProgressiveMeshEdgeData instance to store results
  \param flags [in]   Set of OdGiProgressiveMeshEdgeData::EdgeDataFlags bits that specifies required edge data fields
  \returns
  Returns set of OdGiProgressiveMeshEdgeData::EdgeDataFlags bits that specifies obtained edge data fields
  \remarks
  Returns edge data from input data if progressive mesh generation fails
  */
  virtual OdUInt32 obtainEdgeData( OdGiProgressiveMeshEdgeData& data, OdUInt32 flags ) const = 0;
  /** \details
  Retrieves maximum level of details.
  \returns
   maximum level of details for the generated progressive mesh or 1 if progressive mesh generation fails.
  */
  virtual OdUInt32 numLODs() const = 0;
  /** \details
  Retrieves current level of details.
  
  \returns
  current level of details as OdUInt32 value.
  \remarks
  0 is a minimum level of details, numLODs() is a maximum level of details
  */
  virtual OdUInt32 currentLOD() const = 0;
  /** \details
  Switches progressive mesh to the specified level of details.
  \returns
  false if number of level of details equals to 0, true otherwise.
  \remarks
  if the specified level of details is greater that the maximum level of details, then an exception is thrown and level of details is set to the maximum allowed level of details.
  */
  virtual bool setLOD( OdUInt32 ) = 0;
  /** \details
  Retrieves number of faces of progressive mesh on current level of details.
  \returns
  number of faces for progressive mesh on current level of details as OdUInt32 value.
  */
  virtual OdUInt32 numFaces() const = 0;
  /** \details
  Retrieves number of vertices of progressive mesh on current level of details.
  \returns
  number of vertices for progressive mesh on current level of details as OdUInt32 value.
  */
  virtual OdUInt32 numVertices() const = 0;
  /** \details
  Retrieves extents of fully-detailed progressive mesh
  \returns
  extents represented by a reference to OdGeExtents3d instance.
  */
  virtual const OdGeExtents3d& extents() const = 0;
  /** \details
  Writes progressive mesh into the specified stream buffer.
  \param pBuff      [in] Pointer to a stream buffer to write the mesh to.
  \param pConverter      [in] Pointer to the object that converts ObjectId to Int.
  \param version      [in] Progressive stream buffer version.
  \returns
  true if operation succeeded, false otherwise.
  \remarks
  OdGiProgressiveMeshObjectIdConverter is required for writing DbStubs to the stream (materials).
  Converter may be NULL. In this case Data Flags with OdDbStub fileld will be reseted for writing. So, for example, face materials will be lost. 
  \remarks
  For the version parameter you can currently choose one of two available versions:
  kVersion1 supports:
  <table>
    <B>Face</B>                     <B>Vertices</B>                                  <B>Edges</B>
    Colors, normals                 Colors, normals                                  -----
  </table>
  
  kVersion2 supports:
  <table>
    <B>Face</B>                     <B>Vertices</B>                                  <B>Edges</B>
    Colors, normals, materials      Colors, normals, UV (texture coordinates)        Visibility
  </table>
  
  Current version (kVersionActual) is kVersion2.
  */
  virtual bool write( OdStreamBuf* pBuff, const OdGiProgressiveMeshObjectIdConverter* pConverter = NULL, OdGiProgressiveMesh::ProgressiveMeshStreamVersion version = OdGiProgressiveMesh::kVersionActual ) const = 0;
  /** \details
  Checks whether progressive mesh is generated.
  \returns
  true if and only if Progressive Mesh was generated, false otherwise
  */
  virtual bool isProgressiveMeshGenerated() const = 0;
  /** \details
  Prepares initial data for progressive mesh generation
  \param nPoints      [in] A number of input points
  \param points       [in] A pointer to the input points array
  \param faceListSize [in] A size of face list
  \param faces        [in] A pointer to the faces list
  \remarks
  Faces list is a list of OdInt32 with the following sequence for each face: Number of points in face, Index of first point, Index of second point, etc
  */
  virtual void setupInitialShell( OdInt32 nPoints, const OdGePoint3d * points, OdInt32 faceListSize, const OdInt32 * faces ) = 0;
  /** \details
  Generates progressive mesh. If fails, keeps shell representation.
  \param pVertexData     [in] A pointer to the vertex data
  \param pFaceData       [in] A pointer to the face data
  \param pEdgeData       [in] A pointer to the edge data
  \returns
  true if progressive mesh is built successfully, false otherwise.
  \remarks
  Progressive mesh generation stops when one of conditions are reached:
  1) Face count reaches minFaces
  2) Vertices reaches minVertices
  3) There are no more valid simplifications operations available
  Also, progressive mesh will not be generated if initial data is a kind of generationAbortFlags()
  */
  virtual bool buildProgressiveMesh( const OdGiVertexData* pVertexData = NULL, const OdGiFaceData* pFaceData = NULL, const OdGiEdgeData* pEdgeData = NULL ) = 0;
  /** \details
  Creates progressive mesh extension from stream buffer.
  \param pBuff      [in] Pointer to a stream buffer to read the mesh from.
  \param pConverter      [in] Pointer to the object that converts ObjectId to Int.
  \param version      [in] Progressive stream buffer version.
  \returns
  true if progressive mesh is successfully read from buffer, false otherwise.
  \remarks
  OdGiProgressiveMeshObjectIdConverter is required for reading DbStubs from the stream (materials).
  Converter may be NULL. In this case Data Flags with OdDbStub fileld will be reseted. So, for example, face materials will be lost. 
  \remarks
  For the version parameter you can currently choose one of two available versions:
  kVersion1 supports:
  <table>
    <B>Face</B>                     <B>Vertices</B>                                  <B>Edges</B>
    Colors, normals                 Colors, normals                                  -----
  </table>
  
  kVersion2 supports:
  <table>
    <B>Face</B>                     <B>Vertices</B>                                  <B>Edges</B>
    Colors, normals, materials      Colors, normals, UV (texture coordinates)        Visibility
  </table>
  
  Current version (kVersionActual) is kVersion2.
  */
  virtual bool readProgressiveMeshExFrom( OdStreamBuf* pBuff, const OdGiProgressiveMeshObjectIdConverter* pConverter = NULL, 
    OdGiProgressiveMesh::ProgressiveMeshStreamVersion version = OdGiProgressiveMesh::kVersionActual ) = 0;
  /** \details
  Retrieves mesh simplification options.
  \returns
  reference to OdGiProgressiveMeshGeneratorOptions that represents mesh simplification options.
  */
  virtual const OdGiProgressiveMeshGeneratorOptions& options() const = 0;
  /** \details
  Retrieves mesh simplification options.
  \returns
  reference to OdGiProgressiveMeshGeneratorOptions that represents mesh simplification options.
  */
  virtual OdGiProgressiveMeshGeneratorOptions& options() = 0;
  /** \details
  Specifies mesh simplification options.
  */
  virtual void setOptions( const OdGiProgressiveMeshGeneratorOptions& ) = 0;
  /** \details
  Specifies generation abort flags
  \param flags [in] A set of OdGiProgressiveMeshGenerator::Status flags.
  \remarks
  If progressive mesh generator returns one of the flags in status, mesh generation will be aborted.
  */
  virtual void setGenerationAbortFlags( OdUInt8 flags ) = 0;
  /** \details
  Retrieves generation abort flags.
  \returns
  a set of generation abort flags as OdUInt8 value.
  */
  virtual OdUInt8 generationAbortFlags() const = 0;
  /** \details
  Specifies internal progressive mesh.
  \param flags [in] A set of OdGiProgressiveMeshGenerator::Status flags
  */
  virtual void setProgressiveMesh( OdGiProgressiveMeshPtr pMesh ) = 0;
  /** \details
  Retrieves internal progressive mesh.
  \returns
  a smart pointer to an underlaying progressive mesh.
  */
  virtual OdGiProgressiveMeshPtr progressiveMesh() const = 0;
  /** \details
  Checks whether this progressive mesh extension has data.
  \returns
  true if progressive mesh was generated or data for shell was transfered (buildProgressiveMesh() call) 
  */
  virtual bool hasData() const = 0;
};

/** \details
A data type that represents a smart pointer to an <link OdGiProgressiveMeshEx, OdGiProgressiveMeshEx> object.
*/
typedef OdSmartPtr<OdGiProgressiveMeshEx> OdGiProgressiveMeshExPtr;

#endif


