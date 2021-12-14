// <copyright file="RenderBufferComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;

    /// <summary>
    /// A render buffer component is used in the client to store the vertices
    /// data needed to render the Element on the gpu.
    /// Its data is not persisted to the db, only the fact that an Element should
    /// have a render buffer component meaning that it will be rendered on screen.
    /// </summary>
    public class RenderBufferComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="RenderBufferComp"/> class.
        /// </summary>
        public RenderBufferComp()
            : base()
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.RenderBuffer, this);
        }
    }
}
