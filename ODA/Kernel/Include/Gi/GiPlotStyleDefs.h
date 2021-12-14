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

#ifndef __ODGIPSDEFS_H__
#define __ODGIPSDEFS_H__

#include "TD_PackPush.h"

#include "GiExport.h"
#include "UInt8Array.h"
#include "Gi/GiRasterImage.h"
#include "Ps/PlotStyles.h"
#include "Gi/GiLinetype.h"
#include "HatchPattern.h"

/** \details
    This class represents PlotStyle linetypes definitions.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiPsLinetypes
{
  public:
    /** \details
        Type of definitions utilized by framework.
    */
    enum DefType
    {
      kPsDefs     = 0, // PlotStyle linetype definitions in internal format.
      kGiDefs     = 1, // PlotStyle linetype definitions in Gi format.
      kGDIDefs    = 2, // PlotStyle linetype definitions in Windows GDI-compatible format.
      kOpenGLDefs = 4  // PlotStyle linetype definitions in OpenGL-compatible format.
    };

    /** \details
      Structure that represents PlotStyle linetype in internal format.
    */
    struct ODGI_EXPORT PsLinetypeDef
    {
      int    m_numDashes;      // Number of dashes in PlotStyle linetype.
      double m_dashLength[10]; // Lengths of dashes and gaps in PlotStyle linetype.

      /** \details
        Retrieves length of PlotStyle linetype pattern.
        \returns
        length of PlotStyle linetype pattern as a value of the double type.
      */
      double patternLength() const;
    };
    /** \details
      Structure that represents PlotStyle linetype in Windows GDI-compatible format.
    */
    struct ODGI_EXPORT PsLinetypeGDI
    {
      OdUInt32 m_numDashes;      // Number of dashes in PlotStyle linetype.
      OdUInt32 m_dashLength[20]; // Lengths of dashes and gaps in PlotStyle linetype.

      /** \details
        Retrieves the length of PlotStyle linetype pattern.
        \returns
        length of PlotStyle linetype pattern as OdUInt32 value.
      */
      OdUInt32 patternLength() const;
      /** \details
        Clears number of dashes and dash lengths by zeroes.
      */
      void clean();
    };
    /** \details
      Structure represents PlotStyle linetype in OpenGL-compatible format.
    */
    struct ODGI_EXPORT PsLinetypeOGL
    {
      OdUInt16 m_patternLength; // Linetype pattern factor (multiplier).
      OdUInt16 m_pattern;       // 16-bit linetype pattern.
    };
  protected:
    static const PsLinetypeDef g_psLinetypeDefs[OdPs::kLtpLast - 1];
    OdArray<OdGiLinetype>      m_giLinetypeDefs;
    PsLinetypeGDI              m_gdiLinetypeDef[OdPs::kLtpLast];
    PsLinetypeOGL              m_oglLinetypeDef[OdPs::kLtpLast];
    OdMutex                   *m_pMutex;
    volatile bool              m_bIsInit[4];
  public:
    /** \details
      Default constructor for OdGiPsLinetypes class.
      \param bInitialize [in]  Flag that specifies whether to initialize specified plotstyle definitions.
      \param nDefs [in]  Specifies type of definitions to check.
    */
    OdGiPsLinetypes(bool bInitialize = true, OdUInt32 nDefs = kGiDefs | kGDIDefs | kOpenGLDefs);
    
    /** \details
      Destructor for OdGiPsLinetypes class.
    */
    ~OdGiPsLinetypes();

    /** \details
      Checks whether PlotStyle linetypes manager internal structures are initialized.
      \param nDefs [in]  Specifies type of definitions to check.
      \returns
      true if plotstyle linetypes manager internal structures are initialized, false otherwise.
    */
    bool isInitialized(OdUInt32 nDefs = kPsDefs) const;
    /** \details
      Initializes PlotStyle linetype manager internal structures.
      \param nDefs [in]  Specifies type of definitions to initialize.
    */
    void initialize(OdUInt32 nDefs = kPsDefs);
    /** \details
      Frees PlotStyle linetype manager internal structures.
      \param nDefs [in]  Specifies type of definitions to uninitialize.
    */
    void uninitialize(OdUInt32 nDefs = kPsDefs | kGiDefs | kGDIDefs | kOpenGLDefs);

    /** \details
      Returns pointer onto set of PlotStyle linetype definitions in internal format.
    */
    const PsLinetypeDef *getPsDefinitions() const;
    /** \details
      Returns reference onto PlotStyle linetype definition in internal format.
      \param psLtp [in]  Linetype index.
    */
    const PsLinetypeDef &getPsDefinition(OdPs::LineType psLtp) const;

    /** \details
      Retrieves reference to array which consists of PlotStyle linetype definitions in Gi format.
      \returns
      reference to an array of OdGiLinetype objects.
    */
    const OdArray<OdGiLinetype> &getGiDefinitions() const;
    /** \details
      Retrieves reference to PlotStyle linetype definition in Gi format.
      \param psLtp [in]  Linetype index.
      \returns
      reference to linetype definition in Gi format at the specified index.
    */
    const OdGiLinetype &getGiDefinition(OdPs::LineType psLtp) const;

    /** \details
      Rasterizes linetype into a set of bytes.
      \param psLtp [in]  Linetype index.
      \param numRepetitions [in]  Number of linetype pattern repetitions.
      \param ltpValue [in]  Value which will be set for enabled linetype pattern bytes.
      \returns
      a set of bytes that represents a rasterized linetype.
      \remarks
      Background bytes (linetype gaps) will be drawn as zeroes.
      Foreground bytes (linetype dashes) will be drawn using ltpValue.
    */
    OdUInt8Array rasterizeLinetype(OdPs::LineType psLtp, OdUInt32 numRepititions = 1, OdUInt8 ltpValue = 0xFF) const;

    /** \details
      Rasterize linetype into monochrome raster image.
      \param psLtp [in]  Linetype index.
      \param numRepetitions [in]  Number of linetype pattern repetitions.
      \param numLevels [in]  Raster image height (each scanline will be created as copy of first scanline).
      \param backgroundColor [in]  Background color (linetype gaps will be drawn using this color).
      \param foregroundColor [in]  Foreground color (linetype dashes and dots will be drawn using this color).
      \returns
      a smart pointer to the rasterized linetype as monochrome image.
    */
    OdGiRasterImagePtr rasterizeLinetypeImage(OdPs::LineType psLtp, OdUInt32 numRepititions = 1, OdUInt32 numLevels = 1,
                                              ODCOLORREF backgroundColor = ODRGB(0, 0, 0),
                                              ODCOLORREF foregroundColor = ODRGB(255, 255, 255)) const;

    /** \details
      Retrieves a pointer to a set of PlotStyle linetype definitions in Windows GDI-compatible format.
      \returns
      a pointer to the array of linetypes in GDI-compatible format.
    */
    const PsLinetypeGDI *getGDIDefinitions() const;
    /** \details
      Retrieves reference onto PlotStyle linetype definition in Windows GDI-compatible format.
      \param psLtp [in]  Linetype index.
      \returns
      a reference to a GDI-compatible linetype at the specified index.
    */
    const PsLinetypeGDI &getGDIDefinition(OdPs::LineType psLtp) const;

    /** \details
      Retrieves pointer onto set of PlotStyle linetype definitions in OpenGL-compatible format.
      \returns
      a pointer to an array of OpenGL-compatible linetypes.
    */
    const PsLinetypeOGL *getOpenGLDefinitions() const;
    /** \details
      Retrieves reference onto PlotStyle linetype definition in OpenGL-compatible format.
      \param psLtp [in]  Linetype index.
      \returns
      a reference to an OpenGL-compatible linetype definition at the specified index.
    */
    const PsLinetypeOGL &getOpenGLDefinition(OdPs::LineType psLtp) const;
  protected:
    void internalInitGiLinetypes();
    void internalInitGDILinetypes();
    void internalInitOpenGLLinetypes();
    OdGiRasterImagePtr internalMakeImage(const OdUInt8 *pPixels, OdUInt32 nWidth, OdUInt32 numLevels = 1,
                                         ODCOLORREF backgroundColor = ODRGB(0, 0, 0),
                                         ODCOLORREF foregroundColor = ODRGB(255, 255, 255)) const;
  private:
    void internalDebugMe();
};

