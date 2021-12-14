using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Components
{
    public class StrokeStyleIdComp : StyleIdComp
    {
        public StrokeStyleIdComp(string styleId)
            : base(styleId)
        {
        }

        public StrokeStyleIdComp(Element style)
           : base(style)
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.StrokeStyleId, this);
        }
    }
}
