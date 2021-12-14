// <copyright file="HiddenComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;

    /// <summary>
    /// A empty component used to flag an entity as locked.
    /// Layers, Groups or geometry elements can be locked for example.
    /// </summary>
    public class LockedComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="LockedComp"/> class.
        /// </summary>
        public LockedComp()
            : base()
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Locked, this);
        }
    }
}
