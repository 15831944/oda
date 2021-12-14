/////////////////////////////////////////////////////////////////////////////// 
// Created by Open Design Alliance 
///////////////////////////////////////////////////////////////////////////////

#ifndef ODRENDERSTATUS_H_
#define ODRENDERSTATUS_H_

#include <memory>
#include <vector>

#include "core/fpdfapi/page/cpdf_clippath.h"
#include "core/fpdfapi/page/cpdf_graphicstates.h"
#include "core/fpdfapi/render/cpdf_renderoptions.h"
#include "core/fxcrt/unowned_ptr.h"
#include "core/fxge/cfx_renderdevice.h"
#include "core/fpdfapi/render/cpdf_renderstatus.h"

class OdRenderStatus : public CPDF_RenderStatus 
{
 public:
   OdRenderStatus(std::wstring& current_layer_name, bool& is_object_visible, long options);
  ~OdRenderStatus();

  void RenderSingleObject(CPDF_PageObject* pObj, const CFX_Matrix* pObj2Device);
  bool ContinueSingleObject(CPDF_PageObject* pObj, const CFX_Matrix* pObj2Device, IFX_PauseIndicator* pPause);
  void DrawTilingPattern(CPDF_TilingPattern* pPattern, CPDF_PageObject* pPageObj, const CFX_Matrix* pObj2Device, bool bStroke);
  virtual void DrawShading(CPDF_ShadingPattern* pPattern, CFX_Matrix* pMatrix, FX_RECT& clip_rect, int alpha, bool bAlphaMode);

  virtual void RenderObjectList(const CPDF_PageObjectHolder* pObjectHolder, const CFX_Matrix* pObj2Device);

 protected:
  bool ProcessTransparency(CPDF_PageObject* PageObj, const CFX_Matrix* pObj2Device);
  void ProcessObjectNoClip(CPDF_PageObject* PageObj, const CFX_Matrix* pObj2Device);
  void DrawObjWithBackground(CPDF_PageObject* pObj, const CFX_Matrix* pObj2Device);
  bool DrawObjWithBlend(CPDF_PageObject* pObj, const CFX_Matrix* pObj2Device);
  bool ProcessImage(CPDF_ImageObject* pImageObj, const CFX_Matrix* pObj2Device);
  bool ProcessType3Text(CPDF_TextObject* textobj, const CFX_Matrix* pObj2Device);
  bool ProcessText(CPDF_TextObject* textobj, const CFX_Matrix* pObj2Device, CFX_PathData* pClippingPath);
  bool ProcessForm(const CPDF_FormObject* pFormObj, const CFX_Matrix* pObj2Device);
  RetainPtr<CFX_DIBitmap> LoadSMask(CPDF_Dictionary* pSMaskDict, FX_RECT* pClipRect, const CFX_Matrix* pMatrix);

  std::wstring& m_CurrentLayerName;
  bool&         m_IsObjectVisible;
  long          m_PdfImportOptions;
};

#endif  // ODRENDERSTATUS_H_
