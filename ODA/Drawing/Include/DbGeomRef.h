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

#ifndef DbGeomRef_INCLUDED_
#define DbGeomRef_INCLUDED_ /*!DOM*/

#include "DbSubentId.h"
#include "DbCompoundObjectId.h"
#include "Ge/GeCurve3d.h"
#include "RxObjectImpl.h"

#include "TD_PackPush.h"

class OdDbEntity;
typedef OdSmartPtr<OdDbEntity> OdDbEntityPtr;


/** \details
  This class represents a reference to a geometry object. It is the base class for references.
*/
class DBENT_EXPORT OdDbGeomRef : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeomRef);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbGeomRef();

  /** \details
    Resets this geometry reference.
  */
  virtual void reset();

  /** \details
    Checks whether this geometry reference is valid.

    \returns true if geometry reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this geometry reference is empty.

    \returns true if geometry reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    Creates an entity using this geometry reference.

    \returns created entity.
  */
  virtual OdDbEntityPtr createEntity() const;

  /** \details
    Evaluates and caches the geometry.

    \returns eOk if geometry is evaluated cached.
  */
  virtual OdResult  evaluateAndCacheGeometry();
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbGeomRef class.

  \sa
  <link smart_pointers, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbGeomRef> OdDbGeomRefPtr;


/** \details
  This class represents a reference to a subent object.
*/
class DBENT_EXPORT OdDbSubentRef : public OdDbGeomRef
{
public:
  ODRX_DECLARE_MEMBERS(OdDbSubentRef);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbSubentRef();

  /** \details
    Creates an instance of this class.

    \param compId [in] input component id
    \param subent [in] input subentity id
  */
  OdDbSubentRef(const OdDbCompoundObjectId& compId, 
                const OdDbSubentId& subent = OdDbSubentId()); 

  /** \details
    Creates an instance of this class.
  
    \param compId [in] input component id
    \param subent [in] input subentity id
  */
  static OdSmartPtr<OdDbSubentRef> createObject( const OdDbCompoundObjectId& compId,
                const OdDbSubentId& subent = OdDbSubentId() );

  /** \details
    Deprecated.
  */
  static OdSmartPtr<OdDbSubentRef> createObject( const OdDbCompoundObjectId* pid,
                const OdDbSubentId& subentid = OdDbSubentId() )
  {
    return createObject( *pid, subentid );
  }

  /** \details
    Creates OdDbSubentRef using other OdDbSubentRef.
  
    \param src [in] subentity reference that will be copied
  */
  OdDbSubentRef& operator =(const OdDbSubentRef& src);

  /** \details
    Creates OdDbSubentRef using other OdDbSubentRef.

    \param src [in] subentity reference that will be copied
  */
  virtual void copyFrom(const OdRxObject* src);

  /** \details
    Resets this subent reference.
  */
  virtual void reset();

  /** \details
    Checks whether this subent reference is valid.

    \returns true if subent reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this subent reference is empty.

    \returns true if subent reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    Returns an object ID of the compounded entity.
  */
  virtual const OdDbCompoundObjectId& entity() const;

  /** \details
    Returns an subent ID.
  */
  virtual OdDbSubentId subentId() const;

  /** \details
    Creates an entity using this subent reference.

    \returns created entity.
  */
  virtual OdDbEntityPtr createEntity() const;

  /** \details
    Evaluates and caches the geometry.

    \returns eOk if geometry is evaluated cached.
  */
  virtual OdResult evaluateAndCacheGeometry(); 

protected:
  /** \details
    Sets the compounded entity using an object ID.

    \param entityId [in] the object ID.
  */
  void setEntity(const OdDbCompoundObjectId& entityId);
 
  /** \details
    Sets an subent ID.
  */
  void setSubent(const OdDbSubentId& subentId);

  /** \details
    Sets an subent ID using the full subent path.

    \param pathSubEnt [in] the full subent path.
  */
  void setFromSubentPath(const OdDbFullSubentPath& pathSubEnt);

private:
  /*!DOM*/
  OdDbCompoundObjectId m_entityId;
  /*!DOM*/
  OdDbSubentId m_SubentId;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbSubentRef class.

