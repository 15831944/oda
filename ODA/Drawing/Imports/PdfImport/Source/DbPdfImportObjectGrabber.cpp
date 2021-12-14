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
#include "OdaCommon.h"
#include "DbPdfImportObjectGrabber.h"

#include <algorithm>
#include <memory>
#include <vector>
#include <functional>

#include "DbDatabase.h"
#include "DbBlockTableRecord.h"
#include "DbHostAppServices.h"
#include "Db2LineAngularDimension.h"
#include "Db2dPolyline.h"
#include "Db2dVertex.h"
#include "Db3PointAngularDimension.h"
#include "Db3dPolyline.h"
#include "Db3dPolylineVertex.h"
#include "Db3dSolid.h"
#include "DbAlignedDimension.h"
#include "DbArc.h"
#include "DbArcAlignedText.h"
#include "DbArcDimension.h"
#include "DbAttribute.h"
#include "DbAttributeDefinition.h"
#include "DbBlockReference.h"
#include "DbBlockTable.h"
#include "DbBody.h"
#include "DbCircle.h"
#include "DbDiametricDimension.h"
#include "DbDimAssoc.h"
#include "DbDimStyleTable.h"
#include "DbEllipse.h"
#include "DbFace.h"
#include "DbFaceRecord.h"
#include "DbFcf.h"
#include "DbField.h"
#include "DbGroup.h"
#include "DbHyperlink.h"
#include "DbLayerTable.h"
#include "DbLayout.h"
#include "DbLeader.h"
#include "DbMLeader.h"
#include "DbLine.h"
#include "DbLinetypeTable.h"
#include "DbMaterial.h"
#include "DbMInsertBlock.h"
#include "DbMline.h"
#include "DbMlineStyle.h"
#include "DbOrdinateDimension.h"
#include "DbPoint.h"
#include "DbPolyFaceMesh.h"
#include "DbPolyFaceMeshVertex.h"
#include "DbPolygonMesh.h"
#include "DbPolygonMeshVertex.h"
#include "DbPolyline.h"
#include "DbRadialDimension.h"
#include "DbRadialDimensionLarge.h"
#include "DbRasterImageDef.h"
#include "DbRasterVariables.h"
#include "DbRay.h"
#include "DbRegion.h"
#include "DbRotatedDimension.h"
#include "DbShape.h"
#include "DbSolid.h"
#include "DbSortentsTable.h"
#include "DbSpline.h"
#include "DbTable.h"
#include "DbMText.h"
#include "DbText.h"
#include "DbTextStyleTable.h"
#include "DbTrace.h"
#include "DbViewport.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbWipeout.h"
#include "DbXline.h"
#include "DbXrecord.h"
#include "RText.h"
#include "DbOle2Frame.h"
#include "DbHelix.h"
#include "DbUnderlayDefinition.h"
#include "DbUnderlayReference.h"
#include "HatchPatternManager.h"
#include "DbLayerTableRecord.h"
#include "OdPath.h"

#include "Ge/GeCircArc2d.h"
#include "Ge/GeEllipArc2d.h"
#include "Ge/GeSplineEnt2d.h"
#include "Ge/GeScale3d.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeKnotVector.h"
#include "Ge/GeNurbCurve2d.h"
#include "Gi/GiMaterial.h"
#include "Gi/GiRasterWrappers.h"
#include "Ge/GeExtents2d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GePolyline2d.h"
#include "Ge/GeLineSeg2d.h"

#include "MemoryStream.h"
#include "OdPlatformStreamer.h"
#include "RxRasterServices.h"
#include "Gs/LineWtIndex.h"
#include "OdRound.h"
#include <math.h>
#include <type_traits>
#include <utility>
#include "Ge/GeCircArc3d.h"
#include "Ge/GePolyline3d.h"
#include "Ge/GeEllipArc3d.h"
#include "SharedPtr.h"
#include "Ge/GeLine2d.h"
#include "Ge/GeCompositeCurve3d.h"
#include "Ge/GeLineSeg3d.h"
#include "DbLinetypeTableRecord.h"

//////////////////////////////////////////////////////////////////////////
/// INTERSECTOR WRAPPER

void* createCurveSurfaceIntersector();
void destroyCurveSurfaceIntersector(void* intersector);
void intersectorSetSurface(void* intersector, const OdGeSurface* iSurface, const OdGeInterval& iRangeU, const OdGeInterval& iRangeV, const void* iRegion = NULL);
void intersectorSetCurve(void* intersector, const OdGeCurve3d* iCurve);
bool intersectorRun(void* intersector);
void intersectorProcessIntersections(void* intersector, void (*callback_function)(OdGeCurve2d* curve, void* context), void* context);

inline void intersector_callback_func(OdGeCurve2d* curve, void* context)
{
  auto process_func = (std::function<void(OdGeCurve2d* curve)>*)context;
  (*process_func)(curve);
}

class GeIntersectorWrapper
{
public:
  GeIntersectorWrapper()
  {
    inersector = createCurveSurfaceIntersector();
  }

  ~GeIntersectorWrapper()
  {
    destroyCurveSurfaceIntersector(inersector);
    inersector = nullptr;
  }

  inline bool run()
  {
    return intersectorRun(inersector);
  }

  inline void setSurface(const OdGeSurface* iSurface, const OdGeInterval& iRangeU, const OdGeInterval& iRangeV, const void* iRegion = NULL)
  {
    intersectorSetSurface(inersector, iSurface, iRangeU, iRangeV, iRegion);
  }

  inline void setCurve(const OdGeCurve3d* iCurve)
  {
    intersectorSetCurve(inersector, iCurve);
  }
   
  inline void processIntersections(void* context)
  {
    intersectorProcessIntersections(inersector, intersector_callback_func, context);
  }

private:
  void* inersector;
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// BOOLEAN WRAPPER
void* createPlanarOdMdBodyFromCurves(OdArray< OdArray<OdGeCurve2d*> >& loops, const OdGeTol& tol = OdGeContext::gTol);
void  destroyMdBody(void* body);
OdGe::PointContainment odMdBodyCheckPointLocation(void* body, const OdGePoint3d& point, const OdGeTol& tol = OdGeContext::gTol);
OdGeExtents3d odMdBodyGetGeomExtents(void* body, const OdGeMatrix3d* coordSystem);
void* cloneOdMdBody(void* body, bool iSaveIds = false);
bool booleanOperationForMdBody(OdGe::BooleanType iType, void* body1, bool iOwn1, void* body2, bool iOwn2, void *& result);
bool odMdBodyProcessFaces(void* body, void* faces, void (*callback_function)(const OdGeSurface* iSurface, const OdGeInterval& iRangeU,
  const OdGeInterval& iRangeV, const void* iRegion, void* context), void* context);
bool odMdBodyProcessLoops(void* body, void (*callback_function)(OdArray<OdGeCurve2d*>& edgePtrs, const bool is_line_segs, unsigned long loops_count, void* context), void* context);
void* getOdMdFacesFromMdBody(void* body);
void destroyOdMdFaces(void* faces);

inline void process_faces_callback_func(const OdGeSurface* iSurface, const OdGeInterval& iRangeU,
  const OdGeInterval& iRangeV, const void* iRegion, void* context)
{
  auto process_func = (std::function<void(const OdGeSurface* iSurface, const OdGeInterval& iRangeU,
    const OdGeInterval& iRangeV, const void* iRegion)>*)context;
  (*process_func)(iSurface, iRangeU, iRangeV, iRegion);
}

inline void process_loops_callback_func(OdArray<OdGeCurve2d*>& edgePtrs, const bool is_line_segs, unsigned long loops_count, void* context)
{
  auto process_func = (std::function<void(OdArray<OdGeCurve2d*>& edgePtrs, const bool is_line_segs, unsigned long loops_count)>*)context;
  (*process_func)(edgePtrs, is_line_segs, loops_count);
}

MdFaceWrapper::MdFaceWrapper()
  :m_refCount(nullptr)
  ,m_pMdBody(nullptr)
  ,m_pMdFaces(NULL)
{

}

MdFaceWrapper::MdFaceWrapper(void* object) 
  :m_refCount(nullptr)
  ,m_pMdFaces(NULL)
{
  if ((m_pMdBody = object) != 0)
  {
    m_refCount = (int*)::odrxAlloc(sizeof(int*));
    *m_refCount = 1;
  }
}

MdFaceWrapper::MdFaceWrapper(const MdFaceWrapper& other)
{
  m_pMdBody = other.m_pMdBody;
  m_pMdFaces = other.m_pMdFaces;
  m_refCount = other.m_refCount;
  if (m_refCount)
  {
    ++* m_refCount;
  }
}

MdFaceWrapper::~MdFaceWrapper()
{
  if (m_refCount && !-- * m_refCount)
  {
    destroyMdBody(m_pMdBody);
    destroyOdMdFaces(m_pMdFaces);
    odrxFree(m_refCount);
  }
}

OdGe::PointContainment MdFaceWrapper::checkPointLocation(const OdGePoint3d& point, const OdGeTol& tol /*= OdGeContext::gTol*/) const
{
  return odMdBodyCheckPointLocation(m_pMdBody, point, tol);
}

void* MdFaceWrapper::clone(bool iSaveIds /*= false*/) const
{
  return cloneOdMdBody(m_pMdBody, iSaveIds);
}

OdGeExtents3d MdFaceWrapper::getGeomExtents(const OdGeMatrix3d* coordSystem /*= NULL*/) const
{
  return odMdBodyGetGeomExtents(m_pMdBody, coordSystem);
}

void* MdFaceWrapper::getOdMdFaces()
{
  if(NULL == m_pMdFaces)
    m_pMdFaces = getOdMdFacesFromMdBody(m_pMdBody);
  return m_pMdFaces;
}

bool MdFaceWrapper::processFaces(void* context)
{
  return odMdBodyProcessFaces(m_pMdBody, m_pMdFaces, process_faces_callback_func, context);
}

bool MdFaceWrapper::processLoops(void* context)
{
  return odMdBodyProcessLoops(m_pMdBody, process_loops_callback_func, context);
}

void* MdFaceWrapper::detach()
{
  if (!m_refCount)
    return nullptr;
  if (*m_refCount > 1)
  {
    ODA_FAIL();
  }
  odrxFree(m_refCount);
  m_refCount = nullptr;
  void* ret = m_pMdBody;
  destroyOdMdFaces(m_pMdFaces);
  m_pMdFaces = NULL;
  m_pMdBody = nullptr;
  return ret;
}

MdFaceWrapper& MdFaceWrapper::operator=(const MdFaceWrapper& other)
{
  if (m_pMdBody != other.m_pMdBody)
  {
    if (m_refCount && !-- * m_refCount)
    {
      odrxFree(m_refCount);
      destroyMdBody(m_pMdBody);
      destroyOdMdFaces(m_pMdFaces);
    }

    m_pMdBody = other.m_pMdBody;
    m_pMdFaces = other.m_pMdFaces;
    m_refCount = other.m_refCount;
    if (m_refCount)
    {
      ++* m_refCount;
    }
  }
  return *this;
}

class LoopsAutoDispose
{
public:
  LoopsAutoDispose(OdArray< OdArray<OdGeCurve2d*> >& loops)
    :m_Loops(loops)
  {

  }
  ~LoopsAutoDispose()
  {
    for (auto& loop : m_Loops)
    {
      for (auto& curve : loop)
      {
        delete curve;
      }
    }
  }
private:
  OdArray< OdArray<OdGeCurve2d*> >& m_Loops;
};
//////////////////////////////////////////////////////////////////////////

class PdfImportModule;

enum LayersUseType
{
  eUsePdfLayers = 0,
  eCreateObjectLayers = 1,
  eUseCurrentLayer = 2
};

static const OdChar forbidden_chars[] = { L'\\', L':',L';',L'=',L'?',L'<',L'>',L'/',L'*',L'"',L'|', L',', L'`' };

inline void ReplaceForbiddenChars(OdString& str)
{
  for (auto& ch : forbidden_chars)
    str.replace(ch, L'$');
}

OdDbPdfImportObjectGrabber::OdDbPdfImportObjectGrabber(PdfImportInfoHolderPtr& info, const std::map<std::wstring, bool>& layers_info, 
  const OdGePoint2dArray& clipBoundary, const bool is_clip_boundary_inverted, const OdString& images_path, PreferableFontCallback callback)
  :OdDbBasePdfImportObjectGrabber(info)
  , m_LayersInfo(layers_info)
  , m_ClipBoundary(clipBoundary)
  , m_isClipBoundaryInverted(is_clip_boundary_inverted)
  , m_ImagesPath(images_path)
  , m_PreferableFontCallback(callback)
{
    OdDbDatabasePtr pDb = OdDbDatabasePtr(m_Info->m_pProperties->get_Database());
    
    m_PdfFileName = extractFileName(m_Info->m_pProperties->get_PdfPath());

    OdString block_name(m_PdfFileName);
    ReplaceForbiddenChars(block_name);

    if (m_Info->m_pProperties->get_ImportAsBlock() && !block_name.isEmpty())
    {
        OdDbBlockTablePtr pTable = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  
        OdDbObjectId block_id = pTable->getAt(block_name);
  
        if (block_id.isNull())
        {
          OdDbBlockTableRecordPtr pRecord = OdDbBlockTableRecord::createObject();
          pRecord->setName(block_name);
          block_id = pTable->add(pRecord);
  
          OdDbBlockTableRecordPtr active_space = pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);
          OdDbBlockReferencePtr block_ref = OdDbBlockReference::createObject();
          block_ref->setBlockTableRecord(block_id);
          active_space->appendOdDbEntity(block_ref);
        }
        m_pBTR = block_id.safeOpenObject(OdDb::kForWrite);
    }
    else
    {
      m_pBTR = (OdDbBlockTableRecordPtr)
        (pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite));
    }
  
    m_TransforMatrix.setToIdentity();
    m_TransforMatrix *= OdGeMatrix3d::scaling(m_Info->m_pProperties->get_Scaling());
    m_TransforMatrix *= OdGeMatrix3d::rotation(m_Info->m_pProperties->get_Rotation(), OdGeVector3d::kZAxis);
    m_TransforMatrix.setTranslation(OdGeVector3d(m_Info->m_pProperties->get_InsertionPointX(), m_Info->m_pProperties->get_InsertionPointY(), 0.0));

    m_Info->m_ObjectCounter = 0;

    if (m_Info->m_pProperties->get_UseClipping() && 0 != m_ClipBoundary.size())
    {
      OdGeVector3d z_axis(OdGeVector3d::kZAxis);
      double elevation = 0.0;
      OdGeMatrix2d matrix(m_TransforMatrix.convertToLocal(z_axis, elevation));

      OdArray< OdArray<OdGeCurve2d*> > loops;
      loops.resize(1);
      loops[0].resize(m_ClipBoundary.size());
      for (OdGePoint2dArray::size_type pt = 0; pt < m_ClipBoundary.size() - 1; ++pt)
      {
        loops[0][pt] = new OdGeLineSeg2d(m_ClipBoundary[pt].transformBy(matrix), m_ClipBoundary[pt + 1].transformBy(matrix));
      }
      loops[0][m_ClipBoundary.size() - 1] = new OdGeLineSeg2d(m_ClipBoundary[m_ClipBoundary.size() - 1].transformBy(matrix), m_ClipBoundary[0].transformBy(matrix));
      try
      {
        m_ClipBoundaryBody = createPlanarOdMdBodyFromCurves(loops);
      }
      catch (...)
      {
      }
      //TODO
      if (!m_ClipBoundaryBody.isNull())
      {
        m_ClipPath = m_ClipBoundaryBody;
        SaveClipState();
      }
    }
    else
    {
      m_ClipBoundaryExtents.addPoints(m_ClipBoundary);
    }
}

