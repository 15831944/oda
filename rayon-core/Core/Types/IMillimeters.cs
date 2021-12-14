// <copyright file="IMillimeters.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Text;

namespace Rayon.Core.Types
{
    using Rayon.Core.Components;

    /// <summary>
    /// Interface indicating that a Class can be converted to millimeters according to the provided unit
    /// </summary>
    public interface IMillimeters
    {
        public void ToMillimeters(ModelSettingsComp.UnitSystemEnum unit);
    }
}
