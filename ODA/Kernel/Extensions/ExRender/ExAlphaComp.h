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

/* We can restore alpha component for bitmap devices which isn't support transparency.
   To do that we need two images rendered on black and white background. Final alpha
   image will be computed from result of these two images.                             */

#ifndef EX_H_ALPHACOMP
#define EX_H_ALPHACOMP

#include "TD_PackPush.h"

/** <group ExRender_Classes>
  \details
  Represent pixel color in floating point format. Provide all necessary operations for alpha calculation.
*/
struct PixColor
{
  float r, g, b, a;

  PixColor() {}
  PixColor(float _r, float _g, float _b, float _a = 1.0f) : r(_r), g(_g), b(_b), a(_a) {}
  PixColor(float _i, float _a = 1.0f) : r(_i), g(_i), b(_i), a(_a) {}
  PixColor(ODCOLORREF cr) : r(float(ODGETRED(cr)) / 255), g(float(ODGETGREEN(cr)) / 255),
                            b(float(ODGETBLUE(cr)) / 255), a(float(ODGETALPHA(cr)) / 255) {}

  PixColor &blend(const PixColor &src, bool bAlpha = false)
  {
    r = src.r * src.a + r * (1.0f - src.a);
    g = src.g * src.a + g * (1.0f - src.a);
    b = src.b * src.a + b * (1.0f - src.a);
    if (bAlpha)
      a = src.a * src.a + a * (1.0f - src.a);
    return *this;
  }
  PixColor &blendPremultipliedAlpha(const PixColor &src, bool bAlpha = false)
  {
    r = src.r + r * (1.0f - src.a);
    g = src.g + g * (1.0f - src.a);
    b = src.b + b * (1.0f - src.a);
    if (bAlpha)
      a = src.a + a * (1.0f - src.a);
    return *this;
  }

  PixColor operator -(const PixColor &p2) const
  { return PixColor(r - p2.r, g - p2.g, b - p2.b, a - p2.a); }
  PixColor &operator -=(const PixColor &p2)
  { r -= p2.r; g -= p2.g; b -= p2.b; return *this; }
  PixColor operator +(const PixColor &p2) const
  { return PixColor(r + p2.r, g + p2.g, b + p2.b, a + p2.a); }
  PixColor &operator +=(const PixColor &p2)
  { r += p2.r; g += p2.g; b += p2.b; return *this; }

  float max(bool bAlpha = false) const
  { float mv = odmax(r, g); mv = odmax(mv, b); if (bAlpha) mv = odmax(mv, a); return mv; }
  float min(bool bAlpha = false) const
  { float mv = odmin(r, g); mv = odmin(mv, b); if (bAlpha) mv = odmin(mv, a); return mv; }

  float average(bool bAlpha = false) const
  { if (!bAlpha) return (r + g + b) / 3;
    return (r + g + b + a) / 4; }

  PixColor &inverse() { r = 1.0f - r; g = 1.0f - g; b = 1.0f - b; a = 1.0f - a; return *this; }
  PixColor operator -() const { return PixColor(1.0f - r, 1.0f - g, 1.0f - b, 1.0f - a); }

  PixColor &setMax(const PixColor &p2) { r = odmax(r, p2.r); g = odmax(g, p2.g); b = odmax(b, p2.b); a = odmax(a, p2.a); return *this; }
  PixColor &setMin(const PixColor &p2) { r = odmin(r, p2.r); g = odmin(g, p2.g); b = odmin(b, p2.b); a = odmin(a, p2.a); return *this; }

  PixColor operator *(float v) const { return PixColor(r * v, g * v, b * v, a * v); }
  PixColor &operator *=(float v) { r *= v; g *= v; b *= v; a *= v; return *this; }
  PixColor operator /(float v) const { return PixColor(r / v, g / v, b / v, a / v); }
  PixColor &operator /=(float v) { r /= v; g /= v; b /= v; a /= v; return *this; }

  PixColor &setColor(float _r, float _g, float _b) { r = _r; g = _g; b = _b; return *this; }
  PixColor &setAlpha(float _a) { a = _a; return *this; }

  PixColor &setZero() { r = g = b = a = 0.0f; return *this; }
  PixColor &setOne() { r = g = b = a = 1.0f; return *this; }

  PixColor &clamp() { if (r < 0.0f) r = 0.0f; else if (r > 1.0f) r = 1.0f;
                      if (g < 0.0f) g = 0.0f; else if (g > 1.0f) g = 1.0f;
                      if (b < 0.0f) b = 0.0f; else if (b > 1.0f) b = 1.0f;
                      if (a < 0.0f) a = 0.0f; else if (a > 1.0f) a = 1.0f; return *this; }
  ODCOLORREF asCRef() const { return ODRGBA(r * 255, g * 255, b * 255, a * 255); }
};

