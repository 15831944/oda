using System;
using System.Collections.Generic;
using System.Text;

namespace RayonImport.Lib.Components
{
    internal class HatchStyleIdComp : StyleIdComp
    {
        public HatchStyleIdComp(string styleId)
           : base(styleId)
        {
        }

        public HatchStyleIdComp(Element style)
           : base(style)
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.HatchStyleId, this);
        }
    }
}
