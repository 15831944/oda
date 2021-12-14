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
#include "PropertiesCommand.h"
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

static OdString toString(const SPAParameters::SPA_ADJUST& adjustMode)
{
  switch(adjustMode)
  {
  case SPAParameters::SPA_ADJUST_NON_GRID:
    return OD_T("SPA_ADJUST_NON_GRID");
  case SPAParameters::SPA_ADJUST_ALL:
    return OD_T("SPA_ADJUST_ALL");
  };
  return OD_T("SPA_ADJUST_NONE");
}

static OdString toString(const SPAParameters::SPA_GRID & gridMode)
{
  switch(gridMode)
  {
  case SPAParameters::SPA_GRID_INTERIOR:
    return OD_T("SPA_GRID_INTERIOR");
  case SPAParameters::SPA_GRID_TO_EDGES:
    return OD_T("SPA_GRID_TO_EDGES");
  case SPAParameters::SPA_GRID_ONE_DIR:
    return OD_T("SPA_GRID_ONE_DIR");
  };
  return OD_T("SPA_GRID_NONE");
}

static OdString toString(const SPAParameters::SPA_TRIANG_MODE & triangMode)
{
  switch(triangMode)
  {
  case SPAParameters::SPA_TRIANG_ALL:
    return OD_T("SPA_TRIANG_ALL");
  case SPAParameters::SPA_TRIANG_FRINGE_1:
    return OD_T("SPA_TRIANG_FRINGE_1");
  case SPAParameters::SPA_TRIANG_FRINGE_2:
    return OD_T("SPA_TRIANG_FRINGE_2");
  case SPAParameters::SPA_TRIANG_FRINGE_3:
    return OD_T("SPA_TRIANG_FRINGE_3");
  case SPAParameters::SPA_TRIANG_FRINGE_4:
    return OD_T("SPA_TRIANG_FRINGE_4");
  };
  return OD_T("SPA_TRIANG_NONE");
}

const OdString PropertiesCommand::groupName() const
{
  return OD_T("MODELER GEOMETRY");
}

const OdString PropertiesCommand::globalName() const
{
  return OD_T("ModelerProperties");
}

