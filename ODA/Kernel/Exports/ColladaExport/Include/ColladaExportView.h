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

#ifndef _COLLADA_EXPORT_VIEW_INCLUDED_
#define _COLLADA_EXPORT_VIEW_INCLUDED_
#pragma once
#include "OdaCommon.h"
#include "OdError.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiDrawable.h"
#include "Gi/GiGeometrySimplifier.h"
#include "Gi/GiLightTraitsData.h"
#include "Gi/GiUtils.h"
#include "Gs/GsBaseVectorizer.h"
#include "Gs/GsBaseMaterialView.h"
#include "RxObjectImpl.h"
#include "RxDynamicModule.h"
#include "COLLADASWEffectProfile.h"
#include "LightExporter.h"


/** \details
  Provides a set of classes, structures and enumerations for working with Collada export.
*/
namespace TD_COLLADA_EXPORT
{
  /** Count of collada entities */
  static OdUInt64 iColladaEntCounter = 0;

  /** \details
    This class implements the material data for Collada.
  */
  struct MaterialData
  {
    /** Ambient color */
    COLLADASW::Color ambientColor;
    /** Diffuse color */
    COLLADASW::Color diffuseColor;
    /** Specular color */
    COLLADASW::Color specularColor;
    /** Ambient color factor */
    double           dAmbientColorFactor;
    /** Diffuse color factor */
    double           dDiffuseColorFactor;
    /** Specular color factor */
    double           dSpecularColorFactor;
    /** Availability diffuse texture */
    bool             bDiffuseHasTexture;
    /** Availability specular texture */
    bool             bSpecularHasTexture;
    /** Availability ambient channel */
    bool             bAmbientChannelEnabled;
    /** Availability diffuse channel */
    bool             bDiffuseChannelEnabled;
    /** Availability specular channel */
    bool             bSpecularChannelEnabled;
    /** Diffuse file source */
    OdString         sDiffuseFileSource;
    /** Specular file source */
    OdString         sSpecularFileSource;
    /** Glos factor */
    float glossFactor;
    /** Availability opacity channel */
    bool             bOpacityChannelEnabled;
    /** Opacity percentage */
    double           dOpacityPercentage;
    /** Availability opacity texture */
    bool             bOpacityHasTexture;
    /** Opacity file source */
    OdString         sOpacityFileSource;
    /** Availability reflection channel */
    bool             bReflectionChannelEnabled;
    /** Reflection percentage */
    double           dReflectionPercentage;
    /** Availability reflection texture */
    bool             bReflectionHasTexture;
    /** Reflection file source */
    OdString         sReflectionFileSource;
    /** Availability bump channel */
    bool             bBumpChannelEnabled;
    /** bump percentage */
    double           dBumpPercentage;
    /** Availability bump texture */
    bool             bBumpHasTexture;
    /** Availability bump texture */
    OdString         sBumpFileSource;
    /** Emmission percentage */
    double           dEmissionPercentage;
    /** Availbility refraction channel */
    bool             bRefractionChannelEnabled;
    /** Refraction index */
    double           dRefractionIndex;
    /** Translucence */
    double           dTranslucence;

