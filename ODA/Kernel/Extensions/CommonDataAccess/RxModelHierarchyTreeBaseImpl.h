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

#ifndef _ODRXHIERARCHYTREEBASEIMPL_H_DEFINED_
#define _ODRXHIERARCHYTREEBASEIMPL_H_DEFINED_

#include "RxModelHierarchyTreeBase.h"
#include "../Include/RxModelTreeBaseNode.h"
#include "../Include/RxProperty.h"
#include "../Include/RxAttribute.h"
#define STL_USING_ALL
#include "OdaSTL.h"
#include "OdHashMap.h"


class ODCDA_EXPORT OdRxModelHierarchyTreeBaseImpl
{
public:
  ODRX_HEAP_OPERATORS();
  static OdRxModelHierarchyTreeBaseImpl* getImpl(const OdRxModelHierarchyTreeBasePtr& pObj)
  {
    return const_cast<OdRxModelHierarchyTreeBaseImpl*>(pObj->m_pImpl);
  }

  OdRxModelHierarchyTreeBaseImpl();
  virtual ~OdRxModelHierarchyTreeBaseImpl();

  virtual OdRxModelTreeBaseNodePtr createDatabaseHierarchyTree(const OdRxObject* pDb, const bool create_properties_cache = true, 
    const bool class_dependent_hierarchy = false, const bool class_dependent_property = false);

  virtual OdRxModelTreeBaseNodePtr createDatabaseHierarchyTree(const OdRxObject* pDb, const std::set<OdString>* collected_properties, 
    const bool class_dependent_hierarchy = false, const bool class_dependent_property = false);

  virtual OdRxModelTreeBaseNodePtr getDatabaseNode() const;

  void serialize(OdBaseHierarchyTreeFiler* pFiler) const;

  OdResult deserialize(OdBaseHierarchyTreeFiler* pFiler);

  virtual const OdArray<PropertyInfo>& getNodeProperties(OdUInt64 unique_DB_ID, OdUInt64 unique_source_ID);

protected:

  struct AttributesInfo
  {
    AttributesInfo()
      :pNameAttribute(NULL)
      ,pHierarchyAttribute(NULL)
      ,pUiPlacementAttribute(NULL)
      ,pLocalizedNameAttribute(NULL)
      ,is_collection(false)
      ,is_property(false)
    {}
    OdRxDisplayAsAttribute* pNameAttribute;
    OdRxHierarchyLevelAttribute* pHierarchyAttribute;
    OdRxUiPlacementAttribute* pUiPlacementAttribute;
    OdRxLocalizedNameAttribute* pLocalizedNameAttribute;
    bool is_collection;
    bool is_property;
  };

  virtual OdRxModelTreeBaseNodePtr createNodeObject() = 0;

  void processNode(OdRxModelTreeBaseNode* pParent, const OdRxObject* pObj, std::map < OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> >& cache,
    const bool create_properties_cache, const bool class_dependent_hierarchy, const bool class_dependent_property, const OdUInt64 databaseID);

  void processElement(OdRxModelTreeBaseNode* pParent, const AttributesInfo& attributes_info, 
    OdRxValue& prop_value, std::map < OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> >& cache,
    const bool create_properties_cache, const bool class_dependent_hierarchy, const bool class_dependent_property, const OdUInt64 databaseID);
  
  void processRxProperty(OdRxModelTreeBaseNode* pParent, const OdRxObject* pObj, OdRxProperty* pProperty, PropertyInfo* info,
    const AttributesInfo& attributes_info, std::map < OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> >& cache,
    const bool create_properties_cache, const bool class_dependent_hierarchy, const bool class_dependent_property, const OdUInt64 databaseID);

  void processRxCollection(OdRxModelTreeBaseNode* pParent, const OdRxObject* pObj, const OdRxCollectionProperty* pPropertyCollection, PropertyInfo* info,
    const AttributesInfo& attributes_info, std::map < OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> >& cache,
    const bool create_properties_cache, const bool class_dependent_hierarchy, const bool class_dependent_property, const OdUInt64 databaseID);

  void processProperty(OdRxModelTreeBaseNode* pParent, const OdRxObject* pObj, const OdRxMember* pRxMember, PropertyInfo* info,
    std::map < OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> >& cache, const bool create_properties_cache, const OdUInt64 databaseID);

  void processPropertyInternal(OdRxModelTreeBaseNode* pParent, const OdRxObject* pObj, const OdRxMember* pRxMember, PropertyInfo* info,
    std::map < OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> >& cache, const bool create_properties_cache, const AttributesInfo& attributes_info,
    const bool class_dependent_hierarchy, const bool class_dependent_property, const OdUInt64 databaseID);

  struct NodeCache
  {
    OdRxModelTreeBaseNodePtr m_pNode;
    OdArray<OdUInt64>        m_IDs;
  };
  OdUInt64 createNodeCache(const OdRxModelTreeBaseNodePtr& pNode, OdArray<NodeCache>& cache, std::map<OdRxModelTreeBaseNode*, OdUInt64>& id_cahe) const;

  OdRxModelTreeBaseNodePtr  m_DatabaseNode;

  std::map<OdUInt64, std::map<OdUInt64, OdRxModelTreeBaseNodePtr> > m_NodeCache;
  std::set<OdString> m_CollectedProperties;

#ifdef UNORDEREDMAP_SUPPORTED
  template<class T> struct PointerHash { size_t operator()(const T* m) const { return (size_t)m; } }; // for unique objects only
  typedef OdHashMap<OdRxMember*, AttributesInfo, PointerHash<OdRxMember> > ClassAttributeInfo;
  typedef OdHashMap<OdRxClass*, ClassAttributeInfo, PointerHash<OdRxClass> > AttributeMap;
#else
  typedef std::map<OdRxMember*, AttributesInfo> ClassAttributeInfo;
  typedef std::map<OdRxClass*, ClassAttributeInfo> AttributeMap;
#endif

  std::set<OdString> m_StringStorage;

  AttributeMap m_AttributeMap;
  ClassAttributeInfo* buildAttributeMap(const OdRxObject* pObj);
  static void collectAttributes(const OdRxMember* pMember, AttributesInfo& attributes_info);
};

#endif
