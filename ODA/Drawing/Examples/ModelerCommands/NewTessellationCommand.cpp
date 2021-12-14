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

//#include "ModelerCommon.h"
#include "OdaCommon.h"
#include "NewTessellationCommand.h"
#include "Ed/EdCommandStack.h"
#include "RxDynamicModule.h"
#include "Ed/EdUserIO.h"
#include "DbCommandContext.h"
#include "Wr/wrTriangulationParams.h"
#include "ModelerGeometry/ModelerModule.h"
#include "ModelerGeometryCreator.h"
#include "RxDynamicModule.h"
#include "SpaModeler/SpaParameters.h"
#include "SpaModeler/SpaModeler.h"
#include "OdDToStr.h"

TOOLKIT_EXPORT OdRxClassPtr odrxGetModelerGeometryCreatorService();

const OdString NewTessellationCommand::groupName() const
{
  return OD_T("MODELER GEOMETRY");
}

const OdString NewTessellationCommand::globalName() const
{
  return OD_T("EnableNewTessellation");
}

void NewTessellationCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabase* pDatabase = OdDbDatabase::cast(pCmdCtx->baseDatabase()).get();

  OdEdUserIO* pIO = pCmdCtx->userIO();
  if (!pIO)
    return;

  OdUInt32 nKeyword(0);

  OdRxModulePtr pModule;
  OdString sModelerModuleName = pDatabase ? OdModelerGeometryModuleName
                                          : (pCmdCtx->baseDatabase() ? OdDgnModelerGeometryModuleName : L"");
  if (!sModelerModuleName.isEmpty())
    pModule = odrxDynamicLinker()->getModule(sModelerModuleName);
  if (pModule.get())
  {
    // TODO create common interface with virtual getTriangulationParams/setTriangulationParams
    ModelerModule* pModModule = static_cast<ModelerModule*>(pModule.get());

    wrTriangulationParams TriangulationParams;
    pModModule->getTriangulationParams(TriangulationParams);

    bool tesselationState = TriangulationParams.bUseTesselation;
    nKeyword = pIO->getKeyword(OdString("New tessellation is now ") + (tesselationState ? OdString("Enabled") : OdString("Disabled")) + OdString(" Use new tessellation algorithm? [Yes/No]:"), OdString("Yes No"));

    switch (nKeyword)
    {
      case 0: // Yes
      {
        TriangulationParams.maxNumGridLines = 10000;
        TriangulationParams.maxFacetEdgeLength = 100000;
        TriangulationParams.normalTolerance = 360;
        TriangulationParams.surfaceTolerance = 0;
        TriangulationParams.gridAspectRatio = 0;
        TriangulationParams.BetweenKnots = 0;
        TriangulationParams.PointsPerEdge = 0;
        TriangulationParams.bRecalculateSurfaceTolerance = true;
        TriangulationParams.bFastMode = false;
        TriangulationParams.bUseTesselation = true;

        pModModule->setTriangulationParams(TriangulationParams);
        break;
      }
      case 1: // No
      {
        TriangulationParams.maxNumGridLines = 10000;
        TriangulationParams.maxFacetEdgeLength = 0;
        TriangulationParams.normalTolerance = 15;
        TriangulationParams.surfaceTolerance = 0;
        TriangulationParams.gridAspectRatio = 0;
        TriangulationParams.BetweenKnots = 2;
        TriangulationParams.PointsPerEdge = 0;
        TriangulationParams.bRecalculateSurfaceTolerance = false;
        TriangulationParams.bFastMode = false;
        TriangulationParams.bUseTesselation = false;

        pModModule->setTriangulationParams(TriangulationParams);
        break;
      }
    };
    
    tesselationState = TriangulationParams.bUseTesselation;
    pIO->putString(tesselationState ? OdString("Enabled") : OdString("Disabled"));

    return;
  }

  {
    OdRxModulePtr pModule = odrxDynamicLinker()->getModule(OdSpaModelerModuleName);
    if (pModule.get())
    {
      pIO->putError("Command incompatible with SpaModeler");
      return;
    }
  }

  {
    OdRxModulePtr pModule = odrxDynamicLinker()->getModule(OdC3dModelerModuleName);
    if (pModule.get())
    {
      pIO->putError("Command incompatible with C3dModeler");
      return;
    }
  }
  
  pIO->putError("No 3d-modeler engine TX found (neither ModelerGeometry nor SpaModeler or C3dModeler)");
}
