using Xunit;
using System;
using System.Text.Json;
using System.Collections.Generic;
using System.Text.Json.Serialization;
using RayonImport.Lib;
using RayonImport.Lib.Components;
using RayonImport.Lib.Geometry;
using RayonImport.Lib.Components.Types;

namespace RhinoImport.Tests
{
    public class ModelShouldBeCorrectlySerialized
    {
        [Fact]
        public void TestModelMockGetElements()
        {
            var user = new User();
            var model = new Model("Test model", user, null);

            // create a style entity
            var pathStyle = new StyleComp(
                new PathStyle(Color.Blue(), Length.Pixels(2), PathStyle.DashPatternEnum.Solid),
                StyleComp.StyleCreatorEnum.User
            );

            var style = new Element(model).With(pathStyle);
            model.AddElement(style);

            // Create a layer entity
            var layerStyle = Element.CreatePathStyle(model, "layer style", Color.Black(), 1.0, PathStyle.DashPatternEnum.Solid); ;
            var layer = Element.CreateLayer(model, "MyLayer", null, 0.45, Guid.NewGuid().ToString(), layerStyle, Color.Black());
            model.AddElement(layer);
            model.AddElement(layerStyle);

            // create a path entity
            var pathComp = new PathComp();
            pathComp.BeginAt(new RPoint2d(0, 0));
            pathComp.LineTo(new RPoint2d(500, 0));
            pathComp.LineTo(new RPoint2d(500, 500));
            pathComp.LineTo(new RPoint2d(0, 500));
            pathComp.LineTo(new RPoint2d(0, 0));
            pathComp.Close();

            var path = new Element(model)
                .With(pathComp)
                .With(new CategoryComp(CategoryComp.CategoryEnum.Path))
                .With(new NameComp("Path"))
                .With(new TransformComp())
                .With(new GeometryComp())
                .With(new BboxComp())
                .With(new LayerIdComp(layer))
                .With(new StrokeStyleIdComp(style));

            model.AddElement(path);

            //model.MockGetElements(@"C:\Users\basti\rayon\resources\fixtures\test.json", true);
        }
    }
}
