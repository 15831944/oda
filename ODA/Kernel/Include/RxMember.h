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

#ifndef _ODRXMEMBER_INCLUDED_
#define _ODRXMEMBER_INCLUDED_

#include "OdString.h"
#include "OdArray.h"
#include "RxOverrule.h"
#include "SharedPtr.h"

//DOM-IGNORE-BEGIN
class OdRxValue;
class OdRxValueType;
class OdRxAttributeCollection;
class OdRxPromotingQueryContext;

class OdRxMemberIteratorImpl;
class OdRxMemberQueryContextImpl;
class OdRxMemberQueryEngineImpl;
class OdRxMemberImpl;
class OdRxMemberCollectionImpl;

class OdRxMember;
//DOM-IGNORE-END

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdRxMember object pointers.
*/
typedef OdSmartPtr<OdRxMember> OdRxMemberPtr;

/** \details
  Base class for properties and methods.
  
  <group OdRx_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxMember : public OdRxObject
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxMember);
//DOM-IGNORE-END

  /** \details
    Static unlocalized name of the property.

    \returns
    Name of the property.
  */
  const OdString& name() const throw();

  /** \details
    Localized name (can be loaded from resources via OdRxLocalizedNameAttribute::getLocalizedName).

    \returns
    Localized name of the property.
  */
  OdString localName() const throw();

  /** \details
    Property value type.

    \returns
    Property value type.
  */
  const OdRxValueType& type()const throw();

  /** \details
    Attributes are used on some properties to modify their behaviour. E.g.
    OdRxLocalizedNameAttribute defines localName().
    See RxAttributes.h for more information.

    \returns
    Collection of property attributes.
  */
  const OdRxAttributeCollection& attributes() const throw();

  /** \details
    Attributes are used on some properties to modify their behaviour. E.g.
    OdRxLocalizedNameAttribute defines localName().
    See RxAttributes.h for more information.

    \returns
    Collection of property attributes.
  */
  OdRxAttributeCollection& attributes() throw();

  /** \details
    For the usual properties owner is the OdRxClass they are attached to.
    For subproperties owner is the main property.

    \returns
    Owner of the property.
  */
  const OdRxObject* owner() const throw();

  /** \details
    Some properties may have child properties, e.g. "EndPoint" property of
    OdDbLine has subproperties "EndPoint/X", "EndPoint/Y", "EndPoint/Z".

    \returns
    Array of subproperties (child properties).
  */
  const OdArray<OdRxMemberPtr>* children() const throw();

  /** \details
    This method should be used to delete properties while unloading. It removes
    this member from the owner, and fires "goodbye" reactors registered with the
    OdRxMemberQueryEngine.

    \param pMember [in] Property to be removed.
  */
  static void deleteMember(const OdRxMember* pMember) throw();
protected:
  friend class OdRxMemberImpl;
  virtual ~OdRxMember();
  OdRxMember(OdRxMemberImpl*);
  
  /** \details
    Descendants may overload localized name obtaining method.
  */
  virtual OdString subLocalName() const;
  
  /** \details
    First call to children calls subChildren, if it returned eNotApplicable then
    subChildrenEx is called. subChildren() is expected to adds members children
    array.
  */
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const;

  /** \details
    subChildrenEx allows adding properties in bulk. pChildren array is managed
    by the function itself.
  */
  virtual OdResult subChildrenEx(OdArray<OdRxMemberPtr>*& pChildren) const;

  /** \details
    Initialization method used by descendants to assign unlocalized name, type
    and owner of this member.
  */
  void init(const OdString& name, const OdRxValueType* type, const OdRxObject* owner = 0);
  OdRxMemberImpl* m_pImpl;
};

//DOM-IGNORE-BEGIN
class OdRxEnumTag;
//DOM-IGNORE-END

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdRxEnumTag object pointers.
*/
typedef OdSmartPtr<OdRxEnumTag> OdRxEnumTagPtr;
//DOM-IGNORE-BEGIN
class OdRxEnumTagImpl;
//DOM-IGNORE-END

/** <group OdRx_Classes>
  \details
  This class describes a single item of an enumeration in OdRxEnumType.
*/
class FIRSTDLL_EXPORT OdRxEnumTag : public OdRxMember
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxEnumTag);
//DOM-IGNORE-END

  /** \details
    Creates a new enumeration member descriptor.

    \param name  [in] Name of the enumeration item.
    \param value [in] Value, containing the underlying enumeration item.
    \param owner [in] Owner of the enumeration item (OdRxEnumType).

    \returns
    Pointer to the created enumeration member descriptor.
  */
  static OdRxEnumTagPtr createObject(const OdChar* name, const OdRxValue& value, const OdRxObject* owner);

  /** \details
    Value with the element of the enumeration, described by this tag.

    \returns
    Value, described by this tag.
  */
  const OdRxValue& value() const;

  /** \details
    This function queries OdRxAlternateLocalizedNameAttribute to retrieve
    the alternative local name.

    \returns
    Alternate local name.
  */
  OdString alternateLocalName() const;
