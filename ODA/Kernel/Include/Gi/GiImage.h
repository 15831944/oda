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

#ifndef __ODGIIMAGE_H__
#define __ODGIIMAGE_H__

#include "GiExport.h"
#include "UInt32Array.h"
#include "RxObject.h"

#include "TD_PackPush.h"

// ODCOLORREF is RGBA32

/** \details
    This class defines pixel color parameters in RGB format, which stores an integer
	for red, green, and blue values, and also includes a value for alpha-shading.

    Corresponding C++ library: TD_Gi

    <group OdGi_Classes> 
*/
class OdGiPixelBGRA32
{
  public:
    /** \details
      Constructor for the OdGiPixelBGRA32 class. Creates an object that represent a fully opaque black color.
    */
    OdGiPixelBGRA32 ()
    {
      m_bgra = ODRGBA(0, 0, 0, 255);
    }

    /** \details
      Constructor for the OdGiPixelBGRA32 class. Creates an object with a specified value which represents
      a color in a decimal format.
      
      \param bgra [in]  Color in decimal format.
    */
    OdGiPixelBGRA32(OdUInt32 bgra)
      : m_bgra(bgra)
    {
    }

    /** \details
      Constructor for the OdGiPixelBGRA32 class. Creates an object with specified blue, red, green and alpha values.
      
      \param blue [in]  Blue color component.
      \param green [in]  Green color component.
      \param red [in]  Red color component.
      \param alpha [in]  Alpha color component.
    */
    OdGiPixelBGRA32(OdUInt8 blue, OdUInt8 green, OdUInt8 red, OdUInt8 alpha)
    {
      m_bgra = ODRGBA(blue, green, red, alpha);
    }

    /** \details
      Retrieves color in BGRA format.
      
      \returns
      Color in BGRA format as decimal value.
    */
    OdUInt32 getBGRA() const;
    
    /** \details
      Retrieves color in RGBA format.
      
      \returns
      Color in RGBA format as decimal value.
    */
    OdUInt32 getRGBA() const;
    
    /** \details
      Retrieves blue component of the color.
      
      \returns
      blue component as OdUInt8 value.
    */
    OdUInt8 blue() const;
    
    /** \details
      Retrieves green component of the color.
      
      \returns
      green component as OdUInt8 value.
    */
    OdUInt8 green() const;
    
    /** \details
      Retrieves red component of the color.
      
      \returns
      red component as OdUInt8 value.
    */
    OdUInt8 red() const;
    
    /** \details
      Retrieves alpha component of the color.
      
      \returns
      alpha component as OdUInt8 value.
    */
    OdUInt8 alpha() const;

    /** \details
      Specifies a new color.
      
      \param bgra [in]  BGRA color in decimal format.
    */
    void setBGRA(OdUInt32 bgra);
    
    /** \details
      Specifies a color with individual blue, green, red and alpha values.
      
      \param blue [in]  Blue color component.
      \param green [in]  Green color component.
      \param red [in]  Red color component.
      \param alpha [in]  Alpha color component.
    */
    void setBGRA(OdUInt8 blue, 
                 OdUInt8 green, 
                 OdUInt8 red,
                 OdUInt8 alpha);
    
    /** \details
      Specifies a new color.
      
      \param rgba [in]  RGBA color in decimal format.
    */
    void setRGBA(OdUInt32 rgba);
    
    /** \details
      Specifies a color with individual red, green, blue and alpha values.
      
      \param red [in]  Red color component.
      \param green [in]  Green color component.
      \param blue [in]  Blue color component.
      \param alpha [in]  Alpha color component.
    */
    void setRGBA(OdUInt8 red, 
                 OdUInt8 green, 
                 OdUInt8 blue,
                 OdUInt8 alpha);
    
    /** \details
      Specifies a blue component of color.
      
      \param blue [in]  Blue color component.
    */
    void setBlue(OdUInt8 blue);
    
    /** \details
      Specifies a green component of color.
      
      \param green [in]  Green color component.
    */
    void setGreen(OdUInt8 green);
    
    /** \details
      Specifies a red component of color.
      
      \param red [in]  Red color component.
    */
    void setRed(OdUInt8 red);
    
    /** \details
      Specifies alpha component of color.
      
      \param alpha [in]  Alpha color component.
    */
    void setAlpha(OdUInt8 alpha);
  private:
    OdUInt32 m_bgra;
};

/** \details
      Gets the color values in blue, green, red, alpha order.
*/
inline OdUInt32 OdGiPixelBGRA32::getBGRA() const
{
  return m_bgra;
}

/** \details
      Gets the color values in red, green, blue, alpha order.
*/
inline OdUInt32 OdGiPixelBGRA32::getRGBA() const
{
  return ODRGBA(ODGETBLUE(m_bgra), ODGETGREEN(m_bgra), ODGETRED(m_bgra), ODGETALPHA(m_bgra));
}

/** \details
      Gets the blue value of the color.
*/
inline OdUInt8 OdGiPixelBGRA32::blue() const
{
  return ODGETRED(m_bgra);
}

/** \details
      Gets the green value of the color.
*/
inline OdUInt8 OdGiPixelBGRA32::green() const
{
  return ODGETGREEN(m_bgra);
}

/** \details
      Gets the red value of the color.
*/
inline OdUInt8 OdGiPixelBGRA32::red() const
{
  return ODGETBLUE(m_bgra);
}

/** \details
      Gets the alpha shading value of the color.
*/
inline OdUInt8 OdGiPixelBGRA32::alpha() const
{
  return ODGETALPHA(m_bgra);
}

/** \details
  Sets the color values in blue, green, red, alpha order.
*/
inline void OdGiPixelBGRA32::setBGRA(OdUInt32 bgra)
{
  m_bgra = bgra;
}

