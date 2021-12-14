// <copyright file="RSize.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Geometry
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json;
    using System.Text.Json.Serialization;

    [JsonConverter(typeof(SizeConverter))]
    public class RSize : RPoint2d
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="RSize"/> class.
        /// </summary>
        /// <param name="width">The width of the size</param>
        /// <param name="height">The height of the size</param>
        public RSize(double width, double height)
            : base(width, height)
        {
        }

        public double Width()
        {
            return this.X;
        }

        public double Height()
        {
            return this.Y;
        }

        /// <summary>
        /// Expected serialization: [12.2,14.5]
        /// Width first, height second.
        /// </summary>
        public class SizeConverter : JsonConverter<RSize>
        {
            public override RSize Read(
                ref Utf8JsonReader reader,
                Type typeToConvert,
                JsonSerializerOptions options) => new RSize(0.0, 0.0);

            public override void Write(
                Utf8JsonWriter writer,
                RSize size,
                JsonSerializerOptions options)
            {
                writer.WriteStartArray();
                writer.WriteNumberValue(size.X);
                writer.WriteNumberValue(size.Y);
                writer.WriteEndArray();
            }
        }
    }
}
