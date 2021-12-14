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

//#include "StringArray.h"

#ifndef ODTBOOL
#define ODTBOOL                 bool                    /*!DOM*/
#endif

#ifndef ODTDIST
#define ODTDIST                 double                  /*!DOM*/
#endif

#ifndef REGVAR_DEF_ARRAY
#define REGVAR_DEF_ARRAY(type, name, unused3, unused4, unused5)                                /*!DOM*/
#endif

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    PREVIEW_WIDTH specifies the width in pixels of generated preview bitmap images."
    </remarks>
  </properties>
*/
REGVAR_DEF(OdUInt32,            PREVIEW_WIDTH,  (180),                (),       ValidateNone())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    PREVIEW_HEIGHT specifies the height in pixels of generated preview bitmap images."
    </remarks>
  </properties>
*/
REGVAR_DEF(OdUInt32,            PREVIEW_HEIGHT, (88),                 (),       ValidateNone())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    FAST_FONT specifies the name of the default RSC font"
    </remarks>
  </properties>
*/
REGVAR_DEF(OdString,           FAST_FONT,    (OD_T("CHAR_FAST_FONT")),    (),       ValidateNone())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    MS_RSRC specifies the main default resource file."
    </remarks>
  </properties>
*/
REGVAR_DEF(OdString,           MS_RSRC,    (OD_T("ustation.rsc")),    (),       ValidateNone())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    MS_SYMBRSRC specifies a list of symbology resource files. The last file in the list has the highest priority."
    </remarks>
  </properties>
*/
REGVAR_DEF_ARRAY(OdStringArray, MS_SYMBRSRC, 0,                       (),       ValidateNone())

/** * VARDOCS:
<properties>
<remarks>
"\n    MS_PATTERN specifies a list of directories with material diffuse texture files. The last file in the list has the highest priority."
</remarks>
</properties>
*/
REGVAR_DEF_ARRAY(OdStringArray, MS_PATTERN, 0,                       (),       ValidateNone())
/** * VARDOCS:
<properties>
<remarks>
"\n    MS_BUMP specifies a list of directories with material bump map texture files. The last file in the list has the highest priority."
</remarks>
</properties>
*/
REGVAR_DEF_ARRAY(OdStringArray, MS_BUMP, 0,                       (),       ValidateNone())
/** * VARDOCS:
<properties>
<remarks>
"\n    MS_MATERIAL specifies a list of directories with material files. The last file in the list has the highest priority."
</remarks>
</properties>
*/
REGVAR_DEF_ARRAY(OdStringArray, MS_MATERIAL, 0,                       (),       ValidateNone())
/** * VARDOCS:
<properties>
<remarks>
"\n    MS_MTBL specifies a list of directories with material table assignment files. The last file in the list has the highest priority."
</remarks>
</properties>
*/
REGVAR_DEF_ARRAY(OdStringArray, MS_MTBL, 0,                       (),       ValidateNone())
/** * VARDOCS:
<properties>
<remarks>
"\n    MS_MATPALETTE specifies a list of default external material palette files. The last file in the list has the highest priority."
</remarks>
</properties>
*/
REGVAR_DEF_ARRAY(OdStringArray, MS_MATPALETTE, 0,                       (),       ValidateNone())

/** * VARDOCS:
<properties>
<remarks>
"\n    TG_NURBS_GRID specifies the grid size ( quality ) for b spline surface draw algorithm."
</remarks>
</properties>
*/
REGVAR_DEF(OdUInt32,            TG_NURBS_GRID, (10),                 (),       ValidateNone())

/** * VARDOCS:
<properties>
<remarks>
"\n    TG_USE_XREFCTBL specifies color table, that is used when reference attachment is drawn."
"\n"
"\n    @table"
"\n    Value    Description"
"\n    false    use master file color table"
"\n    true     use reference file color table"
</remarks>
</properties>
*/
REGVAR_DEF(ODTBOOL,            TG_USE_XREFCTBL, (1),                 (),       ValidateNone())

/** * VARDOCS:
<properties>
<remarks>
"\n    TG_REMAP_CLRIDX specifies additional processing of element color indexes when coping elements "
"\n    between databases.
"\n"
"\n    @table"
"\n    Value    Description"
"\n    false    Color indexes are not processed"
"\n    true     Use closest color match in master file color table"
</remarks>
</properties>
*/
REGVAR_DEF(ODTBOOL,            TG_REMAP_CLRIDX, (1),                 (),       ValidateNone())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    MS_3DOBJ_WF_ISOLINES controls 3d object draw mode for k2dOptemised and kWireframe render mode."
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    false    Draw all solid edges and faces same way as we do it for shaded rendering mode"
      "\n    true     Draw isolines only ( for solids and smart solids we draw only geometry cache )"
    </remarks>
  </properties>
