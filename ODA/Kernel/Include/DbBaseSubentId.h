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




#ifndef ODDB_DBBASESUBENT_H
#define ODDB_DBBASESUBENT_H /*!DOM*/

#include "TD_PackPush.h"

#include "DbRootExport.h"
#include "DbStubPtrArray.h"

namespace OdDb
{
  /** \details
    This enumeration specifies subentity types for objects of the OdDbSubentId
    class.
  */
  enum SubentType
  {
    kNullSubentType = 0, //Null
    kFaceSubentType,     //Face
    kEdgeSubentType,     //Edge
    kVertexSubentType,   //Vertex
    kMlineSubentCache,   //Multiline
    kClassSubentType     //Other
  };
}

/** \details
    This class implements Subentity Id objects for OdDbEntity objects in an OdDbDatabase instance.  

    \remarks
    A given OdDbEntity object may consist of any number of graphical subentities.
    
    Subentity type must be one of the following:

    <table>
    Name                        Value
    OdDb::kNullSubentType       0
    OdDb::kFaceSubentType       1
    OdDb::kEdgeSubentType       2
    OdDb::kVertexSubentType     3   
    OdDb::kMlineSubentCache     4  
    </table>

    Each SubentityId object is created from a Subentity type and an index.
    
    Corresponding C++ library: TD_DbRoot

    <group OdDbRoot_Classes>
*/
class DBROOT_EXPORT OdDbSubentId
{
public:

  /** \details
    Default constructor for objects of the OdDbSubentId class. Constructs a
    subentity of the kNullSubentType type and index equal to 0.
  */
  OdDbSubentId()
    : m_Type(OdDb::kNullSubentType)
    , m_Index(0) 
  {}

  /** \details
    Constructor for objects of the OdDbSubentId class.
    
    \param type [in] Subentity type.
    \param index [in] Subentity index.
  */
  OdDbSubentId(OdDb::SubentType type, OdGsMarker index)
    : m_Type(type)
    , m_Index(index) 
  {}

  /** \details
    Comparison operator. This operator compares this object with another object
    of the OdDbSubentId type.

    \param subentId [in] Object to be compared with this object.

    \returns True if the objects are equal to each other; false otherwise.
  */
  bool operator ==(const OdDbSubentId& subentId) const
  {
    return ((m_Index == subentId.m_Index) && (m_Type == subentId.m_Type));
  }
      
  /** \details
    Comparison operator "not equal". This operator compares this object with
    another object of the OdDbSubentId type.

    \param subentId [in] Object to be compared with this object.

    \returns True if the objects are not equal to each other; false otherwise.
  */
  bool operator !=(const OdDbSubentId& subentId) const
  {
    return ((m_Index != subentId.m_Index) || (m_Type != subentId.m_Type));
  }
  
  /** \details
    Returns the type of this SubentId object.

    \returns Type of this SubentId object.

    \remarks
    The return value can be one of the following:

    <table>
    Name                        Value
    OdDb::kNullSubentType       0
    OdDb::kFaceSubentType       1
    OdDb::kEdgeSubentType       2
    OdDb::kVertexSubentType     3   
    OdDb::kMlineSubentCache     4  
    </table>
  */
  OdDb::SubentType type () const 
  { 
    return m_Type; 
  }
  
  /** \details
    Sets the type of this SubentId object.

    \param type [in]  Subentity type.

    \remarks
    Subentity type must be one of the following:

    <table>
    Name                        Value
    OdDb::kNullSubentType       0
    OdDb::kFaceSubentType       1
    OdDb::kEdgeSubentType       2
    OdDb::kVertexSubentType     3   
    OdDb::kMlineSubentCache     4  
    </table>
  */
  void setType(OdDb::SubentType type) 
  { 
    m_Type = type; 
  }

  /** \details
    Returns the index of this SubentId object.

    \returns Index of this SubentId object.
  */
  OdGsMarker index() const 
  { 
    return m_Index; 
  }

  /** \details
    Sets the index of this SubentId object.

    \param index [in]  Index.
  */
  void setIndex(OdGsMarker index) 
  { 
    m_Index = index; 
  }

private:
  OdDb::SubentType m_Type;
  OdGsMarker       m_Index;
};

/** \details
    This base class uniquely defines subentities within in a database instance.

    \remarks
    Each OdDbBaseFullSubentPath object consists of a OdDbSubentId object and
    an ordered array of Object IDs. The SubentId consists the index and subentType
    of the object. The array of Object IDs defines the path to the 
    subentity from the outermost entity to the entity containing the subentity.
    
    Corresponding C++ library: TD_DbRoot

    <group OdDbRoot_Classes>
*/
class DBROOT_EXPORT OdDbBaseFullSubentPath
{
public:

  /** \details
    Default constructor for objects of the OdDbBaseFullSubentPath class.
  */
  OdDbBaseFullSubentPath() 
  {}

  /** \details
    Constructor for objects of the OdDbBaseFullSubentPath class. Constructs a
    subentity based on type and index.

    \param type  [in] Subentity type.
    \param index [in] Index.
  */
  OdDbBaseFullSubentPath(OdDb::SubentType type, OdGsMarker index)
    : m_SubentId(type, index) 
  {}

  /** \details
    Constructor for objects of the OdDbBaseFullSubentPath class.

    \param entId [in] Object ID.
    \param subId [in] Subentity ID.
  */
  OdDbBaseFullSubentPath(OdDbStub* entId, OdDbSubentId subId)
    : m_SubentId(subId) 
  {
    m_ObjectIds.clear();
    m_ObjectIds.append(entId) ;
  }

