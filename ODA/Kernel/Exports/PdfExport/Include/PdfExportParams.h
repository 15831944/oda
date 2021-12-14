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

#ifndef _PDFEXPORTPARAMS_INCLUDED_
#define _PDFEXPORTPARAMS_INCLUDED_ /*!DOM*/ 

#include "PdfExportDef.h"

#include "Gs/GsPageParams.h"

#include "DbBaseDatabase.h"
#include "OdStreamBuf.h"
#include "PrcContextForPdfExport.h"
#include "Pdf/Pdf3dEnums.h"
#include "UInt32Array.h"

class OdGsBaseVectorizer;
/** \details
  The namespace contains declarations related to exporting to the PDF format functionality.
  <group TD_Namespaces>
*/
namespace TD_PDF_2D_EXPORT {

  /** \details
  A set of parameters for adding a watermark annotation to the output PDF file.
  */
  struct Watermark
  {
    /** \details 
    Enumerates fonts, which can be used in a watermark annotation.
    */
    enum WatermarkFonts
    {
      /** Times Roman font set.*/
      kTimesRoman,
      /** Helvetica font set.*/
      kHelvetica,
      /** Courier monospace font set.*/
      kCourier,
      /** Symbol font set.*/
      kSymbol,
      /** Bold Times font set.*/
      kTimesBold,
      /** Bold Helvetica font set.*/
      kHelveticaBold,
      /** Bold Courier monospace font set.*/
      kCourierBold,
      /** Zapf Dingbats font set.*/
      kZapfDingbats,
      /** Times italic font set.*/
      kTimesItalic,
      /** Helvetica oblique font set.*/
      kHelveticaOblique,
      /** Courier oblique font set.*/
      kCourierOblique,
      /** Times bold and italic font set.*/
      kTimesBoldItalic,
      /** Helvetica bold and oblique font set.*/
      kHelveticaBoldOblique,
      /** Courier bold and oblique font set.*/
      kCourierBoldOblique
    };

    /** \details
    Enumerates available positions for the watermark annotation.
    */
    enum WatermarkPosition
    {
      /** From left to right in the page's center.*/
      kLeftToRight,
      /** From the upper-left corner to the lower-right corner of the page.*/
      kUpperLeftToLowerRight,
      /** From the lower-left corner to the upper-right corner of the page.*/
      kLowerLeftToUpperRight, 
      /** The upper-left corner of the page.*/
      kUpperLeft, 
      /** The upper-right corner of the page.*/
      kUpperRight, 
      /** The lower-right corner of the page.*/
      kLowerRight, 
      /** The lower-left corner of the page.*/
      kLowerLeft 
    };

    /** Watermark text.*/
    OdString text; 
    /** Watermark text color (default value is black).*/
    ODCOLORREF color; 
    /** Watermark text font size (default value is 48).*/
    OdUInt16 fontSize; 
    /** The level of the watermark's opacity (in percents from 0 to 100; the default value is 50).*/
    OdUInt16 opacity; 
    /** Watermark text font (default value is kTimesRoman).*/
    WatermarkFonts font; 
    /** Watermark text direction (default value is kLeftToRight).*/
    WatermarkPosition position; 
    /**The fit to page flag.The flag determines whether the watermark is scaled to fit the page.Font size is recalculated(default value is false).*/
    bool scaleToPage;           

    /** \details 
    Creates a new watermark annotation object with default parameters.
    */
    Watermark() :
      color(0)
      , opacity(50)
      , fontSize(48)
      , font(kTimesRoman)
      , position(kLeftToRight)
      , scaleToPage(false)
    {};
  };

  /** \details
  A pure virtual class that provides an interface for a reactor to handle pdf export actions on the callin subroutine level.
  A reactor can be used, for example, to calculate surface tolerance, which should be used by ACIS for tessellation.
  */
  class PdfExportReactor
  {
  public:
  
    /** \details
    Destroys the reactor object.
    */
    virtual ~PdfExportReactor() {};
    
    /** \details 
    Starts the vectorization of the specified client view.
    \param vect [in] A OdGsBaseVectorizer object.
    */
    virtual void beginViewVectorization(OdGsBaseVectorizer& vect) = 0;
    
    /** \details 
    Ends the vectorization process.
    \param vect [in] A OdGsBaseVectorizer object.
    */
    virtual void endViewVectorization(OdGsBaseVectorizer& vect) = 0;
  };

/** \details
  This class implements the PDF export parameters.
  
  <group OdExport_Classes> 
*/
class PDFExportParams
{
public:

  /** \details
  PDF export flags enumeration that can be used for handling the export process.
  */
  enum PDFExportFlags
  {
    /** All flags disabled.*/
    kZeroFlag = 0, 
    /** Enable embedding of True Type font program to PDF file. The whole True Type font file will be embedded (huge .pdf size).*/
    kEmbededTTF = 1, 
    /** Enable True Type font text to geometry conversion.*/
    kTTFTextAsGeometry = 2, 
    /** Enable SHX font text to geometry conversion.*/
    kSHXTextAsGeometry = 4, 
    /** Enable simple geometry optimization (separated segments to one polyline, Bezier curve control points).*/
    kSimpleGeomOptimization = 8, 
    /** If enabled, layer support will be added to .pdf file (1.5 version required).*/
    kEnableLayers = 16, 
    /** If enabled, invisible layers will be added to .pdf file (1.5 version required).*/
    kIncludeOffLayers = 32, 
    /** Enable embedding of True Type font program to .pdf file. The True Type font will be reduced by removing unused parts (smaller .pdf file size, but limited editing).*/
    kEmbededOptimizedTTF = 64, 
    /** Determines whether software uses vector hidden-line removal for corresponding viewports.*/
    kUseHLR = 128, 
    /** Enable compression of internal PDF streams.*/
    kFlateCompression = 256, 
    /** Enable ASCIIHex encoding of internal PDF streams.*/
    kASCIIHexEncoding = 512, 
    /** Enable hyperlinks export.*/
    kExportHyperlinks = 1024, 
    /** If enabled, layout geometry will be zoomed to paper size, else layout settings will be applied.
    Note: Z2E mode works like Layout settings: "Plot Area" - "Extents", "Fit to Paper", "Center the plot" applied to specified paper size and orientation,
    but some settings like "Plot lineweight", "Plot style" are still applying from Layout settings.*/
    kZoomToExtentsMode = 2048, 
    /** Create linearized .pdf file.*/
    kLinearized = 4096, 
    /** Enable merging the colors of crossing lines.*/
    kMergeLines = 8192, 
    /** Enable measuring scale in pdf.*/
    kMeasuring = 16384, 

