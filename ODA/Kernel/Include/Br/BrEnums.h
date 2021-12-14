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



// OdBrEnums.h -- declaration of enumerated types used in the
//                OdBr B-Rep and traverser classes.

#pragma warning ( disable : 4482 )  

#ifndef ODBRENUMS_H
#define ODBRENUMS_H
#include "OdError.h"
#include "BrExport.h"
#include "Gi/Gi.h"

/** \details
  Represents the B-rep error status.
*/
enum OdBrErrorStatus
{
  /** The operation is finished successfully. */
  odbrOK                     = ::eOk,
  /** The type of returned object is not supported as a B-rep. */
  odbrWrongObjectType        = eWrongObjectType,
  /** The OdBr* object is not initialized or is invalid. */
  odbrInvalidObject          = eUnrecoverableErrors,
  odbrUnsuitableTopology     = 0XBC5,
  /** Represents the partially or fully missing geometry. */
  odbrMissingGeometry        = eDegenerateGeometry,
  /** The input arguments point to an invalid object. */
  odbrInvalidInput           = eInvalidInput,
  /** The subentity does not map to the topology. */
  odbrDegenerateTopology     = 0XBCC,
  /** The OdBr* object is not initialized. */
  odbrUninitialisedObject    = 0xBCD,
  /** The memory for the object could not be allocated. */
  odbrOutOfMemory            = eOutOfMemory,
  /** The object has been modified since this OdBr* object was last set. */
  odbrBrepChanged            = 0xBC0,
  /** Returned if the function is not implemented yet. */
  odbrNotImplementedYet      = ::eNotImplementedYet,
  /** Returned when the subentity path does not point to an object. */
  odbrNullObjectId           = eNullObjectId,
  /** Represents the situation when the inherited function is not applicable to this subclass. */
  odbrNotApplicable          = eNotApplicable,
  /** The subentity type does not match the subclass. */
  odbrWrongSubentityType     = eWrongSubentityType,
  /** Returned when the subentity path does not point to a subentity. */
  odbrNullSubentityId        = eInvalidIndex,
  /** Returned when the function implementation is missing. */
  odbrNullObjectPointer      = eUnrecoverableErrors,
  /** The traverser list owner and list position do not point to the same object. */
  odbrObjectIdMismatch       = eWrongDatabase,
  /** Returned when the traverser list position cannot be set because the subentity is not connected to the list owner. */
  odbrTopologyMismatch       = eWrongDatabase,
  /** The geometry of OdGe object is unsuitable for this function. */
  odbrUnsuitableGeometry     = eAmbiguousOutput,
  /** The topology does not map to a subentity. */
  odbrMissingSubentity       = eNotInDatabase,
  /** The result is ambiguous. */
  odbrAmbiguousOutput        = eAmbiguousOutput,

  odbrUnrecoverableErrors    = eUnrecoverableErrors,
  odbrMissingTopology        = odbrDegenerateTopology,
  odbrWrongDatabase          = eWrongDatabase,
  odbrNotInDatabase          = eNotInDatabase,
  odbrDegenerateGeometry     = eDegenerateGeometry
};

/** \details
  Represents the type of a loop.
*/
enum BrLoopType{
  /** The loop type is ambiguous or cannot be determined at this time. */
  odbrLoopUnclassified = 0,
  /** The loop is on a peripheral boundary. */
  odbrLoopExterior     = 1,
  /** The loop represents a hole in the interior of a face. */
  odbrLoopInterior     = 2,
  /** The loop is winding on a conical surface. */
  odbrLoopWinding      = 3
};

/** \details
  Represents the shell type.
*/
enum BrShellType{
  /** The shell type cannot be determined at this time. */
  odbrShellUnclassified = 0,
  /** The shell is on a peripheral boundary of region or B-rep. */
  odbrShellExterior     = 1,
  /** The shell is empty in the interior of a region or B-rep. */
  odbrShellInterior     = 2
};

/** \details
  Represents the validation level.
*/
enum BrValidationLevel {
  /** Full validation. */
  odbrFullValidation    = 0,
  /** No validation. */
  odbrNoValidation      = 1
};

/** \details
  Represents the element shape criteria.
*/
enum Element2dShape { 
  /** Default. */
  kDefault           = 0,
  /** All polygons. */
  kAllPolygons       = 1,
  /** All quadrilaterals. */
  kAllQuadrilaterals = 2,
  /** All triangles. */
  kAllTriangles      = 3
};

/** \details
  Represents bit flags for BrEntity.
*/
enum BrEntityFlags {
  /** No flags. */
  kNoFlags         = 0,
  /** If it is DoubleSide then not SingleSide and vice versa. */
  kDoubleSide      = 0x001,
  /** Visible should be set by default. kNoFlags is default/drawings version. */
  kVisible         = 0x002,
  /** Invisible. */
  kInvisible       = 0x004,
  /** Highlight. */
  kHighlight       = 0x008,
  /** It cannot be selected. */
  kSelectionIgnore = 0x010,
  /** Internal use only. */
  kBimRvEdgeSwapFaces = 0x01000000,
};

/** \details
  This class is a specialization of the OdError class for OdBr errors.
  Corresponding C++ library: TD_Br
  <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrException : public OdError
{
public:
  /** \details
    Constructor.

    \param errorStatus [in]  Error status.
  */
  OdBrException(OdBrErrorStatus errorStatus);

  /** \details
    Returns the error status associated with this Exception object.
  */
  OdBrErrorStatus getErrorStatus() const;
};

#define BR_THROW(n) throw OdBrException(n)

#endif // ODBRENUMS_H
