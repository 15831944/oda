#include "oda_files/oda_functions.h"

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
#include "RenderDevice.h"


//////////////////////////////////////////////////////////////////////////
FPDF_EXPORT void FPDF_CALLCONV FPDF_GetMeasureDictInfo(FPDF_DOCUMENT document, int page_index, std::vector<FPDF_PageMeasureDictInfo>& measure_info)
{
  measure_info.clear();

  CPDF_Document* pDoc = CPDFDocumentFromFPDFDocument(document);
  if (pDoc)
  {
    CPDF_Dictionary* pPageDict = pDoc->GetPage(page_index);
    if (pPageDict)
    {
      CPDF_Array* pVPArr = pPageDict->GetArrayFor("VP");
      if (pVPArr)
      {
        for (auto& pObj : *pVPArr)
        {
          CPDF_Dictionary* pVP = pObj->AsDictionary();
          if (pVP)
          {
            CPDF_Dictionary* pMeasureDict = pVP->GetDictFor("Measure");
            if (pMeasureDict)
            {
              CPDF_Array* pXArr = pMeasureDict->GetArrayFor("X");
              if (pXArr && pXArr->GetCount() == 1) //see INT-763 for more info about pXArr->GetCount()
              {
                CPDF_Dictionary* pX = pXArr->GetDictAt(0);
                if (pX)
                {
                  FPDF_PageMeasureDictInfo info;
                  info.measure_factor = pX->GetFloatFor("C");
                  info.BBox.left = info.BBox.bottom = info.BBox.right = info.BBox.top = 0.;
                  CPDF_Array* pBBox = pVP->GetArrayFor("BBox");
                  if (pBBox && 4 == pBBox->GetCount())
                  {
                    info.BBox.left = pBBox->GetNumberAt(0);
                    info.BBox.bottom = pBBox->GetNumberAt(1);
                    info.BBox.right = pBBox->GetNumberAt(2);
                    info.BBox.top = pBBox->GetNumberAt(3);
                  }
                  measure_info.emplace_back(info);
                }
              }
            }
          }
        }
      }
    }
  }
  return;
}

FPDF_EXPORT bool FPDF_CALLCONV FPDF_LoadLayers(FPDF_DOCUMENT document, FPDF_ARRAY& pLay, FPDF_ARRAY& pON, FPDF_ARRAY& pOFF)
{
  if (pLay || pON || pOFF)
    return false;
  CPDF_Array* pLayers = 0;
  CPDF_Array* pONLayers = 0;
  CPDF_Array* pOFFLayers = 0;

  CPDF_Document* pDoc = CPDFDocumentFromFPDFDocument(document);
  if (!pDoc)
    return false;
  CPDF_Dictionary* pRoot = pDoc->GetRoot(); //required
  if (!pRoot)
    return false;
  if (pRoot->KeyExist("OCProperties"))
  {
    pLayers = pRoot->GetDictFor("OCProperties")->GetArrayFor("OCGs"); //required
    pLay = static_cast<FPDF_ARRAY>(pLayers);
    if (!pLayers)
      return true;
    CPDF_Dictionary* pD = pRoot->GetDictFor("OCProperties")->GetDictFor("D"); //required
    if (!pD)
      return true;

    pONLayers = pD->GetArrayFor("ON");
    pOFFLayers = pD->GetArrayFor("OFF");
    pON = static_cast<FPDF_ARRAY>(pONLayers);
    pOFF = static_cast<FPDF_ARRAY>(pOFFLayers);
    if (!pOFFLayers || !pONLayers)
    {
      if (!pONLayers)
      {
        pONLayers = pD->SetNewFor<CPDF_Array>("ON");
        pON = static_cast<FPDF_ARRAY>(pONLayers);
      }
      if (!pOFFLayers)
      {
        pOFFLayers = pD->SetNewFor<CPDF_Array>("OFF");
        pOFF = static_cast<FPDF_ARRAY>(pOFFLayers);
      }
      ByteString baseSt = pD->GetStringFor("BaseState");
      if (baseSt.GetStringLength() == 0 || baseSt == "ON")
      {
        pONLayers->Clear();
        for (unsigned int i = 0; i < pLayers->GetCount(); i++)
        {
          if (pOFFLayers->end() == std::find_if(pOFFLayers->begin(), pOFFLayers->end(),
            [&](const std::unique_ptr<CPDF_Object>& candidate)
          {
            return candidate->GetDirect() == pLayers->GetObjectAt(i)->GetDirect();
          }))
          {
            pONLayers->Add(pLayers->GetObjectAt(i)->Clone());
          }
        }
      }
      else if (baseSt == "OFF")
      {
        pOFFLayers->Clear();
        for (unsigned int i = 0; i < pLayers->GetCount(); i++)
        {
          if (pONLayers->end() == std::find_if(pONLayers->begin(), pONLayers->end(),
            [&](const std::unique_ptr<CPDF_Object>& candidate)
          {
            return candidate->GetDirect() == pLayers->GetObjectAt(i)->GetDirect();
          }))
          {
            pOFFLayers->Add(pLayers->GetObjectAt(i)->Clone());
          }
        }
      }
    }
  }
  return true;
}

