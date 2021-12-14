using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Components.Types
{
    public class Color
    {
        public Color(double r, double g, double b, double a)
        {
            this.R = r;
            this.G = g;
            this.B = b;
            this.A = a;
        }

        public static Color Red()
        {
            return new Color(1, 0, 0, 1);
        }

        public static Color Blue()
        {
            return new Color(0, 0, 1, 1);
        }

        public static Color Green()
        {
            return new Color(0, 1, 0, 1);
        }

        public static Color Black()
        {
            return new Color(0, 0, 0, 1);
        }

        [JsonPropertyName("r")]
        public double R { get; set; }

        [JsonPropertyName("g")]
        public double G { get; set; }

        [JsonPropertyName("b")]
        public double B { get; set; }

        [JsonPropertyName("a")]
        public double A { get; set; }
    }
}
