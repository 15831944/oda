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

#ifndef DbAssocNetworkDeepCloneReactor_INCLUDED_
#define DbAssocNetworkDeepCloneReactor_INCLUDED_ /*!DOM*/

#include "DbDatabase.h"
#include "RxEvent.h"

#define STL_USING_MAP
#include "OdaSTL.h"
#include "DbObjectOverrule.h"

class OdDbAssocAction;
class OdDbIdMapping;
class OdDbAssocNetCloneCtx;

/** \details
  This class implements the deep clone overrule for assoc. network deep/wblock cloning.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class OdDbAssocNetCloneOverrule : public OdStaticRxObject<OdDbObjectOverrule> {
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocNetCloneOverrule);

  virtual OdDbObjectPtr deepClone(const OdDbObject* subject, OdDbIdMapping& idMap, OdDbObject* owner, bool primary = true);

  virtual OdDbObjectPtr wblockClone(const OdDbObject* subject, OdDbIdMapping& idMap, OdDbObject* owner, bool primary = true);

  virtual bool isApplicable(const OdRxObject* subject) const;

  OdDbAssocNetCloneOverrule();

  void addOverruleClient();
  void removeOverruleClient();

private:
  unsigned m_clients;
  bool m_WasOverruling;
};


/** \details
  This class implements the deep clone reactor.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class OdDbAssocNetworkDeepCloneReactor: public OdRxEventReactor
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocNetworkDeepCloneReactor();

  /** \details
    Destructor destroys an instance of this class.
  */
  ~OdDbAssocNetworkDeepCloneReactor();

  /** \details
    Adds the reference to the deep clone reactor.
  */
  void addRefReactor();

  /** \details
    Releases the deep clone reactor.
  */
  void releaseReactor();

  // OdRxEventReactor methods

  void beginDeepClone(OdDbDatabase* pToDb, OdDbIdMapping& idMap);

  void beginDeepCloneXlation(OdDbIdMapping& idMap);

  /** \details
    Aborts the deep clone.
  */
  void abortDeepClone(OdDbIdMapping& idMap);

  /** \details
    Ends the deep clone.
  */
  void endDeepClone(OdDbIdMapping& idMap);

  void databaseToBeDestroyed(OdDbDatabase* pDb);

private:
  /*!DOM*/
  class ContextTerminator
  {
  public:
    ContextTerminator(OdDbAssocNetworkDeepCloneReactor& reactor,
                      OdDbDatabase* pContextDb);

    ~ContextTerminator();

  private:
    OdDbAssocNetworkDeepCloneReactor& m_reactor;
    OdDbDatabase* m_pContextDb;
  };

  /*!DOM*/
  void startProcess(OdDbIdMapping& idMap);
  /*!DOM*/
  ContextTerminator endProcessDeferred(OdDbDatabase* pDb);
  /*!DOM*/
  OdSmartPtr<OdDbAssocNetCloneCtx> getProcessContext(OdDbIdMapping& idMap);

  /*!DOM*/
  int m_Counter;
  /*!DOM*/
  OdDbAssocNetCloneOverrule m_Overrule;
  /*!DOM*/
  typedef void(*EventHandler)(OdDbAssocAction* source, OdDbAssocNetCloneCtx& ctx);
  /*!DOM*/
  void processEvent(OdDbAssocNetCloneCtx& ctx, EventHandler eventHandler);
};

#endif