    /** Default flag combination.*/
    kDefault = kTTFTextAsGeometry | kSHXTextAsGeometry | kFlateCompression | kASCIIHexEncoding | kZoomToExtentsMode
  };

  /** \details 
  PDF/A format mode enumeration. 
  */
  enum PDF_A_mode
  {
    /** PDF/A format is not supported*/
    kPDFA_None = 0,
    /**PDF/A-1b version*/
    kPDFA_1b,
    /**PDF/A-2b version*/
    kPDFA_2b
  };

  /** \details
  PRC export support enumeration.
  */
  enum PRCSupport
  {
    /** Export to PRC is disabled.*/
    kDisabled = 0, 
    /** Exports PRC is supported as Boundary representation.*/
    kAsBrep   = 1, 
    /** Exports PRC is supported as Mesh representation.*/
    kAsMesh   = 2  
  };

  /** \details
  Hatches export type. 
  */
  enum ExportHatchesType
  {
    /** Exports hatch as a bitmap.*/
    kBitmap =   0, 
    /** Exports hatch as a set of triangles (vectorizer).*/
    kDrawing =  1, 
    /** Exports hatch as a PDF path (solid hatches only).*/
    kPdfPaths = 2,
    /** Exports hatch as a polygon (solid hatches only).*/
    kPolygons = 3
  };

  /** \details
  Searchable text type.
  */
  enum SearchableTextType
  {
    /** No searchable text.*/
    kNoSearch = 0, 
    /** Only text displayed with SHX font is searchable.*/
    kSHX =  1,
    /** Only text displayed with TTF font is searchable.*/
    kTTF =  2 
  };

  /** \details
  Export color policy. Determines how colors form an original drawing are exported to .pdf file.
  */
  enum ColorPolicy
  {
    /** No color policy. A drawing is exported with its native color palette (as is).*/
    kNoPolicy = 0, 
    /** Drawing is exported to a monochrome .pdf file.*/
    kMono = 1, 
    /** Drawing is exported to a grayscale .pdf file. Native colors are converted into grayscale.*/
    kGrayscale = 2 
  };

  /** \details
	PDF format versions enumeration, used during the export process.
  */
  enum PDFExportVersions
  {
    /** PDF format version 1.0.*/
    kPDFv1_0 = 10,
    /** PDF format version 1.1.*/
    kPDFv1_1 = 11,
    /** PDF format version 1.2.*/
    kPDFv1_2 = 12,
    /** PDF format version 1.3.*/
    kPDFv1_3 = 13,
    /** PDF format version 1.4.*/
    kPDFv1_4 = 14,
    /** PDF format version 1.5.*/
    kPDFv1_5 = 15,
    /** PDF format version 1.6.*/
    kPDFv1_6 = 16,
    /** PDF format version 1.7.*/
    kPDFv1_7 = 17,
    /** The latest PDF format version supported by the PDF export functionality.*/
    kLast    = kPDFv1_7
  };

  /** \details
  Shaded viewport export policy
  */
  enum PDFShadedViewportExportMode
  {
    /** Shaded Viewports will be exported as one-piece image with low resolution*/
    kExportOnePiece = 0,
    /** Shaded Viewports will be exported as tiled images according to vector resolution (geometry DPI)*/
    kExportAsTiles =  1,
    /** Shaded Viewports will be exported as striped images according to vector resolution (geometry DPI). This mode has better performance 
    for PDF viewers than tiled mode, but can provide some problems on old graphics cards if hardware rendering
    is used (GLES2 for example)*/
    kExportAsStrips = 2
  };

  /** \details
  Creates a new set of export parameters with default values. 
  */
  PDFExportParams()
    : m_pDb(0)
    , m_ver(currentVersion)
    , m_background(ODRGB(255, 255, 255))
    , m_pPalette(0)
    , m_flags(kDefault)
    , m_hatchDPI(72)
    , m_solidHatchesExportType(kBitmap)
    , m_otherHatchesExportType(kBitmap)
    , m_gradientHatchesExportType(kBitmap)
    , m_VectorResolution(600)
    , m_colorImagesDPI(400)
    , m_bwImagesDPI(400)
    , m_PRCMode(kDisabled)
    , m_PRCContext()
    , m_prcHasBackground(false)
    , m_prcBackground(ODRGB(255, 255, 255))
    , m_PRCRenderMode(PDF3D_ENUMS::kDefaultMode)
    , m_searchableTextType(kNoSearch)
    , m_colorPolicy(kNoPolicy)
    , m_bCropImages(true)
    , m_bDCTCompression(false)
    , m_DCTQuality(50)
    , m_PRCCompressionLevel(0)
    , m_useViewExtents(false)
    , m_pdfAMode(kPDFA_None)
    , m_reserved1(0)
    , m_Export2XObject(false)
    , m_ForceDisableGsDevice(false)
    , m_TransparentShadedVpBg(false)
    , m_ShadedVpExportMode(kExportOnePiece)
    , m_bUpscaleImages(false)
    , m_exportReactor(0)
    , m_useGsCache(false)
    , m_bParallelVectorization(false)
    , m_bParallelDisplay(false)
    , m_bSearchableTextAsHiddenText(false)
    , m_bSearchableTextInRenderedViews(false)
  {
  }

