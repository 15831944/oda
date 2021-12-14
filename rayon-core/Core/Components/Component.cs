// <copyright file="Component.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Text.Json;
    using System.Text.Json.Serialization;

    public class Component
    {
        private static readonly JsonSerializerOptions Options = new JsonSerializerOptions
        {
            IgnoreNullValues = true,
        };

        private string valueAsString;

        /// <summary>
        /// Initializes a new instance of the <see cref="Component"/> class.
        /// </summary>
        public Component()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Component"/> class.
        /// </summary>
        /// <param name="element"></param>
        /// <param name="componentType"></param>
        /// <param name="value"></param>
        public Component(Element element, ComponentTypeEnum componentType, ComponentValue value)
        {
            this.Handle = element.Handle;
            this.ComponentType = componentType;
            this.Value = value;

            // add the linked handle if necessary
            if (value is ILinked linkedComponent)
            {
                this.LinkedElement = linkedComponent.GetLinkedHandle();
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Component"/> class.
        /// </summary>
        /// <param name="handle"></param>
        /// <param name="componentType"></param>
        /// <param name="value"></param>
        public Component(string handle, ComponentTypeEnum componentType, ComponentValue value)
        {
            this.Handle = handle;
            this.ComponentType = componentType;
            this.Value = value;

            // add the linked handle if necessary
            if (value is ILinked linkedComponent)
            {
                this.LinkedElement = linkedComponent.GetLinkedHandle();
            }
        }

        public enum ComponentTypeEnum
        {
            Handle = 0, // not persisted
            Immediate = 1, // not persisted
            Pickable = 2, // not persisted
            Hovered = 3, // not persisted

            // StrokeStyleId = 4, // DEPRECATED
            Path = 5,

            // PointStyleId = 6, // DEPRECATED
            Point = 7,

            Bbox = 8,
            Category = 9,
            Selected = 10, // not persisted
            Hidden = 11,

            // Segment = 12, DEPRECATED
            Arc = 13,

            Text = 14,
            Name = 16,
            RenderBuffer = 17,
            Grid = 18,

            // Image = 19, DEPRECATED
            // Style = 20, DEPRECATED
            // Parent = 21, DEPRECATED
            Layer = 22,

            LayerId = 23,
            Active = 24, // not persisted

            // Marked = 25, DEPRECATED
            ControlLine = 26, // not persisted

            ModelSettings = 27,

            // GridStyleId = 28, DEPRECATED
            // TextStyleId = 29, DEPRECATED
            // ImageStyleId = 30,  DEPRECATED
            // HatchStyleId = 31,  DEPRECATED
            Transform = 32,

            Locked = 33,
            Group = 34,
            GroupId = 35,
            Rectangle = 36,
            RegularPolygon = 37,
            Tag = 38,
            StyleId = 39,
            StrokeStyle = 40,
            FillStyle = 41,
            TextStyle = 42,
            GridStyle = 43,
            PointStyle = 44,
            Builtin = 45,
            Block = 47,
            BlockId = 48,
            BlockInstance = 49,
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
                    string serializedValue = JsonSerializer.Serialize(this.Value, Options);
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

        [JsonPropertyName("lh")]
        public string LinkedElement { get; set; }

        [JsonIgnore]
        public ComponentValue Value { get; set; }

        [JsonIgnore]
        public Guid ModelId { get; set; }

        [JsonIgnore]
        public DateTime? UpdatedAt { get; set; }

        [JsonIgnore]
        public Guid ModifiedById { get; set; }

        [JsonIgnore]
        public virtual Element Element { get; set; }

        [JsonIgnore]
        public virtual User ModifiedBy { get; set; }
    }
}
