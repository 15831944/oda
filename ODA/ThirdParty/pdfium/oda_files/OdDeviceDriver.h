/////////////////////////////////////////////////////////////////////////////// 
// Created by Open Design Alliance 
///////////////////////////////////////////////////////////////////////////////

#ifndef _ODPDFDEVICEDRIVER_INCLUDED_
#define _ODPDFDEVICEDRIVER_INCLUDED_

#include "PdfImportBaseObjectGrabber.h"

#include <memory>
#include <vector>

#include "core/fxcrt/retain_ptr.h"
#include "core/fxge/cfx_pathdata.h"
#include "core/fxge/ifx_renderdevicedriver.h"


struct OdPdfiumFontInfo
{
  struct Type3FontInfo
  {
    float m_TextWidth { 0. };
    float m_TextHeight { 0. };
  } type3info;

  double space_width { 0.0 };
};

struct PreviousFontInfo
{
  CFX_Font* pFont{ nullptr };
  CFX_Matrix matrix;
  float font_size{ 0. };
  uint32_t color{ 0 };
  double underline_position{ 0. };
  CFX_PointF dir_pos;
  double text_width{ 0. };
  double full_text_width{ 0. };
};

struct LinkInfo
{
  ByteString     link;
  CFX_FloatRect  rect;
  bool           applied{ false };
};

class CPDF_TextObject;

class OdDeviceDriver : public IFX_RenderDeviceDriver
{
public:
  explicit OdDeviceDriver(OdPdfImportBaseObjectGrabber* grabber, const std::wstring& layer_name, bool& is_object_visible, FPDF_PAGE page);
  ~OdDeviceDriver() override;

  // IFX_RenderDeviceDriver
  virtual int GetDeviceCaps(int caps_id) const;
  virtual void SaveState();
  virtual void RestoreState(bool bKeepSaved);
  virtual bool SetClip_PathFill(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, int fill_mode);
  virtual bool SetClip_PathStroke(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, const CFX_GraphStateData* pGraphState);
  virtual bool GetClipBox(FX_RECT* pRect);
  virtual bool StretchDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, int dest_left, int dest_top, int dest_width,
    int dest_height, const FX_RECT* pClipRect, uint32_t flags, int blend_type);
  virtual bool FillRectWithBlend(const FX_RECT* pRect, uint32_t fill_color, int blend_type);
  virtual bool DrawShading(const CPDF_ShadingPattern* pPattern, const CFX_Matrix* pMatrix, const FX_RECT& clip_rect,
    int alpha, bool bAlphaMode);
  virtual bool SetBitsWithMask(const RetainPtr<CFX_DIBSource>& pBitmap, const RetainPtr<CFX_DIBSource>& pMask,
    int left, int top, int bitmap_alpha, int blend_type);
  virtual int GetDriverType() const;

  bool DrawPath(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, const CFX_GraphStateData* pGraphState,
    uint32_t fill_color, uint32_t stroke_color, int fill_mode, int blend_type) override;
  bool SetDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, const FX_RECT* pSrcRect, int left, int top, int blend_type) override;
  bool StartDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, int bitmap_alpha, uint32_t color, const CFX_Matrix* pMatrix, uint32_t render_flags,
    std::unique_ptr<CFX_ImageRenderer>* handle, int blend_type) override;
  bool DrawDeviceText(int nChars, const FXTEXT_CHARPOS* pCharPos, CFX_Font* pFont, const CFX_Matrix* pObject2Device, float font_size, uint32_t color) override;
  bool DrawCosmeticLine(const CFX_PointF& ptMoveTo, const CFX_PointF& ptLineTo, uint32_t color, int blend_type);
  bool SetPixel(int x, int y, uint32_t color) override;
  bool DrawShadingAsBitmap(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, const FX_RECT* pSrcRect, int left, int top, int blend_type);

  bool DrawDeviceTextEx(int nChars, const FXTEXT_CHARPOS* pCharPos, CFX_Font* pFont, const CFX_Matrix* pObject2Device, float font_size, uint32_t color,
    const bool isType3Font, const WideString& unicode_str, const OdPdfiumFontInfo& font_info, CPDF_Font* pCPDFFont);

private:
  void fillBmiHeader(RetainPtr<CFX_DIBSource>& pBitmap, BmiHeaderInfo& bmiHeader);

  bool preparePath(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, std::vector<OdPdfImportPathData>& out_path);

  float CheckSpace(const CFX_Matrix* pObject2Device, const double text_width, const double& space_width, bool& is_char_space, bool& is_space);
  bool isSameFontInfo(const CFX_Font* pFont, const CFX_Matrix* pObject2Device, const float font_size, const  uint32_t color);

  const char* getHyperlink(const CFX_FloatRect& rect);

  bool CheckFullClipped(const CFX_FloatRect& rect);

  const std::wstring&                  m_CurrentLayerName;
  bool&                                m_IsObjectVisible;
  OdPdfImportBaseObjectGrabber*        m_Grabber;
  static const double                  DPI;
  
  PreviousFontInfo                     m_PreviousFontInfo;
  bool                                 m_NeedToCombineText{false};
  double                               m_CharSpaceFactor{0.6};
  double                               m_SpaceFactor{1.5};
  std::vector<LinkInfo>                m_LinkInfo;

  struct ClipInfo 
  {
    CFX_PathData path;
    CFX_Matrix   matrix;
    int fill_mode{ 0 };
    CFX_FloatRect bbox;
  };
  std::vector<ClipInfo>               m_ClipInfo;
  std::vector<std::vector<ClipInfo> > m_ClipStack;
  ClipInfo                            m_AdditionalClipPath; //for underlay clipping
};

#endif // _ODPDFDEVICEDRIVER_INCLUDED_
