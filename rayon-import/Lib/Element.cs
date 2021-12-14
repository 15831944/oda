using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;
using RayonImport.Lib.Components;
using RayonImport.Lib.Components.Types;
using RayonImport.Lib.Geometry;

namespace RayonImport.Lib
{
    public class Element
    {
        public Element()
        {
            this.Handle = Guid.NewGuid().ToString();
        }

        public Element(Model model)
        {
            this.Handle = Guid.NewGuid().ToString();
            this.Model = model;
            this.ModelId = model.Id;
        }

        public Element(Model model, string handle)
        {
            this.Handle = handle;
            this.Model = model;
            this.ModelId = model.Id;
        }

        public static Element CreateLayer(Model model, string name, string parent, double z, string handle, Element style, Color color)
        {
            // we need to check for the empty guid and change it to null
            string correctParent = parent == Guid.Empty.ToString() ? null : parent;
            var rLayer = new Element(model, handle)
                .With(new LayerComp(false, color))
                .With(new NameComp(name))
                .With(new LayerIdComp(correctParent, z))
                .With(new CategoryComp(CategoryComp.CategoryEnum.Layer))
                .With(new StrokeStyleIdComp(style));

            return rLayer;
        }

        /// <summary>
        /// Creates a Path Entity representing a line
        /// </summary>
        /// <param name="Handle"></param>
        /// <param name="layer"></param>
        /// <param name="style"></param>
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
                .With(new GeometryComp())
                .With(new TransformComp())
                .With(bboxComp)
                .With(new LayerIdComp(layer))
                .With(new StrokeStyleIdComp(style));

            return path;
        }

        public static Element CreateArc(Model model, string handle, Element layer, Element style, string name, ArcComp arcComp)
        {
            var bboxComp = arcComp.GetBoundingBox();
            var path = new Element(model, handle)
                .With(arcComp)
                .With(new CategoryComp(CategoryComp.CategoryEnum.Arc))
                .With(new NameComp(name ?? "Arc"))
                .With(new GeometryComp())
                .With(new TransformComp())
                .With(bboxComp)
                .With(new LayerIdComp(layer))
                .With(new StrokeStyleIdComp(style));

            return path;
        }

        /// <summary>
        /// Creates an Entity representing a Path Style
        /// </summary>
        /// <param name="name"></param>
        /// <param name="color"></param>
        /// <param name="thicknessInPixel"></param>
        /// <param name="pattern"></param>
        /// <returns>An Entity.</returns>
        public static Element CreatePathStyle(Model model, string name, Color color, double thicknessInPixel, PathStyle.DashPatternEnum pattern)
        {
            var pathStyle = new StyleComp(
                new PathStyle(color, Length.Pixels(thicknessInPixel), pattern),
                StyleComp.StyleCreatorEnum.User
            );

            var style = new Element(model)
                .With(pathStyle)
                .With(new NameComp(name));

            return style;
        }

        [JsonPropertyName("handle")]
        public string Handle { get; set; }

        [JsonPropertyName("components")]
        public virtual ICollection<Component> Components { get; } = new List<Component>();

        public Guid ModelId { get; set; }

        public DateTime? CreatedAt { get; set; }

        public Guid CreatedById { get; set; }

        public User CreatedBy { get; set; }

        public virtual Model Model { get; set; }

        public Element AddComponent(Component component)
        {
            //component.ModifiedById = this.CreatedById;
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
