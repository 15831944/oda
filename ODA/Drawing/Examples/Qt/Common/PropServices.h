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
// PropServices.h
//

#ifndef OD_PROP_SERVICES_H_
#define OD_PROP_SERVICES_H_

#include "TD_PackPush.h"

#include "RxObject.h"
#include "Ed/EdCommandContext.h"
#include "StringArray.h"
#include "DbDatabase.h"
#include "DbBaseDatabase.h"
#include "DbBaseSubentId.h"
#include "SharedPtr.h"
#include "OdList.h"
#include "RxVariant.h"
#include "OdDToStr.h"

class OdPropBase;
class OdPropServices;

//------------------------------------------------------------------------
// casted from OdEdCommandContext

class OdPropSource
{
public:
  typedef enum Action_t { 
    enAction_None = 0
  , enAction_Close = 1 // Ok - (there is no other way to close modal dialog on Mac)
  , enAction_Cancel = 2
//, enAction_RecreateWidgets = 4 // (internal)
  , enAction_RefreshSource = 8 // enAction_RefreshAll
  , enAction_RefreshSourceWidgets = 8 + 4 // with recreating of widgets // enAction_ForceRefreshAll
  , enAction_RefreshAllSources = 16
  , enAction_RefreshAllSourcesWidgets = 16 + 4 // with recreating of widgets
  , enAction_evaluateTopLevelNetwork = 32
  , enAction_FailedToSet = 64
  } Action;

  virtual ~OdPropSource() 
  {
  }

  virtual bool updateXml(OdString& sXml) = 0; // get / update
  virtual bool setPropertyValue(const OdString&, // sPath
                                const OdString&, // sValue
                                Action&) //action
  {
    return false;
  }
  virtual bool getPropertyPossibleValues(const OdString&, // sPath
                                         OdStringArray&, // values
                                         OdStringArray&) // iconPaths
  {
    return false;
  }

  static OdString extractFirst(OdString& sPath, 
                               OdChar ch = __OD_T('/'), 
                               bool bTrimSpaces = false)
  {
    OdString sFirst;
    int pos = sPath.find(ch);
    if (pos < 0)
    {
      sFirst = sPath;
      sPath.empty();
      return sFirst;
    }

    sFirst = sPath.left(pos);
    sPath = sPath.mid(++pos);
    if (bTrimSpaces)
    {
      sFirst.trimRight();
      sPath.trimLeft();
    }
    return sFirst;
  }
};

// to use only CDA properties in property palette for all databases except DWG
inline int overridePropSourceFlagsForPalette(OdRxObject* pRxDb, int flags)
{
  if (   !GETBIT(flags, 4) // CDA 
    //&& GETBIT(flags, 2) // RegistredXmlSrc
      && GETBIT(flags, 1) // XmlPattern
      && pRxDb && OdDbDatabase::cast(pRxDb).isNull())
  {
    SETBIT(flags, 1, false); SETBIT(flags, 2, false);
    SETBIT(flags, 4, true);
  }
  return flags;
}

//------------------------------------------------------------------------

const char tCategory[] = "category";
const char tBool[] = "bool";
const char tBoolPic[] = "boolpictured";
const char tBoolName[] = "boolnamed";
const char tBoolButton[] = "button";
const char tInt[] = "int";
const char tDouble[] = "double";
const char tDistance[] = "distance";
const char tAngle[] = "angle";
const char tString[] = "string";
const char tCombo[] = "combo";
const char tColor[] = "colorref";
const char tOdColor[] = "odcolor";
const char tLinetype[] = "linetype";
const char tLineweight[] = "lineweight";
const char tTransparency[] = "transparency";
const char tFolder[] = "folder";
const char sXCoord[] = "X";
const char sYCoord[] = "Y";
const char sZCoord[] = "Z";

class OdPropContext
{
protected:
  friend class OdPropBase;
  OdPropBase* m_pPropBase; /// for getFormatter() 
  int m_indent;
  OdSharedPtr<OdString> m_pXmlParent;
  OdList<OdSharedPtr<OdString> > m_lstXmlByKids; // filled by destltructor last copy of kid
  OdString m_sXmlLine,
           m_sTag, m_sPath, m_sName; // are used for kid contents
  OdRxObjectPtr m_pObj;

