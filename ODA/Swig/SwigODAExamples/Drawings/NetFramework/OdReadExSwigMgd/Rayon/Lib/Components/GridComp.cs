// <copyright file="GridComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;

    /// <summary>
    /// TODO : skip this for the time being
    /// </summary>
    internal class GridComp : ComponentValue
    {
        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Grid, this);
        }
    }
}
