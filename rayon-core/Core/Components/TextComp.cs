// <copyright file="TextComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text.Json.Serialization;
    using Rayon.Core.Types;

    /// <summary>
    /// A component describing a Text Element.
    /// </summary>
    public class TextComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="TextComp"/> class.
        /// </summary>
        public TextComp(string content, double width, double height, RPoint2d position, AnchorEnum anchor)
            : base()
        {
            this.Content = content;
            this.Position = position;
            this.Height = height;
            this.Width = width;
            this.Anchor = anchor;
        }

        public enum AnchorEnum
        {
            TopLeft = 0,
            TopMiddle = 1,
            TopRight = 2,
            MiddleLeft = 3,
            Middle = 4,
            MiddleRight = 5,
            BottomLeft = 6,
            BottomMiddle = 7,
            BottomRight = 8,
        }

        [JsonPropertyName("c")]
        public string Content { get; set; }

        [JsonPropertyName("h")]
        public double Height { get; set; }

        [JsonPropertyName("w")]
        public double Width { get; set; }

        [JsonPropertyName("a")]
        public AnchorEnum Anchor { get; set; }

        [JsonPropertyName("p")]
        public RPoint2d Position { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Text, this);
        }
    }
}
