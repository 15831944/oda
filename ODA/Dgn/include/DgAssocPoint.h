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
#ifndef __DG_ASSOCPOINT_H__
#define __DG_ASSOCPOINT_H__

#include "TD_PackPush.h"

//---------------------------------------------------------------------
/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgAssocPointRoot : public OdRxObject
{
public:

  enum OdDgAssocPointRootType
  {
    kUnknownAssociation = 0,
    kLinearAssociation = 1,
    kIntersectAssociation  = 2,
    kArcAssociation = 3,
    kMultilineAssociation = 4,
    kBSplineCurveAssociation = 5,
    kProjectionAssociation = 6,
    kOriginAssociation = 7,
    kIntersect2Association  = 8,
    kMeshVertexAssociation = 10,
    kMeshEdgeAssociation = 11,
    kBSplineSurfaceAssociation = 13
  };

public:

  ODRX_DECLARE_MEMBERS(OdDgAssocPointRoot);

  OdDgAssocPointRoot(){};
  virtual ~OdDgAssocPointRoot(){};

  virtual OdDgAssocPointRootType getType() const = 0;

  virtual OdUInt64 getElementId() const = 0;
  virtual void     setElementId( OdUInt64 uId ) = 0;

  virtual OdUInt64 getRefAttachmentId() const = 0;
  virtual void     setRefAttachmentId( OdUInt64 uId ) = 0;
};

typedef OdSmartPtr<OdDgAssocPointRoot> OdDgAssocPointRootPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgUnknownAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgUnknownAssociation);

  OdDgUnknownAssociation();
  virtual ~OdDgUnknownAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdBinaryData getData() const;
  void setData( OdBinaryData data );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  virtual  OdRxObjectPtr clone() const;

private:

  OdBinaryData m_data;
  OdUInt64     m_elementId;
  OdUInt64     m_refAttachmentId;
};

typedef OdSmartPtr<OdDgUnknownAssociation> OdDgUnknownAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgLinearAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgLinearAssociation);

  OdDgLinearAssociation();
  virtual ~OdDgLinearAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt16 getVertexNumber() const;
  void     setVertexNumber( OdUInt16 uVertex );

  OdUInt16 getNumerator() const;
  void     setNumerator( OdUInt16 uNum );

  OdUInt16 getDivisor() const;
  void     setDivisor( OdUInt16 uDiv );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  OdUInt16 getNVertices() const;
  void     setNVertices( OdUInt16 nVertices );

  virtual  OdRxObjectPtr clone() const;

private:

  OdUInt16 m_uVertex;
  OdUInt16 m_uNumerator;
  OdUInt16 m_uDivisor;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  OdUInt16 m_nVertices;
};

typedef OdSmartPtr<OdDgLinearAssociation> OdDgLinearAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgIntersectAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgIntersectAssociation);

  OdDgIntersectAssociation();
  virtual ~OdDgIntersectAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt8  getIndex() const;
  void     setIndex( OdUInt8 uIndex );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getElement2Id() const;
  void     setElement2Id( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  OdUInt64 getRefAttachment2Id() const;
  void     setRefAttachment2Id( OdUInt64 uId );

  virtual  OdRxObjectPtr clone() const;

private:

  OdUInt8  m_uIndex;
  OdUInt64 m_element1Id;
  OdUInt64 m_uRefAttach1Id;
  OdUInt64 m_element2Id;
  OdUInt64 m_uRefAttach2Id;
};

typedef OdSmartPtr<OdDgIntersectAssociation> OdDgIntersectAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgArcAssociation : public OdDgAssocPointRoot
{
public:

  enum OdDgArcAssociationKeyPoint
  {
    kAnglePoint  = 0,
    kCenterPoint = 1,
    kStartPoint  = 2,
    kEndPoint    = 3
  };

public:

  ODRX_DECLARE_MEMBERS(OdDgArcAssociation);

  OdDgArcAssociation();
  virtual ~OdDgArcAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdDgArcAssociationKeyPoint getKeyPoint() const;
  void     setKeyPoint( OdDgArcAssociationKeyPoint keyPoint );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  double   getAngle() const;
  void     setAngle( double dAngle );

  virtual  OdRxObjectPtr clone() const;

private:

  OdDgArcAssociationKeyPoint m_uKeyPoint;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  double   m_dAngle;
};

typedef OdSmartPtr<OdDgArcAssociation> OdDgArcAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMultilineAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgMultilineAssociation);

  OdDgMultilineAssociation();
  virtual ~OdDgMultilineAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt16 getVertexNumber() const;
  void     setVertexNumber( OdUInt16 uVertex );

  OdUInt8  getLineNumber() const;
  void     setLineNumber( OdUInt8 uLineNum );

  bool     getJointFlag() const;
  void     setJointFlag( bool bSet );

  bool     getProjectFlag() const;
  void     setProjectFlag( bool bSet );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  double   getOffset() const;
  void     setOffset( double dOffset );

  OdUInt16 getNVertices() const;
  void     setNVertices( OdUInt16 nVertices );

  virtual  OdRxObjectPtr clone() const;

private:

  OdUInt16 m_uVertex;
  OdUInt16 m_uFlags;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  double   m_dOffset;
  OdUInt16 m_nVertices;
};

