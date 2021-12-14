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
//
// SetupLayoutHelper.h
//

#ifndef OD_SETUP_LAYOUT_HELPER_H_
#define OD_SETUP_LAYOUT_HELPER_H_

#include "OdaCommon.h"
#include "ExtDbModule.h"
#include "DbGsManager.h"
#include "ColorMapping.h"

inline void setViewportBorderProperties(OdGsDevice* pGsDevice)
{
  ODA_ASSERT_ONCE(pGsDevice); // test
  OdGsLayoutHelperPtr pLayoutHelper = OdGsLayoutHelper::cast(pGsDevice);
  if (!pLayoutHelper.get())
    return; // DGN way
  OdGsViewPtr pActiveView = pLayoutHelper->activeView();

  OdGsViewPtr pOverallView;
  OdGsPaperLayoutHelperPtr pPSHelper = OdGsPaperLayoutHelper::cast(pGsDevice);
  if (pPSHelper.get())
    pOverallView = pPSHelper->overallView();

  int count = pGsDevice->numViews();
  if (count <= 1)
    return;
  const ODCOLORREF* pColors = odcmAcadPalette(pGsDevice->getBackgroundColor()); // odcmAcadDarkPalette() or odcmAcadLightPalette();

  for (int index = 0; index < count; index++)
  {
    OdGsViewPtr pGsView = pGsDevice->viewAt(index);
    if (   (pGsView == pOverallView) 
      //|| (OdGsPaperLayoutHelper::cast(pGsDevice).get() && (pGsView != pActiveView))
      )
    {
      // no border
      pGsView->setViewportBorderVisibility(false);
    }
    else if (pGsView != pActiveView)
    {
      pGsView->setViewportBorderVisibility(true);
      pGsView->setViewportBorderProperties(pColors[7], 1);
    }
    else
    {
      pGsView->setViewportBorderVisibility(true);
      pGsView->setViewportBorderProperties(pColors[7], 3);
    }
  }
}


inline OdString currentLayoutName(OdRxObject* pRxDb)
{
  OdString sCurrentLayout;

  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
  ODA_ASSERT_ONCE(pDbPE.get());
  if (pDbPE.isNull())
    return sCurrentLayout;

  OdRxObjectPtr pLayout = pDbPE->currentLayout(pRxDb);
  OdDbBaseLayoutPEPtr pLayoutInfo(pLayout);
  ODA_ASSERT_ONCE(pLayoutInfo.get());
  if (pLayoutInfo.isNull())
    return sCurrentLayout;

  sCurrentLayout = pLayoutInfo->name(pLayout);
  return sCurrentLayout;
}

inline OdStringArray layoutNames(OdRxObject* pRxDb)
{
  OdStringArray lstLayouts;

  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
  ODA_ASSERT_ONCE(!pDbPE.isNull());
  if (pDbPE.isNull())
    return lstLayouts;

  OdRxObjectPtr pLayout = pDbPE->currentLayout(pRxDb);
  OdDbBaseLayoutPEPtr pLayoutInfo(pLayout);
  OdString sCurrentLayout,
           sName;
  ODA_ASSERT_ONCE(pLayoutInfo.get());
  if (pLayoutInfo.get())
  {
    sCurrentLayout = pLayoutInfo->name(pLayout);
    lstLayouts.push_back(sCurrentLayout); // to have the same numeration as apiHandlers.js
  }

  OdRxIteratorPtr pLayIter = pDbPE->layouts(pRxDb);
  for (; pLayIter.get() && !pLayIter->done(); pLayIter->next())
  {
    pLayout = pLayIter->object();
    pLayoutInfo = OdDbBaseLayoutPE::cast(pLayout);
    ODA_ASSERT_ONCE(!pLayoutInfo.isNull());
    if (pLayoutInfo.isNull())
      continue;
    
    sName = pLayoutInfo->name(pLayout);
    ODA_ASSERT_ONCE(!sName.isEmpty());
    if (sName.isEmpty()) 
    {
      ODA_ASSERT_ONCE(!sName.isEmpty());
      continue;
    }

    unsigned int nIndex = 0;
    if (lstLayouts.find(sName, nIndex))
    {
      continue;
    }

    lstLayouts.push_back(sName);
  } // end for

  return lstLayouts; 
}

inline unsigned int numLayouts(OdRxObject* pRxDb)
{
  return layoutNames(pRxDb).size();
}

inline OdString layoutNameByIndex(OdRxObject* pRxDb, int index)
{
  OdStringArray names = layoutNames(pRxDb);
  ODA_ASSERT_ONCE(index >= 0 && index < (int) names.size());
  return names[index];
}

