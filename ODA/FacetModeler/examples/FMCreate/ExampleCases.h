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

//           example name     description                                                   called method   
FMCREATECASE(Cube,            "single body - an example of using the Body::box",            CreateBodyExample  )
FMCREATECASE(Cuboid,          "single body - an example of using the Body::box",            CreateBodyExample  )
FMCREATECASE(Tetrahedron,     "single body - an example of using the Body::pyramid",        CreateBodyExample  )
FMCREATECASE(Pyramid,         "single body - an example of using the Body::pyramid",        CreateBodyExample  )
FMCREATECASE(Cone,            "single body - an example of using the Body::pyramid",        CreateBodyExample  )
FMCREATECASE(Prism,           "single body - an example of using the Body::extrusion",      CreateBodyExample  )
FMCREATECASE(Cylinder,        "single body - an example of using the Body::extrusion",      CreateBodyExample  )
FMCREATECASE(Sphere,          "single body - an example of using the Body::revolution",     CreateBodyExample  )
FMCREATECASE(SemiSphere,      "single body - an example of using the Body::revolution",     CreateBodyExample  )
FMCREATECASE(SpherePart,      "single body - an example of using the Body::revolution",     CreateBodyExample  )
FMCREATECASE(Snowman,         "single body - an example of using the Body::boolOper",       CreateBodyExample  )
FMCREATECASE(Dice,            "single body - an example of using the Body::boolOper",       CreateBodyExample  )
FMCREATECASE(Gear,            "single body - an example of using the Body::boolOper",       CreateBodyExample  )
FMCREATECASE(Wrench,          "single body - an example of using the Body::boolOper",       CreateBodyExample  )
FMCREATECASE(Logo,            "single body - an example of using the Body::boolOper",       CreateBodyExample  )
FMCREATECASE(MeshTetra,       "single body - an example of using the Body::createFromMesh", CreateBodyExample  )
FMCREATECASE(MeshCuboid,      "single body - an example of using the Body::createFromMesh", CreateBodyExample  )
FMCREATECASE(SliceBody,       "single profile - an example of using the Body::slice",       propsBodyExample )
FMCREATECASE(IsectBody,       "points array - an example of using the Body::intersectLine", propsBodyExample )
FMCREATECASE(VolumeBody,      "double value - an example of using the Body::volume",        propsBodyExample )
FMCREATECASE(SetTags,         "iterates all body entities and sets tags",                   propsBodyExample )
FMCREATECASE(Square,          "single profile - an example of contour2d/profile2d creation",          CreateContourExample )
FMCREATECASE(Circle,          "single profile - an example of contour2d/profile2d creation",          CreateContourExample )
FMCREATECASE(Polygon,         "single profile - an example of contour2d/profile2d creation",          CreateContourExample )
FMCREATECASE(NutProfile,      "single profile - an example of using the Profile2D::PerformOperation", CreateContourExample )

#undef FMCREATECASE
