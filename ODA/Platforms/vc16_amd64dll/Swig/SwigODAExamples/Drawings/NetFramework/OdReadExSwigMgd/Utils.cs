using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Teigha.TD;
using Teigha.Core;
using Rayon;
using Rayon.Core.Components;

namespace OdReadExSwigMgd
{
    public class Utils {

        public static OdGePoint3d getArcMiddlePt(OdDbPolyline pPoly, uint i)
        {

            OdGePoint3d pt = new OdGePoint3d();
            pPoly.getPointAt(i, pt);
            Console.WriteLine(pt);
            OdGePoint3d nextPt = new OdGePoint3d();
            pPoly.getPointAt(i + 1, nextPt);
            Console.WriteLine(nextPt);

            double paramPt;
            pPoly.getParamAtPoint(pt, out paramPt);
            double paramNextPt;
            pPoly.getParamAtPoint(nextPt, out paramNextPt);
            double paramArcMiddlePoint = (paramPt + paramNextPt) / 2;

            OdGePoint3d arcMiddlePoint = new OdGePoint3d();
            pPoly.getPointAtParam(paramArcMiddlePoint, arcMiddlePoint);

            return arcMiddlePoint;
        }

        public static void print2dVertex(OdDb2dVertex pVertex, int i)
        {
            Console.WriteLine("  Point #" + i);
            Console.WriteLine("    ID = {0}", pVertex.getDbHandle());
            Console.WriteLine("    Vertex Type = {0}", pVertex.vertexType());
            Console.WriteLine("    Position = {0}", pVertex.position());
            Console.WriteLine("  ");
        }

        public static void printCurveData(OdDbEntity pEnt)
        {
            OdDbCurve pEntity = (OdDbCurve)pEnt;

            Console.WriteLine("  Layer = {0}", pEntity.layer());
            Console.WriteLine("  Layer ID = {0}", pEntity.layer());
            Console.WriteLine("  Lineweight = {0}", pEntity.lineWeight());
            Console.WriteLine("  Linetype = {0}", pEntity.linetype());
            Console.WriteLine("  Color = {0}", pEntity.color().ToString());

            OdGePoint3d startPoint = new OdGePoint3d();
            if (OdResult.eOk == pEntity.getStartPoint(startPoint))
            {
                Console.WriteLine("  Start Point = {0}", startPoint.ToString());
            }

            OdGePoint3d endPoint = new OdGePoint3d();
            if (OdResult.eOk == pEntity.getEndPoint(endPoint))
            {
                Console.WriteLine("  End Point  = {0}", endPoint.ToString());
            }
            Console.WriteLine("  Closed = {0}", pEntity.isClosed());
        }

        public static Rayon.Core.Types.RPoint2d OdGePoint2dToRPoint2d(OdGePoint2d point)
        {
            return new Rayon.Core.Types.RPoint2d(point.x, point.y);
        }
        static Rayon.Core.Types.RPoint2d scalePoint(Rayon.Core.Types.RPoint2d point, double factor)
        {
            return new Rayon.Core.Types.RPoint2d(point.X * factor, point.Y * factor);
        }
        static Rayon.Core.Types.RPoint2d translatePoint(Rayon.Core.Types.RPoint2d point, double xDisplacement, double yDisplacement)
        {
            return new Rayon.Core.Types.RPoint2d(point.X - xDisplacement, point.Y - yDisplacement);
        }
        static Rayon.Core.Types.RPoint2d rotatePoint(Rayon.Core.Types.RPoint2d point, double angle)
        {
            return new Rayon.Core.Types.RPoint2d(point.X * Math.Cos(angle) - point.Y * Math.Sin(angle), point.X * Math.Sin(angle) + point.Y * Math.Cos(angle));
        }

        public static Rayon.Core.Types.RPoint2d toUnitCircle(Rayon.Core.Types.RPoint2d point, double scaleFactor, Rayon.Core.Types.RPoint2d centerPoint, double rotationAngle)
        {
            // translate
            Rayon.Core.Types.RPoint2d translated = translatePoint(point, centerPoint.X, centerPoint.Y);
            // scale
            Rayon.Core.Types.RPoint2d scaled = scalePoint(translated, scaleFactor);
            // rotate
            Rayon.Core.Types.RPoint2d rotated = rotatePoint(scaled, rotationAngle);
            return rotated;
        }

