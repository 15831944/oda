// <copyright file="Extensions.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Extensions
{
    using System;
    using Rayon.Core.Components;

    /// <summary>
    /// Class Extensions in Rayon Core.
    /// </summary>
    public static class Extensions
    {
        /// <summary>
        /// Returns the ratio of the given unit to millimeters.
        /// </summary>
        /// <param name="unit">A given unit.</param>
        /// <returns>The unit in millimeters.</returns>
        public static double ToMillimeters(this ModelSettingsComp.UnitSystemEnum unit)
        {
            return unit switch
            {
                ModelSettingsComp.UnitSystemEnum.Angstroms => throw new ArgumentException("Unit not supported"),
                ModelSettingsComp.UnitSystemEnum.AstronomicalUnits => throw new ArgumentException("Unit not supported"),
                ModelSettingsComp.UnitSystemEnum.Centimeters => 10.0,
                ModelSettingsComp.UnitSystemEnum.CustomUnits => 1.0,
                ModelSettingsComp.UnitSystemEnum.Decimeters => 100.0,
                ModelSettingsComp.UnitSystemEnum.Dekameters => 10000.0,
                ModelSettingsComp.UnitSystemEnum.Feet => 304.8,
                ModelSettingsComp.UnitSystemEnum.Gigameters => throw new ArgumentException("Unit not supported"),
                ModelSettingsComp.UnitSystemEnum.Hectometers => 100000.0,
                ModelSettingsComp.UnitSystemEnum.Inches => 25.4,
                ModelSettingsComp.UnitSystemEnum.Kilometers => 1000000.0,
                ModelSettingsComp.UnitSystemEnum.LightYears => throw new ArgumentException("Unit not supported"),
                ModelSettingsComp.UnitSystemEnum.Megameters => throw new ArgumentException("Unit not supported"),
                ModelSettingsComp.UnitSystemEnum.Meters => 1000.0,
                ModelSettingsComp.UnitSystemEnum.Microinches => 0.0000254,
                ModelSettingsComp.UnitSystemEnum.Microns => throw new ArgumentException("Unit not supported"),
                ModelSettingsComp.UnitSystemEnum.Miles => throw new ArgumentException("Unit not supported"),
                ModelSettingsComp.UnitSystemEnum.Millimeters => 1.0,
                ModelSettingsComp.UnitSystemEnum.Mils => 0.0254,
                ModelSettingsComp.UnitSystemEnum.NauticalMiles => throw new ArgumentException("NauticalMiles Unit not supported"),
                ModelSettingsComp.UnitSystemEnum.None => 1.0,
                ModelSettingsComp.UnitSystemEnum.Nanometers => 1e-6,
                ModelSettingsComp.UnitSystemEnum.Yards => 914.4,
                ModelSettingsComp.UnitSystemEnum.PrinterPoints => throw new ArgumentException("PrinterPoints Unit not supported"),
                ModelSettingsComp.UnitSystemEnum.PrinterPicas => throw new ArgumentException("PrinterPicas Unit not supported"),
                ModelSettingsComp.UnitSystemEnum.Parsecs => throw new ArgumentException("Parsec Unit not supported"),
                ModelSettingsComp.UnitSystemEnum.Unset => 1.0,
                _ => 1.0,
            };
        }
    }
}
