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
// Point Cloud test object and Point Cloud test object command

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbEntity.h"
#include "DbFiler.h"
#include "DbProxyEntity.h"
#include "DbBlockTableRecord.h"
#include "DbCommandContext.h"
#include "Ed/EdCommandStack.h"
#include "Ge/GeScale3d.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiViewport.h"
#if 1
#include "Gi/GiPointCloud.h"

/* -------------------------------------------------------------------------------------- */
/* Abstraction layer for communication with database entity                               */
/* -------------------------------------------------------------------------------------- */
class ExPointCloudCom
{
  public:
    virtual OdUInt32 density() const = 0;
    virtual OdUInt32 componentsMask() const = 0;
};

/* -------------------------------------------------------------------------------------- */
/* Implementation of Point Cloud testing data                                             */
/* -------------------------------------------------------------------------------------- */

class ExPointCloudImpl : public OdStaticRxObject<OdGiPointCloud>
{
  protected:
    const ExPointCloudCom *m_pCom;
  protected:
    // Box encoding: -X, +X, -Y, +Y, -Z, +Z
    static const struct BoxEncoding { int x, y, z; } boxEncoding[6];
    OdUInt32 computeVisibleSides(const OdGeVector3d &viewDir) const
    { OdUInt32 rVal = 0;
      for (OdUInt32 nSide = 0; nSide < 6; nSide++)
      {
        if (OdNegative(OdGeVector3d(1.0 * boxEncoding[nSide].x, 1.0 * boxEncoding[nSide].y, 1.0 * boxEncoding[nSide].z).dotProduct(viewDir)))
          rVal |= 1 << nSide;
      }
      return rVal;
    }
    OdGeVector3d modelDirection(const OdGiViewport &pVp) const
    {
      return pVp.viewDir().transformBy(pVp.getEyeToModelTransform() * pVp.getWorldToEyeTransform());
    }
  public:
    ExPointCloudImpl(const ExPointCloudCom *pCom = NULL)
      : m_pCom(pCom) { }
    void setupCom(const ExPointCloudCom *pCom) { m_pCom = pCom; }
    // OdGiPointCloud interface implementation
    virtual OdUInt32 totalPointsCount() const
    {
      return m_pCom->density() * m_pCom->density() * 6;
    }
    virtual OdUInt32 componentsMask() const
    {
      return m_pCom->componentsMask();
    }
    virtual bool getExtents(OdGeBoundBlock3d &bb) const
    {
      bb.set(OdGePoint3d(-0.5, -0.5, -0.5), OdGePoint3d(0.5, 0.5, 0.5));
      return true;
    }
    virtual bool isDataCompatible(const OdGiViewport &pVp1, const OdGiViewport &pVp2) const
    {
      return computeVisibleSides(modelDirection(pVp1)) == computeVisibleSides(modelDirection(pVp2));
    }
    virtual bool updatePointsData(OdGiPointCloudReceiver *pReceiver, OdUInt32 components, const OdGiViewport *pVp, OdUInt32 /*pointSize*/) const
    {
      const OdUInt32 sideMask = (pVp) ? computeVisibleSides(modelDirection(*pVp)) : 0x3F;
      const OdUInt32 density = m_pCom->density();
      OdGiPointCloud::Components genComps;
      // Generate points
      genComps.m_points.resize(density * density);
      if (GETBIT(components, OdGiPointCloud::kColors))
        genComps.m_colors.resize(density * density);
      if (GETBIT(components, OdGiPointCloud::kNormals))
        genComps.m_normals.resize(density * density);
      for (OdUInt32 nSide = 0; nSide < 6; nSide++)
      {
        if (GETBIT(sideMask, 1 << nSide))
        {
          double *pCoordA = (!boxEncoding[nSide].x) ? &genComps.m_points.first().x : &genComps.m_points.first().y;
          double *pCoordB = (!boxEncoding[nSide].z) ? &genComps.m_points.first().z : &genComps.m_points.first().y;
          double *pCoord0 = (boxEncoding[nSide].x) ? &genComps.m_points.first().x : ((boxEncoding[nSide].y) ? &genComps.m_points.first().y : &genComps.m_points.first().z);
          OdCmEntityColor *pColors = genComps.m_colors.asArrayPtr();
          OdGeVector3d *pNormals = genComps.m_normals.asArrayPtr();
          const double offset = 0.5 * (boxEncoding[nSide].x + boxEncoding[nSide].y + boxEncoding[nSide].z);
          const OdCmEntityColor color(OdUInt8(255 * ((nSide + 1) & 1)), OdUInt8(255 * (((nSide + 1) & 2) >> 1)), OdUInt8(255 * (((nSide + 1) & 4) >> 2)));
          const OdGeVector3d normal(1.0 * boxEncoding[nSide].x, 1.0 * boxEncoding[nSide].y, 1.0 * boxEncoding[nSide].z);
          const double delta = 1.0 / density;
          for (OdUInt32 a = 0; a < density; a++)
          {
            const double coordA = -0.5 + delta * a;
            for (OdUInt32 b = 0; b < density; b++, pCoordA += 3, pCoordB += 3, pCoord0 += 3)
            {
              *pCoordA = coordA; *pCoordB = -0.5 + delta * b; *pCoord0 = offset;
              if (pColors) *pColors++ = color;
              if (pNormals) *pNormals++ = normal;
            }
          }
          if (!pReceiver->addPoints(OdGiPointCloud::ComponentsRaw().fromComponents(genComps), density * density, nSide))
            return true;
        }
      }
      return true;
    }
};
const ExPointCloudImpl::BoxEncoding ExPointCloudImpl::boxEncoding[6] = 
{
  { -1, 0, 0 }, { 1, 0, 0 }, { 0, -1, 0 }, { 0, 1, 0 }, { 0, 0, -1 }, { 0, 0, 1 }
};

