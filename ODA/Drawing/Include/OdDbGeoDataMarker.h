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

#ifndef OD_DBGEODATAMARKER_H
#define OD_DBGEODATAMARKER_H

#include "Gi/GiDrawable.h"
#include "Gs/GsModel.h"

class OdDbGeoDataMarker;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbGeoDataMarker object pointers.
*/
typedef OdSmartPtr<OdDbGeoDataMarker> OdDbGeoDataMarkerPtr;

/** \details
  OdDbGeoDataMarker is OdDbGeoData location marker.
  It should be drawn only in active viewport, for both model and paper spaces
  when GEOMARKERVISIBILITY == true and GeoData is present in database.
  It shouldn't be drawn in paper space viewport if activeView is overallView.

  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGeoDataMarker : public OdGiDrawable
{
  OdGsViewPtr m_pView; // View in which current object is drawn.
  OdDbGeoDataMarker& operator = (const OdDbGeoDataMarker&);
  OdGsCache* m_pGsNode;
  OdRefCounter m_nRefCounter;
protected:
  OdDbGeoDataMarker();
public:
  void addRef();
  void release();
  long numRefs() const;
  void setGsNode(OdGsCache* pGsNode);
  OdGsCache* gsNode() const;
  bool isPersistent() const;
  OdDbStub* id() const;

  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw(OdGiViewportDraw* pVd) const;

  /** \details
    Virtual destructor.
  */
  ~OdDbGeoDataMarker();

  /** \details
    It is the pseudo-constructor that creates an instance of this class and returns the typified smart pointer to it.
  */
  static OdDbGeoDataMarkerPtr createObject();

  /** \details
    Set view in which current object is drawn.
    
    \param pView [in] Pointer to an OdGsView object.
    \param pGsModel [in] Pointer to an OdGsModel object.
  */
  void setView(OdGsView* pView, OdGsModel* pGsModel);

  /** \details
    Clear current view value.
  */
  void clearView();

  /** \details
    Get view in which current object is drawn.

    \returns Returns OdGsViewPtr or NULL if current view wasn't set.
  */
  OdGsViewPtr getView() const;
};

#endif
