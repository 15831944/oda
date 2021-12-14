/////////////////////////////////////////////////////////////////////////////// 
// Created by Open Design Alliance 
///////////////////////////////////////////////////////////////////////////////

#include "OdDeviceDriver.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "core/fxcrt/fx_system.h"
#include "core/fxge/cfx_font.h"
#include "core/fxge/cfx_windowsrenderdevice.h"
#include "core/fxge/cfx_graphstatedata.h"
#include "core/fxge/dib/cfx_dibextractor.h"
#include "core/fxge/dib/cfx_imagerenderer.h"
#include "core/fxge/dib/cstretchengine.h"
#include "core/fxge/fx_freetype.h"
#include "third_party/base/ptr_util.h"
#include "core/fxge/fx_freetype.h"
#include "core/fpdfapi/font/cpdf_font.h"
#include "core/fpdfapi/cpdf_modulemgr.h"
#include "core/fpdfapi/font/cpdf_font.h"
#include "core/fpdfapi/page/cpdf_form.h"
#include "core/fpdfapi/page/cpdf_page.h"
#include "core/fpdfapi/page/cpdf_pathobject.h"
#include "core/fpdfapi/page/cpdf_textobject.h"
#include "core/fpdfapi/page/cpdf_shadingpattern.h"
#include "core/fpdfapi/page/cpdf_function.h"
#include "core/fpdfapi/page/cpdf_stitchfunc.h"
#include "core/fpdfapi/parser/cpdf_document.h"
#include "core/fpdfapi/parser/cpdf_name.h"
#include "core/fpdfapi/parser/cpdf_parser.h"
#include "core/fpdfapi/parser/cpdf_reference.h"
#include "core/fpdfapi/parser/cpdf_array.h"
#include "core/fxge/cfx_unicodeencoding.h"
#include "core/fxge/cfx_cliprgn.h"
#include "third_party/base/ptr_util.h"
#include "fpdfsdk/fsdk_define.h"
#include "core/fxge/agg/fx_agg_driver.h"

void getLinkInfoFromPDFPage(FPDF_PAGE page, std::vector<LinkInfo>& link_info);
CFX_Matrix GetDisplayMatrix(CPDF_Page* pPage, float xPos, float yPos, float xSize, float ySize, int iRotate);
const double OdDeviceDriver::DPI = 72.0;

OdDeviceDriver::OdDeviceDriver(OdPdfImportBaseObjectGrabber* grabber, const std::wstring& layer_name, bool& is_object_visible, FPDF_PAGE page)
  :IFX_RenderDeviceDriver()
  ,m_CurrentLayerName(layer_name)
  ,m_Grabber(grabber)
  ,m_IsObjectVisible(is_object_visible)
{
  if (nullptr != grabber)
  {
    m_NeedToCombineText = grabber->combineTextObjects();
    grabber->getSpaceFactors(m_CharSpaceFactor, m_SpaceFactor);
  }
  getLinkInfoFromPDFPage(page, m_LinkInfo);
  CPDF_Page* pPage = CPDFPageFromFPDFPage(page);
  auto page_matrix = GetDisplayMatrix(pPage, 0, 0, grabber->getWidth(), grabber->getHeight(), 0);
  for (auto& elem : m_LinkInfo)
  {
    elem.rect = page_matrix.TransformRect(elem.rect);
  }

  std::vector<OdPdfImportPathData> path_data;
  if (m_Grabber->getClipBoundary(path_data))
  {
    for (auto& elem : path_data)
    {
      m_AdditionalClipPath.path.AppendPoint(CFX_PointF(elem.m_Point.x * DPI, elem.m_Point.y * DPI - std::fabs(m_Grabber->getHeight())), static_cast<FXPT_TYPE>(elem.m_Type), elem.m_CloseFigure);
    }
  }
}

OdDeviceDriver::~OdDeviceDriver() 
{
  const auto method = m_Grabber->getImportHyperlinkMethod();
  if (ImportHyperlinkMethod::eImportAsEntityHyperlink != method)
  {
    for (auto& elem : m_LinkInfo)
    {
      if ((ImportHyperlinkMethod::eImportAsTrasparentRegion == method) || (ImportHyperlinkMethod::eMixedModeImport == method && !elem.applied))
      {
        CFX_PathData path_data;
        path_data.AppendRect(elem.rect.left, elem.rect.bottom, elem.rect.right, elem.rect.top);
        std::vector<OdPdfImportPathData> out_path;

        preparePath(&path_data, nullptr, out_path);
        m_Grabber->addHyperlinkAsTransparentRegion(out_path.data(), (unsigned long)out_path.size(), elem.link.c_str());
      }
    }
  }

  EndRendering();
}

int OdDeviceDriver::GetDriverType() const
{
  return 1;
}

int OdDeviceDriver::GetDeviceCaps(int caps_id) const 
{
  switch (caps_id)
  {
  case FXDC_DEVICE_CLASS:
    return FXDC_PRINTER;
  case FXDC_PIXEL_WIDTH:
    return m_Grabber->getWidth();
  case FXDC_PIXEL_HEIGHT:
    return m_Grabber->getHeight();
  case FXDC_BITS_PIXEL:
    return DPI;
  case FXDC_RENDER_CAPS:
    return FXRC_GET_BITS | FXRC_ALPHA_PATH | FXRC_ALPHA_IMAGE |
      FXRC_BLEND_MODE | FXRC_SOFT_CLIP | FXRC_SHADING | FXRC_FILLSTROKE_PATH;
  case FXDC_HORZ_SIZE:
    return 0;
  case FXDC_VERT_SIZE:
    return 0;
  }
  return 0;
}

void OdDeviceDriver::SaveState()
{
  if (m_ClipInfo.empty())
    return;
  m_ClipStack.push_back(m_ClipInfo);

  m_Grabber->SaveClipState();
}

void OdDeviceDriver::RestoreState(bool bKeepSaved)
{
  m_ClipInfo = std::vector<ClipInfo>();
  if (m_ClipStack.empty())
    return;
  if (bKeepSaved)
  {
    m_ClipInfo = m_ClipStack.back();
  }
  else
  {
    m_ClipInfo = m_ClipStack.back();
    m_ClipStack.pop_back();
  }

  m_Grabber->RestoreClipState(bKeepSaved);
}

