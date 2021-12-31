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
using System.Drawing;
using System.IO;
using System.Text.Json.Serialization;
using System.Collections.Generic;
using System.Linq;

using Teigha.TD;
using Teigha.Core;

using Rayon;
using Rayon.Core.Components;


namespace OdReadExSwigMgd
{

    class DbDumper
    {

        /************************************************************************/
        /****      RAYON PRINTS & DUMPS                                     *****/
        /************************************************************************/

        // GROUP

        void dumpObject(OdDbObjectId id, String itemName, Rayon.Core.Model newModel)
        {

            OdDbObject pObject = id.safeOpenObject();

            //GROUP
            if (pObject.isA().name() == "AcDbGroup")
            {
                OdDbGroup pGroup = (OdDbGroup)pObject;
                OdDbGroupIterator pIter = pGroup.newIterator();
                Console.WriteLine("Group ID: {0}", pGroup.handle());

                // new Group Element 
                string groupId = Guid.NewGuid().ToString();
                Rayon.Core.Element newGroup = new Rayon.Core.Element(newModel, groupId);
                newGroup
                    .With(new Rayon.Core.Components.GroupComp())
                    .With(new Rayon.Core.Components.NameComp("Group"))
                    .With(new Rayon.Core.Components.CategoryComp(Rayon.Core.Components.CategoryComp.CategoryEnum.Group))
                    .With(new Rayon.Core.Components.TransformComp());
                newModel.AddElement(newGroup);

                // adding groupIdComp to group children
                for (; !pIter.done(); pIter.next())
                {
                    Console.WriteLine("  ID: {0}", pIter.objectId().safeOpenObject().handle());

                    Rayon.Core.Components.GroupIdComp newGroupId =
                        new Rayon.Core.Components.GroupIdComp(newGroup.Handle);
                    Rayon.Core.Element childElement = newModel.Elements.Find(
                        x =>
                            x.Handle == pIter.objectId().safeOpenObject().handle().ToString()
                        );

                    childElement.With(newGroupId);
                }
                Console.WriteLine("  ");
            }

            if (pObject.isKindOf(OdDbDictionary.desc()))
            {
                OdDbDictionary pDic = (OdDbDictionary)pObject;
                OdDbDictionaryIterator pIter = pDic.newIterator();
                for (; !pIter.done(); pIter.next())
                {
                    dumpObject(pIter.objectId(), pIter.name(), newModel);
                }
            }

        }

        // print 2D-POLYLINE
        static void print2DPolyline(OdDbEntity pEnt, OdDbHandle parentID)
        {
            OdDb2dPolyline pPolyline = (OdDb2dPolyline)pEnt;
            Console.WriteLine("2DPOLYLINE");

            printBlockData(pEnt, parentID);
            Utils.printCurveData(pPolyline);
            OdDbObjectIterator pIter = pPolyline.vertexIterator();
            for (int i = 0; !pIter.done(); i++, pIter.step())
            {
                OdDb2dVertex pVertex = pIter.entity() as OdDb2dVertex;
                if (pVertex != null)
                {
                    Utils.print2dVertex(pVertex, i);
                }
            }
        }

        // print POLYLINE
        static void printPolyline(OdDbEntity pEnt, OdDbHandle parentID)
        {

            OdDbPolyline pPoly = (OdDbPolyline)pEnt;
            Console.WriteLine("POLYLINE");

            printBlockData(pEnt, parentID);
            Utils.printCurveData(pPoly);

            // verbs + points
            for (uint i = 0; i < (int)pPoly.numVerts(); i++)
            {
                Console.WriteLine("Point #" + i);
                Console.WriteLine("  ***");
                OdGePoint3d pt = new OdGePoint3d();
                pPoly.getPointAt(i, pt);

                // Begin
                if (i == 0)
                {
                    Console.WriteLine("  Point Type = {0}", "Begin");
                    Console.WriteLine("  ***");
                }

                // Line
                if (pPoly.vb_segType(i).ToString() == "kLine")
                {
                    Console.WriteLine("  Point Type = {0}", "LineTo");
                    Console.WriteLine("  Position = {0}", pt);
                    Console.WriteLine("  ***");
                }

                // Arc
                else if (pPoly.vb_segType(i).ToString() == "kArc")
                {
                    Console.WriteLine("  Point Type = {0}", "ArcTo");
                    Console.WriteLine("  Position = {0}", pt);
                    Console.WriteLine("  ***");
                    Console.WriteLine("  Point Type = {0}", "ArcTo");
                    Console.WriteLine(pPoly.numVerts());

                    OdGePoint3d arcMiddlePoint = Utils.getArcMiddlePt(pPoly, i);
                    Console.WriteLine("  Arc Mid. Point Position = {0} ", arcMiddlePoint);
                    Console.WriteLine("  ***");
                }

                // End Point
                else if (pPoly.vb_segType(i).ToString() == "kPoint")
                {
                    Console.WriteLine("  Point Type = {0}", "End");
                    Console.WriteLine("  Position = {0}", pt);
                    Console.WriteLine("  ***");
                }
            }
        }

        // TO DO : Debug Path function
        static void dumpPathFromPolyline(OdDbEntity pEnt, Rayon.Core.Model newModel, OdDbHandle parentID)
        {
            Console.WriteLine("  Running PathFromPolyline");

            //PROPERTIES
            // path layer
            Rayon.Core.Element thisElementLayer = Utils.getEntityLayer(pEnt, newModel);
            // style
            Rayon.Core.Element newStyle = Utils.getEntityStyle(pEnt, newModel);
            // ID
            string pathId = pEnt.handle().ToString();

            // PATH
            OdDbPolyline pPoly = (OdDbPolyline)pEnt;

            Rayon.Core.Components.PathComp newPathComp =
                new Rayon.Core.Components.PathComp();

            newPathComp.Points = new List<Rayon.Core.Types.RPoint2d>();

            // Closed
            newPathComp.Closed = pPoly.isClosed();

            Rayon.Core.Types.RPoint2d endPoint = new Rayon.Core.Types.RPoint2d(0, 0);

            var pointCounter = pPoly.isClosed() ? (int)pPoly.numVerts() + 1 : (int)pPoly.numVerts();

            Console.WriteLine("  Verbs & Points");
            Console.WriteLine("  ---");
            Console.WriteLine("  num. of points: {0}", pPoly.numVerts());
            Console.WriteLine("  is Pol. closed: {0}", pPoly.isClosed());

            Console.WriteLine("  ");
            Console.WriteLine("  AutoCad curve");

            for (uint i = 0; i < pPoly.numVerts(); i++)
            {
                Console.WriteLine("  {0}", pPoly.vb_segType(i).ToString());
                OdGePoint3d pt = new OdGePoint3d();
                pPoly.getPointAt(i, pt);
                Console.WriteLine("     X: {0}, Y: {0}", pt.x, pt.y);
                Console.WriteLine("     ...");
            }

            Console.WriteLine("  ");

            // Points & Verbs
            for (uint i = 0; i < pointCounter; i++)
            {
                OdGePoint3d pt = new OdGePoint3d();
                if (i < (int)pPoly.numVerts())
                {
                    pPoly.getPointAt(i, pt);
                }
                else
                {
                    pPoly.getPointAt(0, pt);
                }

                Console.WriteLine("   x: {0}", pt.x);
                Console.WriteLine("   y: {0}", pt.y);

                if (i == 0)
                {
                    Console.WriteLine("   pointVerb: Begin");
                    newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.Begin);
                    //endPoint = OdGePoint3dToRPoint2d(pt);

                    // Add to Points
                    Rayon.Core.Types.RPoint2d newPoint = Utils.OdGePoint3dToRPoint2d(pt);
                    newPathComp.Points.Add(newPoint);
                }
                else
                {
                    string segVerb = pPoly.vb_segType(i - 1).ToString();
                    Console.WriteLine("   pointVerb: {0}", segVerb);

                    // Line
                    if (segVerb == "kLine")
                    {
                        // Add to verbs
                        newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.LineTo);

                        // Add to Points
                        Rayon.Core.Types.RPoint2d newPoint = Utils.OdGePoint3dToRPoint2d(pt);
                        newPathComp.Points.Add(newPoint);
                    }

                    // Arc
                    if (segVerb == "kArc")
                    {
                        // get arc segment
                        OdGeCircArc2d pArc = new OdGeCircArc2d();

                        pPoly.getArcSegAt(i - 1, pArc);
                        // get all sub arcs and their resp. control points
                        List<Rayon.Core.Types.RPoint2d> points = Utils.ArcToCubicBezier(pArc);

                        // Add Points and Verbs
                        for (var j = 0; j < points.Count; j++)
                        {
                            // Add Points
                            newPathComp.Points.Add(points[j]);
                            // Add Verbs
                            if (j % 3 == 1)
                            {
                                newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.CubicTo);
                            }
                        }
                    }
                }

