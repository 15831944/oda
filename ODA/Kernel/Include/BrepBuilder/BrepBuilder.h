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

#ifndef _BREPBUILDER_H_
#define _BREPBUILDER_H_

#include "OdaCommon.h"
#include "BrepBuilderExport.h"
#include "Ge/GeCurve3d.h"
#include "CmColorBase.h"
#include "OdResult.h"
#include "TD_PackPush.h"

class OdDbStub;
class OdGiMapper;

/** \details
  Defines B-Rep object types.

  <group OdBrepBuilder_Classes>
*/
enum BrepType
{
  /** The output geometry is an open shell. */
  kOpenShell,
  /** The output geometry is a solid. */
  kSolid,
  /** The output geometry is a void (i.e., an inverted solid). */
  kVoid
};

typedef OdUInt32 BRepBuilderGeometryId;

/** \details
  BrepBuilderErrorsHolder contains information about errors found during
  validation.

  <group OdBrepBuilder_Classes>
*/
struct BREPBUILDEREXPORT BrepBuilderErrorsHolder
{
public:
  static const BRepBuilderGeometryId kInvalidId = BRepBuilderGeometryId(-1);

  /** \details
    Creates a new instance of a BrepBuilderErrorsHolder object with default
    parameters.
  */
  BrepBuilderErrorsHolder()
    : m_errorCode(::eOk)
    , m_complexId(kInvalidId)
    , m_shellId(kInvalidId)
    , m_faceId(kInvalidId)
    , m_loopId(kInvalidId)
    , m_edgeId(kInvalidId)
    , m_coedgeId(kInvalidId)
    , m_errorMessage("")
  {};

  /** \details
    Creates a new instance of a BrepBuilderErrorsHolder object with given
    parameters.

    \param errCode   [in] Error code.
    \param errMsg    [in] Error message.
    \param complexId [in] Complex ID. Optional parameter.
    \param shellId   [in] Shell ID. Optional parameter.
    \param faceId    [in] Face ID. Optional parameter.
    \param loopId    [in] Loop ID. Optional parameter.
    \param edgeId    [in] Edge ID. Optional parameter.
    \param coedgeId  [in] Coedge ID. Optional parameter.
  */
  explicit BrepBuilderErrorsHolder(OdResult errCode, const OdString &errMsg = "",
    BRepBuilderGeometryId complexId = kInvalidId,
    BRepBuilderGeometryId shellId = kInvalidId,
    BRepBuilderGeometryId faceId = kInvalidId,
    BRepBuilderGeometryId loopId = kInvalidId,
    BRepBuilderGeometryId edgeId = kInvalidId,
    BRepBuilderGeometryId coedgeId = kInvalidId)
    : m_errorCode(errCode)
    , m_complexId(complexId)
    , m_shellId(shellId)
    , m_faceId(faceId)
    , m_loopId(loopId)
    , m_edgeId(edgeId)
    , m_coedgeId(coedgeId)
    , m_errorMessage(errMsg)
  {};

  /** Error code. */
  OdResult m_errorCode;
  /** Complex ID. */
  BRepBuilderGeometryId m_complexId;
  /** Shell ID. */
  BRepBuilderGeometryId m_shellId;
  /** Face ID. */
  BRepBuilderGeometryId m_faceId;
  /** Loop ID. */
  BRepBuilderGeometryId m_loopId;
  /** Edge ID. */
  BRepBuilderGeometryId m_edgeId;
  /** Coedge ID. */
  BRepBuilderGeometryId m_coedgeId;
  /** Error message. */
  OdString m_errorMessage;
};

/** \details
  This template class is a specialization of the OdArray class for
  BrepBuilderErrorsHolder objects.
*/
typedef OdArray <BrepBuilderErrorsHolder> ValidationErrors;


/** \details
  This is the class that allows direct construction of geometry objects (solids,
  surfaces or open shells).

  <group OdBrepBuilder_Classes>
*/
class BREPBUILDEREXPORT OdBrepBuilder
{
public:
  /** Default Shell ID. */
  BREPBUILDEREXPORT_STATIC static const BRepBuilderGeometryId kDefaultShellId;
  /** Default Vertex ID. */
  BREPBUILDEREXPORT_STATIC static const BRepBuilderGeometryId kDefaultVertexId;

