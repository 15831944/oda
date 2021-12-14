// <copyright file="RVector.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Geometry
{
    using System;
    using System.Text.Json;
    using System.Text.Json.Serialization;

    [JsonConverter(typeof(VectorConverter))]
    public class RVector : RPoint2d
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="RVector"/> class.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        public RVector(double x, double y)
            : base(x, y)
        {
        }

        /// <summary>
        /// Expected serialization: [12.2,14.5]
        /// </summary>
        public class VectorConverter : JsonConverter<RVector>
        {
            public override RVector Read(
                ref Utf8JsonReader reader,
                Type typeToConvert,
                JsonSerializerOptions options) => new RVector(0.0, 0.0);

            public override void Write(
                Utf8JsonWriter writer,
                RVector vector,
                JsonSerializerOptions options)
            {
                writer.WriteStartArray();
                writer.WriteNumberValue(vector.X);
                writer.WriteNumberValue(vector.Y);
                writer.WriteEndArray();
            }
        }
    }
}