protected:
  OdRxEnumTag(OdRxMemberImpl*);
  /** \details
    Overloaded implementation of alternateLocalName() function. Should use OdRxAlternateLocalizedNameAttribute by default
  */
  virtual OdString subAlternateLocalName() const;
};

/** <group OdRx_Classes>
  \details
  Simple container interface for the members.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxMemberCollection
{
public:

  /** \details
    Destructor for objects of the OdRxMemberCollection class.
  */
  virtual ~OdRxMemberCollection();

  /** \details
    Returns the number of members in the collection.

    \returns
    Number of members.
  */
  virtual int count() const = 0;

  /** \details
    Returns the member at the specified index.

    \param index [in] Index of the member to be got.

    \returns
    Member at the specified index.
  */
  virtual OdRxMember* getAt(int index) const = 0;
};

/** <group OdRx_Classes>
  \details
  Helper class passed to the user callback to add new members to the class.
*/
class FIRSTDLL_EXPORT OdRxMemberCollectionBuilder
{
public:

  /** \details
    Adds specified member to the class member collection.

    \param member [in] Member to be added.
  */
  void add(OdRxMember* member);

  /** \details
    The object that members are being added to.

    \returns
    Owner object.
  */
  const OdRxClass* owner() const { return m_owner; }
private:
  friend class OdRxMemberCollectionImpl;
  OdRxMemberCollectionBuilder(OdRxMemberCollectionImpl* imp, const OdRxClass* owner) : m_pImpl(imp), m_owner(owner) {}
  const OdRxClass* m_owner;
  OdRxMemberCollectionImpl* m_pImpl;
};

/** <group OdRx_Classes>
  \details
  Member iterator interface. Used by OdRxMemberQueryEngine.
*/
class FIRSTDLL_EXPORT OdRxMemberIterator
{
public:

  /** \details
    Destructor for objects of the OdRxMemberIterator class.
  */
  virtual ~OdRxMemberIterator();

  /** \details
    Returns true if the iteration is finished.

    \returns
    True if the traversal by this iterator object is complete; false otherwise.
  */
  virtual bool done();

  /** \details
    Step to the next member.

    \returns
    True if the iteration succeded; false otherwise.
  */
  virtual bool next();

  /** \details
    Returns the member currently being pointed at by the iterator.

    \returns
    Member, currently being pointed at.
  */
  virtual OdRxMember* current() const;

  /** \details
    Iterates the list until member with that name is found (otherwise returns
    NULL).

    \param name [in] Name of the member to be found.

    \returns
    Pointer to the member if it was found; NULL otherwise.
  */
  virtual OdRxMemberPtr find(const OdChar* name) const;
  
  /** \details
    Returns the number of iterated objects.

    \returns the size of the collection by which iteration is performed.
  */
  virtual OdUInt64 size() const;
  
protected:
  OdRxMemberIterator();
private:
  friend class OdRxMemberIteratorImpl;
  OdRxMemberIteratorImpl* m_pImpl;
};

/** \details
  This template class is a specialization of the OdSharedPtr class for
  OdRxMemberIterator object pointers.
*/
typedef OdSharedPtr<OdRxMemberIterator> OdRxMemberIteratorPtr;

/** <group OdRx_Classes>
  \details
  Class that actually makes a list of members for the iterator.
  It is called by the engine with the list of facets it has collected.
  Facets include object class. It's up to the context to check up the class
  parents.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxMemberQueryContext : public OdRxObject
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxMemberQueryContext);
//DOM-IGNORE-END

  /** \details
    Builds an iterator listing all the members of these classes (optionally
    including members of parents and children of members).
    Parameters are called "facets", because they can include "extension"
    classes.

    \param facets [in] Array of objects.

    \returns
    Pointer to the iterator of members of specified objects.
  */
  OdRxMemberIteratorPtr newMemberIterator(const OdArray<OdRxClassPtr>& facets) const;

  /** \details
    Gets the pointer to the member of the class by name.

    \param facets [in] Array of objects.
    \param name   [in] The pointer to the name of member to be found.

    \returns
    Pointer to the member if it was found; NULL otherwise.
  */
  virtual OdRxMemberPtr find(const OdArray<OdRxClassPtr>& facets, const OdChar* name) const;
protected:
  friend class OdRxMemberQueryContextImpl;
  OdRxMemberQueryContextImpl* m_pImpl;
  OdRxMemberQueryContext();
  virtual ~OdRxMemberQueryContext();
  virtual OdRxMemberIteratorPtr subNewMemberIterator(const OdArray<OdRxClassPtr>& facets) const = 0;
};

/** <group OdRx_Classes>
  \details
  This interface provides a way to extend a given class (or an object) with
  methods/properties.
*/
struct ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxFacetProvider : OdRxObject
{
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxFacetProvider);
//DOM-IGNORE-END

  /** \details
    This method is called every time when OdRxMemberQueryEngine::find or
    OdRxMemberQueryEngine::newMemberIterator is called.
    Facet is an extension class which is used only to get a list of members.
    Facet provider may add different facets depending on context and object.

    \param pO       [in] Object to get facets of.
    \param pContext [in] Context.
    \param facets  [out] Array of facets.
  */
  virtual void getFacets(const OdRxObject* pO, const OdRxMemberQueryContext* pContext, OdArray<OdRxClassPtr>& facets) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdRxFacetProvider object pointers.
