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


#ifndef _OD_MATERIALRESOLVER_INCLUDED_
#define _OD_MATERIALRESOLVER_INCLUDED_

#include "TD_PackPush.h"

#define STL_USING_MAP
#include "OdaSTL.h"

#include "DbBaseDatabase.h"

/** <group Other_Classes>
*/
class DBROOT_EXPORT OdMaterialResolver
{
public:
  typedef std::map<OdUInt64 /*materialId*/, OdUInt64 /*objectId*/> OdIdIdMap;

private:
  OdDbBaseDatabase *m_pDb;
  OdIdIdMap         m_Map;

public:

  OdMaterialResolver()
    : m_pDb(0)
  {
  }

  OdMaterialResolver(const OdMaterialResolver &resolver)
    : m_pDb(resolver.m_pDb)
    , m_Map(resolver.m_Map)
  {
  }

  OdMaterialResolver(OdDbBaseDatabase *pDb, const OdIdIdMap &map)
    : m_pDb(0)
    , m_Map(map)
  {
  }

  OdDbBaseDatabase *database() const 
  {
    return m_pDb;
  }

  void setDatabase(OdDbBaseDatabase *pDb)
  {
    m_pDb = pDb;
  }

  OdIdIdMap &map() 
  {
    return m_Map;
  }

  const OdIdIdMap &map() const 
  {
    return m_Map;
  }

  void clear()
  {
    m_pDb = 0;
    m_Map.clear();
  }

  bool isEmpty() const
  {
    return m_pDb == 0 && m_Map.size() == 0;
  }

  bool getPair(OdUInt64 id, OdUInt64 &pair) const
  {
    OdIdIdMap::const_iterator pIt = m_Map.find(id);
    if (pIt == m_Map.end())
    {
      return false;
    }

    pair = pIt->second;
    return true;
  }

};

#include "TD_PackPop.h"

#endif // _OD_MATERIALRESOLVER_INCLUDED_
