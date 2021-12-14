namespace Rayon.Lib.Components.Styles
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json.Serialization;
    using Rayon.Lib.Components.Types;
    using Rayon.Lib.Geometry;

    /// <summary>
    /// A class depicting the properties of a Fill style for a Style Entity.
    /// </summary>
    internal class FillStyleComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="FillStyleComp"/> class.
        /// </summary>
        /// <param name="color">The color of the fill</param>
        /// <param name="texture">
        /// A nullable parameter indicating the id of an image texture.
        /// If provided the shape will be filled with the texture instead of the fill color.
        /// </param>
        public FillStyleComp(RColor color, string texture)
            : base()
        {
            this.Color = color;
            this.Texture = texture;
        }

        [JsonPropertyName("c")]
        public RColor Color { get; set; }

        [JsonPropertyName("t")]
        public string Texture { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.FillStyle, this);
        }
    }
}