*/
typedef OdSmartPtr<OdRxFacetProvider> OdRxFacetProviderPtr;

/** <group OdRx_Classes>
  \details
  Member engine reactor. Currently supports only "goodbye" event.
*/
class FIRSTDLL_EXPORT OdRxMemberReactor
{
public:

  /** \details
    Destructor for objects of the OdRxMemberReactor class.
  */
  virtual ~OdRxMemberReactor() {}

  /** \details
    Called when a member is removed from the owner (OdRxMember::deleteMember).
  */
  virtual void goodbye(const OdRxMember* ) {}
};


/** <group OdRx_Classes>
  \details
  This class provides ability to enumerate and find members associated with the
  given object, add new property providers (OdRxFacetProvider) and reactors.
*/
class FIRSTDLL_EXPORT OdRxMemberQueryEngine
{
public:

  /** \details
    This singleton object is the main access point to the member engine
    services.

    \returns
    The member engine services access point object.
  */
  static OdRxMemberQueryEngine* theEngine();

  /** \details
    Finds a member by name, uses defaultContext() if no context is passed.

    \param pO           [in] Object to search the member in.
    \param name         [in] Name of the member to be found.
    \param pContext     [in] Context. Optional.
    \param bQueryFacets [in] Flag determines if the function will query members
                             added dynamically by facet providers. Default value
                             is true.

    \returns
    Pointer to found member.
  */
  OdRxMemberPtr find(const OdRxObject* pO, const OdChar* name, const OdRxMemberQueryContext* pContext = NULL, bool bQueryFacets = true) const;

  /** \details
    Enumerates all the properties of the object, uses defaultContext() if no
    context is passed.

    \param pO       [in] Object to enumerate the properties of.
    \param pContext [in] Context. Optional.

    \returns
    Pointer to the iterator of the enumerated properties.
  */
  OdRxMemberIteratorPtr newMemberIterator(const OdRxObject* pO, const OdRxMemberQueryContext* pContext = NULL) const;

  /** \details
    Reference to the default query context.
    Query context handles collecting members in a list.

    \returns
    Query context.
  */
  const OdRxMemberQueryContext* defaultContext() const;

  /** \details
    Reference to the default promoting query context.
    Handles collecting members in a list, but unlike the default, this one
    includes children (sub-properties) in the list, instead of parents.
    For example OdDbLine property "EndPoint" would be replaced by "EndPoint/X",
    "EndPoint/Y", "EndPoint/Z" if queried using this context.

    \returns
    Promoting query context.
  */
  const OdRxPromotingQueryContext* promotingContext() const;

  /** \details
    Register a new facet provider (to provide extension classes with additional
    members).

    \param pProvider [in] Facet provider to be registered.
  */
  void addFacetProvider(OdRxFacetProvider* pProvider);

  /** \details
    Unregister an extension provider.

    \param pProvider [in] Facet provider to be removed.
  */
  void removeFacetProvider(OdRxFacetProvider* pProvider);

  /** \details
    Register a new reactor.

    \param pReactor [in] Reactor to be registered.
  */
  void addReactor(OdRxMemberReactor* pReactor);

  /** \details
    Unregister a reactor.

    \param pReactor [in] Reactor to be removed.
  */
  void removeReactor(OdRxMemberReactor* pReactor);
private:
  friend class OdRxMemberQueryEngineImpl;
  OdRxMemberQueryEngineImpl* m_pImpl;
  OdRxMemberQueryEngine(OdRxMemberQueryEngineImpl*);
};


/** <group OdRx_Classes>
  \details
  Base class for properties overrules.
  Works the same as usual overrules, but per member - not per class.
  Currently not implemented.
*/
class FIRSTDLL_EXPORT OdRxMemberOverrule : public OdRxObject
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxMemberOverrule);
//DOM-IGNORE-END

  /** \details
    Register a new overruling.

    \param pOverruledSubject [in] Overruled member.
    \param pOverrule         [in] Overrule.
    \param bAddAtLast        [in] If true the overrule is added last. Optional.

    \returns
    eOk if operation succeded.

    \remarks
    Not implemented yet.
  */
  static OdResult addOverrule(OdRxMember* pOverruledSubject, OdRxMemberOverrule* pOverrule, bool bAddAtLast = false);

  /** \details
    Unregister an overruling.

    \param pOverruledSubject [in] Overruled member.
    \param pOverrule         [in] Overrule.

    \returns
    eOk if operation succeded.

    \remarks
    Not implemented yet.
  */
  static OdResult removeOverrule(OdRxMember* pOverruledSubject, OdRxMemberOverrule* pOverrule);
};

#endif 
