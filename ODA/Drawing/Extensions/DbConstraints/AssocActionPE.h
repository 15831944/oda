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

#ifndef AssocActionPE_INCLUDED_
#define AssocActionPE_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "RxObject.h"
#include "DbAssocAction.h"

#include "TD_PackPush.h"

/** \details
  This is the abstract base class that provides the protocol extension for associated actions.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT AssocActionPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(AssocActionPE);  

  /** \details
    Constructor creates an instance of this class.
  */
  AssocActionPE();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~AssocActionPE();

  /** \details
    The pure virtual method that evaluates this action and must be overridden in derived classes.
  */
  virtual void evaluate(OdDbAssocAction *action) = 0;

  /** \details
    The pure virtual method that runs diagnostics for this action and must be overridden in derived classes.
  */
  virtual void runDiagnostics(const OdDbAssocAction *action) = 0;
};

#include "TD_PackPop.h"

#endif
