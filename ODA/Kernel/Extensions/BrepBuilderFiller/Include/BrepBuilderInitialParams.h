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

#pragma once

#include "BrepBuilderFillerDef.h"

#include "DbBaseDatabase.h"


/** \details
  Corresponding C++ library: TD_BrepBuilderFiller
  <group Extension_Classes>
*/
class BREPBUILDERFILLER_DLL OdBrepBuilderFillerParams
{
public:
  /** \details
    Defines B-Rep topology types.
  */
  enum BrepType
  {
    /** Unknown B-Rep type. */
    kBrepUnknown = 0,
    /** Acis DGN B-Rep. */
    kBrepAcisDgn,
    /** Acis DWG B-Rep. */
    kBrepAcisDwg,
    /** BimRv B-Rep. */
    kBrepBimRv,
    /** Ifc B-Rep. */
    kBrepIfc,
    /** Internal B-Rep. */
    kBrepMd,
    /** Prc B-Rep. */
    kBrepPrc,
    /** Parasolid B-Rep. */
    kBrepPS,
    /** Visualize B-Rep. */
    kBrepVisualize,
  };

private:
  mutable OdDbBaseDatabasePtr m_pSourceDb;
  mutable OdDbBaseDatabasePtr m_pDestinationDb;
  BrepType m_sourceBrepType;
  BrepType m_destinationBrepType;

  //DOM-IGNORE-BEGIN
  enum FillerOptions
  {
    optsDefault = 0,
    optsSkipNullSurface = 1 << 0,
    optsSkipCoedge2dCurve = 1 << 1,
    optsCheckLoopType = 1 << 2,
    optsGenerateExplicitLoops = 1 << 3,
    optsMakeEllipMajorGreaterMinor = 1 << 4,
    optsIgnoreComplexShell = 1 << 5,
    optsGenerateVertices = 1 << 6,
    optsMake2dIntervalInclude3d = 1 << 7,
  };
  void resetOptions();
  //DOM-IGNORE-END

  OdUInt32 m_options;

public:
  /** \details
    Creates a new instance of a OdBrepBuilderFiller parameters.
  */
  OdBrepBuilderFillerParams()
    : m_sourceBrepType(kBrepUnknown)
    , m_destinationBrepType(kBrepUnknown)
    , m_options(optsDefault)
  {}

  /** \details
    Sets a flag to skip a face which can not return Ge-surface.

    \param val [in] Flag value to set.
    \returns Reference to this.
  */
  OdBrepBuilderFillerParams& setSkipNullSurface(bool val) {
    SETBIT(m_options, optsSkipNullSurface, val);
    return *this;
  }
  //DOM-IGNORE-BEGIN
  bool isSkipNullSurface() const {
    return (m_options & optsSkipNullSurface) != 0;
  }
  //DOM-IGNORE-END

  /** \details
    Sets a flag to ignore all coedge parametric curves.

    \param val [in] Flag value to set.
    \returns Reference to this.
  */
  OdBrepBuilderFillerParams& setSkipCoedge2dCurve(bool val) {
    SETBIT(m_options, optsSkipCoedge2dCurve, val);
    return *this;
  }
  //DOM-IGNORE-BEGIN
  bool isSkipCoedge2dCurve() const {
    return (m_options & optsSkipCoedge2dCurve) != 0;
  }
  //DOM-IGNORE-END

  /** \details
    Sets a flag to ignore the number of outer loops.

    \param val [in] Flag value to set.
    \returns Reference to this.
    \remarks
    This option is useful for ODA BimRv to acis conversion.
  */
  OdBrepBuilderFillerParams& setSkipCheckLoopType(bool val) {
    SETBIT(m_options, optsCheckLoopType, !val);
    return *this;
  }
  //DOM-IGNORE-BEGIN
  bool isSkipCheckLoopType() const {
    return (m_options & optsCheckLoopType) == 0;
  }
  //DOM-IGNORE-END

  /** \details
    Sets a flag to add explicit loops to faces that have no loops.

    \param val [in] Flag value to set.
    \returns Reference to this.
    \remarks
    Exceptions: full periodic surfaces such as sphere and torus.
  */
  OdBrepBuilderFillerParams& setGenerateExplicitLoops(bool val) {
    SETBIT(m_options, optsGenerateExplicitLoops, val);
    return *this;
  }
  //DOM-IGNORE-BEGIN
  bool isGenerateExplicitLoops() const { 
    return (m_options & optsGenerateExplicitLoops) != 0;
  }
  //DOM-IGNORE-END

  /** \details
    Sets a flag to make major radius grater then minor radius of OdGeEllipArc3d, OdGeEllipCone, and OdGeEllipCylinder.

    \param val [in] Flag value to set.
    \returns Reference to this.
  */
  OdBrepBuilderFillerParams& setMakeEllipMajorGreaterMinor(bool val) {
    SETBIT(m_options, optsMakeEllipMajorGreaterMinor, val);
    return *this;
  }
  //DOM-IGNORE-BEGIN
  bool isMakeEllipMajorGreaterMinor() const {
    return (m_options & optsMakeEllipMajorGreaterMinor) != 0;
  }
  //DOM-IGNORE-END

