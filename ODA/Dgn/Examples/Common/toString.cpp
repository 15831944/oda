/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2021, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Open Design Alliance software pursuant to a license 
//   agreement with Open Design Alliance.
//   Open Design Alliance Copyright (C) 2002-2021 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

#define STL_USING_IOSTREAM
#include "OdaSTL.h"

#define  STD(a)  std:: a

#include "toString.h"

#include "DgElement.h"
#include "DgTableRecord.h"

/************************************************************************/
/* Shorten a path with ellipses.                                        */
/************************************************************************/
OdString shortenPath(OdString path, int maxPath)
{
  /**********************************************************************/
  /* If the path fits, just return it                                   */
  /**********************************************************************/
  if (path.getLength()<= maxPath)
  {
    return path;
  }
  /**********************************************************************/
  /* If there's no backslash, just truncate the path                    */
  /**********************************************************************/
  int lastBackslash = path.reverseFind('\\');
  if (lastBackslash < 0)
  {
      return path.left(maxPath - 3) + OD_T("...");
  }    

  /**********************************************************************/
  /* Shorten the front of the path                                      */
  /**********************************************************************/
  int fromLeft = (lastBackslash - 3) - (path.getLength() - maxPath);
  // (12 - 3) - (19 - 10) = 9 - 9 = 0 
  if ((lastBackslash <= 3)|| (fromLeft < 1))  
  {
    path = OD_T("...") + path.mid(lastBackslash);
  }
  else
  {
    path = path.left(fromLeft) + OD_T("...") + path.mid(lastBackslash);
  }
  
  /**********************************************************************/
  /* Truncate the path                                                  */
  /**********************************************************************/
  if (path.getLength() > maxPath)
  {
    path = path.left(maxPath-3) + OD_T("...");
  }
    
  return path;
}


/************************************************************************/
/* Convert the specified value to a degree string                       */
/************************************************************************/
OdString toDegreeString(const double val)
{ 
  return toString(val * 180.0 / OdaPI) + OD_T("d");
}

/************************************************************************/
/* Convert the specified value to a hex string                          */
/************************************************************************/
OdString toHexString(const int val)
{ 
  OdString buffer;
  buffer.format(OD_T("%#x"), val); 
  return buffer;
}

