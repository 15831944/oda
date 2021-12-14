using System;
using System.Collections.Generic;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using RayonImport.Lib.Geometry;
using System.Linq;

namespace RayonImport.Lib.Components
{
    /// <summary>
    /// Expected serialization
    /// {
    /// "points": [
    ///    [-189.18933, 355.0],
    ///    [-189.18933, 433.34363],
    ///    [-253.65639, 497.81067],
    ///    [-319.0, 436.81067],
    /// ],
    /// "verbs": ["Begin", "CubicTo", "CubicTo", "CubicTo", "CubicTo"],
    /// "closed": true
    ///}
    /// </summary>
    public class PathComp : ComponentValue, IBoundingBox
    {
        public const int MAX_POINTS = 65535;

        public PathComp() : base()
        {
            this.Closed = false;
            this.Points = new List<RPoint2d>();
            this.Verbs = new List<PathVerb>();
        }

        public enum PathVerb
        {
            Begin = 0,
            LineTo = 1,
            QuadraticTo = 2,
            CubicTo = 3,
        }

        public void BeginAt(RPoint2d at)
        {
            this.Points.Add(at);
            this.Verbs.Add(PathVerb.Begin);
        }

        public void LineTo(RPoint2d to)
        {
            this.Points.Add(to);
            this.Verbs.Add(PathVerb.LineTo);
        }

        public void QuadraticTo(RPoint2d to, RPoint2d Ctrl)
        {
            this.Points.Add(Ctrl);
            this.Points.Add(to);
            this.Verbs.Add(PathVerb.QuadraticTo);
        }

        public void CubicTo(RPoint2d to, RPoint2d ctrl1, RPoint2d ctrl2)
        {
            this.Points.Add(ctrl1);
            this.Points.Add(ctrl2);
            this.Points.Add(to);
            this.Verbs.Add(PathVerb.CubicTo);
        }

        public RPoint2d GetlastTo()
        {
            var count = this.Points.Count;
            if (count == 0)
            {
                return null;
            }
            return this.Points[count - 1];
        }

        public bool IsFull()
        {
            // we need at least 3 points for a cubic bezier
            return (this.Points.Count > MAX_POINTS - 3);
        }

        public bool IsEmpty()
        {
            return this.Points.Count == 0;
        }

        public void Close()
        {
            this.Closed = true;
        }

        [JsonPropertyName("p")]
        public List<RPoint2d> Points { get; set; }

        [JsonPropertyName("v")]
        public List<PathVerb> Verbs { get; set; }

        [JsonPropertyName("c")]
        public bool Closed { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Path, this);
        }

        /// <summary>
        /// Returns an approximation of the bounding box of the path using the control points coordinates
        /// </summary>
        /// <returns></returns>
        public BboxComp GetBoundingBox()
        {
            var x_min = this.Points.Select(p => p.X).Min();
            var x_max = this.Points.Select(p => p.X).Max();
            var y_min = this.Points.Select(p => p.Y).Min();
            var y_max = this.Points.Select(p => p.Y).Max();

            return new BboxComp(new RPoint2d(x_min, y_min), new RPoint2d(x_max, y_max), false);
        }
    }
}