        public static Rayon.Core.Types.RPoint2d fromUnitCircle(Rayon.Core.Types.RPoint2d point, double scaleFactor, Rayon.Core.Types.RPoint2d centerPoint, double rotationAngle)
        {
            // rotate
            Rayon.Core.Types.RPoint2d rotated = rotatePoint(point, rotationAngle);
            // scale
            Rayon.Core.Types.RPoint2d scaled = scalePoint(rotated, scaleFactor);
            // translate
            Rayon.Core.Types.RPoint2d translated = translatePoint(scaled, -centerPoint.X, -centerPoint.Y);
            return translated;
        }

        public static List<Rayon.Core.Types.RPoint2d> ArcToCubicBezier(OdGeCircArc2d arc)
        {

            double endParam = arc.endAng();
            double startParam = arc.startAng();
            double lengthParam = (endParam - startParam);
            bool isClockWise = arc.isClockWise();
            int numberOfSegments = (int)Math.Ceiling(lengthParam / (Math.PI / 2));
            double radius = arc.radius();

            List<Rayon.Core.Types.RPoint2d> outPoints = new List<Rayon.Core.Types.RPoint2d>();

            for (var i = 0; i < numberOfSegments; i++)
            {

                OdGePoint2d startPointOdGe = arc.evalPoint((lengthParam / numberOfSegments) * i);
                OdGePoint2d endPointOdGe = arc.evalPoint((lengthParam / numberOfSegments) * (i + 1));
                Rayon.Core.Types.RPoint2d startPt = OdGePoint2dToRPoint2d(startPointOdGe);
                Rayon.Core.Types.RPoint2d endPt = OdGePoint2dToRPoint2d(endPointOdGe);
                Rayon.Core.Types.RPoint2d centerPt = new Rayon.Core.Types.RPoint2d(arc.center().x, arc.center().y);
                double scaleFactor = 1 / arc.radius();

                double angleWithXAxis = isClockWise ?
                    arc.startAngFromXAxis() - lengthParam / numberOfSegments * i :
                    arc.startAngFromXAxis() + lengthParam / numberOfSegments * i;

                double angle = isClockWise ?
                    -lengthParam / numberOfSegments :
                    lengthParam / numberOfSegments;

                Rayon.Core.Types.RPoint2d startPtTransformed = toUnitCircle(startPt, scaleFactor, centerPt, -angleWithXAxis);
                Rayon.Core.Types.RPoint2d endPtTransformed = toUnitCircle(endPt, scaleFactor, centerPt, -angleWithXAxis);

                double k = 4 / 3 * Math.Tan(angle / 3);

                // P2
                Rayon.Core.Types.RPoint2d ctrlPt1 = new Rayon.Core.Types.RPoint2d(
                    1,
                    k
                );

                // P3
                Rayon.Core.Types.RPoint2d ctrlPt2 = new Rayon.Core.Types.RPoint2d(
                    Math.Cos(angle) + k * Math.Sin(angle),
                    Math.Sin(angle) - k * Math.Cos(angle)
                );

                outPoints.Add(fromUnitCircle(startPtTransformed, 1 / scaleFactor, centerPt, angleWithXAxis));
                outPoints.Add(fromUnitCircle(ctrlPt1, 1 / scaleFactor, centerPt, angleWithXAxis));
                outPoints.Add(fromUnitCircle(ctrlPt2, 1 / scaleFactor, centerPt, angleWithXAxis));

            }

            return outPoints;
        }