OdDbPdfImportObjectGrabber::~OdDbPdfImportObjectGrabber()
{
  if (0 != m_LayerItems.size())
  {
    for (auto& layer_elem : m_LayerItems)
    {
      ODCOLORREF layer_col;
      std::vector<OdDbEntityPtr>::size_type max_count(0);
      for (auto& color_elem : layer_elem.second)
      {
        if (max_count < color_elem.second.size())
        {
          layer_col = color_elem.first;
          max_count = color_elem.second.size();
        }
      }
      OdDbLayerTableRecordPtr pLayer = layer_elem.first.openObject(OdDb::kForWrite);
      pLayer->setColor(toCmColor(layer_col));
      OdCmColor col;
      col.setColorIndex(OdCmEntityColor::kACIbyLayer);
      for (auto& obj_elem : layer_elem.second[layer_col])
      {
        obj_elem->setColor(col);
      }
    }

  }
}

OdString OdDbPdfImportObjectGrabber::extractFileName(const OdString& file_path)
{
  OdString ret;

  int i1 = file_path.reverseFind('\\');
  int i2 = file_path.reverseFind('/');
  int i = odmax(i1, i2);
  ret = file_path.mid(i + 1);
  i = ret.find(L'.');
  if (i != -1)
    ret = ret.left(i);
  return ret;
}

OdCmColor OdDbPdfImportObjectGrabber::toCmColor(ODCOLORREF color)
{
  OdCmColor col;

  if (m_Info->m_pProperties->get_UseRgbColor() || 0xFFFFFF == color)
  {
    col.setRGB(ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color));
  }
  else
  {
    OdInt16 idx = OdCmEntityColor::kACIWhite;
    if (0 != color)
    {
      idx = (OdInt16)OdCmEntityColor::lookUpACI(ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color));
    }
    col.setColorIndex(idx);
    if (0 != color && color != ODRGB(col.red(), col.green(), col.blue()))
      col.setRGB(ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color));
  }
  return col;
}

void fillKnots(const OdUInt32 size, OdGeKnotVector& knotsVec, OdGeDoubleArray& wts)
{
  OdGeDoubleArray knots;

  /* weight values */
  wts.resize(size, 1.);

  /* Knot points */
  knots.resize(size + 4, 0.);
  double kVal = 1.;
  int j = 0;
  for (j = 0; j < 4; j++)
    knots[j] = 0.;
  for (size_t i = 1; i < (size - 1) / 3; i++)
  {
    knots[j++] = kVal;
    knots[j++] = kVal;
    knots[j++] = kVal;
    kVal += 1.;
  }
  for (int i = 0; i < 4; i++)
    knots[j++] = kVal;
  for(;j < (int)knots.size();)
    knots[j++] = kVal;

  knotsVec = knots;
  knotsVec.setTolerance(0.);
}

bool OdDbPdfImportObjectGrabber::pointInPolygon(const OdGePoint2d& point)
{
  const OdUInt32 points_count = m_ClipBoundary.size();
  if (0 == points_count)
    return true;
  bool res = false;

  for (OdUInt32 i = 0, j = points_count - 1; i < points_count; j = i++)
  {
    if (((m_ClipBoundary[i].y >= point.y) != (m_ClipBoundary[j].y >= point.y)) &&
      (point.x <= (m_ClipBoundary[j].x - m_ClipBoundary[i].x) * (point.y - m_ClipBoundary[i].y) /
      (m_ClipBoundary[j].y - m_ClipBoundary[i].y) + m_ClipBoundary[i].x))
      res = !res;
  }
  return m_isClipBoundaryInverted ? !res : res;
}

double curvature(const OdGePoint2d* points, const double& t)
{
  std::function<OdGeVector2d(const OdGePoint2d* points, const double& t)> tangent = [](const OdGePoint2d* points, const double& t)
  {
    const double t_2_0 = t * t;
    const double t_0_2 = (1 - t) * (1 - t);

    const double _1__4t_1_0_3t_2_0 = 1 - 4 * t + 3 * t_2_0;
    const double _2t_1_0_3t_2_0 = 2 * t - 3 * t_2_0;

    return OdGeVector2d(-3 * points[0].x * t_0_2
      + 3 * points[1].x * _1__4t_1_0_3t_2_0
      + 3 * points[2].x * _2t_1_0_3t_2_0
      + 3 * points[3].x * t_2_0,
      -3 * points[0].y * t_0_2
      + 3 * points[1].y * _1__4t_1_0_3t_2_0
      + 3 * points[2].y * _2t_1_0_3t_2_0
      + 3 * points[3].y * t_2_0);
  };

  std::function<OdGeVector2d(const OdGePoint2d* points, const double& t)> d_tangent = [](const OdGePoint2d* points, const double& t)
  {
    return OdGeVector2d(6 * ((-points[0].x + 3 * points[1].x - 3 * points[2].x + points[3].x) * t + (points[0].x - 2 * points[1].x + points[2].x)),
      6 * ((-points[0].y + 3 * points[1].y - 3 * points[2].y + points[3].y) * t + (points[0].y - 2 * points[1].y + points[2].y)));
  };

  OdGeVector2d dpp = tangent(points, t);
  dpp.set(-dpp.y, dpp.x);
  OdGeVector2d ddp = d_tangent(points, t);
  /* normal vector len squared */
  double len = dpp.length();
  if (OdZero(len))
    return 0.;
  double curvature = dpp.dotProduct(ddp) / (len * len * len);
  return curvature;

}

bool OdDbPdfImportObjectGrabber::fitSerment(const OdGePoint2dArray& samples, const OdGeCurve2dPtr& in_curve, const double& tol, const bool is_closed,
  OdGeCurve3dPtr& out_curve, BaseCurve::SegmentType& seg_type)
{

  OdGePoint2dArray out_points;

  bool is_circle =  m_CircleFitter.fit(samples, out_points, false);


  double circle_std_deviation = m_CircleFitter.getStdDeviation();

  bool is_added = false;

  double arc_area(0.);

  if (is_circle && OdPositive(m_CircleFitter.getRadius(), 1e-12))
  {
    double radius = m_CircleFitter.getRadius();
    double coef = 100. / radius * circle_std_deviation;

    if (coef < tol)
    {
      OdGeCurve3dPtr circle = m_CircleFitter.getCurve();
      if (circle.isNull())
        return false;

      seg_type = BaseCurve::kCircle;
      if (!is_closed)
      {
        double start_angle = circle->paramOf(OdGePoint3d(out_points[0].x, out_points[0].y, 0.));
        double end_angle = circle->paramOf(OdGePoint3d(out_points[out_points.size() - 1].x, out_points[out_points.size() - 1].y, 0.));

        OdGeVector2d v1(out_points[out_points.size() / 2] - out_points[0]);
        OdGeVector2d v2(out_points[out_points.size() - 1] - out_points[out_points.size() / 2]);

        if (v1.x * v2.y < v1.y * v2.x)
        { // clockwise
          std::swap(start_angle, end_angle);
        }

        static_cast<OdGeCircArc3d*>(circle.get())->setAngles(start_angle, end_angle);
        seg_type = BaseCurve::kCircleArc;
      }

      double in_len(0.);
      if (in_curve.isNull())
      {
        OdGePolyline2d poly(samples);
        in_len = poly.length();
      }
      else
      {
        in_len = in_curve->length();
      }
      double arc_len = circle->length();
      double ratio = arc_len / in_len;

      if (OdEqual(ratio, 1.0, 1e-2))
      {
        out_curve = circle;
        is_added = true;
      }
    }
  }

  bool is_elllipse = m_EllipseFitter.fit(samples, out_points);

  if (is_elllipse && OdPositive(m_EllipseFitter.getMajorRadius(), 1e-12) && OdPositive(m_EllipseFitter.getMinorRadius(), 1e-12))
  {
    double ellipse_std_deviation = m_EllipseFitter.getStdDeviation();
    if (is_added && (circle_std_deviation < ellipse_std_deviation))
    {
      return true;
    }

    double coef = 100. / m_EllipseFitter.getMajorRadius() * ellipse_std_deviation; 

    if (coef < tol)
    {
      OdGeCurve3dPtr ellipse = m_EllipseFitter.getCurve();
      if (ellipse.isNull())
        return false;

      seg_type = BaseCurve::kEllipse;
      if (!is_closed)
      {
        double start_angle = ellipse->paramOf(OdGePoint3d(out_points[0].x, out_points[0].y, 0.));
        double end_angle = ellipse->paramOf(OdGePoint3d(out_points[out_points.size() - 1].x, out_points[out_points.size() - 1].y, 0.));

        OdGeVector2d v1(out_points[out_points.size() / 2] - out_points[0]);
        OdGeVector2d v2(out_points[out_points.size() - 1] - out_points[out_points.size() / 2]);

        if (v1.x * v2.y < v1.y * v2.x)
        { // clockwise
          std::swap(start_angle, end_angle);
        }

        static_cast<OdGeEllipArc3d*>(ellipse.get())->setAngles(start_angle, end_angle);
        seg_type = BaseCurve::kEllipseArc;
      }

      double in_len(0.);
      if (in_curve.isNull())
      {
        OdGePolyline2d poly(samples);
        in_len = poly.length();
      }
      else
      {
        in_len = in_curve->length();
      }
      double arc_len = ellipse->length();
      double ratio = arc_len / in_len;

      if (OdEqual(ratio, 1.0, 1e-2))
      {
        out_curve = ellipse;
        return true;
      }
    }
  }
  return is_added;
}

