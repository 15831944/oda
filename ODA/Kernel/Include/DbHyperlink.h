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



#ifndef _ODDBHYPERLINK_INCLUDED_
#define _ODDBHYPERLINK_INCLUDED_

#include "TD_PackPush.h"
#include "RxObject.h"
#include "DbStubPtrArray.h"

/** \details
  This class is the base class for Hyperlink objects.

  Corresponding C++ library: TD_Db

  \remarks
  OdDbHyperlink objects contain the following data:

  <table>
  Name               Description 
  Hyperlink Name     Filename or URL.
  Sublocation        Named view, range of cells, etc.
  Description        Display name for a Hyperlink.
  </table>

  \sa
  * OdDbEntityHyperlinkPE
  * OdDbHyperlink
  * OdDbHyperlinkCollection

  <group OdDb_Classes>
*/
class DBROOT_EXPORT OdDbHyperlink
{
public:
  /** \details
    This enumeration specifies flags that can be set for a hyperlink object.
  */
  enum HLinkFlags 
  {
    /** Undefined */
    kUndefined = 0,
    /** Convert DWG to DWF */
    kConvertDwgToDwf = 0x1
  };

  /** \details
    Default constructor. Creates a new object of the OdDbHyperlink class.
  */
  OdDbHyperlink() {};

  /** \details
    Virtual destructor. Destroys an object of the OdDbHyperlink class and
    releases all resources that were used by the object.
  */
  virtual ~OdDbHyperlink(){};

  /** \details
    Returns the name associated with this Hyperlink object.
  */
  virtual const OdString name() const = 0;

  /** \details
    Sets the name associated with this Hyperlink object.

    \param name [in]  Name for this Hyperlink object.
  */
  virtual void setName(
    const OdString& name) = 0;

  /** \details
    Returns the description associated with this Hyperlink object.
  */
  virtual const OdString description() const = 0;

  /** \details
    Sets the description associated with this Hyperlink object.

    \param description [in]  Description for this Hyperlink object.

    \remarks
    "<Name> - <SubLocation>" will be returned by getDisplayString() if
    description is NULL.
  */
  virtual void setDescription(
    const OdString& description) = 0;

  /** \details
    Returns the sublocation associated with this Hyperlink object.
  */
  virtual const OdString subLocation() const = 0;

  /** \details
    Sets the sublocation associated with this Hyperlink object.

    \param subLocation [in]  Sublocation for this Hyperlink object.
  */
  virtual void setSubLocation(
    const OdString& subLocation) = 0;

  /** \details
    Returns the display string associated with this Hyperlink object.

    \remarks
    Returns "<Name> - <SubLocation>" if the description set by setDescription()
    is NULL.
  */
  virtual const OdString getDisplayString() const = 0;

  /** \details
    Returns true if and only if this Hyperlink object is associated with the
    outermost container.
  */
  virtual bool isOutermostContainer() const = 0;

  /** \details
    Returns the nesting level of this Hyperlink object.
  */

  virtual int getNestedLevel() const = 0;

  /** \details
    Gets flags for this Hyperlink object.

    \remarks
    Flag values are defined by the HLinkFlags enumeration.
  */
  virtual OdInt32 flags() const  = 0;

  /** \details
    Sets flags for this Hyperlink object.

    \param lFlags [in] Flags to set.

    \remarks
    Flag values are defined by the HLinkFlags enumeration.
  */
  virtual void setFlags(OdInt32 lFlags) = 0;

};