        public static double[,] getDwgColors()
        {
            double[,] Colors = new double[257, 4] {
                {0, 0, 0,0},
                {255, 0, 0,0},
                {255, 255, 0,0},
                {0, 255, 0,0},
                {0, 255, 255,0},
                {0, 0, 255,0},
                {255, 0, 255,0},
                {0, 0, 0, 0},
                {65, 65, 65,0},
                {128, 128, 128,0},
                {255, 0, 0,0},
                {255, 170, 170,0},
                {189, 0, 0,0},
                {189, 126, 126,0},
                {129, 0, 0,0},
                {129, 86, 86,0},
                {104, 0, 0,0},
                {104, 69, 69,0},
                {79, 0, 0,0},
                {79, 53, 53,0},
                {255, 63, 0,0},
                {255, 191, 170,0},
                {189, 46, 0,0},
                {189, 141, 126,0},
                {129, 31, 0,0},
                {129, 96, 86,0},
                {104, 25, 0,0},
                {104, 78, 69,0},
                {79, 19, 0,0},
                {79, 59, 53,0},
                {255, 127, 0,0},
                {255, 212, 170,0},
                {189, 94, 0,0},
                {189, 157, 126,0},
                {129, 64, 0,0},
                {129, 107, 86,0},
                {104, 52, 0,0},
                {104, 86, 69,0},
                {79, 39, 0,0},
                {79, 66, 53,0},
                {255, 191, 0,0},
                {255, 234, 170,0},
                {189, 141, 0,0},
                {189, 173, 126,0},
                {129, 96, 0,0},
                {129, 118, 86,0},
                {104, 78, 0,0},
                {104, 95, 69,0},
                {79, 59, 0,0},
                {79, 73, 53,0},
                {255, 255, 0,0},
                {255, 255, 170,0},
                {189, 189, 0,0},
                {189, 189, 126,0},
                {129, 129, 0,0},
                {129, 129, 86,0},
                {104, 104, 0,0},
                {104, 104, 69,0},
                {79, 79, 0,0},
                {79, 79, 53,0},
                {191, 255, 0,0},
                {234, 255, 170,0},
                {141, 189, 0,0},
                {173, 189, 126,0},
                {96, 129, 0,0},
                {118, 129, 86,0},
                {78, 104, 0,0},
                {95, 104, 69,0},
                {59, 79, 0,0},
                {73, 79, 53,0},
                {127, 255, 0,0},
                {212, 255, 170,0},
                {94, 189, 0,0},
                {157, 189, 126,0},
                {64, 129, 0,0},
                {107, 129, 86,0},
                {52, 104, 0,0},
                {86, 104, 69,0},
                {39, 79, 0,0},
                {66, 79, 53,0},
                {63, 255, 0,0},
                {191, 255, 170,0},
                {46, 189, 0,0},
                {141, 189, 126,0},
                {31, 129, 0,0},
                {96, 129, 86,0},
                {25, 104, 0,0},
                {78, 104, 69,0},
                {19, 79, 0,0},
                {59, 79, 53,0},
                {0, 255, 0,0},
                {170, 255, 170,0},
                {0, 189, 0,0},
                {126, 189, 126,0},
                {0, 129, 0,0},
                {86, 129, 86,0},
                {0, 104, 0,0},
                {69, 104, 69,0},
                {0, 79, 0,0},
                {53, 79, 53,0},
                {0, 255, 63,0},
                {170, 255, 191,0},
                {0, 189, 46,0},
                {126, 189, 141,0},
                {0, 129, 31,0},
                {86, 129, 96,0},
                {0, 104, 25,0},
                {69, 104, 78,0},
                {0, 79, 19,0},
                {53, 79, 59,0},
                {0, 255, 127,0},
                {170, 255, 212,0},
                {0, 189, 94,0},
                {126, 189, 157,0},
                {0, 129, 64,0},
                {86, 129, 107,0},
                {0, 104, 52,0},
                {69, 104, 86,0},
                {0, 79, 39,0},
                {53, 79, 66,0},
                {0, 255, 191,0},
                {170, 255, 234,0},
                {0, 189, 141,0},
                {126, 189, 173,0},
                {0, 129, 96,0},
                {86, 129, 118,0},
                {0, 104, 78,0},
                {69, 104, 95,0},
                {0, 79, 59,0},
                {53, 79, 73,0},
                {0, 255, 255,0},
                {170, 255, 255,0},
                {0, 189, 189,0},
                {126, 189, 189,0},
                {0, 129, 129,0},
                {86, 129, 129,0},
                {0, 104, 104,0},
                {69, 104, 104,0},
                {0, 79, 79,0},
                {53, 79, 79,0},
                {0, 191, 255,0},
                {170, 234, 255,0},
                {0, 141, 189,0},
                {126, 173, 189,0},
                {0, 96, 129,0},
                {86, 118, 129,0},
                {0, 78, 104,0},
                {69, 95, 104,0},
                {0, 59, 79,0},
                {53, 73, 79,0},
                {0, 127, 255,0},
                {170, 212, 255,0},
                {0, 94, 189,0},
                {126, 157, 189,0},
                {0, 64, 129,0},
                {86, 107, 129,0},
                {0, 52, 104,0},
                {69, 86, 104,0},
                {0, 39, 79,0},
                {53, 66, 79,0},
                {0, 63, 255,0},
                {170, 191, 255,0},
                {0, 46, 189,0},
                {126, 141, 189,0},
                {0, 31, 129,0},
                {86, 96, 129,0},
                {0, 25, 104,0},
                {69, 78, 104,0},
                {0, 19, 79,0},
                {53, 59, 79,0},
                {0, 0, 255,0},
                {170, 170, 255,0},
                {0, 0, 189,0},
                {126, 126, 189,0},
                {0, 0, 129,0},
                {86, 86, 129,0},
                {0, 0, 104,0},
                {69, 69, 104,0},
                {0, 0, 79,0},
                {53, 53, 79,0},
                {63, 0, 255,0},
                {191, 170, 255,0},
                {46, 0, 189,0},
                {141, 126, 189,0},
                {31, 0, 129,0},
                {96, 86, 129,0},
                {25, 0, 104,0},
                {78, 69, 104,0},
                {19, 0, 79,0},
                {59, 53, 79,0},
                {127, 0, 255,0},
                {212, 170, 255,0},
                {94, 0, 189,0},
                {157, 126, 189,0},
                {64, 0, 129,0},
                {107, 86, 129,0},
                {52, 0, 104,0},
                {86, 69, 104,0},
                {39, 0, 79,0},
                {66, 53, 79,0},
                {191, 0, 255,0},
                {234, 170, 255,0},
                {141, 0, 189,0},
                {173, 126, 189,0},
                {96, 0, 129,0},
                {118, 86, 129,0},
                {78, 0, 104,0},
                {95, 69, 104,0},
                {59, 0, 79,0},
                {73, 53, 79,0},
                {255, 0, 255,0},
                {255, 170, 255,0},
                {189, 0, 189,0},
                {189, 126, 189,0},
                {129, 0, 129,0},
                {129, 86, 129,0},
                {104, 0, 104,0},
                {104, 69, 104,0},
                {79, 0, 79,0},
                {79, 53, 79,0},
                {255, 0, 191,0},
                {255, 170, 234,0},
                {189, 0, 141,0},
                {189, 126, 173,0},
                {129, 0, 96,0},
                {129, 86, 118,0},
                {104, 0, 78,0},
                {104, 69, 95,0},
                {79, 0, 59,0},
                {79, 53, 73,0},
                {255, 0, 127,0},
                {255, 170, 212,0},
                {189, 0, 94,0},
                {189, 126, 157,0},
                {129, 0, 64,0},
                {129, 86, 107,0},
                {104, 0, 52,0},
                {104, 69, 86,0},
                {79, 0, 39,0},
                {79, 53, 66,0},
                {255, 0, 63,0},
                {255, 170, 191,0},
                {189, 0, 46,0},
                {189, 126, 141,0},
                {129, 0, 31,0},
                {129, 86, 96,0},
                {104, 0, 25,0},
                {104, 69, 78,0},
                {79, 0, 19,0},
                {79, 53, 59,0},
                {51, 51, 51,0},
                {80, 80, 80,0},
                {105, 105, 105,0},
                {130, 130, 130,0},
                {190, 190, 190,0},
                {255, 255, 255,0},
                {0, 0, 0,0}
                };

            return Colors;
        }

