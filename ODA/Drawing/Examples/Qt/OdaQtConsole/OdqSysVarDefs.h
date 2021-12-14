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
// OdqSysVarDefs.h
//
// advanced quasi variables
//

// _RO - read only

#ifndef ODA_QT_VAR_DEF
#define ODA_QT_VAR_DEF ODA_QT_VAR_DEF_RO /* {Secret} */
#define DOUNDEF_ODA_QT_VAR_DEF
#endif

#ifndef ODTBOOL
#define ODTBOOL bool                    /* {Secret} */
#endif

///////////////////////////////////////////////////////////////////////////////
//                Type              Name                    Value                 setValue      Hidden (out of list)

// file name(...NAME) and path(...PREFIX)

ODA_QT_VAR_DEF_RO(OdString,         filename,               getFileName(pCmdCtx->baseDatabase()),
                                                                                  ,             false)
ODA_QT_VAR_DEF_RO(OdString,         fileprefix,             getFilePrefix(pCmdCtx->baseDatabase()),
                                                                                  ,             false)

ODA_QT_VAR_DEF_RO(OdString,         dgnname,                getFileName(pCmdCtx->baseDatabase(), enDb_Dgn),
                                                                                  ,             getFileName(pCmdCtx->baseDatabase(), enDb_Dgn).isEmpty())
ODA_QT_VAR_DEF_RO(OdString,         dgnprefix,              getFilePrefix(pCmdCtx->baseDatabase(), enDb_Dgn),
                                                                                  ,             getFileName(pCmdCtx->baseDatabase(), enDb_Dgn).isEmpty())

// dwgname and dwgprefix are dublicated here to get access from DGN and empty context with empty result

// Stores the name of the current drawing.
// If a new drawing has not been saved yet, DWGNAME defaults to Drawing1.dwg. Additional new drawings are named Drawing2.dwg, Drawing3.dwg, and so on. 
ODA_QT_VAR_DEF_RO(OdString,         dwgname,                getFileName(pCmdCtx->baseDatabase(), enDb_Dwg),
                                                                                  ,             true)

// Stores the drive and folder prefix for the drawing.
ODA_QT_VAR_DEF_RO(OdString,         dwgprefix,              getFilePrefix(pCmdCtx->baseDatabase(), enDb_Dwg),
                                                                                  ,             true)

// Indicates whether the current drawing has been named.
ODA_QT_VAR_DEF_RO(ODTBOOL,          dwgtitled,              getTitled(pCmdCtx->baseDatabase(), enDb_Dwg),
                                                                                  ,             true)

// system codepage of application (to check reading old DXF files).
ODA_QT_VAR_DEF_RO(OdCodePageId,        dwgcodepage,  odSystemServices()->systemCodePage(),
                                                                                  ((ExSystemServices*) odSystemServices())->setSystemCodePage(sName), false)

ODA_QT_VAR_DEF_RO(OdString,         tigname,                getFileName(pCmdCtx->baseDatabase(), enDb_Tig),
                                                                                  ,             true)


/////
// The next is needed in DGN only to resolve diesel expression like 
//  $(if,$(and,$(eq,$(getvar,tilemode),0),$(eq,$(getvar,cvport),1)),~,)Visual &Styles

// tilemode is duplicated here to get access from DGN (all tabs are models = 1)
ODA_QT_VAR_DEF_RO(ODTBOOL,          tilemode,               true,                 ,             true)

// cvport is duplicated here to get access from DGN (the identification number of the current viewport)
ODA_QT_VAR_DEF_RO(OdInt16,          cvport,                 0,                    ,             true)

/////

// TODO :

// Indicates whether the command window is open or closed.
ODA_QT_VAR_DEF_RO(OdInt16,          clistate,               getIConsole()->isConsoleVisible() ? 1 : 0,     
                                                                                  ,             false)

// Stores a value that indicates whether the clean screen state is on.
ODA_QT_VAR_DEF_RO(OdInt16,          cleanscreenstate,       0,                    ,             true)

//Reflects whether or not the Block Editor is open
ODA_QT_VAR_DEF_RO(OdInt16,          blockeditor,            0,                    ,             true)

// Controls how objects that are made invisible for the current visibility state are displayed in the Block Editor.
ODA_QT_VAR_DEF_RO(OdInt16,          bvmode,                 0,                    ,             true)

// Displays the name of the reference being edited.
ODA_QT_VAR_DEF_RO(OdString,         refeditname,            OdString::kEmpty,     ,             true)

//Indicates whether the Materials window is open
ODA_QT_VAR_DEF_RO(OdInt16,          matstate,               0,                    ,             true)