typedef OdSmartPtr<OdDgMultilineAssociation> OdDgMultilineAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgBSplineCurveAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgBSplineCurveAssociation);

  OdDgBSplineCurveAssociation();
  virtual ~OdDgBSplineCurveAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  double   getParam() const;
  void     setParam( double dParam );

  virtual  OdRxObjectPtr clone() const;

private:

  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  double   m_dParam;
};

typedef OdSmartPtr<OdDgBSplineCurveAssociation> OdDgBSplineCurveAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgProjectionAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgProjectionAssociation);

  OdDgProjectionAssociation();
  virtual ~OdDgProjectionAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt16 getVertexNumber() const;
  void     setVertexNumber( OdUInt16 uVertex );

  OdUInt16 getNVertices() const;
  void     setNVertices( OdUInt16 nVertices );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  double   getFraction() const;
  void     setFraction( double dFraction );

  virtual  OdRxObjectPtr clone() const;

private:

  OdUInt16 m_uVertex;
  OdUInt16 m_nVertices;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  double   m_dFraction;
};

typedef OdSmartPtr<OdDgProjectionAssociation> OdDgProjectionAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgOriginAssociation : public OdDgAssocPointRoot
{
public:

  enum OdDgOriginAssocOption
  {
    kInsertionPoint = 0,
    kUpperLeftPoint = 1
  };
public:

  ODRX_DECLARE_MEMBERS(OdDgOriginAssociation);

  OdDgOriginAssociation();
  virtual ~OdDgOriginAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdDgOriginAssocOption getTextOriginOption() const;
  void     setTextOriginOption( OdDgOriginAssocOption uOption );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  virtual  OdRxObjectPtr clone() const;

private:

  OdDgOriginAssocOption m_uOption;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
};

typedef OdSmartPtr<OdDgOriginAssociation> OdDgOriginAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgIntersect2Association : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgIntersect2Association);

  OdDgIntersect2Association();
  virtual ~OdDgIntersect2Association();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt8  getIndex() const;
  void     setIndex( OdUInt8 uIndex );

  OdUInt8  getNSeg1() const;
  void     setNSeg1( OdUInt8 uNSeg );

  OdUInt8  getNSeg2() const;
  void     setNSeg2( OdUInt8 uNSeg );

  OdUInt16 getSegment1() const;
  void     setSegment1( OdUInt8 uSeg );

  OdUInt16 getSegment2() const;
  void     setSegment2( OdUInt8 uSeg );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getElement2Id() const;
  void     setElement2Id( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  OdUInt64 getRefAttachment2Id() const;
  void     setRefAttachment2Id( OdUInt64 uId );

  virtual  OdRxObjectPtr clone() const;

private:

  OdUInt8  m_uIndex;
  OdUInt8  m_uNSeg1;
  OdUInt8  m_uNSeg2;
  OdUInt16 m_uSeg1;
  OdUInt16 m_uSeg2;
  OdUInt64 m_element1Id;
  OdUInt64 m_uRefAttach1Id;
  OdUInt64 m_element2Id;
  OdUInt64 m_uRefAttach2Id;
};

typedef OdSmartPtr<OdDgIntersect2Association> OdDgIntersect2AssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMeshVertexAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgMeshVertexAssociation);

  OdDgMeshVertexAssociation();
  virtual ~OdDgMeshVertexAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt32 getVertexIndex() const;
  void     setVertexIndex( OdUInt32 uVertex );

  OdUInt32 getNVertices() const;
  void     setNVertices( OdUInt32 nVertices );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  virtual  OdRxObjectPtr clone() const;

private:

  OdUInt32 m_uVertex;
  OdUInt32 m_nVertices;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
};

typedef OdSmartPtr<OdDgMeshVertexAssociation> OdDgMeshVertexAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMeshEdgeAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgMeshEdgeAssociation);

  OdDgMeshEdgeAssociation();
  virtual ~OdDgMeshEdgeAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt32 getEdgeIndex() const;
  void     setEdgeIndex( OdUInt32 uEdge );

  OdUInt32 getNEdges() const;
  void     setNEdges( OdUInt32 nEdges );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  double   getUParam() const;
  void     setUParam( double dParam );

  virtual  OdRxObjectPtr clone() const;

private:

  OdUInt32 m_uEdge;
  OdUInt32 m_nEdges;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  double   m_dParam;
};

typedef OdSmartPtr<OdDgMeshEdgeAssociation> OdDgMeshEdgeAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgBSplineSurfaceAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgBSplineSurfaceAssociation);

  OdDgBSplineSurfaceAssociation();
  virtual ~OdDgBSplineSurfaceAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  double   getUParam() const;
  void     setUParam( double dParam );

  double   getVParam() const;
  void     setVParam( double dParam );

  virtual  OdRxObjectPtr clone() const;

private:

  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  double   m_dUParam;
  double   m_dVParam;
};

typedef OdSmartPtr<OdDgBSplineSurfaceAssociation> OdDgBSplineSurfaceAssociationPtr;

//---------------------------------------------------------------------

TG_EXPORT OdArray<OdDgElementPtr> getAssociatedElements(const OdDgAssocPointRootPtr& pDimAssoc, OdDgDatabase* pDb);

#include "TD_PackPop.h"

#endif // __DG_ASSOCPOINT_H__
