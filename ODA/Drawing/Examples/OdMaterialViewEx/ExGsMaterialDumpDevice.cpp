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

#include "DbObject.h"

#include "RxObjectImpl.h"

#include "ExGsMaterialDumpDevice.h"
#include "ColorMapping.h"
#include "toString.h"

// ExDumpOutput

ExDumpOutput::ExDumpOutput()
  : m_indentLevel(0)
{
}

ExDumpOutput::~ExDumpOutput()
{
}

OdSmartPtr<ExDumpOutput> ExDumpOutput::createObject()
{
  return OdRxObjectImpl<ExDumpOutput>::createObject();;
}

/************************************************************************/
/* Output a string in the form                                          */
/*   str1:. . . . . . . . . . . .str2                                   */
/************************************************************************/
void ExDumpOutput::output(const OdString& str1, 
                          const OdString& str2)
{
  const OdString spaces(OD_T("                                                            "));
  const OdString leader(OD_T(". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . "));

  OdString buffer;

  const int colWidth = 38;

  buffer = str1;
  /**********************************************************************/
  /* If rightString is not specified, just output the indented          */
  /* leftString. Otherwise, fill the space between leftString and       */
  /* rightString with leader characters.                                */
  /**********************************************************************/  
  if (str2 != OD_T(""))
  {
    int leaders = colWidth-(str1.getLength() + m_indentLevel);
    if (leaders > 0){
      buffer = str1 + leader.mid(str1.getLength() + m_indentLevel, leaders) + str2;  
    }
    else
    {
      buffer = OD_T(">") + str1 + OD_T(" ") + str2;  
    }
  }
  output(buffer);
} // end ExDumpOutput::output
/************************************************************************/
/* Output and indent a character string                                 */
/************************************************************************/
void ExDumpOutput::output(const OdString& str)
{
  OdString indent;
  for (int i = 0; i < m_indentLevel; i++)
    indent += ' ';
  odPrintConsoleString(L"%ls%ls\n", indent.c_str(), str.c_str());
} // end ExDumpOutput::output

// ExGsMaterialDumpDevice

ExGsMaterialDumpDevice::ExGsMaterialDumpDevice()
{
  /**********************************************************************/
  /* Set a palette with a white background.                             */
  /**********************************************************************/  
  setLogicalPalette(odcmAcadLightPalette(), 256);

  /**********************************************************************/
  /* Set the default size of the vectorization window                   */
  /**********************************************************************/  
  onSize(OdGsDCRect(0, 100, 0, 100));
} // end ExGsMaterialDumpDevice::ExGsMaterialDumpDevice

OdGsDevicePtr ExGsMaterialDumpDevice::createObject()
{
  OdGsDevicePtr pRes = OdRxObjectImpl<ExGsMaterialDumpDevice, OdGsDevice>::createObject();
  ExGsMaterialDumpDevice* pMyDev = static_cast<ExGsMaterialDumpDevice*>(pRes.get());

  /**********************************************************************/
  /* Create the output geometry dumper                                  */
  /**********************************************************************/  
  pMyDev->m_pDumper = ExDumpOutput::createObject();

  return pRes;
} // end ExGsMaterialDumpDevice::createObject

/************************************************************************/
/* Creates a new OdGsView object, and associates it with this Device    */
/* object.                                                              */
/*                                                                      */
/* pInfo is a pointer to the Client View Information for this Device    */
/* object.                                                              */
/*                                                                      */
/* bEnableLayerVisibilityPerView specifies that layer visibility        */
/* per viewport is to be supported.                                     */
/************************************************************************/  
OdGsViewPtr ExGsMaterialDumpDevice::createView(const OdGsClientViewInfo* pInfo, 
                                               bool bEnableLayerVisibilityPerView)
{
  /**********************************************************************/
  /* Create a View                                                      */
  /**********************************************************************/
  OdGsViewPtr pView = ExGsMaterialDumpView::createObject();
  ExGsMaterialDumpView* pMyView = static_cast<ExGsMaterialDumpView*>(pView.get());

  /**********************************************************************/
  /* This call is required by DD 1.13+                                  */
  /**********************************************************************/
  pMyView->init(this, pInfo, bEnableLayerVisibilityPerView);

  return (OdGsView*)pMyView;
} // end ExGsMaterialDumpDevice::createView

/************************************************************************/
/* Returns the geometry dumper associated with this object.             */
/************************************************************************/  
ExDumpOutputPtr ExGsMaterialDumpDevice::dumper()
{
  return m_pDumper;
} // end ExGsMaterialDumpDevice::dumper

