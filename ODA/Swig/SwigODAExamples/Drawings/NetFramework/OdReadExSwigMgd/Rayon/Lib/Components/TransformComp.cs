// <copyright file="TransformComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text.Json;
    using System.Text.Json.Serialization;

    /// <summary>
    /// A component describing a transform in the 2d plane applied to the Element.
    /// When translating, rotating, scaling an Element, its internal geometry is never
    /// modified, only its transform. This enables to chain transforms of linked entities
    /// (eg. group transforms are inherited by their children) and to apply the
    /// transform computation directly in the gpu shader for maximum performance.
    /// </summary>
    [JsonConverter(typeof(TransformCompConverter))]
    public class TransformComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="TransformComp"/> class.
        /// </summary>
        public TransformComp()
        {
            this.Matrix = new double[] { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="TransformComp"/> class.
        /// </summary>
        /// <param name="matrix"></param>
        public TransformComp(IList<double> matrix)
        {
            this.Matrix = matrix;
        }

        /// <summary>
        /// A 3x3 matrix representing a transformation in the 2d plane
        /// </summary>
        public IList<double> Matrix { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Transform, this);
        }

        /// <summary>
        /// Custom JSON Serializater for <see cref="TransformComp"/>
        /// Expected serialization: [0,0,1,0.0,0.2,1.0,2.0,3.0,1.0]
        /// </summary>
        public class TransformCompConverter : JsonConverter<TransformComp>
        {
            public override TransformComp Read(
                ref Utf8JsonReader reader,
                Type typeToConvert,
                JsonSerializerOptions options)
            {
                JsonTokenType tokenType;

                tokenType = reader.TokenType;
                if (tokenType != JsonTokenType.StartArray)
                {
                    throw new FormatException();
                }

                bool found_value;
                var output = new List<double>();
                double double_value;
                for (var i = 0; i < 6; i++)
                {
                    found_value = reader.Read();
                    if (!found_value)
                    {
                        throw new FormatException();
                    }

                    tokenType = reader.TokenType;
                    if (tokenType != JsonTokenType.Number)
                    {
                        throw new FormatException();
                    }

                    double_value = reader.GetDouble();
                    output.Add(double_value);
                }

                found_value = reader.Read();
                if (!found_value)
                {
                    throw new FormatException();
                }

                tokenType = reader.TokenType;
                if (tokenType != JsonTokenType.EndArray)
                {
                    throw new FormatException();
                }

                return new TransformComp(output);
            }

            public override void Write(
                Utf8JsonWriter writer,
                TransformComp transformComp,
                JsonSerializerOptions options)
            {
                writer.WriteStartArray();
                foreach (double value in transformComp.Matrix)
                {
                    writer.WriteNumberValue(value);
                }

                writer.WriteEndArray();
            }
        }
    }
}
