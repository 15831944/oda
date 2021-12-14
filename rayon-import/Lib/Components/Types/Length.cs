using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Components.Types
{
    [JsonConverter(typeof(LengthConverter))]
    public class Length
    {
        public Length(double value, LengthTypeEnum type)
        {
            this.Value = value;
            this.Type = type;
        }

        public static Length Units(double value)
        {
            return new Length(value, LengthTypeEnum.Units);
        }

        public static Length Pixels(double value)
        {
            return new Length(value, LengthTypeEnum.Pixels);
        }

        public double Value { get; set; }

        public LengthTypeEnum Type { get; set; }

        public enum LengthTypeEnum
        {
            Pixels,
            Units,
        }
    }

    /// <summary>
    /// Expected serialization: {\"pixels\":3.0}"
    /// </summary>
    public class LengthConverter : JsonConverter<Length>
    {
        public override Length Read(
            ref Utf8JsonReader reader,
            Type typeToConvert,
            JsonSerializerOptions options) => new Length(0.0, Length.LengthTypeEnum.Pixels);

        public override void Write(
            Utf8JsonWriter writer,
            Length length,
            JsonSerializerOptions options)
        {
            writer.WriteStartObject();
            writer.WritePropertyName(Enum.GetName(typeof(Length.LengthTypeEnum), length.Type));
            writer.WriteNumberValue(length.Value);
            writer.WriteEndObject();
        }
    }
}
