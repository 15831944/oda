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
#include "OdaCommon.h"
#include "ColorMapping.h"

#include "PdfiumWrapperImpl.h"
#include "FlatMemStream.h"
#include "DynamicLinker.h"
#include "Ge/GeExtents2d.h"
#include "Gi/GiViewportDraw.h"
#include "GiDefaultContext.h"
#include "OdColorAdjuster.h"
#include "PdfUnderlay.h"

#include "public/fpdf_annot.h"
#include "public/fpdf_transformpage.h"
#include "public/fpdf_edit.h"
#include "public/fpdf_progressive.h"

#define TOCHAR(a) ((char *)OdAnsiString(OdString(a)).c_str())

// Registration
ODRX_CONS_DEFINE_MEMBERS(PdfiumWrapperImpl, PdfDirectWrapper, RXIMPL_CONSTR);

// PdfDirectWrapper
OdSmartPtr<PdfDirectWrapper> PdfDirectWrapper::createObjectImpl()
{
  PdfDirectWrapperPtr ptrtmp = PdfiumWrapperImpl::createObject();
  return ptrtmp;
}

void PdfDirectWrapper::rxInitWrapper()
{
  if (odrxClassDictionary()->has(L"OdDbPdfUnderlayItemPE")) {
    //Underlay module already initialized
    throw OdError(eAlreadyActive);
  }

  FPDF_InitLibrary();// Initialize the library.

  // it should be after the code with throw OdError
  PdfDirectWrapper::rxInit();
  PdfiumWrapperImpl::rxInit();
  OdDbPdfUnderlayItemPE::rxInit();
  OdDbUnderlayItem::desc()->addX(OdDbPdfUnderlayItemPE::desc(), &m_PdfItemPE);
}

void PdfDirectWrapper::rxUninitWrapper()
{
  FPDF_DestroyLibrary();

  OdDbUnderlayItem::desc()->delX(OdDbPdfUnderlayItemPE::desc());
  PdfiumWrapperImpl::rxUninit();
  PdfDirectWrapper::rxUninit();
  OdDbPdfUnderlayItemPE::rxUninit();
}

// PdfiumWrapperImpl
PdfiumWrapperImpl::PdfiumWrapperImpl()
  : m_pDocument(0)
  , m_pONLayers(0)
  , m_pOFFLayers(0)
  , m_pLayers(0)
  , m_bDrawAnnots(false)
{
  m_Deviation = 0;
  m_CountOfPages = 0;
  m_layerInfoCachePage = -1;
  desc()->module()->addRef();
}

PdfiumWrapperImpl::~PdfiumWrapperImpl()
{
  if (m_PagesMap.size() > 0)
  {
    std::map<OdUInt32, FPDF_PAGE>::iterator iter = m_PagesMap.begin();
    for (iter; iter != m_PagesMap.end(); iter++)
      FPDF_ClosePage(iter->second);
    m_PagesMap.clear();
  }
  if (m_pDocument)
  {
    FPDF_CloseDocument(m_pDocument);
    m_pDocument = 0;
  }
  m_cash.clear();
  desc()->module()->release();
}

bool PdfiumWrapperImpl::checkAnnotationsOnPage(OdUInt32 nPage) const
{
  std::map<OdUInt32, FPDF_PAGE>::iterator iter = m_PagesMap.find(nPage);
  if (iter == m_PagesMap.end())
    m_PagesMap[nPage] = FPDF_LoadPage(m_pDocument, nPage);

  OdArray<bool>::size_type annotationsSize = m_Annotations.size();
  if (annotationsSize <= nPage)
  {
    m_Annotations.resize(nPage+1, false);
    for (OdArray<bool>::size_type f = annotationsSize; f <= nPage; ++f)
      m_Annotations[f] = 0 < FPDFPage_GetAnnotCount(m_PagesMap[nPage]);
  }
  return m_Annotations[nPage];
}

OdResult PdfiumWrapperImpl::loadFile(OdString path, OdString password)
{
  m_path = path;
  m_password = password;

  if (m_PagesMap.size() > 0)
  {
    std::map<OdUInt32, FPDF_PAGE>::iterator iter = m_PagesMap.begin();
    for (iter; iter != m_PagesMap.end(); iter++)
      FPDF_ClosePage(iter->second);
    m_PagesMap.clear();
  }

  if (m_pDocument)
    FPDF_CloseDocument(m_pDocument);
  m_pDocument = FPDF_LoadDocument(m_path.c_str(), TOCHAR(m_password.c_str()));
  if (m_pDocument == NULL)
  {
    unsigned long error = FPDF_GetLastError();
    if (FPDF_ERR_PASSWORD == error)
      return eInvalidPassword;
    return eCantOpenFile;
  }

  m_CountOfPages = FPDF_GetPageCount(m_pDocument);

  //for (int i = 0; i < m_CountOfPages; i++)
  //  m_Pages.append(FPDF_LoadPage(m_pDocument, i));

  m_Extents.resize(m_CountOfPages, OdGePoint2d::kOrigin);

  if (!FPDF_LoadLayers(m_pDocument, m_pLayers, m_pONLayers, m_pOFFLayers))
    return eLoadFailed;

  return eOk;
}