FPDF_EXPORT bool FPDF_CALLCONV FPDF_GetPageLayers(FPDF_DOCUMENT document, FPDF_PAGE pPage, int page_index, FPDF_PageLayerInfo& info)
{
  if (!document || !pPage)
    return false;

  memset(&info, 0, sizeof(FPDF_PageLayerInfo));
  std::vector<WideString> layers_list;

  CPDF_Document* pDoc = CPDFDocumentFromFPDFDocument(document);
  if (!pDoc)
    return false;

  //First we need to check the page resources
  ByteString sOCG("OCG");
  CPDF_Dictionary* pPageDict = pDoc->GetPage(page_index);
  if (pPageDict)
  {
    CPDF_Dictionary* pResDict = pPageDict->GetDictFor("Resources");
    if (pResDict)
    {
      CPDF_Dictionary* pPropDict = pResDict->GetDictFor("Properties");
      if (pPropDict && pPropDict->GetCount())
      {
        CPDF_Dictionary::const_iterator iter = pPropDict->begin();
        while (iter != pPropDict->end())
        {
          CPDF_Reference* pRef = (*iter).second->AsReference();
          if (pRef)
          {
            CPDF_Dictionary* pOCG = pRef->GetDirect()->AsDictionary();

            if (pOCG && sOCG == pOCG->GetStringFor("Type"))
            {
              //WideString name = pOCG->GetUnicodeTextFor("Name");
              //const wchar_t* str = name.c_str();
              layers_list.push_back(pOCG->GetUnicodeTextFor("Name"));
            }
          }
          ++iter;
        }
      }
    }
  }

  //Then check the objects on page
  int objCount = FPDFPage_CountObjects(pPage);
  CPDF_Page* page = CPDFPageFromFPDFPage(pPage);
  if (!page)
    return false;

  for (int i = 0; i < objCount; i++)
  {
    CPDF_PageObject* pObj = page->GetPageObjectList()->GetPageObjectByIndex(i);
    if (pObj)
    {
      size_t countIt = 0;
      if (pObj->m_ContentMark.HasRef())
        countIt = pObj->m_ContentMark.CountItems();

      for (unsigned int j = 0; j < countIt; ++j)
      {
        const CPDF_ContentMarkItem& item = pObj->m_ContentMark.GetItem(j);
        if (item.GetName() == "OC" && item.GetParamType() == CPDF_ContentMarkItem::PropertiesDict)
        {
          CPDF_Dictionary* pDict = item.GetParam();
          if (pDict)
          {
            std::vector<WideString>::const_iterator iter = std::find(layers_list.begin(), layers_list.end(), pDict->GetUnicodeTextFor("Name"));
            if (iter == layers_list.end())
            {
              layers_list.push_back(pDict->GetUnicodeTextFor("Name"));
            }
          }
        }
      }
    }
  }

  std::vector<WideString>::const_iterator iter = layers_list.begin();
  while (iter != layers_list.end())
  {
    wcsncpy(info.names[info.nol], iter->c_str(), iter->GetStringLength());
    ++info.nol;
    ++iter;
  }
  return true;
}

