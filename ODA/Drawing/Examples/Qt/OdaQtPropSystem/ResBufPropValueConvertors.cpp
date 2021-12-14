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
//
// ResBufPropValueConvertors.cpp
//

#include "OdaCommon.h"
#include "SharedPtr.h"

#include "DbDatabase.h"
#include "DbBaseDatabase.h"
#include "DbBaseSubentId.h"
#include "DbUnitsFormatter.h"
#include "DbMaterial.h"
#include "DbSymbolTableRecord.h"
#include "RxDictionary.h"
#include "DbDictionary.h"
#include "DbHyperlink.h"
#include "DbCurve.h"
#include "DbArc.h"
#include "DbEllipse.h"
#include "DbHatch.h"

#include "ResBufPropValueConvertors.h"

//------------------------------------------------------------------------

static OdPropServices* propServices()
{
  OdPropServicesPtr pPropServices = ::odrxSysRegistry()->getAt(OD_PROP_SERVICES);
  ODA_ASSERT_ONCE(!pPropServices.isNull());
  return pPropServices.get();
}

static OdString getDictRecName(const OdString& sHandle, 
                               OdDbObjectId idDictionary,
                               const OdString sDefault = OD_T("Standard"))
{
  if (sHandle.isEmpty())
    return sDefault;
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(idDictionary.database()).get();
  ODA_ASSERT_ONCE(pDwgDb);
  if (!pDwgDb)
    return sDefault;
  OdUInt64 handle = 0;
  if (!odSScanf(sHandle, L"%" PRIx64W, &handle) || OdDbHandle(handle).isNull())
  {
    ODA_FAIL_ONCE(); // test
    return sDefault;
  }

  OdDbObjectId id = pDwgDb->getOdDbObjectId(OdDbHandle(handle));
  ODA_ASSERT_ONCE(!id.isNull());
  if (id.isNull())
    return sDefault;
  OdDbObjectPtr pObject = id.openObject(); // OdDbPlaceHolder, OdDbMaterial ...
  ODA_ASSERT_ONCE(!pObject.isNull());
  if (pObject.isNull())
    return sDefault;

  OdString sName;
  OdDbMaterial* pMaterial = NULL;
  OdDbSymbolTableRecord* pRecord;
  if ((pMaterial = OdDbMaterial::cast(pObject.get()).get()))
    sName = pMaterial->name();
  else if ((pRecord = OdDbSymbolTableRecord::cast(pObject.get()).get()))
    sName = pRecord->getName();
  else
  {
    ODA_ASSERT_ONCE(idDictionary == pObject->ownerId());
    OdDbDictionaryPtr pDict = OdDbDictionary::cast(idDictionary.openObject());
    if (!pDict.isNull())
      sName = pDict->nameAt(id);
  }

  ODA_ASSERT_ONCE(!sName.isEmpty());
  if (sName.isEmpty())
    return sDefault;
  return sName;
}

static OdString setDictRecName(const OdString& sPaletteValue, OdDbObjectId idDictionary)
{
  ODA_ASSERT_ONCE(!idDictionary.isNull());
  if (idDictionary.isNull())
    return TD_MARKER_TO_CANCEL;

  OdDbObjectPtr pDictObj = idDictionary.openObject();
  ODA_ASSERT_ONCE(!pDictObj.isNull());
  if (pDictObj.isNull())
    return TD_MARKER_TO_CANCEL;

  OdDbDictionaryPtr pDict = OdDbDictionary::cast(pDictObj.get());
  ODA_ASSERT_ONCE(!pDict.isNull()); // TODO OdDbSymbolTableRecord* pSymTable = OdDbSymbolTableRecord::cast(pDictObj.get()).get()
  if (pDict.isNull())
    return TD_MARKER_TO_CANCEL;

  OdDbObjectId idStyle = pDict->getAt(sPaletteValue);
  ODA_ASSERT_ONCE(!idStyle.isNull());
  if (idStyle.isNull())
    return TD_MARKER_TO_CANCEL;

  OdString sValue;
  sValue.format(L"%" PRIX64W, (OdUInt64) idStyle.getHandle());
  return sValue;
}

static bool getDictRecPossibleNames(OdDbObjectId idDictionary,
                                    OdStringArray& values)
{
  ODA_ASSERT_ONCE(!idDictionary.isNull());
  if (idDictionary.isNull())
    return false;

  OdDbObjectPtr pDictObj = idDictionary.openObject();
  ODA_ASSERT_ONCE(!pDictObj.isNull());
  if (pDictObj.isNull())
    return false;

  OdDbDictionaryPtr pDict = OdDbDictionary::cast(pDictObj.get());
  ODA_ASSERT_ONCE(!pDict.isNull()); // TODO OdDbSymbolTableRecord* pSymTable = OdDbSymbolTableRecord::cast(pDictObj.get()).get()
  if (pDict.isNull())
    return false;

  for (OdDbDictionaryIteratorPtr pItr = pDict->newIterator(); 
       !pItr->done(); pItr->next())
  {
    OdString sName = pItr->name();
    ODA_ASSERT_ONCE(!sName.isEmpty());
    if (values.isEmpty() && sName != OD_T("ByLayer") && sName != OD_T("ByBlock"))
    { // it is so for PlotStyleNameDictionary ...
      values.push_back(propServices()->tr("ByLayer"));
      values.push_back(propServices()->tr("ByBlock"));
    }

    values.push_back(sName);
  }
  return true;
}

//------------------------------------------------------------------------

