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

#ifndef _OD_EX_PDF_TO_RASTER_H_
#define _OD_EX_PDF_TO_RASTER_H_

#include "TD_PackPush.h"

#include "Gi/GiRasterImage.h"
#include "RxPdfToRasterServices.h"
#include "oda_files/oda_functions.h"

typedef void* PDFDOC;

/** \details
    This class implements platform-dependent loading and converting
    of PDF files for ODA SDK.
 
    <group ExServices_Classes> 
    Library: Source code provided. 
*/
class OdPdf2ImageConverterPdfium : public OdPdf2ImageConverter
{
public:
  ODRX_DECLARE_MEMBERS(OdPdf2ImageConverterPdfium);

  OdPdf2ImageConverterPdfium();
  virtual ~OdPdf2ImageConverterPdfium();

  /** \details
      Loads the specified PDF file.
  */
  virtual OdResult loadPdf(const OdString &filename, const  OdString &password = "");
  
  /** \details
      Loads the PDF file from memory.
  */
  virtual OdResult loadPdf(OdStreamBufPtr &pBuf, const  OdString &password = "");

  /** \details
      Check if the pdf is loaded into converter.
  */
  virtual bool isLoaded();

  /** \details
      Gets the name of the PDF file, if it was loaded from disk. Otherwise returns empty string.
  */
  virtual OdString getFilename();

  /** \details
      Gets the page count of the PDF file.
  */
  virtual OdUInt16 getPagesCount();

  /** \details
      Sets the active page of the loaded PDF.
  */
  virtual OdResult setActivePage(OdUInt16 pageNum = 1);

  /** \details
      Gets the active page of the PDF file.
  */
  virtual OdUInt16 getActivePage();

  /** \details
      Get layers list for active page in PDF.
  */
  virtual OdResult getLayers(OdPdfLayerArray& layers);

  /** \details
      Get page size for active page in PDF.
  */
  virtual OdResult getPageSize(OdGsDCRect& rect);

   /** \details
      Convert PDF to raster image.
  */
  virtual OdGiRasterImagePtr convertPdf(const OdPdf2ImageConversionParams* params = 0);

private:
  virtual OdGiRasterImagePtr convertBmpToRaster(const BITMAPINFOHEADER &bmi, const unsigned char *imagedata)  const;
  virtual bool setupLayers(PDFDOC pDoc, const OdPdfLayerArray& layers);
  virtual OdResult unloadPdf();

  PDFDOC             m_pDoc;
  OdBinaryData       m_data;
  OdString           m_fileName;
  OdUInt16           m_activePage;
  FPDF_PageLayerInfo m_info;
};

/**  
  <group ExServices_Classes> 
*/
class ExPdfToRasterModule : public OdRxPdfToRasterServices
{
public:
  /** \details
      Initialize module.
  */
  void initApp();
  /** \details
      Uninitialize module.
  */
  void uninitApp();

  virtual OdPdf2ImageConverterPtr createConverter();
};

#include "TD_PackPop.h"

#endif //#ifndef _OD_EX_PDF_TO_RASTER_H_