OdGiRasterImagePtr PdfiumWrapperImpl::getThumbnail(int nPage, int pic_width, int pic_height) const
{
  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return OdGiRasterImagePtr();

  try
  {
    // 170 is ARX default
    //int pic_width = 170, pic_height = 170;

    std::map<OdUInt32, FPDF_PAGE>::iterator iter = m_PagesMap.find(nPage);
    if (iter == m_PagesMap.end())
      m_PagesMap[nPage] = FPDF_LoadPage(m_pDocument, nPage);

    double page_width = FPDF_GetPageWidth(m_PagesMap[nPage]);
    double page_height = FPDF_GetPageHeight(m_PagesMap[nPage]);
    if (page_width >= page_height)
      pic_height *= page_height / page_width;
    else
      pic_width *= page_width / page_height;
    
    FPDF_BITMAP bitmap = FPDFBitmap_Create(pic_width, pic_height, 1);
    FPDFBitmap_FillRect(bitmap, 0, 0, pic_width, pic_height, 0xFFFFFFFF);
    int flags = FPDF_ANNOT | FPDF_RENDER_NO_SMOOTHIMAGE | FPDF_RENDER_NO_SMOOTHPATH;
    FPDF_RenderPageBitmap(bitmap, m_PagesMap[nPage], 0, 0, pic_width, pic_height, 0, flags);

    BITMAPINFOHEADER bmiHeader;
    bmiHeader.biSize = 40;
    bmiHeader.biWidth = pic_width;
    bmiHeader.biHeight = -pic_height;  // top-down image
    bmiHeader.biPlanes = 1;
    bmiHeader.biBitCount = 32;
    bmiHeader.biCompression = 0L; //BI_RGB;
    bmiHeader.biSizeImage = FPDFBitmap_GetStride(bitmap)*pic_height;
    unsigned char *imagedata = (unsigned char *)FPDFBitmap_GetBuffer(bitmap);

    if (!imagedata)
      return OdGiRasterImagePtr();
    else
    {
      FPDF_FORMFILLINFO formInfo = {};
      formInfo.version = 1;
      FPDF_FORMHANDLE fHandle = FPDFDOC_InitFormFillEnvironment(m_pDocument, &formInfo);

      FPDF_FFLDraw(fHandle, bitmap, m_PagesMap[nPage], 0, 0, pic_width, pic_height, 0, 0);
      FPDFDOC_ExitFormFillEnvironment(fHandle);
    }

    OdGiRasterImagePtr pTmp = convertBmpToRaster(bmiHeader, imagedata);

    FPDFBitmap_Destroy(bitmap);
    return pTmp;
  }
  catch(...) 
  {
  }
  return OdGiRasterImagePtr();
}


int PdfiumWrapperImpl::getItemsCount() const
{
  return m_CountOfPages;
}

OdString PdfiumWrapperImpl::getItemName(int nPage) const
{
  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return OdString::kEmpty;

  return OdString().format(OD_T("%d"), nPage+1);
}

const OdGePoint2d &PdfiumWrapperImpl::getPageSize(int nPage) const
{
  OdGePoint2d &pCurExtents = m_Extents[nPage];

  std::map<OdUInt32, FPDF_PAGE>::iterator iter = m_PagesMap.find(nPage);
  if (iter == m_PagesMap.end())
    m_PagesMap[nPage] = FPDF_LoadPage(m_pDocument, nPage);

  if (pCurExtents.x == 0 && pCurExtents.y == 0 && nPage < m_CountOfPages && 0 != m_PagesMap[nPage])
  {
    pCurExtents.x = FPDF_GetPageWidth(m_PagesMap[nPage]);
    pCurExtents.y = FPDF_GetPageHeight(m_PagesMap[nPage]);
  }
  return pCurExtents;
}