*/

REGVAR_DEF(ODTBOOL,             MS_3DOBJ_WF_ISOLINES,      (0),                  (),       ValidateBOOL())

/** * VARDOCS:
<properties>
<remarks>
"\n    MS_PENTABLE specifies the pen table ( dgn plot style ) file."
</remarks>
</properties>
*/

REGVAR_DEF(OdString,            MS_PENTABLE, (OD_T("")),    (),       ValidateNone())

/** * VARDOCS:
<properties>
<remarks>
"\n    MS_DEFCTBL specifies the path to rsc file with default color table description."
</remarks>
</properties>
*/

REGVAR_DEF(OdString,            MS_DEFCTBL, (OD_T("")),    (),       ValidateNone())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    PICKFIRST controls noun-verb object selection."
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    false    Disabled"
      "\n    true     Enabled"
    </remarks>
  </properties>
*/
REGVAR_DEF(ODTBOOL,             PICKFIRST,      (1),                  (),       ValidateBOOL())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    PICKBOX specifies the object selection target height, in pixels."
    </remarks>
  </properties>
*/
REGVAR_DEF(OdInt16,             PICKBOX,        (3),                  (),       ValidateRange(0,50))

/** * VARDOCS:
<properties>
<remarks>
"\n   Sets the size of the grip box in pixels."
</remarks>
</properties>
*/
REGVAR_DEF(OdInt16,             DG_GRIPSIZE,       (5),                  (),       ValidateRange(1,255))

/** * VARDOCS:
<properties>
<remarks>
"\n   Controls the color of active grip point"
</remarks>
</properties>
*/
REGVAR_DEF(OdInt16,             DG_GRIPHOT,        (2),                  (),       ValidateRange(1,255))

/** * VARDOCS:
<properties>
<remarks>
"\n   Controls the color of non-selected grips"
</remarks>
</properties>
*/
REGVAR_DEF(OdInt16,             DG_GRIPCOLOR,      (119),                (),       ValidateRange(1,255))

/** * VARDOCS:
<properties>
<remarks>
"\n    "
</remarks>
</properties> specifies the ACI color index of unselected grips when the cursor hovers over them.
*/
REGVAR_DEF(OdInt16,             DG_GRIPHOVER,      (4),                  (),       ValidateRange(1,255))

/** * VARDOCS:
<properties>
<remarks>
"\n    DG_GRIPOBJLIMIT specifies the number of selected objects beyond which grips are suppressed."
" for a selection set."
</remarks>
</properties>
*/
REGVAR_DEF(OdInt16,             DG_GRIPOBJLIMIT,   (100),                (),       ValidateRange(0,32767))

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    DG_ACTIVELEVELALWAYSVISIBLE controls visibility of active level."
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    false    visibility of active level is the same as visibility of any other level."
      "\n    true     active level is always visible. Even if its display flag is reset and "
      "\n             its freeze flag is set."
    </remarks>
  </properties>
*/
REGVAR_DEF(ODTBOOL,             DG_ACTIVELEVELALWAYSVISIBLE,      (0),                  (),       ValidateBOOL())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    DG_PROCESSTAGASSOC controls processing of tag association by ODA Software."
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    false    only added reactors to designate associated with tag elements."
      "\n    true     process tag association and recalculate tag position after operations
      "\n             with associated with tag element.
    </remarks>
  </properties>