FPDF_EXPORT bool FPDF_CALLCONV FPDF_EnableLayer(FPDF_ARRAY layers, FPDF_ARRAY pONLayers, FPDF_ARRAY pOFFLayers, const wchar_t* pName, bool bEnable)
{
  CPDF_Array* pLayers = static_cast<CPDF_Array*>(layers);
  CPDF_Array *pToADD, *pToRemove;
  if (bEnable)
  {
    pToADD = static_cast<CPDF_Array*>(pONLayers);
    pToRemove = static_cast<CPDF_Array*>(pOFFLayers);
  } 
  else
  {
    pToADD = static_cast<CPDF_Array*>(pOFFLayers);
    pToRemove = static_cast<CPDF_Array*>(pONLayers);
  }

  if (!pLayers || pLayers->IsEmpty() || !pToADD || !pToRemove || !pName)
    return false;

  for (unsigned int i = 0; i < pLayers->GetCount(); i++)
  {
    CPDF_Dictionary* pOCG = pLayers->GetObjectAt(i)->AsReference()->GetDirect()->AsDictionary();
    if (pOCG)
    {
      WideString name(pName);
      if (name == pOCG->GetUnicodeTextFor("Name"))
      {
        //remove
        for (unsigned int j = 0; j < pToRemove->GetCount(); j++)
        {
          CPDF_Dictionary* pOCGRem = pToRemove->GetObjectAt(j)->AsReference()->GetDirect()->AsDictionary();
          if (pOCGRem)
          {
            if (name == pOCGRem->GetUnicodeTextFor("Name"))
            {
              pToRemove->RemoveAt(j);
              break;
            }
          }
        }
        //add
        bool bNeedAdd = true;
        for (unsigned int j = 0; j < pToADD->GetCount(); j++)
        {
          CPDF_Dictionary* pOCGAdd = pToADD->GetObjectAt(j)->AsReference()->GetDirect()->AsDictionary();
          if (pOCGAdd)
          {
            if (name == pOCGAdd->GetUnicodeTextFor("Name"))
            {
              bNeedAdd = false;
              break;
            }
          }
        }
        if (bNeedAdd)
          pToADD->Add(pLayers->GetObjectAt(i)->Clone());

        break;
      }
    }
  }
  return true;
}

FPDF_EXPORT bool FPDF_CALLCONV FPDF_IsLayerEnabled(FPDF_ARRAY pOFF, const wchar_t* pName)
{
  CPDF_Array* pOFFLayers = static_cast<CPDF_Array*>(pOFF);
  if (pOFFLayers)
  {
    for (unsigned int i = 0; i < pOFFLayers->GetCount(); i++)
    {
      CPDF_Dictionary* pOCG_OFF = pOFFLayers->GetObjectAt(i)->AsReference()->GetDirect()->AsDictionary();
      if (!pOCG_OFF)
        return true;
      WideString name(pName);
      WideString nameOFF(pOCG_OFF->GetUnicodeTextFor("Name"));
      if (name == nameOFF)
        return false;
    }
  }
  return true;
}

FPDF_EXPORT void FPDF_CALLCONV FPDF_SetPageRenderContext(FPDF_PAGE page, FPDF_PageRenderContext context)
{
  CPDF_Page *pPage = CPDFPageFromFPDFPage(page);
  if (!pPage)
    return;
  if (!context)
    pPage->SetRenderContext(nullptr);
  else
  {
    CPDF_PageRenderContext* pContext = static_cast<CPDF_PageRenderContext*>(context);
    pPage->SetRenderContext(pdfium::WrapUnique(pContext));
  }
}