  OdPropContext(const OdSharedPtr<OdString>& pXmlParent, OdPropBase* pPropBase,
                const OdString& sPath, const OdString& sName = OdString::kEmpty,
                int indent = 0,
                const OdString& sTag = L"Property")
    : m_pPropBase(pPropBase)
    , m_pXmlParent(pXmlParent)
    , m_sPath(sPath)
    , m_sName(sName)
    , m_indent(indent)
    , m_sTag(sTag)
  {
    ODA_ASSERT_VAR(int ref = m_pXmlParent.refCount();)
    ODA_ASSERT_ONCE(ref == 2);
    if (sName.isEmpty())
      return;
    for (int idx = 0; idx < m_indent; idx++)
      m_sXmlLine += L' ';
    m_sXmlLine += OdString().format(L"<%ls ", sTag.c_str());
    addAttrib("Path", sPath).addAttrib("Name", sName);
  }

  OdString toOdString(const OdVariant& vValue, 
                      const OdString& sValueType = tString,
                      bool bIntAsUnsigned = false);
public:
  static inline OdString encodeToXmlAttrib(const OdString& cqsStr);
  static inline OdString decodeFromXmlAttrib(const OdString& cqsStr);

  const OdString& name() const
  {
    return m_sName;
  }
  OdString pathName() const
  {
    OdString sPathName = m_sPath;
    if (!m_sName.isEmpty())
    {
      if (!sPathName.isEmpty())
        sPathName += L'/';
      sPathName += m_sName;
    }
    return sPathName;
  }
protected:

  OdPropContext addNode(const OdString& sName)
  {
    ODA_ASSERT_ONCE(!sName.isEmpty());
    OdString sPath = pathName();

    OdSharedPtr<OdString>& pXmlByKid = *m_lstXmlByKids.append();
    ODA_ASSERT_ONCE(pXmlByKid.isNull());
    pXmlByKid = new OdString();
    ODA_ASSERT_ONCE(pXmlByKid.refCount() == 1);
    return OdPropContext(pXmlByKid, m_pPropBase, sPath, sName, m_indent + 2, m_sTag);
  }

public:
  virtual ~OdPropContext()
  {
    int ref = m_pXmlParent.refCount();
    if (ref == 2) // filled by destltructor of last copy of kid (except upper holder)
    {
      ODA_ASSERT_ONCE(ref == 2 && (!m_sXmlLine.isEmpty() || m_sName.isEmpty()) && m_pXmlParent->isEmpty());
      if (!m_sXmlLine.isEmpty())
        m_sXmlLine += L"/>\r\n";
      for (OdList<OdSharedPtr<OdString> >::iterator pItr = m_lstXmlByKids.begin();
           pItr != m_lstXmlByKids.end(); pItr++)
      {
        OdSharedPtr<OdString>& pXmlKid = *pItr;
        m_sXmlLine += *pXmlKid.get();
      }
      *m_pXmlParent.get() += m_sXmlLine;
    }
  }

  OdPropContext& addAttrib(const OdString& sAttrib, const OdString& sValue)
  {
    m_sXmlLine += OdString().format(L"%ls=\"%ls\" ", sAttrib.c_str(), sValue.c_str());
    return *this;
  }

  //OdPropContext& addAttrib(const OdString& sAttrib, const OdVariant& vValue)
  //{
  //  return addAttrib(sAttrib, toOdString(vValue));
  //}

  OdPropContext addCategory(const OdString& sName)
  {
    OdPropContext ctx = addNode(sName);
    return ctx.addAttrib(L"Type", tCategory);
  }

  OdPropContext addProperty(const OdString& sName, const OdString& sValue, 
                            const OdString& sValueType = tString)
  {
    OdPropContext ctx = addNode(sName);
    ctx.addAttrib(L"Value", encodeToXmlAttrib(sValue)); // CORE-13627 // sValue);
    return ctx.addAttrib(L"Type", sValueType);
  }
  OdPropContext addProperty(const OdString& sName, const char* pcszValue) 
  {
    return addProperty(sName, OdString(pcszValue));
  }
  OdPropContext addProperty(const OdString& sName, const wchar_t* pcszValue)
  {
    return addProperty(sName, OdString(pcszValue));
  }
  OdPropContext addProperty(const OdString& sName, const OdVariant& vValue = OdString::kEmpty,
                            const OdString& sValueType = tString)
  {
    return addProperty(sName, toOdString(vValue, sValueType), sValueType);
  }

  OdPropContext& visName(const OdString& sVisName)
  {
    return addAttrib(L"VisibleName", sVisName);
  }

  OdPropContext& image(const OdString& sImagePath)
  {
    return addAttrib(L"Image", sImagePath);
  }

