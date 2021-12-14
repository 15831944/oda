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

#include "BodyExample.h"
#include "FMProfile3D.h"
#include "FMCreateStart.h"
#include "FMDataSerialize.h"
#include "Modeler/FMMdlIterators.h"

#include "Ge/GeCircArc2d.h"
#include "Ge/GeScale3d.h"
#include "Ge/GeLine3d.h"
#include "Ge/GeCircArc3d.h"

#include "CmColorBase.h"

using namespace FacetModeler;

void CreateBodyExample(const DeviationParams& devParams, OdStreamBuf* pStream, CreationMode mode)
{
  Body body;

  switch (mode)
  {
  case kCone:        body = createCone(devParams); break;
  case kCube:        body = createCube(devParams); break;
  case kCuboid:      body = createCuboid(devParams); break;
  case kCylinder:    body = createCylinder(devParams); break;
  case kDice:        body = createDice(devParams); break;
  case kGear:        body = createGear(devParams); break;
  case kLogo:        body = createLogo(devParams); break;
  case kPrism:       body = createPrism(devParams); break;
  case kPyramid:     body = createPyramid(devParams); break;
  case kSnowman:     body = createSnowman(devParams); break;
  case kSphere:      body = createSphere(devParams); break;
  case kSemiSphere:  body = createSemiSphere(devParams); break;
  case kSpherePart:  body = createSpherePart(devParams); break;
  case kTetrahedron: body = createTetrahedron(devParams); break;
  case kWrench:      body = createWrench(devParams); break;
  case kMeshTetra:   body = createMeshTetrahedron(devParams); break;
  case kMeshCuboid:    body = createMeshCuboid(devParams); break;
  default: ODA_ASSERT(false);
  }

  
  BinaryStream sout;
  sout.Create(pStream);
  sout.Write(body);
}

void propsBodyExample(const DeviationParams& devParams, OdStreamBuf* pStream, CreationMode mode)
{
  BinaryStream sout;
  sout.Create(pStream);

  switch (mode)
  {
  case kSliceBody:
  {
    Profile3D profile = sliceBody(devParams);
    sout.Write(profile.as2d());
  }
  break;
  case kIsectBody:
  {
    std::vector<OdGePoint3d> points = intersectBody(devParams);
    for (unsigned int i = 0; i < points.size(); i++)
    {
      sout.Write(points[i]);
    }
  }
  break;
  case kVolumeBody:
  {
    double volume = calculateBodyVolumeArea(devParams);
    sout.Write(volume);
  }
  break;
  case kSetTags:
  {
    Body body = iterateBodyAndSetTags(devParams);
    sout.Write(body);
  }
  break;

  default: ODA_ASSERT(false);
  }
}

Body createCube(const DeviationParams& devDeviation, double edgeLen)
{
  OdGeVector3d dimensions(edgeLen, edgeLen, edgeLen); // Create dimensions vector
  OdGeVector3d shift(edgeLen / 2, edgeLen / 2, 0.0);  // For moving the origin point to the center of the cube base

  return Body::box(OdGePoint3d::kOrigin - shift, dimensions);
}

Body createCuboid(const DeviationParams& devParams, OdGeVector3d vSizes)
{
  OdGeVector3d shift(vSizes.x / 2, vSizes.y / 2, 0.0);

  return Body::box(OdGePoint3d::kOrigin - shift, vSizes);
}

Body createTetrahedron(const DeviationParams& devDeviation, double edgeLen)
{
  OdGePoint2d center = OdGePoint3d::kOrigin.convert2d();
  OdGeCircArc2d circle(center, edgeLen); // Create circle for triangle inscribing
  OdGePoint2dArray points;
  circle.getSamplePoints(4, points);     // Inscribe a triangle in a circle and get vertices

  Profile2D cBase;         // Create base profile
  cBase.resize(1);                       // Only with one contour
  cBase.front().appendVertices(points);  // Fill contour with vertices

  cBase.front().setClosed();             // Close profile
  cBase.front().makeCCW();               // Make contour outer

  OdGePoint3d apex(
    OdGePoint3d::kOrigin.x,              // Create apex point with height the same as length of base edge
    OdGePoint3d::kOrigin.y,
    OdGePoint3d::kOrigin.z + (points[1] - points[0]).length()
  );

  // Create pyramid
  return Body::pyramid(cBase, apex, devDeviation);
}

