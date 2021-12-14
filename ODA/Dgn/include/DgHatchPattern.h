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
#ifndef __DG_HATCHPATTERN_H__
#define __DG_HATCHPATTERN_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "Ge/GeQuaternion.h"
#include "DoubleArray.h"

class OdDgHatchPattern;
typedef OdSmartPtr<OdDgHatchPattern> OdDgHatchPatternPtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgHatchPattern : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS( OdDgHatchPattern );

  //
  // Type declarations
  //

  //types of patterns
  enum PatternType
  {
    kLinearPattern,
    kCrossPattern,
    kSymbolPattern,
    kDWGPattern,
    kProxyPattern
  };

  //
  // Manipulations with sub-types
  //

  virtual PatternType getType() const = 0;
  static OdDgHatchPatternPtr createObject( PatternType type );

  //
  // Common features
  //

  // tolerance determine the minimal perimeter of hatched areas
  virtual double getTolerance() const = 0;
  virtual void setTolerance( double tolerance ) = 0;

  virtual bool getUseToleranceFlag() const = 0;
  virtual void setUseToleranceFlag( bool value ) = 0;

  // common rotation
  virtual OdGeQuaternion getRotation() const = 0;
  virtual void setRotation( OdGeQuaternion const& rotation ) = 0;

  //these methods should be used if the pattern is own by a 2d element
  void getRotation2d( OdGeMatrix2d& matrix ) const;
  void setRotation2d( const OdGeMatrix2d& matrix );

  virtual bool getUseRotationFlag() const = 0;
  virtual void setUseRotationFlag( bool value ) = 0;

  //used for multiline, determines the low and high lines to set the hatch between
  virtual OdUInt32 getLowMultilineIndex() const = 0;
  virtual void setLowMultilineIndex( OdUInt32 index ) = 0;
  virtual OdUInt32 getHighMultilineIndex() const = 0;
  virtual void setHighMultilineIndex( OdUInt32 index ) = 0;

  virtual bool getUseMultilineIndicesFlag() const = 0;
  virtual void setUseMultilineIndicesFlag( bool value ) = 0;

  //it determines the base for all hatches and allows to set its offset
  virtual void getOffset( OdGePoint3d& offset ) const = 0;
  virtual void setOffset( const OdGePoint3d& offset ) = 0;

  virtual bool getUseOffsetFlag() const = 0;
  virtual void setUseOffsetFlag( bool value ) = 0;

  // flags
  virtual bool getSnappableFlag() const = 0;
  virtual void setSnappableFlag( bool value ) = 0;

  virtual void scalePattern( double dScale ) = 0;
  virtual OdDgHatchPatternPtr copy() const = 0;
};

