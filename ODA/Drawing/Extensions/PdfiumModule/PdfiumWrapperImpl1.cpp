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

//File added because of names conflict between cpdf_pageobject.h (in it's includes) and Gi enum TextRenderingMode
#include "OdaCommon.h"
#include "PdfiumWrapperImpl.h"

#include "Ge/GeKnotVector.h"
#include "Ge/GeNurbCurve3d.h"

#include "public/fpdf_edit.h"
#include "fpdfsdk/fsdk_define.h"
#include "core/fpdfapi/parser/cpdf_document.h"
#include "core/fpdfapi/parser/cpdf_reference.h"
#include "core/fpdfapi/page/cpdf_page.h"
#include "core/fpdfapi/page/cpdf_pageobject.h"
#include "core/fpdfapi/page/cpdf_pathobject.h"
#include "core/fpdfapi/render/cpdf_renderoptions.h"
#include "core/fpdfapi/cpdf_pagerendercontext.h"
#include "core/fxge/ifx_renderdevicedriver.h"


class OdGeomCacheObjectGrabber : public OdPdfImportBaseObjectGrabber
{
  class BaseCurve
  {
  public:
    BaseCurve() {}
    ~BaseCurve() {}
    void addSeg(OdGePoint2dArray points, OdPdfImportPtType type)
    {
      m_PointStorage.emplace_back(std::make_pair(points, type));
      if (type != OdPdfImportPtType::OdLineTo)
      {
        m_isPolyline = false;
      }
    }
    void setClosed(bool state)
    {
      m_isClosed = state;
    }
    bool getClosed() const
    {
      return m_isClosed;
    }
    bool isPolyline() const
    {
      return m_isPolyline;
    }

    const std::vector<std::pair<OdGePoint2dArray, OdPdfImportPtType>>& getStorage() const
    {
      return m_PointStorage;
    }

    void clear()
    {
      m_PointStorage.clear();
      m_isClosed = false;
    }

  protected:
    std::vector<std::pair<OdGePoint2dArray, OdPdfImportPtType>> m_PointStorage;
    bool                                                        m_isClosed{ false };
    bool                                                        m_isPolyline{ true };

  };
public:
  OdGeomCacheObjectGrabber(int width, int height, double scale, const PdfiumWrapperImplPtr& wrp)
    : m_pWrapper(wrp)
    , m_Width(width)
    , m_Height(height)
    , m_Scale(scale)
  {}
  virtual ~OdGeomCacheObjectGrabber(){}

  virtual bool needToAddGrathics(bool* ret)
  {
    return true;
  }

  virtual void addPath(const OdPdfImportPathData* path_data, const unsigned long path_point_count,
    const OdPdfImportColor* fill_color, const OdPdfImportColor* stroke_color, const float* line_width, const bool is_object_visible,
    const wchar_t* cur_layer_name, const LineTypeInfo* line_type, const char* hyperlink = nullptr)
  {
    if (nullptr == path_data || 0 == path_point_count)
      return;

    try
    {

      std::vector<BaseCurve> paths;
      paths.resize(1);
      OdGePoint2dArray pt_cache;

      for (size_t i = 0; i < path_point_count; i++)
      {

        OdGePoint2d point(path_data[i].m_Point.x * 72., path_data[i].m_Point.y * 72.);


        OdGeMatrix2d matrix;
        matrix.setToIdentity();
        matrix *= OdGeMatrix2d::scaling(m_Scale);
        //      matrix *= OdGeMatrix2d::rotation(m_pProperties->get_Rotation());
        //      matrix.setTranslation(OdGeVector2d(m_pProperties->get_InsertionPointX(), m_pProperties->get_InsertionPointY()));
        point.transformBy(matrix);

        OdPdfImportPtType point_type = path_data[i].m_Type;
        if (point_type == OdPdfImportPtType::OdMoveTo)
        {
          if (pt_cache.size())
          {
            paths.back().addSeg(pt_cache, path_data[i - 1].m_Type);
            if (path_data[i - 1].m_CloseFigure || paths.back().getStorage().front().first.first().isEqualTo(pt_cache.last()))
            {
                paths.back().setClosed(path_data[i - 1].m_CloseFigure);
                paths.emplace_back(BaseCurve());
            }
          }
          pt_cache.clear();
          pt_cache.append(point);
        }
        else if (point_type == OdPdfImportPtType::OdLineTo)
        {

          if (OdPdfImportPtType::OdBezierTo == path_data[i - 1].m_Type)
          {
            paths.back().addSeg(pt_cache, path_data[i - 1].m_Type);
            if (path_data[i - 1].m_CloseFigure || paths.back().getStorage().front().first.first().isEqualTo(pt_cache.last()))
            {
                paths.back().setClosed(path_data[i - 1].m_CloseFigure);
                paths.emplace_back(BaseCurve());

            }

            OdGePoint2d ptTmp = pt_cache.last();
            pt_cache.clear();
            pt_cache.append(ptTmp);
          }
          pt_cache.append(point);
        }
        else if (point_type == OdPdfImportPtType::OdBezierTo)
        {
          if (OdPdfImportPtType::OdLineTo == path_data[i - 1].m_Type)
          {
            paths.back().addSeg(pt_cache, path_data[i - 1].m_Type);
            if (path_data[i - 1].m_CloseFigure || paths.back().getStorage().front().first.first().isEqualTo(pt_cache.last()))
            {
                paths.back().setClosed(path_data[i - 1].m_CloseFigure);
                paths.emplace_back(BaseCurve());
            }
            OdGePoint2d ptTmp = pt_cache.last();
            pt_cache.clear();
            pt_cache.append(ptTmp);
          }
          pt_cache.append(point);
        }
      }
      if (OdPdfImportPtType::OdMoveTo != path_data[path_point_count - 1].m_Type)
      {
        paths.back().addSeg(pt_cache, path_data[path_point_count - 1].m_Type);
        if (path_data[path_point_count - 1].m_CloseFigure || paths.back().getStorage().front().first.first().isEqualTo(pt_cache.last()))
        {
          paths.back().setClosed(path_data[path_point_count - 1].m_CloseFigure);
        }
      }
      savePath(paths);

    }
    catch (...)
    {
    }
  }