OdResult PdfiumWrapperImpl::getMediaBox(int nPage, OdGePoint2d& minc, OdGePoint2d& maxc) const
{
  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return eInvalidInput;

  std::map<OdUInt32, FPDF_PAGE>::iterator iter = m_PagesMap.find(nPage);
  if (iter == m_PagesMap.end())
    m_PagesMap[nPage] = FPDF_LoadPage(m_pDocument, nPage);

  float min_x = 0, min_y = 0, max_x = 0, max_y = 0;
  int res = FPDFPage_GetMediaBox(m_PagesMap[nPage], &min_x, &min_y, &max_x, &max_y);
  if (!res)
    return eFileInternalErr;

  int rotation = FPDFPage_GetRotation(m_PagesMap[nPage]);//pPage->GetPageRotation();

  if (rotation == 1 /*90*/ || rotation == 3 /*270*/) 
  {
    minc.x = min_y;
    minc.y = min_x;
    maxc.x = max_y;
    maxc.y = max_x;
  }
  else
  {
    minc.x = min_x;
    minc.y = min_y;
    maxc.x = max_x;
    maxc.y = max_y;
  }
  return eOk;
}

void PdfiumWrapperImpl::getItemExtents(int nPage, OdGePoint2d& minc, OdGePoint2d& maxc) const
{
  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return;
  const OdGePoint2d &pCurExtents = getPageSize(nPage);

  minc.set(0, 0);
  double xScale = getMeasureDictInfo(nPage);
  xScale *= 72.;
  maxc.set(pCurExtents.x / 72. * xScale, pCurExtents.y / 72. * xScale);
}

void PdfiumWrapperImpl::setupLayers(const OdArray<OdUnderlayLayer> &layers) const
{
  for (OdUInt32 f = 0; f < layers.size(); ++f)
  {
    OdAnsiString tmp( layers[f].name() );
    bool isOn = layers[f].state() == OdUnderlayLayer::kOn;

    if (tmp == "Comments and Markups")
      m_bDrawAnnots = isOn;
    else if (m_pLayers && m_pONLayers && m_pOFFLayers)
    {
      FPDF_EnableLayer(m_pLayers, m_pONLayers, m_pOFFLayers, layers[f].name().c_str(), isOn);
    }
  }
}

//#define PDF_DEBUG
#ifdef PDF_DEBUG
#include "RxRasterServices.h"
#endif

namespace FxColorTransform
{
  void BGRToHSL(const OdUInt8* BGR, double* HSL)
  {
    const OdUInt8& R = BGR[2];
    const OdUInt8& G = BGR[1];
    const OdUInt8& B = BGR[0];

    double& H = HSL[0];
    double& S = HSL[1];
    double& L = HSL[2];

    double var_R = (R / 255.0); //RGB from 0 to 255
    double var_G = (G / 255.0);
    double var_B = (B / 255.0);

    double var_Min = odmin(var_R, odmin(var_G, var_B)); //Min. value of RGB
    double var_Max = odmax(var_R, odmax(var_G, var_B)); //Max. value of RGB
    double del_Max = var_Max - var_Min; //Delta RGB value

    L = (var_Max + var_Min) / 2.0;

    if (del_Max == 0.0) //This is a gray, no chroma...
    {
      H = 0.0; //HSL results from 0 to 1
      S = 0.0;
    }
    else //Chromatic data...
    {
      if (L < 0.5) S = del_Max / (var_Max + var_Min);
      else           S = del_Max / (2 - var_Max - var_Min);

      double del_R = (((var_Max - var_R) / 6.0) + (del_Max / 2.0)) / del_Max;
      double del_G = (((var_Max - var_G) / 6.0) + (del_Max / 2.0)) / del_Max;
      double del_B = (((var_Max - var_B) / 6.0) + (del_Max / 2.0)) / del_Max;

      if (var_R == var_Max) H = del_B - del_G;
      else if (var_G == var_Max) H = (1.0 / 3.0) + del_R - del_B;
      else if (var_B == var_Max) H = (2.0 / 3.0) + del_G - del_R;

      if (H < 0.0) H += 1.0;
      if (H > 1.0) H -= 1.0;
    }
  }

  double HueToRGB(double v1, double v2, double vH)
  {
    if (vH < 0.0) vH += 1.0;
    if (vH > 1.0) vH -= 1.0;

    if (6.0 * vH < 1.0) return (v1 + (v2 - v1) * 6.0 * vH);
    if (2.0 * vH < 1.0) return (v2);
    if (3.0 * vH < 2.0) return (v1 + (v2 - v1) * (2.0 / 3.0 - vH) * 6.0);
    return (v1);
  }

  void HSLToBGR(const double* HSL, OdUInt8* BGR)
  {
    const double& H = HSL[0];
    const double& S = HSL[1];
    const double& L = HSL[2];

    OdUInt8& R = BGR[2];
    OdUInt8& G = BGR[1];
    OdUInt8& B = BGR[0];

    if (S == 0.0) //HSL from 0 to 1
    {
      R = L * 255; //RGB results from 0 to 255
      G = L * 255;
      B = L * 255;
    }
    else
    {
      double var_1 = 0.0, var_2 = 0.0;

      if (L < 0.5) var_2 = L * (1.0 + S);
      else           var_2 = (L + S) - (S * L);

      var_1 = 2.0 * L - var_2;

      R = 255.0 * HueToRGB(var_1, var_2, H + (1.0 / 3.0));
      G = 255.0 * HueToRGB(var_1, var_2, H);
      B = 255.0 * HueToRGB(var_1, var_2, H - (1.0 / 3.0));
    }
  }
}

