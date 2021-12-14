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

// Mapper item.

#ifndef __ODGIMAPPERITEM_H__
#define __ODGIMAPPERITEM_H__

#include "GiExport.h"
#include "GiMaterial.h"
#include "GiMaterialTraitsData.h"
#include "GiGeometry.h"

#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeExtents3d.h"

#include "RxObject.h"

#include "TD_PackPush.h"

/** \details
    Material mapper item entry for separate texture channel.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMapperItemEntry : public OdRxObject
{
  public:
    /** \details
      Input mapper triangle type.
    */
    struct MapInputTriangle {
      OdGePoint3d inPt[3]; // Mapping xyz
    };
    /** \details
      Output mapper vertices texture coordinates.
    */
    struct MapOutputCoords {
      OdGePoint2d outCoord[3]; // Mapped uv
    };
  public:
    ODRX_DECLARE_MEMBERS(OdGiMapperItemEntry);

    /** \details
      Retrieves a mapper (return result mapper combination for pMapper and pInheritMapper).
      
      \returns
      mapper as OdGiMapper instance.
      
      \sa
      <link setMapper, setMapper>
    */
    virtual const OdGiMapper &mapper() const = 0;
    /** \details
      Sets mapper combination.
      \param pMapper [in]  Entity mapper.
      \param pInheritMapper [in]  Material (default) mapper.
    */
    virtual void setMapper(const OdGiMapper &pMapper, const OdGiMapper &pInheritMapper) = 0;
    /** \details
      Sets mapper combination and object/model matrix.
      \param pMapper [in]  Entity mapper.
      \param pInheritMapper [in]  Material (default) mapper.
      \param tm [in]  Model or object matrix.
    */
    virtual void setMapper(const OdGiMapper &pMapper, const OdGiMapper &pInheritMapper, const OdGeMatrix3d &tm) = 0;
    /** \details
      Sets mapper for material (entity mapper not specified).
      \param pMaterialMapper [in]  Material mapper.
    */
    virtual void setMapper(const OdGiMapper &pMaterialMapper) = 0;
    /** \details
      Sets mapper for material and object/model matrix.
      \param pMaterialMapper [in]  Material mapper.
      \param tm [in]  Model or object matrix.
    */
    virtual void setMapper(const OdGiMapper &pMaterialMapper, const OdGeMatrix3d &tm) = 0;

    /** \details
      Retrieves input vertices transformation matrix.
      
      \returns
      input vertices inside the transformation matrix represented by the OdGeMatrix3d instance.
    */
    virtual const OdGeMatrix3d &inputTransform() const = 0;
    /** \details
      Sets input vertices transformation matrix.
      \param tm [in]  Input vertices inside the transform matrix.
    */
    virtual void setInputTransform(const OdGeMatrix3d &tm) = 0;

    /** \details
      Retrieves input vertices device transformation matrix.

      \returns
      inside the transformation matrix represented by the OdGeMatrix3d instance.
      
      \remarks
      This is a device-dependent optional transformation matrix.
    */
    virtual const OdGeMatrix3d &deviceTransform() const = 0;
    /** \details
      Sets input vertices device transformation matrix.
      
      \param dtm [in]  Input device vertices inside the transform matrix.

      \remarks
      This is a device-dependent optional transformation matrix.
    */
    virtual void setDeviceTransform(const OdGeMatrix3d &dtm) = 0;

    /** \details
      Retrieves model transformation matrix (used only for kModel mappers).
      \returns
      model transformation matrix as OdGeMatrix3d instance.
    */
    virtual const OdGeMatrix3d &modelTransform() const = 0;
    /** \details
      Sets model transformation matrix (used only for kModel mappers).
      
      \param mtm [in]  Model transform vertices inside the transform matrix.
    */
    virtual void setModelTransform(const OdGeMatrix3d &mtm) = 0;

    /** \details
      Retrieves object transformation matrix (used only for kObject mappers).
      
      \returns
      object transformation matrix as OdGeMatrix3d instance.
    */
    virtual const OdGeMatrix3d &objectTransform() const = 0;
    
    /** \details
      Sets object transformation matrix (used only for kObject mappers).
      
      \param otm [in]  Object transform vertices inside the transform matrix.
    */
    virtual void setObjectTransform(const OdGeMatrix3d &otm) = 0;

    /** \details
      Retrieves output texture coordinates transformation matrix.
      
      \returns
      output texture coordinates transformation matrix as OdGeMatrix3d instance.
    */
    virtual const OdGeMatrix3d &outputTransform() const = 0;
    
    /** \details
      Sets output texture coordinates transformation matrix.
      
      \param tm [in]  Output transform vertices inside the transform matrix.
    */
    virtual void setOutputTransform(const OdGeMatrix3d &tm) = 0;

    /** \details
      Recomputes transformation matrices.
      \remarks
      Must be called after mapper or model transforms set.
      \sa
      setMapper, setModelTransform
    */
    virtual void recomputeTransformations() = 0;

    /** \details
      Calculates texture coordinates for input triangle.
      \param trg [in]  Input triangle coordinates (X, Y, Z).
      \param uv [out]  Output texture coordinates (U, V).
    */
    virtual void mapCoords(const MapInputTriangle &trg, MapOutputCoords &uv) const = 0;
    /** \details
      Calculate texture coordinates for input triangle.
      \param trg [in]  Input triangle coordinates (X, Y, Z).
      \param uv [out]  Output texture coordinates (U, V).
    */
    virtual void mapCoords(const OdGePoint3d *trg, OdGePoint2d *uv) const = 0;
    /** \details
      Calculates texture coordinates for input triangle.
      \param trg [in]  Input triangle coordinates (X, Y, Z).
      \param uv [out]  Output texture coordinates (U, V).
    */
    virtual void mapCoords(const double *trg, double *uv) const = 0;
    /** \details
      Calculates texture coordinates for input triangle.
      \param trg [in]  Input triangle coordinates (X, Y, Z).
      \param normal [in]  Input triangle normal (X, Y, Z).
      \param uv [out]  Output texture coordinates (U, V).
    */
    virtual void mapCoords(const MapInputTriangle &trg, const OdGeVector3d &normal, MapOutputCoords &uv) const = 0;
    /** \details
      Calculates texture coordinates for input triangle.
      \param trg [in]  Input triangle coordinates (X, Y, Z).
      \param normal [in]  Input triangle normal (X, Y, Z).
      \param uv [out]  Output texture coordinates (U, V).
    */
    virtual void mapCoords(const OdGePoint3d *trg, const OdGeVector3d &normal, OdGePoint2d *uv) const = 0;
    /** \details
      Calculates texture coordinates for input triangle.
      \param trg [in]  Input triangle coordinates (X, Y, Z).
      \param normal [in]  Input triangle normal (X, Y, Z).
      \param uv [out]  Output texture coordinates (U, V).
    */
    virtual void mapCoords(const double *trg, const double *normal, double *uv) const = 0;

    // Shell mapping extensions for 3.4

    enum MappingIteratorType
    {
      /** Shell-style iterator.*/
      kShellMapIt = 0,
      /** Triangles list { { 0, 1, 2 }, { 1, 2, 3 } }.*/
      kTriListMapIt,
      /** Simple list of indices (probably shell/mesh face, polygon or unstructured data)
      kTriListMapIt makes subdividing of vertices without indices on triangles. Set kShellMapIt or
      kPolyIt if input data doesn't stored as triangles list.*/
      kPolyIt
    };

    /** \details
      Calculates texture coordinates for input shellmeshpolygon vertices.
      \param nVerts [in]  Count of input vertices.
      \param pVerts [in]  Array of input vertices (should be equal to vertices count).
      \param pUV [out]  Output array of texture coordinates (should be equal to vertices count).
      \param nList [in]  Count of indices in indices array (include face sizes for shell-style arrays).
      \param pIds [in]  Vertex indices array.
      \param mapIt [in]  Type of indices iterator.
      \param pNormal [in]  Optional normal (if already precalculated and doesn't specified in face/vertex data).
      \param pFaceNormals [in]  Optional face normals array (have greater priority than optional normal).
      \param pVertNormals [in]  Optional vertex normals array (have greater priority than optional face normals).
      \param oType [in]  Used for calculation of normal if doesn't specified.
    */
    virtual void mapCoords(OdUInt32 nVerts, const OdGePoint3d *pVerts, OdGePoint2d *pUV,
                           OdUInt32 nList = 0, const OdInt32 *pIds = NULL, MappingIteratorType mapIt = kTriListMapIt,
                           const OdGeVector3d *pNormal = NULL, const OdGeVector3d *pFaceNormals = NULL,
                           const OdGeVector3d *pVertNormals = NULL, OdGiOrientationType oType = kOdGiNoOrientation) = 0;

    // Predefined coordinates extensions

    /** \details
      Recalculates predefined texture coordinates using current material transformation.
      \param pInUVW [in]  Input array of texture coordinates.
      \param pOutUV [out]  Output array of transformed texture coordinates.
      \param nPoints [in]  Count of input and output texture coordinates.
    */
    virtual void mapPredefinedCoords(const OdGePoint3d *pInUVW, OdGePoint2d *pOutUV, OdUInt32 nPoints) const = 0;

    //

    /** \details
      Checks whether entity mapper is enabled.
      \returns
      true if entity mapper is specified, false otherwise.
    */
    virtual bool isEntityMapper() const = 0;
    /** \details
      Return true if mapper needs object matrix.
    */
    virtual bool isObjectMatrixNeed() const = 0;
    /** \details
      Checks whether a model matrix is needed for this mapper.
      \returns
      true if mapper needs a model matrix, false otherwise.
    */
    virtual bool isModelMatrixNeed() const = 0;
    /** \details
      Checks whether a model matrix is needed for this mapper.
      \returns
      true if mapper requires object or vertex transform.
    */
    virtual bool isDependsFromObjectMatrix() const = 0;
    /** \details
      Checks whether mapper requires a vertex transform. A vertex transform is required in case if mapper has automatic transform mode == kObject.
      \returns
      true if mapper requires vertex transform, false otherwise.
    */
    virtual bool isVertexTransformRequired() const = 0;

    /** \details
      Sets vertex transform from points array.
      \param nCount [in]  Number of points.
      \param pPoints [in]  Pointer to the array of points.
    */
    virtual void setVertexTransform(OdInt32 nCount, const OdGePoint3d *pPoints) = 0;
    /** \details
      Sets vertex transform from object extents.
      \param exts [in]  Extents that defines vertex transform matrix.
    */
    virtual void setVertexTransform(const OdGeExtents3d &exts) = 0;
    /** \details
      Sets vertex transform from points array.
      \param nCount [in]  Number of points.
      \param pPoints [in]  Pointer to the array of points.
    */
    virtual void setObjectTransform(OdInt32 nCount, const OdGePoint3d *pPoints) = 0;
    /** \details
      Sets object transform from extents.
      \param exts [in]  Extents that defines object transform matrix.
    */
    virtual void setObjectTransform(const OdGeExtents3d &exts) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMapperItemEntry object pointers.
	
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMapperItemEntry> OdGiMapperItemEntryPtr;

