using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace RayonImport.Lib.Components
{
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
        /// <param name="handle"></param>
        /// <param name="category"></param>
        public CategoryComp(CategoryEnum category) : base()
        {
            this.Category = category;
        }

        [JsonPropertyName("c")]
        public CategoryEnum Category { get; set; }

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
            TextBoxTop = 21,
            TextBoxLeft = 22,
            TextBoxRight = 23,
            TextBoxBottom = 24,
            Text = 25,
            Arc = 26,
            SnapSegment = 27,
            SnapText = 28,
            HelperSegment = 29,
            ControlLine = 30,
            Layer = 31,
            Empty = 255,
        }

        public override Component ToComponent(Element entity)
        {
            return new Component(entity, Component.ComponentTypeEnum.Category, this);
        }
    }
}
