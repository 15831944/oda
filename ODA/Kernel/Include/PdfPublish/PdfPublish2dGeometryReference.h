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


///////////////////////////////////////////////////////////////////////////////
//
// PdfPublish2dGeometryReference.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_2DGEOM_REF_INCLUDED_
#define _PDF_PUBLISH_2DGEOM_REF_INCLUDED_

#include "PdfPublishCommon.h"
#include "PdfPublishBaseObject.h"
#include "PdfPublish2dGeometryBlock.h"

/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements a reference to the two-dimensional geometry in .pdf documents created with Publish SDK.
  The geometry options for this reference can be set by setting the transformation matrix, scale, rotation and translation. 
  These options are applied to the rendering of the geometry referenced by the Od2dGeometryReference object. 
  
  \remarks 
  For example, assume that a document has an Od2dGeometryBlock object with size 10x10 units.
  If you create a reference to it represented with an Od2dGeometryReference object and apply the scale 2.0 to it, Publish will render a block with size 20x20 units. 
*/
class PDFPUBLISH_TOOLKIT Od2dGeometryReference : public OdObject
{
protected:
  //DOM-IGNORE-BEGIN
  Od2dGeometryReference();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(Od2dGeometryReference);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys the reference to a geometry object.
  */
  virtual ~Od2dGeometryReference();

  /** \details
    Sets a new value of the border flag for the geometry object reference.
    \param state [in] A new flag value.
    \remarks
    If the object has a border, the flag value is equal to true, otherwise it is equal to false.
  */
  void setBorder(bool state);

  /** \details
    Sets a new color for the geometry reference object.
    \param color [in] A new color value to be set.
  */
  void setBorderColor(ODCOLORREF color);

  /** \details
    Returns the current value of the border flag for the geometry reference object.
    The method fills a passed value and returns it to a calling subroutine.
    \param state [out] A placeholder for the border flag value.
    \remarks
    If the object has a border, returns true, otherwise returns false.
  */
  void getBorder(bool& state) const;

  /** \details
    Returns the current border color for the geometry reference object.
    The method fills a passed value and returns it to a calling subroutine.
    \param color [out] A placeholder for the border color value.
  */
  void getBorderColor(ODCOLORREF& color) const;

  /** \details
    Sets a new transfomation matrix for the geometry reference object.
    \param xfm [in] A transformation matrix object.
  */
  void setTransform(const OdGeMatrix2d& xfm);

  /** \details
    Returns the current transformation matrix for the geometry reference object.
    The method fills a passed matrix object and returns it to a calling subroutine.
    \param xfm [out] A placeholder for the transformation matrix.
  */
  void getTransform(OdGeMatrix2d& xfm) const;

  /** \details
    Sets a new geometry block to be referenced.
    \param block [in] A smart pointer to a geometry block object.
  */
  void setGeometryBlock(const Od2dGeometryBlockPtr& block);

  /** \details
    Returns the currently referenced geometry block.
    The method fills a passed smart pointer object and returns it to a calling subroutine.
    \param block [out] A placeholder for a smart pointer to a geometry block object.
  */
  void getGeometryBlock(Od2dGeometryBlockPtr& block) const;

  /** \details
    Sets a scale value for the geometry reference object.
    \param scale [in] A scale value to be set.
    \remarks
    If the new scale value is equal to 0, it means that the object is scaled to its rectangle.
  */
  void setScale(double scale);

  /** \details
    Returns the current scaling value for the geometry block reference.
    The method fills a passed value and returns it to a calling subroutine.
    \param scale [out] A placeholder for the scale value.
  */
  void getScale(double& scale) const;

  /** \details
    Sets a new rotation angle (in radians) for the geometry reference object.
    \param rotation [in] A rotation angle value to be set.
  */
  void setRotation(double rotation = 0);

  /** \details
    Returns the current rotation angle for the geometry block reference.
    The method fills a passed value and returns it to a calling subroutine.
    \param rotation [out] A placeholder for the rotation angle value.
  */
  void getRotation(double& rotation) const;

  /** \details
    Sets a new translation vector for the geometry reference object.
    \param vec [in] A translation vector to be set.
  */
  void setTranslation(const OdGeVector2d& vec);

  /** \details
    Returns the current translation vector for the geometry block reference.
    The method fills a passed value and returns it to a calling subroutine.
    \param vec [out] A placeholder for the translation vector.
  */
  void getTranslation(OdGeVector2d& vec) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::Od2dGeometryReference, Od2dGeometryReference> object.
*/
SMARTPTR(Od2dGeometryReference);

}
#endif // _PDF_PUBLISH_2DGEOM_REF_INCLUDED_

