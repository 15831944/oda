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
#include "GiDrawObjectForGetGeometry.h"
#include "toString.h"

OdGiDrawObjectForGetGeometry::OdGiDrawObjectForGetGeometry()
: m_pDumper(0)
{
}
  
OdGiDrawObjectForGetGeometry::~OdGiDrawObjectForGetGeometry()
{
}

OdGiDrawObjectForGetGeometryPtr OdGiDrawObjectForGetGeometry::createObject(OdGiContext* pUserContext, 
                                                                           OdGiConveyorGeometry* pOutGeom, 
                                                                           OdGiDumper* pDumper)
{
  OdGiDrawObjectForGetGeometryPtr pThis = new OdStaticRxObject<OdGiDrawObjectForGetGeometry>;
  pThis->m_pDumper = pDumper;
  pThis->setContext(pUserContext);
  pThis->output().setDestGeometry(*pOutGeom);
  return pThis;
}

/************************************************************************/
/* Setup OdGiContext for this OdGiDrawObjectForGetGeometry object       */
/************************************************************************/
void OdGiDrawObjectForGetGeometry::setContext(OdGiContext* pUserContext)
{
  OdGiBaseVectorizer::setContext(pUserContext);
  m_pModelToEyeProc->setDrawContext(drawContext());
}

/************************************************************************/
/* Calls are delegated to OdGiBaseVectorizer, so as to takes clipping   */
/* To disregard clipping, do not delegate these calls                   */
/************************************************************************/
void OdGiDrawObjectForGetGeometry::pushClipBoundary(OdGiClipBoundary* pBoundary)
{
  OdGiBaseVectorizer::pushClipBoundary(pBoundary);
}

void OdGiDrawObjectForGetGeometry::popClipBoundary()
{
  OdGiBaseVectorizer::popClipBoundary();
}

void OdGiDrawObjectForGetGeometry::onTraitsModified()
{
  OdGiBaseVectorizer::onTraitsModified();
  const OdGiSubEntityTraitsData& currTraits = effectiveTraits();
  if(currTraits.trueColor().isByColor())
  {
    draw_color(ODTOCOLORREF(currTraits.trueColor()));
  }
  else
  {
    draw_color_index(currTraits.color());
  }

  OdDb::LineWeight lw = currTraits.lineWeight();
  draw_lineWidth(lw);
}

void OdGiDrawObjectForGetGeometry::draw_color(ODCOLORREF color)
{
  OdString s;
  s.format(L"draw_color (rgb): (%d, %d, %d)", 
           ODGETRED(color), 
           ODGETGREEN(color), 
           ODGETBLUE(color));
  m_pDumper->output(s);
}

void OdGiDrawObjectForGetGeometry::draw_color_index(OdUInt16 colorIndex)
{
  OdString s;
  s.format(L"draw_color_index: %d", colorIndex);
  m_pDumper->output(s);
}

void OdGiDrawObjectForGetGeometry::draw_lineWidth(OdDb::LineWeight weight)
{
  OdString s;
  s.format(L"draw_lineWidth: %d", weight);
  m_pDumper->output(s);
}

void OdGiDrawObjectForGetGeometry::ownerDrawDc(
  const OdGePoint3d& origin,
  const OdGeVector3d& u,
  const OdGeVector3d& v,
  const OdGiSelfGdiDrawable* pDrawable,
  bool dcAligned,
  bool allowClipping)
{
  /**********************************************************************/
  /* ownerDrawDc is not conveyor primitive. Therefore, we must, process */
  /* all rendering transforms by ourselves                              */
  /**********************************************************************/
  OdGeMatrix3d eyeToOutput = eyeToOutputTransform();
  OdGePoint3d originXformed(eyeToOutput * origin);
  OdGeVector3d uXformed(eyeToOutput * u), vXformed(eyeToOutput * v);

  m_pDumper->output(L"ownerDrawDc() is called");
  
  /**********************************************************************/
  /* It's shown only in 2d mode, but we don't know the mode             */
  /**********************************************************************/
  {
    OdString s;
    
    s.format(L"Begin ownerDrawDc, origin: %ls)", ::toString(originXformed).c_str());
    m_pDumper->output(s);
    
    s.format(L"  u: %ls, v: %ls", ::toString(uXformed).c_str(), ::toString(vXformed).c_str());
    m_pDumper->output(s);

    s.format(L"  dcAligned: %ls, allowClipping: %ls", ::toString(dcAligned).c_str(), ::toString(allowClipping).c_str());
    m_pDumper->output(s);
  }
}
