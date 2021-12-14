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

// ExDynamicBlocks.cpp : Defines the initialization routines for the DLL.
//

#include "StdAfx.h"
#include "ExDynamicBlocksModule.h"
#include "RxDynamicModule.h"
#include "DbDatabase.h"
#include "DbSSet.h"
#include "DbBlockReference.h"
#include "Ed/EdUserIO.h"
#include "DbLine.h"
#include "DbUserIO.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTable.h"
#include "OdRound.h"

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER) //  MacOS X dynamic library loading. //#ifdef _TOOLKIT_IN_DLL_


extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID )
{
  switch ( dwReason )
  {
  case DLL_PROCESS_ATTACH:
    // remove this if you need per-thread initialization
    DisableThreadLibraryCalls( (HMODULE)hInstance );
    break;
  }
  return TRUE;
}

#endif //_TOOLKIT_IN_DLL_ //  MacOS X dynamic library loading.


ODRX_DEFINE_DYNAMIC_MODULE(ExDynamicBlocksModule);


ExDynamicBlocksModule::ExDynamicBlocksModule()
{
}

ExDynamicBlocksModule::~ExDynamicBlocksModule()
{
}

void ExDynamicBlocksModule::initApp()
{
  odedRegCmds()->addCommand( &_cmd1 );
  odedRegCmds()->addCommand( &_cmd2 );
  odedRegCmds()->addCommand( &_cmd3 );
  odedRegCmds()->addCommand( &_cmd4 );
  odedRegCmds()->addCommand( &_cmd5 );
  odedRegCmds()->addCommand( &_cmd6 );
  odedRegCmds()->addCommand( &_cmd7 );
  odedRegCmds()->addCommand( &_cmd8 );
#ifdef DYNAMIC_GRIPS_TEST
  OdDbSBAppData::rxInit();
  OdRxOverrule::setIsOverruling(true);
  OdRxOverrule::addOverrule(OdDbBlockReference::desc(), &_grips);
#endif
}

void ExDynamicBlocksModule::uninitApp()
{
#ifdef DYNAMIC_GRIPS_TEST
  OdDbSBAppData::rxUninit();
  OdRxOverrule::removeOverrule(OdDbBlockReference::desc(), &_grips);
#endif
  odedRegCmds()->removeGroup( OD_T("DynamicBlocks") );
}

static OdString formatResbuf( const OdResBuf* rb )
{
  if (!rb)
    return "";
  OdString s;
  switch ( rb->restype() )
  {
  case OdResBuf::kDxfReal:
    s.format(OD_T("%g"), rb->getDouble() );
    break;
  case OdResBuf::kDxfText:
    return rb->getString();
  case OdResBuf::kDxfInt16:
    s.format(OD_T("%d"), rb->getInt16() );
    break;
  case OdResBuf::kDxfInt32:
    s.format(OD_T("%d"), rb->getInt32() );
    break;
  case 11:
    {
      OdGePoint3d p = rb->getPoint3d();
      s.format(OD_T("{%g,%g,%g}"), p.x, p.y, p.z );
    }
    break;
  case 10:
    {
      OdGePoint2d p = rb->getPoint2d();
      s.format(OD_T("{%g,%g}"), p.x, p.y );
    }
    break;
  }
  return s;
}

void ListPropertiesCmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select dynamic block reference:") );
  for ( OdDbSelectionSetIteratorPtr it = pSSet->newIterator(); !it->done(); it->next() )
  {
    OdDbBlockReferencePtr br = OdDbBlockReference::cast(it->objectId().safeOpenObject());
    if ( br.isNull() )
      continue;
    OdDbDynBlockReference ref( br.get() );
    if ( !ref.isDynamicBlock() )
      continue;
    OdDbDynBlockReferencePropertyArray a; ref.getBlockProperties(a);
    for ( unsigned i = 0; i < a.size(); i++ )
    {
      OdString s; s.format( OD_T("%ls (%ls,%ls), %ls = %ls"),
        a[i]->propertyName().c_str(),
        a[i]->show() ? OD_T("v") : OD_T("h"),
        a[i]->readOnly() ? OD_T("r") : OD_T("w"),
        a[i]->description().c_str(),
        formatResbuf( a[i]->value() ).c_str());
      if (a[i]->unitsType() == OdDbDynBlockReferenceProperty::kAngular)
      {
        OdString angle;
        angle.format(L" (%d\u00B0)", OdRoundToLong(a[i]->value()->getDouble()*180/OdaPI));
        s += angle;
      }
      pIO->putString( s );
    }
  }
}

