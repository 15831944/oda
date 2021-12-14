// <copyright file="StyleIdComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json.Serialization;

    public class StyleIdComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="StyleIdComp"/> class.
        /// </summary>
        /// <param name="styleId"></param>
        public StyleIdComp(string styleId)
           : base()
        {
            this.StyleId = styleId;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="StyleIdComp"/> class.
        /// </summary>
        /// <param name="style"></param>
        public StyleIdComp(Element style)
           : base()
        {
            this.StyleId = style.Handle;
        }

        /// <summary>
        /// The handle of the Style Element
        /// </summary>
        [JsonPropertyName("h")]
        public string StyleId { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.StyleId, this);
        }
    }
}
