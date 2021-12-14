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


#ifndef __OD_GI_EXTACCUM__
#define __OD_GI_EXTACCUM__

#include "Ge/GeExtents3d.h"
#include "Gi/GiConveyorNode.h"
#include "Gi/GiDeviation.h"

#include "TD_PackPush.h"

/** \details
    This class is the base class for custom classes that compute the extents of OdGiConveyorGeometry objects.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiExtAccum : public OdGiConveyorNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGiExtAccum);

  /** \details
    Sets the draw context associated with this object.
    \param pDrawContext [in]  Pointer to the draw context.
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawContext) = 0;

  /** \details
    Sets max deviation for curve tessellation.
    
    \param deviations [in]  Deviation values for curve tessellation.
  */
  virtual void setDeviation(const OdGeDoubleArray& deviations) = 0;

  /** \details
    Sets deviation object to obtain max deviation for curve tessellation.
    
    \param deviations [in]  Pointer to the OdGiDeviation object to define deviation for curve tessellation.
  */
  virtual void setDeviation(const OdGiDeviation* pDeviation) = 0;

  /** \details
    Returns the OdGiConveyorGeometry object associated with this object.  
    
    \returns
    a reference to the OdGiConveyorGeometry instance.
    
    \remarks
    This instance contains the functions that can be used by an entity to
    vectorize itself.
  */
  virtual OdGiConveyorGeometry& geometry() = 0;

  /** \details
    Retrieves the accumulated extents for the OdGiConveyorGeometry object associated with this object.
    \param extents [out]  Receives the extents.
    
    \returns
    true if extents is valid, false otherwise.
  */
  virtual bool getExtents(OdGeExtents3d& extents) const = 0;

  /** \details
    Resets the accumulated extents for the OdGiConveyorGeometry object associated with this object.
    \param newExtents [out]  New extents.
  */
  virtual void resetExtents(const OdGeExtents3d& newExtents = OdGeExtents3d()) = 0;

  /** \details
    Adds the specified extents to the the accumulated extents for the OdGiConveyorGeometry object associated with this object.
    \param extents [out]  Extents to be added.
  */
  virtual void addExtents(const OdGeExtents3d& extents) = 0;

  /** \details
    Checks if pline primitives contain bulges.
    
    \returns
    true if any of input pline primitives contain bulges.
  */
  virtual bool plineContainBulges() const = 0;

  /** \details
    Computes text extents.
    \param pDrawContext [in]  Current conveyor context pointer.
    \param textStyle [in]  Text style.
    \param pStr [in]  Pointer to a text string.
    \param nLength [in]  Text length.
    \param raw [in]  Text flags.
    \param minExt [out]  Minimal text extents point.
    \param maxExt [out]  Maximal text extents point.
    \param pEndPos [out]  Optional pointer to end text point.
    \param pExactExtents [out]  Optional pointer to exact text extents.
  */
  static void textExtents(OdGiConveyorContext* pDrawContext, const OdGiTextStyle& textStyle,
                          const OdChar* pStr, int nLength, OdUInt32 raw, OdGePoint3d& minExt,
                          OdGePoint3d& maxExt, OdGePoint3d* pEndPos = NULL, OdGeExtents3d *pExactExtents = NULL);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiExtAccum object pointers.
*/
typedef OdSmartPtr<OdGiExtAccum> OdGiExtAccumPtr;

#include "TD_PackPop.h"

#endif // __OD_GI_EXTACCUM__
