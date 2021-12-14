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
// GLES2 device textures

#ifndef ODTRVISTEXTURE
#define ODTRVISTEXTURE

#include "TD_PackPush.h"

#include "TrVisDefs.h"

/** \details
  This interface is a common texture interface
*/

class OdTrVisTexture : public OdRxObject
{
protected:
  OdUInt32 m_nWidth, m_nHeight;
  OdUInt32 m_nAlignment, m_nScanLength;
  void *m_pTextureData;

  OdTrVisTexture();

public:
  /** \details
    Supported image formats
  */
  enum Format
  { // Basic 8 bit formats
    kRed,
    kAlpha,
    kRG,
    kRGB,
    kRGBA,
    kBGR,
    kBGRA,
    kPalette,

    //Depth/Stencil
    kStencil8,
    kDepth16,
    kDepth24Stencil8,//Most used depth format. getColor() returns only depth value. For stencil data use textureData() function
    kDepth32,
    
    //16 bit
    kR16,
    kRG16,
    kRGBA16,
    kAlpha16,

    //Float
    kFloatR,
    kFloatRG,
    kFloatRGBA,
    kHalfFloatR,
    kHalfFloatRG,
    kHalfFloatRGBA,

    //Signed normalized
    kRSNorm,
    kAlphaSNorm,
    kRGSNorm,
    kRGBASNorm,
    kR16SNorm,
    kAlpha16SNorm,
    kRG16SNorm,
    kRGBA16SNorm,

    kFormatCount
  };
  /** \details
    This union represents pixel color.

    \remarks
    If image is integer-based (or OdUInt8-based) integer part should be used, otherwise float part
  */
  union TextureColor
  {
    OdUInt32 intRGBA[ 4 ];
    float floatRGBA[ 4 ];
  };
  /** \details
  This structure is required for correct TextureColor transformation between different formats
  */
  struct TextureColorContext
  {
    enum Difference
    {
      kNone = 0,
      kDifferentDataTypes = 1,
      kDifferentScales = 2
    };
    OdUInt8 difference;

    void prepareContext( const OdTrVisTexture* pSrc, const OdTrVisTexture* pDst );
    
    bool isSrcInt;
    bool isDstInt;
    TextureColor srcMax;
    TextureColor dstMax;
    double scalesMult[ 4 ];
  };

  virtual ~OdTrVisTexture();

  // non-virtual part of OdTrVisTexture inline interface.
 
  // Typical data accessors
  OdUInt32 getTextureWidth() const { return m_nWidth; }
  OdUInt32 getTextureHeight() const { return m_nHeight; }
  OdUInt32 getScanlineLength() const { return m_nScanLength; }
  OdUInt32 getDataAlignment() const { return m_nAlignment; }

  const OdUInt8 *textureData() const { return (const OdUInt8*)m_pTextureData; }
  OdUInt8 *textureData() { return (OdUInt8*)m_pTextureData; }

  // Part of virtual OdTrVisTexture interface
 
  // Some types of texture formats can have limitation (for example S3TC - DXT formats require consists from 4x4 blocks, so texture size must be divideable by 4).
  virtual OdUInt32 subDivLimit() const = 0;

  // Typically textures store data as integers, but may as floats. So we require ability to access texture pixels similarly for all texture sub-types.
  virtual bool isFPImage() const = 0;

  virtual void getColor(OdUInt32 nX, OdUInt32 nY, TextureColor &color) const = 0;
  virtual void setColor( OdUInt32 nX, OdUInt32 nY, const TextureColor &color ) = 0;
 
  // This is also can be helpful for algorithms optimization to have accessors for entire scanlines
  virtual void getScanline(OdUInt32 startLine, OdUInt32 linesCount, TextureColor *pScanline) const
  {
    for( OdUInt32 i = 0; i < linesCount; ++i )
    {
      OdUInt32 offset = getTextureWidth() * i;
      for( OdUInt32 j = 0; j < getTextureWidth(); ++j )
      {
        getColor( j, startLine + i, pScanline[ j + offset ] );
      }
    }
  }

  virtual void setScanline(OdUInt32 startLine, OdUInt32 linesCount, const TextureColor *pScanline)
  {
    for( OdUInt32 i = 0; i < linesCount; ++i )
    {
      OdUInt32 offset = getTextureWidth() * i;
      for( OdUInt32 j = 0; j < getTextureWidth(); ++j )
      {
        setColor( j, startLine + i, pScanline[ j + offset ] );
      }
    }
  }