  \sa
  <link smart_pointers, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbSubentRef> OdDbSubentRefPtr;


/** \details
  This class represents a reference to a face object.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBENT_EXPORT OdDbFaceRef : public OdRxObjectImpl<OdDbSubentRef>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbFaceRef);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbFaceRef();

  /** \details
    Creates an instance of this class.

    \param compId[in] input component id
    \param subEnt[in] input subentity
    \param pBody[in] input BODY
  */
  OdDbFaceRef(const OdDbCompoundObjectId& compId, 
              const OdDbSubentId& = OdDbSubentId(), 
              class BODY*pBody = NULL); 

  /** \details
    Copies an instance of this class from input faceRef.
  
    \param faceRef[in] face reference
  */
  OdDbFaceRef(const OdDbFaceRef& faceRef);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbFaceRef();

  /** \details
    Copies a given fase reference to this reference.

    \param src[in] face reference
  */
  OdDbFaceRef& operator =(const OdDbFaceRef& src);

  /** \details
    Initializes this reference using a given object.

    \param src[in] face reference
  */
  virtual void copyFrom(const OdRxObject* src);

  /** \details
    Resets this fase reference.
  */
  virtual void reset();

  /** \details
    Checks whether this face reference is valid.

    \returns true if face reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this face reference is empty.

    \returns true if face reference is empty. 
  */
  virtual bool isEmpty() const;

  /** \details
    Returns the ASM BODY object which this face reference may hold.
  */
  class BODY* asmBody() const;

  /** \details
    Creates an entity using this face reference.

    \returns created entity.
  */
  virtual OdDbEntityPtr createEntity() const;

  /** \details
    Evaluates and caches the geometry.

    \returns eOk if geometry chached.
  */
  virtual OdResult evaluateAndCacheGeometry();

private:
  /*!DOM*/
  class BODY* m_pAsmBody;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbFaceRef class.

  \sa
  <link smart_pointers, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbFaceRef> OdDbFaceRefPtr;


/** \details
  This class represents a reference to an endge object.
*/
class DBENT_EXPORT OdDbEdgeRef : public OdRxObjectImpl<OdDbSubentRef>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbEdgeRef);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbEdgeRef();
  
  /** \details
    Creates OdDbEdgeRef from an entity in a block table record and referenced via a path of block inserts.

    \param path [in]  path.
  */
  OdDbEdgeRef(const OdDbFullSubentPath& path);
  
  /** \details
    Creates an instance of this class.

    \param comId [in]  Component id.
    \param edgeSubentId [in] edge subentid.
    \param faceSubentId [in] face subentid.
    \param pCurve [in]  geometry curve.
  */
  OdDbEdgeRef(const OdDbCompoundObjectId& comId,
              const OdDbSubentId& edgeSubentId = OdDbSubentId(),
              const OdDbSubentId& faceSubentId = OdDbSubentId(),
              const OdGeCurve3d* pCurve = NULL);
  OdDbEdgeRef(const OdGeCurve3d* pGeCurve);

  /** \details
    Creates an OdDbEdgeRef based on another OdDbEdgeRef object.

    \param src [in] edge reference that will be copied.
  */
  OdDbEdgeRef(const OdDbEdgeRef& src);

  /** \details
    Deprecated.
  */
  static OdSmartPtr<OdDbEdgeRef> createObject( const OdDbCompoundObjectIdPtr & pid, const OdGeCurve3d* pCurve ) {
    return createObject( *pid, pCurve );
  }

  /** \details
    Creates an instance of this class.
  
    \param compId[in] input component id
    \param pCurve[in] input geometry curve
  */
  static OdSmartPtr<OdDbEdgeRef> createObject( const OdDbCompoundObjectId& compId, const OdGeCurve3d* pCurve );

  /** \details
    Creates an instance of this class.

    \param pEnt[in] entity
  */
  static OdSmartPtr<OdDbEdgeRef> createObject( const OdDbEntity* pEnt );

  /** \details
    Creates an instance of this class.

    \param pCurve[in] geometry curve
  */
  static OdSmartPtr<OdDbEdgeRef> createObject( const OdGeCurve3d* pCurve );

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbEdgeRef();

  /** \details
    Copies a given edge reference to this reference.

    \param src [in] edge reference that will be copied.
  */
  OdDbEdgeRef& operator =(const OdDbEdgeRef& src);

  /** \details
    Initializes this reference using a given object.

    \param src [in] edge reference that will be copied.
  */
  virtual void copyFrom(const OdRxObject* src);

  /** \details
    Resets this edge reference.
  */
  virtual void reset();

  /** \details
    Checks whether this edge reference is valid.

    \returns true if edge reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this edge reference is empty.

    \returns true if edge reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    The edge reference can use the subent ID of a face subentity. The face entity is used to restrict
    the referenced edge subentity.

    \param src [in]  Face subentityid.
  */
  void setFaceSubentity(const OdDbSubentId& faceSubentId);

  /** \details
    Returns the face subent ID of one of the faces adjacent to the referenced edge.
  */
  OdDbSubentId faceSubentId() const;

  /** \details
    Returns the constant three-dimensional curve which the edge reference can hold.
  */
  const OdGeCurve3d* curve() const;

  /** \details
    Evaluates the curve.

    \param pCurve [out] evaluated curve.

    \returns eOk if curve is evaluated.
  */
  virtual OdResult evaluateCurve(OdGeCurve3d*& src) const;

  /** \details
    Creates an entity using this edge reference.

    \returns created entity.
  */
  virtual OdDbEntityPtr createEntity() const;

  /** \details
    Evaluates and caches the geometry.

    \returns eOk if geometry is cached.
  */
  virtual OdResult evaluateAndCacheGeometry(); 

  
  /** \details
    Sets the constant three-dimensional curve to be hold by this edge reference.

    \param pCurve [in] input curve.
  */
  void setCurve(const OdGeCurve3d* pCurve);

