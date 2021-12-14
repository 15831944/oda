namespace Rayon.Lib.Components.Styles
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json.Serialization;
    using Rayon.Lib.Components.Types;
    using Rayon.Lib.Geometry;

    /// <summary>
    /// A class depicting the properties of a Grid style for a Style Entity.
    /// This style is ony used internally currently.
    /// </summary>
    internal class GridStyleComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="GridStyleComp"/> class.
        /// </summary>
        /// <param name="color">The primary color of the point</param>
        /// <param name="secondaryColor">The color of the secondary axis of the grid</param>
        /// <param name="size">The thickness of the axis or the dots</param>
        /// <param name="unit">The length between two axis</param>
        /// <param name="subdivision">The number of secondary axis</param>
        /// <param name="shape">The shape of the grid: lines or dots</param>
        public GridStyleComp(RColor color, RColor secondaryColor, double size, double unit, int subdivision, GridShapeEnum shape)
            : base()
        {
            this.Color = color;
            this.Shape = shape;
            this.Size = size;
            this.SecondaryColor = secondaryColor;
            this.Unit = unit;
            this.Subdivision = subdivision;
        }

        public enum GridShapeEnum
        {
            Points = 0,
            Lines = 1,
        }

        [JsonPropertyName("c")]
        public RColor Color { get; set; }

        [JsonPropertyName("sc")]
        public RColor SecondaryColor { get; set; }

        /// <summary>
        ///  The thickness of the grid in px
        /// </summary>
        [JsonPropertyName("s")]
        public double Size { get; set; }

        /// <summary>
        ///  The length of a grid unit in world units
        /// </summary>
        [JsonPropertyName("u")]
        public double Unit { get; set; }

        /// <summary>
        ///  The number of subdivision of the grid (eg. : 5)
        /// </summary>
        [JsonPropertyName("sd")]
        public int Subdivision { get; set; }

        [JsonPropertyName("sh")]
        public GridShapeEnum Shape { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.PointStyle, this);
        }
    }
}
