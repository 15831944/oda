// <copyright file="BlockInstanceComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json.Serialization;

    /// <summary>
    /// A component used to indicate that an <see cref="Element"/> is an instance of a Block Element,
    /// by referencing the Handle of the corresponding Block <see cref="Element"/>.
    /// The Block Element is expected to have a <see cref="BlockComp"/> component.
    /// </summary>
    public class BlockInstanceComp : ComponentValue, ILinked
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="BlockInstanceComp"/> class.
        /// </summary>
        /// <param name="blockInstanceId"></param>
        public BlockInstanceComp(string blockInstanceId)
           : base()
        {
            this.BlockId = blockInstanceId;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="BlockInstanceComp"/> class.
        /// </summary>
        /// <param name="block"></param>
        public BlockInstanceComp(Element block)
           : base()
        {
            this.BlockId = block.Handle;
        }

        /// <summary>
        /// Gets or sets the handle of the BlockInstance Element reference via this component
        /// </summary>
        [JsonPropertyName("h")]
        public string BlockId { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.BlockInstance, this);
        }

        public string GetLinkedHandle()
        {
            return this.BlockId;
        }
    }
}
