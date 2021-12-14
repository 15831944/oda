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

#ifndef __TF_OBJECTREACTOR_H__
#define __TF_OBJECTREACTOR_H__

#include "TD_PackPush.h"

#include "RxObject.h"
#include "TfExport.h"

class OdTfObject;
class OdDbStub;

/** \details
  This class is the base class for custom classes that receive different event notifications from an OdTfObject class instance.
    
  \remarks
  The default implementations of all methods in this class do nothing but return.

  Corresponding C++ library: Od_Tf

  <group OdTf_Classes>
*/
class ODRX_ABSTRACT TFCORE_EXPORT OdTfObjectReactor : public OdRxObject {
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdTfObjectReactor);
  //DOM-IGNORE-BEGIN
public:
  /** \details
    Notification function called whenever the notifying element has had its clone() member function called.

    \param pElement     [in]  A pointer to the notifying element.
    \param pNewElement  [in]  A pointer to the element resulting from the copy.

    \remarks
    The default implementation of this function does nothing.
    This function can be overridden in custom classes.
  */
  virtual void copied(const OdTfObject* pElement,
                      const OdTfObject* pNewElement) = 0;

  /** \details
    Notification function called whenever an element has been erased or unerased.

    \param pElement [in] A pointer to the element that was erased/unerased.
    \param erasing  [in] An erase flag value. The flag value is equal to true if the element was erased; otherwise it is equal to false.

    \remarks
    The default implementation of this function does nothing.
    This function can be overridden in custom classes.
  */
  virtual void erased(const OdTfObject* pElement, bool erasing = true) = 0;

  /** \details
    Notification function called just before an element is deleted from the memory.

    \param pElement [in] A pointer to the element that is being deleted.

    \remarks
    The default implementation of this function does nothing. 
    This function can be overridden in custom classes.
  */
  virtual void goodbye(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever an element is opened for modifications (OdDg::kForWrite mode).

    \param pElement [in] A pointer to the element that is being opened.

    \remarks
    The default implementation of this function does nothing. 
    This function can be overridden in custom classes.
  */
  virtual void openedForModify(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever an element that was previously opened for modifications (OdDg::kForWrite mode) was successfully modified and now is being closed.

    \param pElement [in]  A pointer to the element that is being closed after being modified.

    \remarks
    The default implementation of this function does nothing. 
    This function can be overridden in custom classes.
  */
  virtual void modified(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever an element derived from the OdDgGraphicsElement class is modified.

    \param pElement [in]  A pointer to the modified element.

    \remarks
    The default implementation of this function does nothing. This function can be overridden in custom classes.
    This function is called whenever one of the following events occurs:
# The calling element is opened for modification (OdDg::kForWrite mode).
# One of the calling element's member functions: 
## Calls its assertWriteEnabled() method with the recordModified parameter that is equal to true.
## Calls its recordGraphicsModified() method passing to it true as a parameter value.
# The calling element is being closed.
    This function is called whenever the element is modified. 
    Therefore calling this function indicates that only the graphics for the element is changed.
  */
  virtual void modifiedGraphics(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever the attribute linkages have been written to the notifying element.

    \param pElement [in] A pointer to the notifying element.

    \remarks
    Notification is made upon the pElement->close() method call.
    The notifying element should be opened for reading (OdDg::kForRead mode).
    The default implementation of this function does nothing.
    This function can be overridden in custom classes.
  */
  virtual void modifiedLinkages(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever any method of a specified sub element calls the assertWriteEnabled() method.

    \param pElement [in] A pointer to the notifying element.
    \param pSubElm  [in] A pointer to the sub element that is owned by the element specified with the pElement parameter.

    \remarks
    Notification is made upon the pElement->close() method call.
  */
  virtual void subElementModified(const OdTfObject* pElement,
                                  const OdTfObject* pSubElm) = 0;

  /** \details
    Notification function called whenever the notifying element is in progress of an Undo operation that is undoing modifications.

    \param pElement [in] A pointer to the notifying element.

    \remarks
    The notifying element should be open for reading (OdDg::kForRead mode).
    The default implementation of this function does nothing. 
    This function can be overridden in custom classes.
  */
  virtual void modifyUndone(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever the Undo process cancels the appending of the notifying element to the database.

    \param pElement [in] A pointer to the notifying element.

    \remarks
    The notifying element is marked as erased. 
    It is not removed from the database and can be unerased, even before it is created.
    The default implementation of this function does nothing. 
    This function can be overridden in custom classes.
  */
  virtual void unappended(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever a Redo process re-appends the notifying element to the database.

    \param pElement [in]  A pointer to the notifying element.

    \remarks
    The default implementation of this function does nothing. 
    This function can be overridden in custom classes.
  */
  virtual void reappended(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called immediately before the element is closed.

    \param elementId [in]  An identifier of the element that is being closed.

    \remarks
    The default implementation of this function does nothing. 
    This function can be overridden in custom classes.
  */
  virtual void closed(OdDbStub* elementId) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdTfObjectReactor object pointers.
  <group OdTf_Types>
*/
typedef OdSmartPtr<OdTfObjectReactor> OdTfObjectReactorPtr;

/** \details
  This template class is a specialization of the OdArray class for OdTfObjectReactor object smart pointers.
  <group OdTf_Types>
*/
typedef OdArray<OdTfObjectReactor*> OdTfObjectReactorArray;

/** \details
  This class is the base class for persistent reactors, which notify top-level objects about modification events.
  To implement your own notification mechanism, re-implement the recvPropagateModify() and xmitPropagateModify() methods of the base class. 
  
  <group OdTf_Classes>
*/
class ODRX_ABSTRACT TFCORE_EXPORT OdTfPersistentReactor : public OdTfObjectReactor {
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdTfPersistentReactor);
  //DOM-IGNORE-END
public:

  /** \details
    Allows a sub-element of a complex element to notify its parent element that it has been changed.

    \param pSubElm [in] A pointer to the modified sub-element.

    \remarks
    The notification algorithm works the following way:
# The call of the method close() of the sub-element object triggers a modification notification handler, which calls the xmitPropagateModify() method.
# The sub-element object calls the recvPropagateModify() method of its parent element and passes to it the pointer to itself.
# The method recvPropagateModify() of the parent element sends a modification notification to the top element of its class hierarchy.

    The default implementation of this function does nothing. 
    This function can be overridden in custom classes.
  */
  virtual void recvPropagateModify(const OdTfObject* pSubElm) = 0;

  /** \details
    Allows a sub-element of a complex element to notify its root element that it has been changed.

    \remarks
    The default implementation of the function informs the database that contains the owner object about modifications 
    and also attaches triggering reactor objects to the database.
    This function can be overridden in custom classes.
  */
  virtual void xmitPropagateModify() const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdTfPersistentReactor object pointers.
  <group OdTf_Types>
*/
typedef OdSmartPtr<OdTfPersistentReactor> OdTfPersistentReactorPtr;

/** \details
  This template class is a specialization of the OdArray class for OdTfPersistentReactor object smart pointers.
  <group OdTf_Types>
*/
typedef OdArray<OdTfPersistentReactor*> OdTfPersistentReactorArray;

#include "TD_PackPop.h"

#endif // __TF_OBJECTREACTOR_H__
