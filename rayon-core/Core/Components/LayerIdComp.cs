// <copyright file="LayerIdComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text.Json.Serialization;

    public class LayerIdComp : ComponentValue, ILinked
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="LayerIdComp"/> class.
        /// </summary>
        /// <param name="layer"></param>
        public LayerIdComp(Element layer)
            : base()
        {
            this.LayerId = layer.Handle;
            this.ZIndex = 0.0;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="LayerIdComp"/> class.
        /// </summary>
        /// <param name="layerId"></param>
        /// <param name="zIndex"></param>
        public LayerIdComp(string layerId, double zIndex)
            : base()
        {
            this.LayerId = layerId;
            this.ZIndex = zIndex;
        }

        [JsonPropertyName("z")]
        public double ZIndex { get; set; }

        [JsonPropertyName("h")]
        public string LayerId { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.LayerId, this);
        }

        public string GetLinkedHandle()
        {
            return this.LayerId;
        }
    }
}
