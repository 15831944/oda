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
// OdqPlotSettingsValidatorPEImpl.h 
//

#ifndef ODA_QT_DBPLOTSETVALPEIMPL_H
#define ODA_QT_DBPLOTSETVALPEIMPL_H

#include "TD_PackPush.h"
#include "DbPlotSettingsValidatorPE.h"

/** Description:
    This class is the Plot Settings Validator Protocol Extension class implementation.
    library: Source *code* provided. 
    {group:Qt_Classes} 
*/
class OdqPlotSettingsValidatorPEImpl : public OdDbPlotSettingsValidatorPE
{
public:

  /** Description:
    Returns a list of plot device names for this ValidatorPE object.
    Arguments:
    listDevices (O) Receives the list of device names.
  */
  virtual OdResult getDeviceList(OdArray<OdString>& listDevices);
  /** Description:
    Returns a list of media names for the specified device name for this ValidatorPE object.
    Arguments:
    listMedia (O) Receives the list of media names.
    sDeviceName (I) Device name.
  */
  virtual OdResult getMediaList(const OdString& sDeviceName, 
                                OdArray<psvPaperInfo>& listMedia, 
                                bool bUpdateMediaMargins);

  /** Description:
  Returns a default media name for the specified device name for this ValidatorPE object.
  Arguments:
  sDefaultMedia (O) Default media name.
  sDeviceName (I) Device name.
  Note : sDefaultMedia is local (not canonical) name.
  */
  virtual OdResult getDefaultMedia(const OdString& sDeviceName,
                                   OdString& sDefaultMedia);
  /** Description:
    Returns a list of plot style sheet names for this ValidatorPE object.
    Arguments:
    listPlotStyleSheets (O) Receives the list of plot style sheet names.
  */
  virtual OdResult getPlotStyleSheetList(OdArray<OdString>& listPlotStyleSheets, OdDbBaseHostAppServices *pHostApp);

  virtual OdResult getMediaMargins(const OdString& sDeviceName, psvPaperInfo& infoMedia);
};

#include "TD_PackPop.h"

#endif // ODA_QT_DBPLOTSETVALPEIMPL_H
