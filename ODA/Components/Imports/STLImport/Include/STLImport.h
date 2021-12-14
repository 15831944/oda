/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2020, Open Design Alliance (the "Alliance"). 
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
//   Open Design Alliance Copyright (C) 2002-2020 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

#ifndef STLFILEIMPORT_H
#define STLFILEIMPORT_H

#include "TD_PackPush.h"

#include "Ge/GePoint3d.h"
#include "CmColorBase.h"

#include "RxObject.h"
#include "RxModule.h"
#include "DynamicLinker.h"
#include "Ge/GeVector3dArray.h"
#include "UInt32Array.h"

#ifdef STLIMPORT_EXPORTS
#define STLIMPORT_TOOLKIT         OD_TOOLKIT_EXPORT
#define STLIMPORT_TOOLKIT_STATIC  OD_STATIC_EXPORT
#else                               
#define STLIMPORT_TOOLKIT         OD_TOOLKIT_IMPORT
#define STLIMPORT_TOOLKIT_STATIC  OD_STATIC_IMPORT
#endif

namespace STLFileImport {

enum OdStlImportRes
{
    eNoError               = 0
//Read errors:
  , eNoFile                = 1
  , eFileTooShort          = 2
  , eEndSolidNotFound      = 3
  , eFailOnReadingFacet    = 4
  , eNormalNotFound        = 7
  , eEndFacetNotFound      = 8
  , eFailOnReadingOuter    = 9
  , eLoopWordNotFound      = 10
  , eEndLoopNotFound       = 11
  , eFailOnReadingVertex   = 12
  , eFailOnReadingNormal   = 13
  , eInvalidNumber         = 14
//GetShellErrors:
  , eImportCallReturnError = 15
  , eMultiOuterFace        = 16
  , eOuterNotATriangle     = 17
};


/** \details
This is an interface class implements the STL import.
*/
class STLIMPORT_TOOLKIT OdStlImport : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdStlImport);

  virtual ~OdStlImport(){};

  virtual OdStlImportRes import(const OdString &strFilePath) = 0;
  virtual OdStlImportRes import(const OdStreamBufPtr& pStreamBuf) = 0;

  /* The GetShellFlags enum describes the flags according to which the 
     requested data will be calculated.

     kFixNormals -              check the normals and correct them in two cases:
                                      1) zero normals;
                                      2) normals does not agree with the normal calculated according to the right-hand rule;
                                The 'normals' array will contain the normals at faces.

     kUnifyDuplicatedVertices - perform the unifying the duplicated vertices.
                                The 'vertices' array will contain only non-repeating vertices

     kCalcNormalsAtvertices   - perform the primitive calculation of the normals at vertices.
                                Normals at vertices can used in Smooth Shaded rendering modes
                                1) If the flag 'kUnifyDuplicatedVertices' is off, the normals at vertices is simply equal to the 
                                normals at face, which contain the vertex
                                2) If the flag 'kUnifyDuplicatedVertices' is on, the normals at vertices is calculated
                                by averaging the normals at faces which contain the vertex.
                                This method is very rough and works fine only for the cases of smooth shells, while, for example,
                                in case of cube it gives bad results. In such cases it is need to use more clever algorithms at the user side/
                                The 'normals' array will contain the normals at vertices.
  */
  enum GetShellFlags
  {
    kFixNormals = 1,
    kUnifyDuplicatedVertices = 2,
    kCalcNormalsAtVertices   = 4
  };

  /* The getShell method will return the parameters of the shell on basis of the readed data and according to the 
     input 'GetShellFlags'
  */
  virtual OdStlImportRes getShell(OdGePoint3dArray     &vertices,
                                  OdUInt32Array        &faces,
                                  OdGeVector3dArray    &normals,
                                  OdUInt8              flags = 0) const = 0;

  /* The 'getSolidName' method returns the name of the solid in the case of ASCII file and the string "STL_BINARY" for the binary STL.
  */
  virtual OdStlImportRes getSolidName(OdString& name) const = 0;
};
typedef OdSmartPtr<OdStlImport> OdStlImportPtr;

/** \details
This class implements the STL import module.
*/
class STLIMPORT_TOOLKIT OdStlImportModule : public OdRxModule
{
public:
  virtual OdStlImportPtr create() = 0;
};
typedef OdSmartPtr<OdStlImportModule> OdStlImportModulePtr;



inline OdStlImportPtr createStlImporter()
{
  OdStlImportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdStlImportModuleName);
  if (!pModule.isNull()) return pModule->create();
  return (OdStlImport*)0;
}

}//namespace STLFileImport

#include "TD_PackPop.h"

#endif

