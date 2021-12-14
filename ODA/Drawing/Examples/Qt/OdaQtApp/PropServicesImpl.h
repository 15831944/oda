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
// PropServicesImpl.h
//

#ifndef OD_PROP_SERVICES_IMPL_H_
#define OD_PROP_SERVICES_IMPL_H_

#include "PropServices.h"

class OdqXmlUtils;
class OdqPropertyDialog;
class OdqPropDialogData;

///////////////////////////////////////////////////////////////////////////////
// casted from OdEdCommandContext

class OdPropServicesImpl : public OdPropServices
{
protected:
  OdPropServicesImpl();
public:
  ODRX_DECLARE_MEMBERS(OdPropServicesImpl);
  ~OdPropServicesImpl();

  // return unique dialog key (sDlgKey)
  virtual OdString openDialog(const OdString& sTitle = L"%ls Property Dialog", // %ls = product name
                              void* pLayoutQObjectToAddDlg = NULL,
                              bool bRefreshBySetPropEnd = false, // via "<event-refresh:setprop_ended>"
                              TSeparateCategoryTreePlace sepCatPlace = enSepCatPlace_None,
                              double dWidthPercentsOfMain = 0.75,
                              double dHeightPercentsOfMain = 0.5);

  // in perpendicular direction to current (default internal first layout in vertical)
  virtual bool pushLayout(const OdString& sDlgKey);
  virtual bool popLayout(const OdString& sDlgKey);
  virtual bool addSource(const OdString& sDlgKey, 
                         OdPropSource* pSource,
                         const OdString& sXmlBasePath = OdString::kEmpty,
                         const OdString& sXmlRootNodeName = OD_T("Properties"),
                         const OdChar* pcszWidgetAttribs = NULL, // use like "title=:type=palette:type=palette_simple:alignment=top:..."
                         const OdChar cDelimWidgetAttribs = __OD_T(':'));
  virtual bool setMergeFilter(const OdString& sDlgKey, 
                              OdPropSource* pSource,
                              const OdString& sMergeFilter = OdString::kEmpty, // empty for all
                              const OdString& sMergeFilterAttribName =  OD_T("Filter"), // empty is already set by previous call
                              const OdString& sUnmergedBasePath = OdString::kEmpty); // already set by previous call

  virtual bool executeDialog(const OdString& sDlgKey);
  // for external update via events 
  // - is used for palettes only (pLayoutQObjectToAddDlg is not NULL)
  virtual bool refreshSources(const OdString& sDlgKey,
                              OdPropSource* pSourceOnly = NULL, // null for all
                              bool bRecreateWidgets = false); 

  virtual bool closeDialog(const OdString& sDlgKey);
  
  // return index of chosen button
  virtual int messageBox(const OdChar* pszTitle, // %ls = product name (L"%ls Error")
                         const OdChar* pszText,
                         const OdChar* pszButton0 = NULL, // [text_of_button, ... ~text_of_button_in focus, ... text_of_button,] NULL(should be at end - is used as end of arguments)
                         ...);

  virtual class OdDbBaseHostAppServices* getBaseHostAppServices();
  virtual OdString tr(const char* pcszText); // translate via Qt Dictionaties (via QObject::tr(pcszText))
  virtual bool isInputStringReady(OdEdCommandContext* pCmdCtx);

  virtual void registerXmlProp(const OdString& sClassName, OdXmlPropCreateFunction* pFunc);
  virtual void unregisterXmlProp(const OdString& sClassName);
  virtual bool isRegisteredXmlProp(const OdString& sClassName);

  virtual OdString collectXmlProps(const OdString& sXmlBasePath,
                                   OdDbStub* id, const OdStringArray& asClassNames, 
                                   int flagsToUse, OdRxObject* pRxDb = NULL);
  virtual bool getPossibleValuesXmlProps(OdDbStub* id, const OdStringArray& asClassNames,
                                         const OdString& sPathName, 
                                         OdStringArray& values, OdStringArray& iconPaths, 
                                         int flagsToUse, OdRxObject* pRxDb = NULL);
  // return error message (an empty string if OK)
  virtual OdString setValueXmlProps(OdDbStub* id, const OdStringArray& asClassNames,
                                    const OdString& sPathName,
                                    const OdString& sValue,
                                    const OdString& sHintGrCode, // for alternative gr code - like PloteStyle
                                    OdPropSource::Action& action, 
                                    int flagsToUse, OdRxObject* pRxDb = NULL);

