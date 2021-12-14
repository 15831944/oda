namespace Rayon.Lib.Components.Styles
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json.Serialization;
    using Rayon.Lib.Components.Types;
    using Rayon.Lib.Geometry;

    /// <summary>
    /// A class depicting the properties of a Text style for a Style Entity.
    /// </summary>
    internal class TextStyleComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="TextStyleComp"/> class.
        /// </summary>
        /// <param name="color">The color of the text</param>
        /// <param name="size">The font size of the text</param>
        /// <param name="font">The font identifier</param>
        /// <param name="weight">The weight of the font (not implemented)</param>
        /// <param name="stroke">The optional color of a stroke around the font (wip)</param>
        public TextStyleComp(RColor color, RLength size, FontEnum font, FontWeightEnum weight, RColor stroke)
            : base()
        {
            this.Color = color;
            this.Font = font;
            this.Size = size;
            this.Weight = weight;
            this.Stroke = stroke;
        }

        public enum FontWeightEnum
        {
            Thin = 1,
            Light = 2,
            Book = 3,
            Regular = 4,
            Medium = 5,
            SemiBold = 6,
            Bold = 7,
            ExtraBold = 8,
            UltraBold = 9,
        }

        public enum FontEnum
        {
            RobotoMedium = 0,
            RobotoMonoRegular = 1,
            TrainOneRegular = 2,
            PoppinsMedium = 3,
            MontserratBold = 4,
            OswaldRegular = 5,
            PlayfairDisplayRegular = 6,
            RalewayMedium = 7,
            UbuntuMedium = 8,
            LatoRegular = 9,
            AbelRegular = 10,
            AnonymousProRegular = 11,
            ArchitectsDaughterRegular = 12,
            ComfortaaRegular = 13,
            CourierPrimeRegular = 14,
            DosisRegular = 15,
            LoraRegular = 16,
            QuicksandRegular = 17,
            TinosRegular = 18,
            OpenSansRegular = 19,
        }

        [JsonPropertyName("c")]
        public RColor Color { get; set; }

        [JsonPropertyName("s")]
        public RLength Size { get; set; }

        [JsonPropertyName("f")]
        public FontEnum Font { get; set; }

        [JsonPropertyName("w")]
        public FontWeightEnum Weight { get; set; }

        [JsonPropertyName("st")]
        public RColor Stroke { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.TextStyle, this);
        }
    }
}
