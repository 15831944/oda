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

#ifndef __ODGIAUXDATA_H__
#define __ODGIAUXDATA_H__

#include "TD_PackPush.h"

#include "RxObject.h"

class OdDbStub;

/** \details
This class is the base class for level overrides support.

\sa
TD_Gi

<group OdGi_Classes>
*/

class FIRSTDLL_EXPORT OdGiLayerOverrides : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiLayerOverrides);

  /** \details
     Retrieves a layer override.
    
    \param idNativeLayer [in]  Pointer to an identifier of a native layer.
    
    \returns
    pointer to the instance of the OdDbStub that represent layer override.
  */
  virtual OdDbStub* getLayerOverride(OdDbStub* idNativeLayer) const = 0;
};

/** \details
  A data type that represents a smart pointer to an <link OdGiLayerOverrides, OdGiLayerOverrides> object.
*/
typedef OdSmartPtr<OdGiLayerOverrides> OdGiLayerOverridesPtr;

/** \details
This class is the base class for auxiliary data context.

\sa
TD_Gi

<group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiAuxiliaryData : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiAuxiliaryData);

  /** \details
     Retrieves a smart pointer to layer overrides.
    
    \returns
    a smart pointer to layer overrides.
  */
  virtual OdGiLayerOverridesPtr getLevelOverrides() const = 0;
  
  /** \details
     Sets layer overrides.
    
    \param pLevelOverrides [in]  Smart pointer to layer overrides.
  */
  virtual void setLevelOverrides(const OdGiLayerOverridesPtr& pLevelOverrides) = 0;
};

/** \details
  A data type that represents a smart pointer to an <link OdGiAuxiliaryData, OdGiAuxiliaryData> object.
*/
typedef OdSmartPtr<OdGiAuxiliaryData> OdGiAuxiliaryDataPtr;

#include "TD_PackPop.h"

#endif //__ODGIAUXDATA_H__

