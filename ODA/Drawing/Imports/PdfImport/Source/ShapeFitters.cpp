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
#include "ShapeFitters.h"

#include <iomanip>
#include "Ge/GeCircArc3d.h"
#include "Ge/GeEllipArc3d.h"

const int CircleFitter::kCircleUnknowns = 3; // unknowns for a circle are centrex, centrey, radius

CircleFitter::CircleFitter()
  : BaseFitter(kCircleUnknowns)
{
}

void CircleFitter::generateResults()
{
  double centrex = 0.5 * (m_MaxX + m_MinX);
  double centrey = 0.5 * (m_MaxY + m_MinY);
  double radius = 0.5 * std::max<double>(m_MaxX - m_MinX, m_MaxY - m_MinY);

  m_Results(0, 0) = centrex;
  m_Results(1, 0) = centrey;
  m_Results(2, 0) = radius;
}

void CircleFitter::calcMatrices()
{
  double x0 = m_Results(0, 0);
  double y0 = m_Results(1, 0);
  double radius = m_Results(2, 0);

  for (int i = 0; i < m_NumPoints; i++)
  {
    double dx = m_Outputs(i, 0) - x0;
    double dy = m_Outputs(i, 1) - y0;
    double dxsqr = dx * dx;
    double dysqr = dy * dy;

    m_Construction(i, 0) = -2.0 * dx;
    m_Construction(i, 1) = -2.0 * dy;
    m_Construction(i, 2) = -2.0 * radius;
    m_QWeight(i, i) = 1.0 / (4.0 * (dxsqr + dysqr));
    m_Len(i, 0) = (radius * radius) - dxsqr - dysqr;
  }
}

void CircleFitter::createCurveByParams()
{
  m_Curve = OdGeCurve3dPtr(new OdGeCircArc3d(OdGePoint3d(getCenter().x, getCenter().y, 0.), OdGeVector3d::kZAxis, getRadius()));
}

double CircleFitter::getCriteria() const
{
  return getRadius();
}

OdGePoint2d CircleFitter::getCenter() const
{
  return OdGePoint2d(m_Results(0, 0), m_Results(1, 0));
}

double CircleFitter::getRadius() const
{
  return m_Results(2, 0);
}

const int EllipseFitter::kEllipseUnknowns = 5; // unknowns for an ellipse are centrex, centrey, major, minor, rotation

EllipseFitter::EllipseFitter()
  : BaseFitter(kEllipseUnknowns)
{
}

void EllipseFitter::generateResults()
{
  double centrex = 0.5 * (m_MaxX + m_MinX);
  double centrey = 0.5 * (m_MaxY + m_MinY);
  double dx = m_MaxX - m_MinX;
  double dy = m_MaxY - m_MinY;
  double major = 0.5 * std::max<double>(dx, dy);
  double minor = 0.5 * std::min<double>(dx, dy);
  double rotation = atan(dy / dx);

  m_Results(0, 0) = centrex;
  m_Results(1, 0) = centrey;
  m_Results(2, 0) = major;
  m_Results(3, 0) = minor;
  m_Results(4, 0) = rotation;
}


void EllipseFitter::calcMatrices()
{
  double x0 = m_Results(0, 0);
  double y0 = m_Results(1, 0);
  double a = m_Results(2, 0);
  double b = m_Results(3, 0);

  double sinr = sin(m_Results(4, 0));
  double cosr = cos(m_Results(4, 0));
  double asqr = a * a;
  double bsqr = b * b;

  for (int i = 0; i < m_NumPoints; i++)
  {
    double x = m_Outputs(i, 0);
    double y = m_Outputs(i, 1);

    double d1 = ((y - y0) * cosr - (x - x0) * sinr);
    double d2 = ((x - x0) * cosr + (y - y0) * sinr);

    m_Construction(i, 0) = 2.0 * ((asqr * d1 * sinr) - (bsqr * d2 * cosr));
    m_Construction(i, 1) = -2.0 * ((bsqr * d2 * sinr) + (asqr * d1 * cosr));
    m_Construction(i, 2) = 2.0 * a * ((d1 * d1) - bsqr);
    m_Construction(i, 3) = 2.0 * b * ((d2 * d2) - asqr);
    m_Construction(i, 4) = 2.0 * d1 * d2 * (bsqr - asqr);

    double p = 2.0 * ((asqr * d1 * sinr) + (bsqr * d2 * cosr));
    double q = -2.0 * ((bsqr * d2 * sinr) + (asqr * d1 * cosr));
    m_QWeight(i, i) = 1.0 / (p * p + q * q);
    m_Len(i, 0) = -((asqr * d1 * d1) + (bsqr * d2 * d2) - (asqr * bsqr));
  }
}

OdGePoint2d EllipseFitter::getCenter() const
{
  return OdGePoint2d(m_Results(0, 0), m_Results(1, 0));
}

double EllipseFitter::getMajorRadius() const
{
  return m_Results(2, 0);
}

double EllipseFitter::getMinorRadius() const
{
  return m_Results(3, 0);
}

double EllipseFitter::getRotation() const
{
  return m_Results(4, 0);
}

void EllipseFitter::normalizeAdjustedPoints()
{
  double theta = fabs(m_Results(4, 0));
  bool negative = OdNegative(m_Results(4, 0));

  theta = fmod(theta, Oda2PI);
  if (negative)
    theta = Oda2PI - theta;

  double major = m_Results(2, 0);
  double minor = m_Results(3, 0);
  if (OdGreater(minor, major))
  {
    m_Results(2, 0) = minor;
    m_Results(3, 0) = major;

    theta -= OdaPI2;
    if (OdNegative(theta))
      theta += Oda2PI;
  }

  m_Results(4, 0) = theta;
}

void EllipseFitter::createCurveByParams()
{
  OdGeVector3d major = OdGeVector3d::kXAxis;
  major.rotateBy(getRotation(), OdGeVector3d::kZAxis);
  OdGeVector3d minor = OdGeVector3d::kYAxis;
  minor.rotateBy(getRotation(), OdGeVector3d::kZAxis);

  OdGePoint2d center = getCenter();
  m_Curve = OdGeCurve3dPtr(new OdGeEllipArc3d(OdGePoint3d(center.x, center.y, 0.),
    major, minor, getMajorRadius(), getMinorRadius()));
}

double EllipseFitter::getCriteria() const
{
  return getMajorRadius();
}

