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

using System;
using System.Collections.Generic;
using System.Text;
using Teigha.Core;
using Teigha.TD;
using System.IO;
using Microsoft.Win32;

namespace OdReadExSwigMgd
{
  class OdExStringHelpers
  {
    public static String toString(OdDbObjectId val)
    {
      if (val.isNull())
      {
        return "Null";
      }

      if (val.isErased())
      {
        return "Erased";
      }

      /**********************************************************************/
      /* Open the object                                                    */
      /**********************************************************************/
      OdDbObject pObject = val.safeOpenObject();

      /**********************************************************************/
      /* Return the name of an OdDbSymbolTableRecord                        */
      /**********************************************************************/
      if (pObject is OdDbSymbolTableRecord)
      {
        OdDbSymbolTableRecord pSTR = (OdDbSymbolTableRecord)pObject;
        return pSTR.getName();
      }

      /**********************************************************************/
      /* Return the name of an OdDbMlineStyle                               */
      /**********************************************************************/
      if (pObject is OdDbMlineStyle)
      {
        OdDbMlineStyle pStyle = (OdDbMlineStyle)pObject;
        return pStyle.name();
      }

      /**********************************************************************/
      /* Return the name of a PlotStyle                                      */
      /**********************************************************************/
      if (pObject is OdDbPlaceHolder)
      {
        OdDbDictionary pDictionary = (OdDbDictionary)val.database().getPlotStyleNameDictionaryId().safeOpenObject();
        String plotStyleName = pDictionary.nameAt(val);
        return plotStyleName;
      }

      /**********************************************************************/
      /* Return the name of an OdDbMaterial                                 */
      /**********************************************************************/
      if (pObject is OdDbMaterial)
      {
        OdDbMaterial pMaterial = (OdDbMaterial)pObject;
        return pMaterial.name();
      }

      /**********************************************************************/
      /* We don't know what it is, so return the description of the object  */
      /* object specified by the ObjectId                                   */
      /**********************************************************************/
      return pObject.isA().name();
    }

    public static String toDegreeString(double val)
    {
      return (val * 180.0 / Math.PI).ToString() + "\u00B0";
    }

    public static String shortenPath(String path)
    {
      return shortenPath(path, 40);
    }

    public static String shortenPath(String path, int maxPath)
    {
      /**********************************************************************/
      /* If the path fits, just return it                                   */
      /**********************************************************************/
      if (path.Length <= maxPath)
      {
        return path;
      }
      /**********************************************************************/
      /* If there's no backslash, just truncate the path                    */
      /**********************************************************************/
      int lastBackslash = path.LastIndexOf('\\');
      if (lastBackslash < 0)
      {
        return path.Substring(0, maxPath - 3) + "...";
      }

      /**********************************************************************/
      /* Shorten the front of the path                                      */
      /**********************************************************************/
      int fromLeft = (lastBackslash - 3) - (path.Length - maxPath);
      // (12 - 3) - (19 - 10) = 9 - 9 = 0 
      if ((lastBackslash <= 3) || (fromLeft < 1))
      {
        path = "..." + path.Substring(lastBackslash);
      }
      else
      {
        path = path.Substring(0, fromLeft) + "..." + path.Substring(lastBackslash);
      }

      /**********************************************************************/
      /* Truncate the path                                                  */
      /**********************************************************************/
      if (path.Length > maxPath)
      {
        path = path.Substring(0, maxPath - 3) + "...";
      }

      return path;
    }

    /************************************************************************/
    /* Convert the specified value to a LoopType string                     */
    /************************************************************************/
    public static String toLooptypeString(OdDbHatch.HatchLoopType loopType)
    {
      String retVal = "";
      if ((loopType & OdDbHatch.HatchLoopType.kExternal) != 0)
        retVal += " | kExternal";

      if ((loopType & OdDbHatch.HatchLoopType.kPolyline) != 0)
        retVal += " | kPolyline";

      if ((loopType & OdDbHatch.HatchLoopType.kDerived) != 0)
        retVal += " | kDerived";

      if ((loopType & OdDbHatch.HatchLoopType.kTextbox) != 0)
        retVal += " | kTextbox";

      if ((loopType & OdDbHatch.HatchLoopType.kOutermost) != 0)
        retVal += " | kOutermost";

      if ((loopType & OdDbHatch.HatchLoopType.kNotClosed) != 0)
        retVal += " | kNotClosed";

      if ((loopType & OdDbHatch.HatchLoopType.kSelfIntersecting) != 0)
        retVal += " | kSelfIntersecting";

      if ((loopType & OdDbHatch.HatchLoopType.kTextIsland) != 0)
        retVal += " | kTextIsland";

      if ((loopType & OdDbHatch.HatchLoopType.kDuplicate) != 0)
        retVal += " | kDuplicate";

      return retVal == "" ? "kDefault" : retVal.Substring(3);
    }

    /************************************************************************/
    /* Convert the specified value to an ArcSymbolType string               */
    /************************************************************************/
    public static String toArcSymbolTypeString(int val)
    {
      String retVal = "???";
      switch (val)
      {
        case 0: retVal = "Precedes text"; break;
        case 1: retVal = "Above text"; break;
        case 2: retVal = "None"; break;
      }
      return retVal;
    }
  }

}