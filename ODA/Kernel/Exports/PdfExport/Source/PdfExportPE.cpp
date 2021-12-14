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
#include "PdfExportPE.h"

#include "Gs/GsViewImpl.h"

namespace TD_PDF_2D_EXPORT{

ODRX_NO_CONS_DEFINE_MEMBERS(OdPdfExportPE, OdRxObject)

void OdPdfExportPE::addDrawables(OdGiDrawablePtrArray& aDrw, OdGsDevice* pDevice, OdGsModel* pModel)
{
  for (int i = 0, n = pDevice->numViews(); i < n; ++i)
  {
    OdGsViewImpl* pView = OdGsViewImpl::cast(pDevice->viewAt(i));
    if (!pView || pView->isDependentGeometryView())
      continue;
    for (unsigned j = 0; j < aDrw.size(); ++j)
      pView->add(aDrw[j], pModel);
  }
}

void OdPdfExportPE::evaluateFields(OdDbBaseDatabase* pDb) const
{
  OdDbBaseDatabasePEPtr(pDb)->evaluateFields(pDb, 0x04/*OdDbField::kPlot*/);
}

}
using namespace TD_PDF_2D_EXPORT;
