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

#ifndef _DB_DUMPER_H
#define _DB_DUMPER_H

#include "OdaCommon.h"

#include "DbDatabase.h"
#include "DbFiler.h"

#include "DbMaterial.h"
#include "Db2dVertex.h"
#include "Db3dPolylineVertex.h"
#include "DbAbstractViewTableRecord.h"
#include "DbArcAlignedText.h"
#include "DbAttribute.h"
#include "DbBlockReference.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "DbCurve.h"
#include "DbDictionary.h"
#include "DbDimAssoc.h"
#include "DbDimStyleTable.h"
#include "DbDimStyleTableRecord.h"
#include "DbEntity.h"
#include "DbLayerTable.h"
#include "DbLayerTableRecord.h"
#include "DbLayout.h"
#include "DbLeader.h"
#include "DbLinetypeTable.h"
#include "DbLinetypeTableRecord.h"
#include "DbMText.h"
#include "DbMlineStyle.h"
#include "DbOle2Frame.h"
#include "DbPolyline.h"
#include "DbRasterImage.h"
#include "DbRasterImageDef.h"
#include "DbRegAppTable.h"
#include "DbRegAppTableRecord.h"
#include "DbSubentId.h"
#include "DbText.h"
#include "DbTextStyleTable.h"
#include "DbTextStyleTableRecord.h"
#include "DbUCSTable.h"
#include "DbUCSTableRecord.h"
#include "DbViewTable.h"
#include "DbViewTableRecord.h"
#include "DbViewport.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbXrecord.h"
#include "DbMLeaderStyle.h"
//#include "ExHostAppServices.h"
#include "ExProtocolExtension.h"
#include "Ge/Ge.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeScale2d.h"
#include "Ge/GeScale3d.h"
#include "Gi/Gi.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiGeometry.h"
#include "Gi/GiPolyline.h"
#include "Gi/GiTextStyle.h"
#include "Gs/Gs.h"
#include "TextDefs.h"
#include "DbPlaceHolder.h"
#include "Ge/GeEllipArc3d.h"
#include "ExPrintConsole.h"


void dumpAttributeData( int indent, OdDbAttribute* pAttr, int i);

void dumpBlockRefData( OdDbBlockReference* pBlkRef, int indent);

void dumpCurveData( OdDbEntity* pEnt, int indent);

void dumpEntityData( OdDbEntity* pEnt, int indent);

void dumpTextData( OdDbText* pText, int indent);

class DbDumper
{
public:
  DbDumper() {}
  virtual void dumpBlocks                     (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpDimStyles                  (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpEntity                     (OdDbObjectId id,                       int indent = 0);
  virtual void dumpHeader                     (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpLayers                     (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpLinetypes                  (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpRegApps                    (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpSymbolTableRecord          (OdDbSymbolTableRecordPtr pRecord,      int indent = 0);
  virtual void dumpTextStyles                 (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpAbstractViewTableRecord    (OdDbAbstractViewTableRecordPtr pView,  int indent = 0);
  virtual void dumpDimAssoc                   (OdDbObjectPtr pObject,                 int indent = 0);
  virtual void dumpMLineStyles                (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpMLeaderStyles              (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpOsnapPointRef              (OdDbOsnapPointRefPtr pRef, int index,  int indent = 0);
  virtual void dumpUCSTable                   (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpViewports                  (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpViews                      (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpResBuf                     (OdResBuf* resbuf, const OdString& caption, int indent = 0);
  virtual void dumpXdata                      (OdResBuf* xIter,                       int indent = 0);
  virtual void dumpXrefFullSubentPath         (OdDbXrefFullSubentPath& subEntPath,    int indent = 0);
  virtual void dump                           (OdDbDatabase* pDb,                     int indent = 0);
  virtual void dumpProxyObject                (OdDbObjectId id, const OdString& itemName, int indent = 0);
  virtual void dumpObject                     (OdDbObjectId id, OdString itemName,    int indent = 0);
  virtual void dumpSummaryInfo                (OdDbDatabase* pDb,                     int indent = 0);
};

#endif
