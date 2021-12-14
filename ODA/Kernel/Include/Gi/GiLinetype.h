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

#ifndef _ODGILINETYPESEG_INCLUDED_
#define _ODGILINETYPESEG_INCLUDED_


#include "TD_PackPush.h"

#include "OdString.h"
#include "OdArray.h"
#include <math.h>


class OdDbStub;


/** \details

    <group OdGi_Classes> 
*/
class OdGiLinetypeDash
{
public:
  ODRX_HEAP_OPERATORS();

  /** \details
    Represents shape offset by x and y axes.
  */
  struct offset
  {
  /** Offset by x axis. */
  double            x;
  /** Offset by y axis. */
	double            y;
  };
  /** Dash length. */
  double            length;
	/** Scale of a shape. */
  double            shapeScale;
  /** Shape rotation. */
	double            shapeRotation;
  /** Dash flags that represent whether dash rotation is absolute, whether rotation is upright, etc. */
  OdUInt16          flags;
  /** Shape number in the ASCII table. */
  OdUInt16          shapeNumber;
  /** Shape offset. */
	offset            shapeOffset;
	/** Dash as a text string. */
  OdString          textString;
  /** Text style identifier for a text dash. */
	OdDbStub*         styleId;

  /** \details
    Default constructor for the OdGiLinetypeDash class. Sets the following data members:
    <table>
    Data member           Value     Description
    shapeScale            1.0       Scale of a shape
    shapeRotation         0.0       Shape rotation
    flags                 0         Dash flags that represent whether dash rotation is absolute, whether rotation is upright, etc
    shapeOffset           0.0       Shape offset by x and y axes
    styleId               NULL      Text style identifier for a text dash
    </table>
  */
  OdGiLinetypeDash()
  {
    shapeScale = 1.0;
    shapeRotation = shapeScale = length = 0.0;
    flags = shapeNumber = 0;
    shapeOffset.x = shapeOffset.y = 0.0;                                    // MKU 10.03.2004   CR 1690
    styleId = NULL;
  }

  /** \details
    Checks whether orientation for the linetype dash is absolute (UCS-aligned).
    
    \returns
    true if linetype dash is UCS-aligned, false otherwise.
  */
  bool isRotationAbsolute() const { return GETBIT(flags, 1); }
	
  /** \details
    Specifies whether orientation for the linetype dash should be absolute (UCS-aligned).
    
    \param value [in]  Flag that specifies whether orientation of the dash is absolute.
  */
  void setRotationAbsolute(bool value = true) { SETBIT(flags, 1, value); }

  /** \details
    Checks whether rotation of linetype elements is upright, i.e. some elements which may be required to drawn upside down or backward (e.g. on circular paths) are still drawn upright.
    
    \returns
    true if linetype elements are rotated upright, false otherwise.
  */
  bool isRotationUpright() const { return GETBIT(flags, 8); }
	
  /** \details
    Specifies whether rotation of linetype elements should be upright, i.e. some elements will be drawn upright even if situation requires them to be drawn  upside down or backward (e.g. on circular paths).
    
    \param value [in]  Flag that specifies whether rotation is upright.
  */
  void setRotationUpright(bool value = true) { SETBIT(flags, 8, value); }

  /** \details
    Checks whether this linetype dash is an embedded text string.
    
    \returns
    true if this linetype dash is an embedded text string, false otherwise.
  */
  bool isEmbeddedTextString() const { return GETBIT(flags, 2); }
	
  /** \details
    Specifies whether this linetype dash should be an embedded text string.
    
    \param value [in]  Flag that specifies whether this linetype dash should be an embedded text string.
  */
  void setEmbeddedTextString(bool value = true) { SETBIT(flags, 2, value); }

  /** \details
    Checks whether this linetype dash is an embedded shape.
    
    \returns
    true if this linetype dash is an embedded shape, false otherwise.
  */
  bool isEmbeddedShape() const { return GETBIT(flags, 4); }
	
  /** \details
    Specifies whether this linetype dash should be an embedded shape.
    
    \param value [in]  Flag that specifies whether this linetype dash should be an embedded shape.
  */
  void setEmbeddedShape(bool value = true) { SETBIT(flags, 4, value); }
};

typedef OdArray<OdGiLinetypeDash> OdGiLinetypeDashArray;
/** \details

    <group OdGi_Classes> 
*/
class OdGiLinetype
{
protected:
	enum
	{
		/** Linetype is inherited by block definition. */
    kByBlock            = 1,
		/** Linetype is inherited by layer. */
    kByLayer            = 2,
    /** Linetype pattern length is valid. */
		kPatternLengthValid = 4
	};

  mutable OdUInt32          m_flags;
  mutable double            m_patternLength;
  OdGiLinetypeDashArray     m_dashes;
  friend class OdDbLinetypeTableRecord;
  friend class OdDbLinetypeTableRecordImpl;
public:
  /** \details
    Default constructor for the OdGiLinetype class. Sets the flag that specifies that pattern length is valid and set pattern length to 0.
  */
  OdGiLinetype();

  /** \details
    Checks whether this linetype is continuous (linetype has no dashes).
    
    \returns
    true if this linetype is continuous, false otherwise.
  */
  bool isContinuous() const;

