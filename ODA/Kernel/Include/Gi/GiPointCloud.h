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

#ifndef __ODGIPOINTCLOUD_H__
#define __ODGIPOINTCLOUD_H__

#include "Gi/GiViewport.h"
#include "Ge/GePoint3dArray.h"
#include "Ge/GeVector3dArray.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeBoundBlock3d.h"
#include "CmEntityColorArray.h"
#include "CmTransparencyArray.h"
#include "DoubleArray.h"
#include "UInt32Array.h"

#include "TD_PackPush.h"

#include "Gi/GiExport.h"

// Forward declarations
class OdGiPointCloudFilter;
typedef OdSmartPtr<OdGiPointCloudFilter> OdGiPointCloudFilterPtr;
class OdGiPointCloudReceiver;
typedef OdSmartPtr<OdGiPointCloudReceiver> OdGiPointCloudReceiverPtr;

/** \details
  Basic point cloud interface object.

  <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiPointCloud : public OdRxObject
{
  public:
    enum Component
    { kNoComponents   = 0,
      kColors         = (1 << 0),
      kTransparencies = (1 << 1),
      kNormals        = (1 << 2)
    };
    struct ComponentData
    {
      OdGePoint3d m_point;
      OdCmEntityColor m_color;
      OdCmTransparency m_transparency;
      OdGeVector3d m_normal;
    };
    struct Components
    {
      OdGePoint3dArray m_points;
      OdCmEntityColorArray m_colors;
      OdCmTransparencyArray m_transparencies;
      OdGeVector3dArray m_normals;
    };
    struct ComponentsRaw
    {
      const OdGePoint3d *m_pPoints;
      const OdCmEntityColor *m_pColors;
      const OdCmTransparency *m_pTransparencies;
      const OdGeVector3d *m_pNormals;
      ComponentsRaw &fromComponents(Components &comps)
      { m_pPoints = comps.m_points.asArrayPtr();
        m_pColors = comps.m_colors.asArrayPtr();
        m_pTransparencies = comps.m_transparencies.asArrayPtr();
        m_pNormals = comps.m_normals.asArrayPtr();
        return *this; }
    };
  public:
    ODRX_DECLARE_MEMBERS(OdGiPointCloud);

    /** \details
      Returns total number of points, contained by point cloud entity.
    */
    virtual OdUInt32 totalPointsCount() const = 0;
    /** \details
      Returns set of data components available for point cloud entity.
    */
    virtual OdUInt32 componentsMask() const { return kNoComponents; }
    /** \details
      Returns global transformation, which should be applied by underlying renderer to render point could data.
    */
    virtual const OdGeMatrix3d &globalTransform() const { return OdGeMatrix3d::kIdentity; }
    /** \details
      Returns default point size.
    */
    virtual OdInt32 defaultPointSize() const { return 0; }

    /** \details
      Return extents of point cloud entity.
    */
    virtual bool getExtents(OdGeBoundBlock3d & /*bb*/) const { return false; }
    /** \details
      Return extents of point cloud entity.
    */
    virtual bool calculateExtents(OdGeExtents3d &extents, const OdGiPointCloudFilter *pFilter = NULL) const;

    /** \details
      Check that point data compatible for specified viewports.
    */
    virtual bool isDataCompatible(const OdGiViewport &pVp1, const OdGiViewport &pVp2) const = 0;
    /** \details
      Compute point cloud data.
    */
    virtual bool updatePointsData(OdGiPointCloudReceiver *pReceiver, OdUInt32 components = kNoComponents,
                                  const OdGiViewport *pVp = NULL, OdUInt32 pointSize = 0) const = 0;
};

typedef OdSmartPtr<OdGiPointCloud> OdGiPointCloudPtr;

/** \details
  Point cloud filter.

  <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiPointCloudFilter : public OdRxObject
{
  protected:
    OdGiPointCloudFilterPtr m_pPrevFilter;
  public:
    ODRX_DECLARE_MEMBERS(OdGiPointCloudFilter);

    void attachFilter(const OdGiPointCloudFilter *pFilter)
    { m_pPrevFilter = pFilter; }
    OdGiPointCloudFilterPtr detachFilter()
    { OdGiPointCloudFilterPtr pFilter = m_pPrevFilter; m_pPrevFilter.release(); return pFilter; }
  protected:
    virtual bool filterPointsImpl(OdGiPointCloud::ComponentsRaw &points, OdUInt32 &nPoints) const = 0;
    virtual bool filterBoundingBoxImpl(OdGeBoundBlock3d &bb) const = 0;
    virtual void extractTransformImpl(OdGeMatrix3d & /*xForm*/) const { }
  public:
    bool filterPoints(OdGiPointCloud::ComponentsRaw &points, OdUInt32 &nPoints) const
    { if (m_pPrevFilter.isNull() || m_pPrevFilter->filterPoints(points, nPoints))
        return filterPointsImpl(points, nPoints);
      return false;
    }
    bool filterBoundingBox(OdGeBoundBlock3d &bb) const
    { if (m_pPrevFilter.isNull() || m_pPrevFilter->filterBoundingBox(bb))
        return filterBoundingBoxImpl(bb);
      return false;
    }
    OdGeMatrix3d extractTransform() const
    { if (!m_pPrevFilter.isNull())
      { OdGeMatrix3d xForm = m_pPrevFilter->extractTransform();
        extractTransformImpl(xForm);
        return xForm;
      } else {
        OdGeMatrix3d xForm; extractTransformImpl(xForm); return xForm;
      }
    }
};