OdResult calibrateColors(unsigned char *buf, unsigned int bufLen, OdInt32 width, OdInt32 height, ODCOLORREF backgroundColor, const OdDbUnderlayDrawContext& context)
{
#if 1
  // init OdColorAdjuster
  bool isDwgLightBackground = odcmIsBackgroundLight(backgroundColor);

  OdColorAdjuster dca(context.monochrome, isDwgLightBackground ? context.contrast - 111 : context.contrast, context.fade, backgroundColor);
  bool isNeedColorRecompution = dca.isNeedColorRecompution();

  OdUInt32 counter = 0;
  OdUInt8 *pBGRA = buf;
  // convert colors (if necessary) but not alpha channel.
  while (counter < bufLen)
  {
    if (context.adjustColorForBackground)
    {
      if (pBGRA[3] != 0 && !isDwgLightBackground)
      {
        double HSL[3];
        FxColorTransform::BGRToHSL(pBGRA, HSL);
        HSL[2] = 1.0 - HSL[2];
        FxColorTransform::HSLToBGR(HSL, pBGRA);
      }
    }
    if (isNeedColorRecompution)
    {
      dca.computeShadedColorRGB(pBGRA[2], pBGRA[1], pBGRA[0]);
    }
    pBGRA += 4;
    counter += 4;
  }
  return eOk;
#else
  // init OdColorAdjuster
  bool bIsBackgroundLight = context.contrast >= 111;

  OdColorAdjuster dca(context.monochrome, bIsBackgroundLight ? context.contrast - 111 : context.contrast, context.fade, backgroundColor);
  bool isNeedColorRecompution = dca.isNeedColorRecompution();

  OdUInt32 counter = 0;
  OdUInt8 *pCurBGRA = buf;
  // convert colors (if necessary) but not alpha channel.
  while (counter < bufLen)
  {
    if (context.adjustColorForBackground)
    {
      if (bIsBackgroundLight && pCurBGRA[0] == 254 && pCurBGRA[1] == 254 && pCurBGRA[2] == 254)
      {
        pCurBGRA[0] = pCurBGRA[1] = pCurBGRA[2] = 255;
      }
      else if (!bIsBackgroundLight && pCurBGRA[0] == 1 && pCurBGRA[1] == 1 && pCurBGRA[2] == 1)
      {
        pCurBGRA[0] = pCurBGRA[1] = pCurBGRA[2] = 0;
      }
      else
      {
        if (!bIsBackgroundLight && pCurBGRA[0] == 0 && pCurBGRA[1] == 0 && pCurBGRA[2] == 0)
        {
          pCurBGRA[2] = 255;
          pCurBGRA[1] = 255;
          pCurBGRA[0] = 255;
        }
        else if (!bIsBackgroundLight && pCurBGRA[0] == 255 && pCurBGRA[1] == 255 && pCurBGRA[2] == 255)
        {
          pCurBGRA[2] = 0;
          pCurBGRA[1] = 0;
          pCurBGRA[0] = 0;
        }
      }
      if (isNeedColorRecompution)
        dca.computeShadedColorRGB(pCurBGRA[2], pCurBGRA[1], pCurBGRA[0]);
    }
    else
    {
      if (isNeedColorRecompution)
        dca.computeShadedColorRGB(pCurBGRA[2], pCurBGRA[1], pCurBGRA[0]);
    }
    pCurBGRA += 4;
    counter += 4;
  }
  return eOk;
#endif
}

