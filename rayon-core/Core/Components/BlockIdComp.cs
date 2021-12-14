// <copyright file="BlockIdComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json.Serialization;

    /// <summary>
    /// A component used to indicate that an <see cref="Element"/> is a member of a the definition of a block,
    /// by referencing the Handle of the corresponding Block Element.
    /// The Block Element is expected to have a <see cref="BlockComp"/> component.
    /// </summary>
    public class BlockIdComp : ComponentValue, ILinked
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="BlockIdComp"/> class.
        /// </summary>
        /// <param name="blockId"></param>
        public BlockIdComp(string blockId)
           : base()
        {
            this.BlockId = blockId;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="BlockIdComp"/> class.
        /// </summary>
        /// <param name="block"></param>
        public BlockIdComp(Element block)
           : base()
        {
            this.BlockId = block.Handle;
        }

        /// <summary>
        /// Gets or sets the handle of the Block Element reference via this component
        /// </summary>
        [JsonPropertyName("h")]
        public string BlockId { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.BlockId, this);
        }

        public string GetLinkedHandle()
        {
            return this.BlockId;
        }
    }
}
