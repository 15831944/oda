using System;
using System.Collections.Generic;
using System.Linq;
using Rhino.FileIO;
using Rhino.Geometry;
using RhinoUnitSystem = Rhino.UnitSystem;
using Rhino.DocObjects;
using Rayon.Core;
using Rayon.Core.Components;
using Rayon.Core.Types;
using Rayon.Rhino.Extension;

namespace Rayon.Rhino
{
    internal class RhinoImporter
    {
        private Dictionary<int, Element> layerIndexToEntity = new Dictionary<int, Element>();
        private Dictionary<int, Element> layerToStyleEntity = new Dictionary<int, Element>();

        public RhinoImporter(Model model, File3dm file)
        {
            this.Model = model;
            this.File = file;
            this.Model.SetSettings(file.Settings.ModelUnitSystem.ToRayon());
            this.Unit = file.Settings.ModelUnitSystem.ToRayon();
        }

        internal Model Model { get; }

        internal File3dm File { get; }

        internal ModelSettingsComp.UnitSystemEnum Unit { get; set; }

        internal void SaveToDisk(bool withHeader)
        {
            // Append text to an existing file named "WriteLines.txt".
            var path = @"C:\Users\basti\rayon\resources\model.json";
            Console.WriteLine("Starting Serialization...");
            this.Model.MockGetElements(path, withHeader);
            Console.WriteLine("Exported to disk");
        }

        /// <summary>
        /// Create all layers
        /// </summary>
        /// <param name="layer"></param>
        internal void ImportAllLayers()
        {
            File3dmLayerTable layerTables = this.File.AllLayers;
            int maxIndex = layerTables.Select((l) => l.Index).Max() + 10;
            int minIndex = layerTables.Select((l) => l.Index).Min();
            foreach (Layer layer in layerTables)
            {
                var name = layer.Name;
                var parent = layer.ParentLayerId;
                var handle = layer.Id.ToString();
                var color = layer.Color;

                // note : in rhino lowest index layer is highest on screen
                //        in rayon it is the opposite, the highest index is highest on screen
                double z = ((double)(maxIndex - layer.Index)) / (maxIndex - minIndex);
                var style = Element.CreateStrokeStyle(this.Model, name, RColor.Black(), 1.0, StrokeStyleComp.StrokePatternEnum.Solid);
                var rLayer = Element.CreateLayer(this.Model, name, parent.ToString(), z, handle, style, color.ToRayon());

                Console.WriteLine("Creating Layer with Color {0}", color.ToRayon().B);
                this.Model.AddElement(rLayer);
                this.Model.AddElement(style);

                // add reference in dictionary for later
                this.layerIndexToEntity[layer.Index] = rLayer;
                this.layerToStyleEntity[layer.Index] = style;
            }
        }

        internal void ImportAllGeometryObjects()
        {
            File3dmObjectTable objectTable = this.File.Objects;
            foreach (File3dmObject obj in objectTable)
            {
                GeometryBase geometry = obj.Geometry;
                this.ImportGeometryObject(obj, geometry, objectTable);
            }
        }

        internal void ImportGeometryObject(File3dmObject obj, GeometryBase geometry, File3dmObjectTable objectTable)
        {
            // note : we do not import block because we duplicate all the geometry
            if (obj.Attributes.IsInstanceDefinitionObject)
            {
                return;
            }

            if (geometry is InstanceReferenceGeometry geometryReference)
            {
                foreach (GeometryBase geom in this.GetGeometryOfInstanceReference(geometryReference, objectTable))
                {
                    // we need to assign new Guuid because we are making copies of the same object for each sub-geometry
                    obj.Id = Guid.NewGuid();
                    this.ImportGeometryObject(obj, geom, objectTable);
                }
            }

            if (geometry is LineCurve lineCurve)
            {
                var pathComp = ImportLine(lineCurve);
                AddEntityToModel(obj, pathComp, obj.Id.ToString());
            }
            else if (geometry is PolylineCurve polylineCurve)
            {
                var pathComp = ImportPolyline(polylineCurve);
                AddEntityToModel(obj, pathComp, obj.Id.ToString());
            }
            else if (geometry is ArcCurve arcCurve)
            {
                var arcComp = ImportArcCurve(arcCurve);
                AddEntityToModel(obj, arcComp, obj.Id.ToString());
            }
            else if (geometry is PolyCurve polyCurve)
            {
                ImportPolyCurve(polyCurve, obj);
            }
            else if (geometry is NurbsCurve nurbsCurve)
            {
                var pathComp = ImportNurbCurve(nurbsCurve);
                AddEntityToModel(obj, pathComp, obj.Id.ToString());
            }
        }

