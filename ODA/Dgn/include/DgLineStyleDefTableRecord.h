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
#ifndef __DG_LINESTYLEDEFTABLERECORD_H__
#define __DG_LINESTYLEDEFTABLERECORD_H__


#include "TD_PackPush.h"

#include "SharedPtr.h"
#include "OdBinaryData.h"
#include "UInt64Array.h"
#include "DgLineStyleTableRecord.h"

class OdDgLineStyleResource;
class OdGiLinetypeDash;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgLineStyleResource object pointers.
*/
typedef OdSmartPtr<OdDgLineStyleResource> OdDgLineStyleResourcePtr;

/** \details
    This class represents an abstract LineStyle resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLineStyleResource : public OdRxObject
{
public:

  enum OdLsResourceType
  {
    kLsUnknownRes       = -1,
    kLsInternalRes      = 0,
    kLsPointSymbolRes   = 1,
    kLsPointSymbolResV7 = 2,
    kLsCompoundRes      = 3,
    kLsLinePointRes     = 4,
    kLsLineCodeRes      = 5,
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgLineStyleResource);

  OdDgLineStyleResource() {}
  virtual ~OdDgLineStyleResource() {}

/** \details
  Reads resource data from a stream.

  \param pFiler   [in] Pointer to the data filer.
  \param dataSize [in] Data block size.
*/
  virtual void dgnInFields( OdDgFiler* pFiler, OdUInt32 dataSize ) = 0;

/** \details
  Writes resource data to a stream.

  \param pFiler   [in] Pointer to the data filer.
*/
  virtual void dgnOutFields( OdDgFiler* pFiler ) const = 0;

/** \details
  Returns the type of linestyle definition resource.
*/
  virtual OdUInt16 getType() const = 0;

/** \details
  Returns the size of the linestyle definition resource binary data.
*/
  virtual OdUInt16 getRawDataSize() const = 0;

/** \details
  Scales the linestyle resource.
*/
  virtual void scaleResource(double dScale) = 0;

  static OdDgLineStyleResourcePtr createObject(OdUInt16 type);
  static OdDgLineStyleResourcePtr createObject(OdUInt32 rscid);
};

/** \details
    This class represents a PointSymbol linestyle resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgPointSymbolResource : public OdDgLineStyleResource
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPointSymbolResource);

  OdDgPointSymbolResource();
  virtual ~OdDgPointSymbolResource();

/** \details
  Reads resource data from a stream.

  \param pFiler   [in] Pointer to the data filer.
  \param dataSize [in] Data block size.
*/
  virtual void dgnInFields( OdDgFiler* pFiler, OdUInt32 dataSize );

/** \details
  Writes resource data to a stream.

  \param pFiler   [in] Pointer to the data filer.
*/
  virtual void dgnOutFields( OdDgFiler* pFiler ) const;

/** \details
  Returns the type of linestyle definition resource.
*/
  virtual OdUInt16 getType() const;

/** \details
  Returns the size of linestyle definition resource binary data.
*/
  virtual OdUInt16 getRawDataSize() const;

/** \details
  Returns the description of a point resource.
*/
  OdString         getDescription() const;

/** \details
  Sets a description of a point resource.

  \param strDescription [in] String of the resource description.
 */
  void             setDescription( const OdString& strDescription );

/** \details
  Returns the point resource symbol offset from the symbol origin.
*/
  OdGePoint3d      getSymbolOffset()  const;

/** \details
  Sets a point resource symbol offset from the resource symbol origin.

  \param ptOffset [in] Offset of symbol resource.
*/
  void             setSymbolOffset( OdGePoint3d ptOffset );

/** \details
  Returns the point resource symbol scale.
*/
  double           getSymbolScale() const;

/** \details
  Sets a point resource symbol scale.

  \param dScale [in] Scale of symbol resource.
*/
  void             setSymbolScale( double dScale );

/** \details
  Returns the element ID handle of a dependent symbol cell header.
*/
  OdUInt64         getDependedCellHeaderHandle() const;

/** \details
  Sets a dependent element ID handle for a symbol resource.

  \param idHandle [in] Handle of the symbol resource.
*/
  void             setDependedCellHeaderHandle( OdUInt64 idHandle  );

/** \details
  Returns the dependent symbol extents.
*/
  OdGeExtents3d    getSymbolExtents() const;

/** \details
  Sets a dependent symbol extents.

  \param extSymbol [in] Dependent symbol extents.

  \remarks
    Symbol extents must be the same as the extents of the symbol cell header.
*/
  void             setSymbolExtents( const OdGeExtents3d& extSymbol );

/** \details
  Returns the binary data of a symbol cell header.

  \param symbolData [out] Binary data of the symbol cell header.

  \remarks
    The data is stored into a buffer only in RSC files. Otherwise the cell header 
    is stored in the linestyle definition table and its ID must be returned by getDependedCellHeaderHandle.
*/
  void             getSymbolBuffer( OdBinaryData& symbolData ) const;