void BlockMoveCmd::execute( OdEdCommandContext* pCmdCtx )
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select dynamic block reference:") );
  for ( OdDbSelectionSetIteratorPtr it = pSSet->newIterator(); !it->done(); it->next() )
  {
    OdDbBlockReferencePtr br = OdDbBlockReference::cast(it->objectId().safeOpenObject());
    if ( br.isNull() )
      continue;
    OdDbDynBlockReference ref( br.get() );
    if ( !ref.isDynamicBlock() )
      continue;
    OdDbDynBlockReferencePropertyArray a; ref.getBlockProperties(a);
    int xProp = -1;
    int yProp = -1;
    for ( unsigned i = 0; i < a.size(); i++ )
    {
      if ( a[i]->propertyName() == OD_T("Position X") )
        xProp = i;
      if ( a[i]->propertyName() == OD_T("Position Y") )
        yProp = i;
    }
    if ( yProp == -1 || xProp == -1 )
      continue;
    OdGePoint3d base = br->position();
    base.x += a[xProp]->value()->getDouble(); // should go to UCS really
    base.y += a[yProp]->value()->getDouble();

    OdGePoint3d p = pIO->getPoint( OD_T("Enter position"), OdEd::kInpThrowEmpty, &base );
    a[xProp]->setValue( OdResBuf::newRb( 40, p.x - br->position().x ) );
    a[yProp]->setValue( OdResBuf::newRb( 40, p.y - br->position().y ) );
  }
}

static int printProperties( const OdDbDynBlockReferencePropertyArray& a,
                            OdDbCommandContext* pDbCmdCtx )
{
  unsigned j = 1;
  for ( unsigned i = 0; i < a.size(); i++ )
  {
    if ( !a[i]->show() || a[i]->readOnly() ) continue;
    OdString s; s.format( OD_T("%d. %ls, %ls = %ls"), j++, a[i]->propertyName().c_str(), a[i]->description().c_str(),
      formatResbuf( a[i]->value() ).c_str() );
    pDbCmdCtx->userIO()->putString( s );
  }
  return j - 1;
}

static void modifyProperty( unsigned N, const OdDbDynBlockReferencePropertyArray& a,
                           OdDbCommandContext* pDbCmdCtx )
{
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  for ( unsigned i = 0, j = 1; i < a.size(); i++ )
  {
    if ( !a[i]->show() || a[i]->readOnly() )
      continue;
    if  ( N != j++ )
      continue;
    OdDbDynBlockReferencePropertyPtr prop = a[i];
    OdString s; s.format( OD_T("%ls = %ls"), prop->propertyName().c_str(), formatResbuf( prop->value() ).c_str() );
    OdResBufArray av;  prop->getAllowedValues( av );
    if ( !av.isEmpty() )
    {
      pIO->putString( OD_T("Allowed values:") );
      for ( unsigned k = 0; k < av.size(); k++ )
      {
        OdString s; s.format(OD_T("%d. %ls"), k, formatResbuf( av[k] ).c_str() );
        pIO->putString( s );
      }
      int n = pIO->getInt( OD_T("Select value:"), OdEd::kInpDefault, -1 );
      if ( n < 0 || n >= (int)av.size() )
        return;
      if ( prop->setValue( av[n] ) )
        pIO->putString( OD_T("Value set successfully") );
      else
        pIO->putString( OD_T("Value set failed") );
    }
    else
    {
      int rt = prop->value()->restype();
      OdResBufPtr pRb = OdResBuf::newRb(rt);
      const wchar_t* prompt = OD_T("Enter new value:");

      try
      {
        switch( rt )
        {
        case OdResBuf::kDxfReal:
          pRb->setDouble( pIO->getReal( prompt, OdEd::kInpThrowEmpty ) );
          break;

        case OdResBuf::kDxfText:
          pRb->setString( pIO->getString( prompt, OdEd::kInpThrowEmpty ) );
          break;

        case OdResBuf::kDxfInt16:
          pRb->setInt16( OdUInt16(pIO->getInt( prompt, OdEd::kInpThrowEmpty )) );
          break;

        case OdResBuf::kDxfInt32:
          pRb->setInt32( OdUInt32(pIO->getInt( prompt, OdEd::kInpThrowEmpty )) );
          break;

        case 11:
          pRb->setPoint3d( pIO->getPoint( prompt, OdEd::kInpThrowEmpty, &prop->value()->getPoint3d() ) );
          break;

        case 10:
          ODA_FAIL_ONCE();
          break;
        }
        if (prop->setValue( pRb ))
          pIO->putString( OD_T("Value set successfully") );
        else
          pIO->putString( OD_T("Value set failed") );
      }
      catch(const OdEdEmptyInput)
      {// do not change property
      }
    }
    return;
  }
}

void ModifyPropertiesCmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select dynamic block reference:") );
  for ( OdDbSelectionSetIteratorPtr it = pSSet->newIterator(); !it->done(); it->next() )
  {
    OdDbBlockReferencePtr br = OdDbBlockReference::cast(it->objectId().safeOpenObject());
    if ( br.isNull() )
      continue;
    OdDbDynBlockReference ref( br.get() );
    if ( !ref.isDynamicBlock() )
      continue;
    OdDbDynBlockReferencePropertyArray a; ref.getBlockProperties(a);
    int N = printProperties( a, pDbCmdCtx );
    if ( N == 0 )
      continue;
    int n = pDbCmdCtx->userIO()->getInt( OD_T("Select property:"), OdEd::kInpDefault, 1 );
    if ( n > N || n < 1 )
      return;
    modifyProperty( n, a, pDbCmdCtx );
  }
}

void ResetBlockCmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select dynamic block reference to reset:") );
  for ( OdDbSelectionSetIteratorPtr it = pSSet->newIterator(); !it->done(); it->next() )
  {
    OdDbBlockReferencePtr br = OdDbBlockReference::cast(it->objectId().safeOpenObject());
    if ( br.isNull() )
      continue;
    OdDbDynBlockReference ref( br.get() );
    if ( !ref.isDynamicBlock() )
      continue;
    ref.resetBlock();
  }
}

void ConvertToStaticBlockCmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdDbSelectionSetPtr pSSet = pIO->select( OD_T("Select dynamic block reference to convert:") );
  for ( OdDbSelectionSetIteratorPtr it = pSSet->newIterator(); !it->done(); it->next() )
  {
    OdDbBlockReferencePtr br = OdDbBlockReference::cast(it->objectId().safeOpenObject());
    if ( br.isNull() )
      continue;
    OdDbDynBlockReference ref( br.get() );
    if ( !ref.isDynamicBlock() )
      continue;
    ref.convertToStaticBlock( pIO->getString( OD_T("Enter new block name:"), OdEd::kGstAllowSpaces, OD_T("*U") ) );
  }
}

void PrintDynamicBlockNamesCmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr db = pDbCmdCtx->database();
  OdDbBlockTablePtr blockTablePtr = db->getBlockTableId().safeOpenObject();
  for (OdDbSymbolTableIteratorPtr itPtr = blockTablePtr->newIterator(); !itPtr->done(); itPtr->step())
  {
    OdDbBlockTableRecordPtr recordPtr = itPtr->getRecord(OdDb::kForRead);
    if (!recordPtr.isNull())
    {
      if (recordPtr->isAnonymous() && OdDbDynBlockReference::isDynamicBlock(recordPtr->objectId()))
      {
        OdResBufPtr pXData = recordPtr->xData(L"AcDbDynamicBlockTrueName");
        pXData = pXData->next();
        OdString recordName = pXData->getString();
        pDbCmdCtx->dbUserIO()->putString(recordName);
      }
    }
  }
}

#include "DynamicBlocks/DbBlockElement.h"
#include "DynamicBlocks/DbBlockActions.h"

