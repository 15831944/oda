using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Components
{
    /// <summary>
    /// Abstract class containing the value of a component
    /// </summary>
    [JsonConverter(typeof(ComponentValueConverter))]
    public abstract class ComponentValue
    {
        public ComponentValue()
        {
        }

        public abstract Component ToComponent(Element entity);
    }

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
            // This is fugly but could not loose time to find a cleaner way
            if (value is ArcComp arcComp)
            {
                JsonSerializer.Serialize(writer, arcComp);
            }
            else if (value is BboxComp comp)
            {
                JsonSerializer.Serialize(writer, comp);
            }
            else if (value is CategoryComp categoryComp)
            {
                JsonSerializer.Serialize(writer, categoryComp);
            }
            else if (value is GeometryComp geometryComp)
            {
                JsonSerializer.Serialize(writer, geometryComp);
            }
            else if (value is GridComp gridComp)
            {
                JsonSerializer.Serialize(writer, gridComp);
            }
            else if (value is HiddenComp hiddenComp)
            {
                JsonSerializer.Serialize(writer, hiddenComp);
            }
            else if (value is LayerComp layerComp)
            {
                JsonSerializer.Serialize(writer, layerComp);
            }
            else if (value is LayerIdComp layerIdComp)
            {
                JsonSerializer.Serialize(writer, layerIdComp);
            }
            else if (value is NameComp nameComp)
            {
                JsonSerializer.Serialize(writer, nameComp);
            }
            else if (value is PathComp pathComp)
            {
                JsonSerializer.Serialize(writer, pathComp);
            }
            else if (value is PointComp pointComp)
            {
                JsonSerializer.Serialize(writer, pointComp);
            }
            else if (value is SegmentComp segmentComp)
            {
                JsonSerializer.Serialize(writer, segmentComp);
            }
            else if (value is StyleComp styleComp)
            {
                JsonSerializer.Serialize(writer, styleComp);
            }
            else if (value is StrokeStyleIdComp styleIdComp)
            {
                JsonSerializer.Serialize(writer, styleIdComp);
            }
            else if (value is ModelSettingsComp modelSettingsComp)
            {
                JsonSerializer.Serialize(writer, modelSettingsComp);
            }
            else if (value is TransformComp transformComp)
            {
                JsonSerializer.Serialize(writer, transformComp);
            }
            else
            {
                throw new ArgumentException();
            }
        }
    }
}
