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

using System;
using System.Collections.Generic;

using System.Text;
using Teigha.Core;
using Teigha.TD;
using System.Runtime.InteropServices;

public class SampleFillInitialData
{
  List<OdBrEdge> m_edges;
  uint m_zeroBase = 0;
  const double m_toleranceRestore2dCurve = 1e-6;
  const double m_toleranceDiffPoints = 1e-3;
  const double m_toleranceInterval = 1e-9;

  public static void ConsoleDumpBrepBuilderInitialData(BrepBuilderInitialData initData)
  {
    BrepBuilderComplexArray complexes = initData.complexes;
    BrepBuilderInitialEdgeArray edges = initData.edges;

    int shells_Idx = 0;
    foreach (BrepBuilderShellsArray shells in complexes)
    {
      Console.WriteLine("BrepBuilderShellsArray Idx: " + shells_Idx);
      shells_Idx++;
      int init_surfaces_Idx = 0;
      foreach (BrepBuilderInitialSurfaceArray init_surfaces in shells)
      {
        Console.WriteLine("BrepBuilderInitialSurfaceArray Idx: " + init_surfaces_Idx);
        init_surfaces_Idx++;
        int init_surface_Idx = 0;
        foreach (BrepBuilderInitialSurface init_surface in init_surfaces)
        {
          Console.WriteLine("BrepBuilderInitialSurface Idx: " + init_surface_Idx);
          init_surface_Idx++;
          Console.WriteLine("BrepBuilderInitialSurface direction :" + init_surface.direction);
          Console.WriteLine("BrepBuilderInitialSurface pSurf is null :" + (init_surface.get_pSurf() == null).ToString());
          int loop_Idx = 0;
          foreach (BrepBuilderInitialLoop loops in init_surface.loops)
          {
            Console.WriteLine("Loop Idx: " + loop_Idx);
            loop_Idx++;
            int coedge_Idx = 0;
            foreach (BrepBuilderInitialCoedge coedge in loops.coedges)
            {
              Console.WriteLine("Coedge Idx: " + coedge_Idx);
              coedge_Idx++;
              Console.WriteLine("EdgeIndex :" + coedge.GetEdgeIndex());
              Console.WriteLine("Curve is null :" + (coedge.GetCurve() == null).ToString());
              Console.WriteLine("Direction :" + coedge.direction);
            }
          }
        }
      }
    }
  }

  public OdResult getDataFrom(OdBrBrep brep, ref BrepBuilderInitialData initData)
  {
    m_zeroBase = (uint)initData.edges.Count;
    m_edges = new List<OdBrEdge>();

    OdBrBrepComplexTraverser complxTrav = new OdBrBrepComplexTraverser();
    OdBrErrorStatus errStatus = complxTrav.setBrep(brep);

    if (OdBrErrorStatus.odbrOK != errStatus)
    {
      return OdResult.eInvalidInput;
    }

    while (!complxTrav.done())
    {
      OdBrComplex complex = complxTrav.getComplex();

      OdResult res = performComplex(ref initData, ref complex);
      if (OdResult.eOk != res)
      {
        return res;
      }

      if (OdBrErrorStatus.odbrOK != complxTrav.next())
      {
        return OdResult.eInvalidInput;
      }
    }

    return OdResult.eOk;
  }

  #region Private
  OdResult performComplex(ref BrepBuilderInitialData initialData, ref OdBrComplex complex)
  {
    OdBrComplexShellTraverser complxShellTrav = new OdBrComplexShellTraverser();
    OdBrErrorStatus errStatus = complxShellTrav.setComplex(complex);
    if (OdBrErrorStatus.odbrUnsuitableTopology == errStatus)
    {
      return OdResult.eOk;
    }
    if (OdBrErrorStatus.odbrOK != errStatus)
    {
      return OdResult.eInvalidInput;
    }

    BrepBuilderShellsArray shells = new BrepBuilderShellsArray();
    while (!complxShellTrav.done())
    {
      OdBrShell shell = complxShellTrav.getShell();

      OdResult res = performShell(ref initialData, ref shell, ref shells);
      if (OdResult.eOk != res)
      {
        return res;
      }

      if (OdBrErrorStatus.odbrOK != complxShellTrav.next())
      {
        return OdResult.eInvalidInput;
      }
    }
    GC.SuppressFinalize(shells);
    initialData.complexes.Add(shells);

    return OdResult.eOk;
  }

