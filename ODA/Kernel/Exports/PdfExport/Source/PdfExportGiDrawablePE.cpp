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

#include "PdfExportCommon.h"
#include "PdfExportGiDrawablePE.h"
#include "PrcContextForPdfExport.h"

#include "RxObject.h"
#include "RxObjectImpl.h"

ODRX_CONS_DEFINE_MEMBERS(PdfExportGiDrawablePE, OdRxObject, RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS(PdfExportLayerPE, OdRxObject, RXIMPL_CONSTR);

OdResult PdfExportGiDrawablePE::drawableToPRC(const OdGiDrawable *pDrawable,
  const PDF2PRCExportParams *pPdfPrcParams,
  OdRxObject *context
) const
{
  return eNotImplementedYet;
}

OdResult PdfExportGiDrawablePE::drawableToPRC(const OdGiDrawable *pDrawable,
  const PDF2PRCExportParams *pPdfPrcParams, OdRxObject *context, 
  OdRxObject* pParent, OdRxObject *pPartDefinition, bool hasRefs
) const
{
  return eNotImplementedYet;
}

OdCmEntityColor PdfExportGiDrawablePE::getColor(const OdGiDrawable *pDrawable) const
{
  return OdCmEntityColor::kNone;
}
OdDbStub * PdfExportGiDrawablePE::getMaterial(const OdGiDrawable *pDrawable) const
{
  return NULL;
}

OdCmTransparency PdfExportGiDrawablePE::getTransparency(const OdGiDrawable *pDrawable) const
{
  return OdCmTransparency(1.);
  //return OdCmTransparency::kErrorValue;
}

OdCmEntityColor PdfExportLayerPE::getColor(const OdRxObject *pDrawable) const
{
  return OdCmEntityColor::kNone;
}
OdDbStub * PdfExportLayerPE::getMaterial(const OdRxObject *pDrawable) const
{
  return NULL;
}

OdCmTransparency PdfExportLayerPE::getTransparency(const OdRxObject *pDrawable) const
{
  return OdCmTransparency(1.);
  //return OdCmTransparency::kErrorValue;
}