  // Probably we will require color setters or etc.
 
  // Textures in Lut format contain color indexes. Color palette can be stored simply as another 1D texture.
  virtual void setPalette(const OdTrVisTexture *pTexture) = 0;
  virtual OdTrVisTexture *palette() = 0;
  virtual const OdTrVisTexture *palette() const = 0;
 
  // We can't rescale indexed textures with bilinear interpolation
  virtual bool isSmoothResampleSupported() const = 0;

  virtual Format format() const = 0;
  /** \details
  Since different format may have different color value intervals (0-255, -1.f - 1.f) we need this function to obtain maxs
  */
  virtual TextureColor getMaxValues() const = 0;
  /** \details
  Since different format may have different color value intervals (0-255, -1.f - 1.f) we need this function to obtain mins
  */
  virtual TextureColor getMinValues() const;
  /** \details
  This function converts TextureColors from one format to another (OdUInt8-based to float-based, etc) using TextureColorContext
  */
  static void prepareTextureColors( TextureColor* colors, OdUInt32 size, const TextureColorContext& context );
  /** \details
  Sometimes image have to be preprocessed befor we can call setColor or setScanline (Palette or compressed images, for example)
  */
  virtual void beginEditing() = 0;
  virtual void endEditing() = 0;
};

typedef OdSmartPtr< OdTrVisTexture > OdTrVisTexturePtr;