  void savePath(const std::vector<BaseCurve>& paths)
  {
    for (auto& fig : paths)
    {
      for (auto& seg : fig.getStorage())
      {
        if (OdPdfImportPtType::OdLineTo == seg.second)
          m_pWrapper->savePolyline(seg.first);
        else
          m_pWrapper->saveSpline(seg.first);
      }
    }
  }

  virtual float getWidth()
  {
    return m_Width;
  }
  virtual float getHeight()
  {
    return m_Height;
  }

protected:
  const PdfiumWrapperImplPtr m_pWrapper;
  float                      m_Width;
  float                      m_Height;
  double                     m_Scale;

};


//class geomCacheDriver : public IFX_RenderDeviceDriver
//{
//  class BaseCurve
//  {
//  public:
//    BaseCurve() {}
//    ~BaseCurve() {}
//    void addSeg(OdGePoint2dArray points, FXPT_TYPE type)
//    {
//      m_PointStorage.emplace_back(std::make_pair(points, type));
//      if (type != FXPT_TYPE::LineTo)
//      {
//        m_isPolyline = false;
//      }
//    }
//    void setClosed(bool state)
//    {
//      m_isClosed = state;
//    }
//    bool getClosed() const
//    {
//      return m_isClosed;
//    }
//    bool isPolyline() const
//    {
//      return m_isPolyline;
//    }
//
//    const std::vector<std::pair<OdGePoint2dArray, FXPT_TYPE>>& getStorage() const
//    {
//      return m_PointStorage;
//    }
//  protected:
//    std::vector<std::pair<OdGePoint2dArray, FXPT_TYPE>> m_PointStorage;
//    bool                                                m_isClosed{ false };
//    bool                                                m_isPolyline{ true };
//
//  };
//public:
//  explicit geomCacheDriver(int width, int height, double scale, const PdfiumWrapperImplPtr& wrp)
//    : m_pWrapper(wrp)
//    , m_Width(width)
//    , m_Height(height)
//    , m_Scale(scale)
//  {};
//  ~geomCacheDriver() override
//  {
//    EndRendering();
//  }
//protected:
//  const PdfiumWrapperImplPtr m_pWrapper;
//  int                        m_Width;
//  int                        m_Height;
//  double                     m_Scale;
//
//  void SaveState() override {};
//  void RestoreState(bool bKeepSaved) override {};
//  bool SetClip_PathFill(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, int fill_mode) override { return false; };
//  bool SetDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, const FX_RECT* pSrcRect, int left, int top, int blend_type) override { return false; };
//  bool StretchDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, int dest_left, int dest_top, int dest_width,
//    int dest_height, const FX_RECT* pClipRect, uint32_t flags, int blend_type) override { return false; };
//  bool StartDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, int bitmap_alpha, uint32_t color, const CFX_Matrix* pMatrix, uint32_t render_flags,
//    std::unique_ptr<CFX_ImageRenderer>* handle, int blend_type) override { return false; };
//  bool DrawDeviceText(int nChars, const FXTEXT_CHARPOS* pCharPos, CFX_Font* pFont, const CFX_Matrix* pObject2Device,
//    float font_size, uint32_t color)
//  {
//    return true; //if false is returned, some texts will be handled as geometry (snap geometry will be cached for them) 
//  }
//  bool GetClipBox(FX_RECT* pRect)
//  {
//    pRect->left = 0;
//    pRect->right = m_Width;
//    pRect->top = 0;
//    pRect->bottom = m_Height;
//    return true;
//  }
//  int GetDeviceCaps(int caps_id) const {
//    switch (caps_id)
//    {
//    case FXDC_DEVICE_CLASS:
//      return FXDC_DISPLAY;
//    case FXDC_PIXEL_WIDTH:
//      return m_Width;
//    case FXDC_PIXEL_HEIGHT:
//      return m_Height;
//    case FXDC_BITS_PIXEL:
//      return 72.;
//    case FXDC_RENDER_CAPS:
//      return FXRC_GET_BITS | FXRC_ALPHA_PATH | FXRC_ALPHA_IMAGE |
//        FXRC_BLEND_MODE | FXRC_SOFT_CLIP;
//    case FXDC_HORZ_SIZE:
//      return m_Width;
//    case FXDC_VERT_SIZE:
//      return m_Height;
//    }
//    return 0;
//  }
//  bool DrawPath(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, const CFX_GraphStateData* pGraphState,
//    uint32_t fill_color, uint32_t stroke_color, int fill_mode, int blend_type) override
//  {
//    const std::vector<FX_PATHPOINT>& pPoints = pPathData->GetPoints();
//
//    if (0 == pPoints.size())
//      return true;
//
//    OdGePoint2dArray pt_cache;
//
//    std::vector<BaseCurve> paths;
//    paths.resize(1);
//
//    for (size_t i = 0; i < pPoints.size(); i++)
//    {
//      CFX_PointF pos = pPoints[i].m_Point;
//      if (pObject2Device)
//      {
//        pos = pObject2Device->Transform(pos);
//      }
//      pos.y += abs(m_Height);
//
//      OdGePoint2d point(pos.x /*/ DPI*/, pos.y /*/ DPI*/);
//
//      OdGeMatrix2d matrix;
//      matrix.setToIdentity();
//      matrix *= OdGeMatrix2d::scaling(m_Scale);
////      matrix *= OdGeMatrix2d::rotation(m_pProperties->get_Rotation());
////      matrix.setTranslation(OdGeVector2d(m_pProperties->get_InsertionPointX(), m_pProperties->get_InsertionPointY()));
//
//      point.transformBy(matrix);
//
//      FXPT_TYPE point_type = pPoints[i].m_Type;
//      if (point_type == FXPT_TYPE::MoveTo)
//      {
//        if (pt_cache.size())
//        {
//          paths.back().addSeg(pt_cache, pPoints[i - 1].m_Type);
//          if (pPoints[i - 1].m_CloseFigure)
//          {
//            paths.back().setClosed(pPoints[i - 1].m_CloseFigure);
//            paths.emplace_back(BaseCurve());
//          }
//        }
//        pt_cache.clear();
//        pt_cache.append(point);
//      }
//      else if (point_type == FXPT_TYPE::LineTo)
//      {
//        if (FXPT_TYPE::BezierTo == pPoints[i - 1].m_Type)
//        {
//          paths.back().addSeg(pt_cache, pPoints[i - 1].m_Type);
//          if (pPoints[i - 1].m_CloseFigure)
//          {
//            paths.back().setClosed(pPoints[i - 1].m_CloseFigure);
//            paths.emplace_back(BaseCurve());
//          }
//          OdGePoint2d ptTmp = pt_cache.last();
//          pt_cache.clear();
//          pt_cache.append(ptTmp);
//        }
//        pt_cache.append(point);
//      }
//      else if (point_type == FXPT_TYPE::BezierTo)
//      {
//        if (FXPT_TYPE::LineTo == pPoints[i - 1].m_Type)
//        {
//          paths.back().addSeg(pt_cache, pPoints[i - 1].m_Type);
//          if (pPoints[i - 1].m_CloseFigure)
//          {
//            paths.back().setClosed(pPoints[i - 1].m_CloseFigure);
//            paths.emplace_back(BaseCurve());
//          }
//          OdGePoint2d ptTmp = pt_cache.last();
//          pt_cache.clear();
//          pt_cache.append(ptTmp);
//        }
//        pt_cache.append(point);
//      }
//    }
//    if (FXPT_TYPE::MoveTo != pPoints.back().m_Type)
//    {
//      paths.back().addSeg(pt_cache, pPoints.back().m_Type);
//      if (pPoints.back().m_CloseFigure)
//        paths.back().setClosed(pPoints.back().m_CloseFigure);
//    }
//    savePath(paths);
//    return true;
//  };
//
//  void savePath(const std::vector<BaseCurve>& paths)
//  {
//    for (auto& fig : paths)
//    {
//      for (auto& seg : fig.getStorage())
//      {
//        if (FXPT_TYPE::LineTo == seg.second)
//          m_pWrapper->savePolyline(seg.first);
//        else
//          m_pWrapper->saveSpline(seg.first);
//      }
//    }
//  };
//};

