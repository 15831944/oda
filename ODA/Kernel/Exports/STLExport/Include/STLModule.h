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


#ifndef _ODSTLMODULEIMPL_INCLUDED_
#define _ODSTLMODULEIMPL_INCLUDED_

#include "STLExportDef.h"
#include "RxDynamicModule.h"
#include "DbBaseDatabase.h"

class OdGiDrawable;
class OdStreamBuf;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_STL_EXPORT
{
  /** \details
   This class implements the module for STL export.
  */
  class STLEXPORT_DLL STLModule : public OdRxModule
  {
  protected:
    /** \details
      Initializes the STL export module.
    */
    void initApp();

    /** \details
      Uninitializes the STL export module.
    */
    void uninitApp();
  public:
    /** \details
      Exports an element to STL format stream.
    
      \param pDb            [in] Database.
      \param pEntity        [in] Entity to export.
      \param pOutStream    [out] Output stream.
      \param bTextMode      [in] If true, export to ASCII STL format, else to binary STL format.
      \param dDeviation     [in] Maximum allowed deviation.
      \param positiveOctant [in] If true, move STL coordinates into all positive octant.
      \returns eOk if successful, or an appropriate error code otherwise.

      \remarks
      If positiveOctant is true and at least one of the obtained triangle points
      has at least one coordinate from the (-inf, 0] interval,
      then all points will be shifted to an octant, where all coordinates
      are positive numbers from the (0.01, +inf) interval.
    */
    virtual OdResult exportSTL(OdDbBaseDatabase *pDb, const OdGiDrawable &pEntity, OdStreamBuf &pOutStream, bool bTextMode, double dDeviation, bool positiveOctant = true);

    /** \details
      Exports an element to STL format stream with checking of solid topology.
    
      \param pDb            [in] Database.
      \param pEntity        [in] Entity to export.
      \param pOutStream    [out] Output stream.
      \param bTextMode      [in] If true, export to ASCII STL format, else to binary STL format.
      \param dDeviation     [in] Maximum allowed deviation.
      \param positiveOctant [in] If true, move STL coordinates into all positive octant.
      \returns eOk if successful, or an appropriate error code otherwise.
      
      \remarks
      If positiveOctant is true and at least one of the obtained triangle points
      has at least one coordinate from the (-inf, 0] interval,
      then all points will be shifted to an octant, where all coordinates
      are positive numbers from the (0.01, +inf) interval.
    */
    virtual OdResult exportSTLEx(OdDbBaseDatabase *pDb, const OdGiDrawable &pEntity, OdStreamBuf &pOutStream, bool bTextMode, double dDeviation, bool positiveOctant = true);
  };

  /** \details
    This template class is a specialization of the OdSmartPtr class for STLModule object pointers.
  */
  typedef OdSmartPtr<STLModule> STLModulePtr;
}
#endif //_ODSTLMODULEIMPL_INCLUDED_
