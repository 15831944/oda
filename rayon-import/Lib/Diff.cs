using System;
using System.Collections.Generic;
using System.Text;
using RayonImport.Lib.Components;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace RayonImport.Lib
{
    public class Diff
    {
        private static JsonSerializerOptions options = new JsonSerializerOptions
        {
            Converters = { new JsonStringEnumConverter() }
        };

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
                return JsonSerializer.Serialize(this.Prev, options);
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
                return JsonSerializer.Serialize(this.Current, options);
            }
            set
            {
            }
        }

        public Guid CommitId { get; set; }

        public virtual Commit Commit { get; set; }

        public enum EventTypeEnum
        {
            Inserted = 0,
            Modified = 1,
            Removed = 2,
        }

        /// <summary>
        /// This really sucks but the enum must be different for the db
        /// </summary>
        public enum ComponentTypeEnum
        {
            Handle = 0,
            Immediate = 1,
            Pickable = 2,
            Hovered = 3,
            StyleId = 4,
            Path = 5,
            RenderGroupIndex = 6,
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
        }
    }
}
