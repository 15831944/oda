// <copyright file="Length.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components.Types
{
    using System;
    using System.Collections.Generic;
    using System.Text.Json;
    using System.Text.Json.Serialization;

    [JsonConverter(typeof(LengthConverter))]
    public class RLength
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="RLength"/> class.
        /// </summary>
        /// <param name="value"></param>
        /// <param name="type"></param>
        public RLength(double value, LengthTypeEnum type)
        {
            this.Value = value;
            this.Type = type;
        }

        public enum LengthTypeEnum
        {
            Pixels,
            Units,
        }

        public double Value { get; set; }

        public LengthTypeEnum Type { get; set; }

        public static RLength Units(double value)
        {
            return new RLength(value, LengthTypeEnum.Units);
        }

        public static RLength Pixels(double value)
        {
            return new RLength(value, LengthTypeEnum.Pixels);
        }
    }

    /// <summary>
    /// Expected serialization: {\"pixels\":3.0}"
    /// </summary>
    public class LengthConverter : JsonConverter<RLength>
    {
        public override RLength Read(
            ref Utf8JsonReader reader,
            Type typeToConvert,
            JsonSerializerOptions options) => new RLength(0.0, RLength.LengthTypeEnum.Pixels);

        public override void Write(
            Utf8JsonWriter writer,
            RLength length,
            JsonSerializerOptions options)
        {
            writer.WriteStartObject();
            writer.WritePropertyName(Enum.GetName(typeof(RLength.LengthTypeEnum), length.Type));
            writer.WriteNumberValue(length.Value);
            writer.WriteEndObject();
        }
    }
}