void OdDbPdfImportObjectGrabber::OptimizePath(std::vector<BaseCurve>& paths)
{
  for (auto& fig : paths)
  {
    for (auto& seg : fig.getSegments())
    {
      if (BaseCurve::kLine == seg.getType())
      {
        if(9 > seg.getPointStorage().size() || 500 < seg.getPointStorage().size())
          continue;

        bool need_exit = false;
        for (OdGePoint2dArray::size_type i = 0; i < seg.getPointStorage().size() - 3; ++i)
        {

          if (fabs((seg.getPointStorage()[i+2].x - seg.getPointStorage()[i].x) / 
            (seg.getPointStorage()[i+1].x - seg.getPointStorage()[i].x) - (seg.getPointStorage()[i+2].y - seg.getPointStorage()[i].y) /
            (seg.getPointStorage()[i+1].y - seg.getPointStorage()[i].y)) < 1e-10)
          {
            need_exit = true;
            break;
          }
        }
        if (need_exit)
          continue;

        OdGeCurve3dPtr curve;
        BaseCurve::SegmentType seg_type = seg.getType();
        if (fitSerment(seg.getPointStorage(), nullptr, 0.5, seg.getPointStorage().first().isEqualTo(seg.getPointStorage().last()), curve, seg_type))
        {
          seg.setCurve(curve);
          seg.setType(seg_type); 
        }
      }
      else
      {
        OdUInt16 bezier_count = (seg.getPointStorage().size() - 1) / 3;
        const OdUInt8 sample_points_count = 10;
        bool curvature_changed = false;
        bool is_positive = curvature(&seg.getPointStorage()[0], 0) > 0.;
        for (OdUInt16 bez = 0; bez < bezier_count; ++bez)
        {
          for (int i = 1; i < 11; ++i)
          {
            if (is_positive != curvature(&seg.getPointStorage()[bez * 3], (double)i / 10.) > 0.)
            {
              curvature_changed = true;
              break;
            }
          }
          if(curvature_changed)
            break;
        } 
        if(curvature_changed)
          continue;

        OdGePoint2dArray ctrlPts;
        std::copy(seg.getPointStorage().begin(), seg.getPointStorage().end(), std::back_inserter(ctrlPts));

        OdGeKnotVector knotsVec;
        OdGeDoubleArray wts;

        fillKnots(ctrlPts.size(), knotsVec, wts);

        OdGeCurve2dPtr nurbs(new OdGeNurbCurve2d(3, knotsVec, ctrlPts, wts));

        OdGePoint2dArray samples;
        nurbs->getSamplePoints(sample_points_count, samples);

        OdGeCurve3dPtr curve;
        BaseCurve::SegmentType seg_type = seg.getType();

        const bool is_closed = seg.getPointStorage().first().isEqualTo(seg.getPointStorage().last());
        if (fitSerment(samples, nurbs, 0.1, is_closed, curve, seg_type))
        {
          seg.setCurve(curve);
          seg.setType(seg_type);
          if (is_closed && BaseCurve::kCircle == seg_type && bezier_count >= 4 && 0 == bezier_count % 4)
          {
            OdGeLine2d line1(seg.getPointStorage()[0], seg.getPointStorage()[3 * bezier_count / 2]);
            OdGeLine2d line2(seg.getPointStorage()[3 * bezier_count / 4], seg.getPointStorage()[9 * bezier_count / 4]);
            OdGePoint2d center;
            line1.intersectWith(line2, center);
            double radius = ((center - seg.getPointStorage()[0]).length() + (center - seg.getPointStorage()[3 * bezier_count / 2]).length() +
              (center - seg.getPointStorage()[3 * bezier_count / 4]).length() + (center - seg.getPointStorage()[9 * bezier_count / 4]).length()) / 4.0;
            OdGeCircArc3d* arc = static_cast<OdGeCircArc3d*>(seg.getCurve().get());
            arc->setCenter(OdGePoint3d(center.x, center.y, 0.));
            arc->setRadius(radius);
          }
        }
      }
    }
  }
}

void OdDbPdfImportObjectGrabber::addPath(const OdPdfImportPathData* path_data, const unsigned long path_point_count, 
  const OdPdfImportColor* fill_color, const OdPdfImportColor* stroke_color, const float* line_width, const bool is_object_visible,
  const wchar_t* cur_layer_name, const LineTypeInfo* line_type, const char* hyperlink)
{
  ++m_Info->m_ObjectCounter;

  if (nullptr == path_data || nullptr == fill_color || nullptr == stroke_color || 0 == path_point_count)
  {
    ++m_Info->m_ErrorCounter;
    return;
  }

    try
    {
  
      OdGePoint2dArray pt_cache;
      bool fill = m_Info->m_pProperties->get_ImportSolidFills() && (0 != fill_color->alfa);
      bool stroke = (0 != stroke_color->alfa);
    
      if (!stroke && !fill)
      {
        return;
      }
  
      ODCOLORREF od_stroke_color = ODRGB(stroke_color->r, stroke_color->g, stroke_color->b);
      ODCOLORREF od_fill_color = ODRGB(fill_color->r, fill_color->g, fill_color->b);
  
      OdDbDatabasePtr pDb = OdDbDatabasePtr(m_Info->m_pProperties->get_Database());
  
      std::vector<BaseCurve> paths;

      fillPathData(path_data, path_point_count, paths);
      if (!m_Info->m_pProperties->get_UseClipping() && 0 != m_ClipBoundary.size())
      {
        cutPath(paths, std::bind(&OdDbPdfImportObjectGrabber::pointInPolygon, this, std::placeholders::_1));
      }

      if(0 == paths.size())
        --m_Info->m_ObjectCounter;

      if(m_Info->m_pProperties->get_UseGeometryOptimization())
        OptimizePath(paths);

      if (m_Info->m_pProperties->get_ImportSolidFills() && fill)
        saveSolid(paths, od_fill_color, is_object_visible, cur_layer_name, fill_color->alfa, hyperlink);
  
      if (stroke && (!fill || od_fill_color != od_stroke_color))
        savePath(paths, od_stroke_color, line_width, is_object_visible, cur_layer_name, stroke_color->alfa, line_type, hyperlink);
  
      m_Info->MeterProgress();
    }
    catch (...)
    {
      ++m_Info->m_ErrorCounter;
    }
}

void OdDbPdfImportObjectGrabber::addCosmeticLine(const OdPdfImportPoint* point1, const OdPdfImportPoint* point2, const OdPdfImportColor* color, 
  const bool is_object_visible, const wchar_t* cur_layer_name, const char* hyperlink)
{
  ++m_Info->m_ObjectCounter;

  if (nullptr == point1 || nullptr == point2 || nullptr == color)
  {
    ++m_Info->m_ErrorCounter;
    return;
  }

  try
  {
    ODCOLORREF cr_color = ODRGB(color->r, color->g, color->b);

    OdDb::LineWeight line_widht(OdDb::kLnWtByLwDefault);

    OdCmColor color = toCmColor(cr_color);

    static const OdChar* layer_name = L"PDF_Geometry";
    OdDbObjectId layer_id = getLayer(layer_name, is_object_visible, cur_layer_name);
    OdDbLayerTableRecordPtr pLayer = layer_id.openObject(OdDb::kForRead);
    if (eUseCurrentLayer == m_Info->m_pProperties->get_LayersUseType() && color == pLayer->color())
      color.setColorIndex(OdCmEntityColor::kACIbyLayer);

    OdGePoint2d od_point1(point1->x, point1->y);
    OdGePoint2d od_point2(point2->x, point2->y);

    std::function< OdDbPolylinePtr () > create_poly = [this, &color, &layer_id, &cr_color, &line_widht, hyperlink]()
    {
      OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();
      pPolyline->setDatabaseDefaults(m_pBTR->database());
      m_pBTR->appendOdDbEntity(pPolyline);
      pPolyline->setLayer(layer_id);

      pPolyline->setColor(color);
      if (eUseCurrentLayer != m_Info->m_pProperties->get_LayersUseType())
        m_LayerItems[layer_id][cr_color].push_back(pPolyline);
      pPolyline->setLineWeight(line_widht);

      if (nullptr != hyperlink)
        addHyperlink(pPolyline, hyperlink);
      return pPolyline;
    };

    if (m_Info->m_pProperties->get_UseClipping() && !m_ClipPath.isNull())
    {
      if (NULL == m_ClipPath.getOdMdFaces())
        return;

      GeIntersectorWrapper intersector_wrapper;

      std::function<void(OdGeCurve2d* curve)> process_curve = [&](OdGeCurve2d* curve)
      {
        if (NULL == curve)
          return;
        ODA_ASSERT(OdGe::kLineSeg2d == curve->type());

        OdDbPolylinePtr pPolyline = create_poly();
        OdGeCurve3dPtr input_curve3d = curve->convertTo3d();
        pPolyline->setFromOdGeCurve(*input_curve3d.get());
        pPolyline->subTransformBy(m_TransforMatrix);
      };

      std::function<void(const OdGeSurface* iSurface, const OdGeInterval& iRangeU, const OdGeInterval& iRangeV, const void* iRegion)> process_face =
        [&](const OdGeSurface* iSurface, const OdGeInterval& iRangeU, const OdGeInterval& iRangeV, const void* iRegion)
      {
        intersector_wrapper.setSurface(iSurface, iRangeU, iRangeV, iRegion);

        OdGeCurve3dPtr curve3d = new OdGeLineSeg3d(OdGePoint3d(od_point1.x, od_point1.y, 0.), OdGePoint3d(od_point2.x, od_point2.y, 0.));
        intersector_wrapper.setCurve(curve3d.get());
        intersector_wrapper.run();

        intersector_wrapper.processIntersections(&process_curve);
      };

      if (!m_ClipPath.processFaces(&process_face))
        return;
    }
    else
    {
      if (!pointInPolygon(od_point1) && !pointInPolygon(od_point2))
      {
        --m_Info->m_ObjectCounter;
        return;
      }
      OdDbPolylinePtr pPolyline = create_poly();

      pPolyline->addVertexAt(0, od_point1);
      pPolyline->addVertexAt(0, od_point2);
      pPolyline->subTransformBy(m_TransforMatrix);
    }

    m_Info->MeterProgress();

  }
  catch (...)
  {
    ++m_Info->m_ErrorCounter;
  }
}

void OdDbPdfImportObjectGrabber::addPixel(const OdPdfImportPoint* point, const OdPdfImportColor* color, const bool is_object_visible, 
  const wchar_t* cur_layer_name, const char* hyperlink)
{
  ++m_Info->m_ObjectCounter;

  if (nullptr == point || nullptr == color)
  {
    ++m_Info->m_ErrorCounter;
    return;
  }

  try
  {

    ODCOLORREF cr_color = ODRGB(color->r, color->g, color->b);

    OdGePoint2d od_point(point->x, point->y);

    OdCmColor color = toCmColor(cr_color);

    static const OdChar* layer_name = L"PDF_Geometry";
    OdDbObjectId layer_id = getLayer(layer_name, is_object_visible, cur_layer_name);
    OdDbLayerTableRecordPtr pLayer = layer_id.openObject(OdDb::kForRead);
    if (eUseCurrentLayer == m_Info->m_pProperties->get_LayersUseType() && color == pLayer->color())
      color.setColorIndex(OdCmEntityColor::kACIbyLayer);

    std::function< OdDbPointPtr() > create_point = [this, &color, &layer_id, &cr_color, hyperlink]()
    {
      OdDbPointPtr pPoint = OdDbPoint::createObject();
      pPoint->setDatabaseDefaults(m_pBTR->database());
      m_pBTR->appendOdDbEntity(pPoint);
      pPoint->setLayer(layer_id);

      pPoint->setColor(color);
      if (eUseCurrentLayer != m_Info->m_pProperties->get_LayersUseType())
        m_LayerItems[layer_id][cr_color].push_back(pPoint);

      if (nullptr != hyperlink)
        addHyperlink(pPoint, hyperlink);
      return pPoint;
    };

    if (m_Info->m_pProperties->get_UseClipping() && !m_ClipPath.isNull())
    {
      if (OdGe::kOutside != m_ClipPath.checkPointLocation(OdGePoint3d(od_point.x, od_point.y, 0.)))
      {
        OdDbPointPtr pPoint = create_point();
        pPoint->setPosition(OdGePoint3d(od_point.x, od_point.y, 0));

        pPoint->subTransformBy(m_TransforMatrix);
      }
    }
    else
    {
      if (!pointInPolygon(od_point))
      {
        --m_Info->m_ObjectCounter;
        return;
      }

      OdDbPointPtr pPoint = create_point();
      pPoint->setPosition(OdGePoint3d(od_point.x, od_point.y, 0));

      pPoint->subTransformBy(m_TransforMatrix);
    }

    m_Info->MeterProgress();
  }
  catch (...)
  {
    ++m_Info->m_ErrorCounter;
  }
}

