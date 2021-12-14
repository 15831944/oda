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

#ifndef _RXVOIDSTORE_INCLUDED_
#define _RXVOIDSTORE_INCLUDED_

#include "RxObjectImpl.h"

class OdRxVoidStore;

typedef OdSmartPtr<OdRxVoidStore> OdRxVoidStorePtr;

/**   
  <group OdRx_Classes>
*/
class OdRxVoidStore : public OdRxObject
{
public:
  static OdRxVoidStorePtr createVoidStore(void *p = 0)
  {
    OdRxVoidStorePtr pVoid = OdRxObjectImpl<OdRxVoidStore>::createObject();
    pVoid->m_pVoid = p;
    return pVoid;
  }

  OdRxVoidStore() 
    : m_pVoid(NULL)
  {
  }

  OdRxVoidStore(void *pVoid)
    : m_pVoid(pVoid)
  {
  }
  
  ~OdRxVoidStore() 
  {
  }

  void *m_pVoid;
};

#endif  //#define _RXVOIDSTORE_INCLUDED_
