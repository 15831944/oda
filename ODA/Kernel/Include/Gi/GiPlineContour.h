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

#ifndef __ODGIPLINECONTOUR_H__
#define __ODGIPLINECONTOUR_H__

#include "Gi/GiGeometrySimplifier.h"
#include "Gi/GiPathNode.h"
#define STL_USING_SET
#include "OdaSTL.h"

#include "TD_PackPush.h"

/** \details
    Empty inheritance class for pline primitive contours calculator.
    <group OdGi_Classes>
*/
struct OdGiPlineContourCalcEmptyInheritance {
  /** \details
      Stub for pline primitive vectorization method.
  */
  void pline(const OdGiPolyline & /*polyline*/, OdUInt32 /*fromIndex*/ = 0, OdUInt32 /*numSegs*/ = 0) {}
};

/** \details
    Simplifier extension interface for pline primitive contours calculator.
    <group OdGi_Classes>
*/
struct OdGiPlineContourCalcSimplifierInterface
{
  /** \details
    Add pline primitive wires into OdGiPlineContourCalc map.
    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  List of numbers that define the faces in the shell.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
  */
  virtual void plineCalcShellWires(OdInt32 faceListSize, const OdInt32 *faceList,
                                   const OdGiEdgeData *pEdgeData = 0, const OdGiFaceData *pFaceData = 0) = 0;
  /** \details
    Drop calculated pline primitive contours onto vectorizer.
    \param bStrips [in]  Output pline contours as line strips or as separate line segments.
  */
  virtual void dropPlineCalcShellWires(bool bStrips) = 0;
};

/** \details
    Helper class to provide direct inheritance from OdGiGeometrySimplifier for pline primitive contours calculator.
    <group OdGi_Classes>
*/
template <typename SimplClass = OdGiGeometrySimplifier>
class OdGiPlineContourCalcSimplifierInheritance : public SimplClass
{
  OdGiPlineContourCalcSimplifierInterface *m_pPlineCtrCalcInterface;
  protected:
    /** \details
      Setup pline calculation interface.
      \param pInterface [in]  Pointer onto pline calculation interface.
    */
    void setPlineCalcInterface(OdGiPlineContourCalcSimplifierInterface *pInterface) { m_pPlineCtrCalcInterface = pInterface; }
    /** \details
      Tessellates a wireframe shell.
      \param faceListSize [in]  Number of entries in faceList.
      \param faceList [in]  List of numbers that define the faces in the shell.
      \param pEdgeData [in]  Pointer to additional edge data.
      \param pFaceData [in]  Pointer to additional face data.
    */
    virtual void generateShellWires(OdInt32 faceListSize, const OdInt32 *faceList,
                                    const OdGiEdgeData *pEdgeData = 0, const OdGiFaceData *pFaceData = 0)
    {
      m_pPlineCtrCalcInterface->plineCalcShellWires(faceListSize, faceList, pEdgeData, pFaceData);
    }
    /** \details
      Redirects generateShellWires call onto geometry simplifier.
      \param faceListSize [in]  Number of entries in faceList.
      \param faceList [in]  List of numbers that define the faces in the shell.
      \param pEdgeData [in]  Pointer to additional edge data.
      \param pFaceData [in]  Pointer to additional face data.
    */
    void plineCalcRedirectGenerateShellWires(OdInt32 faceListSize, const OdInt32 *faceList,
                                             const OdGiEdgeData *pEdgeData, const OdGiFaceData *pFaceData)
    {
      SimplClass::generateShellWires(faceListSize, faceList, pEdgeData, pFaceData);
    }
    /** \details
      Returns geometry simplifier interface.
    */
    OdGiGeometrySimplifier *plineCalcSimplifier() { return this; }
};

