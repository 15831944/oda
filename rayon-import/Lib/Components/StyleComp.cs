using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;
using RayonImport.Lib.Components.Types;
using RayonImport.Lib.Geometry;

namespace RayonImport.Lib.Components
{
    /// <summary>
    /// A class describing a style of an entity
    /// </summary>
    [JsonConverter(typeof(StyleCompConverter))]
    public class StyleComp : ComponentValue
    {
        public StyleComp(SpecificStyle specific, StyleCreatorEnum creator)
            : base()
        {
            this.Specific = specific;
            this.Creator = creator;
        }

        [JsonPropertyName("s")]
        public SpecificStyle Specific { get; set; }

        public enum StyleCreatorEnum
        {
            /// Defined by the user
            User,

            /// Automatically created by the app for the user to use
            Default,

            /// Internal styles that should not be queried or modified by the user
            Builtin,
        }

        [JsonPropertyName("c")]
        public StyleCreatorEnum Creator { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Style, this);
        }
    }

    /// <summary>
    /// Custom serialization to handle rust enums
    /// </summary>
    public class StyleCompConverter : JsonConverter<StyleComp>
    {
        public override StyleComp Read(
            ref Utf8JsonReader reader,
            Type typeToConvert,
            JsonSerializerOptions options)
        {
            throw new NotImplementedException();
        }

        public override void Write(
            Utf8JsonWriter writer,
            StyleComp styleComp,
            JsonSerializerOptions options)
        {
            writer.WriteStartObject();
            writer.WriteNumber("c", (int)styleComp.Creator);
            writer.WritePropertyName("s");

            if (styleComp.Specific is PathStyle pathStyle)
            {
                JsonSerializer.Serialize(writer, pathStyle);
            }
            else if (styleComp.Specific is PointStyle pointStyle)
            {
                JsonSerializer.Serialize(writer, pointStyle);
            }
            else if (styleComp.Specific is TextStyle textStyle)
            {
                JsonSerializer.Serialize(writer, textStyle);
            }
            else if (styleComp.Specific is GridStyle gridStyle)
            {
                JsonSerializer.Serialize(writer, gridStyle);
            }
            else if (styleComp.Specific is ImageStyle imageStyle)
            {
                JsonSerializer.Serialize(writer, imageStyle);
            }
            else
            {
                throw new ArgumentException();
            }
            writer.WriteEndObject();
        }
    }

    public abstract class SpecificStyle
    {
        public SpecificStyle(SpecificStyleEnum type)
        {
            this.Type = type;
        }

        [JsonConverter(typeof(JsonStringEnumConverter))]
        [JsonPropertyName("t")]
        public SpecificStyleEnum Type { get; set; }

        public enum SpecificStyleEnum
        {
            Stroke,
            Grid,
            Point,
            Text,
            Image,
        }
    }

    public class PathStyle : SpecificStyle
    {
        public PathStyle(Color color, Length thickness, DashPatternEnum dashPattern)
            : base(SpecificStyleEnum.Stroke)
        {
            this.Color = color;
            this.Thickness = thickness;
            this.Pattern = dashPattern;
        }

        public static PathStyle Default()
        {
            return new PathStyle(Color.Black(), Length.Pixels(1), DashPatternEnum.Solid);
        }

        public enum DashPatternEnum
        {
            Solid = 0,
            DenselyDotted = 1,
            Dotted = 2,
            LooselyDotted = 3,
            DenselyDashed = 4,
            Dashed = 5,
            LooselyDashed = 6,
            DenselyDashDotted = 7,
            DashDotted = 8,
            LooselyDashDotted = 9,
            DenselyDashDotDotted = 10,
            DashDotDotted = 11,
            LooselyDashDotDotted = 12,
            ForwardArrow = 13,
            BackwardArrow = 14,
        }

        [JsonPropertyName("color")]
        public Color Color { get; set; }

        [JsonPropertyName("thickness")]
        public Length Thickness { get; set; }

        [JsonPropertyName("pattern")]
        public DashPatternEnum Pattern { get; set; }
    }

    public class TextStyle : SpecificStyle
    {
        public TextStyle(Length size, Color color) :
            base(SpecificStyleEnum.Text)
        {
            this.Size = size;
            this.Color = color;
        }

        [JsonPropertyName("size")]
        public Length Size { get; set; }

        [JsonPropertyName("color")]
        public Color Color { get; set; }
    }

    public class GridStyle : SpecificStyle
    {
        public GridStyle(Color color, double unit, int subdivision, double pointSize)
            : base(SpecificStyleEnum.Grid)
        {
            this.Color = color;
            this.Unit = unit;
            this.Subdivision = subdivision;
            this.PointSize = pointSize;
        }

        [JsonPropertyName("color")]
        public Color Color { get; set; }

        [JsonPropertyName("unit")]
        public double Unit { get; set; }

        [JsonPropertyName("subdivision")]
        public int Subdivision { get; set; }

        [JsonPropertyName("point_size")]
        public double PointSize { get; set; }
    }

    public class PointStyle : SpecificStyle
    {
        public PointStyle(Color color, Color backgroundColor, double size, PointShapeEnum shape)
            : base(SpecificStyleEnum.Point)
        {
            this.Color = color;
            this.BackgroundColor = backgroundColor;
            this.Size = size;
            this.Shape = shape;
        }

        [JsonPropertyName("color")]
        public Color Color { get; set; }

        [JsonPropertyName("bg_color")]
        public Color BackgroundColor { get; set; }

        [JsonPropertyName("size")]
        public double Size { get; set; }

        public enum PointShapeEnum
        {
            OutlinedSquare = 0,
            OutlinedDisc = 1,
        }

        [JsonPropertyName("shape")]
        public PointShapeEnum Shape { get; set; }
    }

    public class ImageStyle : SpecificStyle
    {
        public ImageStyle(double size, Color color)
            : base(SpecificStyleEnum.Image)
        {
            this.Size = size;
            this.Color = color;
        }

        [JsonPropertyName("size")]
        public double Size { get; set; }

        [JsonPropertyName("color")]
        public Color Color { get; set; }
    }
}