bool OdDbPdfImportObjectGrabber::isImageClippingEnabled()
{
  return m_Info->m_pProperties->get_UseImageClipping();
}

OdStreamBufPtr convertBmpToRaster(const BmiHeaderInfo* bmi_info, const unsigned char *imagedata)
{

  BITMAPINFOHEADER bmiHeader;
  memset(&bmiHeader, 0, sizeof(BITMAPINFOHEADER));
  bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmiHeader.biBitCount = bmi_info->bpp;
  bmiHeader.biCompression = 0L;
  bmiHeader.biHeight = bmi_info->height;
  bmiHeader.biPlanes = 1;
  bmiHeader.biWidth = bmi_info->width;
  bmiHeader.biSizeImage = bmi_info->pitch*fabs(bmi_info->height);


  OdStreamBufPtr pFileBuf = OdMemoryStream::createNew();

  OdUInt32 paletteSize = (nullptr != bmi_info->palette) ? bmi_info->palette_size *4 : 0;

  if (nullptr == bmi_info->palette && bmiHeader.biBitCount == 1)
  {
    paletteSize = 8;
  }

  OdPlatformStreamer::wrInt16(*pFileBuf, 0x4d42);
  OdUInt32 scanLinesPos = (OdUInt32)sizeof(BITMAPFILEHEADER) + (OdUInt32)sizeof(BITMAPINFOHEADER) + paletteSize;  // Below version is correct only on Windows
  OdUInt32 size = scanLinesPos + OdGiRasterImage::calcBMPScanLineSize(bmiHeader.biWidth, bmiHeader.biBitCount)*bmiHeader.biHeight;

  OdPlatformStreamer::wrInt32(*pFileBuf, size);
  OdPlatformStreamer::wrInt32(*pFileBuf, 0); // reserved
  OdPlatformStreamer::wrInt32(*pFileBuf, scanLinesPos); // offBits
  OdPlatformStreamer::wrInt32(*pFileBuf, bmiHeader.biSize);  // not portable: sizeof(BITMAPINFOHEADER));
                                                       // save BITMAPINFOHEADER
  OdPlatformStreamer::wrInt32(*pFileBuf, bmiHeader.biWidth);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmiHeader.biHeight);
  OdPlatformStreamer::wrInt16(*pFileBuf, bmiHeader.biPlanes);
  OdPlatformStreamer::wrInt16(*pFileBuf, bmiHeader.biBitCount);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmiHeader.biCompression);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmiHeader.biSizeImage);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmiHeader.biXPelsPerMeter);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmiHeader.biYPelsPerMeter);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmiHeader.biClrUsed);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmiHeader.biClrImportant);

  if (nullptr == bmi_info->palette && bmiHeader.biBitCount == 1)
  {
    OdUInt32 tmp = 0;
    pFileBuf->putBytes(&tmp, 4);
    tmp = 0xffffff;
    pFileBuf->putBytes(&tmp, 4);
  }
  if (nullptr != bmi_info->palette)
  {
    pFileBuf->putBytes(bmi_info->palette, paletteSize);
  }

  pFileBuf->putBytes(imagedata, bmiHeader.biSizeImage);

  pFileBuf->rewind();

  return pFileBuf;
}

