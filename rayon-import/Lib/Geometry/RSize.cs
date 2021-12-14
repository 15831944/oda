using System;
using System.Collections.Generic;
using System.Text;
using System.Text.Json.Serialization;
using System.Text.Json;

namespace RayonImport.Lib.Geometry
{
    [JsonConverter(typeof(SizeConverter))]
    public class RSize : RPoint2d
    {
        public RSize(double x, double y) : base(x, y)
        {
        }
    }

    /// <summary>
    /// Expected serialization: [12.2,14.5]
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