bool OdDeviceDriver::preparePath(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, std::vector<OdPdfImportPathData>& out_path)
{
  const std::vector<FX_PATHPOINT>& pPoints = pPathData->GetPoints();

  if (0 == pPoints.size())
    return true;

  out_path.resize(pPoints.size());

  for (size_t i = 0; i < pPoints.size(); ++i)
  {

    CFX_PointF pos = pObject2Device == nullptr ? pPoints[i].m_Point : pObject2Device->Transform(pPoints[i].m_Point);
    out_path[i].m_Point.x = pos.x / DPI;
    out_path[i].m_Point.y = (pos.y + std::fabs(m_Grabber->getHeight())) / DPI;

    out_path[i].m_CloseFigure = pPoints[i].m_CloseFigure;

    switch (pPoints[i].m_Type)
    {
    case FXPT_TYPE::MoveTo:
      out_path[i].m_Type = OdPdfImportPtType::OdMoveTo;
      break;
    case FXPT_TYPE::LineTo:
      out_path[i].m_Type = OdPdfImportPtType::OdLineTo;
      break;
    case FXPT_TYPE::BezierTo:
      out_path[i].m_Type = OdPdfImportPtType::OdBezierTo;
      break;
    }
  }
  return false;
}

bool OdDeviceDriver::SetClip_PathFill(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, int fill_mode)
{
  ClipInfo clip_info;
  clip_info.path = *pPathData;
  if(nullptr != pObject2Device)
    clip_info.matrix = *pObject2Device;
  clip_info.fill_mode = fill_mode;
  clip_info.bbox = pPathData->GetBoundingBox();
  clip_info.bbox = clip_info.matrix.TransformRect(clip_info.bbox);
  clip_info.bbox.Normalize();
  m_ClipInfo.push_back(clip_info);

  if (0 != m_AdditionalClipPath.path.GetPoints().size())
  {
    m_ClipInfo.push_back(m_AdditionalClipPath);
  }

  std::vector<OdPdfImportPathData> out_path;

  if (preparePath(pPathData, pObject2Device, out_path))
    return true;

  m_Grabber->addClipPath(out_path.data(), (unsigned long)out_path.size());

  return true;
}

bool OdDeviceDriver::SetClip_PathStroke(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, const CFX_GraphStateData* pGraphState)
{
  return false;
}

bool OdDeviceDriver::FillRectWithBlend(const FX_RECT* pRect, uint32_t fill_color, int blend_type)
{
  return false;
}

bool OdDeviceDriver::DrawShading(const CPDF_ShadingPattern* pPattern, const CFX_Matrix* pMatrix, const FX_RECT& clip_rect, int alpha, bool bAlphaMode)
{
  return false;
}

bool OdDeviceDriver::SetBitsWithMask(const RetainPtr<CFX_DIBSource>& pBitmap, const RetainPtr<CFX_DIBSource>& pMask, int left, int top, int bitmap_alpha, int blend_type)
{
  return false;
}

bool OdDeviceDriver::StretchDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, int dest_left, int dest_top, int dest_width,
  int dest_height, const FX_RECT* pClipRect, uint32_t flags, int blend_type)
{
  return false;
}

bool OdDeviceDriver::GetClipBox(FX_RECT* pRect)
{
  pRect->left = 0;
  pRect->right = m_Grabber->getWidth();
  pRect->top = 0;
  pRect->bottom = m_Grabber->getHeight();
  return true;
}

const char* OdDeviceDriver::getHyperlink(const CFX_FloatRect& rect)
{
  if (ImportHyperlinkMethod::eImportAsTrasparentRegion == m_Grabber->getImportHyperlinkMethod())
    return nullptr;

  const char* hyperlink{ nullptr };

  CFX_FloatRect min_link_rect;

  for (auto& elem : m_LinkInfo)
  {
    auto link_rect = elem.rect;
    link_rect.Inflate(link_rect.Width() * 0.05, link_rect.Height() * 0.05);
    if (link_rect.Contains(rect))
    {
      if (min_link_rect.IsEmpty() || min_link_rect.Contains(link_rect))
      {
        hyperlink = elem.link.c_str();
        min_link_rect = link_rect;
        elem.applied = true;
      }
    }
  }

  return hyperlink;
}

bool OdDeviceDriver::CheckFullClipped(const CFX_FloatRect& rect)
{
  for (const auto& clip_info : m_ClipInfo)
  {
    CFX_FloatRect bbox = clip_info.bbox;
    bbox.Intersect(rect);
    if (bbox.IsEmpty())
    {
      return true;
      break;
    }
  }
  return false;
}

bool OdDeviceDriver::DrawPath(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, const CFX_GraphStateData* pGraphState,
  uint32_t fill_color, uint32_t stroke_color, int fill_mode, int blend_type)
{
  bool ret_val(true);
  if (!m_Grabber->needToAddGrathics(&ret_val))
    return ret_val;

  std::vector<OdPdfImportPathData> out_path;

  if (preparePath(pPathData, pObject2Device, out_path))
    return true;

  auto path_rect = pPathData->GetBoundingBox();
  if(pObject2Device)
    path_rect = pObject2Device->TransformRect(path_rect);
  path_rect.Deflate(1, 1);
  path_rect.left = (int)path_rect.left;
  path_rect.right = (int)path_rect.right;
  path_rect.top = (int)path_rect.top;
  path_rect.bottom = (int)path_rect.bottom;

  OdPdfImportColor od_fill_color(FXARGB_R(fill_color), FXARGB_G(fill_color), FXARGB_B(fill_color), FXARGB_A(fill_color)),
    od_stroke_color(FXARGB_R(stroke_color), FXARGB_G(stroke_color), FXARGB_B(stroke_color), FXARGB_A(stroke_color));

  float line_width(1.);
  if (nullptr != pGraphState)
  {
    if(nullptr != pObject2Device)
      line_width = pObject2Device->TransformDistance(line_width);
    if(line_width > 1e-7)
      line_width = pGraphState->m_LineWidth / (72. / line_width / 25.4);
  }

  std::unique_ptr<LineTypeInfo> line_type;
  std::vector<float> dash_array;
  if (nullptr != pGraphState && pGraphState->m_DashCount > 0)
  {
    line_type.reset(new LineTypeInfo);
    dash_array.resize(pGraphState->m_DashCount);
    for (int i = 0; i < pGraphState->m_DashCount; ++i)
    {
      dash_array[i] = (i % 2) ? -pGraphState->m_DashArray[i] /72. : pGraphState->m_DashArray[i] /72.;
      if (nullptr != pObject2Device)
        dash_array[i] = pObject2Device->TransformDistance(dash_array[i]);
    }
    line_type->dash_phase = pGraphState->m_DashPhase;
    line_type->dash_array = dash_array.data();
    line_type->dash_count = pGraphState->m_DashCount;
  }

  m_Grabber->addPath(out_path.data(), (unsigned long)out_path.size(), &od_fill_color, &od_stroke_color,
    nullptr == pGraphState ? nullptr : &line_width, m_IsObjectVisible, m_CurrentLayerName.c_str(), line_type.get(), getHyperlink(path_rect));

  return true;
}

