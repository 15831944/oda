/////////////////////////////////////////////////////////////////////////////// 
// Created by Open Design Alliance 
///////////////////////////////////////////////////////////////////////////////

#ifndef ODPROGRESSIVERENDERER_H_
#define ODPROGRESSIVERENDERER_H_

#include <memory>

#include "core/fpdfapi/page/cpdf_pageobjectlist.h"
#include "core/fpdfapi/render/cpdf_rendercontext.h"
#include "core/fpdfapi/render/cpdf_progressiverenderer.h"
#include "core/fxcrt/fx_coordinates.h"
#include "core/fxcrt/fx_system.h"
#include "core/fxcrt/bytestring.h"

class CPDF_RenderOptions;
class CPDF_RenderStatus;
class CFX_RenderDevice;
class IFX_PauseIndicator;

class OdProgressiveRenderer : public CPDF_ProgressiveRenderer 
{
 public:

  OdProgressiveRenderer(CPDF_RenderContext* pContext, CFX_RenderDevice* pDevice, const CPDF_RenderOptions* pOptions, 
                        std::wstring& current_layer_name, bool& is_object_visible, long options);
  ~OdProgressiveRenderer();

  void Continue(IFX_PauseIndicator* pPause);

 private:
   std::wstring&  m_CurrentLayerName;
   bool&          m_IsObjectVisible;
   long           m_PdfImportOptions;
};

#endif  // ODPROGRESSIVERENDERER_H_
