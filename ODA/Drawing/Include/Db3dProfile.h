/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2020, Open Design Alliance (the "Alliance"). 
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

#ifndef   _DB3DPROFILE_H_INCLUDED_
#define   _DB3DPROFILE_H_INCLUDED_

#include "RxObject.h"
#include "TD_PackPush.h"
#include "DbGeomRef.h"

class OdDb3dProfile;
class OdDb3dProfileData;

typedef OdSmartPtr<OdDb3dProfile> OdDb3dProfilePtr;
typedef OdSmartPtr<OdDb3dProfileData> OdDb3dProfileDataPtr;

/** \details
  <group OdDb_Classes>

  This class is used to represent a 3d profile that can be used as input to
  functions such as createExtrudedSurface(), createRevolvedSurface(), and
  createSweptSurface(). An object of this class can represent an entity
  such as a curve, region, surface, face, trace, solid, or it can represent an edge or a set of edges
  that form a connected chain.
  
*/
class DBENT_EXPORT OdDb3dProfile : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDb3dProfile);
  ODRX_HEAP_OPERATORS();

  /** \details
    Creates a new instance of a 3d profile with default parameters.
  */
  OdDb3dProfile();

  /** \details
    Creates a new instance of a 3d profile with OdDbEntity.

    \param pEntity [in]  Entity.
  */
  OdDb3dProfile(OdDbEntity* pEntity);

  /** \details
    Creates a new instance of a 3d profile with OdDbFullSubentPath.

    \param faceSubentPath [in]  Subent path of the face.
  */
  OdDb3dProfile(const OdDbFullSubentPath& faceSubentPath);

  /** \details
    Creates a new instance of a 3d profile with OdDbFullSubentPath.

    \param faceSubentPath [in]  Subent path of the face.
    \returns created OdSmartPtr<OdDb3dProfile>.
  */
  static OdSmartPtr<OdDb3dProfile> createObject(const OdDbFullSubentPath& faceSubentPath);

  /** \details
    Creates a new instance of a 3d profile with OdDbVertexRef.

    \param vertexRef [in]  Reference to the vertex.
  */
  OdDb3dProfile(const OdDbVertexRef& vertexRef);

  /** \details
    Creates a new instance of a 3d profile with OdDb3dProfile.

    \param src [in]  Source object to be copied.
  */
  OdDb3dProfile(const OdDb3dProfile& src);

  /** \details
    Creates a new instance of a 3d profile with OdDbPathRef.

    \param pathRef [in]: Reference to the path
  */
  OdDb3dProfile(const OdDbPathRef& pathRef);

  /** \details
    Destroy instance of 3d profile.
  */
  virtual ~OdDb3dProfile();

  /** \details
    Initializes this reference using a given object.

    \param src [in] Pointer to the src.
  */
  void copyFrom(const OdRxObject* src);

  /** \details
    Returns the profile entity.  If the profile is for an edge or set of
    edges then the returned entity will be an AcDbCurve of the edge or edges.
    The caller should not delete this pointer or post it to the database.

    \returns Profile entity. The caller should not delete this pointer.
  */
  OdDbEntity* entity() const;

  /** \details
    Sets a new instance of a 3d profile with OdDbEntity.

    \param pEntity [in] Pointer to the entity.
    \returns eOk if successful.
  */
  OdResult set(OdDbEntity* pEntity);

  /** \details
    Sets a new instance of a 3d profile with OdDbVertexRef.

    \param vertexRef [in]  Reference to the vertex.
    \returns eOk if successful.
  */
  OdResult set(const OdDbVertexRef& vertexRef);

  /** \details
    Returns the vertex reference of the vertex if the profile is a vertex.
    If the profile is not an edge or set of edges, vertexRef will be
    empty

    \param vertexRef [in]  Reference to the vertex.
    \returns eOk if successful.
  */
  OdResult getVertexRef(OdDbVertexRef& vertexRef) const;

  /** \details
    Sets a new instance of 3d profile to a path

    \param pathRef [in]  Reference to the path.
    \returns eOk if successful.
  */
  OdResult set(const OdDbPathRef& pathRef);

  /** \details
    Initializes this reference using a given object.

    \param src [in]  Source 3d profile.
    \returns resulting 3d profile.
  */
  OdDb3dProfile& operator =(const OdDb3dProfile& src);

  /** \details
    Checks whether the profile forms a closed loop.

    \param tol [in]  Tolerance.
    \returns true if profile is closed, false otherwise.
  */
  bool isClosed(OdGeTol tol = OdGeTol()) const;

  /** \details
    Checks whether the profile is planar.

    \returns true if profile is planar, false otherwise.
  */
  bool isPlanar() const;

  /** \details
    Checks whether the profile is an edge or face.

    \returns true if profile is an edge or face, false otherwise.
  */
  bool isSubent() const;

  /** \details
    Checks whether the profile is a face.

    \returns true if profile is a face, false otherwise.
  */
  bool isFace() const;

  /** \details
    Checks whether the profile is an edge.

    \returns true if profile is an edge, false otherwise.
  */
  bool isEdge() const;

  /** \details
    Checks whether the profile is valid and may be used to create a surface.

    \returns true if profile is a valid entity, edge, or face.
  */
  bool isValid() const;

  /** \details
    Returns the path reference of the edge if the profile is an edge or
    set of edges.  If the profile is not an edge or set of edges, pathRef will be
    empty

    \param pathRef [out] path reference of the edge(s).

    \returns eOk if successful.
  */
  OdResult getPathRef(OdDbPathRef& pathRef) const;

  /** \details
    Merges connected sets of profiles into one or more profiles

    \param profileArr [in]  Input array of profiles that are to be merged.
    \param mergeEdges [in]  If this parameter is false then any edge profiles in the input array will not be merged and will be copied directly to the ouput array.
    \param mergeCurves [in] If this parameter is false then any curve profiles in the input array will not be merged and will be copied directly to the ouput array.
    \param mergedProfileArr [out]  Output array of merged profiles. The caller is responsible for deleting all objects that are returned in this array.
    \returns eOk if successful.
  */
  static OdResult mergeProfiles(const OdArray<OdDb3dProfile*>& profileArr,
    bool mergeEdges, bool mergeCurves,
    OdArray<OdDb3dProfile*>& mergedProfileArr);

  /** \details
    Converts path array into new profiles, that are of a type that can be used for
    extrude, revolve, sweep, or loft operations.

    \param aPath [in]  Input path array.
    \returns resulting 3d profile.
  */
  OdDb3dProfile* convertProfile(const OdArray<OdDbPathRef>& aPath);

  /** \details
    Converts profile into new profiles that are of a type that can be used for
    extrude, revolve, sweep, or loft operations.  The converted profiles are returned in
    the output array.  If this profile's type is AcDbTrace, AcDbSolid, or AcDbFace then
    the converted profile will be for a region.  If this profile's type is
    AcDb3dPolyline and the profile is linear or whose 'Fit' property is quadratic or
    cubic then the converted profile will be a line or a spline.

    \param explodeMultiFaceRegions [in] If this profile's type is AcDbSurface or AcDbRegion and has more than one face, then we explode to get multiple surface or region profiles that each have only one face.
    \param convertSurfaceToEdges [in] If this profile is a face or is of type AcDbSurface, AcDbRegion, AcDbTrace, AcDbSolid, or AcDbFace then we convert to an edge profile that is defined by the surface or region edges.
    \param nonPlanarOnly [in] This parameter is only relevant when convertSurfaceToEdges is set to true. If this parameter is true then only non-planar surfaces will be converted to edge profiles.  Regions and planar surfaces will not be converted.
    \param outerLoopOnly [in] This parameter is only relevant when convertSurfaceToEdges is set to true. If this parameter is true then only the outer loop of the surface or region will be converted to an edge profile.
    \param convertedProfileArr [out] Output list of converted profiles. The caller is responsible for deleting the objects in this list.
    \returns eOk if successful. Even when this function returns eOk, it is possible for the output list to be empty depending on the profile type and the values of the other parameters.
  */
  OdResult convertProfile(bool explodeMultiFaceRegions, bool convertSurfaceToEdges,
    bool nonPlanarOnly, bool outerLoopOnly,
    OdArray<OdDb3dProfile*>& convertedProfileArr) const;

private:
  void setEntity(OdDbEntity* pEntity);

  OdDb3dProfileDataPtr m_pProfileData;

  friend class OdDb3dProfileData;
};

#include "TD_PackPop.h"
#endif  // _DB3DPROFILE_H_INCLUDED_
