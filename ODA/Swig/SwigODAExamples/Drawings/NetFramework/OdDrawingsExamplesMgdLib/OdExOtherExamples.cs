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

using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib
{
  public static class OdExOtherExamples
  {
    public static void ChangeObjectColor(OdDbDatabase _db, OdDbBlockTableRecord bBTR, ulong handleId, OdCmColor color)
    {
      OdDbObjectId objectId = _db.getOdDbObjectId(new OdDbHandle(handleId));

      using (OdDbObject dbObject = objectId.safeOpenObject(Teigha.TD.OpenMode.kForWrite))
      {
        //((OdDbEntity)dbObject).setColor(color);
        OdDbCircle circle = (OdDbCircle)dbObject;
        if (circle != null)
        {
          OdCmColor ccc1 = circle.color();
          circle.setColorIndex((ushort)(OdCmEntityColor.ACIcolorMethod.kACIGreen));
          OdCmColor ccc = circle.color();

          OdDbCircle pCircle = OdDbCircle.createObject();
          pCircle.setDatabaseDefaults(bBTR.database());
          bBTR.appendOdDbEntity(pCircle);
          OdGePoint3d center = circle.center();
          pCircle.setCenter(center);
          pCircle.setRadius(circle.radius() / 2);
          OdCmColor color1 = new OdCmColor();
          color1.setRGB(255, 0, 0);
          pCircle.setColor(color1);

          //((OdDbEntity)dbObject).setColorIndex((uint)OdCmEntityColor.ACIcolorMethod.kACIGreen);
        }

        if (dbObject is OdDbBlockReference)
        {
          using (OdDbBlockTableRecord blockTableRecord = (OdDbBlockTableRecord)((OdDbBlockReference)dbObject).blockTableRecord().safeOpenObject(Teigha.TD.OpenMode.kForWrite))
          {
            OdDbObjectIterator iter = blockTableRecord.newIterator();
            for (; !iter.done(); iter.step())
            {
              OdDbObjectId subObjectId = iter.entity().objectId();

              OdCmColor colorByBlock = new OdCmColor(color);
              colorByBlock.setColorMethod(OdCmEntityColor.Items_ColorMethod.kByBlock);

              ChangeObjectColor(_db, bBTR, subObjectId.getHandle().ToUInt64(), colorByBlock);
            }
          }
        }
      }
    }
  }
}