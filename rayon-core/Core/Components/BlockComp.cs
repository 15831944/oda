// <copyright file="BlockComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text.Json.Serialization;
    using Rayon.Core.Types;

    /// <summary>
    /// A Component identifying an element as a Block definition.
    /// </summary>
    public class BlockComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="BlockComp"/> class.
        /// An <see cref="Element"/> with a <see cref="BlockComp"/> component, can be
        /// referenced by a <see cref="BlockIdComp"/> for the elements constituting the
        /// definition of the block (such as paths, texts etc.) or can be
        /// referenced by a <see cref="BlockInstanceComp"/> for the elements
        /// that are instances of this block definition.
        /// </summary>
        public BlockComp()
            : base()
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Block, this);
        }
    }
}
