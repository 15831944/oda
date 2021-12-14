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

#ifndef _DBPDFIMPORTOBJECTGRABBER_INCLUDED_
#define _DBPDFIMPORTOBJECTGRABBER_INCLUDED_

#include "TD_PackPush.h"
#include "DbRasterImage.h"
#include "DbBasePdfImportObjectGrabber.h"

#include <memory>
#include <map>
#include <vector>
#include <array>
#include "Ge/GePoint2dArray.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeClipBoundary2d.h"
#include "PdfImport.h"

#include "ShapeFitters.h"
#include "DbCurve.h"
#include "Ge/GeCurve2dPtrArray.h"
#include "SharedPtr.h"
#include "DbMText.h"
#include "DbLinetypeTableRecord.h"
#include "Ge/GeExtents2d.h"

class OdDbDatabase;

class MdFaceWrapper
{
public:
  MdFaceWrapper();
  MdFaceWrapper(void* object);
  MdFaceWrapper(const MdFaceWrapper& other);

  ~MdFaceWrapper();


  OdGe::PointContainment checkPointLocation(const OdGePoint3d& point, const OdGeTol& tol = OdGeContext::gTol) const;
  void* clone(bool iSaveIds = false) const;
  OdGeExtents3d getGeomExtents(const OdGeMatrix3d* coordSystem = NULL) const;
  void* getOdMdFaces();
  bool processFaces(void* context);
  bool processLoops(void* context);

  void* get() const { return m_pMdBody; }

  void* detach();

  operator void* () { return m_pMdBody; }
  operator const void* () const { return m_pMdBody; }
  bool isNull() const { return nullptr == m_pMdBody; }
  MdFaceWrapper& operator=(const MdFaceWrapper& other);

private:
  void* m_pMdBody;
  void* m_pMdFaces;
  int*  m_refCount;
};

class OdDbPdfImportObjectGrabber : public OdDbBasePdfImportObjectGrabber
{
  class BaseCurve
  {
  public:
    enum SegmentType { kLine, kBezier, kCircleArc, kCircle, kEllipseArc, kEllipse };

    class Segment
    {
    public:
      Segment(const OdGePoint2dArray& points, const SegmentType type, const OdPdfImportPtType points_type)
        :m_PointStorage(points)
        ,m_SegmentType(type)
        ,m_PointsType(points_type)
      {}

      ~Segment(){}

      SegmentType getType() const
      {
        return m_SegmentType;
      }

      void setType(SegmentType type)
      {
        m_SegmentType = type;
      }

      OdPdfImportPtType getPointsType() const
      {
        return m_PointsType;
      }

      OdGeCurve3dPtr getCurve() const
      {
        return m_OptimizedCurve;
      }

      void setCurve(OdGeCurve3dPtr& curve)
      {
        m_OptimizedCurve = curve;
      }

      const OdGePoint2dArray& getPointStorage() const
      {
        return m_PointStorage;
      }

      void setPointStorage(OdGePoint2dArray& points)
      {
        m_PointStorage = points;
      }

      bool isClosed() const
      {
        return (0 != m_PointStorage.size()) && (m_PointStorage.first() == m_PointStorage.last());
      }

      OdGeCurve3dPtrArray m_Curves;

    protected:
      SegmentType         m_SegmentType{ kLine };
      OdGeCurve3dPtr      m_OptimizedCurve;
      OdGePoint2dArray    m_PointStorage;
      OdPdfImportPtType   m_PointsType;
    };

    BaseCurve() {}
    ~BaseCurve() {}


    void addSeg(const OdGePoint2dArray& points, const OdPdfImportPtType type)
    {
      SegmentType seg_type(kLine);
      if (OdPdfImportPtType::OdBezierTo == type)
        seg_type = kBezier;
      m_Segments.emplace_back(Segment(points, seg_type, type));
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
      for (auto& seg : m_Segments)
      {
        if (OdPdfImportPtType::OdLineTo != seg.getPointsType())
          return false;
      }
      return true;
    }

    const std::vector<BaseCurve::Segment>& getSegments() const
    {
      return m_Segments;
    }

    std::vector<BaseCurve::Segment>& getSegments()
    {
      return m_Segments;
    }

    void clear()
    {
      m_Segments.clear();
      m_isClosed = false;
    }

  protected:
    std::vector<BaseCurve::Segment> m_Segments;
    bool                            m_isClosed{ false };

  };
public:
  OdDbPdfImportObjectGrabber(PdfImportInfoHolderPtr& info, const std::map<std::wstring, bool>& layers_info,
        const OdGePoint2dArray& clipBoundary, const bool is_clip_boundary_inverted, const OdString& images_path, PreferableFontCallback callback);
  virtual ~OdDbPdfImportObjectGrabber();

  static OdString extractFileName(const OdString& file_path);

  virtual void addPath(const OdPdfImportPathData* path_data, const unsigned long path_point_count, const OdPdfImportColor* fill_color,
    const OdPdfImportColor* stroke_color, const float* line_width, const bool is_object_visible, const wchar_t* cur_layer_name, 
    const LineTypeInfo* line_type, const char* hyperlink = nullptr);
  virtual void addCosmeticLine(const OdPdfImportPoint* point1, const OdPdfImportPoint* point2, const OdPdfImportColor* color,
    const bool is_object_visible, const wchar_t* cur_layer_name, const char* hyperlink = nullptr);
  virtual void addPixel(const OdPdfImportPoint* point, const OdPdfImportColor* color, const bool is_object_visible,
    const wchar_t* cur_layer_name, const char* hyperlink = nullptr);

