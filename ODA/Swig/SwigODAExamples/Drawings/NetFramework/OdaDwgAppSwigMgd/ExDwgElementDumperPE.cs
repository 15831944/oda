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
using System.Windows.Forms;
using System.Collections.Generic;

namespace OdaDwgAppSwigMgd
{
  class ListFillerDwg : OdDbDwgFiler
  {
    ListView m_pObjList;
    OdDbDatabase m_pDb;
    public override OdDbDatabase database()
    {
      return m_pDb;
    }
    public override FilerType filerType()
    {
      return FilerType.kCopyFiler;
    }

    public ListFillerDwg(OdDbDatabase pDb, ListView pObjList)
    {
      m_pObjList = pObjList;
      m_pDb = pDb;
    }
    private void dumpFieldGroupCode(string fieldGroupCode)
    {
      m_pObjList.Items.Add(fieldGroupCode);
    }
    private void dumpFieldType(string fieldType)
    {
      m_pObjList.Items[m_pObjList.Items.Count - 1].SubItems.Add(fieldType);
    }
    private void dumpFieldValue(string fieldValue)
    {
      m_pObjList.Items[m_pObjList.Items.Count - 1].SubItems.Add(fieldValue);
    }

    private string objectIdAndClassName(OdDbObjectId id)
    {
      OdDbObject pObj = id.openObject();
      string str = "";
      try
      {
        str = pObj.objectId().getHandle().ascii();
        str += (" : <");
        str += pObj.isA().name();
        str += (">");
      }
      catch
      {
        str = "0 : (null)";
      }
      return str;
    }

    public override void wrString(string value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("String");
      dumpFieldValue(value);
    }
    public override void wrBool(bool value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Boolean");
      dumpFieldValue(value ? "true" : "false");
    }
    public void wrInt8(int value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Int8");
      dumpFieldValue(value.ToString());
    }
    public void wrUInt8(int value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("UInt8");
      dumpFieldValue(value.ToString());
    }
    public override void wrInt16(Int16 value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Int16");
      dumpFieldValue(value.ToString());
    }
    public void wrUInt16(UInt16 value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("UInt16");
      dumpFieldValue(value.ToString());
    }
    public override void wrInt32(Int32 value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Int32");
      dumpFieldValue(value.ToString());
    }
    public void wrUInt32(UInt32 value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("UInt32");
      dumpFieldValue(value.ToString());
    }
    public override void wrInt64(Int64 value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Int64");
      dumpFieldValue(value.ToString());
    }
    public void wrUInt64(UInt64 value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("UInt64");
      dumpFieldValue(value.ToString());
    }
    public void wrHandle(OdDbHandle value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Handle");
      dumpFieldValue(value.ascii());
    }
    public override void wrSoftOwnershipId(OdDbObjectId value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("SoftOwnershipRef");
      dumpFieldValue(objectIdAndClassName(value));
    }
    public override void wrHardOwnershipId(OdDbObjectId value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("HardOwnershipRef");
      dumpFieldValue(objectIdAndClassName(value));
    }
    public override void wrSoftPointerId(OdDbObjectId value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("SoftPointerRef");
      dumpFieldValue(objectIdAndClassName(value));
    }
    public override void wrHardPointerId(OdDbObjectId value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("HardPointerRef");
      dumpFieldValue(objectIdAndClassName(value));
    }
    public override void wrDouble(double value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Double");
      dumpFieldValue(value.ToString("G4"));
    }
    public override void wrPoint2d(OdGePoint2d value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Point2d");
      //dumpFieldValue(value.ToString());
      dumpFieldValue(String.Format("({0:G4}, {1:G4})", value.x, value.y));
    }
    public override void wrPoint3d(OdGePoint3d value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Point3d");
      //dumpFieldValue(value.ToString());
      dumpFieldValue(String.Format("({0:G4}, {1:G4}, {2:G4})", value.x, value.y, value.z));
    }
    public override void wrVector2d(OdGeVector2d value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Vector2d");
      //dumpFieldValue(value.ToString());
      dumpFieldValue(String.Format("({0:G4}, {1:G4})", value.x, value.y));
    }
    public override void wrVector3d(OdGeVector3d value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Vector3d");
      //dumpFieldValue(value.ToString());
      dumpFieldValue(String.Format("({0:G4}, {1:G4}, {2:G4})", value.x, value.y, value.z));
    }
    public override void wrScale3d(OdGeScale3d value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Scale3d");
      //dumpFieldValue(value.ToString());
      dumpFieldValue(String.Format("({0:G4}, {1:G4}, {2:G4})", value.sx, value.sy, value.sz));
    }
    public void wrBytes(int value)
    {
      dumpFieldGroupCode("");
      dumpFieldType("Binary");
      dumpFieldValue("(" + value.ToString() + ") {...}");
    }
  }

  class ListFillerDxf : OdDbDxfFiler
  {
    ListView m_pObjList;
    OdDbDatabase m_pDb;
    public override OdDbDatabase database()
    {
      return m_pDb;
    }
    public override bool includesDefaultValues()
    {
      return true;
    }
    public override FilerType filerType()
    {
      return OdDbFiler.FilerType.kBagFiler;
    }

