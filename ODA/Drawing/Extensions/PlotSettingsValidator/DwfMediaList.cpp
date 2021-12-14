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
#include "DwfMediaList.h"

OdResult getDWFMediaList(OdArray<OdDbPlotSettingsValidatorPE::psvPaperInfo> &pMediaList)
{
  struct Paper
  {
    const OdChar* _localeName;
    const OdChar* _canonicalName;
    double _w;
    double _h;
    OdDbPlotSettings::PlotPaperUnits _units;

    double _xminMargin;
    double _yminMargin;
    double _xmaxMargin;
    double _ymaxMargin;
  };

  static const Paper dwfPapers [] = 
  {
    { OD_T("ISO full bleed B5 (250.00 x 176.00 MM)")    , OD_T("ISO_full_bleed_B5_(250.00_x_176.00_MM)")    , 250        , 176         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed B5 (176.00 x 250.00 MM)")    , OD_T("ISO_full_bleed_B5_(176.00_x_250.00_MM)")    , 176        , 250         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed B4 (353.00 x 250.00 MM)")    , OD_T("ISO_full_bleed_B4_(353.00_x_250.00_MM)")    , 353        , 250         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed B4 (250.00 x 353.00 MM)")    , OD_T("ISO_full_bleed_B4_(250.00_x_353.00_MM)")    , 250        , 353         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed B3 (500.00 x 353.00 MM)")    , OD_T("ISO_full_bleed_B3_(500.00_x_353.00_MM)")    , 500        , 353         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed B3 (353.00 x 500.00 MM)")    , OD_T("ISO_full_bleed_B3_(353.00_x_500.00_MM)")    , 353        , 500         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed B2 (707.00 x 500.00 MM)")    , OD_T("ISO_full_bleed_B2_(707.00_x_500.00_MM)")    , 707        , 500         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed B2 (500.00 x 707.00 MM)")    , OD_T("ISO_full_bleed_B2_(500.00_x_707.00_MM)")    , 500        , 707         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed B1 (1000.00 x 707.00 MM)")   , OD_T("ISO_full_bleed_B1_(1000.00_x_707.00_MM)")   , 1000       , 707         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed B1 (707.00 x 1000.00 MM)")   , OD_T("ISO_full_bleed_B1_(707.00_x_1000.00_MM)")   , 707        , 1000        , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed B0 (1414.00 x 1000.00 MM)")  , OD_T("ISO_full_bleed_B0_(1414.00_x_1000.00_MM)")  , 1414       , 1000        , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed B0 (1000.00 x 1414.00 MM)")  , OD_T("ISO_full_bleed_B0_(1000.00_x_1414.00_MM)")  , 1000       , 1414        , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed A5 (210.00 x 148.00 MM)")    , OD_T("ISO_full_bleed_A5_(210.00_x_148.00_MM)")    , 210        , 148         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed A5 (148.00 x 210.00 MM)")    , OD_T("ISO_full_bleed_A5_(148.00_x_210.00_MM)")    , 148        , 210         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed 2A0 (1189.00 x 1682.00 MM)") , OD_T("ISO_full_bleed_2A0_(1189.00_x_1682.00_MM)") , 1189       , 1682        , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed 4A0 (1682.00 x 2378.00 MM)") , OD_T("ISO_full_bleed_4A0_(1682.00_x_2378.00_MM)") , 1682       , 2378        , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed A4 (297.00 x 210.00 MM)")    , OD_T("ISO_full_bleed_A4_(297.00_x_210.00_MM)")    , 297        , 210         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed A4 (210.00 x 297.00 MM)")    , OD_T("ISO_full_bleed_A4_(210.00_x_297.00_MM)")    , 210        , 297         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed A3 (420.00 x 297.00 MM)")    , OD_T("ISO_full_bleed_A3_(420.00_x_297.00_MM)")    , 420        , 297         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed A3 (297.00 x 420.00 MM)")    , OD_T("ISO_full_bleed_A3_(297.00_x_420.00_MM)")    , 297        , 420         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed A2 (594.00 x 420.00 MM)")    , OD_T("ISO_full_bleed_A2_(594.00_x_420.00_MM)")    , 594        , 420         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed A2 (420.00 x 594.00 MM)")    , OD_T("ISO_full_bleed_A2_(420.00_x_594.00_MM)")    , 420        , 594         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed A1 (841.00 x 594.00 MM)")    , OD_T("ISO_full_bleed_A1_(841.00_x_594.00_MM)")    , 841        , 594         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed A1 (594.00 x 841.00 MM)")    , OD_T("ISO_full_bleed_A1_(594.00_x_841.00_MM)")    , 594        , 841         , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO full bleed A0 (841.00 x 1189.00 MM)")   , OD_T("ISO_full_bleed_A0_(841.00_x_1189.00_MM)")   , 841        , 1189        , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ARCH full bleed E1 (30.00 x 42.00 Inches)") , OD_T("ARCH_full_bleed_E1_(30.00_x_42.00_Inches)") , 762.000000 , 1066.800049 , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ARCH full bleed E (36.00 x 48.00 Inches)")  , OD_T("ARCH_full_bleed_E_(36.00_x_48.00_Inches)")  , 914.400024 , 1219.199951 , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ARCH full bleed D (36.00 x 24.00 Inches)")  , OD_T("ARCH_full_bleed_D_(36.00_x_24.00_Inches)")  , 914.400024 , 609.599976  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ARCH full bleed D (24.00 x 36.00 Inches)")  , OD_T("ARCH_full_bleed_D_(24.00_x_36.00_Inches)")  , 609.599976 , 914.400024  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ARCH full bleed C (24.00 x 18.00 Inches)")  , OD_T("ARCH_full_bleed_C_(24.00_x_18.00_Inches)")  , 609.599976 , 457.200012  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ARCH full bleed C (18.00 x 24.00 Inches)")  , OD_T("ARCH_full_bleed_C_(18.00_x_24.00_Inches)")  , 457.200012 , 609.599976  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ARCH full bleed B (18.00 x 12.00 Inches)")  , OD_T("ARCH_full_bleed_B_(18.00_x_12.00_Inches)")  , 457.200012 , 304.799988  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ARCH full bleed B (12.00 x 18.00 Inches)")  , OD_T("ARCH_full_bleed_B_(12.00_x_18.00_Inches)")  , 304.799988 , 457.200012  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ARCH full bleed A (12.00 x 9.00 Inches)")   , OD_T("ARCH_full_bleed_A_(12.00_x_9.00_Inches)")   , 304.799988 , 228.600006  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ARCH full bleed A (9.00 x 12.00 Inches)")   , OD_T("ARCH_full_bleed_A_(9.00_x_12.00_Inches)")   , 228.600006 , 304.799988  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ANSI full bleed F (28.00 x 40.00 Inches)")  , OD_T("ANSI_full_bleed_F_(28.00_x_40.00_Inches)")  , 711.200012 , 1016.000000 , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ANSI full bleed E (34.00 x 44.00 Inches)")  , OD_T("ANSI_full_bleed_E_(34.00_x_44.00_Inches)")  , 863.599976 , 1117.599976 , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ANSI full bleed D (34.00 x 22.00 Inches)")  , OD_T("ANSI_full_bleed_D_(34.00_x_22.00_Inches)")  , 863.599976 , 558.799988  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ANSI full bleed D (22.00 x 34.00 Inches)")  , OD_T("ANSI_full_bleed_D_(22.00_x_34.00_Inches)")  , 558.799988 , 863.599976  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ANSI full bleed C (22.00 x 17.00 Inches)")  , OD_T("ANSI_full_bleed_C_(22.00_x_17.00_Inches)")  , 558.799988 , 431.799988  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ANSI full bleed C (17.00 x 22.00 Inches)")  , OD_T("ANSI_full_bleed_C_(17.00_x_22.00_Inches)")  , 431.799988 , 558.799988  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ANSI full bleed B (17.00 x 11.00 Inches)")  , OD_T("ANSI_full_bleed_B_(17.00_x_11.00_Inches)")  , 431.799988 , 279.399994  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ANSI full bleed B (11.00 x 17.00 Inches)")  , OD_T("ANSI_full_bleed_B_(11.00_x_17.00_Inches)")  , 279.399994 , 431.799988  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ANSI full bleed A (11.00 x 8.50 Inches)")   , OD_T("ANSI_full_bleed_A_(11.00_x_8.50_Inches)")   , 279.399994 , 215.899994  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ANSI full bleed A (8.50 x 11.00 Inches)")   , OD_T("ANSI_full_bleed_A_(8.50_x_11.00_Inches)")   , 215.899994 , 279.399994  , OdDbPlotSettings::kMillimeters , 0. , 0. , 0. , 0. } ,
    { OD_T("ISO expand A0 (841.00 x 1189.00 MM)")       , OD_T("ISO_expand_A0_(841.00_x_1189.00_MM)")       , 841        , 1189        , OdDbPlotSettings::kMillimeters , 5. , 10., 5. , 10. } ,
    { OD_T("ISO A0 (841.00 x 1189.00 MM)")              , OD_T("ISO_A0_(841.00_x_1189.00_MM)")              , 841        , 1189        , OdDbPlotSettings::kMillimeters , 5. , 17., 5. , 17. } ,
    { OD_T("ISO expand A1 (841.00 x 594.00 MM)")        , OD_T("ISO_expand_A1_(841.00_x_594.00_MM)")        , 841        , 594         , OdDbPlotSettings::kMillimeters , 5. , 10., 5. , 10. } ,
    { OD_T("ISO expand A1 (594.00 x 841.00 MM)")        , OD_T("ISO_expand_A1_(594.00_x_841.00_MM)")        , 594        , 841         , OdDbPlotSettings::kMillimeters , 5. , 10., 5. , 10. } ,
    { OD_T("ISO A1 (841.00 x 594.00 MM)")               , OD_T("ISO_A1_(841.00_x_594.00_MM)")               , 841        , 594         , OdDbPlotSettings::kMillimeters , 5. , 17., 5. , 17. } ,
    { OD_T("ISO A1 (594.00 x 841.00 MM)")               , OD_T("ISO_A1_(594.00_x_841.00_MM)")               , 594        , 841         , OdDbPlotSettings::kMillimeters , 5. , 17., 5. , 17. } ,
    { OD_T("ISO expand A2 (594.00 x 420.00 MM)")        , OD_T("ISO_expand_A2_(594.00_x_420.00_MM)")        , 594        , 420         , OdDbPlotSettings::kMillimeters , 5. , 10., 5. , 10. } ,
    { OD_T("ISO expand A2 (420.00 x 594.00 MM)")        , OD_T("ISO_expand_A2_(420.00_x_594.00_MM)")        , 420        , 594         , OdDbPlotSettings::kMillimeters , 5. , 10., 5. , 10. } ,
    { OD_T("ISO A2 (594.00 x 420.00 MM)")               , OD_T("ISO_A2_(594.00_x_420.00_MM)")               , 594        , 420         , OdDbPlotSettings::kMillimeters , 5. , 17., 5. , 17. } ,
    { OD_T("ISO A2 (420.00 x 594.00 MM)")               , OD_T("ISO_A2_(420.00_x_594.00_MM)")               , 420        , 594         , OdDbPlotSettings::kMillimeters , 5. , 17., 5. , 17. } ,
    { OD_T("ISO expand A3 (420.00 x 297.00 MM)")        , OD_T("ISO_expand_A3_(420.00_x_297.00_MM)")        , 420        , 297         , OdDbPlotSettings::kMillimeters , 5. , 10., 5. , 10. } ,
    { OD_T("ISO expand A3 (297.00 x 420.00 MM)")        , OD_T("ISO_expand_A3_(297.00_x_420.00_MM)")        , 297        , 420         , OdDbPlotSettings::kMillimeters , 5. , 10., 5. , 10. } ,
    { OD_T("ISO A3 (420.00 x 297.00 MM)")               , OD_T("ISO_A3_(420.00_x_297.00_MM)")               , 420        , 297         , OdDbPlotSettings::kMillimeters , 5. , 17., 5. , 17. } ,
    { OD_T("ISO A3 (297.00 x 420.00 MM)")               , OD_T("ISO_A3_(297.00_x_420.00_MM)")               , 297        , 420         , OdDbPlotSettings::kMillimeters , 5. , 17., 5. , 17. } ,
    { OD_T("ISO expand A4 (297.00 x 210.00 MM)")        , OD_T("ISO_expand_A4_(297.00_x_210.00_MM)")        , 297        , 210         , OdDbPlotSettings::kMillimeters , 5. , 10., 5. , 10. } ,
    { OD_T("ISO expand A4 (210.00 x 297.00 MM)")        , OD_T("ISO_expand_A4_(210.00_x_297.00_MM)")        , 210        , 297         , OdDbPlotSettings::kMillimeters , 5. , 10., 5. , 10. } ,
    { OD_T("ISO A4 (297.00 x 210.00 MM)")               , OD_T("ISO_A4_(297.00_x_210.00_MM)")               , 297        , 210         , OdDbPlotSettings::kMillimeters , 5. , 17., 5. , 17. } ,
    { OD_T("ISO A4 (210.00 x 297.00 MM)")               , OD_T("ISO_A4_(210.00_x_297.00_MM)")               , 210        , 297         , OdDbPlotSettings::kMillimeters , 5. , 17., 5. , 17. } ,
    { OD_T("ARCH expand E1 (30.00 x 42.00 Inches)")     , OD_T("ARCH_expand_E1_(30.00_x_42.00_Inches)")     , 762.000000 , 1066.800049 , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ARCH E1 (30.00 x 42.00 Inches)")            , OD_T("ARCH_E1_(30.00_x_42.00_Inches)")            , 762.000000 , 1066.800049 , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ARCH expand E (36.00 x 48.00 Inches)")      , OD_T("ARCH_expand_E_(36.00_x_48.00_Inches)")      , 914.400024 , 1219.199951 , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ARCH E (36.00 x 48.00 Inches)")             , OD_T("ARCH_E_(36.00_x_48.00_Inches)")             , 914.400024 , 1219.199951 , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ARCH expand D (36.00 x 24.00 Inches)")      , OD_T("ARCH_expand_D_(36.00_x_24.00_Inches)")      , 914.400024 , 609.599976  , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ARCH expand D (24.00 x 36.00 Inches)")      , OD_T("ARCH_expand_D_(24.00_x_36.00_Inches)")      , 609.599976 , 914.400024  , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ARCH D (36.00 x 24.00 Inches)")             , OD_T("ARCH_D_(36.00_x_24.00_Inches)")             , 914.400024 , 609.599976  , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ARCH D (24.00 x 36.00 Inches)")             , OD_T("ARCH_D_(24.00_x_36.00_Inches)")             , 609.599976 , 914.400024  , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ARCH expand C (24.00 x 18.00 Inches)")      , OD_T("ARCH_expand_C_(24.00_x_18.00_Inches)")      , 609.599976 , 457.200012  , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ARCH expand C (18.00 x 24.00 Inches)")      , OD_T("ARCH_expand_C_(18.00_x_24.00_Inches)")      , 457.200012 , 609.599976  , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ARCH C (24.00 x 18.00 Inches)")             , OD_T("ARCH_C_(24.00_x_18.00_Inches)")             , 609.599976 , 457.200012  , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ARCH C (18.00 x 24.00 Inches)")             , OD_T("ARCH_C_(18.00_x_24.00_Inches)")             , 457.200012 , 609.599976  , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ANSI expand E (34.00 x 44.00 Inches)")      , OD_T("ANSI_expand_E_(34.00_x_44.00_Inches)")      , 863.599976 , 1117.599976 , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ANSI E (34.00 x 44.00 Inches)")             , OD_T("ANSI_E_(34.00_x_44.00_Inches)")             , 863.599976 , 1117.599976 , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ANSI expand D (34.00 x 22.00 Inches)")      , OD_T("ANSI_expand_D_(34.00_x_22.00_Inches)")      , 863.599976 , 558.799988  , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ANSI expand D (22.00 x 34.00 Inches)")      , OD_T("ANSI_expand_D_(22.00_x_34.00_Inches)")      , 558.799988 , 863.599976  , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ANSI D (34.00 x 22.00 Inches)")             , OD_T("ANSI_D_(34.00_x_22.00_Inches)")             , 863.599976 , 558.799988  , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ANSI D (22.00 x 34.00 Inches)")             , OD_T("ANSI_D_(22.00_x_34.00_Inches)")             , 558.799988 , 863.599976  , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ANSI expand C (22.00 x 17.00 Inches)")      , OD_T("ANSI_expand_C_(22.00_x_17.00_Inches)")      , 558.799988 , 431.799988  , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ANSI expand C (17.00 x 22.00 Inches)")      , OD_T("ANSI_expand_C_(17.00_x_22.00_Inches)")      , 431.799988 , 558.799988  , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ANSI C (22.00 x 17.00 Inches)")             , OD_T("ANSI_C_(22.00_x_17.00_Inches)")             , 558.799988 , 431.799988  , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ANSI C (17.00 x 22.00 Inches)")             , OD_T("ANSI_C_(17.00_x_22.00_Inches)")             , 431.799988 , 558.799988  , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ANSI expand B (17.00 x 11.00 Inches)")      , OD_T("ANSI_expand_B_(17.00_x_11.00_Inches)")      , 431.799988 , 279.399994  , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ANSI expand B (11.00 x 17.00 Inches)")      , OD_T("ANSI_expand_B_(11.00_x_17.00_Inches)")      , 279.399994 , 431.799988  , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ANSI B (17.00 x 11.00 Inches)")             , OD_T("ANSI_B_(17.00_x_11.00_Inches)")             , 431.799988 , 279.399994  , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ANSI B (11.00 x 17.00 Inches)")             , OD_T("ANSI_B_(11.00_x_17.00_Inches)")             , 279.399994 , 431.799988  , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ANSI expand A (11.00 x 8.50 Inches)")       , OD_T("ANSI_expand_A_(11.00_x_8.50_Inches)")       , 279.399994 , 215.899994  , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ANSI expand A (8.50 x 11.00 Inches)")       , OD_T("ANSI_expand_A_(8.50_x_11.00_Inches)")       , 215.899994 , 279.399994  , OdDbPlotSettings::kMillimeters , 5. , 10. , 5. , 10. } ,
    { OD_T("ANSI A (11.00 x 8.50 Inches)")              , OD_T("ANSI_A_(11.00_x_8.50_Inches)")              , 279.399994 , 215.899994  , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
    { OD_T("ANSI A (8.50 x 11.00 Inches)")              , OD_T("ANSI_A_(8.50_x_11.00_Inches)")              , 215.899994 , 279.399994  , OdDbPlotSettings::kMillimeters , 5. , 17. , 5. , 17. } ,
  };

  OdUInt32 numPapers = sizeof(dwfPapers)/sizeof(dwfPapers[0]);
  OdUInt32 f = pMediaList.size();
  pMediaList.resize(f + numPapers);
  for(OdUInt32 i=0; i<numPapers; ++i,++f)
  {
    pMediaList[f].canonicalName = dwfPapers[i]._canonicalName;
    pMediaList[f].localeName    = dwfPapers[i]._localeName;
    pMediaList[f].w             = dwfPapers[i]._w;
    pMediaList[f].h             = dwfPapers[i]._h;
    pMediaList[f].left          = dwfPapers[i]._xminMargin;
    pMediaList[f].top           = dwfPapers[i]._ymaxMargin;
    pMediaList[f].right         = dwfPapers[i]._xmaxMargin;
    pMediaList[f].bottom        = dwfPapers[i]._yminMargin;
    pMediaList[f].units         = dwfPapers[i]._units;
  }

  return eOk;
}