  /** \details
  Destroys the set of export parameters. 
  */
  ~PDFExportParams() {}

  /** \details
  Sets export to PRC flag value.
  
  \param flags [in]  A new flag value to be set.
  */
  void setPRCMode(PRCSupport flags)
  {
    if (kDisabled != flags)
    {
      m_flags = (PDFExportFlags)(m_flags & ~kUseHLR);
      m_ver = currentVersion;
    }
    m_PRCMode = flags;
  }
  
  /** \details
  Retrieves the PRC mode option.
  \returns true if the export to PRC option is enabled; otherwise, the method returns false.
  */
  PRCSupport getPRCMode(void) const { return m_PRCMode; }

  /** \details
  Retrieves the current PRC export context. 
  \returns Returns a raw pointer to the PRC export context object that is applied to the set of parameters.
  remarks 
  You can use the returned pointer to modify the context object. 
  */
  OdPrcContextForPdfExport* getPRCContext(void)  { return (OdPrcContextForPdfExport*)m_PRCContext.get(); }

  /** \details
  Retrieves the currnet PRC export context.
  \returns Returns a raw pointer to the PRC export context object that is applied to the set of parameters.
  \remarks 
  Please note that the method returns context object in read-only mode. 
  */
  const OdPrcContextForPdfExport* getPRCContext(void) const { return (OdPrcContextForPdfExport*)m_PRCContext.get(); }
  
  /** \details
  Sets prc export context

  \param pContext [in] A context that will be used during the export of prc file
  */
  void setPRCContext(OdRxObject *pContext) { m_PRCContext = pContext; }

  /** \details
  Retrieves the PRC background option.
  \returns true if the PRC background color is set; otherwise, the method returns false.
  */
  bool hasPrcBackground(void) const { return m_prcHasBackground; }

  /** \details
  Retrieves the background color for PRC content.
  \returns An ODCOLORREF value that represents the currently set color of PRC background.
  */
  ODCOLORREF getPrcBackground(void) const { return m_prcBackground; }

  /** \details
  Sets a new PRC background color.

  \param bacgr [in] A color of default background in created prc file.
  */
  void setPrcBackground(ODCOLORREF bacgr) { m_prcHasBackground = true; m_prcBackground = bacgr; }

  /** \details
  Clears the PRC background color.
  */
  void clearPrcBackground() { m_prcHasBackground = false; }

  /** \details
  Retrieves the current rendering mode for PRC content.
  \returns A value of the PDF3D_ENUMS::PRCRenderingMode enumeration that represents the current rendering mode for PRC content.
  */
  PDF3D_ENUMS::PRCRenderingMode getPrcRenderingMode() { return m_PRCRenderMode; }

  /** \details
  Sets prc rendering mode

  \param renderMode [in] A new default rendering mode value
  */
  void setPrcRenderingMode(PDF3D_ENUMS::PRCRenderingMode renderMode) { m_PRCRenderMode = renderMode; }

  /** \details
  Retrieves the current compression level for PRC content.
  \param compressionLev [out] A placeholder for the compression level value.
  \returns true if the compression of PRC content is enabled; otherwise, the method returns false.
  \remarks
  The method fills the passed value of the PDF3D_ENUMS::PRCCompressionLevel enumeration with the current compression level and returns it to a calling subroutine.
  */
  bool hasPrcBrepCompression(PDF3D_ENUMS::PRCCompressionLevel &compressionLev) const
  {
    if ((m_PRCCompressionLevel & 0x00000004) != 0)
    {
      // apply mask
      compressionLev = (PDF3D_ENUMS::PRCCompressionLevel)(m_PRCCompressionLevel & 0x00000003);
      return true;
    }
    return false;
  }

  /** \details
  Retrieves whether the tessellation compression for PRC content is enabled.
  \returns true if the tessellation compression for PRC content is enabled; otherwise, the method returns false.
  */
  bool hasPrcTessellationCompression() const { return ((m_PRCCompressionLevel & 0x00000008) != 0); }

  /** \details
  Sets a database to be exported.
  
  \param pDb [in] A raw pointer to the database object to be exported.
  */
  void setDatabase(OdDbBaseDatabase *pDb) { m_pDb = pDb; }
  
  /** \details
  Retrieves the database to be exported.
  \returns A raw pointer to a database object to be exported.
  */
  OdDbBaseDatabase* database() const { return m_pDb; }

  /** \details
  Sets a new value for the version parameter of the output PDF file.
  
  \param version [in]  A new value of the PDF version parameter.
  */
  void setVersion(PDFExportVersions version) { m_ver = version; }

  /** \details
  Retrieves the version of the output PDF file.
  \returns A value of the PDFExportVersions enumeration that represents the version for the output PDF file. 
  */
  PDFExportVersions version() const { return m_ver; }

  /** \details
  Sets a new output stream object for writing the output PDF file.
  
  \param output [in] A reference to a smart pointer to an OdStreamBuff object that represents the stream to be used for writing data into the PDF file.
  */
  void setOutput(const OdStreamBufPtr& output) { m_outputStream = output; }

  /** \details
  Retrieves the output stream that is used for writing the output PDF file.
  \returns A smart pointer to an OdStreamBuf object that represents the stream for writing the output PDF file.
  */
  OdStreamBufPtr output() const { return m_outputStream; }

  /** \details
  Sets a new export flags value.
  
  \param flags [in]  An instance of PDF export flags to be set.
  */
  void setExportFlags(PDFExportFlags flags) { m_flags = flags; }
  
  /** \details
  Retrieves the current PDF export flags.
  \returns A value of the PDFExportFlags data type that represents the current value of PDF export flags.
  */
  PDFExportFlags exportFlags() const { return m_flags; }

  /** \details
  Sets a new PDF/A mode value.

  \param mode [in]  An instance of PDF/A mode to be set.
  */
  void setArchived(PDF_A_mode mode) { m_pdfAMode = mode; }

