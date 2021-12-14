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
#include "BaseFitter.h"
#include "Ge/GePoint3d.h"

#include <limits>
#include <cassert>
#include <stdexcept>
#include <iomanip>

#define STL_USING_ALL
#include <OdaSTL.h>


#define TOL 0.000000001
#define MAX_ITERATIONS 50

BaseFitter::BaseFitter(int unknowns)
  : m_Solution(unknowns, 1)
  , m_Results(unknowns, 1)
  , m_NumPoints(0)
  , m_MinNumPoints(unknowns)
  , m_MinX(std::numeric_limits<double>::max())
  , m_MaxX(-std::numeric_limits<double>::max())
  , m_MinY(std::numeric_limits<double>::max())
  , m_MaxY(-std::numeric_limits<double>::max())
  , m_Variance(std::numeric_limits<double>::max())
  , m_StdDeviation(std::numeric_limits<double>::max())
{

}

BaseFitter::~BaseFitter()
{
}

bool BaseFitter::fit(const OdGePoint2dArray& points, OdGePoint2dArray& out_points, bool preserve)
{
  if (points.size() < m_MinNumPoints + 1)
    return false;


  m_NumPoints = points.size();
  prepareMatrices(preserve);

  for (int i = 0; i < m_NumPoints; ++i)
  {
    addPoint(i, points[i].x , points[i].y);
  }

  bool ret = fit();

  fillOutput(out_points);
  return ret;
}

double BaseFitter::distanceTo(double x, double y) const
{
  return m_Curve->distanceTo(OdGePoint3d(x, y, 0));
}

void BaseFitter::addPoint(int count, double x, double y)
{
  m_Outputs(count, 0) = x;
  m_Outputs(count, 1) = y;

  m_MinX = std::min<double>(m_MinX, x);
  m_MaxX = std::max<double>(m_MaxX, x);
  m_MinY = std::min<double>(m_MinY, y);
  m_MaxY = std::max<double>(m_MaxY, y);
}

bool BaseFitter::fit()
{
  generateResults();
  bool ret = true;
  int counter = 0;

  while (true)
  {
    calcMatrices();
    if (evaluatePoints())
    {
      ++counter;
      evaluateResults();

      if (isFit())
        break;
      if (counter >= MAX_ITERATIONS)
        break;
    }
    else
    {
      ret = false;
      break;
    }
  }

  ret = ret && (counter > 0 && counter < MAX_ITERATIONS);
  if (ret)
  {
    normalizeAdjustedPoints();
    evaluateDeviations();
    evaluateOutputs();
    calcDispersion();
    createCurveByParams();
    ret = checkResults();
  }

  return ret;
}

void BaseFitter::prepareMatrices(bool preserve)
{
  m_MinX = std::numeric_limits<double>::max();
  m_MaxX = -std::numeric_limits<double>::max();
  m_MinY = std::numeric_limits<double>::max();
  m_MaxY = -std::numeric_limits<double>::max();
  m_Variance = std::numeric_limits<double>::max();
  m_StdDeviation = std::numeric_limits<double>::max();

  m_Solution.makeZero();
  m_Results.makeZero();

  m_Deviations.setSize(m_NumPoints, 1, preserve);
  m_Deviations.makeZero();
  m_Construction.setSize(m_NumPoints, m_MinNumPoints, preserve);
  m_Construction.makeZero();
  m_Len.setSize(m_NumPoints, 1, preserve);
  m_Len.makeZero();
  m_QWeight.setSize(m_NumPoints, m_NumPoints, false);
  m_QWeight.makeZero();
  m_Outputs.setSize(m_NumPoints, 2, preserve);
  m_Outputs.makeZero();
  m_Curve = OdGeCurve3dPtr();
}

bool BaseFitter::evaluatePoints()
{
  MathMatrix<double> pa = m_QWeight*m_Construction;
  MathMatrix<double> atpa = transpose(m_Construction)*pa;

  MathMatrix<double> inverse_matrix(atpa.getNumRows(), atpa.getNumCols());

  bool ret;
  inverse_matrix = inverse(atpa, &ret);
  if (ret)
  {
    MathMatrix<double> pl = m_QWeight*m_Len;
    MathMatrix<double> atpl = transpose(m_Construction)*pl;

    m_Solution = inverse_matrix*atpl;
    return true;
  }
  return false;
}

void BaseFitter::evaluateResults()
{
  m_Results += m_Solution;
}

void BaseFitter::evaluateDeviations()
{
  m_Deviations = m_Construction*m_Solution - m_Len;
}

void BaseFitter::calcFinalDeviation(int point, double &dxi, double &dyi) const
{
  dxi = m_Construction(point, 0) * m_QWeight(point, point) * m_Deviations(point, 0);
  dyi = m_Construction(point, 1) * m_QWeight(point, point) * m_Deviations(point, 0);
}

void BaseFitter::evaluateOutputs()
{
  for (int i = 0; i < m_NumPoints; ++i)
  {
    double dxi = 0.0;
    double dyi = 0.0;
    calcFinalDeviation(i, dxi, dyi); 

    m_Outputs(i, 0) += dxi;
    m_Outputs(i, 1) += dyi;
  }
}

bool BaseFitter::checkResults()
{
  MathVector<double> deviation(m_NumPoints, 0);
  bool pass = (m_NumPoints > 0);

  double criteria = getCriteria();

  for (int i = 0; i < m_NumPoints; ++i)
  {
    double x = m_Outputs(i, 0);
    double y = m_Outputs(i, 1);

    deviation[i] = distanceTo(x, y);
    pass = pass && fabs(deviation[i]) <  0.01*criteria;
  }

  if (!pass)
    return false;

  MathMatrix<double> atp = transpose(m_Construction) * m_QWeight;
  MathMatrix<double> atpv = atp * m_Deviations;

  pass = true;

  for (unsigned int j = 0; j < m_MinNumPoints; ++j)
  {
    double tol = std::max(1e-10, fabs(/*std::*/nextafter(m_Results(j, 0), INFINITY) - m_Results(j, 0)));
    pass = pass && OdZero(fabs(atpv(j, 0)), tol);
  }

  return pass;
}

bool BaseFitter::isFit() const
{
  bool tol_too_small_for_all = true;
  for (unsigned int i = 0; i < m_MinNumPoints; ++i)
  {
    if (fabs(/*std::*/nextafter(m_Results(i, 0), INFINITY) - m_Results(i, 0)) < fabs(m_Solution(i, 0)))
      tol_too_small_for_all = false;
  }

  if (tol_too_small_for_all)
    return true;

  for (unsigned int i = 0; i < m_MinNumPoints; ++i)
  {
    if (fabs(m_Solution(i, 0)) > TOL)
      return false;
  }
  return true;
}

void BaseFitter::calcDispersion()
{
  MathMatrix<double> pv = m_QWeight * m_Deviations;
  MathMatrix<double> vtpv = transpose(m_Deviations) * pv;

  int degreesFreedom = m_NumPoints - m_MinNumPoints;
  m_Variance = vtpv(0, 0) / degreesFreedom;
  m_StdDeviation = sqrt(m_Variance);
}

void BaseFitter::fillOutput(OdGePoint2dArray& out_points) const
{
  out_points.resize(m_NumPoints);
  for (int j = 0; j < m_NumPoints; ++j)
  {
    out_points[j] = OdGePoint2d(m_Outputs(j, 0), m_Outputs(j, 1));
  }
}