/************************************************************************/
/* Convert the specified value to an ArcSymbolType string               */
/************************************************************************/
OdString toArcSymbolTypeString(const int val)
{
  OdString retVal (OD_T("???"));
  switch(val)
  {
    case 0: retVal = OD_T("Precedes text"); break;
    case 1: retVal = OD_T("Above text");    break;
    case 2: retVal = OD_T("None");          break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdGiVisibility string              */
/************************************************************************/
OdString toOdGiVisibilityString(const OdUInt8 val)
{ 
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case kOdGiInvisible  : retVal = OD_T("kOdGiInvisible");   break; 
    case kOdGiVisible    : retVal = OD_T("kOdGiVisible");    break; 
  }
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdGiFillType string                */
/************************************************************************/
OdString toString(const OdGiFillType val)
{ 
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case kOdGiFillAlways   : retVal = OD_T("kOdGiFillAlways");   break; 
    case kOdGiFillNever    : retVal = OD_T("kOdGiFillNever");    break; 
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to a bool string                         */
/************************************************************************/
OdString toString(const bool val)
{ 
  return (val  ? OD_T("true") : OD_T("false")); 
}

/************************************************************************/
/* Convert the specified value to a fixed-point double string with      */
/* trailing zeros suppressed                                            */
/************************************************************************/
OdString toString(const double val)
{ 
  OdString temp;
  if (fabs(val) < 1e6)
  {
    temp.format(OD_T("%.4f"), val);

    /***********************************************************************/
    /* Strip all but one trailing zero from buffer                         */
    /***********************************************************************/
    for (int n = temp.getLength(); ((temp[n-2] != '.') && (temp[n-1] == '0')); n--)
    {
      temp = temp.left(temp.getLength() - 1);
    }
  }
  else
  {
    temp.format(OD_T("%.4E"), val);
  }
  return temp;
}

/************************************************************************/
/* Convert the specified value to formatted double string               */
/************************************************************************/
OdString toString(const OdString& fmt, double val)
{ 
  OdString buffer;
  buffer.format(fmt.c_str(), val); 
  return buffer;
}

/************************************************************************/
/* Convert the specified value to an int string                         */
/************************************************************************/
OdString toString(const int val)
{ 
  OdString buffer;
  buffer.format(OD_T("%d"), val); 
  return buffer;
}

/************************************************************************/
/* Convert the specified value to a formatted int string                */
/************************************************************************/
OdString toString(const OdString& fmt, int val)
{ 
  OdString buffer;
  buffer.format(fmt.c_str(), val); 
  return buffer;
}

/************************************************************************/
/* Convert the specified value to an OdGiPolyline::SegType string       */
/************************************************************************/
OdString toString(const OdGiPolyline::SegType val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdGiPolyline::kArc        : retVal = OD_T("kArc");        break;
    case OdGiPolyline::kCoincident : retVal = OD_T("kCoincident"); break;
    case OdGiPolyline::kEmpty      : retVal = OD_T("kEmpty");      break;  
    case OdGiPolyline::kLine       : retVal = OD_T("kLine");       break;  
    case OdGiPolyline::kPoint      : retVal = OD_T("kPoint");      break;  
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified 24 bit color value to an RGB string            */
/************************************************************************/
OdString toRGBString(const OdUInt32& val) 
{ 
  OdString retVal = OD_T("r") + toString((int)ODGETRED(val)) + OD_T(":g") 
    + toString((int)ODGETGREEN(val)) + OD_T(":b") + toString((int)ODGETBLUE(val));
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdDgCmEntityColor string           */
/*                                                                      */
/************************************************************************/
OdString toString(const OdDgCmEntityColor& val) 
{ 
  OdString retVal(OD_T("???"));
  if (val.isByLayer())
  {
    retVal =  OD_T("ByLayer");
  }
  else if (val.isByBlock())
  {
    retVal = OD_T("ByBlock");
  }
  else if (val.isForeground())
  {
   retVal = OD_T("Foreground");
  }
  else if (val.isNone())
  {
    retVal = OD_T("None");
  }
  else if (val.isByACI()
           || val.isByDgnIndex())
  {
    retVal = toString(OD_T("ACI %d"), val.colorIndex());
  }
  else if (val.isByColor())
  {
    retVal = OdString(OD_T("ByColor")) + OD_T(" r") + toString(val.red()) + OD_T(":g") 
        + toString(val.green()) + OD_T(":b") + toString(val.blue());
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdCmTransparency string            */
/************************************************************************/
OdString toString(const OdCmTransparency& val) 
{ 
  OdString retVal(OD_T("???"));
  if (val.isByLayer())
  {
    retVal = OD_T("ByLayer");
  }
  else if (val.isByBlock())
  {
    retVal = OD_T("ByBlock");
  }
  else if (val.isClear())
  {
   retVal = OD_T("Clear");
  }
  else if (val.isSolid())
  {
    retVal = OD_T("Solid");
  }
  else if (val.isByAlpha())
  {
    retVal = toString(OD_T("ByAlpha %d"), (int)val.alpha());
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdGsView::RenderMode string        */
/************************************************************************/
OdString toString(const OdGsView::RenderMode val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdGsView::kBoundingBox                : retVal = OD_T("kBoundingBox");                break;
    case OdGsView::k2DOptimized                : retVal = OD_T("k2DOptimized");                break;
    case OdGsView::kWireframe                  : retVal = OD_T("kWireframe");                  break;
    case OdGsView::kHiddenLine                 : retVal = OD_T("kHiddenLine");                 break;
    case OdGsView::kFlatShaded                 : retVal = OD_T("kFlatShaded");                 break;
    case OdGsView::kGouraudShaded              : retVal = OD_T("kGouraudShaded");              break;
    case OdGsView::kFlatShadedWithWireframe    : retVal = OD_T("kFlatShadedWithWireframe");    break;
    case OdGsView::kGouraudShadedWithWireframe : retVal = OD_T("kGouraudShadedWithWireframe"); break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDbHandle string                  */
/************************************************************************/
OdString toString(const OdDbHandle& val)
{
  return OD_T("[") + val.ascii() + OD_T("]");
}


/************************************************************************/
/* Convert the specified value to an OdRxClass name string              */
/************************************************************************/
OdString toString(const OdRxClass* val)
{
  return OdString(OD_T("<")) + (val->name()) + OD_T(">");
}

/************************************************************************/
/* Convert the specified value to an OdGeScale2d string                 */
/************************************************************************/
OdString toString(const OdGeScale2d& val)
{ 
  return OD_T("{") + toString(val.sx) + OD_T(" ") + toString(val.sy) + OD_T("}");
}

/************************************************************************/
/* Convert the specified value to an OdGeScale3d string                 */
/************************************************************************/
OdString toString(const OdGeScale3d& val)
{ 
  OdString s;
  s.format(OD_T("[%.1f %.1f %.1f]"), val.sx, val.sy, val.sz);
  return s;
}
/************************************************************************/
/* Convert the specified value to an OdGePoint2d string                 */
/************************************************************************/
OdString toString(const OdGePoint2d& val)
{ 
  OdString s;
  s.format(OD_T("[%.1f %.1f]"), val.x, val.y);
  return s;
}

/************************************************************************/
/* Convert the specified value to an OdGePoint3d string                 */
/************************************************************************/
OdString toString(const OdGePoint3d& val)
{ 
  OdString s;
  s.format(OD_T("[%.1f %.1f %.1f]"), val.x, val.y, val.z);
  return s;
}

/************************************************************************/
/* Convert the specified value to an OdGeVector2d string                */
/************************************************************************/
OdString toString(const OdGeVector2d& val)
{ 
  OdString s;
  s.format(OD_T("[%.1f %.1f]"), val.x, val.y);
  return s;
}

/************************************************************************/
/* Convert the specified value to an OdGeVector3d string                */
/************************************************************************/
OdString toString(const OdGeVector3d& val)
{ 
  OdString s;
  s.format(OD_T("[%.1f %.1f %.1f]"), val.x, val.y, val.z);
  return s;
}
/************************************************************************/
/* Convert the specified value to an OdGeVector3d string                */
/************************************************************************/
OdString toString(const OdGeVector3d* pVal)
{ 
  if(pVal && !pVal->isZeroLength())
    return toString(*pVal);
  else
    return OD_T("Null");
}
/************************************************************************/
/* Convert the specified value to an OdString string                    */
/************************************************************************/
OdString toString(const OdChar* val) 
{ 
  if (!val) 
  {
    return OD_T("\"\"");
  }
  
  return val;
}
/************************************************************************/
/* Convert the specified value to an OdString string                    */
/************************************************************************/
OdString toString(const OdString& val) 
{ 
  if (val.isEmpty()) 
  {
    return OD_T("\"\"");
  }
  return val;
}

/************************************************************************/
/* Convert the specified value to an OdGiRasterImage::Units string      */
/************************************************************************/
OdString toString(const OdGiRasterImage::Units val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdGiRasterImage::kNone         : retVal = OD_T("kNone");         break;
    case OdGiRasterImage::kMillimeter   : retVal = OD_T("kMillimeter");   break;
    case OdGiRasterImage::kCentimeter   : retVal = OD_T("kCentimeter");   break;
    case OdGiRasterImage::kMeter        : retVal = OD_T("kMeter");        break;
    case OdGiRasterImage::kKilometer    : retVal = OD_T("kKilometer");    break;
    case OdGiRasterImage::kInch         : retVal = OD_T("kInch");         break;
    case OdGiRasterImage::kFoot         : retVal = OD_T("kFoot");         break;
    case OdGiRasterImage::kYard         : retVal = OD_T("kYard");         break;
    case OdGiRasterImage::kMile         : retVal = OD_T("kMile");         break;
    case OdGiRasterImage::kMicroinches  : retVal = OD_T("kMicroinches");  break;
    case OdGiRasterImage::kMils         : retVal = OD_T("kMils");         break;
    case OdGiRasterImage::kAngstroms    : retVal = OD_T("kAngstroms");    break;
    case OdGiRasterImage::kNanometers   : retVal = OD_T("kNanometers");   break;
    case OdGiRasterImage::kMicrons      : retVal = OD_T("kMicrons");      break;
    case OdGiRasterImage::kDecimeters   : retVal = OD_T("kDecimeters");   break;
    case OdGiRasterImage::kDekameters   : retVal = OD_T("kDekameters");   break;
    case OdGiRasterImage::kHectometers  : retVal = OD_T("kHectometers");  break;
    case OdGiRasterImage::kGigameters   : retVal = OD_T("kGigameters");   break;
    case OdGiRasterImage::kAstronomical : retVal = OD_T("kAstronomical"); break;
    case OdGiRasterImage::kLightYears   : retVal = OD_T("kLightYears");   break;
    case OdGiRasterImage::kParsecs      : retVal = OD_T("kParsecs");      break;
  }
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdGiArcType string                 */
/************************************************************************/
OdString toString(const OdGiArcType val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case kOdGiArcSimple  : retVal = OD_T("kOdGiArcSimple");   break;
    case kOdGiArcSector  : retVal = OD_T("kOdGiArcSector");   break;
    case kOdGiArcChord   : retVal = OD_T("kOdGiArcChord");    break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdGe::EntityId string              */
/************************************************************************/
OdString toString(const OdGe::EntityId val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdGe::kAugPolyline2d         : retVal = OD_T("kAugPolyline2d");          break;
    case OdGe::kAugPolyline3d         : retVal = OD_T("kAugPolyline3d");          break;
    case OdGe::kBezierCurve3d         : retVal = OD_T("kBezierCurve3d");          break;
    case OdGe::kBoundBlock2d          : retVal = OD_T("kBoundBlock2d");           break;
    case OdGe::kBoundBlock3d          : retVal = OD_T("kBoundBlock3d");           break;
    case OdGe::kBoundedPlane          : retVal = OD_T("kBoundedPlane");           break;
    case OdGe::kCircArc2d             : retVal = OD_T("kCircArc2d");              break;
    case OdGe::kCircArc3d             : retVal = OD_T("kCircArc3d");              break;
    case OdGe::kClipBoundary2d        : retVal = OD_T("kClipBoundary2d");         break;
    case OdGe::kCompositeCrv2d        : retVal = OD_T("kCompositeCrv2d");         break;
    case OdGe::kCompositeCrv3d        : retVal = OD_T("kCompositeCrv3d");         break;
    case OdGe::kCone                  : retVal = OD_T("kCone");                   break;
    case OdGe::kConic2d               : retVal = OD_T("kConic2d");                break;
    case OdGe::kConic3d               : retVal = OD_T("kConic3d");                break;
    case OdGe::kCubicSplineCurve2d    : retVal = OD_T("kCubicSplineCurve2d");     break;
    case OdGe::kCubicSplineCurve3d    : retVal = OD_T("kCubicSplineCurve3d");     break;
    case OdGe::kCurve2d               : retVal = OD_T("kCurve2d");                break;
    case OdGe::kCurve3d               : retVal = OD_T("kCurve3d");                break;
    case OdGe::kCurveBoundedSurface   : retVal = OD_T("kCurveBoundedSurface");    break;
    case OdGe::kCurveCurveInt2d       : retVal = OD_T("kCurveCurveInt2d");        break;
    case OdGe::kCurveCurveInt3d       : retVal = OD_T("kCurveCurveInt3d");        break;
    case OdGe::kCurveSampleData       : retVal = OD_T("kCurveSampleData");        break;
    case OdGe::kCurveSurfaceInt       : retVal = OD_T("kCurveSurfaceInt");        break;
    case OdGe::kCylinder              : retVal = OD_T("kCylinder");               break;
    case OdGe::kDSpline2d             : retVal = OD_T("kDSpline2d");              break;
    case OdGe::kDSpline3d             : retVal = OD_T("kDSpline3d");              break;
    case OdGe::kEllipArc2d            : retVal = OD_T("kEllipArc2d");             break;
    case OdGe::kEllipArc3d            : retVal = OD_T("kEllipArc3d");             break;
    case OdGe::kEllipCone             : retVal = OD_T("kEllipCone");              break;
    case OdGe::kEllipCylinder         : retVal = OD_T("kEllipCylinder");          break;
    case OdGe::kEntity2d              : retVal = OD_T("kEntity2d");               break;
    case OdGe::kEntity3d              : retVal = OD_T("kEntity3d");               break;
    case OdGe::kEnvelope2d            : retVal = OD_T("kEnvelope2d");             break;
    case OdGe::kExternalBoundedSurface: retVal = OD_T("kExternalBoundedSurface"); break;
    case OdGe::kExternalCurve2d       : retVal = OD_T("kExternalCurve2d");        break;
    case OdGe::kExternalCurve3d       : retVal = OD_T("kExternalCurve3d");        break;
    case OdGe::kExternalObject        : retVal = OD_T("kExternalObject");         break;
    case OdGe::kExternalSurface       : retVal = OD_T("kExternalSurface");        break;
    case OdGe::kFitData3d             : retVal = OD_T("kFitData3d");              break;
    case OdGe::kHatch                 : retVal = OD_T("kHatch");                  break;
    case OdGe::kIntervalBoundBlock    : retVal = OD_T("kIntervalBoundBlock");     break;
    case OdGe::kLine2d                : retVal = OD_T("kLine2d");                 break;
    case OdGe::kLine3d                : retVal = OD_T("kLine3d");                 break;
    case OdGe::kLineSeg2d             : retVal = OD_T("kLineSeg2d");              break;
    case OdGe::kLineSeg3d             : retVal = OD_T("kLineSeg3d");              break;
    case OdGe::kLinearEnt2d           : retVal = OD_T("kLinearEnt2d");            break;
    case OdGe::kLinearEnt3d           : retVal = OD_T("kLinearEnt3d");            break;
    case OdGe::kNurbCurve2d           : retVal = OD_T("kNurbCurve2d");            break;
    case OdGe::kNurbCurve3d           : retVal = OD_T("kNurbCurve3d");            break;
    case OdGe::kNurbSurface           : retVal = OD_T("kNurbSurface");            break;
    case OdGe::kObject                : retVal = OD_T("kObject");                 break;
    case OdGe::kOffsetCurve2d         : retVal = OD_T("kOffsetCurve2d");          break;
    case OdGe::kOffsetCurve3d         : retVal = OD_T("kOffsetCurve3d");          break;
    case OdGe::kOffsetSurface         : retVal = OD_T("kOffsetSurface");          break;
    case OdGe::kPlanarEnt             : retVal = OD_T("kPlanarEnt");              break;
    case OdGe::kPlane                 : retVal = OD_T("kPlane");                  break;
    case OdGe::kPointEnt2d            : retVal = OD_T("kPointEnt2d");             break;
    case OdGe::kPointEnt3d            : retVal = OD_T("kPointEnt3d");             break;
    case OdGe::kPointOnCurve2d        : retVal = OD_T("kPointOnCurve2d");         break;
    case OdGe::kPointOnCurve3d        : retVal = OD_T("kPointOnCurve3d");         break;
    case OdGe::kPointOnSurface        : retVal = OD_T("kPointOnSurface");         break;
    case OdGe::kPolyline2d            : retVal = OD_T("kPolyline2d");             break;
    case OdGe::kPolyline3d            : retVal = OD_T("kPolyline3d");             break;
    case OdGe::kPolynomCurve3d        : retVal = OD_T("kPolynomCurve3d");         break;
    case OdGe::kPosition2d            : retVal = OD_T("kPosition2d");             break;
    case OdGe::kPosition3d            : retVal = OD_T("kPosition3d");             break;
    case OdGe::kRay2d                 : retVal = OD_T("kRay2d");                  break;
    case OdGe::kRay3d                 : retVal = OD_T("kRay3d");                  break;
    case OdGe::kSphere                : retVal = OD_T("kSphere");                 break;
    case OdGe::kSplineEnt2d           : retVal = OD_T("kSplineEnt2d");            break;
    case OdGe::kSplineEnt3d           : retVal = OD_T("kSplineEnt3d");            break;
    case OdGe::kSurface               : retVal = OD_T("kSurface");                break;
    case OdGe::kSurfaceCurve2dTo3d    : retVal = OD_T("kSurfaceCurve2dTo3d");     break;
    case OdGe::kSurfaceCurve3dTo2d    : retVal = OD_T("kSurfaceCurve3dTo2d");     break;
    case OdGe::kSurfaceSurfaceInt     : retVal = OD_T("kSurfaceSurfaceInt");      break;
    case OdGe::kTorus                 : retVal = OD_T("kTorus");                  break;
    case OdGe::kTrimmedCrv2d          : retVal = OD_T("kTrimmedCrv2d");           break;
    case OdGe::kTrimmedCurve2d        : retVal = OD_T("kTrimmedCurve2d");         break;
    case OdGe::kTrimmedCurve3d        : retVal = OD_T("kTrimmedCurve3d");         break;
    case OdGe::kTrimmedSurface        : retVal = OD_T("kTrimmedSurface");         break;
  }
  return retVal;
}


/************************************************************************/
/* Convert the specified value to an OdGiOrientationType string         */
/************************************************************************/
OdString toString(const OdGiOrientationType val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case kOdGiCounterClockwise    : retVal = OD_T("kOdGiCounterClockwise");  break;
    case kOdGiNoOrientation       : retVal = OD_T("kOdGiNoOrientation");     break;
    case kOdGiClockwise           : retVal = OD_T("kOdGiClockwise");         break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDgObjectId string                */
/************************************************************************/
OdString toString(const OdDgElementId& val)
{
  if (val.isNull()) 
  {
    return OD_T("Null");
  }

  if (val.isErased())
  {
    return OD_T("Erased");
  }

  /**********************************************************************/
  /* Open the object                                                    */
  /**********************************************************************/
//  OdRxObjectPtr pObject = val.safeOpenObject();
  OdDgElementPtr pElm = val.safeOpenObject();

  /**********************************************************************/
  /* Return the name of an OdDgTableRecord                              */
  /**********************************************************************/
  if (pElm->isKindOf(OdDgTableRecord::desc()))
  {
    OdDgTableRecordPtr pRec = pElm;
    return pRec->getName(); 
  }

  /**********************************************************************/
  /* Return the name of an OdDgSymbolTableRecord                        */
  /**********************************************************************/
//  if (pObject->isKindOf(OdDgSymbolTableRecord::desc()))
//  {
//    OdDgSymbolTableRecordPtr pSTR = pObject;
//    return pSTR->getName(); 
//  }

  /**********************************************************************/
  /* Return the name of an OdDgMlineStyle                               */
  /**********************************************************************/
//  if (pObject->isKindOf(OdDgMlineStyle::desc()))
//  {
//    OdDgMlineStylePtr pStyle = pObject;
//    return pStyle->name(); 
//  }

  /**********************************************************************/
  /* Return the name of a PlotStyle                                      */
  /**********************************************************************/
//  if (pObject->isKindOf(OdDgPlaceHolder::desc()))
//  {
//    OdDgDictionaryPtr pDictionary = val.database()->getPlotStyleNameDictionaryId().safeOpenObject(); 
//    OdString plotStyleName = pDictionary->nameAt(val);
//    return plotStyleName; 
//  }

  /**********************************************************************/
  /* Return the name of an OdDgMaterial                                 */
  /**********************************************************************/
//  if (pObject->isKindOf(OdDgMaterial::desc()))
//  {
//    OdDgMaterialPtr pMaterial = pObject;
//    return pMaterial->name(); 
//  }
  
  /**********************************************************************/
  /* We don't know what it is, so return the description of the object  */
  /* object specified by the ObjectId                                   */
  /**********************************************************************/
//  return toString(pObject->isA());
  return toString(pElm->isA());
}

/************************************************************************/
/* Convert the specified value to an OdDg::LineWeight string            */
/************************************************************************/
OdString toString(const OdDb::LineWeight val)
{ 
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdDb::kLnWt000          : retVal = OD_T("kLnWt000");         break;
    case OdDb::kLnWt005          : retVal = OD_T("kLnWt005");         break;
    case OdDb::kLnWt009          : retVal = OD_T("kLnWt009");         break;
    case OdDb::kLnWt013          : retVal = OD_T("kLnWt013");         break;
    case OdDb::kLnWt015          : retVal = OD_T("kLnWt015");         break;
    case OdDb::kLnWt018          : retVal = OD_T("kLnWt018");         break;
    case OdDb::kLnWt020          : retVal = OD_T("kLnWt020");         break;
    case OdDb::kLnWt025          : retVal = OD_T("kLnWt025");         break;
    case OdDb::kLnWt030          : retVal = OD_T("kLnWt030");         break;
    case OdDb::kLnWt035          : retVal = OD_T("kLnWt035");         break;
    case OdDb::kLnWt040          : retVal = OD_T("kLnWt040");         break;
    case OdDb::kLnWt050          : retVal = OD_T("kLnWt050");         break;
    case OdDb::kLnWt053          : retVal = OD_T("kLnWt053");         break;
    case OdDb::kLnWt060          : retVal = OD_T("kLnWt060");         break;
    case OdDb::kLnWt070          : retVal = OD_T("kLnWt070");         break;
    case OdDb::kLnWt080          : retVal = OD_T("kLnWt080");         break;
    case OdDb::kLnWt090          : retVal = OD_T("kLnWt090");         break;
    case OdDb::kLnWt100          : retVal = OD_T("kLnWt100");         break;
    case OdDb::kLnWt106          : retVal = OD_T("kLnWt106");         break;
    case OdDb::kLnWt120          : retVal = OD_T("kLnWt120");         break;
    case OdDb::kLnWt140          : retVal = OD_T("kLnWt140");         break;
    case OdDb::kLnWt158          : retVal = OD_T("kLnWt158");         break;
    case OdDb::kLnWt200          : retVal = OD_T("kLnWt200");         break;
    case OdDb::kLnWt211          : retVal = OD_T("kLnWt211");         break;
    case OdDb::kLnWtByLayer      : retVal = OD_T("kLnWtByLayer");     break;
    case OdDb::kLnWtByBlock      : retVal = OD_T("kLnWtByBlock");     break;
    case OdDb::kLnWtByLwDefault  : retVal = OD_T("kLnWtByLwDefault"); break;
  }
  return retVal; 
}
