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

#ifndef __ODGITEXTSTYLE_H__
#define __ODGITEXTSTYLE_H__

#include "TD_PackPush.h"

#include "OdCodePage.h"
#include "OdFont.h"
#include "Gi/TtfDescriptor.h"
#include "DbBaseDatabase.h"

class OdGiWorldDraw;
class OdGiTextStyleImpl;
class OdString;
class OdGePoint2d;


/** \details
    This class provides an interface to the display attributes of text.

    \remarks
    This class is used as an argument to OdGiGeometry::text().
    \sa
    <link tv_working_with_attributes_text_style, Work with Text Styles>

    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiTextStyle
{
  // Members
  //

  // : public OdTtfDescriptor                         // MKU 14.02.2003

  OdTtfDescriptor       m_ttfDescriptor;

  mutable OdFontPtr     m_pFont;
  mutable OdFontPtr     m_pBigFont;

  mutable OdString      m_sFontPath;
  mutable OdString      m_sBigFontPath;

  OdString              m_sBigfontFile;
  double                m_dTextSize;
  double                m_dXScale;
  double                m_dObliquingAngle;
  double                m_dTrackingPercent;
  mutable OdUInt16      m_flags;
  mutable OdCodePageId  m_Codepage;

  OdString              m_StyleName;

  double                m_dIntercharSpacing;

public:
  enum
  {
    kShape       = 0x01,
    kUpsideDown  = 0x02,
    kVertical    = 0x04,
    kUnderlined  = 0x08,
    kOverlined   = 0x10,
    kShxFont     = 0x20,
    kPreLoaded   = 0x40,
    kBackward    = 0x80,
    kShapeLoaded = 0x100,
    kStriked     = 0x200,
    kUseIntercharSpacing   = 0x400,
    kFixedIntercharSpacing = 0x800,
    kNoUsePreferableFont  = 0x1000
  };

  /** \details
    Controls if the text font file for this TextStyle marked as loaded.

    \param shapeLoaded [in]  Controls shape loaded.
  */
  void setShapeLoaded(
    bool shapeLoaded) const;

  /** \details
    Checks whether the text font for this text style is loaded.
    \returns
    true if and only if the text font file for this TextStyle marked as loaded, false otherwise.
  */
  bool isShapeLoaded() const { return GETBIT(m_flags, kShapeLoaded); }

  /** \details
    Default constructor for the OdGiTextStyle class. Sets the following data members:
    <table>
    Name                  Value               Description
    m_pFont               default shx font*   Default .shx font that is defined in the font table (default font is assigned only if the system registry contains a dictionary that stores a name of properly initialized font service object).
    m_dXScale             1.0                 Scale of text by the x-axis.
    m_dTrackingPercent    1.0                 Multiplier for the spacing between characters as defined in the font file.
    m_dTextSize           0.2                 Fixed text size in drawing units.
    m_dObliquingAngle     0.0                 Oblique (skew) angle for the text.
    m_flags               0                   Various text style flags (for example, upside down characters, backward characters, etc.).
    m_Codepage            CP_UNDEFINED        Font codepage.
    m_dIntercharSpacing   0                   Inter-character spacing.
    </table>
  */
  OdGiTextStyle();
  
  /** \details
    Destructor for the OdGiTextStyle class.
  */
  ~OdGiTextStyle();

  /** \details
    Sets the parameters of this TextStyle object according to its parameters.

    \param fontName [in]  Font filename.
    \param bigFontName [in]  BigFont filename.
    \param textSize [in]  Fixed text size.
    \param xScale [in]  X-scale (width) factor of text.
    \param obliquingAngle [in]  Obliquing angle of text.
    \param trackingPercent [in]  Tracking percent of text.
    \param isBackward [in]  Controls backwards drawing of text.
    \param isUpsideDown [in]  Controls upside down drawing of text.
    \param isVertical [in]  Controls vertical drawing of text.
    \param isOverlined [in]  Controls overlined drawing of text.
    \param isUnderlined [in]  Controls overlined drawing of text.

    \remarks
    obliquingAngle has a domain of ±1.48335 radians (±85°). Negative angles will have Oda2PI added to them.

    \remarks
    All angles are expressed in radians.
  */
  void set(
    const OdString& fontName,
    const OdString&  bigFontName,
    const double textSize,
    const double xScale,
    const double obliquingAngle,
    const double trackingPercent,
    const bool isBackward,
    const bool isUpsideDown,
    const bool isVertical,
    const bool isOverlined,
    const bool isUnderlined);

  /** \details
    Loads the font file(s) associated with this TextStyle.

    \param pDb [in]  Pointer to the database whose OdDbHostAppServices object will be used.
    \remarks
    This function is run after a call to setFileName() or setBigFontFileName(), and
    must be called after loading a True Type font (TTF).
  */
  void loadStyleRec(
    OdDbBaseDatabase* pDb) const;

  /** \details
    Returns the fully qualified font file path associated with this TextStyle.

    \param pDb [in]  Pointer to the database whose OdDbHostAppServices object will be used.
  */
  OdString getFontFilePath(
    OdDbBaseDatabase* pDb) const;

  /** \details
    Returns the fully qualified BigFont file path associated with this TextStyle.

    \param pDb [in]  Pointer to the database whose OdDbHostAppServices object will be used.
  */
  OdString getBigFontFilePath(
    OdDbBaseDatabase* pDb) const;

  /** \details
    Sets the fully qualified font file path associated with this TextStyle.

    \param fontFilePath [in]  Font file path.
    \remarks
    loadStyleRec() should be called after calling this function:
  */
  void setFontFilePath(
    const OdString& fontFilePath){m_sFontPath = fontFilePath;}

  /** \details
    Sets the fully qualified BigFont file path associated with this TextStyle.

    \param bigFontFilePath [in]  BigFont file path.
    \remarks
    loadStyleRec() should be called after calling this function.
  */
  void setBigFontFilePath(
    const OdString& bigFontFilePath){m_sBigFontPath = bigFontFilePath;}

  /** \details
    Sets the fixed text size for this TextStyle.

    \param textSize [in]  Fixed text size.
  */
  void setTextSize(
    double textSize);

  /** \details
    Sets the X-scale (width) factor for this TextStyle.

    \param xScale [in]  X-scale factor.
  */
  void setXScale(
    double xScale);

  /** \details
    Sets the obliquing angle for this TextStyle.

    \param obliquingAngle [in]  Obliquing angle.

    \remarks
    obliquingAngle has a domain of ±1.48335 radians (±85°). Negative angles will have Oda2PI added to them.

    \remarks
    All angles are expressed in radians.
  */
  void setObliquingAngle(double obliquingAngle);

  /** \details
    Sets the tracking percent for this TextStyle.

    \param trackingPercent [in]  TrackingPercent [0.75, 4.0].
    \remarks
    Tracking percent is a multiplier for the spacing between characters as defined in the font file.
  */
  void setTrackingPercent(
    double trackingPercent);

  /** \details
    Controls the backwards drawing of text with this TextStyle.

    \param isBackwards [in]  Controls backwards drawing.
  */
  void setBackward(
    bool isBackward);

  /** \details
    Controls the upside down drawing of text with this TextStyle.

    \param isUpsideDown [in]  Controls upside down drawing.
  */
  void setUpsideDown(
    bool isUpsideDown);

  /** \details
    Controls the vertical drawing of text with this TextStyle.

    \param isVertical [in]  Controls vertical drawing.
  */
  void setVertical(
    bool isVertical);

  /** \details
    Controls the underlined drawing of text with this TextStyle.

    \param underlined [in]  Controls underlined drawing.
  */
  void setUnderlined(
    bool isUnderlined);

  /** \details
    Controls the overlined drawing of text with this TextStyle.

    \param overlined [in]  Controls overlined drawing.
  */
  void setOverlined(
    bool isOverlined);

  /** \details
    Controls the strike drawing of text with this TextStyle.

    \param overlined [in]  Controls strike drawing.
  */
  void setStriked(
    bool isStriked);

  /** \details
    Specifies whether this text style has a pre-loaded font. For internal use.

    \param value [in]  Flag that specifies whether an associated font is pre-loaded.
    \remarks
    If font is pre-loaded then internally there is no need to search, load and perform other action for the font.
  */
  void setPreLoaded(bool value) const;

  /** \details
    Specifies whether the set font is shx font. For internal use.

    \param value [in]  Flag that specifies whether an associated font is shx font.
  */
  void setShxFont(bool value) const;

  /** \details
    Sets the name of the font file associated with this TextStyle.

    \param fontFileName [in]  Filename.
  */
  void setFileName(
    const OdString& fontFileName);
    
  /** \details
    Sets the name of the BigFont file associated with this TextStyle.

    \param bigFontFileName [in]  Filename.
  */
  void setBigFontFileName(
    const OdString& bigFontFileName);

  /** \details
    Checks whether text is drawn backwards with this TextStyle.
    \returns 
    true if text is drawn backwards with this TextStyle, false otherwise.
  */
  bool isBackward() const   { return GETBIT(m_flags, kBackward);  }

  /** \details
    Checks whether text is drawn upside down with this TextStyle.
    \returns 
    true if text is drawn upside down with this TextStyle, false otherwise.
  */
  bool isUpsideDown() const { return GETBIT(m_flags, kUpsideDown);}

  /** \details
    Checks whether text is drawn vertical with this TextStyle.
    \returns 
    true if text is drawn vertical with this TextStyle, false otherwise.
  */
  bool isVertical() const   { return GETBIT(m_flags, kVertical);  }

  /** \details
    Checks whether text is drawn underlined with this TextStyle.
    \returns 
    true if text is drawn underlined with this TextStyle, false otherwise.
  */
  bool isUnderlined() const { return GETBIT(m_flags, kUnderlined);}

  /** \details
    Checks whether text is drawn overlined with this TextStyle.
    \returns 
    true if text is drawn overlined with this TextStyle, false otherwise.
  */
  bool isOverlined() const  { return GETBIT(m_flags, kOverlined); }

  /** \details
    Checks whether text is drawn striked with this TextStyle.
    \returns 
    true if text is drawn striked with this TextStyle, false otherwise.
  */
  bool isStriked() const  { return GETBIT(m_flags, kStriked); }

  /** \details
    Checks whether this text style has a pre-loaded font. For internal use.
    \returns
    true if an associated font is pre-loaded, false otherwise.
    \remarks
    If font is pre-loaded then internally there is no need to search, load and perform other action for the font.
  */
  bool isPreLoaded() const  { return GETBIT(m_flags, kPreLoaded); }

  /** \details
    Specifies whether the set font is shx font. For internal use.
    \returns
    true if an associated font is shx font, false otherwise.
  */
  bool isShxFont() const    { return GETBIT(m_flags, kShxFont);   }

  /** \details
    Specifies whether the set font is ttf font. For internal use.
    \returns
    true if an associated font is ttf font, false otherwise.
  */
  bool isTtfFont() const    { return !isShxFont();  }

  /** \details
    Controls if the font file for this TextStyle is treated as a shape file.
    \param isShape [in]  Controls setting.
  */
   void setIsShape(
    bool isShape) { SETBIT(m_flags, kShape, isShape); }

  /** \details
    Checks whether the font file for this TextStyle is treated as a shape file.
    \returns 
    true if the font file for this TextStyle is a shape file, false otherwise.
  */
  bool isShape() const { return GETBIT(m_flags, kShape); }


  /** \details
    Sets this TextStyle to use the specified Windows font characteristics.

    \param typeface [in] Typeface name of the font.
    \param bold [in] Bold if and only if true.
    \param italic [in] Italic if and only if true.
    \param charset [in] Windows character set identifier.
    \param pitchAndFamily [in] Windows pitch and character family identifier.

    \remarks
    If typeface is null, the Windows font information is removed from this text style.
  */
  void setFont(
    const OdString& typeface, 
    bool bold, bool italic, 
    int charset, 
    int pitchAndFamily);

  /** \details
    Sets this TextStyle to use the specified OdFont characteristics.

    \param pFont [in] Pointer to the font.
  */
  void setFont(
    OdFont* font){m_pFont = font;}

  /** \details
    Sets this TextStyle to use the specified OdFont as the BigFont.

    \param pBigFont [in]  Pointer to the BigFont.
  */
  void setBigFont(
    OdFont* pBigFont){m_pBigFont = pBigFont;}


  /** \details
    Returns the Windows font characteristics for this TextStyle.

    \param typeface [out]  Typeface name of the font.
    \param bold [out]  True if and only if bold.
    \param italic [out]  True if and only if italic.
    \param charset [out]  Windows character set identifier.
    \param pitchAndFamily [out]  Windows pitch and character family identifier.
  */
  void font(
    OdString& typeface,
    bool& bold,
    bool& italic,
    int& charset,
    int& pitchAndFamily) const;

  /** \details
    Returns the OdFont associated with this TextStyle.
  */
  OdFont* getFont() const     { return m_pFont;     }

  /** \details
    Returns the OdFont associated with the BigFont for this TextStyle.
  */
  OdFont* getBigFont() const  { return m_pBigFont;  }

  /** \details
    Returns a reference to the TTF descriptor for this TextStyle.
  */
  OdTtfDescriptor& ttfdescriptor()
  {
    return m_ttfDescriptor;
  }
  const OdTtfDescriptor& ttfdescriptor() const
  {
    return m_ttfDescriptor;
  }

  /** \details
    Returns the BigFont filename for this TextStyle.
  */
  const OdString &bigFontFileName() const
  {
    return m_sBigfontFile;
  }
  /** \details
      Returns the fixed text size for this TextStyle.
  */
  double textSize() const
  {
    return m_dTextSize;
  }

  /** \details
    Returns the X-scale (width) factor for this TextStyle.
  */
  double xScale() const
  {
    return m_dXScale;
  }

  /** \details
    Returns the obliquing angle for this TextStyle.

    \remarks
    obliquingAngle has a domain of ±1.48335 radians (±85°). Negative angles will have Oda2PI added to them.

    \remarks
    All angles are expressed in radians.
  */
  double obliquingAngle() const
  {
    return m_dObliquingAngle;
  }
  /** \details
    Returns the tracking percent for this TextStyle.

    \remarks
    Tracking percent is a multiplier for the spacing between characters as defined in the font file.

    trackingPercent() has a range of  [0.75..4].
  */
  double trackingPercent() const
  {
    return m_dTrackingPercent;
  }

