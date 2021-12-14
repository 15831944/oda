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


#ifndef _ODBREPRENDERERIMPL_INCLUDED_
#define _ODBREPRENDERERIMPL_INCLUDED_

#include "Wr/wrExport.h"
#include "BrepRenderer.h"

class OdBrBrep; 

class wrRenderCache;
class OdGiCommonDraw ;
class OdGiViewportDraw;
class wrTriangulationParams;
class OdGeMatrix3d;

#include "TD_PackPush.h"
#include "Gi/GiCommonDraw.h"

class wrRenderCacheElement;
namespace GeMesh { class OdGeTrMesh; }
class OdGeExtents3d;

/** \details
  This class implements platform-dependent ACIS rendering for the ODA Platform.

  Corresponding C++ library: TD_BrepRenderer

  <group Other_Classes>
*/
class RENDER_EXPORT OdBrepRendererImpl : public OdBrepRenderer
{
public:
  enum Mode
  {
    kDefault = 0,
    kBIM = 1
  };
private:
  const OdBrBrep              *m_pBrep;
  bool                         m_enableCache;
  wrTriangulationParams        m_pTriangulationParams;
  const wrTriangulationParams* m_pTriangulationParamsGlobal; // this pointer is for old tessellation back-compatibility
  wrRenderCache               *m_pCache;
  wrRenderCache               *m_pFaceMeshCache;
  Mode                         m_Mode;
  OdDbBaseDatabase            *m_pDb;
  wrRenderFilter              *m_pFaceFilter;
  double                       m_facetRes; // 0 - use deviation from triangulation params, non-zero - use "lazy" deviation

public:

  OdBrepRendererImpl();
  virtual ~OdBrepRendererImpl();

  void setDatabase(OdDbBaseDatabase *pDb) { m_pDb = pDb; }

  void setMode(OdBrepRendererImpl::Mode m);
  OdBrepRendererImpl::Mode mode() const;

  virtual void setBrep(const OdBrBrep& brep);

  virtual void enableCaching();
  virtual void disableCaching(bool clearCache = true);
  virtual bool isCachingEnabled() const;

  virtual void setTriangulationParams(const wrTriangulationParams *pTriangulationParams);

  virtual const wrTriangulationParams * getTriangulationParams() const;

  virtual bool draw(OdGiCommonDraw * pWd, OdUInt32 geomType, const NumIsolines *pNumIsolines = 0);

  virtual bool drawSilhouettes(OdGiViewportDraw* pVd);

  virtual void transformBy(const OdGeMatrix3d &xfm);

  virtual bool drawFace(OdGiCommonDraw * pWd, OdGsMarker iFace, const wrTriangulationParams& triangulationParams);
  virtual bool getFaceMesh(GeMesh::OdGeTrMesh& mesh, OdGsMarker iFace, const wrTriangulationParams& triangulationParams);

  virtual bool getExtents(OdGeExtents3d& extents, OdUInt32 nIso = 6);
  virtual void setFaceFilter(wrRenderFilter* pFaceFilter);

  virtual bool getPtSurface(OdGePoint3d& pt); // get the point for calculating deviation in perspective mode

  /** \details
    Determines the types of trimming loops for the given face.

    \param face       [in] Face to calculate loop types for.
    \param aLoopType [out] Pairs of loop indices and their type flags.

    \returns
    True if the operation is successful; false otherwise.

    \remarks
    aLoopType is a combination of one or more of the following:

    <table>
    Name             Value
    HOLE             1
    FULLNESS         2
    ZEROSQUARE       4
    FULLBORDER       8
    UNPREDICTABLE    0
    </table>
  */
  bool getLoopTypes(const OdBrFace& face, OdArray< std::pair<int, int> >& aLoopType);

  /** \details
    Gets the max deviation for facets.

    \param pWd       [in]  Pointer to the OdGiCommonDraw interface.
    \param deviation [out] Max deviation for facets.
    \returns
    True if the operation is successful; false otherwise.
  */
  bool getDeviation(OdGiCommonDraw* pWd, double& deviation);

  /** \details
    Sets "lazy" deviation calculation mode.

    \param facetRes  [in]  FACETRES variable value to take into account calculating deviation.
    \remarks
    If 'facetRes' parameter is zero, "lazy" calculation is not used.
  */
  void setLazyDeviation(double facetRes);
};

#include "TD_PackPop.h"

#endif //_ODBREPRENDERERIMPL_INCLUDED_
