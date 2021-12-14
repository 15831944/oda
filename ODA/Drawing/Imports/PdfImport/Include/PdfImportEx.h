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

#ifndef _PDF_IMPORTER_EX_INCLUDED_ 
#define _PDF_IMPORTER_EX_INCLUDED_

#include "PdfImport.h"
#include <map>
#include "Ge/GePoint2dArray.h"
#define STL_USING_STRING
#include "OdaSTL.h"

class PdfProperties;
typedef OdSmartPtr<PdfProperties> PdfPropertiesPtr;

/** \details
An abstract class that provides the interface for the upper level of the PDF import manager.
<group OdPdfImport_Classes> 
*/
class PdfImporterEx : public OdPdfImport
{
public:

  /** \details
  Retrieves the current set of the PDF import properties.
  \returns Returns a smart pointer to a dictionary object that contains import properties.
  */
  virtual OdRxDictionaryPtr properties() = 0;

  /** \details
  Imports the data from a specified .pdf file to a drawing database.
  \returns Returns a value of the <link OdPdfImport::ImportResult, ImportResult> enumeration that contains the result of the import operation.
  */
  virtual ImportResult import() = 0;

  /** \details 
  Loads the content of a specified .pdf document.
  \returns Returns a value of the <link OdPdfImport::ImportResult, ImportResult> enumeration that contains the result of the loading operation.
  */
  virtual ImportResult loadDocument() = 0;

  /** \details 
  Retrieves whether the .pdf document is successfully loaded.
  \returns Returns true if the document content is loaded; otherwise, the method returns false.
  */
  virtual bool isDocumentLoaded() const = 0;

  /** \details 
  Loads the content of a specified page from the .pdf document.
  \returns Returns a value of the <link OdPdfImport::ImportResult, ImportResult> enumeration that contains the result of the loading operation.
  */
  virtual ImportResult loadPage() = 0;

  /** \details 
  Retrieves whether the page from the .pdf document is successfully loaded.
  \returns Returns true if the page content is loaded; otherwise, the method returns false.
  */
  virtual bool isPageLoaded() const = 0;

  /** \details 
  Retrieves the quantity of pages in the .pdf document.
  \returns Returns the current page quantity.
  */
  virtual OdUInt32 getPageCount() const = 0;

  /** \details 
  Retrieves the current measuring coefficient of the .pdf document.
  \returns A double value that contains the current measuring coefficient.
  \remarks 
  If the .pdf document contains only one viewport the method returns the page or viewport measuring coefficient depending on which coefficient is bigger.
  If the .pdf document contains more than one viewport, the method returns the page measuring coefficient.
  */
  virtual double getMeasureDictInfo() = 0;

  /** \details 
  Retrieves the layer information from the .pdf document.
  \param layers_info [out] A placeholder for an STL map object that contains layer information.
  \returns Returns a value of the <link OdPdfImport::ImportResult, ImportResult> enumeration that contains the result of the filling operation.
  \remarks 
  The layer information contains a set of key-value pairs. Each pair contains a layer name as a key and a boolean flag that determines the visibility of the layer as a value.
  The set of key-value pairs is stored in an STL map object.
  The method fills the passed map object with current layer information retrieved from the PDF document and returns it to a calling subroutine.  
  */
  virtual OdPdfImport::ImportResult fillLayersInfo(std::map<std::wstring, bool>& layers_info) = 0;

  /** \details 
  Retrieves the current page width of the .pdf document.
  \returns Returns the current page width in measure units.
  */
  virtual double getPageWidth() const = 0;

  /** \details 
  Retrieves the current page height of the .pdf document.
  \returns Returns the current page height in measure units.
  */
  virtual double getPageHeight() const = 0;

  /** \details 
  Sets a new clipping boundary for the .pdf document content.
  \param clipBoundary [in] A point array that defines the boundary of clipping.
  \param is_clip_boundary_inverted [in] A boolean value that determines whether the boundary is inverted (if equal to true) or not (if equal to false).
  */
  virtual void setClipBoundary(const OdGePoint2dArray& clipBoundary, const bool is_clip_boundary_inverted) = 0;

  /** \details 
  Sets layer information.
  \param layers_info [in] An STL map object that contains layer information.
  \remarks 
  The layer information contains a set of key-value pairs. 
  Each pair contains a layer name as a key and a boolean flag that determines the visibility of the layer as a value.
  The set of key-value pairs is stored in an STL map object that is passed to the method. Using this map object, you can change the visibility of each layer that was 
  previously retrieved from the .pdf document by calling the fillLayersInfo() method.  
  */
  virtual void setLayersInfo(std::map<std::wstring, bool>& layers_info) = 0;

  /** \details 
  Clears underlay information. 
  */
  virtual void clearUnderlayInfo() = 0;

  /** \details
  Retrieves information about import results.
  \param object_count [out] A placeholder for the number of PDF objects to be imported.
  \param error_count [out] A placeholder for the number of PDF objects imported with an error.
  */
  virtual void getImportedObjectsInfo(size_t& object_count, size_t& error_count) = 0;

  /** \details.
  Retrieves font information from the current page of the .pdf document.
  \param font_map[out] Placeholder for the STL map object that contains font information.
  \returns Returns the value of the <link OdPdfImport::ImportResult, ImportResult> enumeration that contains the result of the filler operation.
  \remarks
  The font information contains a set of key-value pairs. Each pair contains the font name as the key and FontFamily of the font as the value.
  The set of key-value pairs is stored in the STL map object.
  The method fills the passed map object with the current font information extracted from the current page of the PDF document and returns it to the calling subroutine.
  */
  virtual OdPdfImport::ImportResult fillFontsInfo(std::map<OdAnsiString, OdAnsiString>& font_map) = 0;

  /** \details
  Sets a function to select the preferred font.
  \param callback                 [in] A pointer to the function for selecting the preferred font.
  \remarks
  If the font name is changed using PreferableFontCallback, this takes precedence over the Type3TextDefaultFont option.
  */
  virtual void setPreferableFontCallback(PreferableFontCallback callback) = 0;

  /** \details.
  Extracts embedded TrueType fonts from a .pdf document, saves them and retrieves a list of font files. 
  \param font_map[out] Placeholder for the OdStringArray that contains names for extracted fonts.
  \returns Returns the value of the <link OdPdfImport::ImportResult, ImportResult> enumeration that contains the result of the filler operation.
  \remarks
  Fonts are extracted for objects that can be exported with specified properties. Installation of the extracted fonts is performed on the application side depending
  on the platform. It is recommended to use this function before calling import() to be able to set the extracted fonts.
  */
  virtual OdPdfImport::ImportResult extractEmbededFonts(OdStringArray& font_list) const = 0;
};

/** \details 
A data type that represents a smart pointer to a <link PdfImporterEx, PDF importer> object.
*/
typedef OdSmartPtr<PdfImporterEx> PdfImporterExPtr;

#endif // _PDF_IMPORTER_EX_INCLUDED_