static bool convertorGetPossible_LAYER(OdStringArray& values, OdStringArray& iconPaths, 
                                       OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(pRxDatabase);
  for (OdRxIteratorPtr pIter = OdDbBaseDatabasePEPtr(pRxDatabase)->layers(pRxDatabase); 
       !pIter->done(); pIter->next())
  {
    OdRxObjectPtr pLayerObject = pIter->object();
    OdDbBaseLayerPEPtr pLayerPE = OdDbBaseLayerPE::cast(pLayerObject);
    ODA_ASSERT_ONCE(!pLayerPE.isNull());
    if (pLayerPE.isNull())
      continue;
    OdString sLayerName = pLayerPE->name(pLayerObject);
    ODA_ASSERT_ONCE(!sLayerName.isEmpty());
    if (   sLayerName.isEmpty() 
        || pLayerPE->isOff(pLayerObject) || pLayerPE->isFrozen(pLayerObject))
      continue;

    values.push_back(sLayerName);
  }
  return true;
}

static OdString convertorFromResBuf_PLOTSTYLE(const OdString& sResBufValues, OdDbStub* id)
{
  OdDb::PlotStyleNameType type = OdDb::kPlotStyleNameByLayer;
  OdDbDatabase* pDwgDb = NULL;
  if (id)
    pDwgDb = OdDbDatabase::cast(baseDatabaseBy(id)).get();
  ODA_ASSERT_ONCE(pDwgDb);

  OdString sValue(sResBufValues);
  OdString sGrCode = OdPropSource::extractFirst(sValue, __OD_T(','), true);

  if (!pDwgDb || pDwgDb->getPSTYLEMODE()) // ByColor
  {
    ODA_ASSERT_ONCE(sGrCode.isEmpty());
    sValue = propServices()->tr("ByColor");
    sValue += OD_T("\" Enabled=\"false");
    return sValue;
  }

  if (sGrCode == OD_T("380"))
    odSScanf(sValue, __OD_T("%i"), &type);
  else if (sGrCode == OD_T("390"))
    type = OdDb::kPlotStyleNameById; 

  switch (type)
  {
  case OdDb::kPlotStyleNameByLayer:
    sValue = propServices()->tr("ByLayer");
    break;
  case OdDb::kPlotStyleNameByBlock:
    sValue = propServices()->tr("ByBlock");
    break;
  case OdDb::kPlotStyleNameIsDictDefault:
    ODA_FAIL_ONCE(); // test
  case OdDb::kPlotStyleNameById:
    sValue = getDictRecName(sValue, pDwgDb->getPlotStyleNameDictionaryId(), OD_T("Normal"));
    break;
  default:
    sValue = OdString::kEmpty;
    break;
  }
  ODA_ASSERT_ONCE(!sValue.isEmpty());
  return sValue;
}

static OdString convertorToResBuf_PLOTSTYLE(const OdString& sPaletteValue, OdDbStub* id, OdString& sHintGrCode)
{
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(baseDatabaseBy(id)).get();
  if (!pDwgDb || pDwgDb->getPSTYLEMODE()) // ByColor
  {
    ODA_FAIL_ONCE();
    return TD_MARKER_TO_CANCEL;
  }

  sHintGrCode = OD_T("380");
  if (sPaletteValue == propServices()->tr("ByLayer"))
    return OD_T("0"); // OdDb::kPlotStyleNameByLayer
  if (sPaletteValue == propServices()->tr("ByBlock"))
    return OD_T("1"); // OdDb::kPlotStyleNameByBlock

  sHintGrCode = OD_T("390"); // OdDb::kPlotStyleNameById
  if (sPaletteValue == propServices()->tr("Other..."))
  {
    ODA_FAIL_ONCE(); // TODO
    //propServices()->openDialog(...);
    return TD_MARKER_TO_CANCEL;
  }

  OdDbDictionaryPtr pDict = pDwgDb->getPlotStyleNameDictionaryId().openObject();
  ODA_ASSERT_ONCE(!pDict.isNull());
  if (pDict.isNull())
    return TD_MARKER_TO_CANCEL;
  OdDbObjectId idStyle = pDict->getAt(sPaletteValue);
  ODA_ASSERT_ONCE(!idStyle.isNull());
  if (idStyle.isNull())
    return TD_MARKER_TO_CANCEL;

  OdString sValue;
  sValue.format(L"%" PRIX64W, (OdUInt64) idStyle.getHandle());
  return sValue;
}

static bool convertorGetPossible_PLOTSTYLE(OdStringArray& values, OdStringArray& iconPaths, 
                                           OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(pRxDatabase);
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase).get();
  ODA_ASSERT_ONCE(pDwgDb);
  if (!pDwgDb || pDwgDb->getPSTYLEMODE()) // ByColor
    return false;

  return getDictRecPossibleNames(pDwgDb->getPlotStyleNameDictionaryId(), values);
}

static OdString convertorFromResBuf_MATERIAL(const OdString& sResBufValue, OdDbStub* id) 
{
  OdDbObjectId idDictionary;
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(baseDatabaseBy(id)).get();
  ODA_ASSERT_ONCE(pDwgDb); // test
  if (pDwgDb)
    idDictionary = pDwgDb->getMaterialDictionaryId();

  return getDictRecName(sResBufValue, idDictionary, propServices()->tr("ByLayer"));
}

static OdString convertorToResBuf_MATERIAL(const OdString& sPaletteValue, OdDbStub* id, OdString&) // sHintGrCode
{
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(baseDatabaseBy(id)).get();
  ODA_ASSERT_ONCE(pDwgDb); // test
  if (!pDwgDb)
    return TD_MARKER_TO_CANCEL;

  return setDictRecName(sPaletteValue, pDwgDb->getMaterialDictionaryId());
}

