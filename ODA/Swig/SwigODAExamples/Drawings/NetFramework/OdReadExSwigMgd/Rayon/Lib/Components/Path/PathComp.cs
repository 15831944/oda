// <copyright file="PathComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Text.Json;
    using System.Text.Json.Serialization;
    using Rayon.Lib.Geometry;

    /// <summary>
    /// Expected serialization
    /// {
    /// "p": [
    ///    [-189.18933, 355.0],
    ///    [-189.18933, 433.34363],
    ///    [-253.65639, 497.81067],
    ///    [-319.0, 436.81067],
    /// ],
    /// "v": ["Begin", "CubicTo"],
    /// "t": { 1: 0, 2: 0}
    /// "v": true
    /// }
    /// </summary>
    public class PathComp : ComponentValue, IBoundingBox
    {
        public const int MAXPOINTS = 65535;

        /// <summary>
        /// Initializes a new instance of the <see cref="PathComp"/> class.
        /// </summary>
        public PathComp()
            : base()
        {
            this.Closed = false;
            this.Points = new List<RPoint2d>();
            this.Verbs = new List<PathVerbEnum>();
            this.ControlPointsConstraints = null;
        }

        public enum PathVerbEnum
        {
            Begin = 0,
            LineTo = 1,
            QuadraticTo = 2,
            CubicTo = 3,
        }

        public enum ControlPointsConstraintEnum
        {
            Undefined = 0,
            Independent = 1,
            Aligned = 2,
            Mirrored = 3,
        }

        [JsonPropertyName("p")]
        public List<RPoint2d> Points { get; set; }

        [JsonPropertyName("v")]
        public List<PathVerbEnum> Verbs { get; set; }

        /// <summary>
        /// A map indicating for each control points, identified via their index in the Points List,
        /// what kind of constraint should be applied between their "twin" control points.
        /// eg. a control point that is aligned, should always form a straight line with
        ///    its corresponding end point and its sibling control point.
        /// Per default, the map is null for a new path. Meaning that there are no constraint
        /// on its control points.
        /// </summary>
        [JsonPropertyName("t")]
        [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
        public Dictionary<int, ControlPointsConstraintEnum> ControlPointsConstraints { get; set; }

        [JsonPropertyName("c")]
        public bool Closed { get; set; }

        public void BeginAt(RPoint2d at)
        {
            this.Points.Add(at);
            this.Verbs.Add(PathVerbEnum.Begin);
        }

        public void LineTo(RPoint2d to)
        {
            this.Points.Add(to);
            this.Verbs.Add(PathVerbEnum.LineTo);
        }

        public void QuadraticTo(RPoint2d to, RPoint2d ctrl)
        {
            this.Points.Add(ctrl);
            this.Points.Add(to);
            this.Verbs.Add(PathVerbEnum.QuadraticTo);
        }

        public void CubicTo(RPoint2d to, RPoint2d ctrl1, RPoint2d ctrl2)
        {
            this.Points.Add(ctrl1);
            this.Points.Add(ctrl2);
            this.Points.Add(to);
            this.Verbs.Add(PathVerbEnum.CubicTo);
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
            return this.Points.Count > MAXPOINTS - 3;
        }

        public bool IsEmpty()
        {
            return this.Points.Count == 0;
        }

        public void Close()
        {
            this.Closed = true;
        }

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
