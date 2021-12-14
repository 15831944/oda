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

#ifndef _ODRXHIERARCHYTREEBASE_INCLUDED_
#define _ODRXHIERARCHYTREEBASE_INCLUDED_

#include "RxModelTreeBaseNode.h"
#define STL_USING_SET
#include "OdaSTL.h"

//DOM-IGNORE-BEGIN
class OdRxModelHierarchyTreeBaseImpl;
class OdRxModelHierarchyTreeBase;
//DOM-IGNORE-END

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdRxModelHierarchyTreeBase objects.
*/
typedef OdSmartPtr<OdRxModelHierarchyTreeBase> OdRxModelHierarchyTreeBasePtr;

/** \details
  This template class is a specialization of the OdArray class for
  OdRxModelHierarchyTreeBasePtr objects.
*/
typedef OdArray<OdRxModelHierarchyTreeBasePtr> OdRxModelHierarchyTreeBasePtrArray;

/** \details
  This class is a base class that implements a database hierarchical tree for
  the Common Data Access API.

  <group OdRx_Classes>
*/
class ODCDA_EXPORT OdRxModelHierarchyTreeBase : public OdRxObject
{
  public:
    //DOM-IGNORE-BEGIN
    ODRX_DECLARE_MEMBERS(OdRxModelHierarchyTreeBase);
    ODRX_HEAP_OPERATORS();
    //DOM-IGNORE-END

    /** \details
      Destroys an instance of the OdRxModelHierarchyTreeBase class.
    */
    virtual ~OdRxModelHierarchyTreeBase();

    /** \details
      Creates a hierarchical tree for the database.

      \param pDb                       [in] Pointer to the source database object.
      \param create_properties_cache   [in] Flag to create cache of properties. Default value is true.
      \param class_dependent_hierarchy [in] Flag to optimize the creation of a hierarchical tree.
                                            It can only be set to true if the properties of database
                                            objects with a hierarchical attribute are statically
                                            dependent on the object type. Default value is false.
      \param class_dependent_property  [in] Flag to optimize cache creation. It can be set to true only
                                            if all properties of database objects are statically
                                            dependent on the object type. Default value is false.
      \returns Smart pointer to the root node of the hierarchical tree.
    */
    virtual OdRxModelTreeBaseNodePtr createDatabaseHierarchyTree(const OdRxObject* pDb, const bool create_properties_cache = true,
      const bool class_dependent_hierarchy = false, const bool class_dependent_property = false);

    /** \details
      Creates a hierarchical tree for the database.

      \param pDb                       [in] Pointer to the source database object.
      \param collected_properties      [in] Pointer to a set of property names that will be added
                                            to the cache. If NULL, all properties will be cached.
      \param class_dependent_hierarchy [in] Flag to optimize the creation of a hierarchical tree.
                                            It can only be set to true if the properties of database
                                            objects with a hierarchical attribute are statically
                                            dependent on the object type. Default value is false.
      \param class_dependent_property  [in] Flag to optimize cache creation. It can be set to true only
                                            if all properties of database objects are statically
                                            dependent on the object type. Default value is false.
      \returns Smart pointer to the root node of the hierarchical tree.
    */
    virtual OdRxModelTreeBaseNodePtr createDatabaseHierarchyTree(const OdRxObject* pDb, const std::set<OdString>* collected_properties,
      const bool class_dependent_hierarchy = false, const bool class_dependent_property = false);

    /** \details
      Gets the root node of the hierarchical tree.

      \returns Smart pointer to the root node of the tree.
    */
    virtual OdRxModelTreeBaseNodePtr getDatabaseNode() const;

    /** \details
      Gets the properties for a node by unique identifier.

      \param unique_DB_ID     [in] Unique Database identifier represented as OdUInt64.
      \param unique_source_ID [in] Unique identifier represented as OdUInt64.
      \returns Array of PropertyInfo.
    */
    virtual const OdArray<PropertyInfo>& getNodeProperties(OdUInt64 unique_DB_ID, OdUInt64 unique_source_ID);

    /** \details
      Serializes the hierarchical tree.

      \param pDb [in] Pointer to a filer object.
    */
    void serialize(OdBaseHierarchyTreeFiler* pFiler) const;

    /** \details
      Deserializes the hierarchical tree.

      \param pDb [in] Pointer to a filer object.
      \returns Result of the deserialization operation.
    */
    OdResult deserialize(OdBaseHierarchyTreeFiler* pFiler);

    //DOM-IGNORE-BEGIN
  protected:
    OdRxModelHierarchyTreeBase();
    OdRxModelHierarchyTreeBase(OdRxModelHierarchyTreeBaseImpl* impl);
    friend class OdRxModelHierarchyTreeBaseImpl;
    OdRxModelHierarchyTreeBaseImpl* m_pImpl;
    //DOM-IGNORE-END
};

#endif

