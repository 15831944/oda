// <copyright file="PointComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text.Json.Serialization;
    using Rayon.Lib.Geometry;

    public class PointComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="PointComp"/> class.
        /// </summary>
        /// <param name="position"></param>
        public PointComp(RPoint2d position)
            : base()
        {
            this.Position = position;
        }

        [JsonPropertyName("p")]
        public RPoint2d Position { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Point, this);
        }
    }
}
