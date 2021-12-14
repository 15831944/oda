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

#include "FMCreateStart.h"
#include "FMGeometry.h"

#define FMCREATECASE(a, b, c) void c(const FacetModeler::DeviationParams& devParams, OdStreamBuf* pStream, CreationMode mode);
#include "ExampleCases.h"

OdString getTestName(CreationMode mode)
{
  OdString name;
#define FMCREATECASE(a, b, c) if (mode == k##a) name = #a; else 
#include "ExampleCases.h"
  name = "";

  return name;
}

void FMCreateStart(const FacetModeler::DeviationParams& devParams, OdStreamBuf* pStream, CreationMode mode)
{
#define FMCREATECASE(a, b, c) if (mode == k##a) c(devParams, pStream, mode); else 
#include "ExampleCases.h"
  ODA_ASSERT(false);
}
