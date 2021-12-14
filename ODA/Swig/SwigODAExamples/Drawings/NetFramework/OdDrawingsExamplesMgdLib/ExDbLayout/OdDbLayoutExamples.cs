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
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib.ExDbLayout
{
  public static class OdDbLayoutExamples
  {
    public static void FillLayoutList(OdDbDatabase db, OdStringArray aLayouts, bool bAllLayouts, bool withEmptyLayouts)
    {
      if (db == null)
        return;

      if (bAllLayouts)
      {
        OdDbDictionary pLayoutDict = (OdDbDictionary)db.getLayoutDictionaryId().safeOpenObject();
        OdDbDictionaryIterator pIter = pLayoutDict.newIterator();
        for (; !pIter.done(); pIter.next())
        {
          OdDbObjectId id = pIter.objectId();
          OdDbLayout pLayout = OdDbLayout.cast(id.safeOpenObject());
          if (pLayout == null)
          {
            continue;
          }
          string sName = pLayout.getLayoutName();
          if (pLayout.getBlockTableRecordId() == db.getModelSpaceId())
          {
            aLayouts.Insert(0, sName);
          }
          else
          {
            OdDbBlockTableRecord pRec = (OdDbBlockTableRecord)pLayout.getBlockTableRecordId().safeOpenObject();
            OdDbObjectIterator pIt = pRec.newIterator();
            bool bEmpty = pIt.done();
            if (!bEmpty || withEmptyLayouts)
            {
              aLayouts.Add(sName);
            }
          }
        }
      }
      else
      {
        OdDbBlockTableRecord pBlock = (OdDbBlockTableRecord)db.getActiveLayoutBTRId().safeOpenObject();
        OdDbLayout pLayout = (OdDbLayout)pBlock.getLayoutId().safeOpenObject();
        String layoutName = pLayout.getLayoutName();
        aLayouts.Add(layoutName);
      }
    }


    public static void fillListForDb(OdDbDatabase pDb, OdStringArray layouts, OdRxObjectPtrArray databases, bool bMultipleDatabases, bool withEmptyLayouts)
    {
      OdDbDictionary pLayoutDict = pDb.getLayoutDictionaryId().safeOpenObject() as OdDbDictionary;
      OdDbDictionaryIterator pIt = pLayoutDict.newIterator();
      int index = layouts.Count;

      while (!pIt.done())
      {
        OdDbLayout pLayout = pIt.getObject() as OdDbLayout;
        String sName = pLayout.getLayoutName();

        if (pLayout.getBlockTableRecordId().IsEqual(pDb.getModelSpaceId()))
        {
          layouts.Insert(index, sName);
          if (bMultipleDatabases)
          {
            databases.Insert(index, pDb);
          }
        }
        else
        {
          OdDbBlockTableRecord pRec = pLayout.getBlockTableRecordId().safeOpenObject() as OdDbBlockTableRecord;
          OdDbObjectIterator pIt2 = pRec.newIterator();
          bool bEmpty = pIt2.done();
          if (!bEmpty || withEmptyLayouts)
          {
            layouts.Add(sName);
            if (bMultipleDatabases)
            {
              databases.Add(pDb);
            }
          }
        }
        pIt.next();
      }
    }
  }
}