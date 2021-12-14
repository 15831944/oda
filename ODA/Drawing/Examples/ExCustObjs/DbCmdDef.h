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
// Debug commands container
//

#ifndef CMD_DEF
#define CMD_DEF(a,b)
#define DO_UNDEF_CMD_DEF
#endif

#ifndef CMD_DEF_CLASS
#define CMD_DEF_CLASS(a,b) CMD_DEF(a,b)
#define DO_UNDEF_CMD_DEF_CLASS
#endif



//       CommandName   GroupName

CMD_DEF       ( deepclone                   , OD_T("DrxDebug clone tests"))
CMD_DEF       ( deepclone1                  , OD_T("DrxDebug clone tests"))
CMD_DEF       ( wblockclone                 , OD_T("DrxDebug clone tests"))
CMD_DEF       ( wblockCloneRecord           , OD_T("DrxDebug clone tests"))
CMD_DEF       ( wblock                      , OD_T("DrxDebug clone tests"))
CMD_DEF       ( proxyCloneTest              , OD_T("DrxDebug clone tests"))
CMD_DEF       ( wblockTest                  , OD_T("DrxDebug clone tests"))
CMD_DEF       ( xrefAttach                  , OD_T("Xref"))
CMD_DEF       ( XrefBind                    , OD_T("Xref"))
CMD_DEF       ( XrefDetach                  , OD_T("Xref"))
CMD_DEF       ( XrefStatus                  , OD_T("Xref"))
CMD_DEF       ( XrefUnloadAll               , OD_T("Xref"))
CMD_DEF       ( thawlayers                  , OD_T("DrxDebug"))
CMD_DEF       ( testRedirectedId            , OD_T("DrxDebug"))
CMD_DEF       ( testSymRecordUnErase        , OD_T("DrxDebug"))
CMD_DEF       ( testDbInsert                , OD_T("DrxDebug"))
CMD_DEF       ( testGroup                   , OD_T("DrxDebug"))
CMD_DEF       ( testTable                   , OD_T("DrxDebug"))
CMD_DEF       ( testFormatMeasurement       , OD_T("DrxDebug"))
CMD_DEF_CLASS ( setreactor                  , OD_T("DrxDebug"))
CMD_DEF       ( GetClassDictionary          , OD_T("DrxDebug"))
CMD_DEF       ( setBlockInsertUnits         , OD_T("DrxDebug"))
CMD_DEF       ( setLockPositionInBlock      , OD_T("DrxDebug"))
CMD_DEF       ( ListOpenDatabases           , OD_T("DrxDebug"))
CMD_DEF       ( dumpText                    , OD_T("DrxDebug"))
CMD_DEF       ( selectByLayer               , OD_T("DrxDebug"))
CMD_DEF       ( testBatchRead               , OD_T("DrxDebug"))
CMD_DEF       ( getFirstDeriv               , OD_T("Curve test"))
CMD_DEF       ( getPointAtParam             , OD_T("Curve test"))
CMD_DEF       ( getParamAtDist              , OD_T("Curve test"))
CMD_DEF       ( getDistAtParam              , OD_T("Curve test"))
CMD_DEF       ( getOffsetCurves             , OD_T("Curve test"))
CMD_DEF       ( getSpline                   , OD_T("Curve test"))
CMD_DEF       ( getArea                     , OD_T("Curve test"))
CMD_DEF       ( extend                      , OD_T("Curve test"))
CMD_DEF       ( getParamAtPoint             , OD_T("Curve test"))
CMD_DEF       ( reverseCurve                , OD_T("Curve test"))
CMD_DEF       ( STRETCH                     , OD_T("DrxDebug"))
CMD_DEF       ( ExCreateSphere              , OD_T("ODA Example Commands"))
CMD_DEF       ( ExCreateCustomObject        , OD_T("ODA Example Commands"))
CMD_DEF       ( ExCreateCustomEntity        , OD_T("ODA Example Commands"))
CMD_DEF       ( ExCreateHatch               , OD_T("ODA Example Commands"))
CMD_DEF       ( explodeGeometry             , OD_T("Explode"))
CMD_DEF       ( explodeGeometryToBlock      , OD_T("Explode"))
CMD_DEF       ( explode_test                , OD_T("Explode"))
CMD_DEF       ( explodeToBlock              , OD_T("Explode"))
CMD_DEF       ( PEDIT                       , OD_T("DrxDebug"))
CMD_DEF       ( reduce                      , OD_T("DrxDebug"))
CMD_DEF       ( SomePyramid                 , OD_T("3D create"))
CMD_DEF       ( SomeWedge                   , OD_T("3D create"))
CMD_DEF       ( CreateBreps                 , OD_T("3D create"))