                Console.WriteLine("  ---");
            }

            // create new Rayon path
            Rayon.Core.Element newPath = Rayon.Core.Element.CreatePath(
                // Rayon model
                newModel,
                // path ID
                pathId,
                // path's Rayon layer
                thisElementLayer,
                // path's Rayon style
                newStyle,
                // path's name
                "Path",
                // path's comp
                newPathComp
                );

            // if in a block, link to parent block using parent's ID
            if (parentID.ToString() != DbDumper.ModelId)
            {
                Rayon.Core.Components.BlockIdComp newBlockIdComp = new Rayon.Core.Components.BlockIdComp(parentID.ToString());
                newPath.With(newBlockIdComp);
            }

            // test path Verb/point sequence
            testPath(newPathComp, pPoly.isClosed());

            // add path to model
            newModel.Elements.Add(newPath);

        }


        static void dumpPathFrom2dPolyline(OdDbEntity pEnt, Rayon.Core.Model newModel, OdDbHandle parentID)
        {

            Console.WriteLine("  Running PathFrom2DPolyline");

            OdDb2dPolyline pPolyline = (OdDb2dPolyline)pEnt;

            //  path id
            string pathId = pEnt.handle().ToString();

            // path name
            string pathName = "Path";

            // path layer
            Rayon.Core.Element thisElementLayer = Utils.getEntityLayer(pEnt, newModel);

            // path style
            Rayon.Core.Element newStyle = Utils.getEntityStyle(pEnt, newModel);

            Rayon.Core.Components.PathComp newPathComp = new Rayon.Core.Components.PathComp();

            OdDbObjectIterator pIter = pPolyline.vertexIterator();
            Rayon.Core.Types.RPoint2d endPoint = new Rayon.Core.Types.RPoint2d(0, 0);

            for (int i = 0; !pIter.done(); i++, pIter.step())
            {
                OdDb2dVertex pVertex = (OdDb2dVertex)pIter.entity();
                if (pVertex != null)
                {
                    // add point
                    Rayon.Core.Types.RPoint2d newPoint = Utils.OdDb2dVertexToRPoint2d(pVertex);
                    newPathComp.Points.Add(newPoint);

                    // add verb
                    string vertexType = pVertex.vertexType().ToString();
                    if (i == 0)
                    {
                        // begin
                        newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.Begin);
                        endPoint = Utils.OdDb2dVertexToRPoint2d(pVertex);
                    }
                    else
                    {
                        if (vertexType == "k2dVertex")
                        {
                            newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.LineTo);
                        }
                    }
                }
            }

            // repeat starting point
            if (pPolyline.isClosed())
            {
                newPathComp.Points.Add(endPoint);
                newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.LineTo);
            }

            // test path Verb/point sequence
            testPath(newPathComp, pPolyline.isClosed());

            //// to rayon
            Rayon.Core.Element newPath = Rayon.Core.Element.CreatePath(
                newModel,
                pathId,
                thisElementLayer,
                newStyle,
                pathName,
                newPathComp
            );

            // if in block, link to block with BlockID
            if (parentID.ToString() != DbDumper.ModelId)
            {
                Rayon.Core.Components.BlockIdComp newBlockIdComp = new Rayon.Core.Components.BlockIdComp(parentID.ToString());
                newPath.With(newBlockIdComp);
            }

            newModel.Elements.Add(newPath);

        }

        static void testPath(PathComp newPathComp, bool isPathClosed)
        {
            Console.WriteLine("  Testing Path Sequence...");

            Console.WriteLine("isPathClosed : {0}", isPathClosed);

            //test
            var count = 0;
            for (int i = 0; i < newPathComp.Verbs.Count; i++)
            {
                /// Begin
                if (newPathComp.Verbs[i].ToString() == "Begin")
                {
                    count = count + 1;
                }
                /// LineTo
                if (newPathComp.Verbs[i].ToString() == "LineTo")
                {
                    count = count + 1;
                }
                // QuadraticTo
                if (newPathComp.Verbs[i].ToString() == "QuadraticTo")
                {
                    count = count + 2;
                }
                // CubicTo
                if (newPathComp.Verbs[i].ToString() == "CubicTo")
                {
                    count = count + 3;
                }

                Console.WriteLine(newPathComp.Verbs[i].ToString());

            }

            for (int i = 0; i < newPathComp.Points.Count; i++)
            {
                Console.WriteLine("X:{0}", newPathComp.Points[i].X);
                Console.WriteLine("Y:{0}", newPathComp.Points[i].Y);
                Console.WriteLine("--");
            }

            Console.WriteLine("Point Count: {0}", newPathComp.Points.Count);
            Console.WriteLine("Verbs Count: {0}", newPathComp.Verbs.Count);
            Console.WriteLine("Count: {0}", count);

            if (count != newPathComp.Points.Count)
            {
                throw new InvalidOperationException("  Corrupted Path Sequence");
            }
            else
            {
                Console.WriteLine("  ...Test Passed!");
            }
        }

        static void printCircle(OdDbEntity pEnt, OdDbHandle parentID)
        {
            Console.WriteLine("CIRCLE");
            OdDbCircle pCircle = (OdDbCircle)pEnt;

            printBlockData(pEnt, parentID);
            Utils.printCurveData(pCircle);

            Console.WriteLine("  Center = {0}", pCircle.center().ToString());
            Console.WriteLine("  Radius = {0}", pCircle.radius());
            Console.WriteLine("  Thickness = {0}", pCircle.thickness());
        }

        static void printLine(OdDbEntity pEnt, OdDbHandle parentId)
        {
            Console.WriteLine("LINE");
            OdDbLine pLine = (OdDbLine)pEnt;

            printBlockData(pEnt, parentId);
            Utils.printCurveData(pLine);

        }

        static void dumpPathFromLine(OdDbEntity pEnt, Rayon.Core.Model newModel, OdDbHandle parentID)
        {

            OdDbLine pLine = (OdDbLine)pEnt;

            // path id
            string pathId = pEnt.handle().ToString();

            // path layer
            Rayon.Core.Element thisElementLayer = Utils.getEntityLayer(pEnt, newModel);

            // path style
            // BUG HERE
            Rayon.Core.Element newStyle = Utils.getEntityStyle(pEnt, newModel);

            Rayon.Core.Components.PathComp newPathComp = new Rayon.Core.Components.PathComp();

            // begin
            newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.Begin);

            //add start Point
            Rayon.Core.Types.RPoint2d startPoint = Utils.OdGePoint3dToRPoint2d(pLine.startPoint());
            newPathComp.Points.Add(startPoint);
            newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.LineTo);

            //add end Point
            Rayon.Core.Types.RPoint2d endPoint = Utils.OdGePoint3dToRPoint2d(pLine.endPoint());
            newPathComp.Points.Add(endPoint);

            // to rayon
            Rayon.Core.Element newPath = Rayon.Core.Element.CreatePath(
                newModel,
                pathId,
                thisElementLayer,
                newStyle,
                "Path",
                newPathComp
            );

            // if in block, link to block with BlockID
            if (parentID.ToString() != DbDumper.ModelId)
            {
                Rayon.Core.Components.BlockIdComp newBlockIdComp = new Rayon.Core.Components.BlockIdComp(parentID.ToString());
                newPath.With(newBlockIdComp);
            }

            newModel.Elements.Add(newPath);

        }

        static void printArc(OdDbEntity pEnt, OdDbHandle parentId)
        {
            Console.WriteLine("ARC");
            OdDbArc pArc = (OdDbArc)pEnt;
            Utils.printCurveData(pArc);
            printBlockData(pEnt, parentId);
            Console.WriteLine("  Center = {0}", pArc.center().ToString());
            Console.WriteLine("  Radius = {0}", pArc.radius().ToString());
            Console.WriteLine("  Start Angle = {0}", OdExStringHelpers.toDegreeString(pArc.startAngle()));
            Console.WriteLine("  End Angle = {0}", OdExStringHelpers.toDegreeString(pArc.endAngle()));
            Console.WriteLine("  Thickness = {0}", pArc.thickness().ToString());
        }

        static void dumpArc(OdDbEntity pEnt, Rayon.Core.Model newModel, OdDbHandle parentID)
        {

            // path id
            string circleId = pEnt.handle().ToString();

            // path layer
            Rayon.Core.Element thisElementLayer = Utils.getEntityLayer(pEnt, newModel);

            // path style
            Rayon.Core.Element newStyle = Utils.getEntityStyle(pEnt, newModel);

            // center point + radius
            Rayon.Core.Types.RPoint2d centerPoint = new Rayon.Core.Types.RPoint2d(0, 0);
            Rayon.Core.Types.RVector radius = new Rayon.Core.Types.RVector(0, 0);
            string arcName = "undefined";
            bool isClosed = false;

            // if ellispse
            if (pEnt.GetType().ToString() == "Teigha.TD.OdDbEllipse")
            {
                OdDbEllipse pEllipse = (OdDbEllipse)pEnt;
                double majorAxis = pEllipse.majorAxis().length() > pEllipse.minorAxis().length() ? pEllipse.majorAxis().length() : pEllipse.minorAxis().length();
                double minorAxis = pEllipse.majorAxis().length() < pEllipse.minorAxis().length() ? pEllipse.majorAxis().length() : pEllipse.minorAxis().length();
                Console.WriteLine("Major Axis {0}:", pEllipse.majorAxis().length());
                Console.WriteLine("Minor Axis {0}:", pEllipse.minorAxis().length());
                radius = Utils.getEllipseRadii(majorAxis, minorAxis);
                centerPoint = Utils.OdGePoint3dToRPoint2d(pEllipse.center());
                arcName = "Ellipse";
                isClosed = true;
            }
            //if circle
            if (pEnt.GetType().ToString() == "Teigha.TD.OdDbCircle")
            {
                OdDbCircle pCircle = (OdDbCircle)pEnt;
                radius = Utils.getArcRadii(pCircle.radius());
                centerPoint = Utils.OdGePoint3dToRPoint2d(pCircle.center());
                arcName = "Circle";
                isClosed = true;
            }
            //if Arc
            if (pEnt.GetType().ToString() == "Teigha.TD.OdDbArc")
            {
                OdDbArc pArc = (OdDbArc)pEnt;
                radius = Utils.getArcRadii(pArc.radius());
                centerPoint = Utils.OdGePoint3dToRPoint2d(pArc.center());
                arcName = "Arc";
            }

            Rayon.Core.Components.ArcComp newArcComp =
                new Rayon.Core.Components.ArcComp(centerPoint, radius);

            if (pEnt.GetType().ToString() == "Teigha.TD.OdDbEllipse")
            {
                OdDbEllipse pEllipse = (OdDbEllipse)pEnt;
                OdGeVector3d firstDeriv = new OdGeVector3d(0, 0, 0);
                pEllipse.getFirstDeriv(0, firstDeriv);
                OdGeVector3d normalVector = ((OdDbEllipse)pEnt).normal();
                double xRotation = Math.Atan2(firstDeriv.x - 0, firstDeriv.y - 1);
                newArcComp.XRotation = new Rayon.Core.Types.RAngle(Math.PI - xRotation);
            }

            if (!isClosed)
            {

                OdDbArc pArc = (OdDbArc)pEnt;

                // sweepAngle
                double sweepAngle = pArc.startAngle() < pArc.endAngle() ? pArc.endAngle() - pArc.startAngle() : 2*Math.PI - pArc.startAngle() + pArc.endAngle();
                newArcComp.SweepAngle = new Rayon.Core.Types.RAngle(sweepAngle);

                // startAngle
                newArcComp.StartAngle = pArc.normal().z == -1 ? 
                    new Rayon.Core.Types.RAngle(Math.PI - pArc.endAngle()) :
                    new Rayon.Core.Types.RAngle(pArc.startAngle());

            }

            // to rayon
            Rayon.Core.Element newArc = Rayon.Core.Element.CreateArc(
                newModel,
                circleId,
                thisElementLayer,
                newStyle,
                arcName,
                newArcComp
             );

            // if in block, link to block with BlockID
            if (parentID.ToString() != DbDumper.ModelId)
            {
                Rayon.Core.Components.BlockIdComp newBlockIdComp = new Rayon.Core.Components.BlockIdComp(parentID.ToString());
                newArc.With(newBlockIdComp);
            }

            newModel.Elements.Add(newArc);
        }

        static void printSpline(OdDbEntity pEnt, Rayon.Core.Model newModel, OdDbHandle parentId)
        {
            Console.WriteLine("SPLINE");
            OdDbSpline pSpline = (OdDbSpline)pEnt;
            Utils.printCurveData(pSpline);
        }

        static void dumpSpline(OdDbEntity pEnt, Rayon.Core.Model newModel, OdDbHandle parentId)
        {

            OdDbSpline pSpline = OdDbSpline.cast(pEnt);
            // return if cast failed
            if (pSpline == null)
            {
                Console.WriteLine("cast OdDbEntity>OdDbSpline failed!");
                return;
            }

            OdDbCurve newCurve = OdDbPolyline.createObject();

            // precision value between 0 and 100%
            int precision = 1;
            OdResult conversionResult = pSpline.toPolyline(ref newCurve, true, true, precision);

            //return if conversion failed
            if (conversionResult.ToString() != "eOk") 
            {
                Console.WriteLine("toPolyline Conversion failed!");
                return;
            }

            OdDbPolyline pPoly = OdDbPolyline.cast(newCurve);
            // return if cast failed
            if (pPoly == null)
            {
                Console.WriteLine("cast OdDbCurve>OdDbPolyline failed!");
                return;
            }

            Rayon.Core.Components.PathComp newPathComp = new Rayon.Core.Components.PathComp();
            Rayon.Core.Types.RPoint2d closePoint = new Rayon.Core.Types.RPoint2d(0, 0);

            newPathComp.Closed = pPoly.isClosed();
            

            // path layer
            Rayon.Core.Element thisElementLayer = Utils.getEntityLayer(pEnt, newModel);
            // Style
            Rayon.Core.Element newStyle = Utils.getEntityStyle(pEnt, newModel);
            // ID
            string pathId = pEnt.handle().ToString();

            Console.WriteLine("***Test-{0}***", 4);

            Console.WriteLine("*** {0} ***", pPoly.numVerts());

            for (var i = 0; i < pPoly.numVerts()-1; i++)
            {
                OdGePoint3d pt = new OdGePoint3d();
                pPoly.getPointAt((uint)i, pt);

                // Arc
                if (i == 0)
                {
                    newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.Begin);
                    closePoint = Utils.OdGePoint3dToRPoint2d(pt);
                }

                if (pPoly.vb_segType((uint)i).ToString() == "kArc")
                {
                    // get arc segment
                    OdGeCircArc2d pArc = new OdGeCircArc2d();
                    pPoly.getArcSegAt((uint)i, pArc);

                    // get all sub arcs and their resp. control points
                    List<Rayon.Core.Types.RPoint2d> points = Utils.ArcToCubicBezier(pArc);

                    // Add Points and Verbs
                    for (var j = 0; j < points.Count; j++)
                    {
                        // Add Points
                        newPathComp.Points.Add(points[j]);
                        // Add Verbs
                        if (j % 3 == 1)
                        {
                            newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.CubicTo);
                        }
                    }
                }

                if (pPoly.vb_segType((uint)i).ToString() == "kLine") 
                {
                    Rayon.Core.Types.RPoint2d pointLine = Utils.OdGePoint3dToRPoint2d(pt);
                    newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.LineTo);
                    newPathComp.Points.Add(pointLine);
                }

                // End Point
                if (pPoly.vb_segType((uint)i).ToString() == "kPoint")
                {
                    //Add to Points
                    Rayon.Core.Types.RPoint2d newPoint = Utils.OdGePoint3dToRPoint2d(pt);
                    newPathComp.Points.Add(newPoint);
                }
            }

            // append endpoint
            OdGePoint3d ptFinal = new OdGePoint3d();
            pPoly.getPointAt((uint)pPoly.numVerts() - 1, ptFinal);
            Rayon.Core.Types.RPoint2d finalPoint = Utils.OdGePoint3dToRPoint2d(ptFinal);
            newPathComp.Points.Add(finalPoint);

            if (pSpline.isClosed())
            {
                newPathComp.Points.Add(closePoint);
            }

            Rayon.Core.Element newPath = Rayon.Core.Element.CreatePath(
                newModel,
                pathId,
                thisElementLayer,
                newStyle,
                "Path",
                newPathComp
                );

            // if in block, link to block with BlockID
            if (parentId.ToString() != DbDumper.ModelId)
            {
                Rayon.Core.Components.BlockIdComp newBlockIdComp = new Rayon.Core.Components.BlockIdComp(parentId.ToString());
                newPath.With(newBlockIdComp);
            }

            newModel.Elements.Add(newPath);

        }

        static void printEllipse(OdDbEntity pEnt, OdDbHandle parentId)
        {
            Console.WriteLine("ELLIPSE");
            OdDbEllipse pEllipse = (OdDbEllipse)pEnt;
            Utils.printCurveData(pEllipse);
            printBlockData(pEnt, parentId);

            Console.WriteLine("  Center = {0}", pEllipse.center().ToString());
            Console.WriteLine("  Major Axis = {0}", pEllipse.majorAxis().ToString());
            Console.WriteLine("  Minor Axis = {0}", pEllipse.minorAxis().ToString());
            Console.WriteLine("  Major Radius = {0}", pEllipse.majorAxis().length().ToString());
            Console.WriteLine("  Minor Radius = {0}", pEllipse.minorAxis().length().ToString());
            Console.WriteLine("  Radius Ratio = {0}", pEllipse.radiusRatio().ToString());
            Console.WriteLine("  Start Angle = {0}", OdExStringHelpers.toDegreeString(pEllipse.startAngle()));
            Console.WriteLine("  End Angle = {0}", OdExStringHelpers.toDegreeString(pEllipse.endAngle()));

        }

        static void dumpHatch(OdDbEntity pEnt, Rayon.Core.Model newModel, OdDbHandle parentId)
        {

            // TO DO add arc to hatch contour (with ArcToBezier)
            OdDbHatch pHatch = (OdDbHatch)pEnt;
            OdGePoint2dArray pts = new OdGePoint2dArray();
            EdgeArray edges = new EdgeArray();

            // only process handled hatch type ("kNormal")
            if (pHatch.hatchStyle().ToString() == "kNormal")
            {
                // for each loop in hatch
                int numLoops = pHatch.numLoops();

                // ...get points in FIRST OUTER loop
                pHatch.getLoopAt(0, pts, new OdGeDoubleArray());


                Console.WriteLine("TEST 3");

                OdGePoint2dArray.OdGePoint2dArrayEnumerator ptEnum = pts.GetEnumerator();

                Console.WriteLine("TEST 4");

                Rayon.Core.Components.PathComp newPathComp = new Rayon.Core.Components.PathComp();

                newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.Begin);

                int i = 0;
                Rayon.Core.Types.RPoint2d endPoint = new Rayon.Core.Types.RPoint2d(0, 0);

                Console.WriteLine("TEST 5");
                while (ptEnum.MoveNext())
                {
                    OdGePoint2d pt = ptEnum.Current;

                    if (i == 0)
                    {
                        endPoint = new Rayon.Core.Types.RPoint2d(pt.x, pt.y);
                    }

                    newPathComp.Verbs.Add(Rayon.Core.Components.PathComp.PathVerbEnum.LineTo);
                    newPathComp.Points.Add(new Rayon.Core.Types.RPoint2d(pt.x, pt.y));
                    i = i + 1;
                }

                newPathComp.Points.Add(endPoint);

                // Hatch layer
                Rayon.Core.Element thisElementLayer = Utils.getEntityLayer(pEnt, newModel);

                // Hatch style name
                string styleName = "Hatch_Style";

                //Hatch color
                Rayon.Core.Types.RColor hatchColor = new Rayon.Core.Types.RColor(0, 0, 0, 1);
                //if inherited from layer
                if (pHatch.color().ToString() == "ByLayer")
                {
                    // find hatch layer
                    Rayon.Core.Element layerOfEnt = newModel.Elements.Find(
                        x =>
                            x.Handle == pEnt.layerId().getHandle().ToString()
                        );
                    Console.WriteLine(layerOfEnt.Handle);
                    layerOfEnt.Components.ForEach(delegate (Component item)
                    {
                        Console.WriteLine("     LAYER HANDLE: {0}", item.Handle);
                    });
                    // get layer Comp
                    Rayon.Core.Components.Component layerComp = layerOfEnt.Components.Find(
                        x =>
                            x.ComponentType == Component.ComponentTypeEnum.Layer
                        );
                    Console.WriteLine(layerComp);
                    // get layer Color
                    hatchColor = ((Rayon.Core.Components.LayerComp)layerComp.Value).Color;

                    if (hatchColor == null)
                    {
                        double[,] Colors = Utils.getDwgColors();
                        int colorIndex = pHatch.colorIndex();
                        hatchColor = new Rayon.Core.Types.RColor(Colors[colorIndex, 0] / 255, Colors[colorIndex, 1] / 255, Colors[colorIndex, 2] / 255, 1);
                    }
                }
                // else create new style based on custom color
                else
                {
                    double[,] Colors = Utils.getDwgColors();
                    int colorIndex = pHatch.colorIndex();
                    hatchColor = new Rayon.Core.Types.RColor(Colors[colorIndex, 0] / 255, Colors[colorIndex, 1] / 255, Colors[colorIndex, 2] / 255, 1);
                }

                Console.WriteLine(hatchColor);

                // Hatch style
                string hatchStyleName = "Hatch_Style";
                // FillComp > RETROFIT
                Rayon.Core.Components.Styles.FillStyleComp fillComp = new Rayon.Core.Components.Styles.FillStyleComp(
                    hatchColor,
                    "texture"
                    );

                Rayon.Core.Element hatchStyle = new Rayon.Core.Element(newModel);
                hatchStyle
                    .With(fillComp)
                    .With(new NameComp(hatchStyleName));

                newModel.Elements.Add(hatchStyle);

                // Hatch Element 
                string pathId = pEnt.handle().ToString();

                Rayon.Core.Element newHatch = Rayon.Core.Element.CreatePath(
                    newModel,
                    pathId,
                    thisElementLayer,
                    hatchStyle,
                    "Hatch",
                    newPathComp
                   );

                if (parentId.ToString() != DbDumper.ModelId)
                {
                    Rayon.Core.Components.BlockIdComp newBlockIdComp = new Rayon.Core.Components.BlockIdComp(parentId.ToString());
                    newHatch.With(newBlockIdComp);
                }

                newModel.Elements.Add(newHatch);
            }
        }

        static void printBlockData(OdDbEntity pEnt, OdDbHandle parentID)
        {
            Console.WriteLine("  ParentID = {0}", parentID);
            Console.WriteLine("  ID = {0}", pEnt.handle());
        }

        static void dumpBlock(OdDbBlockTableRecord pBlock, Rayon.Core.Model newModel, OdDbHandle parentId)
        {

            // new Block Comp
            BlockComp newBlockComp = new Rayon.Core.Components.BlockComp();

            // new Parent Block
            Rayon.Core.Element newBlock = new Rayon.Core.Element(newModel, pBlock.handle().ToString())
                .With(new Rayon.Core.Components.NameComp(pBlock.getName()))
                .With(new Rayon.Core.Components.BboxComp())
                .With(new Rayon.Core.Components.CategoryComp(CategoryComp.CategoryEnum.Block))
                .With(newBlockComp);

            // add to Model
            newModel.Elements.Add(newBlock);
        }

        static void dumpBlockInstance(OdDbEntity pEnt, Rayon.Core.Model newModel, OdDbHandle parentId)
        {
            OdDbBlockReference pBlkRef = (OdDbBlockReference)pEnt;
            OdDbBlockTableRecord pRecord = pBlkRef.blockTableRecord().openObject() as OdDbBlockTableRecord;

            // get block layer
            Rayon.Core.Element entLayer = Utils.getEntityLayer(pEnt, newModel);

            // get transform matrix
            System.Drawing.Drawing2D.Matrix newMat = new System.Drawing.Drawing2D.Matrix();

            Console.WriteLine("Rot: {0}", pBlkRef.rotation());
            Console.WriteLine("Scale: {0} , {1}", pBlkRef.scaleFactors().sx, pBlkRef.scaleFactors().sy);
            Console.WriteLine("Translate: {0} , {1}, {2}", pBlkRef.position().x, pBlkRef.position().y, pBlkRef.position().z);

            newMat.Rotate((float)(pBlkRef.rotation() * 180 / Math.PI));
            newMat.Scale((float)pBlkRef.scaleFactors().sx, (float)pBlkRef.scaleFactors().sy, System.Drawing.Drawing2D.MatrixOrder.Append);
            newMat.Translate((float)pBlkRef.position().x, (float)pBlkRef.position().y, System.Drawing.Drawing2D.MatrixOrder.Append);

            Console.WriteLine(pBlkRef.blockTransform().getCsXAxis());
            Console.WriteLine(pBlkRef.blockTransform().getCsYAxis());
            Console.WriteLine(pBlkRef.blockTransform().getCsZAxis());

            Console.WriteLine(pBlkRef.blockTransform());
            OdGeMatrix3d matTransform = pBlkRef.blockTransform();

            List<double> transformValues = new List<double> {


            matTransform[1,0],
            matTransform[1,1],

            matTransform[0,0],
            matTransform[0,1],

            //matTransform[3,1],
            //matTransform[3,0]
            0,0
            };

            //List<double> transformValues = new List<double> {
            //newMat.Elements[0],
            //newMat.Elements[1],
            //newMat.Elements[2],
            //newMat.Elements[3],
            //newMat.Elements[4],
            //newMat.Elements[5]
            //};

            // Instance 
            BlockInstanceComp newBlockInstanceComp = new BlockInstanceComp(pRecord.objectId().getHandle().ToString());
            // new Block Instance
            Rayon.Core.Element newInstanceBlock = new Rayon.Core.Element(newModel, pEnt.handle().ToString())
                .With(new Rayon.Core.Components.NameComp(pRecord.getName() + " Instance"))
                .With(new Rayon.Core.Components.BboxComp())
                .With(new Rayon.Core.Components.CategoryComp(CategoryComp.CategoryEnum.BlockInstance))
                .With(new Rayon.Core.Components.LayerIdComp(entLayer))
                .With(new Rayon.Core.Components.TransformComp(transformValues))
                .With(newBlockInstanceComp);

            if (parentId.ToString() != DbDumper.ModelId)
            {
                Rayon.Core.Components.BlockIdComp newBlockIdComp = new Rayon.Core.Components.BlockIdComp(parentId.ToString());
                newInstanceBlock.With(newBlockIdComp);
            }

            // add Instance to Model
            newModel.Elements.Add(newInstanceBlock);

        }

        static void printText(OdDbEntity pEnt)
        {
            Console.WriteLine("TEXT");
            bool bold;
            bool italic;
            int charset;
            int pitchAndFamily;
            string fontName = "";

            OdDbText pText = (OdDbText)pEnt;

            OdDbTextStyleTableRecord pRecord = (OdDbTextStyleTableRecord)pText.textStyle().openObject();
            pRecord.font(ref fontName, out bold, out italic, out charset, out pitchAndFamily);

            Console.WriteLine("  ID : {0}", pText.handle());
            Console.WriteLine("  Font Name : {0}", fontName);
            Console.WriteLine("  isBold : {0}", bold);
            Console.WriteLine("  isItalic : {0}", italic);
            Console.WriteLine("  Font Size : {0}", pText.height());
            Console.WriteLine("  Text Content : {0}", pText.textString());
            Console.WriteLine("  Text X Position : {0}", pText.position().x);
            Console.WriteLine("  Text Y Position : {0}", pText.position().y);
            Console.WriteLine("  Layer : {0}", pText.layerId().getHandle().ToString());
        }

        static void dumpText(OdDbEntity pEnt, Rayon.Core.Model newModel, OdDbHandle parentID)
        {

            OdDbText pText = (OdDbText)pEnt;

            bool bold;
            bool italic;
            int charset;
            int pitchAndFamily;
            string fontName = "";

            OdDbTextStyleTableRecord pRecord = (OdDbTextStyleTableRecord)pText.textStyle().openObject();
            pRecord.font(ref fontName, out bold, out italic, out charset, out pitchAndFamily);

            string content = pText.textString();
            double height = (double)pText.height();
            double width = (double)pText.height();
            Rayon.Core.Types.RPoint2d position = new Rayon.Core.Types.RPoint2d((double)pText.position().x, (double)pText.position().y);
            Rayon.Core.Element thisElementLayer = Utils.getEntityLayer(pEnt, newModel);

            // Text Element
            Rayon.Core.Components.TextComp textComp = new Rayon.Core.Components.TextComp(
                content,
                100,
                100,
                position,
                TextComp.AnchorEnum.BottomLeft
                );

            // get bounding box
            OdGePoint3dArray ptsBBox = new OdGePoint3dArray();
            pText.getBoundingPoints(ptsBBox);

            Rayon.Core.Element textElement = new Rayon.Core.Element(newModel, pText.handle().ToString())
                .With(textComp)
                .With(new Rayon.Core.Components.StyleIdComp(pText.textStyle().getHandle().ToString()))
                .With(new Rayon.Core.Components.BboxComp(
                    new Rayon.Core.Types.RPoint2d(ptsBBox[2].x, ptsBBox[2].y),
                    new Rayon.Core.Types.RPoint2d(ptsBBox[1].x, ptsBBox[1].y),
                    true
                    )
                )
                .With(new Rayon.Core.Components.RenderBufferComp())
                .With(new TransformComp())
                .With(new CategoryComp(CategoryComp.CategoryEnum.Text))
                .With(new NameComp("Text"))
                .With(new LayerIdComp(thisElementLayer));

            Console.WriteLine("&&&&& TYPEEE {0}", textElement.GetType());

            newModel.Elements.Add(textElement);

        }

        static void printMText(OdDbEntity pEnt)
        {
            Console.WriteLine("M-TEXT");
            OdDbMText pText = (OdDbMText)pEnt;

            bool bold;
            bool italic;
            int charset;
            int pitchAndFamily;
            string fontName = "";
            OdDbTextStyleTableRecord pRecord = (OdDbTextStyleTableRecord)pText.textStyle().openObject();
            pRecord.font(ref fontName, out bold, out italic, out charset, out pitchAndFamily);

            string[] subString = pText.contents().Split(new string[] { "\\P" }, StringSplitOptions.None);
            string content = "";
            for (var i = 0; i < subString.Length; i++)
            {
                content = content + subString[i];
                if (i < subString.Length - 1)
                {
                    content = content + "\n";
                }
            }

            OdGePoint3dArray ptsArray = new OdGePoint3dArray();
            pText.getBoundingPoints(ptsArray);

            Console.WriteLine("  ID : {0}", pText.handle());
            Console.WriteLine("  Font Name : {0}", fontName);
            Console.WriteLine("  isBold : {0}", bold);
            Console.WriteLine("  isItalic : {0}", italic);
            Console.WriteLine("  Font Size : {0}", pText.height());
            Console.WriteLine("  Text Content : {0}", content);
            Console.WriteLine("  Text X Position : {0}", ptsArray[0].x);
            Console.WriteLine("  Text Y Position : {0}", ptsArray[0].y);
            Console.WriteLine("  Layer : {0}", pText.layerId().getHandle().ToString());

        }

        static void dumpMText(OdDbEntity pEnt, Rayon.Core.Model newModel, OdDbHandle parentID)
        {
            OdDbMText pText = (OdDbMText)pEnt;

            bool bold;
            bool italic;
            int charset;
            int pitchAndFamily;
            string fontName = "";
            OdDbTextStyleTableRecord pRecord = (OdDbTextStyleTableRecord)pText.textStyle().openObject();
            pRecord.font(ref fontName, out bold, out italic, out charset, out pitchAndFamily);

            string[] subString = pText.contents().Split(new string[] { "\\P" }, StringSplitOptions.None);
            string content = "";
            for (var i = 0; i < subString.Length; i++)
            {
                content = content + subString[i];
                if (i < subString.Length - 1)
                {
                    content = content + "\n";
                }
            }

            OdGePoint3dArray ptsArray = new OdGePoint3dArray();
            pText.getBoundingPoints(ptsArray);

            double height = (double)pText.height();
            double width = (double)pText.height();

            Rayon.Core.Types.RPoint2d position = new Rayon.Core.Types.RPoint2d((double)ptsArray[0].x, (double)ptsArray[0].y);
            Rayon.Core.Element thisElementLayer = Utils.getEntityLayer(pEnt, newModel);

            // Text Element
            Rayon.Core.Components.TextComp textComp = new Rayon.Core.Components.TextComp(
                content,
                100,
                100,
                position,
                TextComp.AnchorEnum.TopLeft
                );

            // get bounding box
            OdGePoint3dArray ptsBBox = new OdGePoint3dArray();
            pText.getBoundingPoints(ptsBBox);

            string textStyleId = pText.textStyle().getHandle().ToString();

            Rayon.Core.Element textElement = new Rayon.Core.Element(newModel, pText.handle().ToString())
                .With(textComp)
                .With(new Rayon.Core.Components.StyleIdComp(textStyleId))
                .With(new Rayon.Core.Components.BboxComp(
                    new Rayon.Core.Types.RPoint2d(ptsBBox[2].x, ptsBBox[2].y),
                    new Rayon.Core.Types.RPoint2d(ptsBBox[1].x, ptsBBox[1].y),
                    true
                    )
                )
                .With(new Rayon.Core.Components.RenderBufferComp())
                .With(new TransformComp())
                .With(new CategoryComp(CategoryComp.CategoryEnum.Text))
                .With(new NameComp("Text"))
                .With(new LayerIdComp(thisElementLayer));

            newModel.Elements.Add(textElement);

        }
        static void dumpDimension(OdDbEntity pEnt, Rayon.Core.Model newModel, OdDbHandle parentID)
        {
            OdDbRotatedDimension pDim = (OdDbRotatedDimension)pEnt;
            Console.WriteLine("handle dim {0}", pDim.dimBlockId().openObject().handle());
        }







        /************************************************************************/
        /****      RAYON MAIN                                               *****/
        /************************************************************************/

        void dumpRayonMetadata(OdDbDatabase pDb, Rayon.Core.Model newModel)
        {
            DateTime creationDate = DateTime.Now;
            DateTime updateDate = DateTime.Now;
            string fileName = pDb.getFilename();
            Console.WriteLine("  File Name: = {0}", pDb.getFilename());
            Console.WriteLine("  File DWG Version: = {0}", (pDb.originalFileVersion()));
            Console.WriteLine("  Cretion Date: {0}", creationDate);
            Console.WriteLine("  Update Date: {0}", updateDate);
            Console.WriteLine("  Model Unit System: {0}", Utils.getUnitSystem(pDb.getINSUNITS().ToString()).ToString());
            Console.WriteLine(" ");

            // add metadata to Model
            newModel.Name = pDb.getFilename();
            newModel.CreatedAt = creationDate;
            newModel.UpdatedAt = updateDate;
            newModel.SetSettings(Utils.getUnitSystem(pDb.getINSUNITS().ToString()));
        }

        void dumpRayonLayersAndStyles(OdDbDatabase pDb, Rayon.Core.Model newModel)
        {

            OdDbLayerTable pTable = (OdDbLayerTable)pDb.getLayerTableId().safeOpenObject();
            OdDbSymbolTableIterator pIter = pTable.newIterator();

            int zIndex = 1;
            double defaultLineWidth = 1;

            // add layers and their styles to the model
            for (pIter.start(); !pIter.done(); pIter.step())
            {

                OdDbLayerTableRecord pRecord = (OdDbLayerTableRecord)pIter.getRecordId().safeOpenObject();

                if (pRecord.GetType().ToString() == "Teigha.TD.OdDbLayerTableRecord")
                {

                    // new Color
                    Rayon.Core.Types.RColor defaultBlack = new Rayon.Core.Types.RColor(0, 0, 0, 1);

                    // new Style
                    double newLineWidth = (double)pRecord.lineWeight() < 0 ? defaultLineWidth : (double)pRecord.lineWeight() / 100;

                    string styleName = "Style " + pRecord.getName();
                    StrokeStyleComp.StrokePatternEnum newLineStyle = Utils.getStrokeStyleRecord(pRecord);

                    Rayon.Core.Element newStyle = Rayon.Core.Element.CreateStrokeStyle(
                        newModel,
                        styleName,
                        defaultBlack,
                        newLineWidth,
                        newLineStyle
                        );

                    newModel.Elements.Add(newStyle);

                    Console.WriteLine();
                    Console.WriteLine("STYLE");
                    Console.WriteLine("  Style Name = {0}", styleName);
                    Console.WriteLine("  ID = {0}", newStyle.Handle);
                    Console.WriteLine("  Linetype = {0}", OdExStringHelpers.toString(pRecord.linetypeObjectId()));
                    Console.WriteLine("  Lineweight = {0}", (pRecord.lineWeight()));
                    Console.WriteLine("  Lineweight = {0}", (double)pRecord.lineWeight());

                    // new Layer
                    string layerName = pRecord.getName();
                    bool isHidden = pRecord.isOff();
                    bool isFrozen = pRecord.isLocked();
                    //value between 0 and 1
                    double layerZIndex = zIndex;
                    string layerId = pRecord.handle().ToString();
                    Rayon.Core.Types.RColor newLayerDisplayColor = Utils.getColor(pRecord);

                    var newLayer = new Rayon.Core.Element(newModel, layerId)
                        .With(new LayerComp(false, newLayerDisplayColor))
                        .With(new NameComp(layerName))
                        .With(new LayerIdComp(null, layerZIndex))
                        .With(new CategoryComp(CategoryComp.CategoryEnum.Layer))
                        .With(new StyleIdComp(newStyle));


                    if (isHidden)
                    {
                        newLayer.With(new HiddenComp());
                    }
                    if (isFrozen)
                    {
                        newLayer.With(new LockedComp());
                    }

                    newModel.Elements.Add(newLayer);

                    zIndex = zIndex + 1;

                    // Dump information
                    Console.WriteLine();
                    Console.WriteLine("LAYER");
                    Console.WriteLine("  Name = {0}", layerName);
                    Console.WriteLine("  ID = {0}", layerId);
                    Console.WriteLine("  On = {0}", (!pRecord.isOff()));
                    Console.WriteLine("  Locked = {0}", (pRecord.isLocked()));
                    Console.WriteLine("  Associateed Style ID = {0}", newStyle.Handle);

                }
                Console.WriteLine(" ");
            }

            // add generic block style to the model
            Rayon.Core.Element entStyle = Rayon.Core.Element.CreateStrokeStyle(
                // model
                newModel,
                // styleName
                "Default_Block_Style",
                //color
                new Rayon.Core.Types.RColor(0, 0, 0, 1),
                //line width,
                1,
                //line style
                StrokeStyleComp.StrokePatternEnum.Solid
                );

            Utils.blockStyleId = entStyle.Handle.ToString();


            newModel.Elements.Add(entStyle);
        }

        // printing DWG elements and adding them to Rayon model
        void dumpRayonBlocks(OdDbDatabase pDb, Rayon.Core.Model newModel)
        {

            OdDbBlockTable pTable = (OdDbBlockTable)pDb.getBlockTableId().safeOpenObject();
            OdDbSymbolTableIterator pBlkIter = pTable.newIterator();
            // Get Model Space ID
            OdDbHandle modelSpaceID = pTable.getModelSpaceId().safeOpenObject().handle();
            int i = 0;
            bool inBlock = false;

            // iterate through RECORDS in BLOCK RECORD TABLE
            for (pBlkIter.start(); !pBlkIter.done(); pBlkIter.step())
            {

                OdDbBlockTableRecord pBlock = (OdDbBlockTableRecord)pBlkIter.getRecordId().safeOpenObject();
                OdDbObjectIterator pEntIter = pBlock.newIterator();
                OdDbHandle parentId = new OdDbHandle();
                parentId = pBlkIter.getRecordId().safeOpenObject().handle();
                // if first block in list, block is the model space
                if (i == 0)
                {
                    Console.WriteLine(">> NODES IN MODEL SPACE");
                    Console.WriteLine(" ");
                    inBlock = true;
                }
                // otherwise block is not in the model space but exist in the model
                else if (i > 0 & inBlock == true)
                {
                    Console.WriteLine(">> OTHER NODES");
                    Console.WriteLine(" ");
                    inBlock = false;
                }
                // get and store Model space ID
                if (pBlock.getName().ToString() == "*Model_Space")
                {
                    Console.WriteLine("Model Space ID: {0}", pBlock.handle());
                    DbDumper.ModelId = pBlock.handle().ToString();
                    Console.WriteLine("  ");
                }
                // process Block (excluding Paper Space blocks)
                //else if (!pBlock.getName().ToString().Contains("*Paper_Space"))
                //{

                //    Console.WriteLine(" ");
                //    Console.WriteLine("### NEW NODE ###");
                //    Console.WriteLine(" ");
                //    Console.WriteLine(" ");
                //    Console.WriteLine("Node ID = {0}", pBlock.handle());
                //    Console.WriteLine("Node Name = {0}", pBlock.getName());
                //    Console.WriteLine(" ");

                //    dumpBlock(pBlock, newModel, parentId);

                //}
                else
                {

                    Console.WriteLine(" ");
                    Console.WriteLine("### NEW NODE ###");
                    Console.WriteLine(" ");
                    Console.WriteLine(" ");
                    Console.WriteLine("Node ID = {0}", pBlock.handle());
                    Console.WriteLine("Node Name = {0}", pBlock.getName());
                    Console.WriteLine(" ");

                    dumpBlock(pBlock, newModel, parentId);

                }

                // iterate through all blocks in model, and print/add to Rayon model
                // supported types:
                //      1. Polyline
                //      2. 2DPolyline
                //      3. Line
                //      4. Circle
                //      5. Arc
                //      6. Ellipse
                //      7. Spline
                //      8. Block
                //      9. Hatch // Work in Progress
                // missing types: Dimension, Text, ...
                for (pEntIter.start(); !pEntIter.done(); pEntIter.step())
                {

                    OdDbEntity pEnt = OdDbEntity.cast(pEntIter.objectId().openObject());
                    if (pEnt == null)
                    {
                        Console.WriteLine("cast OdDbObjectIterator>OdDbEntity failed!");
                        return;
                    }

                    Console.WriteLine(pEnt.GetType().ToString());

                    //  1. Polyline
                    if (pEnt.GetType().ToString() is "Teigha.TD.OdDbPolyline")
                    {
                        //add to model
                        dumpPathFromPolyline(pEnt, newModel, parentId);
                    }

                    //  2. 2DPolyline
                    if (pEnt.GetType().ToString() is "Teigha.TD.OdDb2dPolyline")
                    {
                        //add to model
                        dumpPathFrom2dPolyline(pEnt, newModel, parentId);
                    }

                    //  3. Line
                    if (pEnt.GetType().ToString() is "Teigha.TD.OdDbLine")
                    {
                        //print
                        printLine(pEnt, parentId);
                        //add to model
                        dumpPathFromLine(pEnt, newModel, parentId);
                    }

                    //  4. Circle
                    if (pEnt.GetType().ToString() is "Teigha.TD.OdDbCircle")
                    {
                        //print
                        printCircle(pEnt, parentId);
                        //add to model
                        dumpArc(pEnt, newModel, parentId);
                    }

                    //  5. Arc
                    if (pEnt.GetType().ToString() is "Teigha.TD.OdDbArc")
                    {
                        //print
                        printArc(pEnt, parentId);
                        //add to model
                        dumpArc(pEnt, newModel, parentId);
                    }

                    //  6. Ellipse
                    if (pEnt.GetType().ToString() is "Teigha.TD.OdDbEllipse")
                    {
                        //print
                        printEllipse(pEnt, parentId);
                        //add to model
                        dumpArc(pEnt, newModel, parentId);
                    }

                    //  7. Spline
                    if (pEnt.GetType().ToString() is "Teigha.TD.OdDbSpline")
                    {
                        //print
                        printSpline(pEnt, newModel, parentId);
                        //add to model
                        dumpSpline(pEnt, newModel, parentId);
                    }

                    //  8. Block
                    if (pEnt.GetType().ToString() is "Teigha.TD.OdDbBlockReference")
                    {
                        Console.WriteLine("  >> Block Instance Found");
                        dumpBlockInstance(pEnt, newModel, parentId);
                    }

                    // 9. Hatch
                    //if (pEnt.GetType().ToString() is "Teigha.TD.OdDbHatch")
                    //{
                    //    dumpHatch(pEnt, newModel, parentId);
                    //}

                    Console.WriteLine(" ");

                }
                i = i + 1;
            }
        }

        // saving JSON to file
        void saveJSON(Rayon.Core.Model Model)
        {
            Model.MockGetElements(
                "C:/Users/piron/Desktop/rayon/ODA/ODA/out/newModel.json",
                false
                );
        }

        // checking JSON validity
        // Tests: 
        //      1. looks for missing Ids
        void checkJSON(Rayon.Core.Model Model)
        {
            //check Elements
            Console.WriteLine("Test 1: checking file for missing refs...");

            List<string> elements = new List<string>();
            foreach (Rayon.Core.Element elem in Model.Elements)
            {
                elements.Add(elem.Handle);
            }
            List<string> linkedElements = new List<string>();
            foreach (Rayon.Core.Components.Component comp in Model.Components)
            {
                if (comp.LinkedElement != null)
                {
                    linkedElements.Add(comp.LinkedElement);
                    //comp.;
                }
            }

            IEnumerable<string> missing = linkedElements.Except(elements);

            if (missing.ToList().Count > 0)
            {
                Console.WriteLine("  FAILED: Missing {0} Refs!", missing.ToList().Count);
                foreach (var item in missing)
                {
                    Console.WriteLine("  ID: " + item);
                }
            }
            else
            {
                Console.WriteLine("  PASSED: No Missing Refs!");
            }

            //count styles
            foreach (Rayon.Core.Element elem in Model.Elements)
            {
                foreach (Rayon.Core.Components.Component comp in elem.Components) 
                {

                    //Console.WriteLine(comp.ComponentType);

                } ;
            }
        }


        /************************************************************************/
        /*        Dump ODA > Rayon                                              */
        /************************************************************************/

        // stores new Model ID
        public static string ModelId;

        // Prints all DWG elements and adds them to new Rayon model
        public void dump(OdDbDatabase pDb)
        {

            MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

            // Creates new Rayon Model
            Rayon.Core.Model newModel = new Rayon.Core.Model(
                "My New Model",
                new Rayon.Core.User(),
                Guid.NewGuid()
                );

            // 0. HEADER
            Console.WriteLine(" ");
            Console.WriteLine("---------------------------------------------------");
            Console.WriteLine(" ");
            Console.WriteLine("RAYON dumper");
            Console.WriteLine(" ");

            // 1. METADATA
            Console.WriteLine("1. Metadata");
            Console.WriteLine(" ");

            dumpRayonMetadata(pDb, newModel);

            // 2. LAYERS 
            Console.WriteLine("2. Layers and Styles");
            Console.WriteLine(" ");

            dumpRayonLayersAndStyles(pDb, newModel);

            // 3. BLOCKS
            Console.WriteLine("3. Blocks");
            Console.WriteLine(" ");

            dumpRayonBlocks(pDb, newModel);

            // 4. GROUPS
            Console.WriteLine("4. Groups");
            Console.WriteLine(" ");

            dumpObject(pDb.getNamedObjectsDictionaryId(), "Named Objects Dictionary", newModel);

            Console.WriteLine("----------------------------------------------------");
            Console.WriteLine("  ");

            // 5. JSON
            // checks if JSON is valid
            checkJSON(newModel);
            // saves JSON to file
            saveJSON(newModel);

            Console.WriteLine("  ");
            Console.WriteLine("----------------------------------------------------");

            MemoryManager.GetMemoryManager().StopTransaction(mTr);

        }
    }
}
