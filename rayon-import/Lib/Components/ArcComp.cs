using System;
using System.Collections.Generic;
using System.Text;
using RayonImport.Lib.Geometry;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Components
{
    public class ArcComp : ComponentValue, IBoundingBox
    {
        public ArcComp(RPoint2d center, RVector radii) : base()
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