*/
REGVAR_DEF(ODTBOOL,             DG_PROCESSTAGASSOC,      (1),                  (),       ValidateBOOL())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    MS_REF_NEWLEVELDISPLAY controls visibility of new levels (level, that was added into reference file
      "\n    after creation of reference attachment) into reference attachments."
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    false    doesn't display."
      "\n    true     display.
    </remarks>
  </properties>
*/
REGVAR_DEF(ODTBOOL,             MS_REF_NEWLEVELDISPLAY,      (0),                  (),       ValidateBOOL())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    MS_EXACT_EXTENTS controls quality of reference attachment extents calculation."
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    false    Calculate extents of referenced model and transform the extents by reference attachment transformation."
      "\n    true     Calculate extents through rendering of reference attachment."
    </remarks>
  </properties>
*/

REGVAR_DEF(ODTBOOL,             DG_EXACT_EXTENTS,      (0),                  (),       ValidateBOOL())

/** * VARDOCS:
  <properties>
    <remarks>
      "\n    DG_INTERNAL_LS_REGEN_DRAW allow to forbid cache use for curve objects with internal line styles (from 1 to 7)"
      "\n    and recalculate them every time when we draw them. The default state of the mode is off because it can decrease
      "\n    performance for some cases.
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    false    mode is off."
      "\n    true     mode is on."
    </remarks>
  </properties>
*/

REGVAR_DEF(ODTBOOL,             DG_INTERNAL_LS_REGEN_DRAW,      (0),                  (),       ValidateBOOL())

/** * VARDOCS:
<properties>
<remarks>
"\n    MS_FONTPATH specifies a list of directories with shx font files. The last file in the list has the highest priority."
</remarks>
</properties>
*/
REGVAR_DEF_ARRAY(OdStringArray, MS_FONTPATH, 0,                       (),       ValidateNone())

/**
  <title MTMODE>
  <toctitle MTMODE>

  <table>
   <b>Type</b>            OdInt16
   <b>Initial value</b>   1
   <b>Read-Only</b>       No
   <b>Saved In</b>        Registry
   <b>Versions</b>        N/A
  </table>

  The MTMODE variable specifies whether multi-threading is used. The value is a bit mask:

  <table>
   <b>Value</b>   <b>Description</b>
   0              Multi-threaded file loading and regeneration are disabled.
   1              Multi-threaded file loading is enabled.
   2              Multi-threaded regeneration is enabled.
   3              Multi-threaded file loading and regeneration are enabled.
  </table>

  <table>
   <b>Access Methods</b>
   OdDbDatabase::getSysVar()
   OdDbDatabase::setSysVar()
   &nbsp;
   OdDbHostAppServices::getMtMode()
   OdDbHostAppServices::setMtMode()
  </table>

  For example, to get the value:
  <code>
   OdRusBufPtr pRb = pDb-\>getSysVar(L"MTMODE");
   OdInt16 mtmode = pRb-\>getInt16();
   odPrintConsoleString(L"nMulti-thread mode is %s", ((mtmode &amp; 0x01) ? "ON" : "OFF"));
   // or //
   odPrintConsoleString(L"nMulti-thread mode = %d", pDb-\>appServices()-\>getMtMode());
  </code>
  For example, to set the value:
  <code>
   OdRusBufPtr pRb = OdResBuf::newRb(OdResBuf::kRtInt16, 1);
   pDb-\>setSysVar(L"MTMODE", pRb);
   // or //
   pDb-\>appServices()-\>setMtMode(1);
  </code>

  <group TD_SysVars_M>
  */
  //<dom_var_def>OdInt16 MTMODE;
  REGVAR_DEF(OdInt16, MtMode, (0), (), ValidateNone())

  /** * VARDOCS:
  <properties>
    <remarks>
      "\n    TG_TEXTSIZE specifies the default text size for default text style."
    </remarks>
  </properties>
  */
  REGVAR_DEF(double, TG_TEXTSIZE, (2500.0), (), ValidatePositive())

  /** * VARDOCS:
  <properties>
    <remarks>
      "\n    DG_PROCESSDIMASSOC controls processing of dimension association by ODA Software."
      "\n"
      "\n    @table"
      "\n    Value    Description"
      "\n    false    only added reactors to designate associated with dimension elements."
      "\n    true     process dimension association and recalculate dimension points position after operations
      "\n             with associated with dimension element.
    </remarks>
  </properties>
  */
  REGVAR_DEF(ODTBOOL, DG_PROCESSDIMASSOC, (1), (), ValidateBOOL())

  /** * VARDOCS:
  <properties>
  <remarks>
    "\n    DG_PROCESSMLINEASSOC controls processing of multiline association by ODA Software."
    "\n"
    "\n    @table"
    "\n    Value    Description"
    "\n    false    only added reactors to designate associated with multiline elements."
    "\n    true     process dimension association and recalculate multiline points position after operations
    "\n             with associated with multiline element.
  </remarks>
  </properties>
  */
  REGVAR_DEF(ODTBOOL, DG_PROCESSMLINEASSOC, (1), (), ValidateBOOL())

  /** * VARDOCS:
  <properties>
  <remarks>
  "\n    TG_LSSCALE_TRANSFORM specifies mode of line style scale transformation.
  "\n"
  "\n    @table"
  "\n    Value    Description"
  "\n    false    disable line style scale transformation by transformBy() method.
  "\n    true     enable line style scale transformation by transformBy() method.
  </remarks>
  </properties>
  */
  REGVAR_DEF(ODTBOOL, TG_LSSCALE_TRANSFORM, (1), (), ValidateNone())

#undef ODTBOOL
#undef ODTDIST
#undef REGVAR_DEF_ARRAY
