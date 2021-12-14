using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;
using System.Text.Json;

namespace RayonImport.Lib.Components
{
    [JsonConverter(typeof(TransformCompConverter))]
    public class TransformComp : ComponentValue
    {
        public TransformComp()
        {
            this.Matrix = new double[] { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
        }

        public TransformComp(IList<double> matrix)
        {
            this.Matrix = matrix;
        }

        public IList<double> Matrix { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Transform, this);
        }
    }

    /// <summary>
    /// Expected serialization: [12.2,14.5]
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
