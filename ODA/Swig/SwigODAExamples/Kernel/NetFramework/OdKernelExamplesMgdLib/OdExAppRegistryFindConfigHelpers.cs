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

using Microsoft.Win32;
using System;

namespace OdKernelExamplesMgdLib
{
  public class OdExAppRegistryFindConfigHelpers
  {
    public static String FindConfigPath(String configType)
    {
      String subkey = GetRegistryAcadProfilesKey();
      if (subkey.Length > 0)
      {
        subkey += String.Format("\\General");
        String searchPath;
        if (GetRegistryString(Registry.CurrentUser, subkey, configType, out searchPath))
          return searchPath;
      }
      return String.Format("");
    }

    private static bool GetRegistryString(RegistryKey rKey, String subkey, String name, out String value)
    {
      bool rv = false;
      object objData = null;

      RegistryKey regKey;
      regKey = rKey.OpenSubKey(subkey);
      if (regKey != null)
      {
        objData = regKey.GetValue(name);
        if (objData != null)
        {
          rv = true;
        }
        regKey.Close();
      }
      if (rv)
        value = objData.ToString();
      else
        value = String.Format("");

      rKey.Close();
      return rv;
    }

    public static String GetRegistryAVEMAPSFromProfile()
    {
      String subkey = GetRegistryAcadProfilesKey();
      if (subkey.Length > 0)
      {
        subkey += String.Format("\\General");
        // get the value for the ACAD entry in the registry
        String tmp;
        if (GetRegistryString(Registry.CurrentUser, subkey, String.Format("AVEMAPS"), out tmp))
          return tmp;
      }
      return String.Format("");
    }

    private static String GetRegistryAcadProfilesKey()
    {
      String subkey = String.Format("SOFTWARE\\Autodesk\\AutoCAD");
      String tmp;

      if (!GetRegistryString(Registry.CurrentUser, subkey, String.Format("CurVer"), out tmp))
        return String.Format("");
      subkey += String.Format("\\{0}", tmp);

      if (!GetRegistryString(Registry.CurrentUser, subkey, String.Format("CurVer"), out tmp))
        return String.Format("");
      subkey += String.Format("\\{0}\\Profiles", tmp);

      if (!GetRegistryString(Registry.CurrentUser, subkey, String.Format(""), out tmp))
        return String.Format("");
      subkey += String.Format("\\{0}", tmp);
      return subkey;
    }
    public static String GetRegistryAcadLocation()
    {
      String subkey = String.Format("SOFTWARE\\Autodesk\\AutoCAD");
      String tmp;

      if (!GetRegistryString(Registry.CurrentUser, subkey, String.Format("CurVer"), out tmp))
        return String.Format("");
      subkey += String.Format("\\{0}", tmp);

      if (!GetRegistryString(Registry.CurrentUser, subkey, String.Format("CurVer"), out tmp))
        return String.Format("");
      subkey += String.Format("\\{0}", tmp);

      if (!GetRegistryString(Registry.CurrentUser, subkey, String.Format(""), out tmp))
        return String.Format("");
      return tmp;
    }

    public static String FindConfigFile(String configType, String file)
    {
      String searchPath = FindConfigPath(configType);
      if (searchPath.Length > 0)
      {
        searchPath = String.Format("{0}\\{1}", searchPath, file);
        if (System.IO.File.Exists(searchPath))
          return searchPath;
      }
      return String.Format("");
    }

    public static String GetRegistryACADFromProfile()
    {
      String subkey = GetRegistryAcadProfilesKey();
      if (subkey.Length > 0)
      {
        subkey += String.Format("\\General");
        // get the value for the ACAD entry in the registry
        String tmp;
        if (GetRegistryString(Registry.CurrentUser, subkey, String.Format("ACAD"), out tmp))
          return tmp;
      }
      return String.Format("");
    }
  }
}