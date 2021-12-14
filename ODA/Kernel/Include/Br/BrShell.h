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



#ifndef _INC_DDBRSHELL_3F83F3F8033C_INCLUDED
#define _INC_DDBRSHELL_3F83F3F8033C_INCLUDED

#include "Br/BrEntity.h"

class OdBrComplex;

#include "TD_PackPush.h"


/** \details
    This class is the interface class for BREP shells.
    
    \remarks
    Shells correspond to sets of adjacent faces on solids.

    Shells are secondary BREP elements with no associated geometries. 

    Corresponding C++ library: TD_Br

    <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrShell : public OdBrEntity
{
public:
  OdBrShell();
#ifdef OD_ADD_MOVE_CTOR_HAVE_DEF_DEL_FUNC
  //DOM-IGNORE-BEGIN
  OdBrShell(const OdBrShell&) = default;
  OdBrShell& operator=(const OdBrShell&) = default;
  OdBrShell(OdBrShell&&) = default;
  OdBrShell& operator=(OdBrShell&&) = default;
  //DOM-IGNORE-END
#endif

  ~OdBrShell();

  /** \details
    Returns the Complex entity bounded by this Shell entity.
  */
  OdBrComplex getComplex() const;

  /** \details
  Returns the shell type (exterior, interior, unclassified).
  \returns the shell type: odbrShellUnclassified if the shell type cannot be determined at this time,
                           odbrShellExterior     if the shell is on a peripheral boundary of region or B-rep,
                           odbrShellInterior     if the shell is empty in the interior of a region or B-rep.
  */
  BrShellType getType() const;
};

#include "TD_PackPop.h"

#endif /* _INC_DDBRSHELL_3F83F3F8033C_INCLUDED */