/** \details
  Sets the binary data of a symbol cell header.

  \param symbolData [in] Binary data of the symbol cell header.
*/
  void             setSymbolBuffer( const OdBinaryData& symbolData );

/** \details
  Returns the 3D flag symbol cell header.

  \remarks
    Returns true if the dependent symbol is 3D.
*/
  bool             getSymbol3DFlag() const;

/** \details
  Sets the 3D flag of the symbol cell header.

  \param bSet [in] Value of the symbol 3D flag.

  \remarks
    The data is necessary ro restore a symbol from binary data. Must be the same as
    the 3D flag of the symbol cell header.
*/
  void             setSymbol3DFlag( bool bSet );

/** \details
  Returns the element ID of the symbol cell header.

  \param pDb           [in] Pointer to a DGN file database.
  \param symbolEntryId [in] Symbol entry ID from an RSC file.
  \param bV7Symbol     [in] DGN V7 flag.

  \remarks
    This function returns the element ID of a symbol header for internal linestyles or creates a new
    symbol from binary data of an RSC resource and returns the element ID of the symbol. symbolEntryId
    and bV7Symbol must be initialized if definition is from an RSC file.
*/
  OdDgElementId    getCell( OdDgDatabase* pDb, OdUInt32 symbolEntryId, bool bV7Symbol );

/** \details
  Scales a linestyle resource.
*/
  void             scaleResource(double dScale);

private:

  OdInt8       m_description[128];
  OdUInt64     m_reserved1;
  OdUInt32     m_auxType;
  OdUInt32     m_reserved2;
  OdGePoint3d  m_rangeLow;
  OdGePoint3d  m_rangeHigh;
  OdGePoint3d  m_offset;
  double       m_scale;
  double       m_reserved3[12];
  OdUInt32     m_symbolFlags;
  OdBinaryData m_symbolBuffer;

  OdUInt64     m_uCellHeaderId;

  OdGiLinetypeDash* m_pShapeDash;
  friend class OdDgLineStyleShaper;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgPointSymbolResource object pointers.
*/
typedef OdSmartPtr<OdDgPointSymbolResource> OdDgPointSymbolResourcePtr;


/** \details
    This class represents ComponentInfo of a compound linestyle resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgCompoundLineStyleComponentInfo
{
public:
  OdDgCompoundLineStyleComponentInfo();
  ~OdDgCompoundLineStyleComponentInfo();

  void dgnInFields( OdDgFiler* pFiler );
  void dgnOutFields( OdDgFiler* pFiler ) const;

public:

/** \details
  Returns the resource type of a compound linestyle component.
*/
  OdDgLineStyleResource::OdLsResourceType getComponentType() const;

/** \details
  Sets the resource type of a compound linestyle component.

  \param uType [in] Resource type of the compound linestyle component.
*/
  void                setComponentType( OdDgLineStyleResource::OdLsResourceType uType );

/** \details
  Returns the element ID handle of a linestyle definition.

  \remarks
    If the function returns 0, it is an RSC resource and it is necessary to use
    getComponentType() and getComponentEntryId() to get the resource.
*/
  OdUInt64            getComponentHandleId() const;

/** \details
  Sets the element ID handle of a linestyle definition.

  \param uComponentId [in] Element ID handle of the linestyle definition table record.

  \remarks
    uComponentId must not be zero only for linestyle definition records from the linestyle definition table.
*/
  void                setComponentHandleId( OdUInt64 uComponentId );

/** \details
  Returns the entry ID of an RSC linestyle definition.
*/
  OdUInt32            getComponentEntryId() const;

/** \details
  Sets the entry ID of an RSC linestyle definition.

  \param uEntryId [in] Entry ID of the RSC linestyle definition.
*/
  void                setComponentEntryId( OdUInt32 uEntryId );

/** \details
  Returns the vertical offset of a compound linestyle component.
*/
  double              getComponentOffset() const;

/** \details
  Sets the vertical offset of a compound linestyle component.

  \param dYOffset [in] Vertical offset of the compound linestyle component.
*/
  void                setComponentOffset( double dYOffset );

private:

  OdUInt32    m_rscId;
  OdUInt32    m_id;
  OdUInt32    m_reserved1[4];
  double      m_offset;
  OdUInt64    m_reserved2[3];

  OdUInt64    m_uComponentId;
};

