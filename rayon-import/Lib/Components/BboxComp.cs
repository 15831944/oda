using System;
using System.Collections.Generic;
using System.Text;
using RayonImport.Lib.Geometry;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Components
{
    /// <summary>
    ///
    /// "value": "{\"min\":[-266.6369,-171.95538],\"max\":[183.3631,328.04465]}",
    /// </summary>
    public class BboxComp : ComponentValue
    {
        public BboxComp()
        {
        }

        public BboxComp(
            RPoint2d min,
            RPoint2d max,
            bool rigid
            )
            : base()
        {
            this.Min = min;
            this.Max = max;
            this.Rigid = rigid;
        }

        [JsonPropertyName("a")]
        public RPoint2d Min { get; set; }

        [JsonPropertyName("b")]
        public RPoint2d Max { get; set; }

        [JsonPropertyName("r")]
        public bool Rigid { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Bbox, this);
        }
    }
}
