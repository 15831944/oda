using System;
using System.Collections.Generic;
using System.Text;
using System.Text.Json.Serialization;
using System.Text.Json;

namespace RayonImport.Lib.Geometry
{
    [JsonConverter(typeof(VectorConverter))]
    public class RVector : RPoint2d
    {
        public RVector(double x, double y) : base(x, y)
        {
        }
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
