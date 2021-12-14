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

#ifndef _IFC_EXPORT_H_
#define _IFC_EXPORT_H_

#include "OdPlatformSettings.h"
#include "OdModuleNames.h"
#include "RxModule.h"
#include "RxDictionary.h"
#include "DynamicLinker.h"
#include "DbObjectId.h"

#ifdef IFC2DWG_EXPORTS
  #define IFC2DWG_EXPORT          OD_TOOLKIT_EXPORT
  #define IFC2DWG_EXPORT_STATIC   OD_STATIC_EXPORT
#else                               
  #define IFC2DWG_EXPORT          OD_TOOLKIT_IMPORT
  #define IFC2DWG_EXPORT_STATIC   OD_STATIC_IMPORT
#endif

#define ODRX_DECLARE_IFC2DWG_STATIC_MODULES_ENTRY_POINTS() \
  ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdSDAIModule); \
  ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdIfcCoreModule); \
  ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdIfc2x3Module); \
  ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdIfc4Module); \
  ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdIfcGeomModuleImpl); \
  ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdIfcBrepBuilderModule); \
  ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdIfcFacetModelerModule); \
  ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdIfc2DwgModuleImpl);
// end of ODRX_DECLARE_IFC2DWG_STATIC_MODULES_ENTRY_POINTS macro

#define ODRX_DEFINE_IFC2DWG_STATIC_APPMODULES() \
  ODRX_DEFINE_STATIC_APPMODULE(OdSDAIModuleName, OdSDAIModule) \
  ODRX_DEFINE_STATIC_APPMODULE(OdIfcCoreModuleName, OdIfcCoreModule) \
  ODRX_DEFINE_STATIC_APPMODULE(OdIfc2x3ModuleName, OdIfc2x3Module) \
  ODRX_DEFINE_STATIC_APPMODULE(OdIfc4ModuleName, OdIfc4Module) \
  ODRX_DEFINE_STATIC_APPMODULE(OdIfcGeomModuleName, OdIfcGeomModuleImpl) \
  ODRX_DEFINE_STATIC_APPMODULE(OdIfcBrepBuilderModuleName, OdIfcBrepBuilderModule) \
  ODRX_DEFINE_STATIC_APPMODULE(OdIfcFacetModelerModuleName, OdIfcFacetModelerModule) \
  ODRX_DEFINE_STATIC_APPMODULE(OdIfc2DwgModuleName, OdIfc2DwgModuleImpl)
// end of ODRX_DEFINE_IFC2DWG_STATIC_APPMODULES macro

class OdDbDatabase;

class OdIfcFile;

/** \details 
A data type that represents a smart pointer to an OdIfcFile object.
*/
typedef OdSmartPtr<OdIfcFile> OdIfcFilePtr;

/** \details 
Contains declarations related to the functionality of conversion data from the IFC format to a .dwg file.
*/
namespace TD_IFC_EXPORT {

  /** \details 
  Contains declarations, which represents the .ifc to .dwg conversion modes.
  */
  enum ExportMode
  {
    /**Export 3D geometry as polyface mesh.*/
    kAsPolyFaceMesh,
    
    /**Export 3D geometry as subDMeshes.*/
    kAsSubDMesh
  };

  /** \details
  The class that implements a connection map between .dwg entities and appropriate objects in the IFC model.
  This map contains information about data storages in the .ifc and .dwg formats used for the data exchange.
  */
  class IFC2DWG_EXPORT OdIfcConnectionMap : public OdRxObject
  {
  public:

    /** \details
    Searches the corresponding IFC entity represented with its object identifier in the destination drawinging database. 
    \param id [in] An identifier of the entity in the destination drawing database.
    \returns A handle of the found entity.
    */
    virtual OdDbHandle find(OdDbObjectId id) = 0;

    /** \details
    Retrieves the attached IFC file object that contains the source IFC model.
    \returns A smart pointer to the attached instance of the OdIfcFile class.
    */
    virtual OdIfcFilePtr getIfcFile() = 0;

  };
  
  /** \details 
  A data type that represents a smart pointer to an object of the OdIfcConnectionMap class.
  */
  typedef OdSmartPtr<OdIfcConnectionMap> OdIfcConnectionMapPtr;

  /** \details 
  An abstract class that provides the interface for .ifc to .dwg format data conversion.
  */
  class OdIfcExport : public OdRxObject
  {
  public:
    
    /** \details 
    Contains .ifc to .dwg format conversion results.
    */
    enum ExportResult 
    {
      /**The conversion is successfully done.*/
      success, 
      /**The conversion is failed.*/
      fail, 
      /**The password is incorrect.*/
      bad_password, 
      /**The file is invalid.*/
      bad_file, 
      /**The input .ifc file schema is not supported.*/
      schema_not_supported, 
      /**The output .dwg database is invalid.*/
      bad_database, 
      /**The conversion failed because of a file internal error.*/
      file_internal_error
    };
  
    /** \details
    A pure virtual method that provides an interface for the conversion of an input .ifc file to a .dwg file.
    \returns The ExportResult::success value if the conversion process is successful; otherwise, the method returns an appropriate error code. 
    \remarks 
    Use the properties() method to set the conversion process options.
    */
    virtual ExportResult exportIfc() = 0;

    /** \details
    A pure virtual method that provides an interface for access to the properties of the .ifc to .dwg conversion process.
    \returns A smart pointer to an Rx dictionary object that contains the conversion properties values.
    \remarks
    See <link ifc_ifc2dwg_module, Export IFC to a .dwg Database> topic for the list of possible parameters.
    */
    virtual OdRxDictionaryPtr properties() = 0;
  };
  
  /** \details 
  A data type that represents a smart pointer to an object that is derived from the OdIfcExport class.
  */
  typedef OdSmartPtr<OdIfcExport> OdIfcExportPtr;

  /** \details
  An abstract class that provides the interface for the .ifc to .dwg format data conversion module.
  */
  class OdIfc2DwgModule : public OdRxModule
  {
  public:

    /** \details 
    A pure virtual method that provides an interface for the creation of a new .ifc to .dwg converter instance.
    \returns A smart pointer to the created object.
    */  
    virtual OdIfcExportPtr create () = 0;
  };
  
  /** \details 
  A data type that represents a smart pointer to an object that is derived from the OdIfc2DwgModule class.
  */
  typedef OdSmartPtr<OdIfc2DwgModule> OdIfc2DwgModulePtr;

  /** \details 
  Creates an instance of the .ifc to .dwg format converter.
  \returns A smart pointer to an object derived from the OdIfc2DwgModule class.
  */
  inline OdIfcExportPtr createIfcExporter()
  {
    OdIfc2DwgModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdIfc2DwgModuleName);
    if (!pModule.isNull())
      return pModule->create();
    return OdIfcExportPtr();
  }

}

#endif // _IFC_EXPORT_H_
