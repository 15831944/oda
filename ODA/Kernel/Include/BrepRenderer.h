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



#ifndef _BREPRENDERER_H_
#define _BREPRENDERER_H_

#include "Wr/wrExport.h"
#include "Wr/wrTriangulationParams.h"
#include "BrepRenderModes.h"

class OdGiCommonDraw ;
class OdGiViewportDraw;
class OdBrBrep;
class OdGeMatrix3d;

/** \details
  Corresponding C++ library: TD_BrepRenderer

  <group TD_Namespaces>
*/
namespace GeMesh { class OdGeTrMesh; }
class OdGeExtents3d;
class OdBrFace;
class OdGePoint3d;

/** \details
  This class is the base class for classes that provide platform-dependent ACIS
  rendering for the ODA Platform.

  Corresponding C++ library: TD_BrepRenderer

  <group Other_Classes>
*/
class wrRenderFilter
{
public:
  /** \details
    Filters the specified face.

    \param face [in] Face to filter.

    \returns
    True if the specified face matches filter criteria; false otherwise.
  */
  virtual bool filterFace(const OdBrFace &face) = 0;

  /** \details
    This is the default destructor for objects of the wrRenderFilter class.
  */
  virtual ~wrRenderFilter() {}
};

/** \details
  This class is the base class for classes that provide platform-dependent ACIS
  rendering for the ODA Platform.

  Corresponding C++ library: TD_BrepRenderer

  <group Other_Classes>
*/
class RENDER_EXPORT OdBrepRenderer
{
protected:
  //DOM-IGNORE-BEGIN
  OdBrepRenderer() {}
  //DOM-IGNORE-END

public:
  /** \details
    This is the default destructor for objects of the OdBrepRenderer class.
  */
  virtual ~OdBrepRenderer() {}

  /** \details
    Sets the BREP data for this Renderer object.
    \param brep [in] BREP data.
  */
  virtual void setBrep(const OdBrBrep& brep) = 0;

  /** \details
    Enables caching for this Renderer object.
  */
  virtual void enableCaching() = 0;

  /** \details
    Disables caching for this Renderer object.
    \param clearCache [in] Clears rendering cache if and only if true.
  */
  virtual void disableCaching(bool clearCache = true) = 0;

  /** \details
    Returns true if and only if caching is enabled for this Renderer object.
    \returns True if caching is enabled for this Renderer object; false
    otherwise.
  */
  virtual bool isCachingEnabled() const = 0;

  /** \details
    Sets the Triangular Parameters for this Renderer object.
    \param pTriangulationParams [in] Pointer to the Triangular Parameters.
  */
  virtual void setTriangulationParams(const wrTriangulationParams *pTriangulationParams) = 0;

  /** \details
    Returns the Triangular Parameters for this Renderer object.
    \returns Triangular Parameters for this Renderer object.
  */
  virtual const wrTriangulationParams * getTriangulationParams() const = 0;

  /** \details
    This structure stores the number of isolines.
  */
  struct NumIsolines
  {
    /** U-isolines number */
    OdUInt32 numU,
    /** V-isolines number */
             numV;
  };

  /** \details
    Creates a geometric representation of the BREP data of this Renderer object.

    \param pWd          [in] Pointer to the OdGiCommonDraw interface.
    \param geomType     [in] Geometry type.
    \param pNumIsolines [in] Number of isolines.

    \returns
    True if and only if the geometric representation can be generated.

    \remarks
    The value of the geomType parameter must be a combination of one or more of
    the following:

    <table>
    Name             Value
    kNothing         0
    kIsolines        1
    kEdges           2
    kShells          4
    kOrderedEdges    8
    kSurfIsolines    16
    </table>
  */
  virtual bool draw(OdGiCommonDraw * pWd, OdUInt32 geomType, const NumIsolines *pNumIsolines = 0) = 0;

  /** \details
    Creates a silhouette representation of the BREP data of this Renderer
    object.

    \param pVd [in] Pointer to the OdGiViewportDraw interface.

    \returns
    True if and only if the silhouette representation can be generated.
  */
  virtual bool drawSilhouettes(OdGiViewportDraw* pVd) = 0;

  /** \details
    Applies the 3D transformation matrix to the BREP data of Renderer object.

    \param xfm [in] 3D transformation matrix.
  */
  virtual void transformBy(const OdGeMatrix3d &xfm) = 0;

  /** \details
    Creates a geometric representation of the particular face of this Renderer
    object.

    \param pWd                 [in] Pointer to the OdGiCommonDraw interface.
    \param iFace               [in] Face marker.
    \param triangulationParams [in] Triangulation Parameters.

    \returns
    True if and only if the geometric representation can be generated.
  */
  virtual bool drawFace(OdGiCommonDraw * pWd, OdGsMarker iFace, const wrTriangulationParams& triangulationParams) = 0;

  /** \details
    Gets the mesh of the particular face of this Renderer object.

    \param mesh               [out] Resulting mesh.
    \param iFace               [in] Face marker.
    \param triangulationParams [in] Triangulation Parameters.

    \returns
    True if successful; false otherwise.
  */
  virtual bool getFaceMesh(GeMesh::OdGeTrMesh& mesh, OdGsMarker iFace, const wrTriangulationParams& triangulationParams) = 0;

  /** \details
    Gets the extents the geometric representation of this Renderer object.

    \param extents [out] Resulting extents.
    \param nIso     [in] Number of isolines.

    \returns
    True if successful; false otherwise.
  */
  virtual bool getExtents(OdGeExtents3d& extents, OdUInt32 nIso = 6) { return false; }

  /** \details
    Sets the face filter for this Renderer object.

    \param pFaceFilter [in] Face filter.
  */
  virtual void setFaceFilter(wrRenderFilter* pFaceFilter) = 0;

  /** \details
    Gets the point for calculating the deviation in perspective mode.

    \param pt [out] Resulting point.

    \returns
    True if successful; false otherwise.
  */
  virtual bool getPtSurface(OdGePoint3d& pt) { return false; } // get the point for calculating deviation in perspective mode
};

#endif //_BREPRENDERER_H_