Body createPyramid(const DeviationParams& devDeviation, double radius, double height, unsigned int sidesCnt)
{
  OdGeCircArc2d circle(OdGePoint3d::kOrigin.convert2d(), radius); // Create circle for polygon inscribing
  OdGePoint2dArray points;
  circle.getSamplePoints(sidesCnt, points); // Inscribe a polygon in a circle and get vertices

  Profile2D cBase;            // Create base profile
  cBase.resize(1);                          // Only with one contour
  cBase.front().appendVertices(points);     // Fill contour with vertices

  cBase.front().setClosed();                // Close profile
  cBase.front().makeCCW();                  // Make contour outer

  OdGePoint3d apex(                         // Create apex
    OdGePoint3d::kOrigin.x,
    OdGePoint3d::kOrigin.y,
    OdGePoint3d::kOrigin.z + height
  );

  // Create pyramid
  return Body::pyramid(cBase, apex, devDeviation);
}

Body createCone(const DeviationParams& devDeviation, double radius, double height)
{
  Profile2D cBase;            // Create base profile
  cBase.resize(1);                          // With one contour

  cBase.front().appendVertex(OdGePoint2d::kOrigin - OdGeVector2d::kXAxis * radius, 1); // Add vertex with bulge == 1 (Arc)
  cBase.front().appendVertex(OdGePoint2d::kOrigin + OdGeVector2d::kXAxis * radius, 1);

  cBase.front().setOrientationAt(0, efoFront);
  cBase.front().setOrientationAt(1, efoFront);

  cBase.front().setClosed();                // Close profile
  cBase.front().makeCCW();                  // Make contour outer

  OdGePoint3d apex(
    OdGePoint3d::kOrigin.x,                 // Create apex point
    OdGePoint3d::kOrigin.y,
    OdGePoint3d::kOrigin.z + height
  );

  return Body::pyramid(cBase, apex, devDeviation);
}

Body createPrism(const DeviationParams& devDeviation, double radius, double height, unsigned int sidesCnt)
{
  OdGeCircArc2d circle(OdGePoint3d::kOrigin.convert2d(), radius); // Create circle for polygon inscribing
  OdGePoint2dArray points;
  circle.getSamplePoints(sidesCnt, points); // Inscribe a polygon in a circle and get vertices

  Profile2D cBase;            // Create base profile
  cBase.resize(1);                          // With one contour
  cBase.front().appendVertices(points);     // Add vertices

  cBase.front().setClosed();                // Close profile
  cBase.front().makeCCW();                  // Make contour outer

  // Create extruded along Z-axis Body
  return Body::extrusion(cBase, OdGeVector3d::kZAxis * height, devDeviation);
}

Body createCylinder(const DeviationParams& devDeviation, double radius, double height)
{
  Profile2D cBase;            // Create base profile
  cBase.resize(1);                          // With one contour

  cBase.front().appendVertex(OdGePoint2d::kOrigin - OdGeVector2d::kXAxis * radius, 1);
  cBase.front().appendVertex(OdGePoint2d::kOrigin + OdGeVector2d::kXAxis * radius, 1);

  cBase.front().setOrientationAt(0, efoFront);
  cBase.front().setOrientationAt(1, efoFront);

  cBase.front().setClosed();                // Close profile
  cBase.front().makeCCW();                  // Make contour outer

  return Body::extrusion(cBase,
    OdGeVector3d::kZAxis * height, devDeviation);
}

