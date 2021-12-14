using System;
using System.Collections.Generic;
using System.Text;

namespace RayonImport.Lib.Components
{
    internal class TextStyleIdComp : StyleIdComp
    {
        public TextStyleIdComp(string styleId)
          : base(styleId)
        {
        }

        public TextStyleIdComp(Element style)
           : base(style)
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.TextStyleId, this);
        }
    }
}