CMD_DEF       ( AcisOutModelSpace           , OD_T("Sat tests"))
CMD_DEF       ( Set3dSolidsColor            , OD_T("Sat tests"))
CMD_DEF       ( CreateRegionsFromCurves     , OD_T("Sat tests"))
CMD_DEF       ( ExtrudeRegionsFromCurves    , OD_T("Sat tests"))
CMD_DEF       ( ExtrudeRegionsFromModelSpace, OD_T("Sat tests"))
CMD_DEF       ( ExtrudeSolid                , OD_T("Sat tests"))
CMD_DEF       ( RevolveRegionsFromModelSpace, OD_T("Sat tests"))
CMD_DEF       ( CheckInterference           , OD_T("Sat tests"))
CMD_DEF       ( getMassProp                 , OD_T("Sat tests"))
CMD_DEF       ( getAreaProp                 , OD_T("Sat tests"))
CMD_DEF				( ViewSection                 ,	OD_T("Sat tests"))
CMD_DEF       ( GetSection                  , OD_T("Sat tests"))
CMD_DEF       ( GetSlice                    , OD_T("Sat tests"))
CMD_DEF       ( GetSliceSurf                , OD_T("Sat tests"))
CMD_DEF       ( thicken                     , OD_T("Sat tests"))
CMD_DEF       ( convertToRegion             , OD_T("Sat tests"))
CMD_DEF       ( createFromCurves            , OD_T("Sat tests"))
CMD_DEF       ( createFrom                  , OD_T("Sat tests"))
CMD_DEF       ( SeparateBody                , OD_T("Sat tests"))
CMD_DEF       ( Sweep                       , OD_T("Surface solid tests"))
CMD_DEF       ( PipeDemo                    , OD_T("Surface solid tests"))
CMD_DEF       ( Revolve                     , OD_T("Surface solid tests"))
CMD_DEF       ( CreateLoftedSolid           , OD_T("Surface solid tests"))
CMD_DEF       ( CreateLoftSurface           , OD_T("Surface solid tests"))
CMD_DEF       ( intersectWith               , OD_T("Surface solid tests"))
CMD_DEF       ( CreateNurbSurface           , OD_T("Surface solid tests"))
CMD_DEF       ( BooleanKeyDemo              , OD_T("Surface solid tests"))
CMD_DEF       ( BooleanPETest               , OD_T("Surface solid tests"))
//CMD_DEF       ( BooleanPEAttach             , OD_T("Surface solid tests"))
//CMD_DEF       ( BooleanPEDetach             , OD_T("Surface solid tests"))
CMD_DEF       ( ExtrudeAlongPath            , OD_T("Surface solid tests"))
CMD_DEF       ( CreateSmoothPipeEx          , OD_T("Surface solid tests"))
CMD_DEF       ( CreateComlexSmoothPipeEx    , OD_T("Surface solid tests"))
CMD_DEF       ( CopySubent                  , OD_T("Subentity tests"))
CMD_DEF       ( ExtrudeFaces                , OD_T("Subentity tests"))
CMD_DEF       ( RemoveFaces                 , OD_T("Subentity tests"))
CMD_DEF       ( OffsetFaces                 , OD_T("Subentity tests"))
CMD_DEF       ( NumChanges                  , OD_T("Subentity tests"))
CMD_DEF       ( TransformFaces              , OD_T("Subentity tests"))
CMD_DEF       ( ShellBody                   , OD_T("Subentity tests"))
CMD_DEF       ( TaperFaces                  , OD_T("Subentity tests"))
CMD_DEF       ( SubentPaths_GsMarker        , OD_T("Subentity tests"))
CMD_DEF       ( SubentPaths_GsMarker_Block  , OD_T("Subentity tests"))
CMD_DEF       ( SetSubentColor              , OD_T("Subentity tests"))
CMD_DEF       ( SetSubentMaterial           , OD_T("Subentity tests"))
CMD_DEF       ( ChamferEdges                , OD_T("Subentity tests"))
CMD_DEF       ( addSection                  , OD_T("DrxDebug"))
CMD_DEF       ( toggleLiveSection           , OD_T("DrxDebug"))
CMD_DEF       ( renameLayout                , OD_T("DrxDebug"))
CMD_DEF       ( ForceErase                  , OD_T("DrxDebug"))
// Moved to ExCommands
//CMD_DEF       ( TransformByMirr             , OD_T("DrxDebug"))
//CMD_DEF       ( TransformCopyMirr           , OD_T("DrxDebug"))
CMD_DEF       ( ADJUSTTEXT                  , OD_T("DrxDebug"))
CMD_DEF       ( RECOMPUTETABLE              , OD_T("DrxDebug"))
CMD_DEF       ( CreateLinkedTable           , OD_T("DrxDebug"))
CMD_DEF       ( SubSelectTable              , OD_T("DrxDebug"))
CMD_DEF       ( ISEMR                       , OD_T("DrxDebug"))
CMD_DEF       ( VBAEXTRACT                  , OD_T("DrxDebug"))
CMD_DEF       ( copyFromPlotSettings        , OD_T("DrxDebug"))
CMD_DEF       ( TESTDIM                     , OD_T("DrxDebug"))
CMD_DEF       ( ERASE                       , OD_T("DrxDebug"))
CMD_DEF       ( DUMPMTEXTFRAGMENTS          , OD_T("DrxDebug"))
CMD_DEF       ( DIMINSPECT                  , OD_T("Dimensions"))
CMD_DEF       ( DIMJOGLINE                  , OD_T("Dimensions"))
CMD_DEF       ( DimsScale                   , OD_T("Dimensions"))
CMD_DEF       ( ADS_TEST                    , OD_T("DrxDebug"))
CMD_DEF       ( ANNOTATION_TEST             , OD_T("DrxDebug"))
CMD_DEF       ( InsertEx                    , OD_T("DrxDebug"))
CMD_DEF       ( UPDATEALLFIELDS             , OD_T("DrxDebug"))
CMD_DEF       ( REFEDITEX                   , OD_T("Long transactions"))
CMD_DEF       ( REFCLOSEEX                  , OD_T("Long transactions"))
CMD_DEF       ( REFSETEX                    , OD_T("Long transactions"))
CMD_DEF       ( REFEDIT_XREF_NESTED_BLOCK   , OD_T("Long transactions"))
CMD_DEF       ( REFEDIT_SAVE                , OD_T("Long transactions"))
CMD_DEF       ( NEWLAYOUT                   , OD_T("DrxDebug"))
CMD_DEF       ( SETLAYOUT                   , OD_T("DrxDebug"))
CMD_DEF       ( SelectionFilterTest         , OD_T("DrxDebug"))
CMD_DEF       ( MoveModelToPaperspace       , OD_T("DrxDebug"))
CMD_DEF       ( NumHatchLines               , OD_T("DrxDebug"))
CMD_DEF       ( ExClipTestCmd               , OD_T("DrxDebug"))
CMD_DEF       ( ExClipPlaneTestCmd          , OD_T("DrxDebug"))
CMD_DEF       ( ViewportsOnOff              , OD_T("Viewports"))
CMD_DEF       ( CreateViewport              , OD_T("Viewports"))
CMD_DEF       ( SetActiveViewport           , OD_T("Viewports"))
CMD_DEF       ( CVPORTTest                  , OD_T("Viewports"))
CMD_DEF       ( RayEmitter                  , OD_T("Viewports"))
CMD_DEF       ( DrawViewportFirst           , OD_T("Viewports"))
CMD_DEF       ( XClipVolumeIntersectionTest , OD_T("Viewports"))
CMD_DEF       ( PlineRayIntersect           , OD_T("Transient Manager"))
CMD_DEF       ( ClearTransients             , OD_T("Transient Manager"))
CMD_DEF       ( AddTransientMode            , OD_T("Transient Manager"))
CMD_DEF       ( EraseTransientMode          , OD_T("Transient Manager"))
CMD_DEF       ( FreeTransientMode           , OD_T("Transient Manager"))
CMD_DEF       ( UpdateTransientMode         , OD_T("Transient Manager"))
CMD_DEF       ( LinesByCurvePath            , OD_T("Curve test"))
CMD_DEF       ( MLineGetClosestPointTo      , OD_T("Curve test"))
CMD_DEF       ( MLineGetClosestPointToProj  , OD_T("Curve test"))
CMD_DEF       ( MLineAppendSeg              , OD_T("Curve test"))
CMD_DEF       ( MLineRemoveLastSeg          , OD_T("Curve test"))
CMD_DEF       ( MLineSetScale               , OD_T("Curve test"))
CMD_DEF       ( XrefExport                  , OD_T("DrxDebug"))
CMD_DEF       ( STLOut                      , OD_T("STL"))
#ifdef STLIMPORT_ENABLED
CMD_DEF       ( STLIn                       , OD_T("STL"))
#endif
CMD_DEF       ( getGeomExtents              , OD_T("DrxDebug"))
CMD_DEF       ( FlushGraphicsTest           , OD_T("DrxDebug"))
CMD_DEF       ( SORTENTS_TEST               , L"SORTENTS tests")
CMD_DEF       ( SORTENTS_TEST_Under         , L"SORTENTS tests")
CMD_DEF       ( SORTENTS_TEST_Above         , L"SORTENTS tests")
CMD_DEF       ( SORTENTS_TEST_Front         , L"SORTENTS tests")
CMD_DEF       ( SORTENTS_TEST_Back          , L"SORTENTS tests")
CMD_DEF       ( SORTENTS_TEST_MoveToBottom  , L"SORTENTS tests")
CMD_DEF       ( SORTENTS_TEST_MoveToTop     , L"SORTENTS tests")
CMD_DEF       ( ExplodeToBlockTest          , L"DrxDebug clone tests")
CMD_DEF       ( EvaluateNetwork             , L"Constraints tests")
CMD_DEF       ( BEDIT                       , OD_T("DrxDebug"))
CMD_DEF       ( BCLOSE                      , OD_T("DrxDebug"))
CMD_DEF       ( CompoundIdTest              , OD_T("DrxDebug"))
CMD_DEF       ( CONVERTTOMTEXTATTRIBUTE     , OD_T("DrxDebug"))
CMD_DEF       ( AddCustomProperty           , OD_T("DrxDebug"))
CMD_DEF       ( TransactionPerformanceTest  , L"DrxDebug clone tests")
CMD_DEF       ( showPdfSnaps                , L"PdfUnderlay")
CMD_DEF       ( showPdfGrips                , L"PdfUnderlay")
CMD_DEF       ( InvertImageClipBoundary     , OD_T("DrxDebug"))
CMD_DEF       ( UnderlayClip                , OD_T("DrxDebug"))
CMD_DEF       ( BlockRefClip                , OD_T("DrxDebug"))
CMD_DEF       ( BlockRefHighlightTest       , OD_T("DrxDebug"))
CMD_DEF       ( ConvertAcAdParts            , L"Convert AcAdPart Proxies to 3dSolids")
CMD_DEF       ( AddMleader                  , L"DrxDebug")
CMD_DEF       ( SubDMeshCode                , L"SubDMesh commands")

