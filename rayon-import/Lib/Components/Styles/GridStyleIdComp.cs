using System;
using System.Collections.Generic;
using System.Text;

namespace RayonImport.Lib.Components
{
    internal class GridStyleIdComp : StyleIdComp
    {
        public GridStyleIdComp(string styleId)
            : base(styleId)
        {
        }

        public GridStyleIdComp(Element style)
           : base(style)
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.GridStyleId, this);
        }
    }
}
