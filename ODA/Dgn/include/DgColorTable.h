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
#ifndef __DG_COLORTABLE_H__
#define __DG_COLORTABLE_H__


#include "TD_PackPush.h"

#include "DgElement.h"
#include "DgCmColor.h"

class OdCmEntityColor;

/** \details
    1. The ordinary color table of DGN files. Each database (file) could have its color table of 256 colors (otherwise, the default color table is available).
        This table keeps all color for the indices [0...255].

    2. Also, the extended color table could be stored among xAttributes of the file (see the static methods below).
        This table could keep an unlimited number of colors linked by indices of { indices > 255 }.

    3. Please, note that a set of values is predefined (kColorByCell, kByLevel, etc). See OdDgGraphicsElement.h for details. 
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgColorTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgColorTable);
public:

  //uses this table only to search for the color;
  //returns zero if fails (+the method to check indices)
  ODCOLORREF lookupRGB(OdUInt32 colorIndex);
  bool isCorrectIndex( OdUInt32 colorIndex );

  //use the color tables of the given database to search for the color (either ordinary or extended ones);
  //returns zero if fails (+the method to check indices)
  static OdDgCmEntityColor lookupTrueColor(const OdDgDatabase* pDb, OdUInt32 colorIndex);
  static ODCOLORREF lookupRGB(OdDgDatabase* pDb, OdUInt32 colorIndex);
  static bool isCorrectIndex( const OdDgDatabase* pDb, OdUInt32 colorIndex );

  //it tries to find the given RGB color within existing color tables (or add a new color to the extended color table)
  static OdUInt32 getColorIndexByRGB(  OdDgDatabase* pDb, ODCOLORREF rgbColor ) ;

  const ODCOLORREF* palette();
  void setPalette(OdArray<ODCOLORREF> colors);

  /** \details
  Load color table from color resource *.tbl file.

  \remarks
  Return true if success.
  */
  virtual bool loadFromFile(const OdString& strFileName);

  /** \details
  Save color table to color resource *.tbl file.
  */
  virtual void saveToFile(const OdString& strFileName) const;

  static const ODCOLORREF* currentPalette(const OdDgDatabase* pDb);
  static const ODCOLORREF* defaultPalette();
  static const ODCOLORREF* defaultACadPalette();

  // If background is white MicroStation draws elements black instead of white.
  //  To do the same if background color is defined for this palette as white (element with index #254) 
  //  then all elements in palette with white will be tuned to black.
  static bool correctPaletteForWhiteBackground(ODCOLORREF* palette);

  /** \details
    Return true if rgb color is present into color table or extended color table.

    \param pDb [in] pointer to database.
    \param rgbColor [in] rgb color.
    \param uColorIndex [out] index of dgn color.
  */

  static bool hasRGBColor( OdDgDatabase* pDb, const ODCOLORREF& rgbColor, OdUInt32& uColorIndex );

  /** \details
    Return count of extended colors into extended color table.

    \param pDb [in] pointer to database.
  */
  static OdUInt32 getExtendedColorCount( OdDgDatabase* pDb );

  /** \details
    Return dgn color index of extended color by index into extended color table.

    \param pDb [in] pointer to database.
    \param uNumberIntoTable [in] Index of extended color into table.
  */
  static OdUInt32 getExtendedColorIndex( OdDgDatabase* pDb, OdUInt32 uNumberIntoTable );

  /** \details
    Add extended color to extended color table. Return dgn color index of new color.

    \param pDb [in] pointer to database.
    \param newDgnColor [in] Color to add.
  */
  static OdUInt32 addExtendedColor( OdDgDatabase* pDb, const OdDgCmColor& newDgnColor );

  /** \details
    Get extended color from extended color table by dgn color index.
    Return true if color with requested dgn color index is present into table.

    \param pDb [in] pointer to database.
    \param uColorIndex [in] Dgn color index of requested color.
    \param dgnColor [out] Requested color.
  */
  static bool     getExtendedColor( OdDgDatabase* pDb, OdUInt32 uColorIndex, OdDgCmColor& dgnColor );

  /** \details
    Set extended color to extended color table by dgn color index.
    Return true if color with requested dgn color index is presented into table and
    modification of color successfully finished.

    \param pDb [in] pointer to database.
    \param uColorIndex [in]  Dgn color index of requested color.
    \param dgnColor    [in]  Color to set.
  */
  static bool     setExtendedColor( OdDgDatabase* pDb, OdUInt32 uColorIndex, const OdDgCmColor& dgnColor );
};

typedef OdSmartPtr<OdDgColorTable> OdDgColorTablePtr;


#include "TD_PackPop.h"

#endif // __DG_COLORTABLE_H__