CMD_DEF       ( DXF_IN_OUT                  , L"Unit tests")
CMD_DEF       ( DWG_IN_OUT_UNDO             , L"Unit tests")
CMD_DEF       ( TRANSFORM_BY                , L"Unit tests")

CMD_DEF       ( ROUNDTRIP_DWG14             , L"Round-trip tests")
CMD_DEF       ( ROUNDTRIP_DWG15             , L"Round-trip tests")
CMD_DEF       ( ROUNDTRIP_DWG2004           , L"Round-trip tests")
CMD_DEF       ( ROUNDTRIP_DWG2007           , L"Round-trip tests")
CMD_DEF       ( ROUNDTRIP_DWG2010           , L"Round-trip tests")
CMD_DEF       ( ROUNDTRIP_DWG2013           , L"Round-trip tests")

CMD_DEF       ( ROUNDTRIP_DXF14             , L"Round-trip tests")
CMD_DEF       ( ROUNDTRIP_DXF15             , L"Round-trip tests")
CMD_DEF       ( ROUNDTRIP_DXF2004           , L"Round-trip tests")
CMD_DEF       ( ROUNDTRIP_DXF2007           , L"Round-trip tests")
CMD_DEF       ( ROUNDTRIP_DXF2010           , L"Round-trip tests")
CMD_DEF       ( ROUNDTRIP_DXF2013           , L"Round-trip tests")

