/////////////////////////////////////////////////////////////////////////////// 
// Created by Open Design Alliance 
///////////////////////////////////////////////////////////////////////////////

#include "oda_files/RenderDevice.h"

#include "core/fpdfapi/parser/cpdf_document.h"
#include "core/fpdfapi/parser/cpdf_array.h"
#include "core/fpdfapi/page/cpdf_page.h"
#include "core/fpdfapi/page/cpdf_pageobject.h"
#include "core/fpdfapi/parser/cpdf_dictionary.h"
#include "core/fpdfapi/parser/cpdf_reference.h"
#include "core/fpdfapi/cpdf_pagerendercontext.h"
#include "public/fpdf_edit.h"
#include "fpdfsdk/fsdk_define.h"
#include "core/fpdfapi/render/cpdf_renderoptions.h"
#include "fpdfsdk/fsdk_pauseadapter.h"

#include "oda_files/ProgressiveRenderer.h"
#include "oda_files/OdDeviceDriver.h"
#include "core/fxge/cfx_font.h"
#include "core/fxge/cfx_gemodule.h"



RenderDevice::RenderDevice() :CFX_RenderDevice()
{

}

bool RenderDevice::GetZeroAreaPath(const CFX_PathData* Path, const CFX_Matrix* pMatrix, bool bAdjust, CFX_PathData* NewPath, bool* bThin, bool* setIdentity) const
{
  *setIdentity = false;

  std::vector<FX_PATHPOINT> points = Path->GetPoints();

  if (points.size() < 3)
    return false;

  if (points.size() == 3 && points[0].m_Type == FXPT_TYPE::MoveTo &&
    points[1].m_Type == FXPT_TYPE::LineTo &&
    points[2].m_Type == FXPT_TYPE::LineTo &&
    points[0].m_Point == points[2].m_Point) {
    for (size_t i = 0; i < 2; i++) {
      CFX_PointF point = points[i].m_Point;
      if (bAdjust) {
        if (pMatrix)
          point = pMatrix->Transform(point);

        //point = CFX_PointF(static_cast<int>(point.x) + 0.5f,
        //                   static_cast<int>(point.y) + 0.5f);
      }
      NewPath->AppendPoint(
        point, i == 0 ? FXPT_TYPE::MoveTo : FXPT_TYPE::LineTo, false);
    }
    if (bAdjust && pMatrix)
      *setIdentity = true;

    // Note, they both have to be not equal.
    if (points[0].m_Point.x != points[1].m_Point.x &&
      points[0].m_Point.y != points[1].m_Point.y) {
      *bThin = true;
    }
    return true;
  }

  if (((points.size() > 3) && (points.size() % 2))) {
    size_t mid = points.size() / 2;
    bool bZeroArea = false;
    CFX_PathData t_path;
    for (int i = 0; i < mid; i++) {
      if (!(points[mid - i - 1].m_Point == points[mid + i + 1].m_Point &&
        points[mid - i - 1].m_Type != FXPT_TYPE::BezierTo &&
        points[mid + i + 1].m_Type != FXPT_TYPE::BezierTo)) {
        bZeroArea = true;
        break;
      }

      t_path.AppendPoint(points[mid - i].m_Point, FXPT_TYPE::MoveTo, false);
      t_path.AppendPoint(points[mid - i - 1].m_Point, FXPT_TYPE::LineTo,
        false);
    }
    if (!bZeroArea) {
      NewPath->Append(&t_path, nullptr);
      *bThin = true;
      return true;
    }
  }

  size_t startPoint = 0;
  size_t next = 0;
  for (size_t i = 0; i < points.size(); i++) {
    FXPT_TYPE point_type = points[i].m_Type;
    if (point_type == FXPT_TYPE::MoveTo) {
      startPoint = i;
    }
    else if (point_type == FXPT_TYPE::LineTo) {
      next = (i + 1 - startPoint) % (points.size() - startPoint) + startPoint;
      if (points[next].m_Type != FXPT_TYPE::BezierTo &&
        points[next].m_Type != FXPT_TYPE::MoveTo) {
        if ((points[i - 1].m_Point.x == points[i].m_Point.x &&
          points[i].m_Point.x == points[next].m_Point.x) &&
          ((points[i].m_Point.y - points[i - 1].m_Point.y) *
          (points[i].m_Point.y - points[next].m_Point.y) >
            0)) {
          size_t pre = i;
          if (fabs(points[i].m_Point.y - points[i - 1].m_Point.y) <
            fabs(points[i].m_Point.y - points[next].m_Point.y)) {
            pre--;
            next--;
          }

          NewPath->AppendPoint(points[pre].m_Point, FXPT_TYPE::MoveTo, false);
          NewPath->AppendPoint(points[next].m_Point, FXPT_TYPE::LineTo,
            false);
        }
        else if ((points[i - 1].m_Point.y == points[i].m_Point.y &&
          points[i].m_Point.y == points[next].m_Point.y) &&
          ((points[i].m_Point.x - points[i - 1].m_Point.x) *
          (points[i].m_Point.x - points[next].m_Point.x) >
            0)) {
          size_t pre = i;
          if (fabs(points[i].m_Point.x - points[i - 1].m_Point.x) <
            fabs(points[i].m_Point.x - points[next].m_Point.x)) {
            pre--;
            next--;
          }

          NewPath->AppendPoint(points[pre].m_Point, FXPT_TYPE::MoveTo, false);
          NewPath->AppendPoint(points[next].m_Point, FXPT_TYPE::LineTo,
            false);
        }
        else if (points[i - 1].m_Type == FXPT_TYPE::MoveTo &&
          points[next].m_Type == FXPT_TYPE::LineTo &&
          points[i - 1].m_Point == points[next].m_Point &&
          points[next].m_CloseFigure) {
          NewPath->AppendPoint(points[i - 1].m_Point, FXPT_TYPE::MoveTo,
            false);
          NewPath->AppendPoint(points[i].m_Point, FXPT_TYPE::LineTo, false);
          *bThin = true;
        }
      }
    }
    else if (point_type == FXPT_TYPE::BezierTo) {
      i += 2;
      continue;
    }
  }

  size_t new_path_size = NewPath->GetPoints().size();
  if (points.size() > 3 && new_path_size > 0)
    *bThin = true;
  return new_path_size != 0;
}