  OdPropContext& comment(const OdString& sComment)
  {
    return addAttrib(L"Comment", sComment);
  }

  OdPropContext& enable(bool bVal = true)
  {
    return addAttrib(L"Enabled", toOdString(bVal));
  }
  OdPropContext& editable(bool bVal = true) { return enable(bVal); }
  OdPropContext& readOnly() { return enable(false); }

  OdPropContext& skip()
  {
    return addAttrib(L"SkipBase", L"true");
  }
  OdPropContext& skipBase() { return skip(); }
};

//------------------------------------------------------------------------

class OdPropBase : public OdPropSource, public OdRxObject
{
private:
  OdDbStub* m_id;
  OdRxObject* m_pRxDb; // of m_id
  int m_propSrcFlags;

  mutable OdUnitsFormatter* m_pBaseFormatter;
  mutable OdPropServices* m_pPropServ;
public:
  ODA_ASSERT_VAR(OdString m_sRegistredForClassName;)

protected:
  OdPropBase() 
    : m_id(NULL), m_pRxDb(NULL), m_propSrcFlags(0), m_pBaseFormatter(NULL), m_pPropServ(NULL)
  {}
public:
  virtual ~OdPropBase() {}

  virtual bool getPropertyValues(OdPropContext& ctx) = 0;

  virtual bool updateXml(OdString& sXml)
  {
    OdSharedPtr<OdString> pXml = new OdString();
    ODA_ASSERT_ONCE(pXml.refCount() == 1);
    {
      OdPropContext ctx(pXml, this, L"Entity");
      if (!getPropertyValues(ctx))
        return false;
    }
    ODA_ASSERT_ONCE(!pXml->isEmpty() || m_sRegistredForClassName == L"CdaBased"); // test
    //sXml = L"<?xml version=\'1.0\' encoding=\'windows-1252\'?>\r\n<Properties>\r\n";
    sXml += *pXml.get();
    //sXml += L"</Properties>\r\n";
    return true;
  }

  typedef enum
  {
    enPropSrcBased_XmlPattern = 1,
    enPropSrcBased_RegistredXmlSrc = 2, // wrapper existance of upper class (the last in list) is more priority than CDA 
    enPropSrcBased_Cda = 4,
    enPropSrcBased_CdaSkipEmptyCategory = 8,
    enPropSrcBased_ShowWeightAsNamePrefix = 16  // for test only
  } OdqPropSrcFlags;
  void setPropSrcFlags(int flags)
  {
    m_propSrcFlags = flags;
  }
  int propSrcFlags() const
  {
    return m_propSrcFlags;
  }
  bool isPropSrcFlagsOn(OdqPropSrcFlags flg) const
  {
    return GETBIT(m_propSrcFlags, flg) != 0;
  }

  OdPropBase& setId(OdDbStub* id, OdRxObject* pRxDb = NULL)
  {
    ODA_ASSERT_ONCE(!m_id || id == m_id);
    m_id = id;
    if (!(m_pRxDb = pRxDb))
      m_pRxDb = m_id ? baseDatabaseBy(m_id) : NULL;
    return *this;
  }
  OdPropBase& setRxDb(OdRxObject* pRxDb) // used to get/set props of database
  {
    ODA_ASSERT_ONCE(pRxDb && !m_id); // id is useless for database properties
    m_id = NULL;
    m_pRxDb = pRxDb;
    return *this;
  }
  OdDbStub* id()
  {
    return m_id;
  }
  OdRxObject* baseDatabase()
  {
    return m_pRxDb;
  }
  OdRxObjectPtr object(bool bForWrite = false) const
  {
    ODA_ASSERT_ONCE(m_pRxDb);
    if (!m_id)
      return m_pRxDb; // bForWrite is useless for this case
    OdRxObjectPtr pObj;
    OdDbBaseDatabasePE* pDbPe = OdDbBaseDatabasePE::cast(m_pRxDb).get();
    if (!pDbPe)
      return pObj;
    pObj = pDbPe->openObject(m_id, bForWrite);
    return pObj;
  }
  OdUnitsFormatter* formatter() const
  {
    if (!m_pBaseFormatter && m_pRxDb)
    {
      OdDbBaseDatabasePEPtr pDbPe = OdDbBaseDatabasePE::cast(m_pRxDb);
      if (pDbPe.get())
        m_pBaseFormatter = pDbPe->baseFormatter(m_pRxDb);
    }
    return m_pBaseFormatter;
  }

  // is usable for such classes as assoc array (block reference) to get visual class name
  virtual bool overrideClassName(OdString&) const // sClassName
  {
    return false;
  }

