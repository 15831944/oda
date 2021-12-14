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

#ifndef __OD_GI_RASTER_WRAPPERS__
#define __OD_GI_RASTER_WRAPPERS__

#include "Gi/GiExport.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiImage.h"
#include "UInt8Array.h"

#include "TD_PackPush.h"

/** \details
    This class is a dummy implementation of the OdGiRasterImage interface.

    \remarks
    This class is intended to be used to preset image parameters for 
    OdDbRasterImageDef objects without the actual loading of raster image files.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiRasterImageDesc : public OdGiRasterImageParam
{
  OdUInt32                          m_pixelWidth, m_pixelHeight, m_colorDepth, m_palSize, m_alignment;
  OdGiRasterImage::PixelFormatInfo  m_pixFmt;
  Units                             m_units;
  double                            m_xPelsPerUnit, m_yPelsPerUnit;
  OdUInt8*                          m_pPal;
  OdGiRasterImage::ImageSource      m_imageSource;
  OdGiRasterImage::TransparencyMode m_transparencyMode;
  OdString                          m_fileName;
public:
  /** \details
    Default constructor for the OdGiRasterImageDesc class. Sets the following data members:
    <table>
    Name                  Value             Description
    m_pixelWidth          0                 Image width in pixels.
    m_pixelHeight         0                 Image height in pixels.
    m_colorDepth          0                 Number of bits that represent image color depth.
    m_palSize             0                 Color palette size.
    m_alignment           1                 Scanline alignment in bytes.
    m_units               kNone             Measurement units.
    m_xPelsPerUnit        0.0               Pixel per units in the x direction.
    m_yPelsPerUnit        0.0               Pixel per units in the y direction.
    m_pPal                NULL              Pointer to color palette.
    m_imageSource         kUndefinedSource  Image source type.
    m_transparencyMode    kTransparencyDef  Transparency mode.
    </table>
  */
  OdGiRasterImageDesc();
  
  /** \details
    Destructor for the OdGiRasterImageDesc class, clears palette colors.
  */
  virtual ~OdGiRasterImageDesc();

  /** \details
    Creates an OdGiRasterImageDesc object with the specified parameters. 
    \param pixelWidth [in]  Image width in pixels.
    \param pixelHeight [in]  Image height in pixels.
    \param units [in]  Units / pixel.
    \param xPelsPerUnit [in]  Pixels per unit value (x direction).
    \param yPelsPerUnit [in]  Pixels per unit value (y direction).

    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(OdUInt32 pixelWidth, OdUInt32 pixelHeight, Units units = kNone, double xPelsPerUnit = 0.0, double yPelsPerUnit = 0.0);
  
  /** \details
    Creates an OdGiRasterImageDesc object with the specified parameters. 
    \param pixelWidth [in]  Image width in pixels.
    \param pixelHeight [in]  Image height in pixels.
    \param colorDepth [in]  Number of bits per pixel used for colors.
    \param units [in]  Units / pixel.
    \param xPelsPerUnit [in]  Pixels per unit value (x direction).
    \param yPelsPerUnit [in]  Pixels per unit value (y direction).
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(OdUInt32 pixelWidth, OdUInt32 pixelHeight, OdUInt32 colorDepth, Units units = kNone, double xPelsPerUnit = 0.0, double yPelsPerUnit = 0.0);
  
  /** \details
    Creates an OdGiRasterImageDesc object with the specified parameters. 

    \param pSourceImage [in]  Source raster image to copy parameters from.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage *pSourceImage);

  /** \details
    Copies parameters from an original OdGiRasterImage object.
    \param pSourceImage [in]  Pointer to the original image object.
  */
  void setFrom(const OdGiRasterImage *pSourceImage);
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
    \returns
    smart pointer to the cloned object.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
  /** \details
    Retrieves the image width in pixels of this RasterImage object.
    \returns
    image width in pixels.
  */
  OdUInt32 pixelWidth() const;
  /** \details
    Retrieves the image height in pixels of this RasterImage object.
    \returns
    image height in pixels.
  */
  OdUInt32 pixelHeight() const;
   /** \details
    Retrieves the number of bits per pixel used for colors by this RasterImage object.
    \returns
    number of bits per pixel that represents color depth.
  */
  OdUInt32 colorDepth() const;
  /** \details
    Retrieves the number of colors in the palette of this RasterImage object.
    \returns
    number of bits per pixel that represents color depth.
  */
  OdUInt32 numColors() const;
  /** \details
    Retrieves the color for the specified index from the palette of this RasterImage object.
    \param colorIndex [in]  Color index.
    \returns
    color at the specified index as ODCOLORREF (OdUInt32 value).
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
    Retrieves the size (in bytes) of the palette data of this RasterImage object.
    \returns
    palette data size as OdUInt32 value.
  */
  OdUInt32 paletteDataSize() const;
  /** \details
    Retrieves the palette in BMP format of this RasterImage object.
    \param bytes [out]  Receives the palette data.
    
    \remarks
    It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const;
  /** \details
    Retrieves the scanline size of this RasterImage object.
    
    \returns
    the number of bytes between the beginning of scanline N and
    the beginning of scanline N+1 (taking into account any padding that is added to the 
    end of the scanline).
  */
  OdUInt32 scanLineSize() const;
  /** \details
    Retrieves scan lines for this object.
    \returns 
    NULL for an OdGiRasterImageDesc object.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Retrieves the specified set of scanlines in BMP format for this RasterImage object,
    or the pixel data in BMP format for this RasterImage object.

    \param scnLines [out]  Receives the scanline data.
    \param firstScanline [in]  Index of first scanline to retrieve.
    \param numLines [in]  Number of scanlines to retrieve.

    \remarks
    * The number of accessible scanlines is equal to the value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \remarks
    It us up to the caller to allocate sufficient memory for the scanline data.

    \remarks
    Returns nothing for an OdGiRasterImageDesc object.
  */
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Retrieves the pixel format for this RasterImage object.
    
    \returns
    the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself.
  */
  PixelFormatInfo pixelFormat() const;
  /** \details
    Retrieves the scanline alignment, in bytes, for this RasterImage object.
    Example:
    scanLinesAlignment() returns 4 for Windows BMP.
    \returns
    scanline alignment as OdUInt32 value.
  */
  OdUInt32 scanLinesAlignment() const;
  /** \details
    Retrieves the default image resolution in pixels per unit of this RasterImage object.

    \param xPelsPerUnit [out]  Receives the pixels per unit value (x direction).
    \param yPelsPerUnit [out]  Receives the pixels per unit value (y direction).
    \returns
    units that are used to measure image dimensions.
    \remarks
    If the returned value is kNone, then xPelsPerUnit and yPelsPerUnit are not set.
  */
  Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;

  /** \details
    Sets the image width in pixels of this RasterImageDesc object.
    \param pixelWidth [in]  Pixel width.
  */
  void setPixelWidth(OdUInt32 pixelWidth);
  /** \details
    Sets the image height in pixels of this RasterImageDesc object.
    \param pixelHeight [in]  Pixel height.
  */
  void setPixelHeight(OdUInt32 pixelHeight);

  /** \details
    Sets the number of bits per pixel used for colors by this RasterImageDesc object.
    \param colorDepth [in]  Color depth.
  */
  void setColorDepth(OdUInt32 colorDepth);
  /** \details
    Retrieves the pixel format for this RasterImage object.
    
    \returns
    the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself.
  */
  OdGiRasterImage::PixelFormatInfo& pixelFormat();

  /** \details
    Sets the palette in BMP format for this RasterImageDesc object.
    \param paletteData [in]  Palette data.
    \param paletteSize [in]  Palette size in bytes.
  */
  void setPalette(OdUInt32 paletteSize, OdUInt8* paletteData);

  /** \details
    Sets the scanline alignment, in bytes, for this RasterImage object.
    \param alignment [in]  Scanline alignment.
    Example:
    Alignment is 4 for Windows BMP.
  */
  void setScanLinesAlignment(OdUInt32 alignment);

  /** \details
    Sets the default raster image resolution for this RasterImage object.
    \param units [in]  Units / pixel.
    \param xPelsPerUnit [in]  Pixels per unit value (x direction).
    \param yPelsPerUnit [in]  Pixels per unit value (y direction).
  */
  void setDefaultResolution(Units units, double xPelsPerUnit, double yPelsPerUnit);
  /** \details
    Retrieves the ParamType bitflags with which parameters are supported by this interface.
    \returns
    bitflags that represent supported parameters.
  */
  OdUInt32 supportedParams() const;
  /** \details
    Retrieves the image source.
    \returns
    image source represented by a value from the OdGiRasterImage::ImageSource enumeration.
  */
  OdGiRasterImage::ImageSource imageSource() const;
  /** \details
    Sets an image source.
    \param source [in]  New image source value.
  */
  void setImageSource(OdGiRasterImage::ImageSource source);
  /** \details
    Retrieves the image source file name.
    \returns
    full path to a file from which image is loaded.
  */
  const OdString &sourceFileName() const;
  /** \details
    Sets an image source file name.
    \param fileName [in]  New value for image source file name.
  */
  void setSourceFileName(const OdString &fileName);
  /** \details
    Retrieves the transparency mode for 32bpp formats.
    \returns
    transparency mode represented by a value from the TransparencyMode enumeration.
  */
  OdGiRasterImage::TransparencyMode transparencyMode() const;
  /** \details
    Sets an image transparency mode.
    \param mode [in]  New transparency mode value.
  */
  void setTransparencyMode(OdGiRasterImage::TransparencyMode mode);
};