/** \details
    Container item for material data storage.
    OdGiMapperItem used for real-time devices, such as DirectX or OpenGL.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMapperItem : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMapperItem);

    /** \details
      Sets mapper for diffuse texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setDiffuseMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets mapper for diffuse texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setDiffuseMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Sets mapper for diffuse texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setDiffuseMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets mapper for diffuse texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setDiffuseMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Retrieves a smart pointer to a diffuse mapper object.
      \returns
      mapper for diffuse texture channel.
    */
    virtual OdGiMapperItemEntryPtr diffuseMapper() const = 0;
    /** \details
      Retrieves a smart pointer to a diffuse mapper object.
      \returns
      mapper for diffuse texture channel.
    */
    virtual OdGiMapperItemEntryPtr diffuseMapper() = 0;

    /** \details
      Sets mapper for all texture channels.
      \param traitsData [in]  Input traits data.
      \param pMaterial [in]  Material ID for input traits data.
    */
    virtual void setMapper(const OdGiMaterialTraitsData &traitsData, const OdDbStub *pMaterial = NULL) = 0;
    /** \details
      Sets mapper for all texture channels.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
      \param pMaterial [in]  Material ID for input traits data.
    */
    virtual void setMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm, const OdDbStub *pMaterial = NULL) = 0;
    /** \details
      Sets mapper for all texture channels.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param pMaterial [in]  Material ID for input traits data.
    */
    virtual void setMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdDbStub *pMaterial = NULL) = 0;
    /** \details
      Sets mapper for all texture channels.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
      \param pMaterial [in]  Material ID for input traits data.
    */
    virtual void setMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm, const OdDbStub *pMaterial = NULL) = 0;

    /** \details
      Sets model transformation for all texture channels which needs model transformation.
      \param mtm [in]  Model transformation matrix.
      \param recomputeTransforms [in]  Flag that specifies whether to call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setModelTransform(const OdGeMatrix3d &mtm, bool recomputeTransforms = true) = 0;

    /** \details
      Sets object transformation for all texture channels which needs object transformation.
      \param otm [in]  Object transformation matrix.
      \param recomputeTransforms [in]  Flag that specifies whether to call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setObjectTransform(const OdGeMatrix3d &otm, bool recomputeTransforms = true) = 0;
    /** \details
      Sets object transformation for all texture channels which needs object transformation.
      \param nCount [in]  Number of points into pPoints array.
      \param pPoints [in]  Pointer to an array of points.
      \param recomputeTransforms [in]  Flag that specifies whether to call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setObjectTransform(OdInt32 nCount, const OdGePoint3d *pPoints, bool recomputeTransforms = true) = 0;
    /** \details
      Sets object transformation for all texture channels which needs object transformation.
      \param exts [in]  Object extents.
      \param recomputeTransforms [in]  Flag that specifies whether to call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setObjectTransform(const OdGeExtents3d &exts, bool recomputeTransforms = true) = 0;

    /** \details
      Sets device-dependant transformation for all texture channels.
      \param dtm [in]  Device transformation matrix.
      \param recomputeTransforms [in]  Flag that specifies whether to call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setDeviceTransform(const OdGeMatrix3d &dtm, bool recomputeTransforms = true) = 0;

    /** \details
      Checks whether the current material is valid for use with previously set up mapper data.
      \param pMaterial [in]  Material ID.
      \returns
      true if the current material is valid for use with previously set up mapper data, false otherwise.
    */
    virtual bool isLastProcValid(const OdDbStub *pMaterial) = 0;
    /** \details
      Checks whether the current material and matrix are valid for use with previously set up mapper data.
      \param pMaterial [in]  Material ID.
      \param tm [in]  Object/model transformation matrix.
      \returns
      true if the current material and matrix are valid for use with previously set up mapper data, false otherwise.
    */
    virtual bool isLastProcValid(const OdDbStub *pMaterial, const OdGeMatrix3d &tm) = 0;
    /** \details
      Checks whether the current mapper and material are valid for use with previously set up mapper data.
      \param pMapper [in]  Entity mapper.
      \param pMaterial [in]  Material ID.
      \returns
      true if the current material and mapper are valid for use with previously set up mapper data, false otherwise.
    */
    virtual bool isLastProcValid(const OdGiMapper *pMapper, const OdDbStub *pMaterial) = 0;
    /** \details
      Checks whether the current mapper, material, and matrix are valid for use with previously set up mapper data.
      \param pMapper [in]  Entity mapper.
      \param pMaterial [in]  Material ID.
      \param tm [in]  Object/model transformation matrix.
      \returns
      true if the current material, mapper and tranform matrix are valid for use with previously set up mapper data, false otherwise.
    */
    virtual bool isLastProcValid(const OdGiMapper *pMapper, const OdDbStub *pMaterial, const OdGeMatrix3d &tm) = 0;
    /** \details
      Checks whether the current object/model matrix is valid for use with previously set up mapper data.
      \param tm [in]  Object/model transformation matrix.
      \returns
      true if the current object/model matrix is valid for use with previously set up mapper data, false otherwise.
    */
    virtual bool isLastProcValid(const OdGeMatrix3d &tm) = 0;

    /** \details
      Checks whether entity mapper is specified for this mapper.
      \returns
      true if entity mapper is specified, false otherwise.
    */
    virtual bool isEntityMapper() const = 0;
    /** \details
      Checks whether an object matrix is needed for this mapper.
      \returns
      true if mapper needs an object matrix, false otherwise.
    */
    virtual bool isObjectMatrixNeed() const = 0;
    /** \details
      Checks whether a model matrix is needed for this mapper.
      
      \returns
      true if mapper needs model matrix, false.
    */
    virtual bool isModelMatrixNeed() const = 0;
    /** \details
      Checks whether this mapper item requires a model transform.
      
      \returns
      true if mapper requires object or vertex transform, false otherwise.
    */
    virtual bool isDependsFromObjectMatrix() const = 0;
    /** \details
      Checks whether this mapper item requires a vertex transform.
      
      \returns
      true if mapper requires vertex transform, false otherwise.
    */
    virtual bool isVertexTransformRequired() const = 0;

    /** \details
      Sets vertex transform from points array for channels which needs vertex transformation.
      
      \param nCount [in]  Number of transform points in the array.
      \param pPoints [in]  Pointer to an array of vertex transform points.
    */
    virtual void setVertexTransform(OdInt32 nCount, const OdGePoint3d *pPoints) = 0;
    /** \details
      Sets vertex transform from object extents for channels which needs vertex transformation.
      \param exts [in]  Extents that defines vertex transform vertices.
    */
    virtual void setVertexTransform(const OdGeExtents3d &exts) = 0;

    /** \details
      Sets input tranform for all channels.
      \param tm [in]  Input transformation matrix.
      \param bVertexDependantOnly [in]  Flag that specifies whether to set transformation matrix only to channels which require vertex.
    */
    virtual void setInputTransform(const OdGeMatrix3d &tm, bool bVertexDependantOnly = true) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMapperItem object pointers.

	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMapperItem> OdGiMapperItemPtr;

