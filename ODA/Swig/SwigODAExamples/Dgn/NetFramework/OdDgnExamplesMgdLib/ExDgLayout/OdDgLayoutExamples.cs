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
using Teigha.Core;
using Teigha.TG;

namespace OdDgnExamplesMgdLib.ExDgLayout
{
  public static class OdDgLayoutExamples
  {
    public static void FillLayoutList(OdDgDatabase db, OdStringArray aLayouts, bool bAllLayouts)
    {
      if (db == null)
        return;

      if (bAllLayouts)
      {
        OdDgModelTable pModelTable = db.getModelTable();
        if (pModelTable != null)
        {
          OdDgElementIterator pIter = pModelTable.createIterator();
          for (; !pIter.done(); pIter.step())
          {
            OdDgModel pModel = OdDgModel.cast(pIter.item().openObject());
            if (null != pModel)
            {
              aLayouts.Add(pModel.getName());
            }
          }
        }
      }
      else
      {
        OdDgModel pModel = null;
        OdDgElementId idModel = db.getActiveModelId();

        if (!idModel.isNull())
          pModel = OdDgModel.cast(idModel.openObject(Teigha.TG.OpenMode.kForRead));

        if (pModel != null)
        {
          String name = pModel.getName();

          OdDgElementId idViewGroup = pModel.getViewGroup(true);

          if (!idViewGroup.isNull())
          {
            OdDgViewGroup pViewGroup = OdDgViewGroup.cast(idViewGroup.openObject(Teigha.TG.OpenMode.kForRead));

            if (pViewGroup != null)
            {
              OdDgElementIterator pViewIter = pViewGroup.createIterator();

              for (; !pViewIter.done(); pViewIter.step())
              {
                OdDgView pView = OdDgView.cast(pViewIter.item().openObject(Teigha.TG.OpenMode.kForRead));

                if (pView != null)
                {
                  if (pView.getVisibleFlag())
                  {
                    String strLayoutAndView = String.Format("${0}", pView.getIndex());
                    strLayoutAndView = name + strLayoutAndView;
                    aLayouts.Add(strLayoutAndView);
                  }
                }
              }
            }
          }
        }
      }
    }
  }

}