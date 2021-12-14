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

// Procedural textures generator.

#ifndef __ODGIPROCEDURALGENERATOR_H__
#define __ODGIPROCEDURALGENERATOR_H__

#include "RootExport.h"
#include "GiExport.h"
#include "UInt32Array.h"
#include "GiImage.h"
#include "GiMaterial.h"

#include "RxObject.h"

#include "TD_PackPush.h"

/** \details
    Protocol extension for OdGiProceduralGenerator.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiProceduralGeneratorPE : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiProceduralGeneratorPE);

    OdGiProceduralGeneratorPE();

    /** \details
      Generates procedural texture.
      \param pTexture [in]  Procedural texture pointer.
      \param image [out]  Output image.
      \param renderCoef [in]  Texture detail multiplier. 1.0 - is most standard device value, 10.0 - for normal rendering.
                     This value can be calculated from raster resolution.
      \returns
      false if protocol extension not initialized or does not support current procedural texture type.
    */
    virtual bool generateProceduralTexture(OdGiProceduralTexturePtr pTexture,
                                           OdGiImageBGRA32 &image, double renderCoef = 1.0) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiProceduralGeneratorPE object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiProceduralGeneratorPE> OdGiProceduralGeneratorPEPtr;

/** \details
    Color gradient generator.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiGradientGenerator
{
  public:
    /** \details
        Type of color interpolation function (linear, exponential, inverted exponential, cosine).
    */
    enum InterpolationType
    {
      /**Linear interpolation. */
      kLinearInterpolation = 0,
      /**Exponential interpolation. */
      kExpInterpolation,
      /**Inverted exponential interpolation. */
      kInvExpInterpolation,
      /**Cosine interpolation. */
      kCosInterpolation
    };
    /** \details
        Color interval addressing mode.
    */
    enum AddressMode
    {
      /**kClampMode clamp color address coordinates to the [0.0, 1.0] range. */
      kClampMode = 0,
      /**kWrapMode Repeat color address on every integer junction. */
      kWrapMode
    };
  protected:
    OdUInt32Array m_colorGradient;
    AddressMode m_addressMode;
  public:
    /** \details
      Default constructor for OdGiGradientGenerator class.
    */
    OdGiGradientGenerator();
    
    /** \details
      Copy constructor for OdGiGradientGenerator class.
      \param other [in]  Other gradient generator to copy.
    */
    OdGiGradientGenerator(const OdGiGradientGenerator &other);
    ~OdGiGradientGenerator();

    /** \details
      Creates gradient colors array, filled with black color.
      \param nColors [in]  Count of colors in gradient array.
    */
    void createColorArray(OdUInt32 nColors);
    /** \details
      Creates gradient colors array, filled with the specified color.
      \param color [in]  Color for all elements in array.
      \param nColors [in]  Count of colors in gradient array.
    */
    void createColorArrayFilled(ODCOLORREF color, OdUInt32 nColors);
    /** \details
      Creates gradient colors array from other colors array.
      \param other [in]  Input colors array for copy from.
    */
    void copyGradient(const OdUInt32Array &other);
    /** \details
      Creates gradient colors array from other colors array.
      \param colors [in]  Linear array of colors.
      \param nColors [in]  Count of colors in colors linear array.
    */
    void copyGradient(const ODCOLORREF *colors, OdUInt32 nColors);
    /** \details
      Creates gradient from two input colors.
      \param color1 [in]  First color.
      \param color2 [in]  Second color.
      \param nColors [in]  Number of color entries to be created in gradient colors array.
      \param ipl [in]  Interpolation function.
    */
    void createGradient(ODCOLORREF color1, ODCOLORREF color2, OdUInt32 nColors,
                        OdGiGradientGenerator::InterpolationType ipl = OdGiGradientGenerator::kLinearInterpolation);
    /** \details
      Creates gradient from three input colors.
      \param color1 [in]  First color.
      \param color2 [in]  Second color.
      \param color3 [in]  Third color.
      \param nColors [in]  Number of color entries to be created in gradient colors array.
      \param ipl [in]  Interpolation function.
    */
    void createGradient(ODCOLORREF color1, ODCOLORREF color2, ODCOLORREF color3, OdUInt32 nColors,
                        OdGiGradientGenerator::InterpolationType ipl = OdGiGradientGenerator::kLinearInterpolation);
    /** \details
      Creates gradient from three input colors.
      \param color1 [in]  First color.
      \param color2 [in]  Second color.
      \param color3 [in]  Third color.
      \param nColors [in]  Number of color entries to be created in gradient colors array.
      \param ipl1 [in]  Interpolation function for interval between color1 and color2.
      \param ipl2 [in]  Interpolation function for interval between color2 and color3.
    */
    void createGradient(ODCOLORREF color1, ODCOLORREF color2, ODCOLORREF color3, OdUInt32 nColors,
                        OdGiGradientGenerator::InterpolationType ipl1, OdGiGradientGenerator::InterpolationType ipl2);
    /** \details
      Creates gradient from four input colors.
      \param color1 [in]  First color.
      \param color2 [in]  Second color.
      \param color3 [in]  Third color.
      \param color4 [in]  Four color.
      \param nColors [in]  Number of color entries to be created in gradient colors array.
      \param ipl [in]  Interpolation function.
    */
    void createGradient(ODCOLORREF color1, ODCOLORREF color2, ODCOLORREF color3, ODCOLORREF color4, OdUInt32 nColors,
                        OdGiGradientGenerator::InterpolationType ipl = OdGiGradientGenerator::kLinearInterpolation);
    /** \details
      Creates gradient from four input colors.
      \param color1 [in]  First color.
      \param color2 [in]  Second color.
      \param color3 [in]  Third color.
      \param color4 [in]  Fourth color.
      \param nColors [in]  Number of color entries to be created in gradient colors array.
      \param ipl1 [in]  Interpolation function for interval between color1 and color2.
      \param ipl2 [in]  Interpolation function for interval between color2 and color3.
      \param ipl3 [in]  Interpolation function for interval between color3 and color4.
    */
    void createGradient(ODCOLORREF color1, ODCOLORREF color2, ODCOLORREF color3, ODCOLORREF color4, OdUInt32 nColors,
                        OdGiGradientGenerator::InterpolationType ipl1, OdGiGradientGenerator::InterpolationType ipl2,
                        OdGiGradientGenerator::InterpolationType ipl3);
    /** \details
      Creates gradient from five input colors.
      \param color1 [in]  First color.
      \param color2 [in]  Second color.
      \param color3 [in]  Third color.
      \param color4 [in]  Fourth color.
      \param color5 [in]  Fifth color.
      \param nColors [in]  Number of color entries to be created in gradient colors array.
      \param ipl [in]  Interpolation function.
    */
    void createGradient(ODCOLORREF color1, ODCOLORREF color2, ODCOLORREF color3, ODCOLORREF color4, ODCOLORREF color5, OdUInt32 nColors,
                        OdGiGradientGenerator::InterpolationType ipl = OdGiGradientGenerator::kLinearInterpolation);
    /** \details
      Creates gradient from five input colors.
      \param color1 [in]  First color.
      \param color2 [in]  Second color.
      \param color3 [in]  Third color.
      \param color4 [in]  Fourth color.
      \param color5 [in]  Fifth color.
      \param nColors [in]  Number of color entries to be created in gradient colors array.
      \param ipl1 [in]  Interpolation function for interval between color1 and color2.
      \param ipl2 [in]  Interpolation function for interval between color2 and color3.
      \param ipl3 [in]  Interpolation function for interval between color3 and color4.
      \param ipl4 [in]  Interpolation function for interval between color4 and color5.
    */
    void createGradient(ODCOLORREF color1, ODCOLORREF color2, ODCOLORREF color3, ODCOLORREF color4, ODCOLORREF color5, OdUInt32 nColors,
                        OdGiGradientGenerator::InterpolationType ipl1, OdGiGradientGenerator::InterpolationType ipl2,
                        OdGiGradientGenerator::InterpolationType ipl3, OdGiGradientGenerator::InterpolationType ipl4);

    /** \details
      Fills gradient range between from and to.
      \param color [in]  Fill color.
      \param from [in]  Start of range for fill.
      \param to [in]  End of range for fill.
    */
    void fillInterval(ODCOLORREF color, double from = 0.0, double to = 1.0);
    /** \details
      Fills gradient range between from and to.
      \param color [in]  Fill color.
      \param from [in]  Start of range for fill.
      \param to [in]  End of range for fill.
    */
    void fillInterval(ODCOLORREF color, OdUInt32 from, OdUInt32 to);
    /** \details
      Fills range of gradient colors array with another gradient.
      \param color1 [in]  First color.
      \param color2 [in]  Second color.
      \param from [in]  Start of range for fill.
      \param to [in]  End of range for fill.
      \param ipl [in]  Interpolation function.
    */
    void generateInterval(ODCOLORREF color1, ODCOLORREF color2, double from = 0.0, double to = 1.0,
                          OdGiGradientGenerator::InterpolationType ipl = OdGiGradientGenerator::kLinearInterpolation);
    /** \details
      Fills range of gradient colors array by another gradient.
      \param color1 [in]  First color.
      \param color2 [in]  Second color.
      \param from [in]  Start of range for fill.
      \param to [in]  End of range for fill.
      \param ipl [in]  Interpolation function.
    */
    void generateInterval(ODCOLORREF color1, ODCOLORREF color2, OdUInt32 from, OdUInt32 to,
                          OdGiGradientGenerator::InterpolationType ipl = OdGiGradientGenerator::kLinearInterpolation);

    /** \details
      Retrieves count of colors in gradient colors array.
      \returns
      count of colors as OdUInt32 value.
    */
    OdUInt32 colorsCount() const;
    /** \details
      Retrieves color from gradient colors array.
      \param nColor [in]  Index of color in array.
      \returns
      Color as ODCOLORREF (OdUInt32 value).
    */
    ODCOLORREF colorAt(OdUInt32 nColor) const;
    /** \details
      Retrieves color from gradient colors array.
      \param at [in]  Color coordinate (0.0 equals 0 in array; 1.0 equals colorsCount() - 1 in array).
      \returns
      Color as ODCOLORREF (OdUInt32 value).
    */
    ODCOLORREF colorAt(double at) const;
    /** \details
      Tests whether gradient is initialized.
      \returns
      true if gradient is initialized, false otherwise.
    */
    bool isInitialized() const;
    /** \details
      Retrieves reference to array of gradient colors.
      \returns
      reference to an array that stores gradient colors.
    */
    const OdUInt32Array &asArray() const;

    /** \details
      Sets addressing mode.
      \param mode [in]  New addressing mode.
    */
    void setAddressMode(OdGiGradientGenerator::AddressMode mode);
    /** \details
      Retrieves color addressing mode which is used for this gradient generator.
      \returns
      color addressing mode as a value from the OdGiGradientGenerator::AddressMode enumeration.
    */
    OdGiGradientGenerator::AddressMode addressMode() const;

    /** \details
      Interpolates color between color1 and color2.
      \param color1 [in]  First color.
      \param color2 [in]  Second color.
      \param at [in]  Interpolation coordinate (0.0 - 1.0 for range without additional addressing).
      \param to [in]  End of range for fill.
      \param ipl [in]  Interpolation function.
      \param mode [in]  Addressing mode.
      \returns
      interpolated color as ODCOLORREF (OdUInt32 value).
    */
    static ODCOLORREF interpolateColor(ODCOLORREF color1, ODCOLORREF color2, double at,
                                       OdGiGradientGenerator::InterpolationType ipl = OdGiGradientGenerator::kLinearInterpolation,
                                       OdGiGradientGenerator::AddressMode mode = OdGiGradientGenerator::kWrapMode);

    /** \details
      Assignment operator for OdGiGradientGenerator class.
      \param other [in]  Gradient generator to assign.
      \returns
      reference to this OdGiGradientGenerator.
    */
    const OdGiGradientGenerator &operator =(const OdGiGradientGenerator &other);

    /** \details
      Creates a spectrum gradient.
      \param nColors [in]  Number of colors in gradient.
      \param alpha [in]  Alpha value for all spectrum colors.
      \returns
      spectrum gradient generator.
    */
    static OdGiGradientGenerator createSpectrumGradient(OdUInt32 nColors, OdUInt8 alpha = 0);

    /** \details
      Appends colors of the passed gradient generator to colors of this gradient generator.
      \param other [in]  Other gradient generator.
    */
    void appendGradient(const OdGiGradientGenerator &other);
    
    /** \details
      Appends colors of the passed gradient generator to colors of this gradient generator.
      \param other [in]  Other gradient generator.
    */
    void operator +=(const OdGiGradientGenerator &other);
    friend OdGiGradientGenerator operator +(const OdGiGradientGenerator &g1, const OdGiGradientGenerator &g2);
  protected:
    /** \details
      inVal addressing.
    */
    OdUInt32 clampToRange(OdUInt32 inVal) const;
    /** \details
      inVal addressing.
    */
    OdUInt32 clampToRange(double inVal) const;
    /** \details
      Convert inVal to [0.0 - 1.0] range.
    */
    double toDoubleRange(OdUInt32 inVal) const;

    /** \details
      Linear interpolation function.
    */
    static double linterp(double a, double b, double t);
    /** \details
      Exponential interpolation function.
    */
    static double einterp(double a, double b, double t);
    /** \details
      Cosine interpolation function.
    */
    static double cinterp(double a, double b, double t);
    /** \details
      Inverted exponential interpolation function.
    */
    static double ieinterp(double a, double b, double t);
};

