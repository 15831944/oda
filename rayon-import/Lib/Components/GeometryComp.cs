using System;
using System.Collections.Generic;
using System.Text;

namespace RayonImport.Lib.Components
{
    public class GeometryComp : ComponentValue
    {
        public GeometryComp() : base()
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Geometry, this);
        }
    }
}