  // is used to convert value from resbuf string form to palette one and back
  virtual OdControllerOfResBufPropValueConvertors* controllerOfResBufPropValueConvertors();
  virtual void setControllerOfResBufPropValueConvertors(OdControllerOfResBufPropValueConvertors* pController);

  virtual OdString colorBaseToStr(OdRxObject* pRxDb, const OdCmColorBase& color);
  virtual bool colorBaseByStr(OdRxObject* pRxDb, const OdString& sColor, OdCmColorBase& colorDest);

  virtual OdString linetypeToStr(OdRxObject* pRxDb, OdDbStub* id);
  virtual bool linetypeByStr(OdRxObject* pRxDb, const OdString& sLinetype, OdDbStub** pIdDest);

  virtual OdString lineweightToStr(OdRxObject* pRxDb, int nLineWeight, bool bLineWeightAsIndex = false);
  virtual bool lineweightByStr(OdRxObject* pRxDb, const OdString& sLineweight, OdDb::LineWeight& lwDest);

  virtual OdString transparencyToStr(OdRxObject* pRxDb, const class OdCmTransparency& transparency);
  virtual bool transparencyByStr(OdRxObject* pRxDb, const OdString& sTransparency, OdCmTransparency& transparencyDest);

  virtual bool overrideCategoryWeight(OdRxObject* pRxDb, const OdString& sCat,
                                      float& weight); // out
  virtual bool overrideCategoryName(OdRxObject* pRxDb, OdString& sCat, // in / out
                                    const OdString& sHintPropName, int propSrcFlags);
  virtual bool overridePropertyName(OdRxObject* pRxDb, const OdString& sCat,
                                    OdString& sName, OdString* psVisName = NULL); // in / out
  virtual bool overridePropertyType(OdRxObject* pRxDb, const OdString& sCat, const OdString& sName,
                                    OdString& type); // in / out
  virtual bool overrideClassName(OdRxObject* pRxDb, OdDbStub* id,
                                 OdString& sClassName); // in / out

  void saveCatWeights();
protected:
  void loadCatWeights();

  OdqPropDialogData* getDialogData(const OdString& sDlgKey,
                                   QMap<OdString, OdSharedPtr<OdqPropDialogData> >::iterator* pItr = NULL);
  QString getPlacementKey(OdqPropertyDialog* pDlg);

  OdPropBasePtr createXmlPropByClassName(const OdString& sClassName, 
                                         int propSrcFlags,
                                         OdDbStub* id, OdRxObject* pRxDb = NULL);

  OdSharedPtr<OdqXmlUtils>& loadXmlPattern(const QString& qsClassName);
  QString collectByPatternUtils(const OdString& sXmlBasePath, 
                                OdqXmlUtils& utils, class OdqResBufPropWrapper& data); // in 
  // return true if path was found
  bool setValueByPatternUtils(OdqXmlUtils& utils, OdqResBufPropWrapper& data,
                              const QString& qsPathName, const QString& qsValue,
                              const QString& qsHintGrCode, // for alternative gr code - like PloteStyle
                              OdPropSource::Action& action, QString& qsError); // out

  QStack<OdString> m_stackDlgKeys;
  QMap<OdString, OdSharedPtr<OdqPropDialogData> > m_mapData;

  QMap<OdString, OdXmlPropCreateFunction*> m_mapXmlPropCreatorsByClassName; // more priority than way via patterns

  QHash<QString, OdSharedPtr<OdqXmlUtils> > m_mapXmlPatterns;

  // is used to convert value from resbuf string form to palette one and back
  OdControllerOfResBufPropValueConvertors* m_pStrConvController;

  QMap<OdString, float> m_mapCatWeights;
};
//typedef OdSmartPtr<OdPropServicesImpl> OdPropServicesImplPtr;

void initOdPropServices();
void uninitOdPropServices();

#endif // OD_PROP_SERVICES_IMPL_H_
