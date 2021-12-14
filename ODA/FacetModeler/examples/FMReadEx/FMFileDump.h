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

#pragma once

#include "StringArray.h"
#include "ExPrintConsole.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "FMContour2D.h"
#include "FMProfile2D.h"
#include "Modeler/FMMdlFace.h"
#include "Modeler/FMMdlIterators.h"
#include "FMDataSerialize.h"

/** \details
   Prints object data to the console with specified indent string.

   \param object [in] Object to print properties from.
   \param indent [in] Indent to add at the beginning of each line.
*/
static void printToConsole(const OdGePoint2d& object, const OdString& indent = OdString::kEmpty)
{
  odPrintConsoleString(L"%sPoint2d:%f,%f\n", indent.c_str(), object.x, object.y);
};

/** \details
   Prints object data to the console with specified indent string.

   \param object [in] Object to print properties from.
   \param indent [in] Indent to add at the beginning of each line.
*/
static void printToConsole(const OdGePoint3d& object, const OdString& indent = OdString::kEmpty)
{
  odPrintConsoleString(L"%sPoint3d:%f,%f,%f\n", indent.c_str(), object.x, object.y, object.z);
}

/** \details
   Prints object data to the console with specified indent string.

   \param object [in] Object to print properties from.
   \param indent [in] Indent to add at the beginning of each line.
*/
static void printToConsole(const FacetModeler::Contour2D& object, const OdString& indent = OdString::kEmpty)
{
  odPrintConsoleString(L"%sContour2d with:\n", indent.c_str());
  odPrintConsoleString(L"%scontour area: %f\n", indent.c_str(), object.signedArea());
  odPrintConsoleString(L"%scontour length: %f\n", indent.c_str(), object.length());
  odPrintConsoleString(L"%ssegments count: %d\n", indent.c_str(), object.numSegments());
}

/** \details
   Prints object data to the console with specified indent string.

   \param object [in] Object to print properties from.
   \param indent [in] Indent to add at the beginning of each line.
*/
static void printToConsole(const FacetModeler::Profile2D& object, const OdString& indent = OdString::kEmpty)
{
  odPrintConsoleString(L"%sProfile2d with:\n", indent.c_str());
  odPrintConsoleString(L"%sprofile signed area: %f\n", indent.c_str(), object.signedArea());
  odPrintConsoleString(L"%sprofile perimeter: %f\n", indent.c_str(), object.perimeter());
  odPrintConsoleString(L"%scontours count: %d\n", indent.c_str(), object.size());
}

/** \details
   Prints object data to the console with specified indent string.

   \param object [in] Object to print properties from.
   \param indent [in] Indent to add at the beginning of each line.
*/
static void printToConsole(const FacetModeler::Face& object, const OdString& indent = OdString::kEmpty)
{
  odPrintConsoleString(L"%sFace with:\n", indent.c_str());
  odPrintConsoleString(L"%sface area: %f\n", indent.c_str(), object.area());

  OdString depperIndent = indent + L"  ";
  FacetModeler::EdgeFaceIterator iterator(&object);
  for (; !iterator.done(); iterator.next())
  {
    printToConsole(iterator.get()->vertex()->point(), depperIndent);
  }
}

/** \details
   Prints object data to the console with specified indent string.

   \param object [in] Object to print properties from.
   \param indent [in] Indent to add at the beginning of each line.
*/
static void printToConsole(const FacetModeler::Body& object, const OdString& indent = OdString::kEmpty)
{
  odPrintConsoleString(L"%sBody with:\n", indent.c_str());
  odPrintConsoleString(L"%sbody volume: %f\n", indent.c_str(), object.volume());
  odPrintConsoleString(L"%sfaces count: %d\n", indent.c_str(), object.faceCount());
  odPrintConsoleString(L"%sedges count: %d\n", indent.c_str(), object.countEdges());

  OdString depperIndent = indent + L"  ";
  FacetModeler::FaceIterator iterator(&object);
  for (; !iterator.done(); iterator.next())
  {
    printToConsole(*iterator.get(), depperIndent);
  }
}

/** \details
   Reads Facet Modeler entities which names specified by args array from the stream.

   \param pStream [in] Stream to read from.
   \param args    [in] Arguments array.
   \returns true if the read was successful, false - otherwise.
*/
static bool readFromStream(OdStreamBuf* pStream, const OdStringArray& args)
{
  bool bStatus = true;
  FacetModeler::BinaryStream fmbs;
  bStatus = fmbs.Open(pStream);
  if (!bStatus) return bStatus;

  OdUInt64 iLength = pStream->length(), iCurPos;
  FacetModeler::BinaryStream::Version iVer = fmbs.getVersion();

  odPrintConsoleString(L"File stream length: %d\n", iLength);
  odPrintConsoleString(L"File version number: %d\n", iVer);

  iCurPos = pStream->tell();
  odPrintConsoleString(L"Current position: %d, %d%%\n", iCurPos, 100 * iCurPos / iLength);

  for (unsigned int i = 2; i < args.length(); i++)
  {
    if (args[i].iCompare(L"body") == 0)
    {
      FacetModeler::Body object;
      fmbs.Read(object);
      printToConsole(object, OdString(OD_T("  ")));
    }
    else if (args[i].iCompare(L"face") == 0)
    {
      FacetModeler::Body object;
      fmbs.ReadFaceToBody(object);
      printToConsole(*object.faceList());
    }
    else if (args[i].iCompare(L"contour2d") == 0)
    {
      FacetModeler::Contour2D object;
      fmbs.Read(object);
      printToConsole(object, L"  ");
    }
    else if (args[i].iCompare(L"profile2d") == 0)
    {
      FacetModeler::Profile2D object;
      fmbs.Read(object);
      printToConsole(object, L"  ");
    }
    else if (args[i].iCompare(L"point2d") == 0)
    {
      OdGePoint2d object;
      fmbs.Read(object);
      printToConsole(object, L"  ");
    }
    else if (args[i].iCompare(L"point3d") == 0)
    {
      OdGePoint3d object;
      fmbs.Read(object);
      printToConsole(object, L"  ");
    }
    else
    {
      return false;
    }

    OdUInt64 iOldPos = iCurPos;
    iCurPos = pStream->tell();
    odPrintConsoleString(L"Object Size with marker is: %d bytes\n", iCurPos - iOldPos);
    odPrintConsoleString(L"Current position: %d, %d%%\n", iCurPos, 100 * iCurPos / iLength);

    if (pStream->isEof())
    {
      break;
    }
  }
  return bStatus;
}

/** \details
   Reads Facet Modeler entities which names specified by args array from the file.
  
   \param filename [in] Name of the file.
   \param args     [in] Arguments array.
   \returns true if the read was successful, false - otherwise.
*/
static bool readFromFile(const OdStringArray& args)
{
  OdStreamBufPtr pStream = odrxSystemServices()->createFile(args[1], Oda::kFileRead, Oda::kShareDenyNo, Oda::kOpenExisting);

  if (!pStream.isNull())
  {
    return readFromStream(pStream, args);
  }
  return false;
}
