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
// External Scene Graph tasks scheduler

#ifndef ODTRRNDSGSCENEGRAPHTASKSCHEDULER
#define ODTRRNDSGSCENEGRAPHTASKSCHEDULER

#include "RxObject.h"

#include "TD_PackPush.h"

/** \details
    External Scene Graph tasks scheduler.
    <group ExRender_Windows_Classes>
*/
class OdTrRndSgSceneGraphTaskScheduler : public OdRxObject
{
  public:
    enum TaskType
    {
      kGenericTask = 0,  // Generic Scene Graph tasks, like trees balancing and etc.
      kAuxiliaryTask,    // Auxiliary Scene Graph tasks, like intersections computation and etc.
      kMiscellaneousTask // Miscellaneous Scene Graph tasks, like graphic streams postprocessing and etc.
    };
    enum ProcessType
    {
      kProcessTask = 0,  // Continue task processing
      kInformCompletion, // Continue task processing and call status completion callback
      kSkipTask,         // Skip this task processing and find next 
      kBreakProcess      // Break tasks processing
    };
  public:
    OdTrRndSgSceneGraphTaskScheduler() { }

    /** \details
      Called before starting of Scene Graph tasks queue processing.
      \returns
      Return true if callback implementor allows task queue processing.
    */
    virtual bool beginQueueProcessing() { return true; }
    /** \details
      Called on completion of Scene Graph tasks queue processing if it is allowed by callbck implementor.
    */
    virtual void endQueueProcessing() { }

    /** \details
      Called before Scene Graph task processing.
      \param taskType [in] Type of queue task.
      \returns
      Return type of task processing.
    */
    virtual ProcessType beginTaskProcessing(TaskType /*taskType*/) { return kProcessTask; }
    /** \details
      Called after Scene Graph task processing if callback implementor allows it.
      \param taskType [in] Type of queue task.
      \param bStatus  [in] 'true' if task processing completed correctly and 'false' if it is canceled.
    */
    virtual void endTaskProcessing(TaskType /*taskType*/, bool /*bStatus*/) { }
};

/** \details
    External Scene Graph tasks scheduler smart pointer.
    <group ExRender_Windows_Classes>
*/
typedef OdSmartPtr<OdTrRndSgSceneGraphTaskScheduler> OdTrRndSgSceneGraphTaskSchedulerPtr;

#include "TD_PackPop.h"

#endif // ODTRRNDSGSCENEGRAPHTASKSCHEDULER