#if 0 // Solution test function
static void runAlphaCompTest()
{
  const PixColor testPixels[8] =
  {
    { 0.0f, 0.0f, 0.0f, 1.0f }, // Opaque black color
    { 1.0f, 1.0f, 1.0f, 1.0f }, // Opaque white color
    { 1.0f, 0.0f, 0.0f, 1.0f }, // Opaque red color
    { 0.9f, 0.5f, 0.1f, 1.0f }, // Opaque some color
    { 0.9f, 0.5f, 0.1f, 0.0f }, // Transparent some color
    { 0.9f, 0.5f, 0.1f, 0.25f }, // Semi-transparent some color
    { 0.9f, 0.5f, 0.1f, 0.5f }, // Semi-transparent some color
    { 0.9f, 0.5f, 0.1f, 0.75f }  // Semi-transparent some color
  };
  for (OdUInt32 nTest = 0; nTest < 8; nTest++)
  {
    // Mix color with black
    const PixColor black = PixColor(0.0f).blend(testPixels[nTest]);
    // Mix color with white
    const PixColor white = PixColor(1.0f).blend(testPixels[nTest]);
    // Compute differences from black and white
    const PixColor diffBW = white - black;
    PixColor result;
    if (OdEqual(diffBW.max(), 1.0f))
      result.setZero(); // Completely transparent - use background color
    else if (OdZero(diffBW.min()))
      result = black; // Original opaque color
    else
    { const float avgDiff = diffBW.average();
      const float avgAlpha = 1.0f - avgDiff;
      if (OdPositive(avgAlpha))
      { const float colorMult = 1.0f / avgAlpha;
        result = (black * colorMult).setAlpha(avgAlpha);
      }
      else // Probably alpha too small
        result.setZero(); // Completely transparent - use background color
    }
    ODA_ASSERT(OdEqual(result.a, testPixels[nTest].a));
    if (OdPositive(result.a))
    {
      ODA_ASSERT(OdEqual(result.r, testPixels[nTest].r));
      ODA_ASSERT(OdEqual(result.g, testPixels[nTest].g));
      ODA_ASSERT(OdEqual(result.b, testPixels[nTest].b));
    }
  }
}
#endif

/** <group ExRender_Classes>
  \details
  Describe input/output image pixels.
*/
template <typename CompType>
struct PixImageDesc
{
  CompType *m_pComponent[4]; // Address of color component (0 - R, 1 - G, 2 - B, 3 - A).
  OdInt32 m_nComponents; // Number of color components.
  OdInt32 m_nScanLength; // Scanline length.
  void incW(OdUInt32 n = 1) { for (OdInt32 nComp = 0; nComp < m_nComponents; nComp++)
                                m_pComponent[nComp] += m_nComponents * OdInt32(n); }
  void incH(OdUInt32 n = 1) { for (OdInt32 nComp = 0; nComp < m_nComponents; nComp++)
                                m_pComponent[nComp] += m_nScanLength * OdInt32(n); }
};
typedef PixImageDesc<const OdUInt8> PixImageDescIn;
typedef PixImageDesc<OdUInt8> PixImageDescOut;

static void computeAlphaTransparency(const PixImageDescIn &blackBg, const PixImageDescIn &whiteBg,
                                     OdUInt32 imgWidth, OdUInt32 imgHeight, const PixImageDescOut &outImg, ODCOLORREF bkColor)
{
  const PixColor bk(bkColor);
  for (OdUInt32 nY = 0; nY < imgHeight; nY++)
  {
    PixImageDescIn blackImg = blackBg; blackImg.incH(nY);
    PixImageDescIn whiteImg = whiteBg; whiteImg.incH(nY);
    PixImageDescOut outData = outImg; outData.incH(nY);
    for (OdUInt32 nX = 0; nX < imgWidth; nX++, blackImg.incW(), whiteImg.incW(), outData.incW())
    {
      // Black and white background pixels
      const PixColor black(float(*blackImg.m_pComponent[0]) / 255, float(*blackImg.m_pComponent[1]) / 255, float(*blackImg.m_pComponent[2]) / 255);
      const PixColor white(float(*whiteImg.m_pComponent[0]) / 255, float(*whiteImg.m_pComponent[1]) / 255, float(*whiteImg.m_pComponent[2]) / 255);
      // Compute differences from black and white
      const PixColor diffBW = white - black;
      PixColor result;
      if (OdEqual(diffBW.max(), 1.0f))
        result.setZero(); // Completely transparent - use background color
      else if (OdZero(diffBW.min()))
        result = black; // Original opaque color
      else
      { const float avgDiff = diffBW.average();
        const float avgAlpha = 1.0f - avgDiff;
        if (OdPositive(avgAlpha))
        { const float colorMult = 1.0f / avgAlpha;
          result = (black * colorMult).setAlpha(avgAlpha);
        }
        else // Probably alpha too small
          result.setZero(); // Completely transparent - use background color
      }
      // Mix final result with background
      result = PixColor(bk).blendPremultipliedAlpha(result, true).clamp();
      *outData.m_pComponent[0] = OdUInt8(result.r * 255);
      *outData.m_pComponent[1] = OdUInt8(result.g * 255);
      *outData.m_pComponent[2] = OdUInt8(result.b * 255);
      *outData.m_pComponent[3] = OdUInt8(result.a * 255);
    }
  }
}

#include "TD_PackPop.h"

#endif // EX_H_ALPHACOMP
