/////////////////////////////////////////////////////////////////////////////// 
// Created by Open Design Alliance 
///////////////////////////////////////////////////////////////////////////////

#ifndef ODRENDERDEVICE_H_
#define ODRENDERDEVICE_H_

#include "core/fxge/cfx_renderdevice.h"
#include "core/fpdfapi/page/cpdf_textobject.h"
#include "OdDeviceDriver.h"

inline bool getOption(long flags, long bit)
{
  return (flags & bit) ? true : false;
}

//CORE-14446 CORE-14441 PdfImport for Mac
//CORE-14782
class RenderDevice : public CFX_RenderDevice
{
public:
  RenderDevice();

  bool GetZeroAreaPath(const CFX_PathData* Path, const CFX_Matrix* pMatrix,
    bool bAdjust,
    CFX_PathData* NewPath,
    bool* bThin,
    bool* setIdentity) const;

  virtual bool DrawPathWithBlend(const CFX_PathData* pPathData,
    const CFX_Matrix* pObject2Device,
    const CFX_GraphStateData* pGraphState,
    uint32_t fill_color,
    uint32_t stroke_color,
    int fill_mode,
    int blend_type);

  bool DrawNormalTextEx(int nChars,
    const FXTEXT_CHARPOS* pCharPos,
    CFX_Font* pFont,
    float font_size,
    const CFX_Matrix* pText2Device,
    uint32_t fill_color,
    uint32_t text_flags,
    const bool isType3Font,
    const WideString& unicode_str,
    const OdPdfiumFontInfo& type3_info,
    CPDF_Font* pCPDFFont);

protected:
  virtual bool ShouldDrawDeviceText(const CFX_Font* pFont, uint32_t text_flags);
};

#endif // ODRENDERDEVICE_H_