/** \details
    This class represents a compound linestyle resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgCompoundLineStyleResource : public OdDgLineStyleResource
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCompoundLineStyleResource);

  OdDgCompoundLineStyleResource();
  ~OdDgCompoundLineStyleResource();

  /** \details
    Reads resource data from a stream.

    \param pFiler   [in] Pointer to the data filer.
    \param dataSize [in] Data block size.
  */
  virtual void dgnInFields( OdDgFiler* pFiler, OdUInt32 dataSize );

  /** \details
    Writes resource data to a stream.

    \param pFiler   [in] Pointer to the data filer.
  */
  virtual void dgnOutFields( OdDgFiler* pFiler ) const;

  /** \details
    Returns the type of the linestyle definition resource.
  */
  virtual OdUInt16 getType() const;

  /** \details
    Returns the size of the linestyle definition resource binary data.
  */
  virtual OdUInt16 getRawDataSize() const;

  /** \details
    Returns the description of a point resource.
  */
  OdString         getDescription() const;

  /** \details
    Sets the description of a point resource.

    \param strDescription [in] String of the resource description.
  */
  void             setDescription( const OdString& strDescription );

  /** \details
    Returns the number of components in a compound linestyle.
  */
  OdUInt32         getComponentCount() const;

  /** \details
    Gets a component of a compound linestyle.

    \param uIndex [in] Index of the component in the compound resource.
    \param component [out] Returned component data.

    \remarks
      Returns true if getting component data is successful.
  */
  bool             getComponent( OdUInt32 uIndex, OdDgCompoundLineStyleComponentInfo& component ) const;

  /** \details
    Sets a component of a compound linestyle.

    \param uIndex [in] Index of the componenet in the compound resource.
    \param component [in] Component data to set.

    \remarks
      Returns true if setting component data is successful.
  */
  bool             setComponent( OdUInt32 uIndex, const OdDgCompoundLineStyleComponentInfo& component );

  /** \details
    Adds a component to a compound linestyle.

    \param component [in] Component data to add.
  */
  void             addComponent( const OdDgCompoundLineStyleComponentInfo& component );

  /** \details
    Removes a component from a compound linestyle.

    \param uIndex [in] Index of the componenet to remove from the compound resource.

    \remarks
      Returns true if the component data is deleted successfully.
  */
  bool             deleteComponent( OdUInt32 uIndex );

  /** \details
    Scales a linestyle resource.
  */
  void             scaleResource(double dScale);

private:

  OdInt8      m_description[128];
  OdUInt64    m_reserved1;
  OdUInt32    m_auxType;
  OdUInt32    m_reserved2[8];
  OdArray<OdDgCompoundLineStyleComponentInfo> m_components;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgCompoundLineStyleResource object pointers.
*/
typedef OdSmartPtr<OdDgCompoundLineStyleResource> OdDgCompoundLineStyleResourcePtr;