void OdDeviceDriver::fillBmiHeader(RetainPtr<CFX_DIBSource>& pBitmap, BmiHeaderInfo& bmiHeader)
{
  if (pBitmap->IsAlphaMask() && pBitmap->GetBPP() == 1)
  {
    uint8_t* buf = pBitmap->GetBuffer();
    for (uint32_t i = 0; i < pBitmap->GetPitch()*pBitmap->GetHeight(); ++i)
    {
      buf[i] = ~buf[i];
    }
  }
  
  bmiHeader.width = pBitmap->GetWidth();
  bmiHeader.height = pBitmap->GetHeight();
  bmiHeader.bpp = pBitmap->GetBPP();
  bmiHeader.pitch = pBitmap->GetPitch();
  bmiHeader.palette = pBitmap->GetPalette();
  bmiHeader.palette_size = pBitmap->GetPaletteSize();
}

bool OdDeviceDriver::SetDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, const FX_RECT* pSrcRect, int left,
  int top, int blend_type) 
{
  bool ret_val(true);
  if (!m_Grabber->needToAddImage(&ret_val))
    return ret_val;

  OdPdfImportRect rect(0, 0, pSrcRect->right / DPI, pSrcRect->bottom / DPI);

  OdPdfImportMatrix matrix;
  matrix.e = left / DPI;
  matrix.f = (top + std::fabs(m_Grabber->getHeight())) / DPI;

  RetainPtr<CFX_DIBSource> pBitmapImpl;
  if (m_Grabber->isImageClippingEnabled())
  {
    CFX_FloatRect img_rect(left, top, left + pSrcRect->right, top + pSrcRect->bottom);
    img_rect.Normalize();
    if (CheckFullClipped(img_rect))
      return true;

    auto pClippedBitmap = pdfium::MakeRetain<CFX_DIBitmap>();
    pClippedBitmap->Create(pBitmap->GetWidth(), pBitmap->GetHeight(), FXDIB_Argb);

    auto image_clip_driver = pdfium::MakeUnique<CFX_AggDeviceDriver>(pClippedBitmap, false, nullptr, false);

    CFX_Matrix image_clip_matrix;
    for (auto& clip_info : m_ClipInfo)
    {
      CFX_PathData clip_path(clip_info.path);
      CFX_Matrix path_clip_matrix;
      path_clip_matrix.e = -left;
      path_clip_matrix.f = clip_info.matrix.f - top;
      clip_path.Transform(&path_clip_matrix);
      image_clip_driver->SetClip_PathFill(&clip_path, &image_clip_matrix, clip_info.fill_mode);
    }



    image_clip_matrix.a = pBitmap->GetWidth();
    image_clip_matrix.d = -pBitmap->GetHeight();
    image_clip_matrix.f = pBitmap->GetHeight();

    std::unique_ptr<CFX_ImageRenderer> image_render_handle;
    image_clip_driver->StartDIBits(pBitmap, 255, color, &image_clip_matrix, 0, &image_render_handle, blend_type);
    image_clip_driver->ContinueDIBits(image_render_handle.get(), nullptr);
    pBitmapImpl = pClippedBitmap;
  }
  else
  {
    pBitmapImpl = pBitmap;
    if (nullptr == pBitmap->GetBuffer())
      pBitmapImpl = pBitmap->Clone(nullptr);
  }

  BmiHeaderInfo bmiHeader;
  fillBmiHeader(pBitmapImpl, bmiHeader);

  CFX_FloatRect image_rect(*pSrcRect);
  image_rect.Translate(left, top);
  image_rect.Deflate(1, 1);

  m_Grabber->addImage(&bmiHeader, pBitmapImpl->GetBuffer(), &rect, &matrix, m_IsObjectVisible, m_CurrentLayerName.c_str(), getHyperlink(image_rect));

  return true;
}

