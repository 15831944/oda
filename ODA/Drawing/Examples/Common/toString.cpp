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

#include "toString.h"
#include "OdUtilAds.h"

static PrintConsoleFunc g_printConsoleFunc = odPrintConsoleString;

/************************************************************************/
/* Sets printConsoleString function                                     */
/************************************************************************/
void odExSetPrintConsoleFunc(PrintConsoleFunc printConsoleFunc)
{
    g_printConsoleFunc = printConsoleFunc;
}

#if defined (_MSC_VER) && (_MSC_VER >= 1300) && defined(_WINRT)
void odExSetPrintConsoleFunc(PrintConsoleFuncNative writeConsoleFunc)
{
    g_printConsoleFunc = (PrintConsoleFunc)writeConsoleFunc;
}
#endif
/************************************************************************/
/* Output a string in the form                                          */
/*   leftString:. . . . . . . . . . . .rightString                      */
/************************************************************************/
void writeLine(int indent, OdString leftString, OdString rightString, int colWidth)
{
  const OdString spaces(OD_T("                                                            "));
  const OdString leader(OD_T(". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . "));

  OdString buffer;
  
  const int tabSize = 2;

  /**********************************************************************/
  /* Indent leftString with spaces characters                           */
  /**********************************************************************/  
  leftString = spaces.left(tabSize*indent) + leftString;

  /**********************************************************************/
  /* If rightString is not specified, just output the indented          */
  /* leftString. Otherwise, fill the space between leftString and       */
  /* rightString with leader characters.                                */
  /**********************************************************************/  
  if (rightString.isEmpty())
  {
    buffer = leftString;
  }
  else
  {
    int leaders = colWidth-leftString.getLength();
    if (leaders > 0){
      buffer = leftString + leader.mid(leftString.getLength(), leaders) + rightString;  
    }
    else
    {
      buffer = leftString + L' ' + rightString;  
    }
  }

  g_printConsoleFunc(L"%ls\n", buffer.c_str());
}  


/************************************************************************/
/* Fix negative zeroes with format("%.1f").                             */
/************************************************************************/
static double FixNZ(const double &dVal, const double dTol = 5.1e-2)
{
  return OdZero(dVal, dTol) ? 0. : dVal;
}

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
/* Convert the specified value to a LoopType string                     */
/************************************************************************/
OdString toLooptypeString(const int loopType)
{
  OdString retVal;
  if(loopType & OdDbHatch::kExternal)
    retVal += OD_T(" | kExternal");

  if(loopType & OdDbHatch::kPolyline)
    retVal += OD_T(" | kPolyline");

  if(loopType & OdDbHatch::kDerived)
    retVal += OD_T(" | kDerived");
    
  if(loopType & OdDbHatch::kTextbox)
    retVal += OD_T(" | kTextbox");
  
  if(loopType & OdDbHatch::kOutermost)
    retVal += OD_T(" | kOutermost");
  
  if(loopType & OdDbHatch::kNotClosed)
    retVal += OD_T(" | kNotClosed");
  
  if(loopType & OdDbHatch::kSelfIntersecting)
    retVal += OD_T(" | kSelfIntersecting");
  
  if(loopType & OdDbHatch::kTextIsland)
    retVal += OD_T(" | kTextIsland");
  
  if(loopType & OdDbHatch::kDuplicate)
    retVal += OD_T(" | kDuplicate");

  return (retVal.isEmpty() ? OdString(OD_T("kDefault")) : retVal.mid(3));
}