bool RenderDevice::DrawPathWithBlend(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, const CFX_GraphStateData* pGraphState, uint32_t fill_color, uint32_t stroke_color, int fill_mode, int blend_type)
{
  uint8_t stroke_alpha = pGraphState ? FXARGB_A(stroke_color) : 0;
  uint8_t fill_alpha = (fill_mode & 3) ? FXARGB_A(fill_color) : 0;
  const std::vector<FX_PATHPOINT>& pPoints = pPathData->GetPoints();
  if (stroke_alpha == 0 && pPoints.size() == 2) {
    CFX_PointF pos1 = pPoints[0].m_Point;
    CFX_PointF pos2 = pPoints[1].m_Point;
    if (pObject2Device) {
      pos1 = pObject2Device->Transform(pos1);
      pos2 = pObject2Device->Transform(pos2);
    }
    DrawCosmeticLine(pos1, pos2, fill_color, fill_mode, blend_type);
    return true;
  }

  if ((pPoints.size() == 5 || pPoints.size() == 4) && stroke_alpha == 0) {
    CFX_FloatRect rect_f;
    if (!(fill_mode & FXFILL_RECT_AA) &&
      pPathData->IsRect(pObject2Device, &rect_f)) {
      FX_RECT rect_i = rect_f.GetOuterRect();

      // Depending on the top/bottom, left/right values of the rect it's
      // possible to overflow the Width() and Height() calculations. Check that
      // the rect will have valid dimension before continuing.
      if (!rect_i.Valid())
        return false;

      int width = static_cast<int>(ceil(rect_f.right - rect_f.left));
      if (width < 1) {
        width = 1;
        if (rect_i.left == rect_i.right)
          ++rect_i.right;
      }
      int height = static_cast<int>(ceil(rect_f.top - rect_f.bottom));
      if (height < 1) {
        height = 1;
        if (rect_i.bottom == rect_i.top)
          ++rect_i.bottom;
      }
      if (rect_i.Width() >= width + 1) {
        if (rect_f.left - static_cast<float>(rect_i.left) >
          static_cast<float>(rect_i.right) - rect_f.right) {
          ++rect_i.left;
        }
        else {
          --rect_i.right;
        }
      }
      if (rect_i.Height() >= height + 1) {
        if (rect_f.top - static_cast<float>(rect_i.top) >
          static_cast<float>(rect_i.bottom) - rect_f.bottom) {
          ++rect_i.top;
        }
        else {
          --rect_i.bottom;
        }
      }
      if (FillRectWithBlend(&rect_i, fill_color, blend_type))
        return true;
    }
  }
  if ((fill_mode & 3) && stroke_alpha == 0 && !(fill_mode & FX_FILL_STROKE) &&
    !(fill_mode & FX_FILL_TEXT_MODE)) {
    CFX_PathData newPath;
    bool bThin = false;
    bool setIdentity = false;
    if (GetZeroAreaPath(pPathData, pObject2Device,
      !!m_pDeviceDriver->GetDriverType(), &newPath,
      &bThin, &setIdentity)) {
      CFX_GraphStateData graphState;
      graphState.m_LineWidth = 0.0f;

      uint32_t strokecolor = fill_color;
      if (bThin)
        strokecolor = (((fill_alpha >> 2) << 24) | (strokecolor & 0x00ffffff));

      const CFX_Matrix* pMatrix = nullptr;
      if (pObject2Device && !pObject2Device->IsIdentity() && !setIdentity)
        pMatrix = pObject2Device;

      int smooth_path = FX_ZEROAREA_FILL;
      if (fill_mode & FXFILL_NOPATHSMOOTH)
        smooth_path |= FXFILL_NOPATHSMOOTH;

      m_pDeviceDriver->DrawPath(&newPath, pMatrix, &graphState, 0, strokecolor,
        smooth_path, blend_type);
    }
  }
  if ((fill_mode & 3) && fill_alpha && stroke_alpha < 0xff &&
    (fill_mode & FX_FILL_STROKE)) {
    if (m_RenderCaps & FXRC_FILLSTROKE_PATH) {
      return m_pDeviceDriver->DrawPath(pPathData, pObject2Device, pGraphState,
        fill_color, stroke_color, fill_mode,
        blend_type);
    }
    return DrawFillStrokePath(pPathData, pObject2Device, pGraphState,
      fill_color, stroke_color, fill_mode, blend_type);
  }
  return m_pDeviceDriver->DrawPath(pPathData, pObject2Device, pGraphState,
    fill_color, stroke_color, fill_mode,
    blend_type);
}

