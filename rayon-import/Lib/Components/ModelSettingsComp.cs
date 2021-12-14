using System;
using System.Collections.Generic;
using System.Text;
using RayonImport.Lib.Geometry;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Components
{
    /// <summary>
    /// </summary>
    public class ModelSettingsComp : ComponentValue
    {
        public ModelSettingsComp()
        {
        }

        public ModelSettingsComp(
            UnitSystem unit
        )
            : base()
        {
            this.Unit = unit;
        }

        [JsonPropertyName("u")]
        public UnitSystem Unit { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.ModelSettings, this);
        }
    }

    public enum UnitSystem : byte
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
        Unset = 255
    }
}
