// <copyright file="TagComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text.Json.Serialization;
    using Rayon.Core.Types;

    /// <summary>
    /// A component describing a Tag Element.
    /// A tag element contains a Content (eg. "Surface {area}") and an Handle
    /// pointing to its link parent (eg. the handle of a Path Element).
    /// </summary>
    public class TagComp : ComponentValue, ILinked
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="TagComp"/> class.
        /// </summary>
        public TagComp(string content, Element parent)
            : base()
        {
            this.Content = content;
            this.Handle = parent.Handle;
        }

        [JsonPropertyName("h")]
        public string Handle { get; set; }

        [JsonPropertyName("c")]
        public string Content { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Tag, this);
        }

        public string GetLinkedHandle()
        {
            return this.Handle;
        }
    }
}