  OdResult performShell(ref BrepBuilderInitialData initialData, ref OdBrShell shell, ref BrepBuilderShellsArray arrShells)
  {
    OdBrShellFaceTraverser shellFaceTrav = new OdBrShellFaceTraverser();
    OdBrErrorStatus errStatus = shellFaceTrav.setShell(shell);
    if (OdBrErrorStatus.odbrUnsuitableTopology == errStatus)
    {
      return OdResult.eOk;
    }
    if (OdBrErrorStatus.odbrOK != errStatus)
    {
      return OdResult.eInvalidInput;
    }

    BrepBuilderInitialSurfaceArray arrSurfaces = new BrepBuilderInitialSurfaceArray();
    while (!shellFaceTrav.done())
    {
      OdBrFace face = shellFaceTrav.getFace();
      int nFacesCreated = arrSurfaces.Count;
      OdResult res = performFace(ref initialData, ref face, ref arrSurfaces);
      if (OdResult.eOk != res)
      {
        return res;
      }
      nFacesCreated = arrSurfaces.Count - nFacesCreated;
      if (nFacesCreated > 1)//additional face has been created
      {                     //this face(s) should be in another shell. So after processing all faces we should check it 
                            //bCheckFaces = true;
      }
      if (OdBrErrorStatus.odbrOK != shellFaceTrav.next())
      {
        return OdResult.eInvalidInput;
      }
    }

    //if (bCheckFaces)
    //{
    //  //detect contiguous faces and move it to logical groups. Each of this group should be in separate shell
    //  OdArray < std::set < unsigned int> > gropedFaces;
    //  groupFaces(arrSurfaces, gropedFaces);
    //  for (unsigned int i = 0; i < gropedFaces.length(); i++)
    //  {
    //    BrepBuilderInitialSurfaceArray arrNewSurfaces;
    //    std::set < unsigned int> indexes = gropedFaces[i];
    //    for (std::set < unsigned int >::iterator it = indexes.begin(); it != indexes.end(); it++)
    //    {
    //      arrNewSurfaces.append(arrSurfaces[*it]);
    //    }
    //    arrShells.append(arrNewSurfaces);
    //  }
    //  return eOk;
    //}
    GC.SuppressFinalize(arrSurfaces);
    arrShells.Add(arrSurfaces);

    return OdResult.eOk;
  }

  OdResult performFace(ref BrepBuilderInitialData initialData, ref OdBrFace face, ref BrepBuilderInitialSurfaceArray arrSurfaces)
  {
    OdResult eStatus;
    BrepBuilderInitialSurface surfData = new BrepBuilderInitialSurface();
    surfData.set_pSurf(getFaceSurface(face));
    if (surfData.get_pSurf() == null)
    {
      return OdResult.eInvalidInput;
    }

    // face direction
    surfData.direction = face.getOrientToSurface() ? OdBrepBuilder.EntityDirection.kForward : OdBrepBuilder.EntityDirection.kReversed;

    OdBrFaceLoopTraverser faceLoopTrav = new OdBrFaceLoopTraverser();
    OdBrErrorStatus err = faceLoopTrav.setFace(face);
    if (OdBrErrorStatus.odbrUnsuitableTopology == err)
    {
      // Face without loops (sphere, torus)
      GC.SuppressFinalize(surfData);
      arrSurfaces.Add(surfData);
      return OdResult.eOk;
    }
    if (OdBrErrorStatus.odbrOK != err)
    {
      return OdResult.eInvalidInput;
    }

    while (!faceLoopTrav.done())
    {
      OdBrLoop loop = faceLoopTrav.getLoop();
      eStatus = performLoop(ref initialData, ref loop, ref surfData);
      if (OdResult.eOk != eStatus)
      {
        return eStatus;
      }

      if (OdBrErrorStatus.odbrOK != faceLoopTrav.next())
      {
        return OdResult.eInvalidInput;
      }
    }
    GC.SuppressFinalize(surfData);
    arrSurfaces.Add(surfData);
    return OdResult.eOk;
  }