Body createSphere(const DeviationParams& devDeviation, double radius)
{
  Profile2D cSphereProfile;   // Create sphere profile
  cSphereProfile.resize(1);                 // With one contour

  cSphereProfile.front().appendVertex(OdGePoint2d::kOrigin); // Add first point
  cSphereProfile.front().appendVertex(OdGePoint2d::kOrigin + OdGeVector2d::kXAxis * radius * 2, 1); // Add vertex with bulge == 1 (Arc)

  // Will re reversed in revolution method.
  cSphereProfile.front().setOrientationAt(0, efoBack);
  cSphereProfile.front().setOrientationAt(1, efoBack);

  cSphereProfile.front().setClosed();       // Close profile
  cSphereProfile.front().makeCCW();         // Make contour outer

  // Create revolution body
  return Body::revolution(cSphereProfile, radius, radius * 2, devDeviation);
}

Body createSemiSphere(const DeviationParams& devDeviation, double radius)
{
  Profile2D cSphereProfile;   // Create sphere profile
  cSphereProfile.resize(1);                 // With one contour

  cSphereProfile.front().appendVertex(OdGePoint2d(0., 0.)); // Add first point
  cSphereProfile.front().appendVertex(OdGePoint2d(radius, 0.), tan(OdaPI2/4));
  cSphereProfile.front().appendVertex(OdGePoint2d(0., radius));

  // Will be re reversed in revolution method.
  cSphereProfile.front().setOrientationAt(0, efoBack);
  cSphereProfile.front().setOrientationAt(1, efoBack);

  cSphereProfile.front().setClosed();       // Close profile
  cSphereProfile.front().makeCCW();         // Make contour outer

  // Create revolution body
  return Body::revolution(cSphereProfile, radius, radius, devDeviation);
}

Body createSpherePart(const DeviationParams& devDeviation, double radius)
{
  Profile2D cSphereProfile;   // Create sphere profile
  cSphereProfile.resize(1);                 // With one contour

  cSphereProfile.front().appendVertex(OdGePoint2d(0., 0.)); // Add first point
  cSphereProfile.front().appendVertex(OdGePoint2d(radius, 0.), tan(OdaPI2/4));
  cSphereProfile.front().appendVertex(OdGePoint2d(0., radius));

  // Will be re reversed in revolution method.
  cSphereProfile.front().setOrientationAt(0, efoBack);
  cSphereProfile.front().setOrientationAt(1, efoBack);
  cSphereProfile.front().setOrientationAt(2, efoBack);

  cSphereProfile.front().setClosed();       // Close profile
  cSphereProfile.front().makeCCW();         // Make contour outer

  const OdGeCircArc3d arcRotation(
    OdGePoint3d::kOrigin, 
    OdGeVector3d::kZAxis, 
    OdGeVector3d::kXAxis, 
    radius, 
    0., OdaPI2);

  // Create revolution body
  return Body::revolution(cSphereProfile, arcRotation, devDeviation);
}

Body createSnowman(const DeviationParams& devDeviation, double radius, double pressing)
{
  Body upper = createSphere(devDeviation, radius / 3);  // Snowman consists of 3 connected spheres
  Body middle = createSphere(devDeviation, radius / 2);
  Body lower = createSphere(devDeviation, radius);

  middle.transform(OdGeMatrix3d::translation(OdGeVector3d(0.0, 0.0, radius * 2 * (1 - pressing)))); // Move middle sphere up
  upper.transform(OdGeMatrix3d::translation(OdGeVector3d(0.0, 0.0, radius * 3 * (1 - pressing))));  // Move upper sphere up

  Body snowman = Body::boolOper(eUnion, upper, middle);   // Merge 2 spheres

  return Body::boolOper(eUnion, snowman, lower);                        // Merge previous result with last sphere
}