/** \details
    Helper class to provide indirect inheritance from OdGiGeometrySimplifier for pline primitive contours calculator.
    <group OdGi_Classes>
*/
class OdGiPlineContourCalcNonSimplifierInheritance
{
  OdGiGeometrySimplifier *m_pPlineCtrCalcSimplifier;
  public:
    OdGiPlineContourCalcNonSimplifierInheritance() : m_pPlineCtrCalcSimplifier(NULL) {}
  protected:
    /** \details
      Setup pline calculation interface (method stub).
    */
    void setPlineCalcInterface(OdGiPlineContourCalcSimplifierInterface * /*pInterface*/) {}
    /** \details
      Setup geometry simplifier.
      \param pSimplifier [in]  Geometry simplifier pointer.
    */
    void setPlineCalcSimplifier(OdGiGeometrySimplifier *pSimplifier) { m_pPlineCtrCalcSimplifier = pSimplifier; }
    /** \details
      Redirects generateShellWires call onto geometry simplifier.
      \param faceListSize [in]  Number of entries in faceList.
      \param faceList [in]  List of numbers that define the faces in the shell.
      \param pEdgeData [in]  Pointer to additional edge data.
      \param pFaceData [in]  Pointer to additional face data.
    */
    void plineCalcRedirectGenerateShellWires(OdInt32 faceListSize, const OdInt32 *faceList,
                                             const OdGiEdgeData *pEdgeData, const OdGiFaceData *pFaceData)
    {
      m_pPlineCtrCalcSimplifier->generateShellWires(faceListSize, faceList, pEdgeData, pFaceData);
    }
    /** \details
      Returns geometry simplifier interface.
    */
    OdGiGeometrySimplifier *plineCalcSimplifier() { return m_pPlineCtrCalcSimplifier; }
};

/** \details
    This is helper class which can be used to compute pline primitive contours on top of geometry simplifier.
    <group OdGi_Classes> 
*/
template <typename VectClass = OdGiPlineContourCalcEmptyInheritance, typename SimplClass = OdGiPlineContourCalcNonSimplifierInheritance>
class OdGiPlineContourCalc : public VectClass, public SimplClass, public OdGiPlineContourCalcSimplifierInterface
{
  protected:
    enum PlineCtrCalcFlags
    {
      kPlineCtrCalcEnabled = (1 << 0),
      kPlineCtrCalcStrip   = (1 << 1),
      kPlineCtrCalcInPrim  = (1 << 2),
      kPlineCtrCalcHasGeo  = (1 << 3),
    };
  protected:
    OdUInt32 m_nPlineCtrCalcFlags;
    typedef std::pair<OdGePoint3d, OdGePoint3d> EdgePointsPair;
    typedef std::pair<OdInt32, OdInt32> EdgePointsKey;
    class PlineCtrCalcNode : public std::pair<OdGePoint3d, OdGePoint3d>
    {
      OdGsMarker m_gsMarker;
      static inline int cmp_el(double d1, double d2, const OdGeTol &tol = OdGeContext::gTol)
      {
        if (OdLess(d1, d2, tol.equalPoint()))
          return 1;
        if (OdGreater(d1, d2, tol.equalPoint()))
          return 0;
        return -1;
      }
      public:
        PlineCtrCalcNode(const OdGePoint3d &pt1, const OdGePoint3d &pt2, OdGsMarker gsMarker)
          : std::pair<OdGePoint3d, OdGePoint3d>(pt1, pt2), m_gsMarker(gsMarker)
        {}
        OdGsMarker subentMarker() const { return m_gsMarker; }