bool RenderDevice::ShouldDrawDeviceText(const CFX_Font* pFont, uint32_t text_flags)
{
#if _FX_PLATFORM_ == _FX_PLATFORM_APPLE_

  const ByteString bsPsName = pFont->GetPsName();
  if (bsPsName.Contains("+ZJHL"))
    return false;

  if (bsPsName == "CNAAJI+cmex10")
    return false;
#endif
  return true;
}

namespace {

  void AdjustGlyphSpace(std::vector<FXTEXT_GLYPHPOS>* pGlyphAndPos) {
    ASSERT(pGlyphAndPos->size() > 1);
    std::vector<FXTEXT_GLYPHPOS>& glyphs = *pGlyphAndPos;
    bool bVertical = glyphs.back().m_Origin.x == glyphs.front().m_Origin.x;
    if (!bVertical && (glyphs.back().m_Origin.y != glyphs.front().m_Origin.y))
      return;

    for (size_t i = glyphs.size() - 1; i > 1; --i) {
      FXTEXT_GLYPHPOS& next = glyphs[i];
      int next_origin = bVertical ? next.m_Origin.y : next.m_Origin.x;
      float next_origin_f = bVertical ? next.m_fOrigin.y : next.m_fOrigin.x;

      FXTEXT_GLYPHPOS& current = glyphs[i - 1];
      int& current_origin = bVertical ? current.m_Origin.y : current.m_Origin.x;
      float current_origin_f =
        bVertical ? current.m_fOrigin.y : current.m_fOrigin.x;

      int space = next_origin - current_origin;
      float space_f = next_origin_f - current_origin_f;
      float error = fabs(space_f) - fabs(static_cast<float>(space));
      if (error > 0.5f)
        current_origin += space > 0 ? -1 : 1;
    }
  }

  const uint8_t g_TextGammaAdjust[256] = {
      0,   2,   3,   4,   6,   7,   8,   10,  11,  12,  13,  15,  16,  17,  18,
      19,  21,  22,  23,  24,  25,  26,  27,  29,  30,  31,  32,  33,  34,  35,
      36,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  51,  52,
      53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,
      68,  69,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,
      84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,
      99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
      114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
      129, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
      143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 156,
      157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171,
      172, 173, 174, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185,
      186, 187, 188, 189, 190, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
      200, 201, 202, 203, 204, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
      214, 215, 216, 217, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
      228, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 239, 240,
      241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 250, 251, 252, 253, 254,
      255,
  };

