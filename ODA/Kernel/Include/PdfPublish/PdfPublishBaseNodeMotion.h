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
// PdfPublishBaseNodeMotion.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_BASE_NODE_MOTION_INCLUDED_
#define _PDF_PUBLISH_BASE_NODE_MOTION_INCLUDED_

#include "PdfPublishCommon.h"
#include "PdfPublishDef.h"
#include "SharedPtr.h"
#include "OdArray.h"
#include "Ge/GeMatrix3d.h"


/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements a base interface for node motion handling during an animation in .pdf documents created with Publish SDK.
  \remarks
  To implement functionality of handling motion of an assembly node, define a class inherited from the OdBaseNodeMotion class
  and add the implementation of the step() method.
  The parametric animation algorithm calls the step() method for each frame, and if it returns true, calls other methods of the inherited class.
  Each interface method accepts an output parameter that affects the animation only if the appropriate method returns true.
  Default implementation in the base interface class assumes that all methods return false. 
  This means that you have to re-implement methods of the base class to correctly define the animation parameters. 
*/
class PDFPUBLISH_TOOLKIT OdBaseNodeMotion 
{
//DOM-IGNORE-BEGIN
protected:
  OdBaseNodeMotion();
//DOM-IGNORE-END
public:
//DOM-IGNORE-BEGIN
  ODRX_HEAP_OPERATORS();
//DOM-IGNORE-END

  /** \details
    Destroys the node motion object.
  */
  virtual ~OdBaseNodeMotion();

  /** \details
    Returns the current node name. 
    The method fills a passed string value with the node motion object's name then returns it to a calling subroutine.
    \param name [out] A placeholder for the node motion object's name.
    \returns Returns true if the node motion object's name has been successfully returned; otherwise returns false.
    \remarks 
    This method should be re-implemented and return true if you want to set the animated node name.
    Default implementation returns false.
  */
  virtual bool getName(OdString& name);
  
  /** \details
    Returns the current transformation matrix for the node motion object. 
    The method accepts a transformation matrix, fills it and then returns it to a calling subroutine.
    \param matrix [out] A placeholder for the node motion transformation matrix object.
    \returns Returns true if the node transformation should be taken into account during the animation; otherwise it returns false.
    \remarks 
    This method should be re-implemented and return true if you want the node animation algorithm to take the transformation into account.
    Default implementation returns false.
  */
  virtual bool getTransformation(OdGeMatrix3d& matrix);
  
  /** \details
    Returns the current node motion color. 
    The method fills a passed ODCOLORREF value with the current node motion's color then returns it to a calling subroutine.
    \param color [out] A placeholder for the node motion color value.
    \returns Returns true if the node color should be taken into account during the animation; otherwise it returns false.
    \remarks 
    This method should be re-implemented and return true if you want the node animation algorithm to take the node color into account.
    Default implementation returns false.
  */  
  virtual bool getColor(ODCOLORREF& color);
  
  /** \details
    Returns the current opacity level for the node motion object. 
    The method fills a passed double value with the opacity value then returns it to a calling subroutine.
    \param opacity [out] A placeholder for the opacity value.
    \returns Returns true if the node opacity should be taken into account during the animation; otherwise it returns false.
    \remarks 
    This method should be re-implemented and return true if you want the node animation algorithm to take the node opacity level into account.
    Default implementation returns false.
  */
  virtual bool getOpacity(double& opacity);

  /** \details
    Moves the node animation to a specified frame. 
    \param frame [in] A frame index to move the animation to.
    \returns Returns true if the node animation should be moved to the specified frame; otherwise it returns false.
    \remarks 
    This method is mandatory for a re-implementation when defining a custom class inherited from the OdBaseNodeMotion class.
    To advance the node animation, this method must return true. Default implementation returns false.
  */
  virtual bool step(const OdUInt64& frame);
  
};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdBaseNodeMotion, OdBaseNodeMotion> object.
*/
typedef OdSharedPtr<OdBaseNodeMotion> OdBaseNodeMotionPtr;

/** \details
 A data type that represents an array of smart pointers to an <link OdPdfPublish::OdBaseNodeMotion, OdBaseNodeMotion> object.
*/
typedef OdArray<OdBaseNodeMotionPtr> OdBaseNodeMotionPtrArray;

}
#endif // _PDF_PUBLISH_BASE_NODE_MOTION_INCLUDED_

