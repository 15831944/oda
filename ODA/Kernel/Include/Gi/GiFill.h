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

#ifndef _OD_GIFILL_H_
#define _OD_GIFILL_H_

#include "RxObject.h"
#include "HatchPattern.h"
#include "UInt8Array.h"
#include "CmColorBase.h"

#include "TD_PackPush.h"

/** \details
    This class is a base class for boundary filling definitions.

    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiFill : public OdRxObject
{
  protected:
    double m_dDeviation;
  public:
    ODRX_DECLARE_MEMBERS(OdGiFill);
    /** \details
      Default constructor for the OdGiFill class. Sets deviation of fill to 0.
    */
    OdGiFill();

    /** \details
      Retrieves deviation value of the fill.
    */
    double deviation() const;
    
    /** \details
      Sets deviation of the fill.
      
      \param dDeviation [in]  Deviation for the fill.
    */
    void setDeviation(double dDeviation);

    virtual void copyFrom(const OdRxObject* pSource);

    virtual bool operator ==(const OdGiFill& fill) const;
    virtual bool operator !=(const OdGiFill& fill) const;

    virtual void saveBytes(OdUInt8Array &bytes) const;
    virtual void loadBytes(const OdUInt8 *pBytes);

    static OdSmartPtr<OdGiFill> loadFill(const OdUInt8 *pBytes);
  protected:
    void pushDbl(OdUInt8Array &bytes, double dDbl) const;
    void pushInt(OdUInt8Array &bytes, OdUInt32 nInt) const;
    void popDbl(const OdUInt8 *&pBytes, double &dDbl) const;
    void popInt(const OdUInt8 *&pBytes, OdUInt32 &nInt) const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiFill object pointers.
*/
typedef OdSmartPtr<OdGiFill> OdGiFillPtr;

inline
OdGiFill::OdGiFill()
  : m_dDeviation(0.0)
{ }

inline
double OdGiFill::deviation() const
{
  return m_dDeviation;
}

inline
void OdGiFill::setDeviation(double dDeviation)
{
  m_dDeviation = dDeviation;
}

inline
void OdGiFill::copyFrom(const OdRxObject* pSource)
{
  OdGiFillPtr pSrcFill = OdGiFill::cast(pSource);
  if (!pSrcFill.isNull())
    setDeviation(pSrcFill->deviation());
  else
    throw OdError(eNotApplicable);
}

inline
bool OdGiFill::operator ==(const OdGiFill& fill) const
{
  return (isA() == fill.isA()) && OdEqual(deviation(), fill.deviation());
}

inline
bool OdGiFill::operator !=(const OdGiFill& fill) const
{
  return (isA() != fill.isA()) || !OdEqual(deviation(), fill.deviation());
}

inline
void OdGiFill::saveBytes(OdUInt8Array &bytes) const
{
  bytes.push_back(0);
  pushDbl(bytes, m_dDeviation);
}

inline
void OdGiFill::loadBytes(const OdUInt8 *pBytes)
{
  if (*pBytes++ != 0)
    throw OdError(eInvalidInput);
  popDbl(pBytes, m_dDeviation);
}

inline
void OdGiFill::pushDbl(OdUInt8Array &bytes, double dDbl) const
{
  OdUInt8 nBytes[sizeof(double)];
  ::memcpy(nBytes, &dDbl, sizeof(double));
  bytes.insert(bytes.end(), nBytes, nBytes + sizeof(double));
}

inline
void OdGiFill::pushInt(OdUInt8Array &bytes, OdUInt32 nInt) const
{
  OdUInt8 nBytes[sizeof(OdUInt32)];
  ::memcpy(nBytes, &nInt, sizeof(OdUInt32));
  bytes.insert(bytes.end(), nBytes, nBytes + sizeof(OdUInt32));
}

