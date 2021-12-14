// <copyright file="RPoint2d.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Geometry
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json;
    using System.Text.Json.Serialization;

    [JsonConverter(typeof(PointConverter))]
    public class RPoint2d
    {
        public const double TOLERANCE = 0.1;

        /// <summary>
        /// Initializes a new instance of the <see cref="RPoint2d"/> class.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        public RPoint2d(double x, double y)
        {
            this.X = x * Constant.UNITFACTOR;
            this.Y = y * Constant.UNITFACTOR;
        }

        public double X { get; set; }

        public double Y { get; set; }

        public static RPoint2d operator +(RPoint2d a, RPoint2d b)
        {
            return new RPoint2d(a.X + b.X, a.Y + b.Y);
        }

        public static RPoint2d operator -(RPoint2d a, RPoint2d b)
        {
            return new RPoint2d(a.X - b.X, a.Y - b.Y);
        }

        public static RPoint2d operator *(RPoint2d a, double b)
        {
            return new RPoint2d(a.X * b, a.Y * b);
        }

        public static double operator *(RPoint2d a, RPoint2d b)
        {
            return (a.X * b.X) + (a.Y * b.Y);
        }

        public bool AlmostEqualTo(RPoint2d other)
        {
            if (other == null)
            {
                return false;
            }

            return (this - other) * (this - other) <= TOLERANCE * TOLERANCE;
        }

        /// <summary>
        /// Expected serialization: [12.2,14.5]
        /// </summary>
        public class PointConverter : JsonConverter<RPoint2d>
        {
            public override RPoint2d Read(
                ref Utf8JsonReader reader,
                Type typeToConvert,
                JsonSerializerOptions options) => new RPoint2d(0.0, 0.0);

            public override void Write(
                Utf8JsonWriter writer,
                RPoint2d point,
                JsonSerializerOptions options)
            {
                writer.WriteStartArray();
                writer.WriteNumberValue(point.X);
                writer.WriteNumberValue(point.Y);
                writer.WriteEndArray();
            }
        }
    }
}
