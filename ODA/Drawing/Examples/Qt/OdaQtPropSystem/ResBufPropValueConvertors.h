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
// ResBufPropValueConvertors.h
//

#if !defined(RESBUF_PROP_VALUE_CONVERTORS_H_) && !defined(OD_RB_VALUE_CONV)
#define RESBUF_PROP_VALUE_CONVERTORS_H_

#include "PropServices.h"

#define STL_USING_MAP
#include "OdaSTL.h"

#include "TD_PackPush.h"

class OdResBufPropControllerImpl : public OdControllerOfResBufPropValueConvertors
{
private:
  OdResBufPropControllerImpl();

  struct CallBacks
  {
    CallBacks();

    TConvertorFromResBufPropValue* m_fnFromRB;
    TConvertorToResBufPropValue* m_fnToRB;
    TConvertorPossibleValues* m_fnGetPossible;
  };
  std::map<OdString, CallBacks> m_mapCallbacks; // by convertor in XML patterns (...="$(convertor,...)"...)
  std::map<OdString, OdString> m_mapConvertorNames; // by path in XML patterns (parh/name)

  std::map<OdString, OdString> m_mapAdvArgsToSetValue; // by path in XML patterns (parh/name)
  OdString m_sCurrentPath;

public:

  virtual void addConvertor(const OdString& sConvertorName,
                            TConvertorFromResBufPropValue* pConvertorFrom,
                            TConvertorToResBufPropValue* pConvertorTo,
                            TConvertorPossibleValues* pGetPossibleValues);
  virtual void removeConvertor(const OdString& sConvertorName);

  virtual OdString getConvertorAdvArg(const OdString& sName);
  virtual void setConvertorAdvArg(const OdString& sName, const OdString& sArgs); // for such args like scale of LINEAR convertor
  OdString setCurrentPath(const OdString& sPath);

  void addPath(const OdString& sPath, const OdString& sConvertorName);
  TConvertorFromResBufPropValue* getConvFuncFromRB(const OdString& sPath);
  TConvertorToResBufPropValue* getConvFuncToRB(const OdString& sPath);
  TConvertorPossibleValues* getConvFuncGetPossible(const OdString& sPath);

  static void init();
  static void uninit();
};

OdResBufPropControllerImpl& getResBufPropController();

#include "TD_PackPop.h"

#endif // RESBUF_PROP_VALUE_CONVERTORS_H_

//------------------------------------------------------------------------
//OD_RB_VALUE_CONV(KEY, true/false for fromResBUF, true/false for toResBUF, true/false for possibleValyes)

#ifdef OD_RB_VALUE_CONV

  // convertors for LINETYPE and LINEWEIGHT are automatically added 
  // in PropServicesImpl.cpp via setContoller (see ResBufPropValueBaseConvertors.h)

  OD_RB_VALUE_CONV(LAYER, false, false, true)
  OD_RB_VALUE_CONV(PLOTSTYLE, true, true, true)
  OD_RB_VALUE_CONV(MATERIAL, true, true, true)
  OD_RB_VALUE_CONV(HYPERLINK, true, true, false)

  OD_RB_VALUE_CONV(LINEAR, true, true, false)
  OD_RB_VALUE_CONV(ANGULAR, true, false, false)

  OD_RB_VALUE_CONV(X, true, true, false)
  OD_RB_VALUE_CONV(Y, true, true, false)
  OD_RB_VALUE_CONV(Z, true, true, false)

  OD_RB_VALUE_CONV(START, true, true, false)
  OD_RB_VALUE_CONV(END, true, true, false)
  OD_RB_VALUE_CONV(DELTA, true, false, false)
  OD_RB_VALUE_CONV(LENGTH, true, false, false)
  OD_RB_VALUE_CONV(ANGLE, true, false, false)
  OD_RB_VALUE_CONV(AREA, true, false, false)
  OD_RB_VALUE_CONV(RADIUS, true, true, false)
  OD_RB_VALUE_CONV(AXIS, true, false, false)

#undef OD_RB_VALUE_CONV
#endif // OD_RB_VALUE_CONV
