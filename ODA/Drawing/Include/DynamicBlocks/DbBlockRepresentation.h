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

#ifndef _DbBlockRepresentation_h_Included_
#define _DbBlockRepresentation_h_Included_

#include "TD_PackPush.h"

#include "RxObjectImpl.h"
#include "DbBlockReference.h"
#include "DbEvalGraph.h"
#define STL_USING_MAP
#include "OdaSTL.h"

class OdDbBlockParameter;
class OdDbDynBlockReference;
class OdDbAttributeDefinition;
class OdDbAttribute;

/** \details

    
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockRepresentationContext : public OdRxObject
{
  OdDbObjectId m_pDynamicBlock;
  OdDbObjectId m_pAnonymousBlock;
  OdDbObjectId m_pBlockData;
  OdDbObjectId m_pRef;
  OdDbObjectId m_pGraph;
  OdDbBlockReference* m_pNDBROBlockRef;
  OdDbEvalGraphPtr m_NDBRO_Graph;
  bool m_bBlockUpdateMode;
  void eraseOldArrayClones();
  void updateAttributeMap();
  bool m_bNDBROAttributes;
  std::map<OdDbAttributeDefinition*, void*> m_pAttributeMap;
  void updateCachedData(OdDbBlockReference* ref);
  void updateBPT();
public:
  OdDbBlockRepresentationContext();
  ~OdDbBlockRepresentationContext();
  void init(const OdDbDynBlockReference* ref, OdDbBlockReference* dbref, OdDbEvalGraph* gr, bool bRequireEvaluate = false, bool loadIndstanceData = true);
  void loadInstanceData(bool bRequireEvaluate = false, bool noHistoryData = true);
  void compactRepresentation();
  bool originalPathToRepresentationPath(const OdDbObjectIdArray&, OdDbEntityPtrArray& res)const;
  bool representationPathToOriginalPath(OdDbObjectIdArray&)const;
  // set RepBTag on representation block
  static void tagBlockRecord(OdDbBlockTableRecord* repBTR, const OdDbHandle& originalBlockHandle);
  // remove dynamic block related xdata && xdictionary entries
  static void clearRepBlock(OdDbBlockTableRecord* repRTR);
  // create representation block for the insert
  void createRepresentation(bool copied = true);
  void addHistoryRecord(const OdString& name, const OdResBuf* value, OdDbEvalNodeId node);
  // 
  OdDbXrecordPtr getHistoryRecord() const;
  // update parameter value from the history record
  bool applyHistory(OdResBufPtr pRb);
  void updateRepresentation();
  OdDbObjectId getRepresentation() const { return m_pAnonymousBlock.isErased() ? OdDbObjectId::kNull : m_pAnonymousBlock; }
  OdDbObjectId getOriginal() const { return m_pDynamicBlock; }
  OdDbEvalExprPtr getRepresentationNode(OdDbEvalNodeId id);
  OdDbBlockReferencePtr getReference();
  OdDbEvalGraphPtr getGraph()
  {
    return m_NDBRO_Graph;
  }
  OdDbBlockTableRecordPtr getBlock();
  void writeUndo(OdDbEvalGraph* gr);
  void readUndo(OdDbObjectId refId, OdDbDwgFiler* pFiler);
  static OdDbBlockRepresentationContext* getRepresentationContext(OdDbBlockReference* br, bool validate = true);
  OdDbAttributePtr getAttribute(OdDbAttributeDefinition*);
  OdGeMatrix3d getRelativeMatrix(const OdGeMatrix3d& m);
  OdGeVector3d getRelativeOffset(const OdGeVector3d& v);
  OdDbObjectId getBlockData() const { return m_pBlockData; }
  void setBlockUpdateMode(bool b) { m_bBlockUpdateMode = b; }
  bool blockUpdateMode() const { return m_bBlockUpdateMode; }
  OdDbObjectId getBlockHData() const;
};

#include "TD_PackPop.h"

#endif // _DbBlockRepresentation_h_Included_