    public ListFillerDxf(OdDbDatabase pDb, ListView listView)
    {
      m_pObjList = listView;
      m_pDb = pDb;
    }
    private void dumpFieldGroupCode(string fieldGroupCode)
    {
      m_pObjList.Items.Add(fieldGroupCode);
    }
    private void dumpFieldType(string fieldType)
    {
      m_pObjList.Items[m_pObjList.Items.Count - 1].SubItems.Add(fieldType);
    }
    private void dumpFieldValue(string fieldValue)
    {
      m_pObjList.Items[m_pObjList.Items.Count - 1].SubItems.Add(fieldValue);
    }
    private string objectIdAndClassName(OdDbObjectId id)
    {
      string str = "";
      try
      {
        OdDbObject pObj = id.openObject();
        str = pObj.objectId().getHandle().ascii();
        str += (" : <");
        str += pObj.isA().name();
        str += (">");
      }
      catch
      {
        str = "0 : (null)";
      }
      return str;
    }

    public override void wrString(int group, string value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("String");
      dumpFieldValue(value);
    }
    public override void wrName(int group, string value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Name");
      dumpFieldValue(value);
    }
    public override void wrBool(int group, bool value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Boolean");
      dumpFieldValue(value ? "true" : "false");
    }
    public override void wrInt8(int group, sbyte value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Int8");
      dumpFieldValue(value.ToString());
    }
    public override void wrUInt8(int group, byte value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("UInt8");
      dumpFieldValue(value.ToString());
    }
    public override void wrInt16(int group, Int16 value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Int16");
      dumpFieldValue(value.ToString());
    }
    public override void wrUInt16(int group, UInt16 value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("UInt16");
      dumpFieldValue(value.ToString());
    }
    public override void wrInt32(int group, Int32 value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Int32");
      dumpFieldValue(value.ToString());
    }
    public override void wrUInt32(int group, UInt32 value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("UInt32");
      dumpFieldValue(value.ToString());
    }
    public override void wrInt64(int group, Int64 value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Int64");
      dumpFieldValue(value.ToString());
    }
    public override void wrUInt64(int group, UInt64 value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("UInt64");
      dumpFieldValue(value.ToString());
    }
    public override void wrHandle(int group, OdDbHandle value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Handle");
      dumpFieldValue(value.ascii());
    }
    public override void wrObjectId(int group, OdDbObjectId value)
    {
      dumpFieldGroupCode(group.ToString());
      string strType = "";
      switch (OdDxfCode._getType(group))
      {
        case OdDxfCode.Type.SoftPointerId:
          strType = "SoftPointerRef";
          break;
        case OdDxfCode.Type.HardPointerId:
          strType = "HardPointerRef";
          break;
        case OdDxfCode.Type.SoftOwnershipId:
          strType = "SoftOwnershipRef";
          break;
        case OdDxfCode.Type.HardOwnershipId:
          strType = "HardOwnershipRef";
          break;
        default:
          strType = "ArbitraryId";
          break;
      }
      dumpFieldType(strType);
      dumpFieldValue(objectIdAndClassName(value));
    }
    public override void wrAngle(int group, double value, int precision)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Angle");
      //if (precision == OdDbDxfFiler.kDfltPrec)
      //	dumpFieldValue(value.ToString());
      //else
      dumpFieldValue(value.ToString("G4"));

    }
    public override void wrDouble(int group, double value, int precision)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Double");
      //if (precision == OdDbDxfFiler.kDfltPrec)
      //	dumpFieldValue(value.ToString());
      //else
      dumpFieldValue(value.ToString("G4"));
    }
    public override void wrPoint2d(int group, OdGePoint2d value, int precision)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Point2d");
      //if (precision == kDfltPrec)
      //	dumpFieldValue(value.ToString());
      //else
      dumpFieldValue(String.Format("({0:G4}, {1:G4})", value.x, value.y));
    }
    public override void wrPoint3d(int group, OdGePoint3d value, int precision)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Point3d");
      //if (precision == kDfltPrec)
      //	dumpFieldValue(value.ToString());
      //else
      dumpFieldValue(String.Format("({0:G4}, {1:G4}, {2:G4})", value.x, value.y, value.z));
    }
    public override void wrVector2d(int group, OdGeVector2d value, int precision)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Vector2d");
      //if (precision == kDfltPrec)
      //	dumpFieldValue(value.ToString());
      //else
      dumpFieldValue(String.Format("({0:G4}, {1:G4})", value.x, value.y));

    }
    public override void wrVector3d(int group, OdGeVector3d value, int precision)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Vector3d");
      //if (precision == kDfltPrec)
      //	dumpFieldValue(value.ToString());
      //else
      dumpFieldValue(String.Format("({0:G4}, {1:G4}, {2:G4})", value.x, value.y, value.z));
    }
    public override void wrScale3d(int group, OdGeScale3d value, int precision)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Scale3d");
      //if (precision == kDfltPrec)
      //	dumpFieldValue(value.ToString());
      //else
      dumpFieldValue(String.Format("({0:G4}, {1:G4}, {2:G4})", value.sx, value.sy, value.sz));
    }
    public override void wrBinaryChunk(int group, byte[] value)
    {
      dumpFieldGroupCode(group.ToString());
      dumpFieldType("Binary");
      dumpFieldValue("(" + value.Length + ") {...}");
    }
  }
}