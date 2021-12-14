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
#include "RxObjectImpl.h"
#include "ExPointCloudHostPE.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiViewportDraw.h"
//#include "Ge/GePoint3d.h"

#include "../Extensions/ExServices/OdFileBuf.h"
#include "DbHostAppServices.h"
#include "OdPlatformStreamer.h"

ODRX_CONS_DEFINE_MEMBERS(ExPointCloudItem, OdDbPointCloudItem, RXIMPL_CONSTR);


OdGeExtents3d ExPointCloudItem::extents() const
{
  return m_extents;
}

OdInt64 ExPointCloudItem::pointsCount() const
{
  return m_points.size();
}

bool ExPointCloudItem::worldDrawPoints(const OdDbPointCloud* pEnt, OdGiWorldDraw* pWd) const
{
  drawPoints(pWd->geometry());
  return true;
}

void ExPointCloudItem::viewportDrawPoints(const OdDbPointCloud* pEnt, OdGiViewportDraw* pWd) const
{
//  drawPoints(pWd->geometry());
}

void ExPointCloudItem::drawPoints(OdGiGeometry& geom) const
{
  OdInt32 nCount = (OdInt32)m_points.size();
  const OdGePoint3d* pPt = m_points.getPtr();
  geom.polypoint(nCount, pPt);
}

/////////////////////////////////////////////////////////////////////
ODRX_NO_CONS_DEFINE_MEMBERS(ExPointCloudHostPE, OdDbPointCloudHostPE);

OdResult ExPointCloudHostPE::load(const OdString& filename, OdDbPointCloudItemPtr& pItem)
{
  if(!::odSystemServices()->accessFile(filename, Oda::kFileRead))
    return eCantOpenFile;

  OdRdFileBuf pcgFile(filename);

  char cMagic[3];
  pcgFile.getBytes(cMagic, 3);
  if (cMagic[0] != 'P' || cMagic[1] != 'C' || cMagic[2] != 'G')
    return eUnsupportedFileFormat;
    
  // read pcg file version
  OdUInt16 pcgFileVer = OdPlatformStreamer::rdInt16(pcgFile);

  int nExtentsOffset;
  int nPointDataSize;
  switch (pcgFileVer) {
  case 3:
    nExtentsOffset = 7;
    nPointDataSize = 4 + 3 * sizeof(float);
    break;
  case 4:
    nExtentsOffset = 19;
    nPointDataSize = 8 + 3 * sizeof(float);
    break;
  default:
    return eInvalidFileVersion;
  }

  pcgFile.seek(nExtentsOffset, OdDb::kSeekFromStart);

  // extents reading
  OdGePoint3d p1, p2;

  double x = OdPlatformStreamer::rdDouble(pcgFile);
  double y = OdPlatformStreamer::rdDouble(pcgFile);
  double z = OdPlatformStreamer::rdDouble(pcgFile);
  p1.set(x, y, z);

  x = OdPlatformStreamer::rdDouble(pcgFile);
  y = OdPlatformStreamer::rdDouble(pcgFile);
  z = OdPlatformStreamer::rdDouble(pcgFile);
  p2.set(x, y, z);

  // translation reading
  OdGeVector3d translation;
  x = OdPlatformStreamer::rdDouble(pcgFile);
  y = OdPlatformStreamer::rdDouble(pcgFile);
  z = OdPlatformStreamer::rdDouble(pcgFile);
  translation.set(x, y, z);

  ExPointCloudItemImplPtr exItem = ExPointCloudItem::createObject();
  exItem->m_extents.set(p1, p2);

  OdGePoint3d point;

  //start reading points data from end of the file,
  //because we dont know where points data is started
  unsigned nRestDataLength = pcgFile.length() - pcgFile.tell();
  OdArray<OdUInt8> buf;
  buf.resize(nRestDataLength);

  OdUInt8 * pData = buf.asArrayPtr();
  pcgFile.getBytes(pData, nRestDataLength);

  OdUInt8* pRead = pData + nRestDataLength;
  exItem->m_points.reserve(nRestDataLength / nPointDataSize);

// Temp float variables are necessary because of possible alignment problems
  float coords[3];
  OdUInt32 * pCoords = (OdUInt32*) coords;
  while(true)
  {
    pRead -= nPointDataSize;
    ODA_ASSUME(sizeof(float) == 4);
    memcpy(pCoords, pRead, 3 * sizeof(float));
    // Where are no platform streamer operations or macros for "float" yet
    // So take care about bytes ordering manually for now
    odSwap4BytesNumber(*pCoords);
    odSwap4BytesNumber(*(pCoords + 1));
    odSwap4BytesNumber(*(pCoords + 2));

    // if the points run out, exit from the reading cycle
    if ((coords[0] == 0.0 && coords[1] == 0.0 && coords[2] == 0.0) || pRead <= pData)
      break;

    //sum coords with translation coords and set it to point
    point.set(coords[0] + translation.x, coords[1] + translation.y, coords[2] + translation.z);
    exItem->m_points.push_back(point);
  }
  pItem = exItem;
  return eOk;
}