class OdGiImageBGRA32;

/** \details
    Emulates OdGiImageBGRA32 within the OdGiRasterImage interface.
    \remarks
    Stores only a pointer to the original image, not a SmartPointer, so deletion of the original image before
    this class may cause an access violation error.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiRasterImageBGRA32 : public OdGiRasterImageParam
{
protected:
  OdGiImageBGRA32 *m_pBGRAImage;
  OdGiRasterImage::TransparencyMode m_transparencyMode;
public:
  /** \details
    Default constructor for the OdGiRasterImageBGRA32 class. Sets the following data members:
    <table>
    Name                  Value               Description
    m_pBGRAImage          NULL                Pointer to an image in BGRA format.
    m_transparencyMode    kTransparency8Bit   Transparency mode.
    </table>
  */
  OdGiRasterImageBGRA32();
  
  /** \details
    Destructor for the OdGiRasterImageBGRA32 class.
  */
  virtual ~OdGiRasterImageBGRA32();
  
  /** \details
    Creates an OdGiRasterImageBGRA32 object with the specified parameters.
    \param pImage [in]  Input BGRA32 image.
    \param transparencyMode [in]  Transparency mode.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(OdGiImageBGRA32 *pImage, OdGiRasterImage::TransparencyMode transparencyMode = OdGiRasterImage::kTransparency8Bit);
  /** \details
    Retrieves the image width in pixels of this RasterImage object.
    \returns
    image width in pixels.
  */
  OdUInt32 pixelWidth() const;
  /** \details
    Retrieves the image height in pixels of this RasterImage object.
    \returns
    image height in pixels.
  */
  OdUInt32 pixelHeight() const;
  /** \details
    Retrieves the number of bits per pixel used for colors by this RasterImage object.
    \returns
    number of bits per pixel that represents color depth.
  */
  OdUInt32 colorDepth() const;
  /** \details
    Retrieves the number of colors in the palette of this RasterImage object.
    \returns
    number of bits per pixel that represents color depth.
  */
  OdUInt32 numColors() const;
  /** \details
    Retrieves the color for the specified index from the palette of this RasterImage object.
    \param colorIndex [in]  Color index.
    \returns
    color at the specified index as ODCOLORREF (OdUInt32 value).
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
    Retrieves the size (in bytes) of the palette data of this RasterImage object.
    \returns
    palette data size as OdUInt32 value.
  */
  OdUInt32 paletteDataSize() const;
  /** \details
    Retrieves the palette in BMP format of this RasterImage object.
    \param bytes [out]  Receives the palette data.
    
    \remarks
    It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const;
  /** \details
    Retrieves the scanline size of this RasterImage object.
    
    \returns
    the number of bytes between the beginning of scanline N and
    the beginning of scanline N+1 (taking into account any padding that is added to the 
    end of the scanline).
  */
  OdUInt32 scanLineSize() const;
  /** \details
    Retrieves the set of scanlines.

    \returns
    set of scanlines.
    
    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Retrieves the specified set of scanlines in BMP format for this RasterImage object,
    or the pixel data in BMP format for this RasterImage object.

    \param scnLines [out]  Receives the scanline data.
    \param firstScanline [in]  Index of first scanline to retrieve.
    \param numLines [in]  Number of scanlines to retrieve.

    \remarks
    * The number of accessible scanlines is equal to the value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \remarks
    It us up to the caller to allocate sufficient memory for the scanline data.
  */
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Retrieves the pixel format for this RasterImage object.
    
    \returns
    pixel format of a palette or a raster image.
    \remarks
    This function returns the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself.
  */
  OdGiRasterImage::PixelFormatInfo pixelFormat() const;
  /** \details
    Retrieves the scanline alignment, in bytes, for this RasterImage object.
    Example:
    scanLinesAlignment() returns 4 for Windows BMP.
    \returns
    scanline alignment as OdUInt32 value.
  */
  OdUInt32 scanLinesAlignment() const;
  /** \details
    Retrieves the default image resolution in pixels per unit of this RasterImage object. 

    \param xPelsPerUnit [out]  Receives the pixels per unit value (x direction). 
    \param yPelsPerUnit [out]  Receives the pixels per unit value (y direction).

    \returns
    units that are used to measure image dimensions.
    \remarks If the returned value is kNone, then xPelsPerUnit and yPelsPerUnit are not set.
  */
  OdGiRasterImage::Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;
  /** \details
    Retrieves the ParamType bitflags with which parameters are supported by this interface.
    \returns
    bitflags that represent supported parameters.
  */
  OdUInt32 supportedParams() const;
  /** \details
    Retrieves the image source.
    \returns
    image source represented by a value from the OdGiRasterImage::ImageSource enumeration.
  */
  OdGiRasterImage::ImageSource imageSource() const;
  /** \details
    Retrieves the transparency mode for 32bpp formats.
    \returns
    transparency mode represented by a value from the OdGiRasterImage::TransparencyMode enumeration.
  */
  OdGiRasterImage::TransparencyMode transparencyMode() const;
  /** \details
    Sets an image transparency mode.
    \param mode [in]  New transparency mode value.
  */
  void setTransparencyMode(OdGiRasterImage::TransparencyMode mode);
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
    \returns
    smart pointer to the cloned object.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    Extends the OdGiRasterImageBGRA32 class to keep a copy of OdGiImageBGRA32 inside.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiRasterImageBGRA32Holder : public OdGiRasterImageBGRA32
{
protected:
  OdGiImageBGRA32 m_ImageCopy;
public:
  /** \details
    Default constructor for the OdGiRasterImageBGRA32Holder class.
  */
  OdGiRasterImageBGRA32Holder();
  
  /** \details
    Destructor for the OdGiRasterImageBGRA32Holder class.
  */
  ~OdGiRasterImageBGRA32Holder();
  
  /** \details
    Creates an OdGiRasterImageBGRA32Holder object with specified parameters.
    \param pImage [in]  Input BGRA32 image.
    \param transparencyMode [in]  Transparency mode.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(OdGiImageBGRA32 *pImage, OdGiRasterImage::TransparencyMode transparencyMode = OdGiRasterImage::kTransparency8Bit);
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
    \returns
    smart pointer to the cloned object.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    Extends the OdGiRasterImageBGRA32 class to keep a copy of OdGiImageBGRA32 and pixels inside.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiRasterImageBGRA32Copy : public OdGiRasterImageBGRA32Holder
{
private:
  OdGiPixelBGRA32Array m_pxArray;
public:
  /** \details
    Default constructor for the OdGiRasterImageBGRA32Holder class.
  */
  OdGiRasterImageBGRA32Copy();
  
  /** \details
    Destructor for the OdGiRasterImageBGRA32Holder class.
  */
  ~OdGiRasterImageBGRA32Copy();
  
  /** \details
    Creates an OdGiRasterImageBGRA32Copy object with specified parameters.
    \param pImage [in]  Input BGRA32 image.
    \param transparencyMode [in]  Transparency mode.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(OdGiImageBGRA32 *pImage, OdGiRasterImage::TransparencyMode transparencyMode = OdGiRasterImage::kTransparency8Bit);
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    This class stores a full copy of original raster image data.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiRasterImageHolder : public OdGiRasterImageParam
{
  OdUInt32                          m_pixelWidth, m_pixelHeight, m_colorDepth, m_alignment;
  OdGiRasterImage::PixelFormatInfo  m_pixFmt;
  Units                             m_units;
  double                            m_xPelsPerUnit, m_yPelsPerUnit;
  OdGiRasterImage::ImageSource      m_imageSource;
  OdString                          m_fileName;
  OdGiRasterImage::TransparencyMode m_transparencyMode;
  int                               m_transparentColor;
  OdUInt8Array                      m_palData;
  OdUInt8Array                      m_pixData;
public:
  /** \details
    Default constructor for the OdGiRasterImageHolder class. Sets the following data members:
    <table>
    Name                  Value             Description
    m_pixelWidth          0                 Image width in pixels.
    m_pixelHeight         0                 Image height in pixels.
    m_colorDepth          0                 Number of bits that represent image color depth.
    m_alignment           4                 Scanline alignment in bytes.
    m_units               kNone             Measurement units.
    m_xPelsPerUnit        0.0               Pixel per units in the x direction.
    m_yPelsPerUnit        0.0               Pixel per units in the y direction.
    m_imageSource         kUndefinedSource  Image source type.
    m_transparencyMode    kTransparencyDef  Transparency mode.
    </table>
  */
  OdGiRasterImageHolder();
  
  /** \details
    Destructor for the OdGiRasterImageHolder class.
  */
  virtual ~OdGiRasterImageHolder();

  /** \details
    Stores data from an original OdGiRasterImage object.
    \param pOrig [in]  Pointer to the original image object.
  */
  void setFrom(const OdGiRasterImage* pOrig);

  /** \details
    Creates an OdGiRasterImageHolder object from an original OdGiRasterImage object.
    \param pImage [in]  Pointer to the original image object.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pImage);
  /** \details
    Retrieves the image width in pixels of this RasterImage object.
    \returns
    image width in pixels.
  */
  OdUInt32 pixelWidth() const;
  /** \details
    Retrieves the image height in pixels of this RasterImage object.
    \returns
    image width in pixels.
  */
  OdUInt32 pixelHeight() const;
  /** \details
    Retrieves the default image resolution in pixels per unit of this RasterImage object. 

    \param xPelsPerUnit [out]  Receives the pixels per unit value (x direction). 
    \param yPelsPerUnit [out]  Receives the pixels per unit value (y direction).

    \returns
    units that are used to measure image dimensions.
    \remarks If the returned value is kNone, then xPelsPerUnit and yPelsPerUnit are not set.
  */
  Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;
  /** \details
    Retrieves the number of bits per pixel used for colors by this RasterImage object.
    \returns
    number of bits per pixel that represents color depth.
  */
  OdUInt32 colorDepth() const;
  /** \details
    Retrieves the number of colors in the palette of this RasterImage object.
    \returns
    number of colors in the palette.
  */
  OdUInt32 numColors() const;
  /** \details
    Retrieves the color for the specified index from the palette of this RasterImage object.
    \param colorIndex [in]  Color index.
    \returns
    color at the specified index as ODCOLORREF (OdUInt32) value.
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
    Retrieves the size (in bytes) of the palette data of this RasterImage object.
    \returns
    number of bytes that represents palette data size.
  */
  OdUInt32 paletteDataSize() const;
  /** \details
    Retrieves the size (in bytes) of the palette data of this RasterImage object.
    \param bytes [out]  Receives the palette data.
    
    \remarks
    It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const;
  /** \details
    Retrieves the scanline size of this RasterImage object.
    
    \returns
    the number of bytes between the beginning of scanline N and
    the beginning of scanline N+1 (taking into account any padding that is added to the 
    end of the scanline).
  */
  OdUInt32 scanLineSize() const;
  /** \details
    Retrieves the specified set of scanlines in BMP format for this RasterImage object,
    or the pixel data in BMP format for this RasterImage object.

    \param scnLines [out]  Receives the scanline data.
    \param firstScanline [in]  Index of first scanline to retrieve.
    \param numLines [in]  Number of scanlines to retrieve.

    \remarks
    * The number of accessible scanlines is equal to the value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \remarks
    It us up to the caller to allocate sufficient memory for the scanline data.
  */
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Retrieves the set of scanlines.

    \returns
    set of scanlines.
    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Retrieves the pixel format for this RasterImage object.
    
    \returns
    the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself.
  */
  PixelFormatInfo pixelFormat() const;
  /** \details
    Retrieves the scanline alignment, in bytes, for this RasterImage object.
    Example:
    scanLinesAlignment() returns 4 for Windows BMP.
    \returns
    scanline alignment as OdUInt32 value.
  */
  OdUInt32 scanLinesAlignment() const;
  /** \details
    Retrieves the index of transparent color in the image.
    \returns
    transparent color.
    \remarks
    If there is no transparent color, -1 should be returned.
  */
  int transparentColor() const;
  /** \details
    Retrieves the image source.
    \returns
    image source represented by a value from the ImageSource enumeration.
  */
  ImageSource imageSource() const;
  /** \details
    Retrieves the image source file name.
    \returns
    full path to a file from which image is loaded.
  */
  const OdString &sourceFileName() const;
  /** \details
    Retrieves the transparency mode for 32bpp formats.
    \returns
    transparency mode represented by a value from the TransparencyMode enumeration.
  */
  TransparencyMode transparencyMode() const;
  /** \details
    Retrieves the ParamType bitflags with which parameters are supported by this interface.
    \returns
    bitflags that represent supported parameters.
  */
  OdUInt32 supportedParams() const;
  /** \details
    Sets an image source.
    \param source [in]  New image source value.
  */
  void setImageSource(ImageSource source);
  /** \details
    Sets an image source file name.
    \param fileName [in]  New value for image source file name.
  */
  void setSourceFileName(const OdString &fileName);
  /** \details
    Sets an image transparency mode.
    \param mode [in]  New transparency mode value.
  */
  void setTransparencyMode(TransparencyMode mode);
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
    \returns
    smart pointer to the cloned object.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    This class is the base class for raster image transformer objects.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.
    The default methods for this class do nothing but return the
    corresponding values from the original object.
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiRasterImageWrapper : public OdGiRasterImageParam
{
  OdGiRasterImagePtr m_pOrig;
public:
  /** \details
    Default constructor for the OdGiRasterImageWrapper class.
  */
  OdGiRasterImageWrapper();
  /** \details
    Destructor for the OdGiRasterImageWrapper class.
  */
  virtual ~OdGiRasterImageWrapper();

  /** \details
    Sets the original OdGiRasterImage object associated with this RasterImageWrapper object.
    \param pOrig [in]  Pointer to the original image object.
  */
  void setOriginal(const OdGiRasterImage* pOrig);
  /** \details
    Retrieves the original OdGiRasterImage object associated with this RasterImageWrapper object.
    \returns
    pointer to the original underlaying raster image.
  */
  const OdGiRasterImage *original() const;
  /** \details
    Copies the original OdGiRasterImage object associated with this RasterImageWrapper object.
    \returns
    smart pointer to the cloned original underlaying raster image.
  */
  OdGiRasterImagePtr cloneOriginal() const;
  /** \details
    Retrieves the image width in pixels of this RasterImage object.
    \returns
    image width in pixels.
  */
  OdUInt32 pixelWidth() const;
  /** \details
    Retrieves the image height in pixels of this RasterImage object.
    \returns
    image height in pixels.
  */
  OdUInt32 pixelHeight() const;
  /** \details
    Retrieves the default image resolution in pixels per unit of this RasterImage object. 

    \param xPelsPerUnit [out]  Receives the pixels per unit value (x direction). 
    \param yPelsPerUnit [out]  Receives the pixels per unit value (y direction).
    \returns
    units that are used to measure image dimensions.
    \remarks If the returned value is kNone, then xPelsPerUnit and yPelsPerUnit are not set.
  */
  Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;
  /** \details
    Retrieves the number of bits per pixel used for colors by this RasterImage object.
    \returns
    number of bits per pixel that represents color depth.
  */
  OdUInt32 colorDepth() const;
  /** \details
    Retrieves the number of colors in the palette of this RasterImage object.
    \returns
    number of colors in the palette.
  */
  OdUInt32 numColors() const;
  /** \details
    Retrieves the color for the specified index from the palette of this RasterImage object.
    \param colorIndex [in]  Color index.
    \returns
    color at the specified index as ODCOLORREF (OdUInt32) value.
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
    Retrieves the size (in bytes) of the palette data of this RasterImage object.
    \returns
    number of bytes that represents palette data size.
  */
  OdUInt32 paletteDataSize() const;
  /** \details
    Retrieves the size (in bytes) of the palette data of this RasterImage object.
    \param bytes [out]  Receives the palette data.
    
    \remarks
    It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const;
  /** \details
    Retrieves the scanline size of this RasterImage object.
    
    \remarks
    This function returns the number of bytes between the beginning of scanline N and
    the beginning of scanline N+1 (taking into account any padding that is added to the 
    end of the scanline).
  */
  OdUInt32 scanLineSize() const;
  /** \details
    Retrieves the specified set of scanlines in BMP format from this RasterImage object,
    or the pixel data in BMP format for this RasterImage object.

    \param scnLines [out]  Receives the scanline data.
    \param firstScanline [in]  Index of first scanline to retrieve.
    \param numLines [in]  Number of scanlines to retrieve.

    \remarks
    * The number of accessible scanlines is equal to the value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \remarks
    It us up to the caller to allocate sufficient memory for the scanline data.
  */
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Retrieves the set of scanlines.

    \returns
    set of scanlines.
    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Retrieves the pixel format for this RasterImage object.

    \returns
    the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself.
  */
  PixelFormatInfo pixelFormat() const;
  /** \details
    Retrieves the scanline alignment, in bytes, for this RasterImage object.
    Example:
    scanLinesAlignment() returns 4 for Windows BMP.
    \returns
    scanline alignment as OdUInt32 value.
  */
  OdUInt32 scanLinesAlignment() const;
  /** \details
    Retrieves the index of transparent color in the image.

    \returns
    transparent color.
    \remarks If there is no transparent color, -1 should be returned.
  */
  int transparentColor() const;
  /** \details
    Retrieves the image source.
    \returns
    image source represented by a value from the ImageSource enumeration.
  */
  ImageSource imageSource() const;
  /** \details
    Retrieves the image source file name.
    \returns
    full path to a file from which image is loaded.
  */
  const OdString &sourceFileName() const;
  /** \details
    Retrieves the transparency mode for 32bpp formats.
    \returns
    transparency mode represented by a value from the TransparencyMode enumeration.
  */
  TransparencyMode transparencyMode() const;
  /** \details
    Retrieves the ParamType bitflags with which parameters are supported by this interface.
    \returns
    bitflags that represent supported parameters.
  */
  OdUInt32 supportedParams() const;
  /** \details
    Sets an image source.
    \param source [in]  New image source value.
  */
  void setImageSource(ImageSource source);
  /** \details
    Sets an image source file name.
    \param fileName [in]  New value for image source file name.
  */
  void setSourceFileName(const OdString &fileName);
  /** \details
    Sets an image transparency mode.
    \param mode [in]  New transparency mode value.
  */
  void setTransparencyMode(TransparencyMode mode);
  /** \details
    Retrieves the user-end implementation of raster image functionality.
    \remarks
    If not overridden by a user-end raster image implementation, always returns null.
  */
  void* imp() const;
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
    \returns
    smart pointer to the cloned object.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    This raster image transformer class transforms OdGiRasterImage objects to Bitonal images.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiBitonalRasterTransformer : public OdGiRasterImageWrapper
{
  RGBQUAD m_palette[2];
  int m_transpColor;
public:
  /** \details
    Default constructor for the OdGiBitonalRasterTransformer class. Sets transparency color to -1 (absent).
  */
  OdGiBitonalRasterTransformer();
  /** \details
    Destructor for the OdGiBitonalRasterTransformer class.
  */
  virtual ~OdGiBitonalRasterTransformer();

  /** \details
      Creates a bitonal raster transformer.
      \param pOrig [in]  Original raster image pointer.
      \param foregroundColor [in]  Foreground color.
      \param backgroundColor [in]  Background color.
      \param transparent [in]  Image is transparent if and only if true.
      \returns
      smart pointer to the created object.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig, ODCOLORREF foregroundColor, ODCOLORREF backgroundColor, bool transparent);

  /** \details
      Sets bitonal raster transformer parameters.
      \param pOrig [in]  Original raster image pointer.
      \param foregroundColor [in]  Foreground color.
      \param backgroundColor [in]  Background color.
      \param transparent [in]  Image is transparent if and only if true.
  */
  void setOriginal(const OdGiRasterImage* pOrig, ODCOLORREF foregroundColor, ODCOLORREF backgroundColor, bool transparent);

  /** \details
      Retrieves the number of colors in the palette of this RasterImage object.
      \returns
      number of colors in the palette.
  */
  OdUInt32 numColors() const;
  /** \details
      Retrieves the number of colors in the palette of this RasterImage object.

      \param colorIndex [in]  Color index.
      \returns
      color at the specified index as ODCOLORREF (OdUInt32) value.
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
      Retrieves the size (in bytes) of the palette data of this RasterImage object.
      \returns
      number of bytes that represents palette data size.
  */
  OdUInt32 paletteDataSize() const;
  /** \details
      Retrieves the palette in BMP format of this RasterImage object.

      \param bytes [out] Output palette data.

      \remarks It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const;
  /** \details
      Retrieves the pixel format for this RasterImage object.

      \returns
      the pixel format of the palette,
      unless there is no palette, in which case it returns
      the pixel format of the image itself.
  */
  PixelFormatInfo pixelFormat() const;
  /** \details
      Retrieves the index of transparent color in the image.
      \returns
      -1 because there is no transparency color for a bitonal image.
  */
  int transparentColor() const;
  /** \details
      Creates a *clone* of this object, and returns a pointer to the *clone*.
      \returns
      smart pointer to the cloned object.
  */
  OdRxObjectPtr clone() const;
  /** \details
      Copies data of a source object to the current object.

      \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    This raster image transformer class mirrors an image upside down.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiUpsideDownRasterTransformer : public OdGiRasterImageWrapper
{
public:
  /** \details
    Default constructor for the OdGiRasterImageBGRA32Holder class.
  */
  OdGiUpsideDownRasterTransformer();
  
  /** \details
    Destructor for the OdGiRasterImageBGRA32Holder class.
  */
  virtual ~OdGiUpsideDownRasterTransformer();

  /** \details
    Creates a new upside down image transformer.
    \param pOrig [in]  Original raster image pointer.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig);
  /** \details
    Retrieves the specified set of scanlines in BMP format for this RasterImage object,
    or the pixel data in BMP format for this RasterImage object.

    \param scnLines [out]  Receives the scanline data.
    \param firstScanline [in]  Index of first scanline to retrieve.
    \param numLines [in]  Number of scanlines to retrieve.

    \remarks
    * The number of accessible scanlines is equal to the value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \remarks
    It us up to the caller to allocate sufficient memory for the scanline data.
  */
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Retrieves the set of scanlines.
    
    \returns
    set of scanlines.
    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
    \returns
    smart pointer to the cloned object.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    This raster image transformer class mirrors an image right to left.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiLeftToRightRasterTransformer : public OdGiRasterImageWrapper
{
public:
  /** \details
    Default constructor for the OdGiLeftToRightRasterTransformer class.
  */
  OdGiLeftToRightRasterTransformer();
  
  /** \details
    Destructor for the OdGiLeftToRightRasterTransformer class.
  */
  virtual ~OdGiLeftToRightRasterTransformer();

  /** \details
    Creates a new left to right image transformer.
    \param pOrig [in]  Original raster image pointer.
    \returns
    smart pointer to the created object.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig);
  /** \details
    Retrieves the specified set of scanlines in BMP format for this RasterImage object,
    or the pixel data in BMP format for this RasterImage object.

    \param scnLines [out]  Receives the scanline data.
    \param firstScanline [in]  Index of first scanline to retrieve.
    \param numLines [in]  Number of scanlines to retrieve.

    \remarks
    * The number of accessible scanlines is equal to the value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \remarks
    It us up to the caller to allocate sufficient memory for the scanline data.
  */
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Retrieves the set of scanlines.
    \returns
    set of scanlines.
    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    the scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
    \returns
    smart pointer to the cloned object.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    This raster image transformer class negates image colors.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiInversionRasterTransformer : public OdGiRasterImageWrapper
{
public:
  /** \details
    Default constructor for the OdGiInversionRasterTransformer class.
  */
  OdGiInversionRasterTransformer();
  /** \details
    Destructor for the OdGiInversionRasterTransformer class.
  */
  virtual ~OdGiInversionRasterTransformer();

  /** \details
    Creates a new inversion image transformer.
    \param pOrig [in]  Original raster image pointer.
    \returns
    smart pointer to the created object.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig);
  /** \details
    Retrieves the number of colors in the palette of this RasterImage object.

    \param colorIndex [in]  Color index.
    \returns
    color at the specified index as ODCOLORREF (OdUInt32) value.
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
    Retrieves the palette in BMP format of this RasterImage object.

    \param bytes [out] Output palette data.

    \remarks It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const;
  
  /** \details
    Retrieves the specified set of scanlines in BMP format for this RasterImage object,
    or the pixel data in BMP format for this RasterImage object.

    \param scnLines [out]  Receives the scanline data.
    \param firstScanline [in]  Index of first scanline to retrieve.
    \param numLines [in]  Number of scanlines to retrieve.

    \remarks
    * The number of accessible scanlines is equal to the value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \remarks
    It us up to the caller to allocate sufficient memory for the scanline data.
  */
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Retrieves the set of scanlines.
    \returns
    set of scanlines if original image has color depth less or equal than 8, in other cases returns NULL.
    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
    \returns
    smart pointer to the cloned object.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
protected:
  virtual ODCOLORREF colorXform(ODCOLORREF color) const;
};

/** \details
    This raster image transformer class converts image colors into grayscale.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiGrayscaleRasterTransformer : public OdGiInversionRasterTransformer
{
public:
  /** \details
    Default constructor for the OdGiGrayscaleRasterTransformer class.
  */
  OdGiGrayscaleRasterTransformer();
  
  /** \details
    Destructor for the OdGiGrayscaleRasterTransformer class.
  */
  virtual ~OdGiGrayscaleRasterTransformer();

  /** \details
      Creates a new grayscale image transformer.
      \param pOrig [in]  Original raster image pointer.
      \returns
      smart pointer to the created object.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig);
  /** \details
      Creates a *clone* of this object, and returns a pointer to the *clone*.
      \returns
      smart pointer to the cloned object.
  */
  OdRxObjectPtr clone() const;
  /** \details
      Copies data of a source object to the current object.

      \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
protected:
  virtual ODCOLORREF colorXform(ODCOLORREF color) const;
};

/** \details
This raster image transformer class apply tint color to image.

Corresponding C++ library: TD_Gi

\remarks
Transforming an image does not affect the orignal.
<group OdGi_Classes>
\sa
<link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiTintRasterTransformer : public OdGiInversionRasterTransformer
{
public:
	/** \details
    Default constructor for the OdGiTintRasterTransformer class. Sets red, green and blue scales to 1.0.
  */
  OdGiTintRasterTransformer();
	
  /** \details
    Destructor for the OdGiTintRasterTransformer class.
  */
  virtual ~OdGiTintRasterTransformer();

	/** \details
	Creates a new tint image transformer.
	\param pOrig [in]  Original raster image pointer.
  \param clrTint [in]  Tint color value that is used to set corresponding scales for separate color channels.
	*/
	static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig, const ODCOLORREF& clrTint );
	/** \details
	Creates a *clone* of this object, and returns a pointer to the *clone*.
  \returns
  smart pointer to the cloned object.
	*/
	OdRxObjectPtr clone() const;
	/** \details
	Copies data of a source object to the current object.

	\param pSource [in] Input source object.
	*/
	void copyFrom(const OdRxObject *pSource);
protected:
	virtual ODCOLORREF colorXform(ODCOLORREF color) const;
protected:
	double m_dRedScale;
	double m_dGreenScale;
	double m_dBlueScale;
};

/** \details
    This raster image transformer class converts image colors into monochrome.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiMonochromaticRasterTransformer : public OdGiGrayscaleRasterTransformer
{
  int m_threshold;
public:
  /** \details
    Default constructor for the OdGiMonochromaticRasterTransformer class.
  */
  OdGiMonochromaticRasterTransformer();
  /** \details
    Destructor for the OdGiMonochromaticRasterTransformer class.
  */
  virtual ~OdGiMonochromaticRasterTransformer();

  /** \details
      Creates a new monochromatic image transformer.
      \param pOrig [in]  Original raster image pointer.
      \param threshold [in]  Threshold between light and dark image components.
      \returns
      smart pointer to the created object.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig, int threshold = 127);
  /** \details
      Creates a *clone* of this object, and returns a pointer to the *clone*.
      \returns
      smart pointer to the cloned object.
  */
  OdRxObjectPtr clone() const;
  /** \details
      Copies data of a source object to the current object.

      \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
  /** \details
      Sets a threshold value to this object.

      \param treshold [in] Input threshold value.
  */
  void setThreshold(OdUInt8 treshold) { m_threshold = treshold; if (m_threshold > 254) m_threshold = 254; };
  /** \details
      Retrieves the threshold value of this object.
      \returns
      threshold below which values represent dark image components, and above which - light image components.
  */
  int threshold() const { return m_threshold; }
protected:
  virtual ODCOLORREF colorXform(ODCOLORREF color) const;
};

/** \details
    Extends the original OdGiRasterImage with alpha channel data.
    \remarks
    Stores only a pointer to the original image, not a SmartPointer, so deletion of the original image before
    this class may cause an access violation error.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiRasterImageAlphaChannelAdder : public OdGiRasterImageParam
{
protected:
  const OdGiRasterImage*  m_pOrigImage;
  ODCOLORREF              m_clPixelAlpha;
  OdUInt8                 m_nPixelAlphaThreshold;
public:
  
  /** \details
    Default constructor for the OdGiRasterImageAlphaChannelAdder class. Sets the following data members:
    <table>
    Name                      Value               Description
    m_pOrigImage              NULL                Pointer to an original image.
    m_clPixelAlpha            ODRGB( 0, 0, 0 )    Alpha pixel color.
    m_nPixelAlphaThreshold    0                   Alpha pixel color threshold.
    </table>
  */
  OdGiRasterImageAlphaChannelAdder();
  
  /** \details
    Destructor for the OdGiRasterImageAlphaChannelAdder class.
  */
  virtual ~OdGiRasterImageAlphaChannelAdder();
  
  /** \details
    Creates an OdGiRasterImageAlphaChannelAdder object with the specified parameters.
    \param pOrig [in]  Input original image.
    \param cutColor [in]  Alpha pixel color.
    \param threshold [in] Alpha pixel color threshold.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject( const OdGiRasterImage* pOrig, ODCOLORREF cutColor, OdUInt8 threshold = 0 );

  /** \details
    Retrieves the scanline size of this RasterImage object.
    
    \remarks
    This function returns the number of bytes between the beginning of scanline N and
    the beginning of scanline N+1 (taking into account any padding that is added to the 
    end of the scanline).
  */
  OdUInt32 scanLineSize() const;
  /** \details
    Retrieves the set of scanlines.
    \returns
    NULL.
    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Retrieves the specified set of scanlines in BMP format for this RasterImage object,
    or the pixel data in BMP format for this RasterImage object.

    \param scnLines [out]  Receives the scanline data.
    \param firstScanline [in]  Index of first scanline to retrieve.
    \param numLines [in]  Number of scanlines to retrieve.

    \remarks
    * The number of accessible scanlines is equal to the value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \remarks
    It us up to the caller to allocate sufficient memory for the scanline data.
  */
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;

  /** \details
    Retrieves the image width in pixels of this RasterImage object.
    \returns
    image width in pixels.
  */
  OdUInt32 pixelWidth() const;
  /** \details
    Retrieves the image height in pixels of this RasterImage object.
    \returns
    image height in pixels.
  */
  OdUInt32 pixelHeight() const;
  /** \details
    Retrieves the number of bits per pixel used for colors by this RasterImage object.
    \returns
    number of bits per pixel that represents color depth.
  */
  OdUInt32 colorDepth() const;
  /** \details
    Retrieves the number of colors in the palette of this RasterImage object.
    \returns
    number of colors in the palette.
  */
  OdUInt32 numColors() const;
  /** \details
    Retrieves the color for the specified index from the palette of this RasterImage object.
    \param colorIndex [in]  Color index.
    \returns
    color at the specified index as ODCOLORREF (OdUInt32) value.
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
    Retrieves the size (in bytes) of the palette data of this RasterImage object.
    \returns
    number of bytes that represents palette data size.
  */
  OdUInt32 paletteDataSize() const;
  /** \details
    Retrieves the palette in BMP format of this RasterImage object.
    \param bytes [out]  Receives the palette data.
    
    \remarks
    It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const;
  /** \details
    Retrieves the pixel format for this RasterImage object.
    
    \returns
    the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself. Returns BGRA pixel format.
  */
  OdGiRasterImage::PixelFormatInfo pixelFormat() const;
  /** \details
    Retrieves the scanline alignment, in bytes, for this RasterImage object.
    Example:
    scanLinesAlignment() returns 4 for Windows BMP.
    \returns
    scan lines alignment which is 4 for this object.
  */
  OdUInt32 scanLinesAlignment() const;
  /** \details
    Retrieves the default image resolution in pixels per unit of this RasterImage object. 

    \param xPelsPerUnit [out]  Receives the pixels per unit value (x direction). 
    \param yPelsPerUnit [out]  Receives the pixels per unit value (y direction).
    \returns
    units that are used to measure image dimensions.
    \remarks If the returned value is kNone, then xPelsPerUnit and yPelsPerUnit are not set.
  */
  OdGiRasterImage::Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;
  /** \details
    Retrieves the ParamType bitflags with which parameters are supported by this interface.
    \returns
    bitflags that represent supported parameters.
  */
  OdUInt32 supportedParams() const;
  /** \details
    Retrieves the image source.
    \returns
    image source represented by a value from the OdGiRasterImage::ImageSource enumeration.
  */
  OdGiRasterImage::ImageSource imageSource() const;
  /** \details
    Retrieves the transparency mode for 32bpp formats.
    \returns
    transparency mode represented by a value from the OdGiRasterImage::TransparencyMode enumeration.
  */
  OdGiRasterImage::TransparencyMode transparencyMode() const;
  /** \details
    Sets an image transparency mode.
    \param mode [in]  New transparency mode value.
  */
  void setTransparencyMode(OdGiRasterImage::TransparencyMode mode);
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
    \returns
    smart pointer to the cloned object.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);

