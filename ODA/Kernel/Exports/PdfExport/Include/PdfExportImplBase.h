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
//  PdfExportImplBase.h : definition of the CPdfExportImplBase class
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PDFEXPORTIMPL_BASE_DEFINED
#define PDFEXPORTIMPL_BASE_DEFINED

#include "Gs/Gs.h"

#include "PdfType3Optimizer.h"
#include "PdfExportParamsHolder.h"

using namespace TD_PDF;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT {

  class PDF2dExportDevice;

class CPdfExportImplBase
{
public:
  virtual ~CPdfExportImplBase();

  // Implementation

  /** \details
  Initiates PDF export with specified parameters.

  \remarks
  Initiates PDF export with specified parameters (passed in the pParams parameter). Returns an instance of the PDFResultEx class as the result of the initiantion.

  \param pParams [in]  Reference to the PDFExportParams class instance containing export parameter values.
  */
  virtual PDFResultEx init(const PDFExportParams &pParams) = 0;

  /** \details
  Runs export to PDF format for the current document.

  \remarks
  Initiates PDF export with specified parameters (passed in the pParams parameter). Returns an instance of the PDFResultEx class as the result of the initiantion.
  */
  virtual PDFResultEx run() = 0;

protected:
  class DatabasesHolder
  {
  public:
    void startDbTransaction(OdDbBaseDatabase* pDb, bool bSaveGsModel);
    void saveGsModel(OdGiDrawable* pDbAsDrawable);
    void abortTransactions();
    ~DatabasesHolder() { abortTransactions(); }

  private:
    typedef std::map<OdDbBaseDatabase*, SavedGsModelData> _mmap;
    _mmap m_data;
  };

protected:
  /** \details
  Applies the /scale factor/ for displaying Lineweights in the Viewport object.

  \remarks
  Applies the /scale factor/ for displaying Lineweights in the Viewport object associated with PDF export class.
  */
  void applyLineweightToDcScale(const PDFExportParams& prms);

  /** \details
  set measuring viewport into pdf
  */
  void setMeasuringViewport(PdfExportParamsHolder& paramsHolder,
    const OdGsDCRect& clipBox,
    const OdGeBoundBlock3d* ext = 0,
    const OdGsPageParams* pPageParams = 0);

  void setupPdfRenderDevices(const PDFExportParams& prms,
    OdGiDefaultContext* pCtx, PDF2dExportDevice& pdfDevice,
    OdDbStub* objectId, OdGsDCRect& clipBox, const OdGeBoundBlock3d& extents);

  OdGsDevicePtr setupPdfRenderDevice(const PDFExportParams& prms,
    OdDbBaseDatabasePE* pDbPE,
    OdGiDefaultContext* pCtx, OdGsDevice& renderDevice,
    bool bZ2E, OdUInt32 extentsFlag,
    OdDbStub* objectId, OdGsDCRect& clipBox,
    const OdGeBoundBlock3d& extents);

  //apply layout settings depending on Z2E mode
  void setupPdfLayout(PdfExportParamsHolder& paramsHolder,
    OdGsPageParams& pPageParams, OdGiDefaultContextPtr pCtx,
    PDF2dExportDevice* pDevicePdf, OdDbStub* objectId = 0);

  PDFResultEx base_run(PdfExportParamsHolder& paramsHolder, bool isExport2XObj);

  CPdfExportImplBase();
  PDFResultEx startTransactions(const PDFExportParams &pParams);

protected:
  OdGsDevicePtr       m_pDevice;        //Pointer to device used for the PDF export.
  PDFType3Optimizer   m_Type3Optimizer; // Type optimization options for a PDF document.
  typedef std::map<OdRxObject*, SavedGsModelData> DbToSavedGsModel;
  DatabasesHolder m_dbHolder;
};

}
#endif //#define PDFEXPORTIMPL_BASE_DEFINED
