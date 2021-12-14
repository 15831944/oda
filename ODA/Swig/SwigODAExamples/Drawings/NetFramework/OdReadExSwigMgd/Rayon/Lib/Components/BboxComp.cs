// <copyright file="BboxComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json.Serialization;
    using Rayon.Lib.Geometry;

    /// <summary>
    ///
    /// "value": "{\"min\":[-266.6369,-171.95538],\"max\":[183.3631,328.04465]}",
    /// </summary>
    public class BboxComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="BboxComp"/> class.
        /// </summary>
        public BboxComp()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="BboxComp"/> class.
        /// </summary>
        /// <param name="min"></param>
        /// <param name="max"></param>
        /// <param name="rigid"></param>
        public BboxComp(
            RPoint2d min,
            RPoint2d max,
            bool rigid)
            : base()
        {
            this.Min = min;
            this.Max = max;
            this.Rigid = rigid;
        }

        [JsonPropertyName("a")]
        public RPoint2d Min { get; set; }

        [JsonPropertyName("b")]
        public RPoint2d Max { get; set; }

        /// <summary>
        /// Whether the Element can be scaled non uniformaly in the x and y direction.
        /// If set to true, the Element can only be scaled uniformaly.
        /// </summary>
        [JsonPropertyName("r")]
        public bool Rigid { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Bbox, this);
        }
    }
}