  /** \details
  Retrieves the current value of the PDF/A mode.
  \returns A value of the PDF_A_mode enumeration that represents the current value of PDF/A mode.
  */
  PDF_A_mode archived() const { return m_pdfAMode; }

  /** \details
  Sets a new searchable text type value.

  \param type [in]  A new value of text search type to be set.
  */
  void setSearchableTextType(SearchableTextType type) { m_searchableTextType = type; }

  /** \details
  Retrieves the current value of the searchable text type.
  \returns A value of the SearchableTextType enumeration that represents the current value of the searchable text type.
  */
  SearchableTextType searchableTextType() const { return m_searchableTextType; }

  /** \details
  Sets a new export color policy value.

  \param policy [in] color policy to be set.
  */
  void setColorPolicy(ColorPolicy policy) { m_colorPolicy = policy; }

  /** \details
  Retrieves the color policy.
  \returns A value of the ColorPolicy enumeration that represents the current value of the color policy.
  */
  ColorPolicy colorPolicy() const { return m_colorPolicy; }

  /** \details
  Sets an array of drawing layouts to be exported to PDF.
  
  \param layouts  [in]  A new array of layouts to be exported.
  \param pDbArray [in] A raw pointer to an array of smart-pointers to OdRxObject instances.
  
  \remarks
  If the array size is equal to zero, only active layout is exported.
  */
  void setLayouts(const OdArray<OdString>& layouts, const OdRxObjectPtrArray* pDbArray = 0)
  {
    if (!pDbArray || !layouts.size())
    {
      m_layouts = layouts;
      m_Databases.clear();
    }
    else if (layouts.size() == pDbArray->size())
    {
      m_layouts = layouts;
      m_Databases = *pDbArray;
    }
  }

  /** \details
  Retrieves the array of drawing layouts to be exported to PDF.
  \returns An array of OdString object that contains layout names.
  \remarks
  This is obsolete method which will be removed further
  */
  OdStringArray& layouts() { return m_layouts; }

  /** \details
  Adds the specified layout to the layouts array to export.
  \param s [in] Layout to be exported.
  */
  void addLayout(const OdString& s) { m_layouts.push_back(s); }

  /** \details
  Retrieves the array of drawing layouts to be exported to PDF.
  \returns An array of OdString object that contains layout names.
  \remarks
  If the array size is equal to zero, only active layout is exported.
  */
  const OdStringArray& layouts() const { return m_layouts; }

  /** \details
  Retrieves the array of databases to be exported to PDF.
  \returns An array of RxObjectPtr that contains pointers to the databases.
  \remarks
  If the array size is equal to zero, only current db is exported.
  */
  const OdRxObjectPtrArray& databases() const { return m_Databases; }

  /** \details
  Retrieves the array of databases to be exported to PDF.
  \returns An array of RxObjectPtr that contains pointers to the databases.
  \remarks
  This is obsolete method which will be removed further
  */
  OdRxObjectPtrArray& databases() { return m_Databases; }

  /** \details
  Sets a new array of page parameters to be used during the exporting.
  
  \param pageParams [in]  A new page parameters array to be set.
  
  \remarks
  Page parameters contain such options as paper size, margins. All the values are measured in mm.
  */
  void setPageParams(const OdArray<OdGsPageParams>& pageParams) { m_pageParams = pageParams; }
  
  /** \details
  Retrieves the array of page parameters to be used during the exporting.
  \returns An array of OdGsPageParams objects that contains page parameters.
  \remarks
  Page parameters contain such options as paper size, margins. All the values are measured in mm.
  */
  OdArray<OdGsPageParams>& pageParams() { return m_pageParams; }

  /** \details
  Sets a new value of the background color for the output PDF document.
  
  \param background [in]  A new value for background color.
  
  \remarks
  Default background color is white.
  */
  void setBackground(ODCOLORREF background) { m_background = background; }
  
  /** \details
  Retrieves the current value of the background color for the output PDF document.
  \returns An ODCOLORREF value that represents the background color for the output PDF document.
  \remarks 
  Default background color is white.
  */
  ODCOLORREF background() const { return m_background; }

  /** \details
  Sets a new palette to be used during the exporting.
  
  \param pPalette [in]  A new palette to be set.
  
  \remarks
  If pPalette is equal to NULL, one of two default palettes will be used depending on background color value.
  */
  void setPalette(const ODCOLORREF* pPalette) { m_pPalette = pPalette; }
  
  /** \details
  Retrieves the palette to be used during the exporting.
  \returns A raw pointer to an ODCOLORREF value that represents the used palette.
  \remarks
  If the returned pointer is equal to NULL, one of the two default palettes will be used depending on background color value.
  */
  const ODCOLORREF* palette() const { return m_pPalette; }

  /** \details
  Sets a new value for vector resolution.
  
  \param dpi [in]  A new value for vector resolution to be set (in dots per inch, dpi).
  
  \remarks
  Standard values are: 72, 150, 200, 300, 400, 600, 720, 1200, 2400 and 4800 DPI. 
  Default value is 600 DPI.
  */
  void setGeomDPI(OdUInt16 dpi) { if (dpi >= 72 && dpi <= 40000) m_VectorResolution = dpi; };
  
  /** \details
  Retrieves the currently used vector resolution (in dots per inch, dpi).
  \returns An unsigned 16-bit integer value that contains the current resolution
  \remarks
  Standard values are: 72, 150, 200, 300, 400, 600, 720, 1200, 2400 and 4800 DPI. 
  Default value is 600 DPI.
    */
  OdUInt16 getGeomDPI() const { return m_VectorResolution; };

  /** \details
  Sets a new value of the resolution for images created as a result of hatch export.
  
  \param dpi [in]  A new value of the resolution to be set (in dots per inch, dpi).
  
  \remarks
  Hatch resolution value is used when the hatch entity is exported as a bitmap.
  */
  void setHatchDPI(OdUInt16 dpi) { m_hatchDPI = dpi; }
  
