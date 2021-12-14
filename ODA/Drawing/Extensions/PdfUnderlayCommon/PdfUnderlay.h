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


///////////////////////////////////////////////////////////////////////////////
//
// PdfUnderlay.h - Open Design auxiliary export methods for support of snap points in Pdf Underlays.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_UNDERLAY_INCLUDED_
#define _PDF_UNDERLAY_INCLUDED_

#include "TD_PackPush.h"

#include "OdPlatformSettings.h"
#include "Gi/GiGeometry.h"
#include "DbUnderlayReference.h"
#include "DbUnderlayHost.h"
#include "RxModule.h"
#include "StaticRxObject.h"

/** \details
Retrieves snap PDF geometry for the referenced PDF entity.
\param pPdfRef  [in]  A PDF reference object that refers to the PDF content.
\param pGrabber [out] A raw pointer to the geometry grabber object. The method fills this object with exploded geometry. 
\param pickPtMc [in]  A raw pointer to the snap points array.
\returns Returns a value of the OdResult enumeration that contains the result of the operation.
*/
OdResult OD_DLL_EXPORT getSnapGeometry(const OdDbPdfReferencePtr &pPdfRef, OdGiGeometry *pGrabber, const OdGePoint3d* pickPtMc);

class OdDbUnderlayHostPE;

/** \details
The class provides module with functionality for working with PDF underlay entities.
<group PdfServices_Classes>
\remarks
Library:  Source code provided.
*/
class OD_DLL_EXPORT PdfUnderlayModule : public OdRxModule
{
  //DOM-IGNORE-BEGIN
  OdDbUnderlayHostPEPtr m_PdfHostPE;
  //DOM-IGNORE-END
public:
  /** \details
  Creates a new instance of the PdfUnderlay module.
  */
  PdfUnderlayModule();

  /** \details
  Initializes a PDF underlay application.
  */
  virtual void initApp();
  
  /** \details
  Uninitializes a PDF underlay application.
  */
  virtual void uninitApp();

  /** \details
  Retrieves exploded PDF geometry for specified referenced PDF entity.
  \param pPdfRef        [in]  A PDF reference object that refers to the PDF content.
  \param pGrabber       [out] A raw pointer to the geometry grabber object. The method fills this object with exploded geometry. 
  \param bIgnoreImages  [in]  A boolean flag value that determines whether images are ignored during exploding process (if is equal to true).
  \returns Returns a value of the OdResult enumeration that contains the result of the explode operation.
  */
  virtual OdResult getExplodeGeometry(const OdDbPdfReferencePtr &pPdfRef, OdGiGeometry *pGrabber, bool bIgnoreImages);

  //DOM-IGNORE-BEGIN
  /** \details
  This method is for internal usage only and should not be called at the custom side.
  */
  virtual OdResult forInternalUseOnly(OdUInt32 n, void *p);
  //DOM-IGNORE-END

  /** \details
  Retrieves snap PDF geometry for a specified referenced PDF entity.
  \param pPdfRef  [in]  A PDF reference object that refers to the PDF content.
  \param pGrabber [out] A raw pointer to the geometry grabber object. The method fills this object with exploded geometry. 
  \param pickPtMc [in]  A raw pointer to the snap points array.
  \returns Returns a value of the OdResult enumeration that contains the result of the operation.
  */
  virtual OdResult getSnapGeometry(const OdDbPdfReferencePtr &pPdfRef, OdGiGeometry *pGrabber, const OdGePoint3d* pickPtMc);

  /** \details
  Destroys the PDF underlay module instance.
  */
  ~PdfUnderlayModule();
};

/** \details
A data type that represents a smart pointer to an <link PdfUnderlayModule, PdfUnderlayModule> object.
*/
typedef OdSmartPtr<PdfUnderlayModule> PdfUnderlayModulePtr;

#include "TD_PackPop.h"

#endif // _PDF_UNDERLAY_INCLUDED_

