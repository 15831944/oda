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

// ExCustEntity.h: interface for the ExCustEntity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(EXFMBODY_INCLUDED)
#define EXFMBODY_INCLUDED

#include "ExFMBodyExport.h"
#include "DbEntity.h"
#include "Modeler/FMMdlBody.h"
#include "Db3dSolid.h"

class ExFmBody;

typedef OdSmartPtr<ExFmBody> ExFmBodyPtr;

class EXFMBODYEXPORT ExFmBody : public OdDbEntity
{
  static const OdInt16 lastKnownVersion;

protected:
  OdArray<FacetModeler::Body> m_aBody;

public:
  static const int tagSection;
  static const int tagTransparant;

  // Macro to declare
  ODDB_DECLARE_MEMBERS(ExFmBody);

  ExFmBody();
  virtual ~ExFmBody();

  static OdInt16 getVersion();

  void setBody(const FacetModeler::Body& body);
  void setBodies(const OdArray<FacetModeler::Body>& bodies);
  void getBodies(OdArray<FacetModeler::Body>& bodies) const;
  unsigned int nBodies() const;
  void copyBodiesFrom(const ExFmBody& body);
  void convertBodiesFrom(const OdArray<OdDb3dSolidPtr>& aSolid, double surfaceTol);

  OdResult readBody(OdStreamBuf* pStreamBuff);
  OdResult readBody(const OdString& fName);
  void setColor(OdUInt32 color);
  ExFmBodyPtr boolOper(const ExFmBody& body, FacetModeler::BooleanOperation operation) const;
  ExFmBodyPtr slice(const OdGePlane& cutter, FacetModeler::TagType tag = 0) const;
  void enableShadedRendering(double angleTol);
  void simplification(double param);
  int numFaces() const;

  // Methods to override
  OdResult dwgInFields(OdDbDwgFiler* pFiler);
  void dwgOutFields(OdDbDwgFiler* pFiler) const;
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;
  bool subWorldDraw(OdGiWorldDraw * pWd) const;
  OdResult subTransformBy(const OdGeMatrix3d& xform);
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
};

#endif