    /** \details
      Comparison operator for the MaterialData object.

      \returns
      A boolean value that indicates whether the input MaterialData is identical to this MaterialData.
    */
    bool operator == ( MaterialData& val ) const
    {
      return (bAmbientChannelEnabled ? (ambientColor == val.ambientColor && OdEqual(dAmbientColorFactor, val.dAmbientColorFactor) && bAmbientChannelEnabled == val.bAmbientChannelEnabled):true)&&
       (bDiffuseChannelEnabled ? (diffuseColor == val.diffuseColor && OdEqual(dDiffuseColorFactor ,val.dDiffuseColorFactor) && bDiffuseChannelEnabled == val.bDiffuseChannelEnabled &&
       (bDiffuseHasTexture ? (bDiffuseHasTexture == val.bDiffuseHasTexture && sDiffuseFileSource == val.sDiffuseFileSource):true)):true) &&
       (bSpecularChannelEnabled ? (specularColor == val.specularColor && OdEqual(dSpecularColorFactor, val.dSpecularColorFactor) && bSpecularChannelEnabled == val.bSpecularChannelEnabled &&
       (bSpecularHasTexture ? (bSpecularHasTexture == val.bSpecularHasTexture && sSpecularFileSource == val.sSpecularFileSource):true)):true) &&
       (bOpacityChannelEnabled ? (OdEqual(dOpacityPercentage ,val.dOpacityPercentage) && bOpacityChannelEnabled == val.bOpacityChannelEnabled &&
       (bOpacityHasTexture ? (bOpacityHasTexture == val.bOpacityHasTexture && sOpacityFileSource == val.sOpacityFileSource):true)):true) &&
       (bReflectionChannelEnabled ? (OdEqual(dReflectionPercentage, val.dReflectionPercentage) && bReflectionChannelEnabled == val.bReflectionChannelEnabled &&
       (bReflectionHasTexture ? (bReflectionHasTexture == val.bReflectionHasTexture && sReflectionFileSource == val.sReflectionFileSource):true)):true) &&
       (bBumpChannelEnabled ? (OdEqual(dBumpPercentage, val.dBumpPercentage) && bBumpChannelEnabled == val.bBumpChannelEnabled &&
       (bBumpHasTexture ? (bBumpHasTexture == val.bBumpHasTexture && sBumpFileSource == val.sBumpFileSource):true)):true) &&
       (bRefractionChannelEnabled ? (bRefractionChannelEnabled == val.bRefractionChannelEnabled && OdEqual(dRefractionIndex, val.dRefractionIndex)):true) &&
       (OdEqual(glossFactor, val.glossFactor) && OdEqual(dEmissionPercentage, val.dEmissionPercentage) && OdEqual(dTranslucence, val.dTranslucence));
    }

    /** \details
      Creates a new instance of a MaterialData with default parameters.
    */
    MaterialData() : dAmbientColorFactor(0), dDiffuseColorFactor(0), dSpecularColorFactor(0),
                     bDiffuseHasTexture(false), bSpecularHasTexture(false), bAmbientChannelEnabled(false),
                     bDiffuseChannelEnabled(false), bSpecularChannelEnabled(false), bOpacityChannelEnabled(false),
                     dOpacityPercentage(0), bOpacityHasTexture(false), bReflectionChannelEnabled(false),
                     dReflectionPercentage(0), bReflectionHasTexture(false), bBumpChannelEnabled(false),
                     dBumpPercentage(0), bBumpHasTexture(false), dEmissionPercentage(0),
                     bRefractionChannelEnabled(false), dRefractionIndex(0), dTranslucence(0) {}
  };

  /** \details
    Defines flags of material
  */
  enum Flags
  {
    /** Raster image */
    kRasterImage    = 1,
    /** Raster image material */
    kRasterImageMat = 2,
    /** Entity */
    kAddEntity      = 4
  };

  /** \details
    This structure implements the material mapper for Collada.
  */
  struct ColladaMaterialMapper
  {
    /** Points of texture coordinates */
    OdGePoint2dArray   m_ptTextureCoordArr;
    /** Indexes of texture coordinates */
    OdUInt32Array      m_indTextureCoordArr;
  };

  /** \details
    This structure stores the ent data for Collada.
  */
  struct ColladaEntData
  {
    /** Array of points */
    OdGePoint3dArray      m_ptArr;
    /** Array of point's indexes */
    OdUInt32Array         m_indPtsArr;
    /** Array of point's normals */
    OdGeVector3dArray     m_normVtxArr;
    /** Array of normal's indexes */
    OdUInt32Array         m_indVtxNormArr;
    /** Array of normal's indexes */
    ColladaMaterialMapper m_pDiffuseMaterialMapperArr;
    /** Entity id */
    OdUInt64              m_iId;
    /** Index of entity material */
    unsigned int          m_iEntMaterial;

    /** \details
      Creates a new instance of a ColladaEntData with default parameters.
    */
    ColladaEntData() : m_iId(0), m_iEntMaterial(0) {}
  };

  /** \details
    This structure stores the material data for Collada.
  */
  struct ColladaMaterialData
  {
    /** Array of material data */
    OdArray<MaterialData>           m_matDataArr;
    /** Array of material data id's */
    OdUInt64Array                   m_matIdArr;
  };

  typedef OdArray<ColladaEntData> ColladaEntDataArray;
  typedef OdArray<ColladaEntDataArray> EntityDataArray;

  /** \details
    This class implements the stub vectorize device for Collada.
  */
  class StubVectorizeDevice : public OdGsBaseVectorizeDevice
  {
  protected:
    ODRX_USING_HEAP_OPERATORS(OdGsBaseVectorizeDevice);
  public:

