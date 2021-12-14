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

#ifndef OD_RST_TOOLS_H_
#define OD_RST_TOOLS_H_

#include "Gi/GiRasterImage.h"
#include "ExGiRasterImage.h"

inline bool OdGiRasterImage_isGreyscale(const OdGiRasterImage* pImage)
{
  ODA_ASSERT(pImage);
  for (OdUInt32 idx = 0, sz = (pImage->paletteDataSize() / sizeof(ODCOLORREF)); idx < sz; idx++)
  {
    ODCOLORREF rgb = pImage->color(idx);
    OdUInt8 red = ODGETRED(rgb), green = ODGETGREEN(rgb), blue = ODGETBLUE(rgb);
    if (red != green || red != blue)
      return false;
  }
  return true;
}

inline bool OdGiRasterImage_isColorPresent(const OdGiRasterImage* pImage, ODCOLORREF color, bool bDoAlphaStrongOnly = false)
{
  ODA_ASSERT(pImage);
  bool bRes = false;
  if (pImage->paletteDataSize()) // CORE-13779
  {
    ODA_FAIL_ONCE(); // TODO
    return bRes;
  }
  OdUInt32 depth = pImage->colorDepth();
  ODA_ASSERT(depth == 24 || depth == 32);
  OdUInt32 offs = (depth == 24) ? 3 : 4;
  OdUInt8 clr[3];
  if (pImage->pixelFormat().isBGR() || pImage->pixelFormat().isBGRA())
    clr[2] = ODGETRED(color), clr[1] = ODGETGREEN(color), clr[0] = ODGETBLUE(color);
  else if (pImage->pixelFormat().isRGB() || pImage->pixelFormat().isRGBA())
    clr[0] = ODGETRED(color), clr[1] = ODGETGREEN(color), clr[2] = ODGETBLUE(color);
  else
  { 
    ODA_FAIL_ONCE(); 
  }
  ODA_ASSERT_ONCE(!ODGETALPHA(color));

  const OdUInt8* pData = pImage->scanLines();
  OdUInt32 height = pImage->pixelHeight(),
           width = pImage->pixelWidth();
  ODA_ASSERT_ONCE((width * offs) == pImage->scanLineSize());
  for (OdUInt32 row = 0; row < height; row++)
  {
    for (OdUInt32 col = 0; col < width; col++, pData += offs)
    {
      if (offs == 4)
      {
        if (!pData[3])
          continue; // alpha
        if (pData[3] != 255 && bDoAlphaStrongOnly)
        {
          // CORE-11493
          OdUInt8* pData_ = const_cast<OdUInt8*>(pData);
          pData_[0] = pData_[1] = pData_[2] = pData_[3] = 0;
          continue; // alpha
        }
      }
      if (clr[0] != pData[0] || clr[1] != pData[1] || clr[2] != pData[2])
        continue;
      if (!bDoAlphaStrongOnly)
        return true;
      bRes = true;
    }
  }
  return bRes;
}