  /** \details
    Creates a new instance of the OdBrepBuilder object with default parameters.
  */
  OdBrepBuilder();

  /** \details
    Destructor. Destroys the OdBrepBuilder object.
  */
  ~OdBrepBuilder();

  /** \details
    Defines the entity direction in relation to the parent entity.
  */
  enum EntityDirection
  {
    /** The direction of the entity is the same as the direction of the parent entity. */
    kForward,
    /** The direction of the entity is opposite to the direction of the parent entity. */
    kReversed
  };

  /** \details
    Returns the type of the output geometry.
    \returns Type of the output geometry.
  */
  BrepType getType();

  /** \details
    Sets the product-dependent implementation.

    \param pFile [in] Pointer to the product-dependent implementation.
  */
  void set(const void* pFile);

  /** \details
    Adds a coedge associated to a previously added edge.
    A coedge represents the use of an edge on one of the edge's faces.

    \param loopId         [in] ID of the loop which the added code belongs to.
    \param edgeId         [in] ID of the edge.
    \param codgeDirection [in] Coedge direction in relation to the edge.
    \param pParCur        [in] Pointer to a paramectic curve. Optional parameter.
    \returns BRepBuilderGeometryId of the added coedge.

    \remarks
    OdError(eInvalidInput) exception is thrown if the type of the parametric
    curve (pParCur) is not supported by the BrepBuilder implementation.
  */
  BRepBuilderGeometryId addCoedge(const BRepBuilderGeometryId& loopId, const BRepBuilderGeometryId& edgeId, EntityDirection codgeDirection = kForward, const OdGeCurve2d* pParCur = NULL);

  /** \details
    Adds a new edge to the geometry being built.

    \param pCurveForEdge [in] Pointer to the edge geometry (object-space curve).
    \returns BRepBuilderGeometryId of the added edge.
  */
  BRepBuilderGeometryId addEdge(const OdGeCurve3d* pCurveForEdge);
  
  /** \details
    Adds a new edge to the geometry being built.

    \param pCurveForEdge [in] Pointer to the edge geometry (object-space curve).
    \param vertex1Id     [in] ID of the first vertex.
    \param vertex2Id     [in] ID of the second vertex.
    \returns BRepBuilderGeometryId of the added edge.
  */
  BRepBuilderGeometryId addEdge(const OdGeCurve3d* pCurveForEdge, const BRepBuilderGeometryId& vertex1Id, const BRepBuilderGeometryId& vertex2Id);

  /** \details
    Adds a new vertex to the geometry being built.

    \param point [in] 3D point.
    \returns BRepBuilderGeometryId of the added vertex.
  */
  BRepBuilderGeometryId addVertex(const OdGePoint3d& point);

  /** \details
    Creates an empty face in the geometry being built.

    \param pSurf         [in] Pointer to the underlying surface.
    \param faceDirection [in] Face's orientation in relation to the underlying surface.
    \param shellId       [in] Shell ID.
    \returns BRepBuilderGeometryId of the added face.
  */
  BRepBuilderGeometryId addFace(const OdGeSurface* pSurf, EntityDirection faceDirection, const BRepBuilderGeometryId& shellId = kDefaultShellId);

  /** \details
    Creates an empty loop in a given face of the geometry being built.

    \param faceId [in] Face ID.
    \returns BRepBuilderGeometryId of the added face.
  */
  BRepBuilderGeometryId addLoop(const BRepBuilderGeometryId& faceId);

  /** \details
    Creates an empty shell.

    \param complexId [in] Complex ID.
    \returns BRepBuilderGeometryId of the added face.
  */
  BRepBuilderGeometryId addShell(const BRepBuilderGeometryId& complexId);

  /** \details
    Creates an empty complex &mdash; the upper topology element.

    \returns BRepBuilderGeometryId of the added complex.
  */
  BRepBuilderGeometryId addComplex();