OdResult PdfiumWrapperImpl::getImage(ImageCache& pdfInfo, int nPage, OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context, OdGeExtents2d& clipRect,
                                 double DPI, int /*ImagePixW*/, int /*ImagePixH*/) const
{
  std::map<OdUInt32, FPDF_PAGE>::iterator iter = m_PagesMap.find(nPage);
  if (iter == m_PagesMap.end())
    m_PagesMap[nPage] = FPDF_LoadPage(m_pDocument, nPage);

  const OdGePoint2d &pCurExtents = getPageSize(nPage);
  ODCOLORREF backgroundColor = 0;
  OdGiDefaultContextPtr pCtx = OdGiDefaultContext::cast(pVd->context());
  if (pCtx.get())
    backgroundColor = pCtx->paletteBackground();
  else
    ODA_ASSERT(0);

  if (pdfInfo.m_dpi < DPI && !pVd->context()->isPlotGeneration() && pdfInfo.m_dpi != 0.)
  {
    DPI *= 1.25;//when zooming in we need the higher DPI to avoid quality degrading.
    pdfInfo.m_dpi = DPI * 1.25;//I guess it gives us more performance with less image quality degradation
  }
  else
    pdfInfo.m_dpi = DPI;//when zooming out or no zooming, DPI doesn't matter, so we don't increase it

  pdfInfo.m_ext = clipRect;
  pdfInfo.m_drawCtx = context;
  pdfInfo.m_nPage = nPage;

  setupLayers(context.layers);

  // Set some background color
  // This value is a random background color. But antialiasing effect depends of nDefColor whiteness.
  // kPDPageDoLazyErase is disabled - bitmap background clearing disabled.
  // Note that nDefColor can be black or white (0,0,0 or 255,255,255) only
  OdUInt8 nDefColor = 255;

  // at black background with adjustColorForBackground = true
  // artifacts possible
  //bool fullAntialiasing = true;

  // fill background with black or white to true antialiasing effect.
  // Note that PDF generates BGR bitmap (3 bytes, not BGRA).
  OdDbUnderlayDrawContext contextWithPaleteFlag = context;
  if (pCtx.get())
  {
    pdfInfo.m_backgroundColor = backgroundColor;
    if (odcmIsBackgroundLight(backgroundColor))
    {
      // code to send palette flag (black or white palette)
      contextWithPaleteFlag.contrast += 111;
    }
    else
    {
      nDefColor = 0;
    }

    // init nDefColor with 1 or 254 is a potential problem. All pixels with nDefColor color will be transparent.
    // Fix of this problem requires two renderings with different bg colors and creation of transparency mask.
    if (contextWithPaleteFlag.adjustColorForBackground)
    {
      if (odcmIsBackgroundLight(backgroundColor))
        nDefColor = 255;
      else
        nDefColor = 0;
    }
  }
  OdUInt32 bgcolor = nDefColor | nDefColor << 8 | nDefColor << 16 | 0x00 << 24;

  if (pVd->context()->isPlotGeneration())
  { //picture may occur too big because of hi dpi for plot, so we try to slice it to several pieces
    //calculate result picture size:
    OdUInt64 resW = (clipRect.maxPoint().x - clipRect.minPoint().x) * DPI / 72.;
    OdUInt64 resH = (clipRect.maxPoint().y - clipRect.minPoint().y) * DPI / 72.;
    OdUInt64 resSize = resH * resW * 4; //3 byte per pixel + alpha channel
    OdUInt32 numParts = (OdUInt32)((double)resSize / 90000000.) + 1; //90000000. - it is about the limit of picture size to be created
    if (numParts > 1)
    {
      return sliceImage(contextWithPaleteFlag, clipRect, pVd, numParts, nPage, bgcolor, DPI);
    }
  }

  int flags = m_bDrawAnnots ? (FPDF_ANNOT | FPDF_RENDER_NO_SMOOTHIMAGE | FPDF_RENDER_NO_SMOOTHPATH) : (FPDF_RENDER_NO_SMOOTHIMAGE | FPDF_RENDER_NO_SMOOTHPATH);

  double dpi_ratio = DPI / 72.;//default dpi is 72. for pdf files

  int pic_width = clipRect.maxPoint().x - clipRect.minPoint().x;
  int pic_height = clipRect.maxPoint().y - clipRect.minPoint().y;

  while (pic_width*dpi_ratio * pic_height*dpi_ratio > 22000000) //It gives us a bit less than 90000000 bytes for 4 bytes-for-pixel bitmap
    dpi_ratio *= .95;

  pic_width *= dpi_ratio;
  pic_height *= dpi_ratio;

  FPDF_BITMAP bitmap = FPDFBitmap_Create(pic_width, pic_height, 1);//FPDFBitmap_CreateEx(pic_width, pic_height, FPDFBitmap_BGR, 0, 0);
  FPDFBitmap_FillRect(bitmap, 0, 0, pic_width, pic_height, bgcolor);

  IFSDK_PAUSE pause;
  pause.version = 1;
  pause.NeedToPauseNow = [](struct _IFSDK_PAUSE* pThis) -> FPDF_BOOL
  {
    return true;
    OdGiViewportDraw* pVd = (OdGiViewportDraw*)pThis->user;
    if (pVd)
      return pVd->regenAbort();//(true);
    else
      return false;
  };
  pause.user = pVd;
  int result = FPDF_RenderPageBitmap_Start(bitmap, m_PagesMap[nPage],
    -clipRect.minPoint().x*dpi_ratio,
    clipRect.maxPoint().y*dpi_ratio - (int)pCurExtents.y*dpi_ratio, //start y-coordinate is inverted (top coordinate instead of down in OdGeExtents2d)
    (int)pCurExtents.x*dpi_ratio,
    (int)pCurExtents.y*dpi_ratio, 0, flags, &pause);

  while (result == FPDF_RENDER_TOBECOUNTINUED)
  {
    if (pVd->regenAbort())
      break;
    result = FPDF_RenderPage_Continue(m_PagesMap[nPage], &pause);
  }

  if (result == FPDF_RENDER_DONE)
  {
    FPDF_FORMFILLINFO formInfo = {};
    formInfo.version = 1;
    FPDF_FORMHANDLE fHandle = FPDFDOC_InitFormFillEnvironment(m_pDocument, &formInfo);

    FPDF_FFLDraw(fHandle, bitmap, m_PagesMap[nPage], -clipRect.minPoint().x*dpi_ratio,
      clipRect.maxPoint().y*dpi_ratio - (int)pCurExtents.y*dpi_ratio, //start y-coordinate is inverted (top coordinate instead of down in OdGeExtents2d)
      (int)pCurExtents.x*dpi_ratio,
      (int)pCurExtents.y*dpi_ratio, 0, flags);
    FPDFDOC_ExitFormFillEnvironment(fHandle);
  }

  FPDF_RenderPage_Close(m_PagesMap[nPage]);
  if (result != FPDF_RENDER_DONE)
  {
    pdfInfo.m_pImage = 0;
    pdfInfo.m_ext = OdGeExtents2d();
    if (bitmap)
      FPDFBitmap_Destroy(bitmap);
    return eCreateFailed;
  }

  BITMAPINFOHEADER bmiHeader;
  bmiHeader.biSize = 40;
  bmiHeader.biWidth = pic_width;
  bmiHeader.biHeight = -pic_height;//top-down image
  bmiHeader.biPlanes = 1;
  bmiHeader.biBitCount = 32;
  bmiHeader.biCompression = 0L; //BI_RGB;
  bmiHeader.biSizeImage = FPDFBitmap_GetStride(bitmap)*pic_height;
  unsigned char *imagedata = (unsigned char *)FPDFBitmap_GetBuffer(bitmap);

  if (!imagedata)
  {
    pdfInfo.m_ext = OdGeExtents2d();
    return eNullPtr;
  }

  // allocate 4 bytes per pixel (FLYSdk needs only 3) to future conversion from BGR to RGBA.
  OdUInt8Array buffer;
  buffer.resize(bmiHeader.biSizeImage, 255);

  memcpy(buffer.asArrayPtr(), imagedata, bmiHeader.biSizeImage);

#ifdef PDF_DEBUG
  {
    OdGiRasterImagePtr rast1;
    rast1 = convertBmpToRaster(bmiHeader, imagedata);
    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if (!pRasSvcs.isNull())
      pRasSvcs->saveRasterImage(rast1, "f:\\pdfium1.bmp");// > loadRasterImage(pMem);
  }
#endif

  OdGiRasterImagePtr rast;
  OdResult res = calibrateColors(imagedata, bmiHeader.biSizeImage, Od_abs(bmiHeader.biWidth), Od_abs(bmiHeader.biHeight), backgroundColor, contextWithPaleteFlag);
  if (res == eOk)
  {
    rast = convertBmpToRaster(bmiHeader, imagedata);

#ifdef PDF_DEBUG
    {
      OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
      if (!pRasSvcs.isNull())
        pRasSvcs->saveRasterImage(rast, "f:\\pdfium2.bmp");
    }
#endif

    pdfInfo.m_pImage = rast;
    pdfInfo.m_biWidth = bmiHeader.biWidth;
    pdfInfo.m_biHeight = bmiHeader.biHeight;
    FPDFBitmap_Destroy(bitmap);

    if (pdfInfo.m_pImage.isNull())
    {
      pdfInfo.m_ext = OdGeExtents2d();
      return eNullPtr;
    }
  }
  return eOk;
}

