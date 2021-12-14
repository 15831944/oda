// <copyright file="PointStyleComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components.Styles
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json.Serialization;
    using Rayon.Core.Types;

    /// <summary>
    /// A class depicting the properties of a Point style for a Style Entity.
    /// </summary>
    internal class PointStyleComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="PointStyleComp"/> class.
        /// </summary>
        /// <param name="color">The primary color of the point</param>
        /// <param name="backgroundColor">The background color of the point</param>
        /// <param name="size">The size of the point</param>
        /// <param name="shape">The shape of the point</param>
        public PointStyleComp(RColor color, RColor backgroundColor, RLength size, PointShapeEnum shape)
            : base()
        {
            this.Color = color;
            this.Shape = shape;
            this.Size = size;
            this.BackgroundColor = backgroundColor;
        }

        public enum PointShapeEnum
        {
            OutlinedSquare = 0,
            OutlinedDisc = 1,
            FilledSquare = 2,
            FilledDisc = 3,
        }

        [JsonPropertyName("c")]
        public RColor Color { get; set; }

        [JsonPropertyName("bc")]
        public RColor BackgroundColor { get; set; }

        [JsonPropertyName("s")]
        public RLength Size { get; set; }

        [JsonPropertyName("sh")]
        public PointShapeEnum Shape { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.PointStyle, this);
        }
    }
}
