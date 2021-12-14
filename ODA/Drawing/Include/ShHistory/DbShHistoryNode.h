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

#ifndef ODDB_DBSHHISTORYNODE_H
#define ODDB_DBSHHISTORYNODE_H

#include "TD_PackPush.h"
#include "DbEvalGraph.h"
#include "TD_3DSolidHistoryDef.h"

/** \details

        <group OdDbSh_Classes> 
*/
class DB3DSOLIDHISTORY_EXPORT OdDbShHistoryNode : public OdDbEvalConnectable 
{
public:
  ODDB_DECLARE_MEMBERS(OdDbShHistoryNode);

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  virtual OdCmColor color() const;
  virtual void setColor(OdCmColor const &);

  virtual OdDbObjectId nodeMaterialId() const;
  virtual void setNodeMaterialId(OdDbObjectId id);

  virtual unsigned int stepId() const;
  virtual void setStepId(unsigned int stepId);

  virtual OdGeMatrix3d transform() const;
  OdResult transformBy(const OdGeMatrix3d& xform);

  OdResult postInDatabase(OdDbObjectId& objId, OdDbDatabase* pDatabase);
  /** \details
  Evaluates the expression represented by this Node object.
  \param evalContext [in]  Pointer to an OdDbEvalContext object to provide custom data.

  Returns true if and only if successful.

  evalContext may be NULL.
  */
  virtual bool evaluate(OdDbEvalContext const* evalContext);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbShHistoryNode object pointers.
*/
typedef OdSmartPtr<OdDbShHistoryNode> OdDbShHistoryNodePtr;

typedef OdArray<OdDbShHistoryNodePtr> OdDbShHistoryNodePtrArray;

#include "TD_PackPop.h"

#endif