  OdPropServices* propServ() const;

  int toInt(const OdString& sValue)
  {
    return odStrToInt(sValue);
  }

  double toDouble(const OdString& sValue)
  {
    return odStrToD(sValue);
  }

  double toDistance(const OdString& sValue)
  {
    if (formatter())
      return formatter()->unformatLinear(sValue);
    return toDouble(sValue);
  }

  double toScale(const OdString& sValue)
  {
    double val = toDouble(sValue);
    if (OdZero(val))
      throw OdError(eMustBeNonZero);
    return val;
  }

  double toAngle(const OdString& sValue, bool bKeep2Pi = false)
  {
    if (formatter())
    {
      double dAngle = formatter()->unformatAngle(sValue);
      if (   bKeep2Pi && dAngle == 0 
          && sValue == formatter()->formatAngle(  Oda2PI 
                                                - 1.e-10)) // normalize to avoid convert it to 0 by formatter
        dAngle = Oda2PI;
      return dAngle;
    }
    return toDouble(sValue);
  }

  static double toRadian(double deg)
  {
    if (OdZero(deg))
      deg = 0.0;
    double ang = OdaToRadian(deg);
    while (ang < 0.0)
      ang += Oda2PI;
    while (ang > Oda2PI)
      ang -= Oda2PI;
    if (OdEqual(ang, Oda2PI))
      ang = Oda2PI - 1.e-10; // normalize to avoid convert it to 0 by formatter
    return ang;
  }

  static double toDegree(double rad)
  {
    if (OdZero(rad))
      rad = 0.0;
    double ang = OdaToDegree(rad);
    while (ang < 0.0)
      ang += 360.0;
    while (ang > 360.0)
      ang -= 360.0;
    return ang;
  }

  virtual OdSharedPtr<OdCmColorBase> toCmColor(const OdString& sValue)
  {
    if (formatter())
      return formatter()->unformatCmColor(sValue);
    ODA_FAIL_ONCE();
    return OdSharedPtr<OdCmColorBase>();
  }
};
typedef OdSmartPtr<OdPropBase> OdPropBasePtr;
typedef OdPropBasePtr(OdXmlPropCreateFunction)();

inline OdString OdPropContext::toOdString(const OdVariant& vValue, 
                                          const OdString& sValueType, // = tString
                                          bool bIntAsUnsigned) // = false
{
  OdString sValue;
  // see also OdVariant_toString
  switch (vValue.varType())
  {
  case OdVariant::kVoid:
    ODA_FAIL_ONCE(); // test
    return OdString::kEmpty;
  case OdVariant::kBool:
    return vValue.getBool() ? L"true" : L"false";
  case OdVariant::kString:
    return vValue.getString();
  case OdVariant::kInt8: // == OdVariant::kUInt8
    return sValue.format(bIntAsUnsigned ? L"%u" : L"%d", vValue.getInt8());
  case OdVariant::kInt16: // == OdVariant::kUInt16
    return sValue.format(bIntAsUnsigned ? L"%u" : L"%d", vValue.getInt16());
  case OdVariant::kInt32: // == OdVariant::kUInt32
    return sValue.format(bIntAsUnsigned ? L"%lu" : L"%d", vValue.getInt32());
  case OdVariant::kInt64: // == OdVariant::kUInt64
    return sValue.format(L"%" PRIu64W, vValue.getUInt64());
  case OdVariant::kDouble:
    if (sValueType == tDistance && m_pPropBase->formatter())
      return m_pPropBase->formatter()->formatLinear(vValue.getDouble());
    if (sValueType == tAngle && m_pPropBase->formatter())
      return m_pPropBase->formatter()->formatAngle(vValue.getDouble());
    return sValue.format(L"%g", vValue.getDouble());
    break;
  case OdVariant::kWString: // // == OdVariant::kAnsiString
    return OdString(vValue.getAnsiString());
  }
  ODA_FAIL_ONCE(); // TODO
  return OdString::kEmpty;
}

inline OdString OdPropContext::encodeToXmlAttrib(const OdString& csStr)
{
  OdString sStr = csStr;
  sStr.replace(L"&", L"&amp;");
  sStr.replace(L"\"", L"&quot;");
  sStr.replace(L"<", L"&lt;");
  sStr.replace(L">", L"&gt;");
  sStr.replace(L"\'", L"&apos;");
  return sStr;
}

