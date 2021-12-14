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

#include "DgTableProperties.h"
#include "DgDatabase.h"
#include "DgLevelTableRecord.h"
#include "DgMaterialTableRecord.h"
#include "DgLineStyleTableRecord.h"
#include "DgDisplayStyle.h"
#include "DgReferenceAttach.h"

//-----------------------------------------------------------------------------------------------------------------------
//                                          element: OdDgLevelTable
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Items
//-----------------------------------------------------------------------------------------------------------------------

OdRxValueIteratorPtr OdDgLevelTableItemsProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  const OdDgLevelTablePtr pTable = OdDgLevelTable::cast(pO);

  if( !pTable.isNull() )
  {
    res = OdRxObjectImpl<OdDgElementIdValueIterator>::createObject();
    ((OdDgElementIdValueIterator*)res.get())->m_pIter = pTable->createIterator();
    ((OdDgElementIdValueIterator*)res.get())->m_bAllowNull = false;
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLevelTableItemsProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdRxValueIteratorPtr res;
  const OdDgLevelTablePtr pTable = OdDgLevelTable::cast(pO);

  if( pTable.isNull() )
    return eNotApplicable;

  int N = 0;

  for (OdDgElementIteratorPtr it = pTable->createIterator(); !it->done(); it->step())
    ++N;
  count = N;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          element: OdDgLineStyleTable
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Items
//-----------------------------------------------------------------------------------------------------------------------

struct OdDgLineStyleTableValueIterator : OdRxValueIterator
{
	OdDgElementId					 m_idTable;
	OdDgElementIteratorPtr m_pIter;
	OdUInt32							 m_uIndex;
  bool                   m_bShowByLevel;
  bool                   m_bShowByCell;

	virtual bool done()
	{
		if ( (m_uIndex > 7) && m_pIter.isNull() )
			return true;
		return ((m_uIndex > 7) && m_pIter->done()) ? true : false;
	}
	virtual bool next()
	{
		if( done() )
			return false;

    if( !m_bShowByLevel )
    {
      m_bShowByLevel = true;
      return true;
    }

    if( !m_bShowByCell )
    {
      m_bShowByCell = true;
      return true;
    }

		if( m_uIndex > 7)
		{
			if (!m_pIter.isNull())
				m_pIter->step();
			else
				return false;
		}

		m_uIndex++;
		return true;
	}
	virtual OdRxValue current() const
	{
		OdRxValue retVal;

    if( !m_bShowByLevel )
    {
      if( !m_idTable.isNull() )
      {
        OdDgLineStyleTablePtr pTable = m_idTable.openObject(OdDg::kForRead);
        retVal = pTable->getAt(OdDg::kLineStyleByLevel);
      }
    }
    else if( !m_bShowByCell )
    {
      if( !m_idTable.isNull() )
      {
        OdDgLineStyleTablePtr pTable = m_idTable.openObject(OdDg::kForRead);
        retVal = pTable->getAt(OdDg::kLineStyleByCell);
      }
    }
		else if( m_uIndex < 8 )
		{
			if (!m_idTable.isNull())
			{
				OdDgLineStyleTablePtr pTable = m_idTable.openObject(OdDg::kForRead);
				retVal = pTable->getAt(m_uIndex);
			}
		}
		else
			retVal = m_pIter->item();

		return retVal;
	}
};

//----------------------------------------------------------------------------------------------------------------------

OdRxValueIteratorPtr OdDgLineStyleTableItemsProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  const OdDgLineStyleTablePtr pTable = OdDgLineStyleTable::cast(pO);

  if( !pTable.isNull() )
  {
    res = OdRxObjectImpl<OdDgLineStyleTableValueIterator>::createObject();
		((OdDgLineStyleTableValueIterator*)res.get())->m_idTable = pTable->elementId();
    ((OdDgLineStyleTableValueIterator*)res.get())->m_pIter   = pTable->createIterator();
		((OdDgLineStyleTableValueIterator*)res.get())->m_uIndex  = 0;
    ((OdDgLineStyleTableValueIterator*)res.get())->m_bShowByLevel = false;
    ((OdDgLineStyleTableValueIterator*)res.get())->m_bShowByCell  = false;
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLineStyleTableItemsProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdRxValueIteratorPtr res;
  const OdDgLineStyleTablePtr pTable = OdDgLineStyleTable::cast(pO);

  if( pTable.isNull() )
    return eNotApplicable;

  int N = 9;

  for (OdDgElementIteratorPtr it = pTable->createIterator(); !it->done(); it->step())
    ++N;
  count = N;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          element: OdDgMaterialTable
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Items
//-----------------------------------------------------------------------------------------------------------------------

OdRxValueIteratorPtr OdDgMaterialTableItemsProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  const OdDgMaterialTablePtr pTable = OdDgMaterialTable::cast(pO);

  if( !pTable.isNull() )
  {
    res = OdRxObjectImpl<OdDgElementIdValueIterator>::createObject();
    ((OdDgElementIdValueIterator*)res.get())->m_pIter = pTable->createIterator();
    ((OdDgElementIdValueIterator*)res.get())->m_bAllowNull = true;
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgMaterialTableItemsProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdRxValueIteratorPtr res;
  const OdDgMaterialTablePtr pTable = OdDgMaterialTable::cast(pO);

  if( pTable.isNull() )
    return eNotApplicable;

  int N = 0;

  for (OdDgElementIteratorPtr it = pTable->createIterator(); !it->done(); it->step())
    ++N;
  count = N;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          element: OdDgModelTable
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Items
//-----------------------------------------------------------------------------------------------------------------------

OdRxValueIteratorPtr OdDgModelTableItemsProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  const OdDgModelTablePtr pTable = OdDgModelTable::cast(pO);

  if (!pTable.isNull())
  {
    res = OdRxObjectImpl<OdDgElementIdValueIterator>::createObject();
    ((OdDgElementIdValueIterator*)res.get())->m_pIter = pTable->createIterator();
    ((OdDgElementIdValueIterator*)res.get())->m_bAllowNull = true;
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgModelTableItemsProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdRxValueIteratorPtr res;
  const OdDgModelTablePtr pTable = OdDgModelTable::cast(pO);

  if (pTable.isNull())
    return eNotApplicable;

  int N = 0;

  for (OdDgElementIteratorPtr it = pTable->createIterator(); !it->done(); it->step())
    ++N;
  count = N;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          element: OdDgDisplayStyleTable
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Items
//-----------------------------------------------------------------------------------------------------------------------

OdRxValueIteratorPtr OdDgDisplayStyleTableItemsProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  const OdDgDisplayStyleTablePtr pTable = OdDgDisplayStyleTable::cast(pO);

  if( !pTable.isNull() )
  {
    res = OdRxObjectImpl<OdDgElementIdValueIterator>::createObject();
    ((OdDgElementIdValueIterator*)res.get())->m_pIter = pTable->createIterator();
    ((OdDgElementIdValueIterator*)res.get())->m_bAllowNull = true;
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDisplayStyleTableItemsProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdRxValueIteratorPtr res;
  const OdDgDisplayStyleTablePtr pTable = OdDgDisplayStyleTable::cast(pO);

  if( pTable.isNull() )
    return eNotApplicable;

  int N = 0;

  for (OdDgElementIteratorPtr it = pTable->createIterator(); !it->done(); it->step())
    ++N;
  count = N;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          element: OdDgLevelTableRecord
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Index
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLevelTableRecordIndexProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLevelTableRecordPtr pObj = OdDgLevelTableRecord::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getEntryId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Name
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLevelTableRecordNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLevelTableRecordPtr pObj = OdDgLevelTableRecord::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getName();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLevelTableRecordNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLevelTableRecordPtr pObj = OdDgLevelTableRecord::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setName(*rxvalue_cast<OdString>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          element: OdDgLineStyleTableRecord
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Index
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLineStyleTableRecordIndexProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLineStyleTableRecordPtr pObj = OdDgLineStyleTableRecord::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getEntryId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Name
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLineStyleTableRecordNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLineStyleTableRecordPtr pObj = OdDgLineStyleTableRecord::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getName();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLineStyleTableRecordNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLineStyleTableRecordPtr pObj = OdDgLineStyleTableRecord::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setName(*rxvalue_cast<OdString>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          element: OdDgMaterialTableRecord
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Name
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgMaterialTableRecordNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgMaterialTableRecordPtr pObj = OdDgMaterialTableRecord::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getName();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgMaterialTableRecordNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgMaterialTableRecordPtr pObj = OdDgMaterialTableRecord::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setName(*rxvalue_cast<OdString>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          element: OdDgSharedCellDefinition
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Name
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSharedCellDefinitionNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSharedCellDefinitionPtr pObj = OdDgSharedCellDefinition::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getName();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSharedCellDefinitionNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSharedCellDefinitionPtr pObj = OdDgSharedCellDefinition::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setName(*rxvalue_cast<OdString>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Elements
//-----------------------------------------------------------------------------------------------------------------------

OdRxValueIteratorPtr OdDgSharedCellDefinitionElementsProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;

  const OdDgSharedCellDefinitionPtr pObj = OdDgSharedCellDefinition::cast(pO);

  if( !pObj.isNull() )
  {
    res = OdRxObjectImpl<OdDgElementIdValueIterator>::createObject();
    ((OdDgElementIdValueIterator*)res.get())->m_pIter = pObj->createIterator();
    ((OdDgElementIdValueIterator*)res.get())->m_bAllowNull = false;
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSharedCellDefinitionElementsProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdRxValueIteratorPtr res;
  const OdDgSharedCellDefinitionPtr pObj = OdDgSharedCellDefinition::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  int N = 0;

  OdDgElementIteratorPtr pIter = pObj->createIterator();

  for (; !pIter->done(); pIter->step())
    N++;

  count = N;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          element: OdDgDisplayStyleTableRecord
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Index
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDisplayStyleTableRecordIndexProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDisplayStyleTableRecordPtr pObj = OdDgDisplayStyleTableRecord::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getEntryId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Name
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDisplayStyleTableRecordNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgDisplayStyleTableRecordPtr pObj = OdDgDisplayStyleTableRecord::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getName();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgDisplayStyleTableRecordNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgDisplayStyleTableRecordPtr pObj = OdDgDisplayStyleTableRecord::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setName(*rxvalue_cast<OdString>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          element: OdDgModel
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Name
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgModelNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgModelPtr pObj = OdDgModel::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getName();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgModelNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgModelPtr pObj = OdDgModel::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdString* pStrName = rxvalue_cast<OdString>(&value);

  if (!pStrName)
    return eNotThatKindOfClass;

  pObj->setName(*pStrName);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Elements
//-----------------------------------------------------------------------------------------------------------------------

struct OdDgModelElementsValueIterator : OdRxValueIterator
{
  OdDgElementIteratorPtr m_pIter;
  OdUInt32							 m_uIndex;
  OdDgElementIdArray     m_arrXRefs;

  virtual bool done()
  {
    if( m_pIter->done() && (m_uIndex >= m_arrXRefs.size()) )
      return true;

    return false;
  }
  virtual bool next()
  {
    bool bRet = true;

    if( !m_pIter->done() )
      m_pIter->step();
    else
    {
      if( m_uIndex < m_arrXRefs.size() )
        m_uIndex++;
      else
        bRet = false;
    }

    return bRet;
  }

  virtual OdRxValue current() const
  {
    OdRxValue retVal;

    if (!m_pIter->done())
      retVal = m_pIter->item();
    else if (m_uIndex < m_arrXRefs.size())
      retVal = m_arrXRefs[m_uIndex];

    return retVal;
  }
};

//----------------------------------------------------------------------------------------------------------------------

OdRxValueIteratorPtr OdDgModelElementsProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  const OdDgModelPtr pModel = OdDgModel::cast(pO);

  if( !pModel.isNull() )
  {
    res = OdRxObjectImpl<OdDgModelElementsValueIterator>::createObject();
    ((OdDgModelElementsValueIterator*)res.get())->m_pIter    = pModel->createGraphicsElementsIterator();
    ((OdDgModelElementsValueIterator*)res.get())->m_uIndex   = 0;

    OdDgElementIdArray arrXRefs;

    OdDgElementIteratorPtr pCtrlIter = pModel->createControlElementsIterator();

    for (; !pCtrlIter->done(); pCtrlIter->step())
    {
      OdDgElementPtr pElm = pCtrlIter->item().openObject(OdDg::kForRead);

      if( !pElm.isNull() && pElm->isKindOf(OdDgReferenceAttachmentHeader::desc()) )
        arrXRefs.push_back(pCtrlIter->item());
    }

    ((OdDgModelElementsValueIterator*)res.get())->m_arrXRefs = arrXRefs;
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgModelElementsProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdRxValueIteratorPtr res;
  const OdDgModelPtr pModel = OdDgModel::cast(pO);

  if( pModel.isNull() )
    return eNotApplicable;

  int N = 0;

  OdDgElementIteratorPtr pGraphIter = pModel->createGraphicsElementsIterator();

  for (; !pGraphIter->done(); pGraphIter->step())
  {
    N++;
  }

  OdDgElementIteratorPtr pCtrlIter = pModel->createControlElementsIterator();

  for (; !pCtrlIter->done(); pCtrlIter->step())
  {
    OdDgElementPtr pElm = pCtrlIter->item().openObject(OdDg::kForRead);

    if( !pElm.isNull() && pElm->isKindOf(OdDgReferenceAttachmentHeader::desc()) )
      N++;
  }

  count = N;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------



