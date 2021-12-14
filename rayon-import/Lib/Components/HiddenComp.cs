using System;
using System.Collections.Generic;
using System.Text;

namespace RayonImport.Lib.Components
{
    public class HiddenComp : ComponentValue
    {
        public HiddenComp() : base()
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Hidden, this);
        }
    }
}