CMD_DEF       ( ArrayExplode                , L"AssocArray")
CMD_DEF       ( EvaluateTopLevelNetwork     , L"AssocArray")
CMD_DEF       ( setBasePlane                , L"AssocArray")
CMD_DEF       ( setBasePoint                , L"AssocArray")

//CMD_DEF       ( ExplodeCenterMark			, L"CenterMark")
CMD_DEF       ( EvaluateOverrideCenterMark	, L"CenterMark")
CMD_DEF       ( LeftLineOvershoot           , L"CenterMark")
CMD_DEF       ( RightLineOvershoot          , L"CenterMark")
CMD_DEF       ( TopLineOvershoot            , L"CenterMark")
CMD_DEF       ( BottomLineOvershoot         , L"CenterMark")
CMD_DEF       ( CrossHairGap                , L"CenterMark")
CMD_DEF       ( CrossHairLength             , L"CenterMark")
CMD_DEF       ( ExtentionLinesAreVisible    , L"CenterMark")
CMD_DEF		    ( SetRotationCenterMark		    , L"CenterMark")

CMD_DEF       (EvaluateOverrideCenterLine   , L"CenterLine")
CMD_DEF       (StartOvershoot               , L"CenterLine")
CMD_DEF       (EndOvershoot                 , L"CenterLine")