// Stores a value that indicates whether the Render Settings palette is open.
ODA_QT_VAR_DEF_RO(OdInt16,          renderprefsstate,       0,                    ,             true)

// Indicates whether the Layer Properties Manager is open or closed.
ODA_QT_VAR_DEF_RO(OdInt16,          layermanagerstate,      0,                    ,             true)

// Stores a value that indicates whether the Properties palette is open, closed or hidden.
ODA_QT_VAR_DEF_RO(OdInt16,          opmstate,               (getIPropSystem() && getIPropSystem()->isPropVisible()) ? 1 : 0,
                                                                                  ,             true)

// Stores a value that indicates whether the Revision Control palette is open, closed or hidden.
ODA_QT_VAR_DEF_RO(OdInt16,          rvstate,               (getIRevSystem() && getIRevSystem()->isRevVisible()) ? 1 : 0,
                                                                                  ,             true)

// Indicates whether the DesignCenter window is open or closed.
ODA_QT_VAR_DEF_RO(OdInt16,          adcstate,               0,                    ,             true)

// Indicates whether the Tool Palettes window is open or closed.
ODA_QT_VAR_DEF_RO(OdInt16,          tpstate,                0,                    ,             true)

// Indicates whether the Sheet Set Manager window is open or closed.
ODA_QT_VAR_DEF_RO(OdInt16,          ssmstate,               0,                    ,             true)

// Indicates whether the Markup Set Manager is open or closed.
ODA_QT_VAR_DEF_RO(OdInt16,          msmstate,               0,                    ,             true)

// Indicates whether the QuickCalc calculator is open or closed.
ODA_QT_VAR_DEF_RO(OdInt16,          qcstate,                0,                    ,             true)

// Indicates whether the Lights in Model window is open.
ODA_QT_VAR_DEF_RO(OdInt16,          lightliststate,         0,                    ,             true)

// Indicates whether the Sun Properties window is open or closed.
ODA_QT_VAR_DEF_RO(OdInt16,          sunpropertiesstate,     0,                    ,             true)

// Indicates whether the Visual Styles window is open or closed.
ODA_QT_VAR_DEF_RO(OdInt16,          vsstate,                0,                    ,             true)

// Indicates whether the External References palette is open or closed.
ODA_QT_VAR_DEF_RO(OdInt16,          erstate,                0,                    ,             true)

// Indicates whether the ribbon palette is open or closed.
ODA_QT_VAR_DEF_RO(OdInt16,          ribbonstate,            0,                    ,             true)

// Indicates whether the Parameters Manager palette is displayed or hidden
ODA_QT_VAR_DEF_RO(OdInt16,          parametersstatus,       0,                    ,             true)

// Indicates whether the palettes, including the ribbon (mistaken - ribbonclose does not change it to zero) 
// and the Command window, are displayed or hidden. 
ODA_QT_VAR_DEF_RO(OdInt16,          showpalettestate,       getIConsole()->isConsoleVisible() ? 1 : 0,
                                                                                  ,             true)

// Returns the program name. ("ODADrawingsExplorer")("acad")
ODA_QT_VAR_DEF_RO(OdString,         program,                getIApp()->toOdString(QFileInfo(QCoreApplication::applicationFilePath()).baseName()),
                                                                                  ,             false)

// Returns the product name. ("ODA File Viewer")("AutoCAD") 
ODA_QT_VAR_DEF_RO(OdString,         product,                getIApp()->toOdString(getIApp()->getAppName()),
                                                                                  ,             false)
  
// Displays the names of the active and transparent commands.
ODA_QT_VAR_DEF_RO(OdString,         cmdnames,               getIConsole()->activeCommandName(pCmdCtx->baseDatabase()),
                                                                                  ,             false)

// Indicates whether an ordinary command, transparent command, script, or dialog box is active.
// (system variable CMDACTIVE returns the sum of TCmdActiveState values)
ODA_QT_VAR_DEF_RO(OdInt16,          cmdactive,              getIConsole()->activeCommandState(pCmdCtx->baseDatabase()),
                                                                                  ,             false)

// Indicates whether the viewport is maximized or not.
ODA_QT_VAR_DEF_RO(OdInt16,          vpmaximizedstate,       0,                    ,             true)

// Controls whether using object snap overrides are allowed, or just running object snaps. 
// This system variable was implemented to support the Temporary Override feature.
// If set to 1, the object snap settings in the OSNAP dialog are enforced. 
// (ACAD Integer + Not saved + default 0 + undocumanted)
//ODA_QT_VAR_DEF_RO(OdInt16,        osnapoverride,          0,                    ,             true)

// registry variables