/** \details
  Find transparent color index in image palette and set it.
    
  \param pImage [in/out]  Image to adjust.
  \returns transparent color index in palette or -1.
*/
inline int OdGiRasterImage_adjustTransparentColor(OdGiRasterImage* pImage)
                                                //,bool bConvertColorDepth1To8 = false,
                                                //  bool bForceToZeroClrDepth1 = false,
                                                //  int* pIdxFreeColor = NULL) // for bConvertColorDepth1To8 only
{
  //if (pIdxFreeColor)
  //  *pIdxFreeColor = -1;
  OdUInt32 clrDepth = pImage->colorDepth();
  if (!clrDepth || clrDepth > 8)
    return -1;
  ODA_ASSERT_ONCE(clrDepth == 1 || clrDepth == 8); //test

  ODA_ASSERT_VAR(OdGiRasterImage::TransparencyMode mode = pImage->transparencyMode();)
    ODA_ASSERT_ONCE(mode != OdGiRasterImage::kTransparencyOff);
  int idx = pImage->transparentColor();
  if (idx >= 0)
    return idx; // already done

  OdExGiRasterImage* pExImg = static_cast<OdExGiRasterImage*>(pImage); // unfortunatelly rxInit is missing for this class

  OdExGiRasterImage::Palette& colors = pExImg->palette();
  int sz = (int) colors.numColors();
  if (!sz)
    return -1;
  ODA_ASSERT_ONCE_X(TDWF, sz)

  OdBinaryData& rst = pExImg->bits();
  int rstSz = (int)rst.size();
  int idxTransparentColor = -1, // none
      idxFirst = rst.getAt(0);
  idxFirst &= ((1 << clrDepth) - 1);
  OdUInt8 blue, green, red, alpha;
  for (idx = 0; idx < sz; idx++)
  {
    colors.colorAt(idx, blue, green, red, &alpha);
    if (alpha)
      continue;
    if (idxTransparentColor < 0 || idx == idxFirst)
      idxTransparentColor = int(idx);
    if (clrDepth != 1)
      break;
  }

  //if (idxTransparentColor >= 0 && (bConvertColorDepth1To8 || bForceToZeroClrDepth1))
  //{
  //  OdUInt8 blue, green, red, alpha,
  //    blue0, green0, red0, alpha0;
  //  colors.colorAt(0, blue0, green0, red0, &alpha0);
  //  colors.colorAt(1, blue, green, red, &alpha);
  //  if (bConvertColorDepth1To8 && clrDepth == 1)
  //  {
  //    OdUInt32 srcLineSz = pExImg->scanLineSize(); // 1 bit per pixel index + around
  //    int width = pImage->pixelWidth(), height = pImage->pixelHeight();
  //    pExImg->setMetrics(pImage->pixelWidth(), pImage->pixelHeight(), 8);
  //    colors.setNumColors(256);
  //    if (pIdxFreeColor)
  //    {
  //      *pIdxFreeColor = 2;
  //    }
  //    //else
  //    //{
  //    //  // free (0, 0, 0) for background
  //    //  // now only it works as background for image with palette in  OdDwfDevice::rasterImageProc
  //    //  OdUInt8 tone = 64;
  //    //  if (!idxTransparentColor && !blue && !green && !red)
  //    //    colors.setColorAt(1, tone, tone, tone, alpha); // blue = blue0, green = green0, red = red0;
  //    //  else if (idxTransparentColor == 1 && !blue0 && !green0 && !red0)
  //    //    colors.setColorAt(0, tone, tone, tone, alpha0);
  //    //}
  //    blue = (blue + blue0) / 2;
  //    green = (green + green0) / 2;
  //    red = (red + red0) / 2;
  //
  //    for (idx = 2; idx < 256; idx++)
  //    {
  //      colors.setColorAt(idx, blue, green, red, alpha);
  //    }
  //    if (idxTransparentColor && bForceToZeroClrDepth1)
  //    {
  //      colors.colorAt(idxTransparentColor, blue, green, red, &alpha);
  //      colors.setColorAt(0, blue, green, red, alpha);
  //      colors.setColorAt(idxTransparentColor, blue0, green0, red0, alpha0);
  //    }
  //
  //    OdUInt32 dstLineSz = pExImg->scanLineSize(); // 1 byte for pixel index + around
  //
  //    OdBinaryData srcRst = pExImg->bits();
  //    OdUInt32 srcSz = OdExGiRasterImage::bitDataSize(width, height, 1);
  //    ODA_ASSERT_ONCE_X(TDWF, srcSz == srcRst.size() && srcSz == (srcLineSz * height));
  //    OdUInt32 dstSz = OdExGiRasterImage::bitDataSize(width, height, 8);
  //    ODA_ASSERT_ONCE_X(TDWF, dstSz == (dstLineSz * height));
  //    rst.resize(dstSz);
  //    for (int row = 0; row < height; row++)
  //    {
  //      for (int col = 0; col < width; col += 8)
  //      {
  //        OdUInt8 pixels = srcRst[row * srcLineSz + (col / 8)];
  //        OdUInt8 mask = 0x80, pxl;
  //        for (int idxBit = 0; idxBit < 8; idxBit++)
  //        {
  //          if ((col + idxBit) >= (int)dstLineSz)
  //            break;
  //          if (idxBit)
  //            mask = (0x80 >> idxBit);
  //          if (mask & pixels)
  //            pxl = (idxTransparentColor && bForceToZeroClrDepth1) ? 0 : 1;
  //          else
  //            pxl = (idxTransparentColor && bForceToZeroClrDepth1) ? 1 : 0;
  //          rst[row * dstLineSz + col + idxBit] = pxl;
  //        }
  //      }
  //    }
  //    if (idxTransparentColor && bForceToZeroClrDepth1)
  //      idxTransparentColor = 0;
  //  }
  //  else if (idxTransparentColor && clrDepth == 1 && bForceToZeroClrDepth1)
  //  {
  //    colors.colorAt(idxTransparentColor, blue, green, red, &alpha);
  //    colors.setColorAt(0, blue, green, red, alpha);
  //    colors.setColorAt(idxTransparentColor, blue0, green0, red0, alpha0);
  //    for (idx = 0; idx < rstSz; idx++)
  //    {
  //      OdUInt8& pixels = rst[idx];
  //      pixels = ~pixels;
  //    }
  //    idxTransparentColor = 0;
  //  }
  //}

  if (idxTransparentColor >= 0)
    pExImg->setTransparentColor(idxTransparentColor);
  return idxTransparentColor;
}

#endif //OD_RST_TOOLS_H_
