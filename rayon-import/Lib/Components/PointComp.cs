using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;
using RayonImport.Lib.Geometry;

namespace RayonImport.Lib.Components
{
    public class PointComp : ComponentValue
    {
        public PointComp(RPoint2d position) : base()
        {
            this.Position = position;
        }

        [JsonPropertyName("p")]
        public RPoint2d Position { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Point, this);
        }
    }
}