  int TextGammaAdjust(int value) {
    ASSERT(value >= 0);
    ASSERT(value <= 255);
    return g_TextGammaAdjust[value];
  }

  int CalcAlpha(int src, int alpha) {
    return src * alpha / 255;
  }

  void Merge(uint8_t src, int channel, int alpha, uint8_t* dest) {
    *dest = FXDIB_ALPHA_MERGE(*dest, channel, CalcAlpha(src, alpha));
  }

  void MergeGammaAdjust(uint8_t src, int channel, int alpha, uint8_t* dest) {
    *dest =
      FXDIB_ALPHA_MERGE(*dest, channel, CalcAlpha(TextGammaAdjust(src), alpha));
  }

  void MergeGammaAdjustBgr(const uint8_t* src,
    int r,
    int g,
    int b,
    int a,
    uint8_t* dest) {
    MergeGammaAdjust(src[0], b, a, &dest[0]);
    MergeGammaAdjust(src[1], g, a, &dest[1]);
    MergeGammaAdjust(src[2], r, a, &dest[2]);
  }

  void MergeGammaAdjustRgb(const uint8_t* src,
    int r,
    int g,
    int b,
    int a,
    uint8_t* dest) {
    MergeGammaAdjust(src[2], b, a, &dest[0]);
    MergeGammaAdjust(src[1], g, a, &dest[1]);
    MergeGammaAdjust(src[0], r, a, &dest[2]);
  }

  int AverageRgb(const uint8_t* src) {
    return (src[0] + src[1] + src[2]) / 3;
  }

  uint8_t CalculateDestAlpha(uint8_t back_alpha, int src_alpha) {
    return back_alpha + src_alpha - back_alpha * src_alpha / 255;
  }

  void ApplyAlpha(uint8_t* dest, int b, int g, int r, int alpha) {
    dest[0] = FXDIB_ALPHA_MERGE(dest[0], b, alpha);
    dest[1] = FXDIB_ALPHA_MERGE(dest[1], g, alpha);
    dest[2] = FXDIB_ALPHA_MERGE(dest[2], r, alpha);
  }

  void ApplyDestAlpha(uint8_t back_alpha,
    int src_alpha,
    int r,
    int g,
    int b,
    uint8_t* dest) {
    uint8_t dest_alpha = CalculateDestAlpha(back_alpha, src_alpha);
    ApplyAlpha(dest, b, g, r, src_alpha * 255 / dest_alpha);
    dest[3] = dest_alpha;
  }

  void NormalizeArgb(int src_value,
    int r,
    int g,
    int b,
    int a,
    uint8_t* dest,
    int src_alpha) {
    uint8_t back_alpha = dest[3];
    if (back_alpha == 0)
      FXARGB_SETDIB(dest, FXARGB_MAKE(src_alpha, r, g, b));
    else if (src_alpha != 0)
      ApplyDestAlpha(back_alpha, src_alpha, r, g, b, dest);
  }

  void NormalizeDest(bool has_alpha,
    int src_value,
    int r,
    int g,
    int b,
    int a,
    uint8_t* dest) {
    if (has_alpha) {
      NormalizeArgb(src_value, r, g, b, a, dest,
        CalcAlpha(TextGammaAdjust(src_value), a));
      return;
    }
    int src_alpha = CalcAlpha(TextGammaAdjust(src_value), a);
    if (src_alpha == 0)
      return;

    ApplyAlpha(dest, b, g, r, src_alpha);
  }

  void NormalizeSrc(bool has_alpha,
    int src_value,
    int r,
    int g,
    int b,
    int a,
    uint8_t* dest) {
    if (!has_alpha) {
      ApplyAlpha(dest, b, g, r, CalcAlpha(TextGammaAdjust(src_value), a));
      return;
    }
    int src_alpha = CalcAlpha(TextGammaAdjust(src_value), a);
    if (src_alpha != 0)
      NormalizeArgb(src_value, r, g, b, a, dest, src_alpha);
  }

  void NextPixel(uint8_t** src_scan, uint8_t** dst_scan, int bpp) {
    *src_scan += 3;
    *dst_scan += bpp;
  }

  void SetAlpha(bool has_alpha, uint8_t* alpha) {
    if (has_alpha)
      alpha[3] = 255;
  }

