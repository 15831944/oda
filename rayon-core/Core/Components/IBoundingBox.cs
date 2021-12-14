﻿// <copyright file="IBoundingBox.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Core.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text;

    internal interface IBoundingBox
    {
        public BboxComp GetBoundingBox();
    }
}
