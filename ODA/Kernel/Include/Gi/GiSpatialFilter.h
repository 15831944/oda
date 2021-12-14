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

#ifndef __ODGISPATIALFILTER__
#define __ODGISPATIALFILTER__

#include "Gi/GiConveyorNode.h"

#include "Ge/GeDoubleArray.h"
#include "Ge/GeExtents2d.h"

#include "TD_PackPush.h"

class OdGiDeviation;
class OdGiConveyorContext;

/** \details
    This class defines an interface for spatial filtering by 3D rectangular prisms.
    * exts        - base of the prism.
    * bClipLowerZ - if lower boundary exists.
    * dLowerZ     - lower boundary.
    * bClipUpperZ - if upper boundary exists.
    * dUpperZ     - upper boundary.
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiSpatialFilter : public OdRxObject
{
protected:
  OdGiSpatialFilter();

public:
  ODRX_DECLARE_MEMBERS(OdGiSpatialFilter);

  virtual OdGiConveyorInput& input() = 0;
  virtual OdGiConveyorOutput& insideOutput() = 0;
  virtual OdGiConveyorOutput& intersectsOutput() = 0;
  virtual OdGiConveyorOutput& disjointOutput() = 0;

  // set output conveyor geometry to this member, if it is needn't

  ODGI_EXPORT_STATIC static OdGiConveyorGeometry& kNullGeometry;

  /** details
    Sets properties for this spatial filter.
    
    \param exts [in]  Base of the prism.
    \param bClipLowerZ [in]  Flag that specifies whether to clip lower boundary (if exists).
    \param dLowerZ [in]  lower boundary.
    \param bClipUpperZ [in]  Flag that specifies whether to clip upper boundary (if exists).
    \param dUpperZ [in]  Upper boundary.
  */
  virtual void set(const OdGeExtents2d& exts,
                   bool bClipLowerZ = false,
                   double dLowerZ = 0.0,
                   bool   bClipUpperZ = false,
                   double dUpperZ = 0.0) = 0;

  /** details
    Retrieves properties for this spatial filter.
    
    \param exts [out]  Base of the prism.
    \param bClipLowerZ [out]  Flag that specifies whether to clip lower boundary (if exists).
    \param dLowerZ [out]  lower boundary.
    \param bClipUpperZ [out]  Flag that specifies whether to clip upper boundary (if exists).
    \param dUpperZ [out]  Upper boundary.
  */
  virtual void get(OdGeExtents2d& exts,
                   bool&   bClipLowerZ,
                   double& dLowerZ,
                   bool&   bClipUpperZ,
                   double& dUpperZ) const = 0;

  virtual bool isSimplifyOpt(OdUInt32 opt) const = 0;

  /** \details
    Sets the draw context object (to access to traits, etc).
    
    \param pDrawCtx [in]  Pointer to the drawing context.
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawCtx) = 0;

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
};

/** \details
  A data type that represents a smart pointer to an <link OdGiSpatialFilter, OdGiSpatialFilter> object.
*/
typedef OdSmartPtr<OdGiSpatialFilter> OdGiSpatialFilterPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGISPATIALFILTER__