private:
  ///** \details
  //  Sets the constant three-dimensional curve to be hold by this edge reference.
  //*/
  //void setCurve(const OdGeCurve3d* pCurve);
  /*!DOM*/
  OdDbSubentId m_FaceSubentId;
  /*!DOM*/
  OdGeCurve3dPtr m_pCurve;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbEdgeRef class.

  \sa
  <link smart_pointers, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbEdgeRef> OdDbEdgeRefPtr;


/** \details
  This class represents a reference to a vertex object.
*/
class DBENT_EXPORT OdDbVertexRef : public OdRxObjectImpl<OdDbSubentRef>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbVertexRef);

  /** \details
    Declares the implied types.
  */
  enum ImpliedType
  {
    /** Directly specified vertex.*/
    kExplicitVertex,
    /** Unknown type vertex.*/
    kUnknownType,
    /** Edge start vertex*/
    kEdgeStart,
    /** Edge end vertex*/
    kEdgeEnd,
    /** Edge mid vertex*/
    kEdgeMid,
    /** Edge center vertex*/
    kEdgeCenter,
    /** Edge spline control point*/
    kEdgeSplineControlPoint,
    /** Edge spline fit point*/
    kEdgeSplineFitPoint,
  };

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbVertexRef();
  
  /** \details
    Sets the data members of the OdDbVertexRef.

    \param comId [in]  Component id.
    \param subentId [in] subentid.
    \param point3d [in] face subentid.
  */
  OdDbVertexRef(const OdDbCompoundObjectId& comId,
                const OdDbSubentId& subentId = OdDbSubentId(),
                const OdGePoint3d& point3d = OdGePoint3d::kOrigin);

  /** \details
    Creates the OdDbVertexRef from an entity in a block table recordand referenced via a path of block inserts.

    \param fullPath [in]  Component id.
  */
  OdDbVertexRef(const OdDbFullSubentPath& fullPath);

  /** \details
    Causes the OdDbVertexRef to maintain the OdDbObjectId of the given OdDbEntity.
    Moreover, if the entity is derived from OdDbPoint, it maintains its OdGePoint3d as OdGePoint3d.

    \param pEnt [in] given entity.
  */
  OdDbVertexRef(const OdDbEntity* pEnt);
  
  /** \details
    The constructor causes the OdDbVertexRef to maintain the OdGePoint3d.
  
    \param point3d [in] given point.
  */
  OdDbVertexRef(const OdGePoint3d& point3d);

  /** \details
    Creates the OdDbVertexRef from set of parameters.

    \param implType [in] implied type.
    \param controlOrFitPointIndex [in] point index.
    \param point3d [in] point.
  */
  OdDbVertexRef(ImpliedType implType, 
                const OdDbEdgeRef& edgeRef,
                int controlOrFitPointIndex = 0,
                const OdGePoint3d& point3d = OdGePoint3d::kOrigin);

  /** \details
    Creates the OdDbVertexRef from other OdDbVertexRef.
  
    \param src [in] other OdDbVertexRef.
  */
  OdDbVertexRef(const OdDbVertexRef& src);

  /** \details
    Causes the OdDbVertexRef to maintain the OdGePoint3d.

    \param point3d [in] given point.
  */
  static OdSmartPtr<OdDbVertexRef> createObject( const OdGePoint3d& );

  /** \details
    The method sets the data members of the OdDbVertexRef.

    \param entId [in]  object id.
    \param subId [in] subentid.
    \param point [in] point.
  */
  static OdSmartPtr<OdDbVertexRef> createObject( const OdDbObjectId entId, const OdDbSubentId subId, const OdGePoint3d& point );

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbVertexRef();

  /** \details
    Copies a given vertex reference to this reference.

    \param other [in] other vertex.
  */
  OdDbVertexRef& operator =(const OdDbVertexRef& other);

  /** \details
    Initializes this reference using a given object.

    \param other [in] other vertex.
  */
  virtual void copyFrom(const OdRxObject* src);

  /** \details
    Resets this vertex reference.
  */
  virtual void reset();

  /** \details
    Checks whether this vertex reference is valid.

    \returns true if veretex reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this vertex reference is empty.

    \If referencedRef() is not NULL, returns its OdDbCompoundObjectId, otherwise returns OdDbCompoundObjectId from the base-class OdDbSubentRef.
  */
  virtual const OdDbCompoundObjectId& entity() const;

  /** \details
    Returns the subent ID.
  */
  virtual OdDbSubentId subentId() const;

  /** \details
    Returns the implied type.
  */
  ImpliedType impliedType() const;

  /** \details
    Returns the geometry reference referenced by this vertex reference.
  */
  const OdDbGeomRef* referencedRef() const;

  /** \details
    Returns the vertex index.
  */
  int index() const;

  /** \details
    Returns the three-dimensional point directly held by this vertex reference.
  */
  OdGePoint3d point() const;

  /** \details
    Evaluates the point from the referenced geometry. If no geometry, returns the cached point coordinates.

    \param point [out] evaluated point.

    \returns eOk if point is evaluated.
  */
  virtual OdResult evaluatePoint(OdGePoint3d& point) const;

  /** \details
    Creates an entity using this vertex reference.
    
    \returns created entity.
  */
  OdDbEntityPtr createEntity()const;

  /** \details
    Evaluates and caches the geometry.

    \returns eOk if geometry is cached.
  */
  virtual OdResult evaluateAndCacheGeometry(); 