  /** \details
  Retrieves the current value of the resolution for images created as a result of hatch export (in dots per inch, dpi).
  \returns An unsigned 16-bit integer value that contains the current resolution.
  \remarks
  Hatch resolution value is used when the hatch entity is exported as a bitmap.
  */
  OdUInt16 hatchDPI() const { return m_hatchDPI; }

  /** \details
  Sets a new value of the resolution for color and grayscale images.
  
  \param dpi [in]  A new value of the resolution to be set  (in dots per inch, dpi).
  
  \remarks
  The resolution value cannot exceed the vector resolution value.
  */
  void setColorImagesDPI(OdUInt16 dpi) { if (dpi >= 72 && dpi <= m_VectorResolution) m_colorImagesDPI = dpi; }
  
  /** \details
  Retrieves the current value of the resolution for color and grayscale images  (in dots per inch, dpi).
  \returns An unsigned 16-bit integer value that contains the current resolution.
  \remarks
  The resolution value cannot exceed the vector resolution value.
  */
  OdUInt16 colorImagesDPI() const { return m_colorImagesDPI; }

  /** \details
  Sets a new value of the resolution for monochrome images in the output PDF document.
  
  \param dpi [in]  A new value of the resolution for monochrome images (in dots per inch, dpi).
  
  \remarks
  The resolution value cannot exceed the vector resolution value.
  */
  void setBWImagesDPI(OdUInt16 dpi) { if (dpi >= 72 && dpi <= m_VectorResolution) m_bwImagesDPI = dpi; }

  /** \details
  Retrieves the current value of the resolution for monochrome images in the output PDF document (in dots per inch, dpi).
  \returns An unsigned 16-bit integer value that contains the current resolution.
  \remarks
  The resolution value cannot exceed the vector resolution value.
  */
    OdUInt16 bwImagesDPI() const { return m_bwImagesDPI; }

  /** \details
  Sets a new title for the output PDF document.
  
  \param sTitle [in]  A new value for the output document's title.
  */
  void setTitle(const OdString& sTitle) { m_Title = sTitle; }
  
  /** \details
  Retrieves the current title for the output PDF document.
  \returns An OdString object that contains the current PDF document title.
  */
  OdString title() const { return m_Title; }

  /** \details
  Sets a new value of the author of the output PDF document.
  
  \param sAuthor [in]  A new value of the author of the output document.
  */
  void setAuthor(const OdString& sAuthor) { m_Author = sAuthor; }
  
  /** \details
  Retrieves the author of the output PDF document.
  \returns An OdString object that contains the author of the output PDF document.
  */
  OdString author() const { return m_Author; }

  /** \details
  Sets a new value of the output PDF document subject.
  
  \param sSubject [in]  A new value for the subject.
  */
  void setSubject(const OdString& sSubject) { m_Subject = sSubject; }
  
  /** \details
  Retrieves a value of the output PDF document subject.
  \returns An OdString object that contains the subject of the output PDF document.
  */
  OdString subject() const { return m_Subject; }

  /** \details
  Sets keywords associated with the output PDF document.
  
  \param sKeywords [in]  A new keywords string to be set.
  */
  void setKeywords(const OdString& sKeywords) { m_Keywords = sKeywords; }
  
  /** \details
  Retrieves keywords associated with the output PDF document.
  \returns An OdString object that contains keywords associated with the output PDF document.
  */
  OdString keywords() const { return m_Keywords; }

  /** \details
  Sets a new value of the output PDF document creator.
  
  \param sCreator [in] A new value of the creator.
  
  \remarks
  If the output document was converted to PDF from another format, the name of the application (for example, Adobe FrameMaker) that created the original document from which it was converted.
  */
  void setCreator(const OdString& sCreator) { m_Creator = sCreator; }
  
  /** \details
  Retrieves the output PDF document creator.
  \returns An OdString object that contains the output PDF document creator.
  \remarks
  If the output document was converted to PDF from another format, the name of the application (for example, Adobe FrameMaker) that created the original document from which it was converted.
  */
  OdString creator() const { return m_Creator; }

  /** \details
  Sets a new value of the output PDF document producer.
  
  \param sProducer [in]  A new value of the producer to be set.
  
  \remarks
  If the document was converted to PDF from another format, the name of the application (for example, Acrobat Distiller) that converted it to PDF.
  If the specified name is null or is empty, the document producer name becomes equal to the string &quot;ODA PDF Export vX.X&quot;.
  */
  void setProducer(const OdString& sProducer) { m_Producer = sProducer; }
  
  /** \details
  Retrieves the output PDF document producer.
  \returns An OdString object that contains the output PDF document producer.
  \remarks
  If the document was converted to PDF from another format, the name of the application (for example, Acrobat Distiller) that converted it to PDF.
  If the producer data was not specified, the document producer name becomes equal to the string &quot;ODA PDF Export vX.X&quot;.
  */
  OdString producer() const { return m_Producer; }

  /** \details
  Sets the type of solid hatches export.
  
  \param type [in]  A new value of the solid hatches export type.
  
  \remarks
  There are 3 types of exporting to PDF for solid hatches:
  * Bitmap (type = kBitmap);
  * Vectorizer (type = kDrawing);
  * PDF path (type = kPdfPaths);
  */
  void setSolidHatchesExportType(ExportHatchesType type) { m_solidHatchesExportType = type; }
  
  /** \details
  Retrieves the current type of solid hatches export.
  \returns A value of the ExportHatchesType enumeration that represents the current type of solid hatches export.
  \remarks
  There are 3 types of exporting to PDF for solid hatches:
  * Bitmap (type = kBitmap);
  * Vectorizer (type = kDrawing);
  * PDF path (type = kPdfPaths);
  */
  ExportHatchesType solidHatchesExportType() const { return m_solidHatchesExportType; }

