/////////////////////////////////////////////////////////////////////////////// 
// Created by Open Design Alliance 
///////////////////////////////////////////////////////////////////////////////

#ifndef ODIMAGERENDERER_H_
#define ODIMAGERENDERER_H_

#include <memory>

#include "core/fpdfapi/render/cpdf_imagerenderer.h"
#include "core/fpdfapi/render/cpdf_imageloader.h"
#include "core/fxcrt/fx_coordinates.h"
#include "core/fxcrt/unowned_ptr.h"
#include "core/fxge/dib/cfx_imagerenderer.h"
#include "core/fxcrt/bytestring.h"

class CPDF_ImageObject;
class CPDF_RenderStatus;

class OdImageRenderer : public CPDF_ImageRenderer 
{
 public:
   OdImageRenderer(std::wstring& current_layer_name, bool& is_object_visible);
  virtual ~OdImageRenderer();

  virtual bool Start(CPDF_RenderStatus* pStatus, CPDF_ImageObject* pImageObject, const CFX_Matrix* pObj2Device,
             bool bStdCS, int blendType);

  virtual bool Start(CPDF_RenderStatus* pStatus, const RetainPtr<CFX_DIBSource>& pDIBSource, FX_ARGB bitmap_argb,
             int bitmap_alpha, const CFX_Matrix* pImage2Device, uint32_t flags, bool bStdCS, int blendType);

 protected:
  virtual bool DrawMaskedImage();
  virtual void CalculateDrawImage(CFX_DefaultRenderDevice* bitmap_device1, CFX_DefaultRenderDevice* bitmap_device2,
                          const RetainPtr<CFX_DIBSource>& pDIBSource, CFX_Matrix* pNewMatrix, const FX_RECT& rect) const;

  std::wstring& m_CurrentLayerName;
  bool&         m_IsObjectVisible;
};

#endif  // ODIMAGERENDERER_H_