  void DrawNormalTextHelper(const RetainPtr<CFX_DIBitmap>& bitmap,
    const RetainPtr<CFX_DIBitmap>& pGlyph,
    int nrows,
    int left,
    int top,
    int start_col,
    int end_col,
    bool bNormal,
    bool bBGRStripe,
    int x_subpixel,
    int a,
    int r,
    int g,
    int b) {
    const bool has_alpha = bitmap->GetFormat() == FXDIB_Argb;
    uint8_t* src_buf = pGlyph->GetBuffer();
    int src_pitch = pGlyph->GetPitch();
    uint8_t* dest_buf = bitmap->GetBuffer();
    int dest_pitch = bitmap->GetPitch();
    const int Bpp = has_alpha ? 4 : bitmap->GetBPP() / 8;
    for (int row = 0; row < nrows; ++row) {
      int dest_row = row + top;
      if (dest_row < 0 || dest_row >= bitmap->GetHeight())
        continue;

      uint8_t* src_scan = src_buf + row * src_pitch + (start_col - left) * 3;
      uint8_t* dest_scan = dest_buf + dest_row * dest_pitch + start_col * Bpp;
      if (bBGRStripe) {
        if (x_subpixel == 0) {
          for (int col = start_col; col < end_col; ++col) {
            if (has_alpha) {
              Merge(src_scan[2], r, a, &dest_scan[2]);
              Merge(src_scan[1], g, a, &dest_scan[1]);
              Merge(src_scan[0], b, a, &dest_scan[0]);
            }
            else {
              MergeGammaAdjustBgr(&src_scan[0], r, g, b, a, &dest_scan[0]);
            }
            SetAlpha(has_alpha, dest_scan);
            NextPixel(&src_scan, &dest_scan, Bpp);
          }
          continue;
        }
        if (x_subpixel == 1) {
          MergeGammaAdjust(src_scan[1], r, a, &dest_scan[2]);
          MergeGammaAdjust(src_scan[0], g, a, &dest_scan[1]);
          if (start_col > left)
            MergeGammaAdjust(src_scan[-1], b, a, &dest_scan[0]);
          SetAlpha(has_alpha, dest_scan);
          NextPixel(&src_scan, &dest_scan, Bpp);
          for (int col = start_col + 1; col < end_col - 1; ++col) {
            MergeGammaAdjustBgr(&src_scan[-1], r, g, b, a, &dest_scan[0]);
            SetAlpha(has_alpha, dest_scan);
            NextPixel(&src_scan, &dest_scan, Bpp);
          }
          continue;
        }
        MergeGammaAdjust(src_scan[0], r, a, &dest_scan[2]);
        if (start_col > left) {
          MergeGammaAdjust(src_scan[-1], g, a, &dest_scan[1]);
          MergeGammaAdjust(src_scan[-2], b, a, &dest_scan[0]);
        }
        SetAlpha(has_alpha, dest_scan);
        NextPixel(&src_scan, &dest_scan, Bpp);
        for (int col = start_col + 1; col < end_col - 1; ++col) {
          MergeGammaAdjustBgr(&src_scan[-2], r, g, b, a, &dest_scan[0]);
          SetAlpha(has_alpha, dest_scan);
          NextPixel(&src_scan, &dest_scan, Bpp);
        }
        continue;
      }
      if (x_subpixel == 0) {
        for (int col = start_col; col < end_col; ++col) {
          if (bNormal) {
            int src_value = AverageRgb(&src_scan[0]);
            NormalizeDest(has_alpha, src_value, r, g, b, a, dest_scan);
          }
          else {
            MergeGammaAdjustRgb(&src_scan[0], r, g, b, a, &dest_scan[0]);
            SetAlpha(has_alpha, dest_scan);
          }
          NextPixel(&src_scan, &dest_scan, Bpp);
        }
        continue;
      }
      if (x_subpixel == 1) {
        if (bNormal) {
          int src_value = start_col > left ? AverageRgb(&src_scan[-1])
            : (src_scan[0] + src_scan[1]) / 3;
          NormalizeSrc(has_alpha, src_value, r, g, b, a, dest_scan);
        }
        else {
          if (start_col > left)
            MergeGammaAdjust(src_scan[-1], r, a, &dest_scan[2]);
          MergeGammaAdjust(src_scan[0], g, a, &dest_scan[1]);
          MergeGammaAdjust(src_scan[1], b, a, &dest_scan[0]);
          SetAlpha(has_alpha, dest_scan);
        }
        NextPixel(&src_scan, &dest_scan, Bpp);
        for (int col = start_col + 1; col < end_col; ++col) {
          if (bNormal) {
            int src_value = AverageRgb(&src_scan[-1]);
            NormalizeDest(has_alpha, src_value, r, g, b, a, dest_scan);
          }
          else {
            MergeGammaAdjustRgb(&src_scan[-1], r, g, b, a, &dest_scan[0]);
            SetAlpha(has_alpha, dest_scan);
          }
          NextPixel(&src_scan, &dest_scan, Bpp);
        }
        continue;
      }
      if (bNormal) {
        int src_value =
          start_col > left ? AverageRgb(&src_scan[-2]) : src_scan[0] / 3;
        NormalizeSrc(has_alpha, src_value, r, g, b, a, dest_scan);
      }
      else {
        if (start_col > left) {
          MergeGammaAdjust(src_scan[-2], r, a, &dest_scan[2]);
          MergeGammaAdjust(src_scan[-1], g, a, &dest_scan[1]);
        }
        MergeGammaAdjust(src_scan[0], b, a, &dest_scan[0]);
        SetAlpha(has_alpha, dest_scan);
      }
      NextPixel(&src_scan, &dest_scan, Bpp);
      for (int col = start_col + 1; col < end_col; ++col) {
        if (bNormal) {
          int src_value = AverageRgb(&src_scan[-2]);
          NormalizeDest(has_alpha, src_value, r, g, b, a, dest_scan);
        }
        else {
          MergeGammaAdjustRgb(&src_scan[-2], r, g, b, a, &dest_scan[0]);
          SetAlpha(has_alpha, dest_scan);
        }
        NextPixel(&src_scan, &dest_scan, Bpp);
      }
    }
  }

}  // namespace

