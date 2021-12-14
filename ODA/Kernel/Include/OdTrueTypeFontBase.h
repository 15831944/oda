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


#ifndef _OD_ODTRUETYPEFONTBASE_INCLUDED_
#define _OD_ODTRUETYPEFONTBASE_INCLUDED_

#define  STL_USING_MAP
#include "OdaSTL.h"

#include "OdFont.h"
#include "DbRootExport.h"
#include "IntArray.h"
#include "BoolArray.h"
#include "Ge/GeVector2dArray.h"

class OdGiConveyorGeometry;

/** \details

    <group Other_Classes>
*/
class ODRX_ABSTRACT DBROOT_EXPORT OdTrueTypeFontBase : public OdFont
{
public:
  ODRX_DECLARE_MEMBERS(OdTrueTypeFontBase);

  /** \details
    Returns glyph indices and possibly some extended information needed to render given character array.

    glyphs will always be filled in.
    visualOrderFromLogical will be filled in if isSupportReorder() or isSupportExtendedLayouting() return true.
    isLTR and offsets will be filled in if isSupportExtendedLayouting() returns true.

    \param srcArray [in]
        Input text.
    \param glyphs   [out]
        Glyph indices for rendering given text.
    \param visualOrderFromLogical [out]
        Mapping from srcArray to glyphs. Please note that this is not always a one-to-one mapping.
        The length of glyphs array can be smaller or greater than the length of srcArray.
    \param isLTR [out]
        Specifies whether a character in srcArray a left-to-right character.
    \param offsets [out]
        For each glyph from glyphs array, specifies the offset of this glyph from pen position. Please
        note that this is something different from advance (which adjusts pen position after each glyph).
  */
  virtual void getGlyphIndexes(const OdCharArray& srcArray,
    OdCharArray& glyphs,
    OdIntArray* visualOrderFromLogical = NULL,
    OdBoolArray* isLTR = NULL,
    OdGeVector2dArray* offsets = NULL) = 0;

  /** \details
    If this returns true, getGlyphIndexes() method should be able to fill the input arrays visualOrderFromLogical, 
    isLTR and offsets. If this returns true, isSupportReorder() should also return true.
  */
  virtual bool isSupportExtendedLayouting() = 0;

  /** \details
    Returns true in case font support symbols reordering.
  */
  virtual bool isSupportReorder() = 0;

  virtual OdString getSubstituteFontByChar(OdChar unicodeChar, void* pSubstitutedFontData = NULL) const = 0;
};

typedef OdSmartPtr<OdTrueTypeFontBase> OdTrueTypeFontBasePtr;

#endif // _OD_ODTRUETYPEFONTBASE_INCLUDED_
