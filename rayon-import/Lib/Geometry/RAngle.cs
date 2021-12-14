using System;
using System.Collections.Generic;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Geometry
{
    public class RAngle
    {
        public RAngle(double angle)
        {
            this.Radians = angle;
        }

        [JsonPropertyName("radians")]
        public double Radians { get; set; }

        public static RAngle TwoPi()
        {
            return new RAngle(Math.PI * 2);
        }
    }
}
