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

#ifndef _DGNEXPORT_INCLUDED_
#define _DGNEXPORT_INCLUDED_

#include "RxModule.h"
#include "RxDictionary.h"
#include "DynamicLinker.h"
#include "OdPlatformSettings.h"

class OdDbDatabase;

//DD:EXPORT_ON

/** \details
  <group OdExport_Classes> 
*/
namespace TD_DGN_EXPORT {

/** \details
  This class implements the DGN exporter.
*/
class OdDgnExport : public OdRxObject
{
public:
  enum ExportResult { success, fail, bad_password, bad_file, bad_database, encrypted_file};
  virtual ExportResult exportDb() = 0;

  /** \details
    Passes parameters for Dgn exporting.
    
    \remarks
  
    See the Drawings SDK Developer Guide, DGN Export Parameters for the list of possible parameters.
  */

  virtual OdRxDictionaryPtr properties() = 0;
};

typedef OdSmartPtr<OdDgnExport> OdDgnExportPtr;

/** \details
  This class implements the DGN import line weights map.
*/

class OdDgnExportLineWeightsMap;
typedef OdSmartPtr<OdDgnExportLineWeightsMap> OdDgnExportLineWeightsMapPtr;

class OdDgnExportLineWeightsMap : public OdRxObject
{
public:
  virtual OdResult         setDgnIndexForDwgLineWeight(OdDb::LineWeight uDwgLineWeigth, OdUInt32 uDgnIndex ) = 0;
  virtual void             copyLineWeightsMap(const OdDgnExportLineWeightsMapPtr& pMap) = 0;
  virtual OdUInt32         getDgnIndexByDwgLineWeight(OdDb::LineWeight uDwgLineWeigth) const = 0;
  virtual void             resetToDefaults() = 0;
  virtual OdArray< OdDb::LineWeight> getOverridedDwgLineWeights() const = 0;
};

/** \details
  This class implements the DGN export module.
*/
class OdDgnExportModule : public OdRxModule
{
public:
    virtual OdDgnExportPtr create () = 0;
    
};

typedef OdSmartPtr<OdDgnExportModule> OdDgnExportModulePtr;

inline OdDgnExportPtr createDgnExporter()
{
     OdDgnExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDgnExportModuleName);
     if ( !pModule.isNull() ) return pModule->create ();
     return (OdDgnExport*)0;
}

}
//DD:EXPORT_OFF

#endif // _DGNEXPORT_INCLUDED_