/** \details
    This class represents StrokeData of a LineCode linestyle resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLineCodeResourceStrokeData
{
public:

  enum OdLsStrokeWidthMode
  {
    kLsNoWidth    = 0,
    kLsWidthLeft  = 1,
    kLsWidthRight = 2,
    kLsWidthFull  = 3
  };

  enum OdLsStrokeCapsType
  {
    kLsCapsClosed   = 0,
    kLsCapsOpen     = 1,
    kLsCapsExtended = 2,
    kLsCapsHexagon  = 3,
    kLsCapsOctagon  = 4,
    kLsCapsDecagon  = 5,
    kLsCapsArc      = 30
  };

public:
  OdDgLineCodeResourceStrokeData();
  ~OdDgLineCodeResourceStrokeData();

  /** \details
    Reads resource data from a stream.

    \param pFiler   [in] Pointer to a data filer.
    \param dataSize [in] Data block size.
  */
  void dgnInFields( OdDgFiler* pFiler );

  /** \details
    Writes resource data to a stream.

    \param pFiler   [in] Pointer to a data filer.
  */
  void dgnOutFields( OdDgFiler* pFiler ) const;

  /** \details
    Returns the length of a stroke.
  */
  double getLength() const;

  /** \details
    Sets the length of a stroke.

    \param dLength [in] Length of stroke.
  */
  void   setLength( double dLength );

  /** \details
    Returns the start width of a stroke.
  */
  double getStartWidth() const;

  /** \details
    Sets the start width of a stroke.

    \param dWidth [in] Width value in units.
  */
  void   setStartWidth(  double dWidth  );

  /** \details
    Returns the end width of a stroke.
  */
  double getEndWidth() const;

  /** \details
    Sets the end width of a stroke.

    \param dWidth [in] Width value in units.
  */
  void   setEndWidth(  double dWidth  );

  /** \details
    Returns the dash flag of a stroke.

    \remarks
      If true, the stroke is dashed, otherwise the stroke is a gap.
  */
  bool   getDashFlag() const;

  /** \details
    Sets the dash flag of a stroke.

    \param bSet [in] Set to true for a dash, false for a gap.
  */
  void   setDashFlag( bool bSet );

  /** \details
    Returns the bypass corner flag of a stroke.

    \remarks
      If false, the stroke can be broken at element vertices. 
      Otherwise it runs through the corner.
  */
  bool   getByPassCornerFlag() const;

  /** \details
    Sets the bypass corner flag of a stroke.

    \param bSet [in] Value of the flag.
  */
  void   setByPassCornerFlag( bool bSet );

  /** \details
    Returns the can-be-scaled flag of a stroke.

    \remarks
      If true, the stroke can be scaled by the linetype scale. 
      Otherwise the stroke bypasses the vertex and continues 
      straight to the nearest point on the element.
  */
  bool   getCanBeScaledFlag() const;

  /** \details
    Sets the can-be-scaled flag of a stroke.

    \param bSet [in] Value of the flag.
  */
  void   setCanBeScaledFlag( bool bSet );

  /** \details
    Returns the InvertStrokeInFirstCode flag of a stroke.

    \remarks
      If true, the stroke is inverted in the first repetition of 
      the stroke pattern (at the origin of the element).
  */
  bool   getInvertStrokeInFirstCodeFlag() const;

  /** \details
    Sets the InvertStrokeInFirstCode flag of a stroke.

    \param bSet [in] Value of flag.
  */
  void   setInvertStrokeInFirstCodeFlag( bool bSet );

  /** \details
    Returns the InvertStrokeInLastCode flag of a stroke.

    \remarks
      If true, the stroke is inverted in the last repetition 
      of the stroke pattern (at the end of the element).
  */
  bool   getInvertStrokeInLastCodeFlag() const;

  /** \details
    Sets the InvertStrokeInLastCode flag of a stroke.

    \param bSet [in] Value of flag.
  */
  void   setInvertStrokeInLastCodeFlag( bool bSet );

  /** \details
    Returns the width mode of a stroke.

    \remarks
      Values:
        kLsNoWidth     Width settings are ignored. The stroke is displayed without width.
        kLsWidthFull   The stroke is displayed with the specified widths.
        kLsWidthLeft   Only the left half of the stroke is displayed with the specified widths.
        kLsWidthRight  Only the right half of the stroke is displayed with the specified widths.
  */
  OdLsStrokeWidthMode getWidthMode() const;

  /** \details
    Sets the width mode of a stroke.

    \param iMode [in] Width mode value.
  */
  void   setWidthMode( OdLsStrokeWidthMode iMode );

  /** \details
    Returns the increasing taper flag of a stroke.

    \remarks
      The increasing taper flag allows you to set the endWidth > startWidth. If it is false
      and endWidth > startWidth, then endWidth = startWidth.
  */
  bool   getIncreasingTaperFlag() const;

  /** \details
    Sets the increasing taper flag of a stroke.

    \param bSet [in] Value of flag.
  */
  void   setIncreasingTaperFlag( bool bSet );

  /** \details
   Returns the decreasing taper flag of a stroke.

   \remarks
     The decreasing taper flag allows you to set the endWidth < startWidth. If it is false
     and endWidth < startWidth, then endWidth = startWidth.
  */
  bool   getDecreasingTaperFlag() const;

  /** \details
    Sets the decreasing taper flag of a stroke.

    \param bSet [in] Value of flag.
  */
  void   setDecreasingTaperFlag( bool bSet );

  /** \details
    Returns the start and end caps type of a stroke.
  */
  OdLsStrokeCapsType getCapsType() const;

  /** \details
    Sets the start and end caps type of a stroke.

    \param iType [in] Value of caps type.
  */
  void   setCapsType( OdLsStrokeCapsType iType );

  /** \details
    Scales stroke data.
  */
  void   scale(double dScale);

private:

  double      m_length;
  double      m_width;
  double      m_endWidth;
  OdUInt32    m_reserved1[5];
  OdUInt8     m_strokeMode;
  OdUInt8     m_widthMode;
  OdUInt8     m_capMode;
  OdUInt8     m_reserved2;
};

