// <copyright file="ArcComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json.Serialization;
    using Rayon.Lib.Geometry;

    public class ArcComp : ComponentValue, IBoundingBox
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="ArcComp"/> class.
        /// </summary>
        /// <param name="center"></param>
        /// <param name="radii"></param>
        public ArcComp(RPoint2d center, RVector radii)
            : base()
        {
            this.Center = center;
            this.Radii = radii;

            // default values
            this.StartAngle = new RAngle(0.0);
            this.SweepAngle = RAngle.TwoPi(); // full arc
            this.XRotation = new RAngle(0.0);
        }

        [JsonPropertyName("c")]
        public RPoint2d Center { get; set; }

        [JsonPropertyName("r")]
        public RVector Radii { get; set; }

        [JsonPropertyName("st_a")]
        public RAngle StartAngle { get; set; }

        [JsonPropertyName("sw_a")]
        public RAngle SweepAngle { get; set; }

        [JsonPropertyName("x_rot")]
        public RAngle XRotation { get; set; }

        public BboxComp GetBoundingBox()
        {
            var min = this.Center - this.Radii;
            var max = this.Center + this.Radii;
            return new BboxComp(min, max, false);
        }

        public bool IsEmpty()
        {
            return this.Center == null || this.Radii == null;
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Arc, this);
        }
    }
}