void PropertiesCommand::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabase* pDatabase = OdDbDatabase::cast(pCmdCtx->baseDatabase()).get();

  OdEdUserIO* pIO = pCmdCtx->userIO();
  if (!pIO)
    return;

  OdUInt32 nKeyword(0);

  OdRxModulePtr pModule;
  OdString sModelerModuleName = pDatabase ? L"ModelerGeometry.tx" 
                                          : (pCmdCtx->baseDatabase() ? L"TG_ModelerGeometry.tx" : L"");
  if (!sModelerModuleName.isEmpty())
    pModule = odrxDynamicLinker()->getModule(sModelerModuleName);
  if (pModule.get())
  {
    // TODO create common interface with virtual getTriangulationParams/setTriangulationParams
    ModelerModule* pModModule = static_cast<ModelerModule*>(pModule.get()); 
    do
    {
      nKeyword = pIO->getKeyword(OD_T("Enter parameter to change [View/EdgeLength/NormalTolerance/NurbsParam/PointsPerEdge/SurfaceTolerance/RecalculateSurfaceTolerance/FastMode/Finish]:"),
                                 OD_T("View EdgeLength NormalTolerance NurbsParam PointsPerEdge SurfaceTolerance RecalculateSurfaceTolerance FastMode Finish"));
      
      wrTriangulationParams TriangulationParams;
      pModModule->getTriangulationParams(TriangulationParams);
      
      OdString tmp;
      try
      {
        switch(nKeyword)
        {
        case 0: // View
          tmp.format(OD_T("Maximum facet edge length : %f"), TriangulationParams.maxFacetEdgeLength);
          pIO->putString(tmp);
          tmp.format(OD_T("Normal tolerance : %f"), TriangulationParams.normalTolerance);
          pIO->putString(tmp);
          tmp.format(OD_T("Nurb surface param : %d"), TriangulationParams.BetweenKnots);
          pIO->putString(tmp);
          tmp.format(OD_T("Points per edge : %d"), TriangulationParams.PointsPerEdge);
          pIO->putString(tmp);
          tmp.format(OD_T("Surface tolerance : %f"), TriangulationParams.surfaceTolerance);
          pIO->putString(tmp);
          tmp.format(OD_T("Recalculate surface tolerance enabled : %ls"), TriangulationParams.bRecalculateSurfaceTolerance ? OD_T("YES") : OD_T("NO"));
          pIO->putString(tmp);
          throw OdEdEmptyInput();

        case 1: // Edge_length
          tmp.format(OD_T("Enter maximum facet edge length <%f>"), TriangulationParams.maxFacetEdgeLength);
          TriangulationParams.maxFacetEdgeLength = pIO->getReal(tmp, OdEd::kInpThrowEmpty);
          break;

        case 2: // normal_Tolerance
          tmp.format(OD_T("Enter normal tolerance <%f>"), TriangulationParams.normalTolerance);
          TriangulationParams.normalTolerance = pIO->getReal(tmp, OdEd::kInpThrowEmpty);
          break;

        case 3: // Nurbs_param
          tmp.format(OD_T("Enter NURB surface param <%d>"), TriangulationParams.BetweenKnots);
          TriangulationParams.BetweenKnots = (OdUInt16)pIO->getInt(tmp, OdEd::kInpThrowEmpty);
          break;

        case 4: // Points_per_edge
          tmp.format(OD_T("Enter points per edge <%d>"), TriangulationParams.PointsPerEdge);
          TriangulationParams.PointsPerEdge = (OdUInt32)pIO->getInt(tmp, OdEd::kInpThrowEmpty);
          break;

        case 5: // Surface tolerance
          tmp.format(OD_T("Enter Surface tolerance <%f>"), TriangulationParams.surfaceTolerance);
          TriangulationParams.surfaceTolerance = pIO->getReal(tmp, OdEd::kInpThrowEmpty);
          break;

        case 6: // bRecalculateSurfaceTolerance support
          tmp.format(OD_T("Enable Recalculate surface tolerance <%ls>"), TriangulationParams.bRecalculateSurfaceTolerance ? OD_T("YES") : OD_T("NO"));
          TriangulationParams.bRecalculateSurfaceTolerance = (pIO->getKeyword(tmp, OD_T("NO YES"), OdEd::kInpThrowEmpty)==1);

          if (TriangulationParams.bRecalculateSurfaceTolerance)
            TriangulationParams.normalTolerance = 15.;
          break;

        case 7: // FastMode
          tmp.format(OD_T("Enable fast mode<%ls>"), TriangulationParams.bFastMode ? OD_T("YES") : OD_T("NO"));
          TriangulationParams.bFastMode = (pIO->getKeyword(tmp, OD_T("NO YES"), OdEd::kInpThrowEmpty)==1);
          break;
        };
        pModModule->setTriangulationParams(TriangulationParams);
      }
      catch(const OdEdEmptyInput)
      {
      }
    }
    while(nKeyword < 7); // strange but FastMode is used as marker to end also as Finish
    return;
  }


  {
     OdRxModulePtr pModule = odrxDynamicLinker()->getModule(OD_T("SpaModeler.tx"));
     if (pModule.get())
     {
       OdSmartPtr<SpaModeler> pModModule = SpaModeler::cast(pModule);
       if (pModModule.get())
       {
         do
         {
           nKeyword = pIO->getKeyword(OD_T("Enter parameter to change [View/AdjustMode/GridMode/TriangMode/SurfaceTol/NormalTol/MaxEdgeLength/GridAspectRatio/MaxGridLines/MinUGridLines/MinVGridLines/UseFacetRes/Finish]:"),
             OD_T("View AdjustMode GridMode TriangMode SurfaceTol NormalTol MaxEdgeLength GridAspectRatio MaxGridLines MinUGridLines MinVGridLines UseFacetRes Finish"));


           SPAParameters Params;
           pModModule->getSPAParameters(Params);

           OdString tmp;
           try
           {
             switch(nKeyword)
             {
             case 0: // View
               tmp.format(OD_T("AdjustMode : %s"), toString(Params.adjust_mode).c_str());
               pIO->putString(tmp);
               tmp.format(OD_T("GridMode : %s"), toString(Params.grid_mode).c_str());
               pIO->putString(tmp);
               tmp.format(OD_T("TriangMode : %s"), toString(Params.triang_mode).c_str());
               pIO->putString(tmp);
               tmp.format(OD_T("SurfaceTol : %s"), odDToStr(Params.surface_tol, 'f', 6).c_str());
               pIO->putString(tmp);
               tmp.format(OD_T("NormalTol : %s"), odDToStr(Params.normal_tol, 'f', 6).c_str());
               pIO->putString(tmp);
               tmp.format(OD_T("MaxEdgeLength : %s"), odDToStr(Params.max_edge_length, 'f', 6).c_str());
               pIO->putString(tmp);
               tmp.format(OD_T("GridAspectRatio : %s"), odDToStr(Params.grid_aspect_ratio, 'f', 6).c_str());
               pIO->putString(tmp);
               tmp.format(OD_T("MaxGridLines : %d"), Params.max_grid_lines);
               pIO->putString(tmp);
               tmp.format(OD_T("MinUGridLines : %d"), Params.min_u_grid_lines);
               pIO->putString(tmp);
               tmp.format(OD_T("MinVGridLines : %d"), Params.min_v_grid_lines);
               pIO->putString(tmp);
               tmp.format(OD_T("UseFacetRes : %s"), Params.bUseFacetRES ? OD_T("TRUE") : OD_T("FALSE"));
               pIO->putString(tmp);

               throw OdEdEmptyInput();

             case 1: // AdjustMode
               tmp.format(OD_T("Enter Adjust Mode <%s>"), toString(Params.adjust_mode).c_str());
               Params.adjust_mode = (SPAParameters::SPA_ADJUST) pIO->getKeyword(tmp, OD_T("SPA_ADJUST_NONE SPA_ADJUST_NON_GRID SPA_ADJUST_ALL"), OdEd::kInpThrowEmpty);
               break;

             case 2: // gridMode
               tmp.format(OD_T("Enter Grid Mode <%s>"), toString(Params.grid_mode).c_str());
               Params.grid_mode = (SPAParameters::SPA_GRID) pIO->getKeyword(tmp, OD_T("SPA_GRID_NONE SPA_GRID_INTERIOR SPA_GRID_TO_EDGES SPA_GRID_ONE_DIR"), OdEd::kInpThrowEmpty);
               break;

             case 3: // triangMode
               tmp.format(OD_T("Enter Triang Mode <%s>"), toString(Params.triang_mode).c_str());
               Params.triang_mode =(SPAParameters::SPA_TRIANG_MODE)  pIO->getKeyword(tmp, OD_T("SPA_TRIANG_NONE SPA_TRIANG_ALL SPA_TRIANG_FRINGE_1 SPA_TRIANG_FRINGE_2 SPA_TRIANG_FRINGE_3 SPA_TRIANG_FRINGE_4"), OdEd::kInpThrowEmpty);
               break;

             case 4: // surface_tol
               tmp.format(OD_T("SurfaceTol : %s"), odDToStr(Params.surface_tol, 'f', 6).c_str());
               Params.surface_tol = pIO->getReal(tmp, OdEd::kInpThrowEmpty);
               break;
             case 5: // normal_tol
               tmp.format(OD_T("NormalTol : %s"), odDToStr(Params.normal_tol, 'f', 6).c_str());
               Params.normal_tol = pIO->getReal(tmp, OdEd::kInpThrowEmpty);
               break;
             case 6: // max_edge_length
               tmp.format(OD_T("MaxEdgeLength : %s"), odDToStr(Params.max_edge_length, 'f', 6).c_str());
               Params.max_edge_length = pIO->getReal(tmp, OdEd::kInpThrowEmpty);
               break;
             case 7: // grid_aspect_ratio
               tmp.format(OD_T("GridAspectRatio : %s"), odDToStr(Params.grid_aspect_ratio, 'f', 6).c_str());
               Params.grid_aspect_ratio = pIO->getReal(tmp, OdEd::kInpThrowEmpty);
               break;
             case 8: // max_grid_lines
               tmp.format(OD_T("MaxGridLines : %d"), Params.max_grid_lines);
               Params.max_grid_lines = pIO->getInt(tmp, OdEd::kInpThrowEmpty);
               break;
             case 9: // min_u_grid_lines
               tmp.format(OD_T("MinUGridLines : %d"), Params.min_u_grid_lines);
               Params.min_u_grid_lines = pIO->getInt(tmp, OdEd::kInpThrowEmpty);
               break;
             case 10: // min_v_grid_lines
               tmp.format(OD_T("MinVGridLines : %d"), Params.min_v_grid_lines);
               Params.min_v_grid_lines = pIO->getInt(tmp, OdEd::kInpThrowEmpty);
               break;        

             case 11: // FACETRES support
               tmp.format(OD_T("Enable FACETRES <%ls>"), Params.bUseFacetRES ? OD_T("YES") : OD_T("NO"));
               Params.bUseFacetRES = (pIO->getKeyword(tmp, OD_T("NO YES"), OdEd::kInpThrowEmpty)==1);

               if (Params.bUseFacetRES)
                 Params.normal_tol = 15.;
               break;
             };
             pModModule->setSPAParameters(Params);
           }
           catch(const OdEdEmptyInput)
           {
           }
         }
         while(nKeyword != 12);
       }
       return;
     }
  }
  pIO->putError("No 3d-modeler engine TX found (neither ModelerGeometry nor SpaModeler)");
}
