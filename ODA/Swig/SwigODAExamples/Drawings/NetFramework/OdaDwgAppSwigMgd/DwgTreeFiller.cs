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
using System.Windows.Forms;

namespace OdaDwgAppSwigMgd
{
  using TableMap = System.Collections.Generic.Dictionary<TreeNode, OdRxObject>;
  class DwgTreeFiller : OdIdFiler
  {
    //TableMap m_nonDBROItems = new TableMap();
    //TreeView m_pObjTree;
    OdDbDatabase m_pDb;
    TreeNode m_dbTreeItem;
    Stack<KeyValuePair<TreeNode, OdDbDictionaryIterator>> _itemStack;

    public override OdDbDatabase database()
    {
      return m_pDb;
    }

    public override void wrSoftOwnershipId(OdDbObjectId value)
    {
      addObject2Tree(value, _itemStack.Peek().Key);
    }

    public override void wrHardOwnershipId(OdDbObjectId value)
    {
      addObject2Tree(value, _itemStack.Peek().Key);
    }

    public override void wrSoftPointerId(OdDbObjectId value)
    {
    }

    public override void wrHardPointerId(OdDbObjectId value)
    {
    }

    public override void addReference(OdDbObjectId id, ReferenceType rt)
    {
      if ((rt & ReferenceType.kSoftOwnershipRef) != 0)
      {
        addObject2Tree(id, _itemStack.Peek().Key);
      }
    }


    public DwgTreeFiller(OdDbDatabase pDb, TreeView pObjTree)
    {
      m_pDb = pDb;

      //Add root database node
      String sName = String.Format("{0} : <{1}>", pDb.handle().ascii(), pDb.isA().name());
      m_dbTreeItem = pObjTree.Nodes.Add(sName);
      m_dbTreeItem.Tag = m_pDb.objectId();

      _itemStack = new Stack<KeyValuePair<TreeNode, OdDbDictionaryIterator>>();
      KeyValuePair<TreeNode, OdDbDictionaryIterator> pairRoot = new KeyValuePair<TreeNode, OdDbDictionaryIterator>(m_dbTreeItem, null);

      _itemStack.Push(pairRoot);
    }

    public TreeNode addObject2Tree(OdDbObjectId pElm, TreeNode hParent)
    {
      return addObject2Tree(pElm, hParent, null);
    }

    public TreeNode addObject2Tree(OdDbObjectId objId, TreeNode hParent, TreeNode hInsertAfter)
    {
      String sName;
      if (objId.isErased())
        return null;
      OdDbObject pObj = objId.openObject();
      if (pObj != null)
      {
        String str = "";
        String dictStr = "";
        String prx = "";
        OdDbSymbolTableRecord pRec = OdDbSymbolTableRecord.cast(pObj);
        if (pRec != null)
        {
          str = pRec.getName();
        }
        else if (_itemStack.Peek().Value != null)
        {
          OdDbDictionaryIterator pIter = _itemStack.Peek().Value;
          if (pIter.setPosition(objId))
          {
            dictStr = pIter.name() + "  ";
          }
        }
        if (str == "")
        {
          OdDbProxyExt pProxyExt = OdDbProxyExt.cast(pObj);
          if (pProxyExt != null)
          {
            prx = " <Proxy> : ";
            str = pProxyExt.originalClassName(pObj);
          }
          else
          {
            if (pObj.isKindOf(OdDbDatabase.desc())
                && !((OdDbDatabase)pObj).xrefBlockId().isNull())
            {
              dictStr = "XREF:";
              prx = "";
              str = pObj.isA().name();
            }
            else
            {
              prx = "";
              str = pObj.isA().name();
            }
          }
        }
        String strHandle = objId.getHandle().ascii();
        sName = String.Format("{0}: {1}<{2}{3}>", strHandle, dictStr, prx, str);

        TreeNode hItem = (hInsertAfter == null) ? hParent.Nodes.Add(sName)
                                                : hParent.Nodes.Insert(hParent.Nodes.IndexOf(hInsertAfter), sName);
        if (!pObj.objectId().isNull())
          hItem.Tag = pObj.objectId();
        return hItem;
      }
      return null;
    }

    public void outSubObjects2Tree(OdDbObjectId objId, TreeNode hParent)
    {
      OdDbDictionaryIterator pIter = null;
      OdDbObject pObj = objId.openObject();
      OdDbDictionary pDict = OdDbDictionary.cast(pObj);
      if (pDict != null)
        pIter = pDict.newIterator(DictIterType.kDictCollated);
      _itemStack.Push(new KeyValuePair<TreeNode, OdDbDictionaryIterator>(hParent, pIter));
      pObj.dwgOutFields(this);
      if (pObj.isKindOf(OdDbBlockTableRecord.desc())
          && ((OdDbBlockTableRecord)(pObj)).xrefDatabase() != null)
      {
        addObject2Tree(((OdDbBlockTableRecord)pObj).xrefDatabase().objectId(), hParent);
      }
      _itemStack.Pop();
    }

    public void explandItem(TreeNode hItem)
    {
      OdDbObject pElm = null;
      if (hItem.Tag != null)
        pElm = ((OdDbObjectId)hItem.Tag).openObject();

      if (pElm != null)
        outSubObjects2Tree(pElm.objectId(), hItem);
      return;
    }

    public TreeNode DbTreeItem
    {
      get
      {
        return m_dbTreeItem;
      }
    }

    public OdRxObject getObject(TreeNode hItem)
    {
      if (hItem == m_dbTreeItem)
        return database();
      OdDbObjectId elmId = hItem.Tag as OdDbObjectId;
      if (elmId != null)
        return elmId.openObject();
      return null;
    }
  }

}