/** \details
  Default implementation of point cloud data transformation filter.

  <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiPointCloudXformFilter : public OdGiPointCloudFilter
{
  protected:
    mutable OdGiPointCloud::Components m_comps;
    OdGeMatrix3d m_xForm;
  public:
    ODRX_DECLARE_MEMBERS(OdGiPointCloudXformFilter);
  protected:
    virtual bool filterPointsImpl(OdGiPointCloud::ComponentsRaw &points, OdUInt32 &nPoints) const
    {
      m_comps.m_points.resize(nPoints);
      OdGePoint3d *pOutPoints = m_comps.m_points.asArrayPtr();
      OdGeVector3d *pOutNormals = NULL;
      if (points.m_pNormals) { m_comps.m_normals.resize(nPoints); pOutNormals = m_comps.m_normals.asArrayPtr(); }
      for (OdUInt32 nPoint = 0; nPoint < nPoints; nPoint++)
      {
        pOutPoints[nPoint] = points.m_pPoints[nPoint];
        pOutPoints[nPoint].transformBy(m_xForm);
        if (pOutNormals)
          pOutNormals[nPoint] = points.m_pNormals[nPoint],
          pOutNormals[nPoint].transformBy(m_xForm);
      }
      points.m_pPoints = pOutPoints; points.m_pNormals = pOutNormals;
      return true;
    }
    virtual bool filterBoundingBoxImpl(OdGeBoundBlock3d &bb) const
    {
      bb.transformBy(m_xForm);
      return true;
    }
    virtual void extractTransformImpl(OdGeMatrix3d &xForm) const
    { xForm.preMultBy(m_xForm); }
  public:
    void setXform(const OdGeMatrix3d &xForm) { m_xForm = xForm; }
    void addXform(const OdGeMatrix3d &xForm) { m_xForm.preMultBy(xForm); }
    const OdGeMatrix3d &getXform() const { return m_xForm; }
    void resetXform() { m_xForm.setToIdentity(); }
    bool hasXform() const { return m_xForm != OdGeMatrix3d::kIdentity; }
    OdGeMatrix3d &accessXform() { return m_xForm; }

    static OdGiPointCloudFilterPtr createObject(const OdGeMatrix3d &xForm, const OdGiPointCloudFilter *pPrevFilter = NULL)
    { if (pPrevFilter && pPrevFilter->isA() == desc())
      { // For optimization purposes we can reuse exist transformation filter
       static_cast<OdGiPointCloudXformFilter*>(OdGiPointCloudFilterPtr(pPrevFilter).get())->addXform(xForm);
        return pPrevFilter;
      }
      OdSmartPtr<OdGiPointCloudXformFilter> pObj = createObject();
      pObj->attachFilter(pPrevFilter);
      pObj->setXform(xForm);
      return pObj;
    }
};

/** \details
  Point cloud data receiver.

  <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiPointCloudReceiver : public OdRxObject
{
  protected:
    OdGiPointCloudFilterPtr m_pFilter;
  public:
    ODRX_DECLARE_MEMBERS(OdGiPointCloudReceiver);

  protected:
    void attachFilter(const OdGiPointCloudFilter *pFilter)
    { m_pFilter = pFilter; }
    OdGiPointCloudFilterPtr detachFilter()
    { OdGiPointCloudFilterPtr pFilter = m_pFilter; m_pFilter.release(); return pFilter; }

    virtual bool addPointsImpl(const OdGiPointCloud::ComponentsRaw &points, OdUInt32 nPoints, OdUInt32 nCellId) = 0;
  public:
    bool addPoints(const OdGiPointCloud::ComponentsRaw &points, OdUInt32 nPoints, OdUInt32 nCellId = 0xFFFFFFFF)
    { OdGiPointCloud::ComponentsRaw pointsCopy(points);
      if (m_pFilter.isNull() || m_pFilter->filterPoints(pointsCopy, nPoints))
        return addPointsImpl(pointsCopy, nPoints, nCellId);
      return true;
    }
};

/** \details
  Default implementation of point cloud data receiver for extents computation.

  <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiPointCloudExtentsReceiver : public OdGiPointCloudReceiver
{
  protected:
    OdGeExtents3d m_extents;
  public:
    ODRX_DECLARE_MEMBERS(OdGiPointCloudExtentsReceiver);

  protected:
    virtual bool addPointsImpl(const OdGiPointCloud::ComponentsRaw &points, OdUInt32 nPoints, OdUInt32 /*nCellId*/)
    {
      for (OdUInt32 nPoint = 0; nPoint < nPoints; nPoint++)
        m_extents.addPoint(points.m_pPoints[nPoint]);
      return true;
    }
  public:
    static OdGiPointCloudReceiverPtr createObject(const OdGiPointCloudFilter *pFilter)
    { OdSmartPtr<OdGiPointCloudExtentsReceiver> pObj = createObject();
      pObj->attachFilter(pFilter);
      return pObj;
    }

    const OdGeExtents3d &getExtents() const { return m_extents; }
    void resetExtents() { m_extents = OdGeExtents3d::kInvalid; }
};

inline bool OdGiPointCloud::calculateExtents(OdGeExtents3d &extents, const OdGiPointCloudFilter *pFilter) const
{ OdSmartPtr<OdGiPointCloudExtentsReceiver> pExtentsGrabber = OdGiPointCloudExtentsReceiver::createObject(pFilter);
  if (updatePointsData(pExtentsGrabber.get()))
    extents.addExt(pExtentsGrabber->getExtents());
  return extents.isValidExtents();
}

#include "TD_PackPop.h"

#endif  // __ODGIPOINTCLOUD_H__
