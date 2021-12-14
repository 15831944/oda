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

#ifndef _PDF_IMPORTER_IMPL_INCLUDED_ 
#define _PDF_IMPORTER_IMPL_INCLUDED_

#include "PdfImportEx.h"
#include "oda_files/oda_functions.h"
#include <map>
#include "Ge/GePoint2dArray.h"
#include "DbBasePdfImportObjectGrabber.h"

class PdfProperties;
typedef OdSmartPtr<PdfProperties> PdfPropertiesPtr;

/** \details
  <group OdPdfImport_Classes> 

  This class implements the upper level import manager.
*/
class PdfImporter : public PdfImporterEx
{
  PdfPropertiesPtr             m_pProperties;
  FPDF_DOCUMENT                m_pDocument;
  FPDF_PAGE                    m_Page;
  OdUInt32                     m_PageNum;
  OdGePoint2dArray             m_ClipBoundary;
  bool                         m_isClipBoundaryInverted;
  std::map<std::wstring, bool> m_LayersInfo;
  FPDF_PageLayerInfo           m_PageInfo;
  PdfImportInfoHolderPtr       m_Import_info;
  PreferableFontCallback       m_PreferableFontCallback;
  std::vector<std::wstring>    m_EbeddedFontsList;

public:
  /** Constructor. 
  */
  PdfImporter();

  ~PdfImporter();

  /** \details
    Returns the propertities.
  */
  OdRxDictionaryPtr properties();

  /** \details
    Imports the data.
  */
  ImportResult import();

  ImportResult loadDocument();

  bool isDocumentLoaded() const;

  ImportResult loadPage();

  bool isPageLoaded() const;

  OdUInt32 getPageCount() const;

  double getMeasureDictInfo();

  OdPdfImport::ImportResult fillLayersInfo(std::map<std::wstring, bool>& layers_info);

  double getPageWidth() const;

  double getPageHeight() const;

  void setClipBoundary(const OdGePoint2dArray& clipBoundary, const bool is_clip_boundary_inverted);

  void setLayersInfo(std::map<std::wstring, bool>& layers_info);

  void clearUnderlayInfo();

  void getImportedObjectsInfo(size_t& object_count, size_t& error_count);

  virtual OdPdfImport::ImportResult fillFontsInfo(std::map<OdAnsiString, OdAnsiString>& font_map);

  virtual void setPreferableFontCallback(PreferableFontCallback callback);

  virtual OdPdfImport::ImportResult extractEmbededFonts(OdStringArray& font_list) const;

private:
	

};

typedef OdSmartPtr<PdfImporter> PdfImporterPtr;

#endif // _PDF_IMPORTER_IMPL_INCLUDED_
