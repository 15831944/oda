// <copyright file="RAngle.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Geometry
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json;
    using System.Text.Json.Serialization;

    public class RAngle
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="RAngle"/> class.
        /// </summary>
        /// <param name="angle"></param>
        public RAngle(double angle)
        {
            this.Radians = angle;
        }

        [JsonPropertyName("radians")]
        public double Radians { get; set; }

        public static RAngle TwoPi()
        {
            return new RAngle(Math.PI * 2);
        }
    }
}