  OdGeSurface getFaceSurface(OdBrFace face)
  {
    OdGeSurface surf = face.getSurface();
    if (surf == null)
    {
      OdGeNurbSurface nurbSurf = new OdGeNurbSurface();
      if (OdBrErrorStatus.odbrOK == face.getSurfaceAsNurb(nurbSurf))
      {
        return nurbSurf.copy() as OdGeSurface;
      }
      return null;
    }

    OdGe.EntityId entType = surf.type();
    if (OdGe.EntityId.kExternalBoundedSurface == entType)
    {
      OdGeExternalBoundedSurface extSurf = new OdGeExternalBoundedSurface(OdGeSurface.getCPtr(surf).Handle, false);
      OdGeSurface tResSurf = extSurf.getBaseSurfaceEx();
      if (tResSurf != null && tResSurf.type() != OdGe.EntityId.kExternalSurface)
        surf = tResSurf;
      else if (tResSurf != null && tResSurf.type() == OdGe.EntityId.kExternalSurface)
        surf = checkExtSurface(tResSurf, face);
    }
    else if (OdGe.EntityId.kExternalSurface == entType)
      surf = checkExtSurface(surf, face);

    return surf;
  }

  OdGeSurface checkExtSurface(OdGeSurface resSurf, OdBrFace face)
  {
    OdGeExternalSurface extSurf = resSurf as OdGeExternalSurface;
    OdGeSurface pSurf = null;
    if (extSurf.isNativeSurface(ref pSurf))
      return pSurf;

    OdGeNurbSurface nurbFace = new OdGeNurbSurface();
    face.getSurfaceAsNurb(nurbFace);
    return new OdGeNurbSurface(nurbFace);
  }

  OdResult performLoop(ref BrepBuilderInitialData initialData, ref OdBrLoop loop, ref BrepBuilderInitialSurface surfData)
  {
    OdBrErrorStatus err;
    OdBrLoopEdgeTraverser loopEdgeTrav = new OdBrLoopEdgeTraverser();
    err = loopEdgeTrav.setLoop(loop);

    // Loop with apex:
    if (OdBrErrorStatus.odbrDegenerateTopology == err) // maybe there should be odbrUnsuitableTopology (see arx)
    {
      OdGeCurve3d curve = null;
      OdGeCurve2d paramCurve = null;
      OdResult resStatus = performLoopWithApex(ref loop, ref curve, ref paramCurve);

      uint idxBrEdge;
      if (OdResult.eOk == resStatus)
      {
        BrepBuilderInitialEdge init_edge = new BrepBuilderInitialEdge(curve);
        GC.SuppressFinalize(init_edge);
        initialData.edges.Add(init_edge);

        OdBrEdge brEdge = new OdBrEdge();
        GC.SuppressFinalize(brEdge);
        m_edges.Add(brEdge);
        idxBrEdge = (uint)m_edges.Count - 1;

        BrepBuilderInitialLoop init_loop = new BrepBuilderInitialLoop(
          paramCurve,
          m_zeroBase + idxBrEdge, // stub edge for correct index
          OdBrepBuilder.EntityDirection.kForward
        );
        GC.SuppressFinalize(init_loop);
        surfData.loops.Add(init_loop);

        return OdResult.eOk;
      }
      return resStatus;
    }
    if (OdBrErrorStatus.odbrOK != err)
    {
      return OdResult.eInvalidInput;
    }

    // Regular loop:
    BrepBuilderInitialLoop loopData = new BrepBuilderInitialLoop();

    while (!loopEdgeTrav.done())
    {
      OdBrEdge edge = loopEdgeTrav.getEdge();

      BrepBuilderInitialCoedge coedgeData = new BrepBuilderInitialCoedge();
      coedgeData.SetCurve(null);

      OdGeCurve3d curve = null;
      bool isNewEdge = true;

      for (int idx = 0; idx < m_edges.Count; ++idx)
      {
        if (m_edges[idx].isEqualTo(edge))
        {
          isNewEdge = false;
          coedgeData.SetEdgeIndex((uint)(m_zeroBase + idx));
          curve = initialData.edges[(int)coedgeData.GetEdgeIndex()].GetCurve();
          break;
        }
      }

      if (isNewEdge)
      {
        if (!getEdgeCurveFixed(edge, out curve))
        {
          return OdResult.eInvalidInput;
        }

        BrepBuilderInitialEdge edgeData = new BrepBuilderInitialEdge();
        edgeData.SetCurve(curve);
        initialData.edges.Add(edgeData);
        GC.SuppressFinalize(edgeData);
        m_edges.Add(edge);
        uint idxBrEdge = (uint)m_edges.Count - 1;
        coedgeData.SetEdgeIndex(m_zeroBase + idxBrEdge);
      }

      coedgeData.direction = (edge.getOrientToCurve() == loopEdgeTrav.getEdgeOrientToLoop())
        ? OdBrepBuilder.EntityDirection.kForward : OdBrepBuilder.EntityDirection.kReversed;

      if (OdBrErrorStatus.odbrOK != loopEdgeTrav.next())
      {
        return OdResult.eInvalidInput;
      }

      GC.SuppressFinalize(coedgeData);
      loopData.coedges.Add(coedgeData);
    }
    // skip loop without coedge
    if (loopData.coedges.Count != 0)
    {
      GC.SuppressFinalize(loopData);
      surfData.loops.Add(loopData);
    }

    return OdResult.eOk;

  }

