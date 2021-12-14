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




#ifndef _ODODGISELECTOR_INCLUDED_
#define _ODODGISELECTOR_INCLUDED_

#include "Gi/GiConveyorNode.h"
#include "Ge/GeDoubleArray.h"
#include "Ge/GePoint2dArray.h"
#include "Gs/GsSelectionReactor.h"

class OdGiDeviation;

#include "TD_PackPush.h"

/** \details
    
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiSelector : public OdGiConveyorNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGiSelector);

  /** \details
    Sets the drawing context.
    \param pDrawCtx [in]  Pointer to the drawing context to set.
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawCtx) = 0;

  /** \details
    Sets the reactor to process selections.
    \param selectionReactor [in]  Selection reactor.
  */
  virtual void setReactor(OdGsSelectionReactor& selectionReactor) = 0;
};

/** \details
  A data type that represents a smart pointer to an <link OdGiSelector, OdGiSelector> object.
*/
typedef OdSmartPtr<OdGiSelector> OdGiSelectorPtr;

#include "TD_PackPop.h"

#endif // #ifndef _ODODGISELECTOR_INCLUDED_