/** \details
    This class represents a LineCode linestyle resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLineCodeResource : public OdDgLineStyleResource
{
public:
  ODRX_DECLARE_MEMBERS(OdDgLineCodeResource);

  OdDgLineCodeResource();
  virtual ~OdDgLineCodeResource();

  /** \details
    Reads resource data from a stream.

    \param pFiler   [in] Pointer to the data filer.
    \param dataSize [in] Data block size.
  */
  virtual void dgnInFields( OdDgFiler* pFiler, OdUInt32 dataSize );

  /** \details
    Writes resource data to a stream.

    \param pFiler   [in] Pointer to the data filer.
  */
  virtual void dgnOutFields( OdDgFiler* pFiler ) const;

  /** \details
    Returns the type of linestyle definition resource.
  */
  virtual OdUInt16 getType() const;

  /** \details
    Returns the size of linestyle definition resource binary data.
  */
  virtual OdUInt16 getRawDataSize() const;

  /** \details
    Returns the description of a point resource.
  */
  OdString         getDescription() const;

  /** \details
    Sets the description of a point resource.

    \param strDescription [in] String of the resource description.
  */
  void             setDescription( const OdString& strDescription );

  /** \details
    Returns the fraction or shift value of a stroke resource.

    \remarks
      If the auto phase flag is set, the value is a fraction, otherwise 
      the value is a shift distance.
      - Shift is an offset of the stroke pattern in units relative to the 
      start of the line or line segment.
      - Fraction is an offset of the stroke pattern in a percent of the pattern 
      length relative to the start of the line or line segment.
      - The value is ignored if the center stretch phase mode is on.
  */
  double           getPhase() const;

  /** \details
    Sets the fraction or shift value of a stroke resource.

    \param dPhase [in] Value of the shift in units or value of the fraction.
  */
  void             setPhase( double dPhase );

  /** \details
    Returns the phase mode of a stroke resource.

    \remarks
      If true, the phase value is a fraction, otherwise the phase value is a shift.
  */
  bool             getAutoPhaseFlag() const;

  /** \details
    Sets the phase mode of a stroke resource.

    \param bSet [in] True for fraction mode and false for phase mode.
  */
  void             setAutoPhaseFlag( bool bSet );

  /** \details
    Returns the iteration limit flag of a stroke resource.

    \remarks
      If true, a limit is used, otherwise there is no limit to the iteration for the pattern.
  */
  bool             getUseIterationLimitFlag() const;

  /** \details
    Sets the iteration limit flag of a stroke resource.

    \param bSet [in] Value of flag.
   */
  void             setUseIterationLimitFlag( bool bSet );

  /** \details
    Returns the single segment mode flag of a stroke resource.

    \remarks
      If true, the pattern is restarted for every segment, otherwise
      the pattern continues to be drawn on the next segment.
  */
  bool             getSingleSegmentModeFlag() const;

  /** \details
    Sets the iteration single segment mode flag of a stroke resource.

    \param bSet [in] Value of flag.
  */
  void             setSingleSegmentModeFlag( bool bSet );

  /** \details
    Returns the center stretch phase mode flag of a stroke resource.

    \remarks
      If true, the center of the pattern is the same as the center of 
      the line or line segment. Shift and fraction values are ignored.
  */
  bool             getCenterStretchPhaseModeFlag() const;

  /** \details
    Sets the center stretch phase mode flag of a stroke resource.

    \param bSet [in] Value of flag.
  */
  void             setCenterStretchPhaseModeFlag( bool bSet );

  /** \details
    Returns the iteration limit of a stroke resource.
  */
  OdUInt32         getMaxIterations() const;

  /** \details
    Sets the iteration limit of a stroke resource.

    \param iMaxNum [in] Iteration limit value.
  */
  void             setMaxIterations( OdUInt32 iMaxNum );

  /** \details
    Returns the number of strokes in a stroke resource.
  */
  OdUInt32         getStrokeCount() const;

  /** \details
    Returns a stroke by index.

    \param uIndex [in] Index of the stroke.
    \param strokeData [out] Returned stroke data value.

    \remarks
      Returns true if getting the stroke data is successful.
  */
  bool             getStroke( OdUInt32 uIndex, OdDgLineCodeResourceStrokeData& strokeData ) const;

  /** \details
    Sets a stroke by index.

    \param uIndex [in] Index of stroke.
    \param strokeData [in] Stroke data value to set.

    \remarks
      Returns true if setting the stroke data value is successful.
  */
  bool             setStroke( OdUInt32 uIndex, const OdDgLineCodeResourceStrokeData& strokeData );

  /** \details
    Adds a stroke by index.

    \param strokeData [in] Stroke data value to add.
  */
  void             addStroke( const OdDgLineCodeResourceStrokeData& strokeData );

  /** \details
    Removes a stroke by index.

    \param uIndex [in] Index of the stroke.

    \remarks
      Returns true if the stroke data value is deleted successfully.
  */
  bool             deleteStroke( OdUInt32 uIndex );

  /** \details
    Scales a linestyle resource.
  */
  void             scaleResource(double dScale);

private:

  OdInt8      m_description[128];
  OdUInt64    m_reserved1;
  OdUInt32    m_auxType;
  OdUInt32    m_reserved2;
  double      m_phase;
  double      m_reserved3[5];
  OdUInt32    m_options;
  OdUInt32    m_maxIterate;
  OdUInt32    m_reserved4;
  OdArray<OdDgLineCodeResourceStrokeData> m_strokes;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgLineCodeResource object pointers.
*/
typedef OdSmartPtr<OdDgLineCodeResource> OdDgLineCodeResourcePtr;