  /** \details
    Constructor for objects of the OdDbBaseFullSubentPath class. Constructs a
    subentity based on type and index.

    \param entId [in] Object ID.
    \param type  [in] Subentity type.
    \param index [in] Index.
  */
  OdDbBaseFullSubentPath(OdDbStub* entId, OdDb::SubentType type, OdGsMarker index)
    : m_SubentId(type, index)
  {
    m_ObjectIds.clear();
    m_ObjectIds.append(entId) ;
  }

  /** \details
    Constructor for objects of the OdDbBaseFullSubentPath class.

    \param objectIds [in] Array of object IDs.
    \param subId [in] Subentity ID.
  */
  OdDbBaseFullSubentPath(OdDbStubPtrArray objectIds, OdDbSubentId subId)
    : m_ObjectIds(objectIds) 
    , m_SubentId(subId) 
  {
  }

  /** \details
    Returns the array of object IDs that define the path to the subentity.

    \param objectIdsArg [out] Array of object IDs.
  */
  void objectIds(OdDbStubPtrArray& objectIdsArg) const
  {
    objectIdsArg = m_ObjectIds;
  }

  /** \details
    This is the assignment operator for objects of the OdDbBaseFullSubentPath
    class. Assigns the value of the second operand to the first one.

    \param fullSubentPath [in]  Object to be assigned.

    \returns
    A reference to this OdDbBaseFullSubentPath object.
  */
  OdDbBaseFullSubentPath& operator =(const OdDbBaseFullSubentPath& fullSubentPath)
  {
    m_ObjectIds = fullSubentPath.objectIds();
    m_SubentId  = fullSubentPath.subentId();
    return *this;
  }

  /** \details
    Comparison operator. This operator compares this object with another object
    of the OdDbBaseFullSubentPath type.

    \param fullSubentPath [in] Object to be compared with this object.

    \returns True if the objects are equal to each other; false otherwise.
  */
  bool operator==(const OdDbBaseFullSubentPath& fullSubentPath) const
  {
    return m_ObjectIds == fullSubentPath.objectIds() && m_SubentId == fullSubentPath.subentId();
  }

  /** \details
    Returns a reference to the embedded OdDbStubPtrArray object in this BaseFullSubentPath object.
    
    \returns Reference to the OdDbStubPtrArray object.
  */
  const OdDbStubPtrArray& objectIds() const
  {
    return m_ObjectIds;
  }
  
  /** \details
    Returns a reference to the embedded OdDbStubPtrArray object in this BaseFullSubentPath object.
    
    \returns Reference to the OdDbStubPtrArray object.
  */
  OdDbStubPtrArray& objectIds()
  {
    return m_ObjectIds;
  }
  
  /** \details
    Returns a copy of the embedded OdDbSubentId object in this BaseFullSubentPath object.

    \returns Copy of the embedded OdDbSubentId object.
  */
  const OdDbSubentId subentId() const
  {
    return m_SubentId;
  }

  /** \details
    Returns a reference to the embedded OdDbSubentId object in this BaseFullSubentPath object.

    \returns Reference to the OdDbSubentId object.
  */
  OdDbSubentId& subentId()
  {
    return m_SubentId;
  }
  
protected:              
  OdDbStubPtrArray  m_ObjectIds;
  OdDbSubentId      m_SubentId;
};

/** \details
    This template class is a specialization of the OdArray class for OdDbBaseFullSubentPath object pointers.
*/
typedef OdArray<OdDbBaseFullSubentPath> OdDbBaseFullSubentPathArray;

/** \details
  Obtais a pointer to the OdDbBaseDatabase object based on the universal object
  ID.

  \param id [in] Universal object ID.

  \returns Pointer to the OdDbBaseDatabase object.

  \sa
  <link tkernel_common_object_id.html, Common Object ID for Different ODA Databases>
*/
DBROOT_EXPORT OdRxObject* baseDatabaseBy(OdDbStub* id);

/** \details
  This interface class is required to convert the OdRxObject* model to the
  OdRxObject* of the OdIfcFile inside the baseDatabaseBy implementation.

  \sa
  <link tkernel_common_object_id.html, Common Object ID for Different ODA Databases>
  
  <group OdDbRoot_Classes>
*/
class OdBaseDatabaseByResolver : public OdRxObject
{
public:
  
  /** \details
    Converts the OdRxObject* model of the database to the OdRxObject* of the
    OdIfcFile.

    \param pRxDb [in] OdRxObject* model of the database.

    \returns OdRxObject* of the OdIfcFile.

    \sa
    <link tkernel_common_object_id.html, Common Object ID for Different ODA Databases>
  */
  virtual OdRxObject* resolve(OdRxObject* pRxDb) = 0;
};

/** \details
  Adds an OdBaseDatabaseByResolver object.

  \param pResolver [in] Pointer to the OdBaseDatabaseByResolver object to add.

  \sa
  <link tkernel_common_object_id.html, Common Object ID for Different ODA Databases>
*/
DBROOT_EXPORT void addBaseDatabaseByResolver(OdBaseDatabaseByResolver* pResolver);

/** \details
  Removes an OdBaseDatabaseByResolver object.

  \param pResolver [in] Pointer to the OdBaseDatabaseByResolver object to remove.

  \sa
  <link tkernel_common_object_id.html, Common Object ID for Different ODA Databases>
*/
DBROOT_EXPORT void removeBaseDatabaseByResolver(OdBaseDatabaseByResolver* pResolver);
#include "TD_PackPop.h"

#endif
