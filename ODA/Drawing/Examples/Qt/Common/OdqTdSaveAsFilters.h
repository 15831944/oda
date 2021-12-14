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
// OdqTdSaveAsFilters.h
//

#ifndef OD_QT_TD_SAVE_AS_FILTERS_H_
#define OD_QT_TD_SAVE_AS_FILTERS_H_

#include <QObject>
#include <QList>
#include <QMap>

#include "OdaCommon.h"

//////////////////////////////////////////////////////////////////////////

class OdqTdSaveAsFilters
{
  QString m_qsPattern;
  bool m_bCutKeywordSuffix;

  QStringList m_keywordsByType[3];
  QMap<QString, OdDb::DwgVersion> m_mapVersByKeyword[3];

  QStringList m_filtersDwgPatterned;
  QMap<QString, OdDb::DwgVersion> m_mapDwgVers;
  QMap<QString, OdDb::SaveType> m_mapDwgTypes;

  void init();
  void addFilter(const QString& qsFilter,
                 const QString& cqsKeyword,
                 OdDb::DwgVersion verDwg,
                 OdDb::SaveType typeDgw)
  {
    if (typeDgw < 3)
    {
      QString qsKeyword = cqsKeyword.trimmed();
      int pos = 0;
      if (m_bCutKeywordSuffix && (pos = qsKeyword.indexOf("(")) > 0) // marker of LT suffix
        qsKeyword = qsKeyword.left(pos).trimmed();
      ODA_ASSERT_ONCE(m_mapVersByKeyword[typeDgw].end() == m_mapVersByKeyword[typeDgw].find(qsKeyword));

      m_keywordsByType[typeDgw] << qsKeyword;
      m_mapVersByKeyword[typeDgw][qsKeyword] = verDwg;
    }

    QString qsExt;
    switch (typeDgw)
    {
    default:
      ODA_FAIL_ONCE();
    case OdDb::kDwg:
      qsExt = "dwg";
      break;
    case OdDb::kDxf:
    case OdDb::kDxb:
      qsExt = "dxf";
      break;
    //case OdDb::kDxb:
    //  qsExt = "dxb";
    //  break;
    } // end switch

    QString qsPatternedFilter = qsFilter;
    if (!m_qsPattern.isEmpty() && !qsExt.isEmpty())
      qsPatternedFilter = m_qsPattern.arg(qsFilter).arg(qsExt);

    ODA_ASSERT_ONCE(   !qsPatternedFilter.isEmpty()
                    && m_mapDwgVers.end() == m_mapDwgVers.find(qsPatternedFilter)
                    && m_mapDwgTypes.end() == m_mapDwgTypes.find(qsPatternedFilter));

    m_filtersDwgPatterned << qsPatternedFilter;
    m_mapDwgVers[qsPatternedFilter] = verDwg;
    m_mapDwgTypes[qsPatternedFilter] = typeDgw;
  }

public:
  OdqTdSaveAsFilters(const QString& qsPattern = "%1 (*.%2)|*.%2|",
                     bool bCutKeywordSuffix = true)
    : m_qsPattern(qsPattern)
    , m_bCutKeywordSuffix(bCutKeywordSuffix)
  {
    init();
  }

  OdDb::DwgVersion getDwgVer(const QString& qsPatternedFilter) const
  {
    ODA_ASSERT_ONCE(m_mapDwgVers.end() != m_mapDwgVers.find(qsPatternedFilter));
    return m_mapDwgVers[qsPatternedFilter];
  }    

  OdDb::SaveType getDwgType(const QString& qsPatternedFilter) const
  {
    ODA_ASSERT_ONCE(m_mapDwgTypes.end() != m_mapDwgTypes.find(qsPatternedFilter));
    return m_mapDwgTypes[qsPatternedFilter];
  }    

  const QStringList& getFilters() const
  {
    return m_filtersDwgPatterned;
  }

  QString getFiltersAsString(const QString& qsEndMarker = "|") const
  {
    QString qsFilters;
    foreach (QString qsFlt, m_filtersDwgPatterned)
      qsFilters += qsFlt;
    qsFilters += qsEndMarker;
    return qsFilters;
  }

  QString getFilter(OdDb::DwgVersion verDwg, OdDb::SaveType typeDgw) const
  {
    foreach (QString qsFlt, m_filtersDwgPatterned)
    {
      if (getDwgVer(qsFlt) == verDwg && getDwgType(qsFlt) == typeDgw)
        return qsFlt;
    }
    ODA_FAIL_ONCE(); // was not found
    return "";
  }