    /** \details
      Creates a new instance of a StubVectorizeDevice with default parameters.
    */
    StubVectorizeDevice() { }
    /** \details
      Destroy instance of a StubVectorizeDevice.
    */
    ~StubVectorizeDevice() { }
  };  

  /** \details
    Fix color by ACI
  
    \param ids [in] inlut ODCOLORREF.
    \param color [in] entity color.

    \returns fixed OdCmEntityColor
  */
  static OdCmEntityColor fixByACI(const ODCOLORREF *ids, const OdCmEntityColor &color)
  {
    if (color.isByACI() || color.isByDgnIndex())
    {
      return OdCmEntityColor(ODGETRED(ids[color.colorIndex()]), ODGETGREEN(ids[color.colorIndex()]), ODGETBLUE(ids[color.colorIndex()]));
    }
    else if (!color.isByColor())
    {
      return OdCmEntityColor(0, 0, 0);
    }
    return color;
  }

  /** \details
    This class implements the Collada output.
  */
  class OdColladaOut : public OdGsBaseMaterialView, 
                       public OdGiGeometrySimplifier
  {
    /** Array of texture points */
    OdGePoint2dArray      m_texCoordsArray;

    /** Pointer on array of array entity data*/
    EntityDataArray*      m_pEntityDataArr;
    /** Pointer on material data*/
    ColladaMaterialData*  m_pColladaMaterialData;
    /** Pointer on light exporter*/
    LightExporter*        m_pLightExp;

    /** Pointer on array of entity data*/
    ColladaEntDataArray*  m_pCurrentColladaEntDataArr;
    /** Pointer on array of array entity data*/
    ColladaEntData*       m_pCurrentColladaEntData;
    /** Index of current material*/
    unsigned int          m_iCurMaterial;
    /** Flags for raster material*/
    OdUInt32              m_flags;
    /** Counter of Light */
    unsigned int          m_iLightCounter;

    /** temporary index */
    unsigned int          iTmpIndex;
    /** Count of points in array */
    OdInt32               m_iPtArrSize;
    /** Matrix for 3D transformation  */
    OdGeMatrix3d          m_matTransform;
    /** Map for deviations  */
    const std::map<OdDbStub*, double>* m_pMapDeviations;
    /** Deviation stack  */
    OdArray<double>       m_arrDeviationStack;

    /** \details
      Add ColladaEntData to array of entities with current material
    */
    void addColladaEntData();

  protected:
    OdGiMapperItemPtr setMapper(const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v, const OdGiRasterImage* pImage);

    void triangleOut( const OdInt32* p3Vertices, const OdGeVector3d* pNormal);

    void fillCache(OdUInt64 matId, const MaterialData& matData, unsigned int iStartFind);
    void fillMaterialCache(MaterialData& mData, const OdGiMaterialTraitsData &materialData);
    OdGiMaterialItemPtr fillMaterialCache(OdGiMaterialItemPtr prevCache, OdDbStub* materialId, const OdGiMaterialTraitsData & materialData);

    void polylineOut(OdInt32 , const OdGePoint3d* );

    void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal);
    void circle(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint);

    void circularArc(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d& startVector,
                     double sweepAngle, OdGiArcType arcType);

    void circularArc(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint, OdGiArcType arcType);

    void polyline(OdInt32 numVertices, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal, OdGsMarker baseSubEntMarker);

    void polygon(OdInt32 numVertices, const OdGePoint3d* vertexList);

    void pline(const OdGiPolyline& polyline, OdUInt32 fromIndex = 0, OdUInt32 numSegs = 0);

    void shape(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
               int shapeNumber, const OdGiTextStyle* pTextStyle);

    void text(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
              double height, double width, double oblique, const OdString& msg);

    void text(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
              const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle);

    void xline(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);
    void ray(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint );

    void nurbs(const OdGeNurbCurve3d& nurbsCurve);

    void ellipArc(const OdGeEllipArc3d& ellipArc, const OdGePoint3d* endPointsOverrides, OdGiArcType arcType);

    void mesh(OdInt32 numRows, OdInt32 numColumns, const OdGePoint3d* vertexList, const OdGiEdgeData* pEdgeData,
              const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData);

    void meshProc(OdInt32 numRows, OdInt32 numColumns, const OdGePoint3d* vertexList, const OdGiEdgeData* pEdgeData = 0,
                  const OdGiFaceData* pFaceData = 0, const OdGiVertexData* pVertexData = 0);

