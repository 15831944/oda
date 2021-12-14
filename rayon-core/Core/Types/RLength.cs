// <copyright file="RLength.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

using System;
using System.Text.Json;
using System.Text.Json.Serialization;
using Rayon.Core.Components;
using Rayon.Core.Extensions;

namespace Rayon.Core.Types
{
    [JsonConverter(typeof(LengthConverter))]
    public class RLength : IMillimeters
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
            /// <summary>
            /// Screen pixels
            /// </summary>
            Pixels,

            /// <summary>
            /// World unit (eg. Meters, Millimeters)
            /// </summary>
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

        public void ToMillimeters(ModelSettingsComp.UnitSystemEnum unit)
        {
            if (this.Type == LengthTypeEnum.Units)
            {
                this.Value *= unit.ToMillimeters();
            }
        }

        /// <summary>
        /// Expected serialization: {\"pixels\":3.0}".
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
}