        bool operator <(const PlineCtrCalcNode &p2) const
        { int
          rv = cmp_el(first.x, p2.first.x); if (rv >= 0) return rv > 0;
          rv = cmp_el(first.y, p2.first.y); if (rv >= 0) return rv > 0;
          rv = cmp_el(first.z, p2.first.z); if (rv >= 0) return rv > 0;
          rv = cmp_el(second.x, p2.second.x); if (rv >= 0) return rv > 0;
          rv = cmp_el(second.y, p2.second.y); if (rv >= 0) return rv > 0;
          return OdLess(second.z, p2.second.z, OdGeContext::gTol.equalPoint());
        }
    };
    typedef std::set<PlineCtrCalcNode> PlineCtrCalcMap;
    PlineCtrCalcMap m_plineCtrCalcMap;
    OdGePoint3dArray m_plineCtrCalcStrip;
  protected:
    void filterPlineCalcMap(const OdGePoint3d &p1, const OdGePoint3d &p2, OdGsMarker gsMarker)
    { OD_TYPENAME PlineCtrCalcMap::iterator it = m_plineCtrCalcMap.find(PlineCtrCalcNode(p2, p1, gsMarker));
      //if (it == m_plineCtrCalcMap.end()) // #CORE-17334 : can eliminate overlapped segments contour
      //  it = m_plineCtrCalcMap.find(PlineCtrCalcNode(p1, p2, gsMarker));
      if (it == m_plineCtrCalcMap.end())
        m_plineCtrCalcMap.insert(PlineCtrCalcNode(p1, p2, gsMarker));
      else // Remove duplicate edge
        m_plineCtrCalcMap.erase(it);
    }
    OdGsMarker buildPlineCalcStrip()
    { bool bFirst = false;
      OD_TYPENAME PlineCtrCalcMap::iterator curPair = m_plineCtrCalcMap.begin();
      const OdGsMarker gsMarker = curPair->subentMarker();
      m_plineCtrCalcStrip.push_back(curPair->first);
      m_plineCtrCalcStrip.push_back(curPair->second);
      for (;;)
      { OD_TYPENAME PlineCtrCalcMap::iterator it = m_plineCtrCalcMap.begin();
        for (; it != m_plineCtrCalcMap.end(); it++)
        { 
          if (it == curPair)
            continue;
          bool bSecond = false;
          if ((bFirst) ? (it->first == curPair->first) : (it->first == curPair->second))
          { m_plineCtrCalcMap.erase(curPair); curPair = it; bFirst = false;
            if (it->subentMarker() == gsMarker)
            { m_plineCtrCalcStrip.push_back(it->second);
              break; }
            return gsMarker;
          }
          else if ((bFirst) ? (it->second == curPair->first) : (it->second == curPair->second))
          { m_plineCtrCalcMap.erase(curPair); curPair = it; bFirst = true;
            if (it->subentMarker() == gsMarker)
            { m_plineCtrCalcStrip.push_back(it->first); 
              break; }
            return gsMarker;
          }
        }
        if (it == m_plineCtrCalcMap.end())
        {
          m_plineCtrCalcMap.erase(curPair);
          break;
        }
      }
      return gsMarker;
    }
  public:
    OdGiPlineContourCalc() : m_nPlineCtrCalcFlags(kPlineCtrCalcEnabled | kPlineCtrCalcStrip)
    { this->setPlineCalcInterface(this); }

    /** \details
      Enables or disable pline contours calculation.
      \param bSet [in]  New pline calculation state.
    */
    void enablePlineContoursCalc(bool bSet) { SETBIT(m_nPlineCtrCalcFlags, kPlineCtrCalcEnabled, bSet); }

    /** \details
      Returns true in case if pline contours calculation enabled.
    */
    bool plineContoursCalcEnabled() const { return GETBIT(m_nPlineCtrCalcFlags, kPlineCtrCalcEnabled); }

    /** \details
      Use this call at start of pline processing in case if direct inheritance from vectorizer non provided.
    */
    void plineCalcStarted()
    {
      if (plineContoursCalcEnabled())
      {
        SETBIT_1(m_nPlineCtrCalcFlags, kPlineCtrCalcInPrim);
      }
    }
    /** \details
      Use this call at end of pline processing in case if direct inheritance from vectorizer non provided.
    */
    void plineCalcEnded()
    {
      if (GETBIT(m_nPlineCtrCalcFlags, kPlineCtrCalcHasGeo))
      {
        if (!m_plineCtrCalcMap.empty())
          dropPlineCalcShellWires(plineCalcStripsEnabled());
      }
      SETBIT_0(m_nPlineCtrCalcFlags, kPlineCtrCalcInPrim | kPlineCtrCalcHasGeo);
    }

    /** \details
      This override will be used in case if direct inheritance from vectorizer is provided to detect pline primitive processing.
      \param polyline [in]  Polyline to process.
      \param fromIndex [in]  Index of the first segment to be processed.
      \param numSegs [in]  Number of segments to be processed.
    */
    virtual void pline(const OdGiPolyline &polyline, OdUInt32 fromIndex = 0, OdUInt32 numSegs = 0)
    {
      plineCalcStarted();
      VectClass::pline(polyline, fromIndex, numSegs);
      plineCalcEnded();
    }

    /** \details
      Returns true in case if pline primitive calculations started.
    */
    bool hasPlineStarted() const
    {
      return GETBIT(m_nPlineCtrCalcFlags, kPlineCtrCalcInPrim);
    }