        /// <summary>
        /// https://developer.rhino3d.com/guides/opennurbs/traverse-instance-definitions/
        /// </summary>
        internal IEnumerable<GeometryBase> GetGeometryOfInstanceReference(InstanceReferenceGeometry geometry, File3dmObjectTable objectTable)
        {
            var output = new List<GeometryBase>();
            Guid parentId = geometry.ParentIdefId;
            var subGeometriesIds = this.GetInstanceGeometry(parentId);
            foreach (Guid id in subGeometriesIds)
            {
                var subObject = objectTable.FindId(id);
                if (subObject.Geometry is InstanceReferenceGeometry refGeometry)
                {
                    output.AddRange(
                        this.GetGeometryOfInstanceReference(refGeometry, objectTable)
                        .Select((g) => { g.Transform(geometry.Xform); return g; })
                    );
                }
                else if (subObject.Geometry != null)
                {
                    var duplicate_geometry = subObject.Geometry.Duplicate();
                    duplicate_geometry.Transform(geometry.Xform);
                    output.Add(duplicate_geometry);
                }
            }
            return output;
        }

        internal IEnumerable<Guid> GetInstanceGeometry(Guid id)
        {
            File3dmInstanceDefinitionTable instanceDefinitionsTable = this.File.AllInstanceDefinitions;
            InstanceDefinitionGeometry instanceDefinition = instanceDefinitionsTable.FindId(id);
            if (instanceDefinition == null)
            {
                return new Guid[] { };
            }
            return instanceDefinition.GetObjectIds();
        }

        internal void AddEntityToModel(File3dmObject obj, PathComp pathComp, string handle)
        {
            if (pathComp == null || pathComp.IsEmpty()) { return; }
            ObjectAttributes attributes = obj.Attributes;
            Element rLayer = this.layerIndexToEntity[attributes.LayerIndex];
            Element rStyle = this.layerToStyleEntity[attributes.LayerIndex];
            string name = attributes.Name;
            var polyLine = Element.CreatePath(this.Model, handle, rLayer, rStyle, name, pathComp);
            this.Model.AddElement(polyLine);
        }

        internal void AddEntityToModel(File3dmObject obj, ArcComp arcComp, string handle)
        {
            if (arcComp == null || arcComp.IsEmpty()) { return; }
            ObjectAttributes attributes = obj.Attributes;
            Element rLayer = this.layerIndexToEntity[attributes.LayerIndex];
            Element rStyle = this.layerToStyleEntity[attributes.LayerIndex];
            string name = attributes.Name;
            var polyLine = Element.CreateArc(this.Model, handle, rLayer, rStyle, name, arcComp); ;
            this.Model.AddElement(polyLine);
        }

        internal PathComp ImportLine(LineCurve lineCurve, PathComp previousPathComp = null)
        {
            if (!this.ValidCurve(lineCurve))
            {
                return null;
            }

            RPoint2d at = lineCurve.Line.From.ToRayon(this.Unit);
            RPoint2d to = lineCurve.Line.To.ToRayon(this.Unit);
            var pathComp = previousPathComp;

            // the curve is not continuous with the previous path
            if (pathComp == null || !at.AlmostEqualTo(pathComp.GetlastTo()) || pathComp.IsFull())
            {
                pathComp = new PathComp();
                pathComp.BeginAt(at);
            }

            pathComp.LineTo(to);
            return pathComp;
        }

