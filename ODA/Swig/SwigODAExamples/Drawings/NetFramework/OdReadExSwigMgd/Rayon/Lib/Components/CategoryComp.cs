// <copyright file="CategoryComp.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

namespace Rayon.Lib.Components
{
    using System;
    using System.Collections.Generic;
    using System.Text.Json.Serialization;

    /// <summary>
    ///
    ///
    /// </summary>
    public class CategoryComp : ComponentValue
    {
        /// <summary>
        ///
        /// Serialization (for the path category): "{\"category\": 2}"
        /// </summary>
        /// <param name="category"></param>
        public CategoryComp(CategoryEnum category)
            : base()
        {
            this.Category = category;
        }

        public enum CategoryEnum
        {
            Undefined = 0,
            Grid = 1,
            Path = 2,
            BboxCornerTopLeft = 3,
            BboxCornerBottomLeft = 4,
            BboxCornerTopRight = 5,
            BboxCornerBottomRight = 6,
            BboxCornerLeft = 7,
            BboxCornerBottom = 8,
            BboxCornerRight = 9,
            BboxCornerTop = 10,
            BboxSideLeft = 11,
            BboxSideRight = 12,
            BboxSideTop = 13,
            BboxSideBottom = 14,
            BboxSideHorizontal = 15,
            BboxSideVertical = 16,
            PathEndPoint = 17,
            PathControlPoint = 18,
            SelectionBbox = 19,
            Cursor = 20,

            // TextBoxTop = 21, deprecated
            // TextBoxLeft = 22, deprecated
            // TextBoxRight = 23, deprecated
            // TextBoxBottom = 24, deprecated
            Text = 25,

            Arc = 26,
            SnapSegment = 27,
            SnapText = 28,
            HelperSegment = 29,
            ControlLine = 30,
            Layer = 31,
            XBasis = 32,
            YBasis = 33,
            SelectionRectangle = 34,
            Image = 35,
            HelperPoint = 36,
            HelperArc = 37,
            Group = 38,
            Rectangle = 39,
            HelperPolygon = 40,
            RegularPolygon = 41,
            Tag = 42,
            HelperPath = 43,
            Style = 44,
            Deleted = 254,
            Empty = 255,
        }

        [JsonPropertyName("c")]
        public CategoryEnum Category { get; set; }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Category, this);
        }
    }
}
