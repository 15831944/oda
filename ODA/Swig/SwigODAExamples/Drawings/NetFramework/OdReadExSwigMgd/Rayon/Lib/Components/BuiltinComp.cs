// <copyright file="HiddenComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;

    public class BuiltinComp : ComponentValue
    {
        /// <summary>
        /// Indicates that the Element is a builtin.
        /// Note : This component should never be persisted to the db as
        /// Builtin element, per definition, are created in the client.
        /// </summary>
        public BuiltinComp()
            : base()
        {
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Builtin, this);
        }
    }
}
