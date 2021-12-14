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

#include "DgDatabaseProperties.h"
#include "DgColorTable.h"
#include "DgTable.h"

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:LevelTableId
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseLevelTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDatabasePtr pObj = OdDgDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDgLevelTablePtr pLevelTable = pObj->getLevelTable();

  OdDgElementId idLevelTable;

  if( !pLevelTable.isNull() )
    idLevelTable = pLevelTable->elementId();

  value = idLevelTable;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:LevelFilterTableId
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseLevelFilterTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDatabasePtr pObj = OdDgDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDgLevelFilterTablePtr pLevelFilterTable = pObj->getLevelFilterTable();

  OdDgElementId idLevelFilterTable;

  if( !pLevelFilterTable.isNull() )
    idLevelFilterTable = pLevelFilterTable->elementId();

  value = idLevelFilterTable;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:FontTableId
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseFontTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDatabasePtr pObj = OdDgDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDgFontTablePtr pTable = pObj->getFontTable();

  OdDgElementId idTable;

  if( !pTable.isNull() )
    idTable = pTable->elementId();

  value = idTable;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:TextStyleTableId
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseTextStyleTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDatabasePtr pObj = OdDgDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDgTextStyleTablePtr pTable = pObj->getTextStyleTable();

  OdDgElementId idTable;

  if( !pTable.isNull() )
    idTable = pTable->elementId();

  value = idTable;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:LineStyleTableId
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseLineStyleTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDatabasePtr pObj = OdDgDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDgLineStyleTablePtr pTable = pObj->getLineStyleTable();

  OdDgElementId idTable;

  if( !pTable.isNull() )
    idTable = pTable->elementId();

  value = idTable;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:Models
//-----------------------------------------------------------------------------------------------------------------------

OdRxValueIteratorPtr OdDgDatabaseModelsProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  OdDgDatabasePtr pDb = OdDgDatabase::cast(pO);

  if( !pDb.isNull() )
  {
    OdDgModelTablePtr pTable = pDb->getModelTable(OdDg::kForRead);

    res = OdRxObjectImpl<OdDgElementIdValueIterator>::createObject();
    ((OdDgElementIdValueIterator*)res.get())->m_pIter = pTable->createIterator();
    ((OdDgElementIdValueIterator*)res.get())->m_bAllowNull = false;
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseModelsProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdDgDatabasePtr pDb = OdDgDatabase::cast(pO);

  if( pDb.isNull() )
    return eNotApplicable;

  OdDgModelTablePtr pTable = pDb->getModelTable(OdDg::kForRead);

  if( pTable.isNull() )
    return eNotApplicable;

  int N = 0;

  for (OdDgElementIteratorPtr it = pTable->createIterator(); !it->done(); it->step())
    ++N;
  count = N;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:MajorVersion
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseMajorVersionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDatabasePtr pObj = OdDgDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->getMajorVersion();
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:MinorVersion
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseMinorVersionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDatabasePtr pObj = OdDgDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->getMinorVersion();
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:FileName
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseFileNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDatabasePtr pObj = OdDgDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->getFilename();
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:ColorPalette
//-----------------------------------------------------------------------------------------------------------------------

ODDG_DEFINE_RX_VALUE_ARRAY_ITERATOR(OdCmEntityColor);

OdRxValueIteratorPtr OdDgDatabaseColorPaletteProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  res = OdRxObjectImpl<OdRxOdCmEntityColorValueIterator>::createObject();

  OdDgDatabasePtr pDb = OdDgDatabase::cast(pO);

  const ODCOLORREF* pPalette = OdDgColorTable::currentPalette(pDb);

  if( pPalette )
  {
    OdArray<OdCmEntityColor> arrPalette;
    arrPalette.resize(256);

    for( OdUInt32 i = 0; i < 256; i++ )
    {
      OdCmEntityColor curColor;
      curColor.setRGB( ODGETRED(pPalette[i]), ODGETGREEN(pPalette[i]), ODGETBLUE(pPalette[i]) );
      arrPalette[i] = curColor;
    }

    ((OdRxOdCmEntityColorValueIterator*)res.get())->init( arrPalette, 0);
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseColorPaletteProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  count = 256;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:MaterialTableId
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseMaterialTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDatabasePtr pObj = OdDgDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDgMaterialTablePtr pMaterialTable = pObj->getMaterialTable();

  OdDgElementId idMatTable;

  if( !pMaterialTable.isNull() )
    idMatTable = pMaterialTable->elementId();

  value = idMatTable;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:DisplayStyleTableId
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseDisplayStyleTableIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDatabasePtr pObj = OdDgDatabase::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDgDisplayStyleTablePtr pDSTable = pObj->getDisplayStyleTable( OdDg::kForRead );

  OdDgElementId idDSTable;

  if( !pDSTable.isNull() )
    idDSTable = pDSTable->elementId();

  value = idDSTable;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property:SharedCellDefinitions
//-----------------------------------------------------------------------------------------------------------------------

OdRxValueIteratorPtr OdDgDatabaseSharedCellDefinitionsProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  OdDgDatabasePtr pDb = OdDgDatabase::cast(pO);

  if( !pDb.isNull() )
  {
    OdDgSharedCellDefinitionTablePtr pTable = pDb->getSharedCellDefinitionTable(OdDg::kForRead);

    res = OdRxObjectImpl<OdDgElementIdValueIterator>::createObject();
    ((OdDgElementIdValueIterator*)res.get())->m_pIter = pTable->createIterator();
    ((OdDgElementIdValueIterator*)res.get())->m_bAllowNull = false;
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDatabaseSharedCellDefinitionsProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdDgDatabasePtr pDb = OdDgDatabase::cast(pO);

  if( pDb.isNull() )
    return eNotApplicable;

  OdDgSharedCellDefinitionTablePtr pTable = pDb->getSharedCellDefinitionTable(OdDg::kForRead);

  if( pTable.isNull() )
    return eNotApplicable;

  int N = 0;

  for (OdDgElementIteratorPtr it = pTable->createIterator(); !it->done(); it->step())
    ++N;
  count = N;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------