OdResult PdfiumWrapperImpl::sliceImage(const OdDbUnderlayDrawContext& contextWithPaleteFlag, const OdGeExtents2d& clipRect, const OdGiViewportDraw* pVd,
                                       OdUInt32 numParts, int nPage, ODCOLORREF backgroundColor, double DPI) const
{
  std::map<OdUInt32, FPDF_PAGE>::iterator iter = m_PagesMap.find(nPage);
  if (iter == m_PagesMap.end())
    m_PagesMap[nPage] = FPDF_LoadPage(m_pDocument, nPage);

  DPI /= sqrt((double)numParts); //as in PDFVI - to repeat the autocad behavior
  int flags = m_bDrawAnnots ? (FPDF_ANNOT | FPDF_RENDER_NO_SMOOTHIMAGE | FPDF_RENDER_NO_SMOOTHPATH) : (FPDF_RENDER_NO_SMOOTHIMAGE | FPDF_RENDER_NO_SMOOTHPATH);
  double dpi_ratio = DPI / 72.;
  double measureScale = getMeasureDictInfo(nPage) * 72.;
  const OdGePoint2d &pCurExtents = getPageSize(nPage);
  OdGePoint2d minP = clipRect.minPoint();
  OdGePoint2d maxP = clipRect.maxPoint();

  double step = (maxP.y - minP.y) / (double)numParts;

  maxP.y = minP.y + step;
  maxP.y = (OdInt32)(maxP.y + .5);
  OdGeExtents2d tmpClipRect(minP, maxP);
  OdGePoint3d origin = OdGePoint3d::kOrigin + OdGeVector3d::kXAxis * tmpClipRect.minPoint().x / 72. + OdGeVector3d::kYAxis * tmpClipRect.minPoint().y / 72.;
  origin *= measureScale;

  for (OdUInt32 counter = 1; counter <= numParts; counter++)
  {
    int pic_width = tmpClipRect.maxPoint().x - tmpClipRect.minPoint().x;
    int pic_height = tmpClipRect.maxPoint().y - tmpClipRect.minPoint().y;

    pic_width *= dpi_ratio;
    pic_height *= dpi_ratio;

    FPDF_BITMAP bitmap = FPDFBitmap_Create(pic_width, pic_height, 1);
    FPDFBitmap_FillRect(bitmap, 0, 0, pic_width, pic_height, backgroundColor);
    FPDF_RenderPageBitmap(bitmap, m_PagesMap[nPage], (int)(-tmpClipRect.minPoint().x*dpi_ratio),
      (int)(tmpClipRect.maxPoint().y*dpi_ratio) - (int)pCurExtents.y*dpi_ratio /*start y-coordinate is inverted (top coordinate instead of down in OdGeExtents2d)*/,
      (int)pCurExtents.x*dpi_ratio,
      (int)pCurExtents.y*dpi_ratio, 0, flags);

    BITMAPINFOHEADER bmiHeader;
    bmiHeader.biSize = 40;
    bmiHeader.biWidth = pic_width;
    bmiHeader.biHeight = -pic_height;  // top-down image
    bmiHeader.biPlanes = 1;
    bmiHeader.biBitCount = 32;
    bmiHeader.biCompression = 0L; //BI_RGB;
    bmiHeader.biSizeImage = FPDFBitmap_GetStride(bitmap)*pic_height;
    unsigned char *imagedata = (unsigned char *)FPDFBitmap_GetBuffer(bitmap);

    if (!imagedata)
      return eNullPtr;
    else
    {
      FPDF_FORMFILLINFO formInfo = {};
      formInfo.version = 1;
      FPDF_FORMHANDLE fHandle = FPDFDOC_InitFormFillEnvironment(m_pDocument, &formInfo);

      FPDF_FFLDraw(fHandle, bitmap, m_PagesMap[nPage], (int)(-tmpClipRect.minPoint().x*dpi_ratio),
        (int)(tmpClipRect.maxPoint().y*dpi_ratio) - (int)pCurExtents.y*dpi_ratio /*start y-coordinate is inverted (top coordinate instead of down in OdGeExtents2d)*/,
        (int)pCurExtents.x*dpi_ratio,
        (int)pCurExtents.y*dpi_ratio, 0, flags);
      FPDFDOC_ExitFormFillEnvironment(fHandle);
    }

    // allocate 4 bytes per pixel (FLYSdk needs only 3) to future conversion from BGR to RGBA.
    OdUInt8Array buffer;
    buffer.resize(Od_abs(bmiHeader.biWidth*bmiHeader.biHeight) * 4, 255);
    memcpy(buffer.asArrayPtr(), imagedata, bmiHeader.biSizeImage);

    OdGiRasterImagePtr pImage;
    OdResult res = calibrateColors(imagedata, bmiHeader.biSizeImage, Od_abs(bmiHeader.biWidth), Od_abs(bmiHeader.biHeight), backgroundColor, contextWithPaleteFlag);
    if (res == eOk)
    {
      pImage = convertBmpToRaster(bmiHeader, imagedata);

      FPDFBitmap_Destroy(bitmap);
      if (pImage.isNull())
        return eNullPtr;
    }
    //OdGePoint3d clippedOrigin = OdGePoint3d::kOrigin + OdGeVector3d::kXAxis * tmpClipRect.minPoint().x / 72. + OdGeVector3d::kYAxis * tmpClipRect.minPoint().y / 72.;
    OdGeVector3d u = OdGeVector3d((tmpClipRect.maxPoint().x - tmpClipRect.minPoint().x) / (72. * bmiHeader.biWidth), 0, 0);
    OdGeVector3d v = OdGeVector3d(0, (tmpClipRect.maxPoint().y - tmpClipRect.minPoint().y) / (72. * -bmiHeader.biHeight), 0);

    u *= measureScale;
    v *= measureScale;
    //clippedOrigin *= measureScale;

    pVd->geometry().rasterImageDc(origin, u, v, pImage, 0, 0, true);
    origin.y += -bmiHeader.biHeight*v.y;

    minP.y = tmpClipRect.maxPoint().y;
    maxP.y += step;
    if (maxP.y > clipRect.maxPoint().y)
      maxP.y = clipRect.maxPoint().y;
    else
      maxP.y = (OdInt32)(maxP.y + .5);
    tmpClipRect.set(minP, maxP);
  }
  return eNotApplicable;
}

