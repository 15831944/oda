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
// Rendering module interface

#ifndef ODTRRNDRENDERMODULE
#define ODTRRNDRENDERMODULE

#include "TD_PackPush.h"

#include "RxModule.h"
#include "TrRndLocalViewer.h"

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrRndRenderModule : public OdRxModule
{
  public:
    // Create new rendition
    virtual OdTrVisRenditionPtr createRendition(OdTrRndLocalRenditionClient *pRenderClient) = 0;
    // Create new rendition data accessor
    virtual OdTrRndLocalRenditionHostPtr createRenditionHost(OdTrVisRendition *pRendition) = 0;
    // Create new rendition context
    virtual OdTrRndLocalContextPtr createLocalContext(OdTrVisRenderClient *pDevice) = 0;

    // Create new standalone renderer
    virtual OdTrRndLocalRendererPtr createLocalRenderer(OdTrRndLocalRenditionClient *pRenderClient, OdTrRndLocalRenditionHost *pRenditionHost = NULL) = 0;
    // Create new standalone viewer
    virtual OdTrRndLocalViewerPtr createLocalViewer(OdTrRndLocalRenditionClient *pRenderClient, OdTrRndLocalRenditionHost *pRenditionHost = NULL) = 0;

    // Check renderer version
    virtual OdUInt32 checkRendererVersion(OdTrVisRenderClient *pDevice, OdUInt32 nCheckVersion = 0, OdTrVisRendition *pRendition = NULL) const = 0;
};

typedef OdSmartPtr<OdTrRndRenderModule> OdTrRndRenderModulePtr;

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrRndRenderModuleStub : public OdTrRndRenderModule
{
  public:
    // Create new rendition data accessor
    virtual OdTrRndLocalRenditionHostPtr createRenditionHost(OdTrVisRendition * /*pRendition*/)
    { return OdTrRndLocalRenditionHostPtr(); }
    // Create new rendition context
    virtual OdTrRndLocalContextPtr createLocalContext(OdTrVisRenderClient * /*pDevice*/)
    { return OdTrRndLocalContextPtr(); }

    // Create new standalone renderer
    virtual OdTrRndLocalRendererPtr createLocalRenderer(OdTrRndLocalRenditionClient * /*pRenderClient*/, OdTrRndLocalRenditionHost * /*pRenditionHost*/ = NULL)
    { return OdTrRndLocalRendererPtr(); }
    // Create new standalone viewer
    virtual OdTrRndLocalViewerPtr createLocalViewer(OdTrRndLocalRenditionClient * /*pRenderClient*/, OdTrRndLocalRenditionHost * /*pRenditionHost*/ = NULL)
    { return OdTrRndLocalViewerPtr(); }

    // Check renderer version
    virtual OdUInt32 checkRendererVersion(OdTrVisRenderClient * /*pDevice*/, OdUInt32 /*nCheckVersion*/ = 0, OdTrVisRendition * /*pRendition*/ = NULL) const
    { return 0; }
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrRndLocalRenderModule : public OdTrRndRenderModuleStub
{
  public:
    // Create new rendition context
    virtual OdTrRndLocalContextPtr createLocalContext(OdTrVisRenderClient *pDevice);

    // Create new standalone renderer
    virtual OdTrRndLocalRendererPtr createLocalRenderer(OdTrRndLocalRenditionClient *pRenderClient, OdTrRndLocalRenditionHost *pRenditionHost = NULL);
    // Create new standalone viewer
    virtual OdTrRndLocalViewerPtr createLocalViewer(OdTrRndLocalRenditionClient *pRenderClient, OdTrRndLocalRenditionHost *pRenditionHost = NULL);
};


#include "TD_PackPop.h"

#endif // ODTRRNDRENDERMODULE