static bool convertorGetPossible_MATERIAL(OdStringArray& values, OdStringArray&, // iconPaths
                                          OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(pRxDatabase);
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase).get();
  ODA_ASSERT_ONCE(pDwgDb);
  if (!pDwgDb)
    return false;

  return getDictRecPossibleNames(pDwgDb->getMaterialDictionaryId(), values);
}

static OdDbHyperlinkCollectionPtr openHyperlinkCollection(OdDbStub* id, 
                                                          OdDbObjectPtr* ppObjectToWrite = NULL)
{
  OdDbHyperlinkCollectionPtr pCollection;

  OdDbDatabase* pDwgDb = OdDbDatabase::cast(baseDatabaseBy(id)).get();
  ODA_ASSERT_ONCE(pDwgDb); // test
  if (!pDwgDb)
    return pCollection;

  OdDbObjectPtr pObj = OdDbObjectId(id).openObject(ppObjectToWrite ? OdDb::kForWrite : OdDb::kForRead);
  ODA_ASSERT_ONCE(!pObj.isNull());
  if (pObj.isNull())
    return pCollection;
  if (ppObjectToWrite)
    *ppObjectToWrite = pObj;

  OdDbEntityHyperlinkPEPtr pHyperlinkPE = OdDbEntityHyperlinkPE::cast(pObj);
  ODA_ASSERT_ONCE(!pHyperlinkPE.isNull()); // test
  if (pHyperlinkPE.isNull())
    return pCollection;

  pCollection = pHyperlinkPE->getHyperlinkCollection(pObj, true); // oneOnly
  ODA_ASSERT_ONCE(!pCollection.isNull());
  return pCollection;
}

static OdString convertorFromResBuf_HYPERLINK(const OdString&, OdDbStub* id)
{
  OdString sValue;
  OdDbHyperlinkCollectionPtr pCollection = openHyperlinkCollection(id);
  OdDbHyperlink* pItem = NULL;
  if (pCollection.isNull() || !pCollection->count() || !(pItem = pCollection->item(0)))
    return sValue;
  
  sValue = pItem->getDisplayString();
  ODA_ASSERT_ONCE(!sValue.isEmpty());
  return sValue;
}

static OdString convertorToResBuf_HYPERLINK(const OdString& sPaletteValue, OdDbStub* id, OdString&) // sHintGrCode
{
  OdDbObjectPtr pObjectToWrite;
  OdDbHyperlinkCollectionPtr pCollection = openHyperlinkCollection(id, &pObjectToWrite);
  if (pCollection.isNull())
    return TD_MARKER_TO_CANCEL;

  while (pCollection->count())
    pCollection->removeTail();

  if (!sPaletteValue.isEmpty())
    pCollection->addHead(sPaletteValue, // Name
                         sPaletteValue, // Description
                         OdString::kEmpty); // SubLocation
  OdDbEntityHyperlinkPE::cast(pObjectToWrite)->setHyperlinkCollection(pObjectToWrite, pCollection);

  return TD_MARKER_FOREACH_TO_SET_DIRECTLY;
}

static OdString convertorFromResBuf_LINEAR_(double dValue, OdDbStub* id)
{
  OdString sValue = OD_T("0.0000");
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(baseDatabaseBy(id));
  if (!pDwgDb) // skip for DGN
    return sValue;

  sValue = pDwgDb->formatter().formatLinear(dValue);
  return sValue;
}

static double convertorToResBuf_LINEAR_(const OdString& sValue, OdDbStub* id)
{
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(baseDatabaseBy(id));
  if (!pDwgDb) // skip for DGN
    return 0.0;

  return pDwgDb->formatter().unformatLinear(sValue);
}

static OdString convertorFromResBuf_LINEAR(const OdString& sResBufValue, OdDbStub* id)
{
  OdString sValues = sResBufValue;
  OdString sValue = OdPropSource::extractFirst(sValues, __OD_T(','), true);
  double dValue = 0.0;
  OdChar* pEnd = NULL;

  if (!sValue.isEmpty())
  {
    //if (!odSScanf(sValue, OD_T("%lg"), &dValue))
    dValue = odStrToD(sValue, &pEnd);
    if (pEnd <= sValue.c_str())
    {
      ODA_FAIL_ONCE();
      return sValue;
    }
  }
  if (!sValues.isEmpty())
  {
    double dScale = 0.0;
    //if (odSScanf(sValues, OD_T("%lg"), &dScale) == 1 && dScale > 0.0)
    dScale = odStrToD(sValues, &pEnd);
    if (pEnd > sValues.c_str() && dScale > 0.0)
    {
      getResBufPropController().setConvertorAdvArg(OD_T("scale"), sValues); // save to used with convertorToResBuf_LINEAR
      dValue *= dScale;
    }
    else
    {
      ODA_FAIL_ONCE();
    }
  }

  return convertorFromResBuf_LINEAR_(dValue, id);
}

static OdString convertorToResBuf_LINEAR(const OdString& sPaletteValue, OdDbStub* id, OdString&) // sHintGrCode
{
  OdString sScale;
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(baseDatabaseBy(id));
  if (!pDwgDb || (sScale = getResBufPropController().getConvertorAdvArg(OD_T("scale"))).isEmpty())
    return sPaletteValue;

  double dValue = 0.0;
  OdChar* pEnd = NULL;

  //if (!odSScanf(sPaletteValue, OD_T("%lg"), &dValue))
  dValue = odStrToD(sPaletteValue, &pEnd);
  if (pEnd <= sPaletteValue.c_str())
  {
    ODA_FAIL_ONCE();
    return sPaletteValue;
  }
  double dScale = 0.0;
  //if (!odSScanf(sScale, OD_T("%lg"), &dScale) || dScale <= 0.0)
  dScale = odStrToD(sScale, &pEnd);
  if (pEnd <= sScale.c_str() || dScale <= 0.0)
  {
    ODA_FAIL_ONCE();
    return sPaletteValue;
  }
  dValue /= dScale;

  OdString sValue = pDwgDb->formatter().formatLinear(dValue);
  return sValue;
}

