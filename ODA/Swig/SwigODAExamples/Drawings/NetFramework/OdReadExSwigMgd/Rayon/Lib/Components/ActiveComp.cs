// <copyright file="ActiveComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;

    /// <summary>
    /// A component used to indicate if an Element is active.
    /// eg. The active layer will have an active component.
    /// This Component should not be persisted to the db
    /// </summary>
    internal class ActiveComp : ComponentValue
    {
        public override Component ToComponent(Element entity)
        {
            throw new NotImplementedException("Should not be persisted to the db");
        }
    }
}