protected:
  OdUInt8 getAlphaValue( OdUInt8 red, OdUInt8 green, OdUInt8 blue ) const;
};
//DOM-IGNORE-BEGIN
#if 0 // @@@TODO: implement at least nearest, bilinear and bicubic resamplers

/** \details
    This is a raster image resampler class with the simplest, nearest interpolation filter.

    Corresponding C++ library: TD_Gi

    \remarks
    Resampling an image does not affect the orignal.
    <group OdGi_Classes>
    \sa
    <link gi_raster_images, Work with Raster Images>
*/
class ODGI_EXPORT OdGiNearestRasterResampler : public OdGiRasterImageWrapper
{
  OdUInt32 m_newPixWidth, m_newPixHeight;
public:
  OdGiNearestRasterResampler();
  virtual ~OdGiNearestRasterResampler();

  /** \details
    Create new image nearest interpolation resampler.
    \param pOrig [in]  Original raster image pointer.
    \param newPixWidth [in]  New image width.
    \param newPixHeight [in]  New image width.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig, OdUInt32 newPixWidth, OdUInt32 newPixHeight);
  /** \details
    Retrieves the specified set of scanlines in BMP format for this RasterImage object,
    or the pixel data in BMP format for this RasterImage object.

    \param scnLines [out]  Receives the scanline data.
    \param firstScanline [in]  Index of first scanline to retrieve.
    \param numLines [in]  Number of scanlines to retrieve.

    \remarks
    * The number of accessible scanlines is equal to the value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \remarks
    It us up to the caller to allocate sufficient memory for the scanline data.
  */
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  
  /** \details
    Retrieves the set of scanlines.

    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};
//DOM-IGNORE-END
#endif

#include "TD_PackPop.h"

#endif //  __OD_GI_RASTER_WRAPPERS__
