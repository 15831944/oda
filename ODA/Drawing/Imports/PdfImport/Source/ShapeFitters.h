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

#ifndef _IMPORT_SHAPE_FITTERS_
#define _IMPORT_SHAPE_FITTERS_

#include "BaseFitter.h"

class CircleFitter : public BaseFitter
{
public:
  static const int kCircleUnknowns;
  CircleFitter();

  OdGePoint2d getCenter() const;
  double getRadius() const;

private:
  void generateResults();
  void calcMatrices();
  virtual void createCurveByParams();
  virtual double getCriteria() const;
};

class EllipseFitter : public BaseFitter
{
public:
  static const int kEllipseUnknowns;
  EllipseFitter();

  OdGePoint2d getCenter() const;
  double getMajorRadius() const;
  double getMinorRadius() const;
  double getRotation() const;

private:
  void generateResults();
  void calcMatrices();
  void normalizeAdjustedPoints();
  virtual void createCurveByParams();
  virtual double getCriteria() const;
};

#endif //_IMPORT_SHAPE_FITTERS_
