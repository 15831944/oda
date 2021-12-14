using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;
using RayonImport.Lib.Components.Types;

namespace RayonImport.Lib.Components
{
    public class LayerComp : ComponentValue
    {
        public LayerComp(bool isInternal, Color color) : base()
        {
            this.IsInternal = isInternal;
            this.Color = color;
        }

        [JsonPropertyName("i")]
        public bool IsInternal { get; set; }

        [JsonPropertyName("c")]
        public Color Color { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Layer, this);
        }
    }
}