private:
  /** \details
    Sets the geometry reference to be hold by this vertex reference.
  */
  void setReferencedRef(OdDbGeomRefPtr pReferencedRef);

  /*!DOM*/
  ImpliedType m_ImpliedType;
  /*!DOM*/
  OdDbGeomRefPtr m_pReferencedRef;
  /*!DOM*/
  int m_Index;
  /*!DOM*/
  OdGePoint3d m_Point; 
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbVertexRef class.

  \sa
  <link smart_pointers, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbVertexRef> OdDbVertexRefPtr;


/** \details
  This class represents a reference to a path object.
*/
class DBENT_EXPORT OdDbPathRef : public OdRxObjectImpl<OdDbGeomRef>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbPathRef);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbPathRef();
  /** \details
    Creates an OdDbPathRef from a sequence of OdDbEdgeRefs.

    \param edges [in] sequence of OdDbEdgeRefs.
  */
  OdDbPathRef(const OdArray<OdDbEdgeRef>& edges);

  /** \details
    Creates for an OdDbPathRef that is a sequence of connected edges.

    \param edgeSubentPathArr [in] Subent paths of the edges that make up the path.
    \param faceSubentPathArr [in] Optional subent paths of the faces for each edge. The number of elements in this array must be 0 or be the same as the number of elements in edgeSubentPathArr.
  */
  OdDbPathRef(const OdDbFullSubentPathArray& edgeSubentPathArr,
              const OdDbFullSubentPathArray& faceSubentPathArr);

  /** \details
    Creates an OdDbPathRef based on another OdDbPathRef object.

    \param src [in] Input a reference to the object to copy.
  */
  OdDbPathRef(const OdDbPathRef& src);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbPathRef();

  /** \details
    Copies a given path reference to this reference.
  */
  OdDbPathRef& operator =(const OdDbPathRef&);

  /** \details
    Initializes this reference using a given object.

    \param src [in] Input a reference to the object to copy.
  */
  virtual void copyFrom(const OdRxObject* src);

  /** \details
    Resets this path reference.
  */
  virtual void reset();

  /** \details
    Checks whether this path reference is valid.

    \returns true if path reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this path reference is empty.

    \returns true if path reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    Sets the path reference from a sequence of edge references.

    \param edges [in] sequence of edge references.
  */
  void setEdgeRefs(const OdArray<OdDbEdgeRef>& edges);

  /** \details
    Returns an array of entities created from the edge references in the path. 
    If the second argument is True, it tries to concatenate them.

    \param entities [out] array of OdDbEntities created from the individual OdDbEdgeRefs in the path. 
    \param concatenate [out] If concatenate is true, it tries to concatenate them.

    \returns eOk if array of OdDbEntities created
  */
  OdResult getEntityArray(OdArray<OdDbEntity*>& entities, bool concatenate) const;

  /** \details
    Returns the array of edge references stored in the path reference.
  */
  const OdArray<OdDbEdgeRef>& edgeRefs() const;

  /** \details
    Checks whether the geometry of the path is equal to the geometry of a given curve.

    \param pOther [in] input other curve which will be compared.

    \returns true if this path geometry is equal with given curve
  */
  bool isEqualTo(const OdGeCurve3d* pOther);

  /** \details
    Checks whether all elements of this path reference are the references to the entities.

    \returns true if this path is referenced to the entity
  */
  bool isReferencePath();

  /** \details
    Creates an entity using this path reference.

    \returns created entity
  */
  virtual OdDbEntityPtr createEntity() const;

  /** \details
    Evaluates and caches the geometry.

    \returns eOk if geometry is evaluated and cached
  */
  virtual OdResult evaluateAndCacheGeometry();

