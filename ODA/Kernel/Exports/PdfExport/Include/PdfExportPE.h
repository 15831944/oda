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


///////////////////////////////////////////////////////////////////////////////
//
// PdfExportPE.h - Open Design Pdf Export definitions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_EXPORT_PE_INCLUDED_
#define _PDF_EXPORT_PE_INCLUDED_

#include "RxModule.h"
#include "PdfExportDef.h"
#include "PdfExportParams.h"

#include "GiDefaultContext.h"
#include "Gi/GiDrawablePtrArray.h"

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT{

class PDFEXPORT_DLL OdPdfExportPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdPdfExportPE);
  virtual OdGiDefaultContextPtr createGiContext(OdDbBaseDatabase* pDb) const = 0;
  virtual void createAuxDrawables(OdGsDevice& device, OdArray<OdGiDrawablePtr>& aDrw) const = 0;
  // allows to exclude drawables from export; used to export only preselected entities
  virtual void filterDrawables(OdGsDevice& device) const = 0;
  virtual void applyPagesetupToLayout(OdUInt32 layoutIdx) const = 0;
  
  virtual void addDrawables(OdGiDrawablePtrArray& aDrw, OdGsDevice* pDevice, OdGsModel* pModel);
  virtual void evaluateFields(OdDbBaseDatabase* pDb) const;

  /** \details
    Create gs device for rendering of given views
    \param aViewObj [in]  Array of OdAbstractViewPE compatible object (OdGsView, OdDbViewport or etc.).
    \param pDb [in]  Pointer to the database context.
    \param flags [in]  Bitmap device flags.
    \param aDevice [out]  Array of result devices corresponding to the given aViewObj.
                          Note: 1) the array can contain NULL elements,
                                2) the same device can correspond to several views.
    Returns eOk if succeeded.
  */
  virtual OdResult gsBitmapDevices(
    const OdRxObjectPtrArray& aViewObj,
    OdDbBaseDatabase* pDb,
    OdUInt32 flags,
    OdArray<OdGsDevicePtr>& aDevice) {
    return OdResult::eNotImplementedYet; 
  }

};

typedef OdSmartPtr<OdPdfExportPE> OdPdfExportPEPtr;

}
#endif // _PDF_EXPORT_PE_INCLUDED_

