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
// PdfPublishCADReference.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_CADREFERENCE_INCLUDED_
#define _PDF_PUBLISH_CADREFERENCE_INCLUDED_

#include "PdfPublishCommon.h"
#include "PdfPublishBaseObject.h"
#include "PdfPublishCADDefinition.h"


/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements a reference to a CAD object in .pdf documents created with Publish SDK.
  The interface of this class sets a visual representation of any two-dimensional CAD object 
  by setting its transformation, scale, rotation angle and translation. 
  The target object is defined with the <link OdPdfPublish::OdCADDefinition, OdCADDefinition class>, and the 
  instance of OdCADReference just refers to it. Any modifications are applied to the visual representation, not to 
  the target object. 
  For example, if you have an object with dimension 10x10 units and create a reference to it through the instance of the 
  OdCADReference class, then call the setScale() method with the input parameter value equal to 2.0, 
  the reference object in the document will have a dimension of 20x20 units. 
*/
class PDFPUBLISH_TOOLKIT OdCADReference : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdCADReference();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdCADReference);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys the CAD reference.
  */
  virtual ~OdCADReference();

  /** \details
    Sets the CADDefinition object for the CAD reference.
    \param cad_definition [in] A new CADDefinition object.
  */
  void setCADDefinition(OdCADDefinitionPtr cad_definition);
  /** \details
    Sets the border flag for the CAD reference.
    \param state [in] A new value of the flag.
    \remarks
    The border flag determines whether the CAD reference is rendered in the 
    document with a border. If the flag is true, the CAD reference has a border, otherwise it does not.
  */
  void setBorder(bool state);
  
  /** \details
    Sets the border color for the CAD reference.
    \param color [in] A new value of the border color.
  */
  void setBorderColor(ODCOLORREF color);
  
  /** \details
    Sets the border width for the CAD reference.
    \param width [in] A new value of the border width.
  */
  void setBorderWidth(const OdUInt16 width);

  /** \details
    Sets the rotation angle for the CAD reference. 
    The angle is set in radians.
    \param rotation [in] A rotation angle value.
  */
  void setRotation(double rotation);

  /** \details
    Sets the clipping boundary for the CAD reference. 
    \param units        [in] A unit used for the clipping boundary definition.
    \param clipBoundary [in] An array of two-dimensional points, which defines the clipping boundary.
  */
  void setClipBoundary(const Page::PaperUnits units, const OdGePoint2dArray& clipBoundary);

  /** \details
    Sets the border flag value for the CAD reference's clipping boundary. 
    The border flag determines whether the clipping boundary has a border.
    If the flag value is true, the boundary has a border, otherwise (when flag value is false) it has no border.
    \param state [in] A flag value.
  */
  void setClipBoundaryBorder(bool state);

  /** \details
    Sets the border color for the CAD reference's clipping boundary. 
    \param color [in] A new color value for the border.
  */
  void setClipBoundaryBorderColor(ODCOLORREF color);

  /** \details
    Sets the border width for the CAD reference's clipping boundary. 
    \param width [in] A new value for the border width.
    \param clipBoundary [in] An array of two-dimensional points, which defines the clipping boundary.
  */
  void setClipBoundaryBorderWidth(const OdUInt16 width);

  /** \details
    Sets the transformation matrix for the CAD reference. 
    \param xfm [in] A transformation matrix object.
    \remarks 
    If you use the <link !!OVERLOADED_addCADReference_OdPdfPublish::OdPage, addCADReference()> method of the <link OdPdfPublish::OdPage, OdPage class> to add a CAD reference to a page, this method has no effect.
  */
  void setTransform(const OdGeMatrix2d& xfm);

  /** \details
    Sets the the CAD reference scale. 
    \param scale [in] A new scale value.
    \remarks 
    If you use the <link !!OVERLOADED_addCADReference_OdPdfPublish::OdPage, addCADReference()> method of the <link OdPdfPublish::OdPage, OdPage class> to add a CAD reference to a page, this method has no effect.
  */
  void setScale(double scale);

  /** \details
    Sets the translation vector for the CAD reference. 
    \param vec [in] A new translation vector object.
    \remarks 
    If you use the <link !!OVERLOADED_addCADReference_OdPdfPublish::OdPage, addCADReference()> method of the <link OdPdfPublish::OdPage, OdPage class> to add a CAD reference to a page, this method has no effect.
  */
  void setTranslation(const OdGeVector2d& vec);

  /** \details
    Returns the current CAD definition object for the CAD reference.
    \param cad_definition [out] A placeholder for the CAD definition object.
  */
  void getCADDefinition(OdCADDefinitionPtr& cad_definition) const;

  /** \details
    Returns the current value of the border flag for the CAD reference.
    \param state [out] A placeholder for the border flag value.
    \remarks
    The border flag determines whether the CAD reference is rendered in the 
    document with a border. If the flag is true, the CAD reference has a border, otherwise it does not.
  */
  void getBorder(bool& state) const;

  /** \details
    Returns the current border color for the CAD reference.
    \param color [out] A placeholder for the border color value.
  */
  void getBorderColor(ODCOLORREF& color) const;

  /** \details
    Returns the current border width for the CAD reference.
    \param width [out] A placeholder for the border width value.
  */
  void getBorderWidth(OdUInt16& width) const;

  /** \details
    Returns the current rotation angle for the CAD reference in radians.
    \param rotation [out] A placeholder for the rotation angle value.
  */
  void getRotation(double& rotation) const;

  /** \details
    Returns the current clipping boundary for the CAD reference.
    \param units        [out] A placeholder for the unit value.
    \param clipBoundary [out] A placeholder for the two-dimensional points array that defines the clipping boundary.
  */
  void getClipBoundary(Page::PaperUnits& units, OdGePoint2dArray& clipBoundary) const;

  /** \details
    Returns the current value of the border flag for the CAD reference's clipping boundary.
    The border flag determines whether the clipping boundary has a border.
    If the flag value is true, the boundary has a border, otherwise (when flag value is false) it has no border.
    \param state [out] A placeholder for the flag value.
  */
  void getClipBoundaryBorder(bool& state) const;

  /** \details
    Returns the current border color for the CAD reference's clipping boundary.
    \param color [out] A placeholder for the border color value.
  */
  void getClipBoundaryBorderColor(ODCOLORREF& color) const;

  /** \details
    Returns the current border width for the CAD reference's clipping boundary.
    \param width [out] A placeholder for the border width value.
  */
  void getClipBoundaryBorderWidth(OdUInt16& width) const;

  /** \details
    Returns the current transformation matrix for the CAD reference.
    \param xfm [out] A placeholder for the transformation matrix.
  */
  void getTransform(OdGeMatrix2d& xfm) const;

  /** \details
    Returns the current scale value for the CAD reference.
    \param scale [out] A placeholder for the scale value.
  */
  void getScale(double& scale) const;

  /** \details
    Returns the current translation vector for the CAD reference.
    \param vec [out] A placeholder for the translation vector.
  */
  void getTranslation(OdGeVector2d& vec) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdCADReference, OdCADReference> object.
*/
SMARTPTR(OdCADReference);

}
#endif // _PDF_PUBLISH_CADREFERENCE_INCLUDED_