Body createDice(const DeviationParams& devDeviation, double edgeLen, double bevelCoef, double holeCoef)
{
  Body cube = createCube(devDeviation, edgeLen);                 // Base of dice with 6 faces
  Body spere = createSphere(devDeviation, edgeLen * bevelCoef);  // Sphere for chamfering
  Body hole = createSphere(devDeviation, edgeLen * holeCoef);    // Sphere for drilling imitation
  Body hole1 = hole;                                             // Working copy of hole

  OdGeMatrix3d move;

  // Move sphere center to (0.0, 0.0, 0.0)
  move.setToTranslation(OdGeVector3d(-edgeLen / 2, -edgeLen / 2, -edgeLen * holeCoef)); // Create translation matrix
  hole.transform(move);                                                        // Use translation matrix

  // 1
  move.setToTranslation(OdGeVector3d(edgeLen / 2, edgeLen / 2, 0.0));          // Move hole1 to center of 1-st face
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1); // Cut the hole

  // 2
  hole1 = hole;
  move.setToTranslation(OdGeVector3d(0.0, edgeLen / 3, edgeLen / 3));          // Place 2 holes on the face
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);

  // 3
  hole1 = hole;
  move.setToTranslation(OdGeVector3d(edgeLen / 4, 0.0, edgeLen / 4));          // Place 3 holes on the face
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);

  // 4
  hole1 = hole;
  move.setToTranslation(OdGeVector3d(edgeLen / 4, edgeLen, edgeLen / 4));      // Place 4 holes on the face
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  move.setToTranslation(OdGeVector3d(0.0, 0.0, edgeLen / 2));
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  move.setToTranslation(OdGeVector3d(edgeLen / 2, 0.0, 0.0));
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  move.setToTranslation(OdGeVector3d(0.0, 0.0, -edgeLen / 2));
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);

  // 5
  hole1 = hole;
  move.setToTranslation(OdGeVector3d(edgeLen, edgeLen / 4, edgeLen / 4));      // Place 5 holes on the face
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  move.setToTranslation(OdGeVector3d(0.0, 0.0, edgeLen / 2));
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  move.setToTranslation(OdGeVector3d(0.0, edgeLen / 2, 0.0));
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  move.setToTranslation(OdGeVector3d(0.0, 0.0, -edgeLen / 2));
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  move.setToTranslation(OdGeVector3d(0.0, -edgeLen / 4, edgeLen / 4));
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);

  // 6
  hole1 = hole;
  move.setToTranslation(OdGeVector3d(edgeLen / 3, edgeLen / 4, edgeLen));      // Place 6 holes on the face
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  move.setToTranslation(OdGeVector3d(0.0, edgeLen / 4, 0.0));
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  move.setToTranslation(OdGeVector3d(edgeLen / 3, 0.0, 0.0));
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  move.setToTranslation(OdGeVector3d(0.0, -edgeLen / 4, 0.0));
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);
  hole1.transform(move);
  cube = Body::boolOper(eDifference, cube, hole1);


  move.setToTranslation(OdGeVector3d(0.0, 0.0, edgeLen * 0.2));
  cube.transform(move);
  return Body::boolOper(eIntersection, cube, spere); // Make chamfer
}

