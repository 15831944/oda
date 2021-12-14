// <copyright file="NameComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text.Json.Serialization;

    public class NameComp : ComponentValue
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="NameComp"/> class.
        /// </summary>
        /// <param name="name"></param>
        public NameComp(string name)
            : base()
        {
            this.Name = name;
        }

        [JsonPropertyName("n")]
        public string Name { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Name, this);
        }
    }
}