void OdRenderPageImpl(CPDF_PageRenderContext* pContext, CPDF_Page* pPage, const CFX_Matrix& matrix, const FX_RECT& clipping_rect,
  int flags, bool bNeedToRestore, IFSDK_PAUSE_Adapter* pause, std::wstring& current_layer_name, bool& is_object_visible, long options)
{
  if (!pContext->m_pOptions)
    pContext->m_pOptions = pdfium::MakeUnique<CPDF_RenderOptions>();

  uint32_t option_flags = pContext->m_pOptions->GetFlags();
  if (flags & FPDF_LCD_TEXT)
    option_flags |= RENDER_CLEARTYPE;
  else
    option_flags &= ~RENDER_CLEARTYPE;

  if (flags & FPDF_NO_NATIVETEXT)
    option_flags |= RENDER_NO_NATIVETEXT;
  if (flags & FPDF_RENDER_LIMITEDIMAGECACHE)
    option_flags |= RENDER_LIMITEDIMAGECACHE;
  if (flags & FPDF_RENDER_FORCEHALFTONE)
    option_flags |= RENDER_FORCE_HALFTONE;
#ifndef PDF_ENABLE_XFA
  if (flags & FPDF_RENDER_NO_SMOOTHTEXT)
    option_flags |= RENDER_NOTEXTSMOOTH;
  if (flags & FPDF_RENDER_NO_SMOOTHIMAGE)
    option_flags |= RENDER_NOIMAGESMOOTH;
  if (flags & FPDF_RENDER_NO_SMOOTHPATH)
    option_flags |= RENDER_NOPATHSMOOTH;
#endif  // PDF_ENABLE_XFA
  pContext->m_pOptions->SetFlags(option_flags);

  // Grayscale output
  if (flags & FPDF_GRAYSCALE)
    pContext->m_pOptions->SetColorMode(CPDF_RenderOptions::kGray);

  const CPDF_OCContext::UsageType usage =
    (flags & FPDF_PRINTING) ? CPDF_OCContext::Print : CPDF_OCContext::View;
  pContext->m_pOptions->SetOCContext(
    pdfium::MakeRetain<CPDF_OCContext>(pPage->m_pDocument.Get(), usage));

  pContext->m_pDevice->SaveState();
  pContext->m_pDevice->SetClip_Rect(clipping_rect);
  pContext->m_pContext = pdfium::MakeUnique<CPDF_RenderContext>(pPage);
  pContext->m_pContext->AppendLayer(pPage, &matrix);

  if (flags & FPDF_ANNOT) {
    pContext->m_pAnnots = pdfium::MakeUnique<CPDF_AnnotList>(pPage);
    //display annots as on screen
    pContext->m_pAnnots->DisplayAnnots(pPage, pContext->m_pContext.get(),
      false, &matrix, getOption(options, kImportWidgets), nullptr);
  }

  pContext->m_pRenderer = pdfium::MakeUnique<OdProgressiveRenderer>(
    pContext->m_pContext.get(), pContext->m_pDevice.get(),
    pContext->m_pOptions.get(), current_layer_name, is_object_visible, options);
  pContext->m_pRenderer->Start(pause);
  if (bNeedToRestore)
    pContext->m_pDevice->RestoreState(false);
}

CFX_Matrix GetDisplayMatrix(CPDF_Page* pPage, float xPos, float yPos, float xSize, float ySize, int iRotate)
{
  CFX_FloatRect BBox = pPage->GetPageBBox();
  float PageWidth = BBox.Width();
  float PageHeight = BBox.Height();

  int rotate = pPage->GetPageRotation();
  if (rotate % 2)
    std::swap(PageWidth, PageHeight);

  CFX_Matrix PageMatrix;

  switch (rotate) {
  case 0:
    PageMatrix = CFX_Matrix(1.0f, 0, 0, 1.0f, -BBox.left, -BBox.bottom);
    break;
  case 1:
    PageMatrix =
      CFX_Matrix(0, -1.0f, 1.0f, 0, -BBox.bottom, BBox.right);
    break;
  case 2:
    PageMatrix = CFX_Matrix(-1.0f, 0, 0, -1.0f, BBox.right, BBox.top);
    break;
  case 3:
    PageMatrix = CFX_Matrix(0, 1.0f, -1.0f, 0, BBox.top, -BBox.left);
    break;
  }

  if (PageWidth < 1e-7 || PageHeight < 1e-7)
    return CFX_Matrix();

  float x0 = 0;
  float y0 = 0;
  float x1 = 0;
  float y1 = 0;
  float x2 = 0;
  float y2 = 0;
  iRotate %= 4;
  switch (iRotate) {
  case 0:
    x0 = xPos;
    y0 = yPos + ySize;
    x1 = xPos;
    y1 = yPos;
    x2 = xPos + xSize;
    y2 = yPos + ySize;
    break;
  case 1:
    x0 = xPos;
    y0 = yPos;
    x1 = xPos + xSize;
    y1 = yPos;
    x2 = xPos;
    y2 = yPos + ySize;
    break;
  case 2:
    x0 = xPos + xSize;
    y0 = yPos;
    x1 = xPos + xSize;
    y1 = yPos + ySize;
    x2 = xPos;
    y2 = yPos;
    break;
  case 3:
    x0 = xPos + xSize;
    y0 = yPos + ySize;
    x1 = xPos;
    y1 = yPos + ySize;
    x2 = xPos + xSize;
    y2 = yPos;
    break;
  }
  CFX_Matrix matrix = PageMatrix;
  matrix.Concat(CFX_Matrix((x2 - x0) / PageWidth, (y2 - y0) / PageWidth,
    (x1 - x0) / PageHeight, (y1 - y0) / PageHeight,
    x0, y0));
  return matrix;
}

