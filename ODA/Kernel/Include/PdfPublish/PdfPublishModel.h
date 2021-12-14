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
// PdfPublishModel.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_MODEL_INCLUDED_
#define _PDF_PUBLISH_MODEL_INCLUDED_

#include "PdfPublishBaseObject.h"

/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements a three-dimensional CAD model that is the source of a .pdf document created with Publish SDK.
  The CAD model source contains drawing data in PRC format.
*/
class PDFPUBLISH_TOOLKIT OdCADModel : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdCADModel();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdCADModel);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END
  
  /** \details
    Destroys the instance of a CAD model.
  */
  virtual ~OdCADModel();

  /** \details
    Sets a new source for the CAD model represented as a full path to a .prc file.
    
    \param prc_file_name [in] A string that contains a full path to a .prc file.
  */
  void setSource(const OdString& prc_file_name);

  /** \details
    Sets a new source for the CAD model represented as a full path to a .prc file.
    
    \param pStreamPrc [in] A stream buffer object that is used for .prc file operations.
  */
  void setSource(const OdStreamBufPtr& pStreamPrc);

  /** \details
    Returns the current full path to the source .prc file for the CAD model.
    This method fills a passed string object with file name and returns it to a calling subroutine.
    
    \param prc_file_name [out] A placeholder for the full path to a .prc file.
  */
  void getSource(OdString& prc_file_name) const;

  /** \details
    Returns the current name of the .prc file that acts as the source for the CAD model.
    This method fills a passed string object with the file name and returns it to a calling subroutine.
    
    \param prc_file_name [out] A placeholder for the full path to a .prc file.
  */
  void getSourceName(OdString& prc_file_name) const;

  /** \details
    Returns the current stream buffer object associated with a .prc file that is the source for the CAD model.
    This method fills a passed smart pointer with the address of the stream buffer object and returns it to a calling subroutine.
    
    \param pStreamPrc [out] A placeholder for the smart pointer to the stream buffer.
  */
  void getSource(OdStreamBufPtr& pStreamPrc) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdCADModel, OdCADModel> object.
*/
SMARTPTR(OdCADModel);

}
#endif // _PDF_PUBLISH_MODEL_INCLUDED_

