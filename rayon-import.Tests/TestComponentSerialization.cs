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
    public class ComponentShouldBeCorrectlySerialized
    {
        [Fact]
        public void TestPathCompSerialization()
        {
            PathComp path = new PathComp();
            path.Points = new List<RPoint2d>() { new RPoint2d(0, 0), new RPoint2d(1, 1), new RPoint2d(2, 3) };
            path.Verbs = new List<PathComp.PathVerb> { PathComp.PathVerb.Begin, PathComp.PathVerb.LineTo, PathComp.PathVerb.LineTo };

            var got = JsonSerializer.Serialize(path);
            var shouldGet = "{\"p\":[[0,0],[1000,1000],[2000,3000]],\"v\":[0,1,1],\"c\":false}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestArcCompSerialization()
        {
            ArcComp arc = new ArcComp(new RPoint2d(0, 0), new RVector(10.0, 10.0));

            var got = JsonSerializer.Serialize(arc);
            var shouldGet = "{\"c\":[0,0],\"r\":[10000,10000],\"st_a\":{\"radians\":0},\"sw_a\":{\"radians\":6.283185307179586},\"x_rot\":{\"radians\":0}}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestBboxCompSerialization()
        {
            var bbox = new BboxComp(new RPoint2d(0, 0), new RPoint2d(10.0, 10.0), true);

            var got = JsonSerializer.Serialize(bbox);
            var shouldGet = "{\"a\":[0,0],\"b\":[10000,10000],\"r\":true}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestCategoryCompSerialization()
        {
            var cat = new CategoryComp(CategoryComp.CategoryEnum.Path);
            var got = JsonSerializer.Serialize(cat);
            var shouldGet = "{\"c\":2}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestGeometryCompSerialization()
        {
            var comp = new GeometryComp();
            var got = JsonSerializer.Serialize(comp);
            var shouldGet = "{}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestHiddenCompSerialization()
        {
            var comp = new HiddenComp();
            var got = JsonSerializer.Serialize(comp);
            var shouldGet = "{}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestLayerCompSerialization()
        {
            var comp = new LayerComp(false, Color.Black());
            var got = JsonSerializer.Serialize(comp);
            var shouldGet = "{\"i\":false,\"c\":{\"r\":0,\"g\":0,\"b\":0,\"a\":1}}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestLayerIdCompSerialization()
        {
            var comp = new LayerIdComp("7b923fb7-c2ec-472a-aa26-04bed2b0275a-1", 0.5);
            var got = JsonSerializer.Serialize(comp);
            var shouldGet = "{\"z\":0.5,\"h\":\"7b923fb7-c2ec-472a-aa26-04bed2b0275a-1\"}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestStyleIdCompSerialization()
        {
            var comp = new StrokeStyleIdComp("7b923fb7-c2ec-472a-aa26-04bed2b0275a-1");
            var got = JsonSerializer.Serialize(comp);
            var shouldGet = "{\"h\":\"7b923fb7-c2ec-472a-aa26-04bed2b0275a-1\"}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestNameCompSerialization()
        {
            var comp = new NameComp("Path");
            var got = JsonSerializer.Serialize(comp);
            var shouldGet = "{\"n\":\"Path\"}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestPointCompSerialization()
        {
            var comp = new PointComp(new RPoint2d(10.0, 23.5));
            var got = JsonSerializer.Serialize(comp);
            var shouldGet = "{\"p\":[10000,23500]}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestLengthSerialization()
        {
            var length = new Length(10.0, Length.LengthTypeEnum.Pixels);
            var got = JsonSerializer.Serialize(length);
            var shouldGet = "{\"Pixels\":10}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestColorSerialization()
        {
            var color = new Color(0.5, 0.3, 0.2, 1.0);
            var got = JsonSerializer.Serialize(color);
            var shouldGet = "{\"r\":0.5,\"g\":0.3,\"b\":0.2,\"a\":1}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestStyleCompSerialization()
        {
            var pathStyle = new PathStyle(
                new Color(0.1, 1.0, 1.0, 1.0),
                new Length(10, Length.LengthTypeEnum.Pixels),
                PathStyle.DashPatternEnum.Solid
            );

            var comp = new StyleComp(pathStyle, StyleComp.StyleCreatorEnum.Default);
            var got = JsonSerializer.Serialize(comp);
            var shouldGet = "{\"c\":1,\"s\":{\"color\":{\"r\":0.1,\"g\":1,\"b\":1,\"a\":1},\"thickness\":{\"Pixels\":10},\"pattern\":0,\"t\":\"Stroke\"}}";
            Assert.Equal(shouldGet, got);
        }

        [Fact]
        public void TestModelSettingsCompSerialization()
        {
            var comp = new ModelSettingsComp(UnitSystem.Meters);
            var got = JsonSerializer.Serialize(comp);
            var shouldGet = "{\"u\":4}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestTransformCompSerialization()
        {
            var comp = new TransformComp();
            var got = JsonSerializer.Serialize(comp);
            var shouldGet = "[1,0,0,1,0,0]";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestTransformCompDeSerialization()
        {
            var jsonString = "[1,2,3,4,5,6]";
            TransformComp comp = JsonSerializer.Deserialize<TransformComp>(jsonString);
            Assert.Equal(comp.Matrix, new double[] { 1.0, 2, 3, 4, 5, 6 });
        }
    }
}
