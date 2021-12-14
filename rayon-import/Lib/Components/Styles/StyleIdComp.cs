using System;
using System.Collections.Generic;
using System.Text;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Components
{
    public abstract class StyleIdComp : ComponentValue
    {
        public StyleIdComp(string styleId)
           : base()
        {
            this.StyleId = styleId;
        }

        public StyleIdComp(Element style)
           : base()
        {
            this.StyleId = style.Handle;
        }

        [JsonPropertyName("h")]
        public string StyleId { get; set; }
    }
}