/** \details
  Retrieves PlotStyle linetypes manager.
  \param nDefs [in]  Specifies type of definitions to initialize.
*/
ODGI_EXPORT OdGiPsLinetypes &odgiGetPsLinetypesManager(OdUInt32 nDefs = OdGiPsLinetypes::kPsDefs);

/** \details
    This class represents PlotStyle fillstyles definitions.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiPsFillstyles
{
  public:
    /** \details
      Type definition represents PlotStyle fillstyle in Windows GDI-compatible format.
    */
    typedef OdUInt16 PsFillstyleGDI[8];
    /** \details
      Type definition represents PlotStyle fillstyle in OpenGL-compatible format.
    */
    typedef OdUInt8 PsFillstyleOGL[128];
  protected:
    inline OdUInt32 fsCorr(OdPs::FillStyle fs) const { return fs - OdPs::kFsSolid; }
    inline bool fsRigth(OdPs::FillStyle fs) const { return (fs >= OdPs::kFsSolid) && (fs < OdPs::kFsLast); }
    static const PsFillstyleGDI g_gdiFillstyleDef[OdPs::kFsLast - OdPs::kFsSolid];
    static const PsFillstyleOGL g_oglFillstyleDef[OdPs::kFsLast - OdPs::kFsSolid];
  public:
    /** \details
      Default constructor for OdGiPsLinetypes class.
    */
    OdGiPsFillstyles();
    
    /** \details
      Destructor for the OdGiPsFillstyles class.
    */
    ~OdGiPsFillstyles();

    /** \details
      Fills array of PlotStyle fillstyle definitions in Gi format.
      \param pPats [out]  Array of patterns to fill.
      \param scale [in]  Patterns scale multiplier.
      \param nFirst [in]  First PlotStyle fillstyle to fill.
      \param nPats [in]  Number of PlotStyle fillstyles to fill.
      \returns
      pointer to pPats patterns array.
    */
    const OdHatchPattern *getGiDefinitions(OdHatchPattern *pPats, double scale, OdUInt32 nFirst = OdPs::kFsSolid + 1, OdUInt32 nPats = OdPs::kFsLast - OdPs::kFsSolid - 1) const;
    /** \details
      Fills PlotStyle fillstyle definition.
      \param psFs [in]  Fillstyle number.
      \param pPat [out]  Reference onto pattern to fill.
      \param scale [in]  Pattern scale multiplier.
      \returns
      reference to pPat pattern definition.
    */
    const OdHatchPattern &getGiDefinition(OdPs::FillStyle psFs, OdHatchPattern &pPat, double scale) const;

    /** \details
      Rasterizes a fillstyle into set of bytes.
      \param psFs [in]  Fillstyle number.
      \param nOffsetX [in]  Horizontal pattern offset.
      \param nOffsetY [in]  Vertical pattern offset.
      \param nWidth [in]  Width of image to fill.
      \param nHeight [in]  Height of image to fill.
      \param fpValue [in]  Value which will be set for enabled fillstyle pattern bytes.
      \returns
      a set of bytes that represents a rasterized fillstyle.
      \remarks
      Background bytes (fillstyle holes) will be drawn as zeroes.
      Foreground bytes (fillstyle points) will be drawn using fpValue.
    */
    OdUInt8Array rasterizeFillstyle(OdPs::FillStyle psFs, OdUInt32 nOffsetX = 0, OdUInt32 nOffsetY = 0,
                                    OdUInt32 nWidth = 32, OdUInt32 nHeight = 32, OdUInt8 fpValue = 0xFF) const;

    /** \details
      Rasterizes a fillstyle into monochrome raster image.
      \param psFs [in]  Fillstyle number.
      \param nOffsetX [in]  Horizontal pattern offset.
      \param nOffsetY [in]  Vertical pattern offset.
      \param nWidth [in]  Width of image to fill.
      \param nHeight [in]  Height of image to fill.
      \param backgroundColor [in]  Background color (fillstyle holes will be drawn using this color).
      \param foregroundColor [in]  Foreground color (fillstyle points will be drawn using this color).
    */
    OdGiRasterImagePtr rasterizeFillstyleImage(OdPs::FillStyle psFs, OdUInt32 nOffsetX = 0, OdUInt32 nOffsetY = 0,
                                               OdUInt32 nWidth = 32, OdUInt32 nHeight = 32,
                                               ODCOLORREF backgroundColor = ODRGB(0, 0, 0),
                                               ODCOLORREF foregroundColor = ODRGB(255, 255, 255)) const;

    /** \details
      Retrieves pointer to a set of PlotStyle fillstyle definitions in Windows GDI-compatible format.
      \returns
      pointer to an array of fillstyle definitions in Windows GDI-compatible format.
    */
    const PsFillstyleGDI *getGDIDefinitions() const;
    /** \details
      Retrieves reference to PlotStyle fillstyle definition in Windows GDI-compatible format.
      \param psFs [in]  Fillstyle number.
      \returns
      a reference to PlotStyle fillstyle definition in Windows GDI-compatible format at the specified index.
    */
    const PsFillstyleGDI &getGDIDefinition(OdPs::FillStyle psFs) const;

    /** \details
      Retrieves pointer to a set of PlotStyle fillstyle definitions in OpenGL-compatible format.
      \returns
      pointer to a set of PlotStyle fillstyle definitions in OpenGL-compatible format.
    */
    const PsFillstyleOGL *getOpenGLDefinitions() const;
    /** \details
      Retrieves reference to PlotStyle fillstyle definition in OpenGL-compatible format.
      \param psFs [in]  Fillstyle number.
      reference to PlotStyle fillstyle definition in OpenGL-compatible format.
    */
    const PsFillstyleOGL &getOpenGLDefinition(OdPs::FillStyle psFs) const;
  protected:
    void internalFillHatchDef(OdPs::FillStyle psFs, OdHatchPattern &pPat, double scale) const;
    OdGiRasterImagePtr internalMakeImage(const OdUInt8 *pPixels, OdUInt32 nWidth, OdUInt32 nHeight,
                                         ODCOLORREF backgroundColor = ODRGB(0, 0, 0),
                                         ODCOLORREF foregroundColor = ODRGB(255, 255, 255)) const;
  private:
    void internalDebugMe() const;
};

/** \details
  Retrieves PlotStyle fillstyles manager.
*/
ODGI_EXPORT OdGiPsFillstyles &odgiGetPsFillstylesManager();

#include "TD_PackPop.h"

#endif //#ifndef __ODGIPSDEFS_H__