  OdResult performLoopWithApex(ref OdBrLoop loop, ref OdGeCurve3d curve, ref OdGeCurve2d paramCurve)
  {
    OdBrLoopVertexTraverser loopVertTrav = new OdBrLoopVertexTraverser();
    if (OdBrErrorStatus.odbrOK != loopVertTrav.setLoop(loop))
    {
      return OdResult.eInvalidInput;
    }

    OdGePoint3d point = loopVertTrav.getVertex().getPoint();

    if ((OdBrErrorStatus.odbrOK != loopVertTrav.next()) || !loopVertTrav.done())
    {
      // unexpected: more then one point
      return OdResult.eInvalidInput;
    }

    curve = new OdGeLineSeg3d(point, point);
    paramCurve = null;

    return OdResult.eOk;
  }

  bool getEdgeCurveFixed(OdBrEdge edge, out OdGeCurve3d pCurve)
  {
    pCurve = getEdgeCurve(edge);

    if (pCurve == null)
    {
      return false;
    }

    OdGe.EntityId curveType = pCurve.type();
    if (OdGe.EntityId.kEllipArc3d == curveType)
    {
      return fixEllipse(ref pCurve, edge);
    }
    else if (OdGe.EntityId.kNurbCurve3d == curveType)
    {
      //return fixNurb(pCurve, edge);
    }

    return true;
  }

