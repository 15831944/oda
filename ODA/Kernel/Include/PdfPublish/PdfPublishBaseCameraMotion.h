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
// PdfPublishBaseCameraMotion.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_BASE_CAMERA_MOTION_INCLUDED_
#define _PDF_PUBLISH_BASE_CAMERA_MOTION_INCLUDED_

#include "PdfPublishCommon.h"
#include "PdfPublishDef.h"
#include "SharedPtr.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"


/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements a base interface for camera motion handling during animation in .pdf documents created with Publish SDK.
  \remarks
  To implement functionality for handling motion of a camera, define a class inherited from the OdBaseCameraMotion class
  and add the implementation of the step() method.
  The parametric animation algorithm calls the step() method for each frame, and if it returns true, calls other methods of the inherited class.
  Each interface method accepts an output parameter that affects the animation only if the appropriate method returns true.
  The default implementation in the base interface class assumes that all methods return false. 
  This means that you have to re-implement methods of the base class to correctly define the animation parameters. 
*/
class PDFPUBLISH_TOOLKIT OdBaseCameraMotion 
{
//DOM-IGNORE-BEGIN
protected:
  OdBaseCameraMotion();
//DOM-IGNORE-END
public:
//DOM-IGNORE-BEGIN
  ODRX_HEAP_OPERATORS();
//DOM-IGNORE-END

  /** \details
    Destroys the camera motion object.
  */
  virtual ~OdBaseCameraMotion();

  /** \details
    Returns the current camera projection type. 
    The method fills a passed value with the camera projection type value then returns it to a calling subroutine.
    \param type [out] A placeholder for the projection type value.
    \returns Returns true if the projection type value should be taken into account during the animation; otherwise returns false.
    \remarks 
    This method should be re-implemented and return true if you want to affect the camera projection during the animation.
    Default implementation returns false.
  */
  virtual bool getProjection(Camera::Projection& type);
  
  /** \details
    Returns the current camera position. 
    The method fills a passed point with the camera position point then returns it to a calling subroutine.
    \param position [out] A placeholder for the camera position.
    \returns Returns true if the position value should be taken into account during the animation; otherwise returns false.
    \remarks 
    This method should be re-implemented and return true if you want to affect the camera position during the animation.
    Default implementation returns false.
  */
  virtual bool getPosition(OdGePoint3d& position);
  
  /** \details
    Returns the current camera target. 
    The method fills a passed point with the camera target point then returns it to a calling subroutine.
    \param target [out] A placeholder for the camera target.
    \returns Returns true if the camera target should be taken into account during the animation; otherwise returns false.
    \remarks 
    This method should be re-implemented and return true if you want to affect the camera target during the animation.
    Default implementation returns false.
  */
  virtual bool getTarget(OdGePoint3d& target);
  
  /** \details
    Returns the current camera up-vector. 
    The method fills a passed vector with the camera up-vector object then returns it to a calling subroutine.
    \param vector [out] A placeholder for the camera up-vector object.
    \returns Returns true if the up-vector should be taken into account during the animation; otherwise returns false.
    \remarks 
    This method should be re-implemented and return true if you want to affect the camera up-vector during the animation.
    Default implementation returns false.
  */
  virtual bool getUpVector(OdGeVector3d& vector);
  
  /** \details
    Returns the current camera target node. 
    The method fills a passed string with the name of the camera target node then returns it to a calling subroutine.
    \param node_name [out] A placeholder for the name of the camera target node.
    \returns Returns true if the target node should be taken into account during the animation; otherwise returns false.
    \remarks 
    This method should be re-implemented and return true if you want to specify the camera target during the animation.
    Default implementation returns false.
  */
  virtual bool getTargetNode(OdString& node_name);
  
  /** \details
    Returns the current size of the camera's view plane. 
    The method fills a passed value with the size of the camera view plane then returns it to a calling subroutine.
    \param size [out] A placeholder for the camera view plane size.
    \returns Returns true if the view plane size should be taken into account during the animation; otherwise returns false.
    \remarks 
    This method should be re-implemented and return true if you want to affect the view plane size during the animation.
    Default implementation returns false.
  */
  virtual bool getViewPlaneSize(double& size);
  
  /** \details
    Returns the current field of view angle of the camera. 
    The method fills a passed value with the field of view angle then returns it to a calling subroutine.
    \param size [out] A placeholder for the camera field of view angle.
    \returns Returns true if the camera's field of view should be taken into account during the animation; otherwise returns false.
    \remarks 
    This method should be re-implemented and return true if you want to handle the camera field of view during the animation.
    Default implementation returns false.
  */
  virtual bool getFov(double& size);

  /** \details
    Moves the animation to a specified frame. 
    \param frame [in] A frame index to move the animation to.
    \returns Returns true if the animation should be moved to the specified frame; otherwise returns false.
    \remarks 
    This method is mandatory for a re-implementation when defining a custom class inherited from the OdBaseCameraMotion class.
    To advance the animation, this method must return true. Default implementation returns false.
  */
  virtual bool step(const OdUInt64& frame);
};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdBaseCameraMotion, OdBaseCameraMotion> object.
*/
typedef OdSharedPtr<OdBaseCameraMotion> OdBaseCameraMotionPtr;


}
#endif // _PDF_PUBLISH_BASE_CAMERA_MOTION_INCLUDED_