/** \details

Linear pattern

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgLinearHatchPattern;
typedef OdSmartPtr<OdDgLinearHatchPattern> OdDgLinearHatchPatternPtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgLinearHatchPattern : public OdDgHatchPattern
{
public:
  ODRX_DECLARE_MEMBERS( OdDgHatchPattern );

  // geometry
  virtual double getSpace() const = 0;
  virtual void setSpace( double space ) = 0;

  virtual double getAngle() const = 0;
  virtual void setAngle( double angle ) = 0;

  // appearance
  virtual OdUInt32 getLineStyleEntryId() const = 0;
  virtual void setLineStyleEntryId( OdUInt32 id ) = 0;

  virtual bool getUseLineStyleFlag() const = 0;
  virtual void setUseLineStyleFlag( bool value ) = 0;

  virtual OdUInt32 getLineWeight() const = 0;
  virtual void setLineWeight( OdUInt32 weight ) = 0;

  virtual bool getUseLineWeightFlag() const = 0;
  virtual void setUseLineWeightFlag( bool value ) = 0;

  virtual OdUInt32 getLineColorIndex() const = 0;
  virtual void setLineColorIndex( OdUInt32 colorIndex ) = 0;

  virtual bool getUseLineColorFlag() const = 0;
  virtual void setUseLineColorFlag( bool value ) = 0;
};

/** \details

Cross pattern

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgCrossHatchPattern;
typedef OdSmartPtr<OdDgCrossHatchPattern> OdDgCrossHatchPatternPtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgCrossHatchPattern : public OdDgHatchPattern
{
public:
  ODRX_DECLARE_MEMBERS( OdDgCrossHatchPattern );

  // geometry
  virtual double getSpace1() const = 0;
  virtual void setSpace1( double space ) = 0;

  virtual double getAngle1() const = 0;
  virtual void setAngle1( double angle ) = 0;

  virtual double getSpace2() const = 0;
  virtual void setSpace2( double space ) = 0;

  virtual double getAngle2() const = 0;
  virtual void setAngle2( double angle ) = 0;

  // appearance
  virtual OdUInt32 getLineStyleEntryId() const = 0;
  virtual void setLineStyleEntryId( OdUInt32 id ) = 0;

  virtual bool getUseLineStyleFlag() const = 0;
  virtual void setUseLineStyleFlag( bool value ) = 0;

  virtual OdUInt32 getLineWeight() const = 0;
  virtual void setLineWeight( OdUInt32 weight ) = 0;

  virtual bool getUseLineWeightFlag() const = 0;
  virtual void setUseLineWeightFlag( bool value ) = 0;

  virtual OdUInt32 getLineColorIndex() const = 0;
  virtual void setLineColorIndex( OdUInt32 colorIndex ) = 0;

  virtual bool getUseLineColorFlag() const = 0;
  virtual void setUseLineColorFlag( bool value ) = 0;
};


/** \details

Pattern with a symbol (shared cells are used here)

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgSymbolHatchPattern;
typedef OdSmartPtr<OdDgSymbolHatchPattern> OdDgSymbolHatchPatternPtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgSymbolHatchPattern : public OdDgHatchPattern
{
public:
  ODRX_DECLARE_MEMBERS( OdDgSymbolHatchPattern );

  // geometry
  virtual double getSpace1() const = 0;
  virtual void setSpace1( double space ) = 0;

  virtual double getAngle1() const = 0;
  virtual void setAngle1( double angle ) = 0;

  virtual double getSpace2() const = 0;
  virtual void setSpace2( double space ) = 0;

  virtual double getAngle2() const = 0;
  virtual void setAngle2( double angle ) = 0;

  virtual double getScale() const = 0;
  virtual void setScale( double scale ) = 0;

  // flags
  virtual bool getTrueScaleFlag() const = 0;
  virtual void setTrueScaleFlag( bool value ) = 0;

  virtual OdUInt32 getColorIndex() const = 0;
  virtual void setColorIndex( OdUInt32 colorIndex ) = 0;

  virtual bool getUseColorFlag() const = 0;
  virtual void setUseColorFlag( bool value ) = 0;

  virtual OdUInt32 getLineStyleEntryId() const = 0;
  virtual void setLineStyleEntryId( OdUInt32 id ) = 0;

  virtual bool getUseLineStyleFlag() const = 0;
  virtual void setUseLineStyleFlag( bool value ) = 0;

  virtual OdUInt32 getLineWeight() const = 0;
  virtual void setLineWeight( OdUInt32 weight ) = 0;

  virtual bool getUseLineWeightFlag() const = 0;
  virtual void setUseLineWeightFlag( bool value ) = 0;

  virtual OdUInt64 getSymbolId() const = 0;
  virtual void setSymbolId( OdUInt64 uSymbolId ) = 0;
};



class OdDgProxyHatchPattern;
typedef OdSmartPtr<OdDgProxyHatchPattern> OdDgProxyHatchPatternPtr;

/** \details

Proxy pattern

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgProxyHatchPattern: public OdDgHatchPattern
{
public:
  ODRX_DECLARE_MEMBERS( OdDgProxyHatchPattern );
  // tolerance determine the minimal perimeter of hatched areas
    virtual double getSpace1() const = 0;
    virtual void setSpace1( double ) = 0;
    virtual double getAngle1() const = 0;
    virtual void setAngle1( double ) = 0;
    virtual double getSpace2() const = 0;
    virtual void setSpace2( double ) = 0;
    virtual double getAngle2() const = 0;
    virtual void setAngle2( double ) = 0;
    virtual double getScale() const = 0;
    virtual void setScale( double ) = 0;
    virtual double getTolerance() const = 0;
    virtual void setTolerance( double ) = 0;
    //virtual void setLineStyle( OdUInt32 );
    virtual OdUInt32 getLineWeight() const = 0;
    virtual void setLineWeight( OdUInt32 ) = 0;
    //virtual void setLineColor( OdUInt32 );
    virtual OdGeQuaternion getRotation( ) const = 0;
    virtual void setRotation( OdGeQuaternion const& ) = 0;
    virtual void getOffset(OdGePoint3d& ) const = 0;
    virtual void setOffset( const OdGePoint3d& ) = 0;
    virtual OdUInt32 getMLineLowIndex() const = 0;
    virtual void setMLineLowIndex( OdUInt32 ) = 0;
    virtual OdUInt16 getHoleStyle() const = 0;
    virtual void setHoleStyle( OdUInt16 ) = 0;
    virtual OdUInt32 getMLineHighIndex() const = 0;
    virtual void setMLineHighIndex( OdUInt32 ) = 0;
    //virtual void setHoleStyle( OdUInt16 );
    //virtual void setDWGHatchDef( hatch );
    virtual double getPixelSize() const = 0;
    virtual void setPixelSize( double ) = 0;
    virtual OdUInt16 getIslandStyle() const = 0;
    virtual void setIslandStyle( OdUInt16 ) = 0;
    virtual bool getTrueScale() const = 0;
    virtual void setTrueScale(bool) = 0;
    //virtual OdGePoint3d getOrigin() const = 0;
    //virtual void setOrigin(const OdGePoint3d&) = 0;
};

/** \details

DWG-style pattern

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgDWGHatchPattern;
typedef OdSmartPtr<OdDgDWGHatchPattern> OdDgDWGHatchPatternPtr;

/** \details
  Data for a single line of the .dwg file hatch
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
struct TG_EXPORT DWGHatchLine
{
  DWGHatchLine();
  bool operator ==(const DWGHatchLine& l) const
  {
    return m_angle == l.m_angle && m_throughPoint == l.m_throughPoint && m_offset == l.m_offset && m_dashes == l.m_dashes;
  }
  double m_angle;
  OdGePoint2d m_throughPoint;
  OdGePoint2d m_offset;
  OdDoubleArray m_dashes;
};

/** \details
    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDWGHatchPattern : public OdDgHatchPattern
{

public:
  ODRX_DECLARE_MEMBERS( OdDgDWGHatchPattern );

  //complete data for the .dwg file hatch
  typedef OdArray< DWGHatchLine > DWGHatch;

  //geometry
  virtual DWGHatch const& getHatch() const = 0;
  virtual void setHatch( DWGHatch const& hatch ) = 0;

  virtual OdString getPatternName() const = 0;
  virtual void     setPatternName( const OdString& strName ) = 0;
};

#include "TD_PackPop.h"

#endif // __DG_PATCHPATTERN_H__
