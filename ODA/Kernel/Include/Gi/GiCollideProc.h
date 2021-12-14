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

#ifndef __ODGICOLLIDEPROC__
#define __ODGICOLLIDEPROC__

#include "Gi/GiConveyorNode.h"
#include "Gs/Gs.h"
#include "Ge/GeDoubleArray.h"
#include "Ge/GePoint2dArray.h"

class OdGiDeviation;
class OdGiConveyorContext;

#include "TD_PackPush.h"

/** \details

    <group OdGi_Classes> 
    This class implements conveyor nodes performing collision detection. 
    
    \remarks
    This conveyor node is used for implementation OdGsView::collide()
    and does not produce any output.
    \sa
    <link gs_collision_detection, Work with Collision Detection>
*/
class ODGI_EXPORT OdGiCollideProc : public OdGiConveyorNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGiCollideProc);

  
  /** \details
  Associates this object with the collision detection reactor and collision detection context.
  
  \param pReactor [in]  Pointer to a custom object that detects collisions.
  \param pCtx [in]  Pointer to the collision detection context.
  */
  virtual void set( OdGsCollisionDetectionReactor* pReactor, const OdGsCollisionDetectionContext* pCtx = NULL ) = 0;

  /** \details
  Sets max deviation for curve tessellation.
  
  \param deviations [in]  Array with deviation values.
  */
  virtual void setDeviation(const OdGeDoubleArray& deviations) = 0;

  /** \details
  Sets deviation object to obtain max deviation for curve tessellation.
  
  \param pDeviation [in]  Pointer to the deviation object.
  */
  virtual void setDeviation(const OdGiDeviation* pDeviation) = 0;

  /** \details
  Sets the draw context object (to access to traits, etc).
  
  \param pDrawCtx [in]  Pointer to the draw context.
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawCtx) = 0;

  /** \details
  Specifies whether line primitives should be checked. If line primitives checking is disabled, only triangles will be checked.
  
  \param bCheck [in]  Flag that specifies whether line primitives should be checked.
  */
  virtual void setLinePrimitivesChecking( bool bCheck ) = 0;
  
  /** \details
  Checks whether line primitives should be checked. If line primitives checking is disabled, only triangles will be checked.
  
  \returns
  true if line primitives checking is enabled, false otherwise which means that only triangles are checked.
  */
  virtual bool linePrimitivesChecking() const = 0;

  /** Additional analytic checking for primitives.
  */
  enum AnalyticPrimitives
  {
    kAnalyticNone = 0, //Disabled
    kAnaliticCircles = (1 << 0), //For circles
    kAnalyticComplexCurves = ( 1 << 1 ) // For complex curves
  };

  /** \details
  Enables analytic mode for specified primitives. Must be implemented in derived classes.
  
  \param options [in]  Option for analytic mode.
  */
  virtual void enableAnalyticMode( OdUInt8 options ) = 0;
  
  /** \details
  Retrieves options that are used in the analytic mode.
  
  \returns
  options that are used in analytic mode. Must be implemented in derived classes.
  */
  virtual OdUInt8 analitycMode() const = 0;

  /** \details
  Represents processing phase of collision detection.
  */
  enum ProcessingPhase
  {
    kPhaseGatherInputData = 0, //During the OdGiCollideProc::kPhaseGatherInputData processing phase, if OdGiPathNode of the currently processing entity is not listed in pInputList, the entity is skipped.
    kPhaseDetectIntersections  //During the OdGiCollideProc::kPhaseDetectIntersections processing phase, if OdGiPathNode of the currently processing entity is listed in pInputList, the entity is skipped to avoid detecting collisions between the same entity
  };

  /** \details
  Disables entity filtering during.
  
  \param bNoFilter [in]  Flag that specifies whether 
  */
  virtual void setNoFilter( bool bNoFilter ) = 0;
  
  /** \details
  Checks whether entity filtering is disabled.
  
  \returns
  true if entity filtering is disabled, false otherwise.
  */
  virtual bool noFilter() const = 0;

  /** \details
  Specifies the current processing phase that allows conveyor node collecting of triangles to different lists.
  
  \param ProcessingPhase [in]  Processing phase.
  */
  virtual void setProcessingPhase( ProcessingPhase ) = 0;
  
  /** \details
  Retrieves the current processing phase.
  
  \returns
  current processing phase represented by the ProcessingPhase enumeration values.
  */
  virtual ProcessingPhase processingPhase() const = 0;

  /** \details
  Retrieves the extents of triangles that were collected during the OdGiCollideProc::kPhaseGatherInputData processing phase.
  
  \returns
  geometrical extents of the collected triangles.
  */
  virtual const OdGeExtents3d& extents() const = 0;

  /** \details
  Specifies a list of OdGiPathNode items that are used in entity filtering.
  
  \param pInputList     [in]  Pointer to a pointer to an input list of drawables.
  \param nInputListSize [in]  Number of drawables in a list.
  */
  virtual void setInputDrawables( OdGiPathNode const*const* pInputList, OdUInt32 nInputListSize ) = 0;
  
  /** \details
  Specifies an additional list of OdGiPathNode that is used in entity filtering during the OdGiCollideProc::kPhaseDetectIntersections processing phase;
  
  \param pInputList     [in]  Pointer to a pointer to an input list of drawables.
  \param nInputListSize [in]  Number of drawables in a list.
  */
  virtual void setCheckWithDrawables( OdGiPathNode const*const* pInputList, OdUInt32 nInputListSize ) = 0;

  /** \details
  Performs processing triangles that were gathered during conveyor node processing phases.
  */
  virtual void processTriangles() = 0;
  /** \details
  Specifies extents extension.
  \param e [in] Extents extension.
  \remarks This method is required for Clearance processing. If not to extend the extents, triangles may be filtered without clearance checking.
  */
  virtual void setExtentsExtension( double e ) = 0;
};

/** \details
  A data type that represents a smart pointer to an <link OdGiCollideProc, OdGiCollideProc> object.
*/
typedef OdSmartPtr<OdGiCollideProc> OdGiCollideProcPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGICOLLIDEPROC__
