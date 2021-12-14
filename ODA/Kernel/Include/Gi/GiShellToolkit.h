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
#ifndef ODGI_SHELLTOOLKIT_H_INCLUDED
#define ODGI_SHELLTOOLKIT_H_INCLUDED

#include "OdVector.h"
#include "Ge/GeExtents3d.h"
#include "GiGeometry.h"

/** \details
This interface describes OdGiShellToolkit.
*/
class ODGI_EXPORT OdGiShellToolkit : public OdRxObject
{
public:
  /** \details
  This class describes base face orientation callback.
  */
  class ODGI_EXPORT OdGiShellFaceOrientationCallback
  {
  public:
    /** \details
    Checks face orientation.
    \param pts [in] Array of vertices.
    \param face [in] Face described as [numVertices][VertexIndex_0]...[VertexIndex_numVertices-1].
    \returns Returns true if face orientation is correct; otherwise returns false.
    \remarks
    Default implementation always returns true and can be used to check that all faces oriened in the same way.
    */
    virtual bool isFaceOrientedCorrectly( const OdGePoint3d* pts, const OdInt32* face ) const
    {
      return true;
    }
  };
  /** \details
  This structure describes original face.
  */
  struct ODGI_EXPORT OdGiShellOriginalFaceDescription
  {
    /** Number of called OdGiShellToolkit::addShell(). */
    OdUInt32 nShell;
    /** Face offset in original data. */
    OdUInt32 nOffset;
  };
  /** \details
  This structure describes shell toolkit face.
  */
  struct ODGI_EXPORT OdGiShellFaceDescription
  {
    /** Face offset OdGiShellToolkit::faceList(). */
    OdUInt32 nOffset;
  };
  /** \details
  This structure describes shell toolkit edge.
  */
  struct ODGI_EXPORT OdGiShellEdgeDescription
  {
    /** Face description. */
    OdGiShellFaceDescription face;
    /** Face edge index. */
    OdUInt32 nEdge;
  };
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS( OdGiShellToolkit );
//DOM-IGNORE-END
  /** \details 
  Creates instance of shell toolkit implementation.
  \returns returns smart pointer to the toolkit implementation.
  */
  static OdSmartPtr<OdGiShellToolkit> create();
  /** \details
  Addes shell to the toolkit.
  \param nPoints [in] Number of vertices.
  \param pPoints [in] Vertices array.
  \param nFaces [in] Size of face list.
  \param pFaces [in] Face list, each face described by [numPoints][Point0][Point1]...
  \param bUnifyVertices [in] Flag that indicates that vertices with the same coordinates should be combined in one.
  \remarks
  This method unifies only points from specified shell and no not change previous data.
  */
  virtual void addShell( OdUInt32 nPoints, const OdGePoint3d* pPoints, OdUInt32 nFaces, const OdInt32* pFaces, bool bUnifyVertices = false ) = 0;
  /** \details
  Clears toolkit.
  */
  virtual void clear() = 0;
  /** \details
  Checks vertices duplication.
  \returns Returns true if and only if shell contains vertices with the same coordinates.
  */
  virtual bool hasDupilcateVertices() const = 0;
  /** \details
  Checks sharp edges.
  \param bOnlyCount [in] Flag that specify should toolkit only check number of connected faces or should it compare faces orientation.
  \returns Returns number of sharp edges.
  */
  virtual OdUInt32 numSharpEdges( bool bOnlyCount = true ) const = 0;
  /** \details
  Checks singular edges.
  \param bOnlyCount [in] Flag that specify should toolkit only check number of connected faces or should it compare faces orientation.
  \returns Returns number of singular edges.
  */
  virtual OdUInt32 numSingularEdges( bool bOnlyCount = true ) const = 0;
  /** \details
  Checks degenerate faces.
  \returns Returns true if and only if shell contains degenerate faces.
  */
  virtual OdUInt32 numDegeneratedFaces() const = 0;
  /** \details
  Checks duplicate faces.
  \param pFirstDuplicateFace [out] Pointer to the first duplicate face.
  \param bRollFaces [in] Flag that specify should toolkit roll faces for duplication check.
  \returns Returns true if and only if shell contains duplicate faces.
  \remarks
  Without rolling faces 1-2-3 and 2-3-1 are not equal.
  */
  virtual bool hasDuplicateFaces( OdUInt32* pFirstDuplicateFace = NULL, bool bRollFaces = false ) const = 0;
  /** \details
  Checks faces orientation.
  \returns Returns true if and only if all faces are correctly oriented.
  \remarks
  This method has no sence if shell contains sharp or singular edges.
  */
  virtual bool checkFacesOrientation() const = 0;
  /** \details
  Checks faces orientation using Face Orientation Callback
  \param pCallback [in] Pointer to the OdGiShellFaceOrientationCallback.
  \returns Returns true if and only if all faces are correctly oriented.
  \remarks
  This method has sence if shell contains sharp or singular edges and may be used instead of virtual bool checkFacesOrientation() const = 0.
  */
  virtual bool checkFacesOrientation( const OdGiShellFaceOrientationCallback* pCallback ) const = 0;
  /** \details
  Unifies shell vertices.
  */
  virtual void unifyVertices() = 0;
  /** \details
  Removes degenerate faces.
  */
  virtual void removeDegenerateFaces() = 0;
  /** \details
  Removes duplicate faces.
  \param bRollFaces [in] Flag that specify should toolkit roll faces for duplication check.
  \remarks
  Without rolling faces 1-2-3 and 2-3-1 are not equal.
  */
  virtual void removeDuplicateFaces( bool bRollFaces = false ) = 0;
  /** \details
  Orients shell faces in correct way.
  \param bRestoreModel [in] Flag that specify should toolkit restore internal shell model.
  \returns Returns true if and only if orientation was successful.
  \remarks
  This method has no sence if shell contains sharp or singular edges.
  After faces re-orientation internal shell model becomes invalid. However usually faces re-orientation is the last operation, so model restoring may be waste of time.
  Toolkit usage after OdGiShellToolkit::fixFaceOrientation( false ) without OdGiShellToolkit::clear() will generate incorrect results.
  */
  virtual bool fixFaceOrientation( bool bRestoreModel = true ) = 0;
  /** \details
  Orients shell faces in correct way using Face Orientation Callback.
  \param pCallback [in] Pointer to the OdGiShellFaceOrientationCallback.
  \param bRestoreModel [in] Flag that specify should toolkit restore internal shell model.
  \returns Returns true if and only if orientation was successful.
  \remarks
  This method has sence if shell contains sharp or singular edges and may be used instead of virtual bool checkFacesOrientation() const = 0.
  After faces re-orientation internal shell model becomes invalid. However usually faces re-orientation is the last operation, so model restoring may be waste of time.
  Toolkit usage after OdGiShellToolkit::fixFaceOrientation( false ) without OdGiShellToolkit::clear() will generate incorrect results.
  */
  virtual bool fixFaceOrientation( const OdGiShellFaceOrientationCallback* pCallback, bool bRestoreModel = true ) = 0;
  /** \details
  Shell toolkit tolerance access.
  \returns Returns shell toolkit tolerance.
  */
  virtual OdGeTol& tolerance() = 0;
  /** \details
  Shell toolkit tolerance access.
  \returns Returns shell toolkit tolerance.
  */
  virtual const OdGeTol& tolerance() const = 0;
  /** \details
  Access to the toolkit number of vertices.
  \returns Returns number of vertices.
  */
  virtual OdUInt32 numVertices() const = 0;
  /** \details
  Access to the toolkit vertices.
  \returns Returns vertices array.
  */
  virtual const OdGePoint3d* vertices() const = 0;
  /** \details
  Access to the toolkit face list size.
  \returns Returns face list size.
  */
  virtual OdUInt32 faceListSize() const = 0;
  /** \details
  Access to the toolkit face list.
  \returns Returns face list.
  */
  virtual const OdInt32* faceList() const = 0;
  /** \details
  Access to the sharp edges.
  \param edges [out] Array of OdGiShellToolkit::OdGiShellEdgeDescription.
  \param bOnlyCount [in] Flag that specify should toolkit only check number of connected faces or should it compare faces orientation.
  */
  virtual void getSharpEdges( OdArray< OdGiShellEdgeDescription >& edges, bool bOnlyCount = true ) const = 0;
  /** \details
  Access to the singular edges.
  \param edges [out] Array of OdGiShellToolkit::OdGiShellEdgeDescription.
  \param bOnlyCount [in] Flag that specify should toolkit only check number of connected faces or should it compare faces orientation.
  */
  virtual void getSingularEdges( OdArray< OdGiShellToolkit::OdGiShellEdgeDescription >& edges, bool bOnlyCount = true ) const = 0;
  /** \details
  Access to the incorrect oriented faces.
  \param edges [out] Array of OdGiShellToolkit::OdGiShellFaceDescription.
  \param bAddNotOriented [in] Flag that specify should toolkit only also put in result array not oriented faces.
  */
  virtual void getIncorrectOrientedFaces( OdArray< OdGiShellFaceDescription >& faces, bool bAddNotOriented = true ) const = 0;
  /** \details
  Access to the incorrect oriented faces.
  \param pCallback [in] Pointer to the OdGiShellFaceOrientationCallback.
  \param edges [out] Array of OdGiShellToolkit::OdGiShellFaceDescription.
  \param bAddNotOriented [in] Flag that specify should toolkit only also put in result array not oriented faces.
  */
  virtual void getIncorrectOrientedFaces( const OdGiShellFaceOrientationCallback* pCallback, OdArray< OdGiShellFaceDescription >& faces, bool bAddNotOriented = true ) const = 0;
  /** \details
  Access to the original face description.
  \param nFaceOffset [in] Face offset in OdGiShellToolkit::faceList()
  \param result [out] Reference to the OdGiShellToolkit::OdGiShellOriginalFaceDescription.
  \returns Returns true if and only if original face description was found.
  */
  virtual bool getOriginalFaceDescription( OdUInt32 nFaceOffset, OdGiShellOriginalFaceDescription& result ) const = 0;
  /** \details
  Generates list of edges with crease angle more than threshold value.
  \param edges [out] Resulting array of OdGiShellEdgeDescription.
  \param dAngle [in] Threshold crease angle value.
  \remarks
  Result is pairs of OdGiShellEdgeDescription, each pair describes the same edge but on different faces.
  */
  virtual void filterEdgesByCreaseAngle( OdArray< OdGiShellToolkit::OdGiShellEdgeDescription >& edges, double dAngle ) const = 0;
  /** \details
  Returns number of non-degenerated non-triangulated faces.
  \returns Returns number of faces with more than 3 points.
  */
  virtual OdUInt32 numNonTriangleFaces() const = 0;
  /** \details
  Triangulates non-degenerated non-triangulated faces.
  */
  virtual void triangulate() = 0;
};
/** \details
Smart pointer to the OdGiShellToolkit.
*/
typedef OdSmartPtr<OdGiShellToolkit> OdGiShellToolkitPtr;

#endif


