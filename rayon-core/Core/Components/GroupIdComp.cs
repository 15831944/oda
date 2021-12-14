// <copyright file="GroupIdComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;
    using System.Text.Json.Serialization;

    /// <summary>
    /// A component used to indicate that an Element is a member of a group,
    /// by referencing the Handle of the corresponding group Element.
    /// The group Element is expected to have a <see cref="GroupComp"/> component.
    /// </summary>
    public class GroupIdComp : ComponentValue, ILinked
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="GroupIdComp"/> class.
        /// </summary>
        /// <param name="groupId"></param>
        public GroupIdComp(string groupId)
           : base()
        {
            this.GroupId = groupId;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="GroupIdComp"/> class.
        /// </summary>
        /// <param name="group"></param>
        public GroupIdComp(Element group)
           : base()
        {
            this.GroupId = group.Handle;
        }

        /// <summary>
        /// Gets or sets the handle of the Group Element reference via this component
        /// </summary>
        [JsonPropertyName("h")]
        public string GroupId { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.GroupId, this);
        }

        public string GetLinkedHandle()
        {
            return this.GroupId;
        }
    }
}