CMD_DEF       ( EmbeddedSolidWrapper        , L"ODA Example Commands")
CMD_DEF       ( PROXY_XTEST                 , L"DrxDebug clone tests")
CMD_DEF       ( PROXY_XTEST1,                 L"DrxDebug clone tests")
CMD_DEF       ( InsertClone,                  L"DrxDebug clone tests")
CMD_DEF       ( ANNOTATION_ADD              , L"DrxDebug")
CMD_DEF       ( ANNORESET,                    L"DrxDebug")
CMD_DEF       ( ATTSYNC,                      L"DrxDebug")
CMD_DEF       ( POLYDISPLAY,                  L"DrxDebug")
CMD_DEF       ( Props,                        L"DrxDebug")

CMD_DEF       ( Intersect,                    L"Entity Tests")

CMD_DEF       ( SaveStorage,                  L"OLE")
CMD_DEF       ( StepBack,                     L"Revisions")
CMD_DEF       ( Commit,                       L"Revisions")
CMD_DEF       ( CreateBranch,                 L"Revisions")
CMD_DEF       ( SwitchBranch,                 L"Revisions")
CMD_DEF       ( MergeBranch,                  L"Revisions")
CMD_DEF       ( ExplodeStorage,               L"Revisions")

CMD_DEF       ( OBJExport                   , OD_T("OBJExport"))
CMD_DEF       ( getRegionArea,                L"DrxDebug")
CMD_DEF       ( ListChilds,                   L"DrxDebug")


#ifdef DO_UNDEF_CMD_DEF
#undef DO_UNDEF_CMD_DEF
#undef CMD_DEF
#endif

#ifdef DO_UNDEF_CMD_DEF_CLASS
#undef DO_UNDEF_CMD_DEF_CLASS
#undef CMD_DEF_CLASS
#endif
