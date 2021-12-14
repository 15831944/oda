using System;
using System.Collections.Generic;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Components
{
    public class Component
    {
        private string valueAsString;

        private static JsonSerializerOptions options = new JsonSerializerOptions
        {
            IgnoreNullValues = true,
        };

        public Component()
        {
        }

        public Component(Element element, ComponentTypeEnum componentType, ComponentValue value)
        {
            this.Handle = element.Handle;
            this.ComponentType = componentType;
            this.Value = value;
        }

        public Component(string handle, ComponentTypeEnum componentType, ComponentValue value)
        {
            this.Handle = handle;
            this.ComponentType = componentType;
            this.Value = value;
        }

        [JsonIgnore]
        public string Handle { get; set; }

        [JsonPropertyName("t")]
        public ComponentTypeEnum ComponentType { get; set; }

        [JsonPropertyName("v")]
        public string ValueAsString
        {
            get
            {
                if (this.valueAsString == null)
                {
                    string serializedValue = JsonSerializer.Serialize(this.Value, options);
                    this.valueAsString = serializedValue;
                    return serializedValue;
                }
                return this.valueAsString;
            }
            set
            {
                this.valueAsString = value;
            }
        }

        [JsonIgnore]
        public ComponentValue Value { get; set; }

        public Guid ModelId { get; set; }

        public DateTime? UpdatedAt { get; set; }

        public Guid ModifiedById { get; set; }

        public virtual Element Element { get; set; }

        public virtual User ModifiedBy { get; set; }

        public enum ComponentTypeEnum
        {
            Handle = 0,
            Immediate = 1,
            Pickable = 2,
            Hovered = 3,
            StrokeStyleId = 4,
            Path = 5,
            PointStyleId = 6,
            Point = 7,
            Bbox = 8,
            Category = 9,
            Selected = 10,
            Hidden = 11,
            Segment = 12,
            Arc = 13,
            Text = 14,
            Anchor = 15,
            Name = 16,
            Geometry = 17,
            Grid = 18,
            Image = 19,
            Style = 20,
            Parent = 21,
            Layer = 22,
            LayerId = 23,
            Active = 24,
            Marked = 25,
            ControlLine = 26,
            ModelSettings = 27,
            GridStyleId = 28,
            TextStyleId = 29,
            ImageStyleId = 30,
            HatchStyleId = 31,
            Transform = 32,
        }
    }
}
