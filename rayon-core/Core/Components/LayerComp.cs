// <copyright file="LayerComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text.Json.Serialization;
    using Rayon.Core.Types;

    public class LayerComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="LayerComp"/> class.
        /// </summary>
        /// <param name="isInternal"></param>
        /// <param name="color"></param>
        public LayerComp(bool isInternal, RColor color)
            : base()
        {
            this.IsInternal = isInternal;
            this.Color = color;
        }

        [JsonPropertyName("i")]
        public bool IsInternal { get; set; }

        [JsonPropertyName("c")]
        public RColor Color { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Layer, this);
        }
    }
}