/* -------------------------------------------------------------------------------------- */
/* ExPointCloud custom entity                                                             */
/* -------------------------------------------------------------------------------------- */

class ExPointCloud : public OdDbEntity, protected ExPointCloudCom
{
  protected:
    ExPointCloudImpl m_impl;
  protected:
    OdGePoint3d m_position;
    OdGeScale3d m_scale;
    OdGeVector3d m_rotate;
    OdUInt32 m_density;
    OdUInt32 m_components;
  protected:
    OdUInt32 componentsMask() const
    { //assertReadEnabled();
      return m_components; }
  public:
    ODDB_DECLARE_MEMBERS(ExPointCloud);

    ExPointCloud()
      : m_density(100)
      , m_components(OdGiPointCloud::kColors | OdGiPointCloud::kNormals)
    { m_impl.setupCom(this); }
    ~ExPointCloud()
    { }

    const OdGePoint3d &position() const
    {
      assertReadEnabled();
      return m_position;
    }
    void setPosition(const OdGePoint3d &pos)
    {
      assertWriteEnabled();
      m_position = pos;
    }
    const OdGeScale3d &scale() const
    {
      assertReadEnabled();
      return m_scale;
    }
    void setScale(const OdGeScale3d &scale)
    {
      assertWriteEnabled();
      m_scale = scale;
    }
    const OdGeVector3d &rotate() const
    {
      assertReadEnabled();
      return m_rotate;
    }
    void setRotate(const OdGeVector3d &rot)
    {
      assertWriteEnabled();
      m_rotate = rot;
    }
    OdUInt32 density() const
    {
      //assertReadEnabled();
      return m_density;
    }
    void setDensity(OdUInt32 density)
    {
      if (density < 1)
        throw OdError(eOutOfRange);
      assertWriteEnabled();
      m_density = density;
    }

    bool colorsEnabled() const
    {
      assertReadEnabled();
      return GETBIT(m_components, OdGiPointCloud::kColors);
    }
    void enableColors(bool bEnable)
    {
      assertWriteEnabled();
      SETBIT(m_components, OdGiPointCloud::kColors, bEnable);
    }
    bool normalsEnabled() const
    {
      assertReadEnabled();
      return GETBIT(m_components, OdGiPointCloud::kNormals);
    }
    void enableNormals(bool bEnable)
    {
      assertWriteEnabled();
      SETBIT(m_components, OdGiPointCloud::kNormals, bEnable);
    }

    OdGeMatrix3d getTransform() const
    {
      assertReadEnabled();
      return OdGeMatrix3d::translation(m_position.asVector()) *
             OdGeMatrix3d::rotation(m_rotate.x, OdGeVector3d::kXAxis) *
             OdGeMatrix3d::rotation(m_rotate.y, OdGeVector3d::kYAxis) *
             OdGeMatrix3d::rotation(m_rotate.z, OdGeVector3d::kZAxis) *
             OdGeMatrix3d::scaling(m_scale);
    }
    void setTransform(const OdGeMatrix3d &tm)
    {
      assertWriteEnabled();
      OdGeVector3d xAxis, yAxis, zAxis; OdGePoint3d origin;
      tm.getCoordSystem(origin, xAxis, yAxis, zAxis);
      m_position = origin;
      m_scale.set(xAxis.normalizeGetLength(), yAxis.normalizeGetLength(), zAxis.normalizeGetLength());
      m_rotate.set(OD_ATAN2(yAxis.z, zAxis.z), -OD_ASIN(xAxis.z), OD_ATAN2(xAxis.y, xAxis.x));
    }
    OdResult subTransformBy(const OdGeMatrix3d& xform)
    {
      setTransform(xform * getTransform());
      return eOk;
    }

    OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const
    {
      OdUInt32 rFlags = OdDbEntity::subSetAttributes(pTraits);
      // rFlags |= OdGiDrawable::kDrawableRegenDraw; // Enable permanent entity regeneration
      return rFlags;
    }
    bool subWorldDraw(OdGiWorldDraw* pWd) const
    {
      const OdGeMatrix3d tx = getTransform() * OdGeMatrix3d::rotation(OdaToRadian(90.0), OdGeVector3d::kXAxis)
                                             * OdGeMatrix3d::rotation(OdaToRadian(90.0), OdGeVector3d::kYAxis);
      pWd->geometry().pushModelTransform(tx);
      pWd->geometry().pointCloud(m_impl);
      pWd->geometry().popModelTransform();
      return true;
    }