/************************************************************************/
/* Called by the ODA vectorization framework to update                  */
/* the GUI window for this Device object                                */
/*                                                                      */
/* pUpdatedRect specifies a rectangle to receive the region updated     */
/* by this function.                                                    */
/*                                                                      */
/* The override should call the parent version of this function,        */
/* OdGsBaseVectorizeDevice::update().                                   */
/************************************************************************/
void ExGsMaterialDumpDevice::update(OdGsDCRect* pUpdatedRect)
{
  OdGsBaseVectorizeDevice::update(pUpdatedRect);
} // end ExGsMaterialDumpDevice::update

/************************************************************************/
/* Called by the ODA vectorization framework to give the                */
/* client application a chance to terminate the current                 */
/* vectorization process.  Returning true from this function will       */
/* stop the current vectorization operation.                            */
/************************************************************************/
bool ExGsMaterialDumpView::regenAbort() const
{
  // return true here to abort the vectorization process
  return false;
} // end ExGsMaterialDumpView::regenAbort

/************************************************************************/
/* Called by the ODA vectorization framework to vectorize               */
/* each entity in this view.  This override allows a client             */
/* application to examine each entity before it is vectorized.          */
/* The override should call the parent version of this function,        */
/* OdGsBaseVectorizeView::draw().                                       */
/************************************************************************/
void ExGsMaterialDumpView::draw(const OdGiDrawable* pDrawable)
{
  OdDbObjectPtr pEnt = OdDbObject::cast(pDrawable);
 
  device()->dumper()->output(OD_T(""));
  if (pEnt.get())
  {
    device()->dumper()->output(OD_T("Start Drawing ") + toString(pEnt->objectId()), toString(pEnt->objectId().getHandle()));
  }
  else
  {
    device()->dumper()->output(OD_T("Start Drawing") + toString(pEnt->objectId()), toString(OD_T("non-DbResident")));
  }

  device()->dumper()->pushIndent();

  /**********************************************************************/
  /* Enable triangle vertices output only for AcDb3dSolid entities.     */
  /**********************************************************************/
  OdRxClassPtr p3dSolidClass = ::odrxClassDictionary()->getAt(OD_T("AcDb3dSolid"));
  OdRxClassPtr pPlaneSurfaceClass = ::odrxClassDictionary()->getAt(OD_T("AcDbPlaneSurface"));
  if (!pDrawable->isKindOf(p3dSolidClass) &&
      !pDrawable->isKindOf(pPlaneSurfaceClass))
  {
    m_bEnableDump = false;
    device()->dumper()->output(OD_T("Skip geometry dump."));
  }
  else
  {
    m_bEnableDump = true;
    m_uNTriangle = 0;
    /********************************************************************/
    /* Reset current material, for force material changing in           */
    /* OdGsBaseMaterialView. This call force output of current material */
    /* at each entity begin.                                            */
    /********************************************************************/
    resetCurrentMaterial();
#ifndef OD_GSMAT_USEDELAYCACHE
    /********************************************************************/
    /* Precompute object extents.                                       */
    /* You could use here any own method, but for test I use            */
    /* OdGsBaseMaterialView function which simply uses OdGiExtCalc for  */
    /* extents computation. It can be also overrided in user code.      */
    /********************************************************************/
    computeDelayedExtents(pDrawable, m_DrawableExtents);
#endif
  }

  /**********************************************************************/
  /* The parent class version of this function must be called.          */
  /**********************************************************************/
  OdGsBaseVectorizer::draw(pDrawable);

  m_bEnableDump = false;

  device()->dumper()->popIndent();
  if (pEnt.get())
  {
    device()->dumper()->output(OD_T("End Drawing ") + toString(pEnt->objectId()), toString(pEnt->objectId().getHandle()));
  }
  else
  {
    device()->dumper()->output(OD_T("End Drawing ") + toString(pEnt->objectId()), toString(OD_T("non-DbResident")));
  }
} // end ExGsMaterialDumpView::draw

/************************************************************************/
/* Flushes any queued graphics to the display device.                   */
/************************************************************************/
void ExGsMaterialDumpView::update()
{
  /**********************************************************************/
  /* Hard-coded set of render mode to shaded, for force rendering of    */
  /* shells as triangles and avoid unnecessary wireframes and contours. */
  /**********************************************************************/
  setMode(OdGsView::kFlatShaded);
  /**********************************************************************/
  /* Hard-coded set of viewer coordinate system to identity, for force  */
  /* vertices coordinates to be in correct WCS.                         */
  /**********************************************************************/
  setView(OdGePoint3d::kOrigin, OdGePoint3d::kOrigin + OdGeVector3d::kZAxis, OdGeVector3d::kYAxis, 1.0, 1.0);

  OdGsBaseVectorizeView::update();
} // end ExGsMaterialDumpView::update()


