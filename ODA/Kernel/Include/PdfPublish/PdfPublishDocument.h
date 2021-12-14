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
// PdfPublishDocument.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_DOCUMENT_INCLUDED_
#define _PDF_PUBLISH_DOCUMENT_INCLUDED_

#include "PdfPublishBaseObject.h"
#include "PdfPublishPage.h"

#include "DbBaseHostAppServices.h"

/** \details
  Contains declarations for Publish SDK.
  <group TD_Namespaces>
*/
namespace OdPdfPublish{

/** \details
  This class implements working with a .pdf document created with Publish SDK.
  A document is a container for other elements and controls 
  (such as <link OdPdfPublish::OdPage, pages>, <link OdPdfPublish::OdImage, images>, <link OdPdfPublish::OdLink, links>, 
  <link OdPdfPublish::OdAnnotation, annotations>, and so on).
*/
class PDFPUBLISH_TOOLKIT OdDocument : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdDocument();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDocument);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details 
    Destroys the Publish SDK document.
  */
  virtual ~OdDocument();

  /** \details 
    Sets common information about the Publish SDK document.
    \param title    [in] A string that contains the document title.
    \param author   [in] A string that contains the document author.
    \param subject  [in] A string that contains the document subject.
    \param creator  [in] A string that contains information about the document creator.
  */
  void setInformation(const OdString& title, const OdString& author, const OdString& subject, const OdString& creator);
  
  /** \details 
    Adds a new JavaScript scenario to the Publish SDK document.
    \param name         [in] A name of a JavaScript scenario.
    \param source       [in] Source code text of a JavaScript scenario.
    \param source_type  [in] JavaScript scenario source type.
    \remarks 
    The script name must be unique in the set of document scripts. 
    \sa
    <link OdPdfPublish::Source::Type, Source::Type enumeration>.
  */
  void addJavaScript(const OdString& name, const OdString& source, const Source::Type source_type = Source::kCode);
  
  /** \details 
    Adds a new icon <link OdPdfPublish::OdImage, image> to the Publish SDK document.
    \param name   [in] A name of an icon image.
    \param image [in] A smart pointer to an image object.
    
    \sa 
    <link OdPdfPublish::OdImage, OdImage class>.
  */
  void addIconImage(const OdString& name, const OdImagePtr& image);
  
  /** \details 
    Adds a new <link OdPdfPublish::OdPage, page> to the Publish SDK document.
    \param page [in] A smart pointer to the OdPage object that should be added.
    
    \sa 
    <link OdPdfPublish::OdPage, OdPage class>.
  */
  void addPage(const OdPagePtr& page);

  /** \details
    Adds a new attachment file to the ODA Publish document.  

    \param file_name    [in] A string that contains the name of the file to be attached.
    \param description  [in] A string that contains the description of the file to be attached.

    \remarks
    If there is an existing attachment on the document with the given filename, it will get overwritten.
  */
  void addAttachment(const OdString& file_name, const OdString& description);

  /** \details 
    Returns the current common information about the Publish SDK document.
    This method fills the passed strings and returns them to a calling subroutine.
    
    \param title    [out] A placeholder for the document title.
    \param author   [out] A placeholder for the document author.
    \param subject  [out] A placeholder for the document subject.
    \param creator  [out] A placeholder for the information about the document creator.
  */
  void getInformation(OdString& title, OdString& author, OdString& subject, OdString& creator) const;
  
  /** \details 
    Returns the collection of JavaScript scenarios encapsulated in the Publish SDK document.
    This method fills the passed arrays with names, source code text and source code types, then returns them to a calling subroutine.
    
    \param names        [out] A placeholder for JavaScript scenario names.
    \param sources      [out] A placeholder for JavaScript scenario source code text.
    \param source_types [out] A placeholder for JavaScript scenario source code types.
  */
  void getJavaScripts(OdStringArray& names, OdStringArray& sources, OdSourceTypeArray& source_types) const;
  
  /** \details
    Returns a collection of icon <link OdPdfPublish::OdImage, images> from the Publish SDK document.
    This method fills the passed string array and smart pointer array with image names and pointers to image objects, then returns them to a calling subroutine.
    
    \param names  [out] A placeholder for icon image names.
    \param images [out] A placeholder for an array of smart pointers that point to the document icon images.
    
    \sa 
    <link OdPdfPublish::OdImage, OdImage class>.
  */
  void getIconImages(OdStringArray& names, OdImagePtrArray& images) const;

  /** \details 
    Returns the array of <link OdPdfPublish::OdPage, pages> that the Publish SDK document contains.
    This method fills the passed array with the OdPage object that represents the document pages and returns it to a calling subroutine.
    
    \param pages [out] A placeholder for the document pages array.
    
    \sa 
    <link OdPdfPublish::OdPage, OdPage class>.
  */
  void getPages(OdPagePtrArray& pages) const;

  /** \details
   Retrieves names of files currently attached to the Publish document.
   The method fills passed string arrays with file names and descriptions; then returns them to a calling subroutine.

   \param file_names   [out] A placeholder for names of attached files.
   \param descriptions [out] A placeholder for descriptions of attached files.
  */
  void getAttachments(OdStringArray& file_names, OdStringArray& descriptions);

  /** \details
    Sets a new application services object for the document.
    \param pHostApp [in] A pointer to a database application services object.
  */
  void setHostServices(OdDbBaseHostAppServices* pHostApp);

  /** \details
    Gets tbe current application services object for the document.
    \return Returns a pointer to the database application services object.
  */
  OdDbBaseHostAppServices* appServices() const;
};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdDocument, OdDocument> object.
*/
SMARTPTR(OdDocument);

}
#endif // _PDF_PUBLISH_DOCUMENT_INCLUDED_