Body createGear(const DeviationParams& devDeviation, double radius, double height, unsigned int toothCnt, double holeCoef, double innerThickness)
{
  double depth = 1.0 - (1.0 - innerThickness) / 2;
  double innerRadius = holeCoef * radius;

  Body groove = createPrism(devDeviation, 2 * radius / toothCnt, height, 6); // Groove for cutting
  Body cylinder = createCylinder(devDeviation, radius, height);              // Working cylinder
  Body gear = cylinder;                                                      // Result body

  // Calculate stepAngle from count of tooths
  double stepAngle = Oda2PI / toothCnt;
  // Create rotation matrix
  OdGeMatrix3d rotate = OdGeMatrix3d::rotation(stepAngle, OdGeVector3d::kZAxis, OdGePoint3d::kOrigin);
  // Move groove body to edge of cylinder
  groove.transform(OdGeMatrix3d::translation(OdGeVector3d(radius, 0.0, 0.0)));
  // Create toots by cutting out grooves
  for (unsigned int i = 0; i < toothCnt; i++)
  {
    // Rotate groove by step angle
    groove.transform(rotate); 
    // Cut groove from gear
    gear = Body::boolOper(eDifference, gear, groove);
  }

  // Create inner hole
  cylinder.transform(OdGeMatrix3d::scaling(OdGeScale3d(holeCoef, holeCoef, 1.0), OdGePoint3d::kOrigin));
  gear = Body::boolOper(eDifference, gear, cylinder);

  // Make central part of gear thinner
  cylinder.transform(OdGeMatrix3d::scaling(OdGeScale3d(0.8 / holeCoef, 0.8 / holeCoef, 1.0), OdGePoint3d::kOrigin));
  cylinder.transform(OdGeMatrix3d::translation(OdGeVector3d(0.0, 0.0, height* depth)));
  gear = Body::boolOper(eDifference, gear, cylinder);
  cylinder.transform(OdGeMatrix3d::translation(OdGeVector3d(0.0, 0.0, -2 * height* depth)));
  gear = Body::boolOper(eDifference, gear, cylinder);

  // Make keyway
  Body cuboid = createCuboid(devDeviation, OdGeVector3d(innerRadius / 2, innerRadius / 2, height));
  cuboid.transform(OdGeMatrix3d::translation(OdGeVector3d(innerRadius, 0.0, 0.0)));
  gear = Body::boolOper(eDifference, gear, cuboid);

  // Return result
  return gear;
}

Body createWrench(const DeviationParams& devDeviation, double radius, double length, double thickness)
{
  Body handle = createCuboid(devDeviation, OdGeVector3d(length, radius, thickness)); // Handle
  Body prism = createPrism(devDeviation, radius * 0.7, radius, 7);                   // Hexagonal prism
  Body cylinder = createCylinder(devDeviation, radius, thickness);                   // Base of U-shaped opening
  Body wrench;

  OdGeMatrix3d move, rotation; // matrices of transformations

  // Make ring spanner part
  Body jew1 = Body::boolOper(eDifference, cylinder, prism); 

  move.setToTranslation(OdGeVector3d(-radius / 2, 0.0, 0.0));
  prism.transform(move);

  // Make open-ended spanner part
  Body jew2 = Body::boolOper(eDifference, jew1, prism);     

  rotation.setToRotation(OdaPI / 6, OdGeVector3d::kZAxis, OdGePoint3d::kOrigin); // Rotate ends by some angle
  move.setToTranslation(OdGeVector3d(length / 2 + radius * 0.7, 0.0, 0.0));      // Move jew1 to right position
  jew1.transform(move * rotation);                                               // Apply transformations
  wrench = Body::boolOper(eUnion, handle, jew1);     // Merge jew1 with handle


  move.setToTranslation(OdGeVector3d(-(length / 2 + radius * 0.7), 0.0, 0.0));   // Move jew2 to right position
  jew2.transform(move * rotation);                                               // Apply transformations
  wrench = Body::boolOper(eUnion, wrench, jew2);     // Merge all parts together

  // return result body
  return wrench;
}