// prints dynamic block evaluation graph in DOT format
// see http://www.graphviz.org/ for details
void PrintGraphCmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdDbSelectionSetPtr pSSet = pIO->select(L"Select dynamic block reference to print graph:");
  for ( OdDbSelectionSetIteratorPtr it = pSSet->newIterator(); !it->done(); it->next() )
  {
    OdDbBlockReferencePtr br = OdDbBlockReference::cast(it->objectId().safeOpenObject());
    if ( br.isNull() )
      continue;
    OdDbDynBlockReference dr( br.get() );
    if ( !dr.isDynamicBlock() )
      continue;
    OdDbEvalGraphPtr gr = OdDbEvalGraph::getGraph(dr.dynamicBlockTableRecord().safeOpenObject(), L"ACAD_ENHANCEDBLOCK");
    if (gr.isNull())
      continue;
    OdString s;
    s.format(L"digraph G%" PRIX64W L" {", (OdUInt64)gr->objectId().getHandle());
    pIO->putString(s);
    OdDbEvalNodeIdArray allNodes;
    gr->getAllNodes(allNodes);
    for (unsigned int i = 0; i < allNodes.size(); ++i)
    {
      int N = allNodes[i];
      OdDbEvalExprPtr e = gr->getNode(N);
      if (e.isNull())
        continue;
      // print node declaration
      if (e->isKindOf(OdDbBlockElement::desc()))
      {
        s.format(L"  Node%d [label=\"%ls (%" PRIX64W L")\\nname = %ls, (%d)\"];", N, e->isA()->name().c_str(), (OdUInt64)e->objectId().getHandle(), OdDbBlockElementPtr(e)->name().c_str(), (int)e->nodeId());
      }
      else if (e->isKindOf(OdDbBlockGripExpr::desc()))
      {
        s.format(L"  Node%d [shape=box,label=\"(%" PRIX64W L"), (%d)\"];", N, (OdUInt64)e->objectId().getHandle(), (int)e->nodeId());
      }
      else
      {
        s.format(L"  Node%d [label=\"%ls (%" PRIX64W L"), (%d)\"];", N, e->isA()->name().c_str(), (OdUInt64)e->objectId().getHandle(), (int)e->nodeId());
      }
      pIO->putString(s);

      OdDbEvalEdgeInfoArray edges;
      gr->getOutgoingEdges(N, edges);
      for (unsigned int j = 0; j < edges.size(); ++j)
      {
        if (edges[j].isInvertible() && edges[j].isSuppressed())
        {
          s.format(L"    Node%d -> Node%d [label=\"I & S\"];", N, edges[j].to());
        }
        else if (edges[j].isInvertible())
        {
          s.format(L"    Node%d -> Node%d [label=\"I\"];", N, edges[j].to());
        }
        else if (edges[j].isSuppressed())
        {
          s.format(L"    Node%d -> Node%d [label=\"S\"];", N, edges[j].to());
        }
        else
        {
          s.format(L"    Node%d -> Node%d;", N, edges[j].to());
        }
        pIO->putString(s);
      } // end edges
      if (e->isKindOf(OdDbBlockAction::desc()))
      {
        OdDbObjectIdArray selection = ((OdDbBlockAction*)e.get())->elementSelectionSet();
        for (unsigned int j = 0; j < selection.size(); ++j)
        {
          OdDbEvalExprPtr se = selection[j].openObject();
          if (!se.isNull())
          {
            s.format(L"    Node%d -> Node%d [style=dotted];", N, se->nodeId());
            pIO->putString(s);
          }
        }
      }
    } // end graph
    pIO->putString(L"}");
  }
}

void UpdateAnonymousBlocksCmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
  OdDbSelectionSetPtr pSSet = pIO->select(OD_T("Select dynamic block reference:"));
  for (OdDbSelectionSetIteratorPtr it = pSSet->newIterator(); !it->done(); it->next())
  {
    {
      OdDbObjectPtr pObj = it->objectId().openObject();
      if (!pObj->isKindOf(OdDbBlockReference::desc()))
        continue;
    }
    OdDbDynBlockReference dbr(it->objectId());
    if (!dbr.isDynamicBlock())
      continue;
    OdDbDynBlockTableRecord dbtr(dbr.dynamicBlockTableRecord());
    dbtr.updateAnonymousBlocks();
  }
}
