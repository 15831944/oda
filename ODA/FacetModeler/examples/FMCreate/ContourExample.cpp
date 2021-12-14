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

#include "ContourExample.h"
#include "FMDataSerialize.h"
#include "FMGeometry.h"
#include "FMCreateStart.h"
#include "Ge/GeExtents2d.h"
#include "Ge/GeCircArc2d.h"

using namespace FacetModeler;

void CreateContourExample(const DeviationParams& devParams, OdStreamBuf* pStream, CreationMode mode)
{
  Profile2D profile;

  switch (mode)
  {
  case kSquare:
    profile = createSquare();
    break;
  case kCircle:
    profile = createCircle();
    break;
  case kPolygon:
    profile = createPolygon();
    break;
  case kNutProfile:
    profile = createNutProfile();
    break;
  default: ODA_ASSERT(false);
  }


  BinaryStream sout;
  sout.Create(pStream);
  sout.Write(profile);
}

Profile2D createSquare(const double side)
{
  Profile2D profile;
  profile.push_back(Contour2D::createSquare(OdGePoint2d::kOrigin, side));

  profile.front().setOrientationAt(0, efoFront);
  profile.front().setOrientationAt(1, efoFront);
  profile.front().setOrientationAt(2, efoFront);
  profile.front().setOrientationAt(3, efoFront);

  return profile;
}

Profile2D createCircle(const double radius)
{
  Profile2D profile;
  profile.push_back(Contour2D::createCircle(OdGePoint2d::kOrigin, radius));

  profile.front().setOrientationAt(0, efoFront);
  profile.front().setOrientationAt(1, efoFront);

  return profile;
}

Profile2D createPolygon(double radius, unsigned int sidesCnt)
{
  // Create circle for polygon inscribing
  OdGeCircArc2d circle(OdGePoint2d::kOrigin, radius); 
  OdGePoint2dArray points;

  // Inscribe a polygon in a circle and get vertices
  circle.getSamplePoints(sidesCnt, points);

  Profile2D profile;                      // Create profile
  profile.resize(1);                      // With one contour
  profile.front().appendVertices(points); // Add vertices

  profile.front().setClosed(); // Close profile
  profile.front().makeCCW();   // Make contour outer

  return profile;
}

Profile2D createNutProfile(double majorRadius, double minorRadius)
{
  // Create profiles
  Profile2D center = createCircle(minorRadius);
  Profile2D hexagon = createPolygon(majorRadius, 6);
  Profile2D nutProfile;

  // Specify the tolerance that will be used during the operation.
  OdGeTol tol;

  // Perform Boolean operation
  Profile2D::PerformOperation(FacetModeler::eDifference, hexagon, center, nutProfile, tol);

  return nutProfile;
}
