using System.Collections.Generic;
using Rayon.Core.Types;
using Rayon.Core.Components;
using Rayon.Core.Extensions;
using Rhino.Geometry;
using RhinoUnitSystem = Rhino.UnitSystem;
using UnitSystemEnum = Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum;

namespace Rayon.Rhino.Extension
{
    public static class Extension
    {
        public static RPoint2d ToRayon(this Point3d point, ModelSettingsComp.UnitSystemEnum unit)
        {
            double inMillimeters = unit.ToMillimeters();
            return new RPoint2d(point.X * inMillimeters, point.Y * inMillimeters);
        }

        public static UnitSystemEnum ToRayon(this RhinoUnitSystem unit)
        {
            return unit switch
            {
                RhinoUnitSystem.Angstroms => UnitSystemEnum.Angstroms,
                RhinoUnitSystem.AstronomicalUnits => UnitSystemEnum.AstronomicalUnits,
                RhinoUnitSystem.Centimeters => UnitSystemEnum.Centimeters,
                RhinoUnitSystem.CustomUnits => UnitSystemEnum.CustomUnits,
                RhinoUnitSystem.Decimeters => UnitSystemEnum.Decimeters,
                RhinoUnitSystem.Dekameters => UnitSystemEnum.Dekameters,
                RhinoUnitSystem.Feet => UnitSystemEnum.Feet,
                RhinoUnitSystem.Gigameters => UnitSystemEnum.Gigameters,
                RhinoUnitSystem.Hectometers => UnitSystemEnum.Hectometers,
                RhinoUnitSystem.Inches => UnitSystemEnum.Inches,
                RhinoUnitSystem.Kilometers => UnitSystemEnum.Kilometers,
                RhinoUnitSystem.LightYears => UnitSystemEnum.LightYears,
                RhinoUnitSystem.Megameters => UnitSystemEnum.Megameters,
                RhinoUnitSystem.Meters => UnitSystemEnum.Meters,
                RhinoUnitSystem.Microinches => UnitSystemEnum.Microinches,
                RhinoUnitSystem.Microns => UnitSystemEnum.Microns,
                RhinoUnitSystem.Miles => UnitSystemEnum.Miles,
                RhinoUnitSystem.Millimeters => UnitSystemEnum.Millimeters,
                RhinoUnitSystem.Mils => UnitSystemEnum.Mils,
                RhinoUnitSystem.NauticalMiles => UnitSystemEnum.NauticalMiles,
                RhinoUnitSystem.None => UnitSystemEnum.None,
                RhinoUnitSystem.Nanometers => UnitSystemEnum.Nanometers,
                RhinoUnitSystem.Yards => UnitSystemEnum.Yards,
                RhinoUnitSystem.PrinterPoints => UnitSystemEnum.PrinterPoints,
                RhinoUnitSystem.PrinterPicas => UnitSystemEnum.PrinterPicas,
                RhinoUnitSystem.Parsecs => UnitSystemEnum.Parsecs,
                RhinoUnitSystem.Unset => UnitSystemEnum.Unset,
                _ => UnitSystemEnum.Meters,
            };
        }

        /// <summary>
        /// Converts the curve to beziercurves. Rational parts of the curve
        /// will be made non-rational and approximated by increasing the number
        /// of controlpoints by a factor 10.
        /// see: https://discourse.mcneel.com/t/converttobeziers/7973/6
        /// </summary>
        /// <param name="crv">The curve.</param>
        /// <returns></returns>
        public static BezierCurve[] ToBezier(this NurbsCurve crv)
        {
            List<BezierCurve> result = new List<BezierCurve>();
            if (!crv.MakePiecewiseBezier(true)) // this does the heavy lifting, all we need to do is convert to a list of BZ
                return null;

            Point4d[] cp = new Point4d[crv.Order];
            int nTotal = 1;
            while (nTotal < crv.Points.Count)
            {
                --nTotal;
                for (int i = 0; i < crv.Order; ++i)
                {
                    ControlPoint aControlPoint = crv.Points[nTotal++];
                    double w = aControlPoint.Weight;
                    cp[i] = new Point4d(w * aControlPoint.Location.X, w * aControlPoint.Location.Y,
                                        w * aControlPoint.Location.Z, w);
                }

                result.Add(new BezierCurve(cp));
            }
            return result.ToArray();
        }

        public static RColor ToRayon(this System.Drawing.Color color)
        {
            var rayonColor = new RColor(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0);
            return rayonColor;
        }
    }
}