/** \details
    Container item for material data storage.
    OdGiMapperRenderItem used for rendering devices.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMapperRenderItem : public OdGiMapperItem
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMapperRenderItem);

    /** \details
      Sets a mapper for diffuse texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setDiffuseMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for diffuse texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setDiffuseMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Sets a mapper for diffuse texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setDiffuseMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for diffuse texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setDiffuseMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Retrieves a mapper for diffuse texture channel.
      
      \returns
      a smart pointer to a mapper for diffuse texture channel.
    */
    virtual OdGiMapperItemEntryPtr diffuseMapper() const = 0;
    /** \details
      Retrieves a mapper for diffuse texture channel.
      
      \returns
      a smart pointer to a mapper for diffuse texture channel.
    */
    virtual OdGiMapperItemEntryPtr diffuseMapper() = 0;

    /** \details
      Sets a mapper for specular texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setSpecularMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for specular texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setSpecularMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Sets a mapper for specular texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setSpecularMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for specular texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setSpecularMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Retrieves a mapper for specular texture channel.
      
      \returns
      smart pointer to a mapper for specular texture channel.
    */
    virtual OdGiMapperItemEntryPtr specularMapper() const = 0;
    /** \details
      Retrieves a smart pointer to a mapper for specular texture channel.
      
      \returns
      smart pointer to a mapper for specular texture channel.
    */
    virtual OdGiMapperItemEntryPtr specularMapper() = 0;

    /** \details
      Sets a mapper for reflection texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setReflectionMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for reflection texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setReflectionMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Sets a mapper for reflection texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setReflectionMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for reflection texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setReflectionMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Retrieves a mapper for reflection texture channel.
      
      \returns
      smart pointer to a mapper for reflection texture channel.
    */
    virtual OdGiMapperItemEntryPtr reflectionMapper() const = 0;
    /** \details
      Return mapper for reflection texture channel.
    */
    virtual OdGiMapperItemEntryPtr reflectionMapper() = 0;

    /** \details
      Sets a mapper for opacity texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setOpacityMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for opacity texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setOpacityMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Sets a mapper for opacity texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setOpacityMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for opacity texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setOpacityMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Retrieves a mapper for opacity texture channel.
      
      \returns
      smart pointer to a mapper for opacity texture channel.
    */
    virtual OdGiMapperItemEntryPtr opacityMapper() const = 0;
    /** \details
      Retrieves a mapper for opacity texture channel.
      
      \returns
      smart pointer to a mapper for opacity texture channel.
    */
    virtual OdGiMapperItemEntryPtr opacityMapper() = 0;

    /** \details
      Sets a mapper for bump texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setBumpMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for bump texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setBumpMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Sets a mapper for bump texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setBumpMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for bump texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setBumpMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Retrieves a mapper for bump texture channel.
      
      \returns
      smart pointer to a mapper for bump texture channel.
    */
    virtual OdGiMapperItemEntryPtr bumpMapper() const = 0;
    /** \details
      Retrieves a mapper for bump texture channel.
      
      \returns
      smart pointer to a mapper for bump texture channel.
    */
    virtual OdGiMapperItemEntryPtr bumpMapper() = 0;

    /** \details
      Sets a mapper for reflection texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setRefractionMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for refraction texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setRefractionMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Sets a mapper for reflection texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setRefractionMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for refraction texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setRefractionMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Retrieves a mapper for reflection texture channel.
      
      \returns
      smart pointer to a mapper for reflection texture channel.
    */
    virtual OdGiMapperItemEntryPtr refractionMapper() const = 0;
    /** \details
      Retrieves a mapper for reflection texture channel.
      
      \returns
      smart pointer to a mapper for reflection texture channel.
    */
    virtual OdGiMapperItemEntryPtr refractionMapper() = 0;

    /** \details
      Sets a mapper for normal map texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setNormalMapMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for normal map texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setNormalMapMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Sets a mapper for normal map texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setNormalMapMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for normal map texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setNormalMapMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Retrieves a mapper for normal map texture channel.
      
      \returns
      smart pointer to a mapper for normal map texture channel.
    */
    virtual OdGiMapperItemEntryPtr normalMapMapper() const = 0;
    /** \details
      Retrieves a mapper for normal map texture channel.
      
      \returns
      smart pointer to a mapper for normal map texture channel.
    */
    virtual OdGiMapperItemEntryPtr normalMapMapper() = 0;

    /** \details
      Sets a mapper for emission texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setEmissionMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for emission texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setEmissionMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Sets a mapper for emission texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setEmissionMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Sets a mapper for emission texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setEmissionMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Retrieves a mapper for emission texture channel.
      
      \returns
      smart pointer to a mapper for emission texture channel.
    */
    virtual OdGiMapperItemEntryPtr emissionMapper() const = 0;
    /** \details
      Retrieves a mapper for emission texture channel.
      
      \returns
      smart pointer to a mapper for emission texture channel.
    */
    virtual OdGiMapperItemEntryPtr emissionMapper() = 0;

    /** \details
      Sets a mapper for all texture channels.
      \param traitsData [in]  Input traits data.
      \param pMaterial [in]  Material ID for input traits data.
    */
    virtual void setMapper(const OdGiMaterialTraitsData &traitsData, const OdDbStub *pMaterial = NULL) = 0;
    /** \details
      Sets a mapper for all texture channels.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param pMaterial [in]  Material ID for input traits data.
    */
    virtual void setMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdDbStub *pMaterial = NULL) = 0;

    /** \details
      Sets model transformation for all texture channels which needs model transformation.
      \param mtm [in]  Model transformation matrix.
      \param recomputeTransforms [in]  Call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setModelTransform(const OdGeMatrix3d &mtm, bool recomputeTransforms = true) = 0;

    /** \details
      Sets object transformation for all texture channels which needs object transformation.
      \param otm [in]  Object transformation matrix.
      \param recomputeTransforms [in]  Flag that specifies whether to call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setObjectTransform(const OdGeMatrix3d &otm, bool recomputeTransforms = true) = 0;
    /** \details
      Sets object transformation for all texture channels which needs object transformation.
      \param nCount [in]  Number of points into pPoints array.
      \param pPoints [in]  Array of points.
      \param recomputeTransforms [in]  Flag that specifies whether to call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setObjectTransform(OdInt32 nCount, const OdGePoint3d *pPoints, bool recomputeTransforms = true) = 0;
    /** \details
      Sets object transformation for all texture channels which needs object transformation.
      \param exts [in]  Object extents.
      \param recomputeTransforms [in]  Flag that specifies whether to call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setObjectTransform(const OdGeExtents3d &exts, bool recomputeTransforms = true) = 0;

    /** \details
      Sets device-dependent transformation for all texture channels.
      \param dtm [in]  Device transformation matrix.
      \param recomputeTransforms [in]  Flag that specifies whether to call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setDeviceTransform(const OdGeMatrix3d &dtm, bool recomputeTransforms = true) = 0;

    /** \details
      Checks whether the current material is valid for use with previously set up mapper data.
      \param pMaterial [in]  Material ID.
      
      \returns
      true if the current material is valid for use with previously set up mapper data, false otherwise.
    */
    virtual bool isLastProcValid(const OdDbStub *pMaterial) = 0;
    /** \details
      Checks whether the current material and matrix are valid for use with previously set up mapper data.
      \param pMaterial [in]  Material ID.
      \param tm [in]  object/model transformation matrix.
      
      \returns
      true if the current material is valid for use with previously set up mapper data, false otherwise.
    */
    virtual bool isLastProcValid(const OdDbStub *pMaterial, const OdGeMatrix3d &tm) = 0;
    /** \details
      Checks whether the current mapper and material are valid for use with previously set up mapper data.
      \param pMapper [in]  Entity mapper.
      \param pMaterial [in]  Material ID.
      
      \returns
      true if the current material is valid for use with previously set up mapper data, false otherwise.
    */
    virtual bool isLastProcValid(const OdGiMapper *pMapper, const OdDbStub *pMaterial) = 0;
    /** \details
      Checks whether the current mapper, material, and matrix are valid for use with previously set up mapper data.
      \param pMapper [in]  Entity mapper.
      \param pMaterial [in]  Material ID.
      \param tm [in]  Object/model transformation matrix.
      
      \returns
      true if the current material is valid for use with previously set up mapper data, false otherwise.
    */
    virtual bool isLastProcValid(const OdGiMapper *pMapper, const OdDbStub *pMaterial, const OdGeMatrix3d &tm) = 0;
    /** \details
      Checks whether the current object/model matrix is valid for use with previously set up mapper data.
      \param tm [in]  Object/model transformation matrix.
      
      \returns
      true if the current material is valid for use with previously set up mapper data, false otherwise.
    */
    virtual bool isLastProcValid(const OdGeMatrix3d &tm) = 0;

    /** \details
      Checks whether the entity mapper is specified for all channels.
      
      \returns
      true if entity mapper is specified for all channels, false otherwise.
    */
    virtual bool isEntityMapper() const = 0;
    /** \details
      Return true if at least one mapper needs object matrix.
    */
    virtual bool isObjectMatrixNeed() const = 0;
    /** \details
      Checks whether any mapper needs a model matrix.
      
      \returns
      true if at least one mapper needs a model matrix.
    */
    virtual bool isModelMatrixNeed() const = 0;
    /** \details
      Checks whether a model matrix is needed for this mapper.
      \returns
      true if mapper requires object or vertex transform.
    */
    virtual bool isDependsFromObjectMatrix() const = 0;
    /** \details
      Checks whether mapper requires a vertex transform. A vertex transform is required in case if mapper has automatic transform mode == kObject.
      \returns
      true if mapper requires vertex transform, false otherwise.
    */
    virtual bool isVertexTransformRequired() const = 0;

    /** \details
      Sets vertex transform from points array.
      \param nCount [in]  Number of points.
      \param pPoints [in]  Pointer to the array of points.
    */
    virtual void setVertexTransform(OdInt32 nCount, const OdGePoint3d *pPoints) = 0;
    /** \details
      Sets vertex transform from object extents.
      \param exts [in]  Extents that defines vertex transform matrix.
    */
    virtual void setVertexTransform(const OdGeExtents3d &exts) = 0;

    /** \details
      Sets input tranform for all channels.
      \param tm [in]  Input transformation matrix.
      \param bVertexDependantOnly [in]  Set to channels which require vertex tranform only.
    */
    virtual void setInputTransform(const OdGeMatrix3d &tm, bool bVertexDependantOnly = true) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMapperRenderItem object pointers.
	
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMapperRenderItem> OdGiMapperRenderItemPtr;

#include "TD_PackPop.h"

#endif // __ODGIMAPPERITEM_H__