        public static Rayon.Core.Components.Styles.TextStyleComp.FontEnum getFont(string fontName)
        {
            switch (fontName)
            {
                case string font when font.ToUpper().Contains("ARIAL"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.LatoRegular;
                case string font when font.ToUpper().Contains("ABEL"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.AbelRegular;
                case string font when font.ToUpper().Contains("ANONYMOUS"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.AnonymousProRegular;
                case string font when font.ToUpper().Contains("ARCHITECTS DAUGHTER"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.ArchitectsDaughterRegular;
                case string font when font.ToUpper().Contains("COMFORTAA"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.ComfortaaRegular;
                case string font when font.ToUpper().Contains("COURIER"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.CourierPrimeRegular;
                case string font when font.ToUpper().Contains("DOSIS"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.DosisRegular;
                case string font when font.ToUpper().Contains("LATO"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.LatoRegular;
                case string font when font.ToUpper().Contains("LORA"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.LoraRegular;
                case string font when font.ToUpper().Contains("MONTSERRAT"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.MontserratBold;
                case string font when font.ToUpper().Contains("OPEN"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.OpenSansRegular;
                case string font when font.ToUpper().Contains("OSWALD"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.OswaldRegular;
                case string font when font.ToUpper().Contains("PLAYFAIR"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.PlayfairDisplayRegular;
                case string font when font.ToUpper().Contains("POPPINS"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.PoppinsMedium;
                case string font when font.ToUpper().Contains("QUICKSAND"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.QuicksandRegular;
                case string font when font.ToUpper().Contains("RALEWAY"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.RalewayMedium;
                case string font when font.ToUpper().Contains("ROBOTO"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.RobotoMonoRegular;
                case string font when font.ToUpper().Contains("ROBOTO"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.RobotoMonoRegular;
                case string font when font.ToUpper().Contains("TINOS"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.TinosRegular;
                case string font when font.ToUpper().Contains("TRAIN ONE"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.TrainOneRegular;
                case string font when font.ToUpper().Contains("UBUNTU"):
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.UbuntuMedium;

                default:
                    return Rayon.Core.Components.Styles.TextStyleComp.FontEnum.PoppinsMedium;

            }
        }

        // TO DO: implement mapping of line styles form DWG styles to Raoyn styles
        static StrokeStyleComp.StrokePatternEnum strokeStyleConversion(string lineType)
        {
            if (lineType == "Continuous")
            {
                return StrokeStyleComp.StrokePatternEnum.Solid;
            }
            else
            {
                return StrokeStyleComp.StrokePatternEnum.Solid;
            }
        }

        public static StrokeStyleComp.StrokePatternEnum getStrokeStyleRecord(OdDbLayerTableRecord pRecord)
        {
            string lineType = OdExStringHelpers.toString(pRecord.linetypeObjectId());
            return strokeStyleConversion(lineType);
        }

        public static Rayon.Core.Types.RColor getColor(OdDbLayerTableRecord pRecord)
        {
            double[,] Colors = getDwgColors();
            int colorIndex = pRecord.colorIndex();
            Rayon.Core.Types.RColor color = new Rayon.Core.Types.RColor(Colors[colorIndex, 0] / 255, Colors[colorIndex, 1] / 255, Colors[colorIndex, 2] / 255, 1);
            return color;
        }

        public static Rayon.Core.Types.RVector getArcRadii(double radius)
        {
            Rayon.Core.Types.RVector radii = new Rayon.Core.Types.RVector(radius, radius);
            return radii;
        }
        public static Rayon.Core.Types.RVector getEllipseRadii(double radiusSmall, double radiusLarge)
        {
            Rayon.Core.Types.RVector radius = new Rayon.Core.Types.RVector(radiusSmall, radiusLarge);
            return radius;
        }

        public static Rayon.Core.Types.RPoint2d OdGePoint3dToRPoint2d(OdGePoint3d pt)
        {
            Rayon.Core.Types.RPoint2d newRpoint = new Rayon.Core.Types.RPoint2d(pt.x, pt.y);
            return newRpoint;
        }

        public static Rayon.Core.Types.RPoint2d OdDb2dVertexToRPoint2d(OdDb2dVertex vert)
        {
            Rayon.Core.Types.RPoint2d newRpoint = new Rayon.Core.Types.RPoint2d(vert.position().x, vert.position().y);
            return newRpoint;
        }

        public static Rayon.Core.Element getEntityLayer(OdDbEntity pEnt, Rayon.Core.Model newModel)
        {
            Rayon.Core.Element entityLayer = newModel.Elements.Find(
                x =>
                    x.Handle == pEnt.layerId().getHandle().ToString()
                );
            return entityLayer;
        }

        public static Rayon.Core.Element getEntityStyle(OdDbEntity pEnt, Rayon.Core.Model newModel)
        {
            Rayon.Core.Element layerOfEnt = new Rayon.Core.Element();
            Rayon.Core.Components.Component layerStyle = new Rayon.Core.Components.Component();
            Rayon.Core.Element entStyle = new Rayon.Core.Element();
            string styleId = "";

            // inherit style from layer
            if (pEnt.linetype() == "ByLayer" && pEnt.color().ToString() == "ByLayer")
            {
                Console.WriteLine("  STYLE: Inherited from Layer");

                string handle = pEnt.layerId().getHandle().ToString();

                // get layer of entities
                layerOfEnt = newModel.Elements.Find(
                    x =>
                        x.Handle == handle
                    );

                // get its style id
                layerStyle = layerOfEnt.Components.Find(
                    x =>
                        x.ComponentType == Component.ComponentTypeEnum.StyleId
                    );

                styleId = layerStyle.LinkedElement;

                // get style entity in Model
                entStyle = newModel.Elements.Find(
                    x =>
                        x.Handle == styleId
                    );
            }

            // else if either one or the other is herited from parent block > create a new matching style
            else
            {
                Console.WriteLine("  STYLE: Created New Style");

                double newLineWidth = (double)pEnt.lineWeight() < 0 ? 1 : (double)pEnt.lineWeight() / 100;
                string styleName = "Default_Style";
                StrokeStyleComp.StrokePatternEnum newLineStyle = getStrokeStyleEntity(pEnt);
                //pEnt.color
                Rayon.Core.Types.RColor defaultBlack = new Rayon.Core.Types.RColor(0, 0, 0, 1);

                entStyle = Rayon.Core.Element.CreateStrokeStyle(
                    newModel,
                    styleName,
                    defaultBlack,
                    newLineWidth,
                    newLineStyle
                    );

                newModel.Elements.Add(entStyle);
            }

            // else if both properties are custom, create a custom style

            return entStyle;
        }

        static StrokeStyleComp.StrokePatternEnum getStrokeStyleEntity(OdDbEntity pEnt)
        {
            string lineType = pEnt.linetype();
            return strokeStyleConversion(lineType);
        }


        public static Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum getUnitSystem(string unitValue)
        {
            switch (unitValue)
            {
                case "kUnitsUndefined":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Millimeters;
                case "kUnitsInches":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Inches;
                case "kUnitsFeet":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Feet;
                case "kUnitsMiles":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Miles;
                case "kUnitsMillimeters":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Millimeters;
                case "kUnitsCentimeters":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Centimeters;
                case "kUnitsMeters":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Meters;
                case "kUnitsKilometers":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Kilometers;
                case "kUnitsMicroinches":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Microinches;
                case "kUnitsMils":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Mils;
                case "kUnitsYards":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Yards;
                case "kUnitsAngstroms":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Angstroms;
                case "kUnitsNanometers":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Nanometers;
                case "kUnitsMicrons":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Microns;
                case "kUnitsDecimeters":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Decimeters;
                case "kUnitsDekameters":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Dekameters;
                case "kUnitsHectometers":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Hectometers;
                case "kUnitsGigameters":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Gigameters;
                case "kUnitsAstronomical":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.AstronomicalUnits;
                case "kUnitsLightYears":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.LightYears;
                case "kUnitsParsecs":
                    return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Parsecs;
            }
            return Rayon.Core.Components.ModelSettingsComp.UnitSystemEnum.Millimeters;
        }


    }

}