    /** \details
      Add pline primitive wires into OdGiPlineContourCalc map.
      \param faceListSize [in]  Number of entries in faceList.
      \param faceList [in]  List of numbers that define the faces in the shell.
      \param pEdgeData [in]  Pointer to additional edge data.
      \param pFaceData [in]  Pointer to additional face data.
    */
    virtual void plineCalcShellWires(OdInt32 faceListSize, const OdInt32* faceList,
                                     const OdGiEdgeData* pEdgeData = 0, const OdGiFaceData* pFaceData = 0)
    {
      if (hasPlineStarted())
      { OdGiGeometrySimplifier *pSimplifier = this->plineCalcSimplifier();
        const OdGsMarker gsMarker = pSimplifier->drawContext()->currentGiPath()->selectionMarker();
        const OdGePoint3d* pVtx = pSimplifier->vertexDataList();
        for (OdInt32 nFaceList = 0; nFaceList < faceListSize; nFaceList++)
        { const OdInt32 nElems = Od_abs(faceList[nFaceList]);
          for (OdInt32 nSeg = 1; nSeg < nElems; nSeg++)
            filterPlineCalcMap(pVtx[faceList[nFaceList + nSeg]], pVtx[faceList[nFaceList + nSeg + 1]], gsMarker);
          filterPlineCalcMap(pVtx[faceList[nFaceList + nElems]], pVtx[faceList[nFaceList + 1]], gsMarker);
          nFaceList += nElems;
        }
        SETBIT_1(m_nPlineCtrCalcFlags, kPlineCtrCalcHasGeo);
      }
      else
        this->plineCalcRedirectGenerateShellWires(faceListSize, faceList, pEdgeData, pFaceData);
    }

    /** \details
      Enables or disables output line strips calculation.
      \param bSet [in]  New state for line strips calculation.
    */
    void enablePlineCalcSrips(bool bSet)
    {
      SETBIT(m_nPlineCtrCalcFlags, kPlineCtrCalcStrip, bSet);
    }

    /** \details
      Returns current state of line strips calculation.
    */
    bool plineCalcStripsEnabled() const
    {
      return GETBIT(m_nPlineCtrCalcFlags, kPlineCtrCalcStrip);
    }

    /** \details
      Drop calculated pline primitive contours onto vectorizer.
      \param bStrips [in]  Output pline contours as line strips or as separate line segments.
    */
    virtual void dropPlineCalcShellWires(bool bStrips)
    { OdGiGeometrySimplifier *pSimplifier = this->plineCalcSimplifier();
      OdGsMarker curMarker = pSimplifier->drawContext()->currentGiPath()->selectionMarker();
      const bool bUpdateMarkers = GETBIT(pSimplifier->drawContext()->drawContextFlags(), OdGiConveyorContext::kForceMarkersOnModified);
      if (!bStrips)
      { OdGePoint3d outPts[2];
        for (OD_TYPENAME PlineCtrCalcMap::iterator it = m_plineCtrCalcMap.begin(); it != m_plineCtrCalcMap.end(); it++)
        { outPts[0] = it->first; outPts[1] = it->second;
          if (curMarker != it->subentMarker())
          { pSimplifier->drawContext()->subEntityTraits().setSelectionMarker(curMarker = it->subentMarker());
            if (bUpdateMarkers)
              pSimplifier->drawContext()->onTraitsModified();
          }
          pSimplifier->polylineOut(2, outPts);
        }
      }
      else
      {
        while (!m_plineCtrCalcMap.empty())
        { const OdGsMarker gsMarker = buildPlineCalcStrip();
          if (curMarker != gsMarker)
          { pSimplifier->drawContext()->subEntityTraits().setSelectionMarker(curMarker = gsMarker);
            if (bUpdateMarkers)
              pSimplifier->drawContext()->onTraitsModified();
          }
          pSimplifier->polylineOut(m_plineCtrCalcStrip.size(), m_plineCtrCalcStrip.getPtr());
          m_plineCtrCalcStrip.clear();
        }
      }
    }
};

#include "TD_PackPop.h"

#endif //#ifndef __ODGIPLINECONTOUR_H__
