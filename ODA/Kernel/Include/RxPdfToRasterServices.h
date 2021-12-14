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

// RxPdfToRasterServices.h - interface of module, performing pdf to raster conversion

#ifndef __OD_PDF_TO_RASTER_MODULE__
#define __OD_PDF_TO_RASTER_MODULE__

#include "RxModule.h"
#include "OdBinaryData.h"
#include "StringArray.h"
#include "Ge/GePoint2d.h"
#include "Gs/GsDefs.h"

class OdGiRasterImage;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGiRasterImage object pointers.
*/
typedef OdSmartPtr<OdGiRasterImage> OdGiRasterImagePtr;

/**
  \details
  This enumeration represents page rendering flags.
  
  <group !!RECORDS_tkernel_apiref>
*/
enum PdfConversionFlags
{
  kAnnotations = 1,          // Set if annotations are to be rendered.
  kLCDText = 2,              // Set if using text rendering optimized for LCD display.
  kNoNativeText = 4,         // Don't use the native text output available on some platforms.
  kGrayScale = 8,            // Grayscale output.
  kReverseByteOrder = 16,    // Set whether to render in a reverse Byte order.
  kDebugInfo = 128,          // Set if you want to get some debug info.
  kNoCatch = 256,            // Set if you don't want to catch exceptions.
  kLimitImageCache = 512,    // Limit image cache size.
  kForceHalfTone = 1024,     // Always use halftone for image stretching.
  kPrinting = 2048,          // Render for printing.
  kDisableAAforText = 4096,  // Set to disable anti-aliasing on text.
  kDisableAAforImage = 8192, // Set to disable anti-aliasing on images.
  kDisableAAforGeom = 16384, // Set to disable anti-aliasing on geometry (pdf paths).
};

/**
  \details
  This structure represents PDF layers.
*/
typedef struct {
  OdString layer_name; //Name of the layer
  bool     is_on;      //Flag if the layer is enabled
} pdf_layer;

/**
  \details
  This type represents the array of PDF layers.
*/
typedef OdArray<pdf_layer> OdPdfLayerArray;

/** \details
  This class provides image conversion parameters for PDF files converting in
  ODA SDK.
  Library: TD_Db
  <group OdRx_Classes> 
*/
class OdPdf2ImageConversionParams {
public:

  /** \details
      This is the default constructor for objects of the
      OdPdf2ImageConversionParams class. It sets background color to white,
      resolution to 72 dpi, no flags and no crop area.
  */
  OdPdf2ImageConversionParams() :
      bg_color(0xFFFFFFFF) //white background
    , dpi(72)              //default pdf resolution
    , flags(0)             //empty set
  {
    cropArea.set_null();
  };

  OdUInt32        bg_color; //Background color
  OdUInt16        dpi;      //Required resolution
  int             flags;    //Set of the PdfConversionFlags
  OdPdfLayerArray layers;   //Layers to be enabled or disabled in resulting image
  OdGsDCRect      cropArea; //Coordinates of the area to be cropped from the whole page
};

#include "TD_PackPush.h"

/** \details
    This class is the base class for classes that provide platform-dependent
    loading and converting of PDF files for ODA SDK.
    Library: TD_Db
    <group OdRx_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdPdf2ImageConverter : public OdRxObject
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdPdf2ImageConverter);
//DOM-IGNORE-END

  /** \details
      Loads the specified PDF file.
      \param filename [in]  Filename of the file to be read.
      \param password [in]  Optional password.
      
      \returns eOk if successful, eCantOpenFile or eNotHandled otherwise. 
  */
  virtual OdResult loadPdf(const OdString &filename, const  OdString &password = "") = 0;

  /** \details
      Loads the specified PDF file.
      \param pBuf     [in]  Pointer to the StreamBuf object the data are to be read from.
      \param password [in]  Optional password.

      \returns eOk if successful, eCantOpenFile or eNotHandled otherwise. 
  */
  virtual OdResult loadPdf(OdStreamBufPtr &pBuf, const  OdString &password = "") = 0;

  /** \details
      Checks if a PDF file is loaded into the converter.
      
      \returns True if a PDF file is loaded, false otherwise.
  */
  virtual bool isLoaded() = 0;

  /** \details
      Gets the name of the PDF file, if it was loaded from disk. Otherwise returns an empty string.
      
      \returns Name of the loaded PDF file. If no file is loaded, returns an empty string.
  */
  virtual OdString getFilename() = 0;

  /** \details
      Gets the page count of the PDF file.
      
      \returns The number of pages of the loaded PDF file.
  */
  virtual OdUInt16 getPagesCount() = 0;

  /** \details
      Sets the page of the PDF file to work with.
      \param pageNum [in]  Page number. The first page has number 1.

      \returns eOk if successful; eNullPtr if no PDF file is loaded;
               eWrongObjectType if the source file has wrong format;
               eInvalidIndex if no page with specified index was found in the
               source PDF file; eNotHandled if PDFium module is not enabled.
  */
  virtual OdResult setActivePage(OdUInt16 pageNum = 1) = 0;

  /** \details
      Gets the active page of the PDF file.
      
      \returns The index of the active page.
  */
  virtual OdUInt16 getActivePage() = 0;

  /** \details
      Gets the layers of the active page in the PDF file.
      \param layers [out]  Layers array.
      
      \returns eOk if successful; eNullPtr if no PDF file is loaded;
               eWrongObjectType if the source file has wrong format;
               eInvalidIndex if the active page index is invalid;
               eNullIterator if the page iterator is not set;
               eExtendedError if layers can't be got from the active page;
               eNotHandled if PDFium module is not enabled.
  */
  virtual OdResult getLayers(OdPdfLayerArray& layers) = 0;

  /** \details
      Gets the image size on the active page of the PDF file (in pixels for default PDF resolution 72dpi).
      \param rect [out]  Image size.
      
      \returns eOk if successful; eNullPtr if no PDF file is loaded;
               eWrongObjectType if the source file has wrong format;
               eInvalidIndex if the active page index is invalid;
               eNullIterator if the page iterator is not set;
               eNotHandled if PDFium module is not enabled.
  */
  virtual OdResult getPageSize(OdGsDCRect& rect) = 0;

  /** \details
      Converts the active page of the PDF file to the raster image using
      specified parameters.
      \param params [out]  conversion params.
      
      \returns The pointer to the resulting raster image object.
  */
  virtual OdGiRasterImagePtr convertPdf(const OdPdf2ImageConversionParams* params = 0) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdPdf2ImageConverter object pointers.
*/
typedef OdSmartPtr<OdPdf2ImageConverter> OdPdf2ImageConverterPtr;

/** \details
    This class is the base class for classes that provide creating converter of the PDF files for ODA SDK.
    Library: TD_Db
    <group OdRx_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxPdfToRasterServices : public OdRxModule
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdRxPdfToRasterServices);
//DOM-IGNORE-END

  /** \details
      Creates a PDF to raster image converter.
      
      \returns The pointer to the created converter object.
  */
  virtual OdPdf2ImageConverterPtr createConverter() = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdRxPdfToRasterServices object pointers.
*/
typedef OdSmartPtr<OdRxPdfToRasterServices> OdRxPdfToRasterServicesPtr;

#include "TD_PackPop.h"

#endif // __OD_PDF_TO_RASTER_MODULE__
