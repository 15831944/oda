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

namespace OdDrawingsExamplesMgdLib
{
  public static class OdExLyLayerFilterManagerDumper
  {
    public static void Dump(OdDbDatabase db)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      try
      {
        OdLyLayerFilterManager layerFilterManager = TD_Db.odlyGetLayerFilterManager(db);
        OdLyLayerFilter root, current;
        layerFilterManager.getFilters(out root, out current);
        String dump = Dump(root);
        Console.WriteLine(dump);
      }
      catch (OdError err)
      {
        Console.WriteLine(String.Format("Error: {0}", err.description()));
      }
      catch (Exception err)
      {
        Console.WriteLine(String.Format("Error: {0}", err.Message));
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }
    }

    private static String Dump(OdLyLayerFilter filterNode)
    {
      StringBuilder sBuilder = new StringBuilder();
      sBuilder.AppendLine(DumpFilterProperties(filterNode));
      foreach (OdLyLayerFilter nestedFilter in filterNode.getNestedFilters())
        sBuilder.AppendLine(Dump(nestedFilter)).AppendLine().AppendLine();
      return sBuilder.ToString();
    }

    private static String DumpFilterProperties(OdLyLayerFilter filterNode)
    {
      StringBuilder sBuilder = new StringBuilder();
      sBuilder.AppendLine(String.Format("Filter Name:\"{0}\"", filterNode.name()));
      sBuilder.AppendLine(String.Format("  isIdFilter:\"{0}\"", filterNode.isIdFilter()));
      sBuilder.AppendLine(String.Format("  isProxy:\"{0}\"", filterNode.isProxy()));
      sBuilder.AppendLine(String.Format("  hasParent:\"{0}\"", filterNode.parent() != null));
      sBuilder.AppendLine(String.Format("  allowDelete:\"{0}\"", filterNode.allowDelete()));
      sBuilder.AppendLine(String.Format("  allowNested:\"{0}\"", filterNode.allowNested()));
      sBuilder.AppendLine(String.Format("  allowRename:\"{0}\"", filterNode.allowRename()));
      sBuilder.AppendLine(String.Format("  dynamicallyGenerated:\"{0}\"", filterNode.dynamicallyGenerated()));
      sBuilder.AppendLine(String.Format("  generateNested:\"{0}\"", filterNode.generateNested().ToString()));
      sBuilder.AppendLine(String.Format("  filterExpression (as String):\"{0}\"", filterNode.filterExpression()));
      sBuilder.AppendLine(String.Format("  filterExpressionTree:\"{0}\"", Dump(filterNode.filterExpressionTree())));
      return sBuilder.ToString();
    }

    private static String Dump(OdLyBoolExpr boolExpr)
    {
      if (boolExpr == null)
        return "null";

      StringBuilder sBuilder = new StringBuilder();

      OdLyAndExprArray andExprs = boolExpr.getAndExprs();
      for (int idxAndExpr = 0; idxAndExpr < andExprs.Count; idxAndExpr++)
      {
        OdLyAndExpr andExpr = andExprs[idxAndExpr];

        OdLyRelExprArray relExprs = andExpr.getRelExprs();
        for (int idxRelExprs = 0; idxRelExprs < relExprs.Count; idxRelExprs++)
        {
          OdLyRelExpr relExpr = relExprs[idxRelExprs];
          sBuilder.Append(String.Format("{0} == \"{1}\"", relExpr.getVariable(), relExpr.getConstant()));

          if (idxRelExprs > 1 && idxRelExprs + 1 != relExprs.Count)
            sBuilder.Append(" AND ");
        }

        if (idxAndExpr > 1 && idxAndExpr + 1 != relExprs.Count)
          sBuilder.Append(" AND ");
      }

      return sBuilder.ToString();
    }
  }

}