Body createLogo(const DeviationParams& devDeviation, double length, double thickness)
{
  Body logo;
  Body letterO;
  Body letterD;
  Body letterA;

  // Dimensions from ODA logo SVG 
  double majorRadius = 220.0;
  double minorRadius = 125.0;
  double stripeThick = 27.0;
  double maxX = 950.0;
  double maxY = 450.75;

  Body baseCylinder = createCylinder(devDeviation, majorRadius, thickness);  // Base of "O" and "D"
  Body innerCylinder = createCylinder(devDeviation, minorRadius, thickness); // Hole for "O" and "D"

  // Letter "O" 
  letterO = baseCylinder;
  letterO = Body::boolOper(eDifference, letterO, innerCylinder); // Get the usual letter O

  // stripe - the body that will be deducted from the letter "O"
  Body stripe = createCuboid(devDeviation, OdGeVector3d(majorRadius, stripeThick, thickness));

  stripe.transform(OdGeMatrix3d::translation(OdGeVector3d(-majorRadius / 2, 0.0, 0.0)));   // Move stripe
  letterO = Body::boolOper(eDifference, letterO, stripe);      // Cut stripe

  stripe.transform(OdGeMatrix3d::translation(OdGeVector3d(0.0, -minorRadius / 2, 0.0)));   // Move stripe
  letterO = Body::boolOper(eDifference, letterO, stripe);      // Cut stripe

  stripe.transform(OdGeMatrix3d::translation(OdGeVector3d(0.0, minorRadius, 0.0)));        // Move stripe
  letterO = Body::boolOper(eDifference, letterO, stripe);      // Cut stripe

  // Move letter to the right position
  letterO.transform(OdGeMatrix3d::translation(OdGeVector3d(majorRadius, majorRadius, 0.0))); 

  // Letter "D"
  Body cuboid = createCuboid(devDeviation, OdGeVector3d(majorRadius * 2, majorRadius * 2, thickness));
  cuboid.transform(OdGeMatrix3d::translation(OdGeVector3d(-majorRadius, 0.0, 0.0)));
  letterD = Body::boolOper(eUnion, baseCylinder, cuboid);

  cuboid.transform(OdGeMatrix3d::rotation(OdaPI4, OdGeVector3d::kZAxis, OdGePoint3d(-majorRadius, 0.0, 0.0)));
  letterD = Body::boolOper(eDifference, letterD, cuboid);

  letterD = Body::boolOper(eDifference, letterD, innerCylinder);
  cuboid = createCuboid(devDeviation, OdGeVector3d(minorRadius * 2, minorRadius * 2, thickness));
  cuboid.transform(OdGeMatrix3d::translation(OdGeVector3d(-minorRadius, 0.0, 0.0)));
  letterD = Body::boolOper(eDifference, letterD, cuboid);

  cuboid = createCuboid(devDeviation, OdGeVector3d(majorRadius * 2, majorRadius * 2, thickness));

  letterD = Body::boolOper(eIntersection, letterD, cuboid);

  // Move letter to the right position
  letterD.transform(OdGeMatrix3d::translation(OdGeVector3d(majorRadius *2.15, majorRadius, 0.0))); 

  // Letter "A"
  // Create letter "A" as extruded profile with array of vertices
  OdGePoint2dArray points;
  points.push_back(OdGePoint2d(950.0, 0.0));
  points.push_back(OdGePoint2d(744.75, 450.75));
  points.push_back(OdGePoint2d(683.25, 375.75));
  points.push_back(OdGePoint2d(825.75, 75.0));
  points.push_back(OdGePoint2d(696.0, 75.0));
  points.push_back(OdGePoint2d(627.75, 0.0));

  Profile2D cBase;          // Create base profile
  cBase.resize(1);                        // With one contour
  cBase.front().appendVertices(points);   // Add vertices
  cBase.front().setClosed();              // Make contour closed
  cBase.front().makeCCW();                // Force CCW

  // Perform extrusion
  letterA = Body::extrusion(cBase, OdGeVector3d::kZAxis * thickness, devDeviation);

  // Combine letters
  logo = Body::boolOper(eUnion, letterO, letterD);
  logo = Body::boolOper(eUnion, logo, letterA);

  // Fit body to specified length 
  logo.transform(OdGeMatrix3d::scaling(length / maxX, OdGePoint3d::kOrigin));

  return logo;
}

template< typename T, size_t N >
std::vector<T> makeVector(const T(&data)[N])
{
  return std::vector<T>(data, data + N);
}