/** \details
  This class is a container object for OdDbHyperlink objects.

  \sa
  * OdDbEntityHyperlinkPE
  * OdDbHyperlink
  * OdDbHyperlinkCollection

  Corresponding C++ library: TD_Db

  <group OdDb_Classes>
*/
class DBROOT_EXPORT OdDbHyperlinkCollection : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbHyperlinkCollection);
  //DOM-IGNORE-END

  /** \details
    Default constructor. Creates a new object of the OdDbHyperlinkCollection
    class.
  */
  OdDbHyperlinkCollection() {};

  /** \details
    Virtual destructor. Destroys an object of the OdDbHyperlinkCollection class
    and releases all resources that were used by the object.
  */
  virtual ~OdDbHyperlinkCollection(){};

  /** \details
    Creates a HyperLink object and adds it to the beginning of this
    HyperlinkCollection object.

    \param name        [in]  Name for the Hyperlink object.
    \param description [in]  Description for the Hyperlink object.
    \param subLocation [in]  Sublocation for the Hyperlink object.
  */
  virtual void addHead(
    const OdString& name, 
    const OdString& description, 
    const OdString& subLocation = OdString::kEmpty) = 0;

  /** \details
    Creates a HyperLink object and adds it to the end of this
    HyperlinkCollection object.

    \param name        [in]  Name for the Hyperlink object.
    \param description [in]  Description for the Hyperlink object.
    \param subLocation [in]  Sublocation for the Hyperlink object.
  */
  virtual void addTail(
    const OdString& name, 
    const OdString& description, 
    const OdString& subLocation = OdString::kEmpty) = 0;

  /** \details
    Creates a HyperLink object and inserts it into this
    HyperlinkCollection object at the specified index.

    \param hyperlinkIndex [in]  Hyperlink index.
    \param name           [in]  Name for the Hyperlink object.
    \param description    [in]  Description for the Hyperlink object.
    \param subLocation    [in]  Sublocation for the Hyperlink object.

    \remarks
    Objects at or after index are moved toward the tail.
  */
  virtual void addAt(
    const int hyperlinkIndex,
    const OdString& name, 
    const OdString& description, 
    const OdString& subLocation = OdString::kEmpty) = 0;

  /** \details
    Removes the first Hyperlink object in this HyperlinkCollection object.
  */
  virtual void removeHead() = 0;

  /** \details
    Removes the last Hyperlink object in this HyperlinkCollection object.
  */
  virtual void removeTail() = 0;

  /** \details
    Removes a Hyperlink object at the specified location in this
    HyperlinkCollection object.

    \param hyperlinkIndex [in]  Index of the Hyperlink to remove.
  */
  virtual void removeAt(
    const int hyperlinkIndex) = 0;

  /** \details
    Returns the number of HyperLink objects in this HyperlinkCollection object.
  */
  virtual int count() const = 0;

  /** \details
    Returns the HyperLink object at the specified location in this
    HyperlinkCollection object.

    \param hyperlinkIndex [in]  Hyperlink index.
  */
  virtual OdDbHyperlink * item(
    const int hyperlinkIndex) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdDbHyperlinkCollection object smart pointers.
*/
typedef OdSmartPtr<OdDbHyperlinkCollection> OdDbHyperlinkCollectionPtr;


/** \details
  This class is the Hyperlink Protocol Extension class.

  \remarks
  This class provides access to the Hyperlink objects associated with
  OdDbDatabase objects.

  \sa
  * OdDbEntityHyperlinkPE
  * OdDbHyperlink
  * OdDbHyperlinkCollection

  Corresponding C++ library: TD_Db

  <group OdDb_Classes>
*/
class DBROOT_EXPORT OdDbEntityHyperlinkPE : public OdRxObject
{
public:

  /** \details
    Default constructor. Creates a new object of the OdDbEntityHyperlinkPE
    class.
  */
  OdDbEntityHyperlinkPE();