/** \details
  Sets the color values in blue, green, red, alpha order.
*/
inline void OdGiPixelBGRA32::setBGRA(OdUInt8 blue, 
                                     OdUInt8 green, 
                                     OdUInt8 red,
                                     OdUInt8 alpha)
{
  m_bgra = ODRGBA(blue, green, red, alpha);
}

/** \details
      Sets the color values in red, green, blue, alpha order.
*/
inline void OdGiPixelBGRA32::setRGBA(OdUInt32 rgba)
{
  m_bgra = ODRGBA(ODGETBLUE(rgba), ODGETGREEN(rgba), ODGETRED(rgba), ODGETALPHA(rgba));
}

/** \details
      Sets the color values in red, green, blue, alpha order.
*/
inline void OdGiPixelBGRA32::setRGBA(OdUInt8 red, 
                                     OdUInt8 green, 
                                     OdUInt8 blue,
                                     OdUInt8 alpha)
{
  m_bgra = ODRGBA(blue, green, red, alpha);
}

/** \details
      Sets the blue value of a color.
*/
inline void OdGiPixelBGRA32::setBlue(OdUInt8 blue)
{
  m_bgra = ODRGBA(blue, ODGETGREEN(m_bgra), ODGETBLUE(m_bgra), ODGETALPHA(m_bgra));
}

/** \details
      Sets the green value of a color.
*/
inline void OdGiPixelBGRA32::setGreen(OdUInt8 green)
{
  m_bgra = ODRGBA(ODGETRED(m_bgra), green, ODGETBLUE(m_bgra), ODGETALPHA(m_bgra));
}

/** \details
      Sets the red value of a color.
*/
inline void OdGiPixelBGRA32::setRed(OdUInt8 red)
{
  m_bgra = ODRGBA(ODGETRED(m_bgra), ODGETGREEN(m_bgra), red, ODGETALPHA(m_bgra));
}

/** \details
      Sets the alpha-shading value of a color.
*/
inline void OdGiPixelBGRA32::setAlpha(OdUInt8 alpha)
{
  m_bgra = ODRGBA(ODGETRED(m_bgra), ODGETGREEN(m_bgra), ODGETBLUE(m_bgra), alpha);
}

/** \details
    This class defines image color parameters in RGB format, which stores an integer
	for red, green, and blue values, and includes a value for alpha-shading.

    Corresponding C++ library: TD_Gi

    <group OdGi_Classes> 
*/
class OdGiImageBGRA32
{
  public:
    /** \details
      Constructor for the OdGiImageBGRA32 class. Creates an object with no underlaying image and with 0 width and height.
    */
    OdGiImageBGRA32()
      : m_pImageData(NULL)
      , m_nImageWidth(0)
      , m_nImageHeight(0)
    {
    }

    /** \details
      Constructor for the OdGiImageBGRA32 class. Creates an image with specified width, height and pixel data.
      
      \param imageWidth [in]  Image width in pixels.
      \param imageHeight [in]  Image height in pixels.
      \param imageData [in]  Pointer to an array of OdGiPixelBGRA32 objects that represent image pixels.
    */
    OdGiImageBGRA32(OdUInt32 imageWidth, OdUInt32 imageHeight, OdGiPixelBGRA32* imageData)
      : m_pImageData(imageData)
      , m_nImageWidth(imageWidth)
      , m_nImageHeight(imageHeight)
    {
    }

    /** \details
      Retrieves pixel data of this image.
      
      \returns
      pointer to an array of OdGiPixelBGRA32 objects that represent pixels of this image.
    */
    OdGiPixelBGRA32* image() const;
    
    /** \details
      Retrieves a width of this image in pixels.
      
      \returns
      image width in pixels.
    */
    OdUInt32 width() const;
    
    /** \details
      Retrieves a height of this image in pixels.
      
      \returns
      image height in pixels.
    */
    OdUInt32 height() const;

    /** \details
      Specifies image parameters.
      
      \param imageWidth [in]  Image width in pixels.
      \param imageHeight [in]  Image height in pixels.
      \param imageData [in]  Pointer to an array of OdGiPixelBGRA32 objects that represent image pixels.
    */
    void setImage(OdUInt32 imageWidth, 
                  OdUInt32 imageHeight, 
                  OdGiPixelBGRA32* imageData);
  private:
    OdGiPixelBGRA32* m_pImageData;
    OdUInt32 m_nImageWidth;
    OdUInt32 m_nImageHeight;
};

/** \details
      Gets the buffer data of an image.
*/
inline OdGiPixelBGRA32* OdGiImageBGRA32::image() const
{
  return m_pImageData;
}

/** \details
      Gets the width of an image.
*/
inline OdUInt32 OdGiImageBGRA32::width() const
{
  return m_nImageWidth;
}

/** \details
      Gets the height of an image.
*/
inline OdUInt32 OdGiImageBGRA32::height() const
{
  return m_nImageHeight;
}

/** \details
      Sets the buffer data, width, and height of an image.
*/
inline void OdGiImageBGRA32::setImage(OdUInt32 imageWidth, 
                                      OdUInt32 imageHeight, 
                                      OdGiPixelBGRA32* imageData)
{
  m_nImageWidth = imageWidth;
  m_nImageHeight = imageHeight;
  m_pImageData = imageData;
}

/** \details
    This template class is a specialization of the OdArray class for OdGiPixelBGRA32 objects.
*/
typedef OdArray<OdGiPixelBGRA32, OdMemoryAllocator<OdGiPixelBGRA32> > OdGiPixelBGRA32Array;

#include "TD_PackPop.h"

#endif // __ODGIIMAGE_H__