  /** \details
    Checks whether this linetype is inherited by the block definition.
    
    \returns
    true if this linetype is inherited by the block definition, false otherwise.
  */
  bool isByBlock() const;
	
  /** \details
    Specifies whether this linetype should inherit linetype parameters provided by the block definition.
    
    \param value [in]  Flag that specifies whether this linetype should be inherited by block.
  */
  void setByBlock(bool value);

  /** \details
    Checks whether this linetype is inherited by the layer.
    
    \returns
    true if this linetype is inherited by the layer, false otherwise.
  */
  bool isByLayer() const;
	
  /** \details
    Specifies whether this linetype should inherit linetype parameters provided by the block definition.
    
    \param value [in]  Flag that specifies whether this linetype should be inherited by layer.
  */
  void setByLayer(bool value);

  /** \details
    Retrieves the overall length of this linetype pattern.
    
    \returns
    pattern length as a value of the double type.
  */
  double patternLength() const;

  /** \details
    Retrieves the overall length of this linetype pattern.
    
    \param patternLength [in]  Overall pattern length.
  */
  void setPatternLength(double patternLength);

  /** \details
    Retrieves the number of dashes of this linetype.
    
    \returns
    quantity of dashes as integer value.
  */
  int numDashes() const;

  /** \details
    Specifies the number of dashes for this linetype.
    
    \param count [in]  Number of dashes in the pattern.
  */
  void setNumDashes(int count);

  /** \details
    Retrieves the dash at the specified index.
    
    \param index [in]  Dash index.
    \param dash [out]  Receives the dash.
  */
  void dashAt(int index, OdGiLinetypeDash& dash) const;
  
  /** \details
    Retrieves the dash at the specified index.
    
    \param index [in]  Dash index.
    
    \returns
    reference to the OdGiLinetypeDash instance which represents the dash.
  */
  const OdGiLinetypeDash& dashAt(int index) const;

  /** \details
    Sets the dash at the specified index.
    
    \param index [in]  Dash index.
    \param value [out]  Dash length.
  */
  void setDashAt(int index, double value);
  
  /** \details
    Sets the dash at the specified index.
    
    \param index [in]  Dash index.
    \param dash [out]  Linetype dash to set.
  */
  void setDashAt(int index, const OdGiLinetypeDash& dash);

  
  /** \details
    Retrieves dashes of this linetype.
    
    \param dashes [out]  Receives linetype dashes.
  */
  void dashes(OdGiLinetypeDashArray& dashes) const;
  
  /** \details
    Sets dashes for this linetype.
    
    \param dashes [in]  Linetype dashes to set.
  */
  void setDashes(const OdGiLinetypeDashArray& dashes);
};


inline bool OdGiLinetype::isByBlock() const     { return GETBIT(m_flags, kByBlock); }
inline void OdGiLinetype::setByBlock(bool value){ SETBIT(m_flags, kByBlock, value); }

inline bool OdGiLinetype::isByLayer() const     { return GETBIT(m_flags, kByLayer); }
inline void OdGiLinetype::setByLayer(bool value){ SETBIT(m_flags, kByLayer, value); }

inline bool OdGiLinetype::isContinuous() const  { return m_dashes.isEmpty(); }

inline void OdGiLinetype::dashes(OdGiLinetypeDashArray& dashes) const { dashes = m_dashes; }
inline void OdGiLinetype::setDashes(const OdGiLinetypeDashArray& dashes)
{
  m_dashes = dashes;
  SETBIT(m_flags, kPatternLengthValid, false);
}

inline OdGiLinetype::OdGiLinetype()
  : m_flags(kPatternLengthValid)
  , m_patternLength(0.)
{
}

inline double OdGiLinetype::patternLength() const
{
  if(!GETBIT(m_flags, kPatternLengthValid))
  {
    int n = m_dashes.size();
    m_patternLength = 0.0;
    while(n--)
    {
      m_patternLength += fabs(m_dashes[n].length);
    }
    SETBIT(m_flags, kPatternLengthValid, true);
  }
  return m_patternLength;
}

inline void OdGiLinetype::setPatternLength(double patternLength)
{
  m_patternLength = patternLength;
  if ( OdZero(patternLength) )
    SETBIT(m_flags, kPatternLengthValid, false);
  else
    SETBIT(m_flags, kPatternLengthValid, true);
}

inline int OdGiLinetype::numDashes() const
{
  return m_dashes.size();
}

inline void OdGiLinetype::setNumDashes(int count)
{
  SETBIT(m_flags, kPatternLengthValid, false);
  m_dashes.setLogicalLength(count);
}

inline void OdGiLinetype::dashAt(int index, OdGiLinetypeDash& dash) const
{
  dash = m_dashes[index];
}

inline const OdGiLinetypeDash& OdGiLinetype::dashAt(int index) const
{
  return m_dashes[index];
}

inline void OdGiLinetype::setDashAt(int index, const OdGiLinetypeDash& dash)
{
  SETBIT(m_flags, kPatternLengthValid, false);
  m_dashes[index] = dash;
}

inline void OdGiLinetype::setDashAt(int index, double dashLength)
{
  SETBIT(m_flags, kPatternLengthValid, false);
  m_dashes[index] = OdGiLinetypeDash();
  m_dashes[index].length = dashLength;
}

#include "TD_PackPop.h"

#endif //_ODGILINETYPESEG_INCLUDED_
