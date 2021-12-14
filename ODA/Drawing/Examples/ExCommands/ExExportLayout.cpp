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


#include "StdAfx.h"
#include "DbViewport.h"
#include "DbLayout.h"
#include "DbHostAppServices.h"
#include "DbIdMapping.h"
#include "AbstractViewPE.h"

////////////////////////////////////////////////////////////////////////////////
// Export Layout - Exports current Paper Layout to Model Space of a new drawing
////////////////////////////////////////////////////////////////////////////////

void _ExportLayout_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  pDb->startUndoRecord();
  pDb->startTransaction();
  try
  {
    OdDbObjectId idBlk = pDb->getPaperSpaceId();
    OdDbBlockTableRecordPtr pBlk = idBlk.safeOpenObject(OdDb::kForWrite);
    OdDbLayoutPtr pLayout = pBlk->getLayoutId().safeOpenObject();

    // Convert Viewports geometry to simple entities
    OdDbObjectIdArray arrViewports = pLayout->getViewportArray();
    if ( arrViewports.size() == 0)
    {
      pIO->putString(L"Layout is not initialized");
    }
    else
    {
      // First viewport in the list is overall - skip it
      for (unsigned i = 1; i < arrViewports.size(); ++i)
      {
        OdDbViewportPtr pVp = arrViewports[i].safeOpenObject(OdDb::kForWrite);
        pVp->explodeGeometryToBlock(pBlk);
        pVp->setNonRectClipEntityId(OdDbObjectId::kNull); // To prevent its erasing
        pVp->erase();
      }
      // Erase overall viewport
      arrViewports[0].safeOpenObject(OdDb::kForWrite)->erase();

      OdDbObjectId idXDic = pBlk->extensionDictionary();
      if (!idXDic.isNull())
      {
        OdDbObjectPtr pXDic = idXDic.safeOpenObject(OdDb::kForWrite);
        pXDic->erase();
      }

      pDb->endTransaction();

      // Paper block is cloned to Paper Space
      // pNewDb = pDb->wblock(idBlk);

      // insert() creates new BTR - can't be used

      OdDbDatabasePtr pNewDb = pDb->appServices()->createDatabase();
      OdDbObjectIdArray arrToCopy;
      OdDbObjectIteratorPtr pIter = pBlk->newIterator();
      while (!pIter->done())
      {
        arrToCopy.append(pIter->objectId());
        pIter->step();
      }
      OdDbIdMappingPtr idMap = OdDbIdMapping::createObject();
      pDb->wblockCloneObjects(arrToCopy, pNewDb->getModelSpaceId(), *idMap.get(), OdDb::kDrcReplace);

      OdString strDefName = pDb->getFilename();
      int nDotPos = strDefName.reverseFind(L'.');
      if (nDotPos >= 0)
      {
        strDefName = strDefName.left(nDotPos);
      }
      strDefName += L"_";
      strDefName += pLayout->getLayoutName();

      OdStreamBufPtr pFile = ::odSystemServices()->createFile(
      pIO->getFilePath(L"Enter file name to save:",
                       OdEd::kGfpForSave,
                       L"Export Layout to Model Space Drawing",
                       L"dwg",
                       strDefName,
                       L"Drawing (*.dwg)|*.dwg||"),
        Oda::kFileWrite,
        Oda::kShareDenyReadWrite,
        Oda::kCreateAlways);

      OdDbObjectId id = pNewDb->activeViewportId();
      OdDbObjectPtr pVp = id.safeOpenObject(OdDb::kForWrite);
      OdAbstractViewPEPtr pAVP(pVp);
      pAVP->zoomExtents(pVp);
  
      pNewDb->writeFile(pFile, OdDb::kDwg, OdDb::kDHL_CURRENT);
    }
  }
  catch (const OdError& err)
  {
    OdString strMessage = err.description();
    pIO->putString(strMessage);

    pDb->abortTransaction();
  }
  pDb->undo();
}