  virtual bool isImageClippingEnabled();
  virtual void addImage(const BmiHeaderInfo* bmi, const unsigned char* imagedata, const OdPdfImportRect* rect, 
    const OdPdfImportMatrix* object_matrix, const bool is_object_visible, const wchar_t* cur_layer_name, const char* hyperlink = nullptr);
  virtual bool getClipBoundary(std::vector<OdPdfImportPathData>& path_data);

  virtual void addText(const wchar_t* text, const PdfImportFontParams* const font_params, const OdPdfImportColor* color, 
    const OdPdfImportMatrix* object_matrix, const bool is_object_visible, const wchar_t* layer_name, const char* hyperlink = nullptr);
  virtual bool combineTextObjects() const;
  virtual void getSpaceFactors(double& char_space_factor, double& space_factor) const;
  virtual bool extractEmbeddedFonts() const;

  virtual void addShading(const BmiHeaderInfo* bmi, const unsigned char* imagedata, const OdPdfImportRect* rect,
    const OdPdfImportMatrix* object_matrix, const bool is_object_visible, const wchar_t* cur_layer_name, const char* hyperlink = nullptr);

  virtual void addClipPath(const OdPdfImportPathData* path_data, const unsigned long path_point_count);

  virtual void SaveClipState();
  virtual void RestoreClipState(bool bKeepSaved);
  virtual bool isClippingEnabled();

  virtual ImportHyperlinkMethod getImportHyperlinkMethod();
  virtual void addHyperlinkAsTransparentRegion(const OdPdfImportPathData* path_data, const unsigned long path_point_count, const char* hyperlink);

  virtual bool getPreferableFont(const char* font_name, const char* font_family, const bool is_found, char* preferable_font_name, unsigned int& font_name_size);

private:
  void savePath(const std::vector<BaseCurve>& paths, const ODCOLORREF cr_color, const float* line_width, const bool is_object_visible, 
    const wchar_t* cur_layer_name, const OdUInt8 alpha, const LineTypeInfo* line_type, const char* hyperlink = nullptr);
  void saveSolid(const std::vector<BaseCurve>& paths, const ODCOLORREF cr_color, const bool is_object_visible, const wchar_t* cur_layer_name,
    const OdUInt8 alpha, const char* hyperlink = nullptr);
  OdDbObjectId getLayer(const OdChar* name, const bool is_object_visible, const wchar_t* cur_layer_name);
  bool createRasterImage(const BmiHeaderInfo* bmi_info, const unsigned char* imagedata, const OdPdfImportRect* rect, 
    const OdPdfImportMatrix* object_matrix, const char* hyperlink, const OdString& layer_name, const bool is_object_visible, const wchar_t* cur_layer_name, OdDbRasterImagePtr& pImage);
  bool pointInPolygon(const OdGePoint2d& point);
  void OptimizePath(std::vector<BaseCurve>& paths);
  bool fitSerment(const OdGePoint2dArray& samples, const OdGeCurve2dPtr& in_curve, const double& tol, const bool is_closed,
    OdGeCurve3dPtr& out_curve, BaseCurve::SegmentType& seg_type);

  void fillPathData(const OdPdfImportPathData* path_data, const unsigned long path_point_count, std::vector<BaseCurve>& paths);
  void fillLoops(const std::vector<BaseCurve>& paths, OdArray< OdArray<OdGeCurve2d*> >& loops, bool fill_solid = false);
  void cutPath(std::vector<BaseCurve>& clipped_paths, const std::function<bool(const OdGePoint2d&)>& check_point_func);

  void addHyperlink(OdDbEntityPtr ent, OdString hyperlink);

  OdCmColor toCmColor(const ODCOLORREF color);

  OdDbBlockTableRecordPtr                                                      m_pBTR;
  OdString                                                                     m_PdfFileName;
  OdString                                                                     m_ImagesPath;
  OdGeMatrix3d                                                                 m_TransforMatrix;
  const std::map<std::wstring, bool>&                                          m_LayersInfo;
  OdGePoint2dArray                                                             m_ClipBoundary;
  bool                                                                         m_isClipBoundaryInverted;
  OdGeExtents2d                                                                m_ClipBoundaryExtents;
  CircleFitter                                                                 m_CircleFitter;
  EllipseFitter                                                                m_EllipseFitter;
  std::map<OdDbObjectId, std::map<ODCOLORREF, std::vector<OdDbEntityPtr>>>     m_LayerItems;
  MdFaceWrapper                                                                m_ClipPath;
  std::vector<MdFaceWrapper>                                                   m_ClipStack;
  MdFaceWrapper                                                                m_ClipBoundaryBody;
  OdGePoint2dArray                                                             m_PageBorder;
  OdString                                                                     m_FormatString;
  OdString                                                                     m_ItalicFormatString;
  OdString                                                                     m_ContentStr;
  OdDbMTextPtr                                                                 m_pLastMText;
  std::vector<OdDbLinetypeTableRecordPtr>                                      m_LineStyles;
  PreferableFontCallback                                                       m_PreferableFontCallback;
};

#include "TD_PackPop.h"

#endif // _DBPDFIMPORTOBJECTGRABBER_INCLUDED_