Body createMeshTetrahedron(const DeviationParams& devDeviation)
{
  // Create array of vertices
  OdGePoint3d aVertices[] = {
    OdGePoint3d(77.0, 0.0,  0.0 ),
    OdGePoint3d(0.0,  70.0, 0.0 ),
    OdGePoint3d(77.0, 77.0, 0.0 ),
    OdGePoint3d(0.0,  77.0, 77.0),
  };

  // Create array with face data
  OdInt32 aFaceData[] = {
    3, 3, 2, 1,
    3, 1, 2, 0,
    3, 2, 3, 0,
    3, 3, 1, 0
  };

  // Optional: Create arrays of flags for faces, edges, vertices in raw format
  OdUInt32 aFaceFlags[] = {
    4, 32, 32, 32
  };
  OdUInt32 aEdgeFlags[] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
  };
  OdUInt32 aVertexFlags[] = {
    0, 0, 0, 0
  };

  // Optional: Specify colors of faces and edges
  OdUInt32 color0 = OdCmEntityColor(0xFF, 0xFF, 0xFF).color();
  OdUInt32 color1 = OdCmEntityColor(0xFF, 0xFF, 0xFF).color();
  OdUInt32 color2 = OdCmEntityColor(0xFF, 0xFF, 0xFF).color();
  OdUInt32 color3 = OdCmEntityColor(0xFF, 0xFF, 0xFF).color();

  OdUInt32 aFaceColors[] = {
    color0, color1, color2, color3
  };
  OdUInt32 aEdgeColors[] = {
    color0, color1, color2, color3,
    color0, color1, color2, color3,
    color0, color1, color2, color3,
    color0, color1, color2, color3
  };
  
  std::vector<OdUInt32> vecFaceFlags = makeVector(aFaceFlags);
  std::vector<OdUInt32> vecEdgeFlags = makeVector(aEdgeFlags);
  std::vector<OdUInt32> vecVertexFlags = makeVector(aVertexFlags);
  std::vector<OdUInt32> vecFaceColors = makeVector(aFaceColors);
  std::vector<OdUInt32> vecEdgeColors = makeVector(aEdgeColors);
  Body body = Body::createFromMesh(
    makeVector(aVertices), makeVector(aFaceData),
    &vecFaceFlags, &vecEdgeFlags, &vecVertexFlags,
    &vecFaceColors, &vecEdgeColors
    );

  return body;
}

Body createMeshCuboid(const DeviationParams& devDeviation)
{
  // Create array of vertices
  OdGePoint3d aVertices[] = {
    OdGePoint3d(0.0, 0.0, 0.0),
    OdGePoint3d(1.0, 0.0, 0.0),
    OdGePoint3d(1.0, 1.0, 0.0),
    OdGePoint3d(0.0, 1.0, 0.0),
    OdGePoint3d(0.0, 0.0, 2.0),
    OdGePoint3d(1.0, 0.0, 2.0),
    OdGePoint3d(1.0, 1.0, 2.0),
    OdGePoint3d(0.0, 1.0, 2.0),
  };

  // Create array with face data
  OdInt32 aFaceData[] = {
    4, 3, 2, 1, 0,
    4, 4, 5, 6, 7,
    4, 2, 3, 7, 6,
    4, 1, 2, 6, 5,
    4, 0, 1, 5, 4,
    4, 3, 0, 4, 7
  };

  Body body = Body::createFromMesh(
    makeVector(aVertices), makeVector(aFaceData)
  );

  return body;
}

Profile3D sliceBody(const DeviationParams& devDeviation)
{
  // Create plane
  OdGePlane plane(OdGePoint3d(1.,1.,1.), OdGeVector3d(5.,4.,3.));
  
  // Create body
  Body body = createCube(devDeviation);

  // Call slice method and receive resulting Profile3D
  Profile3D result;
  body.slice(plane, result, true);

  return result;
}

std::vector<OdGePoint3d> intersectBody(const DeviationParams& devDeviation)
{
  // Create body
  Body body = createCube(devDeviation);

  // Create linear entity
  OdGeLine3d line(OdGePoint3d::kOrigin, OdGePoint3d(0., 2., 4.));

  // Call intersectLine method and receive resulting points array
  std::vector<OdGePoint3d> points;
  body.intersectLine(line, points, true);

  return points;
}

