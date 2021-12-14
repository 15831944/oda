using System;
using System.Collections.Generic;
using System.Text;

namespace RayonImport.Lib.Components
{
    internal class PointStyleIdComp : StyleIdComp
    {
        public PointStyleIdComp(string styleId)
          : base(styleId)
        {
        }

        public PointStyleIdComp(Element style)
           : base(style)
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.PointStyleId, this);
        }
    }
}