int PdfiumWrapperImpl::getLevelsCount(int nPage) const
{
  if (m_path.isEmpty() || nPage < 0 || nPage >= m_CountOfPages)
    return 0;

  std::map<OdUInt32, FPDF_PAGE>::iterator iter = m_PagesMap.find(nPage);
  if (iter == m_PagesMap.end())
    m_PagesMap[nPage] = FPDF_LoadPage(m_pDocument, nPage);

  if (nPage != m_layerInfoCachePage)
  {
    m_layerInfoCachePage = nPage;
    if (!FPDF_GetPageLayers(m_pDocument, m_PagesMap[nPage], nPage, m_LayersCache))
      return -1;
  }

  int levelsCount = m_LayersCache.nol;
  
  if (checkAnnotationsOnPage(nPage))
    levelsCount++;

  return levelsCount;
}

OdString PdfiumWrapperImpl::getLevelName(int nPage, int nLevel) const
{
  if (m_path.isEmpty() || nLevel < 0 || nLevel > PdfiumWrapperImpl::getLevelsCount(nPage))
    return OdString::kEmpty;

  if (checkAnnotationsOnPage(nPage) && nLevel+1 == PdfiumWrapperImpl::getLevelsCount(nPage) )
    return "Comments and Markups";

  if (!m_LayersCache.nol)
    return "0";
  return m_LayersCache.names[nLevel];
}