double calculateBodyVolumeArea(const DeviationParams& devDeviation)
{
  // Create body
  Body body = createCube(devDeviation);
  Body* pBody = &body;

  double dAreaSum = 0.0;

  // Calculate area of all faces of the body with Body Face iterator
  FaceIterator iterFace(pBody);
  for (iterFace; !iterFace.done(); iterFace.next())
  {
    Face* pFace = iterFace.get();
    dAreaSum += pFace->area();
  }

  // Use volume method for finding volume
  return body.volume() / dAreaSum;
}

Body iterateBodyAndSetTags(const DeviationParams& devDeviation)
{
  // Create body
  Body body = createCube(devDeviation);

  //-----------------------------------------------------

  // Tag for assigning to entities
  // User can assign any pointer or TagType data
  unsigned int index = 0;

  Body* pBody = &body;

  // Set Body tag as index
  pBody->setTag(index++);

  // Body Face iterator
  FaceIterator iterFace(pBody);
  for (iterFace; !iterFace.done(); iterFace.next())
  {
    
    Face* pFace = iterFace.get();

    // Set Face tag as index
    pFace->setTag(index++);

    // Face Edge iterator
    EdgeFaceIterator iterEdge(pFace);
    for (iterEdge; !iterEdge.done(); iterEdge.next())
    {
      Edge* pEdge = iterEdge.get();
      
      // Set Edge tag as index
      pEdge->setTag(index++);

      Vertex* pVertex = pEdge->vertex();

      // Set the Vertex tag to the owner edge address
      pVertex->setTag((TagType)pEdge);
    }
  }

  //-----------------------------------------------------

  // Body Vertex iterator
  VertexIterator iterVertex(pBody);
  for (iterVertex; !iterVertex.done(); iterVertex.next())
  {
    Vertex* pVertex = iterVertex.get();

    // Get vertex tag and interpret as pointer to the edge
    Edge* pEdge = (Edge*)pVertex->tag();

    // Get edge tag and interpret as integer value
    index = (unsigned int)pEdge->tag();
  }

  //-----------------------------------------------------

  // Body Vertex iterator
  EdgeBodyIterator iterEdge(pBody);
  for (iterEdge; !iterEdge.done(); iterEdge.next())
  {
    Edge* pEdge = iterEdge.get();

    // Get edge tag and interpret as integer value
    index = (unsigned int)pEdge->tag();
  }

  //-----------------------------------------------------

  return body;
}

Body transformBody(const DeviationParams& devDeviation)
{
  // Create the body
  Body body = createCube(devDeviation);

  // Body can be transformed with any transformation matrix

  // Rotate the body
  OdGeMatrix3d mRotation;
  mRotation.setToRotation(OdaPI4, OdGeVector3d::kZAxis, OdGePoint3d::kOrigin);
  body.transform(mRotation);

  // Move the body
  OdGeMatrix3d mTranslation;
  mTranslation.setToTranslation(OdGeVector3d(10., 20., 30.));
  body.transform(mTranslation);

  // Scale the body
  OdGeMatrix3d mScale;
  mScale.setToScaling(35.0, OdGePoint3d::kOrigin);
  body.transform(mScale);

  // Scale the body
  OdGeMatrix3d mStretching;
  OdGeScale3d scale(0.1, 0.3, 0.5);
  mStretching.setToScaling(scale, OdGePoint3d::kOrigin);
  body.transform(mStretching);

  // Mirror the body
  OdGeMatrix3d mMirroring;
  OdGeLine3d mirroringLine(OdGePoint3d::kOrigin, OdGeVector3d(1., 2., 3.));
  mMirroring.setToMirroring(mirroringLine);
  body.transform(mMirroring);

  return body;
}

//
