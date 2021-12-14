using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Components
{
    public class NameComp : ComponentValue
    {
        public NameComp(string name) : base()
        {
            this.Name = name;
        }

        [JsonPropertyName("n")]
        public string Name { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Name, this);
        }
    }
}
