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

#ifndef _PDFIUMWRAPPERIMPL_INCLUDED_
#define _PDFIUMWRAPPERIMPL_INCLUDED_

#include "TD_PackPush.h"
#include "PdfDirectWrapper.h"

#include "oda_files/oda_functions.h"


/** \details
  Library:  Source code provided.
  <group PdfServices_Classes>
*/
class PdfiumWrapperImpl : public PdfDirectWrapper
{
private:
  FPDF_DOCUMENT     m_pDocument;
  FPDF_ARRAY        m_pONLayers;
  FPDF_ARRAY        m_pOFFLayers;
  FPDF_ARRAY        m_pLayers;

  mutable std::map<OdUInt32, FPDF_PAGE> m_PagesMap;
  mutable bool                          m_bDrawAnnots;
  mutable FPDF_PageLayerInfo            m_LayersCache;

  virtual double getMeasureDictInfo(int nPage) const;
  //void getPageLayerInfo(int nPage) const;
  const OdGePoint2d &getPageSize(int nPage) const;  
  bool checkAnnotationsOnPage(OdUInt32 nPage)  const;
  void setupLayers(const OdArray<OdUnderlayLayer> &layers) const;

  virtual void createGeometryCache(int nPage, const OdArray<OdUnderlayLayer> &Layers) const;

   OdResult sliceImage(const OdDbUnderlayDrawContext& contextWithPaleteFlag, const OdGeExtents2d& clipRect, const OdGiViewportDraw* pVd,
    OdUInt32 numParts, int nPage, ODCOLORREF backgroundColor, double DPI) const;

   virtual OdResult getImage(ImageCache& pdfInfo, int nPage, OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context, OdGeExtents2d& clipRect,
                         double DPI, int ImagePixW, int ImagePixH) const;

public:
  PdfiumWrapperImpl();
  virtual ~PdfiumWrapperImpl();
  ODRX_DECLARE_MEMBERS(PdfiumWrapperImpl);

  virtual OdResult loadFile(OdString path, OdString password);

  virtual OdString getItemName(int nPage) const;
  virtual OdGiRasterImagePtr getThumbnail(int nPage, int width = 170, int height = 170) const;
  virtual OdResult getMediaBox(int nPage, OdGePoint2d& min, OdGePoint2d& max) const;
  virtual void getItemExtents(int nPage, OdGePoint2d& min, OdGePoint2d& max) const;
  virtual int getItemsCount() const;
  
  virtual OdResult getExplodeGeometry(int nPage, OdGiGeometry *pGrabber, const OdArray<OdUnderlayLayer> &Layers, bool bIgnoreImages) const;

  virtual int getLevelsCount(int nPage) const;
  virtual OdString getLevelName(int nPage, int nLevel) const;
  virtual OdUInt8 isLevelEnabled(int nPage, int nLevel) const;
  virtual OdUInt8 isLevelEnabled(int nPage, const OdString &name) const;
  virtual void enableLevel(int nPage, int nLevel, OdUInt8 flag);

  void savePolyline(const OdGePoint2dArray& pt_cache) const;
  void saveSpline(const OdGePoint2dArray& pt_cache) const;
};
typedef OdSmartPtr<PdfiumWrapperImpl> PdfiumWrapperImplPtr;

#include "TD_PackPop.h"

#endif // _PDFIUMWRAPPERIMPL_INCLUDED_