/************************************************************************/
/* Notification function called by the vectorization framework          */
/* whenever the visualization attributes have changed.                  */
/*                                                                      */
/* Retrieves the current vectorization traits (color,                   */
/* lineweight, etc.) and sets them in this device.                      */
/************************************************************************/
void ExGsMaterialDumpView::onTraitsModified()
{
  OdGsBaseMaterialView::onTraitsModified();
} // end ExGsMaterialDumpView::onTraitsModified()


void ExGsMaterialDumpView::beginViewVectorization()
{
  OdGsBaseMaterialView::beginViewVectorization();

  /**********************************************************************/
  /* Directs the output geometry for the view to the                    */
  /* destination geometry object                                        */
  /**********************************************************************/
  output().setDestGeometry(*this);
  /**********************************************************************/
  /* If geometry receiver is a simplifier, we must re-set the draw      */
  /* context for it                                                     */
  /**********************************************************************/
  OdGiGeometrySimplifier::setDrawContext(OdGsBaseVectorizer::drawContext());
} // end ExGsMaterialDumpView::beginViewVectorization

/****************************************************************************/
/* This class will be returned back from delay cache of texture coordinates */
/* calculation need's entity extents.                                       */
/****************************************************************************/
class ExGsDelayedTriangleOut : public OdGsBaseMaterialView::DelayCacheEntry
{
  friend class ExGsMaterialDumpView;
  protected:
    OdUInt32 m_uTriNum;
    OdGePoint3d m_points[3];
    bool m_bNormal;
    OdGeVector3d m_normal;
  public:
    ExGsDelayedTriangleOut(OdUInt32 uTriNum,
                           const OdGePoint3d &p1, const OdGePoint3d &p2, const OdGePoint3d &p3,
                           const OdGeVector3d* pNormal) : OdGsBaseMaterialView::DelayCacheEntry()
    {
      m_uTriNum = uTriNum;
      m_points[0] = p1; m_points[1] = p2; m_points[2] = p3;
      m_bNormal = pNormal != NULL;
      if (pNormal)
        m_normal = *pNormal;
    }
};

/**********************************************************************/
/* Called by OdGiGeometrySimplifier for each output triangle.         */
/**********************************************************************/
void ExGsMaterialDumpView::triangleOut(const OdInt32* vertices,
                                       const OdGeVector3d* pNormal)
{
  if (m_bEnableDump)
  {
    OdGePoint3d trgPoints[3] =
    {
      vertexDataList()[vertices[0]],
      vertexDataList()[vertices[1]],
      vertexDataList()[vertices[2]]
    };

    OdGePoint2d texCoords[3];

    bool bTextureCoordsDelayed = false;

    if (m_bDumpTextureCoordinates)
    {
      /******************************************************************/
      /* Compute texture coordinates.                                   */
      /******************************************************************/
      if (pNormal)
      {
        currentMapper()->diffuseMapper()->mapCoords(trgPoints, *pNormal, texCoords);
      }
      else
      {
        /****************************************************************/
        /* Normal must be calculated, if it is not available.           */
        /****************************************************************/
        currentMapper()->diffuseMapper()->mapCoords(trgPoints, texCoords);
      }

      bTextureCoordsDelayed = isMappingDelayed(); // Always return false if delay cache was disabled
    }

    /********************************************************************/
    /* Dump vertices and texture coordinates to output.                 */
    /********************************************************************/
    device()->dumper()->output(toString(OD_T("Triangle[%d]"), (int)m_uNTriangle++));
    device()->dumper()->pushIndent();
    for (int nVert = 0; nVert < 3; nVert++)
    {
      device()->dumper()->output(toString(OD_T("Vertex[%d]"), nVert), toString(trgPoints[nVert]));
      if (m_bDumpTextureCoordinates)
      {
        device()->dumper()->pushIndent();
        if (!bTextureCoordsDelayed)
        {
          /********************************************************************/
          /* Correct texture coordinates available.                           */
          /********************************************************************/
          device()->dumper()->output(OD_T("Texture Coordinate"), toString(texCoords[nVert]));
        }
        else
        {
          /********************************************************************/
          /* Need entity extents for correct texture coordinates computation. */
          /********************************************************************/
          device()->dumper()->output(OD_T("Texture Coordinate"), toString(OD_T("Delayed")));
        }
        device()->dumper()->popIndent();
      }
    }
    if (bTextureCoordsDelayed)
    {
       appendDelayCacheEntry(new ExGsDelayedTriangleOut(m_uNTriangle - 1, trgPoints[0], trgPoints[1], trgPoints[2],
                                                        pNormal));
    }
    device()->dumper()->popIndent();
  }
} // end ExGsMaterialDumpView::triangleOut

