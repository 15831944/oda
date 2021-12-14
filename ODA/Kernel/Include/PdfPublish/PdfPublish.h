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
// PdfPublish.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_INCLUDED_
#define _PDF_PUBLISH_INCLUDED_

#include "PdfPublishDef.h"
#include "RxModule.h"

#include "PdfPublishDocument.h"

/** \details 
  Namespace contains declarations for the Publish SDK.
  
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements file level operations for publishing drawings in .pdf format.
*/
class PDFPUBLISH_TOOLKIT OdFile : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdFile);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
  Creates a file object for publishing.
  */
  OdFile();
  
  /** \details
  Destroys the file object for publishing.
  */
  virtual ~OdFile();

  /** \details
    Writes specified document content to a specified file.
    \param doc  [in] A smart pointer to the <link OdPdfPublish::OdDocument, document> instance.
    \param file [in] A full path to the output file.
    
    \return Returns the result of the write operation. If data has been written successfully, returns eOk, otherwise returns an error code.
    
    \remarks 
    If a PDFToolkit exception occurs, the method returns an error code from the PDFToolkit level. 
    If another exception occurs, the method returns the particular error code.
    If an unknown exception occurs, the method returns the eInternalError value.
  */
  OdUInt32 exportPdf(OdDocumentPtr& doc, const OdString& file);
  
  /** \details
    Writes specified document content to a specified data stream.
    \param doc    [in] A smart pointer to the <link OdPdfPublish::OdDocument, document> instance.
    \param output [in] A smart pointer to the output data stream buffer.
    
    \return Returns the result of the write operation. If data has been written successfully, returns eOk, otherwise returns an error code.
    
    \remarks 
    If a PDFToolkit exception occurs, the method returns an error code from the PDFToolkit level. 
    If another exception occurs, the method returns the particular error code.
    If an unknown exception occurs, the method returns the eInternalError value.
  */
  OdUInt32 exportPdf(OdDocumentPtr& doc, OdStreamBufPtr& output);
};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdFile, OdFile> object.
*/
SMARTPTR(OdFile);

}
#endif