    void worldLine(const OdGePoint3d points[2]);

    void edge(const OdGiEdge2dArray& edges);

    void shell( OdInt32 numVerts, const OdGePoint3d* pVertexList,
                OdInt32 faceListSize, const OdInt32* pFaceList, const OdGiEdgeData* pEdgeData,
                const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData);

    void shellProc( OdInt32 numVertices, const OdGePoint3d* vertexList,
                    OdInt32 faceListSize, const OdInt32* faceList, const OdGiEdgeData* pEdgeData,
                    const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData);

    void shellFaceOut(OdInt32 faceListSize, const OdInt32* pFaceList, const OdGeVector3d* pNormal);

    void meshFaceOut(const OdInt32* faceList, const OdGeVector3d* pNormal);

    void rasterImageDc( const OdGePoint3d& origin,
                        const OdGeVector3d& u,
                        const OdGeVector3d& v,
                        const OdGiRasterImage* pImage,
                        const OdGePoint2d* uvBoundary,
                        OdUInt32 numBoundPts,
                        bool transparency = false,
                        double brightness = 50.0,
                        double contrast = 50.0,
                        double fade = 0.0);

    void image(const OdGiImageBGRA32& img,
               const OdGePoint3d& origin,
               const OdGeVector3d& uVec,
               const OdGeVector3d& vVec,
               OdGiRasterImage::TransparencyMode trpMode = OdGiRasterImage::kTransparency8Bit);

    void processMaterialNode(OdDbStub *materialId, OdGsMaterialNode *pNode);

    void initTexture( const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v,
                      const OdGiRasterImage* pImage, bool transparency, double brightness, double contrast, double fade);

    void uninitTexture();

    void addColladaEntity();
  public:

    /** \details
      Creates a new instance of a OdColladaOut with default parameters.
    */
    OdColladaOut();

    /** \details
      \returns StubVectorizeDevice if pointer is not NULL, not NULL pointer otherwise.
    */
    StubVectorizeDevice* device();

    /** \details
      Draws the specified drawable.
      
      \param drawableFlags [in]  Drawable flags.
      \param pDrawable [in]  Pointer to the drawable to draw.

      \returns
      true if drawable is successfully drawn, false otherwise.
    */
    bool doDraw(OdUInt32 i, const OdGiDrawable* pDrawable);

    /** \details
      Initializes OdColladaOut class by input parameters.
    
      \param pEntityDataArr [in] Pointer on array of array entity data.
      \param pColladaMaterialData [in]  Pointer on material data.
      \param pLightExp [in]  Pointer on light exporter.
      \param matTransform [in]  Matrix for 3D transformation.
      \param pMapDeviations [in]  Map for deviations.
    */
    void init(EntityDataArray* pEntityDataArr, ColladaMaterialData* pColladaMaterialData, 
      LightExporter* pLightExp, const OdGeMatrix3d& matTransform, const std::map<OdDbStub*, double>* pMapDeviations = NULL);

    /** \details
      Begins view vectorization.
    */
    void beginViewVectorization();

    /** \details
      Ends view vectorization.
    */
    void endViewVectorization();

    /** \details
      Updates the viewport (loads viewport trait  s, updates extents(if necessary), displays associated drawables and draws viewport frame).
    */
    virtual void updateViewport();

    /** \details
      \returns the recommended maximum deviation of the current vectorization, for the specified point on the curve or surface being tesselated.

      \param deviationType [in]  Deviation type.
      \param pointOnCurve [in]  Point on the curve.
    
      \returns
      recommended maximum deviation of the current vectorization as double value.
    
      \remarks
      deviationType must be one of the following:
    
      <table>
      Name                       Value
      kOdGiMaxDevForCircle       0
      kOdGiMaxDevForCurve        1
      kOdGiMaxDevForBoundary     2
      kOdGiMaxDevForIsoline      3
      kOdGiMaxDevForFacet        4
      </table>
    */
    virtual double deviation(const OdGiDeviationType deviationType, const OdGePoint3d& pointOnCurve) const;

    /** \details
      Sets render mode for the view.

      \param mode [in]  Render mode.
    */
    void setMode(OdGsView::RenderMode mode);
  };
}
#endif //_COLLADA_EXPORT_VIEW_INCLUDED_

