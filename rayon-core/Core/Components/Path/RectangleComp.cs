// <copyright file="RectangleComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components.Path
{
    using System.Text.Json.Serialization;
    using Rayon.Core.Types;

    /// <summary>
    /// A component describing the geometry of a simple rectangle.
    /// The origin is the left bottom corner of the rectangle.
    /// The size corresponds to its width and height.
    /// </summary>
    public class RectangleComp : ComponentValue
    {
        public RectangleComp(RPoint2d origin, RSize size)
        {
            this.Origin = origin;
            this.Size = size;
            this.BorderRadii = null;
        }

        /// <summary>
        /// Gets or sets the bottom left corner of the rectangle.
        /// </summary>
        [JsonPropertyName("o")]
        public RPoint2d Origin { get; set; }

        [JsonPropertyName("s")]
        public RSize Size { get; set; }

        [JsonPropertyName("r")]
        public RSize BorderRadii { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Rectangle, this);
        }

        public class RectangleBorderRadii
        {
            public RectangleBorderRadii(double radius)
            {
                this.TopLeft = radius;
                this.TopRight = radius;
                this.BottomLeft = radius;
                this.BottomRight = radius;
            }

            [JsonPropertyName("tl")]
            public double TopLeft { get; set; }

            [JsonPropertyName("tr")]
            public double TopRight { get; set; }

            [JsonPropertyName("bl")]
            public double BottomLeft { get; set; }

            [JsonPropertyName("br")]
            public double BottomRight { get; set; }
        }
    }
}