  /** \details
  Sets the type of other (non-solid) hatches export.
  
  \param type [in]  A new value of the non solid hatches export type.
  
  \remarks
  For non-solid hatches, the following types of PDF export are available:
  * Bitmap (type = kBitmap);
  * Vectorizer (type = kDrawing);
  */
  void setOtherHatchesExportType(ExportHatchesType type)
  {
    m_otherHatchesExportType = type == kBitmap ? kBitmap : kDrawing;
  }
  
  /** \details
  Retrieves the type of other (non-solid) hatches export.
  \returns A value of the ExportHatchesType enumeration that represents the current type of other (non-solid) hatches export.
  \remarks
  For non-solid hatches, the following types of PDF export are available:
  * Bitmap (type = kBitmap);
  * Vectorizer (type = kDrawing);
  */
  ExportHatchesType otherHatchesExportType() const { return m_otherHatchesExportType; }

  /** \details
  Sets the type of gradient hatches export.

  \param type [in]  A new value of the gradient hatches export type.

  \remarks
  For gradient hatches, the following types of PDF export are available:
  * Bitmap (type = kBitmap);
  * Vectorizer (type = kDrawing);
  */
  void setGradientHatchesExportType(ExportHatchesType type)
  {
    m_gradientHatchesExportType = type == kBitmap ? kBitmap : kDrawing;
  }

  /** \details
  Retrieves the type of gradient hatches export.
  \returns A value of the ExportHatchesType enumeration that represents the current type of gradient hatches export.
  \remarks
  For gradient hatches, the following types of PDF export are available:
  * Bitmap (type = kBitmap);
  * Vectorizer (type = kDrawing);
  */
  ExportHatchesType gradientHatchesExportType() const { return m_gradientHatchesExportType; }

  /** \details
  Retrieves whether the image cropping option is enabled.
  \returns true if the image cropping option is enabled; otherwise, the method returns false.
  */
  bool imageCropping() const { return m_bCropImages; }

  /** \details
  Sets a new value of the image cropping option.
  \param bEnable [in] A new option value to be set.
  \remarks 
  The option value is equal to true if the image cropping option is enabled; otherwise, the option value is equal to false.
  */
  void setImageCropping(bool bEnable) { m_bCropImages = bEnable; }

  /** \details
  Retrieves the currently used quality of DCT compression.
  \returns An unsigned 16-bit integer value that represents the compression quality level.
  */
  OdUInt16 dctQuality() const { return m_DCTQuality; }

  /** \details
  Sets a new value of the quality of DCT compression.
  \param quality [in] A quality value to be set.
  */
  void setDCTQuality(OdUInt16 quality)
  {
    if (quality > 100)
      m_DCTQuality = 100;
    else if (quality < 10)
      m_DCTQuality = 10;
    else
      m_DCTQuality = quality;
  }

  /** \details
  Retrieves whether the extents calculation mode is enabled.
  \returns true if the extents calculation mode is enabled; otherwise, the method returns false.  
  */
  bool useViewExtents() const { return m_useViewExtents; }

  /** \details
  Sets the extents calculation mode value.
  \param bViewExtents [in] A flag value that determines whether the extents calculation mode is switched on (if it is equal to true) or off (if it is equal to false).
  */
  void setUseViewExtents(bool bViewExtents) { m_useViewExtents = bViewExtents; }

  /** \details
  Retrieves whether the DCT compression option is enabled.
  \returns true if the DCT compression option is enabled; otherwise, the method returns false.
  */
  bool dctCompression() const { return m_bDCTCompression; }

  /** \details
  Sets the DCT compression option.
  \param bEnable [in] An option value. If its value is equal to true, the DCT compression is enabled.
  */
  void setDCTCompression(bool bEnable) { m_bDCTCompression = bEnable; }

  /** \details
  Retrieves whether the upscaling option is enabled.
  \returns true if the upscale option is enabled; otherwise, the method returns false.
  */
  bool upscaleImages() const { return m_bUpscaleImages; }

  /** \details
  Sets the upscaling option.
  \param bEnable [in] An option value. If its value is equal to true, the upscaling is enabled.
  */
  void setUpscaleImages(bool bEnable) { m_bUpscaleImages = bEnable; }

  /** \details
  Sets a new value of the shaded views transparency option.
  \param bEnable [in] A new option value to be set.
  \remarks 
  The option determines whether backgrounds of shaded views exported with a GS device are transparent. 
  The true value means transparency; the false value represents opaqueness.
  */
  void setTransparentShadedVpBg(bool bEnable) { m_TransparentShadedVpBg = bEnable; };

  /** \details
  Retrieves the transparency for shaded views option value.
  \returns true if the transparency for shaded views is enabled; otherwise, the method returns false.
  \remarks 
  The option determines whether backgrounds of shaded views exported with a GS device are transparent. 
  The true value means transparency; the false value represents opaqueness.
  */
  bool transparentShadedVpBg() const { return m_TransparentShadedVpBg; };

  /** \details
  Sets a new value of the disable GS device for shaded views option.
  \param bDisable [in] A new option value to be set.
  \remarks 
  If the option value is equal to true the GS device is disabled; otherwise (by default) the GS device is enabled.
  If GS device is disabled, shaded viewports will be exported as geometry, using pdf device, otherwise they will be exported as bitmap pictures
  */
  void setForceDisableGsDevice(bool bDisable) { m_ForceDisableGsDevice = bDisable; };

  /** \details
  Retrieves the current value of the disable gs device for shaded views option.
  \returns true if the disable gs device for shaded views option is set; otherwise, the method returns false.
  */
  bool forceDisableGsDevice() const { return m_ForceDisableGsDevice; };

  /** \details
  Sets a new value of the shaded viewports export mode.
  \param mode [in] A new option value to be set.
  */
  void setShadedVpExportMode(PDFShadedViewportExportMode mode) { m_ShadedVpExportMode = mode; };

  /** \details
  Retrieves the current value of the shaded viewports export mode.
  \returns A PDFShadedViewportExportMode enumeration value that represents the current value of the shaded viewports export mode.
  */
  PDFShadedViewportExportMode shadedVpExportMode() const { return m_ShadedVpExportMode; };

