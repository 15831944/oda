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




#ifndef __ODGIBREP_H__
#define __ODGIBREP_H__

#include "TD_PackPush.h"

#include "Gi/GiExport.h"
class OdBrBrep;

/** \details
    This class represents the BREP object.  

    \sa
    TD_Gi:

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiBrep : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiBrep);


  /** \details
    Obtains brep data. Must be implemented in derived classes.
    
    \returns
    true if operation succeeded, false otherwise.
  */
  virtual bool brep( OdBrBrep& ) const = 0;

  /** \details
    Draws this object. Must be implemented in derived classes.
    
    \returns
    false if viewportDraw is required.
  */
  virtual bool worldDraw( OdGiWorldDraw* pWd ) = 0;

  /** \details
  Return database to be used to get information about materials.
  
  \returns
  pointer to the OdDbBaseDatabase object that represents a database where information about materials can be retrieved. 
  */
  virtual OdDbBaseDatabase *database() const = 0;
};


/** \details
  A data type that represents a smart pointer to an <link OdGiBrep, OdGiBrep> object.
*/
typedef OdSmartPtr<OdGiBrep> OdGiBrepPtr;

#include "TD_PackPop.h"

#endif  // __ODGIBREP_H__

