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

CMD_DEF       ( Line                      , L"Draw")
CMD_DEF       ( Circle                    , L"Draw")
CMD_DEF       ( Text                      , L"Draw")

CMD_DEF       ( Box                       , L"Draw")
CMD_DEF       ( Sphere                    , L"Draw")
CMD_DEF       ( Array                     , L"Draw")
CMD_DEF       ( RadialDim                 , L"Draw")
CMD_DEF       ( DimArc                    , L"Draw")
CMD_DEF       ( Dim2LineAngular           , L"Draw")
CMD_DEF       ( DimDiametric              , L"Draw")
CMD_DEF       ( Dim3Point                 , L"Draw")

CMD_DEF       ( CenterMark                , L"Draw")
CMD_DEF       ( CenterLine                , L"Draw")
CMD_DEF       ( AssocRotatedAlignedDimension     , L"Draw")
CMD_DEF       ( AssocMLeader              , L"Draw")
CMD_DEF       ( Loft                      , L"Draw")
CMD_DEF       ( Extrude                   , L"Draw")

CMD_DEF       ( InsertRaster              , L"Draw")
CMD_DEF       ( MLeader                   , L"Draw")
CMD_DEF       ( DimAligned                , L"Draw")

CMD_DEF       ( Move                      , L"Modify")
CMD_DEF       ( Rotate                    , L"Modify")
CMD_DEF       ( Scale                     , L"Modify")
CMD_DEF       ( Mirror                    , L"Modify")
CMD_DEF       ( Join                      , L"Modify")
CMD_DEF       ( TxtExp                    , L"Modify")
CMD_DEF       ( ArrayEdit                 , L"Modify")
CMD_DEF       ( Reverse                   , L"Modify")
CMD_DEF       ( ConvertIdBlkRef           , L"Modify")

CMD_DEF       ( HideObjects               , L"Isolate")
CMD_DEF       ( IsolateObjects            , L"Isolate")
CMD_DEF       ( UnIsolateObjects          , L"Isolate")

CMD_DEF       ( DxfIn                     , L"File")
CMD_DEF       ( ExportLayout              , L"File")
CMD_DEF       ( BlockIcon                 , L"File")
CMD_DEF       ( DIGITALSIGN               , L"File")
CMD_DEF       ( SIGVALIDATE               , L"File")

CMD_DEF       ( Purge                     , L"Drawing Utilities")

CMD_DEF       ( GeoMarkPosition           , L"GeoMap")


#ifdef DO_UNDEF_CMD_DEF
#undef DO_UNDEF_CMD_DEF
#undef CMD_DEF
#endif

#ifdef DO_UNDEF_CMD_DEF_CLASS
#undef DO_UNDEF_CMD_DEF_CLASS
#undef CMD_DEF_CLASS
#endif