bool OdDeviceDriver::StartDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, int bitmap_alpha, uint32_t color, const CFX_Matrix* pMatrix,
  uint32_t render_flags, std::unique_ptr<CFX_ImageRenderer>* handle, int blend_type)
{

  bool ret_val(true);
  if (!m_Grabber->needToAddImage(&ret_val))
    return ret_val;

  double stretch_width = FXSYS_sqrt2(pMatrix->a, pMatrix->b);
  double stretch_height = FXSYS_sqrt2(pMatrix->c, pMatrix->d);
  CFX_Matrix stretch2dest(1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0); // CORE-14408 mirrored image

  stretch2dest.Concat(
    CFX_Matrix(pMatrix->a / stretch_width, pMatrix->b / stretch_width,
      pMatrix->c / stretch_height, pMatrix->d / stretch_height,
      pMatrix->e, pMatrix->f));
  CFX_Matrix dest2stretch = stretch2dest.GetInverse();

  CFX_FloatRect result_clip = pMatrix->GetUnitRect();
  CFX_FloatRect m_StretchClip = dest2stretch.TransformRect(result_clip);
  m_StretchClip.Intersect(CFX_FloatRect(m_StretchClip.left, m_StretchClip.bottom, m_StretchClip.left + stretch_width, m_StretchClip.bottom + stretch_height));

  OdPdfImportRect rect(m_StretchClip.left / DPI, m_StretchClip.top / DPI, m_StretchClip.right / DPI, m_StretchClip.bottom / DPI);

  OdPdfImportMatrix matrix(dest2stretch.a, dest2stretch.b, dest2stretch.c, dest2stretch.d,
    stretch2dest.e / DPI, (stretch2dest.f + std::fabs(m_Grabber->getHeight())) / DPI);

  RetainPtr<CFX_DIBSource> pBitmapImpl;
  if (m_Grabber->isImageClippingEnabled())
  {
    CFX_FloatRect img_rect(pMatrix->GetUnitRect().GetOuterRect());
    img_rect.Normalize();
    if (CheckFullClipped(img_rect))
      return true;

    auto pClippedBitmap = pdfium::MakeRetain<CFX_DIBitmap>();
    pClippedBitmap->Create(pBitmap->GetWidth(), pBitmap->GetHeight(), FXDIB_Argb);

    auto image_clip_driver = pdfium::MakeUnique<CFX_AggDeviceDriver>(pClippedBitmap, false, nullptr, false);

    CFX_Matrix image_matrix;
    for (auto& clip_info : m_ClipInfo)
    {
      CFX_Matrix image_clip_matrix_mirrored;
      image_clip_matrix_mirrored.a = pBitmap->GetWidth() / m_StretchClip.Width();
      image_clip_matrix_mirrored.d = pBitmap->GetHeight() / m_StretchClip.Height();
      image_clip_matrix_mirrored.f = pBitmap->GetHeight();


      CFX_Matrix path_clip_matrix(clip_info.matrix);
      path_clip_matrix.Concat(dest2stretch);

      path_clip_matrix.Concat(image_clip_matrix_mirrored);

      CFX_PathData clip_path(clip_info.path);
      clip_path.Transform(&path_clip_matrix);

      image_clip_driver->SetClip_PathFill(&clip_path, &image_matrix, clip_info.fill_mode);
    }


    image_matrix.a = pBitmap->GetWidth();
    image_matrix.d = -pBitmap->GetHeight();
    image_matrix.f = pBitmap->GetHeight();

    std::unique_ptr<CFX_ImageRenderer> image_render_handle;
    image_clip_driver->StartDIBits(pBitmap, bitmap_alpha, color, &image_matrix, render_flags, &image_render_handle, blend_type);
    image_clip_driver->ContinueDIBits(image_render_handle.get(), nullptr);
    pBitmapImpl = pClippedBitmap;
  }
  else
  {
    pBitmapImpl = pBitmap;
    if (nullptr == pBitmap->GetBuffer())
      pBitmapImpl = pBitmap->Clone(nullptr);
  }

  BmiHeaderInfo bmiHeader;
  fillBmiHeader(pBitmapImpl, bmiHeader);
  bmiHeader.height = -bmiHeader.height; // CORE-14408 mirrored image

  CFX_FloatRect image_rect(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
  if(pMatrix)
    image_rect = pMatrix->TransformRect(image_rect);
  image_rect.Deflate(1, 1);
  image_rect.left = (int)image_rect.left;
  image_rect.right = (int)image_rect.right;
  image_rect.top = (int)image_rect.top;
  image_rect.bottom = (int)image_rect.bottom;

  m_Grabber->addImage(&bmiHeader, pBitmapImpl->GetBuffer(), &rect, &matrix, m_IsObjectVisible, m_CurrentLayerName.c_str(), getHyperlink(image_rect));

  return true;
}

CPDF_Font* createFont(std::unique_ptr<CPDF_Document>& pDoc, CFX_Font* pFont, const ByteString& basefont)
{

  ByteString base_font_name = basefont;
  CPDF_Dictionary* pDict = pDoc->NewIndirect<CPDF_Dictionary>();
  pDict->SetNewFor<CPDF_Name>("Type", "Font");
  pDict->SetNewFor<CPDF_Name>("Subtype", "TrueType");
  
  if (pFont->IsBold() && pFont->IsItalic())
    base_font_name += ",BoldItalic";
  else if (pFont->IsBold())
    base_font_name += ",Bold";
  else if (pFont->IsItalic())
    base_font_name += ",Italic";

  pDict->SetNewFor<CPDF_Name>("BaseFont", base_font_name);

  CPDF_Dictionary* pDesc = pDoc->NewIndirect<CPDF_Dictionary>();
  pDesc->SetNewFor<CPDF_Name>("Type", "FontDescriptor");
  pDesc->SetNewFor<CPDF_Name>("FontName", base_font_name);
  pDict->SetNewFor<CPDF_Reference>("FontDescriptor", pDoc.get(),
    pDesc->GetObjNum());

  return pDoc->LoadFont(pDict);
}

bool OdDeviceDriver::DrawDeviceText(int nChars, const FXTEXT_CHARPOS* pCharPos, CFX_Font* pFont, const CFX_Matrix* pObject2Device,
  float font_size, uint32_t color)
{
  return true;
}

bool OdDeviceDriver::isSameFontInfo(const CFX_Font* pFont, const CFX_Matrix* pObject2Device, const float font_size, const  uint32_t color)
{
  const bool cond1 = m_PreviousFontInfo.pFont && m_PreviousFontInfo.pFont->GetFamilyName() == pFont->GetFamilyName() && m_PreviousFontInfo.pFont->IsVertical() == pFont->IsVertical() &&
    m_PreviousFontInfo.pFont->IsBold() == pFont->IsBold() && m_PreviousFontInfo.pFont->IsItalic() == pFont->IsItalic();
  const bool cond2 = fabs(m_PreviousFontInfo.font_size - font_size) < 1e-5;
  const bool cond3 = m_PreviousFontInfo.color == color;

  const bool cond4 = fabs(m_PreviousFontInfo.matrix.a - pObject2Device->a) < 1e-5;
  const bool cond5 = fabs(m_PreviousFontInfo.matrix.b - pObject2Device->b) < 1e-5;
  const bool cond6 = fabs(m_PreviousFontInfo.matrix.c - pObject2Device->c) < 1e-5;
  const bool cond7 = fabs(m_PreviousFontInfo.matrix.d - pObject2Device->d) < 1e-5;

  return cond1 && cond2 && cond3 && cond4 && cond5 && cond6 && cond7;

}

float OdDeviceDriver::CheckSpace(const CFX_Matrix* pObject2Device, const double text_width, const double& space_width, bool& is_char_space, bool& is_space)
{
  CFX_PointF st_point1;
  st_point1 = m_PreviousFontInfo.matrix.Transform(st_point1);
  CFX_PointF end_point1(m_PreviousFontInfo.text_width, 0);
  end_point1 = m_PreviousFontInfo.matrix.Transform(end_point1);

  CFX_VectorF vec1(end_point1.x - st_point1.x, end_point1.y - st_point1.y);
  vec1.Normalize();
  vec1.Scale(m_PreviousFontInfo.text_width, m_PreviousFontInfo.text_width);
  end_point1.x = st_point1.x + vec1.x;
  end_point1.y = st_point1.y + vec1.y;


  CFX_PointF st_point2;
  st_point2 = pObject2Device->Transform(st_point2);
  CFX_PointF end_point2(text_width, 0);
  end_point2 = pObject2Device->Transform(end_point2);

  CFX_VectorF vec2(end_point2.x - st_point2.x, end_point2.y - st_point2.y);

  vec1.Normalize();
  vec2.Normalize();

  const bool cond1 = (fabs(vec1.x - vec2.x) < 1e-4) && (fabs(vec1.y - vec2.y) < 1e-4);

  CFX_VectorF vec3(st_point2.x - end_point1.x, st_point2.y - end_point1.y);

  float obj_space_size = vec3.Length();

  vec3.Normalize();

  bool cond2 = (fabs(vec3.x - vec1.x) < 1e-4) && (fabs(vec3.y - vec1.y) < 1e-4);
  
  is_char_space = cond1 && cond2 && obj_space_size < m_CharSpaceFactor * pObject2Device->TransformDistance(space_width / 72.);

  is_space = cond1 && cond2 && !is_char_space && (obj_space_size < m_SpaceFactor * pObject2Device->TransformDistance(space_width / 72.));

  return obj_space_size;
  
}

bool CreateFontByDoc(std::unique_ptr<CPDF_Document>& pDoc, CFX_Font* pFont, const ByteString& basefont, CPDF_Font*& out_font, ByteString& out_face_name)
{
  out_font = createFont(pDoc, pFont, basefont);
  out_face_name = ByteString(out_font->GetFace()->family_name);
  return out_face_name.Find("Chrome Sans").has_value() || out_face_name.Find("Chrom Sans").has_value();
}

ByteString NormalizeFontName(const ByteString& name)
{
  ByteString norm(name);
  auto pos = norm.Find('+');
  if (pos.has_value() && pos.value() != 0 && pos.value() <= norm.GetLength() - 1)
    norm = norm.Right(norm.GetLength() - pos.value() - 1);
  return norm;
}

bool Compare_FX_PATHPOINT(FX_PATHPOINT left, FX_PATHPOINT right)
{
  if (left.m_CloseFigure == right.m_CloseFigure && left.m_Type == right.m_Type && left.m_Point == right.m_Point)
    return true;
  else
    return false;
}

bool IsEbeddFontEqualToSystem(const int& nChars, const FXTEXT_CHARPOS* pCharPos, CFX_Font* pCXFFont, std::unique_ptr<CPDF_Document>& pDoc, 
  const ByteString& basefont, CPDF_Font*& out_font, ByteString& out_face_name)
{
  bool local_is_not_found = CreateFontByDoc(pDoc, pCXFFont, basefont, out_font, out_face_name);
  if (!local_is_not_found)
  {
    for (int iChar = 0; iChar < nChars; ++iChar)
    {
      const FXTEXT_CHARPOS& charpos = pCharPos[iChar];
      const CFX_PathData* pPath1 = pCXFFont->LoadGlyphPath(charpos.m_GlyphIndex, charpos.m_FontCharWidth);
      const CFX_PathData* pPath2 = out_font->GetFont()->LoadGlyphPath(charpos.m_GlyphIndex, charpos.m_FontCharWidth);
      if ((pPath1 && !pPath2) || (!pPath1 && pPath2))
      {
        local_is_not_found = true;
        break;
      }
      if (pPath1 && pPath1)
      {
        const std::vector<FX_PATHPOINT>& points1 = pPath1->GetPoints();
        const std::vector<FX_PATHPOINT>& points2 = pPath2->GetPoints();
        if (points1.size() != points2.size())
        {
          local_is_not_found = true;
          break;
        }

        if (!std::equal(points1.begin(), points1.end(), points2.begin(), Compare_FX_PATHPOINT))
        {
          local_is_not_found = true;
          break;
        }
      }
    }
  }
  return !local_is_not_found;
}

bool OdDeviceDriver::DrawDeviceTextEx(int nChars, const FXTEXT_CHARPOS* pCharPos, CFX_Font* pCXFFont, const CFX_Matrix* pObject2Device, float font_size,
  uint32_t color, const bool isType3Font, const WideString& unicode_str, const OdPdfiumFontInfo& font_info, CPDF_Font* pCPDFFont)
{
  bool ret_val(true);
  if (!m_Grabber->needToAddText(&ret_val))
    return ret_val;

  try
  {
    PdfImportFontParams font_params;
    font_params.isType3Font = isType3Font;

    CFX_Font* pImpFont;

    //fake doc for default font
    auto pDoc = pdfium::MakeUnique<CPDF_Document>(nullptr);

    pDoc->CreateNewDoc();

    CPDF_Font* loadedCPDFFont{nullptr};
    ByteString face_name, tmp_face_name;

    auto is_not_found = CreateFontByDoc(pDoc, pCXFFont, pCXFFont->GetFaceName(), loadedCPDFFont, face_name);

    ByteString family_name;

    ByteString font_name = NormalizeFontName(pCPDFFont->GetBaseFont());
    if (font_name.IsEmpty())
    {
      CPDF_Dictionary* pFontDict = pCPDFFont->GetFontDict();
      if (pFontDict)
      {
        font_name = NormalizeFontName(pFontDict->GetStringFor("Name"));
      }
      if (font_name.IsEmpty())
        font_name = NormalizeFontName(pCXFFont->GetFaceName());
    }

    if (pCPDFFont->IsType3Font())
    {
      family_name = font_name;
      is_not_found = true;
    }
    else
    if( pCPDFFont->IsStandardFont())
    {
      if (!is_not_found)
        family_name = pCXFFont->GetFamilyName();
      else
        family_name = font_name;
    }
    else
    {
      CPDF_Dictionary* pFontDict = pCPDFFont->GetFontDict();
      if (pFontDict)
      {
        CPDF_Array* pDescendantFonts = pFontDict->GetArrayFor("DescendantFonts");
        if (pDescendantFonts && pDescendantFonts->GetCount() == 1)
        {
          CPDF_Dictionary* pCIDFontDict = pDescendantFonts->GetDictAt(0);
          if (pCIDFontDict)
          {
            CPDF_Dictionary* pFontDesc = pCIDFontDict->GetDictFor("FontDescriptor");
            if (pFontDesc)
            {
              family_name = NormalizeFontName(pFontDesc->GetStringFor("FontFamily"));
            }
          }
        }
      }
      if(family_name.IsEmpty())
        family_name = NormalizeFontName(pCXFFont->GetFamilyName());
      if (pCXFFont->IsEmbedded() && is_not_found)
      {
        CPDF_Font* local_loadedCPDFFont{ nullptr };
        ByteString local_face_name;
        if (IsEbeddFontEqualToSystem(nChars, pCharPos, pCXFFont, pDoc, family_name, local_loadedCPDFFont, local_face_name))
        {
          is_not_found = false;
          loadedCPDFFont = local_loadedCPDFFont;
          face_name = local_face_name;
        }
      }
    }

    static const auto max_font_name_buf_size = 250;
    unsigned int font_name_buf_size(0);

    const bool is_changed = m_Grabber->getPreferableFont(font_name.c_str(), family_name.c_str(), !is_not_found,
      tmp_face_name.GetBuffer(max_font_name_buf_size), font_name_buf_size);

    tmp_face_name.ReleaseBuffer(font_name_buf_size);

    if (is_changed && !tmp_face_name.IsEmpty())
    {
      loadedCPDFFont = pCPDFFont;
      pImpFont = pCXFFont;
      face_name = tmp_face_name;
      font_params.isChanged = true;
    }
    else
    {
      if (is_not_found)
      {
        if (m_Grabber->extractEmbeddedFonts() && pCXFFont->IsEmbedded() && nullptr != pCXFFont->GetFace()->charmap)
        {
          font_params.embedded_font_data = pCXFFont->GetFontData();
          font_params.embedded_font_data_size = pCXFFont->GetSize();
          font_params.font_file_name = font_name.c_str();

          loadedCPDFFont = pCPDFFont;
          pImpFont = pCXFFont;
          face_name = pCXFFont->GetFaceName();
        }
        else
        {
          CreateFontByDoc(pDoc, pCXFFont, "Arial", loadedCPDFFont, face_name);
          pImpFont = loadedCPDFFont->GetFont();
        }
      }
      else
      {
        pImpFont = loadedCPDFFont->GetFont();
      }
    }
    CFX_Matrix object_matrix(*pObject2Device);
    
    const bool is_negative_font_size = font_size < 0;
    if (is_negative_font_size)
    {
      object_matrix.RotateAt(FX_PI, -object_matrix.e, -object_matrix.f);
      font_size = -font_size;
    }
    
    object_matrix.e = object_matrix.e / DPI;
    object_matrix.f = (object_matrix.f + std::fabs(m_Grabber->getHeight())) / DPI;

    WideString content_str = unicode_str;

    CFX_UnicodeEncoding encoding(pImpFont);

    double text_height(0.0);
    CFX_PointF underline_point;
    double underline_position(0.0);
    CFX_PointF dir_point;
    CFX_Matrix dir_matrix;
    float dir_coef(1.);
    CFX_Matrix tmp_matrix(object_matrix);

    float space_width(font_info.space_width);

    if (isType3Font && font_info.type3info.m_TextWidth > std::numeric_limits<float>::min())
    {
      font_params.text_width = font_info.type3info.m_TextWidth / DPI;

      if (m_NeedToCombineText)
      {
        uint32_t space_char_code = loadedCPDFFont->CharCodeFromUnicode(32);
        if (CPDF_Font::kInvalidCharCode != space_char_code)
          space_width = static_cast<float>(loadedCPDFFont->GetCharWidthF(space_char_code)) * font_size / 1000.;
        if (space_width < 1e-7)
        {
          for (int iChar = 0; iChar < nChars; ++iChar)
          {
            const FXTEXT_CHARPOS& charpos = pCharPos[iChar];
            int ch_code = loadedCPDFFont->CharCodeFromUnicode(charpos.m_Unicode);
            if (ch_code != CPDF_Font::kInvalidCharCode)
            {
              const float val = static_cast<float>(loadedCPDFFont->GetCharWidthF(ch_code) * font_size / 1000.);
              if (space_width < 1e-7)
              {
                space_width = val;
              }
              else
              {
                space_width = std::min(space_width, val);
              }

            }
          }
        }
      }

    }
    else
    {
      double first_symbol_pos(0.0);
      bool is_first_simbol(true);

      for (int iChar = 0; iChar < nChars; ++iChar)
      {
        const FXTEXT_CHARPOS& charpos = pCharPos[iChar];

        CFX_Matrix matrix;
        if (charpos.m_bGlyphAdjust)
        {
          matrix = CFX_Matrix(charpos.m_AdjustMatrix[0], charpos.m_AdjustMatrix[1],
            charpos.m_AdjustMatrix[2], charpos.m_AdjustMatrix[3], 0, 0);
        }

        matrix.Concat(CFX_Matrix(font_size, 0, 0, font_size, is_negative_font_size? -charpos.m_Origin.x : charpos.m_Origin.x, charpos.m_Origin.y));
        const CFX_PathData* pPath = pCXFFont->LoadGlyphPath(charpos.m_GlyphIndex, charpos.m_FontCharWidth);
        if (!pPath)
          continue;

        matrix.Scale(pObject2Device->GetXUnit(), pObject2Device->GetYUnit());

        CFX_PathData TransformedPath(*pPath);
        TransformedPath.Transform(&matrix);
        CFX_FloatRect rect = TransformedPath.GetBoundingBox();
        font_params.text_width = std::max(font_params.text_width, (double)rect.right);
        text_height = std::max(text_height, (double)rect.Height());

        if (is_first_simbol)
        {
          first_symbol_pos = matrix.e + rect.left;
          is_first_simbol = false;
        }

        uint32_t glyph_index(0);
        int ch_code = loadedCPDFFont->CharCodeFromUnicode(charpos.m_Unicode);
        if (CPDF_Font::kInvalidCharCode == ch_code)
          glyph_index = encoding.GlyphFromCharCode(charpos.m_Unicode);
        else
        {
          int res = loadedCPDFFont->GlyphFromCharCodeExt(ch_code);
          glyph_index = (-1 == res) ? encoding.GlyphFromCharCode(charpos.m_Unicode) : res;
        }
        const CFX_PathData* pPathImpl = pImpFont->LoadGlyphPath(glyph_index, charpos.m_FontCharWidth);
        if (!pPathImpl)
          continue;
        CFX_PathData TransformedPathImpl(*pPathImpl);
        TransformedPathImpl.Transform(&matrix);
        CFX_FloatRect rect1 = TransformedPathImpl.GetBoundingBox();
        underline_position = std::min(underline_position, (double)rect1.bottom);
      }

      content_str.Trim();
      if (content_str.IsEmpty())
        return true;

      first_symbol_pos /= DPI;

      text_height /= DPI;
      font_params.text_width /= DPI;
      underline_position /= DPI;

      font_params.text_width -= first_symbol_pos;

      dir_point.x = 0.;
      dir_point.y = underline_position;
      
      dir_matrix = object_matrix;
      dir_matrix.e = 0;
      dir_matrix.f = 0;
      dir_point = dir_matrix.Transform(dir_point);
      dir_coef = std::max(dir_matrix.GetXUnit(), dir_matrix.GetYUnit());
      dir_point.x /= dir_coef;
      dir_point.y /= dir_coef;

      //////////////////////////////////////////////////////////////////////////
      CFX_PointF dir_point_fs(first_symbol_pos, 0.);
      dir_point_fs = dir_matrix.Transform(dir_point_fs);
      dir_point_fs.x /= dir_coef;
      dir_point_fs.y /= dir_coef;

      object_matrix.e += dir_point_fs.x;
      object_matrix.f += dir_point_fs.y;

      tmp_matrix = object_matrix;
      //////////////////////////////////////////////////////////////////////////

      object_matrix.e += dir_point.x;
      object_matrix.f += dir_point.y;



    }

    //////////////////////////////////////////////////////////////////////////

    if (m_NeedToCombineText)
    {
      bool is_same_font = isSameFontInfo(pCXFFont, &tmp_matrix, font_size, color);

      float space_len(0.);

      if (is_same_font)
        space_len = CheckSpace(&tmp_matrix, font_params.text_width, space_width, font_params.is_char_space, font_params.is_space);

      //////////////////////////////////////////////////////////////////////////

      if (font_params.is_char_space || font_params.is_space)
      {
        if (m_PreviousFontInfo.underline_position > underline_position)
        {
          underline_point.y = underline_position;
          m_PreviousFontInfo.underline_position = underline_position;

          underline_point = dir_matrix.Transform(underline_point);
          underline_point.x /= dir_coef;
          underline_point.y /= dir_coef;
        }
        else
        {
          underline_point = m_PreviousFontInfo.dir_pos;
        }

        m_PreviousFontInfo.full_text_width += space_len + font_params.text_width;

      }
      else
      {
        underline_point = dir_point;
      }

      if (!font_params.is_char_space && !font_params.is_space)
      {
        m_PreviousFontInfo.pFont = pCXFFont;
        m_PreviousFontInfo.font_size = font_size;
        m_PreviousFontInfo.color = color;
        m_PreviousFontInfo.dir_pos = underline_point;
        m_PreviousFontInfo.underline_position = underline_position;
        m_PreviousFontInfo.full_text_width = font_params.text_width;
      }
      m_PreviousFontInfo.text_width = font_params.text_width;
      m_PreviousFontInfo.matrix = tmp_matrix;

      font_params.text_width = m_PreviousFontInfo.full_text_width;
    }
////////////////////////////////////////////////////////////////////////// 

    if(isType3Font && font_info.type3info.m_TextHeight > std::numeric_limits<float>::min())
    {
      font_params.font_height = font_info.type3info.m_TextHeight / DPI;
    }
    else
    {
      FXFT_Face font_face = pImpFont->GetFace();

      FT_ULong charA = FT_ULong('A');

      uint32_t glyph_index(0);
      int ch_code = loadedCPDFFont->CharCodeFromUnicode(charA);
      if (CPDF_Font::kInvalidCharCode == ch_code)
        glyph_index = encoding.GlyphFromCharCode(charA);
      else
      {
        int res = loadedCPDFFont->GlyphFromCharCodeExt(ch_code);
        glyph_index = (-1 == res) ? encoding.GlyphFromCharCode(charA) : res;
      }

      font_params.font_height = 1000;
      if (0 != glyph_index)
      {
        if (!FXFT_Load_Glyph(font_face, glyph_index, FXFT_LOAD_NO_SCALE | FXFT_LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH))
          font_params.font_height = (double)FXFT_Get_Glyph_HoriBearingY(font_face);
        int em = FXFT_Get_Face_UnitsPerEM(font_face);
        if (em == 0)
        {
          font_params.font_height = FXFT_Get_Glyph_HoriBearingY(font_face);
        }
        else
        {
          font_params.font_height = FXFT_Get_Glyph_HoriBearingY(font_face) * 1000. / em;
        }
      }

      font_params.font_height = font_params.font_height * font_size * 10. / DPI;

      font_params.font_height = font_params.font_height / 10000.;

      if (font_params.font_height < 1e-10)
        font_params.font_height = text_height;

    }

    OdPdfImportColor od_color(FXARGB_R(color), FXARGB_G(color), FXARGB_B(color), FXARGB_A(color));

    OdPdfImportMatrix matrix(object_matrix.a, object_matrix.b, object_matrix.c, object_matrix.d,
      object_matrix.e, object_matrix.f);

    if (m_NeedToCombineText)
      underline_point = underline_point - m_PreviousFontInfo.dir_pos;

    if (font_params.text_width < 1e-10 || font_params.font_height < 1e-10)
      return true;

    CFX_FloatRect text_rect(0, 0, font_params.text_width*DPI, font_params.font_height*DPI);
    if(pObject2Device)
      text_rect = pObject2Device->TransformRect(text_rect);
    text_rect.Deflate(1, 1);
    text_rect.left = (int)text_rect.left;
    text_rect.right = (int)text_rect.right;
    text_rect.top = (int)text_rect.top;
    text_rect.bottom = (int)text_rect.bottom;

    if (m_Grabber->isClippingEnabled() && CheckFullClipped(text_rect))
      return true;

    font_params.is_bold = pCXFFont->IsBold();
    font_params.is_italic = pCXFFont->IsItalic();
    font_params.in_font_face_name = face_name.c_str();
    font_params.underline_point_X = underline_point.x;
    font_params.underline_point_Y = underline_point.y;

    m_Grabber->addText(content_str.c_str(), &font_params, &od_color, &matrix, m_IsObjectVisible, m_CurrentLayerName.c_str(), getHyperlink(text_rect));
  }
  catch (...)
  {

  }

  return true;
}

bool OdDeviceDriver::DrawCosmeticLine(const CFX_PointF& ptMoveTo, const CFX_PointF& ptLineTo, uint32_t color, int blend_type)
{
  bool ret_val(true);
  if (!m_Grabber->needToAddGrathics(&ret_val))
    return ret_val;

  OdPdfImportPoint point1(ptMoveTo.x / DPI, (ptMoveTo.y + std::fabs(m_Grabber->getHeight()))/ DPI);
  OdPdfImportPoint point2(ptLineTo.x / DPI, (ptLineTo.y + std::fabs(m_Grabber->getHeight())) / DPI);

  OdPdfImportColor od_color(FXARGB_R(color), FXARGB_G(color), FXARGB_B(color), FXARGB_A(color));

  CFX_FloatRect line_rect(ptMoveTo.x, ptMoveTo.y, ptLineTo.x, ptLineTo.y);

  m_Grabber->addCosmeticLine(&point1, &point2, &od_color, m_IsObjectVisible, m_CurrentLayerName.c_str(), getHyperlink(line_rect));

  return true;
}

bool OdDeviceDriver::SetPixel(int x, int y, uint32_t color)
{
  bool ret_val(true);
  if (!m_Grabber->needToAddGrathics(&ret_val))
    return ret_val;

  OdPdfImportPoint point(x / DPI, (y + std::fabs(m_Grabber->getHeight()))/DPI);
  OdPdfImportColor od_color(FXARGB_R(color), FXARGB_G(color), FXARGB_B(color), FXARGB_A(color));
  
  const double offset = 1e-7;

  CFX_FloatRect pixel_rect(x - offset, y - offset, x + offset, y - offset);

  m_Grabber->addPixel(&point, &od_color, m_IsObjectVisible, m_CurrentLayerName.c_str(), getHyperlink(pixel_rect));

  return true;
}

bool OdDeviceDriver::DrawShadingAsBitmap(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, const FX_RECT* pSrcRect, int left, int top, int blend_type)
{
  bool ret_val(true);
  if (!m_Grabber->needToAddShading(&ret_val))
    return ret_val;

  OdPdfImportRect rect(0, 0, pSrcRect->right / DPI, pSrcRect->bottom / DPI);

  OdPdfImportMatrix matrix;
  matrix.e = left / DPI;
  matrix.f = (top + std::fabs(m_Grabber->getHeight())) / DPI;

  RetainPtr<CFX_DIBSource> pBitmapImpl;
  if (m_Grabber->isImageClippingEnabled())
  {
    CFX_FloatRect img_rect(left, top, left + pSrcRect->right, top + pSrcRect->bottom);
    img_rect.Normalize();
    if (CheckFullClipped(img_rect))
      return true;

    auto pClippedBitmap = pdfium::MakeRetain<CFX_DIBitmap>();
    pClippedBitmap->Create(pBitmap->GetWidth(), pBitmap->GetHeight(), FXDIB_Argb);

    auto image_clip_driver = pdfium::MakeUnique<CFX_AggDeviceDriver>(pClippedBitmap, false, nullptr, false);

    CFX_Matrix image_clip_matrix;
    for (auto& clip_info : m_ClipInfo)
    {
      CFX_PathData clip_path(clip_info.path);
      CFX_Matrix path_clip_matrix;
      path_clip_matrix.e = -left;
      path_clip_matrix.f = clip_info.matrix.f - top;
      clip_path.Transform(&path_clip_matrix);
      image_clip_driver->SetClip_PathFill(&clip_path, &image_clip_matrix, clip_info.fill_mode);
    }



    image_clip_matrix.a = pBitmap->GetWidth();
    image_clip_matrix.d = -pBitmap->GetHeight();
    image_clip_matrix.f = pBitmap->GetHeight();

    std::unique_ptr<CFX_ImageRenderer> image_render_handle;
    image_clip_driver->StartDIBits(pBitmap, 255, color, &image_clip_matrix, 0, &image_render_handle, blend_type);
    image_clip_driver->ContinueDIBits(image_render_handle.get(), nullptr);
    pBitmapImpl = pClippedBitmap;
  }
  else
  {
    pBitmapImpl = pBitmap;
    if (nullptr == pBitmap->GetBuffer())
      pBitmapImpl = pBitmap->Clone(nullptr);
  }

  BmiHeaderInfo bmiHeader;
  fillBmiHeader(pBitmapImpl, bmiHeader);

  CFX_FloatRect image_rect(*pSrcRect);
  image_rect.Translate(left, top);
  image_rect.Deflate(1, 1);

  m_Grabber->addShading(&bmiHeader, pBitmapImpl->GetBuffer(), &rect, &matrix, m_IsObjectVisible, m_CurrentLayerName.c_str(), getHyperlink(image_rect));

  return true;
}