        internal PathComp ImportPolyline(PolylineCurve polylineCurve, PathComp previousPathComp = null)
        {
            if (!this.ValidCurve(polylineCurve))
            {
                return null;
            }

            Polyline polyline = polylineCurve.ToPolyline();
            int i = 0;
            var pathComp = previousPathComp ?? new PathComp();
            foreach (Point3d point in polyline)
            {
                if (i == 0)
                {
                    var at = point.ToRayon(this.Unit);

                    // the curve is not continuous with the previous path
                    if (pathComp == null || !at.AlmostEqualTo(pathComp.GetlastTo()) || pathComp.IsFull())
                    {
                        pathComp = new PathComp();
                        pathComp.BeginAt(at);
                    }
                }
                else
                {
                    pathComp.LineTo(point.ToRayon(this.Unit));
                }
                i += 1;
            }

            if (polyline.IsClosed)
            {
                pathComp.Close();
            }

            return pathComp;
        }

        /// <summary>
        /// Convert a Rhino ArcCurve into a Rayon ArcComp
        /// Note: the angles in the Rhino Arc are not signed...
        ///       we must therefore re-compute them by using the start and end points absolute positions
        /// </summary>
        /// <param name="arcCurve">A rhino ArcCurve</param>
        /// <returns></returns>
        internal ArcComp ImportArcCurve(ArcCurve arcCurve)
        {
            if (!this.ValidCurve(arcCurve))
            {
                return null;
            }

            Arc arc = arcCurve.Arc;
            Point3d center = arc.Center;
            Point3d start = arc.StartPoint;
            Vector3d basis = (start - center);

            double startAngle = Vector3d.VectorAngle(Vector3d.XAxis, basis, Vector3d.ZAxis);
            double signedArcAngle = arc.Angle;
            if (arc.Plane.Normal.Z < 0)
            {
                signedArcAngle *= -1;
            }

            double radius = arc.Radius;
            ArcComp arcComp = new ArcComp(center.ToRayon(this.Unit), new RVector(radius, radius));
            arcComp.StartAngle = new RAngle(startAngle);
            arcComp.SweepAngle = new RAngle(signedArcAngle);
            return arcComp;
        }

        internal void ImportPolyCurve(PolyCurve polyCurve, File3dmObject obj)
        {
            if (!this.ValidCurve(polyCurve))
            {
                return;
            }

            // remove nesting
            polyCurve.RemoveNesting();

            int i = 0;

            var initialPathComp = new PathComp();

            foreach (Curve curve in polyCurve.Explode())
            {
                if (curve is LineCurve lineCurve)
                {
                    var pathComp = ImportLine(lineCurve, initialPathComp);
                    if (!Object.ReferenceEquals(pathComp, initialPathComp))
                    {
                        AddEntityToModel(obj, initialPathComp, obj.Id.ToString() + $"-{i}");
                        initialPathComp = pathComp;
                    }
                }
                else if (curve is PolylineCurve polylineCurve)
                {
                    var pathComp = ImportPolyline(polylineCurve, initialPathComp);
                    if (!Object.ReferenceEquals(pathComp, initialPathComp))
                    {
                        AddEntityToModel(obj, initialPathComp, obj.Id.ToString() + $"-{i}");
                        initialPathComp = pathComp;
                    }
                }
                else if (curve is ArcCurve arcCurve)
                {
                    // NOTE: we import arc as nurbs... This is not good but will do while we support rational nurbs / bezier
                    var nurbsCurve = arcCurve.ToNurbsCurve();
                    var pathComp = ImportNurbCurve(nurbsCurve, initialPathComp);
                    if (!Object.ReferenceEquals(pathComp, initialPathComp))
                    {
                        AddEntityToModel(obj, initialPathComp, obj.Id.ToString() + $"-{i}");
                        initialPathComp = pathComp;
                    }
                }
                else if (curve is NurbsCurve nurbsCurve)
                {
                    PathComp pathComp;
                    if (nurbsCurve.IsLinear())
                    {
                        pathComp = this.ImportNurbsCurveAsLinear(nurbsCurve, initialPathComp);
                    }
                    else
                    {
                        pathComp = ImportNurbCurve(nurbsCurve, initialPathComp);
                    }

                    if (!Object.ReferenceEquals(pathComp, initialPathComp))
                    {
                        AddEntityToModel(obj, initialPathComp, obj.Id.ToString() + $"-{i}");
                        initialPathComp = pathComp;
                    }
                }
                else
                {
                    Console.WriteLine("UNKNOWN CURVE !!!!");
                }
                i++;
            }
            AddEntityToModel(obj, initialPathComp, obj.Id.ToString() + $"-{i}");
        }

