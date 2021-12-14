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




#ifndef _OD_GSBITMAP_H_
#define _OD_GSBITMAP_H_

#include "RxObject.h"
#include "Gs/Gs.h"
#include "OdStreamBuf.h"
#include "SmartPtr.h"


/** \details

    <group OdGs_Classes> 
*/
class OdGsPalette : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGsPalette);
  /** \details
    Default constructor for the OdGsPalette class.
  */
  OdGsPalette(){}

  /** \details
    Retrieves a number of colors in the palette.
  */
  virtual OdUInt32 numColors() const = 0;
  
  /** \details
    Sets a new size of this palette.
    \param nColors [in]  New number of colors in this palette.
  */
  virtual void setNumColors(OdUInt32 nColors) = 0;
  
  /** \details
    Sets a color at the specified index.
    \param nIndex [in]  Index of a color in the palette.
    \param blue   [in]  Blue component of a color in range [0..255].
    \param green  [in]  Green component of a color in range [0..255].
    \param red    [in]  Red component of a color in range [0..255].
    \param alpha  [in]  Alpha component of a color in range [0..255].
  */
  virtual void setColorAt(OdUInt32 nIndex, OdUInt8 blue, OdUInt8 green, OdUInt8 red, OdUInt8 alpha = 0) = 0;
  
  /** \details
    Retrieves a color data at the specified index.
    \param nIndex [in]  Index of a color in the palette.
    \param blue   [out]  Receives blue component of a color in range [0..255].
    \param green  [out]  Receives green component of a color in range [0..255].
    \param red    [out]  Red component of a color in range [0..255].
    \param alpha  [out]  Alpha component of a color in range [0..255].
  */
  virtual void colorAt(OdUInt32 nIndex, OdUInt8& blue, OdUInt8& green, OdUInt8& red, OdUInt8* pAlpha = 0) const = 0;
  
  /** \details
    Sets colors for this palette.
    \param nColors [in]  Number of colors in this palette.
    \param pColors [in]  Pointer to an array of colors for this palette.
  */
  virtual void setColors(OdUInt32 nColors, const ODCOLORREF* pColors);
  
  /** \details
    Retrieves colors of this palette.
    \param nColors [out]  Receives number of colors in this palette.
    \param pColors [out]  Pointer to an array of colors for this palette.
  */
  virtual void getColors(OdUInt32 nColors, ODCOLORREF* pColors) const;
};

typedef OdSmartPtr<OdGsPalette> OdGsPalettePtr;


/** \details

    <group OdGs_Classes> 
*/
class OdGsBitMap : public OdRxObject
{
protected:
  OdGsBitMap() {}
public:
  ODRX_DECLARE_MEMBERS(OdGsBitMap);

  /** \details
    Retrieves width of this bitmap.
  */
  virtual OdUInt32 width() const = 0;
  
  /** \details
    Retrieves height of this bitmap.
  */
  virtual OdUInt32 height() const = 0;
  
  /** \details
    Retrieves a number of bits that are used to represent 1 pixel.
  */
  virtual OdUInt8 bitPerPixel() const = 0;
  
  /** \details
    Creates a new bitmap.
    \param width  [in]  Bitmap width.
    \param height [in]  Bitmap height.
    \param bitCount [in]  Count of bits.
  */
  virtual void create(OdUInt32 width, OdUInt32 height, OdUInt8 bitCount) = 0;

  /** \details
    Retrieves bitmap data size.
  */
  virtual OdUInt32 bitDataSize() const;
  
  /** \details
    Retrieves a number of bytes allocated for each line.
  */
  virtual OdUInt32 bytePerLine() const;
  
  /** \details
    Retrieves a pointer to an array of bitmap data.
  */
  virtual const OdUInt8* bits() const = 0;
  
  /** \details
    Sets bitmap data.
    \param pData [in]  Pointer to an array of bits to set.
    \param nSize [in]  Number of bits in array.
  */
  virtual void setBits(const OdUInt8* pData, OdUInt32 nSize) = 0;

  /** \details
    Retrieves a pointer to an associated palette.
  */
  virtual OdGsPalette* palette() = 0;
  
  /** \details
    Retrieves a pointer to an associated palette.
  */
  virtual const OdGsPalette* palette() const = 0;
};

/** \details
  A data type that represents a smart pointer to an <link OdGsBitMap, OdGsBitMap> object.
*/
typedef OdSmartPtr<OdGsBitMap> OdGsBitMapPtr;

#endif //_OD_GSBITMAP_H_
