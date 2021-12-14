// <copyright file="RegularPolygonComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components.Path
{
    using System.Text.Json.Serialization;
    using Rayon.Core.Types;

    /// <summary>
    /// A component representing the geometry of a regular polygon inscribed inside
    /// a circle of a given radius and a given center and with the given number of sides.
    /// </summary>
    public class RegularPolygonComp : ComponentValue
    {
        public RegularPolygonComp(RPoint2d center, double radius, int sides)
        {
            this.Center = center;
            this.Radius = radius;
            this.Sides = sides;
        }

        [JsonPropertyName("c")]
        public RPoint2d Center { get; set; }

        [JsonPropertyName("r")]
        public double Radius { get; set; }

        [JsonPropertyName("s")]
        public int Sides { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.RegularPolygon, this);
        }
    }
}