void PdfiumWrapperImpl::createGeometryCache(int nPage, const OdArray<OdUnderlayLayer> &Layers) const
{
  if (!m_pDocument)
    return;

  std::map<OdUInt32, FPDF_PAGE>::iterator iter = m_PagesMap.find(nPage);
  if (iter == m_PagesMap.end())
    m_PagesMap[nPage] = FPDF_LoadPage(m_pDocument, nPage);

  m_geometryCache.clear(nPage);
  setupLayers(Layers);
  double scale = getMeasureDictInfo(nPage);

  double page_width = FPDF_GetPageWidth(m_PagesMap[nPage]);
  double page_height = FPDF_GetPageHeight(m_PagesMap[nPage]);

  auto pGraber = std::make_shared<OdGeomCacheObjectGrabber>(page_width, -page_height, scale, this);

  FPDF_ImportPage(m_PagesMap[nPage], pGraber.get(), 0);

  //CPDF_PageRenderContext* pContext = new CPDF_PageRenderContext;
  //FPDF_SetPageRenderContext(m_Pages[nPage], pContext);

  //CFX_RenderDevice* pDevice = new CFX_RenderDevice;

  //pDevice->SetDeviceDriver(pdfium::MakeUnique<geomCacheDriver>(page_width, -page_height, scale, this));
  //pContext->m_pDevice.reset(pDevice);

  //FPDF_RenderPage_Retail(pContext, m_Pages[nPage], 0, 0, page_width, -page_height, 0, 0, true, nullptr);

  //FPDF_SetPageRenderContext(m_Pages[nPage], nullptr);
}

