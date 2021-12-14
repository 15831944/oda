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
#include "OdaCommon.h"
#include "ExDgElementReactor.h"

void OdExDgElementReactor::copied(const OdDgElement* pElm, const OdDgElement* /*pNewElm*/)
{
  OdString msg;
  msg.format(OD_T("[ELM_REACTOR] : copied                 <%-24ls %3ls>\n"), pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgElementReactor::erased(const OdDgElement* pElm, bool bErasing)
{
  OdString msg;
  msg.format(OD_T("[ELM_REACTOR] : erased(%ls)          <%-24ls %3ls>\n"), bErasing ? L"True " : L"False", pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgElementReactor::goodbye(const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("[ELM_REACTOR] : goodbye                <%-24ls %3ls>\n"), pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgElementReactor::openedForModify(const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("[ELM_REACTOR] : openedForModify        <%-24ls %3ls>\n"), pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgElementReactor::modified(const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("[ELM_REACTOR] : modified               <%-24ls %3ls>\n"), pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgElementReactor::modifiedGraphics(const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("[ELM_REACTOR] : modifiedGraphics       <%-24ls %3ls>\n"), pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgElementReactor::modifiedLinkages(const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("[ELM_REACTOR] : modifiedLinkages       <%-24ls %3ls>\n"), pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgElementReactor::subElementModified(const OdDgElement* pElm, const OdDgElement* /*pSubObj*/)
{
  OdString msg;
  msg.format(OD_T("[ELM_REACTOR] : subObjModified         <%-24ls %3ls>\n"), pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgElementReactor::modifyUndone(const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("[ELM_REACTOR] : modifyUndone           <%-24ls %3ls>\n"), pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgElementReactor::unappended(const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("[ELM_REACTOR] : unappended             <%-24ls %3ls>\n"), pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgElementReactor::reappended(const OdDgElement* pElm)
{
  OdString msg;
  msg.format(OD_T("[ELM_REACTOR] : reappended            <%-24ls %3ls>\n"), pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
  printMessage(msg);
}

void OdExDgElementReactor::closed(const OdDgElementId& elmId)
{
  OdString msg;
  msg.format(OD_T("[ELM_REACTOR] : closed                 <%3ls>\n"), elmId.getHandle().ascii().c_str());
  printMessage(msg);
}
