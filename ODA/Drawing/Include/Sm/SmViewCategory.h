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




#ifndef _SmViewCategory_h_Included_
#define _SmViewCategory_h_Included_

#include "Sm/SmSheetView.h"
#include "Sm/SmCalloutBlocks.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  /** \details

       
  */
  struct SMDLL_EXPORT OdSmViewCategory : OdSmComponent
  {
    ODRX_DECLARE_MEMBERS(OdSmViewCategory);
    virtual OdSmEnumSheetViewPtr getSheetViewEnumerator() const = 0;
    virtual OdSmCalloutBlocks* getCalloutBlocks() = 0;
    virtual const OdSmCalloutBlocks* getCalloutBlocks() const = 0;
  };
  typedef OdSmartPtr<OdSmViewCategory> OdSmViewCategoryPtr;
}
#endif //_SmViewCategory_h_Included_