bool RenderDevice::DrawNormalTextEx(int nChars,
  const FXTEXT_CHARPOS* pCharPos,
  CFX_Font* pFont,
  float font_size,
  const CFX_Matrix* pText2Device,
  uint32_t fill_color,
  uint32_t text_flags,
  const bool isType3Font,
  const WideString& unicode_str,
  const OdPdfiumFontInfo& type3_info,
  CPDF_Font* pCPDFFont) {

  OdDeviceDriver* driver = dynamic_cast<OdDeviceDriver*>(m_pDeviceDriver.get());
  if (nullptr == driver)
    return false;

  int nativetext_flags = text_flags;
  if (m_DeviceClass != FXDC_DISPLAY) {
    if (!(text_flags & FXTEXT_PRINTGRAPHICTEXT)) {
      if (ShouldDrawDeviceText(pFont, text_flags) &&
        driver->DrawDeviceTextEx(nChars, pCharPos, pFont, pText2Device,
          font_size, fill_color, isType3Font, unicode_str, type3_info, pCPDFFont)) {
        return true;
      }
    }
    if (FXARGB_A(fill_color) < 255)
      return false;
  }
  else if (!(text_flags & FXTEXT_NO_NATIVETEXT)) {
    if (ShouldDrawDeviceText(pFont, text_flags) &&
      driver->DrawDeviceTextEx(nChars, pCharPos, pFont, pText2Device,
        font_size, fill_color, isType3Font, unicode_str, type3_info, pCPDFFont)) {
      return true;
    }
  }
  CFX_Matrix char2device;
  CFX_Matrix text2Device;
  if (pText2Device) {
    char2device = *pText2Device;
    text2Device = *pText2Device;
  }

  char2device.Scale(font_size, -font_size);
  if (fabs(char2device.a) + fabs(char2device.b) > 50 * 1.0f ||
    ((m_DeviceClass == FXDC_PRINTER) &&
      !(text_flags & FXTEXT_PRINTIMAGETEXT))) {
    if (pFont->GetFace()) {
      int nPathFlags =
        (text_flags & FXTEXT_NOSMOOTH) == 0 ? 0 : FXFILL_NOPATHSMOOTH;
      return DrawTextPath(nChars, pCharPos, pFont, font_size, pText2Device,
        nullptr, nullptr, fill_color, 0, nullptr, nPathFlags);
    }
  }
  int anti_alias = FXFT_RENDER_MODE_MONO;
  bool bNormal = false;
  if ((text_flags & FXTEXT_NOSMOOTH) == 0) {
    if (m_DeviceClass == FXDC_DISPLAY && m_bpp > 1) {
      if (!CFX_GEModule::Get()->GetFontMgr()->FTLibrarySupportsHinting()) {
        // Some Freetype implementations (like the one packaged with Fedora) do
        // not support hinting due to patents 6219025, 6239783, 6307566,
        // 6225973, 6243070, 6393145, 6421054, 6282327, and 6624828; the latest
        // one expires 10/7/19.  This makes LCD antialiasing very ugly, so we
        // instead fall back on NORMAL antialiasing.
        anti_alias = FXFT_RENDER_MODE_NORMAL;
      }
      else if ((m_RenderCaps & (FXRC_ALPHA_OUTPUT | FXRC_CMYK_OUTPUT))) {
        anti_alias = FXFT_RENDER_MODE_LCD;
        bNormal = true;
      }
      else if (m_bpp < 16) {
        anti_alias = FXFT_RENDER_MODE_NORMAL;
      }
      else {
        anti_alias = FXFT_RENDER_MODE_LCD;

        bool bClearType = false;
        if (pFont->GetFace())
          bClearType = !!(text_flags & FXTEXT_CLEARTYPE);
        bNormal = !bClearType;
      }
    }
  }
  std::vector<FXTEXT_GLYPHPOS> glyphs(nChars);
  CFX_Matrix matrixCTM = GetCTM();
  float scale_x = fabs(matrixCTM.a);
  float scale_y = fabs(matrixCTM.d);
  CFX_Matrix deviceCtm = char2device;
  CFX_Matrix m(scale_x, 0, 0, scale_y, 0, 0);
  deviceCtm.Concat(m);
  text2Device.Concat(m);

  for (size_t i = 0; i < glyphs.size(); ++i) {
    FXTEXT_GLYPHPOS& glyph = glyphs[i];
    const FXTEXT_CHARPOS& charpos = pCharPos[i];

    glyph.m_fOrigin = text2Device.Transform(charpos.m_Origin);
    if (anti_alias < FXFT_RENDER_MODE_LCD)
      glyph.m_Origin.x = FXSYS_round(glyph.m_fOrigin.x);
    else
      glyph.m_Origin.x = static_cast<int>(floor(glyph.m_fOrigin.x));
    glyph.m_Origin.y = FXSYS_round(glyph.m_fOrigin.y);

    if (charpos.m_bGlyphAdjust) {
      CFX_Matrix new_matrix(
        charpos.m_AdjustMatrix[0], charpos.m_AdjustMatrix[1],
        charpos.m_AdjustMatrix[2], charpos.m_AdjustMatrix[3], 0, 0);
      new_matrix.Concat(deviceCtm);
      glyph.m_pGlyph = pFont->LoadGlyphBitmap(
        charpos.m_GlyphIndex, charpos.m_bFontStyle, &new_matrix,
        charpos.m_FontCharWidth, anti_alias, nativetext_flags);
    }
    else {
      glyph.m_pGlyph = pFont->LoadGlyphBitmap(
        charpos.m_GlyphIndex, charpos.m_bFontStyle, &deviceCtm,
        charpos.m_FontCharWidth, anti_alias, nativetext_flags);
    }
  }
  if (anti_alias < FXFT_RENDER_MODE_LCD && glyphs.size() > 1)
    AdjustGlyphSpace(&glyphs);

  FX_RECT bmp_rect1 = FXGE_GetGlyphsBBox(glyphs, anti_alias, 1.0f, 1.0f);
  if (scale_x > 1 && scale_y > 1) {
    --bmp_rect1.left;
    --bmp_rect1.top;
    ++bmp_rect1.right;
    ++bmp_rect1.bottom;
  }
  FX_RECT bmp_rect(FXSYS_round((float)(bmp_rect1.left) / scale_x),
    FXSYS_round((float)(bmp_rect1.top) / scale_y),
    FXSYS_round((float)bmp_rect1.right / scale_x),
    FXSYS_round((float)bmp_rect1.bottom / scale_y));
  bmp_rect.Intersect(m_ClipBox);
  if (bmp_rect.IsEmpty())
    return true;

  int pixel_width = FXSYS_round(bmp_rect.Width() * scale_x);
  int pixel_height = FXSYS_round(bmp_rect.Height() * scale_y);
  int pixel_left = FXSYS_round(bmp_rect.left * scale_x);
  int pixel_top = FXSYS_round(bmp_rect.top * scale_y);
  if (anti_alias == FXFT_RENDER_MODE_MONO) {
    auto bitmap = pdfium::MakeRetain<CFX_DIBitmap>();
    if (!bitmap->Create(pixel_width, pixel_height, FXDIB_1bppMask))
      return false;
    bitmap->Clear(0);
    for (const FXTEXT_GLYPHPOS& glyph : glyphs) {
      if (!glyph.m_pGlyph)
        continue;
      RetainPtr<CFX_DIBitmap> pGlyph = glyph.m_pGlyph->m_pBitmap;
      bitmap->TransferBitmap(
        glyph.m_Origin.x + glyph.m_pGlyph->m_Left - pixel_left,
        glyph.m_Origin.y - glyph.m_pGlyph->m_Top - pixel_top,
        pGlyph->GetWidth(), pGlyph->GetHeight(), pGlyph, 0, 0);
    }
    return SetBitMask(bitmap, bmp_rect.left, bmp_rect.top, fill_color);
  }
  auto bitmap = pdfium::MakeRetain<CFX_DIBitmap>();
  if (m_bpp == 8) {
    if (!bitmap->Create(pixel_width, pixel_height, FXDIB_8bppMask))
      return false;
  }
  else {
    if (!CreateCompatibleBitmap(bitmap, pixel_width, pixel_height))
      return false;
  }
  if (!bitmap->HasAlpha() && !bitmap->IsAlphaMask()) {
    bitmap->Clear(0xFFFFFFFF);
    if (!GetDIBits(bitmap, bmp_rect.left, bmp_rect.top))
      return false;
  }
  else {
    bitmap->Clear(0);
    if (bitmap->m_pAlphaMask)
      bitmap->m_pAlphaMask->Clear(0);
  }
  int dest_width = pixel_width;
  int a = 0;
  int r = 0;
  int g = 0;
  int b = 0;
  if (anti_alias == FXFT_RENDER_MODE_LCD)
    std::tie(a, r, g, b) = ArgbDecode(fill_color);

  for (const FXTEXT_GLYPHPOS& glyph : glyphs) {
    if (!glyph.m_pGlyph)
      continue;

    pdfium::base::CheckedNumeric<int> left = glyph.m_Origin.x;
    left += glyph.m_pGlyph->m_Left;
    left -= pixel_left;
    if (!left.IsValid())
      return false;

    pdfium::base::CheckedNumeric<int> top = glyph.m_Origin.y;
    top -= glyph.m_pGlyph->m_Top;
    top -= pixel_top;
    if (!top.IsValid())
      return false;

    RetainPtr<CFX_DIBitmap> pGlyph = glyph.m_pGlyph->m_pBitmap;
    int ncols = pGlyph->GetWidth();
    int nrows = pGlyph->GetHeight();
    if (anti_alias == FXFT_RENDER_MODE_NORMAL) {
      if (!bitmap->CompositeMask(left.ValueOrDie(), top.ValueOrDie(), ncols,
        nrows, pGlyph, fill_color, 0, 0,
        FXDIB_BLEND_NORMAL, nullptr, false, 0)) {
        return false;
      }
      continue;
    }
    bool bBGRStripe = !!(text_flags & FXTEXT_BGR_STRIPE);
    ncols /= 3;
    int x_subpixel = static_cast<int>(glyph.m_fOrigin.x * 3) % 3;
    int start_col =
      pdfium::base::ValueOrDieForType<int>(pdfium::base::CheckMax(left, 0));
    pdfium::base::CheckedNumeric<int> end_col_safe = left;
    end_col_safe += ncols;
    if (!end_col_safe.IsValid())
      return false;

    int end_col =
      std::min(static_cast<int>(end_col_safe.ValueOrDie<int>()), dest_width);
    if (start_col >= end_col)
      continue;

    DrawNormalTextHelper(bitmap, pGlyph, nrows, left.ValueOrDie(),
      top.ValueOrDie(), start_col, end_col, bNormal,
      bBGRStripe, x_subpixel, a, r, g, b);
  }
  if (bitmap->IsAlphaMask())
    SetBitMask(bitmap, bmp_rect.left, bmp_rect.top, fill_color);
  else
    SetDIBits(bitmap, bmp_rect.left, bmp_rect.top);
  return true;
}
