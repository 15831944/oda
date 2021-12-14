// <copyright file="ComponentValue.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Text.Json;
    using System.Text.Json.Serialization;
    using Rayon.Core.Components.Path;
    using Rayon.Core.Components.Styles;

    /// <summary>
    /// Abstract class containing the value of a component
    /// </summary>
    [JsonConverter(typeof(ComponentValueConverter))]
    public abstract class ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="ComponentValue"/> class.
        /// </summary>
        public ComponentValue()
        {
        }

        public abstract Component ToComponent(Element entity);

        /// <summary>
        /// Custom serialization to handle rust enums
        /// </summary>
        public class ComponentValueConverter : JsonConverter<ComponentValue>
        {
            public override ComponentValue Read(
                ref Utf8JsonReader reader,
                Type typeToConvert,
                JsonSerializerOptions options)
            {
                throw new NotImplementedException();
            }

            public override void Write(
                Utf8JsonWriter writer,
                ComponentValue value,
                JsonSerializerOptions options)
            {
                switch (value)
                {
                    case PathComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case PointComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case BboxComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case CategoryComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case HiddenComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case ArcComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case TextComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case NameComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case RenderBufferComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case GridComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case LayerComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case LayerIdComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case ModelSettingsComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case TransformComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case LockedComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case GroupComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case GroupIdComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case RectangleComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case RegularPolygonComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case TagComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case StyleIdComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case StrokeStyleComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case FillStyleComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case TextStyleComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case GridStyleComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case PointStyleComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case BuiltinComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case BlockComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case BlockIdComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case BlockInstanceComp comp:
                        JsonSerializer.Serialize(writer, comp);
                        break;

                    case null:
                        throw new ArgumentNullException(nameof(value));
                }

                // This is fugly but could not loose time to find a cleaner way
            }
        }
    }
}
