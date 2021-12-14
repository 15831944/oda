// <copyright file="StyleComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text.Json;
    using System.Text.Json.Serialization;
    using Rayon.Lib.Components.Types;
    using Rayon.Lib.Geometry;

    /// <summary>
    /// A class describing a Stroke Style component of a Style Entity
    /// </summary>
    public class StrokeStyleComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="StrokeStyleComp"/> class.
        /// </summary>
        /// <param name="color">The color of the stroke</param>
        /// <param name="size">The size or thickness of the stroke</param>
        /// <param name="pattern">The dash pattern of the stroke</param>
        public StrokeStyleComp(RColor color, RLength size, StrokePatternEnum pattern)
            : base()
        {
            this.Color = color;
            this.Size = size;
            this.Pattern = pattern;
        }

        public enum StrokePatternEnum
        {
            Solid = 0,

            // dots
            TouchingDots = 1,

            VeryDenseDots = 2,
            DenseDots = 15,
            LooseDots = 16,

            // dashes
            DenseDashes = 17,

            LooseDashes = 18,
            LongDashes = 19,
            DenseRoundDashes = 3,
            LooseRoundDashes = 4,
            LongRoundDashes = 5,

            // mixed
            LongDashesDots = 20,

            LongDashesDoubleDots = 21,
            LongRoundDashesDots = 6,
            LongRoundDashesDoubleDots = 23,

            // Arrows
            DenseArrows = 13,

            LooseArrows = 14,
            LongArrows = 22,

            // Tests
            DashDotted = 30,

            LooselyDashDotDotted = 31,
            Dashed = 32,
        }

        [JsonPropertyName("c")]
        public RColor Color { get; set; }

        [JsonPropertyName("s")]
        public RLength Size { get; set; }

        [JsonPropertyName("p")]
        public StrokePatternEnum Pattern { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.StrokeStyle, this);
        }
    }
}
