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
// Orthogonal basis definition

#ifndef ODTRVISBASIS
#define ODTRVISBASIS

#include "TD_PackPush.h"

#include "TrVisDefs.h"

/** \details
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
class OdTrVisOrthoBasis
{
  protected:
    OdGeVector3d m_axis[3];
  public:
    enum Axis
    { kX = 0, kY = 1, kZ = 2 };
  protected:
    template <typename MtxType>
    static OD_CONSTEXPR bool is4x4Matrix(const MtxType &xMat) { return sizeof(MtxType) > sizeof(OD_TYPENAME MtxType::EntryType) * 9; }
    static void axisNormalize(OdGeVector3d &axis, const OdGeVector3d &defAxis)
    { OdGe::ErrorCondition error = OdGe::kOk;
      axis.normalize(OdGeTol(1.e-300), error);
      if (error != OdGe::kOk)
        axis = defAxis;
    }
  public:
    OdTrVisOrthoBasis() { }

    // Setup default WCS basis
    OdTrVisOrthoBasis &setDefault()
    {
      m_axis[kX] = OdGeVector3d::kXAxis;
      m_axis[kY] = OdGeVector3d::kYAxis;
      m_axis[kZ] = OdGeVector3d::kZAxis;
      return *this;
    }
    // Setup from input vectors
    template <typename VecType>
    OdTrVisOrthoBasis &set(const VecType &xAxis, const VecType &yAxis, const VecType &zAxis)
    {
      m_axis[kX].set(xAxis[0], xAxis[1], xAxis[2]);
      m_axis[kY].set(yAxis[0], yAxis[1], yAxis[2]);
      m_axis[kZ].set(zAxis[0], zAxis[1], zAxis[2]);
      return *this;
    }
    // Setup from Ge matrix
    OdTrVisOrthoBasis &set(const OdGeMatrix3d &xMat)
    {
      m_axis[kX] = xMat.getCsXAxis();
      m_axis[kY] = xMat.getCsYAxis();
      m_axis[kZ] = xMat.getCsZAxis();
      return *this;
    }
    // Setup from any type of OdTr matrix
    template <typename MtxType>
    OdTrVisOrthoBasis &set(const MtxType &xMat)
    {
      m_axis[kX].set(xMat.entry[0][0], xMat.entry[1][0], xMat.entry[2][0]);
      m_axis[kY].set(xMat.entry[0][1], xMat.entry[1][1], xMat.entry[2][1]);
      m_axis[kZ].set(xMat.entry[0][2], xMat.entry[1][2], xMat.entry[2][2]);
      return *this;
    }

    // Local setters
    OdTrVisOrthoBasis &setXAxis(const OdGeVector3d &axis) { m_axis[kX] = axis; return *this; }
    OdTrVisOrthoBasis &setYAxis(const OdGeVector3d &axis) { m_axis[kY] = axis; return *this; }
    OdTrVisOrthoBasis &setZAxis(const OdGeVector3d &axis) { m_axis[kZ] = axis; return *this; }

    OdTrVisOrthoBasis &setAxis(Axis nAxis, const OdGeVector3d &axis) { m_axis[nAxis] = axis; return *this; }

    // Getters
    const OdGeVector3d &xAxis() const { return m_axis[kX]; }
    const OdGeVector3d &yAxis() const { return m_axis[kY]; }
    const OdGeVector3d &zAxis() const { return m_axis[kZ]; }

    const OdGeVector3d &getAxis(Axis axis) { return m_axis[axis]; }

    // Get as Ge matrix
    OdGeMatrix3d &get(OdGeMatrix3d &xMat, const OdGePoint3d &csOrigin = OdGePoint3d::kOrigin) const
    {
      xMat.setCoordSystem(csOrigin, m_axis[kX], m_axis[kY], m_axis[kZ]);
      return xMat;
    }
    // Get as any OdTr matrix
    template <typename MtxType>
    MtxType &get(MtxType &xMat) const
    {
      xMat.entry[0][0] = (OD_TYPENAME MtxType::EntryType)m_axis[kX].x;
      xMat.entry[1][0] = (OD_TYPENAME MtxType::EntryType)m_axis[kX].y;
      xMat.entry[2][0] = (OD_TYPENAME MtxType::EntryType)m_axis[kX].z;
      xMat.entry[0][1] = (OD_TYPENAME MtxType::EntryType)m_axis[kY].x;
      xMat.entry[1][1] = (OD_TYPENAME MtxType::EntryType)m_axis[kY].y;
      xMat.entry[2][1] = (OD_TYPENAME MtxType::EntryType)m_axis[kY].z;
      xMat.entry[0][2] = (OD_TYPENAME MtxType::EntryType)m_axis[kZ].x;
      xMat.entry[1][2] = (OD_TYPENAME MtxType::EntryType)m_axis[kZ].y;
      xMat.entry[2][2] = (OD_TYPENAME MtxType::EntryType)m_axis[kZ].z;
      if (is4x4Matrix(xMat))
        *(xMat.entry[0] + 3) = *(xMat.entry[1] + 3) = *(xMat.entry[2] + 3) =
        *(xMat.entry[2] + 4) = *(xMat.entry[2] + 5) = *(xMat.entry[2] + 6) = 0.0f, *(xMat.entry[2] + 7) = 1.0f;
      return xMat;
    }

    // Comparision
    bool operator ==(const OdTrVisOrthoBasis &basis) const
    {
      return m_axis[kX].isCodirectionalTo(basis.m_axis[kX]) &&
             m_axis[kY].isCodirectionalTo(basis.m_axis[kY]) &&
             m_axis[kZ].isCodirectionalTo(basis.m_axis[kZ]);
    }
    bool operator !=(const OdTrVisOrthoBasis &basis) const
    {
      return !m_axis[kX].isCodirectionalTo(basis.m_axis[kX]) ||
             !m_axis[kY].isCodirectionalTo(basis.m_axis[kY]) ||
             !m_axis[kZ].isCodirectionalTo(basis.m_axis[kZ]);
    }

    // Transformation (Ge specificaiton)
    friend OdTrVisOrthoBasis operator *(const OdTrVisOrthoBasis &basis, const OdGeMatrix3d &xMat)
    {
      return OdTrVisOrthoBasis().set(OdGeVector3d(basis.xAxis()).transformBy(xMat),
                                     OdGeVector3d(basis.yAxis()).transformBy(xMat),
                                     OdGeVector3d(basis.zAxis()).transformBy(xMat));
    }
    friend OdTrVisOrthoBasis operator *(const OdGeMatrix3d &xMat, const OdTrVisOrthoBasis &basis)
    {
      return OdTrVisOrthoBasis().set(OdGeVector3d(basis.xAxis()).transformBy(xMat),
                                     OdGeVector3d(basis.yAxis()).transformBy(xMat),
                                     OdGeVector3d(basis.zAxis()).transformBy(xMat));
    }
    OdTrVisOrthoBasis &operator *=(const OdGeMatrix3d &xMat)
    {
      m_axis[kX].transformBy(xMat);
      m_axis[kY].transformBy(xMat);
      m_axis[kZ].transformBy(xMat);
      return *this;
    }
    OdTrVisOrthoBasis &transform(const OdGeMatrix3d &xMat, OdUInt32 nElem)
    {
      if (GETBIT(nElem, 1)) m_axis[kX].transformBy(xMat);
      if (GETBIT(nElem, 2)) m_axis[kY].transformBy(xMat);
      if (GETBIT(nElem, 4)) m_axis[kZ].transformBy(xMat);
      return *this;
    }

    // Transformation (OdTr specification)
    template <typename MtxType>
    friend OdTrVisOrthoBasis operator *(const OdTrVisOrthoBasis &basis, const MtxType &xMat)
    {
      return OdTrVisOrthoBasis().set(xMat.transformClone(basis.xAxis(), 0.0f),
                                     xMat.transformClone(basis.yAxis(), 0.0f),
                                     xMat.transformClone(basis.zAxis(), 0.0f));
    }
    template <typename MtxType>
    friend OdTrVisOrthoBasis operator *(const MtxType &xMat, const OdTrVisOrthoBasis &basis)
    {
      return OdTrVisOrthoBasis().set(xMat.transformClone(basis.xAxis(), 0.0f),
                                     xMat.transformClone(basis.yAxis(), 0.0f),
                                     xMat.transformClone(basis.zAxis(), 0.0f));
    }
    template <typename MtxType>
    OdTrVisOrthoBasis &operator *=(const MtxType &xMat)
    {
      xMat.transform(m_axis[kX], 0.0f);
      xMat.transform(m_axis[kY], 0.0f);
      xMat.transform(m_axis[kZ], 0.0f);
      return *this;
    }
    template <typename MtxType>
    OdTrVisOrthoBasis &transform(const MtxType &xMat, OdUInt32 nElem)
    {
      if (GETBIT(nElem, 1)) xMat.transform(m_axis[kX], 0.0f);
      if (GETBIT(nElem, 2)) xMat.transform(m_axis[kY], 0.0f);
      if (GETBIT(nElem, 4)) xMat.transform(m_axis[kZ], 0.0f);
      return *this;
    }

    // Normalize basis
    OdTrVisOrthoBasis &normalize()
    {
      axisNormalize(m_axis[kX], OdGeVector3d::kXAxis);
      axisNormalize(m_axis[kY], OdGeVector3d::kYAxis);
      axisNormalize(m_axis[kZ], OdGeVector3d::kZAxis);
      return *this;
    }

    // Transform to local coordinates system
    template <typename VecType>
    OdGePoint2d &toLocal2d(OdGePoint2d &point, const VecType &pt) const
    { const OdGeVector3d projPt(pt[0], pt[1], pt[2]);
      return point.set(m_axis[kX].dotProduct(projPt), m_axis[kY].dotProduct(projPt)); }
    template <typename VecType>
    OdGePoint2d toLocal2d(const VecType &pt) const
    { const OdGeVector3d projPt(pt[0], pt[1], pt[2]);
      return OdGePoint2d(m_axis[kX].dotProduct(projPt), m_axis[kY].dotProduct(projPt)); }
    template <typename VecType>
    OdGePoint3d &toLocal(OdGePoint3d &point, const VecType &pt) const
    { const OdGeVector3d projPt(pt[0], pt[1], pt[2]);
      return point.set(m_axis[kX].dotProduct(projPt), m_axis[kY].dotProduct(projPt), m_axis[kZ].dotProduct(projPt)); }
    template <typename VecType>
    OdGePoint3d toLocal(const VecType &pt) const
    { const OdGeVector3d projPt(pt[0], pt[1], pt[2]);
      return OdGePoint3d(m_axis[kX].dotProduct(projPt), m_axis[kY].dotProduct(projPt), m_axis[kZ].dotProduct(projPt)); }

    // Transform to global coordinates system
    OdGePoint3d &toGlobal(OdGePoint3d &point, double fX, double fY) const
    { return point = (m_axis[kX] * fX + m_axis[kY] * fY).asPoint(); }
    OdGePoint3d toGlobal(double fX, double fY) const
    { return (m_axis[kX] * fX + m_axis[kY] * fY).asPoint(); }
    template <typename VecType>
    OdGePoint3d &toGlobal(OdGePoint3d &point, const VecType &pt) const
    { return point = (m_axis[kX] * pt[0] + m_axis[kY] * pt[1] + m_axis[kZ] * pt[2]).asPoint(); }
    template <typename VecType>
    OdGePoint3d toGlobal(const VecType &pt) const
    { return (m_axis[kX] * pt[0] + m_axis[kY] * pt[1] + m_axis[kZ] * pt[2]).asPoint(); }

    // Calibrate basis
    OdTrVisOrthoBasis &calibrate()
    {
      m_axis[kX] = m_axis[kY].crossProduct(m_axis[kZ]);
      axisNormalize(m_axis[kX], OdGeVector3d::kXAxis);
      m_axis[kY] = m_axis[kZ].crossProduct(m_axis[kX]);
      axisNormalize(m_axis[kY], OdGeVector3d::kYAxis);
      axisNormalize(m_axis[kZ], OdGeVector3d::kZAxis);
      return *this;
    }

    // Setup basis from direction only
    OdTrVisOrthoBasis &makeBasis0(const OdGeVector3d &direction)
    {
      m_axis[kZ] = direction; axisNormalize(m_axis[kZ], OdGeVector3d::kZAxis);
      m_axis[kX] = m_axis[kZ].perpVector(); axisNormalize(m_axis[kX], OdGeVector3d::kXAxis);
      m_axis[kY] = m_axis[kZ].crossProduct(m_axis[kX]);
      axisNormalize(m_axis[kY], OdGeVector3d::kYAxis);
      return *this;
    }
    // Setup basis from direction and up vector
    OdTrVisOrthoBasis &makeBasis(const OdGeVector3d &direction, const OdGeVector3d &upVector = OdGeVector3d::kYAxis)
    {
      m_axis[kZ] = direction; axisNormalize(m_axis[kZ], OdGeVector3d::kZAxis);
      m_axis[kY] = upVector;  axisNormalize(m_axis[kY], OdGeVector3d::kYAxis);
      m_axis[kX] = m_axis[kY].crossProduct(m_axis[kZ]);
      axisNormalize(m_axis[kX], OdGeVector3d::kXAxis);
      return *this;
    }
};

/** \details
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
class OdTrVisViewBasis : public OdTrVisOrthoBasis
{
  protected:
    OdGePoint3d m_origin;
  public:
    OdTrVisViewBasis() { }

    // Setup default WCS basis
    OdTrVisViewBasis &setDefault()
    {
      OdTrVisOrthoBasis::setDefault();
      m_origin = OdGePoint3d::kOrigin;
      return *this;
    }
    // Setup from input vectors
    template <typename VecType, typename VecType2>
    OdTrVisViewBasis &set(const VecType &origin, const VecType2 &xAxis, const VecType2 &yAxis, const VecType2 &zAxis)
    {
      OdTrVisOrthoBasis::set(xAxis, yAxis, zAxis);
      m_origin.set(origin[0], origin[1], origin[2]);
      return *this;
    }
    // Setup from Ge matrix
    OdTrVisViewBasis &set(const OdGeMatrix3d &xMat)
    {
      OdTrVisOrthoBasis::set(xMat);
      m_origin = xMat.getCsOrigin();
      return *this;
    }
    // Setup from any type of OdTr matrix
    template <typename MtxType>
    OdTrVisViewBasis &set(const MtxType &xMat)
    {
      OdTrVisOrthoBasis::set(xMat);
      if (is4x4Matrix(xMat))
        m_origin.set(*(xMat.entry[2] + 4), *(xMat.entry[2] + 5), *(xMat.entry[2] + 6));
      else
        m_origin = OdGePoint3d::kOrigin;
      return *this;
    }

    // Local setter
    OdTrVisViewBasis &setOrigin(const OdGePoint3d &origin) { m_origin = origin; return *this; }

    // Getter
    const OdGePoint3d &origin() const { return m_origin; }

    // Get as Ge matrix
    OdGeMatrix3d &get(OdGeMatrix3d &xMat, const OdGePoint3d &csOrigin = OdGePoint3d::kOrigin) const
    {
      return OdTrVisOrthoBasis::get(xMat, m_origin);
    }
    // Get as any OdTr matrix
    template <typename MtxType>
    MtxType &get(MtxType &xMat) const
    {
      OdTrVisOrthoBasis::get(xMat);
      if (is4x4Matrix(xMat))
        *(xMat.entry[2] + 4) = (OD_TYPENAME MtxType::EntryType)m_origin.x,
        *(xMat.entry[2] + 5) = (OD_TYPENAME MtxType::EntryType)m_origin.y,
        *(xMat.entry[2] + 6) = (OD_TYPENAME MtxType::EntryType)m_origin.z;
      return xMat;
    }

    // Comparision
    bool operator ==(const OdTrVisViewBasis &basis) const
    {
      return OdTrVisOrthoBasis::operator ==(basis) && m_origin.isEqualTo(basis.origin());
    }
    bool operator !=(const OdTrVisViewBasis &basis) const
    {
      return OdTrVisOrthoBasis::operator !=(basis) || !m_origin.isEqualTo(basis.origin());
    }

    // Transformation (Ge specificaiton)
    friend OdTrVisViewBasis operator *(const OdTrVisViewBasis &basis, const OdGeMatrix3d &xMat)
    {
      return OdTrVisViewBasis().set(OdGePoint3d(basis.origin()).transformBy(xMat),
                                    OdGeVector3d(basis.xAxis()).transformBy(xMat),
                                    OdGeVector3d(basis.yAxis()).transformBy(xMat),
                                    OdGeVector3d(basis.zAxis()).transformBy(xMat));
    }
    friend OdTrVisViewBasis operator *(const OdGeMatrix3d &xMat, const OdTrVisViewBasis &basis)
    {
      return OdTrVisViewBasis().set(OdGePoint3d(basis.origin()).transformBy(xMat),
                                    OdGeVector3d(basis.xAxis()).transformBy(xMat),
                                    OdGeVector3d(basis.yAxis()).transformBy(xMat),
                                    OdGeVector3d(basis.zAxis()).transformBy(xMat));
    }
    OdTrVisViewBasis &operator *=(const OdGeMatrix3d &xMat)
    {
      m_axis[kX].transformBy(xMat);
      m_axis[kY].transformBy(xMat);
      m_axis[kZ].transformBy(xMat);
      m_origin.transformBy(xMat);
      return *this;
    }
    OdTrVisViewBasis &transform(const OdGeMatrix3d &xMat, OdUInt32 nElem)
    {
      OdTrVisOrthoBasis::transform(xMat, nElem);
      if (GETBIT(nElem, 8)) m_origin.transformBy(xMat);
      return *this;
    }

    // Transformation (OdTr specification)
    template <typename MtxType>
    friend OdTrVisViewBasis operator *(const OdTrVisViewBasis &basis, const MtxType &xMat)
    {
      return OdTrVisViewBasis().set(xMat.transformClone(basis.origin()),
                                    xMat.transformClone(basis.xAxis(), 0.0f),
                                    xMat.transformClone(basis.yAxis(), 0.0f),
                                    xMat.transformClone(basis.zAxis(), 0.0f));
    }
    template <typename MtxType>
    friend OdTrVisViewBasis operator *(const MtxType &xMat, const OdTrVisViewBasis &basis)
    {
      return OdTrVisViewBasis().set(xMat.transformClone(basis.origin()),
                                    xMat.transformClone(basis.xAxis(), 0.0f),
                                    xMat.transformClone(basis.yAxis(), 0.0f),
                                    xMat.transformClone(basis.zAxis(), 0.0f));
    }
    template <typename MtxType>
    OdTrVisViewBasis &operator *=(const MtxType &xMat)
    {
      xMat.transform(m_axis[kX], 0.0f);
      xMat.transform(m_axis[kY], 0.0f);
      xMat.transform(m_axis[kZ], 0.0f);
      xMat.transform(m_origin);
      return *this;
    }
    template <typename MtxType>
    OdTrVisViewBasis &transform(const MtxType &xMat, OdUInt32 nElem)
    {
      OdTrVisOrthoBasis::transform(xMat, nElem);
      if (GETBIT(nElem, 8)) xMat.transform(m_origin);
      return *this;
    }

    // Translation
    template <typename VecType>
    friend OdTrVisViewBasis operator +(const OdTrVisViewBasis &basis, const VecType &translationVec)
    {
      return OdTrVisViewBasis().set(basis.origin() + translationVec, basis.xAxis(), basis.yAxis(), basis.zAxis());
    }
    template <typename VecType>
    friend OdTrVisViewBasis operator -(const OdTrVisViewBasis &basis, const VecType &translationVec)
    {
      return OdTrVisViewBasis().set(basis.origin() - translationVec, basis.xAxis(), basis.yAxis(), basis.zAxis());
    }
    template <typename VecType>
    OdTrVisViewBasis &operator +=(const VecType &translationVec)
    { m_origin += translationVec;
      return *this;
    }
    template <typename VecType>
    OdTrVisViewBasis &operator -=(const VecType &translationVec)
    { m_origin -= translationVec;
      return *this;
    }

    // Transform to local coordinates system
    template <typename VecType>
    OdGePoint2d &toLocal2d(OdGePoint2d &point, const VecType &pt) const
    { const OdGeVector3d projPt(pt[0] - m_origin.x, pt[1] - m_origin.y, pt[2] - m_origin.z);
      return point.set(m_axis[kX].dotProduct(projPt), m_axis[kY].dotProduct(projPt)); }
    template <typename VecType>
    OdGePoint2d toLocal2d(const VecType &pt) const
    { const OdGeVector3d projPt(pt[0] - m_origin.x, pt[1] - m_origin.y, pt[2] - m_origin.z);
      return OdGePoint2d(m_axis[kX].dotProduct(projPt), m_axis[kY].dotProduct(projPt)); }
    template <typename VecType>
    OdGePoint3d &toLocal(OdGePoint3d &point, const VecType &pt) const
    { const OdGeVector3d projPt(pt[0] - m_origin.x, pt[1] - m_origin.y, pt[2] - m_origin.z);
      return point.set(m_axis[kX].dotProduct(projPt), m_axis[kY].dotProduct(projPt), m_axis[kZ].dotProduct(projPt)); }
    template <typename VecType>
    OdGePoint3d toLocal(const VecType &pt) const
    { const OdGeVector3d projPt(pt[0] - m_origin.x, pt[1] - m_origin.y, pt[2] - m_origin.z);
      return OdGePoint3d(m_axis[kX].dotProduct(projPt), m_axis[kY].dotProduct(projPt), m_axis[kZ].dotProduct(projPt)); }

    // Transform to global coordinates system
    OdGePoint3d &toGlobal(OdGePoint3d &point, double fX, double fY) const
    { return point = (m_axis[kX] * fX + m_axis[kY] * fY + m_origin.asVector()).asPoint(); }
    OdGePoint3d toGlobal(double fX, double fY) const
    { return (m_axis[kX] * fX + m_axis[kY] * fY + m_origin.asVector()).asPoint(); }
    template <typename VecType>
    OdGePoint3d &toGlobal(OdGePoint3d &point, const VecType &pt) const
    { return point = (m_axis[kX] * pt[0] + m_axis[kY] * pt[1] + m_axis[kZ] * pt[2] + m_origin.asVector()).asPoint(); }
    template <typename VecType>
    OdGePoint3d toGlobal(const VecType &pt) const
    { return (m_axis[kX] * pt[0] + m_axis[kY] * pt[1] + m_axis[kZ] * pt[2] + m_origin.asVector()).asPoint(); }

    // Setup basis from direction only
    TD_USING(OdTrVisOrthoBasis::makeBasis0);
    OdTrVisOrthoBasis &makeBasis0(const OdGePoint3d &pos, const OdGePoint3d &target)
    {
      OdTrVisOrthoBasis::makeBasis0(target - pos);
      m_origin = pos;
      return *this;
    }
    // Setup basis from direction and up vector
    TD_USING(OdTrVisOrthoBasis::makeBasis);
    OdTrVisViewBasis &makeBasis(const OdGePoint3d &pos, const OdGePoint3d &target, const OdGeVector3d &upVector = OdGeVector3d::kYAxis)
    {
      OdTrVisOrthoBasis::makeBasis(target - pos, upVector);
      m_origin = pos;
      return *this;
    }
};

#include "TD_PackPop.h"

#endif // ODTRVISBASIS