static OdString convertorFromResBuf_ANGULAR_(double dValue, OdDbStub* id)
{
  OdString sValue = OD_T("0.0000");
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(baseDatabaseBy(id));
  if (!pDwgDb) // skip for DGN
    return sValue;

  sValue = pDwgDb->formatter().formatAngle(dValue);
  return sValue;
}

static double convertorToResBuf_ANGULAR_(const OdString& sValue, OdDbStub* id)
{
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(baseDatabaseBy(id));
  if (!pDwgDb) // skip for DGN
    return 0.0;

  return pDwgDb->formatter().unformatAngle(sValue);
}

static OdString convertorFromResBuf_ANGULAR(const OdString& sResBufValue, OdDbStub* id)
{
  OdString sValue = sResBufValue;
  if (sValue.isEmpty())
    sValue = OD_T("0.0000");
  double dValue = 0.0;
  OdChar* pEnd = NULL;

  //if (!odSScanf(sValue, OD_T("%lg"), &dValue))
  dValue = odStrToD(sValue, &pEnd);
  if (pEnd <= sValue.c_str())
  {
    ODA_FAIL_ONCE();
    return sValue;
  }

  sValue = convertorFromResBuf_ANGULAR_(dValue, id);
  return sValue;
}

static OdString xyzFromResBuf_(const OdString& sResBufValues, int index,  OdDbStub* id)
{
  OdString sValues = sResBufValues;
  OdString sValue = OdPropSource::extractFirst(sValues, __OD_T(','), true); // default
  if (!sValues.isEmpty())
  {
    int cnt = index + 1;
    while (cnt--)
      sValue = OdPropSource::extractFirst(sValues, __OD_T(','), true);
  }
  ODA_ASSERT_ONCE(!sValue.isEmpty());
  
  return convertorFromResBuf_LINEAR(sValue, id);
}

static OdString xyzToResBuf_(const OdString& csPaletteValue, int index, OdDbStub* id)
{
  OdString sPaletteValue = csPaletteValue;
  sPaletteValue.replace(__OD_T(','), __OD_T('.'));

  OdString sValues; 
  sValues.format(OD_T("%ls,%ls,%ls"), !index ? sPaletteValue.c_str() : OD_T(""),
                                      index == 1 ? sPaletteValue.c_str() : OD_T(""),
                                      index == 2 ? sPaletteValue.c_str() : OD_T(""));
  return sValues;
}

static OdString xyzFromResBuf_(const OdString& sCoordName, const OdGePoint3d& point,  OdDbStub* id)
{
  OdString sValues = sCoordName;
  OdString sValue = OdPropSource::extractFirst(sValues, __OD_T(','), true).makeLower();
  ODA_ASSERT_ONCE(sValue.getLength() == 1 && sValues.isEmpty());
  double dValue = 0;
  switch (sValue.getAt(0))
  {
  default:
    ODA_FAIL_ONCE(); // TODO?
  case __OD_T('x'):
    dValue = point.x;
    break;
  case __OD_T('y'):
    dValue = point.y;
    break;
  case __OD_T('z'):
    dValue = point.z;
    break;
  }
  return convertorFromResBuf_LINEAR_(dValue, id);
}

static OdString convertorFromResBuf_X(const OdString& sResBufValues, OdDbStub* id)
{
  return xyzFromResBuf_(sResBufValues, 0, id);
}

static OdString convertorToResBuf_X(const OdString& sPaletteValue, OdDbStub* id, OdString&) // sHintGrCode
{
  return xyzToResBuf_(sPaletteValue, 0, id);
}

static OdString convertorFromResBuf_Y(const OdString& sResBufValues, OdDbStub* id)
{
  return xyzFromResBuf_(sResBufValues, 1, id);
}

static OdString convertorToResBuf_Y(const OdString& sPaletteValue, OdDbStub* id, OdString&) // sHintGrCode
{
  return xyzToResBuf_(sPaletteValue, 1, id);
}

static OdString convertorFromResBuf_Z(const OdString& sResBufValues, OdDbStub* id)
{
  return xyzFromResBuf_(sResBufValues, 2, id);
}

static OdString convertorToResBuf_Z(const OdString& sPaletteValue, OdDbStub* id, OdString&) // sHintGrCode
{
  return xyzToResBuf_(sPaletteValue, 2, id);
}

static OdString convertorFromResBuf_START(const OdString& sCoordName, OdDbStub* id) 
{
  OdString sValue;
  if (id && !sCoordName.isEmpty())
  {
    switch (sCoordName.getAt(0))
    {
    case __OD_T('a'): case __OD_T('A'):
      {
        double dAngle = 0.0;
        OdDbArcPtr pArc;
        OdDbEllipsePtr pEllipse;
        if ((pArc = OdDbArc::cast(OdDbObjectId(id).openObject())).get())
          dAngle = pArc->startAngle();
        else if ((pEllipse = OdDbEllipse::cast(OdDbObjectId(id).openObject())).get())
          dAngle = pEllipse->startAngle();
        else
        {
          ODA_FAIL_ONCE();
          break;
        }
        getResBufPropController().setConvertorAdvArg(OD_T("coordname"), OD_T("a"));

        sValue = convertorFromResBuf_ANGULAR_(dAngle, id);
        break;
      }
    default:
      {
        OdDbCurvePtr pCurve = OdDbCurve::cast(OdDbObjectId(id).openObject());
        ODA_ASSERT_ONCE(!pCurve.isNull()); // TODO
        if (pCurve.isNull())
          break;
  
        OdGePoint3d point;
        if (pCurve->getStartPoint(point) != eOk)
        {
          ODA_FAIL_ONCE();
          break;
        }

        sValue = xyzFromResBuf_(sCoordName, point, id);
        sValue += OD_T("\" Enabled=\"false");
        break;
      }
    } // switch
  }

  return sValue;
}

