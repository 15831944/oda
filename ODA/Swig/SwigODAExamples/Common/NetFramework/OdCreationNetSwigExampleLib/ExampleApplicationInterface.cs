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
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Teigha.Core;

namespace OdCreationNetSwigExampleLib
{
  public abstract class ExampleApplicationInterface<TExampleModeEnum>
    where TExampleModeEnum : struct
  {
    protected OdDbBaseHostAppServices hostApp;
    protected String[] app_args;
    public ExampleApplicationInterface(OdDbBaseHostAppServices hostApp, String[] app_args)
    {
      this.hostApp = hostApp;
      this.app_args = app_args;
    }

    protected abstract Dictionary<TExampleModeEnum, Type> ModeToExampleClass { get; }

    protected TExampleModeEnum GetModeFromExampleType(Type exampleType)
    {
      TExampleModeEnum mode = default(TExampleModeEnum);

      if (!ModeToExampleClass.ContainsValue(exampleType))
        return mode;

      foreach (TExampleModeEnum key in ModeToExampleClass.Keys)
      {
        if (ModeToExampleClass[key] == exampleType)
        {
          mode = key;
          break;
        }
      }
      return mode;
    }

    public void ShowCorrectInputParams()
    {
      Console.WriteLine("Incorrect input parameters!");
      Console.WriteLine("  Usage:");
      int indent = 4;
      foreach (var ex_mode_instance_type in ModeToExampleClass)
      {
        TExampleModeEnum mode = ex_mode_instance_type.Key;
        Example ex = Activator.CreateInstance(ex_mode_instance_type.Value) as Example;
        Console.WriteLine(new String(' ', indent) + String.Format("CMD: \"{0}\" {1}   | Description: {2}", Enum.GetName(typeof(TExampleModeEnum), mode), ex.GetUsageInfo(), ex.Description));
      }
    }

    public virtual Example CreateExampleInstance()
    {
      if (app_args.Length < 1)
        return null;
      String str_mode = app_args[0];
      TExampleModeEnum mode = SolveExampleMode(str_mode);
      Example inst = CreateExample(mode);
      InitExample(inst);
      return inst;
    }

    private static TExampleModeEnum SolveExampleMode(String from)
    {
      TExampleModeEnum res = default(TExampleModeEnum);
      try
      {
        Enum.TryParse<TExampleModeEnum>(from, out res);
      }
      catch (Exception)
      { }
      return res;
    }

    private Example CreateExample(TExampleModeEnum mode)
    {
      if (!ModeToExampleClass.ContainsKey(mode))
        return null;
      return Activator.CreateInstance(ModeToExampleClass[mode]) as Example;
    }

    protected virtual bool InitExample(Example instance)
    {
      if (instance == null)
        return false;
      instance.HostApp = hostApp;
      return true;
    }

  }
}
