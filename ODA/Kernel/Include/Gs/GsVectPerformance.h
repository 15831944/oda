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
#ifndef GS_VECT_PERFORMANCE_H
#define GS_VECT_PERFORMANCE_H

#include "TD_PackPush.h"

/** \details
 <group OdGs_Classes> 
    
 Corresponding C++ library: TD_Gs
*/
class GsDevicePerformanceTm
{
public:
    /** \details
      Default constructor for the GsDevicePerformanceTm class. Resets performance time.
    */
    GsDevicePerformanceTm() { reset(); }
    
    /** \details
      Resets performance time. Sets overall update time, update geometry time, update screen time and update entities time to 0.
    */
    void reset()
    {
        m_update = 0;
        m_updateGeometry = 0;;
        m_updateScreen = 0;
        m_updateEntities = 0;
    }

public:
    double m_update;
    double m_updateGeometry;
    double m_updateScreen;
    double m_updateEntities;
};

enum EParallelVectOptions
{
    eEnableParallelVectorization = 0x00000001,
    eEnableParallelDisplay       = 0x00000002,
    eEnableOptimalThreadsNumber  = 0x00000004,
    eEnableSchedulerLogOutput    = 0x00000008,
    eEnablePerfMeasurements      = 0x00000010,
    eForcePartialUpdateForTest   = 0x00000020,
    eForceParallelVectorization  = 0x00000040,
    eEnableUpdateExtentsOnly     = 0x00000080
};

/** \details
  <group OdGs_Classes> 
    
  Corresponding C++ library: TD_Gs
*/
class GsVectPerformanceData
{
public:
    /** \details
      Default constructor for the GsVectPerformanceData class.
    */
    GsVectPerformanceData(): m_options(0), m_numVectUsedUpdateGeom(0),
        m_numVectUsedUpdateScr(0){}
    
    /** \details
      Checks whether parallel vectorization is enabled.
      \returns
      true if parallel vectorization is enabled, false otherwise.
    */
    bool enableParallelVectorization() const
    { return GETBIT(m_options, eEnableParallelVectorization); }
    
    /** \details
      Checks whether parallel display is enabled.
      \returns
      true if parallel display is enabled, false otherwise.
    */
    bool enableParallelDisplay() const
    { return GETBIT(m_options, eEnableParallelDisplay); }
    
    /** \details
      Checks whether scheduler log output is enabled.
      \returns
      true if scheduler log output is enabled, false otherwise.
    */
    bool schedulerLogOutput() const
    { return GETBIT(m_options, eEnableSchedulerLogOutput); }
    
    /** \details
      Checks whether optimal count of threads is enabled.
      \returns
      true if optimal count of threads is enabled, false otherwise.
    */
    bool optimalThreadsNumber() const
    { return GETBIT(m_options, eEnableOptimalThreadsNumber); }
    
    /** \details
      Checks whether performance measurements are enabled.
      \returns
      true if performance measurements are enabled, false otherwise.
    */
    bool enablePerfMeasurements() const
    { return GETBIT(m_options, eEnablePerfMeasurements); }
    
    /** \details
      Checks whether partial update is forced for test purposes.
      \returns
      true if partial update is forced for test purposes, false otherwise.
    */
    bool forcePartialUpdateForTest() const
    { return GETBIT(m_options, eForcePartialUpdateForTest); }
    
    /** \details
      Checks whether parallel vectorization is forced.
      \returns
      true if parallel vectorization is forced, false otherwise.
    */
    bool forceParallelVectorization() const
    { return GETBIT(m_options, eForceParallelVectorization); }

    /** \details
      Checks whether only vectorization enabled, without display.
      \returns
      true if vectorization without display enabled, false otherwise.
    */
    bool enableVectorizationOnly() const
    { return GETBIT(m_options, eEnableUpdateExtentsOnly); }
public:
    OdUInt32 m_options; //bit flags, see EParallelVectOptions
    GsDevicePerformanceTm m_tm;
    OdUInt32 m_numVectUsedUpdateGeom;
    OdUInt32 m_numVectUsedUpdateScr;
};

#include "TD_PackPop.h"

#endif // GS_VECT_PERFORMANCE_H
