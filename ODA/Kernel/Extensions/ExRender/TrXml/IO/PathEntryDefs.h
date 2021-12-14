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
//
// PathEntryDefs.h
//

#ifndef PATH_ENTRY
#error "PATH_ENTRY declaration is missing."
#endif

#define PATH_ENTRY_EX(path, enKey, func) \
  PATH_ENTRY("MetafileAdded/MetafileData" path, MetafileAdded_MetafileData##enKey, func) \
  PATH_ENTRY("ViewportBackgroundChanged/MetafileData" path, ViewportBackgroundChanged_MetafileData##enKey, func)

///////////////////////////////////////////////////////////////////////////////

  PATH_ENTRY("TextureAdded", TextureAdded, func_Level)
    PATH_ENTRY("TextureAdded/TextureID", TextureAdded_TextureID, func_Id)
    PATH_ENTRY("TextureAdded/Type", TextureAdded_Type, func_UInt32)
    PATH_ENTRY("TextureAdded/Smooth", TextureAdded_Smooth, func_Boolean)
    PATH_ENTRY("TextureAdded/Modulate", TextureAdded_Modulate, func_Boolean)
    PATH_ENTRY("TextureAdded/IntensityMap", TextureAdded_IntensityMap, func_Boolean)
    PATH_ENTRY("TextureAdded/Negative", TextureAdded_Negative, func_Boolean)
    PATH_ENTRY("TextureAdded/NormalMap", TextureAdded_NormalMap, func_Boolean)
    PATH_ENTRY("TextureAdded/SphereMap", TextureAdded_SphereMap, func_Boolean)
    PATH_ENTRY("TextureAdded/CubeMap", TextureAdded_CubeMap, func_Boolean)
    PATH_ENTRY("TextureAdded/Grayscale", TextureAdded_Grayscale, func_Boolean)
    PATH_ENTRY("TextureAdded/Width", TextureAdded_Width, func_UInt32)
    PATH_ENTRY("TextureAdded/Height", TextureAdded_Height, func_UInt32)
    PATH_ENTRY("TextureAdded/ScanlineLength", TextureAdded_ScanlineLength, func_UInt32)
    PATH_ENTRY("TextureAdded/DataAlignment", TextureAdded_DataAlignment, func_UInt32)
    PATH_ENTRY("TextureAdded/TextureData", TextureAdded_TextureData, func_BinaryStream)
    PATH_ENTRY("TextureAdded/PaletteType", TextureAdded_PaletteType, func_UInt32)
    PATH_ENTRY("TextureAdded/PaletteWidth", TextureAdded_PaletteWidth, func_UInt32)
    PATH_ENTRY("TextureAdded/PaletteScanlineLength", TextureAdded_PaletteScanlineLength, func_UInt32)
    PATH_ENTRY("TextureAdded/PaletteDataAlignment", TextureAdded_PaletteDataAlignment, func_UInt32)
    PATH_ENTRY("TextureAdded/PaletteData", TextureAdded_PaletteData, func_BinaryStream)

  PATH_ENTRY("TextureDeleted", TextureDeleted, func_Level)
    PATH_ENTRY("TextureDeleted/TextureID", TextureDeleted_TextureID, func_Id)

  PATH_ENTRY("MaterialAdded", MaterialAdded, func_Level)
    PATH_ENTRY("MaterialAdded/MaterialID", MaterialAdded_MaterialID, func_Id)
#define PATH_MATERIAL_CHANNEL_ENTRY_(channelName, componentName) \
      PATH_ENTRY("MaterialAdded/" #channelName "/" #componentName, MaterialAdded_##channelName##_##componentName, func_Level) \
        PATH_ENTRY("MaterialAdded/" #channelName "/" #componentName "/ChannelColor", MaterialAdded_##channelName##_##componentName##_ChannelColor, func_Floats) \
        PATH_ENTRY("MaterialAdded/" #channelName "/" #componentName "/BlendFactor", MaterialAdded_##channelName##_##componentName##_BlendFactor, func_Float) \
        PATH_ENTRY("MaterialAdded/" #channelName "/" #componentName "/OpacityLevel", MaterialAdded_##channelName##_##componentName##_OpacityLevel, func_Float) \
        PATH_ENTRY("MaterialAdded/" #channelName "/" #componentName "/ChannelValue", MaterialAdded_##channelName##_##componentName##_ChannelValue, func_Float) \
        PATH_ENTRY("MaterialAdded/" #channelName "/" #componentName "/TextureBlend", MaterialAdded_##channelName##_##componentName##_TextureBlend, func_Float) \
        PATH_ENTRY("MaterialAdded/" #channelName "/" #componentName "/TextureID", MaterialAdded_##channelName##_##componentName##_TextureID, func_Id) \
        PATH_ENTRY("MaterialAdded/" #channelName "/" #componentName "/UWrapType", MaterialAdded_##channelName##_##componentName##_UWrapType, func_UInt32) \
        PATH_ENTRY("MaterialAdded/" #channelName "/" #componentName "/VWrapType", MaterialAdded_##channelName##_##componentName##_VWrapType, func_UInt32)
#define PATH_MATERIAL_CHANNEL_ENTRY(channelName) \
    PATH_ENTRY("MaterialAdded/" #channelName, MaterialAdded_##channelName, func_Level) \
      PATH_MATERIAL_CHANNEL_ENTRY_(channelName, RealisticComponent) \
      PATH_MATERIAL_CHANNEL_ENTRY_(channelName, ShadingComponent)
      PATH_MATERIAL_CHANNEL_ENTRY(AmbientChannel)
      PATH_MATERIAL_CHANNEL_ENTRY(DiffuseChannel)
      PATH_MATERIAL_CHANNEL_ENTRY(SpecularChannel)
      PATH_MATERIAL_CHANNEL_ENTRY(EmissionChannel)
      PATH_MATERIAL_CHANNEL_ENTRY(OpacityChannel)
      PATH_MATERIAL_CHANNEL_ENTRY(BumpMapChannel)
      PATH_MATERIAL_CHANNEL_ENTRY(ReflectionChannel)
      PATH_MATERIAL_CHANNEL_ENTRY(RefractionChannel)
      PATH_MATERIAL_CHANNEL_ENTRY(NormalMapChannel)
      PATH_MATERIAL_CHANNEL_ENTRY(TintChannel)
#undef PATH_MATERIAL_CHANNEL_ENTRY
#undef PATH_MATERIAL_CHANNEL_ENTRY_

  PATH_ENTRY("MaterialDeleted", MaterialDeleted, func_Level)
    PATH_ENTRY("MaterialDeleted/MaterialID", MaterialDeleted_MaterialID, func_Id)

  PATH_ENTRY("OverlayExtentsOverride", OverlayExtentsOverride, func_Level)
    PATH_ENTRY("OverlayExtentsOverride/ViewportID", OverlayExtentsOverride_ViewportID, func_Id)
    PATH_ENTRY("OverlayExtentsOverride/OverlayID", OverlayExtentsOverride_OverlayID, func_Id)
    PATH_ENTRY("OverlayExtentsOverride/SceneMin", OverlayExtentsOverride_SceneMin, func_Doubles)
    PATH_ENTRY("OverlayExtentsOverride/SceneMax", OverlayExtentsOverride_SceneMax, func_Doubles)
    PATH_ENTRY("OverlayExtentsOverride/RealMin", OverlayExtentsOverride_RealMin, func_Doubles)
    PATH_ENTRY("OverlayExtentsOverride/RealMax", OverlayExtentsOverride_RealMax, func_Doubles)

  PATH_ENTRY("MetafileAdded", MetafileAdded, func_Level)
    PATH_ENTRY("MetafileAdded/MetafileID", MetafileAdded_MetafileID, func_Id)
    PATH_ENTRY("MetafileAdded/GeneratorID", MetafileAdded_GeneratorID, func_Id)
    PATH_ENTRY("MetafileAdded/Visibility", MetafileAdded_Visibility, func_Boolean)
    PATH_ENTRY("MetafileAdded/Highlighted", MetafileAdded_Highlighted, func_Boolean)
    PATH_ENTRY("MetafileAdded/Temporary", MetafileAdded_Temporary, func_Boolean)
    PATH_ENTRY("MetafileAdded/Nested", MetafileAdded_Nested, func_Boolean)
    PATH_ENTRY("MetafileAdded/BlockRef", MetafileAdded_BlockRef, func_Boolean)
    PATH_ENTRY("MetafileAdded/Utilitary", MetafileAdded_Utilitary, func_Boolean)
    PATH_ENTRY("MetafileAdded/Sectionable", MetafileAdded_Sectionable, func_Boolean)
    PATH_ENTRY("MetafileAdded/Composite", MetafileAdded_Composite, func_Boolean)
    PATH_ENTRY("MetafileAdded/XRef", MetafileAdded_XRef, func_Boolean)
    PATH_ENTRY("MetafileAdded/Faded", MetafileAdded_Faded, func_Boolean)
    PATH_ENTRY("MetafileAdded/ShadowsCast", MetafileAdded_ShadowsCast, func_Boolean)
    PATH_ENTRY("MetafileAdded/ShadowsReceive", MetafileAdded_ShadowsReceive, func_Boolean)
    PATH_ENTRY("MetafileAdded/ExtMin", MetafileAdded_ExtMin, func_Doubles)
    PATH_ENTRY("MetafileAdded/ExtMax", MetafileAdded_ExtMax, func_Doubles)
      //parentsMetadata.push_back("MetafileAdded")) //DropMetadata

  PATH_ENTRY("MetafileHighlightingChanged", MetafileHighlightingChanged, func_Level)
    PATH_ENTRY("MetafileHighlightingChanged/MetafileID", MetafileHighlightingChanged_MetafileID, func_Id)
    PATH_ENTRY("MetafileHighlightingChanged/Highlight", MetafileHighlightingChanged_Highlight, func_Boolean)

  PATH_ENTRY("MetafileVisibilityChanged", MetafileVisibilityChanged, func_Level)
    PATH_ENTRY("MetafileVisibilityChanged/MetafileID", MetafileVisibilityChanged_MetafileID, func_Id)
    PATH_ENTRY("MetafileVisibilityChanged/Visibility", MetafileVisibilityChanged_Visibility, func_Boolean)

  PATH_ENTRY("MetafileFadingChanged", MetafileFadingChanged, func_Level)
    PATH_ENTRY("MetafileFadingChanged/MetafileID", MetafileFadingChanged_MetafileID, func_Id)
    PATH_ENTRY("MetafileFadingChanged/Fade", MetafileFadingChanged_Fade, func_Boolean)

  PATH_ENTRY("MetafileOrderChanged", MetafileOrderChanged, func_Level)
    PATH_ENTRY("MetafileOrderChanged/ViewportID", MetafileOrderChanged_ViewportID, func_Id)
    PATH_ENTRY("MetafileOrderChanged/OverlayID", MetafileOrderChanged_OverlayID, func_Id)
    PATH_ENTRY("MetafileOrderChanged/InvalidFrom", MetafileOrderChanged_InvalidFrom, func_UInt32)
    PATH_ENTRY("MetafileOrderChanged/InvalidLen", MetafileOrderChanged_InvalidLen, func_UInt32)
    PATH_ENTRY("MetafileOrderChanged/ValidLen", MetafileOrderChanged_ValidLen, func_UInt32)
    PATH_ENTRY("MetafileOrderChanged/Ordering", MetafileOrderChanged_Ordering, func_Level)
        //DropDisplayList
      PATH_ENTRY("MetafileOrderChanged/Ordering/MetafileID", MetafileOrderChanged_Ordering_MetafileID, func_Id)

  PATH_ENTRY("MetafileOrderInheritance", MetafileOrderInheritance, func_Level)
    PATH_ENTRY("MetafileOrderInheritance/ViewportID1", MetafileOrderInheritance_ViewportID1, func_Id )
    PATH_ENTRY("MetafileOrderInheritance/OverlayID", MetafileOrderInheritance_OverlayID, func_Id )
    PATH_ENTRY("MetafileOrderInheritance/ViewportID2", MetafileOrderInheritance_ViewportID2, func_Id )
      
  PATH_ENTRY("SurfaceChanged", SurfaceChanged, func_Level)
    PATH_ENTRY("SurfaceChanged/SurfaceSizeXMin", SurfaceChanged_SurfaceSizeXMin, func_Int32)
    PATH_ENTRY("SurfaceChanged/SurfaceSizeYMin", SurfaceChanged_SurfaceSizeYMin, func_Int32)
    PATH_ENTRY("SurfaceChanged/SurfaceSizeXMax", SurfaceChanged_SurfaceSizeXMax, func_Int32)
    PATH_ENTRY("SurfaceChanged/SurfaceSizeYMax", SurfaceChanged_SurfaceSizeYMax, func_Int32)
    PATH_ENTRY("SurfaceChanged/BackgroundColor", SurfaceChanged_BackgroundColor, func_UInt8RGB)
    PATH_ENTRY("SurfaceChanged/TransparentBackground", SurfaceChanged_TransparentBackground, func_Boolean)

  PATH_ENTRY("SurfaceInvalidated", SurfaceInvalidated, func_Level)

  PATH_ENTRY("OverlayAdded", OverlayAdded, func_Level)
    PATH_ENTRY("OverlayAdded/OverlayID", OverlayAdded_OverlayID, func_Id)
    PATH_ENTRY("OverlayAdded/Flags", OverlayAdded_Flags, func_UInt16)
    PATH_ENTRY("OverlayAdded/RenderOrder", OverlayAdded_RenderOrder, func_Int16)

  PATH_ENTRY("OverlayDeleted", OverlayDeleted, func_Level)
    PATH_ENTRY("OverlayDeleted/OverlayID", OverlayDeleted_OverlayID, func_Id)

  PATH_ENTRY("OverlayVisibilityChanged", OverlayVisibilityChanged, func_Level)
    PATH_ENTRY("OverlayVisibilityChanged/OverlayID", OverlayVisibilityChanged_OverlayID, func_Id)
    PATH_ENTRY("OverlayVisibilityChanged/Visibility", OverlayVisibilityChanged_Visibility, func_Boolean)

  PATH_ENTRY("OverlayRenderOrderChanged", OverlayRenderOrderChanged, func_Level)
    PATH_ENTRY("OverlayRenderOrderChanged/OverlayID", OverlayRenderOrderChanged_OverlayID, func_Id)
    PATH_ENTRY("OverlayRenderOrderChanged/RenderOrder", OverlayRenderOrderChanged_RenderOrder, func_Int16)

  PATH_ENTRY("ViewportAdded", ViewportAdded, func_Level)
    PATH_ENTRY("ViewportAdded/ViewportID", ViewportAdded_ViewportID, func_Id)
    PATH_ENTRY("ViewportAdded/OverallVpId", ViewportAdded_OverallVpId, func_Id)
    PATH_ENTRY("ViewportAdded/PSOverallViewport", ViewportAdded_PSOverallViewport, func_Boolean)
    PATH_ENTRY("ViewportAdded/PSModelViewport", ViewportAdded_PSModelViewport, func_Boolean)
    PATH_ENTRY("ViewportAdded/PSHelperViewport", ViewportAdded_PSHelperViewport, func_Boolean)
    PATH_ENTRY("ViewportAdded/ViewportVisible", ViewportAdded_ViewportVisible, func_Boolean)

  PATH_ENTRY("ViewportInserted", ViewportInserted, func_Level)
    PATH_ENTRY("ViewportInserted/ViewportID", ViewportInserted_ViewportID, func_Id)
    PATH_ENTRY("ViewportInserted/Index", ViewportInserted_Index, func_Int32)
    PATH_ENTRY("ViewportInserted/OverallVpId", ViewportInserted_OverallVpId, func_Id)
    PATH_ENTRY("ViewportInserted/PSOverallViewport", ViewportInserted_PSOverallViewport, func_Boolean)
    PATH_ENTRY("ViewportInserted/PSModelViewport", ViewportInserted_PSModelViewport, func_Boolean)
    PATH_ENTRY("ViewportInserted/PSHelperViewport", ViewportInserted_PSHelperViewport, func_Boolean)
    PATH_ENTRY("ViewportInserted/ViewportVisible", ViewportInserted_ViewportVisible, func_Boolean)

  PATH_ENTRY("ViewportDeleted", ViewportDeleted, func_Level)
    PATH_ENTRY("ViewportDeleted/ViewportID", ViewportDeleted_ViewportID, func_Id)

  PATH_ENTRY("ViewportVisibilityChanged", ViewportVisibilityChanged, func_Level)
    PATH_ENTRY("ViewportVisibilityChanged/ViewportID", ViewportVisibilityChanged_ViewportID, func_Id)
    PATH_ENTRY("ViewportVisibilityChanged/Visibility", ViewportVisibilityChanged_Visibility, func_Boolean)

  PATH_ENTRY("ViewportAntiAliasingChanged", ViewportAntiAliasingChanged, func_Level)
    PATH_ENTRY("ViewportAntiAliasingChanged/ViewportID", ViewportAntiAliasingChanged_ViewportID, func_Id)
    PATH_ENTRY("ViewportAntiAliasingChanged/AntiAliasing", ViewportAntiAliasingChanged_AntiAliasing, func_UInt32)

  PATH_ENTRY("ViewportViewParamsModified", ViewportViewParamsModified, func_Level)
    PATH_ENTRY("ViewportViewParamsModified/ViewportID", ViewportViewParamsModified_ViewportID, func_Id)
    PATH_ENTRY("ViewportViewParamsModified/ViewParams", ViewportViewParamsModified_ViewParams, func_Level)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/ScreenMatrix", ViewportViewParamsModified_ViewParams_ScreenMatrix, func_Matrix)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/ViewingMatrix", ViewportViewParamsModified_ViewParams_ViewingMatrix, func_Matrix)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/ProjectionMatrix", ViewportViewParamsModified_ViewParams_ProjectionMatrix, func_Matrix)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/CorrectionMatrix", ViewportViewParamsModified_ViewParams_CorrectionMatrix, func_Matrix)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/OutputMatrix", ViewportViewParamsModified_ViewParams_OutputMatrix, func_Matrix)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/MetafileMatrix", ViewportViewParamsModified_ViewParams_MetafileMatrix, func_Matrix)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/ViewPosition", ViewportViewParamsModified_ViewParams_ViewPosition, func_Doubles)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/ViewBasisX", ViewportViewParamsModified_ViewParams_ViewBasisX, func_Doubles)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/ViewBasisY", ViewportViewParamsModified_ViewParams_ViewBasisY, func_Doubles)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/ViewBasisZ", ViewportViewParamsModified_ViewParams_ViewBasisZ, func_Doubles)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/PerspectiveProjection", ViewportViewParamsModified_ViewParams_PerspectiveProjection, func_Boolean)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/FieldWidth", ViewportViewParamsModified_ViewParams_FieldWidth, func_Double)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/FieldHeight", ViewportViewParamsModified_ViewParams_FieldHeight, func_Double)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/LensLength", ViewportViewParamsModified_ViewParams_LensLength, func_Double)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/ViewTarget", ViewportViewParamsModified_ViewParams_ViewTarget, func_Doubles)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/FrontClip", ViewportViewParamsModified_ViewParams_FrontClip, func_Boolean)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/FrontClipDist", ViewportViewParamsModified_ViewParams_FrontClipDist, func_Double)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/BackClip", ViewportViewParamsModified_ViewParams_BackClip, func_Boolean)
      PATH_ENTRY("ViewportViewParamsModified/ViewParams/BackClipDist", ViewportViewParamsModified_ViewParams_BackClipDist, func_Double)

  PATH_ENTRY("ViewportLineStylesModified", ViewportLineStylesModified, func_Level)
    PATH_ENTRY("ViewportLineStylesModified/ViewportID", ViewportLineStylesModified_ViewportID, func_Id)
    PATH_ENTRY("ViewportLineStylesModified/LineStyleConfig", ViewportLineStylesModified_LineStyleConfig, func_UInt8RGB)

  PATH_ENTRY("ViewportFadingModified", ViewportFadingModified, func_Level)
    PATH_ENTRY("ViewportFadingModified/ViewportID", ViewportFadingModified_ViewportID, func_Id)
    PATH_ENTRY("ViewportFadingModified/FadingConfig", ViewportFadingModified_FadingConfig, func_UInt8RGB)

  PATH_ENTRY("ViewportLineweightsModified", ViewportLineweightsModified, func_Level)
    PATH_ENTRY("ViewportLineweightsModified/ViewportID", ViewportLineweightsModified_ViewportID, func_Id)
    PATH_ENTRY("ViewportLineweightsModified/NLineweightEnum", ViewportLineweightsModified_NLineweightEnum, func_UInt32)
    PATH_ENTRY("ViewportLineweightsModified/LineweightCoef", ViewportLineweightsModified_LineweightCoef, func_Double)
    PATH_ENTRY("ViewportLineweightsModified/LineweightEnum", ViewportLineweightsModified_LineweightEnum, func_Level)
      PATH_ENTRY("ViewportLineweightsModified/LineweightEnum/NPixels", ViewportLineweightsModified_LineweightEnum_NPixels, func_UInt8)

  PATH_ENTRY("ViewportShadingModified", ViewportShadingModified, func_Level)
    PATH_ENTRY("ViewportShadingModified/ViewportID", ViewportShadingModified_ViewportID, func_Id)
    PATH_ENTRY("ViewportShadingModified/ShaderId", ViewportShadingModified_ShaderId, func_Id)
    PATH_ENTRY("ViewportShadingModified/VisualStyleId", ViewportShadingModified_VisualStyleId, func_Id)
    PATH_ENTRY("ViewportShadingModified/DepthBuffer", ViewportShadingModified_DepthBuffer, func_Boolean)
    PATH_ENTRY("ViewportShadingModified/StencilBuffer", ViewportShadingModified_StencilBuffer, func_Boolean)
    PATH_ENTRY("ViewportShadingModified/PolygonOffset", ViewportShadingModified_PolygonOffset, func_Boolean)
    PATH_ENTRY("ViewportShadingModified/Lighting", ViewportShadingModified_Lighting, func_Boolean)
    PATH_ENTRY("ViewportShadingModified/FaceNormals", ViewportShadingModified_FaceNormals, func_Boolean)
    PATH_ENTRY("ViewportShadingModified/FaceFillColor", ViewportShadingModified_FaceFillColor, func_UInt8RGB)

  PATH_ENTRY("ViewportClipModified", ViewportClipModified, func_Level)
    PATH_ENTRY("ViewportClipModified/ViewportID", ViewportClipModified_ViewportID, func_Id)
    PATH_ENTRY("ViewportClipModified/ScreenRect", ViewportClipModified_ScreenRect, func_Doubles)
    PATH_ENTRY("ViewportClipModified/NrcClip", ViewportClipModified_NrcClip, func_Boolean)
    PATH_ENTRY("ViewportClipModified/NNrcCounts", ViewportClipModified_NNrcCounts, func_UInt32)
    PATH_ENTRY("ViewportClipModified/NNrcPoints", ViewportClipModified_NNrcPoints, func_UInt32)
    PATH_ENTRY("ViewportClipModified/NrcCounts", ViewportClipModified_NrcCounts, func_Level)
    PATH_ENTRY("ViewportClipModified/NrcCounts/Count", ViewportClipModified_NrcCounts_Count, func_Int32)
    PATH_ENTRY("ViewportClipModified/NrcPoints", ViewportClipModified_NrcPoints, func_Level)
    PATH_ENTRY("ViewportClipModified/NrcPoints/Point", ViewportClipModified_NrcPoints_Point, func_Floats)
    PATH_ENTRY("ViewportClipModified/Border", ViewportClipModified_Border, func_Boolean)
    PATH_ENTRY("ViewportClipModified/BorderColor", ViewportClipModified_BorderColor, func_UInt8RGB)
    PATH_ENTRY("ViewportClipModified/BorderWidth", ViewportClipModified_BorderWidth, func_Int32)
    PATH_ENTRY("ViewportClipModified/NBorderCounts", ViewportClipModified_NBorderCounts, func_UInt32)
    PATH_ENTRY("ViewportClipModified/NBorderPoints", ViewportClipModified_NBorderPoints, func_UInt32)
    PATH_ENTRY("ViewportClipModified/BorderCounts", ViewportClipModified_BorderCounts, func_Level)
    PATH_ENTRY("ViewportClipModified/BorderCounts/Count", ViewportClipModified_BorderCounts_Count, func_Int32)
    PATH_ENTRY("ViewportClipModified/BorderPoints", ViewportClipModified_BorderPoints, func_Level)
    PATH_ENTRY("ViewportClipModified/BorderPoints/Point", ViewportClipModified_BorderPoints_Point, func_Floats)

  PATH_ENTRY("ViewportFogChanged", ViewportFogChanged, func_Level)
    PATH_ENTRY("ViewportFogChanged/ViewportID", ViewportFogChanged_ViewportID, func_Id)
    PATH_ENTRY("ViewportFogChanged/Enable", ViewportFogChanged_Enable, func_Boolean)
    PATH_ENTRY("ViewportFogChanged/Background", ViewportFogChanged_Background, func_Boolean)
    PATH_ENTRY("ViewportFogChanged/Color", ViewportFogChanged_Color, func_UInt8RGB)
    PATH_ENTRY("ViewportFogChanged/Distance", ViewportFogChanged_Distance, func_Doubles)
    PATH_ENTRY("ViewportFogChanged/Density", ViewportFogChanged_Density, func_Doubles)

  PATH_ENTRY("ViewportSectioningChanged", ViewportSectioningChanged, func_Level)
    PATH_ENTRY("ViewportSectioningChanged/ViewportID", ViewportSectioningChanged_ViewportID, func_Id)
    PATH_ENTRY("ViewportSectioningChanged/NPlanes", ViewportSectioningChanged_NPlanes, func_UInt32)
      PATH_ENTRY("ViewportSectioningChanged/PlanesList", ViewportSectioningChanged_PlanesList, func_Level)
        PATH_ENTRY("ViewportSectioningChanged/PlanesList/Plane", ViewportSectioningChanged_PlanesList_Plane, func_Level)
          PATH_ENTRY("ViewportSectioningChanged/PlanesList/Plane/Row0", ViewportSectioningChanged_PlanesList_Plane_Row0, func_Floats)
          PATH_ENTRY("ViewportSectioningChanged/PlanesList/Plane/Row1", ViewportSectioningChanged_PlanesList_Plane_Row1, func_Floats)
          PATH_ENTRY("ViewportSectioningChanged/PlanesList/Plane/Row2", ViewportSectioningChanged_PlanesList_Plane_Row2, func_Floats)
    PATH_ENTRY("ViewportSectioningChanged/EnableFilling", ViewportSectioningChanged_EnableFilling, func_Boolean)
    PATH_ENTRY("ViewportSectioningChanged/Color", ViewportSectioningChanged_Color, func_UInt8RGB)
    PATH_ENTRY("ViewportSectioningChanged/EnableStippling", ViewportSectioningChanged_EnableStippling, func_Boolean)
    PATH_ENTRY("ViewportSectioningChanged/StipplePattern", ViewportSectioningChanged_StipplePattern, func_UInt8)
    PATH_ENTRY("ViewportSectioningChanged/StipplingColor", ViewportSectioningChanged_StipplingColor, func_UInt8RGB)

  PATH_ENTRY("ViewportAmbientLightChanged", ViewportAmbientLightChanged, func_Level)
    PATH_ENTRY("ViewportAmbientLightChanged/ViewportID", ViewportAmbientLightChanged_ViewportID, func_Id)
    PATH_ENTRY("ViewportAmbientLightChanged/AmbientLight", ViewportAmbientLightChanged_AmbientLight, func_Floats)

  PATH_ENTRY("ViewportDefaultLightsChanged", ViewportDefaultLightsChanged, func_Level)
    PATH_ENTRY("ViewportDefaultLightsChanged/ViewportID", ViewportDefaultLightsChanged_ViewportID, func_Id)
    PATH_ENTRY("ViewportDefaultLightsChanged/DefaultLightsOn", ViewportDefaultLightsChanged_DefaultLightsOn, func_Boolean)
    PATH_ENTRY("ViewportDefaultLightsChanged/NLights", ViewportDefaultLightsChanged_NLights, func_UInt32)
      PATH_ENTRY("ViewportDefaultLightsChanged/LightsList", ViewportDefaultLightsChanged_LightsList, func_Level)
        PATH_ENTRY("ViewportDefaultLightsChanged/LightsList/Light", ViewportDefaultLightsChanged_LightsList_Light, func_Level)
#define PATH_LIGHT_DEFINITION_ENTRIES(pathPrefixStr, pathPrefixKey) \
          PATH_ENTRY(pathPrefixStr "/Type", pathPrefixKey##_Type, func_Int32) \
          PATH_ENTRY(pathPrefixStr "/On", pathPrefixKey##_On, func_Boolean) \
          PATH_ENTRY(pathPrefixStr "/Position", pathPrefixKey##_Position, func_Doubles) \
          PATH_ENTRY(pathPrefixStr "/Direction", pathPrefixKey##_Direction, func_Doubles) \
          PATH_ENTRY(pathPrefixStr "/Color", pathPrefixKey##_Color, func_Floats) \
          PATH_ENTRY(pathPrefixStr "/ConstantAttenuation", pathPrefixKey##_ConstantAttenuation, func_Float) \
          PATH_ENTRY(pathPrefixStr "/LinearAttenuation", pathPrefixKey##_LinearAttenuation, func_Float) \
          PATH_ENTRY(pathPrefixStr "/QuadraticAttenuation", pathPrefixKey##_QuadraticAttenuation, func_Float) \
          PATH_ENTRY(pathPrefixStr "/UseLimits", pathPrefixKey##_UseLimits, func_Boolean) \
          PATH_ENTRY(pathPrefixStr "/StartAttenuationLimit", pathPrefixKey##_StartAttenuationLimit, func_Float) \
          PATH_ENTRY(pathPrefixStr "/EndAttenuationLimit", pathPrefixKey##_EndAttenuationLimit, func_Float) \
          PATH_ENTRY(pathPrefixStr "/SpotCutoff", pathPrefixKey##_SpotCutoff, func_Float) \
          PATH_ENTRY(pathPrefixStr "/SpotExponent", pathPrefixKey##_SpotExponent, func_Float) \
          PATH_ENTRY(pathPrefixStr "/SpecularFactor", pathPrefixKey##_SpecularFactor, func_Float) \
          PATH_ENTRY(pathPrefixStr "/ShadowsEnabled", pathPrefixKey##_ShadowsEnabled, func_Boolean) \
          PATH_ENTRY(pathPrefixStr "/ShadowMapSize", pathPrefixKey##_ShadowMapSize, func_UInt16) \
          PATH_ENTRY(pathPrefixStr "/ShadowSoftness", pathPrefixKey##_ShadowSoftness, func_UInt16) \
          PATH_ENTRY(pathPrefixStr "/ShadowSamples", pathPrefixKey##_ShadowSamples, func_UInt16)
        PATH_LIGHT_DEFINITION_ENTRIES("ViewportDefaultLightsChanged/LightsList/Light", ViewportDefaultLightsChanged_LightsList_Light)

  PATH_ENTRY("ViewportLightAttached", ViewportLightAttached, func_Level)
    PATH_ENTRY("ViewportLightAttached/ViewportID", ViewportLightAttached_ViewportID, func_Id)
    PATH_ENTRY("ViewportLightAttached/LightID", ViewportLightAttached_LightID, func_Id)
    PATH_ENTRY("ViewportLightAttached/NOverrides", ViewportLightAttached_NOverrides, func_UInt32)
    PATH_LIGHT_DEFINITION_ENTRIES("ViewportLightAttached", ViewportLightAttached)

  PATH_ENTRY("ViewportLightDetached", ViewportLightDetached, func_Level)
    PATH_ENTRY("ViewportLightDetached/ViewportID", ViewportLightDetached_ViewportID, func_Id)
    PATH_ENTRY("ViewportLightDetached/LightID", ViewportLightDetached_LightID, func_Id)

  PATH_ENTRY("LightAdded", LightAdded, func_Level)
    PATH_ENTRY("LightAdded/LightID", LightAdded_LightID, func_Id)
    PATH_LIGHT_DEFINITION_ENTRIES("LightAdded", LightAdded)

  PATH_ENTRY("LightModified", LightModified, func_Level)
    PATH_ENTRY("LightModified/LightID", LightModified_LightID, func_Id)
    PATH_ENTRY("LightModified/NModified", LightModified_NModified, func_UInt32)
    PATH_LIGHT_DEFINITION_ENTRIES("LightModified", LightModified)
#undef PATH_LIGHT_DEFINITION_ENTRIES

  PATH_ENTRY("LightDeleted", LightDeleted, func_Level)
    PATH_ENTRY("LightDeleted/LightID", LightDeleted_LightID, func_Id)

  PATH_ENTRY("OverlayViewParamsOverride", OverlayViewParamsOverride, func_Level)
    PATH_ENTRY("OverlayViewParamsOverride/ViewportID", OverlayViewParamsOverride_ViewportID, func_Id)
    PATH_ENTRY("OverlayViewParamsOverride/OverlayID", OverlayViewParamsOverride_OverlayID, func_Id)
    PATH_ENTRY("OverlayViewParamsOverride/Override", OverlayViewParamsOverride_Override, func_Boolean)
    PATH_ENTRY("OverlayViewParamsOverride/ViewParams", OverlayViewParamsOverride_ViewParams, func_Level)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/ScreenMatrix", OverlayViewParamsOverride_ViewParams_ScreenMatrix, func_Matrix)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/ViewingMatrix", OverlayViewParamsOverride_ViewParams_ViewingMatrix, func_Matrix)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/ProjectionMatrix", OverlayViewParamsOverride_ViewParams_ProjectionMatrix, func_Matrix)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/CorrectionMatrix", OverlayViewParamsOverride_ViewParams_CorrectionMatrix, func_Matrix)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/OutputMatrix", OverlayViewParamsOverride_ViewParams_OutputMatrix, func_Matrix)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/MetafileMatrix", OverlayViewParamsOverride_ViewParams_MetafileMatrix, func_Matrix)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/ViewPosition", OverlayViewParamsOverride_ViewParams_ViewPosition, func_Doubles)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/ViewBasisX", OverlayViewParamsOverride_ViewParams_ViewBasisX, func_Doubles)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/ViewBasisY", OverlayViewParamsOverride_ViewParams_ViewBasisY, func_Doubles)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/ViewBasisZ", OverlayViewParamsOverride_ViewParams_ViewBasisZ, func_Doubles)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/PerspectiveProjection", OverlayViewParamsOverride_ViewParams_PerspectiveProjection, func_Boolean)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/FieldWidth", OverlayViewParamsOverride_ViewParams_FieldWidth, func_Double)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/FieldHeight", OverlayViewParamsOverride_ViewParams_FieldHeight, func_Double)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/LensLength", OverlayViewParamsOverride_ViewParams_LensLength, func_Double)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/ViewTarget", OverlayViewParamsOverride_ViewParams_ViewTarget, func_Doubles)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/FrontClip", OverlayViewParamsOverride_ViewParams_FrontClip, func_Boolean)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/FrontClipDist", OverlayViewParamsOverride_ViewParams_FrontClipDist, func_Double)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/BackClip", OverlayViewParamsOverride_ViewParams_BackClip, func_Boolean)
      PATH_ENTRY("OverlayViewParamsOverride/ViewParams/BackClipDist", OverlayViewParamsOverride_ViewParams_BackClipDist, func_Double)

  PATH_ENTRY("VisualStyleAdded", VisualStyleAdded, func_Level)
    PATH_ENTRY("VisualStyleAdded/VisualStyleID", VisualStyleAdded_VisualStyleID, func_Id)
    PATH_ENTRY("VisualStyleAdded/NProps", VisualStyleAdded_NProps, func_UInt32)
    PATH_ENTRY("VisualStyleAdded/NPropsModified", VisualStyleAdded_NPropsModified, func_UInt32)
    PATH_ENTRY("VisualStyleAdded/Prop", VisualStyleAdded_Prop, func_Level)
    PATH_ENTRY("VisualStyleAdded/Prop/NProp", VisualStyleAdded_Prop_NProp, func_UInt32)
    PATH_ENTRY("VisualStyleAdded/Prop/PropType", VisualStyleAdded_Prop_PropType, func_UInt32)
    PATH_ENTRY("VisualStyleAdded/Prop/Set", VisualStyleAdded_Prop_Set, func_Boolean)
      PATH_ENTRY("VisualStyleAdded/Prop/BVal", VisualStyleAdded_Prop_BVal, func_Boolean)
      PATH_ENTRY("VisualStyleAdded/Prop/IVal", VisualStyleAdded_Prop_IVal, func_Int32)
      PATH_ENTRY("VisualStyleAdded/Prop/DVal", VisualStyleAdded_Prop_DVal, func_Double)
      PATH_ENTRY("VisualStyleAdded/Prop/CVal", VisualStyleAdded_Prop_CVal, func_UInt32)
      PATH_ENTRY("VisualStyleAdded/Prop/SVal", VisualStyleAdded_Prop_SVal, func_Chars)
  PATH_ENTRY("VisualStyleModified", VisualStyleModified, func_Level)
    PATH_ENTRY("VisualStyleModified/VisualStyleID", VisualStyleModified_VisualStyleID, func_Id)
    PATH_ENTRY("VisualStyleModified/NProps", VisualStyleModified_NProps, func_UInt32)
    PATH_ENTRY("VisualStyleModified/NPropsModified", VisualStyleModified_NPropsModified, func_UInt32)
    PATH_ENTRY("VisualStyleModified/Prop", VisualStyleModified_Prop, func_Level)
    PATH_ENTRY("VisualStyleModified/Prop/NProp", VisualStyleModified_Prop_NProp, func_UInt32)
    PATH_ENTRY("VisualStyleModified/Prop/PropType", VisualStyleModified_Prop_PropType, func_UInt32)
    PATH_ENTRY("VisualStyleModified/Prop/Set", VisualStyleModified_Prop_Set, func_Boolean)
      PATH_ENTRY("VisualStyleModified/Prop/BVal", VisualStyleModified_Prop_BVal, func_Boolean)
      PATH_ENTRY("VisualStyleModified/Prop/IVal", VisualStyleModified_Prop_IVal, func_Int32)
      PATH_ENTRY("VisualStyleModified/Prop/DVal", VisualStyleModified_Prop_DVal, func_Double)
      PATH_ENTRY("VisualStyleModified/Prop/CVal", VisualStyleModified_Prop_CVal, func_UInt32)
      PATH_ENTRY("VisualStyleModified/Prop/SVal", VisualStyleModified_Prop_SVal, func_Chars)
  PATH_ENTRY("VisualStyleDeleted", VisualStyleDeleted, func_Level)
    PATH_ENTRY("VisualStyleDeleted/VisualStyleID", VisualStyleDeleted_VisualStyleID, func_Id)

  PATH_ENTRY("LayerAdded", LayerAdded, func_Level)
    PATH_ENTRY("LayerAdded/LayerID", LayerAdded_LayerID, func_Id)
      PATH_ENTRY("LayerAdded/Visibility", LayerAdded_Visibility, func_Boolean)
      PATH_ENTRY("LayerAdded/Faded", LayerAdded_Faded, func_Boolean)
  PATH_ENTRY("LayerDeleted", LayerDeleted, func_Level)
    PATH_ENTRY("LayerDeleted/LayerID", LayerDeleted_LayerID, func_Id)

  PATH_ENTRY("LayerVisibilityChanged", LayerVisibilityChanged, func_Level)
    PATH_ENTRY("LayerVisibilityChanged/LayerID", LayerVisibilityChanged_LayerID, func_Id)
    PATH_ENTRY("LayerVisibilityChanged/Visibility", LayerVisibilityChanged_Visibility, func_Boolean)
  PATH_ENTRY("LayerFadingChanged", LayerFadingChanged, func_Level)
    PATH_ENTRY("LayerFadingChanged/LayerID", LayerFadingChanged_LayerID, func_Id)
    PATH_ENTRY("LayerFadingChanged/Fade", LayerFadingChanged_Fade, func_Boolean)

  PATH_ENTRY("LayerVpDepPropsOverride", LayerVpDepPropsOverride, func_Level)
    PATH_ENTRY("LayerVpDepPropsOverride/LayerID", LayerVpDepPropsOverride_LayerID, func_Id)
    PATH_ENTRY("LayerVpDepPropsOverride/ViewportID", LayerVpDepPropsOverride_ViewportID, func_Id)
    PATH_ENTRY("LayerVpDepPropsOverride/Override", LayerVpDepPropsOverride_Override, func_Boolean)
    PATH_ENTRY("LayerVpDepPropsOverride/VpDepSpec", LayerVpDepPropsOverride_VpDepSpec, func_Level)
    PATH_ENTRY("LayerVpDepPropsOverride/VpDepSpec/Visibility", LayerVpDepPropsOverride_VpDepSpec_Visibility, func_Boolean)
    PATH_ENTRY("LayerVpDepPropsOverride/VpDepSpec/Faded", LayerVpDepPropsOverride_VpDepSpec_Faded, func_Boolean)

  PATH_ENTRY("ViewportBackgroundChanged", ViewportBackgroundChanged, func_Level)
    PATH_ENTRY("ViewportBackgroundChanged/ViewportID", ViewportBackgroundChanged_ViewportID, func_Id)
    PATH_ENTRY("ViewportBackgroundChanged/BaseProgramID", ViewportBackgroundChanged_BaseProgramID, func_Id)
    PATH_ENTRY("ViewportBackgroundChanged/Visible", ViewportBackgroundChanged_Visible, func_Boolean)
      //parentsMetadata.push_back("ViewportBackgroundChanged")) //DropMetadata

    //parentsMetadata.push_back("MetafileAdded")) //DropMetadata
    //parentsMetadata.push_back("ViewportBackgroundChanged")) //DropMetadata
    //for (OdUInt32 idx = 0) idx < parentsMetadata.size()) idx++)
    //{
    //  OdAnsiString asParent = parentsMetadata[idx] + "/MetafileData") // "MetafileAdded", "ViewportBackgroundChanged"
    PATH_ENTRY_EX(, ,func_Level)
      PATH_ENTRY_EX("/NArrays", _NArrays, func_UInt32)
      PATH_ENTRY_EX("/Array", _Array, func_Level)
        PATH_ENTRY_EX("/Array/Type", _Array_Type, func_UInt32ArrayType)
        PATH_ENTRY_EX("/Array/NData", _Array_NData, func_UInt32)
        PATH_ENTRY_EX("/Array/ArrayData", _Array_ArrayData, func_Array)
        PATH_ENTRY_EX("/Array/ArrayOffset", _Array_ArrayOffset, func_Floats)
        PATH_ENTRY_EX("/Array/Data", _Array_Data, func_Level)
          PATH_ENTRY_EX("/Array/Data/Entry", _Array_Data_Entry, func_Level)
            PATH_ENTRY_EX("/Array/Data/Entry/Marker", _Array_Data_Entry_Marker, func_UInt64)
            PATH_ENTRY_EX("/Array/Data/Entry/From", _Array_Data_Entry_From, func_UInt32)
        //ParseMetafileStream(pMetafile))
      PATH_ENTRY_EX("/MetafileStream", _MetafileStream, func_Level)
         PATH_ENTRY_EX("/MetafileStream/EnableOpt", _MetafileStream_EnableOpt, func_Level)
           PATH_ENTRY_EX("/MetafileStream/EnableOpt/Mode", _MetafileStream_EnableOpt_Mode, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/DisableOpt", _MetafileStream_DisableOpt, func_Level)
           PATH_ENTRY_EX("/MetafileStream/DisableOpt/Mode", _MetafileStream_DisableOpt_Mode, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/Color", _MetafileStream_Color, func_Level)
           PATH_ENTRY_EX("/MetafileStream/Color/Color", _MetafileStream_Color_Color, func_UInt8RGB)
         PATH_ENTRY_EX("/MetafileStream/EnableArray", _MetafileStream_EnableArray, func_Level)
           PATH_ENTRY_EX("/MetafileStream/EnableArray/Type", _MetafileStream_EnableArray_Type, func_UInt8)
           PATH_ENTRY_EX("/MetafileStream/EnableArray/NArray", _MetafileStream_EnableArray_NArray, func_UInt32)
         PATH_ENTRY_EX("/MetafileStream/DisableArray", _MetafileStream_DisableArray, func_Level)
           PATH_ENTRY_EX("/MetafileStream/DisableArray/Type", _MetafileStream_DisableArray_Type, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/DrawArrays", _MetafileStream_DrawArrays, func_Level)
           PATH_ENTRY_EX("/MetafileStream/DrawArrays/Mode", _MetafileStream_DrawArrays_Mode, func_UInt8)
           PATH_ENTRY_EX("/MetafileStream/DrawArrays/First", _MetafileStream_DrawArrays_First, func_Int32)
           PATH_ENTRY_EX("/MetafileStream/DrawArrays/Count", _MetafileStream_DrawArrays_Count, func_Int32)
         PATH_ENTRY_EX("/MetafileStream/DrawElements", _MetafileStream_DrawElements, func_Level)
           PATH_ENTRY_EX("/MetafileStream/DrawElements/Mode", _MetafileStream_DrawElements_Mode, func_UInt8)
           PATH_ENTRY_EX("/MetafileStream/DrawElements/Count", _MetafileStream_DrawElements_Count, func_Int32)
           PATH_ENTRY_EX("/MetafileStream/DrawElements/NArray", _MetafileStream_DrawElements_NArray, func_UInt32)
         PATH_ENTRY_EX("/MetafileStream/CullFace", _MetafileStream_CullFace, func_Level)
           PATH_ENTRY_EX("/MetafileStream/CullFace/Mode", _MetafileStream_CullFace_Mode, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/LStipple", _MetafileStream_LStipple, func_Level)
           PATH_ENTRY_EX("/MetafileStream/LStipple/Pattern", _MetafileStream_LStipple_Pattern, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/PStipple", _MetafileStream_PStipple, func_Level)
           PATH_ENTRY_EX("/MetafileStream/PStipple/Pattern", _MetafileStream_PStipple_Pattern, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/VPoint", _MetafileStream_VPoint, func_Level)
           PATH_ENTRY_EX("/MetafileStream/VPoint/Point", _MetafileStream_VPoint_Point, func_Floats)
         PATH_ENTRY_EX("/MetafileStream/VLine", _MetafileStream_VLine, func_Level)
           PATH_ENTRY_EX("/MetafileStream/VLine/Point1", _MetafileStream_VLine_Point1, func_Floats)
           PATH_ENTRY_EX("/MetafileStream/VLine/Point2", _MetafileStream_VLine_Point2, func_Floats)
         PATH_ENTRY_EX("/MetafileStream/IPoint", _MetafileStream_IPoint, func_Level)
           PATH_ENTRY_EX("/MetafileStream/IPoint/Index", _MetafileStream_IPoint_Index, func_Int32)
         PATH_ENTRY_EX("/MetafileStream/ILine", _MetafileStream_ILine, func_Level)
           PATH_ENTRY_EX("/MetafileStream/ILine/Index1", _MetafileStream_ILine_Index1, func_Int32)
           PATH_ENTRY_EX("/MetafileStream/ILine/Index2", _MetafileStream_ILine_Index2, func_Int32)
         PATH_ENTRY_EX("/MetafileStream/HLRStencil", _MetafileStream_HLRStencil, func_Level)
           PATH_ENTRY_EX("/MetafileStream/HLRStencil/State", _MetafileStream_HLRStencil_State, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/EnableShading", _MetafileStream_EnableShading, func_Level)
           PATH_ENTRY_EX("/MetafileStream/EnableShading/Mode", _MetafileStream_EnableShading_Mode, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/DisableShading", _MetafileStream_DisableShading, func_Level)
           PATH_ENTRY_EX("/MetafileStream/DisableShading/Mode", _MetafileStream_DisableShading_Mode, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/Material", _MetafileStream_Material, func_Level)
           PATH_ENTRY_EX("/MetafileStream/Material/MaterialID", _MetafileStream_Material_MaterialID, func_Id)
         PATH_ENTRY_EX("/MetafileStream/InitTexture", _MetafileStream_InitTexture, func_Level)
           PATH_ENTRY_EX("/MetafileStream/InitTexture/TextureID", _MetafileStream_InitTexture_TextureID, func_Id)
           PATH_ENTRY_EX("/MetafileStream/InitTexture/Repeat", _MetafileStream_InitTexture_Repeat, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/UninitTexture", _MetafileStream_UninitTexture, func_Level)
               // No data in this entry
         PATH_ENTRY_EX("/MetafileStream/SelectionMarker", _MetafileStream_SelectionMarker, func_Level)
           PATH_ENTRY_EX("/MetafileStream/SelectionMarker/Marker", _MetafileStream_SelectionMarker_Marker, func_UInt64)
         PATH_ENTRY_EX("/MetafileStream/EnableMarkerArray", _MetafileStream_EnableMarkerArray, func_Level)
           PATH_ENTRY_EX("/MetafileStream/EnableMarkerArray/Type", _MetafileStream_EnableMarkerArray_Type, func_UInt8)
           PATH_ENTRY_EX("/MetafileStream/EnableMarkerArray/NArray", _MetafileStream_EnableMarkerArray_NArray, func_UInt32)
         PATH_ENTRY_EX("/MetafileStream/DisableMarkerArray", _MetafileStream_DisableMarkerArray, func_Level)
           PATH_ENTRY_EX("/MetafileStream/DisableMarkerArray/Type", _MetafileStream_DisableMarkerArray_Type, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/VisibilityFlags", _MetafileStream_VisibilityFlags, func_Level)
           PATH_ENTRY_EX("/MetafileStream/VisibilityFlags/Flags", _MetafileStream_VisibilityFlags_Flags, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/Lineweight", _MetafileStream_Lineweight, func_Level)
           PATH_ENTRY_EX("/MetafileStream/Lineweight/Type", _MetafileStream_Lineweight_Type, func_UInt8)
           PATH_ENTRY_EX("/MetafileStream/Lineweight/Ps", _MetafileStream_Lineweight_Ps, func_Double)
           PATH_ENTRY_EX("/MetafileStream/Lineweight/Lw", _MetafileStream_Lineweight_Lw, func_Int16)
         PATH_ENTRY_EX("/MetafileStream/Program", _MetafileStream_Program, func_Level)
           PATH_ENTRY_EX("/MetafileStream/Program/ProgramID", _MetafileStream_Program_ProgramID, func_Id)
         PATH_ENTRY_EX("/MetafileStream/TtfText", _MetafileStream_TtfText, func_Level)
           PATH_ENTRY_EX("/MetafileStream/TtfText/Transform", _MetafileStream_TtfText_Transform, func_Matrix)
           PATH_ENTRY_EX("/MetafileStream/TtfText/Direction", _MetafileStream_TtfText_Direction, func_Doubles)
           PATH_ENTRY_EX("/MetafileStream/TtfText/NText", _MetafileStream_TtfText_NText, func_UInt32)
           PATH_ENTRY_EX("/MetafileStream/TtfText/Text", _MetafileStream_TtfText_Text, func_Level)
             PATH_ENTRY_EX("/MetafileStream/TtfText/Text/Char", _MetafileStream_TtfText_Text_Char, func_Level)
               PATH_ENTRY_EX("/MetafileStream/TtfText/Text/Char/MetafileID", _MetafileStream_TtfText_Text_Char_MetafileID, func_Id)
               PATH_ENTRY_EX("/MetafileStream/TtfText/Text/Char/Multiplier", _MetafileStream_TtfText_Text_Char_Multiplier, func_Double)
         PATH_ENTRY_EX("/MetafileStream/PushMatrix", _MetafileStream_PushMatrix, func_Level)
           PATH_ENTRY_EX("/MetafileStream/PushMatrix/Identity", _MetafileStream_PushMatrix_Identity, func_UInt8)
           PATH_ENTRY_EX("/MetafileStream/PushMatrix/Matrix", _MetafileStream_PushMatrix_Matrix, func_Matrix)
         PATH_ENTRY_EX("/MetafileStream/PopMatrix", _MetafileStream_PopMatrix, func_Level)
             // No data in this entry
         PATH_ENTRY_EX("/MetafileStream/Metafile", _MetafileStream_Metafile, func_Level)
           PATH_ENTRY_EX("/MetafileStream/Metafile/MetafileID", _MetafileStream_Metafile_MetafileID, func_Id)
         PATH_ENTRY_EX("/MetafileStream/GeomMarker", _MetafileStream_GeomMarker, func_Level)
           PATH_ENTRY_EX("/MetafileStream/GeomMarker/Type", _MetafileStream_GeomMarker_Type, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/MetafileMarker", _MetafileStream_MetafileMarker, func_Level)
           PATH_ENTRY_EX("/MetafileStream/MetafileMarker/Marker", _MetafileStream_MetafileMarker_Marker, func_Id)
      //SEA missing MetafileStreamRecords
         PATH_ENTRY_EX("/MetafileStream/UserEntry", _MetafileStream_UserEntry, func_Level)
           PATH_ENTRY_EX("/MetafileStream/UserEntry/NumData", _MetafileStream_UserEntry_NumData, func_UInt32)
           PATH_ENTRY_EX("/MetafileStream/UserEntry/Data", _MetafileStream_UserEntry_Data, func_BinaryStream)
         PATH_ENTRY_EX("/MetafileStream/Linestyle", _MetafileStream_Linestyle, func_Level)
           PATH_ENTRY_EX("/MetafileStream/Linestyle/Default", _MetafileStream_Linestyle_Default, func_Boolean)
           PATH_ENTRY_EX("/MetafileStream/Linestyle/CapsType", _MetafileStream_Linestyle_CapsType, func_UInt8)
           PATH_ENTRY_EX("/MetafileStream/Linestyle/JoinType", _MetafileStream_Linestyle_JoinType, func_UInt8)
         PATH_ENTRY_EX("/MetafileStream/VisualStyle", _MetafileStream_VisualStyle, func_Level)
           PATH_ENTRY_EX("/MetafileStream/VisualStyle/VisualStyleID", _MetafileStream_VisualStyle_VisualStyleID, func_Id)
      // New records (since 2019)
         PATH_ENTRY_EX("/MetafileStream/ExtensionObject", _MetafileStream_ExtensionObject, func_Level)
           PATH_ENTRY_EX("/MetafileStream/ExtensionObject/ExtensionID", _MetafileStream_ExtensionObject_ExtensionID, func_Id)
      // New records (since 2021)
         PATH_ENTRY_EX("/MetafileStream/SelectionStyle", _MetafileStream_SelectionStyle, func_Level)
           PATH_ENTRY_EX("/MetafileStream/SelectionStyle/Index", _MetafileStream_SelectionStyle_Index, func_UInt32)

  PATH_ENTRY("MetafileDeleted", MetafileDeleted, func_Level)
    PATH_ENTRY("MetafileDeleted/MetafileID", MetafileDeleted_MetafileID, func_Id)

  PATH_ENTRY("StateBranchAdded", StateBranchAdded, func_Level)
    PATH_ENTRY("StateBranchAdded/StateBranchID", StateBranchAdded_StateBranchID, func_Id)
    PATH_ENTRY("StateBranchAdded/NChilds", StateBranchAdded_NChilds, func_UInt64)
    PATH_ENTRY("StateBranchAdded/NMarkers", StateBranchAdded_NMarkers, func_UInt64)
    PATH_ENTRY("StateBranchAdded/Childs", StateBranchAdded_Childs, func_Level)
      PATH_ENTRY("StateBranchAdded/Childs/StateBranchID", StateBranchAdded_Childs_StateBranchID, func_Id)
    PATH_ENTRY("StateBranchAdded/Markers", StateBranchAdded_Markers, func_Level)
      PATH_ENTRY("StateBranchAdded/Markers/Marker", StateBranchAdded_Markers_Marker, func_UInt64)

  PATH_ENTRY("StateBranchModified", StateBranchModified, func_Level)
    PATH_ENTRY("StateBranchModified/StateBranchID", StateBranchModified_StateBranchID, func_Id)
    PATH_ENTRY("StateBranchModified/NChilds", StateBranchModified_NChilds, func_UInt64)
    PATH_ENTRY("StateBranchModified/NMarkers", StateBranchModified_NMarkers, func_UInt64)
    PATH_ENTRY("StateBranchModified/Childs", StateBranchModified_Childs, func_Level)
      PATH_ENTRY("StateBranchModified/Childs/StateBranchID", StateBranchModified_Childs_StateBranchID, func_Id)
    PATH_ENTRY("StateBranchModified/Markers", StateBranchModified_Markers, func_Level)
      PATH_ENTRY("StateBranchModified/Markers/Marker", StateBranchModified_Markers_Marker, func_UInt64)

  PATH_ENTRY("StateBranchDeleted", StateBranchDeleted, func_Level)
    PATH_ENTRY("StateBranchDeleted/StateBranchID", StateBranchDeleted_StateBranchID, func_Id)

  PATH_ENTRY("GroupAdded", GroupAdded, func_Level)
    PATH_ENTRY("GroupAdded/GroupID", GroupAdded_GroupID, func_Id)

  PATH_ENTRY("GroupDeleted", GroupDeleted, func_Level)
    PATH_ENTRY("GroupDeleted/GroupID", GroupDeleted_GroupID, func_Id)

  PATH_ENTRY("GroupMetafilesAttached", GroupMetafilesAttached, func_Level)
    PATH_ENTRY("GroupMetafilesAttached/GroupID", GroupMetafilesAttached_GroupID, func_Id)
    PATH_ENTRY("GroupMetafilesAttached/NMetafiles", GroupMetafilesAttached_NMetafiles, func_UInt32)
    PATH_ENTRY("GroupMetafilesAttached/Metafiles", GroupMetafilesAttached_Metafiles, func_Level)
      PATH_ENTRY("GroupMetafilesAttached/Metafiles/MetafileID", GroupMetafilesAttached_Metafiles_MetafileID, func_Id)

  PATH_ENTRY("GroupMetafilesDetached", GroupMetafilesDetached, func_Level)
    PATH_ENTRY("GroupMetafilesDetached/GroupID", GroupMetafilesDetached_GroupID, func_Id)
    PATH_ENTRY("GroupMetafilesDetached/NMetafiles", GroupMetafilesDetached_NMetafiles, func_UInt32)
    PATH_ENTRY("GroupMetafilesDetached/Metafiles", GroupMetafilesDetached_Metafiles, func_Level)
      PATH_ENTRY("GroupMetafilesDetached/Metafiles/MetafileID", GroupMetafilesDetached_Metafiles_MetafileID, func_Id)

  //PATH_ENTRY("GroupMetafilesDeleted", GroupMetafilesDeleted, func_Level)
  //PATH_ENTRY("GroupMetafilesDeleted/GroupID", GroupMetafilesDeleted_GroupID, func_Id)

  PATH_ENTRY("GroupMetafilesVisibilityChanged", GroupMetafilesVisibilityChanged, func_Level)
    PATH_ENTRY("GroupMetafilesVisibilityChanged/GroupID", GroupMetafilesVisibilityChanged_GroupID, func_Id)
    PATH_ENTRY("GroupMetafilesVisibilityChanged/Visibility", GroupMetafilesVisibilityChanged_Visibility, func_Boolean)

  PATH_ENTRY("GroupMetafilesHighlightingChanged", GroupMetafilesHighlightingChanged, func_Level)
    PATH_ENTRY("GroupMetafilesHighlightingChanged/GroupID", GroupMetafilesHighlightingChanged_GroupID, func_Id)
    PATH_ENTRY("GroupMetafilesHighlightingChanged/Highlight", GroupMetafilesHighlightingChanged_Highlight, func_Boolean)

  PATH_ENTRY("GroupMetafilesFadingChanged", GroupMetafilesFadingChanged, func_Level)
    PATH_ENTRY("GroupMetafilesFadingChanged/GroupID", GroupMetafilesFadingChanged_GroupID, func_Id)
    PATH_ENTRY("GroupMetafilesFadingChanged/Fade", GroupMetafilesFadingChanged_Fade, func_Boolean)

  PATH_ENTRY("ExtensionObjectAdded", ExtensionObjectAdded, func_Level)
    PATH_ENTRY("ExtensionObjectAdded/ExtensionID", ExtensionObjectAdded_ExtensionID, func_Id)
    PATH_ENTRY("ExtensionObjectAdded/ModuleName", ExtensionObjectAdded_ModuleName, func_Chars)
    PATH_ENTRY("ExtensionObjectAdded/ExtVersion", ExtensionObjectAdded_ExtVersion, func_UInt32)
    PATH_ENTRY("ExtensionObjectAdded/ObjectType", ExtensionObjectAdded_ObjectType, func_UInt32)
    PATH_ENTRY("ExtensionObjectAdded/ObjectData", ExtensionObjectAdded_ObjectData, func_BinaryStream)

  PATH_ENTRY("ExtensionObjectDeleted", ExtensionObjectDeleted, func_Level)
    PATH_ENTRY("ExtensionObjectDeleted/ExtensionID", ExtensionObjectDeleted_ExtensionID, func_Id)

  PATH_ENTRY("ExtensionObjectVisibilityChanged", ExtensionObjectVisibilityChanged, func_Level)
    PATH_ENTRY("ExtensionObjectVisibilityChanged/ExtensionID", ExtensionObjectVisibilityChanged_ExtensionID, func_Id)
    PATH_ENTRY("ExtensionObjectVisibilityChanged/Visibility", ExtensionObjectVisibilityChanged_Visibility, func_Boolean)

#define PATH_PROPERTY_DETACHED_ENTRY(path) \
  PATH_ENTRY(#path "/PropType", path##_PropType, func_Int32)
#define PATH_PROPERTY_ATTACHED_ENTRY(path) \
  PATH_PROPERTY_DETACHED_ENTRY(path) \
  PATH_ENTRY(#path "/PropVal", path##_PropVal, func_Level) \
    PATH_ENTRY(#path "/PropVal/OwnerID", path##_PropVal_OwnerID, func_DbOwning) \
    PATH_ENTRY(#path "/PropVal/Matrix", path##_PropVal_Matrix, func_Matrix) \
    PATH_ENTRY(#path "/PropVal/LayerID", path##_PropVal_LayerID, func_Id) \
    PATH_ENTRY(#path "/PropVal/StateBranchID", path##_PropVal_StateBranchID, func_Id) \
    PATH_ENTRY(#path "/PropVal/VisualStyleID", path##_PropVal_VisualStyleID, func_Id) \
    PATH_ENTRY(#path "/PropVal/StyleIndex", path##_PropVal_StyleIndex, func_UInt32) \
    PATH_ENTRY(#path "/PropVal/Text", path##_PropVal_Text, func_Chars) \
    PATH_ENTRY(#path "/PropVal/Boolean", path##_PropVal_Boolean, func_Boolean)

  PATH_ENTRY("MetafilePropertyAttached", MetafilePropertyAttached, func_Level)
    PATH_ENTRY("MetafilePropertyAttached/MetafileID", MetafilePropertyAttached_MetafileID, func_Id)
    PATH_PROPERTY_ATTACHED_ENTRY(MetafilePropertyAttached)

  PATH_ENTRY("MetafilePropertyDetached", MetafilePropertyDetached, func_Level)
    PATH_ENTRY("MetafilePropertyDetached/MetafileID", MetafilePropertyDetached_MetafileID, func_Id)
    PATH_PROPERTY_DETACHED_ENTRY(MetafilePropertyDetached)

  PATH_ENTRY("GroupPropertyAttached", GroupPropertyAttached, func_Level)
    PATH_ENTRY("GroupPropertyAttached/GroupID", GroupPropertyAttached_GroupID, func_Id)
    PATH_PROPERTY_ATTACHED_ENTRY(GroupPropertyAttached)

  PATH_ENTRY("GroupPropertyDetached", GroupPropertyDetached, func_Level)
    PATH_ENTRY("GroupPropertyDetached/GroupID", GroupPropertyDetached_GroupID, func_Id)
    PATH_PROPERTY_DETACHED_ENTRY(GroupPropertyDetached)

  PATH_ENTRY("LayerPropertyAttached", LayerPropertyAttached, func_Level)
    PATH_ENTRY("LayerPropertyAttached/LayerID", LayerPropertyAttached_LayerID, func_Id)
    PATH_PROPERTY_ATTACHED_ENTRY(LayerPropertyAttached)

  PATH_ENTRY("LayerPropertyDetached", LayerPropertyDetached, func_Level)
    PATH_ENTRY("LayerPropertyDetached/LayerID", LayerPropertyDetached_LayerID, func_Id)
    PATH_PROPERTY_DETACHED_ENTRY(LayerPropertyDetached)

  PATH_ENTRY("VisualStylePropertyAttached", VisualStylePropertyAttached, func_Level)
    PATH_ENTRY("VisualStylePropertyAttached/VisualStyleID", VisualStylePropertyAttached_VisualStyleID, func_Id)
    PATH_PROPERTY_ATTACHED_ENTRY(VisualStylePropertyAttached)

  PATH_ENTRY("VisualStylePropertyDetached", VisualStylePropertyDetached, func_Level)
    PATH_ENTRY("VisualStylePropertyDetached/VisualStyleID", VisualStylePropertyDetached_VisualStyleID, func_Id)
    PATH_PROPERTY_DETACHED_ENTRY(VisualStylePropertyDetached)

  PATH_ENTRY("MaterialPropertyAttached", MaterialPropertyAttached, func_Level)
    PATH_ENTRY("MaterialPropertyAttached/MaterialID", MaterialPropertyAttached_MaterialID, func_Id)
    PATH_PROPERTY_ATTACHED_ENTRY(MaterialPropertyAttached)

  PATH_ENTRY("MaterialPropertyDetached", MaterialPropertyDetached, func_Level)
    PATH_ENTRY("MaterialPropertyDetached/MaterialID", MaterialPropertyDetached_MaterialID, func_Id)
    PATH_PROPERTY_DETACHED_ENTRY(MaterialPropertyDetached)

  PATH_ENTRY("TexturePropertyAttached", TexturePropertyAttached, func_Level)
    PATH_ENTRY("TexturePropertyAttached/TextureID", TexturePropertyAttached_TextureID, func_Id)
    PATH_PROPERTY_ATTACHED_ENTRY(TexturePropertyAttached)

  PATH_ENTRY("TexturePropertyDetached", TexturePropertyDetached, func_Level)
    PATH_ENTRY("TexturePropertyDetached/TextureID", TexturePropertyDetached_TextureID, func_Id)
    PATH_PROPERTY_DETACHED_ENTRY(TexturePropertyDetached)

  PATH_ENTRY("LightPropertyAttached", LightPropertyAttached, func_Level)
    PATH_ENTRY("LightPropertyAttached/LightID", LightPropertyAttached_LightID, func_Id)
    PATH_PROPERTY_ATTACHED_ENTRY(LightPropertyAttached)

  PATH_ENTRY("LightPropertyDetached", LightPropertyDetached, func_Level)
    PATH_ENTRY("LightPropertyDetached/LightID", LightPropertyDetached_LightID, func_Id)
    PATH_PROPERTY_DETACHED_ENTRY(LightPropertyDetached)

  PATH_ENTRY("StateBranchPropertyAttached", StateBranchPropertyAttached, func_Level)
    PATH_ENTRY("StateBranchPropertyAttached/StateBranchID", StateBranchPropertyAttached_StateBranchID, func_Id)
    PATH_PROPERTY_ATTACHED_ENTRY(StateBranchPropertyAttached)

  PATH_ENTRY("StateBranchPropertyDetached", StateBranchPropertyDetached, func_Level)
    PATH_ENTRY("StateBranchPropertyDetached/StateBranchID", StateBranchPropertyDetached_StateBranchID, func_Id)
    PATH_PROPERTY_DETACHED_ENTRY(StateBranchPropertyDetached)

  PATH_ENTRY("ViewportPropertyAttached", ViewportPropertyAttached, func_Level)
    PATH_ENTRY("ViewportPropertyAttached/ViewportID", ViewportPropertyAttached_ViewportID, func_Id)
    PATH_PROPERTY_ATTACHED_ENTRY(ViewportPropertyAttached)

  PATH_ENTRY("ViewportPropertyDetached", ViewportPropertyDetached, func_Level)
    PATH_ENTRY("ViewportPropertyDetached/ViewportID", ViewportPropertyDetached_ViewportID, func_Id)
    PATH_PROPERTY_DETACHED_ENTRY(ViewportPropertyDetached)

  PATH_ENTRY("OverlayPropertyAttached", OverlayPropertyAttached, func_Level)
    PATH_ENTRY("OverlayPropertyAttached/OverlayID", OverlayPropertyAttached_OverlayID, func_Id)
    PATH_PROPERTY_ATTACHED_ENTRY(OverlayPropertyAttached)

  PATH_ENTRY("OverlayPropertyDetached", OverlayPropertyDetached, func_Level)
    PATH_ENTRY("OverlayPropertyDetached/OverlayID", OverlayPropertyDetached_OverlayID, func_Id)
    PATH_PROPERTY_DETACHED_ENTRY(OverlayPropertyDetached)

  PATH_ENTRY("ExtensionObjectPropertyAttached", ExtensionObjectPropertyAttached, func_Level)
    PATH_ENTRY("ExtensionObjectPropertyAttached/ExtensionID", ExtensionObjectPropertyAttached_ExtensionID, func_Id)
    PATH_PROPERTY_ATTACHED_ENTRY(ExtensionObjectPropertyAttached)

  PATH_ENTRY("ExtensionObjectPropertyDetached", ExtensionObjectPropertyDetached, func_Level)
    PATH_ENTRY("ExtensionObjectPropertyDetached/ExtensionID", ExtensionObjectPropertyDetached_ExtensionID, func_Id)
    PATH_PROPERTY_DETACHED_ENTRY(ExtensionObjectPropertyDetached)

  PATH_ENTRY("ExtensionObjectSpecificData", ExtensionObjectSpecificData, func_Level)
    PATH_ENTRY("ExtensionObjectSpecificData/ExtensionID", ExtensionObjectSpecificData_ExtensionID, func_Id)
    PATH_ENTRY("ExtensionObjectSpecificData/DataID", ExtensionObjectSpecificData_DataID, func_UInt32)
    PATH_PROPERTY_ATTACHED_ENTRY(ExtensionObjectSpecificData)
#undef PATH_PROPERTY_DETACHED_ENTRY
#undef PATH_PROPERTY_ATTACHED_ENTRY

  PATH_ENTRY("FrameSwitchInfo", FrameSwitchInfo, func_Level)
    PATH_ENTRY("FrameSwitchInfo/Interval", FrameSwitchInfo_Interval, func_UInt32)

  // TODO

///////////////////////////////////////////////////////////////////////////////

#undef PATH_ENTRY_EX
#undef PATH_ENTRY
