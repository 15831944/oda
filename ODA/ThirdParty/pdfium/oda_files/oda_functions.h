/////////////////////////////////////////////////////////////////////////////// 
// Created by Open Design Alliance 
///////////////////////////////////////////////////////////////////////////////

#ifndef _OD_PDFIUM_FUNCTIONS_INCLUDED_
#define _OD_PDFIUM_FUNCTIONS_INCLUDED_

#include "public/fpdfview.h"
#include "oda_files/PdfImportBaseObjectGrabber.h"
#include <vector>

typedef void* FPDF_ARRAY;
typedef void* FPDF_PageRenderContext;

typedef struct {
  int   nol;
  wchar_t  names[1024][512];
} FPDF_PageLayerInfo;

enum PdfImportOptions
{
  kImportTTFontAsGeometry         = 0x00001, 
  kImportType3FontAsTT            = 0x00002, 
  kImportInvalidFontAsGeometry    = 0x00004,
  kImportWidgets                  = 0x00008,
};

struct FPDF_PageMeasureDictInfo
{
  float measure_factor;

  struct
  {
    float left;
    float bottom;
    float right;
    float top;
  } BBox;
};

//////////////////////////////////////////////////////////////////////////
FPDF_EXPORT void FPDF_CALLCONV FPDF_GetMeasureDictInfo(FPDF_DOCUMENT document, int page_index, std::vector<FPDF_PageMeasureDictInfo>& measure_info);

FPDF_EXPORT bool FPDF_CALLCONV FPDF_LoadLayers(FPDF_DOCUMENT document, FPDF_ARRAY& pLayers, FPDF_ARRAY& pONLayers, FPDF_ARRAY& pOFFLayers);

FPDF_EXPORT bool FPDF_CALLCONV FPDF_GetPageLayers(FPDF_DOCUMENT document, FPDF_PAGE page, int page_index, FPDF_PageLayerInfo& info);

FPDF_EXPORT bool FPDF_CALLCONV FPDF_EnableLayer(FPDF_ARRAY pLayers, FPDF_ARRAY pONLayers, FPDF_ARRAY pOFFLayers, const wchar_t* pName, bool bEnable);

FPDF_EXPORT bool FPDF_CALLCONV FPDF_IsLayerEnabled(FPDF_ARRAY pOFFLayers, const wchar_t* pName);

FPDF_EXPORT void FPDF_CALLCONV FPDF_SetPageRenderContext(FPDF_PAGE page, FPDF_PageRenderContext context);

FPDF_EXPORT void FPDF_CALLCONV FPDF_ImportPage(FPDF_PAGE page, OdPdfImportBaseObjectGrabber* grabber, long options);

//////////////////////////////////////////////////////////////////////////

#endif // _OD_PDFIUM_FUNCTIONS_INCLUDED_