  /** \details
    Sets a flag to add all faces to OdBrepBuilder::kDefaultShellId shell.

    \param val [in] Flag value to set.
    \returns Reference to this.
  */
  OdBrepBuilderFillerParams& setIgnoreComplexShell(bool val) {
    SETBIT(m_options, optsIgnoreComplexShell, val);
    return *this;
  }
  //DOM-IGNORE-BEGIN
  bool isIgnoreComplexShell() const {
    return (m_options & optsIgnoreComplexShell) != 0;
  }
  //DOM-IGNORE-END

  /** \details
    Sets a flag to read and store vertices from B-Rep.

    \param val [in] Flag value to set.
    \returns Reference to this.
  */
  OdBrepBuilderFillerParams& setGenerateVertices(bool val) {
    SETBIT(m_options, optsGenerateVertices, val);
    return *this; 
  }
  //DOM-IGNORE-BEGIN
  bool isGenerateVertices() const {
    return (m_options & optsGenerateVertices) != 0;
  }
  //DOM-IGNORE-END

  /** \details
    Sets a flag to make coedge curve interval include interval of edge curve.

    \param val [in] Flag value to set.
    \returns Reference to this.
  */
  OdBrepBuilderFillerParams& setMake2dIntervalInclude3d(bool val) {
    SETBIT(m_options, optsMake2dIntervalInclude3d, val);
    return *this;
  }
  //DOM-IGNORE-BEGIN
  bool isMake2dIntervalInclude3d() const {
    return (m_options & optsMake2dIntervalInclude3d) != 0;
  }
  //DOM-IGNORE-END

  /** \details
    Sets source and destination B-Rep Databases and initialize flags specific for conversion.

    \param pSourceDb      [in] Source database.
    \param pDestinationDb [in] Destination database.
    \returns Reference to this.
    \remark
    Beware: This method reset options and should be called before all set* methods.
  */
  OdBrepBuilderFillerParams& setupFor(OdDbBaseDatabase* pSourceDb, OdDbBaseDatabase* pDestinationDb);
  /** \details
    Sets source and destination B-Rep Databases and initialize flags specific for conversion.

    \param sourceBrepType [in] Source B-Rep type.
    \param pSourceDb      [in] Source database.
    \param pDestinationDb [in] Destination database.
    \returns Reference to this.
    \remark
    Beware: This method reset options and should be called before all set* methods.
    This method is useful for conversion from Dgn where B-Rep can be Acis or Parasolid format.
  */
  OdBrepBuilderFillerParams& setupFor(BrepType sourceBrepType, OdDbBaseDatabase* pSourceDb, OdDbBaseDatabase* pDestinationDb);
  /** \details
    Sets source and destination B-Rep Databases and initialize flags specific for conversion.

    \param sourceBrepType [in] Source B-Rep type.
    \param pDestinationDb [in] Destination database.
    \returns Reference to this.
    \remark
    Beware: This method reset options and should be called before all set* methods.
  */
  OdBrepBuilderFillerParams& setupFor(BrepType sourceBrepType, OdDbBaseDatabase* pDestinationDb);
  /** \details
    Sets source and destination B-Rep Databases and initialize flags specific for conversion.

    \param pSourceDb           [in] Source database.
    \param destinationBrepType [in] Destination B-Rep type.
    \returns Reference to this.
    \remark
    Beware: This method reset options and should be called before all set* methods.
  */
  OdBrepBuilderFillerParams& setupFor(OdDbBaseDatabase* pSourceDb, BrepType destinationBrepType);
  /** \details
    Sets source and destination B-Rep Databases and initialize flags specific for conversion.

    \param sourceBrepType      [in] Source B-Rep type.
    \param destinationBrepType [in] Destination B-Rep type.
    \returns Reference to this.
    \remark
    Beware: This method reset options and should be called before all set* methods.
  */
  OdBrepBuilderFillerParams& setupFor(BrepType sourceBrepType, BrepType destinationBrepType);

  /** \details
    Gets a source B-Rep database.

    \returns The source B-Rep database.
  */
  OdDbBaseDatabase* sourceDb() const {
    return m_pSourceDb;
  }
  /** \details
    Gets a source B-Rep type.

    \returns The source B-Rep type.
  */
  BrepType sourceBrepType() const {
    return m_sourceBrepType;
  }
  /** \details
    Gets a destination B-Rep database.

    \returns The destination B-Rep database.
  */
  OdDbBaseDatabase* destinationDb() const {
    return m_pDestinationDb;
  }
  /** \details
    Gets a destination B-Rep type.

    \returns The destination B-Rep type.
  */
  BrepType destinationBrepType() const {
    return m_destinationBrepType;
  }
};