private:
  /*!DOM*/
  OdArray<OdDbEdgeRef> m_EdgeRefs;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbPathRef class.

  \sa
  <link smart_pointers, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbPathRef> OdDbPathRefPtr;


/** \details
  This class represents a reference to a vector object.
*/
class DBENT_EXPORT OdDbVectorRef : public OdRxObjectImpl<OdDbGeomRef>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbVectorRef);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbVectorRef();

  /** \details
    Creates an instance of this class.

    \param vector [in] vector.
  */
  OdDbVectorRef(const OdGeVector3d& vector);

  /** \details
    Creates an instance of this class.
  
    \param src [in] vector reference.
  */
  OdDbVectorRef(const OdDbVectorRef& src);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbVectorRef();

  /** \details
    Copies a given vector reference to this reference.

    \param src [in] vector reference.
  */
  OdDbVectorRef& operator =(const OdDbVectorRef& src);

  /** \details
    Initializes this reference using a given object.

    \param src [in] vector reference.
  */
  virtual void copyFrom(const OdRxObject* src);

  /** \details
    Resets this vector reference.
  */
  virtual void reset();

  /** \details
    Checks whether this vector reference is valid.

    \returns true if vector reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this vector reference is empty.

    \returns true if vector reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    Returns a three-dimensional vector stored by this vector reference.
  */
  const OdGeVector3d vector() const;

  /** \details
    Sets a three-dimensional vector for this vector reference.

    \param vector [in] vector reference.
  */
  void set(const OdGeVector3d& vector);

  /** \details
    Creates an entity using this vector reference.

    \returns created entity.
  */
  virtual OdDbEntityPtr createEntity() const;

  /** \details
    Evaluates and caches the geometry.

    \returns eOk.
  */
  virtual OdResult evaluateAndCacheGeometry();