        internal PathComp ImportNurbCurve(NurbsCurve nurbsCurve, PathComp previousPath = null)
        {
            if (!this.ValidCurve(nurbsCurve))
            {
                return null;
            }

            if (nurbsCurve.IsLinear())
            {
                var pathComp = this.ImportNurbsCurveAsLinear(nurbsCurve, previousPath);
                return pathComp;
            }

            BezierCurve[] beziers = nurbsCurve.ToBezier();
            PathComp path = previousPath ?? new PathComp();
            var start = previousPath == null;
            var previousIsActive = previousPath != null;
            foreach (BezierCurve bezier in beziers)
            {
                int cvCount = bezier.ControlVertexCount;
                if (cvCount == 2)
                {
                    var at = bezier.GetControlVertex3d(0).ToRayon(this.Unit);

                    // the curve is not continuous with the previous path
                    if (previousIsActive && !at.AlmostEqualTo(path.GetlastTo()) || path.IsFull())
                    {
                        path = new PathComp();
                        start = true;
                        previousIsActive = false;
                    }

                    var to = bezier.GetControlVertex3d(1).ToRayon(this.Unit);
                    if (start)
                    {
                        path.BeginAt(at);
                        start = false;
                    }
                    path.LineTo(to);
                }
                else if (cvCount == 3)
                {
                    var at = bezier.GetControlVertex3d(0).ToRayon(this.Unit);

                    // the curve is not continuous with the previous path
                    if (previousIsActive && !at.AlmostEqualTo(path.GetlastTo()) || path.IsFull())
                    {
                        path = new PathComp();
                        start = true;
                        previousIsActive = false;
                    }

                    var ctrl = bezier.GetControlVertex3d(1).ToRayon(this.Unit);
                    var to = bezier.GetControlVertex3d(2).ToRayon(this.Unit);
                    if (start)
                    {
                        path.BeginAt(at);
                        start = false;
                    }
                    path.QuadraticTo(to, ctrl);
                }
                else if (cvCount == 4)
                {
                    var at = bezier.GetControlVertex3d(0).ToRayon(this.Unit);

                    // the curve is not continuous with the previous path
                    if (previousIsActive && !at.AlmostEqualTo(path.GetlastTo()) || path.IsFull())
                    {
                        path = new PathComp();
                        start = true;
                        previousIsActive = false;
                    }

                    var ctrl1 = bezier.GetControlVertex3d(1).ToRayon(this.Unit);
                    var ctrl2 = bezier.GetControlVertex3d(2).ToRayon(this.Unit);
                    var to = bezier.GetControlVertex3d(3).ToRayon(this.Unit);
                    if (start)
                    {
                        path.BeginAt(at);
                        start = false;
                    }
                    path.CubicTo(to, ctrl1, ctrl2);
                }
                else
                {
                    Console.WriteLine("CANNOT IMPORT BEZIER OF DEGREE 4 OR HIGHER");
                }
            }

            if (!start)
            {
                return path;
            }

            return null;
        }

        internal PathComp ImportNurbsCurveAsLinear(NurbsCurve nurbsCurve, PathComp previousPath = null)
        {
            var path = previousPath;
            var to = nurbsCurve.PointAtEnd.ToRayon(this.Unit);
            var at = nurbsCurve.PointAtStart.ToRayon(this.Unit);

            if (previousPath == null || !at.AlmostEqualTo(path.GetlastTo()) || path.IsFull())
            {
                path = new PathComp();
                path.BeginAt(at);
            }

            path.LineTo(to);
            return path;
        }

        internal bool ValidCurve(Curve curve, double tolerance = 1e-6)
        {
            if (curve.IsValid && curve.IsPlanar())
            {
                Plane plane;
                var gotPlane = curve.TryGetPlane(out plane);
                if (gotPlane)
                {
                    return (1 - Math.Abs(plane.Normal.Z)) < tolerance;
                }
            }
            Console.WriteLine("Found Invalid curve");
            return false;
        }
    }
}
