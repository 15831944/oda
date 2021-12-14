// <copyright file="GroupComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;

    public class GroupComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="GroupComp"/> class.
        /// A Group component is an empty Component flagging an Element
        /// as a group. It is used to filter efficiently group elements.
        /// A group Element is referenced by its element members via
        /// a <see cref="GroupIdComp"/>
        /// </summary>
        public GroupComp()
            : base()
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Group, this);
        }
    }
}
