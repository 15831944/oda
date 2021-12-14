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
#ifndef _OD_DB_GEOMAP_PEIMPL_
#define _OD_DB_GEOMAP_PEIMPL_

#include "DbGeoData.h"
#include "DbGeoMapPE.h"
#include "OdDbGeoDataDrawingPE.h"

#include "TD_PackPush.h"

//OdDbGeoMapPEImpl
class OdDbGeoMapPEImpl : public OdDbGeoMapPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoMapPEImpl);
  virtual OdResult updateMapImage(OdDbGeoMap * pGeoMap, bool bReset);
};

typedef OdSmartPtr<OdDbGeoMapPEImpl> OdDbGeoMapPEImplPtr;

namespace OdDbGeoMapHelper
{
  OdResult getOptimalLOD(double dViewportDiagLengthInPixels, double dMapDiagLength, const OdDbGeoData* pGeoData, OdInt8& LOD);
  OdResult getImageScale(const OdDbGeoData* pGeoData, OdInt8 LOD, double& dScale);
  OdResult getImageSize(double dScale, const OdGePoint3dArray& arrVertices, OdGePoint3d& ptImageBottomLeft, OdInt32& width, OdInt32& height, double& dImageWidth, double& dImageHeight);
  OdResult getMap(const OdDbGeoMap* pGeoMap, OdBinaryData& imgPixelData);
  bool normalizeSize(double dWidth, double dHeight, OdUInt32& nWidth, OdUInt32& nHeight);
  void createLLAExtentsBy4Points(const OdGePoint2dArray& arrInput, OdGePoint2dArray& arrRes);
  bool getPolygonsPoint(const OdGePoint2dArray& arr1, const OdGePoint2dArray& arr2, OdGePoint2d& pt);
}

//OdDbGeoDataChunksPEImpl
class OdDbGeoDataChunksPEImpl : public OdDbGeoDataChunksPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoDataChunksPEImpl);
  virtual ~OdDbGeoDataChunksPEImpl();
  virtual OdResult drawGeoDataChunks(OdGiViewportDraw* pVd) const;
  virtual OdResult drawGeoDataProvidersString(OdGiViewportDraw* pVd) const;
};

typedef OdSmartPtr<OdDbGeoDataChunksPEImpl> OdDbGeoDataChunksPEImplPtr;

//OdDbGeoMapImageCreator
class OdDbGeoMapImageCreator;
typedef OdSmartPtr<OdDbGeoMapImageCreator> OdDbGeoMapImageCreatorPtr;

class OdDbGeoMapImageCreator : public OdGiDrawable
{
private:
  OdDbObjectId m_idGeoMap;

  OdDbGeoMapImageCreator& operator = (const OdDbGeoMapImageCreator&);
  OdGsCache* m_pGsNode;
  OdRefCounter m_nRefCounter;
protected:
  OdDbGeoMapImageCreator();
public:
  void addRef();
  void release();
  long numRefs() const;
  void setGsNode(OdGsCache* pGsNode);
  OdGsCache* gsNode() const;
  bool isPersistent() const;
  OdDbStub* id() const;
  ~OdDbGeoMapImageCreator();
  static OdDbGeoMapImageCreatorPtr createObject();
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw(OdGiViewportDraw* pVd) const;
  OdResult drawGeoMap(OdGiViewportDraw* pVd) const;
  void setGeoMapId(OdDbObjectId id);
};

#include "TD_PackPop.h"

#endif //_OD_DB_GEOMAP_PEIMPL_
