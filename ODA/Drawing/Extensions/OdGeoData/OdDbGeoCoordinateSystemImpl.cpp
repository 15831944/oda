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

#include "OdaCommon.h"
#include "OdDbGeoCoordinateSystemImpl.h"
#include "OdDToStr.h"

#include "tinyxml.h"

ODRX_CONS_DEFINE_MEMBERS(OdDbGeoCoordinateSystemCategoryImpl, OdDbGeoCoordinateSystemCategory, RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS(OdDbGeoCoordinateSystemImpl, OdDbGeoCoordinateSystem, RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS(OdDbGeoCoordinateSystemTransformerImpl, OdDbGeoCoordinateSystemTransformer, RXIMPL_CONSTR);

//////////////////////////////////////////////////////
//helper functions prototypes start

extern "C" struct cs_PrjprmMap_ cs_PrjprmMap [];

OdString getProjectionString(int prj_code);
OdString getParameterString(int prm_code); //for OdDbGeoCoordinateSystem::getXmlRepresentation()
OdString getParameterStringForArray(int prm_code); //for OdDbGeoCoordinateSystem::getProjectionParameters()
OdString getOtherParameterString(int prm_code);
void linkAlias(TiXmlElement * parent, OdInt16 id, const OdString & type, const OdString & ObjectId);
void linkDomainOfValidityLL(TiXmlElement* tagCS, double dWest, double dEast, double dSouth, double dNorth);
void linkDomainOfValidityXY(TiXmlElement* tagCS, double dWest, double dEast, double dSouth, double dNorth);
TiXmlElement * createBaseElement(const OdString & sTagName,
                                 const OdString & sId,
                                 const OdString & sName,
                                 const OdString & sDescription,
                                 const OdString & sRemarks,
                                 const OdString & sComment,
                                 const OdString & sAuthority);
TiXmlElement * createAxis(int order, const OdString & sName, const OdString & sAbbreviation, const OdString & sAxisDirection);
OdString getPrimeMeridianName(double org_lng);
TiXmlElement * addParameterValue(TiXmlElement * parent, int eParamCode, double dValue, const OdString & sUnit);
TiXmlElement * addParameterValue(TiXmlElement * parent, const OdString & sParamName, int eLogType, double dValue, const OdString & sUnit, int prec = 15);
void linkConversion(TiXmlElement* tagCS, const cs_Csprm_* cs);
TiXmlElement * getOperationMethod(struct cs_GxXform_* gxXForm);
TiXmlElement * createTransformationOperation(const OdString & sTrOpId, int eDirection);
TiXmlElement * createTransformation(TiXmlElement * dictionary, const OdString & sTransformationPath);
OdString gridFileFormat(int format);
TiXmlElement * addGridFileParam(TiXmlElement * parent, const OdString & sIdValue, int eDirection, const OdString & sFileName);
int getParameterLogicalType(int eParamCode);
TiXmlElement * addAdditionalParameter(TiXmlElement * parent, const OdString & sLabel, double dValue);
void addMulRegCoefficients(TiXmlElement * parent, double * arCoef, const OdChar letter);
OdResult createXml(const cs_Csprm_* cs, OdString& sResultName);
OdDbGeoCoordinateSystem::Unit getUnitByName(const OdString& sName);
void csUnitScaleToString(OdString& sUnitScale, double dUnitScale);

struct CsNameTypeRecord
{
  OdChar * sName;
  OdChar * sUnitType;
};

const CsNameTypeRecord arrCsNameTypeRecord[] =
{
  {L"XY-M",   L"Meter"},
  {L"XY-FT",  L"Foot"},
  {L"XY-IN",  L"Inch"},
  {L"XY-IFT", L"IFoot"},
  {L"XY-CF",  L"ClarkeFoot"},
  {L"XY-IIN", L"IInch"},
  {L"XY-UI",  L"MicroInch"},
  {L"XY-ML",  L"Mil"},
  {L"XY-CM",  L"Centimeter"},
  {L"XY-KM",  L"Kilometer"},
  {L"XY-YD",  L"Yard"},
  {L"XY-SY",  L"SearsYard"},
  {L"XY-MI",  L"Mile"},
  {L"XY-IYD", L"IYard"},
  {L"XY-IMI", L"IMile"},
  {L"XY-KT",  L"Knot"},
  {L"XY-NM",  L"NautM"},
  {L"XY-DM",  L"Decimeter"},
  {L"XY-MM",  L"Millimeter"},
  {L"XY-DAM", L"Decameter"},
  {L"XY-DK",  L"Dekameter"},
  {L"XY-HM",  L"Hectometer"},
  {L"XY-GM",  L"GermanMeter"},
  {L"XY-CG",  L"CaGrid"},
  {L"XY-GC",  L"GunterChain"},
  {L"XY-CC",  L"ClarkeChain"},
  {L"XY-BC",  L"BenoitChain"},
  {L"XY-SC",  L"SearsChain"},
  {L"XY-GL",  L"GunterLink"},
  {L"XY-CL",  L"ClarkeLink"},
  {L"XY-BL",  L"BenoitLink"},
  {L"XY-SL",  L"SearsLink"},
  {L"XY-RD",  L"Rod"},
  {L"XY-PE",  L"Perch"},
  {L"XY-PO",  L"Pole"},
  {L"XY-FU",  L"Furlong"},
  {L"XY-RO",  L"Rood"},
  {L"XY-CA",  L"CapeFoot"},
  {L"XY-BR",  L"Brealey"}
};

OdResult getLocalCsNameByType(OdString & sCS, const OdString & sUnitType)
{
  OdUInt32 nSize = sizeof(arrCsNameTypeRecord) / sizeof(arrCsNameTypeRecord[0]);
  for (OdUInt32 i = 0; i < nSize; ++i)
  {
    if (0 == sUnitType.iCompare(arrCsNameTypeRecord[i].sUnitType))
    {
      sCS = arrCsNameTypeRecord[i].sName;
      return eOk;
    }
  }

  sCS = arrCsNameTypeRecord[0].sName; //assign smt
  return eInvalidInput;
}

//helper functions prototypes end
//////////////////////////////////////////////////////

//OdDbGeoCoordinateSystemCategoryImpl
OdDbGeoCoordinateSystemCategoryImpl::OdDbGeoCoordinateSystemCategoryImpl()
{
}

OdDbGeoCoordinateSystemCategoryImpl::~OdDbGeoCoordinateSystemCategoryImpl()
{
}

OdResult OdDbGeoCoordinateSystemCategoryImpl::getId(OdString& categoryId) const
{
  categoryId = m_sCategoryId;
  return eOk;
}

OdResult OdDbGeoCoordinateSystemCategoryImpl::getNumOfCoordinateSystem(int& num) const
{
  num = CS_getItmNameCount(m_sCategoryId);
  return eOk;
}

OdResult OdDbGeoCoordinateSystemCategoryImpl::getCoordinateSystemAt(int index, OdDbGeoCoordinateSystemPtr& pCoordSys) const
{
  return OdDbGeoCoordinateSystem::create(CS_getItmName(m_sCategoryId, index), pCoordSys);
}

OdResult OdDbGeoCoordinateSystemCategoryImpl::createAll(OdArray<OdDbGeoCoordinateSystemCategoryPtr>& allCategories)
{
  allCategories.clear();
  struct cs_Ctdef_ **pDefArray;
  int count = CS_ctdefAll(&pDefArray);

  for (int i = 0; i < count; ++i)
  {
    OdDbGeoCoordinateSystemCategoryImplPtr pCategory = createObject();
    pCategory->m_sCategoryId = pDefArray[i]->ctName;
    allCategories.append(pCategory.get());

    CS_free(pDefArray[i]->csNames);
  }
  CS_free(pDefArray);
  return eOk;
}

//OdDbGeoCoordinateSystemImpl
OdDbGeoCoordinateSystemImpl::OdDbGeoCoordinateSystemImpl()
  : m_cs(NULL)
{
}

OdDbGeoCoordinateSystemImpl::~OdDbGeoCoordinateSystemImpl()
{
  if (m_cs)
  {
    CS_free(m_cs);
  }
}

OdResult OdDbGeoCoordinateSystemImpl::getId(OdString& coordSysId) const
{
  coordSysId = m_cs->csdef.key_nm;
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::getEpsgCode(int& epsgCode) const
{
  epsgCode = m_cs->csdef.epsgNbr;
  return eOk;
}


OdDbGeoCoordinateSystem::Type getCSTypeByProjCode(OdDbGeoCoordinateSystem::ProjectionCode code)
{
  OdDbGeoCoordinateSystem::Type type;
  if (code)
  {
    if (OdDbGeoCoordinateSystem::kProjectionCodeNerth == code || OdDbGeoCoordinateSystem::kProjectionCodeNrthsrt == code) // 55 || 64
    {
      type = OdDbGeoCoordinateSystem::kTypeArbitrary;
    }
    else if (OdDbGeoCoordinateSystem::kProjectionCodeLL == code) // 1
    {
      type = OdDbGeoCoordinateSystem::kTypeGeographic;
    }
    else
    {
      type = OdDbGeoCoordinateSystem::kTypeProjected;
    }
  }
  else
  {
    type = OdDbGeoCoordinateSystem::kTypeUnknown;
  }
  return type;
}

OdResult OdDbGeoCoordinateSystemImpl::getType(OdDbGeoCoordinateSystem::Type& type) const
{
  type = getCSTypeByProjCode(OdDbGeoCoordinateSystem::ProjectionCode(m_cs->prj_code));
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::getDescription(OdString& coordSysDesc) const
{
  coordSysDesc = m_cs->csdef.desc_nm;
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::getUnit(OdDb::UnitsValue& unit) const
{
  Unit geoUnit = getUnitByName(m_cs->csdef.unit);
  switch (geoUnit)
  {
  case kUnitMeter:
    unit = OdDb::kUnitsMeters; break;
  case kUnitFoot:
    unit = OdDb::kUnitsFeet; break;
  case kUnitMile:
    unit = OdDb::kUnitsMiles; break;
  case kUnitCentimeter:
    unit = OdDb::kUnitsCentimeters; break;
  case kUnitDekameter:
    unit = OdDb::kUnitsDekameters; break;
  case kUnitDecimeter:
    unit = OdDb::kUnitsDecimeters; break;
  case kUnitHectometer:
    unit = OdDb::kUnitsHectometers; break;
  case kUnitInch:
    unit = OdDb::kUnitsInches; break;
  case kUnitKilometer:
    unit = OdDb::kUnitsKilometers; break;
  case kUnitMil:
    unit = OdDb::kUnitsMils; break;
  case kUnitMillimeter:
    unit = OdDb::kUnitsMillimeters; break;
  case kUnitYard:
    unit = OdDb::kUnitsYards; break;
  default:
    unit = OdDb::kUnitsUndefined; break;
  }
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::getUnit(OdDbGeoCoordinateSystem::Unit& unit) const
{
  unit = getUnitByName(m_cs->csdef.unit);
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::getUnitScale(double& unitScale) const
{
  unitScale = m_cs->csdef.unit_scl;
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::getProjectionCode(OdDbGeoCoordinateSystem::ProjectionCode& prjCode) const
{
  prjCode = OdDbGeoCoordinateSystem::ProjectionCode(m_cs->prj_code);
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::getProjectionParameters(OdArray<OdDbGeoProjectionParameter>& prjParams, bool includeSpecialParams) const
{
  prjParams.clear();
  struct cs_Csdef_* cs_def = &(m_cs->csdef);
  switch (m_cs->prj_code)
  {
    case cs_PRJCOD_MSTRO: //xml order applied
    {
      for (int i = 1; i < 24; i += 2)
      {
        {
          OdDbGeoProjectionParameter* pPrjParam = prjParams.append();
          pPrjParam->name = OdString().format(OD_T("Complex Series Coefficient A%d"), i / 2 + 1);
          pPrjParam->value = CS_prmValue(cs_def, i);
        }
        {
          OdDbGeoProjectionParameter* pPrjParam = prjParams.append();
          pPrjParam->name = OdString().format(OD_T("Complex Series Coefficient B%d"), i / 2 + 1);
          pPrjParam->value = CS_prmValue(cs_def, i + 1);
        }
      }
      break;
    }
  default:
    {
      struct cs_PrjprmMap_* mp;
      for (mp = cs_PrjprmMap; mp->prj_code != cs_PRJCOD_END; ++mp)
      {
        if (mp->prj_code == m_cs->prj_code) break;
      }
    
      for (int i = 0; i < 24; ++i)
      {
        int parm_idx = mp->prm_types[i];
        if (parm_idx <= 0)
        {
          break;
        }
        OdDbGeoProjectionParameter* pPrjParam = prjParams.append();
        pPrjParam->name = getParameterStringForArray(parm_idx);
        pPrjParam->value = CS_prmValue(cs_def, i + 1);
      }
      break;
    }
  }

  if (includeSpecialParams)
  {
    ulong32_t prjFlg = m_cs->prj_flags;

    if ((prjFlg & cs_PRJFLG_ORGLNG) == 0)
    {
      OdDbGeoProjectionParameter* pPrjParam = prjParams.append();
      pPrjParam->name = getParameterStringForArray(cs_WKTCOD_ORGLNG);
      pPrjParam->value = cs_def->org_lng;
    }

    if ((prjFlg & cs_PRJFLG_ORGLAT) == 0)
    {
      OdDbGeoProjectionParameter* pPrjParam = prjParams.append();
      pPrjParam->name = getParameterStringForArray(cs_WKTCOD_ORGLAT);
      pPrjParam->value = cs_def->org_lat;
    }

    if ((prjFlg & cs_PRJFLG_SCLRED) != 0)
    {
      OdDbGeoProjectionParameter* pPrjParam = prjParams.append();
      pPrjParam->name = getParameterStringForArray(cs_WKTCOD_SCLRED);
      pPrjParam->value = cs_def->scl_red;
    }

    if ((prjFlg & cs_PRJFLG_ORGFLS) == 0)
    {
      {
        OdDbGeoProjectionParameter* pPrjParam = prjParams.append();
        pPrjParam->name = getParameterStringForArray(cs_WKTCOD_FEAST);
        pPrjParam->value = cs_def->x_off;
      }
      {
        OdDbGeoProjectionParameter* pPrjParam = prjParams.append();
        pPrjParam->name = getParameterStringForArray(cs_WKTCOD_FNORTH);
        pPrjParam->value = cs_def->y_off;
      }
    }
  }
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::getDatum(OdDbGeoDatum& datum) const
{
  datum.desc = m_cs->datum.dt_name;
  datum.id = m_cs->datum.key_nm;
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::getEllipsoid(OdDbGeoEllipsoid& ellipsoid) const
{
  cs_Eldef_ elDef;
  if (CS_getel(m_cs->datum.ell_knm, &elDef) == 0)
  {
    ellipsoid.desc = elDef.name;
    ellipsoid.eccentricity = elDef.ecent;
    ellipsoid.id = elDef.key_nm;
    ellipsoid.polarRadius = elDef.p_rad;
    return eOk;
  }
  return eNotApplicable;
}

OdResult OdDbGeoCoordinateSystemImpl::getOffset(OdGeVector2d& offsetVec) const
{
  offsetVec.set(m_cs->csdef.x_off, m_cs->csdef.y_off);
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::getCartesianExtents(OdGeExtents2d& exts) const
{
  exts.set(
    OdGePoint2d(m_cs->min_xy[0], m_cs->min_xy[1]),
    OdGePoint2d(m_cs->max_xy[0], m_cs->max_xy[1])
  );
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::getGeodeticExtents(OdGeExtents2d& exts) const
{
  exts.set(
    OdGePoint2d(m_cs->min_ll[0] + m_cs->cent_mer, m_cs->min_ll[1]),
    OdGePoint2d(m_cs->max_ll[0] + m_cs->cent_mer, m_cs->max_ll[1])
  );
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::getXmlRepresentation(OdString& strXml) const
{
  return createXml(m_cs, strXml);
}

OdResult OdDbGeoCoordinateSystemImpl::getWktRepresentation(OdString& strWkt) const
{
  OdDbGeoCoordinateSystem::ProjectionCode prjCode = OdDbGeoCoordinateSystem::ProjectionCode(m_cs->prj_code);
  OdDbGeoCoordinateSystem::Type eCSType = getCSTypeByProjCode(prjCode);

  if (kTypeArbitrary == eCSType)
  {
    OdString sUnitName = m_cs->csdef.unit;
    OdString sUnitScale;
    csUnitScaleToString(sUnitScale, m_cs->csdef.unit_scl);
    strWkt =
      L"LOCAL_CS[\"Non-Earth ("
      + sUnitName
      + L")\", LOCAL_DATUM[\"Local Datum\", 0], UNIT[\""
      + sUnitName
      + L"\", "
      + sUnitScale
      + L"], AXIS[\"X\", EAST], AXIS[\"Y\", NORTH]]";
  }
  else
  {
    //with wktFlvrOgc we got same wkt as in arx, but not sure that it's 100% correct
    char bufr[2048];
    if (-1 != CS_cs2Wkt(bufr, sizeof(bufr), m_cs->csdef.key_nm, wktFlvrOgc))
    {
      strWkt = bufr;
    }
    else
    {
      //For some CS, e.g. "AKMS27", arx can't create WKT, but can create XML:
      //ODA_ASSERT(!"Can't create WKT CS representation");
      strWkt.empty();
      return eNotApplicable;
    }
  }
  return eOk;
}

//OdDbGeoCoordinateSystemImpl static methods
OdResult OdDbGeoCoordinateSystemImpl::create(const OdString& coordSysIdOrFullDef, OdDbGeoCoordinateSystemPtr& pCoordSys)
{
  OdDbGeoCoordinateSystemImplPtr pCs = createObject();
  
  cs_Csprm_* & cs = pCs->m_cs;

  // Parse id from xml, we take only csId.
  // it's not the same behavior as in arx, but they do it in a strange way,
  // and maybe they got not what they suppose...
  TiXmlDocument doc;
  doc.Parse(coordSysIdOrFullDef, 0, TIXML_ENCODING_UTF8);
  TiXmlHandle docHandle(&doc);
  TiXmlElement* xmlCs = docHandle.FirstChild("Dictionary").FirstChild().ToElement();
  if (xmlCs != NULL)
  {
    OdString sId = xmlCs->Attribute("id");
    cs = CS_csloc(sId);
  }

  if (NULL == cs && 1 == CS_isWkt(coordSysIdOrFullDef))
  {
    cs_Csdef_ csDef;
    cs_Dtdef_ dtDef;
    cs_Eldef_ elDef;
    if (-1 != coordSysIdOrFullDef.find(L"LOCAL_CS") || -1 != coordSysIdOrFullDef.find(L"LOCALCS"))
    {
      int nUnitTagPos = coordSysIdOrFullDef.find(L"UNIT");
      if (-1 != nUnitTagPos)
      {
        int nStartPos = coordSysIdOrFullDef.find('\"', nUnitTagPos);
        if (-1 != nStartPos)
        {
          int nEndPos = coordSysIdOrFullDef.find('\"', nStartPos + 1);
          if (-1 != nEndPos)
          {
            OdString sUnitType = coordSysIdOrFullDef.mid(nStartPos + 1, nEndPos - nStartPos - 1);
            OdString sId;
            ODA_VERIFY(eOk == getLocalCsNameByType(sId, sUnitType));
            cs = CS_csloc(sId);
          }
        }
      }
    }
    else
    {
      bool bWktOk = true;
      if (CS_wktToCsEx(&csDef, &dtDef, &elDef, wktFlvrNone, coordSysIdOrFullDef, TRUE))
      {
        if (CS_wktToCsEx(&csDef, &dtDef, &elDef, wktFlvrOgc, coordSysIdOrFullDef, TRUE))
        {
          if (CS_wktToCsEx(&csDef, &dtDef, &elDef, wktFlvrEsri, coordSysIdOrFullDef, TRUE))
          {
            if (CS_wktToCsEx(&csDef, &dtDef, &elDef, wktFlvrOracle, coordSysIdOrFullDef, TRUE))
            {
              if (CS_wktToCsEx(&csDef, &dtDef, &elDef, wktFlvrGeoTiff, coordSysIdOrFullDef, TRUE))
              {
                if (CS_wktToCsEx(&csDef, &dtDef, &elDef, wktFlvrGeoTools, coordSysIdOrFullDef, TRUE))
                {
                  if (CS_wktToCsEx(&csDef, &dtDef, &elDef, wktFlvrEpsg, coordSysIdOrFullDef, TRUE))
                  {
                    bWktOk = false;
                  }
                }
              }
            }
          }
        }
      }
      if (bWktOk)
      {
        cs = CS_csloc(csDef.key_nm);
      }
      else
      {
        //arx can't create CS from some of its own WKT, e.g. "T - EDCYLE" WKT, 5819.txt in GeoData test
        //ODA_ASSERT(!"Can't create CS by WKT");
        return eNoDatabase;
      }
    }
  }

  if (NULL == cs && 1 == CS_csIsValid(coordSysIdOrFullDef))
  {
    cs = CS_csloc(coordSysIdOrFullDef);
  }

  if (NULL == cs)
  {
    return eNoDatabase;
  }

  pCoordSys = pCs.get();
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::createAll(const OdGePoint3d& geoPt, OdArray<OdDbGeoCoordinateSystemPtr>& allCoordSys)
{
  allCoordSys.clear();
  cs_Csdef_** pDefArray;
  int csCount = CS_csdefAll(&pDefArray);
  allCoordSys.reserve(csCount);
  for (int i = 0; i < csCount; ++i)
  {
    cs_Csdef_* cs = pDefArray[i];
    OdDbGeoCoordinateSystemPtr pObj;
    OdDbGeoCoordinateSystem::create(cs->key_nm, pObj);
    OdDbGeoCoordinateSystem::Type type;
    pObj->getType(type);

    OdString sId;
    pObj->getId(sId);

    if (OdDbGeoCoordinateSystem::kTypeProjected == type)
    {
      OdGeExtents2d ext;
      pObj->getGeodeticExtents(ext);
      //even if one of extents ordinate is invalid we must check second
      bool bAppendCS = true;
      if (ext.minPoint().x <= ext.maxPoint().x)
      {
        bAppendCS &= (geoPt.x >= ext.minPoint().x && geoPt.x <= ext.maxPoint().x);
      }
      if (ext.minPoint().y <= ext.maxPoint().y)
      {
        bAppendCS &= (geoPt.y >= ext.minPoint().y && geoPt.y <= ext.maxPoint().y);
      }
      if (bAppendCS)
      {
        allCoordSys.append(pObj);
      }
    }
  }

  CS_free(pDefArray);
  return eOk;
}

OdResult OdDbGeoCoordinateSystemImpl::createAll(OdArray<OdDbGeoCoordinateSystemPtr>& allCoordSys, const OdDbGeoCoordinateSystemCategory* pCategory)
{
  allCoordSys.clear();
  if (pCategory == NULL)
  {
    struct cs_Csdef_ **pDefArray;
    int csCount = CS_csdefAll(&pDefArray);
    allCoordSys.reserve(csCount);
    for(int i = 0; i < csCount; ++i)
    {
      OdDbGeoCoordinateSystemPtr pObj;
      OdDbGeoCoordinateSystem::create(pDefArray[i]->key_nm, pObj);
      allCoordSys.append(pObj);
    }

    CS_free(pDefArray);
  }
  else
  {
    int csCount = 0;
    pCategory->getNumOfCoordinateSystem(csCount);
    allCoordSys.reserve(csCount);
    for(int csIndex = 0; csIndex < csCount; ++csIndex)
    {
      OdDbGeoCoordinateSystemPtr pCs;
      pCategory->getCoordinateSystemAt(csIndex, pCs);
      allCoordSys.append(pCs);
    }
  }

  return eOk;
}


//OdDbGeoCoordinateSystemTransformerImpl
OdResult parseCSId(const OdString& sourceCoordSysId, const OdString& targetCoordSysId, OdString& sResSourceCoordSysId, OdString& sResTargetCoordSysId)
{
  OdDbGeoCoordinateSystemPtr pCSSource;
  OdDbGeoCoordinateSystemPtr pCSTarget;

  if (eOk != OdDbGeoCoordinateSystem::create(sourceCoordSysId, pCSSource)
    || eOk != OdDbGeoCoordinateSystem::create(targetCoordSysId, pCSTarget)
    || eOk != pCSSource->getId(sResSourceCoordSysId)
    || eOk != pCSTarget->getId(sResTargetCoordSysId)
    )
  {
    return eNotApplicable;
  }
  return eOk;
}

OdDbGeoCoordinateSystemTransformerImpl::OdDbGeoCoordinateSystemTransformerImpl()
  : m_bSameCS(false)
  , m_pSrc(NULL)
  , m_pDst(NULL)
  , m_pDtc(NULL)
{
}

OdDbGeoCoordinateSystemTransformerImpl::~OdDbGeoCoordinateSystemTransformerImpl()
{
  if (m_pSrc)
  {
    CS_free(m_pSrc);
  }
  if (m_pDst)
  {
    CS_free(m_pDst);
  }
  if (m_pDtc)
  {
    CS_dtcls(m_pDtc);
  }
}

OdResult OdDbGeoCoordinateSystemTransformerImpl::getSourceCoordinateSystemId(OdString& sourceCoordSysId) const
{
  sourceCoordSysId = m_pSrc->csdef.key_nm;
  return eOk;
}

OdResult OdDbGeoCoordinateSystemTransformerImpl::getTargetCoordinateSystemId(OdString& targetCoordSysId) const
{
  targetCoordSysId = m_pDst->csdef.key_nm;
  return eOk;
}

OdResult OdDbGeoCoordinateSystemTransformerImpl::transformPoint(const OdGePoint3d& pointIn, OdGePoint3d& pointOut) const
{
  if (m_bSameCS)
  {
    pointOut = pointIn;
    return eOk;
  }

  double ptXYZ[3] = { pointIn.x, pointIn.y, pointIn.z };
  double ptLLA[3] = { 0., 0., 0. };

  //transformation:
  int st1 = CS_cs3ll(m_pSrc, ptLLA, ptXYZ);
  double dZ = ptLLA[2]; //store Z to reassign
  ptLLA[2] = ptXYZ[2];
  int st2 = CS_dtcvt3D(m_pDtc, ptLLA, ptLLA);
  ptXYZ[2] = ptLLA[2];
  ptLLA[2] = dZ;
  int st3 = CS_ll3cs(m_pDst, ptXYZ, ptLLA);

  pointOut.x = ptXYZ[0];
  pointOut.y = ptXYZ[1];
  pointOut.z = ptXYZ[2];

  if (st2 >= 0)
  {
    if (st1 != cs_CNVRT_NRML || st3 != cs_CNVRT_NRML)
    {
      return eOutOfRange;
    }
    else if (st2 != cs_CNVRT_NRML)
    {
      return eOk;
    }
  }
  else
  {
    return eNotApplicable;
  }

  return eOk;
}

OdResult OdDbGeoCoordinateSystemTransformerImpl::transformPoints(const OdGePoint3dArray& pointsIn, OdGePoint3dArray& pointsOut) const
{
  if (m_bSameCS)
  {
    pointsOut = pointsIn;
    return eOk;
  }

  pointsOut.resize(pointsIn.size());
  for (OdUInt32 i = 0; i < pointsIn.size(); ++i)
  {
    OdResult status = transformPoint(pointsIn[i], pointsOut[i]);
    if (status != eOk)
    {
      return status;
    }
  }
  return eOk;
}

//OdDbGeoCoordinateSystemTransformerImpl static methods
OdResult OdDbGeoCoordinateSystemTransformerImpl::transformPoint(const OdString& sourceCoordSysId,
                                                                const OdString& targetCoordSysId,
                                                                const OdGePoint3d& pointIn,
                                                                OdGePoint3d& pointOut)
{
  OdDbGeoCoordinateSystemTransformerPtr pObj;
  OdResult status = OdDbGeoCoordinateSystemTransformer::create(sourceCoordSysId, targetCoordSysId, pObj);
  if (status != eOk)
  {
    return status;
  }
  return pObj->transformPoint(pointIn, pointOut);
}

OdResult OdDbGeoCoordinateSystemTransformerImpl::transformPoints(const OdString& sourceCoordSysId,
                                                                 const OdString& targetCoordSysId,
                                                                 const OdGePoint3dArray& pointsIn,
                                                                 OdGePoint3dArray& pointsOut)
{
  OdDbGeoCoordinateSystemTransformerPtr pObj;
  OdResult status = OdDbGeoCoordinateSystemTransformer::create(sourceCoordSysId, targetCoordSysId, pObj);
  if (status != eOk)
  {
    return status;
  }
  return pObj->transformPoints(pointsIn, pointsOut);
}

OdResult OdDbGeoCoordinateSystemTransformerImpl::create(const OdString& sourceCoordSysId, 
                                                        const OdString& targetCoordSysId,
                                                        OdDbGeoCoordinateSystemTransformerPtr& pCoordSysTransformer)
{
  OdDbGeoCoordinateSystemTransformerImplPtr pObj = createObject();

  OdString sResSourceCoordSysId;
  OdString sResTargetCoordSysId;

  OdResult res = parseCSId(sourceCoordSysId, targetCoordSysId, sResSourceCoordSysId, sResTargetCoordSysId);
  if (res != eOk)
  {
    return res;
  }

  //init CS data:
  pObj->m_pSrc = CS_csloc(sResSourceCoordSysId);
  pObj->m_pDst = CS_csloc(sResTargetCoordSysId);
  if (pObj->m_pSrc == NULL || pObj->m_pDst == NULL)
  {
    return eNullPtr;
  }

  pObj->m_pDtc = CS_dtcsu(pObj->m_pSrc, pObj->m_pDst, cs_DTCFLG_DAT_F, cs_DTCFLG_BLK_W);
  if (pObj->m_pDtc == NULL)
  {
    return eNullPtr;
  }

  pObj->m_bSameCS = 0 == CS_csDefCmp(&pObj->m_pSrc->csdef, &pObj->m_pDst->csdef, NULL, 0);

  pCoordSysTransformer = pObj;
  return eOk;
}

//////////////////////////////////////////////////////////////
// helper functions

OdString getProjectionString(int prj_code)
{
  switch (prj_code)
  {
  case OdDbGeoCoordinateSystem::kProjectionCodeUnknown:
    return "";
  case OdDbGeoCoordinateSystem::kProjectionCodeAlber:
    return "Albers Equal Area";
  case OdDbGeoCoordinateSystem::kProjectionCodeAzede:
    return "Lambert Azimuthal Equidistant, Elevated Ellipsoid";
  case OdDbGeoCoordinateSystem::kProjectionCodeAzmea:
    return "Lambert Azimuthal Equal Area";
  case OdDbGeoCoordinateSystem::kProjectionCodeAzmed:
    return "Lambert Azimuthal Equidistant";
  case OdDbGeoCoordinateSystem::kProjectionCodeBipolar:
    return "Bipolar Oblique Conformal Conic";
  case OdDbGeoCoordinateSystem::kProjectionCodeBonne:
    return "Bonne";
  case OdDbGeoCoordinateSystem::kProjectionCodeCassini:
    return "Cassini-Soldner";
  case OdDbGeoCoordinateSystem::kProjectionCodeEckert4:
    return "Ekert Pseudocylindrical, Number IV";
  case OdDbGeoCoordinateSystem::kProjectionCodeEckert6:
    return "Ekert Pseudocylindrical, Number VI";
  case OdDbGeoCoordinateSystem::kProjectionCodeEdcnc:
    return "Equidistant Conic";
  case OdDbGeoCoordinateSystem::kProjectionCodeEdcyl:
    return "Equidistant Cylindrical";
  case OdDbGeoCoordinateSystem::kProjectionCodeGaussK:
    return "Gauss Kruger";
  case OdDbGeoCoordinateSystem::kProjectionCodeGnomonic:
    return "Gnomonic";
  case OdDbGeoCoordinateSystem::kProjectionCodeGoode:
    return "Goode Homolosine";
  case OdDbGeoCoordinateSystem::kProjectionCodeHom1uv:
    return "Unrectified Hotine Oblique Mercator";
  case OdDbGeoCoordinateSystem::kProjectionCodeHom1xy:
    return "Hotine Oblique Mercator (variant A)";
  case OdDbGeoCoordinateSystem::kProjectionCodeHom2uv:
    return "Unrectified Hotine Oblique Mercator, Two Point Form";
  case OdDbGeoCoordinateSystem::kProjectionCodeHom2xy:
    return "Rectified Skew Orthomorphic, Two Point Form";
  case OdDbGeoCoordinateSystem::kProjectionCodeKrovak:
    return "Krovak";
  case OdDbGeoCoordinateSystem::kProjectionCodeKrvk95:
    return "Krovak Oblique Conformal Conic/95 Adjustment";
  case OdDbGeoCoordinateSystem::kProjectionCodeLL:
    return "Latitude, Longitude";
  case OdDbGeoCoordinateSystem::kProjectionCodeLm1sp:
    return "Lambert Conic Conformal (1SP)";
  case OdDbGeoCoordinateSystem::kProjectionCodeLm2sp:
    return "Lambert Conic Conformal (2SP)";
  case OdDbGeoCoordinateSystem::kProjectionCodeLmblg:
    return "Lambert Conic Conformal (2SP Belgium)";
  case OdDbGeoCoordinateSystem::kProjectionCodeLmtan:
    return "Lambert Tangential Conformal Conic";
  case OdDbGeoCoordinateSystem::kProjectionCodeMiller:
    return "Miller Cylindrical";
  case OdDbGeoCoordinateSystem::kProjectionCodeMndotl:
    return "Lambert Conformal Conic, Minnesota DOT Variation";
  case OdDbGeoCoordinateSystem::kProjectionCodeMndott:
    return "Transverse Mercator, Minnesota DOT Variation";
  case OdDbGeoCoordinateSystem::kProjectionCodeModpc:
    return "Lallemand IMW Modified Polyconic";
  case OdDbGeoCoordinateSystem::kProjectionCodeMollweid:
    return "Mollweide";
  case OdDbGeoCoordinateSystem::kProjectionCodeMrcat:
    return "Mercator (variant B)";
  case OdDbGeoCoordinateSystem::kProjectionCodeMrcatK:
    return "Mercator (variant A)";
  case OdDbGeoCoordinateSystem::kProjectionCodeMstero:
    return "Modified Stereographic";
  case OdDbGeoCoordinateSystem::kProjectionCodeNeacyl:
    return "Normal Aspect, Equal Area Cylindrical";
  case OdDbGeoCoordinateSystem::kProjectionCodeNerth:
    return "Non-georeferenced";
  case OdDbGeoCoordinateSystem::kProjectionCodeNzealand:
    return "New Zealand Map Grid";
  case OdDbGeoCoordinateSystem::kProjectionCodeOblqM:
    return "Oblique Cylindrical";
  case OdDbGeoCoordinateSystem::kProjectionCodeObqcyl:
    return "Oblique Cylindrical";
  case OdDbGeoCoordinateSystem::kProjectionCodeOrtho:
    return "Orthographic";
  case OdDbGeoCoordinateSystem::kProjectionCodeOstn02:
    return "Ordnance Survey National Grid Transformation of 2002";
  case OdDbGeoCoordinateSystem::kProjectionCodeOstn97:
    return "Ordnance Survey National Grid Transformation of 1997";
  case OdDbGeoCoordinateSystem::kProjectionCodeOstro:
    return "Oblique Stereographic";
  case OdDbGeoCoordinateSystem::kProjectionCodePlycn:
    return "American Polyconic";
  case OdDbGeoCoordinateSystem::kProjectionCodePstro:
    return "Polar Stereographic (variant A)";
  case OdDbGeoCoordinateSystem::kProjectionCodePstrosl:
    return "Polar Stereographic (variant B)";
  case OdDbGeoCoordinateSystem::kProjectionCodeRskew:
    return "Hotine Oblique Mercator (variant B)";
  case OdDbGeoCoordinateSystem::kProjectionCodeRskewc:
    return "Rectified Skew Orthomorphic, False Origin and Azimuth at Center";
  case OdDbGeoCoordinateSystem::kProjectionCodeRskewo:
    return "Rectified Skew Orthomorphic, Skew Azimuth at Rectified Origin";
  case OdDbGeoCoordinateSystem::kProjectionCodeRobinson:
    return "Robinson Cylindrical";
  case OdDbGeoCoordinateSystem::kProjectionCodeSinus:
    return "Sinusoidal";
  case OdDbGeoCoordinateSystem::kProjectionCodeSotrm:
    return "Transverse Mercator (South Orientated)";
  case OdDbGeoCoordinateSystem::kProjectionCodeSstro:
    return "Oblique Sterographic, per Snyder";
  case OdDbGeoCoordinateSystem::kProjectionCodeSwiss:
    return "Swiss Oblique Cylindrical";
  case OdDbGeoCoordinateSystem::kProjectionCodeSys34:
    return "Danish System 34 (pre-1999)";
  case OdDbGeoCoordinateSystem::kProjectionCodeSys34_99:
    return "Danish System 34 (1999)";
  case OdDbGeoCoordinateSystem::kProjectionCodeTeacyl:
    return "Transverse Aspect, Equal Area Cylindrical";
  case OdDbGeoCoordinateSystem::kProjectionCodeTm:
    return "Transverse Mercator";
  case OdDbGeoCoordinateSystem::kProjectionCodeTrmeraf:
    return "Transverse Mercator with Affine Post Process";
  case OdDbGeoCoordinateSystem::kProjectionCodeTrmrkrg:
    return "Transverse Mercator, Kruger Formulation";
  case OdDbGeoCoordinateSystem::kProjectionCodeTrmrs:
    return "Transverse Mercator per J. P. Snyder";
  case OdDbGeoCoordinateSystem::kProjectionCodeVdgrntn:
    return "Van Der Grinten";
  case OdDbGeoCoordinateSystem::kProjectionCodeWccsl:
    return "Lambert Conformal Conic, Wisconsin County Variation";
  case OdDbGeoCoordinateSystem::kProjectionCodeWccst:
    return "Transverse Mercator, Wisconsin County Variation";
  case OdDbGeoCoordinateSystem::kProjectionCodeUtm:
    return "Transverse Mercator Zoned Grid System";
  case OdDbGeoCoordinateSystem::kProjectionCodeWinkl:
    return "Winkel-Tripel, variable standard latitude";
  case OdDbGeoCoordinateSystem::kProjectionCodeNrthsrt:
    return "Non-georeferenced with scale and rotation";
  case OdDbGeoCoordinateSystem::kProjectionCodeLmbrtaf:
    return "Lambert Conformal Conic (2SP) with Affine Post Process";
  case OdDbGeoCoordinateSystem::kProjectionCodeSys34_01:
    return "Danish System 34 (2001)";
  case OdDbGeoCoordinateSystem::kProjectionCodeEdcylE:
    return "Equidistant Cylindrical Projection (Ellipsoidal or Spherical)";
  case OdDbGeoCoordinateSystem::kProjectionCodePlateCarree:
    return "Plate Carree / Simple Cylindrical";
  case OdDbGeoCoordinateSystem::kProjectionCodePvMercator:
    return "Popular Visualisation Pseudo Mercator";
    //case cs_PRJCOD_LMMICH:
  case OdDbGeoCoordinateSystem::kProjectionCodeLmMich:
    return "Lambert Conic Conformal (2SP Michigan)";
  default:
    return "";
  }
}

OdString getParameterStringForArray(int prm_code)
{
  switch (prm_code)
  {
  case cs_WKTCOD_FEAST:
    return "False Easting";
  case cs_WKTCOD_FNORTH:
    return "False Northing";
  case cs_WKTCOD_ORGLNG:
    return "Origin Longitude";
  case cs_WKTCOD_ORGLAT:
    return "Origin Latitude";
  case cs_WKTCOD_SCLRED:
    return "Scale Reduction";
  default:
    return getOtherParameterString(prm_code);
  }
}


OdString getParameterString(int prm_code)
{
  switch (prm_code)
  {
  case cs_WKTCOD_FEAST:
    return "False easting";
  case cs_WKTCOD_FNORTH:
    return "False northing";
  case cs_WKTCOD_ORGLNG:
    return "Longitude of false origin";
  case cs_WKTCOD_ORGLAT:
    return "Latitude of false origin";
  case cs_WKTCOD_SCLRED:
    return "Scaling factor for coord differences";
  default:
    return getOtherParameterString(prm_code);
  }
}


OdString getOtherParameterString(int prm_code)
{
  switch (prm_code)
  {
  case cs_PRMCOD_NOTUSED:
    return "";
  case cs_PRMCOD_CNTMER:
    return "Longitude of natural origin";
  case cs_PRMCOD_NSTDPLL:
    return "Latitude of 1st standard parallel";
  case cs_PRMCOD_SSTDPLL:
    return "Latitude of 2nd standard parallel";
  case cs_PRMCOD_STDPLL:
    return "Standard Parallel";
  case cs_PRMCOD_GCP1LNG:
    return "First Point Longitude";
  case cs_PRMCOD_GCP1LAT:
    return "First Point Latitude";
  case cs_PRMCOD_GCP2LNG:
    return "Second Point Longitude";
  case cs_PRMCOD_GCP2LAT:
    return "Second Point Latitude";
  case cs_PRMCOD_GCPLNG:
    return "Longitude of projection center";
  case cs_PRMCOD_GCPLAT:
    return "Latitude of projection center";
  case cs_PRMCOD_GCAZM:
    return "Azimuth of initial line";
  case cs_PRMCOD_YAXISAZ:
    return "Y Axis Azimuth";
  case cs_PRMCOD_ESTDMER:
    return "Eastern Standard Meridian";
  case cs_PRMCOD_NPARALL:
    return "Northern Parallel";
  case cs_PRMCOD_SPARALL:
    return "Southern Parallel";
  case cs_PRMCOD_P1LNG:
    return "First Pole Longitude";
  case cs_PRMCOD_P1LAT:
    return "First Pole Latitude";
  case cs_PRMCOD_P2LNG:
    return "Second Pole Longitude";
  case cs_PRMCOD_P2LAT:
    return "Second Pole Latitude";
  case cs_PRMCOD_ADP1P2:
    return "Distance Between Poles";
  case cs_PRMCOD_ADSP1:
    return "Distance to First Standard Parallel";
  case cs_PRMCOD_ADSP2:
    return "Distance to Second Standard Parallel";

  case cs_PRMCOD_CMPLXAN:
    return "Complex Series Coefficient A%d";
  case cs_PRMCOD_CMPLXBN:
    return "Complex Series Coefficient B%d";

  case cs_PRMCOD_WESTLL:
    return "Western End of Longitude Range";
  case cs_PRMCOD_EASTLL:
    return "Eastern End of Longitude Range";
  case cs_PRMCOD_UTMZN:
    return "UTM Zone Number";
  case cs_PRMCOD_HSNS:
    return "Hemisphere, North or South";
  case cs_PRMCOD_GHGT:
    return "Average Geoid Height";
  case cs_PRMCOD_AELEV:
    return "Average Elevation";
  case cs_PRMCOD_POLELNG:
    return "Oblique Pole Longitude";
  case cs_PRMCOD_POLELAT:
    return "Oblique Pole Latitude";
  case cs_PRMCOD_OSTDPLL:
    return "Latitude of pseudo standard parallel";
  case cs_PRMCOD_STDCIR:
    return "Latitude of standard parallel";
  case cs_PRMCOD_AFA0:
    return "Affine Transformation A0 Coefficient";
  case cs_PRMCOD_AFB0:
    return "Affine Transformation B0 Coefficient";
  case cs_PRMCOD_AFA1:
    return "Affine Transformation A1 Coefficient";
  case cs_PRMCOD_AFA2:
    return "Affine Transformation A2 Coefficient";
  case cs_PRMCOD_AFB1:
    return "Affine Transformation B1 Coefficient";
  case cs_PRMCOD_AFB2:
    return "Affine Transformation B2 Coefficient";
  case cs_PRMCOD_NRMLPLL:
    return "Normal Parallel";
  case cs_PRMCOD_DENRGN:
    return "Danish Region";
  case cs_PRMCOD_SKWAZM:
    return "Angle from Rectified to Skew Grid";
  case cs_PRMCOD_SCLROTORGX:
    return "X Coordinate of Scale/Rotate Origin";
  case cs_PRMCOD_SCLROTORGY:
    return "Y Coordinate of Scale/Rotate Origin";
  case cs_PRMCOD_NRTHSCL:
    return "Cartesian Scale Factor";
  case cs_PRMCOD_NRTHROT:
    return "Cartesian Rotation Angle";
  case cs_PRMCOD_ELPSCL:
    return "Ellipsoid scaling factor";

  case cs_WKTCOD_DELTAX:
    return "X-axis translation";
  case cs_WKTCOD_DELTAY:
    return "Y-axis translation";
  case cs_WKTCOD_DELTAZ:
    return "Z-axis translation";
  case cs_WKTCOD_ROTATX:
    return "X-axis rotation";
  case cs_WKTCOD_ROTATY:
    return "Y-axis rotation";
  case cs_WKTCOD_ROTATZ:
    return "Z-axis rotation";
  case cs_WKTCOD_BWSCAL:
    return "Scale difference";

  default:
    return "PARAMETER-NAME-UNDEFINED";
  }
}

void linkAlias(TiXmlElement * parent, OdInt16 id, const OdString & type, const OdString & ObjectId)
{
  if (id <= 0)
  {
    return;
  }

  TiXmlElement * csAlias = new TiXmlElement("Alias");
  parent->LinkEndChild(csAlias);
  csAlias->SetAttribute("id", id);
  csAlias->SetAttribute("type", type);
  TiXmlElement * csObjectId = new TiXmlElement("ObjectId");
  csAlias->LinkEndChild(csObjectId);
  TiXmlText * csObjectIdValue = new TiXmlText(ObjectId);
  csObjectId->LinkEndChild(csObjectIdValue);
  TiXmlElement * csNamespace = new TiXmlElement("Namespace");
  csAlias->LinkEndChild(csNamespace);
  TiXmlText * csNamespaceValue = new TiXmlText("EPSG Code");
  csNamespace->LinkEndChild(csNamespaceValue);
}

TiXmlElement* getExtentTag(TiXmlElement* tagCS)
{
  TiXmlElement* extent = NULL;
  for (TiXmlElement* domainOfValidity = tagCS->FirstChildElement(); domainOfValidity != NULL; domainOfValidity = domainOfValidity->NextSiblingElement())
  {
    if (0 == OdString(domainOfValidity->Value()).compare("DomainOfValidity"))
    {
      for (extent = domainOfValidity->FirstChildElement(); extent != NULL; extent = extent->NextSiblingElement())
      {
        if (0 == OdString(extent->Value()).compare("Extent"))
        {
          break;
        }
      }
    }
  }

  TiXmlElement* domainOfValidity = new TiXmlElement("DomainOfValidity");
  tagCS->LinkEndChild(domainOfValidity);
  extent = new TiXmlElement("Extent");
  domainOfValidity->LinkEndChild(extent);
  return extent;
}

void linkDomainOfValidityLL(TiXmlElement* tagCS, double dWest, double dEast, double dSouth, double dNorth)
{
  if (dSouth < dNorth)
  {
    TiXmlElement* extent = getExtentTag(tagCS);
    double sidesValues[4] = { dWest, dEast, dSouth, dNorth };

    TiXmlElement* geographicElement = new TiXmlElement("GeographicElement");
    extent->LinkEndChild(geographicElement);
    TiXmlElement* geographicBoundingBox = new TiXmlElement("GeographicBoundingBox");
    static const OdString sidesGeographic[4] = { "WestBoundLongitude", "EastBoundLongitude", "SouthBoundLatitude", "NorthBoundLatitude" };
    for (int i = 0; i < 4; ++i)
    {
      TiXmlElement* side = new TiXmlElement(sidesGeographic[i]);
      geographicBoundingBox->LinkEndChild(side);
      TiXmlText* sideValue = new TiXmlText(odDToStr(sidesValues[i], 'g', 15));
      side->LinkEndChild(sideValue);
    }
    geographicElement->LinkEndChild(geographicBoundingBox);
  }
}

void linkDomainOfValidityXY(TiXmlElement* tagCS, double dWest, double dEast, double dSouth, double dNorth)
{
  if (dSouth < dNorth)
  {
    TiXmlElement* extent = getExtentTag(tagCS);
    double sidesValues[4] = { dWest, dEast, dSouth, dNorth };
    TiXmlElement* cartesianElement = new TiXmlElement("CartesianElement");
    extent->LinkEndChild(cartesianElement);
    TiXmlElement* cartesianBoundingBox = new TiXmlElement("CartesianBoundingBox");
    static const OdString sidesCartesian[4] = { "MinimalBoundWest", "MaximalBoundEast", "MinimalBoundSouth", "MaximalBoundNorth" };
    for (int i = 0; i < 4; ++i)
    {
      TiXmlElement* side = new TiXmlElement(sidesCartesian[i]);
      cartesianBoundingBox->LinkEndChild(side);
      TiXmlText* sideValue = new TiXmlText(odDToStr(sidesValues[i], 'g', 15));
      side->LinkEndChild(sideValue);
    }
    cartesianElement->LinkEndChild(cartesianBoundingBox);
  }
}

TiXmlElement * createBaseElement(const OdString & sTagName,
                                 const OdString & sId,
                                 const OdString & sName,
                                 const OdString & sDescription,
                                 const OdString & sRemarks,
                                 const OdString & sComment,
                                 const OdString & sAuthority)
{
  TiXmlElement * tag = new TiXmlElement(sTagName);
  tag->SetAttribute("id", sId);

  static OdString valueKey[5] = { "Name",
    "Description",
    "Remarks",
    "Comment",
    "Authority"
  };

  OdString values[5] = { sName,
    sDescription,
    sRemarks,
    sComment,
    sAuthority
  };

  for(int i = 0; i < 5; ++i)
  {
    if (!values[i].isEmpty())
    {
      TiXmlElement * tagName = new TiXmlElement(valueKey[i]);
      tag->LinkEndChild(tagName);
      TiXmlText * tagNameValue = new TiXmlText(values[i]);
      tagName->LinkEndChild(tagNameValue);
    }
  }

  return tag;
}

TiXmlElement * createAxis(int order, const OdString & sName, const OdString & sAbbreviation, const OdString & sAxisDirection)
{
  TiXmlElement * csAxis = new TiXmlElement("CoordinateSystemAxis");

  TiXmlElement * axisOrder = new TiXmlElement("AxisOrder");
  csAxis->LinkEndChild(axisOrder);
  TiXmlText * axisOrderValue = new TiXmlText(OdString().format(OD_T("%d"), order));
  axisOrder->LinkEndChild(axisOrderValue);

  TiXmlElement * axisName = new TiXmlElement("AxisName");
  csAxis->LinkEndChild(axisName);
  TiXmlText * axisNameValue = new TiXmlText(sName);
  axisName->LinkEndChild(axisNameValue);

  TiXmlElement * axisAbbreviation = new TiXmlElement("AxisAbbreviation");
  csAxis->LinkEndChild(axisAbbreviation);
  TiXmlText * axisAbbreviationValue = new TiXmlText(sAbbreviation);
  axisAbbreviation->LinkEndChild(axisAbbreviationValue);

  TiXmlElement * axisDirection = new TiXmlElement("AxisDirection");
  csAxis->LinkEndChild(axisDirection);
  TiXmlText * axisDirectionValue = new TiXmlText(sAxisDirection);
  axisDirection->LinkEndChild(axisDirectionValue);

  return csAxis;
}

OdString getPrimeMeridianName(double org_lng)
{
  // in arx it's always "Greenwich", but its wrong
  return "Greenwich";

  int orgLng = org_lng;
  switch (orgLng) {
  default:
  case   0: return "Greenwich";
  case   2: return "Paris";
  case   4: return "Brussels";
  case   7: return "Bern";
  case   9: return "Lisbon";
  case  10: return "Oslo";
  case  12: return "Rome";
  case  18: return "Stockholm";
  case 106: return "Jakarta";
  case  -3: return "Madrid";
  case -17:
    if (fabs (org_lng - (-17.6665931666667)) < 1.0E-05)
    {
      return "FerroPrecise";
    }
    return "Ferro";
  case -74: return "Bogota";
  }
}

TiXmlElement * addParameterValue(TiXmlElement * parent, int eParamCode, double dValue, const OdString & sUnit)
{
  int prec;
  switch (eParamCode)
  {
  case cs_WKTCOD_FEAST:
  case cs_WKTCOD_FNORTH:
  case cs_WKTCOD_SCLRED:
  case cs_WKTCOD_DELTAX:
  case cs_WKTCOD_DELTAY:
  case cs_WKTCOD_DELTAZ:
    prec = 12;
    break;
  default:
    prec = 15;
    break;
  }

  return addParameterValue(parent, getParameterString(eParamCode), getParameterLogicalType(eParamCode), dValue, sUnit, prec);
}

TiXmlElement * addParameterValue(TiXmlElement * parent, const OdString & sParamName, int eLogType, double dValue, const OdString & sUnit, int prec)
{
  TiXmlElement * parameterValue = new TiXmlElement("ParameterValue");
  parent->LinkEndChild(parameterValue);
  TiXmlElement * operationParameterId = new TiXmlElement("OperationParameterId");
  parameterValue->LinkEndChild(operationParameterId);

  OdString sOperationParameterId = sParamName;

  TiXmlText * operationParameterIdValue = new TiXmlText(sOperationParameterId);
  operationParameterId->LinkEndChild(operationParameterIdValue);

  OdString sPrmType;
  OdString sUom;
  switch (eLogType)
  {
  case cs_PRMLTYP_CMPLXC:
    sPrmType = "Value";
    sUom = "coefficient";
    break;
  case cs_PRMLTYP_ZNBR:
  case cs_PRMLTYP_HSNS:
    sPrmType = "IntegerValue";
    break;

  case cs_PRMLTYP_AFCOEF:
  case cs_PRMLTYP_SCALE:
    sPrmType = "Value";
    sUom = "unity";
    break;

  case cs_PRMLTYP_ANGD:
  case cs_PRMLTYP_AZM:
  case cs_PRMLTYP_LNG:
  case cs_PRMLTYP_LAT:
    sPrmType = "Value";
    sUom = "degree";
    break;

  case cs_PRMLTYP_ELEV:
  case cs_PRMLTYP_XYCRD:
    sPrmType = "Value";
    sUom = sUnit;
    break;
  case cs_PRMLTYP_GHGT:
    sPrmType = "Value";
    sUom = "meter";
    break;
  case cs_PRMLTYP_NONE:
  default:
    sPrmType = "UNRECOGNIZED_VALUE_TYPE";
    break;
  }

  TiXmlElement * value = new TiXmlElement(sPrmType);
  parameterValue->LinkEndChild(value);

  if (!sUom.isEmpty())
  {
    value->SetAttribute("uom", sUom);
  }

  OdString sValue = odDToStr(dValue, 'g', prec);
  TiXmlText * valueValue = new TiXmlText(sValue);
  value->LinkEndChild(valueValue);
  return parameterValue;
}

void linkConversion(TiXmlElement * tagCS, const cs_Csprm_* cs)
{
  TiXmlElement* conversion = new TiXmlElement("Conversion");
  tagCS->LinkEndChild(conversion);
  TiXmlElement * projection = new TiXmlElement("Projection");
  conversion->LinkEndChild(projection);
  TiXmlElement * operationMethodId = new TiXmlElement("OperationMethodId");
  projection->LinkEndChild(operationMethodId);
  OdString sOperationMethodIdValue = getProjectionString(cs->prj_code);
  TiXmlText * operationMethodIdValue = new TiXmlText(sOperationMethodIdValue);
  operationMethodId->LinkEndChild(operationMethodIdValue);

  const cs_Csdef_ * cs_def = &(cs->csdef);
  //main parameters
  switch(cs->prj_code)
  {
  case cs_PRJCOD_MSTRO: //xml order applied
    {
      for(int i = 1; i < 24; i+=2)
      {
        addParameterValue(projection, OdString().format(OD_T("Complex Series Coefficient A%d"), i / 2 + 1), cs_PRMLTYP_AFCOEF, CS_prmValue(cs_def, i), "meter");
        addParameterValue(projection, OdString().format(OD_T("Complex Series Coefficient B%d"), i / 2 + 1), cs_PRMLTYP_AFCOEF, CS_prmValue(cs_def, i + 1), "meter");
      }
      break;
    }
  default:
    {
      struct cs_PrjprmMap_ *mp;
      for (mp = cs_PrjprmMap; mp->prj_code != cs_PRJCOD_END; ++mp)
      {
        if (mp->prj_code == cs->prj_code) break;
      }

      for(int i = 0; i < 24; ++i)
      {
        int parm_idx = mp->prm_types[i];
        if(parm_idx <= 0)
        {
          break;
        }
        addParameterValue(projection, parm_idx, CS_prmValue(cs_def,i+1), cs_def->unit);
      }
      break;
    }
  }

  ulong32_t prjFlg = cs->prj_flags;

  if ((prjFlg & cs_PRJFLG_ORGLNG) == 0)
  {
    addParameterValue(projection, cs_WKTCOD_ORGLNG, cs_def->org_lng, cs_def->unit);
  }

  if ((prjFlg & cs_PRJFLG_ORGLAT) == 0)
  {
    addParameterValue(projection, cs_WKTCOD_ORGLAT, cs_def->org_lat, cs_def->unit);
  }

  if ((prjFlg & cs_PRJFLG_SCLRED) != 0)
  {
    addParameterValue(projection, cs_WKTCOD_SCLRED, cs_def->scl_red, cs_def->unit);
  }

  if((prjFlg & cs_PRJFLG_ORGFLS) == 0)
  {
    addParameterValue(projection, cs_WKTCOD_FEAST, cs_def->x_off, cs_def->unit);
    addParameterValue(projection, cs_WKTCOD_FNORTH, cs_def->y_off, cs_def->unit);
  }
}

TiXmlElement * getOperationMethod(struct cs_GxXform_* gxXForm)
{
  OdString sOperationMethodId;

  switch (gxXForm->methodCode)
  {
    // Geocentric methods
  case cs_DTCMTH_NULLX:
    sOperationMethodId = "Null transformation (no coordinate change)"; break;
  case cs_DTCMTH_MOLOD:
    sOperationMethodId = "Molodensky"; break;
  case cs_DTCMTH_AMOLO:
    sOperationMethodId = "Abridged Molodensky"; break;
  case cs_DTCMTH_GEOCT:
    sOperationMethodId = "Geocentric translations (geog2D domain)"; break;
  case cs_DTCMTH_BURSA:
    sOperationMethodId = "Position Vector transformation (geog2D domain)"; break;
  case cs_DTCMTH_FRAME:
    sOperationMethodId = "Coordinate Frame Rotation (geog2D domain)"; break;
  case cs_DTCMTH_BDKAS:
    sOperationMethodId = "Molodensky-Badekas (geog2D domain)"; break;
  case cs_DTCMTH_3PARM:
    sOperationMethodId = "Three Parameter Transformation"; break;
  case cs_DTCMTH_4PARM:
    sOperationMethodId = "Four Parameter Transformation"; break;
  case cs_DTCMTH_6PARM:
    sOperationMethodId = "Six Parameter Transformation"; break;
  case cs_DTCMTH_7PARM:
    sOperationMethodId = "Seven Parameter Transformation"; break;
  case cs_DTCMTH_WGS72:
    sOperationMethodId = "WGS 72 to WGS 84"; break;

    // Multiple Regression methods
  case cs_DTCMTH_MULRG:
  case cs_DTCMTH_PLYNM:
    sOperationMethodId = "Multiple Regression"; break;

    // Grid file interpolation methods
  case cs_DTCMTH_GFILE:

    break;
  default:
    sOperationMethodId = "UNRECOGNIZED_METHOD_CODE"; break;
  }

  TiXmlElement * operationMethodGroup = NULL;
  TiXmlElement * operationMethod = NULL;

  if (gxXForm->methodCode != cs_DTCMTH_GFILE)
  {
    operationMethod = new TiXmlElement("OperationMethod");
    TiXmlElement * operationMethodId = new TiXmlElement("OperationMethodId");
    operationMethod->LinkEndChild(operationMethodId);
    TiXmlText * operationMethodIdValue = new TiXmlText(sOperationMethodId);
    operationMethodId->LinkEndChild(operationMethodIdValue);
  }

  //add parameters to operation method
  csGeodeticXfromParmsFile_* fileParmPtr;
  cs_GeodeticTransform_::csGeodeticXformParameters::csGeodeticXformParmsGeocentric* geoCtrPtr = &(gxXForm->gxDef.parameters.geocentricParameters);
  cs_GeodeticTransform_::csGeodeticXformParameters::csGeodeticXformParmsGridFiles_* gridFilesPtr = &(gxXForm->gxDef.parameters.fileParameters);
  cs_GeodeticTransform_::csGeodeticXformParameters::csGeodeticXformParmsDmaMulReg_* mulrgParmPtr = &(gxXForm->gxDef.parameters.dmaMulRegParameters);

  const double coef = 3600.; // convert rotate to degree
  switch (gxXForm->methodCode)
  {
  case cs_DTCMTH_NULLX:
  case cs_DTCMTH_WGS72:
    break;

  case cs_DTCMTH_3PARM:
  case cs_DTCMTH_MOLOD:
  case cs_DTCMTH_AMOLO:
  case cs_DTCMTH_GEOCT:
    addParameterValue(operationMethod, cs_WKTCOD_DELTAX, geoCtrPtr->deltaX, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_DELTAY, geoCtrPtr->deltaY, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_DELTAZ, geoCtrPtr->deltaZ, "meter");
    break;
  case cs_DTCMTH_4PARM:
    addParameterValue(operationMethod, cs_WKTCOD_DELTAX, geoCtrPtr->deltaX, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_DELTAY, geoCtrPtr->deltaY, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_DELTAZ, geoCtrPtr->deltaZ, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_BWSCAL, geoCtrPtr->scale, "unity");
    break;
  case cs_DTCMTH_6PARM:
    addParameterValue(operationMethod, cs_WKTCOD_DELTAX, geoCtrPtr->deltaX, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_DELTAY, geoCtrPtr->deltaY, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_DELTAZ, geoCtrPtr->deltaZ, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_ROTATX, geoCtrPtr->rotateX / coef, "degree");
    addParameterValue(operationMethod, cs_WKTCOD_ROTATY, geoCtrPtr->rotateY / coef, "degree");
    addParameterValue(operationMethod, cs_WKTCOD_ROTATZ, geoCtrPtr->rotateZ / coef, "degree");
    break;
  case cs_DTCMTH_BURSA:
  case cs_DTCMTH_FRAME:
  case cs_DTCMTH_7PARM:
    addParameterValue(operationMethod, cs_WKTCOD_DELTAX, geoCtrPtr->deltaX, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_DELTAY, geoCtrPtr->deltaY, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_DELTAZ, geoCtrPtr->deltaZ, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_ROTATX, geoCtrPtr->rotateX / coef, "degree");
    addParameterValue(operationMethod, cs_WKTCOD_ROTATY, geoCtrPtr->rotateY / coef, "degree");
    addParameterValue(operationMethod, cs_WKTCOD_ROTATZ, geoCtrPtr->rotateZ / coef, "degree");
    addParameterValue(operationMethod, cs_WKTCOD_BWSCAL, geoCtrPtr->scale * gxXForm->errorValue, "unity");
    break;
  case cs_DTCMTH_BDKAS:
    addParameterValue(operationMethod, cs_WKTCOD_DELTAX, geoCtrPtr->deltaX, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_DELTAY, geoCtrPtr->deltaY, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_DELTAZ, geoCtrPtr->deltaZ, "meter");
    addParameterValue(operationMethod, cs_WKTCOD_ROTATX, geoCtrPtr->rotateX / coef, "degree");
    addParameterValue(operationMethod, cs_WKTCOD_ROTATY, geoCtrPtr->rotateY / coef, "degree");
    addParameterValue(operationMethod, cs_WKTCOD_ROTATZ, geoCtrPtr->rotateZ / coef, "degree");
    addParameterValue(operationMethod, cs_WKTCOD_BWSCAL, geoCtrPtr->scale * gxXForm->errorValue, "unity");
    addParameterValue(operationMethod, "Ordinate 1 of evaluation point", cs_PRMLTYP_XYCRD, geoCtrPtr->translateX, "meter");
    addParameterValue(operationMethod, "Ordinate 2 of evaluation point", cs_PRMLTYP_XYCRD, geoCtrPtr->translateY, "meter");
    addParameterValue(operationMethod, "Ordinate 3 of evaluation point", cs_PRMLTYP_XYCRD, geoCtrPtr->translateZ, "meter");
    break;
  case cs_DTCMTH_MULRG:
    {
      addParameterValue(operationMethod, "Mid-Latitude of the Normalization", cs_PRMLTYP_LAT, mulrgParmPtr->phiOffset, "degree");
      addParameterValue(operationMethod, "Mid-Longitude of the Normalization", cs_PRMLTYP_LNG, mulrgParmPtr->lambdaOffset, "degree");
      addParameterValue(operationMethod, "Scaling Factor of the Normalization", cs_PRMLTYP_SCALE, mulrgParmPtr->normalizationScale, "unity");

      addMulRegCoefficients(operationMethod, mulrgParmPtr->coeffLambda, 'A');
      addMulRegCoefficients(operationMethod, mulrgParmPtr->coeffPhi, 'B');
      addMulRegCoefficients(operationMethod, mulrgParmPtr->coeffHeight, 'C');
    }
    break;
  case cs_DTCMTH_GFILE:
    {
      operationMethodGroup = new TiXmlElement("OperationMethodGroup");

      //add parameter values
      for (int i = 0; i < gridFilesPtr->fileReferenceCount; ++i)
      {
        fileParmPtr = &gridFilesPtr->fileNames[i];

        TiXmlElement * operationMethod = new TiXmlElement("OperationMethod");
        operationMethodGroup->LinkEndChild(operationMethod);

        TiXmlElement * operationMethodId = new TiXmlElement("OperationMethodId");
        operationMethod->LinkEndChild(operationMethodId);

        TiXmlText * operationMethodIdValue = new TiXmlText(gridFileFormat(fileParmPtr->fileFormat));
        operationMethodId->LinkEndChild(operationMethodIdValue);

        switch(fileParmPtr->fileFormat)
        {
        case cs_DTCFRMT_CNTv1:
          break;
        case cs_DTCFRMT_JAPAN:
        case cs_DTCFRMT_CNTv2:
          {
            OdString fileName = fileParmPtr->fileName;
            addGridFileParam(operationMethod, "Latitude and longitude difference file", fileParmPtr->direction, fileName);
          }
          break;
        case cs_DTCFRMT_NADCN:
          {
            OdString fileName = fileParmPtr->fileName;
            fileName.replace("?","a");
            addGridFileParam(operationMethod, "Latitude difference file", fileParmPtr->direction, fileName);
            fileName = fileParmPtr->fileName;
            fileName.replace("?","o");
            addGridFileParam(operationMethod, "Longitude difference file", fileParmPtr->direction, fileName);
          }
          break;
        case cs_DTCFRMT_FRNCH:
          {
            OdString fileName = fileParmPtr->fileName;
            addGridFileParam(operationMethod, "Geocentric translation file", fileParmPtr->direction, fileName);
          }
          break;
        case cs_DTCFRMT_ATS77:
          break;
        case cs_DTCFRMT_OST97:
          break;
        case cs_DTCFRMT_OST02:
          break;
        case cs_DTCFRMT_GEOCN:
          break;
        case cs_DTCFRMT_NONE:
        default:
          break;
        }

      }
    }
    break;
  case cs_DTCMTH_PLYNM:
    break;
  default:
    break;
  }

  if (gxXForm->methodCode != cs_DTCMTH_GFILE)
  {
    return operationMethod;
  }

  return operationMethodGroup;
}

TiXmlElement * createTransformationOperation(const OdString & sTrOpId, int eDirection)
{
  TiXmlElement * transformationOperation = new TiXmlElement("TransformationOperation");
  TiXmlElement * transformationId = new TiXmlElement("TransformationId");
  transformationOperation->LinkEndChild(transformationId);
  TiXmlText * transformationIdValue = new TiXmlText(sTrOpId);
  transformationId->LinkEndChild(transformationIdValue);

  TiXmlElement * direction = new TiXmlElement("Direction");
  transformationOperation->LinkEndChild(direction);
  OdString sDirection = (eDirection == cs_DTCDIR_FWD) ? "forward" : "inverse";
  TiXmlText * directionValue = new TiXmlText(sDirection);
  direction->LinkEndChild(directionValue);
  return transformationOperation;
}

TiXmlElement * createTransformation(TiXmlElement * dictionary, const OdString & sTransformationPath)
{
  struct cs_GxXform_* gxXForm = CS_gxloc (sTransformationPath, cs_DTCDIR_FWD);
  TiXmlElement * transformation = NULL;
  if (gxXForm != NULL)
  {
    transformation = createBaseElement("Transformation",
      gxXForm->xfrmName,
      gxXForm->xfrmName,
      gxXForm->description,
      "",
      "",
      gxXForm->source);
    dictionary->LinkEndChild(transformation);
    OdString fallback = gxXForm->gxDef.parameters.fileParameters.fallback;
    if(cs_DTCMTH_GFILE == gxXForm->methodCode && !fallback.isEmpty())
    {
      //AdditionalInformation
      TiXmlElement * additionalInformation = new TiXmlElement("AdditionalInformation");
      transformation->LinkEndChild(additionalInformation);
      TiXmlElement * parameterItem = new TiXmlElement("ParameterItem");
      additionalInformation->LinkEndChild(parameterItem);
      parameterItem->SetAttribute("type", "CsMap");
      TiXmlElement * key = new TiXmlElement("Key");
      parameterItem->LinkEndChild(key);
      TiXmlText * keyValue = new TiXmlText("TransformationGridFileFallback");
      key->LinkEndChild(keyValue);
      TiXmlElement * integerValue = new TiXmlElement("StringValue");
      parameterItem->LinkEndChild(integerValue);
      TiXmlText * IntegerValueValue = new TiXmlText(fallback);
      integerValue->LinkEndChild(IntegerValueValue);
    }

    linkDomainOfValidityLL(transformation, gxXForm->gxDef.rangeMinLng, gxXForm->gxDef.rangeMaxLng, gxXForm->gxDef.rangeMinLat, gxXForm->gxDef.rangeMaxLat);

    if (gxXForm->epsgVar != 0)
    {
      //OperationVersion
      TiXmlElement * operationVersion = new TiXmlElement("OperationVersion");
      transformation->LinkEndChild(operationVersion);
      TiXmlText * operationVersionValue = new TiXmlText(OdString().format(OD_T("%d"), gxXForm->epsgVar));
      operationVersion->LinkEndChild(operationVersionValue);
    }

    TiXmlElement * coordinateOperationAccuracy = new TiXmlElement("CoordinateOperationAccuracy");
    transformation->LinkEndChild(coordinateOperationAccuracy);
    TiXmlElement * accuracy = new TiXmlElement("Accuracy");
    coordinateOperationAccuracy->LinkEndChild(accuracy);
    accuracy->SetAttribute("uom", "meter");
    TiXmlText * accuracyValue = new TiXmlText(odDToStr(gxXForm->accuracy, 'g', 8));
    accuracy->LinkEndChild(accuracyValue);

    TiXmlElement * sourceDatumId = new TiXmlElement("SourceDatumId");
    transformation->LinkEndChild(sourceDatumId);
    TiXmlText * sourceDatumIdValue = new TiXmlText(gxXForm->srcDatum.key_nm);
    sourceDatumId->LinkEndChild(sourceDatumIdValue);

    TiXmlElement * targetDatumId = new TiXmlElement("TargetDatumId");
    transformation->LinkEndChild(targetDatumId);
    TiXmlText * targetDatumIdValue = new TiXmlText(gxXForm->trgDatum.key_nm);
    targetDatumId->LinkEndChild(targetDatumIdValue);

    TiXmlElement * isReversible = new TiXmlElement("IsReversible");
    transformation->LinkEndChild(isReversible);
    OdString sIsReversible = gxXForm->inverseSupported ? "true" : "false";
    TiXmlText * isReversibleValue = new TiXmlText(sIsReversible);
    isReversible->LinkEndChild(isReversibleValue);

    //OperationMethod
    TiXmlElement * operationMethod = getOperationMethod(gxXForm);
    transformation->LinkEndChild(operationMethod);

    linkAlias(dictionary, gxXForm->epsgNbr, "Transformation", gxXForm->xfrmName);

    CS_free(gxXForm);
  }
  return transformation;
}

OdString gridFileFormat(int format)
{
  switch(format)
  {
  case cs_DTCFRMT_CNTv1: return "NTv1";
  case cs_DTCFRMT_CNTv2: return "NTv2";
  case cs_DTCFRMT_NADCN: return "NADCON";
  case cs_DTCFRMT_FRNCH: return "France geocentric interpolation";
  case cs_DTCFRMT_JAPAN: return "Japanese Grid Mesh Interpolation";
  case cs_DTCFRMT_ATS77: return "Maritime Provinces polynomial interpolation";
  case cs_DTCFRMT_OST97: return "OST97";
  case cs_DTCFRMT_OST02: return "Ordnance Survey National Transformation";
  case cs_DTCFRMT_GEOCN: return "GEOCON";
  case cs_DTCFRMT_NONE:
  default:
    return "";
  }
}

TiXmlElement * addGridFileParam(TiXmlElement * parent, const OdString & sIdValue, int eDirection, const OdString & sFileName)
{
  TiXmlElement * parameterValue = new TiXmlElement("ParameterValue");
  parent->LinkEndChild(parameterValue);
  TiXmlElement * operationParameterId = new TiXmlElement("OperationParameterId");
  parameterValue->LinkEndChild(operationParameterId);

  TiXmlText * operationParameterIdValue = new TiXmlText(sIdValue);
  operationParameterId->LinkEndChild(operationParameterIdValue);

  TiXmlElement * value = new TiXmlElement("ValueGridFile");
  parameterValue->LinkEndChild(value);
  OdString sDirection = (eDirection == cs_DTCDIR_INV) ? "inverse" : "forward";
  value->SetAttribute("direction", sDirection);

  TiXmlText * valueValue = new TiXmlText(sFileName);
  value->LinkEndChild(valueValue);
  return parameterValue;
}

int getParameterLogicalType(int eParamCode)
{
  switch (eParamCode)
  {
  case cs_PRMCOD_ESTDMER:
  case cs_PRMCOD_CNTMER:
  case cs_PRMCOD_P1LNG:
  case cs_PRMCOD_P2LNG:
  case cs_WKTCOD_ORGLNG:
  case cs_PRMCOD_POLELNG:
  case cs_PRMCOD_GCPLNG:
  case cs_WKTCOD_ROTATX:
    return cs_PRMLTYP_LNG;

  case cs_WKTCOD_ORGLAT:
  case cs_PRMCOD_NSTDPLL:
  case cs_PRMCOD_SSTDPLL:
  case cs_PRMCOD_STDPLL:
  case cs_PRMCOD_NPARALL:
  case cs_PRMCOD_SPARALL:
  case cs_PRMCOD_P1LAT:
  case cs_PRMCOD_P2LAT:
  case cs_PRMCOD_STDCIR:
  case cs_PRMCOD_POLELAT:
  case cs_PRMCOD_OSTDPLL:
  case cs_PRMCOD_NRMLPLL:
  case cs_PRMCOD_GCPLAT:
  case cs_PRMCOD_GCP1LAT:
  case cs_PRMCOD_GCP2LAT:
  case cs_WKTCOD_ROTATY:
    return cs_PRMLTYP_LAT;

  case cs_PRMCOD_YAXISAZ:
  case cs_PRMCOD_GCAZM:
  case cs_PRMCOD_SKWAZM:
  case cs_WKTCOD_ROTATZ:
    return cs_PRMLTYP_AZM;

  case cs_PRMCOD_ADSP1:
  case cs_PRMCOD_ADSP2:
  case cs_PRMCOD_ADP1P2:
    return cs_PRMLTYP_ANGD;

    //return cs_PRMLTYP_CMPLXC; none

  case cs_PRMCOD_UTMZN:
  case cs_PRMCOD_DENRGN:
    return cs_PRMLTYP_ZNBR;

  case cs_PRMCOD_HSNS:
    return cs_PRMLTYP_HSNS;

  case cs_PRMCOD_GHGT:
    return cs_PRMLTYP_GHGT;

  case cs_PRMCOD_AELEV:
    return cs_PRMLTYP_ELEV;

  case cs_PRMCOD_AFA0:
  case cs_PRMCOD_AFA1:
  case cs_PRMCOD_AFA2:
  case cs_PRMCOD_AFB0:
  case cs_PRMCOD_AFB1:
  case cs_PRMCOD_AFB2:
    return cs_PRMLTYP_AFCOEF;

  case cs_WKTCOD_FEAST:
  case cs_WKTCOD_FNORTH:
  case cs_WKTCOD_DELTAX:
  case cs_WKTCOD_DELTAY:
  case cs_WKTCOD_DELTAZ:
    return cs_PRMLTYP_XYCRD;

  case cs_WKTCOD_SCLRED:
  case cs_WKTCOD_BWSCAL:
  case cs_PRMCOD_ELPSCL:
    return cs_PRMLTYP_SCALE;

  default:

    return cs_PRMLTYP_NONE;
  }
}

TiXmlElement * addAdditionalParameter(TiXmlElement * parent, const OdString & sLabel, double dValue)
{
  TiXmlElement * parameterItem = new TiXmlElement("ParameterItem");
  parent->LinkEndChild(parameterItem);
  parameterItem->SetAttribute("type", "CsMap");
  TiXmlElement * key = new TiXmlElement("Key");
  parameterItem->LinkEndChild(key);
  TiXmlText * keyValue = new TiXmlText(sLabel);
  key->LinkEndChild(keyValue);
  TiXmlElement * integerValue = new TiXmlElement("NumericValue");
  parameterItem->LinkEndChild(integerValue);
  TiXmlText * IntegerValueValue = new TiXmlText(odDToStr(dValue, 'g', 15));
  integerValue->LinkEndChild(IntegerValueValue);
  return parameterItem;
}

void addMulRegCoefficients(TiXmlElement * parent, double * arCoef, const OdChar letter)
{
  if (arCoef[0] != 0.)
  {
    addParameterValue(parent, OdString().format(OD_T("%c0"), letter), cs_PRMLTYP_CMPLXC, arCoef[0], "coefficient");
  }

  for(int i = 1; i < cs_MULRG_MAXCOEF - 1; ++i)
  {
    double value = arCoef[i];
    if(value != 0.)
    {
      OdString paramName;
      paramName.format(OD_T("%cu%dv%d"), letter, i / 10, i % 10);
      addParameterValue(parent, paramName, cs_PRMLTYP_CMPLXC, value, "coefficient");
    }
  }
}

OdResult createXml(const cs_Csprm_* cs, OdString & sResultName)
{
  cs_Dtdef_ dtDef;
  cs_Eldef_ elDef;
  bool bDtDefExist = true;
  bool bElDefExist = true;

  if (cs == NULL)
  {
    return eNoDatabase;
  }

  const cs_Csdef_ & csDef = cs->csdef;

  if(CS_getdt(cs->datum.key_nm, &dtDef) != 0)
  {
    bDtDefExist = false;
  }

  if(CS_getel(cs->datum.ell_knm, &elDef) != 0)
  {
    bElDefExist = false;
  }

  TiXmlDocument doc;
  TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "UTF-16", "no");
  doc.LinkEndChild(decl);

  TiXmlElement * dictionary = new TiXmlElement("Dictionary");
  doc.LinkEndChild(dictionary);
  dictionary->SetAttribute("version", "1.0");
  dictionary->SetAttribute("xmlns", "http://www.osgeo.org/mapguide/coordinatesystem");

  OdString sCoordinateSystemType;
  OdDbGeoCoordinateSystem::ProjectionCode prjCode = OdDbGeoCoordinateSystem::ProjectionCode(cs->prj_code);
  OdDbGeoCoordinateSystem::Type eCsType = getCSTypeByProjCode(prjCode);
  switch (eCsType)
  {
  case OdDbGeoCoordinateSystem::kTypeArbitrary:
    sCoordinateSystemType = "ArbitraryCoordinateSystem";
    break;
  case OdDbGeoCoordinateSystem::kTypeGeographic:
    sCoordinateSystemType = "GeographicCoordinateSystem";
    break;
  case OdDbGeoCoordinateSystem::kTypeProjected:
    sCoordinateSystemType = "ProjectedCoordinateSystem";
    break;
  default:
    ODA_ASSERT(false);
    break;
  }

  //CoordinateSystem
  TiXmlElement * tagCS = createBaseElement(sCoordinateSystemType,
    csDef.key_nm,
    csDef.key_nm,
    csDef.desc_nm,
    "",
    "",
    csDef.source);
  dictionary->LinkEndChild(tagCS);

  //AdditionalInformation
  TiXmlElement * additionalInformation = new TiXmlElement("AdditionalInformation");
  tagCS->LinkEndChild(additionalInformation);

  {
    // CSQuadrantSimplified ParameterItem
    TiXmlElement* parameterItem = new TiXmlElement("ParameterItem");
    additionalInformation->LinkEndChild(parameterItem);
    parameterItem->SetAttribute("type", "CsMap");
    TiXmlElement* key = new TiXmlElement("Key");
    parameterItem->LinkEndChild(key);
    TiXmlText* keyValue = new TiXmlText("CSQuadrantSimplified");
    key->LinkEndChild(keyValue);
    TiXmlElement* integerValue = new TiXmlElement("IntegerValue");
    parameterItem->LinkEndChild(integerValue);
    TiXmlText* IntegerValueValue = new TiXmlText(OdString().format(OD_T("%d"), csDef.quad));
    integerValue->LinkEndChild(IntegerValueValue);
  }

  if (OdDbGeoCoordinateSystem::kTypeGeographic == eCsType)
  {
    if (csDef.prj_prm1 != 0. || csDef.prj_prm2 != 0.)
    {
      addAdditionalParameter(additionalInformation, "Geographic CSR Western End of Longitude Range", csDef.prj_prm1);
      addAdditionalParameter(additionalInformation, "Geographic CSR Eastern End of Longitude Range", csDef.prj_prm2);
    }
    if (csDef.org_lng != 0.)
    {
      addAdditionalParameter(additionalInformation, "Geographic CSR Longitude of false origin", csDef.org_lng);
    }
  }

  linkDomainOfValidityLL(tagCS, csDef.ll_min[0], csDef.ll_max[0], csDef.ll_min[1], csDef.ll_max[1]);
  linkDomainOfValidityXY(tagCS, csDef.xy_min[0], csDef.xy_max[0], csDef.xy_min[1], csDef.xy_max[1]);

  if (bDtDefExist)
  {
    //DatumId
    TiXmlElement * datumId = new TiXmlElement("DatumId");
    TiXmlText * datumIdValue = new TiXmlText(csDef.dat_knm);
    datumId->LinkEndChild(datumIdValue);
    tagCS->LinkEndChild(datumId);
  }
  else if (bElDefExist)
  {
    //EllipsoidId
    TiXmlElement * ellipsoidId = new TiXmlElement("EllipsoidId");
    TiXmlText * ellipsoidIdValue = new TiXmlText(elDef.key_nm);
    ellipsoidId->LinkEndChild(ellipsoidIdValue);
    tagCS->LinkEndChild(ellipsoidId);
  }

  {
    //Axis
    TiXmlElement* axis = new TiXmlElement("Axis");
    tagCS->LinkEndChild(axis);
    OdString sUnit = csDef.unit;
    axis->SetAttribute("uom", sUnit);

    TiXmlElement* csAxis1;
    TiXmlElement* csAxis2;
    if (OdDbGeoCoordinateSystem::kTypeGeographic == eCsType)
    {
      //geographic cs
      switch (csDef.epsg_qd)
      {
      case 0:
      case 1:
        csAxis1 = createAxis(1, "Geodetic longitude", "Long", "east");
        csAxis2 = createAxis(2, "Geodetic latitude", "Lat", "north");
        break;
      case 2:
        csAxis1 = createAxis(1, "Geodetic longitude", "Long", "west");
        csAxis2 = createAxis(2, "Geodetic latitude", "Lat", "north");
        break;
      case 3:
        csAxis1 = createAxis(1, "Geodetic longitude", "Long", "west");
        csAxis2 = createAxis(2, "Geodetic latitude", "Lat", "south");
        break;
      case 4:
        csAxis1 = createAxis(1, "Geodetic longitude", "Long", "east");
        csAxis2 = createAxis(2, "Geodetic latitude", "Lat", "south");
        break;
      case -1:
        csAxis1 = createAxis(1, "Geodetic latitude", "Lat", "north");
        csAxis2 = createAxis(2, "Geodetic longitude", "Long", "east");
        break;
      case -2:
        csAxis1 = createAxis(1, "Geodetic latitude", "Lat", "north");
        csAxis2 = createAxis(2, "Geodetic longitude", "Long", "west");
        break;
      case -3:
        csAxis1 = createAxis(1, "Geodetic latitude", "Lat", "south");
        csAxis2 = createAxis(2, "Geodetic longitude", "Long", "west");
        break;
      case -4:
        csAxis1 = createAxis(1, "Geodetic latitude", "Lat", "south");
        csAxis2 = createAxis(2, "Geodetic longitude", "Long", "east");
        break;
      default:
        ODA_ASSERT(false);
        break;
      }
    }
    else
    {
      switch (csDef.epsg_qd)
      {
      case 0:
      case 1:
        csAxis1 = createAxis(1, "Easting", "E", "east");
        csAxis2 = createAxis(2, "Northing", "N", "north");
        break;
      case 2:
        csAxis1 = createAxis(1, "Westing", "W", "west");
        csAxis2 = createAxis(2, "Northing", "N", "north");
        break;
      case 3:
        csAxis1 = createAxis(1, "Westing", "W", "west");
        csAxis2 = createAxis(2, "Southing", "S", "south");
        break;
      case 4:
        csAxis1 = createAxis(1, "Easting", "E", "east");
        csAxis2 = createAxis(2, "Southing", "S", "south");
        break;
      case -1:
        csAxis1 = createAxis(1, "Northing", "N", "north");
        csAxis2 = createAxis(2, "Easting", "E", "east");
        break;
      case -2:
        csAxis1 = createAxis(1, "Northing", "N", "north");
        csAxis2 = createAxis(2, "Westing", "W", "west");
        break;
      case -3:
        csAxis1 = createAxis(1, "Southing", "S", "south");
        csAxis2 = createAxis(2, "Westing", "W", "west");
        break;
      case -4:
        csAxis1 = createAxis(1, "Southing", "S", "south");
        csAxis2 = createAxis(2, "Easting", "E", "east");
        break;
      default:
        ODA_ASSERT(false);
        break;
      }

    }
    axis->LinkEndChild(csAxis1);
    axis->LinkEndChild(csAxis2);
  }

  if (OdDbGeoCoordinateSystem::kTypeArbitrary == eCsType || OdDbGeoCoordinateSystem::kTypeProjected == eCsType)
  {
    linkConversion(tagCS, cs);
  }

  //cs alias
  linkAlias(dictionary, csDef.epsgNbr, "CoordinateSystem", csDef.key_nm);

  if (bDtDefExist)
  {
    //datum 
    TiXmlElement * geodeticDatum = createBaseElement("GeodeticDatum",
      dtDef.key_nm,
      dtDef.key_nm,
      dtDef.name,
      "",
      "",
      dtDef.source);
    dictionary->LinkEndChild(geodeticDatum);

    TiXmlElement * datumPrimeMeridianId = new TiXmlElement("PrimeMeridianId");
    geodeticDatum->LinkEndChild(datumPrimeMeridianId);
    TiXmlText * datumPrimeMeridianIdValue = new TiXmlText(getPrimeMeridianName(csDef.org_lng));
    datumPrimeMeridianId->LinkEndChild(datumPrimeMeridianIdValue);
    TiXmlElement * datumEllipsoidId = new TiXmlElement("EllipsoidId");
    geodeticDatum->LinkEndChild(datumEllipsoidId);
    TiXmlText * datumEllipsoidIdValue = new TiXmlText(dtDef.ell_knm);
    datumEllipsoidId->LinkEndChild(datumEllipsoidIdValue);

    linkAlias(dictionary, dtDef.epsgNbr, "Datum", dtDef.key_nm);
  }

  if (bElDefExist)
  {
    //ellipsoid
    TiXmlElement * ellipsoid = createBaseElement("Ellipsoid",
      elDef.key_nm,
      elDef.key_nm,
      elDef.name,
      "",
      "",
      elDef.source);
    dictionary->LinkEndChild(ellipsoid);

    TiXmlElement * semiMajorAxis = new TiXmlElement("SemiMajorAxis");
    ellipsoid->LinkEndChild(semiMajorAxis);
    semiMajorAxis->SetAttribute("uom", "meter"); // elDef.e_rad always "meter"
    TiXmlText * semiMajorAxisValue = new TiXmlText(odDToStr(elDef.e_rad, 'g', 15));
    semiMajorAxis->LinkEndChild(semiMajorAxisValue);

    TiXmlElement * secondDefiningParameter = new TiXmlElement("SecondDefiningParameter");
    ellipsoid->LinkEndChild(secondDefiningParameter);

    TiXmlElement * semiMinorAxis = new TiXmlElement("SemiMinorAxis");
    secondDefiningParameter->LinkEndChild(semiMinorAxis);
    semiMinorAxis->SetAttribute("uom", "meter"); // elDef.p_rad always "meter"
    TiXmlText * semiMinorAxisValue = new TiXmlText(odDToStr(elDef.p_rad, 'g', 15));
    semiMinorAxis->LinkEndChild(semiMinorAxisValue);

    linkAlias(dictionary, elDef.epsgNbr, "Ellipsoid", elDef.key_nm);
  }

  if (bDtDefExist)
  {
    //Transformation or TransformationPath
    OdString sTransformationPath = dtDef.key_nm;

    //this check is not very good, should be other way for it
    if (sTransformationPath != "WGS84" && sTransformationPath != "CSRS" && sTransformationPath != "ATS77")
    {
      sTransformationPath += "_to_WGS84";
      struct cs_GeodeticPath_ * geodeticPath = CS_gpdef(sTransformationPath);

      if (geodeticPath == NULL)
      {
        createTransformation(dictionary, sTransformationPath);
      }
      else
      {
        //TransformationPath
        TiXmlElement * transformationPath = createBaseElement("TransformationPath",
          sTransformationPath,
          sTransformationPath,
          geodeticPath->description,
          "",
          "",
          geodeticPath->source);

        TiXmlElement * sourceDatumId = new TiXmlElement("SourceDatumId");
        transformationPath->LinkEndChild(sourceDatumId);
        TiXmlText * sourceDatumIdValue = new TiXmlText(geodeticPath->srcDatum);
        sourceDatumId->LinkEndChild(sourceDatumIdValue);

        TiXmlElement * targetDatumId = new TiXmlElement("TargetDatumId");
        transformationPath->LinkEndChild(targetDatumId);
        TiXmlText * targetDatumIdValue = new TiXmlText(geodeticPath->trgDatum);
        targetDatumId->LinkEndChild(targetDatumIdValue);

        //TransformationOperationGroup
        TiXmlElement * transformationOperationGroup = new TiXmlElement("TransformationOperationGroup");
        transformationPath->LinkEndChild(transformationOperationGroup);

        //TransformationOperation 's
        for(int i = 0; i < geodeticPath->elementCount; ++i)
        {
          TiXmlElement * transformationOperation = createTransformationOperation(
            geodeticPath->geodeticPathElements[i].geodeticXformName,
            geodeticPath->geodeticPathElements[i].direction);
          transformationOperationGroup->LinkEndChild(transformationOperation);

          createTransformation(dictionary, geodeticPath->geodeticPathElements[i].geodeticXformName);
        }

        //it's here, because transformation path must link after transformations
        dictionary->LinkEndChild(transformationPath);

        CS_free(geodeticPath);
      }
    }
  }

  // Declare a printer
  TiXmlPrinter printer;
  doc.Accept(&printer);
  sResultName = printer.CStr();
  return eOk;
}

OdDbGeoCoordinateSystem::Unit getUnitByName(const OdString & sName)
{
  if (0 == sName.iCompare("Meter"))
    return OdDbGeoCoordinateSystem::kUnitMeter;
  else if (0 == sName.iCompare("Foot"))
    return OdDbGeoCoordinateSystem::kUnitFoot;
  else if (0 == sName.iCompare("Inch"))
    return OdDbGeoCoordinateSystem::kUnitInch;
  else if (0 == sName.iCompare("IFoot"))
    return OdDbGeoCoordinateSystem::kUnitIFoot;
  else if (0 == sName.iCompare("ClarkeFoot"))
    return OdDbGeoCoordinateSystem::kUnitClarkeFoot;
  else if (0 == sName.iCompare("SearsFoot"))
    return OdDbGeoCoordinateSystem::kUnitSearsFoot;
  else if (0 == sName.iCompare("GoldCoastFoot"))
    return OdDbGeoCoordinateSystem::kUnitGoldCoastFoot;
  else if (0 == sName.iCompare("IInch"))
    return OdDbGeoCoordinateSystem::kUnitIInch;
  else if (0 == sName.iCompare("MicroInch"))
    return OdDbGeoCoordinateSystem::kUnitMicroInch;
  else if (0 == sName.iCompare("Mil"))
    return OdDbGeoCoordinateSystem::kUnitMil;
  else if (0 == sName.iCompare("Centimeter"))
    return OdDbGeoCoordinateSystem::kUnitCentimeter;
  else if (0 == sName.iCompare("Kilometer"))
    return OdDbGeoCoordinateSystem::kUnitKilometer;
  else if (0 == sName.iCompare("Yard"))
    return OdDbGeoCoordinateSystem::kUnitYard;
  else if (0 == sName.iCompare("SearsYard"))
    return OdDbGeoCoordinateSystem::kUnitSearsYard;
  else if (0 == sName.iCompare("IndianYard"))
    return OdDbGeoCoordinateSystem::kUnitIndianYard;
  else if (0 == sName.iCompare("IndianYd37"))
    return OdDbGeoCoordinateSystem::kUnitIndianYd37;
  else if (0 == sName.iCompare("Mile"))
    return OdDbGeoCoordinateSystem::kUnitMile;
  else if (0 == sName.iCompare("IYard"))
    return OdDbGeoCoordinateSystem::kUnitIYard;
  else if (0 == sName.iCompare("IMile"))
    return OdDbGeoCoordinateSystem::kUnitIMile;
  else if (0 == sName.iCompare("Knot"))
    return OdDbGeoCoordinateSystem::kUnitKnot;
  else if (0 == sName.iCompare("NautM"))
    return OdDbGeoCoordinateSystem::kUnitNautM;
  else if (0 == sName.iCompare("Lat-66"))
    return OdDbGeoCoordinateSystem::kUnitLat66;
  else if (0 == sName.iCompare("Lat-83"))
    return OdDbGeoCoordinateSystem::kUnitLat83;
  else if (0 == sName.iCompare("Decimeter"))
    return OdDbGeoCoordinateSystem::kUnitDecimeter;
  else if (0 == sName.iCompare("Millimeter"))
    return OdDbGeoCoordinateSystem::kUnitMillimeter;
  else if (0 == sName.iCompare("Dekameter"))
    return OdDbGeoCoordinateSystem::kUnitDekameter;
  else if (0 == sName.iCompare("Decameter"))
    return OdDbGeoCoordinateSystem::kUnitDecameter;
  else if (0 == sName.iCompare("Hectometer"))
    return OdDbGeoCoordinateSystem::kUnitHectometer;
  else if (0 == sName.iCompare("GermanMeter"))
    return OdDbGeoCoordinateSystem::kUnitGermanMeter;
  else if (0 == sName.iCompare("CaGrid"))
    return OdDbGeoCoordinateSystem::kUnitCaGrid;
  else if (0 == sName.iCompare("ClarkeChain"))
    return OdDbGeoCoordinateSystem::kUnitClarkeChain;
  else if (0 == sName.iCompare("GunterChain"))
    return OdDbGeoCoordinateSystem::kUnitGunterChain;
  else if (0 == sName.iCompare("BenoitChain"))
    return OdDbGeoCoordinateSystem::kUnitBenoitChain;
  else if (0 == sName.iCompare("SearsChain"))
    return OdDbGeoCoordinateSystem::kUnitSearsChain;
  else if (0 == sName.iCompare("ClarkeLink"))
    return OdDbGeoCoordinateSystem::kUnitClarkeLink;
  else if (0 == sName.iCompare("GunterLink"))
    return OdDbGeoCoordinateSystem::kUnitGunterLink;
  else if (0 == sName.iCompare("BenoitLink"))
    return OdDbGeoCoordinateSystem::kUnitBenoitLink;
  else if (0 == sName.iCompare("SearsLink"))
    return OdDbGeoCoordinateSystem::kUnitSearsLink;
  else if (0 == sName.iCompare("Rod"))
    return OdDbGeoCoordinateSystem::kUnitRod;
  else if (0 == sName.iCompare("IntnlChain"))
    return OdDbGeoCoordinateSystem::kUnitInternationalChain;
  else if (0 == sName.iCompare("IntnlLink"))
    return OdDbGeoCoordinateSystem::kUnitInternationalLink;
  else if (0 == sName.iCompare("Perch"))
    return OdDbGeoCoordinateSystem::kUnitPerch;
  else if (0 == sName.iCompare("Pole"))
    return OdDbGeoCoordinateSystem::kUnitPole;
  else if (0 == sName.iCompare("Furlong"))
    return OdDbGeoCoordinateSystem::kUnitFurlong;
  else if (0 == sName.iCompare("Rood"))
    return OdDbGeoCoordinateSystem::kUnitRood;
  else if (0 == sName.iCompare("CapeFoot"))
    return OdDbGeoCoordinateSystem::kUnitCapeFoot;
  else if (0 == sName.iCompare("Brealey"))
    return OdDbGeoCoordinateSystem::kUnitBrealey;
  else if (0 == sName.iCompare("Degree"))
    return OdDbGeoCoordinateSystem::kUnitDegree;
  else if (0 == sName.iCompare("Grad"))
    return OdDbGeoCoordinateSystem::kUnitGrad;
  else if (0 == sName.iCompare("Grade"))
    return OdDbGeoCoordinateSystem::kUnitGrade;
  else if (0 == sName.iCompare("MapInfo"))
    return OdDbGeoCoordinateSystem::kUnitMapInfo;
  else if (0 == sName.iCompare("Mil"))
    return OdDbGeoCoordinateSystem::kUnitMil;
  else if (0 == sName.iCompare("Minute"))
    return OdDbGeoCoordinateSystem::kUnitMinute;
  else if (0 == sName.iCompare("Radian"))
    return OdDbGeoCoordinateSystem::kUnitRadian;
  else if (0 == sName.iCompare("Second"))
    return OdDbGeoCoordinateSystem::kUnitSecond;
  else if (0 == sName.iCompare("Decisec"))
    return OdDbGeoCoordinateSystem::kUnitDecisec;
  else if (0 == sName.iCompare("Centisec"))
    return OdDbGeoCoordinateSystem::kUnitCentisec;
  else if (0 == sName.iCompare("Millisec"))
    return OdDbGeoCoordinateSystem::kUnitMillisec;
  else if (0 == sName.iCompare("BrFootTrunc"))
    return OdDbGeoCoordinateSystem::kUnitBrFootTrunc;
  else if (0 == sName.iCompare("BrChainTrunc"))
    return OdDbGeoCoordinateSystem::kUnitBrChainTrunc;
  else if (0 == sName.iCompare("BrLinkTrunc"))
    return OdDbGeoCoordinateSystem::kUnitBrLinkTrunc;
  else
    return OdDbGeoCoordinateSystem::kUnitUnknown;
}

void csUnitScaleToString(OdString & sUnitScale, double dUnitScale)
{
  if (dUnitScale > 1e-4)
  {
    sUnitScale.format(L"%lf", dUnitScale);

    sUnitScale.trimRight('0');
    sUnitScale.trimRight('.');
    if (sUnitScale.isEmpty())
    {
      sUnitScale = L"0";
    }
  }
  else if (dUnitScale > 1e-12)
  {
    sUnitScale.format(L"%lE", dUnitScale);
  }
  else
  {
    sUnitScale = L"0";
  }
}
