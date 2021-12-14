using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Components
{
    public class LayerIdComp : ComponentValue
    {
        public LayerIdComp(Element layer) : base()
        {
            this.LayerId = layer.Handle;
            this.ZIndex = 0.0;
        }

        public LayerIdComp(string layerId, double zIndex) : base()
        {
            this.LayerId = layerId;
            this.ZIndex = zIndex;
        }

        [JsonPropertyName("z")]
        public double ZIndex { get; set; }

        [JsonPropertyName("h")]
        public string LayerId { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.LayerId, this);
        }
    }
}
