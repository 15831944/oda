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


#ifndef _PDF_EXPORT_GIDRAWABLE_PE_INCLUDED_
#define _PDF_EXPORT_GIDRAWABLE_PE_INCLUDED_

#include "Gi/GiDrawableImpl.h"
#include "PdfExportParams.h"
#include "Pdf2PrcExportParams.h"
#include "PdfExportDef.h"

/** \details 
A base class for protocol extension that implements the PDF export functionality of a Gi drawable entity.
<group OdExport_Classes>
*/
class PDFEXPORT_DLL PdfExportGiDrawablePE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(PdfExportGiDrawablePE);

  /** \details 
  Exports a specified drawable entity to PRC format.
  \param pDrawable     [in] A raw pointer to the drawable to export.
  \param pPdfPrcParams [in] A raw pointer to an object that contains the set of export parameters.
  \param context       [in] A raw pointer to an object that contains the drawable context.
  \returns eOk if the drawable was successfully exported to PRC format; otherwise the method returns false.
  */
  virtual OdResult drawableToPRC(const OdGiDrawable *pDrawable,
    const PDF2PRCExportParams *pPdfPrcParams,
    OdRxObject *context
  ) const;

 virtual OdResult drawableToPRC(const OdGiDrawable *pDrawable,
    const PDF2PRCExportParams *pPdfPrcParams, OdRxObject *context, 
    OdRxObject* pParent, OdRxObject *pPartDefinition, bool hasRefs
  ) const;

  /** \details 
  Retrieves the current color used for exporting a Gi drawable.
  \param pDrawable [in] A raw pointer to the drawable to export.
  \returns Returns an OdCmEntityColor object that contains information about the currently used color.
  */
  virtual OdCmEntityColor getColor(const OdGiDrawable *pDrawable) const;
  
  /** \details 
  Retrieves the current material used for exporting a Gi drawable.
  \param pDrawable [in] A raw pointer to the drawable to export.
  \returns Returns a raw pointer to an OdDbStub object that contains information about the currently used material.
  */
  virtual OdDbStub * getMaterial(const OdGiDrawable *pDrawable) const;

  /** \details 
  Retrieves the current transparency level for exporting a Gi drawable.
  \param pDrawable [in] A raw pointer to the drawable to export.
  \returns Returns an OdCmTransparency object that contains information about the current transparency level.
  */
  virtual OdCmTransparency getTransparency(const OdGiDrawable *pDrawable) const;
};

/** \details 
  A data type that represents a smart pointer to an PdfExportGiDrawablePE object.
*/
typedef OdSmartPtr<PdfExportGiDrawablePE> PdfExportGiDrawablePEPtr;

/** \details 
A base class for protocol extension that implements the functionality for exporting PDF layers.

<group OdExport_Classes>
*/
class PDFEXPORT_DLL PdfExportLayerPE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(PdfExportLayerPE);

  /** \details 
  Retrieves the current color used for exporting a layer to PDF format.
  \param pDrawable [in] A raw pointer to the drawable entity to export.
  \returns Returns an OdCmEntityColor object that contains information about the currently used color.
  */
  virtual OdCmEntityColor getColor(const OdRxObject *pDrawable) const;
  
  /** \details 
  Retrieves the current material used for exporting a layer to PDF format.
  \param pDrawable [in] A raw pointer to the drawable to export.
  \returns Returns a raw pointer to an OdDbStub object that contains information about the currently used material.
  */
  virtual OdDbStub * getMaterial(const OdRxObject *pDrawable) const;
  
  /** \details 
  Retrieves the current transparency level for exporting a layer to PDF format.
  \param pDrawable [in] A raw pointer to the drawable to export.
  \returns Returns an OdCmTransparency object that contains information about the current transparency level.
  */
  virtual OdCmTransparency getTransparency(const OdRxObject *pDrawable) const;
};

/** \details 
  A data type that represents a smart pointer to an PdfExportLayerPE object.
*/
typedef OdSmartPtr<PdfExportLayerPE> PdfExportLayerPEPtr;

#endif // _PDF_EXPORT_GIDRAWABLE_PE_INCLUDED_
