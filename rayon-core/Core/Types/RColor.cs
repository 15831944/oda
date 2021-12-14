// <copyright file="RColor.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Types
{
    using System;
    using System.Collections.Generic;
    using System.Text.Json.Serialization;

    public class RColor
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="RColor"/> class.
        /// </summary>
        /// <param name="r"></param>
        /// <param name="g"></param>
        /// <param name="b"></param>
        /// <param name="a"></param>
        public RColor(double r, double g, double b, double a)
        {
            this.R = r;
            this.G = g;
            this.B = b;
            this.A = a;
        }

        [JsonPropertyName("r")]
        public double R { get; set; }

        [JsonPropertyName("g")]
        public double G { get; set; }

        [JsonPropertyName("b")]
        public double B { get; set; }

        [JsonPropertyName("a")]
        public double A { get; set; }

        public static RColor Red()
        {
            return new RColor(1, 0, 0, 1);
        }

        public static RColor Blue()
        {
            return new RColor(0, 0, 1, 1);
        }

        public static RColor Green()
        {
            return new RColor(0, 1, 0, 1);
        }

        public static RColor Black()
        {
            return new RColor(0, 0, 0, 1);
        }
    }
}