void PdfiumWrapperImpl::savePolyline(const OdGePoint2dArray& pt_cache) const
{
  OdGeExtents2d objExt;
  OdInt32 count = pt_cache.size();
  OdGePoint3dArray points;
  points.resize(count);

  for (int i = 0; i < count; ++i)
  {
    points[i].set(pt_cache[i].x, pt_cache[i].y, 0.);
    objExt.addPoint(pt_cache[i]);
  }

  m_geometryCache.m_polylines.push_back(GeometryCache::Polyline(points, objExt));
}

void PdfiumWrapperImpl::saveSpline(const OdGePoint2dArray& pt_cache) const
{
  OdGePoint3dArray ctrlPts;
  ctrlPts.resize(pt_cache.size());
  OdGeExtents2d objExt;
  OdGeKnotVector knotsVec;
  OdGeDoubleArray knots;
  OdGeDoubleArray wts;

  for (OdUInt32 i = 0; i < pt_cache.size(); i++)
  {
    objExt.addPoint(pt_cache[i]);
    ctrlPts[i] = OdGePoint3d(pt_cache[i].x, pt_cache[i].y, 0);
  }

  /* weight values */
  //int nKnots = (noSeg - 1) * 3 + 1;
  wts.resize(pt_cache.size());
  for (OdUInt32 i = 0; i < pt_cache.size(); i++)
    wts[i] = 1.;

  /* Knot points */
  knots.resize(pt_cache.size() + 4);
  double kVal = 1.;
  int j = 0;
  for (j = 0; j < 4; j++)
    knots[j] = 0.;
  for (OdUInt32 i = 1; i < (pt_cache.size() - 1) / 3; i++)
  {
    knots[j++] = kVal;
    knots[j++] = kVal;
    knots[j++] = kVal;
    kVal += 1.;
  }
  for (int i = 0; i < 4; i++)
    knots[j++] = kVal;

  knotsVec = knots;
  knotsVec.setTolerance(0.);

  OdGeNurbCurve3d spline;
  spline.set(3, knotsVec, ctrlPts, wts, false);
  m_geometryCache.m_splines.push_back(std::pair<OdGeNurbCurve3d, OdGeExtents2d>(spline, objExt));
}