inline OdString OdPropContext::decodeFromXmlAttrib(const OdString& csStr)
{
  OdString sStr = csStr;
  sStr.replace(L"&apos;", L"\'");
  sStr.replace(L"&gt;", L">");
  sStr.replace(L"&lt;", L"<");
  sStr.replace(L"&quot;", L"\"");
  sStr.replace(L"&amp;", L"&");
  return sStr;
}

//------------------------------------------------------------------------
// returned by TConvertorToResBufPropValue for TODO and other unsupported cases
#define TD_MARKER_TO_CANCEL OD_T("*Cancel*") 
// returned by TConvertorToResBufPropValue to repeat call for all selected entities
// and skip resbuf writing (already done via code in convertor
#define TD_MARKER_FOREACH_TO_SET_DIRECTLY OD_T("*Foreach*")
#define TD_MARKER_OUT_OF_RANGE OD_T("*Out of range*")

typedef OdString TConvertorFromResBufPropValue(const OdString& sResBufValues, class OdDbStub* id);
typedef OdString TConvertorToResBufPropValue(const OdString& sPaletteValue, 
                                             // of first selected entity 
                                             // (it is not pRxDatabase because it maybe usable to get current position 
                                             // in advanced dialogs - TODO for General/PlotStyle/Other ...)
                                             OdDbStub* id, 
                                             OdString& sHintGrCode); // out - for alternative gr code - like PloteStyle
typedef bool TConvertorPossibleValues(OdStringArray& values, OdStringArray& iconPaths, 
                                      OdRxObject* pRxDatabase);

class OdControllerOfResBufPropValueConvertors
{
public:
  virtual void addConvertor(const OdString& sConvertorName, // convertor in XML patterns (...="$(convertor,...)"...)
                            TConvertorFromResBufPropValue* pConvertorFrom,
                            TConvertorToResBufPropValue* pConvertorTo,
                            TConvertorPossibleValues* pGetPossibleValues) = 0;
  virtual void removeConvertor(const OdString& sConvertorName) = 0;
  
  // for such args like scale of LINEAR convertor
  virtual OdString getConvertorAdvArg(const OdString& sName) = 0;
  virtual void setConvertorAdvArg(const OdString& sName, const OdString& sArgs) = 0;
};

//------------------------------------------------------------------------