inline
void OdGiFill::popDbl(const OdUInt8 *&pBytes, double &dDbl) const
{
  ::memcpy(&dDbl, pBytes, sizeof(double));
  pBytes += sizeof(double);
}

inline
void OdGiFill::popInt(const OdUInt8 *&pBytes, OdUInt32 &nInt) const
{
  ::memcpy(&nInt, pBytes, sizeof(OdUInt32));
  pBytes += sizeof(OdUInt32);
}

/** \details
    This class is a specialization of OdGiFill for hatch pattern type of filling.

    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiHatchPattern : public OdGiFill
{
  protected:
    OdHatchPattern m_aHatchPattern;
    double m_dPatternScale;
    OdDb::LineWeight m_patternLineWeight;
    OdCmEntityColor m_patternColor;
    OdCmTransparency m_patternTransparency;

    enum Flags { kDraft = (1 << 0), kExternal = (1 << 1) };
    OdUInt32 m_nFlags;
  public:
    ODRX_DECLARE_MEMBERS(OdGiHatchPattern);
    /** \details
      Default constructor for the OdGiHatchPattern class. Sets the pattern scale to 1.0, pattern flags to 0 and initializes
      the underlaying fill object. 
    */
    OdGiHatchPattern();

    /** \details
      Retrieves lines that compose this pattern.  
      
      \returns
      a reference to the OdHatchPattern that represents an array of pattern lines.
    */
    const OdHatchPattern &patternLines() const;
    
    /** \details
      Retrieves lines that compose this pattern.  
      
      \returns
      a reference to the OdHatchPattern that represents an array of pattern lines.
    */
    OdHatchPattern &patternLines();
    
    /** \details
      Specifies pattern lines for this hatch pattern.
      
      \param aHatchPattern [in]  An array of hatch pattern lines.
    */
    void setPatternLines(const OdHatchPattern &aHatchPattern);

    virtual void copyFrom(const OdRxObject* pSource);

    virtual bool operator ==(const OdGiFill& fill) const;
    virtual bool operator !=(const OdGiFill& fill) const;

    virtual void saveBytes(OdUInt8Array &bytes) const;
    virtual void loadBytes(const OdUInt8 *pBytes);

    bool isDraft() const;
    void setDraft(bool draft);

    /** \details
      Retrieves scale of this pattern.  
      
      \returns
      scale of the hatch pattern as a value of double type.
    */
    double patternScale() const;
    
    /** \details
      Specifies pattern scale for this hatch pattern.
      
      \param scale [in]  Scale of the pattern.
    */
    void setPatternScale( double scale );

    bool isExternal() const;
    void setExternal(bool isExt);

    OdCmEntityColor getPatternColor() const;
    void setPatternColor(const OdCmEntityColor& color);

    OdDb::LineWeight getPatternLineWeight() const;
    void setPatternLineWeight(OdDb::LineWeight lineweight);

    /** \details
    Is used to obtain custom pattern transparency value (Default is 255)
    \returns Pattern transparency value
    */
    OdCmTransparency getPatternTransparency() const;

    /** \details
    Sets custom pattern transparency value.
    \param transparency [in] Pattern transparency value.
    */
    void setPatternTransparency(const OdCmTransparency& transparency);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiHatchPattern object pointers.
*/
typedef OdSmartPtr<OdGiHatchPattern> OdGiHatchPatternPtr;

inline
OdGiHatchPattern::OdGiHatchPattern() :
  OdGiFill(),
  m_nFlags(0),
  m_dPatternScale(1.0),
  m_patternLineWeight(OdDb::LineWeight(-1)),
  m_patternColor(OdCmEntityColor::kNone),
  m_patternTransparency((OdUInt8)255)
  { }

inline
const OdHatchPattern &OdGiHatchPattern::patternLines() const
{
  return m_aHatchPattern;
}

inline
OdHatchPattern &OdGiHatchPattern::patternLines()
{
  return m_aHatchPattern;
}