    OdResult dwgInFields(OdDbDwgFiler* pFiler)
    {
      OdResult res = OdDbEntity::dwgInFields(pFiler);
      if (res != eOk)
        return res;
      m_position = pFiler->rdPoint3d();
      m_scale = pFiler->rdScale3d();
      m_rotate = pFiler->rdVector3d();
      m_density = (OdUInt32)pFiler->rdInt32();
      m_components = (OdUInt32)pFiler->rdInt32();
      return eOk;
    }
    void dwgOutFields(OdDbDwgFiler* pFiler) const
    {
      OdDbEntity::dwgOutFields(pFiler);
      pFiler->wrPoint3d(m_position);
      pFiler->wrScale3d(m_scale);
      pFiler->wrVector3d(m_rotate);
      pFiler->wrInt32((OdInt32)m_density);
      pFiler->wrInt32((OdInt32)m_components);
    }
    OdResult dxfInFields(OdDbDxfFiler* pFiler)
    {
      OdResult res = OdDbEntity::dxfInFields(pFiler);
      if (res != eOk)
        return res;
      if (!pFiler->atSubclassData(desc()->name()))
        return eBadDxfSequence;
      m_position = pFiler->nextRb()->getPoint3d();
      m_scale.sx = pFiler->nextRb()->getDouble();
      m_scale.sy = pFiler->nextRb()->getDouble();
      m_scale.sz = pFiler->nextRb()->getDouble();
      m_rotate.x = (pFiler->nextRb(), pFiler->rdAngle());
      m_rotate.y = (pFiler->nextRb(), pFiler->rdAngle());
      m_rotate.z = (pFiler->nextRb(), pFiler->rdAngle());
      m_density = (OdUInt32)pFiler->nextRb()->getInt32();
      m_components = (OdUInt32)pFiler->nextRb()->getInt32();
      return eOk;
    }
    void dxfOutFields(OdDbDxfFiler* pFiler) const
    {
      OdDbEntity::dxfOutFields(pFiler);
      pFiler->wrSubclassMarker(desc()->name());
      pFiler->wrPoint3d(10, m_position);
      pFiler->wrDouble(40, m_scale.sx);
      pFiler->wrDouble(41, m_scale.sy);
      pFiler->wrDouble(42, m_scale.sz);
      pFiler->wrAngle(50, m_rotate.x);
      pFiler->wrAngle(51, m_rotate.y);
      pFiler->wrAngle(53, m_rotate.z);
      pFiler->wrInt32(90, m_density);
      pFiler->wrInt32(91, m_components);
    }
};

ODRX_DXF_DEFINE_MEMBERS(ExPointCloud,
                        OdDbEntity,
                        DBOBJECT_CONSTR,
                        OdDb::vAC24,
                        OdDb::kMRelease0,
                        OdDbProxyEntity::kAllAllowedBits,
                        ExPointCloud,
                        OdaMfcApp|Description: Point Cloud provider example)

class OdExPointCloudCmd : public OdEdCommand
{
  public:
    const OdString groupName() const
    {
      return OD_T("ODAMFCAPP");
    }
    const OdString globalName() const
    {
      return OD_T("ExPointCloud");
    }
    void execute(OdEdCommandContext* pCmdCtx)
    {
      OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
      OdDbDatabasePtr pDb = pDbCmdCtx->database();
      OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
      OdDbBlockTableRecordPtr pBTR = pDb->getActiveLayoutBTRId().openObject(OdDb::kForWrite);
      OdSmartPtr<ExPointCloud> pPointCloud = ExPointCloud::createObject();
      pPointCloud->setDatabaseDefaults(pDb);

      pPointCloud->setPosition(pIO->getPoint(OD_T("\nSpecify center of Point Cloud: ")));
      pPointCloud->setScale(OdGeScale3d(pIO->getDist(OD_T("\nSpecify scale of Point Cloud: "))));
      pPointCloud->setRotate(OdGeVector3d(0.0, 0.0, pIO->getAngle(OD_T("\nSpecify rotation angle around z-axis: "))));

      pBTR->appendOdDbEntity(pPointCloud);
    }
};
static OdStaticRxObject<OdExPointCloudCmd> g_OdExPointCloudCmd;

// Export "extern" functions

void rxInitExPointCloud()
{
  ExPointCloud::rxInit();
  odedRegCmds()->addCommand(&g_OdExPointCloudCmd);
}

void rxUninitExPointCloud()
{
  odedRegCmds()->removeCmd(&g_OdExPointCloudCmd);
  ExPointCloud::rxUninit();
}

#else

void rxInitExPointCloud() { }
void rxUninitExPointCloud() { }

#endif

//
