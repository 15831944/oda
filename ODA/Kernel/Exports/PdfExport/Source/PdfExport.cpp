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
#include "PdfExport.h"
#include "PdfExportPE.h"
#include "PdfAux.h"
#include "PdfExportParams.h"
#include "Pdf2dExportDevice.h"
#include "PdfPageDictionary.h"
#include "PdfPageNodeDictionary.h"
#include "PdfContentStream.h"

#include "RxDispatchImpl.h"
#include "RxDynamicModule.h"
#include "ColorMapping.h"
#include "OdError.h"

#include "MemoryStream.h"

#include "PdfExportImpl.h"
#include "PdfExportImplXObject.h"

#include "PdfExportParamsForXObject.h"
#include "Gs/GsBaseModel.h"

namespace TD_PDF_2D_EXPORT{
ODRX_NO_CONS_DEFINE_MEMBERS(OdPdfExport, OdRxObject)

class PdfExporter : public OdPdfExport
{
public:
  PdfExporter() { ::odgsInitialize(); }; //Initialize Gs to prevent destroying GsModel in 2dDevice
  virtual ~PdfExporter() { ::odgsUninitialize(); };
  OdUInt32 exportPdf(PDFExportParams &pParams);
  OdString exportPdfErrorCode(OdUInt32 errorCode);
  OdUInt32 publish3d(PDFExportParams &pParams, PRCStreamsMap &outStreamsPRC);
  OdUInt32 export2XObject(PDFExportParams &pParams, PdfExportParamsForXObject* pPublishParams);
};
typedef OdSmartPtr<PdfExporter> OdPdfExporterPtr;

bool canUseGsCache(const PDFExportParams &pParams)
{
  return pParams.useGsCache() &&
    (pParams.getPRCMode() == PDFExportParams::PRCSupport::kDisabled);
}

OdUInt32 PdfExporter::exportPdf(PDFExportParams &pParams)
{
  OdUInt32 result = TD_PDF::eOk;
  if (!pParams.database() && !pParams.databases().size())
    return exNullDatabase;

  // Create PDF object tree
  CPdfExportImpl pdfExport;
  try
  {
    PDFResultEx res;
    res = pdfExport.init(pParams);
    if (res == exOk)
    {
      res = pdfExport.run();
      if (res == exOk)
      {
        bool bWriteFile = true;
        OdPrcContextForPdfExport* prcContext = pParams.getPRCContext();
        if (prcContext && prcContext->isKindOf(OdPrcContextForPdfExportWrapper::desc()))
          bWriteFile = ((OdPrcContextForPdfExportWrapper *)prcContext)->getWritePdfFile();

        if (bWriteFile)
          res = pdfExport.exportFile();
      }
    }
    result = res;
  }
  // hold all known exceptions from PDFToolkit
  catch (const PdfException &ex)
  {
    result = ex.m_uErrorCode;
  }
  // hold all known exceptions from PDF export
  catch (const PdfExceptionEx &ex)
  {
    result = ex.m_uErrorCode;
  }
  // hold all unknown exceptions
  catch (const OdError &ex)
  {
    result = exOdError | (ex.code() & 0xFFFF);
  }
  catch (...)
  {
    result = exInternalError;
  }
  return result;
}

OdString PdfExporter::exportPdfErrorCode(OdUInt32 errorCode)
{
  switch (errorCode)
  {
    #define PDF_ERROR_MESSAGE_DEFS(a, b) case a: return b;
    #include "PdfResultExCodes.h"
  };

  switch (errorCode)
  {
    #define PDF_ERROR_MESSAGE_DEFS(a, b) case TD_PDF::a: return b;
    #include "PdfResultCodes.h"
  };

  if (errorCode & 0x20000)
  {
    errorCode &= ~0x20000;
    OdError dderror( (OdResult)errorCode );

    OdString str = dderror.description();
    if (!str.isEmpty())
      return str;
  }

  return OD_T("Undefined error.");
}

void fillPdfExportParamsForPRC(const PDFExportParams &inpParams, PDFExportParams &outParams)
{
  outParams.setDatabase(inpParams.database());
  outParams.setVersion(PDFExportParams::currentVersion);
  outParams.setOutput(OdMemoryStream::createNew()); // tmp stream

  outParams.setExportFlags(PDFExportParams::PDFExportFlags(PDFExportParams::kDefault));

  outParams.setPRCMode(inpParams.getPRCMode() == PDFExportParams::kAsBrep ? PDFExportParams::kAsBrep : PDFExportParams::kAsMesh);
  PDF3D_ENUMS::PRCCompressionLevel compressLev;
  bool hasBrepCompression = inpParams.hasPrcBrepCompression(compressLev);
  outParams.setPRCCompression(compressLev, hasBrepCompression, inpParams.hasPrcTessellationCompression());

  OdPrcContextForPdfExportWrapperPtr pPrcContextWrap = OdPrcContextForPdfExportWrapper::createObject();
  pPrcContextWrap->setUserContext(inpParams.getPRCContext());
  outParams.setPRCContext(pPrcContextWrap);

  outParams.setPalette(inpParams.palette());
}

OdUInt32 PdfExporter::publish3d(PDFExportParams &pParams, PRCStreamsMap &outStreamsPRC)
{
  PDFExportParams paramsForPublish;
  fillPdfExportParamsForPRC(pParams, paramsForPublish);
  OdUInt32 result = TD_PDF::eOk;
  result = exportPdf(paramsForPublish);
  if (paramsForPublish.getPRCContext()->isKindOf(OdPrcContextForPdfExportWrapper::desc()))
  {
    outStreamsPRC = ((OdPrcContextForPdfExportWrapper *)paramsForPublish.getPRCContext())->getOutputPRC();
  }

  return result;
}

OdUInt32 PdfExporter::export2XObject(PDFExportParams &pParams, PdfExportParamsForXObject* pExportParams)
{
  if (NULL == pExportParams)
    return exOdError | (eNullObjectPointer & 0xFFFF);
  OdUInt32 result = TD_PDF::eOk;

  if (!pParams.export2XObject())
    pParams.setExport2XObject(true);

  // Create PDF object tree
  CPdfExportImplXObject pdfExport(pExportParams->m_pCurrentPage, pExportParams->m_pXobjectForm, pExportParams->m_FontOptimizer);
  try
  {
    PDFResultEx res;
    res = pdfExport.init(pParams);
    if (res == exOk)
    {
      res = pdfExport.run();
    }
    result = res;
  }
  // hold all known exceptions from PDFToolkit
  catch (const PdfException &ex)
  {
    result = ex.m_uErrorCode;
  }
  // hold all known exceptions from PDF export
  catch (const PdfExceptionEx &ex)
  {
    result = ex.m_uErrorCode;
  }
  // hold all unknown exceptions
  catch (const OdError &ex)
  {
    result = exOdError | (ex.code() & 0xFFFF);
  }
  catch (...)
  {
    result = exInternalError;
  }
  return result;
}

class OdPdfExportModule : public PdfExportModule
{
public:
  virtual void initApp()
  {
    OdPdfExport::rxInit();
    OdPdfExportPE::rxInit();
  }
  virtual void uninitApp()
  {
    OdPdfExportPE::rxUninit();
    OdPdfExport::rxUninit();
  }

  virtual OdPdfExportPtr create ()
  {
    return OdRxObjectImpl<PdfExporter>::createObject();
  }

};
}
using namespace TD_PDF_2D_EXPORT;
ODRX_DEFINE_DYNAMIC_MODULE(OdPdfExportModule);
