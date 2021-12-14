// <copyright file="ModelSettingsComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json.Serialization;
    using Rayon.Lib.Geometry;

    /// <summary>
    /// </summary>
    public class ModelSettingsComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="ModelSettingsComp"/> class.
        /// </summary>
        public ModelSettingsComp()
        {
            // per default we use the meters unit.
            // Note : all world lenghth are stored in millimeters inside rayon.
            this.Unit = UnitSystemEnum.Meters;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="ModelSettingsComp"/> class.
        /// </summary>
        /// <param name="unit"></param>
        public ModelSettingsComp(
            UnitSystemEnum unit)
            : base()
        {
            this.Unit = unit;
        }

        public enum UnitSystemEnum : byte
        {
            None = 0,
            Microns = 1,
            Millimeters = 2,
            Centimeters = 3,
            Meters = 4,
            Kilometers = 5,
            Microinches = 6,
            Mils = 7,
            Inches = 8,
            Feet = 9,
            Miles = 10,
            CustomUnits = 11,
            Angstroms = 12,
            Nanometers = 13,
            Decimeters = 14,
            Dekameters = 15,
            Hectometers = 16,
            Megameters = 17,
            Gigameters = 18,
            Yards = 19,
            PrinterPoints = 20,
            PrinterPicas = 21,
            NauticalMiles = 22,
            AstronomicalUnits = 23,
            LightYears = 24,
            Parsecs = 25,
            Unset = 255,
        }

        [JsonPropertyName("u")]
        public UnitSystemEnum Unit { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.ModelSettings, this);
        }
    }
}