inline
void OdGiHatchPattern::setPatternLines(const OdHatchPattern &aHatchPattern)
{
  m_aHatchPattern = aHatchPattern;
}

inline
void OdGiHatchPattern::copyFrom(const OdRxObject* pSource)
{
  OdGiHatchPatternPtr pSrcFill = OdGiHatchPattern::cast(pSource);
  if (!pSrcFill.isNull())
  {
    m_patternColor = pSrcFill->m_patternColor;
    m_patternLineWeight = pSrcFill->m_patternLineWeight;
    m_nFlags = pSrcFill->m_nFlags;
    m_dPatternScale = pSrcFill->m_dPatternScale;
    setPatternLines(pSrcFill->patternLines());
    OdGiFill::copyFrom(pSource);
  }
  else
    throw OdError(eNotApplicable);
}

inline
bool OdGiHatchPattern::operator ==(const OdGiFill& fill) const
{
  if (isA() == fill.isA())
  {
    if (OdEqual(deviation(), fill.deviation()))
    {
      const OdGiHatchPattern *pSecond = static_cast<const OdGiHatchPattern*>(&fill);
      if (patternLines().size() == pSecond->patternLines().size() &&
          m_nFlags == pSecond->m_nFlags &&
          OdEqual(m_dPatternScale, pSecond->m_dPatternScale) &&
          m_patternColor == pSecond->m_patternColor &&
          m_patternLineWeight == pSecond->m_patternLineWeight)
      {
        for (OdUInt32 nPat = 0; nPat < patternLines().size(); nPat++)
        {
          const OdHatchPatternLine &pl1 = patternLines().getPtr()[nPat];
          const OdHatchPatternLine &pl2 = pSecond->patternLines().getPtr()[nPat];
          if (!OdEqual(pl1.m_dLineAngle, pl2.m_dLineAngle) ||
              !OdEqual(pl1.m_basePoint.x, pl2.m_basePoint.x) || !OdEqual(pl1.m_basePoint.y, pl2.m_basePoint.y) ||
              !OdEqual(pl1.m_patternOffset.x, pl2.m_patternOffset.x) || !OdEqual(pl1.m_patternOffset.y, pl2.m_patternOffset.y) ||
              pl1.m_dashes.size() != pl2.m_dashes.size())
            return false;
          for (OdUInt32 nDash = 0; nDash < pl1.m_dashes.size(); nDash++)
          {
            if (!OdEqual(pl1.m_dashes.getPtr()[nDash], pl2.m_dashes.getPtr()[nDash]))
              return false;
          }
        }
        return true;
      }
    }
  }
  return false;
}

inline
bool OdGiHatchPattern::operator !=(const OdGiFill& fill) const
{
  return !operator ==(fill);
}

inline
void OdGiHatchPattern::saveBytes(OdUInt8Array &bytes) const
{
  bytes.push_back(3);
  pushInt(bytes, m_nFlags);
  pushDbl(bytes, m_dPatternScale);
  pushInt(bytes, m_patternLineWeight);
  pushInt(bytes, m_patternColor.color());
  pushInt(bytes, m_patternTransparency.alpha());
  pushInt(bytes, m_aHatchPattern.size());
  for (OdUInt32 nPat = 0; nPat < m_aHatchPattern.size(); nPat++)
  {
    const OdHatchPatternLine &pl = m_aHatchPattern.getPtr()[nPat];
    pushDbl(bytes, pl.m_dLineAngle);
    pushDbl(bytes, pl.m_basePoint.x); pushDbl(bytes, pl.m_basePoint.y);
    pushDbl(bytes, pl.m_patternOffset.x); pushDbl(bytes, pl.m_patternOffset.y);
    pushInt(bytes, pl.m_dashes.size());
    for (OdUInt32 nDash = 0; nDash < pl.m_dashes.size(); nDash++)
      pushDbl(bytes, pl.m_dashes.getPtr()[nDash]);
  }
  OdGiFill::saveBytes(bytes);
}