/**********************************************************************/
/* Called by OdGiGeometrySimplifier for each output polyline.         */
/**********************************************************************/
void ExGsMaterialDumpView::polylineOut(OdInt32 /*nVertices*/,
                                       const OdGePoint3d* /*pPoints*/)
{
  // Avoid assertion in OdGiGeometrySimplifier::polylineOut()
} // end ExGsMaterialDumpView::polylineOut

#ifdef OD_GSMAT_USEDELAYCACHE
/**********************************************************************/
/* This callback method called by OdGsBaseMaterialView for delayed    */
/* entries in cache by default.                                       */
/**********************************************************************/
void ExGsMaterialDumpView::playDelayCacheEntry(const DelayCacheEntry *pEntry)
{
  const ExGsDelayedTriangleOut *pTri = static_cast<const ExGsDelayedTriangleOut *>(pEntry);

  OdGePoint2d texCoords[3];

  if (pTri->m_bNormal)
  {
    currentMapper()->diffuseMapper()->mapCoords(pTri->m_points, pTri->m_normal, texCoords);
  }
  else
  {
    /****************************************************************/
    /* Normal must be calculated, if it is not available.           */
    /****************************************************************/
    currentMapper()->diffuseMapper()->mapCoords(pTri->m_points, texCoords);
  }

  /********************************************************************/
  /* Dump vertices and texture coordinates to output.                 */
  /********************************************************************/
  device()->dumper()->output(toString(OD_T("Delayed triangle[%d]"), (int)pTri->m_uTriNum));
  device()->dumper()->pushIndent();
  for (int nVert = 0; nVert < 3; nVert++)
  {
    device()->dumper()->output(toString(OD_T("Vertex[%d]"), nVert), toString(pTri->m_points[nVert]));

    device()->dumper()->pushIndent();

    /********************************************************************/
    /* Correct texture coordinates available.                           */
    /********************************************************************/
    device()->dumper()->output(OD_T("Texture Coordinate"), toString(texCoords[nVert]));

    device()->dumper()->popIndent();
  }
  device()->dumper()->popIndent();
} // end ExGsMaterialDumpView::playDelayCacheEntry
#endif

#ifndef OD_GSMAT_USEDELAYCACHE
/**********************************************************************/
/* This callback method called by OdGsBaseMaterialView for calculate  */
/* entity extents when they are needed for texture mapping if delay   */
/* cache disabled.                                                    */
/**********************************************************************/
bool ExGsMaterialDumpView::computeDrawableExtents(const OdGiDrawable * /*pDrawable*/, OdGeExtents3d &extents)
{
  /********************************************************************/
  /* Don't use here any calls to drawable worldDraw!                  */
  /********************************************************************/
  extents = m_DrawableExtents;
  return extents.isValidExtents();
} // end ExGsMaterialDumpView::computeDrawableExtents
#endif

/************************************************************************/
/* Simple example container for material data.                          */
/************************************************************************/
struct ExMaterialCache : public OdRxObject
{
  OdGiMaterialTraitsData m_MaterialTraits; // Store material traits data in cache
  bool m_bDiffuseTextureEnabled;
};

/************************************************************************/
/* This template class is a specialization of the OdSmartPtr class for  */
/* ExMaterialCache object pointers                                      */
/************************************************************************/
typedef OdSmartPtr<ExMaterialCache> ExMaterialCachePtr;

