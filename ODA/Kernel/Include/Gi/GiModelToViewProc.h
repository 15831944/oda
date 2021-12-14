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

#ifndef __ODGIMODELTOVIEWPROC_H__
#define __ODGIMODELTOVIEWPROC_H__

#include "Gi/GiConveyorNode.h"
#include "Ge/GeDoubleArray.h"

class OdGeMatrix3d;
class OdGiDeviation;
class OdGiXform;
class OdGiAbstractClipBoundary;

/** \details

    <group OdGi_Classes> 
*/
class ODGI_EXPORT ODRX_ABSTRACT OdGiModelToViewProc : public OdRxObject
{
protected:
  OdGiModelToViewProc();
public:
  ODRX_DECLARE_MEMBERS(OdGiModelToViewProc);

  /** \details
  Sets the draw context object (to access to traits, etc).
  
  \param pDrawCtx [in]  Pointer to the draw context.
  */
  virtual void setDrawContext( OdGiConveyorContext* pDrawCtx ) = 0;

  virtual OdGiConveyorInput& modelInput() = 0;
  virtual OdGiConveyorInput& eyeInput() = 0;

  virtual OdGiConveyorOutput& output() = 0;

  virtual void setEyeToOutputTransform(const OdGeMatrix3d& xMat) = 0;
  virtual const OdGeMatrix3d& eyeToOutputTransform() const = 0;

  /** \details
  Sets view (camera) parameters.
  
  \param target [in]  Camera target.
  \param xVector [in]  Vector that defines x-axis.
  \param upVector [in]  Vector that defines upward direction of camera.
  \param eyeVector [in]  Vector that defines direction from camera target to camera position.
  */
  virtual void setView(
    const OdGePoint3d& target,
    const OdGeVector3d& xVector,
    const OdGeVector3d& upVector,
    const OdGeVector3d& eyeVector) = 0;
  
  /** \details
  Sets world to eye transform matrix.
  
  \param xMat [in]  Transform matrix.
  */
  virtual void setWorldToEyeTransform(const OdGeMatrix3d& xMat) = 0;

  /** \details
  Retrieves world to eye transform matrix.
  
  \returns
  reference to OdGeMatrix3d that represents world to eye transform matrix.
  */
  virtual const OdGeMatrix3d& worldToEyeTransform() const = 0;
  
  /** \details
  Retrieves eye to world transform matrix.
  
  \returns
  reference to OdGeMatrix3d that represents eye to world transform matrix.
  */
  virtual const OdGeMatrix3d& eyeToWorldTransform() const = 0;

  /** \details
  Retrieves model to eye transform matrix.
  
  \returns
  reference to OdGeMatrix3d that represents model to eye transform matrix.
  */
  virtual const OdGeMatrix3d& modelToEyeTransform() const = 0;
  
  /** \details
  Retrieves eye to model transform matrix.
  
  \returns
  reference to OdGeMatrix3d that represents eye to model transform matrix.
  */
  virtual const OdGeMatrix3d& eyeToModelTransform() const = 0;
  
  /** \details
    Specifies a model transformation matrix for the current transformation stack.  
    
    \remarks
    The specified transformation is concatenated to the current model transformation 
    (which is initially the identity transform).      
    \param xMat [in]  Transformation matrix.  
  */
  virtual void pushModelTransform(const OdGeMatrix3d& xMat) = 0;
  
  /** \details
    Removes the top transformation off the current transformation stack.
  */
  virtual void popModelTransform() = 0;
  
  /** \details
  Checks whether model transform stack does not contain any transform matrix.
  
  \returns
  true if model matrix stack is empty, false otherwise.
  */
  virtual bool isEmptyModelMatrixStack() const = 0;
  
  /** \details
  Retrieves model to world transform matrix.
  
  \returns
  reference to OdGeMatrix3d that represents model to world transform matrix.
  */
  virtual const OdGeMatrix3d& modelToWorldTransform() const = 0;
  
  /** \details
  Retrieves world to model transform matrix.
  
  \returns
  reference to OdGeMatrix3d that represents world to model transform matrix.
  */
  virtual const OdGeMatrix3d& worldToModelTransform() const = 0;

  virtual OdGiXform *getModelToEyeXform() = 0;
  virtual const OdGiXform *getModelToEyeXform() const = 0;
  virtual OdGiXform *getEyeToModelXform() = 0;
  virtual const OdGiXform *getEyeToModelXform() const = 0;

  /** \details
    Specifies a clip boundary for the current clip stack.

    \param pBoundary [in]  Pointer to the boundary.
    \remarks
    Subsequent objects are clipped until popClipBoundary() is called.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  virtual void pushClipBoundary(OdGiClipBoundary* pBoundary) = 0;
  
  /** \details
    Specifies a clip boundary for the current clip stack.

    \param pBoundary [in]  Pointer to the boundary.
    \param pClipInfo [in]  Pointer to the additional object that specifies clip boundary type.
    \remarks
    Subsequent objects are clipped until popClipBoundary() is called.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  virtual void pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo) = 0;
  
  /** \details
    Removes the top clip boundary from the clip stack.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  virtual void popClipBoundary() = 0;

  /** \details
    Checks whether clipping stage chain is not empty.
    
    \returns
    true if clipping stage chain is not empty, false otherwise.
  */
  virtual bool isClipping() const = 0;
  
  /** \details
    Checks whether clip set is empty.
    
    \returns
    true if clip set is empty, false otherwise.
  */
  virtual bool isEmptyClipSet() const = 0;