  const QStringList& getKeywords(OdDb::SaveType typeDgw) const
  {
    ODA_ASSERT_ONCE(typeDgw < 3);
    return m_keywordsByType[typeDgw];
  }

  QString getKeyword(OdDb::DwgVersion verDwg, OdDb::SaveType typeDgw) const
  {
    foreach (QString qsKeyword, getKeywords(typeDgw))
    {
      if (m_mapVersByKeyword[typeDgw][qsKeyword] == verDwg)
        return qsKeyword;
    }
    ODA_FAIL_ONCE(); // was not found
    return "";
  }

  QString getKeywordByIndex(OdDb::SaveType typeDgw, int indexKeyword) const
  {
    if (indexKeyword < 0 || indexKeyword >= getKeywords(typeDgw).count())
    {
      ODA_FAIL_ONCE();
      return "";
    }

    return getKeywords(typeDgw).at(indexKeyword);
  }

  void getKeywordsDataIO(OdDb::DwgVersion verDwg, OdDb::SaveType typeDgw, // in
                         QString& qsPrompt, // in (prefix) / out
                         QString& qsKeywords, // out
                         int& indexDefault) const // out
  {
    indexDefault = -1;
    QString qsKeywordDefault;
    qsKeywords.clear();
    int index = 0;
    foreach (QString qsKeyword, getKeywords(typeDgw))
    {
      if (!index || m_mapVersByKeyword[typeDgw][qsKeyword] == verDwg)
      {
        indexDefault = index;
        qsKeywordDefault = qsKeyword;
      }

      if (!index++)
        qsPrompt += " [";
      else
      {
        qsPrompt += "/";
        qsKeywords += " ";
      }

      qsPrompt += qsKeyword;
      qsKeywords += qsKeyword;
    }
    switch (typeDgw)
    {
    case OdDb::kDwg:
      qsPrompt += "/DXF"; // "/DXF/DXB";
      qsKeywords += " DXF DWG"; // " DXF DXB DWG";
      break;
    case OdDb::kDxf:
      qsPrompt += "/DWG/Binary"; // "/DWG/DXB";
      qsKeywords += " DWG Binary ASCII"; // " DWG DXB DXF";
      break;
    case OdDb::kDxb:
      qsPrompt += "/DWG/ASCII"; // "/DWG/DXF";
      qsKeywords += " DWG ASCII Binary"; // " DWG DXF DXB";
      break;
    } // end switch
    qsPrompt += "]";

    if (!qsKeywordDefault.isEmpty())
    {
      qsPrompt += " <";
      qsPrompt += qsKeywordDefault;
      qsPrompt += ">";
    }
  }

  // false if was not changed
  bool updateTypeByIndexKeyword(int indexKeyword,
                                OdDb::SaveType& typeDgw) const // in / out
  {
    indexKeyword -= getKeywords(typeDgw).count();
    if (indexKeyword < 0)
      return false;
    if (indexKeyword == 2)
      return true;

    switch (typeDgw)
    {
    case OdDb::kDwg:
      typeDgw = !indexKeyword ? OdDb::kDxf : OdDb::kDxb;
      break;
    case OdDb::kDxf:
      typeDgw = !indexKeyword ? OdDb::kDwg : OdDb::kDxb;
      break;
    case OdDb::kDxb:
      typeDgw = !indexKeyword ? OdDb::kDwg : OdDb::kDxf;
      break;
    } // end switch
    return true;
  }

  OdDb::DwgVersion getVersionByIndexKeyword(OdDb::SaveType typeDgw, int indexKeyword) const
  {
    QString qsKeyword = getKeywordByIndex(typeDgw, indexKeyword);
    OdDb::DwgVersion verDwg = m_mapVersByKeyword[typeDgw][qsKeyword];
    return verDwg;
  }
};

#ifdef NON_UPDATED_TO_DHL_CURRENT

