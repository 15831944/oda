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




#ifndef __ODGILINETYPER_H__
#define __ODGILINETYPER_H__


#include "Gi/GiConveyorNode.h"
#include "Gi/GiNonEntityTraits.h"
#include "Ge/GeDoubleArray.h"

class OdGiDeviation;
class OdGiConveyorGeometry;

#include "TD_PackPush.h"


/** \details

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiLinetyper : public OdGiConveyorNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGiLinetyper);

  /** \details
    Specifies the draw context for this linetyper.
    
    \param pDrawCtx [in]  Pointer to the drawing context.
  */
  virtual void setDrawContext( OdGiConveyorContext* pDrawCtx ) = 0;

  /** \details
    Specifies the deviation for drawing linetypes.
    
    \param deviations [in]  Array of deviation values for drawing linetypes.
  */
  virtual void setDeviation(const OdGeDoubleArray& deviations) = 0;

  /** \details
    Specifies the deviation for drawing linetypes.
    
    \param deviation [in]  Deviation for drawing linetypes.
  */
  virtual void setDeviation(const OdGiDeviation* pDeviation) = 0;

  virtual OdUInt32 setLinetype(OdDbStub* id, double scale, double generationCriteria = 0.,
    double dScaleForNonScalableLineStyles = 1.0 ) = 0;

  /** \details
    Retrieves the linetype traits for this linetyper.
    
    \returns
    reference to linetype traits represented by OdGiLinetypeTraits.
  */
  virtual OdGiLinetypeTraits& linetypeTraits() const = 0;

  /** \details
    Enables this linetyper.
  */
  virtual void enable() = 0;

  /** \details
    Checks whether this linetyper is enabled.
    
    \returns
    true if this linetyper is enabled, false otherwise.
  */
  virtual bool enabled() const = 0;

  /** \details
    Disables this linetyper.
  */
  virtual void disable() = 0;
  
  /** \details
    Makes linetyper to linetype circle curves (circles, circular arcs) analytically or using breaking to polyline.
    
    \param bAnalytic [in]  Flag that specifes whether analytic mode is used.
  */
  virtual void setAnalyticLinetypingCircles(bool bAnalytic) = 0;  
  
  /** \details
    Checks whether this linetyper to linetype circle curves analytically.
    
    \returns
    true if this linetyper linetypes simple curves in analytic mode, false - breaking is used to polyline.
  */
  virtual bool isAnalyticLinetypingCircles() const = 0;
  
  // makes linetyper to linetype complex curves (ellipses, elliptic arcs, nurbs curves) 
  /** \details
    Makes linetyper to linetype complex curves (ellipses, elliptic arcs, nurbs curves) analytically or using breaking to polyline.
    
    \param bAnalytic [in]  Flag that specifes whether analytic mode is used.
  */
  virtual void setAnalyticLinetypingComplexCurves(bool bAnalytic) = 0;  
  
  /** \details
    Checks whether this linetyper to linetype complex curves analytically.
    
    \returns
    true if this linetyper linetypes complex curves in analytic mode, false - breaking is used to polyline.
  */
  virtual bool isAnalyticLinetypingComplexCurves() const = 0;

  virtual OdGiConveyorGeometry* redirectionGeometry() = 0;

  /** \details
    Enables caching for this linetyper.
  */
  virtual void enableCache() = 0;
  
  /** \details
    Disables caching for this linetyper.
  */
  virtual void disableCache() = 0;
  
  /** \details
    Checks whether caching is enabled for this linetyper.
    
    \returns
    true if caching for this linetyper is enabled, false otherwise.
  */
  virtual bool cacheEnabled() const = 0;
  
  /** \details
    Clears cached data of this linetyper.
  */
  virtual void clearCache() = 0;
};

/** \details
A data type that represents a smart pointer to an <link OdGiLinetyper, OdGiLinetyper> object.
*/
typedef OdSmartPtr<OdGiLinetyper> OdGiLinetyperPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGILINETYPER_H__
