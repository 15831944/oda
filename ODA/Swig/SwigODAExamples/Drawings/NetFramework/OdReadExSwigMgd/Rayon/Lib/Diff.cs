// <copyright file="Diff.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json;
    using System.Text.Json.Serialization;
    using Rayon.Lib.Components;

    public class Diff
    {
        private static readonly JsonSerializerOptions Options = new JsonSerializerOptions
        {
            Converters = { new JsonStringEnumConverter() },
        };

        public enum EventTypeEnum
        {
            Inserted = 0,
            Modified = 1,
            Removed = 2,
        }

        public enum ComponentTypeEnum
        {
            Handle = 0,
            Immediate = 1,
            Pickable = 2,
            Hovered = 3,

            // StrokeStyleId = 4, // DEPRECATED
            Path = 5,

            // PointStyleId = 6, // DEPRECATED
            Point = 7,

            Bbox = 8,
            Category = 9,
            Selected = 10,
            Hidden = 11,

            // Segment = 12, DEPRECATED
            Arc = 13,

            Text = 14,
            Name = 16,
            RenderBuffer = 17,
            Grid = 18,

            // Image = 19, DEPRECATED
            // Style = 20, DEPRECATED
            Parent = 21,

            Layer = 22,
            LayerId = 23,
            Active = 24,
            Marked = 25,
            ControlLine = 26,
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
        }

        public Guid Id { get; set; }

        [JsonPropertyName("h")]
        public string Handle { get; set; }

        [JsonPropertyName("t")]
        public Component.ComponentTypeEnum ComponentType { get; set; }

        [JsonPropertyName("e")]
        public int Event { get; set; }

        [JsonIgnore]
        public ComponentValue Prev { get; set; }

        [JsonIgnore]
        public ComponentValue Current { get; set; }

        [JsonPropertyName("prev")]
        public string PrevAsString
        {
            get
            {
                return JsonSerializer.Serialize(this.Prev, Options);
            }

            set
            {
            }
        }

        [JsonPropertyName("curr")]
        public string CurrentAsString
        {
            get
            {
                return JsonSerializer.Serialize(this.Current, Options);
            }

            set
            {
            }
        }

        public Guid CommitId { get; set; }

        public virtual Commit Commit { get; set; }
    }
}