  /** \details
  Set the PRC objects compression option.
  \param compressionLevel       [in] Determines what compression level (depends on the used algorithm) is applied during the export process.
  \param bCompressBrep          [in] A Brep data compression flag. If the value is equal to true, the Brep data is compressed.
  \param bCompressTessellation  [in] A tesselation data compression flag. If the value is equal to true, the tesselation data is compressed.
  */
  void setPRCCompression(PDF3D_ENUMS::PRCCompressionLevel compressionLevel, bool bCompressBrep, bool bCompressTessellation)
  {
    if ((!bCompressBrep) && (!bCompressTessellation))
    {
      SETBIT_0(m_PRCCompressionLevel, 0x00000004);
      SETBIT_0(m_PRCCompressionLevel, 0x00000008);
      return;
    }

    m_PRCCompressionLevel = compressionLevel;
    SETBIT(m_PRCCompressionLevel, 0x00000004, bCompressBrep);
    SETBIT(m_PRCCompressionLevel, 0x00000008, bCompressTessellation);
  }

  /** \details
  Retrieves the current export reactor.
  \returns A raw pointer to the reactor object.
  \remarks
  This method is used for internal use only. It is strictly recommended do NOT use it in your custom source code.
  */
  PdfExportReactor* exportReactor() const { return m_exportReactor; }

  /** \details
  Sets a new export reactor object.
  \param reactor [in] A raw pointer to the reactor object.
  \remarks
  This method is used for internal use only. It is strictly recommended do NOT use it in your custom source code.
  */
  void setExportReactor(PdfExportReactor* reactor) { m_exportReactor = reactor; }

  /** \details 
  Retrieves whether XObjects are exported. 
  \returns true if XObjects are exported; otherwise, the method returns false.
  \remarks
  This method is used for internal use only. It is strictly recommended do NOT use it in your custom source code.
  */
  bool export2XObject() const { return m_Export2XObject; }

  /** \details 
  Sets a new value of the XObject export flag. 
  XObject export flag determines whether XObjects are exported (if it is equal to true) or not (if it is equal to false).
  \param bEnable [in] A new flag value to be set. 
  \remarks
  This method is used for internal use only. It is strictly recommended do NOT use it in your custom source code.
  */
  void setExport2XObject(bool bEnable) { m_Export2XObject = bEnable; }

  /** \details
  Adds a new watermark.
  \param wm [in] A watermark object to be added.
  */
  void addWatermark(Watermark wm) { m_Watermarks.append(wm); };

  /** \details
  Retrieves the array of watermarks.
  \returns An array of Watermarks objects associated with the set of the export parameters.
  */
  const OdArray<Watermark>& watermarks() const { return m_Watermarks; };
  
  /** \details
  Clears the Watermarks array.
  */
  void clearWatermarks() { m_Watermarks.clear(); };

  /** \details
  Clears multiple db settings (array of database and array of indexes).
  */
  void clearMultipleDbSettings()
  {
    m_Databases.clear();
    m_layouts.clear();

    m_pDb = 0;
  }

  /** \details
  Retrieves the current value of the GsCache option.
  \returns true if the cache is enabled; otherwise, the method returns false.
  */
  bool useGsCache() const { return m_useGsCache; }

  /** \details
  Sets the GsCache option.
  \param bEnable [in] An option value. If its value is equal to true, the GsCache is enabled. To switch off the option set this parameter to false.
  */
  void setUseGsCache(bool bEnable) { m_useGsCache = bEnable; }

  /** \details
  Gets the current value of the multithreaded vectorization option.
  \returns true if the multithreaded vectorization is enabled, or false otherwise.
  */
  bool isParallelVectorization() const { return m_bParallelVectorization; }

  /** \details
  Sets the multithreaded vectorization option.
  \param bOn [in] Multithreaded vectorization flag. If it's true, the multithreaded vectorization is enabled. To switch off the option, set this parameter to false.
  \remarks
  *<b>Currently not implemented.</b>*
  */
  void setParallelVectorization(bool bOn) { /*m_bParallelVectorization = bOn;*/ }

  /** \details
  Gets the current value of the multithreaded display option.
  \returns true if the multithreaded display is enabled, or false otherwise.
  */
  bool isParallelDisplay() const { return m_bParallelDisplay; }

  /** \details
  Sets the multithreaded display option.
  \param bOn [in] Multithreaded display flag. If it's true, the multithreaded display is enabled. To switch off the option, set this parameter to false.
  \remarks
  *<b>Currently not implemented.</b>*
  */
  void setParallelDisplay(bool bOn) { /*m_bParallelDisplay = bOn;*/ }

  /** \details
  Determines whether the searchable text is exported as invisible text.
  \returns true if the searchable text is exported as invisible text; otherwise, the method returns false.
  */
  bool searchableTextAsHiddenText() const { return m_bSearchableTextAsHiddenText; }

  /** \details
  Determines whether the searchable text is exported from rendered views.
  \returns true if the searchable text is exported from rendered views; otherwise, the method returns false.
  */
  bool searchableTextInRenderedViews() const { return m_bSearchableTextInRenderedViews; }

  /** \details
  Sets a new value of the invisible text flag. 
  The invisible text flag defines whether the searchable text is exported as invisible text (if equals to true).
  \param bOn [in] A new flag value to be set.
  */
  void setSearchableTextAsHiddenText(bool bOn) { m_bSearchableTextAsHiddenText = bOn; }

  /** \details
  Sets a new value of the export from rendered views flag. 
  The export from rendered views flag defines whether the searchable text is exported 
  from rendered views (if equals to true) or not (if equals to false).
  \param bOn [in] A new flag value to be set.
  */
  void setSearchableTextInRenderedViews(bool bOn) { m_bSearchableTextInRenderedViews = bOn; }

  /** \details
  Determines whether TTF fonts are exported as geometry.
  \returns true if TTF fonts are exported as geometry; otherwise, the method returns false.
  */
  bool isTTFTextAsGeometry() const { return GETBIT(exportFlags(), PDFExportParams::kTTFTextAsGeometry); }