private:
  /*!DOM*/
  OdGeVector3d m_Vector;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbVectorRef class.

  \sa
  <link smart_pointers, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbVectorRef> OdDbVectorRefPtr;


/** \details
  This class represents a reference to a coordinate system.
*/
class DBENT_EXPORT OdDbCoordSystemRef : public OdRxObjectImpl<OdDbGeomRef>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbCoordSystemRef);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbCoordSystemRef();

  /** \details
    Creates an instance of this class.

    \param matrix [in] input matrix.
  */
  OdDbCoordSystemRef(const OdGeMatrix3d& matrix);
  
  /** \details
    Creates an instance of this class.
  
    \param compId [in] input component id.
    \param matrix [in] input matrix.
  */
  OdDbCoordSystemRef(const OdDbCompoundObjectId& compId, const OdGeMatrix3d& matrix);

  /** \details
    Creates an instance of this class.

    \param aSubentRef [in] chain of subentities refs.
  */
  OdDbCoordSystemRef(const OdArray<OdDbSubentRef*>& aSubentRef);

  /** \details
    Creates an instance of this class.

    \param src [in] OdDbCoordSystemRef reference.
  */
  OdDbCoordSystemRef(const OdDbCoordSystemRef& src);

  /** \details
    Destroys an instance of this class.
  */
  virtual ~OdDbCoordSystemRef();

  /** \details
    Copies a given coordinate system reference to this reference.

    \param src [in] OdDbCoordSystemRef reference.
  */
  OdDbCoordSystemRef& operator = (const OdDbCoordSystemRef&);

  /** \details
    Initializes this reference using a given object.

    \param src [in] OdDbCoordSystemRef reference.
  */
  virtual void copyFrom(const OdRxObject*);

  /** \details
    Resets this coordinate system reference.
  */
  virtual void reset();

  /** \details
    Checks whether this coordinate system reference is valid.

    \returns true if coordinate system reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this coordinate system reference is empty.

    \returns true if coordinate system reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    Returns the transformation matrix of the coordinate system held by this reference.
  */
  const OdGeMatrix3d& coordSystem() const;

  /** \details
    Returns an entity ID held by this reference.
  */
  const OdDbCompoundObjectId& entityId() const;

  /** \details
    Returns an array of subent references held by this reference.
  */
  const OdArray<OdDbSubentRef*>& subentRefs() const;

  /** \details
    Sets this reference using a transformation matrix.

    \param matrix [in] transformation matrix.
  */
  void set(const OdGeMatrix3d& matrix);
  
  /** \details
    Sets this reference using an entity ID.

    \param entId [in] an entity ID.
  */
  void set(const OdDbCompoundObjectId& entId);
  
  /** \details
    Sets this reference using an array of subent references held.

    \param subentReferences [in] an array of subent references.
  */
  void set(const OdArray<OdDbSubentRef*>& subentReferences);

  /** \details
    Creates an entity using this coordinate system reference.

    \returns created entity
  */
  virtual OdDbEntityPtr createEntity() const;

  /** \details
    Evaluates and caches the geometry.

    \return eOk if geoemtry is evaluated and cached
  */
  virtual OdResult evaluateAndCacheGeometry();

private:
  /*!DOM*/
  OdGeMatrix3d m_CoordSystem;
  /*!DOM*/
  OdDbCompoundObjectId m_entityId;
  /*!DOM*/
  OdArray<OdDbSubentRef*> m_SubentRefs;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbCoordSystemRef class.

  \sa
  <link smart_pointers, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbCoordSystemRef> OdDbCoordSystemRefPtr;

#include "TD_PackPop.h"

#endif // DbGeomRef_INCLUDED_
