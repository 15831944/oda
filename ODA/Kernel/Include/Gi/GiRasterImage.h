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

#ifndef _OD_ODGIRASTERIMAGE_H_
#define _OD_ODGIRASTERIMAGE_H_

#include "RxObject.h"

#include "TD_PackPush.h"

/** \details
  This class represents a raster image within the ODA Platform vectorization
  framework.
  Corresponding C++ library: TD_Gi

  <group OdGi_Classes>
  \sa
  <link gi_raster_images, Work with Raster Images>
*/
class FIRSTDLL_EXPORT OdGiRasterImage : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdGiRasterImage);
  //DOM-IGNORE-END

  /** \details
    This enumeration represents measurement units.
  */
  enum Units
  {
    /** No measurement units are used. */
    kNone = 0,
    /** Millimeters are used. */
    kMillimeter,
    /** Centimeters are used. */
    kCentimeter,
    /** Meters are used. */
    kMeter,
    /** Kilometers are used. */
    kKilometer,
    /** Inches are used. */
    kInch,
    /** Foots are used. */
    kFoot,
    /** Yards are used. */
    kYard,
    /** Miles are used. */
    kMile,
    /** Microinches are used. */
    kMicroinches,
    /** Mils (thousandths of an inch) are used. */
    kMils,
    /** Angstroms (10^-10 of a meter or ten-billionths of a meter) are used. */
    kAngstroms,
    /** Nanometers are used. */
    kNanometers,
    /** Microns are used. */
    kMicrons,
    /** Decimeters are used. */
    kDecimeters,
    /** Dekameters (10 meters) are used. */
    kDekameters,
    /** Hectometers (10^2 meters) are used. */
    kHectometers,
    /** Gigameters (10^9 meters) are used. */
    kGigameters,
    /** Astronominal units (149597870700 meters) are used. */
    kAstronomical,
    /** Light years (9460730472580800 meters) are used. */
    kLightYears,
    /** Parsecs (approx 3.261563777 light years) are used. */
    kParsecs
  };

  /** \details
    Returns number of millimeters in a given unit.

    \param units [in] Input measurement unit.

    \returns Number of millimeters in the given measurement unit.
  */
  static double millimetersInUnit(Units units)
  {
    switch (units)
    {
    case kNone:
      throw OdError(eInvalidInput);
    case kMillimeter:
      return 1.0;
    case kCentimeter:
      return 10.0;
    case kMeter:
      return 1000.0;
    case kKilometer:
      return 1000000.0;
    case kInch:
      return 25.4;
    case kFoot:
      return 25.4 * 12.0;
    case kYard:
      return 25.4 * 12.0 * 3.;
    case kMile:
      return 25.4 * 12.0 * 3. * 1760.;
    case kMicroinches:
      return 25.4 * 0.000001;
    case kMils:
      return 25.4 * 0.001;
    case kAngstroms:
      return 1000.0 * 1.e-10;
    case kNanometers:
      return 0.000001;
    case kMicrons:
      return 0.001;
    case kDecimeters:
      return 100.0;
    case kDekameters:
      return 10000.0;
    case kHectometers:
      return 100000.0;
    case kGigameters:
      return 1000. * 1.e9;
    case kAstronomical:
      return 1000. * 149597870700.;   // Approx. the length of the semi-major axis of the Earth's elliptical orbit around the Sun
    case kLightYears:
      return 1000. * 9460730472580800.;
    case kParsecs:
      return 1000. * 30856775814671900.;
    }
    ODA_ASSERT_ONCE(!"Unknown unit");
    return 1.0;
  }

  /** \details
    This enumeration represents sources for raster images.
  */
  enum ImageSource
  {
    /** Flag not set (unknown source). */
    kUndefinedSource = -1,
    /** Raster image loaded from raster image definition. */
    kFromDwg         = 0,
    /** Raster image generated from OLE data source. */
    kFromOleObject   = 1,
    /** Raster image generated from rendering. */
    kFromRender      = 2,

    /** Raster image is a part of underlay. */
    kFromUnderlay    = 10,
    /** Raster image wraps an OdGiImageBGRA32 image. */
    kFromImageBGRA32 = 11,

    // Aliases
    /** Raster image generated from PdfUnderlay. */
    kFromPdfUnderlay = kFromOleObject,
    /** Raster image generated from a disk file. */
    kFromFile        = kFromDwg
  };

  /** \details
    This enumeration represents transparency modes.
  */
  enum TransparencyMode
  {
    /** Default for native raster image (alpha 0 is transparent, 1-255 is opaque). */
    kTransparencyDef  = -1,
    /** Alpha ignored for 32bpp formats. */
    kTransparencyOff  = 0,
    /** Alpha 0-254 is transparent, 255 is opaque. */
    kTransparency1Bit = 1,
    /** Complete alpha level support for 32bpp formats. */
    kTransparency8Bit = 2
  };

  /** \details
    Represents data for pixel formatting.
  */
  typedef struct tagPixelFormatInfo
  {
    /** Red bits offset. */
    OdUInt8 redOffset;
    /** Amount of bits for red channel. */
    OdUInt8 numRedBits;
    /** Green bits offset. */
    OdUInt8 greenOffset;
    /** Amount of bits for green channel. */
    OdUInt8 numGreenBits;
    /** Blue bits offset. */
    OdUInt8 blueOffset;
    /** Amount of bits for blue channel. */
    OdUInt8 numBlueBits;
    /** Alpha bits offset. */
    OdUInt8 alphaOffset;
    /** Amount of bits for alpha channel. */
    OdUInt8 numAlphaBits;
    /** Amount of bits per pixel. */
    OdUInt8 bitsPerPixel;

    /** \details
      Checks whether the passed pixel format data is equal to this pixel format
      data.

      \param other [in]  Pixel format data to compare.

      \returns
      True if pixel format data is equal to this pixel format data, false
      otherwise.
    */
    bool operator ==(const struct tagPixelFormatInfo& other) const
    {
      return (redOffset == other.redOffset) && (greenOffset == other.greenOffset) &&
        (blueOffset == other.blueOffset) && (alphaOffset == other.alphaOffset) && (bitsPerPixel == other.bitsPerPixel) &&
        (numRedBits == other.numRedBits) && (numGreenBits == other.numGreenBits) && (numBlueBits == other.numBlueBits) &&
        (numAlphaBits == other.numAlphaBits);
    }

    /** \details
      Constructor for instances of the tagPixelFormatInfo structure. Initializes
      all data members to 0.
    */
    tagPixelFormatInfo()
      : redOffset(0)
      , numRedBits(0)
      , greenOffset(0)
      , numGreenBits(0)
      , blueOffset(0)
      , numBlueBits(0)
      , alphaOffset(0)
      , numAlphaBits(0)
      , bitsPerPixel(0)
    {}

    /** \details
      Checks whether the pixel format of this RasterImage is RGB.
      \returns
      True if the pixel format of this RasterImage object is RGB, false
      otherwise.
    */
    bool isRGB() const
    {
      return bitsPerPixel == 24 &&
        redOffset == 0 && greenOffset == 8 && blueOffset == 16 && numRedBits == 8 && numGreenBits == 8 && numBlueBits == 8;
    }

    /** \details
      Sets the pixel format of this RasterImage object to RGB.

      \remarks
      No palette conversion is performed.
    */
    void setRGB()
    {
      bitsPerPixel = 24;
      redOffset    = 0;
      greenOffset  = 8;
      blueOffset   = 16;
      numRedBits   = 8;
      numGreenBits = 8;
      numBlueBits  = 8;
    }

    /** \details
      Checks whether the pixel format of this RasterImage object is BGR.

      \returns
      True if the pixel format of this RasterImage object is BGR, false
      otherwise.
    */
    bool isBGR() const
    {
      return bitsPerPixel == 24 &&
        redOffset == 16 && greenOffset == 8 && blueOffset == 0 && numRedBits == 8 && numGreenBits == 8 && numBlueBits == 8;
    }

    /** \details
      Sets the pixel format of this RasterImage object to BGR.

      \remarks
      No palette conversion is performed.
    */
    void setBGR()
    {
      bitsPerPixel = 24;
      redOffset    = 16;
      greenOffset  = 8;
      blueOffset   = 0;
      numRedBits   = 8;
      numGreenBits = 8;
      numBlueBits  = 8;
    }

    /** \details
      Checks whether the pixel format of this RasterImage object is 16-bit BGR.

      \returns
      True if the pixel format of this RasterImage object is 16-bit BGR, false
      otherwise.
    */
    bool is16bitBGR() const
    {
      return bitsPerPixel == 16 &&
        redOffset == 10 && greenOffset == 5 && blueOffset == 0 && numRedBits == 5 && numGreenBits == 5 && numBlueBits == 5;
    }

    /** \details
      Sets the pixel format of this RasterImage object to 16-bit BGR.

      \remarks
      No palette conversion is performed.
    */
    void set16bitBGR()
    {
      bitsPerPixel = 16;
      redOffset    = 10;
      greenOffset  = 5;
      blueOffset   = 0;
      numRedBits   = 5;
      numGreenBits = 5;
      numBlueBits  = 5;
    }

    /** \details
      Checks whether the pixel format of this RasterImage object is RGBA.

      \returns
      True if the pixel format of this RasterImage object is RGBA, false
      otherwise.
    */
    bool isRGBA() const
    {
      return bitsPerPixel == 32 && alphaOffset == 24 && numAlphaBits == 8 &&
        redOffset == 0 && greenOffset == 8 && blueOffset == 16 && numRedBits == 8 && numGreenBits == 8 && numBlueBits == 8;
    }

    /** \details
      Sets the pixel format of this RasterImage object to RGBA.

      \remarks
      No palette conversion is performed.
    */
    void setRGBA()
    {
      bitsPerPixel = 32;
      alphaOffset  = 24;
      numAlphaBits = 8;
      redOffset    = 0;
      greenOffset  = 8;
      blueOffset   = 16;
      numRedBits   = 8;
      numGreenBits = 8;
      numBlueBits  = 8;
    }

    /** \details
      Checks whether the pixel format of this RasterImage object is BGRA.

      \returns
      True if the pixel format of this RasterImage object is BGRA, false
      otherwise.
    */
    bool isBGRA() const
    {
      return bitsPerPixel == 32 && alphaOffset == 24 && numAlphaBits == 8 &&
        redOffset == 16 && greenOffset == 8 && blueOffset == 0 && numRedBits == 8 && numGreenBits == 8 && numBlueBits == 8;
    }

    /** \details
      Sets the pixel format of this RasterImage object to BGRA.

      \remarks
      No palette conversion is performed.
    */
    void setBGRA()
    {
      bitsPerPixel = 32;
      alphaOffset  = 24;
      numAlphaBits = 8;
      redOffset    = 16;
      greenOffset  = 8;
      blueOffset   = 0;
      numRedBits   = 8;
      numGreenBits = 8;
      numBlueBits  = 8;
    }
  } PixelFormatInfo; // Represents data for pixel formatting.

  /** \details
    Retrieves the image width in pixels of this RasterImage object.

    \returns
    Number of pixels as an OdUInt32 value that represents width of this raster
    image.
  */
  virtual OdUInt32 pixelWidth() const = 0;

  /** \details
    Retrieves the image height in pixels of this RasterImage object.

    \returns
    Number of pixels as an OdUInt32 value that represents height of this raster
    image.
  */
  virtual OdUInt32 pixelHeight() const = 0;

  /** \details
    Retrieves the default image resolution in pixels per unit of this
    RasterImage object.

    \param xPelsPerUnit [out]  Receives the pixels per unit value (x direction).
    \param yPelsPerUnit [out]  Receives the pixels per unit value (y direction).

    \returns
    Units that are used to measure image dimensions.

    \remarks
    If the returned value is kNone, then xPelsPerUnit and yPelsPerUnit are not set.
  */
  virtual Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;

  /** \details
    Retrieves the number of bits per pixel used for colors by this RasterImage
    object.

    \returns
    Number of bits per pixel that represents color depth.
  */
  virtual OdUInt32 colorDepth() const = 0;

  /** \details
    Retrieves the number of colors in the palette of this RasterImage object.

    \returns
    Number of colors in the palette.
  */
  virtual OdUInt32 numColors() const = 0;

  /** \details
    Retrieves the index of transparent color in the image.

    \returns
    The default implementation of this function returns -1.

    \remarks
    If there is no transparent color, -1 should be returned.
  */
  virtual int transparentColor() const;

  /** \details
    Retrieves the color for the specified index from the palette of this
    RasterImage object.

    \param colorIndex [in]  Color index.

    \returns
    Color at the specified index as an ODCOLORREF (OdUInt32) value.
  */
  virtual ODCOLORREF color(OdUInt32 colorIndex) const = 0;

  /** \details
    Retrieves the size (in bytes) of the palette data of this RasterImage
    object.

    \returns
    Number of bytes that represent palette data size.
  */
  virtual OdUInt32 paletteDataSize() const = 0;

  /** \details
    Retrieves the palette in BMP format of this RasterImage object.

    \param bytes [out]  Receives the palette data.

    \remarks
    It is up to the caller to allocate sufficient memory for the palette data.
  */
  virtual void paletteData(OdUInt8* bytes) const = 0;

  /** \details
    Retrieves the scanline size of this RasterImage object.

    \returns
    Number of bytes between the beginning of the scanline N and the beginning of
    the scanline N+1 (taking into account any padding that is added to the end
    of the scanline).
  */
  virtual OdUInt32 scanLineSize() const;

  /** \details
    Retrieves the set of scanlines.

    \returns
    Set of scanlines.

    \remarks
    Implementation of this function is optional; NULL can be returned if it is
    inconvenient to implement. The caller must take into account that the return
    value can be NULL. If this method returns NULL, scanlines can be accessed by
    the second version of the scanLines() method with arguments where a memory
    buffer for scanline data is preallocated by the caller.
  */
  virtual const OdUInt8* scanLines() const = 0;

  /** \details
    Retrieves the specified set of scanlines in BMP format from this RasterImage
    object or the pixel data in BMP format for this RasterImage object.

    \param scnLines     [out]  Receives the scanline data.
    \param firstScanline [in]  Index of the first scanline to retrieve.
    \param numLines      [in]  Number of scanlines to retrieve.

    \remarks
    * The number of accessible scanlines is equal to the value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \remarks
    It us up to the caller to allocate sufficient memory for the scanline data.
  */
  virtual void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const = 0;

  /** \details
    Retrieves the pixel format for this RasterImage object.

    \returns
    Pixel format of a palette or raster image.

    \remarks
    This function returns the pixel format of the palette, unless there is no
    palette, in which case it returns the pixel format of the image itself.
  */
  virtual PixelFormatInfo pixelFormat() const = 0;

  /** \details
    Retrieves the scanline alignment in bytes for this RasterImage object.
    Example:
    scanLinesAlignment() returns 4 for Windows BMP.

    \returns
    Scanline alignment as an OdUInt32 value.
  */
  virtual OdUInt32 scanLinesAlignment() const = 0;

  /** \details
    Retrieves the image source.

    \returns
    Image source represented by a value from the ImageSource enumeration.
  */
  virtual ImageSource imageSource() const;

  /** \details
    Retrieves the image source file name.
    \returns
    Full path to the file from which the image is loaded.
  */
  virtual const OdString &sourceFileName() const;

  /** \details
    Retrieves the transparency mode for 32bpp formats.

    \returns
    Transparency mode represented by a value from the TransparencyMode
    enumeration.
  */
  virtual TransparencyMode transparencyMode() const;

  /** \details
    Sets an image source and (optionally) source file name.

    \param source    [in]  New image source value.
    \param pFileName [in]  Optional value for source file name.

    \returns
    Smart pointer to the image with the new image source.
  */
  OdSmartPtr<OdGiRasterImage> changeImageSource(ImageSource source, const OdChar *pFileName = NULL);

  /** \details
    Sets an image source file name.

    \param fileName [in]  New value for source file name.

    \returns
    Smart pointer to the image with the new image source file name.
  */
  OdSmartPtr<OdGiRasterImage> changeSourceFileName(const OdString &fileName);

  /** \details
    Sets transparency mode for 32bpp formats.

    \param mode [in]  New transparency mode.

    \returns
    Smart pointer to the image with new transparency mode.
  */
  OdSmartPtr<OdGiRasterImage> changeTransparencyMode(TransparencyMode mode);

  /** \details
    Retrieves the size of a single scanline in BMP format for this RasterImage
    object.

    \param pixelWidth [in]  Image width in pixels.
    \param colorDepth [in]  Number of bits per pixel.

    \returns
    The size of a single scanline in BMP format as an OdUInt32 value.
  */
  static OdUInt32 calcBMPScanLineSize(OdUInt32 pixelWidth, int colorDepth);

  /** \details
    Retrieves the 32-bit mask for the requested number of color bits and offset.

    \param numColorBits [in]  Number of color bits.
    \param colorOffset  [in]  Offset of color bits.

    \returns
    Color mask as an OdUInt32 value.
  */
  static OdUInt32 calcColorMask(OdUInt8 numColorBits, OdUInt8 colorOffset = 0);

  /** \details
    Converts this raster image as specified.

    \param convertPaletteToRGB [in]  Color is converted to RGB if and only if true.
    \param brightness          [in]  Image brightness [0 .. 100].
    \param contrast            [in]  Image contrast [0 .. 100].
    \param fade                [in]  Image fade value [0 .. 100].
    \param backgroundColor     [in]  Background color.
    \param flipX               [in]  Image is flipped about the X-axis if and only if true.
    \param flipY               [in]  Image is flipped about the Y-axis if and only if true.
    \param rotate90            [in]  Image is rotated 90° if and only if true.
    \param pDestDesc           [in]  Pointer to an OdGiRasterImageDesc object to receive the image parameters.
    \param transparency        [in]  Image is transparent if true.

    \returns
    Smart pointer to the converted image.
  */
  OdSmartPtr<OdGiRasterImage> convert(bool convertPaletteToRGB,
    double brightness = 50.0, double contrast = 50.0, double fade = 0.0, ODCOLORREF backgroundColor = 0,
    bool flipX = false, bool flipY = false, bool rotate90 = false,
    const OdGiRasterImage* pDestDesc = 0,
    bool transparency = false) const;

  /** \details
    Crops this raster image using a rectangle. The format of returned raster
    image is identical to the format of the source raster image.

    \param x      [in] X-coordinate of the start point.
    \param y      [in] Y-coordinate of the start point.
    \param width  [in] Width of the crop rectangle.
    \param height [in] Height of the crop rectangle.

    \returns
    Smart pointer to the cropped image.

    \remarks
    All arguments are in pixels.
  */
  virtual OdSmartPtr<OdGiRasterImage> crop(OdUInt32 x, OdUInt32 y, OdUInt32 width, OdUInt32 height) const;

  /** \details
    Retrieves a user-end implementation of the raster image.

    \remarks
    If not overridden by the user-end raster image implementation, always
    returns NULL.
  */
  virtual void *imp() const;
};

