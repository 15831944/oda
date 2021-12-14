// <copyright file="HiddenComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;

    public class HiddenComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="HiddenComp"/> class.
        /// </summary>
        public HiddenComp()
            : base()
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Hidden, this);
        }
    }
}