inline OdDbStub* layoutViewId(OdRxObject* pRxDb, const OdString& sLayoutName)
{
  ODA_ASSERT_ONCE(!sLayoutName.isEmpty());
  OdDbStub* idLayoutView = NULL;

  OdExtDbModule* pExtDb = getExtDbModule(pRxDb);
  if (pExtDb)
  {
    OdStringArray namesLayoutView;
    OdDbStubPtrArray idsLayoutView;
    bool isStandard = !pExtDb->getLayoutViewNamesAndIds(pRxDb, namesLayoutView, idsLayoutView);
    if (!isStandard)
    {
      // TODO fix next for DGN too (Error: Object of class OdDgModel can't be cast to OdDgView.)
      ODA_ASSERT_ONCE(pExtDb->baseModuleName(true) == L"dgn"); // test
      
      unsigned int index = 0;
      if (namesLayoutView.find(sLayoutName, index))
      {
        idLayoutView = idsLayoutView[index];
        return idLayoutView;
      }
      ODA_FAIL_ONCE();
    }
  }

  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
  ODA_ASSERT_ONCE(pDbPE.get());
  if (pDbPE.isNull())
    return idLayoutView;

  OdRxIteratorPtr pLayIter = pDbPE->layouts(pRxDb);
  for (; pLayIter.get() && !pLayIter->done(); pLayIter->next())
  {
    OdRxObjectPtr pLayout = pLayIter->object();
    OdDbBaseLayoutPEPtr pLayoutInfo = OdDbBaseLayoutPE::cast(pLayout);
    ODA_ASSERT_ONCE(!pLayoutInfo.isNull());
    if (pLayoutInfo.isNull())
      continue;
    OdString sName = pLayoutInfo->name(pLayout);
    if (sName != sLayoutName) 
      continue;

    idLayoutView = pDbPE->getId(pLayout);
    break;
  } // end for
  ODA_ASSERT_ONCE(idLayoutView);

  return idLayoutView;
}

inline OdGsDevicePtr setupLayoutHelper(OdRxObject* pRxDb, OdGsDevice* pGsDevice,
                                       int indexLayout = -1, // current id used if < 0
                                       bool bEnableGsModel = true,
                                       ODCOLORREF clrBackground = ODRGB(255, 255, 255),
                                       bool bCheckPaperAndBorder = false,
                                       ODCOLORREF clrPaper = ODRGB(255, 255, 255),
                                       ODCOLORREF clrPaperBackground = ODRGB(173, 173, 173))
{
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
  ODA_ASSERT_ONCE(pDbPE.get());

  OdDbStub* idLayout = NULL;
  if (indexLayout >= 0)
    idLayout = layoutViewId(pRxDb, layoutNameByIndex(pRxDb, indexLayout));

  /** *************************************************************
  */
  /* Create the vectorization context.                            */
  /* This class defines the operations and properties that are    */
  /* used in the vectorization of an database.                    */
  /** *************************************************************
  */
  OdGiDefaultContextPtr pGiCtx = pDbPE->createGiContext(pRxDb);
  pGiCtx->enableGsModel(bEnableGsModel);

  /** *************************************************************
  */
  /* Enable GsModel to have cache.                                */
  /** *************************************************************
  */
  pGiCtx->enableGsModel(bEnableGsModel);

  /** *************************************************************
  */
  /* Prepare the device to render the active layout in            */
  /* this database.                                               */
  /** *************************************************************
  */
  OdGsDevicePtr pGsHelper;
  if (idLayout)
    pGsHelper = pDbPE->setupLayoutView(pGsDevice, pGiCtx, idLayout);
  else
    pGsHelper = pDbPE->setupActiveLayoutViews(pGsDevice, pGiCtx);

  /** *************************************************************
  */
  /* Setup background and border.                                 */
  /** *************************************************************
  */
  pDbPE->setupPalette(pGsHelper, pGiCtx, idLayout, clrBackground);
  if (bCheckPaperAndBorder)
  {
    OdGsPaperLayoutHelperPtr pPSHelper = OdGsPaperLayoutHelper::cast(pGsHelper);
    if (pPSHelper.get())
    {
      pGiCtx->setPaletteBackground(clrPaper);
      pGsDevice->setBackgroundColor(clrPaperBackground);
    }
    setViewportBorderProperties(pGsHelper);
  }

  return pGsHelper;
}

#endif // OD_SETUP_LAYOUT_HELPER_H_