  /** \details
    Virtual destructor. Destroys an object of the OdDbEntityHyperlinkPE class
    and releases all resources that were used by the object.
  */
  virtual ~OdDbEntityHyperlinkPE();

  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbEntityHyperlinkPE);
  //DOM-IGNORE-END

  /** \details
    Creates a HyperlinkCollection object containing the Hyperlink object
    associated with the specified object.

    \param pObject               [in]  Pointer to the object.
    \param oneOnly               [in]  If true, returns only the first
                                       Hyperlink.
    \param ignoreBlockDefinition [in]  If true, ignores Hyperlink objects
                                       associated with block definitions.
    \returns
    Smart pointer to the created HyperlinkColection object.

    \remarks
    Currently the ignoreBlockDefinition argument is ignored.
    It will be fully implemented in future releases.
  */
  virtual OdDbHyperlinkCollectionPtr getHyperlinkCollection(
    const OdRxObject* pObject, 
    bool oneOnly = false, 
    bool ignoreBlockDefinition = true) = 0;

  /** \details
    Creates a HyperlinkCollection object containing Hyperlink objects associated
    with the specified objects.

    \param objectIds             [in]  Array of Object IDs.
    \param oneOnly               [in]  If true, returns only the first
                                       Hyperlink.
    \param ignoreBlockDefinition [in]  If true, ignores Hyperlink objects
                                       associated with block definitions.
    \returns
    Smart pointer to the created HyperlinkColection object.

    \remarks
    Currently the ignoreBlockDefinition argument is ignored.
    It will be fully implemented in future releases.
  */
  virtual OdDbHyperlinkCollectionPtr getHyperlinkCollection(
    const OdDbStubPtrArray*& objectIds,
    bool oneOnly = false, 
    bool ignoreBlockDefinition = true) = 0;

  /** \details
    Associates the specified HyperlinkCollection object with the specified
    object.

    \param pObject [in]  Pointer to the target object.
    \param pHCO    [in]  Pointer to the HyperlinkCollection object.

    \remarks
    Hyperlink objects with NULL name, description or sublocation will not be
    added to the target object.
  */
  virtual void setHyperlinkCollection(
    const OdRxObject * pObject, 
    OdDbHyperlinkCollection * pHCO) = 0;

  /** \details
    Returns the number of Hyperlink objects associated with the specified
    object.

    \param pObject               [in]  Pointer to the object.
    \param ignoreBlockDefinition [in]  If true, ignores Hyperlink objects
                                       associated with block definitions.
    \returns
    Number of Hyperlink objects.

    \remarks
    Currently the ignoreBlockDefinition argument is ignored.
    It will be fully implemented in future releases.
  */
  virtual unsigned int getHyperlinkCount(
    const OdRxObject * pObject, 
    bool ignoreBlockDefinition = true) = 0;

  /** \details
    Returns the number of Hyperlink objects associated with the specified
    objects.

    \param idContainers          [in]  Array of object IDs.
    \param ignoreBlockDefinition [in]  If true, ignores Hyperlink objects
                                       associated with block definitions.

    \returns
    Number of Hyperlink objects.

    \remarks
    Currently the ignoreBlockDefinition argument is ignored.
    It will be fully implemented in future releases.
  */
  virtual unsigned int getHyperlinkCount(
    const OdDbStubPtrArray *& idContainers, 
    bool ignoreBlockDefinition = true) = 0;

  /** \details
    Returns true if and only if the specified object has Hyperlink objects
    associated with it.

    \param pObject               [in]  Pointer to the object.
    \param ignoreBlockDefinition [in]  If true, ignores Hyperlink objects
                                       associated with block definitions.
    \returns
    True if the specified object has Hyperlink objects associated with it; false
    otherwise.

    \remarks
    Currently the ignoreBlockDefinition argument is ignored.
    It will be fully implemented in future releases.
  */
  virtual bool hasHyperlink(
    const OdRxObject * pObject, 
    bool ignoreBlockDefinition = true) = 0;

  /** \details
    Returns true if and only if at least one of the specified objects has
    Hyperlink objects associated with it.

    \param objectIds             [in]  Array of object IDs.
    \param ignoreBlockDefinition [in]  If true, ignores Hyperlink objects
                                       associated with block definitions.
    \returns
    True if one or more of the specified objects have Hyperlink objects
    associated with them; false if none of the specified objects have Hyperlink
    objects associated with them.

    \remarks
    Currently the ignoreBlockDefinition argument is ignored.
    It will be fully implemented in future releases.
  */
  virtual bool hasHyperlink(
    const OdDbStubPtrArray *& objectIds, 
    bool ignoreBlockDefinition = true) = 0;

};
/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdDbEntityHyperlinkPE object smart pointers.
*/
typedef OdSmartPtr<OdDbEntityHyperlinkPE> OdDbEntityHyperlinkPEPtr;

/** \details
  This type specifies callback functions that are called when a Hyperlink
  destructor call is detected. Used by the
  odrxRegisterHyperlinkDestructorCallback() function.
*/
typedef void (*ODDBHYPERLINK_CALLBACK)(const OdDbHyperlink*);

/** \details
  Registeres a callback function to be called when a Hyperlink destructor call
  is detected. This registration allows you to catch the invokation of each
  Hyperlink destructor.

  \param callbackFunc [in]  Function to be called when a Hyperlink destructor
                            call is detected.
*/
DBROOT_EXPORT void odrxRegisterHyperlinkDestructorCallback(ODDBHYPERLINK_CALLBACK callbackFunc);

/** \details
  Unregisteres a previously registered callback function to be called when a
  Hyperlink destructor call is detected.
*/
DBROOT_EXPORT void odrxUnregisterHyperlinkDestructorCallback();
#include "TD_PackPop.h"
#endif // _ODDBHYPERLINK_INCLUDED_
