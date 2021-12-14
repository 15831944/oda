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

#ifndef _IMPORT_BASE_FITTER_
#define _IMPORT_BASE_FITTER_

#include <iostream>
#include "MathMatrix.h"
#include "Ge/GePoint2d.h"
#include "Gs/GsFiler.h"
#include "Ge/GePoint3dArray.h"
#include "Ge/GeCurve3d.h"
#include "Ge/GeCurve3dPtrArray.h"

class BaseFitter
{
public:
  BaseFitter(int unknowns);
  virtual ~BaseFitter();
private:
protected:

public:
  bool fit(const OdGePoint2dArray& points, OdGePoint2dArray& out_points, bool preserve = false);
  
  virtual double distanceTo(double x, double y) const;

  double getVariance()
  {
    return m_Variance;
  }

  double getStdDeviation()
  {
    return m_StdDeviation;
  }

  OdGeCurve3dPtr getCurve()
  {
    return m_Curve;
  }

protected:
  MathMatrix<double> m_Deviations;
  MathMatrix<double> m_Construction;
  MathMatrix<double> m_Len;
  MathMatrix<double> m_QWeight;
  MathMatrix<double> m_Outputs;
  MathMatrix<double> m_Results;

  int m_NumPoints;
  unsigned int m_MinNumPoints;
  double m_MinX;
  double m_MaxX;
  double m_MinY;
  double m_MaxY;

  double m_Variance;
  double m_StdDeviation;

  OdGeCurve3dPtr m_Curve;

private:
  virtual double getCriteria() const = 0;
  virtual void generateResults() = 0;
  virtual void calcFinalDeviation(int point, double &dxi, double &dyi) const;
  virtual void normalizeAdjustedPoints() {}
  virtual void calcMatrices() = 0;
  virtual void createCurveByParams() = 0;

  bool fit();

  void addPoint(int count, double x, double y);
  bool isFit() const;
  bool evaluatePoints();
  void evaluateDeviations();
  void evaluateResults();
  void evaluateOutputs();
  bool checkResults();
  void calcDispersion();
  void prepareMatrices(bool preserve = false);
  void fillOutput(OdGePoint2dArray& out_points) const;

  MathMatrix<double> m_Solution;
};

#endif // _IMPORT_BASE_FITTER_