/************************************************************************/
/* Called by OdGsBaseMaterialView for fill cache for each material.     */
/* prevCache - Previous cache pointer if it available.                  */
/* materialId - Id of currently processing material.                    */
/* materialData - Current material traits container.                    */
/************************************************************************/
OdGiMaterialItemPtr ExGsMaterialDumpView::fillMaterialCache(OdGiMaterialItemPtr /*prevCache*/, OdDbStub * /*materialId*/, const OdGiMaterialTraitsData &materialData)
{
  /**********************************************************************/
  /* Initialize cache objects.                                          */
  /**********************************************************************/
  OdGiMaterialItemPtr pMatItem = OdGiMaterialItem::createObject();
  ExMaterialCachePtr pMatCache = OdRxObjectImpl<ExMaterialCache>::createObject();
  pMatCache->m_MaterialTraits = materialData;
  pMatCache->m_bDiffuseTextureEnabled = false;
  /**********************************************************************/
  /* Enable texture coordinates output flag, if any texture available   */
  /* into material map for diffuse channel.                             */
  /**********************************************************************/
  if (GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseDiffuse))
  {
    OdGiMaterialMap diffuseMap;
    OdGiMaterialColor diffuseColor;
    materialData.diffuse(diffuseColor, diffuseMap);
    if (diffuseMap.source() == OdGiMaterialMap::kFile)
    {
      pMatCache->m_bDiffuseTextureEnabled = !diffuseMap.sourceFileName().isEmpty();
    }
    else if (diffuseMap.source() == OdGiMaterialMap::kProcedural)
    {
      pMatCache->m_bDiffuseTextureEnabled = !diffuseMap.texture().isNull();
    }
    /********************************************************************/
    /* Texture in diffuse channel can be initialized like this.         */
    /********************************************************************/
    if (!pMatItem->createDiffuseTexture()->setGiMaterialTexture(NULL, OdGiMaterialTextureData::defaultTextureDataImplementationDesc(),
                                                                giCtx(), diffuseMap))
    {
      // The texture can't be loaded, remove empty texture data.
      pMatItem->removeDiffuseTexture();
    }
  }
  /**********************************************************************/
  /* Set any own RxObject to material item cache.                       */
  /**********************************************************************/
  pMatItem->setCachedData((OdRxObjectPtr)pMatCache);
  return pMatItem;
} // end ExGsMaterialDumpView::fillMaterialCache

/************************************************************************/
/* Called by OdGsBaseMaterialView for render currently selected         */
/* material cache.                                                      */
/* pCache - Pointer to currently processing cache.                      */
/* materialId - Id of currently processing material.                    */
/************************************************************************/
void ExGsMaterialDumpView::renderMaterialCache(OdGiMaterialItemPtr pCache, OdDbStub *materialId)
{
  /**********************************************************************/
  /* Get cache information from material item.                          */
  /**********************************************************************/
  ExMaterialCachePtr pMatCache = (ExMaterialCachePtr)pCache->cachedData();
  ODA_ASSERT(!pMatCache.isNull());
  /**********************************************************************/
  /* Dump information about material diffuse texture to the output.     */
  /**********************************************************************/
  if (m_bEnableDump)
  {
    device()->dumper()->output(OD_T("Material"), toString(OdDbObjectId(materialId)));
    device()->dumper()->pushIndent();
    if (pMatCache->m_bDiffuseTextureEnabled)
    {
      OdGiMaterialMap diffuseMap;
      OdGiMaterialColor diffuseColor;
      pMatCache->m_MaterialTraits.diffuse(diffuseColor, diffuseMap);
      if (diffuseMap.source() == OdGiMaterialMap::kFile)
      {
        device()->dumper()->output(OD_T("Texture Type"), OD_T("File"));
        device()->dumper()->pushIndent();
        device()->dumper()->output(OD_T("Source File"), diffuseMap.sourceFileName());
        device()->dumper()->popIndent();
      }
      else
      {
        device()->dumper()->output(OD_T("Texture Type"), OD_T("Procedural"));
        device()->dumper()->pushIndent();
        device()->dumper()->output(OD_T("Texture Class"), diffuseMap.texture()->isA()->name());
        device()->dumper()->popIndent();
      }
      /********************************************************************/
      /* Diffuse texture in material item can be accessed like this.      */
      /********************************************************************/
      if (pCache->haveDiffuseTexture())
      {
        OdGiPixelBGRA32Array textureBits;
        OdUInt32 textureWidth, textureHeight;
        pCache->diffuseTexture()->textureData()->textureData(textureBits, textureWidth, textureHeight);
        device()->dumper()->output(OD_T("Texture Data"), OD_T("Available"));
        device()->dumper()->pushIndent();
        device()->dumper()->output(OD_T("Texture Width"), toString((int)textureWidth));
        device()->dumper()->output(OD_T("Texture Height"), toString((int)textureHeight));
        device()->dumper()->popIndent();
      }
      else
      {
        device()->dumper()->output(OD_T("Texture Data"), OD_T("Not Available"));
      }
    }
    device()->dumper()->popIndent();
    /**********************************************************************/
    /* Enable dump texture coordinates if texture is available in diffuse */
    /* material channel.                                                  */
    /**********************************************************************/
    m_bDumpTextureCoordinates = pMatCache->m_bDiffuseTextureEnabled;
  }
} // end ExGsMaterialDumpView::renderMaterialCache

//==========================================================================================
