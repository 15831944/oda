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

#ifndef _ODRXBASETREENODEIMPL_H_DEFINED_
#define _ODRXBASETREENODEIMPL_H_DEFINED_

#include "RxModelTreeBaseNode.h"
#include "../Include/RxProperty.h"
#include "RxVariant.h"
#define STL_USING_SET
#include "OdaSTL.h"

class OdRxModelHierarchyTreeBaseImpl;

class ODCDA_EXPORT OdRxModelTreeBaseNodeImpl
{
public:
  ODRX_HEAP_OPERATORS();
  static OdRxModelTreeBaseNodeImpl* getImpl(const OdRxModelTreeBaseNodePtr& pObj)
  {
    return const_cast<OdRxModelTreeBaseNodeImpl*>(pObj->m_pImpl);
  }

  OdRxModelTreeBaseNodeImpl();
  virtual ~OdRxModelTreeBaseNodeImpl();

  virtual const OdString& getNodeName() const;
  virtual OdUInt64 getUniqueSourceID() const;
  virtual OdUInt64 getUniqueDBID() const;
  virtual HierarchyTreeObjectType getNodeType() const;

  virtual void setNodeName(const OdString& name);
  virtual void setUniqueSourceID(const OdUInt64 id);
  virtual void setNodeType(const HierarchyTreeObjectType type);
  virtual void setUniqueDBID(const OdUInt64 id);

  virtual const OdRxModelTreeBaseNodeRawPtrArray& getParents() const;
  virtual const OdRxModelTreeBaseNodePtrArray& getChildren() const;

  virtual bool addChild(OdRxModelTreeBaseNode* parent, OdRxModelTreeBaseNodePtr& elem);

  virtual void outFields(OdBaseHierarchyTreeFiler* pFiler) const;

  virtual void serializeProprties(OdBaseHierarchyTreeFiler* pFiler, const PropertyInfo& info) const;

  virtual OdResult inFields(OdBaseHierarchyTreeFiler* pFiler, std::set<OdString>& string_storage);

  virtual void deserializeProprties(OdBaseHierarchyTreeFiler* pFiler, PropertyInfo& info, std::set<OdString>& string_storage);

  virtual const OdArray<PropertyInfo>& getProperties() const;

  virtual void addProperty(PropertyInfo& info);

protected:

  friend class OdRxModelHierarchyTreeBaseImpl;

  OdString                             m_Name;
  OdUInt64                             m_UniqueSourceID;
  OdUInt64                             m_UniqueDBID;
  OdRxModelTreeBaseNodeRawPtrArray     m_Parents;
  OdRxModelTreeBaseNodePtrArray        m_Children;
  HierarchyTreeObjectType              m_NodeType;
  OdArray<PropertyInfo>                m_Properties;
};


#endif