  /** \details
    Allows BRepBuilder to remove problematic faces (e.g. due to inaccurate edge
    geometry). If this option is enabled and BRepBuilder removes some faces, the
    output geometry's type will be OpenShell regardless of the expected type,
    specified when the BRepBuilder was created.

    \remarks
    Currently not implemented.
  */
  void allowRemovalOfProblematicFaces();

  /** \details
    A validator function that checks the state of this BRepBuilder object.

    \returns True if this BRepBuilder object is accepting B-Rep data, false
    otherwise.
    \remarks
    Currently not implemented.
  */
  bool canAddGeometry();

  /** \details
    Indicates that the caller has finished defining the given complex.

    \param complexId [in] Complex ID.
  */
  void finishComplex(const BRepBuilderGeometryId& complexId);

  /** \details
    Indicates that the caller has finished defining the given shell.

    \param shellId [in] Shell ID.
  */
  void finishShell(const BRepBuilderGeometryId& shellId);

  /** \details
    Indicates that the caller has finished defining the given face.

    \param faceId [in] Face ID.
  */
  void finishFace(const BRepBuilderGeometryId& faceId);

  /** \details
    Indicates that the caller has finished defining the given loop.

    \param loopId [in] Loop ID.
  */
  void finishLoop(const BRepBuilderGeometryId& loopId);

  /** \details
    Gets the geometry object built by this BRepBuilder. This method clears the
    built geometry stored in the BRepBuilder.
    This function will throw an error if this BRepBuilderGeometryId hasn't
    completed building the B-Rep. Use IsResultAvailable() to verify whether this
    BRepBuilder contains a valid result.
    Built entity is specific for each product-dependent implementation.

    \returns Pointer to the built geometry object.
    \remarks
    Currently not implemented.
  */
  OdRxObjectPtr getResult();

  /** \details
    A validator function that checks whether the surface object is of the type
    that is supported as a face surface by BRepBuilder.

    \param pSurf [in] Pointer to the surface object.
    \returns True if the product-dependent implementation of the B-Rep builder
    can use this surface; false otherwise.

    \remarks
    Not implemented by default, but the product-dependent implementation of the
    B-Rep builder can have their own implemetnations of this method.
  */
  bool isPermittedSurfaceType(const OdGeSurface* pSurf);

  /** \details
    A validator function that checks the state of this BRepBuilder object.

    \returns True if this BRepBuilder object has successfully built a B-Rep;
    false otherwise.
    \remarks
    Currently not implemented.
  */
  bool isResultAvailable();

  /** \details
    A validator function that checks whether the edge ID corresponds to the edge
    previously added to this BRepBuilder object.

    \param edgeId [in] Edge ID.
    \returns True if the edge with the specified ID has been already added;
    false otherwise.
  */
  bool isValidEdgeId(const BRepBuilderGeometryId& edgeId);

  /** \details
    A validator function that checks whether the complex ID corresponds to the
    complex previously added to this BRepBuilder object.

    \param complexId [in] Complex ID.
    \returns True if the complex with the specified ID has been already added;
    false otherwise.
  */
  bool isValidComplexId(const BRepBuilderGeometryId& complexId);

  /** \details
    A validator function that checks whether the shell ID corresponds to the
    shell previously added to this BRepBuilder object.

    \param shellId [in] Shell ID.
    \returns True if the shell with the specified ID has been already added;
    false otherwise.
  */
  bool isValidShellId(const BRepBuilderGeometryId& shellId);

  /** \details
    A validator function that checks whether the face ID corresponds to the face
    previously added to this BRepBuilder object.

    \param faceId [in] Face ID.
    \returns True if the face with the specified ID has been already added;
    false otherwise.
  */
  bool isValidFaceId(const BRepBuilderGeometryId& faceId);

  /** \details
    A validator function that checks whether the loop ID corresponds to the loop
    previously added to this BRepBuilder object.

    \param loopId [in] Loop ID.
    \returns True if the loop with the specified ID has been already added;
    false otherwise.
  */
  bool isValidLoopId(const BRepBuilderGeometryId& loopId);