static OdString convertorToResBuf_START(const OdString& csPaletteValue, OdDbStub* id, OdString&) // sHintGrCode
{
  OdString sCoordName = getResBufPropController().getConvertorAdvArg(OD_T("coordname"));
  if (!id || sCoordName.isEmpty() || sCoordName.getAt(0) != __OD_T('a'))
  {
    ODA_FAIL_ONCE();
    return TD_MARKER_TO_CANCEL;
  }

  OdString sPaletteValue = csPaletteValue;
  sPaletteValue.replace(__OD_T(','), __OD_T('.'));
  double dAngle = 0.0;
  OdChar* pEnd = NULL;

  //if (!odSScanf(sPaletteValue, OD_T("%lg"), &dAngle))
  dAngle = odStrToD(sPaletteValue, &pEnd);
  if (pEnd <= sPaletteValue.c_str())
    return TD_MARKER_TO_CANCEL;
  dAngle = convertorToResBuf_ANGULAR_(sPaletteValue, id);

  OdDbArcPtr pArc;
  OdDbEllipsePtr pEllipse;
  if ((pArc = OdDbArc::cast(OdDbObjectId(id).openObject(OdDb::kForWrite))).get())
    pArc->setStartAngle(dAngle);
  else if ((pEllipse = OdDbEllipse::cast(OdDbObjectId(id).openObject(OdDb::kForWrite))).get())
    pEllipse->setStartAngle(dAngle);
  else
  {
    ODA_FAIL_ONCE();
    return TD_MARKER_TO_CANCEL;
  }

  return TD_MARKER_FOREACH_TO_SET_DIRECTLY;
}

static OdString convertorFromResBuf_END(const OdString& sCoordName, OdDbStub* id) 
{
  OdString sValue;
  if (id && !sCoordName.isEmpty())
  {
    switch (sCoordName.getAt(0))
    {
    case __OD_T('a'): case __OD_T('A'):
      {
        double dAngle = 0.0;
        OdDbArcPtr pArc;
        OdDbEllipsePtr pEllipse;
        if ((pArc = OdDbArc::cast(OdDbObjectId(id).openObject())).get())
          dAngle = pArc->endAngle();
        else if ((pEllipse = OdDbEllipse::cast(OdDbObjectId(id).openObject())).get())
          dAngle = pEllipse->endAngle();
        else
        {
          ODA_FAIL_ONCE();
          break;
        }
        getResBufPropController().setConvertorAdvArg(OD_T("coordname"), OD_T("a"));

        sValue = convertorFromResBuf_ANGULAR_(dAngle, id);
        break;
      }
    default:
      {
        OdDbCurvePtr pCurve = OdDbCurve::cast(OdDbObjectId(id).openObject());
        ODA_ASSERT_ONCE(!pCurve.isNull()); // TODO
        if (pCurve.isNull())
          break;

        OdGePoint3d point;
        if (pCurve->getEndPoint(point) != eOk)
        {
          ODA_FAIL_ONCE();
          break;
        }
        sValue = xyzFromResBuf_(sCoordName, point, id);
        sValue += OD_T("\" Enabled=\"false");
        break;
      }
    } // switch
  }

  return sValue;
}

static OdString convertorToResBuf_END(const OdString& csPaletteValue, OdDbStub* id, OdString&) // sHintGrCode
{
  OdString sCoordName = getResBufPropController().getConvertorAdvArg(OD_T("coordname"));
  if (!id || sCoordName.isEmpty() || sCoordName.getAt(0) != __OD_T('a'))
  {
    ODA_FAIL_ONCE();
    return TD_MARKER_TO_CANCEL;
  }

  OdString sPaletteValue = csPaletteValue;
  sPaletteValue.replace(__OD_T(','), __OD_T('.'));
  double dAngle = 0.0;
  OdChar* pEnd = NULL;

  //if (!odSScanf(sPaletteValue, OD_T("%lg"), &dAngle))
  dAngle = odStrToD(sPaletteValue, &pEnd);
  if (pEnd <= sPaletteValue.c_str())
    return TD_MARKER_TO_CANCEL;
  dAngle = convertorToResBuf_ANGULAR_(sPaletteValue, id);

  OdDbArcPtr pArc;
  OdDbEllipsePtr pEllipse;
  if ((pArc = OdDbArc::cast(OdDbObjectId(id).openObject(OdDb::kForWrite))).get())
    pArc->setEndAngle(dAngle);
  else if ((pEllipse = OdDbEllipse::cast(OdDbObjectId(id).openObject(OdDb::kForWrite))).get())
    pEllipse->setEndAngle(dAngle);
  else
  {
    ODA_FAIL_ONCE();
    return TD_MARKER_TO_CANCEL;
  }

  return TD_MARKER_FOREACH_TO_SET_DIRECTLY;
}

