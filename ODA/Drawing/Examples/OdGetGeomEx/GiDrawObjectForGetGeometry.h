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


#ifndef __OD_GI_DRAW_OBJECT_FOR_GET_GEOMETRY__
#define __OD_GI_DRAW_OBJECT_FOR_GET_GEOMETRY__

#include "Gi/GiBaseVectorizer.h"
#include "SharedPtr.h"
#include "StaticRxObject.h"

#include "../OdVectorizeEx/GiDumper.h"

/************************************************************************/
/* This class is a specialization of the OdGiBaseVectorizer class       */
/* that dumps the vectorization to the specified dumper                 */ 
/************************************************************************/
class OdGiDrawObjectForGetGeometry : public OdGiBaseVectorizer
{
protected:
  /**********************************************************************/
  /* Dumps non-conveyor primitives                                      */
  /**********************************************************************/
  OdGiDumper* m_pDumper; 
public:
  ODRX_HEAP_OPERATORS();

  /**********************************************************************/
  /* This constructor is unusable, use createObject() instead           */
  /**********************************************************************/
  OdGiDrawObjectForGetGeometry();
  virtual ~OdGiDrawObjectForGetGeometry();
  
  /**********************************************************************/
  /* Creates the specified OdGiDrawObjectForGetGeometry object          */
  /**********************************************************************/
  static OdSharedPtr<OdStaticRxObject<OdGiDrawObjectForGetGeometry> > createObject(
    OdGiContext* pUserContext, 
    OdGiConveyorGeometry* pOutGeom, 
    OdGiDumper* pDumper);

  /**********************************************************************/
  /* Setup OdGiContext for this OdGiDrawObjectForGetGeometry object     */
  /**********************************************************************/
  void setContext(OdGiContext* pUserContext);

  /**********************************************************************/
  /*  Pushes a clip boundary onto the current clip stack                */
  /**********************************************************************/
  void pushClipBoundary(OdGiClipBoundary* pBoundary);

  /**********************************************************************/
  /* Pops the topmost clip boundary from the clip stack                 */
  /**********************************************************************/
  void popClipBoundary();

  /**********************************************************************/
  /* Notification function called by the vectorization framework        */
  /* whenever the rendering attributes have changed                     */
  /**********************************************************************/
  void onTraitsModified();
  
  // these functions are called from onTraitsModified
  /**********************************************************************/
  /* The following three functions are called by onTrantsModified       */
  /**********************************************************************/
  
  /**********************************************************************/
  /* Sets the draw color for this object                                */
  /**********************************************************************/
  void draw_color(ODCOLORREF color);
  
  /**********************************************************************/
  /* Sets the draw color index for this object                          */
  /**********************************************************************/
  void draw_color_index(OdUInt16 colorIndex);

  /**********************************************************************/
  /* Sets the draw line width for this object                          */
  /**********************************************************************/
  void draw_lineWidth(OdDb::LineWeight weight);

  // non-conveyor primitive
  /**********************************************************************/
  /* Passes to the rendering framework an non-conveyor primitive that   */
  /*  can render itself.                                                */
  /**********************************************************************/
  void ownerDrawDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiSelfGdiDrawable* pDrawable,
    bool dcAligned,
    bool allowClipping);
};

typedef OdSharedPtr<OdStaticRxObject<OdGiDrawObjectForGetGeometry> > OdGiDrawObjectForGetGeometryPtr;

#endif // __OD_GI_DRAW_OBJECT_FOR_GET_GEOMETRY__