/** \details
    This class represents SymInfo of a LinePoint linestyle resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLinePointResourceSymInfo
{
public:

  enum OdLsSymbolPosOnStroke
  {
    kLsNoSymbol         = 0,
    kLsAtOriginOfStroke = 1,
    kLsAtEndOfStroke    = 2,
    kLsAtCenterOfStroke = 3
  };

public:
  OdDgLinePointResourceSymInfo();
  ~OdDgLinePointResourceSymInfo();

  /** \details
    Reads resource data from a stream.

    \param pFiler   [in] Pointer to the data filer.
    \param dataSize [in] Data block size.
  */
  void dgnInFields( OdDgFiler* pFiler );

  /** \details
    Writes resource data to a stream.

    \param pFiler   [in] Pointer to the data filer.
  */
  void dgnOutFields( OdDgFiler* pFiler ) const;

  /** \details
    Returns the symbol type of a symbol resource.

    \remarks
      Returns kLsPointSymbolRes for DGN V8 symbols and
      kLsPointSymbolResV7 for DGN V7 symbols. Used only for
      linestyle definitions from an RSC file.
  */
  OdDgLineStyleResource::OdLsResourceType getSymbolType() const;

  /** \details
    Sets the symbol type of a symbol resource.

    \param iType [in] Symbol type of the symbol resource.
  */
  void                setSymbolType( OdDgLineStyleResource::OdLsResourceType iType );

  /** \details
    Returns the element ID handle of a point symbol resource.

    \remarks
      Returns 0 for an RSC symbol resource and non-zero for a point symbol
      resource from a linestyle definition table of a file.
  */
  OdUInt64            getPointSymbolHandleId() const;

  /** \details
    Sets the element ID handle of a point symbol resource.

    \param idPtSymbol [in] Element ID handle of the point symbol resource.
  */
  void                setPointSymbolHandleId( OdUInt64 idPtSymbol );

  /** \details
    Returns the entry ID of a point symbol resource from an RSC file.
  */
  OdUInt32            getPointSymbolEntryId() const;

  /** \details
    Sets the entry ID of a point symbol resource from an RSC file.

    \param idPtSymbol [in] Entry ID of the point symbol resource.
  */
  void                setPointSymbolEntryId( OdUInt32 idPtSymbol );

  /** \details
    Returns the index of a stroke on a base pattern where the symbol is drawn.
  */
  OdUInt16            getSymbolStrokeNo() const;

  /** \details
    Sets the index of a stroke on the base pattern where the symbol is drawn.

    \param uStrokeNo [in] Stroke index value.
  */
  void                setSymbolStrokeNo( OdUInt16 uStrokeNo );

  /** \details
    Returns the symbol origin position on a stroke.

    \remarks
      Values:
        - kLsNoSymbol - Skips symbol draw.
        - kLsAtOriginOfStroke - Symbol at the start stroke.
        - kLsAtEndOfStroke    - Symbol at the end stroke.
        - kLsAtCenterOfStroke    - Symbol at the center stroke.
  */
  OdLsSymbolPosOnStroke getSymbolPosOnStroke() const;

  /** \details
    Sets the symbol origin position on a stroke.

    \param iMode [in] Symbol position on a stroke mode.
  */
  void                setSymbolPosOnStroke( OdLsSymbolPosOnStroke iMode );

  /** \details
    Returns the SymbolAtElementOrigin flag.

    \remarks
      If true, the symbol displays one time at the start point of the element or segment.
  */
  bool                getSymbolAtElementOriginFlag() const;

  /** \details
    Sets the SymbolAtElementOrigin flag.

    \param bSet [in] Value of the flag.
  */
  void                setSymbolAtElementOriginFlag( bool bSet );

  /** \details
    Returns the SymbolAtElementEnd flag.

    \remarks
      If true, the symbol displays one time at the end point of the element or segment.
  */
  bool                getSymbolAtElementEndFlag() const;

  /** \details
    Sets the SymbolAtElementEnd flag.

    \param bSet [in] Value of the flag.
  */
  void                setSymbolAtElementEndFlag( bool bSet );

  /** \details
    Returns the SymbolAtEachVertex flag.

    \remarks
      If true, the symbol displays on each vertex position of the element.
  */
  bool                getSymbolAtEachVertexFlag() const;

  /** \details
    Sets the SymbolAtEachVertex flag.

    \param bSet [in] Value of the flag.
  */
  void                setSymbolAtEachVertexFlag( bool bSet );

  /** \details
    Returns the MirrorSymbolForReversedLines flag.

    \remarks
      A reversed line direction is the opposite direction from the x-axis (or the y-axis 
      if the direction is perpendicular to the x-axis, or the z-axis if the direction is parallel 
      to the z-axis). If true, the mirror transformation is applied to symbols
      if the direction of the line is reversed.
  */
  bool                getMirrorSymbolForReversedLinesFlag() const;

  /** \details
    Sets the MirrorSymbolForReversedLines flag.

    \param bSet [in] Value of the flag.
  */
  void                setMirrorSymbolForReversedLinesFlag( bool bSet );

  /** \details
    Returns the AbsoluteRotationAngle flag.

    \remarks
      If true, the symbol rotation doesn't depend on the segment direction.
  */
  bool                getAbsoluteRotationAngleFlag() const;

  /** \details
    Sets the AbsoluteRotationAngle flag.

    \param bSet [in] Value of the flag.
  */
  void                setAbsoluteRotationAngleFlag( bool bSet );

  /** \details
    Returns the DoNotScaleElement flag.

    \remarks
      If true, the symbol doesn't scale by the linetype scale.
  */
  bool                getDoNotScaleElementFlag() const;

  /** \details
    Sets the DoNotScaleElement flag.

    \param bSet [in] Value of the flag.
  */
  void                setDoNotScaleElementFlag( bool bSet );

  /** \details
    Returns the DoNotClipElement flag.

    \remarks
      If true, the symbol is not clipped at the end of the element.
  */
  bool                getDoNotClipElementFlag() const;

  /** \details
    Sets the DoNotClipElement flag.

    \param bSet [in] Value of the flag.
  */
  void                setDoNotClipElementFlag( bool bSet );

  /** \details
    Returns the NoPartialStrokes flag.

    \remarks
      If true, the skip symbol is drawn on partial strokes.
  */
  bool                getNoPartialStrokesFlag() const;

  /** \details
    Sets the NoPartialStrokes flag.

    \param bSet [in] Value of the flag.
  */
  void                setNoPartialStrokesFlag( bool bSet );

  /** \details
    Returns the PartialOriginBeyondEnd flag.

    \remarks
      If true, the symbol origin is beyond the end of partial strokes.
  */
  bool                getPartialOriginBeyondEndFlag() const;

  /** \details
    Sets the PartialOriginBeyondEnd flag.

    \param bSet [in] Value of the flag.
  */
  void                setPartialOriginBeyondEndFlag( bool bSet );

  /** \details
    Returns the UseSymbolColor flag.

    \remarks
      If true, the color from the symbol definition is used to draw the symbol.
      Otherwise the element color is used.
  */
  bool                getUseSymbolColorFlag() const;

  /** \details
    Sets the UseSymbolColor flag.

    \param bSet [in] Value of the flag.
  */
  void                setUseSymbolColorFlag( bool bSet );

  /** \details
    Returns the UseSymbolWeight flag.

    \remarks
      If true, the lineweight from the symbol definition is used to draw the symbol.
      Otherwise the element lineweight is used.
  */
  bool                getUseSymbolWeightFlag() const;

  /** \details
    Sets the UseSymbolWeight flag.

    \param bSet [in] Value of the flag.
  */
  void                setUseSymbolWeightFlag( bool bSet );

  /** \details
    Returns the offset of a symbol from the origin point.
  */
  OdGePoint2d         getOffset() const;

  /** \details
    Sets the offset of a symbol from the origin point.

    \param ptOffset [in] 2D offset of the symbol from the origin point.
  */
  void                setOffset( OdGePoint2d ptOffset );

  /** \details
    Returns the rotation of a symbol from the origin point.
  */
  double              getRotationAngle() const;

  /** \details
    Sets the rotation of a symbol from the origin point.

    \param dRotAngle [in] Rotation angle of the symbol from the origin point.
  */
  void                setRotationAngle( double dRotAngle );