// Displays or hides dynamic constraints. (ACAD default 1)
ODA_QT_VAR_DEF_RO(OdInt16,          dynconstraintdisplay,   getIAppProps()->toBool(OD_T("Vars/dynconstraintdisplay"), false) ? 1 : 0,
                                                                                  ,             true)

// Turns Dynamic Input features on and off. When DYNMODE is set to a negative value, the Dynamic Input features are not turned on, but the setting is stored.
// (Real ACAD default -3)
ODA_QT_VAR_DEF_RO(OdInt16,          dynmode,                getIAppProps()->toInt(OD_T("Vars/dynmode"), 0),
                                                                                  ,             true)

// Locks the position and size of toolbars and dockable windows such as the ribbon, DesignCenter, and the Properties palette.
ODA_QT_VAR_DEF_RO(OdInt16,          lockui,                 getIAppProps()->toInt(OD_T("Vars/lockui"), 0),
                                                                                  ,             true)

// Controls the amount of fading for objects on locked layers. (ACAD dafaut 50)
ODA_QT_VAR_DEF_RO(OdInt16,          laylockfadectl,         getIAppProps()->toInt(OD_T("Vars/laylockfadectl"), 0),
                                                                                  ,             true)

// Controls the dimming for all DWG xref objects. (ACAD default 70)
ODA_QT_VAR_DEF_RO(OdInt16,          xdwgfadectl,            getIAppProps()->toInt(OD_T("Vars/xdwgfadectl"), 0),
                                                                                  ,             true)

// Controls the display of the AutoSnap marker, tooltip, and magnet. (ACAD default 63)
// autosnap returns the sum of the following values:
//typedef enum { // TODO
//    enAutoSnap_Off = 0 // Turns off the AutoSnap marker, tooltips, and magnet. Also turns off polar tracking, object snap tracking, and tooltips for polar tracking, object snap tracking, and Ortho mode
//  , enAutoSnap_Marker = 1 // Turns on the AutoSnap marker
//  , enAutoSnap_Tooltips = 2 // Turns on the AutoSnap tooltips
//  , enAutoSnap_Magnet = 4 // Turns on the AutoSnap magnet
//  , enAutoSnap_PolarTracking = 8 // Turns on polar tracking
//  , enAutoSnap_ObjectSnapTracking = 16 // Turns on object snap tracking
//  , enAutoSnap_TooltipsAll = 32 // Turns on tooltips for polar tracking, object snap tracking, and Ortho mode
//} TOdAutoSnap;
//ODA_QT_VAR_DEF_RO(OdInt16,          autosnap,               getIAppProps()->toInt(OD_T("Vars/autosnap"), 0),
//                                                                                  ,             true)

// The setting is stored as a bitcode using the sum of the following values: (ACAD defailt 4133)
//   NONe = 0, ENDpoint = 1, MIDpoint = 2, CENter = 4, NODe = 8, QUAdrant = 16,
//// INTersection = 32, 
//   INSertion = 64, PERpendicular = 128, TANgent = 256, NEArest = 512, 
//// Clears_all_object_snaps = 1024, APParent_Intersection = 2048,
//// EXTension = 4096, PARallel = 8192
// (override of Drawing\Include\SysVarDefs.h)
ODA_QT_VAR_DEF(OdInt16,             osmode,                   getIAppProps()->toInt(L"Vars/osmode", 4433),
                                    osmode_edit(pCmdCtx, sName, sDefVal, OdResBuf::kRtInt16),
                                    true)

ODA_QT_VAR_DEF(ODTBOOL,             perspective,              perspective_value(pCmdCtx->baseDatabase()),
                                    perspective_edit(pCmdCtx, sName, sDefVal, OdResBuf::kRtBool),
                                    perspective_hidden(pCmdCtx->baseDatabase()))

// TODO : Drawing variables : missing now (TODO for Kernel/Include/SysVardefs.h) : 

// Drawing : vsfaceopacity -60 // Turns on and off a preset level of transparency for 3D objects.
// Drawing : vsobscurededges 0 // Controls whether obscured (hidden) edges are displayed.
// Drawing : vsintersectionedges 0 // Controls the display of intersection edges in the visual style applied to the current viewport.
// Drawing : vssilhedges 0 // Controls display of silhouette edges of solid objects in the visual style applied to the current viewport.
// Drawing : ucsdetect 1 // Controls whether dynamic UCS acquisition is active or not.
// Drawing : navvcubedisplay 0 // Controls the display of the ViewCube tool in the current viewport when the 3D graphics system is active.

///////////////////////////////////////////////////////////////////////////////

#undef ODTBOOL

#ifdef DOUNDEF_ODA_QT_VAR_DEF
#undef ODA_QT_VAR_DEF
#undef DOUNDEF_ODA_QT_VAR_DEF
#endif