void RenderPage_Retail(CPDF_PageRenderContext* pContext, FPDF_PAGE page, float start_x, float start_y, float size_x,
  float size_y, int rotate, int flags, bool bNeedToRestore, IFSDK_PAUSE_Adapter* pause, std::wstring& current_layer_name, bool& is_object_visible, long options)
{
  CPDF_Page* pPage = CPDFPageFromFPDFPage(page);
  if (!pPage)
    return;

  OdRenderPageImpl(pContext, pPage, GetDisplayMatrix(pPage,
    start_x, start_y, size_x, size_y, rotate),
    FX_RECT(start_x, start_y, start_x + size_x, start_y + size_y),
    flags, bNeedToRestore, pause, current_layer_name, is_object_visible, options);
}

FPDF_EXPORT void FPDF_CALLCONV FPDF_ImportPage(FPDF_PAGE page, OdPdfImportBaseObjectGrabber* grabber, long options)
{
  if (NULL == grabber || NULL == page)
    return;
  std::wstring layer_name;
  bool is_object_visible;

  CPDF_Page* pPage = CPDFPageFromFPDFPage(page);

  CPDF_PageRenderContext* pContext = new CPDF_PageRenderContext;
  pPage->SetRenderContext(pdfium::WrapUnique(pContext));

  RenderDevice* pDevice = new RenderDevice;
  pContext->m_pDevice.reset(pDevice);

  int flags = FPDF_ANNOT;
  if (getOption(options, kImportTTFontAsGeometry))
    flags = flags | RENDER_NO_NATIVETEXT | RENDER_NOTEXTSMOOTH;

  pDevice->SetDeviceDriver(pdfium::MakeUnique<OdDeviceDriver>(grabber, layer_name, is_object_visible, page));

  RenderPage_Retail(pContext, page, 0., 0., grabber->getWidth(), grabber->getHeight(), 0, flags, true, nullptr, layer_name, is_object_visible, options);

  pPage->SetRenderContext(nullptr);
}

//////////////////////////////////////////////////////////////////////////

void getLinkInfoFromPDFPage(FPDF_PAGE page, std::vector<LinkInfo>& link_info)
{
  CPDF_Page* pPage = CPDFPageFromFPDFPage(page);
  CPDF_Array* pAnnotList = pPage->m_pFormDict->GetArrayFor("Annots");
  if (!pAnnotList)
    return;

  const auto link_size = pAnnotList->GetCount();

  for (size_t i = 0; i < link_size; ++i)
  {
    CPDF_Dictionary* pAnnot = pAnnotList->GetDictAt(i);
    if (pAnnot && pAnnot->GetStringFor("Subtype") == "Link")
    {
      CPDF_Link link(pAnnot);
      CPDF_Action action = link.GetAction();
      if (CPDF_Action::ActionType::URI == action.GetType())
      {
        auto uri = action.GetURI(pPage->m_pDocument.Get());
        if (!uri.IsEmpty())
        {
          LinkInfo info;
          info.link = uri;
          info.rect = link.GetRect();
          link_info.emplace_back(info);
        }
      }
    }
  }
}