/** \details
  Scales point information resource data.
*/
  void                scale(double dScale);

private:

  OdUInt32  m_symType;
  OdUInt32  m_symID;
  OdUInt16  m_strokeNo;
  OdUInt16  m_mod1;
  OdUInt32  m_reserved1;
  double    m_xOffset;
  double    m_yOffset;
  double    m_reserved2[3];
  double    m_zAngle;
  double    m_reserved3[3];

  OdUInt64  m_uPtSymbolId;
};

/** \details
    This class represents a LinePoint linestyle resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLinePointResource : public OdDgLineStyleResource
{
public:
  ODRX_DECLARE_MEMBERS(OdDgLinePointResource);

  OdDgLinePointResource();
  virtual ~OdDgLinePointResource();

  /** \details
    Reads resource data from a stream.

    \param pFiler   [in] Pointer to the data filer.
    \param dataSize [in] Data block size.
  */
  virtual void dgnInFields( OdDgFiler* pFiler, OdUInt32 dataSize );

  /** \details
    Writes resource data to a stream.

    \param pFiler   [in] Pointer to the data filer.
  */
  virtual void dgnOutFields( OdDgFiler* pFiler ) const;

/** \details
   Returns the type of linestyle definition resource.
 */
  virtual OdUInt16 getType() const;

  /** \details
    Returns the size of linestyle definition resource binary data.
  */
  virtual OdUInt16 getRawDataSize() const;

  /** \details
    Returns the description of a point resource.
  */
  OdString         getDescription() const;

  /** \details
    Sets the description of a point resource.

    \param strDescription [in] String of the resource description.
  */
  void             setDescription( const OdString& strDescription );

  /** \details
    Returns the resource type of a base pattern stroke definition.

    \remarks
      Must be kLsLineCodeRes.
  */
  OdDgLineStyleResource::OdLsResourceType getBasePatternType() const;

  /** \details
    Sets the resource type of a base pattern stroke definition.

    \param iType [in] Resource type of the base pattern stroke definition.
  */
  void             setBasePatternType( OdDgLineStyleResource::OdLsResourceType iType );

  /** \details
    Returns the element ID handle of a base pattern stroke definition.

    \remarks
      Returns 0 for a stroke definition from an RSC file and non-zero for a stroke
      definition from a linestyle definition table of a file.
  */
  OdUInt64         getBasePatternHandleId() const;

  /** \details
    Sets the element ID handle of a base pattern stroke definition.

    \param idBasePattern [in] Element ID handle of the base pattern stroke definition.
  */
  void             setBasePatternHandleId( OdUInt64 idBasePattern );

  /** \details
    Returns the entry ID of a base pattern stroke definition from an RSC file.
  */
  OdUInt32         getBasePatternEntryId() const;

  /** \details
    Sets the entry ID of a base pattern stroke definition from an RSC file.

    \param idEntry [in] Entry ID of the base pattern stroke definition.
  */
  void             setBasePatternEntryId( OdUInt32 idEntry );

  /** \details
    Returns the number of symbols.
  */
  OdUInt32         getSymbolCount() const;

  /** \details
    Gets symbol information.

    \param uIndex [in] Index of the symbol.
    \param symbolInfo [out] Returned symbol information.

    \remarks
      Returns true if getting symbol information is successful.
  */
  bool             getSymbol( OdUInt32 uIndex, OdDgLinePointResourceSymInfo& symbolInfo ) const;

  /** \details
    Sets symbol information.

    \param uIndex [in] Index of the symbol.
    \param symbolInfo [in] Symbol information to set.

    \remarks
      Returns true if setting symbol information is successful.
  */
  bool             setSymbol( OdUInt32 uIndex, const OdDgLinePointResourceSymInfo& symbolInfo );

  /** \details
    Adds symbol information.

    \param symbolInfo [in] Symbol information to add.
  */
  void             addSymbol( const OdDgLinePointResourceSymInfo& symbolInfo );

  /** \details
    Deletes symbol information.

    \param uIndex [in] Index of symbol.

    \remarks
      Returns true if deleting symbol information is successful.
  */
  bool             deleteSymbol( OdUInt32 uIndex );

  /** \details
    Scales line point resource data.
  */
  void             scaleResource(double dScale);