/** \details
  This class manage Texture operations
*/
class OdTrVisTextureManager
{
public:
  enum Resampler
  {
    kNearest,
    kBilinear
  };
public:
  /** \details
  Creates empty texture
  */
  static OdTrVisTexturePtr createTexture( OdTrVisTexture::Format fmt, OdUInt32 nWidth, OdUInt32 nHeight, OdUInt32 nAlign = 4 );
  /** \details
  Converts existing texture to the another format (or data alignment)
  */
  static OdTrVisTexturePtr convert( OdTrVisTexture::Format fmt, const OdTrVisTexture* pTexture, OdUInt32 nNewAlign = 0, const OdTrVisTexture* pPalette = NULL );
  /** \details
  Converts existing texture to the another texture format
  */
  static OdTrVisTexturePtr convert( OdTrVisTexture *pTextureTo, const OdTrVisTexture* pTextureFrom, Resampler method = kNearest );
  /** \details
  Negates existing texture
  */
  static void negateTexture(OdTrVisTexture* pTexture);
  /** \details
  Creates new texture with negated existing texture data
  */
  static OdTrVisTexturePtr negate(const OdTrVisTexture* pTexture);
  /** \details
  Compute grayscale for existing texture
  */
  static void grayscaleTexture(OdTrVisTexture* pTexture, OdUInt32 nApplyComponent = 7);
  /** \details
  Creates new texture with grayscale data from existing texture data
  */
  static OdTrVisTexturePtr grayscale(const OdTrVisTexture* pTexture, OdUInt32 nApplyComponent = 7);
  /** \details
  Apply gaussian blur to existing texture data
  */
  static void blurTexture(OdTrVisTexture* pTexture, OdUInt32 nSizeKernel = 3, float fKernelMultiplier = 1.0f);
  /** \details
  Creates new blurred texture
  */
  static OdTrVisTexturePtr blur(const OdTrVisTexture* pTexture, OdUInt32 nSizeKernel = 3, float fKernelMultiplier = 1.0f);
  /** \details
  Generate spherical map into existing texture (input texture formats must be same)
  */
  static void sphereMapTexture(OdTrVisTexture *pTexture, const OdTrVisTexture *pRightTexture, const OdTrVisTexture *pLeftTexture,
                               const OdTrVisTexture *pTopTexture, const OdTrVisTexture *pBottomTexture, const OdTrVisTexture *pFrontTexture,
                               const OdTrVisTexture *pBackTexture, int antiAliasLevel = 1, bool bSineCorrection = false);
  /** \details
  Creates new texture containing generated spherical map (input texture formats must be same)
  */
  static OdTrVisTexturePtr sphereMap(const OdTrVisTexture *pRightTexture, const OdTrVisTexture *pLeftTexture, const OdTrVisTexture *pTopTexture,
                                     const OdTrVisTexture *pBottomTexture, const OdTrVisTexture *pFrontTexture, const OdTrVisTexture *pBackTexture,
                                     int antiAliasLevel = 1, bool bSineCorrection = false);
  /** \details
    Structure containing information about color channels mapping.
  */
  struct ChannelMapping
  { float m_coef[4];
    ChannelMapping &set(float redMap, float greenMap, float blueMap, float alphaMap)
    { m_coef[0] = redMap; m_coef[1] = greenMap; m_coef[2] = blueMap; m_coef[3] = alphaMap; return *this; }
    explicit ChannelMapping(float redMap = 0.0f, float greenMap = 0.0f, float blueMap = 0.0, float alphaMap = 0.0)
    { set(redMap, greenMap, blueMap, alphaMap); } };
  /** \details
  Modifies color channels mapping in exist texture
  */
  static void remapTextureChannels(OdTrVisTexture* pTexture, const ChannelMapping &redMap = ChannelMapping(1.0f),
                                   const ChannelMapping &greenMap = ChannelMapping(0.0f, 1.0f), const ChannelMapping &blueMap = ChannelMapping(0.0f, 0.0f, 1.0f),
                                   const ChannelMapping &alphaMap = ChannelMapping(0.0f, 0.0f, 0.0f, 1.0f));
  /** \details
  Creates new texture with remapped color channels data from existing texture data
  */
  static OdTrVisTexturePtr mapChannels(const OdTrVisTexture* pTexture, const ChannelMapping &redMap = ChannelMapping(1.0f),
                                       const ChannelMapping &greenMap = ChannelMapping(0.0f, 1.0f), const ChannelMapping &blueMap = ChannelMapping(0.0f, 0.0f, 1.0f),
                                       const ChannelMapping &alphaMap = ChannelMapping(0.0f, 0.0f, 0.0f, 1.0f));
  /** \details
  Creates clone of specified texture
  */
  static OdTrVisTexturePtr clone(const OdTrVisTexture* pTexture);
  /** \details
  Resamples existing texture
  */
  static OdTrVisTexturePtr resample( Resampler method, OdUInt32 newWidth, OdUInt32 newHeight, const OdTrVisTexture* pTexture );
  /** \details
  Converts and resamples existing texture
  */
  static OdTrVisTexturePtr convertAndResample( OdTrVisTexture::Format fmt, Resampler method, OdUInt32 newWidth, OdUInt32 newHeight, const OdTrVisTexture* pTexture, OdUInt32 nNewAlign = 0, const OdTrVisTexture* pPalette = NULL );
  /** \details
  Resample texture so its width and height becomes the power of 2 (if need). Width and height are limited by nMaxSize
  */
  static OdTrVisTexturePtr resamplePow2(Resampler method, bool bRequirePow2, const OdTrVisTexture *pTexture, OdUInt32 nMaxSize);
  /** \details
  Creates textures from buffer
  */
  static OdTrVisTexturePtr createTextureFrom( OdTrVisTexture::Format fmt, OdUInt32 nWidth, OdUInt32 nHeight, OdUInt32 nAlign,
    OdTrVisTexture::Format bufferFmt, OdUInt32 bufferWidth, OdUInt32 bufferHeight, OdUInt32 bufferAlign, const void* buffer, Resampler method, const OdTrVisTexture* pPalette = NULL );

  /** \details
  Creates pow2 textures from buffer
  */
  static OdTrVisTexturePtr createPow2TextureFrom( OdTrVisTexture::Format fmt, bool bRequirePow2, OdUInt32 nMaxSize, OdUInt32 nAlign,
    OdTrVisTexture::Format bufferFmt, OdUInt32 bufferWidth, OdUInt32 bufferHeight, OdUInt32 bufferAlign, const void* buffer, Resampler method, const OdTrVisTexture* pPalette = NULL );

  /** \details
  Creates texture from buffer with the same sizes, format and data alignment
  */
  static OdTrVisTexturePtr createTextureFrom( OdTrVisTexture::Format fmt, OdUInt32 nWidth, OdUInt32 nHeight, OdUInt32 nAlign, const void* buffer, const OdTrVisTexture* pPalette = NULL,
                                              bool bOwnExistBuffer = false );
  /** \details
  Compute resampled texture dimensions.
  */
  static bool pow2ResampleDimensions( OdUInt32 &nResultWidth, OdUInt32 &nResultHeight, OdUInt32 nInWidth, OdUInt32 nInHeight, bool bRequirePow2, OdUInt32 nMaxSize );
  /** \details
  Saves texture in file
  */
  static bool save(const OdTrVisTexture *pTexture, const OdString &fileName);

};


#include "TD_PackPop.h"

#endif
