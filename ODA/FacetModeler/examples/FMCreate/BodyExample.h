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

#pragma once

#include "Modeler/FMMdlBody.h"

FacetModeler::Body createCube(
  const FacetModeler::DeviationParams& devDeviation,
  double edgeLen = 200.0
);

FacetModeler::Body createCuboid(
  const FacetModeler::DeviationParams& devParams,
  OdGeVector3d vSizes = OdGeVector3d(50.0, 100.0, 150.0)
);

FacetModeler::Body createTetrahedron(
  const FacetModeler::DeviationParams& devDeviation,
  double edgeLen = 200.0
);

FacetModeler::Body createPyramid(
  const FacetModeler::DeviationParams& devDeviation,
  double radius = 200.0,
  double height = 300.0,
  unsigned int sidesCnt = 7
);

FacetModeler::Body createCone(
  const FacetModeler::DeviationParams& devDeviation,
  double radius = 100.0,
  double height = 250.0
);

FacetModeler::Body createPrism(
  const FacetModeler::DeviationParams& devDeviation,
  double radius = 100.0,
  double height = 250.0,
  unsigned int sidesCnt = 7
);

FacetModeler::Body createCylinder(
  const FacetModeler::DeviationParams& devDeviation,
  double radius = 100.0,
  double height = 250.0
);

FacetModeler::Body createSphere(
  const FacetModeler::DeviationParams& devDeviation,
  double radius = 150.0
);

FacetModeler::Body createSemiSphere(
  const FacetModeler::DeviationParams& devDeviation,
  double radius = 300.0
);

FacetModeler::Body createSpherePart(
  const FacetModeler::DeviationParams& devDeviation,
  double radius = 300.0
);

FacetModeler::Body createSnowman(
  const FacetModeler::DeviationParams& devDeviation,
  double radius = 100.0,
  double pressing = 0.1
);

FacetModeler::Body createDice(
  const FacetModeler::DeviationParams& devDeviation,
  double edgeLen = 300.0,
  double bevelCoef = 0.7,
  double holeCoef = 0.08
);

FacetModeler::Body createGear(
  const FacetModeler::DeviationParams& devDeviation,
  double radius = 100.0,
  double height = 30.0,
  unsigned int toothCnt = 20,
  double holeCoef = 0.3,
  double innerThickness = 0.7
);

FacetModeler::Body createWrench(
  const FacetModeler::DeviationParams& devDeviation,
  double radius = 200.0,
  double length = 800.0,
  double thickness = 70.0
);

/*
            `.-:++oo++/:.`    .+oooooooooo+/:-``             .oo`
       `-+yhdddddddddddddyo:. -sddddddddddddddhs/-        `/dddo`
     .+hddddddddddddddddddddds:`-sddddddddddddddddy+.    -ydddddo
   .oddddddddddddddddddddddddddy:`:yddddddddddddddddh+`  /dddddddo`
  :hddddddddhs+:----:/shdddddddddo``.----:/+ydddddddddh-  +dddddddo`
 -yyyyyyyys/.          `-oddddddddy`         ./hdddddddh:  /dddddddo`
``......``                -ydddddddo`          `odddddddh.  +ddddddd+`
/ooooooo:                  .hddddddd-           `odddddddo   +dddddddo
::::::::`                   /ddddddd/            `dddddddy`   +ddddddd+`
:::::::-                    /ddddddd+            `dddddddh`    +ddddddd+
+ooooooo.                   sddddddd:            /ddddddds`    `+ddddddd+`
`.......``                `+dddddddy`           :hddddddd:       +ddddddd+`
 +yyyyyyyy/`             -sdddddddd-          .oddddddddo        `+ddddddd+
 `sddddddddho:.`    `.:+yddddddddh-       `-/shdddddddd+`         `+ddddddd+
   /hddddddddddhhyyhhdddddddddddo.`/yyyyyhdddddddddddy:   ``````````oddddddd+
    `+hddddddddddddddddddddddds-`:yddddddddddddddddy/`  .sddddddddddddddddddd+
      `:ohddddddddddddddddds/. -ydddddddddddddddho-`  .odddddddddddddddddddddd+
          .:+syhddddhyso/-`  .sdddddddddddhys+:.`   `oddddddddddddddddddddddddd+
*/
FacetModeler::Body createLogo(
  const FacetModeler::DeviationParams& devDeviation, 
  double length = 1000,
  double thickness = 30.0
);

FacetModeler::Body createMeshTetrahedron(
  const FacetModeler::DeviationParams& devDeviation
);

FacetModeler::Body createMeshCuboid(
  const FacetModeler::DeviationParams& devDeviation
);

FacetModeler::Profile3D sliceBody(
  const FacetModeler::DeviationParams& devDeviation
);


std::vector<OdGePoint3d> intersectBody(
  const FacetModeler::DeviationParams& devDeviation
);

double calculateBodyVolumeArea(
  const FacetModeler::DeviationParams& devDeviation
);

FacetModeler::Body iterateBodyAndSetTags(
  const FacetModeler::DeviationParams& devDeviation
);
//