  /** \details
    Sets the maximum world (WCS) deviation for this object.
    
    \param worldDev [in]  Deviation.
  */
  virtual void setWorldDeviation(const OdGiDeviation& worldDev) = 0;
  
  /** \details
    Sets the maximum world (WCS) deviation for this object.
    
    \param deviations [in]  Array of deviation values.
  */
  virtual void setWorldDeviation(const OdGeDoubleArray& deviations) = 0;

  
  /** \details
    Retrieves the maximum world (WCS) deviation for this object.
    
    \returns
    maximum world space (WCS) deviation as the OdGiDeviation instance.
  */
  virtual const OdGiDeviation& worldDeviation() const = 0;
  
  /** \details
    Retrieves the maximum ModelSpace (UCS) deviation for this VectorizeView object.
    
    \returns
    maximum ModelSpace (UCS) deviation as the OdGiDeviation instance.
  */
  virtual const OdGiDeviation& modelDeviation() const = 0;
  
  /** \details
    Retrieves the maximum EyeSpace (ECS) deviation for this VectorizeView object.
    
    \returns
    maximum eye space (ECS) deviation as the OdGiDeviation instance.
  */
  virtual const OdGiDeviation& eyeDeviation() const = 0;

  /*
  Clipping coordinate system.
  **/
  enum ClippingCS
  {
    /* Clip in eye coordinate system.**/
    kClipInEyeSpace,
    /* Clip in world coordinate system.**/
    kClipInWorldSpace,
    /* Clip in output coordinate system.**/
    kClipInOutputSpace
  };
  
  /** \details
    Sets clipping space.
    
    \param newClipCS [in]  New coordinate space for clipping.
  */
  virtual void setClippingSpace(ClippingCS newClipCS) = 0;
  
  /** \details
    Retrieves current clipping space.
    
    \returns
    current clipping space as a value from the ClippingCS enumeration.
  */
  virtual ClippingCS currentClippingSpace() const = 0;

  /** \details
    Enables or disables analytic mode for curve clipping.
    
    \param bEnable [in]  Flag that specifies whether to enable analytic mode for curve clipping.
  */
  virtual void enableAnalyticCurvesClipping(bool bEnable = true) = 0;
  
  /** \details
    Checks whether analytic mode for curve clipping is enabled.
    
    \returns
    true if analytic mode for curve clipping is enabled, false otherwise.
  */
  virtual bool isAnalyticCurvesClippingEnabled() const = 0;

  /** \details
    Enables clipping debug logging.
    
    \param fileName [in]  Path to file where debug information is logged.
  */
  virtual void enableClippingDebugLog(const OdString &fileName) = 0;
  
  /** \details
    Disables clipping debug logging.
  */
  virtual void disableClippingDebugLog() = 0;
  
  /** \details
    Checks whether clipping debug logging is enabled.
    
    \returns
    true if clipping debug logging is enabled, false otherwise.
  */
  virtual bool isClippingDebugLogEnabled() const = 0;

  /** \details
    Enables or disables sectioning (clipping).
    
    \param bEnable [in]  Flag that specifies whether to enable sectioning.
  */
  virtual void switchSectioning(bool bEnable) = 0;
  
  /** \details
    Checks whether sectioning (clipping) is enabled.
    
    \returns
    true if sectioning is enabled, false otherwise.
  */
  virtual bool isSectioningEnabled() const = 0;
};

/** \details
  This template class is specialization of the OdSmartPtr class for the OdGiModelToViewProc class.

  \sa
  <link smart_pointers, Work with Smart Pointers>
*/
typedef OdSmartPtr<OdGiModelToViewProc> OdGiModelToViewProcPtr;


/** \details
  <group OdGi_Classes>

  This class defines the model section.

  Corresponding C++ library: TD_Gi
*/
class ODGI_EXPORT ODRX_ABSTRACT OdGiModelSection : public OdGiConveyorNode
{
protected:
  OdGiModelSection();
public:
  ODRX_DECLARE_MEMBERS(OdGiModelSection);

  /** \details
  Sets the draw context object (to access to traits, etc).
  
  \param pDrawCtx [in]  Pointer to the draw context.
  */
  virtual void setDrawContext( OdGiConveyorContext* pDrawCtx ) = 0;
  
  /** \details
  Sets the the clipping boundary for this object.
  
  \param bnd [in]  Pointer to the clipping boundary.
  */
  virtual void set(const OdGiClipBoundary* bnd) = 0;
  
  /** \details
  Sets model to world transform matrix.
  
  \param modelToWorld [in]  Models to world transform matrix.
  */
  virtual void setModelToWorld(const OdGeMatrix3d& modelToWorld) = 0;
  
  /** \details
  Checks whether model sectioning is enabled.
  
  \returns
  true if model sectioning is enabled, false otherwise.
  */
  virtual bool enabled() const = 0;
  
  /** \details
  Enables or disable model sectioning.
  
  \param bOn [in]  Flag that specifies whether to enable model sectioning.
  */
  virtual void setEnabled(bool bOn)  = 0;
};

/** \details
  The typified smart pointer for the model section object. This  
  template class is specialization of the OdSmartPtr class for the OdGiModelSection class.
*/
typedef OdSmartPtr<OdGiModelSection> OdGiModelSectionPtr;

#endif //#ifndef __ODGIMODELTOVIEWPROC_H__
