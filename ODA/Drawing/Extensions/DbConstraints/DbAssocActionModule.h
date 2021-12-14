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

#ifndef DbAssocActionModule_INCLUDED_
#define DbAssocActionModule_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "OdModuleNames.h"

#include "TD_PackPush.h"

#define DbAssocActionModuleName DbConstraintsModuleName

/** \details
  This class is the base class for actions which provide the associative behavior. 
  This class itself does not have virtual methods that could be overridden, therefore it is not 
  possible for external developers to derive directly from it and override its methods. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocActionModule : public OdRxModule {
public:
  // ODRX_DECLARE_MEMBERS(OdDbAssocActionModule);

  /** \details
    Checks if the automatic assoc.network evaluation is enabled.
    \remarks 
    It's enabled by default after DbConstraints module is loaded.
  */
  virtual bool autoEvaluationEnabled() const = 0;

  /** \details
    Enables or disables evaluation of top level network on editor events.
    \param bDoAutoEvaluate [in] tells to evaluate automaticaly.
 */
  virtual void enableAutoEvaluation(bool bDoAutoEvaluate = true) = 0;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocActionModule class.

  \sa
  <link smart_pointers, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocActionModule> OdDbAssocActionModulePtr;

#include "TD_PackPop.h"

#endif