  bool fixEllipse(ref OdGeCurve3d pCurve, OdBrEdge edge)
  {
    OdGeEllipArc3d geEllipse = new OdGeEllipArc3d(OdGeCurve3d.getCPtr(pCurve).Handle, false);
    OdBrVertex startVertex = new OdBrVertex();
    OdBrVertex endVertex = new OdBrVertex();
    if (!edge.getVertex1(startVertex) || !edge.getVertex2(endVertex))
    {
      // parasolid variant
      return true;
    }

    OdGePoint3d startPoint;
    OdGePoint3d endPoint;
    if (edge.getOrientToCurve())
    {
      startPoint = startVertex.getPoint();
      endPoint = endVertex.getPoint();
    }
    else
    {
      endPoint = startVertex.getPoint();
      startPoint = endVertex.getPoint();
    }

    OdGePoint3d curveStart = new OdGePoint3d();
    OdGePoint3d curveEnd = new OdGePoint3d();
    if (!geEllipse.hasStartPoint(curveStart)
      || !geEllipse.hasEndPoint(curveEnd))
    {
      return false;
    }

    if (startPoint.isEqualTo(curveStart, new OdGeTol(m_toleranceDiffPoints)))
    {
      return true;
    }

    OdGeInterval interval = new OdGeInterval();
    geEllipse.getInterval(interval);

    if (!geEllipse.isCircular() || !startPoint.isEqualTo(endPoint, new OdGeTol(m_toleranceDiffPoints)))
    {
      double newStartParam = geEllipse.paramOf(startPoint);
      geEllipse.setInterval(new OdGeInterval(newStartParam, newStartParam + interval.length()));
      pCurve = new OdGeNurbCurve3d(geEllipse);

      //m_edgeCurveChanged.insert(pCurve);

      //ODA_ASSERT_ONCE(((OdGeNurbCurve3d*)pCurve)->startPoint().isEqualTo(startPoint, 1e-04));

      return true;
    }

    // closed circle
    OdGePoint3d center = geEllipse.center();
    OdGeVector3d major = geEllipse.majorAxis();
    OdGeVector3d normal = geEllipse.normal();

    //ODA_ASSERT_ONCE(OdEqual(startPoint.distanceTo(center), geEllipse.majorRadius(), 1e-05));

    OdGeVector3d centerToStart = startPoint - center;
    if (major.isCodirectionalTo(centerToStart))
    {
      if (Globals.OdNegative(interval.lowerBound()))
      {
        geEllipse.setInterval(new OdGeInterval(0.0, interval.length()));

        //m_edgeCurveChanged.insert(pCurve);
      }
    }
    else
    {
      double angle = major.angleTo(centerToStart, normal);

      if (!Globals.OdZero(angle))
      {
        geEllipse.rotateBy(angle, normal, center);
        if (Globals.OdNegative(interval.lowerBound()))
        {
          geEllipse.setInterval(new OdGeInterval(0.0, interval.length()));

          // m_edgeCurveChanged.insert(pCurve);
        }
      }
    }

    //ODA_ASSERT_ONCE(geEllipse.hasStartPoint(curveStart));
    //ODA_ASSERT_ONCE(startPoint.isEqualTo(curveStart, 1e-05));

    return true;
  }

  // Edge
  OdGeCurve3d getEdgeCurve(OdBrEdge edge)
  {
    OdGeCurve3d curve3d = edge.getCurve();

    if (curve3d == null)
    {
      OdGeNurbCurve3d nurbCurve3d = new OdGeNurbCurve3d();
      if (edge.getCurveAsNurb(nurbCurve3d))
      {
        return new OdGeCurve3d(OdGeEntity3d.getCPtr(nurbCurve3d.copy()).Handle, false);
      }
      return null;
    }

    OdGe.EntityId entType = curve3d.type();
    if (OdGe.EntityId.kExternalCurve3d == entType)
    {
      OdGeCurve3d resCurve3d = null;
      OdGeExternalCurve3d extCurve = new OdGeExternalCurve3d(OdGeCurve3d.getCPtr(curve3d).Handle, false);

      if (extCurve.isNativeCurve(out resCurve3d))
      {
        OdGeCurve3d pCurveStore = resCurve3d;

        OdGeInterval curvIntOrig = new OdGeInterval(), curvIntNew = new OdGeInterval();
        curve3d.getInterval(curvIntOrig);
        pCurveStore.getInterval(curvIntNew);

        if (isIntervalContain(curvIntOrig, curvIntNew)
          || isIntervalContainReversed(curvIntOrig, curvIntNew))
        {
          curve3d = pCurveStore;
        }
      }
    }

    return curve3d;
  }

  bool isIntervalContain(OdGeInterval frstInt, OdGeInterval scndInt)
  {
    return Globals.OdLessOrEqual(frstInt.lowerBound(), scndInt.lowerBound(), m_toleranceInterval)
      && Globals.OdGreaterOrEqual(frstInt.upperBound(), scndInt.upperBound(), m_toleranceInterval);
  }

  bool isIntervalContainReversed(OdGeInterval frstInt, OdGeInterval scndInt)
  {
    return Globals.OdGreaterOrEqual(frstInt.lowerBound() * -1, scndInt.upperBound(), m_toleranceInterval)
      && Globals.OdLessOrEqual(frstInt.upperBound() * -1, scndInt.lowerBound(), m_toleranceInterval);
  }

  #endregion Private
}