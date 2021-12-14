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



#ifndef _INC_DDBRFACE_3F82D5A203C8_INCLUDED
#define _INC_DDBRFACE_3F82D5A203C8_INCLUDED

#include "Br/BrEntity.h"
#include "Br/BrEnums.h"
#include "Ge/GeSurface.h"
#include "Ge/GeNurbSurface.h"

#include "TD_PackPush.h"

class OdCmEntityColor;
class OdGeExternalBoundedSurface;

/** \details
    This class is the interface class for BREP faces.

    \remarks
    Faces are primary BREP elements with associated geometries: surfaces. 
    
    \sa
    TD_Br
    
    <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrFace : public OdBrEntity
{
public:

  /** \details
    Returns the underlying surface of this Face entity as an external OdGeNurbSurface.
    
    \param nurb [out]  Receives the NURBS surface.
  */
  OdBrErrorStatus getSurfaceAsNurb(OdGeNurbSurface& nurb) const;

  /** \details
    Returns the underlying surface of this Face entity as an OdGeSurface.
  */
  OdGeSurface* getSurface() const;

  /** \details
    Returns the best match of the underlying surface of this Face entity to surfaces supported by OdGe.
    \param surfaceType [out]  Receives the surface type.
  */
  OdBrErrorStatus getSurfaceType(OdGe::EntityId& surfaceType) const;

  /** \details
     Provides the surface area of the face as a double value.
    
     \param area        [out] Surface area of the face.
     \param tolRequired  [in] Optional tolerance required for the result of the calculation.
     \param tolAchieved [out] Optional tolerance achieved in the calculation.
     \returns
     * odbrNotImplementedYet - if the method is not implemented for the current modeler;
     * odbrUninitialisedObject - if object is uninitalised;
     * odbrInvalidInput - if calculation unsuccess;
     * odbrOK - if the calculation was successful;

     \remarks
     If the surface area property is not applicable to the entity, odbrNotApplicable is returned.
     If an error occurs, the value of the surface area argument remains unchanged.
  */
  OdBrErrorStatus	getArea(double& area,
    const double* tolRequired = NULL,
    double* tolAchieved = NULL) const;

  /** \details
      Returns true if and only if the outside of the face is in
      the direction of the surface normal.
  */
  bool getOrientToSurface() const;
 /** \details
    Returns the color of this Face entity.
   
    \returns
    Returns true if and only if this Face entity has a color associated with it.
    \param color [out]  Receives the color.
 */
  bool getColor(OdCmEntityColor &color) const;

  /** \details
  Returns the *material handle* of this Face entity.

  \returns
  Returns true if and only if this Face entity has a material associated with it.
  \param id [out]  Receives the id.
  */
  bool getMaterialID(OdUInt64 &id) const;

  /** \details
  Returns the *material name* of this Face entity.

  \returns
  Returns true if and only if this Face entity has a material associated with it.
  \param strMatName [out]  Receives the *material name*.
  */
  bool getMaterialString(OdString& strMatName) const;

  /** \details
  Returns the *material mapper* of this Face entity.

  \returns
  Returns true if and only if this Face entity has a *material mapper* associated with it.
  \param mx [out]  Receives the *transformation matrix*.
  \param projection [out]  Receives the projection flag.
  \param tiling [out]  Receives the tiling flag.
  \param autoTransform [out]  Receives the *auto transform* flag.
  */

  enum Projection
  {
    kInheritProjection  = 0, // Inherits *projection* from the current material's mapper.
    kPlanar             = 1, // Maps directly to XY coordinates.
    kBox                = 2, // Maps to planes perpendicular to major axes.
    kCylinder           = 3, // Maps to cylinder aligned with Z-axis.
    kSphere             = 4  // Maps to sphere aligned with Z-axis
  };

  enum Tiling
  {
    kInheritTiling    = 0, // Inherits *tiling* from the current material's mapper.
    kTile             = 1, // Repeats map along image axes.
    kCrop             = 2, // Crops map < 0.0 or > 1.0 on image axes.
    kClamp            = 3, // Clamps (stretches) map between 0.0 and 1.0 on image axes.
    kMirror           = 4  // Mirror the material map at every integer boundary.
  };

  enum AutoTransform
  {
    kInheritAutoTransform = 0x0, // Inherits automatic *transform* from the current material/s mapper.
    kNone                 = 0x1, // No automatic *transform*.
    kObject               = 0x2, // Adjusts the mapper *transform* to align with and fit the current object.
    kModel                = 0x4  // Multiples the mapper *transform* by the current block *transform*.
  };

  bool getMaterialMapper(OdGeMatrix3d &mx, Projection &projection, Tiling& tiling, AutoTransform &autoTransform) const;

  OdBrErrorStatus getSurfaceAsTrimmedNurbs(OdUInt32& numNurbs, OdGeExternalBoundedSurface**& ppNurbs) const;

  /** \details
   BimRv specific. Returns entity id that contains filling data for the face.

  \returns
    Returns true if there is filling entity assosiated with the face and false otherwise. 
  \param
    id [out]  Receives the requested id.
  */
  bool getFillPatternId(OdUInt64 &id) const;
  
  /** \details
    BimRv specific. Returns filling attributes for the face.

  \returns
    Returns true if there are filling attributes assosiated with the face and false otherwise.
  \param 
    origin [out] Receives filling origin attribute.
    dir [out] Receives filling direction attribute.
  */
  bool getFillingAttributes(OdGePoint2d &origin, OdGeVector2d &dir) const;

  /** \details
    BimRv specific. Returns extra filling attributes for the face. 
    Extra filling attributes are supposed to be special lineweight, color and transparency 
    for the filling that differs with such of entity.

  \returns
    Returns true if there are extra filling attributes assosiated with the face and false otherwise.
  \param
    pFill [inout] OdGiFill that is written with extra filling attribute.
  */
  bool getExtraFillingAttributes(OdGiFill* pFill) const;

  OdBrFace();
#ifdef OD_ADD_MOVE_CTOR_HAVE_DEF_DEL_FUNC
  //DOM-IGNORE-BEGIN
  OdBrFace(const OdBrFace&) = default;
  OdBrFace& operator=(const OdBrFace&) = default;
  OdBrFace(OdBrFace&&) = default;
  OdBrFace& operator=(OdBrFace&&) = default;
  //DOM-IGNORE-END
#endif

  ~OdBrFace();
};

#include "TD_PackPop.h"

#endif /* _INC_DDBRFACE_3F82D5A203C8_INCLUDED */