/** \details
    Procedural textures generator.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiProceduralGenerator : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiProceduralGenerator);

    // High-level functions

    /** \details
      Generates a procedural texture.
      
      \param pTexture [in]  Procedural texture pointer.
      \param image [out]  Output image.
      \param renderCoef [in]  Texture detail multiplier. 1.0 - most standard device value, 10.0 - normal rendering.
                     This value can be calculated from raster resolution.
      \returns
      false if protocol extension not initialized or no support for current procedural texture type.
    */
    virtual bool generateProceduralTexture(OdGiProceduralTexturePtr pTexture,
                                           OdGiImageBGRA32 &image, double renderCoef = 1.0) = 0;

    /** \details
      Generates a procedural wood texture.
      \param color1 [in]  First wood texture color.
      \param color2 [in]  Second wood texture color.
      \param radialNoise [in]  Radial noise amount.
      \param axialNoise [in]  Axial noise amount.
      \param grainThickness [in]  Thickness of wood grains.
      \param image [out]  Output image.
      \param renderCoef [in]  Texture detail multiplier. 1.0 - most standard device value, 10.0 - normal rendering.
                     This value can be calculated from raster resolution.
    */
    virtual void generateProceduralWood(OdGiPixelBGRA32 color1, OdGiPixelBGRA32 color2, double radialNoise, double axialNoise, double grainThickness,
                                        OdGiImageBGRA32 &image, double renderCoef = 1.0) = 0;
    /** \details
      Generates a procedural marble texture.
      \param veinColor [in]  Color of marble veins.
      \param veinSpacing [in]  Distance between veins multiplier.
      \param veinWidth [in]  Marble veins width multiplier.
      \param image [out]  Output image.
      \param renderCoef [in]  Texture detail multiplier. 1.0 - most standard device value, 10.0 - normal rendering.
                     This value can be calculated from raster resolution.
    */
    virtual void generateProceduralMarble(OdGiPixelBGRA32 stoneColor, OdGiPixelBGRA32 veinColor, double veinSpacing, double veinWidth,
                                          OdGiImageBGRA32 &image, double renderCoef = 1.0) = 0;

    // Low-level functions

    /** \details
      Sets color gradient. 
	  
      \remarks
      If gradient is not set up, it will be generated in generateProceduralXXX
      functions. This function may be called before a generateProceduralXXX call for changing palettes
      in procedural texture generation.
    */
    virtual void setColorGradient(const OdGiGradientGenerator &gradient) = 0;
    /** \details
      Retrieves color gradient.
      \returns
      color gradient as OdGiGradientGenerator instance.
    */
    virtual const OdGiGradientGenerator &colorGradient() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiProceduralGenerator object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiProceduralGenerator> OdGiProceduralGeneratorPtr;

#include "TD_PackPop.h"

#endif // __ODGIPROCEDURALGENERATOR_H__