bool OdDbPdfImportObjectGrabber::createRasterImage(const BmiHeaderInfo* bmi_info, const unsigned char* imagedata, const OdPdfImportRect* rect, 
  const OdPdfImportMatrix* object_matrix, const char* hyperlink, const OdString& layer_name, const bool is_object_visible, const wchar_t* cur_layer_name, OdDbRasterImagePtr& pImage)
{
  bool ret = true;
  
  OdGePoint3d origin;
  OdGeVector3d wVector(rect->right - rect->left, 0.0, 0.0);
  OdGeVector3d hVector(0.0, rect->bottom - rect->top, 0.0);

  OdGeMatrix3d objectMatrix;
  objectMatrix[0][0] = object_matrix->a;
  objectMatrix[0][1] = object_matrix->b;
  objectMatrix[1][0] = object_matrix->c;
  objectMatrix[1][1] = object_matrix->d;
  objectMatrix[0][3] = object_matrix->e;
  objectMatrix[1][3] = object_matrix->f;

  OdGeExtents2d img_ext;
  bool has_intersections = false;
  if (!m_Info->m_pProperties->get_UseClipping() && m_ClipBoundaryExtents.isValidExtents())
  {
    OdGeVector3d z_axis(OdGeVector3d::kZAxis);
    double elevation = 0.0;
    OdGePoint2d point1 = origin.convert2d().transformBy(objectMatrix.convertToLocal(z_axis, elevation));
    OdGePoint2d point2 = (origin.convert2d() + hVector.convert2d()).transformBy(objectMatrix.convertToLocal(z_axis, elevation));
    OdGePoint2d point3 = (origin.convert2d() + wVector.convert2d() + hVector.convert2d()).transformBy(objectMatrix.convertToLocal(z_axis, elevation));
    OdGePoint2d point4 = (origin.convert2d() + wVector.convert2d()).transformBy(objectMatrix.convertToLocal(z_axis, elevation));

    img_ext.addPoint(point1);
    img_ext.addPoint(point2);
    img_ext.addPoint(point3);
    img_ext.addPoint(point4);

    std::function<bool()> has_intersections_fn = [&]()
    {
      std::vector<OdGeLineSeg2d> image_frame;
      image_frame.emplace_back(OdGeLineSeg2d(point1, point2));
      image_frame.emplace_back(OdGeLineSeg2d(point2, point3));
      image_frame.emplace_back(OdGeLineSeg2d(point3, point4));
      image_frame.emplace_back(OdGeLineSeg2d(point4, point1));

      for (OdGePoint2dArray::size_type i = 1; i < m_ClipBoundary.length(); ++i)
      {
        OdGeLineSeg2d seg(m_ClipBoundary[i - 1], m_ClipBoundary[i]);
        for (auto& elem : image_frame)
        {
          OdGePoint2d point;
          if (seg.intersectWith(elem, point))
            return true;
        }
      }
      return false;
    };

    has_intersections = has_intersections_fn();

    if (m_ClipBoundaryExtents.isDisjoint(img_ext) || (!has_intersections && (!pointInPolygon(point1) && !pointInPolygon(point2) &&
      !pointInPolygon(point3) && !pointInPolygon(point4))))
    {
      --m_Info->m_ObjectCounter;
      return true;
    }
  }

  OdStreamBufPtr image_stream_buf = convertBmpToRaster(bmi_info, imagedata);

  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  if (pRasSvcs.isNull())
  {
    m_Info->m_ImageError = OdPdfImport::image_file_error;
    return false;
  }

  try
  {
    OdString image_def_name = OdString().format(L"%ls%ls", m_PdfFileName.c_str(),
      odrxSystemServices()->createOdGUID().toString(OdGUID::StringFormat::Digits).c_str());
  
    OdDbDatabasePtr pDb = OdDbDatabasePtr(m_Info->m_pProperties->get_Database());
  
    OdString image_file_name = m_ImagesPath + image_def_name + L".PNG";
    try
    {
      OdGiRasterImagePtr pRasterImg = pRasSvcs->loadRasterImage(image_stream_buf);
      OdStreamBufPtr image_file = odSystemServices()->createFile(
        image_file_name, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
      ret = pRasSvcs->convertRasterImage(pRasterImg, OdRxRasterServices::kPNG, image_file);
    }
    catch (...)
    {
      m_Info->m_ImageError = OdPdfImport::image_file_error;
      ret = false;
    }

    OdDbObjectId imageDictId = OdDbRasterImageDef::imageDictionary(pDb);
    if (imageDictId.isNull())
    {
      imageDictId = OdDbRasterImageDef::createImageDictionary(pDb);
    }
    OdDbDictionaryPtr pImageDict = imageDictId.safeOpenObject(OdDb::kForWrite);

    OdDbRasterImageDefPtr pImageDef = OdDbRasterImageDef::createObject();
    OdDbObjectId imageDefId = pImageDict->setAt(image_def_name, pImageDef);
  
    pImageDef->setSourceFileName(image_file_name);
  
    if (pImageDef->image().isNull())
    {
      pImageDef->setImage(OdGiRasterImageDesc::createObject(bmi_info->width, bmi_info->height));
      ret = false;
    }
  
    pImage = OdDbRasterImage::createObject();
    pImage->setDatabaseDefaults(pDb);
    m_pBTR->appendOdDbEntity(pImage);
  
    pImage->setLayer(getLayer(layer_name, is_object_visible, cur_layer_name));
  
    pImage->setImageDefId(imageDefId);
  
    pImage->setDisplayOpt(OdDbRasterImage::kShow, true);
    pImage->setDisplayOpt(OdDbRasterImage::kShowUnAligned, true);
    pImage->setDisplayOpt(OdDbRasterImage::kTransparent, true);

    pImage->setOrientation(origin, wVector, hVector);

    pImage->transformBy(objectMatrix);

    pImage->transformBy(m_TransforMatrix);

    if (nullptr != hyperlink)
      addHyperlink(pImage, hyperlink);

    if (!m_Info->m_pProperties->get_UseClipping() && m_ClipBoundaryExtents.isValidExtents())
    {
      if (img_ext.isValidExtents() && (img_ext.contains(m_ClipBoundaryExtents) || has_intersections))
      {
        OdGeVector3d z_axis(OdGeVector3d::kZAxis);
        double elevation = 0.0;
        OdGeMatrix2d tmp = objectMatrix.convertToLocal(z_axis, elevation);
        OdGeMatrix2d img_matrix1;
        img_matrix1[0][0] =wVector.x;
        img_matrix1[1][1] = std::fabs(hVector.y);
        OdGeMatrix2d img_matrix2;
        img_matrix2[0][0] = bmi_info->width;
        img_matrix2[1][1] = -bmi_info->height;
        img_matrix2[1][2] = std::fabs(bmi_info->height);
        OdGeMatrix2d boundary2image = img_matrix2 * img_matrix1.inverse() * objectMatrix.convertToLocal(z_axis, elevation).inverse();

        OdGePoint2dArray boundary(m_ClipBoundary);
        for (auto& elem : boundary)
          elem.transformBy(boundary2image);

        pImage->setClipBoundary(boundary);
        pImage->setClipInverted(m_isClipBoundaryInverted);
        pImage->setDisplayOpt(OdDbRasterImage::kClip, true);
      }
    }
 
  }
  catch (...)
  {
    ret = false;
  }
  return ret;
}

void OdDbPdfImportObjectGrabber::addImage(const BmiHeaderInfo* bmi, const unsigned char* imagedata, const OdPdfImportRect* rect,
  const OdPdfImportMatrix* object_matrix, const bool is_object_visible, const wchar_t* cur_layer_name, const char* hyperlink)
{
  ++m_Info->m_ObjectCounter;

  if (nullptr == bmi || nullptr == imagedata || nullptr == rect || nullptr == object_matrix)
  {
    ++m_Info->m_ErrorCounter;
    return;
  }

  try
  {
    static const OdChar* layer_name = L"PDF_Images";

    OdDbRasterImagePtr pImage;
    if(!createRasterImage(bmi, imagedata, rect, object_matrix, hyperlink, layer_name, is_object_visible, cur_layer_name, pImage))
      ++m_Info->m_ErrorCounter;
    if (pImage.isNull())
      return;

    m_Info->MeterProgress();

  }
  catch (...)
  {
    ++m_Info->m_ErrorCounter;
  }
}

bool OdDbPdfImportObjectGrabber::getClipBoundary(std::vector<OdPdfImportPathData>& path_data)
{
  const bool has_clip_boundary = 0 != m_ClipBoundary.size();
  path_data.resize(has_clip_boundary & m_isClipBoundaryInverted ? m_ClipBoundary.size() + 5 : m_ClipBoundary.size());
  for (OdGePoint2dArray::size_type i = 0; i < m_ClipBoundary.size(); ++i)
  {
    path_data[i].m_Point.x = m_ClipBoundary[i].x;
    path_data[i].m_Point.y = m_ClipBoundary[i].y;
    path_data[i].m_Type = (0 == i) ? OdPdfImportPtType::OdMoveTo : OdPdfImportPtType::OdLineTo;
    path_data[i].m_CloseFigure = i == m_ClipBoundary.size() - 1;
  }
  if (has_clip_boundary & m_isClipBoundaryInverted)
  {
    path_data[m_ClipBoundary.size()].m_Point.x = 0;
    path_data[m_ClipBoundary.size()].m_Point.y = 0;
    path_data[m_ClipBoundary.size()].m_Type = OdPdfImportPtType::OdMoveTo;
    path_data[m_ClipBoundary.size()].m_CloseFigure = false;

    path_data[m_ClipBoundary.size() + 1].m_Point.x = 0;
    path_data[m_ClipBoundary.size() + 1].m_Point.y = getHeight();
    path_data[m_ClipBoundary.size() + 1].m_Type = OdPdfImportPtType::OdLineTo;
    path_data[m_ClipBoundary.size() + 1].m_CloseFigure = false;

    path_data[m_ClipBoundary.size() + 2].m_Point.x = getWidth();
    path_data[m_ClipBoundary.size() + 2].m_Point.y = getHeight();
    path_data[m_ClipBoundary.size() + 2].m_Type = OdPdfImportPtType::OdLineTo;
    path_data[m_ClipBoundary.size() + 2].m_CloseFigure = false;

    path_data[m_ClipBoundary.size() + 3].m_Point.x = getWidth();
    path_data[m_ClipBoundary.size() + 3].m_Point.y = 0;
    path_data[m_ClipBoundary.size() + 3].m_Type = OdPdfImportPtType::OdLineTo;
    path_data[m_ClipBoundary.size() + 3].m_CloseFigure = false;

    path_data[m_ClipBoundary.size() + 4].m_Point.x = 0;
    path_data[m_ClipBoundary.size() + 4].m_Point.y = 0;
    path_data[m_ClipBoundary.size() + 4].m_Type = OdPdfImportPtType::OdMoveTo;
    path_data[m_ClipBoundary.size() + 4].m_CloseFigure = true;
  }

  return has_clip_boundary;
}

void OdDbPdfImportObjectGrabber::addText(const wchar_t* text, const PdfImportFontParams* const font_params, const OdPdfImportColor* color, 
  const OdPdfImportMatrix* object_matrix, const bool is_object_visible, const wchar_t* cur_layer_name, const char* hyperlink)
{
  ++m_Info->m_ObjectCounter;

  try
  {
    OdGeMatrix3d od_object_matrix;
    od_object_matrix.setToIdentity();
    od_object_matrix[0][0] = object_matrix->a;
    od_object_matrix[1][0] = object_matrix->b;
    od_object_matrix[0][1] = object_matrix->c;
    od_object_matrix[1][1] = object_matrix->d;
    od_object_matrix[0][3] = object_matrix->e;
    od_object_matrix[1][3] = object_matrix->f;

    OdGePoint3d origin;
    OdGeVector3d xAxis, yAxis, zAxis;
    od_object_matrix.getCoordSystem(origin, xAxis, yAxis, zAxis);

    double angleXY = xAxis.angleTo(yAxis);

    OdGeVector3d z_axis(OdGeVector3d::kZAxis);
    double elevation = 0.0;
    const OdGePoint2d point1 = OdGePoint2d().transformBy(od_object_matrix.convertToLocal(z_axis, elevation));
    const OdGePoint2d point2 = OdGePoint2d(font_params->text_width, 0.).transformBy(od_object_matrix.convertToLocal(z_axis, elevation));
    const OdGePoint2d point3 = OdGePoint2d(0., font_params->font_height).transformBy(od_object_matrix.convertToLocal(z_axis, elevation));
    const OdGePoint2d point4 = OdGePoint2d(font_params->text_width, font_params->font_height).transformBy(od_object_matrix.convertToLocal(z_axis, elevation));

    if (!pointInPolygon(point1) && !pointInPolygon(point2) &&
      !pointInPolygon(point3) && !pointInPolygon(point4))
    {
      --m_Info->m_ObjectCounter;
      return;
    }

    OdString new_str(text);
    new_str.replace(L"\\", L"\\\\");

    if (font_params->is_space || font_params->is_char_space)
    {
      if (font_params->is_space)
        m_ContentStr += OdString(L" ") + new_str;
      else
        m_ContentStr += new_str;

      m_pLastMText->setContents(m_ContentStr);

      const double m_text_width = m_pLastMText->actualWidth();

      const double coef = OdGeVector3d(font_params->text_width, 0., 0.).transformBy(m_TransforMatrix).length() / m_text_width;
      if (!OdEqual(coef, 1.0))
      {
        m_FormatString = OdString().format(L"\\W%f;", coef);
        m_FormatString.replace(L',', L'.');
      }

      m_pLastMText->setContents(m_FormatString + m_ItalicFormatString + m_ContentStr);

      OdGeVector3d underline_dir(font_params->underline_point_X, font_params->underline_point_Y, 0.);
      underline_dir.transformBy(m_TransforMatrix); 
      m_pLastMText->transformBy(OdGeMatrix3d::translation(underline_dir));
      return;
    }

    OdString font_face_name(font_params->in_font_face_name);

    const bool change_font_for_type3 = !font_params->isChanged && font_params->isType3Font && !m_Info->m_pProperties->get_Type3TextDefaultFont().isEmpty();
    if (change_font_for_type3)
    {
      font_face_name = m_Info->m_pProperties->get_Type3TextDefaultFont();
    }
    OdString font_style_name = OdString(L"PDF ") + font_face_name;
    ReplaceForbiddenChars(font_style_name);

    if (font_params->is_bold)
      font_style_name += L" Bold";
    
    if (font_params->is_italic)
      font_style_name += L" Italic";
    
    m_ContentStr = new_str;
    
    OdDbDatabasePtr pDb = OdDbDatabasePtr(m_Info->m_pProperties->get_Database());
    
    OdDbTextStyleTablePtr pStyles = pDb->getTextStyleTableId().safeOpenObject(OdDb::kForWrite);
    
    OdDbObjectId styleId = pStyles->getAt(font_style_name);
    
    if (styleId.isNull())
    {
      OdDbTextStyleTableRecordPtr pStyle = OdDbTextStyleTableRecord::createObject();
    
      pStyle->setName(font_style_name);
    
      styleId = pStyles->add(pStyle);
    
      pStyle->setTextSize(0.);
    
      if (0 == font_face_name.right(4).iCompare(L".shx"))
        pStyle->setFileName(font_face_name);
      else
        pStyle->setFont(font_face_name, font_params->is_bold, font_params->is_italic, false, 0);
    }
    
    OdDbMTextPtr pMText = OdDbMText::createObject();
    pMText->setDatabaseDefaults(m_pBTR->database());
    m_pBTR->appendOdDbEntity(pMText);

    m_pLastMText = pMText;
    
    OdCmColor od_color = toCmColor(ODRGB(color->r, color->g, color->b));
    static const OdChar* layer_name = L"PDF_Text";
    OdDbObjectId layerId = getLayer(layer_name, is_object_visible, cur_layer_name);
    OdDbLayerTableRecordPtr pLayer = layerId.openObject(OdDb::kForRead);
    if (eUseCurrentLayer == m_Info->m_pProperties->get_LayersUseType() && od_color == pLayer->color())
      od_color.setColorIndex(OdCmEntityColor::kACIbyLayer);
    
    pMText->setLayer(layerId);
        
    pMText->setColor(od_color);
    if (eUseCurrentLayer != m_Info->m_pProperties->get_LayersUseType())
      m_LayerItems[layerId][ODRGB(color->r, color->g, color->b)].push_back(pMText);
    
    pMText->setAttachment(OdDbMText::kBottomLeft);
    
    pMText->setDirection(OdGeVector3d::kXAxis);
    
    pMText->setTextHeight(font_params->font_height);
    
    pMText->setTextStyle(styleId);
    
    pMText->setContents(m_ContentStr);

    if (!OdEqual(angleXY, OdaPI2))
    {
      m_ItalicFormatString = OdDbMText::obliqueChange() + odDToStr(OdaToDegree(OdaPI2 - angleXY), 'f', 5) + OD_T(";");
    }
    else
      m_ItalicFormatString.empty();

    pMText->transformBy(od_object_matrix);

    const double m_text_width = pMText->actualWidth();

    const double coef = font_params->text_width / m_text_width;
    if (!OdEqual(coef, 1.0))
    {
      m_FormatString = OdString().format(L"\\W%f;", coef);
      m_FormatString.replace(L',', L'.');
    }
    pMText->setContents(m_FormatString + m_ItalicFormatString + m_ContentStr);
    
    pMText->subTransformBy(m_TransforMatrix);

    if (nullptr != hyperlink)
      addHyperlink(pMText, hyperlink);
  }
  catch (...)
  {
    ++m_Info->m_ErrorCounter;
  }
}

bool OdDbPdfImportObjectGrabber::combineTextObjects() const
{
  return m_Info->m_pProperties->get_CombineTextObjects();
}

void OdDbPdfImportObjectGrabber::getSpaceFactors(double& char_space_factor, double& space_factor) const
{
  char_space_factor = m_Info->m_pProperties->get_CharSpaceFactor();  space_factor = m_Info->m_pProperties->get_SpaceFactor();
}

bool OdDbPdfImportObjectGrabber::extractEmbeddedFonts() const
{
  return m_Info->m_pProperties->get_ExtractEmbeddedFonts();
}

void OdDbPdfImportObjectGrabber::addShading(const BmiHeaderInfo* bmi, const unsigned char* imagedata, const OdPdfImportRect* rect,
  const OdPdfImportMatrix* object_matrix, const bool is_object_visible, const wchar_t* cur_layer_name, const char* hyperlink)
{
  ++m_Info->m_ObjectCounter;

  if (nullptr == bmi || nullptr == imagedata || nullptr == rect || nullptr == object_matrix)
  {
    ++m_Info->m_ErrorCounter;
    return;
  }

  try
  {
    static const OdChar* layer_name = L"PDF_Gradient Fills";

    OdDbRasterImagePtr pImage;
    if (!createRasterImage(bmi, imagedata, rect, object_matrix, hyperlink, layer_name, is_object_visible, cur_layer_name, pImage))
      ++m_Info->m_ErrorCounter;
    if (pImage.isNull())
      return;

    m_Info->MeterProgress();

  }
  catch (...)
  {
    ++m_Info->m_ErrorCounter;
  }
}

void OdDbPdfImportObjectGrabber::fillPathData(const OdPdfImportPathData* path_data, const unsigned long path_point_count, std::vector<BaseCurve>& paths)
{
  if (nullptr == path_data)
    return;

  OdGePoint2dArray pt_cache;
  paths.resize(1);

  for (size_t i = 0; i < path_point_count; ++i)
  {

    OdGePoint2d point(path_data[i].m_Point.x, path_data[i].m_Point.y);

    OdPdfImportPtType point_type = path_data[i].m_Type;
    if (point_type == OdPdfImportPtType::OdMoveTo)
    {
      if (pt_cache.size())
      {
        paths.back().addSeg(pt_cache, path_data[i - 1].m_Type);
        if (path_data[i - 1].m_CloseFigure || paths.back().getSegments().front().getPointStorage().first().isEqualTo(pt_cache.last()))
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
        if (path_data[i - 1].m_CloseFigure || paths.back().getSegments().front().getPointStorage().first().isEqualTo(pt_cache.last()))
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
        if (path_data[i - 1].m_CloseFigure || paths.back().getSegments().front().getPointStorage().first().isEqualTo(pt_cache.last()))
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
    if (path_data[path_point_count - 1].m_CloseFigure || paths.back().getSegments().front().getPointStorage().first().isEqualTo(pt_cache.last()))
    {
      paths.back().setClosed(path_data[path_point_count - 1].m_CloseFigure);
    }
  }
}

void OdDbPdfImportObjectGrabber::fillLoops(const std::vector<BaseCurve>& paths, OdArray< OdArray<OdGeCurve2d*> >& loops, bool fill_solid)
{
  //TODO optimize

  std::function<void(OdArray<OdGeCurve2d*>& loop, const BaseCurve::Segment& seg)> add_nurbs = 
    [](OdArray<OdGeCurve2d*>& loop, const BaseCurve::Segment& seg)
  {
    OdGeKnotVector knotsVec;
    OdGeDoubleArray wts;

    fillKnots(seg.getPointStorage().size(), knotsVec, wts);
    loop.append(new OdGeNurbCurve2d(3, knotsVec, seg.getPointStorage(), wts));
  };

  std::function<void(OdArray<OdGeCurve2d*>& loop, const BaseCurve::Segment& seg, bool set_close)> add_poly =
    [&fill_solid](OdArray<OdGeCurve2d*>& loop, const BaseCurve::Segment& seg, bool set_close)
  {
    for (OdGePoint2dArray::size_type pt = 0; pt < seg.getPointStorage().size() - 1; ++pt)
    {
      if (((pt == (seg.getPointStorage().size() - 2)) && (0 == loop.size())) || !seg.getPointStorage()[pt].isEqualTo(seg.getPointStorage()[pt + 1]))
        loop.append(new OdGeLineSeg2d(seg.getPointStorage()[pt], seg.getPointStorage()[pt + 1]));
    }
    if (fill_solid && set_close && !seg.getPointStorage().last().isEqualTo(seg.getPointStorage().first()))
    {
      loop.append(new OdGeLineSeg2d(seg.getPointStorage().last(), seg.getPointStorage().first()));
    }
  };

  for (int i = 0; i < paths.size(); ++i)
  {
    const auto& fig = paths[i];

    const bool is_poly = fig.isPolyline();

    auto loop = loops.append();

    const auto segments = fig.getSegments();
    for (int j = 0; j < segments.size(); ++j)
    {
      if(is_poly && j > 0)
        loop = loops.append();
      const auto seg = segments[j];
      switch (seg.getType())
      {
      case BaseCurve::kLine:
      {
        add_poly(*loop, seg, is_poly);
      }
      break;
      case BaseCurve::kBezier:
      {
        add_nurbs(*loop, seg);
      }
      break;
      case BaseCurve::kCircleArc:
      case BaseCurve::kCircle:
      case BaseCurve::kEllipseArc:
      case BaseCurve::kEllipse:
        if (fill_solid)
        {
          if(OdPdfImportPtType::OdLineTo == seg.getPointsType())
            add_poly(*loop, seg, is_poly);
          else
            add_nurbs(*loop, seg);
        }
        else
          loops[i].append(seg.getCurve()->convertTo2d());
        break;
      default:
        break;
      }
    }
  }
}

void OdDbPdfImportObjectGrabber::cutPath(std::vector<BaseCurve>& clipped_paths, const std::function<bool(const OdGePoint2d& point)>& check_point_func)
{
  for (auto& fig : clipped_paths)
  {
    auto& segments = fig.getSegments();
    auto end_iter = std::remove_if(segments.begin(), segments.end(), [this, check_point_func](const BaseCurve::Segment& seg)
      {
        bool ret = false;
        for (auto& point : seg.getPointStorage())
        {
          ret = ret || check_point_func(point);
        }
        return !ret;
      }
    );
    segments.erase(end_iter, segments.end());
  }
}

void OdDbPdfImportObjectGrabber::addHyperlink(OdDbEntityPtr ent, OdString hyperlink)
{
  OdDbEntityHyperlinkPEPtr    hpe = ent;
  OdDbHyperlinkCollectionPtr  urls = hpe->getHyperlinkCollection(ent);

  urls->addTail(hyperlink, OdString());

  hpe->setHyperlinkCollection(ent, urls);
}

void OdDbPdfImportObjectGrabber::addClipPath(const OdPdfImportPathData* path_data, const unsigned long path_point_count)
{
  if (!m_Info->m_pProperties->get_UseClipping())
    return;

  std::vector<BaseCurve> paths;

  fillPathData(path_data, path_point_count, paths);

  if (!m_Info->m_pProperties->get_UsePageBorderClipping())
  {
    if (0 == m_PageBorder.size() && 0 < paths.size() && 0 < paths[0].getSegments().size() && 5 == paths[0].getSegments()[0].getPointStorage().size())
    {
      m_PageBorder = paths[0].getSegments()[0].getPointStorage();
      return;
    }
    if (1 == paths.size() && 1 == paths[0].getSegments().size() && BaseCurve::kLine == paths[0].getSegments()[0].getType() && 5 == paths[0].getSegments()[0].getPointStorage().size())
    {
      const OdGePoint2dArray& point_array = paths[0].getSegments()[0].getPointStorage();
      if(m_PageBorder[0].isEqualTo(point_array[0]) && m_PageBorder[1].isEqualTo(point_array[1]) && m_PageBorder[2].isEqualTo(point_array[2]) &&
        m_PageBorder[3].isEqualTo(point_array[3]) && m_PageBorder[4].isEqualTo(point_array[4]))
        return;
    }
  }

  OdArray< OdArray<OdGeCurve2d*> > loops;

  fillLoops(paths, loops, true);

  if (!m_ClipPath.isNull())
  {
    try
    {
      void* new_clip_path = createPlanarOdMdBodyFromCurves(loops);
      if (nullptr == new_clip_path)
        return;
      void* result(nullptr);
      if (booleanOperationForMdBody(OdGe::kCommon, new_clip_path, true, m_ClipPath, false, result))
        m_ClipPath = result;
      else
        RestoreClipState(true);

    }
    catch (...)
    {
      RestoreClipState(true);
    }
  }
  else
  {
    try
    {
      m_ClipPath = createPlanarOdMdBodyFromCurves(loops);
      if (!m_ClipBoundaryBody.isNull() && nullptr != m_ClipPath )
      {
        void* result(nullptr);
        if (booleanOperationForMdBody(m_isClipBoundaryInverted ? OdGe::kSubtract : OdGe::kCommon, m_ClipPath, false, m_ClipBoundaryBody, false, result))
          m_ClipPath = result;
      }
    }
    catch (...)
    {

    }
  }
}

void OdDbPdfImportObjectGrabber::SaveClipState()
{
  if (!m_Info->m_pProperties->get_UseClipping())
    return;

  if (m_ClipPath.isNull())
    return;
  MdFaceWrapper pClip;
  pClip = m_ClipPath.clone(false);
  m_ClipStack.push_back(pClip);
}

void OdDbPdfImportObjectGrabber::RestoreClipState(bool bKeepSaved)
{
  m_ClipPath = NULL;
  if (m_ClipStack.empty())
    return;
  if (bKeepSaved)
  {
    m_ClipPath = m_ClipStack.back().clone(false);
  }
  else
  {
    m_ClipPath = m_ClipStack.back();
    m_ClipStack.pop_back();
  }
}

bool OdDbPdfImportObjectGrabber::isClippingEnabled()
{
  return m_Info->m_pProperties->get_UseClipping();
}

ImportHyperlinkMethod OdDbPdfImportObjectGrabber::getImportHyperlinkMethod()
{
  switch (m_Info->m_pProperties->get_ImportHyperlinkMethod())
  {
  case 0:
  default:
    return ImportHyperlinkMethod::eImportAsEntityHyperlink;
    break;
  case 1:
    return ImportHyperlinkMethod::eImportAsTrasparentRegion;
  case 2:
    return ImportHyperlinkMethod::eMixedModeImport;
    break;
  }
}

void OdDbPdfImportObjectGrabber::addHyperlinkAsTransparentRegion(const OdPdfImportPathData* path_data, const unsigned long path_point_count, const char* hyperlink)
{
  ODCOLORREF od_fill_color = ODRGB(255, 255, 255);
  std::vector<BaseCurve> paths;

  fillPathData(path_data, path_point_count, paths);
  bool old_state_sth = m_Info->m_pProperties->get_ConvertSolidsToHatches();
  bool old_state_tr = m_Info->m_pProperties->get_UsePdfTransparency();
  m_Info->m_pProperties->put_ConvertSolidsToHatches(true);
  m_Info->m_pProperties->put_UsePdfTransparency(true);
  saveSolid(paths, od_fill_color, true, nullptr, 0, hyperlink);
  m_Info->m_pProperties->put_ConvertSolidsToHatches(old_state_sth);
  m_Info->m_pProperties->put_UsePdfTransparency(old_state_tr);

}

bool OdDbPdfImportObjectGrabber::getPreferableFont(const char* font_name, const char* font_family, const bool is_found, char* preferable_font_name, 
  unsigned int& font_name_size)
{
  if (nullptr != m_PreferableFontCallback && nullptr != preferable_font_name)
  {
    OdAnsiString tmp = m_PreferableFontCallback(font_name, font_family, is_found);
    if (tmp.isEmpty())
      return false;
    strcpy(preferable_font_name, tmp.c_str());
    font_name_size = tmp.getLength();
    return true;
  }
  return false;
}

void OdDbPdfImportObjectGrabber::savePath(const std::vector<BaseCurve>& paths, const ODCOLORREF cr_color,
  const float* line_width, const bool is_object_visible, const wchar_t* cur_layer_name, const OdUInt8 alpha, 
  const LineTypeInfo* line_type, const char* hyperlink)
{
  OdDbDatabasePtr pDb = OdDbDatabasePtr(m_Info->m_pProperties->get_Database());

  static const OdChar* layer_name = L"PDF_Geometry";
  OdCmColor color = toCmColor(cr_color);
  OdDbObjectId layer_id = getLayer(layer_name, is_object_visible, cur_layer_name);
  OdDbLayerTableRecordPtr pLayer = layer_id.openObject(OdDb::kForRead);
  if (eUseCurrentLayer == m_Info->m_pProperties->get_LayersUseType() && color == pLayer->color())
    color.setColorIndex(OdCmEntityColor::kACIbyLayer);

  OdDb::LineWeight line_weight(OdDb::kLnWtByLwDefault);

  if (m_Info->m_pProperties->get_ApplyLineweight() && nullptr != line_width)
  {
    OdDb::LineWeight prev_line_widht(OdDb::kLnWt000);
    for (int i = 0; i < 24; ++i)
    {
      line_weight = ::lineWeightByIndex(i);
      if ((*line_width >= (float)prev_line_widht / 100.) && (*line_width < (float)line_weight / 100.))
      {
        if ((*line_width - (float)prev_line_widht / 100.) < ((float)line_weight / 100. - *line_width))
        {
          line_weight = prev_line_widht;
        }
        break;
      }
      prev_line_widht = line_weight;
    }
  }

  std::function< void (OdDbCurve* pCurve) > fill_entity = [this, &color, &layer_id, &cr_color, &line_weight, alpha, line_type, hyperlink](OdDbCurve* pCurve)
  {
    pCurve->setDatabaseDefaults(m_pBTR->database());
    m_pBTR->appendOdDbEntity(pCurve);
    pCurve->setLayer(layer_id);
    pCurve->setColor(color);
    if (eUseCurrentLayer != m_Info->m_pProperties->get_LayersUseType())
      m_LayerItems[layer_id][cr_color].push_back(pCurve);
    pCurve->setLineWeight(line_weight);
    if(m_Info->m_pProperties->get_UsePdfTransparency())
      pCurve->setTransparency(OdCmTransparency(alpha));
    if (nullptr != line_type)
    {
      OdDbLinetypeTablePtr pLinetypeTable = m_pBTR->database()->getLinetypeTableId().safeOpenObject(OdDb::kForWrite);

      OdDbObjectId style_id;

      for (auto& style : m_LineStyles)
      {
        if (line_type->dash_count == style->numDashes())
        {
          bool is_equal = true;
          for (int i = 0; i < line_type->dash_count; ++i)
          {
            if (!OdEqual(style->dashLengthAt(i), line_type->dash_array[i]))
            {
              is_equal = false;
              break;
            }
          }
          if (is_equal)
          {
            style_id = style->objectId();
            break;
          }
        }
      }

      if (style_id.isNull())
      {
        OdDbLinetypeTableRecordPtr pLineType = OdDbLinetypeTableRecord::createObject();

        pLineType->setNumDashes(line_type->dash_count);

        for (int i = 0; i < line_type->dash_count; ++i)
        {
          pLineType->setDashLengthAt(i, line_type->dash_array[i]);
        }

        OdString style_name = OdString(L"PDF_LineType") + odrxSystemServices()->createOdGUID().toString(OdGUID::StringFormat::Digits);
        pLineType->setName(style_name);

        style_id = pLinetypeTable->add(pLineType);

        m_LineStyles.push_back(pLineType);
      }

      pCurve->setLinetype(style_id);
    }

    if (nullptr != hyperlink)
      addHyperlink(pCurve, hyperlink);
  };

  std::function<void()> default_save_path = [&]()
  {
    for (const auto& fig : paths)
    {
      for (const auto& seg : fig.getSegments())
      {
        switch (seg.getType())
        {
        case BaseCurve::kCircle:
        {
          OdDbCirclePtr pCircle = OdDbCircle::createObject();
          fill_entity(pCircle.get());
          pCircle->setFromOdGeCurve(*seg.getCurve().get());
          pCircle->transformBy(m_TransforMatrix);
        }
        break;
        case BaseCurve::kCircleArc:
        {
          OdDbArcPtr pCircleArc = OdDbArc::createObject();
          fill_entity(pCircleArc.get());
          pCircleArc->setFromOdGeCurve(*seg.getCurve().get());
          pCircleArc->transformBy(m_TransforMatrix);
        }
        break;
        case BaseCurve::kEllipse:
        case BaseCurve::kEllipseArc:
        {
          OdDbEllipsePtr pEllipse = OdDbEllipse::createObject();
          fill_entity(pEllipse.get());
          pEllipse->setFromOdGeCurve(*seg.getCurve().get());
          pEllipse->transformBy(m_TransforMatrix);
        }
        break;
        case BaseCurve::kLine:
        {
          OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();
          fill_entity(pPolyline.get());

          for (OdUInt32 i = 0; i < seg.getPointStorage().size(); ++i)
          {
            pPolyline->addVertexAt(i, seg.getPointStorage()[i]);
          }
          pPolyline->transformBy(m_TransforMatrix);
        }
        break;
        case BaseCurve::kBezier:
        {
          OdGePoint3dArray ctrlPts;
          ctrlPts.resize(seg.getPointStorage().size());

          for (OdUInt32 i = 0; i < seg.getPointStorage().size(); i++)
          {
            ctrlPts[i] = OdGePoint3d(seg.getPointStorage()[i].x, seg.getPointStorage()[i].y, 0);
          }

          OdGeKnotVector knotsVec;
          OdGeDoubleArray wts;

          fillKnots(seg.getPointStorage().size(), knotsVec, wts);

          OdDbSplinePtr pSpline = OdDbSpline::createObject();
          fill_entity(pSpline.get());

          pSpline->setNurbsData(
            3,			/* int degree, */
            1, 			/* bool rational, */
            false, 		/* bool closed, */
            0,			/* bool periodic,*/
            ctrlPts,	        /* const OdGePoint3dArray& controlPoints, */
            knotsVec,		/* const OdGeDoubleArray& knots, */
            wts,			/* const OdGeDoubleArray& weights,*/
            0.0			/* double controlPtTol, */
          );
          pSpline->transformBy(m_TransforMatrix);
        }
        break;
        default:
          break;
        }
      }
    }
  };

  if (m_Info->m_pProperties->get_UseClipping() && !m_ClipPath.isNull())
  {
    if (NULL == m_ClipPath.getOdMdFaces())
      return;

    OdArray< OdArray<OdGeCurve2d*> > loops;
    fillLoops(paths, loops);
    LoopsAutoDispose loopsDisposer(loops);

    GeIntersectorWrapper intersector_wrapper;

    std::function<void(const OdGeSurface* iSurface, const OdGeInterval& iRangeU, const OdGeInterval& iRangeV, const void* iRegion)> process_face =
      [&](const OdGeSurface* iSurface, const OdGeInterval& iRangeU, const OdGeInterval& iRangeV, const void* iRegion)
    {
      intersector_wrapper.setSurface(iSurface, iRangeU, iRangeV, iRegion);

      for (const auto& loop : loops)
      {
        OdDbPolylinePtr pPolyline;

        for (const auto& segment : loop)
        {
          OdGeCurve3dPtr curve3d = segment->convertTo3d();

          if (OdEqual(curve3d->length(), 0.))
          {
            ODA_ASSERT(segment->type() == OdGe::kLineSeg2d);

            if (!pPolyline.isNull())
            {
              OdGeVector3d z_axis(OdGeVector3d::kZAxis);
              double elevation = 0.0;
              OdGeMatrix2d matrix(m_TransforMatrix.convertToLocal(z_axis, elevation));
              const unsigned int num_verts = pPolyline->numVerts();
              if (num_verts > 0)
              {
                OdGePoint2d end_point;
                pPolyline->getPointAt(num_verts - 1, end_point);
                if (end_point.isEqualTo(static_cast<OdGeLineSeg2d*>(segment)->startPoint().transformBy(matrix)))
                {
                  pPolyline->addVertexAt(num_verts, static_cast<OdGeLineSeg2d*>(segment)->endPoint().transformBy(matrix));
                  continue;;
                }
              }
            }

            OdGePoint3d point;
            const bool has_point = curve3d->hasStartPoint(point);
            if (has_point && OdGe::kOutside != m_ClipPath.checkPointLocation(point))
            {
              pPolyline = OdDbPolyline::createObject();
              fill_entity(pPolyline.get());
              pPolyline->addVertexAt(0, static_cast<OdGeLineSeg2d*>(segment)->startPoint());
              pPolyline->addVertexAt(1, static_cast<OdGeLineSeg2d*>(segment)->endPoint());
              pPolyline->transformBy(m_TransforMatrix);
            }
            continue;
          }

          intersector_wrapper.setCurve(curve3d.get());
          if (!intersector_wrapper.run())
          {
            default_save_path();
            return;
          }

          std::function<void(OdGeCurve2d* curve)> process_curve = [&](OdGeCurve2d* curve)
          {
            if (NULL == curve)
              return;

            switch (curve->type())
            {
            case OdGe::kCircArc2d:
            {
              OdGeCurve3dPtr input_curve3d = curve->convertTo3d();

              OdDbCurvePtr pCircleArc;
              if (OdEqual(static_cast<OdGeCircArc2d*>(curve)->startAng(), 0.) &&
                OdEqual(static_cast<OdGeCircArc2d*>(curve)->endAng(), Oda2PI))
                pCircleArc = OdDbCircle::createObject();
              else
                pCircleArc = OdDbArc::createObject();

              fill_entity(pCircleArc.get());
              pCircleArc->setFromOdGeCurve(*input_curve3d.get());
              pCircleArc->transformBy(m_TransforMatrix);
            }
            break;
            case OdGe::kEllipArc2d:
            {
              OdGeCurve3dPtr input_curve3d = curve->convertTo3d();
              OdDbEllipsePtr pEllipse = OdDbEllipse::createObject();

              fill_entity(pEllipse.get());
              pEllipse->setFromOdGeCurve(*input_curve3d.get());
              pEllipse->transformBy(m_TransforMatrix);
            }
            break;
            case OdGe::kLineSeg2d:
            {
              if (!pPolyline.isNull())
              {
                OdGeVector3d z_axis(OdGeVector3d::kZAxis);
                double elevation = 0.0;
                OdGeMatrix2d matrix(m_TransforMatrix.convertToLocal(z_axis, elevation));
                const unsigned int num_verts = pPolyline->numVerts();
                if (num_verts > 0)
                {
                  OdGePoint2d end_point;
                  pPolyline->getPointAt(num_verts - 1, end_point);
                  if (end_point.isEqualTo(static_cast<OdGeLineSeg2d*>(curve)->startPoint().transformBy(matrix)))
                  {
                    pPolyline->addVertexAt(num_verts, static_cast<OdGeLineSeg2d*>(curve)->endPoint().transformBy(matrix));
                    return;
                  }
                }
              }

              OdGeCurve3dPtr input_curve3d = curve->convertTo3d();
              OdGeCurve3dPtrArray array;
              array.append(input_curve3d);
              OdGeCompositeCurve3d curve(array);

              pPolyline = OdDbPolyline::createObject();

              fill_entity(pPolyline.get());
              pPolyline->setFromOdGeCurve(curve);
              pPolyline->transformBy(m_TransforMatrix);
            }
            break;
            case OdGe::kNurbCurve2d:
            {
              OdGeCurve3dPtr input_curve3d = curve->convertTo3d();

              OdDbSplinePtr pSpline = OdDbSpline::createObject();
              pSpline->setDatabaseDefaults(m_pBTR->database());
              fill_entity(pSpline.get());
              pSpline->setFromOdGeCurve(*(input_curve3d.get()));
              pSpline->transformBy(m_TransforMatrix);
            }
            break;
            case OdGe::kEntity2d:
            case OdGe::kEntity3d:
            case OdGe::kPointEnt2d:
            case OdGe::kPointEnt3d:
            case OdGe::kPosition2d:
            case OdGe::kPosition3d:
            case OdGe::kPointOnCurve2d:
            case OdGe::kPointOnCurve3d:
            case OdGe::kPointOnSurface:
            case OdGe::kBoundedPlane:
            case OdGe::kCircArc3d:
            case OdGe::kConic2d:
            case OdGe::kConic3d:
            case OdGe::kCurve2d:
            case OdGe::kCurve3d:
            case OdGe::kEllipArc3d:
            case OdGe::kLine2d:
            case OdGe::kLine3d:
            case OdGe::kLinearEnt2d:
            case OdGe::kLinearEnt3d:
            case OdGe::kLineSeg3d:
            case OdGe::kPlanarEnt:
            case OdGe::kPlane:
            case OdGe::kRay2d:
            case OdGe::kRay3d:
            case OdGe::kSurface:
            case OdGe::kSphere:
            case OdGe::kCylinder:
            case OdGe::kTorus:
            case OdGe::kCone:
            case OdGe::kSplineEnt2d:
            case OdGe::kPolyline2d:
            case OdGe::kAugPolyline2d:
            case OdGe::kDSpline2d:
            case OdGe::kCubicSplineCurve2d:
            case OdGe::kSplineEnt3d:
            case OdGe::kPolyline3d:
            case OdGe::kAugPolyline3d:
            case OdGe::kNurbCurve3d:
            case OdGe::kDSpline3d:
            case OdGe::kCubicSplineCurve3d:
            case OdGe::kTrimmedCrv2d:
            case OdGe::kCompositeCrv2d:
            case OdGe::kCompositeCrv3d:
            case OdGe::kExternalSurface:
            case OdGe::kNurbSurface:
            case OdGe::kTrimmedSurface:
            case OdGe::kOffsetSurface:
            case OdGe::kEnvelope2d:
            case OdGe::kCurveBoundedSurface:
            case OdGe::kExternalCurve3d:
            case OdGe::kExternalCurve2d:
            case OdGe::kSurfaceCurve2dTo3d:
            case OdGe::kSurfaceCurve3dTo2d:
            case OdGe::kExternalBoundedSurface:
            case OdGe::kCurveCurveInt2d:
            case OdGe::kCurveCurveInt3d:
            case OdGe::kBoundBlock2d:
            case OdGe::kBoundBlock3d:
            case OdGe::kOffsetCurve2d:
            case OdGe::kOffsetCurve3d:
            case OdGe::kPolynomCurve3d:
            case OdGe::kBezierCurve3d:
            case OdGe::kObject:
            case OdGe::kFitData3d:
            case OdGe::kHatch:
            case OdGe::kTrimmedCurve2d:
            case OdGe::kTrimmedCurve3d:
            case OdGe::kCurveSampleData:
            case OdGe::kEllipCone:
            case OdGe::kEllipCylinder:
            case OdGe::kIntervalBoundBlock:
            case OdGe::kClipBoundary2d:
            case OdGe::kExternalObject:
            case OdGe::kCurveSurfaceInt:
            case OdGe::kSurfaceSurfaceInt:
            case OdGe::kHelix:
            case OdGe::kSpunSurface:
            case OdGe::kSegmentChain2d:
            case OdGe::kRevolvedSurface:
            case OdGe::kRuled:
            default:
              ODA_FAIL_ONCE();
              break;
            }
          };

          intersector_wrapper.processIntersections(&process_curve);
        }
      }
    };

    if (!m_ClipPath.processFaces(&process_face))
      return;
  }
  else
  {
    default_save_path();
  }
}

void OdDbPdfImportObjectGrabber::saveSolid(const std::vector<BaseCurve>& paths, const ODCOLORREF cr_color,
  const bool is_object_visible, const wchar_t* cur_layer_name, const OdUInt8 alpha, const char* hyperlink)
{
  OdDbDatabasePtr pDb = OdDbDatabasePtr(m_Info->m_pProperties->get_Database());

  static const OdChar* layer_name = L"PDF_Solid Fills";
  static const OdCmTransparency transparency(0.5);

  OdCmColor color = toCmColor(cr_color);
  OdDbObjectId layerID = getLayer(layer_name, is_object_visible, cur_layer_name);
  OdDbLayerTableRecordPtr pLayer = layerID.openObject(OdDb::kForRead);
  if (eUseCurrentLayer == m_Info->m_pProperties->get_LayersUseType() && color == pLayer->color())
    color.setColorIndex(OdCmEntityColor::kACIbyLayer);

  std::function<OdDbHatchPtr()> create_hatch = [this, &color, &layerID, &cr_color, alpha, hyperlink]()
  {
    OdDbHatchPtr pHatch = OdDbHatch::createObject();
    pHatch->setDatabaseDefaults(m_pBTR->database());
    OdDbObjectId redHatchId = m_pBTR->appendOdDbEntity(pHatch);

    pHatch->setAssociative(false);
    pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("SOLID"));
    pHatch->setHatchStyle(OdDbHatch::kNormal);
    pHatch->setColor(color);
    if (eUseCurrentLayer != m_Info->m_pProperties->get_LayersUseType())
      m_LayerItems[layerID][cr_color].push_back(pHatch);
    pHatch->setTransparency(m_Info->m_pProperties->get_UsePdfTransparency()? OdCmTransparency(alpha): transparency);

    pHatch->setLayer(layerID);

    if (nullptr != hyperlink)
      addHyperlink(pHatch, hyperlink);
    return pHatch;
  };

  OdDbHatchPtr pHatch;

  std::function<OdDbSolidPtr()> create_solid = [this, &color, &layerID, &cr_color, alpha, hyperlink]()
  {
    OdDbSolidPtr pSolid = OdDbSolid::createObject();
    pSolid->setDatabaseDefaults(m_pBTR->database());
    m_pBTR->appendOdDbEntity(pSolid);

    pSolid->setColor(color);
    if (eUseCurrentLayer != m_Info->m_pProperties->get_LayersUseType())
      m_LayerItems[layerID][cr_color].push_back(pSolid);
    pSolid->setLayer(layerID);
    pSolid->setTransparency(m_Info->m_pProperties->get_UsePdfTransparency() ? OdCmTransparency(alpha) : transparency);

    if (nullptr != hyperlink)
      addHyperlink(pSolid, hyperlink);
    return pSolid;
  };

  std::function<void(const std::vector<BaseCurve>& paths)> default_save_solid = 
    [this, create_solid, create_hatch, &pHatch, &cr_color, &is_object_visible, &cur_layer_name, alpha, hyperlink](const std::vector<BaseCurve>& paths)
  {
    if (!m_Info->m_pProperties->get_ConvertSolidsToHatches() && 1 == paths.size() && paths[0].isPolyline() &&
      1 == paths[0].getSegments().size() && 6 > paths[0].getSegments()[0].getPointStorage().size() && 3 < paths[0].getSegments()[0].getPointStorage().size())
    {
      OdDbSolidPtr pSolid = create_solid();
      const BaseCurve::Segment& seg = paths[0].getSegments()[0];
      pSolid->setPointAt(0, OdGePoint3d(seg.getPointStorage()[0].x, seg.getPointStorage()[0].y, 0));
      pSolid->setPointAt(1, OdGePoint3d(seg.getPointStorage()[1].x, seg.getPointStorage()[1].y, 0));
      if (5 == seg.getPointStorage().size())
        pSolid->setPointAt(2, OdGePoint3d(seg.getPointStorage()[3].x, seg.getPointStorage()[3].y, 0));
      else
        pSolid->setPointAt(2, OdGePoint3d(seg.getPointStorage()[2].x, seg.getPointStorage()[2].y, 0));
      pSolid->setPointAt(3, OdGePoint3d(seg.getPointStorage()[2].x, seg.getPointStorage()[2].y, 0));
      pSolid->transformBy(m_TransforMatrix);
      return;
    }

    for (auto& fig : paths)
    {

      if (fig.isPolyline())
      {

        for (auto& seg : fig.getSegments())
        {
          OdGePoint2dArray  vertexPts(seg.getPointStorage());
          OdGeDoubleArray   vertexBulges;

          if (pHatch.isNull())
            pHatch = create_hatch();
          pHatch->appendLoop(OdDbHatch::kExternal | OdDbHatch::kPolyline,
            vertexPts, vertexBulges);
        }
      }
      else
      {
        EdgeArray edgePtrs;
        edgePtrs.clear();
        for (auto& seg : fig.getSegments())
        {
          switch (seg.getPointsType())
          {
          case OdPdfImportPtType::OdLineTo:
          {
            for (OdUInt32 i = 0; i < seg.getPointStorage().size() - 1; ++i)
            {
              OdGeLineSeg2d* poly = new OdGeLineSeg2d(seg.getPointStorage()[i], seg.getPointStorage()[i + 1]);
              edgePtrs.append(poly);
            }
          }
          break;
          case OdPdfImportPtType::OdBezierTo:
          {
            OdGeKnotVector knotsVec;
            OdGeDoubleArray wts;

            fillKnots(seg.getPointStorage().size(), knotsVec, wts);

            OdGeNurbCurve2d* spline = new OdGeNurbCurve2d();

            spline->set(3, knotsVec, seg.getPointStorage(), wts, false);

            edgePtrs.push_back(spline);
          }
          break;
          default:
            break;
          }
        }
        if (0 != edgePtrs.size())
        {
          try
          {
            if (pHatch.isNull())
              pHatch = create_hatch();
            pHatch->appendLoop(OdDbHatch::kDefault, edgePtrs);
          }
          catch (...)
          {
            savePath(paths, cr_color, nullptr, is_object_visible, cur_layer_name, alpha, nullptr, hyperlink);
          }

          edgePtrs.clear();
        }
      }
    }
    if (!pHatch.isNull())
    {
      double area(0.0);
      try
      {
        pHatch->getArea(area);
      }
      catch (...)
      {

      }
      if (OdEqual(area, 0.0))
        savePath(paths, cr_color, nullptr, is_object_visible, cur_layer_name, alpha, nullptr, hyperlink);

      pHatch->transformBy(m_TransforMatrix);
    }
  };

  if (m_Info->m_pProperties->get_UseClipping() && !m_ClipPath.isNull())
  {
    OdArray< OdArray<OdGeCurve2d*> > loops;
    fillLoops(paths, loops, true);

    MdFaceWrapper solid_hatch;

    try
    {
      solid_hatch = createPlanarOdMdBodyFromCurves(loops);
      OdGeExtents3d ext;
      
      if(nullptr != solid_hatch)
        ext = solid_hatch.getGeomExtents();

      if (!ext.isValidExtents() || OdEqual(ext.minPoint().x, ext.maxPoint().x) || OdEqual(ext.minPoint().y, ext.maxPoint().y))
      {
        savePath(paths, cr_color, nullptr, is_object_visible, cur_layer_name, alpha, nullptr, hyperlink);
        return;
      }
    }
    catch (...)
    {

    }

    if (!solid_hatch.isNull())
    {

      void* result(nullptr);
      if (booleanOperationForMdBody(OdGe::kCommon, m_ClipPath, false, solid_hatch.detach(), true, result))
        solid_hatch = result;
    }

    if (solid_hatch.isNull())
    {
      const std::function<bool(const OdGePoint2d&)>& check_point_func = [this](const OdGePoint2d& point)
      {
        return OdGe::kOutside != m_ClipPath.checkPointLocation(OdGePoint3d(point.x, point.y, 0.));
      };

      std::vector<BaseCurve> clipped_paths = paths;
      cutPath(clipped_paths, check_point_func);
      default_save_solid(clipped_paths);
      return;
    }

    std::function<void(OdArray<OdGeCurve2d*>& edgePtrs, const bool is_line_segs, unsigned long loops_count)> process_loops =
      [&](OdArray<OdGeCurve2d*>& edgePtrs, const bool is_line_segs, unsigned long loops_count)
    {
      if (!m_Info->m_pProperties->get_ConvertSolidsToHatches() && 1 == loops_count && is_line_segs && 5 > edgePtrs.size() && 2 < edgePtrs.size())
      {
        OdDbSolidPtr pSolid = create_solid();
        pSolid->setPointAt(0, OdGePoint3d(static_cast<OdGeLineSeg2d*>(edgePtrs[0])->startPoint().x, static_cast<OdGeLineSeg2d*>(edgePtrs[0])->startPoint().y, 0));
        pSolid->setPointAt(1, OdGePoint3d(static_cast<OdGeLineSeg2d*>(edgePtrs[0])->endPoint().x, static_cast<OdGeLineSeg2d*>(edgePtrs[0])->endPoint().y, 0));
        if (4 == edgePtrs.size())
          pSolid->setPointAt(2, OdGePoint3d(static_cast<OdGeLineSeg2d*>(edgePtrs[2])->endPoint().x, static_cast<OdGeLineSeg2d*>(edgePtrs[2])->endPoint().y, 0));
        else
          pSolid->setPointAt(2, OdGePoint3d(static_cast<OdGeLineSeg2d*>(edgePtrs[1])->endPoint().x, static_cast<OdGeLineSeg2d*>(edgePtrs[1])->endPoint().y, 0));
        pSolid->setPointAt(3, OdGePoint3d(static_cast<OdGeLineSeg2d*>(edgePtrs[1])->endPoint().x, static_cast<OdGeLineSeg2d*>(edgePtrs[1])->endPoint().y, 0));
        pSolid->transformBy(m_TransforMatrix);
        return;
      }
      if (pHatch.isNull())
        pHatch = create_hatch();
      try
      {

        pHatch->appendLoop(OdDbHatch::kDefault, edgePtrs);
      }
      catch (...)
      {
        savePath(paths, cr_color, nullptr, is_object_visible, cur_layer_name, alpha, nullptr, hyperlink);
      }
    };

    if (!solid_hatch.processLoops(&process_loops))
    {
      savePath(paths, cr_color, nullptr, is_object_visible, cur_layer_name, alpha, nullptr, hyperlink);
      return;
    }

    if (!pHatch.isNull())
    {
      double area(0.0);
      try
      {
        pHatch->getArea(area);
      }
      catch (...)
      {

      }
      if (OdEqual(area, 0.0))
        savePath(paths, cr_color, nullptr, is_object_visible, cur_layer_name, alpha, nullptr, hyperlink);
      pHatch->transformBy(m_TransforMatrix);
    }
  }
  else
  {
    default_save_solid(paths);
  }

  return;
}

OdDbObjectId OdDbPdfImportObjectGrabber::getLayer(const OdChar* name, const bool is_object_visible, const wchar_t* cur_layer_name)
{
  OdDbDatabasePtr pDb = OdDbDatabasePtr(m_Info->m_pProperties->get_Database());
  OdDbLayerTablePtr pLayers = pDb->getLayerTableId().safeOpenObject(OdDb::kForWrite);
  OdDbObjectId layer_id;

  std::function<void()> createObjectLayers = [&]()
  {
    OdString layer_name = is_object_visible ? OdString(name) : (OdString(name) + OdString(L"_Invisible"));
    layer_id = pLayers->getAt(layer_name);
    if (layer_id.isNull())
    {
      OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::createObject();
      pLayer->setName(layer_name);
      layer_id = pLayers->add(pLayer);
      pLayer->setIsOff(!is_object_visible);
    }
  };

  OdString od_cur_layer_name(cur_layer_name);
  ReplaceForbiddenChars(od_cur_layer_name);

  switch (m_Info->m_pProperties->get_LayersUseType())
  {
  case eUsePdfLayers: // Use PDF Layers
  {
    OdString layer_name = od_cur_layer_name.isEmpty() ? OdString(name): (OdString(L"PDF_") + od_cur_layer_name);
    layer_id = pLayers->getAt(layer_name);
    if (layer_id.isNull())
    {
      OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::createObject();
      pLayer->setName(layer_name);
      layer_id = pLayers->add(pLayer);
      bool state = true;
      std::map<std::wstring,bool>::const_iterator iter = m_LayersInfo.find(cur_layer_name);
      if (iter != m_LayersInfo.end())
        state = iter->second;
      pLayer->setIsOff(!state);
    }
  }
  break;
  case eCreateObjectLayers: // Create object layers
  {
    createObjectLayers();
  }
  break;
  case eUseCurrentLayer: // Use current layer
    if (is_object_visible)
      layer_id = pDb->getCLAYER();
    else
      createObjectLayers();
    break;
  default:
    break;
  }
  return layer_id;
}