  /** \details
  Determines whether SHX fonts are exported as geometry.
  \returns true if SHX fonts are exported as geometry; otherwise, the method returns false.
  */
  bool isSHXTextAsGeometry() const { return GETBIT(exportFlags(), PDFExportParams::kSHXTextAsGeometry); }

//DOM-IGNORE-BEGIN
private:
  OdDbBaseDatabase *m_pDb;           // Pointer to the database for export.
  PDFExportVersions m_ver;           // Only PDF 1.4 supported & 1.5 for layers functionality.
  OdStreamBufPtr    m_outputStream;  // Pointer to the stream buffer for PDF export implementation.

  OdStringArray           m_layouts;     // Layouts to export. If zero size array, active layout will be exported.
  OdArray<OdGsPageParams> m_pageParams;  // Page parameters: paper size, margins, in mm.
  // For allowing export of layouts from multiple databases to 1 multi-sheet pdf file)		
  OdRxObjectPtrArray      m_Databases;   // (One entry for each database containing layouts to be exported.)

  ODCOLORREF        m_background;    // Background color. Default: white.
  const ODCOLORREF* m_pPalette;      // Palette to be used. If NULL, one of two default palettes will be used depending on background color.

  OdUInt16          m_hatchDPI; // PdfExport exports hatch entity as bitmap. This value controls DPI resolution for images created during hatch export.
  //OdUInt16          view3dDPI; // 3d viewports exported as images if bUseHLR set to true. This value controls DPI of such inages.
  OdUInt16          m_VectorResolution; // Used for vector resolution. Standard values are 72, 150, 200, 300, 400, 600, 720, 1200, 2400 and 4800 DPI. Default value is 600 DPI
  OdUInt16          m_colorImagesDPI;//Used for control resolution for color and grayscale images. Cannot exceed m_VectorResolution value
  OdUInt16          m_bwImagesDPI;//Used for control resolution for monochrome images. Cannot exceed m_VectorResolution value

  ExportHatchesType m_solidHatchesExportType;
  ExportHatchesType m_gradientHatchesExportType;
  ExportHatchesType m_otherHatchesExportType;

  PDFExportFlags m_flags; // PDF export flag combination used for export.

  SearchableTextType m_searchableTextType;//can be used if SHX or TTF text exported as geometry
  ColorPolicy        m_colorPolicy;

  // All next fields are optional (can be ""). If Producer is null, it will be "ODA PDF Export vX.X".
  OdString m_Title;    // The document title.
  OdString m_Author;   // The name of the person who created the document.
  OdString m_Subject;  // The subject of the document.
  OdString m_Keywords; // Keywords associated with the document.
  OdString m_Creator;  // If the document was converted to PDF from another format, the name of the application (for example, Adobe FrameMaker) that created the original document from which it was converted.
  OdString m_Producer; // If the document was converted to PDF from another format, the name of the application (for example, Acrobat Distiller) that converted it to PDF.
  
  OdArray<Watermark> m_Watermarks; //PDF watermarks (requires version 1.6)

  bool                  m_bCropImages;     // Enable bitmap cropping(crop invisible parts of bitmaps)
  bool                  m_bDCTCompression; // DCT compression for raster images
  OdUInt16              m_DCTQuality;      // Quality of DCT compression
  bool                  m_bUpscaleImages;  // Upscale images if they resolution is lower than settled image resolution. Default: false

  bool                  m_useViewExtents;  // Use view extents instead of plot extents (default) for calculation of drawing extents

  PDF_A_mode            m_pdfAMode;        //PDF/A mode - default value None
  //PDF/A is not compatible with PRC at all (by ISO standard) so the error will be returned if both modes are set
  //Optimized TTF fonts aren't supported yet, so this flag will be unset if PDF/A mode is chosen
  //For more details, see documentation

  //PRC related params
  PRCSupport                    m_PRCMode;
  OdUInt32                      m_PRCCompressionLevel;  //PRCCompressionLevel and 0x00000004 is bCompressBrep and 0x00000008 is bCompressTessellation
  OdRxObjectPtr                 m_PRCContext;           // user-defined PRC context
  bool                          m_prcHasBackground;     // True => Prc backgrounds should be enabled
  ODCOLORREF                    m_prcBackground;        // Default background color for PRC. Default: white.
  PDF3D_ENUMS::PRCRenderingMode m_PRCRenderMode;

  bool                          m_Export2XObject;
  bool                          m_ForceDisableGsDevice; //Force disable Gs device to export shaded views. Default value - false
  bool                          m_TransparentShadedVpBg;//Make background of shaded views, exported with Gs device, transparent. Default value - false
  PDFShadedViewportExportMode   m_ShadedVpExportMode;   //Shaded viewport export policy

  PdfExportReactor*             m_exportReactor;        //Reactor to handle pdf export actions on caller level.

  bool                          m_useGsCache;              //Use GsCache for pdf export. Default value - false
  bool                          m_bParallelVectorization; //Do MT vectorization when Gs cache is enabled. Default value - false
  bool                          m_bParallelDisplay;       //Do MT display when Gs cache is enabled. Default value - false
  bool                          m_bSearchableTextAsHiddenText; // When text is exported as geometry, and searchable text is required, export it as hidden text available for Ctrl+F search. Default value - false
  bool                          m_bSearchableTextInRenderedViews; // Text in render views exported as a hidden one behind a bitmap, available for Ctrl+F search. Default value - false
//DOM-IGNORE-END

public:
  /** Reserved parameter: bit 1 - ZoomToExtents auxiliary flag - default value 0 [ExactExtents]. For internal use only. 
  It is strongly NOT recommended to use it in your code!*/
  OdUInt16          m_reserved1;  
  static const PDFExportVersions currentVersion = kPDFv1_7;
};
}
#endif //_PDFEXPORTPARAMS_INCLUDED_