OdUInt8 PdfiumWrapperImpl::isLevelEnabled(int nPage, int nLevel) const
{
  if (m_path.isEmpty() || nPage < 0 || nPage >= getItemsCount() || nLevel < 0 || nLevel > PdfiumWrapperImpl::getLevelsCount(nPage))
    return 0;

  if (nLevel+1 == PdfiumWrapperImpl::getLevelsCount(nPage) && checkAnnotationsOnPage(nPage))
    return 1;

  OdString name;
  if (m_LayersCache.nol > nLevel)
    name = m_LayersCache.names[nLevel];

  return isLevelEnabled(nPage, name);
}

OdUInt8 PdfiumWrapperImpl::isLevelEnabled(int nPage, const OdString &layerName) const
{
  if (m_path.isEmpty() || nPage < 0 || nPage >= getItemsCount() || !PdfiumWrapperImpl::getLevelsCount(nPage))
    return 0;

  if (!layerName.compare(OdString(L"Comments and Markups")))
    return 1;

  if (FPDF_IsLayerEnabled(m_pOFFLayers, layerName.c_str()))
    return 1;

  return 0;
}

void PdfiumWrapperImpl::enableLevel(int nPage, int nLevel, OdUInt8 flag)
{
  throw OdError(eNotImplementedYet);
}

double PdfiumWrapperImpl::getMeasureDictInfo(int nPage) const
{
  if (nPage > m_CountOfPages || nPage < 0)
    return 0;

  std::map<OdUInt32, FPDF_PAGE>::iterator iter = m_PagesMap.find(nPage);
  if (iter == m_PagesMap.end())
    m_PagesMap[nPage] = FPDF_LoadPage(m_pDocument, nPage);

  std::vector<FPDF_PageMeasureDictInfo> measure_info;
  FPDF_GetMeasureDictInfo(m_pDocument, nPage, measure_info);
  return (0 == measure_info.size()) ? 1. / 72. : measure_info[0].measure_factor;
}

OdResult PdfiumWrapperImpl::getExplodeGeometry(int nPage, OdGiGeometry *pGrabber, const OdArray<OdUnderlayLayer> &Layers, bool bIgnoreImages) const
{
  return eNotImplementedYet;
}

OdResult PdfUnderlayModule::forInternalUseOnly(OdUInt32 n, void *p)
{
  return eNotImplemented;
}
//"PDFiumModule.tx"