inline
void OdGiHatchPattern::loadBytes(const OdUInt8 *pBytes)
{
  OdUInt32 lineWeight;
  OdUInt32 color;
  OdUInt32 alpha;
  
  OdUInt8 version = *pBytes++;
  if (version < 1 || version > 3)
    throw OdError(eInvalidInput);

  popInt(pBytes, m_nFlags);
  popDbl(pBytes, m_dPatternScale);
  
  if (version > 1)
  {
    popInt(pBytes, lineWeight);
    popInt(pBytes, color);
  }
  if (version > 2)
  {
    popInt(pBytes, alpha);
  }

  OdUInt32 nPats = 0; popInt(pBytes, nPats);
  m_aHatchPattern.resize(nPats);
  for (OdUInt32 nPat = 0; nPat < nPats; nPat++)
  {
    OdHatchPatternLine &pl = m_aHatchPattern[nPat];
    popDbl(pBytes, pl.m_dLineAngle);
    popDbl(pBytes, pl.m_basePoint.x); popDbl(pBytes, pl.m_basePoint.y);
    popDbl(pBytes, pl.m_patternOffset.x); popDbl(pBytes, pl.m_patternOffset.y);
    OdUInt32 nDashes = 0; popInt(pBytes, nDashes);
    pl.m_dashes.resize(nDashes);
    for (OdUInt32 nDash = 0; nDash < nDashes; nDash++)
      popDbl(pBytes, pl.m_dashes[nDash]);
  }
  OdGiFill::loadBytes(pBytes);

  if (version > 1)
  {
    m_patternLineWeight = (OdDb::LineWeight)lineWeight;
    m_patternColor.setColor(color);
  }
  if (version > 2)
  {
    m_patternTransparency.setAlpha((OdUInt8)alpha);
  }
}

inline
OdSmartPtr<OdGiFill> OdGiFill::loadFill(const OdUInt8 *pBytes)
{
  OdGiFillPtr pObj;
  switch (*pBytes)
  {
    case 0: pObj = OdGiFill::createObject(); break;
    case 1:
    case 2:
    case 3: pObj = OdGiHatchPattern::createObject(); break;
  }
  if (!pObj.isNull())
    pObj->loadBytes(pBytes);
  return pObj;
}

inline
bool OdGiHatchPattern::isDraft() const
{
  return GETBIT(m_nFlags, kDraft);
}

inline
void OdGiHatchPattern::setDraft(bool draft)
{
  SETBIT(m_nFlags, kDraft, draft);
}

inline
double OdGiHatchPattern::patternScale() const
{
  return m_dPatternScale;
}

inline
void OdGiHatchPattern::setPatternScale(double scale)
{
  m_dPatternScale = scale;
}

inline
bool OdGiHatchPattern::isExternal() const
{
  return GETBIT(m_nFlags, kExternal);
}

inline
void OdGiHatchPattern::setExternal(bool isExt)
{
  SETBIT(m_nFlags, kExternal, isExt);
}

inline
OdCmEntityColor OdGiHatchPattern::getPatternColor() const
{
  return m_patternColor;
}

inline
void OdGiHatchPattern::setPatternColor(const OdCmEntityColor& color)
{
  m_patternColor = color;
}

inline
OdDb::LineWeight OdGiHatchPattern::getPatternLineWeight() const
{
  return m_patternLineWeight;
}

inline
void OdGiHatchPattern::setPatternLineWeight(OdDb::LineWeight lineWeight)
{
  m_patternLineWeight = lineWeight;
}

inline
OdCmTransparency OdGiHatchPattern::getPatternTransparency() const
{
  return m_patternTransparency;
}

inline
void OdGiHatchPattern::setPatternTransparency(const OdCmTransparency& transparency)
{
  m_patternTransparency = transparency;
}

#include "TD_PackPop.h"

#endif //_OD_GIFILL_H_
