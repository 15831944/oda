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

#ifndef __ODGISELECTPROC__
#define __ODGISELECTPROC__

#include "Gi/GiConveyorNode.h"
#include "Gs/Gs.h"
#include "Ge/GeDoubleArray.h"
#include "Ge/GePoint2dArray.h"

class OdGiDeviation;
class OdGiConveyorContext;

#include "TD_PackPush.h"

/** \details
  This class implements conveyor nodes performing selections.

  \remarks
  Behavior of this node depends on the selection mode.
  *  If the mode is "fence", the points specify polyline vertices.
  *  If the mode is "crossing" or "window", the points specify polygon vertices.
     If exactly two points are specified, the polygon is rectangle, with the
     points opposite vertices.

  For any input primitive, this node performs the following actions:
  1.  It checks for primitive intersections with the polygon border.
  2.  If intersections are found, the node MARKS current drawable, and stops to
      handle the primitive.
  3.  If the mode is mode is "crossing", and the primitive is inside the
      polygon, the node node MARKS the current drawable and stops to handle the
      primitive.
  4.  If the mode is "window", and primitive is outside the polygon, the node
      MARKS the current drawable and stops to handle the primitive.

  To MARK the current drawable is means to currentDrawableDesc()->markBySelection(true).
  If the drawable is neither a compound object nor uses nesting, means currentDrawableDesc()->markToSkip(true).

  This conveyor node is used for implementation OdGsBaseVectorizeView::select()
  and does not produce any output.

  <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiSelectProc : public OdGiConveyorNode
{
public:
  enum
  {
    kCheckMarkerOnChange         = 1,
    kCheckMarkerOnViewModeChange = 2,

    kCheckMarkerFinish           = 0x80000000
  };
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdGiSelectProc);
  //DOM-IGNORE-END

  /** \details
    Sets the boundary that forms selection area, selection mode and reactor.

    \param pPoints  [in]  Pointer to an array of points that form selection area.
    \param nPoints  [in]  Number of points in array.
    \param mode     [in]  Selection mode.
    \param pReactor [in]  Pointer to the selection reactor.
  */
  virtual void set(const OdGePoint2d* pPoints,
                   OdUInt32 nPoints,
                   OdGsView::SelectionMode mode, OdGsSelectionReactor* pReactor) = 0;

  /** \details
    Retrieves the boundary that forms selection area, selection mode.

    \param points [out]  Array of points that form the selection area.
    \param mode   [out]  Selection mode.
  */
  virtual void get(OdGePoint2dArray& points,
                   OdGsView::SelectionMode& mode) = 0;

  virtual void check_n_fire_selected(OdUInt32 flags = 0) = 0;

  /** \details
    Sets max deviation for curve tessellation.

    \param deviations [in]  Array with deviation values.
  */
  virtual void setDeviation(const OdGeDoubleArray& deviations) = 0;

  /** \details
    Sets deviation object to obtain max deviation for curve tessellation.
  
    \param pDeviation [in]  Pointer to the deviation object.
  */
  virtual void setDeviation(const OdGiDeviation* pDeviation) = 0;

  /** \details
    Sets the draw context object (to access to traits, etc).

    \param pDrawCtx [in]  Pointer to the draw context.
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawCtx) = 0;

  /** \details
    Checks whether selection reactor and selection mode support subentity
    selection.

    \param pView    [in]  Pointer to the graphic view.
    \param mode     [in]  Selection mode. Currently not used.
    \param pReactor [in]  Pointer to the selection reactor.
    \returns
    True if subentity selection is supported for the selection reactor, false
    otherwise.
  */
  static bool supportsSubentitySelection(const OdGiViewport *pView, OdGsView::SelectionMode mode, OdGsSelectionReactor* pReactor);

  /** \details
    Checks that selection can be performed by checking extents without playing
    content.
    \param extWc [in] Data extents.
    \returns True if content should be selected without playing; otherwise
    returns false.
  */
  virtual bool handleSelectionByExtents( const OdGeExtents3d& extWc ) { return false; }

};

/** \details
  A data type that represents a smart pointer to an
  <link OdGiSelectProc, OdGiSelectProc> object.
*/
typedef OdSmartPtr<OdGiSelectProc> OdGiSelectProcPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGISELECTPROC__
