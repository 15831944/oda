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
// PdfPublishAnimation.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_ANIMATION_INCLUDED_
#define _PDF_PUBLISH_ANIMATION_INCLUDED_

#include "PdfPublishCommon.h"
#include "PdfPublishBaseObject.h"
#include "PdfPublishBaseCameraMotion.h"
#include "PdfPublishBaseNodeMotion.h"
#include "UInt64Array.h"

/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements a parametric animation object in .pdf documents created with Publish SDK.
  
*/
class PDFPUBLISH_TOOLKIT OdAnimation : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdAnimation();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdAnimation);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys the animation object.
  */
  virtual ~OdAnimation();

  /** \details
    Sets the name of the animation object.
    \param name [in] A name of the animation.
  */
  void setName(const OdString& name);
  
  /** \details
    Sets the flag that determines whether the animation should be started when a document opens.
    \param state [in] A new flag value.
    \remarks 
    If the flag is true, the animation starts after the document is opened, otherwise the animation starts 
    on user request.
  */
  void setPlayOnOpenDocument(bool state);
  
  /** \details
    Sets the flag that determines whether the animation should be replayed.
    \param state [in] A new flag value.
    \remarks 
    If the flag is true, the animation is replayed instantly, otherwise the animation stops when it is finished.
  */
  void setAutoReplay(bool state);
  
  /** \details
    Sets the quantity of frames that are played per second during the animation.
    \param frame_per_second [in] A quantity of frames.
  */
  void setFramesPerSecond(const OdUInt32 frame_per_second);
  
  /** \details
    Sets the total frame quantity for the animation.
    \param frame_count [in] A total quantity of frames.
  */
  void setFramesCount(const OdUInt64 frame_count);

  /** \details
    Sets an object that handles the camera motion when the animation is played.
    \param start_frame  [in] A start frame. Determines when the camera motion starts.
    \param end_frame    [in] An end frame. Determines when the camera motion stops.
    \param motion       [in] A smart pointer to a camera motion object.
    \remarks
    The motion object should be an instance of a class inherited from the <link OdPdfPublish::OdBaseCameraMotion, OdBaseCameraMotion> class.
    Note that for any animation, only one object of the OdBaseCameraMotion class can be set. 
    \sa
    <link OdPdfPublish::OdBaseCameraMotion, OdBaseCameraMotion>
    <link OdPdfPublish::OdBaseNodeMotion, OdBaseNodeMotion>.
  */
  void setCameraMotion(const OdUInt64& start_frame, const OdUInt64& end_frame, OdBaseCameraMotionPtr motion);
  
  /** \details
    Adds an object that handles a node motion when the animation is played.
    \param start_frame  [in] A start frame. Determines when the node motion starts.
    \param end_frame    [in] An end frame. Determines when the node motion stops.
    \param motion       [in] A smart pointer to a node motion object.
    \remarks
    The motion object should be an instance of a class inherited from the <link OdPdfPublish::OdBaseNodeMotion, OdBaseNodeMotion> class.
    
    \sa
    <link OdPdfPublish::OdBaseNodeMotion, OdBaseNodeMotion>.
    <link OdPdfPublish::OdBaseCameraMotion, OdBaseCameraMotion>
  */
  void addNodeMotion(const OdUInt64& start_frame, const OdUInt64& end_frame, OdBaseNodeMotionPtr motion);

  /** \details
    Returns the name of the animation object.
    \param name [out] A placeholder for the name of the animation.
  */
  void getName(OdString& name) const;
  
  /** \details
    Returns the current value of the flag that determines whether the animation should be started when a document opens.
    \param state [out] A placeholder for the flag value.
    \remarks 
    If the flag is true, the animation starts after the document is opened, otherwise the animation starts 
    on user request.
  */
  void getPlayOnOpenDocument(bool& state) const;
  
  /** \details
    Returns the current value of the flag that determines whether the animation should be replayed.
    \param state [out] A placeholder for the flag value.
    \remarks 
    If the flag is true, the animation is replayed instantly, otherwise the animation stops when it is finished.
  */  
  void getAutoReplay(bool& state) const;
  
  /** \details
    Returns the quantity of frames that are played per second during the animation.
    \param frame_per_second [out] A placeholder for the quantity of frames.
  */
  void getFramesPerSecond(OdUInt32& frame_per_second) const;
  
  /** \details
    Returns the total frame quantity for the animation.
    \param frame_count [out] A placeholder for the total quantity of frames.
  */
  void getFramesCount(OdUInt64& frame_count) const;

  /** \details
    Returns the current camera motion handler associated with the animation.
    The method fills passed values with the start frame index, end frame index and with a smart pointer to the camera motion object, 
    then returns them to a calling subroutine.
    \param start_frame  [out] A placeholder for the camera motion start frame.
    \param end_frame    [out] A placeholder for the camera motion end frame.
    \param motion       [out] A placeholder for the smart pointer to the camera motion object.
    \sa
    <link OdPdfPublish::OdBaseCameraMotion, OdBaseCameraMotion>
    <link OdPdfPublish::OdBaseNodeMotion, OdBaseNodeMotion>.
  */
  void getCameraMotion(OdUInt64& start_frame, OdUInt64& end_frame, OdBaseCameraMotionPtr& motion) const;
  
  /** \details
    Returns all node motion handlers associated with the animation.
    The method fills the passed arrays with start and end frame numbers and with smart pointers to motion objects, then returns them to a calling subroutine.
    \param starts_frame  [out] A placeholder for motion start frames.
    \param ends_frame    [out] A placeholder for motion end frames.
    \param motions       [out] A placeholder for the array of node motion objects.
    \sa
    <link OdPdfPublish::OdBaseNodeMotion, OdBaseNodeMotion>.
    <link OdPdfPublish::OdBaseCameraMotion, OdBaseCameraMotion>
  */
  void getNodeMotions(OdUInt64Array& starts_frame, OdUInt64Array& ends_frame, OdBaseNodeMotionPtrArray& motions) const;


  
};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdAnimation, OdAnimation> object.
*/
SMARTPTR(OdAnimation);

}
#endif // _PDF_PUBLISH_ANIMATION_INCLUDED_

