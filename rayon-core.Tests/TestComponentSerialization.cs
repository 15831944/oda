using Xunit;
using System;
using System.Text.Json;
using System.Collections.Generic;
using System.Text.Json.Serialization;
using Rayon.Core;
using Rayon.Core.Components;
using Rayon.Core.Types;

namespace Rayon.Tests
{
    public class ComponentShouldBeCorrectlySerialized
    {
        [Fact]
        public void TestPathCompSerialization()
        {
            PathComp path = new PathComp();
            path.Points = new List<RPoint2d>() { new RPoint2d(0, 0), new RPoint2d(1, 1), new RPoint2d(2, 3) };
            path.Verbs = new List<PathComp.PathVerbEnum> { PathComp.PathVerbEnum.Begin, PathComp.PathVerbEnum.LineTo, PathComp.PathVerbEnum.LineTo };

            var got = JsonSerializer.Serialize(path);
            var shouldGet = "{\"p\":[[0,0],[1,1],[2,3]],\"v\":[0,1,1],\"c\":false}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestArcCompSerialization()
        {
            ArcComp arc = new ArcComp(new RPoint2d(0, 0), new RVector(10.0, 10.0));

            var got = JsonSerializer.Serialize(arc);
            var shouldGet = "{\"c\":[0,0],\"r\":[10,10],\"st_a\":{\"radians\":0},\"sw_a\":{\"radians\":6.283185307179586},\"x_rot\":{\"radians\":0}}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestBboxCompSerialization()
        {
            var bbox = new BboxComp(new RPoint2d(0, 0), new RPoint2d(10.0, 10.0), true);

            var got = JsonSerializer.Serialize(bbox);
            var shouldGet = "{\"a\":[0,0],\"b\":[10,10],\"r\":true}";
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
            var comp = new RenderBufferComp();
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
            var comp = new LayerComp(false, RColor.Black());
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
            var comp = new StyleIdComp("7b923fb7-c2ec-472a-aa26-04bed2b0275a-1");
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
            var shouldGet = "{\"p\":[10,23.5]}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestLengthSerialization()
        {
            var length = new RLength(10.0, RLength.LengthTypeEnum.Pixels);
            var got = JsonSerializer.Serialize(length);
            var shouldGet = "{\"Pixels\":10}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestColorSerialization()
        {
            var color = new RColor(0.5, 0.3, 0.2, 1.0);
            var got = JsonSerializer.Serialize(color);
            var shouldGet = "{\"r\":0.5,\"g\":0.3,\"b\":0.2,\"a\":1}";
            Assert.Equal(got, shouldGet);
        }

        [Fact]
        public void TestStyleCompSerialization()
        {
            var comp = new StrokeStyleComp(
                new RColor(0.1, 1.0, 1.0, 1.0),
                new RLength(10, RLength.LengthTypeEnum.Pixels),
                StrokeStyleComp.StrokePatternEnum.Solid
            );

            var got = JsonSerializer.Serialize(comp);
            var shouldGet = "{\"c\":{\"r\":0.1,\"g\":1,\"b\":1,\"a\":1},\"s\":{\"Pixels\":10},\"p\":0}";
            Assert.Equal(shouldGet, got);
        }

        [Fact]
        public void TestModelSettingsCompSerialization()
        {
            var comp = new ModelSettingsComp(ModelSettingsComp.UnitSystemEnum.Meters);
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