  /** \details
    This function removes  some problematic faces from the output geometry.

    \returns True if BRepBuilder removed some problematic faces, false if not.
    \remarks
    If allowRemovalOfProblematicFaces() was not called to enable removal of
    problematic faces, this function will return 'false'. Note that if some
    faces were removed, the output geometry's type will be OpenShell regardless
    of the expected type that was specified when the BRepBuilder was created.
    Currently not implemented.
  */
  bool removedSomeFaces();

  /** \details
    Makes BRepBuilder allow edges that it would normally disallow as being too
    short for geometry.

    \remarks
    Currently not implemented.
  */
  void setAllowShortEdges();

  /** \details
    Sets a specified bit flag on an object.

    \param id     [in] Entity ID.
    \param flag   [in] Bit flag from the BrEntityFlags enumeration.
    \param enable [in] True to enable/set the flag, false to disable/remove the flag.

    \sa
    BrEntityFlags enumeration.
  */
  void setFlag(const BRepBuilderGeometryId &id, OdUInt32 flag, bool enable);

  /** \details
    Sets the material ID to the specified face.

    \param faceId     [in] Face ID.
    \param materialId [in] Material ID.
  */
  void setFacesMaterial(const BRepBuilderGeometryId &faceId, OdDbStub &materialId);

  /** \details
    Sets the material mapper for the face.

    \param faceId         [in] Face ID.
    \param materialMapper [in] Material mapper.
    \returns eOk if the operation is successful; eNotApplicable if it is not
    supported by the current implementation or the material mapping is part of
    the material.
  */
  OdResult setFaceMaterialMapping(const BRepBuilderGeometryId &faceId,
    const OdGiMapper& materialMapper);

  /** \details
    Sets the edge color.

    \param edgeId    [in] Edge ID.
    \param edgeColor [in] Edge color.
    \returns eOk if the operation is successful; eNotApplicable if this function
    is not supported by the current implementation; eInvalidInput if the color
    method is not supported by the current implementation.
  */
  OdResult setEdgeColor(const BRepBuilderGeometryId& edgeId, const OdCmEntityColor& edgeColor);

  /** \details
    Sets the face color.

    \param faceId    [in] Face ID.
    \param faceColor [in] Face color.
    \returns eOk if the operation is successful; eNotApplicable if this function
    is not supported by the current implementation or the face color is part of
    the material.
  */
  OdResult setFaceColor(const BRepBuilderGeometryId& faceId, const OdCmEntityColor& faceColor);

  /** \details
    Completes construction of the geometry. The geometry is validated and, if
    valid, stored in this Builder.

    \returns Pointer to the built entity, that is specific for each
    product-dependent implementation.
  */
  OdRxObjectPtr finish();

  /** \details
    Sets the initial tag value that is used for faces and edges.

    \param indTag [in] Tag ID.
  */
  void setInitialTag(OdUInt32 indTag = 0);

  /** \details
    Sets the tag value that is used for faces and edges. MODE: kSetTagMode.
    Use setTag() just after addEdge() and addFace() calls.

    \param id  [in] B-Rep builder entity ID.
    \param tag [in] Tag value.

    \remarks
    kSetTagMode and kAutoMode are incompatible. You cannot use methods setTag()
    and setInitialTag() at the same time.
  */
  void setTag(const BRepBuilderGeometryId &id, OdUInt32 tag);

  /** \details
    Enables or disables validation in BRepBuilder.

    \param bEnable [in] True to enable validation; false to disable validation.
  */
  void enableValidator(bool bEnable = true);

  /** \details
    Returns errors found during validation.

    \returns Pointer to the ValidationErrors object that stores errors
    encountered during the validation process.
  */
  const ValidationErrors *getValidationErrors() const;

  /** \details
    Checks the state of the product-dependent implementation.

    \returns True if the product-dependent implementation isn't NULL.
  */
  bool isValid() const;

protected:
  //DOM-IGNORE-BEGIN
    void* m_pImpl;
  //DOM-IGNORE-END
};

#include "TD_PackPop.h"

#endif //_BREPBUILDER_H_