static OdString convertorFromResBuf_DELTA(const OdString& sCoordName, OdDbStub* id) 
{
  OdString sValue;
  if (id && !sCoordName.isEmpty())
  {
    switch (sCoordName.getAt(0))
    {
    case __OD_T('a'): case __OD_T('A'):
      {
        double dAngle = 0.0;
        OdDbArcPtr pArc;
        OdDbEllipsePtr pEllipse;
        if ((pArc = OdDbArc::cast(OdDbObjectId(id).openObject())).get())
          dAngle = pArc->endAngle() - pArc->startAngle();
        else if ((pEllipse = OdDbEllipse::cast(OdDbObjectId(id).openObject())).get())
          dAngle = pEllipse->endAngle() - pEllipse->startAngle();
        else
        {
          ODA_FAIL_ONCE();
          break;
        }

        sValue = convertorFromResBuf_ANGULAR_(dAngle, id);
        break;
      }
    default:
      {
        OdDbCurvePtr pCurve = OdDbCurve::cast(OdDbObjectId(id).openObject());
        ODA_ASSERT_ONCE(!pCurve.isNull());
        if (pCurve.isNull())
          break;

        OdGePoint3d startPoint,
                    endPoint;
        if (   pCurve->getStartPoint(startPoint) != eOk
            || pCurve->getEndPoint(endPoint) != eOk)
        {
          ODA_FAIL_ONCE();
          break;
        }
        OdGePoint3d point = endPoint;
        point.x -= startPoint.x;
        point.y -= startPoint.y;
        point.z -= startPoint.z;
        sValue = xyzFromResBuf_(sCoordName, point, id);
        break;
      }
    } // switch
  }

  sValue += OD_T("\" Enabled=\"false");
  return sValue;
}

static OdString convertorFromResBuf_LENGTH(const OdString&, OdDbStub* id) 
{
  OdDbCurvePtr pCurve;
  if (id)
    pCurve = OdDbCurve::cast(OdDbObjectId(id).openObject());

  OdString sValue;
  ODA_ASSERT_ONCE(!pCurve.isNull()); // TODO
  if (!pCurve.isNull())
  {
    double dStartParam = 0.0,
           dEndParam = 0.0,
           dStartDist = 0.0,
           dEndDist = 0.0;
    if (   pCurve->getStartParam(dStartParam) == eOk
        && pCurve->getEndParam(dEndParam) == eOk
        && pCurve->getDistAtParam(dStartParam, dStartDist) == eOk
        && pCurve->getDistAtParam(dEndParam, dEndDist) == eOk)
    {
      ODA_ASSERT_ONCE(dEndDist >= dStartDist && dStartDist == 0.0); // test
      sValue = convertorFromResBuf_LINEAR_(dEndDist - dStartDist, id);
    }
    else
    {
      ODA_FAIL_ONCE(); // TODO
    }
  }

  sValue += OD_T("\" Enabled=\"false");
  return sValue;
}

static OdString convertorFromResBuf_ANGLE(const OdString&, OdDbStub* id) 
{
  OdDbCurvePtr pCurve;
  if (id)
    pCurve = OdDbCurve::cast(OdDbObjectId(id).openObject());

  OdString sValue;
  ODA_ASSERT_ONCE(!pCurve.isNull()); // TODO
  if (!pCurve.isNull())
  {
    OdGePoint3d startPoint,
                endPoint;
    if (   pCurve->getStartPoint(startPoint) == eOk
        && pCurve->getEndPoint(endPoint) == eOk)
    {
      OdGeVector3d ray = endPoint - startPoint;
      double dAngle = ray.angleTo(OdGeVector3d(1, 0, 0), OdGeVector3d(0, 0, -1));
      sValue = convertorFromResBuf_ANGULAR_(dAngle, id);
    }
    else
    {
      ODA_FAIL_ONCE(); // TODO
    }
  }

  sValue += OD_T("\" Enabled=\"false");
  return sValue;
}

static OdString convertorFromResBuf_AREA(const OdString&, OdDbStub* id) 
{
  OdString sValue;

  ODA_ASSERT_ONCE(id);
  if (!id)
    return sValue;

  double dArea = 0.0;
  OdDbObjectPtr pObj = OdDbObjectId(id).openObject();
  OdDbCurvePtr pCurve;
  OdDbHatchPtr pHatch;
  if (!(pCurve = OdDbCurve::cast(pObj.get())).isNull())
  {
    if (pCurve->getArea(dArea) == eOk)
      sValue = convertorFromResBuf_LINEAR_(dArea, id);
    else
    {
      ODA_FAIL_ONCE(); // TODO
    }
  }
  else if (!(pHatch = OdDbHatch::cast(pObj.get())).isNull())
  {
    if (pHatch->getArea(dArea) == eOk)
      sValue = convertorFromResBuf_LINEAR_(dArea, id);
    else
    {
      ODA_FAIL_ONCE(); // TODO
    }
  }
  else
  {
    ODA_FAIL_ONCE(); // TODO
  }

  sValue += OD_T("\" Enabled=\"false");
  return sValue;
}

static OdString convertorFromResBuf_RADIUS(const OdString& sResBufValue, OdDbStub* id) 
{
  OdString sValues = sResBufValue;
  OdString sType = OdPropSource::extractFirst(sValues, __OD_T(','), true).makeLower();
  ODA_ASSERT_ONCE(sValues.isEmpty());

  OdString sValue;
  double dRadius = 0.0;
  OdDbArcPtr pArc;
  OdDbEllipsePtr pEllipse;
  if ((pArc = OdDbArc::cast(OdDbObjectId(id).openObject())).get())
  {
    ODA_ASSERT_ONCE(sType.isEmpty());
    dRadius = pArc->radius();
  }
  else if ((pEllipse = OdDbEllipse::cast(OdDbObjectId(id).openObject())).get())
  {
    if (sType == OD_T("minor"))
      dRadius = pEllipse->minorAxis().length();
    else
    {
      ODA_ASSERT_ONCE(sType.isEmpty() || sType == OD_T("major"));
      sType = OD_T("major");
      dRadius = pEllipse->majorAxis().length();
    }
    getResBufPropController().setConvertorAdvArg(OD_T("type"), sType);
  }
  else
  {
    ODA_FAIL_ONCE();
    sValue += OD_T("\" Enabled=\"false");
    return sValue;
  }

  sValue = convertorFromResBuf_LINEAR_(dRadius, id);
  //sValue += OD_T("\" Enabled=\"false");
  return sValue;
}