//  void odExtentsBox(const OdChar* pStr, int nStrLen, OdUInt32 flags, OdDbDatabase* pDb, OdGePoint3d& min, OdGePoint3d& max, OdGePoint3d* pEndPos = 0);     // MKU 02.06.2003

  /** \details
    Returns the CodePage associated with this TextStyle.
  */
  OdCodePageId getCodePage() const
  {
    return m_Codepage;
  }

  /** \details
    Sets the CodePage associated with this TextStyle.
    \param codePage [in]  CodePage.
  */
  void setCodePage(OdCodePageId codePage)
  {
    m_Codepage = codePage;
  }

  /** \details
    Returns the style name for this TextStyle.
  */
  const OdString& styleName() const
  {
    return m_StyleName;
  }

  /** \details
    Sets the style name for this TextStyle.
    \param name [in]  Style name.
  */
  void setStyleName(const OdString& name)
  {
    m_StyleName = name;
  }

  /** \details
    Specifies whether to use interchar spacing.
    \param isUseIntercharSpacing [in]  Controls setting.
  */
   void setIsUseIntercharSpacing(
    bool isUseIntercharSpacing) { SETBIT(m_flags, kUseIntercharSpacing, isUseIntercharSpacing); }

  /** \details
    Checks whether interchar spacing is used.
    \returns 
    true if interchar spacing used, false otherwise.
  */
  bool isUseIntercharSpacing() const { return GETBIT(m_flags, kUseIntercharSpacing); }

  /** \details
    Specifies whether interchar spacing is fixed.
    \param isUseIntercharSpacing [in]  Controls setting.
  */
   void setIsFixedIntercharSpacing(
    bool isUseIntercharSpacing) { SETBIT(m_flags, kFixedIntercharSpacing, isUseIntercharSpacing); }

  /** \details
    Checks whether interchar spacing is fixed.
    \returns 
    true if fixed interchar spacing is fixed, false otherwise.
  */
  bool isFixedIntercharSpacing() const { return GETBIT(m_flags, kFixedIntercharSpacing); }

  /** \details
    Returns the interchar spacing for this TextStyle.

    \remarks
    Interchar spacing is a factor of text size, that determine distance from start of character to 
    start of next character for fixed interchar spacing mode or a additional distance from end of 
    character to start of next character for common interchar spacing mode.
  */
  double getIntercharSpacing() const;

  /** \details
    Sets interchar spacing for this TextStyle.
    \param dSpacing [in]  Interchar spacing value.
  */
  void setIntercharSpacing( double dSpacing );

  /** \details
    Specifies no using preferable font during loadStyleRec.
    \param isUsePreferableFont [in]  Controls setting.
  */
  void setNoUsePreferableFont(bool isUsePreferableFont) { SETBIT(m_flags, kNoUsePreferableFont, isUsePreferableFont); }

  /** \details
    Checks using preferable font during loadStyleRec.
    \returns
    true if no using preferable font, false otherwise.
  */
  bool isNoUsePreferableFont() const { return GETBIT(m_flags, kNoUsePreferableFont); }

};

#include "TD_PackPop.h"

#endif  // __ODGITEXTSTYLE_H__

