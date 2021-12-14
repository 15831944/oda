using System;
using System.Collections.Generic;
using System.Text;

namespace RayonImport.Lib.Components
{
    internal class ImageStyleIdComp : StyleIdComp
    {
        public ImageStyleIdComp(string styleId)
           : base(styleId)
        {
        }

        public ImageStyleIdComp(Element style)
           : base(style)
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.ImageStyleId, this);
        }
    }
}