static OdString convertorToResBuf_RADIUS(const OdString& csPaletteValue, OdDbStub* id, OdString&) // sHintGrCode
{
  OdString sType = getResBufPropController().getConvertorAdvArg(OD_T("type"));
  if (!id)
  {
    ODA_FAIL_ONCE();
    return TD_MARKER_TO_CANCEL;
  }

  OdString sPaletteValue = csPaletteValue;
  sPaletteValue.replace(__OD_T(','), __OD_T('.'));
  double dRadius = 0.0;
  OdChar* pEnd = NULL;

  //if (!odSScanf(sPaletteValue, OD_T("%lg"), &dRadius))
  dRadius = odStrToD(sPaletteValue, &pEnd);
  if (pEnd <= sPaletteValue.c_str())
    return TD_MARKER_TO_CANCEL;
  if (dRadius <= 0)
    return TD_MARKER_OUT_OF_RANGE;
  dRadius = convertorToResBuf_LINEAR_(sPaletteValue, id);

  OdDbArcPtr pArc;
  OdDbEllipsePtr pEllipse;
  if ((pArc = OdDbArc::cast(OdDbObjectId(id).openObject(OdDb::kForWrite))).get())
  {
    ODA_ASSERT_ONCE(sType.isEmpty());
    pArc->setRadius(dRadius);
  }
  else if ((pEllipse = OdDbEllipse::cast(OdDbObjectId(id).openObject(OdDb::kForWrite))).get())
  {
    if (sType == OD_T("minor"))
    {
      double dMajorRadius = pEllipse->majorAxis().length();
      ODA_ASSERT_ONCE(dMajorRadius > 0); // test
      if (dMajorRadius <= 0)
        return TD_MARKER_TO_CANCEL;

      pEllipse->setRadiusRatio(dRadius / dMajorRadius);
    }
    else
    {
      ODA_ASSERT_ONCE(sType.isEmpty() || sType == OD_T("major"));

      OdGePoint3d center;
      OdGeVector3d unitNormal, majorAxis;
      double radiusRatio = 0.0,
             startAngle = 0.0,
             endAngle = 0.0;
      pEllipse->get(center, unitNormal, majorAxis, radiusRatio, startAngle, endAngle);

      double dMajorRadius = majorAxis.length();
      ODA_ASSERT_ONCE(dMajorRadius > 0); // test
      if (dMajorRadius <=  0)
        return TD_MARKER_TO_CANCEL;
      majorAxis.x *= dRadius / dMajorRadius;
      majorAxis.y *= dRadius / dMajorRadius;
      majorAxis.z *= dRadius / dMajorRadius;
      radiusRatio *= dMajorRadius / dRadius;
      try
      {
        pEllipse->set(center, unitNormal, majorAxis, radiusRatio, startAngle, endAngle);
      }
      catch (...)
      {
        ODA_FAIL_ONCE();
        return TD_MARKER_TO_CANCEL;
      }

    }
  }
  else
  {
    ODA_FAIL_ONCE();
    return TD_MARKER_TO_CANCEL;
  }

  return TD_MARKER_FOREACH_TO_SET_DIRECTLY;
}

static OdString convertorFromResBuf_AXIS(const OdString& sResBufValue, OdDbStub* id) 
{
  OdString sValues = sResBufValue;
  OdString sType = OdPropSource::extractFirst(sValues, __OD_T(','), true).makeLower(),
           sCoordName = OdPropSource::extractFirst(sValues, __OD_T(','), true).makeLower();
  ODA_ASSERT_ONCE(sValues.isEmpty() && !sCoordName.isEmpty());

  OdString sValue;
  OdDbEllipsePtr pEllipse;
  if (   !sCoordName.isEmpty() 
      && (pEllipse = OdDbEllipse::cast(OdDbObjectId(id).openObject())).get())
  {
    OdGeVector3d vAxis;
    if (sType == OD_T("minor"))
      vAxis = pEllipse->minorAxis();
    else
    {
      ODA_ASSERT_ONCE(sType.isEmpty() || sType == OD_T("major"));
      vAxis = pEllipse->majorAxis();
    }
    //getResBufPropController().setConvertorAdvArg(OD_T("type"), sType);
    //getResBufPropController().setConvertorAdvArg(OD_T("coordname"), sCoordName);

    sValue = xyzFromResBuf_(sCoordName, OdGePoint3d(vAxis.x, vAxis.y, vAxis.z), id);
  }
  else
  {
    ODA_FAIL_ONCE();
  }

  sValue += OD_T("\" Enabled=\"false");
  return sValue;
}

//------------------------------------------------------------------------

OdResBufPropControllerImpl::CallBacks::CallBacks()
  : m_fnFromRB(NULL)
  , m_fnToRB(NULL)
  , m_fnGetPossible(NULL)
{
}

OdResBufPropControllerImpl::OdResBufPropControllerImpl()
{
}

void OdResBufPropControllerImpl::addConvertor(const OdString& sConvertorName,
                                              TConvertorFromResBufPropValue* pConvertorFrom,
                                              TConvertorToResBufPropValue* pConvertorTo,
                                              TConvertorPossibleValues* pGetPossibleValues)
{
  ODA_ASSERT_ONCE(   !sConvertorName.isEmpty() 
                  && (pConvertorFrom || pConvertorTo || pGetPossibleValues));
  if (sConvertorName.isEmpty())
    return;

  CallBacks& funcs = m_mapCallbacks[sConvertorName];
  ODA_ASSERT_ONCE(   (!funcs.m_fnFromRB || !pConvertorFrom)
                  && (!funcs.m_fnToRB || !pConvertorTo)
                  && (!funcs.m_fnGetPossible || pGetPossibleValues));

  if (pConvertorFrom)
    funcs.m_fnFromRB = pConvertorFrom;
  if (pConvertorTo)
    funcs.m_fnToRB = pConvertorTo;
  if (pGetPossibleValues)
    funcs.m_fnGetPossible = pGetPossibleValues;
}