/************************************************************************/
/* Convert the specified value to an OdGiVisibility string              */
/************************************************************************/
//OdString toString(const OdAveGraniteMaterial::GraniteColor val)
//{ 
//  OdString retVal(OD_T("???"));
//  switch (val) 
//  {
//    case OdAveGraniteMaterial::kFirst     : retVal = OD_T("kFirst");     break; 
//    case OdAveGraniteMaterial::kSecond    : retVal = OD_T("kSecond");    break; 
//    case OdAveGraniteMaterial::kThird     : retVal = OD_T("kThird");     break; 
//    case OdAveGraniteMaterial::kFourth    : retVal = OD_T("kFourth");    break; 
//  }
//  return retVal;
//}
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
  double _val = FixNZ(val, 5.1e-5);

  if (fabs(_val) < 1e6)
  {
    temp.format(OD_T("%.4f"), _val);

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
    temp.format(OD_T("%.4E"), _val);
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
/* Convert the specified value to an OdArcTextAlignment string          */
/************************************************************************/
OdString toString(const OdArcTextAlignment val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case kFit    : retVal = OD_T("kFit");    break; 
    case kLeft   : retVal = OD_T("kLeft");   break; 
    case kRight  : retVal = OD_T("kRight");  break; 
    case kCenter : retVal = OD_T("kCenter"); break; 
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDbMText::AttachmentPoint string  */
/************************************************************************/
OdString toString(const OdDbMText::AttachmentPoint val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDbMText::kTopLeft      : retVal = OD_T("kTopLeft");      break;
    case OdDbMText::kTopCenter    : retVal = OD_T("kTopCenter");    break;
    case OdDbMText::kTopRight     : retVal = OD_T("kTopRight");     break;
    case OdDbMText::kMiddleLeft   : retVal = OD_T("kMiddleLeft");   break;
    case OdDbMText::kMiddleCenter : retVal = OD_T("kMiddleCenter"); break;
    case OdDbMText::kMiddleRight  : retVal = OD_T("kMiddleRight");  break;
    case OdDbMText::kBottomLeft   : retVal = OD_T("kBottomLeft");   break;
    case OdDbMText::kBottomCenter : retVal = OD_T("kBottomCenter"); break;
    case OdDbMText::kBottomRight  : retVal = OD_T("kBottomRight");  break;
    case OdDbMText::kBaseLeft     : retVal = OD_T("kBaseLeft");     break;
    case OdDbMText::kBaseCenter   : retVal = OD_T("kBaseCenter");   break;
    case OdDbMText::kBaseRight    : retVal = OD_T("kBaseRight");    break;
    case OdDbMText::kBaseAlign    : retVal = OD_T("kBaseAlign");    break;
    case OdDbMText::kBottomAlign  : retVal = OD_T("kBottomAlign");  break;
    case OdDbMText::kMiddleAlign  : retVal = OD_T("kMiddleAlign");  break;
    case OdDbMText::kTopAlign     : retVal = OD_T("kTopAlign");     break;
    case OdDbMText::kBaseFit      : retVal = OD_T("kBaseFit");      break;
    case OdDbMText::kBottomFit    : retVal = OD_T("kBottomFit");    break;
    case OdDbMText::kMiddleFit    : retVal = OD_T("kMiddleFit");    break;
    case OdDbMText::kTopFit       : retVal = OD_T("kTopFit");       break;
    case OdDbMText::kBaseMid      : retVal = OD_T("kBaseMid");      break;
    case OdDbMText::kBottomMid    : retVal = OD_T("kBottomMid");    break;
    case OdDbMText::kMiddleMid    : retVal = OD_T("kMiddleMid");    break;
    case OdDbMText::kTopMid       : retVal = OD_T("kTopMid");       break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDbMText::FlowDirection string    */
/************************************************************************/
OdString toString(const OdDbMText::FlowDirection val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDbMText::kLtoR     : retVal = OD_T("kLtoR");     break;
    case OdDbMText::kRtoL     : retVal = OD_T("kRtoL");     break;
    case OdDbMText::kTtoB     : retVal = OD_T("kTtoB");     break;
    case OdDbMText::kBtoT     : retVal = OD_T("kBtoT");     break;
    case OdDbMText::kByStyle  : retVal = OD_T("kByStyle");  break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::Vertex2dType string          */
/************************************************************************/
OdString toString(const OdDb::Vertex2dType val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDb::k2dVertex          : retVal = OD_T("k2dVertex");          break;
    case OdDb::k2dSplineCtlVertex : retVal = OD_T("k2dSplineCtlVertex"); break;
    case OdDb::k2dSplineFitVertex : retVal = OD_T("k2dSplineFitVertex"); break;
    case OdDb::k2dCurveFitVertex  : retVal = OD_T("k2dCurveFitVertex");  break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::Vertex3dType string          */
/************************************************************************/
OdString toString(const OdDb::Vertex3dType val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDb::k3dSimpleVertex  : retVal = OD_T("k3dSimpleVertex");  break;
    case OdDb::k3dControlVertex : retVal = OD_T("k3dControlVertex"); break;
    case OdDb::k3dFitVertex     : retVal = OD_T("k3dFitVertex");     break;  
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDbPolyline::SegType string       */
/************************************************************************/
OdString toString(const OdDbPolyline::SegType val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDbPolyline::kArc        : retVal = OD_T("kArc");        break;
    case OdDbPolyline::kCoincident : retVal = OD_T("kCoincident"); break;
    case OdDbPolyline::kEmpty      : retVal = OD_T("kEmpty");      break;  
    case OdDbPolyline::kLine       : retVal = OD_T("kLine");       break;  
    case OdDbPolyline::kPoint      : retVal = OD_T("kPoint");      break;  
  }
  return retVal;
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
/* Convert the specified value to an OdCmColor string                   */
/*                                                                      */
/* Note: isByACI() returns true for ACI values of 0 (ByBlock),          */
/* 7 (ByForeground), 256 (ByLayer), and 257 (None).                     */
/************************************************************************/
OdString toString(const OdCmColor& val) 
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
  else if (val.isByACI())
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
/* Convert the specified 24 bit color value to an RGB string            */
/************************************************************************/
OdString toRGBString(const OdUInt32& val) 
{ 
  OdString retVal = OD_T("r") + toString((int)ODGETRED(val)) + OD_T(":g") 
    + toString((int)ODGETGREEN(val)) + OD_T(":b") + toString((int)ODGETBLUE(val));
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdCmEntityColor string             */
/*                                                                      */
/* Note: isByACI() returns true for ACI values of 0 (ByBlock),          */
/* 7 (ByForeground), 256 (ByLayer), and 257 (None).                     */
/************************************************************************/
OdString toString(const OdCmEntityColor& val) 
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
  else if (val.isByACI())
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
    retVal =  OD_T("ByLayer");
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
/* Convert the specified value to an OdDb::DwgVersion string            */
/************************************************************************/
OdString toString(const OdDb::DwgVersion val) 
{ 
  return OdString(OdDb::DwgVersionToStr(val));
}


/************************************************************************/
/* Convert the specified value to an OdDb::EndCaps string               */
/************************************************************************/
OdString toString(const OdDb::EndCaps val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDb::kEndCapNone   : retVal = OD_T("kEndCapNone");   break;
    case OdDb::kEndCapRound  : retVal = OD_T("kEndCapRound");  break;
    case OdDb::kEndCapAngle  : retVal = OD_T("kEndCapAngle");  break;
    case OdDb::kEndCapSquare : retVal = OD_T("kEndCapSquare"); break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::JoinStyle string             */
/************************************************************************/
OdString toString(const OdDb::JoinStyle val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDb::kJnStylNone  : retVal = OD_T("kJnStylNone");  break;
    case OdDb::kJnStylRound : retVal = OD_T("kJnStylRound"); break;
    case OdDb::kJnStylAngle : retVal = OD_T("kJnStylAngle"); break;  
    case OdDb::kJnStylFlat  : retVal = OD_T("kJnStylFlat");  break;
  }
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdDb::LineWeight string            */
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

/************************************************************************/
/* Convert the specified value to an OdDb::MeasurementValue string      */
/************************************************************************/
OdString toString(const OdDb::MeasurementValue val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDb::kEnglish : retVal = OD_T("kEnglish"); break;
    case OdDb::kMetric  : retVal = OD_T("kMetric");  break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::Planarity string             */
/************************************************************************/
OdString toString(const OdDb::Planarity val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDb::kNonPlanar : retVal = OD_T("kNonPlanar"); break;
    case OdDb::kPlanar    : retVal = OD_T("kPlanar");    break;
    case OdDb::kLinear    : retVal = OD_T("kLinear");    break;  
  }
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdDb::PlotStyleNameType string     */
/************************************************************************/
OdString toString(const OdDb::PlotStyleNameType val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDb::kPlotStyleNameByLayer       : retVal = OD_T("kPlotStyleNameByLayer");       break;
    case OdDb::kPlotStyleNameByBlock       : retVal = OD_T("kPlotStyleNameByBlock");       break;
    case OdDb::kPlotStyleNameIsDictDefault : retVal = OD_T("kPlotStyleNameIsDictDefault"); break;  
    case OdDb::kPlotStyleNameById          : retVal = OD_T("kPlotStyleNameById");          break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::ProxyImage string            */
/************************************************************************/
OdString toString(const OdDb::ProxyImage val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDb::kProxyNotShow     : retVal = OD_T("kProxyNotShow");     break;
    case OdDb::kProxyShow        : retVal = OD_T("kProxyShow");        break;
    case OdDb::kProxyBoundingBox : retVal = OD_T("kProxyBoundingBox"); break;  
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
    default:break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::RenderMode string            */
/************************************************************************/
OdString toString(const OdDb::RenderMode val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDb::k2DOptimized                : retVal = OD_T("k2DOptimized");                break;
    case OdDb::kWireframe                  : retVal = OD_T("kWireframe");                  break;
    case OdDb::kHiddenLine                 : retVal = OD_T("kHiddenLine");                 break;
    case OdDb::kFlatShaded                 : retVal = OD_T("kFlatShaded");                 break;
    case OdDb::kGouraudShaded              : retVal = OD_T("kGouraudShaded");              break;
    case OdDb::kFlatShadedWithWireframe    : retVal = OD_T("kFlatShadedWithWireframe");    break;
    case OdDb::kGouraudShadedWithWireframe : retVal = OD_T("kGouraudShadedWithWireframe"); break;
     default:break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::TextHorzMode string          */
/************************************************************************/
OdString toString(const OdDb::TextHorzMode val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDb::kTextLeft        : retVal = OD_T("kTextLeft");   break;
    case OdDb::kTextCenter      : retVal = OD_T("kTextCenter"); break;
    case OdDb::kTextRight       : retVal = OD_T("kTextRight");  break;
    case OdDb::kTextAlign       : retVal = OD_T("kTextAlign");  break;
    case OdDb::kTextMid         : retVal = OD_T("kTextMid");    break;
    case OdDb::kTextFit         : retVal = OD_T("kTextFit");    break;
  }
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdDb::TextVertMode string          */
/************************************************************************/
OdString toString(const OdDb::TextVertMode val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDb::kTextBase        : retVal = OD_T("kTextBase"); break;
    case OdDb::kTextBottom      : retVal = OD_T("kTextBottom"); break;
    case OdDb::kTextVertMid     : retVal = OD_T("kTextVertMid"); break;
    case OdDb::kTextTop         : retVal = OD_T("kTextTop"); break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::UnitsValue string            */
/************************************************************************/
OdString toString(const OdDb::UnitsValue val)
{
  return oddbGetUnitsName(val);
}
/************************************************************************/
/* Convert the specified value to an OdDb::Visibility string            */
/************************************************************************/
OdString toString(const OdDb::Visibility val)
{ 
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDb::kVisible   : retVal = OD_T("kVisible");   break;
    case OdDb::kInvisible : retVal = OD_T("kInvisible"); break;
  }
  return retVal; 
}

/************************************************************************/
/* Convert the specified value to an OdDb::XrefStatus string            */
/************************************************************************/
OdString toString(const OdDb::XrefStatus val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
  case OdDb::kXrfNotAnXref    : retVal = OD_T("kXrfNotAnXref");    break;
  case OdDb::kXrfResolved     : retVal = OD_T("kXrfResolved");     break;
  case OdDb::kXrfUnloaded     : retVal = OD_T("kXrfUnloaded");     break;
  case OdDb::kXrfUnreferenced : retVal = OD_T("kXrfUnreferenced"); break;
  case OdDb::kXrfFileNotFound : retVal = OD_T("kXrfFileNotFound"); break;
  case OdDb::kXrfUnresolved   : retVal = OD_T("kXrfUnresolved");   break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::AnnoType string              */
/************************************************************************/
OdString toString(const OdDbLeader::AnnoType val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdDbLeader::kBlockRef : retVal = OD_T("kBlockRef"); break;
    case OdDbLeader::kFcf      : retVal = OD_T("kFcf");      break;
    case OdDbLeader::kMText    : retVal = OD_T("kMText");    break;
    case OdDbLeader::kNoAnno   : retVal = OD_T("kNoAnno");   break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::HatchPatternType string      */
/************************************************************************/
OdString toString(const OdDbHatch::HatchPatternType val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdDbHatch::kPreDefined    : retVal = OD_T("kPreDefined");    break;
    case OdDbHatch::kUserDefined   : retVal = OD_T("kUserDefined");   break;
    case OdDbHatch::kCustomDefined : retVal = OD_T("kCustomDefined"); break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::HatchObjectType string       */
/************************************************************************/
OdString toString(const OdDbHatch::HatchObjectType val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdDbHatch::kHatchObject    : retVal = OD_T("kHatchObject");    break;
    case OdDbHatch::kGradientObject : retVal = OD_T("kGradientObject"); break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::HatchStyle string            */
/************************************************************************/
OdString toString(const OdDbHatch::HatchStyle val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdDbHatch::kNormal : retVal = OD_T("kNormal");    break;
    case OdDbHatch::kOuter  : retVal = OD_T("kOuter");     break;
    case OdDbHatch::kIgnore : retVal = OD_T("kIgnore");    break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDbBlockTableRecord::BlockScaling */
/* string                                                               */
/************************************************************************/
OdString toString(const OdDbBlockTableRecord::BlockScaling val)
{ 
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdDbBlockTableRecord::kAny     : retVal = OD_T("kAny");     break;
    case OdDbBlockTableRecord::kUniform : retVal = OD_T("kUniform"); break;
  }
  return retVal; 
}

/************************************************************************/
/* Convert the specified value to an OdDbDate string                    */
/************************************************************************/
OdString toString(const OdDbDate& val) { 
  OdString timeString;
  val.ctime(timeString);
  return timeString; 
}

/************************************************************************/
/* Convert the specified value to an OdDbDimAssoc::RotatedType string   */
/************************************************************************/
OdString toString(const OdDbDimAssoc::RotatedDimType val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdDbDimAssoc::kParallel           : retVal = OD_T("kParallel"); break;
    case OdDbDimAssoc::kPerpendicular      : retVal = OD_T("kPerpendicular");      break;
    default:break;
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
/* Convert the specified value to an OdDbObjectId string                */
/************************************************************************/
OdString toString(const OdDbObjectId val)
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
  OdDbObjectPtr pObject = val.safeOpenObject();

  /**********************************************************************/
  /* Return the name of an OdDbSymbolTableRecord                        */
  /**********************************************************************/
  if (pObject->isKindOf(OdDbSymbolTableRecord::desc()))
  {
    OdDbSymbolTableRecordPtr pSTR = pObject;
    return pSTR->getName(); 
  }

  /**********************************************************************/
  /* Return the name of an OdDbMlineStyle                               */
  /**********************************************************************/
  if (pObject->isKindOf(OdDbMlineStyle::desc()))
  {
    OdDbMlineStylePtr pStyle = pObject;
    return pStyle->name(); 
  }

  /**********************************************************************/
  /* Return the name of a PlotStyle                                      */
  /**********************************************************************/
  if (pObject->isKindOf(OdDbPlaceHolder::desc()))
  {
    OdDbDictionaryPtr pDictionary = val.database()->getPlotStyleNameDictionaryId().safeOpenObject(); 
    OdString plotStyleName = pDictionary->nameAt(val);
    return plotStyleName; 
  }

  /**********************************************************************/
  /* Return the name of an OdDbMaterial                                 */
  /**********************************************************************/
  if (pObject->isKindOf(OdDbMaterial::desc()))
  {
    OdDbMaterialPtr pMaterial = pObject;
    return pMaterial->name(); 
  }
  
  /**********************************************************************/
  /* We don't know what it is, so return the description of the object  */
  /* object specified by the ObjectId                                   */
  /**********************************************************************/
  return toString(pObject->isA());
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
  s.format(OD_T("[%.1f %.1f %.1f]"), FixNZ(val.sx), FixNZ(val.sy), FixNZ(val.sz));
  return s;
}
/************************************************************************/
/* Convert the specified value to an OdGePoint2d string                 */
/************************************************************************/
OdString toString(const OdGePoint2d& val)
{ 
  OdString s;
  s.format(OD_T("[%.1f %.1f]"), FixNZ(val.x), FixNZ(val.y));
  return s;
}

/************************************************************************/
/* Convert the specified value to an OdGePoint3d string                 */
/************************************************************************/
OdString toString(const OdGePoint3d& val)
{ 
  OdString s;
  s.format(OD_T("[%.1f %.1f %.1f]"), FixNZ(val.x), FixNZ(val.y), FixNZ(val.z));
  return s;
}

/************************************************************************/
/* Convert the specified value to an OdGeVector2d string                */
/************************************************************************/
OdString toString(const OdGeVector2d& val)
{ 
  OdString s;
  s.format(OD_T("[%.1f %.1f]"), FixNZ(val.x), FixNZ(val.y));
  return s;
}

/************************************************************************/
/* Convert the specified value to an OdGeVector3d string                */
/************************************************************************/
OdString toString(const OdGeVector3d& val)
{ 
  OdString s;
  s.format(OD_T("[%.1f %.1f %.1f]"), FixNZ(val.x), FixNZ(val.y), FixNZ(val.z));
  return s;
}
/************************************************************************/
/* Convert the specified value to an OdGeQuaternion string              */
/************************************************************************/
OdString toString(const OdGeQuaternion& val)
{ 
  OdString s;
  s.format(OD_T("[%.1f %.1f %.1f %.1f]"), FixNZ(val.w), FixNZ(val.x), FixNZ(val.y), FixNZ(val.z));
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
/* Convert the specified value to an OdDbViewport::ShadePlotType string */
/************************************************************************/
OdString toString(const OdDbViewport::ShadePlotType val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdDbViewport::kAsDisplayed : retVal = OD_T("kAsDisplayed"); break;
    case OdDbViewport::kWireframe   : retVal = OD_T("kWireframe");   break;
    case OdDbViewport::kHidden      : retVal = OD_T("kHidden");      break;
    case OdDbViewport::kRendered    : retVal = OD_T("kRendered");    break;
    case OdDbViewport::kVisualStyle : retVal = OD_T("kVisualStyle"); break;
    case OdDbViewport::kRenderPreset: retVal = OD_T("kRenderPreset");break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDbPlotSettings::ShadePlotType    */
/* string                                                               */
/************************************************************************/
OdString toString(const OdDbPlotSettings::ShadePlotType val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdDbPlotSettings::kAsDisplayed : retVal = OD_T("kAsDisplayed"); break;
    case OdDbPlotSettings::kHidden      : retVal = OD_T("kHidden");      break;
    case OdDbPlotSettings::kRendered    : retVal = OD_T("kRendered");    break;
    case OdDbPlotSettings::kWireframe   : retVal = OD_T("kWireframe");   break;
    case OdDbPlotSettings::kVisualStyle : retVal = OD_T("kVisualStyle"); break;
    case OdDbPlotSettings::kRenderPreset: retVal = OD_T("kRenderPreset");break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDbOle2Frame::Type string         */
/************************************************************************/
OdString toString(const OdDbOle2Frame::Type val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdDbOle2Frame::kEmbedded : retVal = OD_T("kEmbedded"); break;
    case OdDbOle2Frame::kLink     : retVal = OD_T("kLink");     break;
    case OdDbOle2Frame::kStatic   : retVal = OD_T("kStatic");   break;
    case OdDbOle2Frame::kUnknown  : retVal = OD_T("kUnknown");  break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDbRasterImage::ClipBoundaryType  */
/* string                                                               */
/************************************************************************/
OdString toString(const OdDbRasterImage::ClipBoundaryType val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdDbRasterImage::kInvalid  : retVal = OD_T("kInvalid"); break;
    case OdDbRasterImage::kPoly     : retVal = OD_T("kPoly");     break;
    case OdDbRasterImage::kRect     : retVal = OD_T("kRect");   break;
  }
  return retVal;
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
  return geToString(val);
}

/************************************************************************/
/* Convert the specified value to an OdDbDimAssoc::Associativity string */
/************************************************************************/
OdString toString(const OdDbDimAssoc::AssocFlags val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdDbDimAssoc::kFirstPointRef    : retVal = OD_T("kFirstPoint");   break;
    case OdDbDimAssoc::kSecondPointRef   : retVal = OD_T("kSecondPoint");  break;
    case OdDbDimAssoc::kThirdPointRef    : retVal = OD_T("kThirdPoint");   break;
    case OdDbDimAssoc::kFourthPointRef   : retVal = OD_T("kFourthPoint");  break;
  }
  return retVal;
}

/************************************************************************/
/* Convert the specified value to an OdDb::OsnapMode string             */
/************************************************************************/
OdString toString(const OdDb::OsnapMode val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdDb::kOsModeEnd     : retVal = OD_T("kOsModeEnd");    break;
    case OdDb::kOsModeMid     : retVal = OD_T("kOsModeMid");    break;
    case OdDb::kOsModeCen     : retVal = OD_T("kOsModeCen");    break;
    case OdDb::kOsModeNode    : retVal = OD_T("kOsModeNode");   break;
    case OdDb::kOsModeQuad    : retVal = OD_T("kOsModeQuad");   break;
    case OdDb::kOsModeIns     : retVal = OD_T("kOsModeIns");    break;
    case OdDb::kOsModePerp    : retVal = OD_T("kOsModePerp");   break;
    case OdDb::kOsModeTan     : retVal = OD_T("kOsModeTan");    break;
    case OdDb::kOsModeNear    : retVal = OD_T("kOsModeNear");   break;
    case OdDb::kOsModeApint   : retVal = OD_T("kOsModeApint");  break;
    case OdDb::kOsModePar     : retVal = OD_T("kOsModePar");    break;
    case OdDb::kOsModeStart   : retVal = OD_T("kOsModeStart");  break;
    case OdDb::kOsModeIntersec: retVal = OD_T("kOsModeIntersec");break;
  }
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdDb::OsnapMode string             */
/************************************************************************/
OdString toString(const OdDb::SubentType val)
{
  OdString retVal(OD_T("???"));  
  switch (val) 
  {
    case OdDb::kNullSubentType     : retVal = OD_T("kNullSubentType");    break;
    case OdDb::kFaceSubentType     : retVal = OD_T("kFaceSubentType");    break;
    case OdDb::kEdgeSubentType     : retVal = OD_T("kEdgeSubentType");    break;
    case OdDb::kVertexSubentType   : retVal = OD_T("kVertexSubentType");  break;
    case OdDb::kMlineSubentCache   : retVal = OD_T("kMlineSubentCache");  break;
    case OdDb::kClassSubentType    : retVal = OD_T("kClassSubentType");   break;
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
/* Convert the specified value to an string                             */
/************************************************************************/
OdString toString(const OdAve::MaterialType val)
{ 
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdAve::kStandard  : retVal = OD_T("kStandard");    break; 
    case OdAve::kGranite   : retVal = OD_T("kGranite");     break; 
    case OdAve::kMarble    : retVal = OD_T("kMarble");      break; 
    case OdAve::kWood      : retVal = OD_T("kWood");        break; 
  }
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdGsLight string                   */
/************************************************************************/
OdString toString(const OdGsLight::LightType val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdGsLight::kPointLight   : retVal = OD_T("kPointLight");    break; 
    case OdGsLight::kDistantLight : retVal = OD_T("kDistantLight");  break; 
    case OdGsLight::kSpotlight    : retVal = OD_T("kSpotlight");     break; 
  }  
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdGiMapper::AutoTransform string   */
/************************************************************************/
OdString toString(const OdGiMapper::AutoTransform val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdGiMapper::kInheritAutoTransform : retVal = OD_T("kInheritAutoTransform"); break;
    case OdGiMapper::kNone                 : retVal = OD_T("kNone");                 break;
    case OdGiMapper::kObject               : retVal = OD_T("kObject");               break;
    case OdGiMapper::kModel                : retVal = OD_T("kModel");                break;
  }
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdGiMapper::Projection string      */
/************************************************************************/
OdString toString(const OdGiMapper::Projection val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdGiMapper::kInheritProjection : retVal = OD_T("kInheritProjection"); break;
    case OdGiMapper::kPlanar            : retVal = OD_T("kPlanar");            break;
    case OdGiMapper::kBox               : retVal = OD_T("kBox");               break;
    case OdGiMapper::kCylinder          : retVal = OD_T("kCylinder");          break;
    case OdGiMapper::kSphere            : retVal = OD_T("kSphere");            break;
    case OdGiMapper::kDgnParametric     : retVal = OD_T("kDgnParametric");     break;
    case OdGiMapper::kDgnPlanar         : retVal = OD_T("kDgnPlanar");         break;
    case OdGiMapper::kDgnCylinder       : retVal = OD_T("kDgnCylinder");       break;
    case OdGiMapper::kDgnCylinderCapped : retVal = OD_T("kDgnCylinderCapped"); break;
    case OdGiMapper::kDgnSphere         : retVal = OD_T("kDgnSphere");         break;
  }
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdGiMapper::Tiling                 */
/************************************************************************/
OdString toString(const OdGiMapper::Tiling val)
{
  OdString retVal(OD_T("???"));
  switch (val) 
  {
    case OdGiMapper::kInheritTiling : retVal = OD_T("kInheritTiling"); break;
    case OdGiMapper::kTile          : retVal = OD_T("kTile");          break;
    case OdGiMapper::kCrop          : retVal = OD_T("kCrop");          break;
    case OdGiMapper::kClamp         : retVal = OD_T("kClamp");         break;
    case OdGiMapper::kMirror        : retVal = OD_T("kMirror");        break;
  }
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdGiSubEntityTraits::ShadowFlags   */
/************************************************************************/
OdString toString(const OdGiSubEntityTraits::ShadowFlags val)
{
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdGiSubEntityTraits::kShadowsCastAndReceive : retVal = OD_T("kShadowsCastAndReceive"); break;
    case OdGiSubEntityTraits::kShadowsDoesNotCast    : retVal = OD_T("kShadowsDoesNotCast");    break;
    case OdGiSubEntityTraits::kShadowsDoesNotReceive : retVal = OD_T("kShadowsDoesNotReceive"); break;
    case OdGiSubEntityTraits::kShadowsIgnore         : retVal = OD_T("kShadowsIgnore");         break;
  }
  return retVal;
}
/************************************************************************/
/* Convert the specified value to an OdGiSubEntityTraits::SelectionFlags*/
/************************************************************************/
OdString toString(const OdGiSubEntityTraits::SelectionFlags val)
{
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdGiSubEntityTraits::kNoSelectionFlags     : retVal = OD_T("kNoSelectionFlags");      break;
    case OdGiSubEntityTraits::kSelectionIgnore      : retVal = OD_T("kSelectionIgnore");       break;
    case OdGiSubEntityTraits::kHighlightingGeometry : retVal = OD_T("kHighlightingGeometry");  break;
  }
  return retVal;
}
/**************************************************************************/
/* Convert the specified value to an OdDbMLeaderStyle::ContentType string */
/**************************************************************************/
OdString toString(const OdDbMLeaderStyle::ContentType val)
{
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdDbMLeaderStyle::kNoneContent : retVal = OD_T("kNoneContent"); break;
    case OdDbMLeaderStyle::kBlockContent : retVal = OD_T("kBlockContent"); break;
    case OdDbMLeaderStyle::kMTextContent : retVal = OD_T("kMTextContent"); break;
    case OdDbMLeaderStyle::kToleranceContent : retVal = OD_T("kToleranceContent"); break;
  }
  return retVal;
}
/***********************************************************************************/
/* Convert the specified value to an OdDbMLeaderStyle::DrawMLeaderOrderType string */
/***********************************************************************************/
OdString toString(const OdDbMLeaderStyle::DrawMLeaderOrderType val)
{
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdDbMLeaderStyle::kDrawContentFirst : retVal = OD_T("kDrawContentFirst"); break;
    case OdDbMLeaderStyle::kDrawLeaderFirst : retVal = OD_T("kDrawLeaderFirst"); break;
  }
  return retVal;
}
/**********************************************************************************/
/* Convert the specified value to an OdDbMLeaderStyle::DrawLeaderOrderType string */
/**********************************************************************************/
OdString toString(const OdDbMLeaderStyle::DrawLeaderOrderType val)
{
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdDbMLeaderStyle::kDrawLeaderHeadFirst : retVal = OD_T("kDrawLeaderHeadFirst"); break;
    case OdDbMLeaderStyle::kDrawLeaderTailFirst : retVal = OD_T("kDrawLeaderTailFirst"); break;
  }
  return retVal;
}
/*******************************************************************************/
/* Convert the specified value to an OdDbMLeaderStyle::SegmentAngleType string */
/*******************************************************************************/
OdString toString(const OdDbMLeaderStyle::SegmentAngleType val)
{
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdDbMLeaderStyle::kAny : retVal = OD_T("kAny"); break;
    case OdDbMLeaderStyle::k15 : retVal = OD_T("k15"); break;
    case OdDbMLeaderStyle::k30 : retVal = OD_T("k30"); break;
    case OdDbMLeaderStyle::k45 : retVal = OD_T("k45"); break;
    case OdDbMLeaderStyle::k60 : retVal = OD_T("k60"); break;
    case OdDbMLeaderStyle::k90 : retVal = OD_T("k90"); break;
    case OdDbMLeaderStyle::kHorz : retVal = OD_T("kHorz"); break;
  }
  return retVal;
}
/*************************************************************************/
/* Convert the specified value to an OdDbMLeaderStyle::LeaderType string */
/*************************************************************************/
OdString toString(const OdDbMLeaderStyle::LeaderType val)
{
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdDbMLeaderStyle::kInVisibleLeader : retVal = OD_T("kInVisibleLeader"); break;
    case OdDbMLeaderStyle::kStraightLeader : retVal = OD_T("kStraightLeader"); break;
    case OdDbMLeaderStyle::kSplineLeader : retVal = OD_T("kSplineLeader"); break;
  }
  return retVal;
}
/********************************************************************************/
/* Convert the specified value to an OdDbMLeaderStyle::TextAlignmentType string */
/********************************************************************************/
OdString toString(const OdDbMLeaderStyle::TextAlignmentType val)
{
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdDbMLeaderStyle::kLeftAlignment : retVal = OD_T("kLeftAlignment"); break;
    case OdDbMLeaderStyle::kCenterAlignment : retVal = OD_T("kCenterAlignment"); break;
    case OdDbMLeaderStyle::kRightAlignment : retVal = OD_T("kRightAlignment"); break;
  }
  return retVal;
}
/****************************************************************************/
/* Convert the specified value to an OdDbMLeaderStyle::TextAngleType string */
/****************************************************************************/
OdString toString(const OdDbMLeaderStyle::TextAngleType val)
{
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdDbMLeaderStyle::kInsertAngle : retVal = OD_T("kInsertAngle"); break;
    case OdDbMLeaderStyle::kHorizontalAngle : retVal = OD_T("kHorizontalAngle"); break;
    case OdDbMLeaderStyle::kAlwaysRightReadingAngle : retVal = OD_T("kAlwaysRightReadingAngle"); break;
  }
  return retVal;
}
/*********************************************************************************/
/* Convert the specified value to an OdDbMLeaderStyle::TextAttachmentType string */
/*********************************************************************************/
OdString toString(const OdDbMLeaderStyle::TextAttachmentType val)
{
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdDbMLeaderStyle::kAttachmentTopOfTop : retVal = OD_T("kAttachmentTopOfTop"); break;
    case OdDbMLeaderStyle::kAttachmentMiddleOfTop : retVal = OD_T("kAttachmentMiddleOfTop"); break;
    case OdDbMLeaderStyle::kAttachmentMiddle : retVal = OD_T("kAttachmentMiddle"); break;
    case OdDbMLeaderStyle::kAttachmentMiddleOfBottom : retVal = OD_T("kAttachmentMiddleOfBottom"); break;
    case OdDbMLeaderStyle::kAttachmentBottomOfBottom : retVal = OD_T("kAttachmentBottomOfBottom"); break;
    case OdDbMLeaderStyle::kAttachmentBottomLine : retVal = OD_T("kAttachmentBottomLine"); break;
    case OdDbMLeaderStyle::kAttachmentBottomOfTopLine : retVal = OD_T("kAttachmentBottomOfTopLine"); break;
    case OdDbMLeaderStyle::kAttachmentBottomOfTop : retVal = OD_T("kAttachmentBottomOfTop"); break;
    case OdDbMLeaderStyle::kAttachmentAllLine : retVal = OD_T("kAttachmentAllLine"); break;
    case OdDbMLeaderStyle::kAttachmentCenter : retVal = OD_T("kAttachmentCenter"); break;
    case OdDbMLeaderStyle::kAttachmentLinedCenter : retVal = OD_T("kAttachmentLinedCenter"); break;
  }
  return retVal;
}
/**********************************************************************************/
/* Convert the specified value to an OdDbMLeaderStyle::BlockConnectionType string */
/**********************************************************************************/
OdString toString(const OdDbMLeaderStyle::BlockConnectionType val)
{
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdDbMLeaderStyle::kConnectExtents : retVal = OD_T("kConnectExtents"); break;
    case OdDbMLeaderStyle::kConnectBase : retVal = OD_T("kConnectBase"); break;
  }
  return retVal;
}
/**************************************************************************************/
/* Convert the specified value to an OdDbMLeaderStyle::TextAttachmentDirection string */
/**************************************************************************************/
OdString toString(const OdDbMLeaderStyle::TextAttachmentDirection val)
{
  OdString retVal(OD_T("???"));
  switch (val)
  {
    case OdDbMLeaderStyle::kAttachmentHorizontal : retVal = OD_T("kAttachmentHorizontal"); break;
    case OdDbMLeaderStyle::kAttachmentVertical : retVal = OD_T("kAttachmentVertical"); break;
  }
  return retVal;
}