class OdPropServices : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdPropServices);

  typedef enum {
    enSepCatPlace_None,
    enSepCatPlace_Left,
    enSepCatPlace_Right,
    enSepCatPlace_Top,
    enSepCatPlace_Bottom
  } TSeparateCategoryTreePlace;

  // return unique dialog key (sDlgKey)
  virtual OdString openDialog(const OdString& sTitle = OD_T("%ls Property Dialog"), // %ls = product name
                              void* pLayoutQObjectToAddDlg = NULL,
                              bool bRefreshBySetPropEnd = false, // via "<event-refresh:setprop_ended>"
                              TSeparateCategoryTreePlace sepCatPlace = enSepCatPlace_None,
                              double dWidthPercentsOfMain = 0.75,
                              double dHeightPercentsOfMain = 0.5) = 0;

  // in perpendicular direction to current (default internal first layout in vertical)
  virtual bool pushLayout(const OdString& sDlgKey) = 0;
  virtual bool popLayout(const OdString& sDlgKey) = 0;
  virtual bool addSource(const OdString& sDlgKey, 
                         OdPropSource* pSource,
                         const OdString& sXmlBasePath = OdString::kEmpty,
                         const OdString& sXmlRootNodeName = OD_T("Properties"),
                         const OdChar* pcszWidgetAttribs = NULL, // use like "title=:type=palette:type=palette_simple:alignment=top:..."
                         const OdChar cDelimWidgetAttribs = __OD_T(':')) = 0;
  virtual bool setMergeFilter(const OdString& sDlgKey,
                              OdPropSource* pSource,
                              const OdString& sMergeFilter = OdString::kEmpty, // empty for all
                              const OdString& sMergeFilterAttribName =  OD_T("Filter"), // empty is already set by previous call
                              const OdString& sUnmergedBasePath = OdString::kEmpty) = 0; // already set by previous call

  virtual bool executeDialog(const OdString& sDlgKey) = 0;
  // for external update via events 
  // - is used for palettes only (pLayoutQObjectToAddDlg is not NULL)
  virtual bool refreshSources(const OdString& sDlgKey, 
                              OdPropSource* pSourceOnly = NULL, // null for all
                              bool bRecreateWidgets = false) = 0; 

  virtual bool closeDialog(const OdString& sDlgKey) = 0;

  // return index of chosen button
  virtual int messageBox(const OdChar* pszTitle, // %ls = product name (L"%ls Error")
                         const OdChar* pszText,
                         const OdChar* pszButton0 = NULL, // [text_of_button, ... ~text_of_button_in focus, ... text_of_button,] NULL(should be at end - is used as end of arguments)
                         ...) = 0;

  virtual class OdDbBaseHostAppServices* getBaseHostAppServices() = 0;
  virtual OdString tr(const char* pcszText) = 0; // translate via Qt Dictionaties (via QObject::tr(pcszText))
  virtual bool isInputStringReady(OdEdCommandContext* pCmdCtx) = 0;

  virtual void registerXmlProp(const OdString& sClassName, OdXmlPropCreateFunction* pFunc) = 0;
  virtual void unregisterXmlProp(const OdString& sClassName) = 0;

  virtual OdString collectXmlProps(const OdString& sXmlBasePath,
                                   OdDbStub* id, const OdStringArray& asClassNames,
                                   int flagsToUse, OdRxObject* pRxDb = NULL) = 0;
  virtual bool getPossibleValuesXmlProps(OdDbStub* id, const OdStringArray& asClassNames,
                                         const OdString& sPathName, 
                                         OdStringArray& values, OdStringArray& iconPaths,
                                         int flagsToUse, OdRxObject* pRxDb = NULL) = 0;
  // return error message (an empty string if OK)
  virtual OdString setValueXmlProps(OdDbStub* id, const OdStringArray& asClassNames,
                                    const OdString& sPathName, const OdString& sValue,
                                    const OdString& sHintGrCode, // for alternative gr code - like PloteStyle
                                    OdPropSource::Action& action, // out
                                    int flagsToUse, OdRxObject* pRxDb = NULL) = 0;

  // is used to convert value from resbuf string form to palette one and back
  virtual OdControllerOfResBufPropValueConvertors* controllerOfResBufPropValueConvertors() = 0;
  virtual void setControllerOfResBufPropValueConvertors(OdControllerOfResBufPropValueConvertors* pController) = 0;

  virtual OdString colorBaseToStr(OdRxObject* pRxDb, const OdCmColorBase& color) = 0;
  virtual bool colorBaseByStr(OdRxObject* pRxDb, const OdString& sColor, OdCmColorBase& colorDest) = 0;
  virtual OdString linetypeToStr(OdRxObject* pRxDb, OdDbStub* id) = 0;
  virtual bool linetypeByStr(OdRxObject* pRxDb, const OdString& sLinetype, OdDbStub** pIdDest) = 0;
  virtual OdString lineweightToStr(OdRxObject* pRxDb, int nLineWeight, bool bLineWeightAsIndex = false) = 0;
  virtual bool lineweightByStr(OdRxObject* pRxDb, const OdString& sLineweight, OdDb::LineWeight& lwDest) = 0;
  virtual OdString transparencyToStr(OdRxObject* pRxDb, const class OdCmTransparency& transparency) = 0;
  virtual bool transparencyByStr(OdRxObject* pRxDb, const OdString& sTransparency, OdCmTransparency& transparencyDest) = 0;

  virtual bool overrideCategoryWeight(OdRxObject* pRxDb, const OdString& sCat,
                                      float& weight) = 0; // out
  virtual bool overrideCategoryName(OdRxObject* pRxDb, OdString& sCat, // in / out
                                    const OdString& sHintPropName, int propSrcFlags) = 0;
  virtual bool overridePropertyName(OdRxObject* pRxDb, const OdString& sCat,
                                    OdString& sName, OdString* psVisName = NULL) = 0; // in / out
  virtual bool overridePropertyType(OdRxObject* pRxDb, const OdString& sCat, const OdString& sName,
                                    OdString& type) = 0; // in / out
  virtual bool overrideClassName(OdRxObject* pRxDb, OdDbStub* id,
                                 OdString& sClassName) = 0; // in / out
};
#define OD_PROP_SERVICES OD_T("OdPropServices")
typedef OdSmartPtr<OdPropServices> OdPropServicesPtr;

inline OdPropServices* OdPropBase::propServ() const
{
  if (!m_pPropServ)
    m_pPropServ = OdPropServices::cast(::odrxSysRegistry()->getAt(OD_PROP_SERVICES).get());
  ODA_ASSERT_ONCE(m_pPropServ);
  return m_pPropServ;
}

#include "TD_PackPop.h"

#endif // OD_PROP_SERVICES_H_