/** \details
  A data type that represents a smart pointer to an
  <link OdGiRasterImage, OdGiRasterImage> object.
*/
typedef OdSmartPtr<OdGiRasterImage> OdGiRasterImagePtr;

/** \details
  This interface simply extends GiRasterImage to set the image source and
  transparency mode values. If the raster image can be cast to this interface,
  values will be set within this interface; otherwise the raster image wrapper
  will be created to reset these values.

  Corresponding C++ library: TD_Gi
  <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiRasterImageParam : public OdGiRasterImage
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdGiRasterImageParam);
  //DOM-IGNORE-END

  /** \details
    This enumeration represents parameter types for raster images.
  */
  enum ParamType
  {
    /** Image source parameter type. */
    kImageSource      = (1 << 0),
    /** Transparency mode parameter type. */
    kTransparencyMode = (1 << 1),
    /** Image source file name parameter type. */
    kSourceFileName   = (1 << 2)
  };

  /** \details
    Retrieves the ParamType bitflags with which parameters are supported by this
    interface.

    \returns
    Bitflags that represent supported parameters.
  */
  virtual OdUInt32 supportedParams() const = 0;

  /** \details
    Sets the image source.

    \param source [in]  New image source value.
  */
  virtual void setImageSource(ImageSource source) { }

  /** \details
    Sets the image source file name.

    \param fileName [in]  New value for the image source file name.
  */
  virtual void setSourceFileName(const OdString &fileName) { }

  /** \details
    Sets the image transparency mode.

    \param mode [in]  New transparency mode value.
  */
  virtual void setTransparencyMode(TransparencyMode mode) { }
};

/** \details
  A data type that represents a smart pointer to an
  <link OdGiRasterImageParam, OdGiRasterImageParam> object.
*/
typedef OdSmartPtr<OdGiRasterImageParam> OdGiRasterImageParamPtr;

#include "TD_PackPop.h"

#endif //#ifndef _OD_ODGIRASTERIMAGE_H_
