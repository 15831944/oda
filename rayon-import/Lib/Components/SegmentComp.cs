using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;
using RayonImport.Lib.Geometry;

namespace RayonImport.Lib.Components
{
    // TODO : delete this
    internal class SegmentComp : ComponentValue
    {
        public SegmentComp(RPoint2d start, RPoint2d end) : base()
        {
            this.Start = start;
            this.End = end;
        }

        [JsonPropertyName("s")]
        public RPoint2d Start { get; set; }

        [JsonPropertyName("e")]
        public RPoint2d End { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Segment, this);
        }
    }
}
