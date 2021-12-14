// <copyright file="Element.cs" company="Rayon">
// Copyright (c) Rayon. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;
using Rayon.Core.Components;
using Rayon.Core.Types;

namespace Rayon.Core
{
    public class Element
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="Element"/> class.
        /// </summary>
        public Element()
        {
            this.Handle = Guid.NewGuid().ToString();
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Element"/> class.
        /// </summary>
        /// <param name="model"></param>
        public Element(Model model)
        {
            this.Handle = Guid.NewGuid().ToString();
            this.Model = model;
            this.ModelId = model.Id;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Element"/> class.
        /// </summary>
        /// <param name="model"></param>
        /// <param name="handle"></param>
        public Element(Model model, string handle)
        {
            this.Handle = handle;
            this.Model = model;
            this.ModelId = model.Id;
        }

        [JsonPropertyName("h")]
        public string Handle { get; set; }

        [JsonPropertyName("c")]
        public virtual List<Component> Components { get; } = new List<Component>();

        [JsonIgnore]
        public Guid ModelId { get; set; }

        [JsonIgnore]
        public DateTime? CreatedAt { get; set; }

        [JsonIgnore]
        public Guid CreatedById { get; set; }

        [JsonIgnore]
        public User CreatedBy { get; set; }

        [JsonIgnore]
        public virtual Model Model { get; set; }

        /// <summary>
        /// Creates a layer
        /// </summary>
        /// <param name="model">The model</param>
        /// <param name="name"></param>
        /// <param name="parent"></param>
        /// <param name="z"></param>
        /// <param name="handle"></param>
        /// <param name="style"></param>
        /// <param name="color"></param>
        /// <returns></returns>
        public static Element CreateLayer(Model model, string name, string parent, double z, string handle, Element style, RColor color)
        {
            // we need to check for the empty guid and change it to null
            string correctParent = parent == Guid.Empty.ToString() ? null : parent;
            var rLayer = new Element(model, handle)
                .With(new LayerComp(false, color))
                .With(new NameComp(name))
                .With(new LayerIdComp(correctParent, z))
                .With(new CategoryComp(CategoryComp.CategoryEnum.Layer))
                .With(new StyleIdComp(style));

            return rLayer;
        }

        /// <summary>
        /// Creates a Path Entity representing a line
        /// </summary>
        /// <param name="model"></param>
        /// <param name="handle"></param>
        /// <param name="layer"></param>
        /// <param name="style"></param>
        /// <param name="name"></param>
        /// <param name="at"></param>
        /// <param name="to"></param>
        /// <returns>An Entity.</returns>
        public static Element CreateLine(Model model, string handle, Element layer, Element style, string name, RPoint2d at, RPoint2d to)
        {
            // create a path entity
            var pathComp = new PathComp();
            pathComp.BeginAt(at);
            pathComp.LineTo(to);

            var path = Element.CreatePath(model, handle, layer, style, name ?? "Line", pathComp);

            return path;
        }

        public static Element CreatePath(Model model, string handle, Element layer, Element style, string name, PathComp pathComp)
        {
            if (pathComp.IsEmpty())
            {
                return null;
            }

            var bboxComp = pathComp.GetBoundingBox();
            var path = new Element(model, handle)
                .With(pathComp)
                .With(new CategoryComp(CategoryComp.CategoryEnum.Path))
                .With(new NameComp(name ?? "PolyLine"))
                .With(new RenderBufferComp())
                .With(new TransformComp())
                .With(bboxComp)
                .With(new LayerIdComp(layer))
                .With(new StyleIdComp(style));

            return path;
        }

        public static Element CreateArc(Model model, string handle, Element layer, Element style, string name, ArcComp arcComp)
        {
            var bboxComp = arcComp.GetBoundingBox();
            var path = new Element(model, handle)
                .With(arcComp)
                .With(new CategoryComp(CategoryComp.CategoryEnum.Arc))
                .With(new NameComp(name ?? "Arc"))
                .With(new RenderBufferComp())
                .With(new TransformComp())
                .With(bboxComp)
                .With(new LayerIdComp(layer))
                .With(new StyleIdComp(style));

            return path;
        }

        /// <summary>
        /// Creates an Entity representing a Stroke Style.
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name"></param>
        /// <param name="color"></param>
        /// <param name="thicknessInPixel"></param>
        /// <param name="pattern"></param>
        /// <returns>An Entity.</returns>
        public static Element CreateStrokeStyle(Model model, string name, RColor color, double thicknessInPixel, StrokeStyleComp.StrokePatternEnum pattern)
        {
            var strokeStyle = new StrokeStyleComp(color, RLength.Pixels(thicknessInPixel), pattern);

            var style = new Element(model)
                .With(strokeStyle)
                .With(new NameComp(name));

            return style;
        }

        public Element AddComponent(Component component)
        {
            component.ModifiedBy = this.CreatedBy;
            component.ModelId = this.ModelId;
            component.Element = this;
            component.Handle = this.Handle;
            this.Components.Add(component);
            this.Model.Components.Add(component);
            return this;
        }

        public Element With(ComponentValue componentValue)
        {
            var component = componentValue.ToComponent(this);
            component.ModelId = this.ModelId;
            component.Element = this;
            component.ModifiedBy = this.CreatedBy;

            component.ModifiedById = this.CreatedById;
            component.Handle = this.Handle;
            this.Components.Add(component);
            this.Model.Components.Add(component);
            return this;
        }
    }
}