void OdResBufPropControllerImpl::removeConvertor(const OdString& sConvertorName)
{
  ODA_ASSERT_ONCE(!sConvertorName.isEmpty());
  if (sConvertorName.isEmpty())
    return;

  m_mapCallbacks[sConvertorName] = CallBacks();
}

void OdResBufPropControllerImpl::addPath(const OdString& sPath, const OdString& sConvertorName)
{
  ODA_ASSERT_ONCE(!sPath.isEmpty() && !sConvertorName.isEmpty());
  if (sPath.isEmpty() || sConvertorName.isEmpty())
    return;

  m_mapConvertorNames[sPath] = sConvertorName;
}

TConvertorFromResBufPropValue* OdResBufPropControllerImpl::getConvFuncFromRB(const OdString& sPath)
{
  ODA_ASSERT_ONCE(!sPath.isEmpty());
  OdString sConvertorName = m_mapConvertorNames[sPath];
  ODA_ASSERT_ONCE(!sConvertorName.isEmpty());
  if (sConvertorName.isEmpty())
    return NULL;

  CallBacks& funcs = m_mapCallbacks[sConvertorName];
  ODA_ASSERT_ONCE(funcs.m_fnFromRB || funcs.m_fnToRB || funcs.m_fnGetPossible); 
  return funcs.m_fnFromRB;
}

TConvertorToResBufPropValue* OdResBufPropControllerImpl::getConvFuncToRB(const OdString& sPath)
{
  ODA_ASSERT_ONCE(!sPath.isEmpty());
  OdString sConvertorName = m_mapConvertorNames[sPath];
  if (sConvertorName.isEmpty())
    return NULL;

  CallBacks& funcs = m_mapCallbacks[sConvertorName];
  ODA_ASSERT_ONCE(funcs.m_fnFromRB || funcs.m_fnToRB || funcs.m_fnGetPossible); 
  return funcs.m_fnToRB;
}

TConvertorPossibleValues* OdResBufPropControllerImpl::getConvFuncGetPossible(const OdString& sPath)
{
  ODA_ASSERT_ONCE(!sPath.isEmpty());
  OdString sConvertorName = m_mapConvertorNames[sPath];
  if (sConvertorName.isEmpty())
    return NULL;

  CallBacks& funcs = m_mapCallbacks[sConvertorName];
  ODA_ASSERT_ONCE(funcs.m_fnFromRB || funcs.m_fnToRB || funcs.m_fnGetPossible); 
  return funcs.m_fnGetPossible;
}

OdString OdResBufPropControllerImpl::setCurrentPath(const OdString& sPath)
{
  OdString sSvPath = m_sCurrentPath;
  m_sCurrentPath = sPath;
  return sSvPath;
}
// for such args like scale of LINEAR convertor
void OdResBufPropControllerImpl::setConvertorAdvArg(const OdString&, // sName // TODO
                                                    const OdString& sValue)
{
  ODA_ASSERT_ONCE(!m_sCurrentPath.isEmpty());
  // TODO
  ODA_ASSERT_ONCE(   m_mapAdvArgsToSetValue[m_sCurrentPath].isEmpty()
                  || m_mapAdvArgsToSetValue[m_sCurrentPath] == sValue);

  m_mapAdvArgsToSetValue[m_sCurrentPath] = sValue;
}
OdString OdResBufPropControllerImpl::getConvertorAdvArg(const OdString&) // sName // TODO
{
  ODA_ASSERT_ONCE(!m_sCurrentPath.isEmpty());
  return m_mapAdvArgsToSetValue[m_sCurrentPath];
}

static OdSharedPtr<OdResBufPropControllerImpl> s_pController;

OdResBufPropControllerImpl& getResBufPropController()
{
  ODA_ASSERT_ONCE(!s_pController.isNull());
  return *s_pController.get();
}

// static
void OdResBufPropControllerImpl::init()
{
  ODA_ASSERT_ONCE(propServices() && s_pController.isNull());
  s_pController = new OdResBufPropControllerImpl();
  propServices()->setControllerOfResBufPropValueConvertors(s_pController);

  // (attension - L#convertor is correct for Windows only)
  #define ARG_true(convertor, prefix) prefix##convertor
  #define ARG_false(convertor, prefix) NULL
  #define OD_RB_VALUE_CONV(convertor, from, to, possible) \
    s_pController->addConvertor(OdString(#convertor), \
                                ARG_##from(convertor, convertorFromResBuf_), \
                                ARG_##to(convertor, convertorToResBuf_), \
                                ARG_##possible(convertor, convertorGetPossible_));

  #include "ResBufPropValueConvertors.h"

  #undef ARG_true
  #undef ARG_false
}

// static
void OdResBufPropControllerImpl::uninit()
{
  ODA_ASSERT_ONCE(propServices() && !s_pController.isNull());
  if (!s_pController.isNull())
  {
    #define OD_RB_VALUE_CONV(convertor, from, to, possible) \
      s_pController->removeConvertor(OdString(#convertor));

    #include "ResBufPropValueConvertors.h"
  }

  propServices()->setControllerOfResBufPropValueConvertors(NULL);
  s_pController = NULL;
}

//------------------------------------------------------------------------