void OdqTdSaveAsFilters::init()
{
  // these data are strored as binary in OdaFileConverter NFC dialog :
  //OdDb::kDwg
  addFilter("2010 DWG",   "2010(LT2010)", OdDb::vAC24, OdDb::kDwg); // R24
  addFilter("2007 DWG",   "2007(LT2007)", OdDb::vAC21, OdDb::kDwg); // R21
  addFilter("2004 DWG",   "2004(LT2004)", OdDb::vAC18, OdDb::kDwg); // R18
  addFilter("2000 DWG",   "2000(LT2000)", OdDb::vAC15, OdDb::kDwg); // R15
  addFilter( "R14 DWG", "R14(LT98&LT97)", OdDb::vAC14, OdDb::kDwg); // R14
  addFilter( "R13 DWG",            "R13", OdDb::vAC13, OdDb::kDwg); // R13
  addFilter( "R12 DWG",            "R12", OdDb::vAC12, OdDb::kDwg); // R12
  //OdDb::kDxf;
  addFilter("2010 ASCII DXF",   "2010(LT2010)", OdDb::vAC24, OdDb::kDxf); // R24
  addFilter("2007 ASCII DXF",   "2007(LT2007)", OdDb::vAC21, OdDb::kDxf); // R21
  addFilter("2004 ASCII DXF",   "2004(LT2004)", OdDb::vAC18, OdDb::kDxf); // R18
  addFilter("2000 ASCII DXF",   "2000(LT2000)", OdDb::vAC15, OdDb::kDxf); // R15
  addFilter( "R14 ASCII DXF", "R14(LT98&LT97)", OdDb::vAC14, OdDb::kDxf); // R14
  addFilter( "R13 ASCII DXF",            "R13", OdDb::vAC13, OdDb::kDxf); // R13
  addFilter( "R12 ASCII DXF",            "R12", OdDb::vAC12, OdDb::kDxf); // R12
  addFilter( "R10 ASCII DXF",            "R10", OdDb::vAC10, OdDb::kDxf); // R10
  addFilter(  "R9 ASCII DXF",             "R9", OdDb::vAC09, OdDb::kDxf); // R9
  // OdDb::kDxb
  addFilter("2010 Binary DXF",   "2010(LT2010)", OdDb::vAC24, OdDb::kDxb); // R24
  addFilter("2007 Binary DXF",   "2007(LT2007)", OdDb::vAC21, OdDb::kDxb); // R21
  addFilter("2004 Binary DXF",   "2004(LT2004)", OdDb::vAC18, OdDb::kDxb); // R18
  addFilter("2000 Binary DXF",   "2000(LT2000)", OdDb::vAC15, OdDb::kDxb); // R15
  addFilter( "R14 Binary DXF", "R14(LT98&LT97)", OdDb::vAC14, OdDb::kDxb); // R14
  addFilter( "R13 Binary DXF",            "R13", OdDb::vAC13, OdDb::kDxb); // R13
  addFilter( "R12 Binary DXF",            "R12", OdDb::vAC12, OdDb::kDxb); // R12
  addFilter( "R10 Binary DXF",            "R10", OdDb::vAC10, OdDb::kDxb); // R10
}

#else

void OdqTdSaveAsFilters::init()
{
  QList<OdDb::DwgVersion> listVers;
  QStringList listPrefs;
  OdDb::DwgVersion ver = OdDb::vAC09;
  int pref = 9;
  while (ver <= OdDb::kDHL_CURRENT)
  {
    QString qsPref = (pref >= 15) ? QString::number((pref == 15) ? 2000 : (pref - 15 + 2001))
                                  : QString("R%1").arg(pref);
    listPrefs.push_front(qsPref);
    listVers.push_front(ver); 
    ver = (OdDb::DwgVersion)(ver + ((pref == 9 || pref >= 13) ? 2 : 3));
    pref += (pref >= 15) ? 3 : ((pref == 10) ? 2 : 1);
    if (pref == 30)
      pref += 2; // for 2018
  }
  int index = 0;
  foreach (ver, listVers)
  {
    if (ver >= OdDb::vAC12)
      addFilter(listPrefs[index] + " DWG", listPrefs[index], ver, OdDb::kDwg);
    index++;
  }
  index = 0;
  foreach (ver, listVers)
  {
    addFilter(listPrefs[index] + " ASCII DXF", listPrefs[index], ver, OdDb::kDxf);
    index++;
  }
  index = 0;
  foreach (ver, listVers)
  {
    if (ver >= OdDb::vAC10)
      addFilter(listPrefs[index] + " Binary DXF", listPrefs[index], ver, OdDb::kDxb);
    index++;
  }
}

#endif

#endif // OD_QT_TD_SAVE_AS_FILTERS_H_