private:

  OdInt8      m_description[128];
  OdUInt64    m_reserved1;
  OdUInt32    m_auxType;
  OdUInt32    m_reserved2;
  double      m_reserved3;
  OdUInt32    m_lcType;
  OdUInt32    m_lcID;
  OdUInt32    m_reserved4;
  OdArray<OdDgLinePointResourceSymInfo> m_symbols;

  OdUInt64    m_uBaseId;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgLinePointResource object pointers.
*/
typedef OdSmartPtr<OdDgLinePointResource> OdDgLinePointResourcePtr;


class OdDgLineStyleDefTableRecord;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgLineStyleDefTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgLineStyleDefTableRecord> OdDgLineStyleDefTableRecordPtr;

/** \details
    This class represents LineStyle records in the OdDgLineStyleDefTableRecord of an OdDgDatabase instance.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLineStyleDefTableRecord : public OdDgTableRecord
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLineStyleDefTableRecord);
public:

  /** \details
    Creates a linestyle definition table record.

    \param type [in] Type of resource.
  */
  static OdDgLineStyleDefTableRecordPtr createObject( OdDg::LineStyleType type );

  /** \details
    Returns the resource type of a linestyle definition table record.
  */
  OdDg::LineStyleType getType() const;

  /** \details
    Sets the resource type of a linestyle definition table record.

    \param val [in] Type of resource.
  */
  void setType(OdDg::LineStyleType val);

  /** \details
    Returns a smart pointer to the resource of a linestyle definition table record.
  */
  OdDgLineStyleResourcePtr getResource();
};

#include "TD_PackPop.h"

#endif // __DG_LINESTYLEDEFTABLERECORD_H__
