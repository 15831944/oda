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

#ifndef _OD_BINGMAPS_
#define _OD_BINGMAPS_

#include "OdaCommon.h"
#include "OdAnsiString.h"
#include "Ge/GePoint3d.h"

namespace OdBingMaps
{
  const double EarthRadius = 6378137;
  const double MinLatitude = -85.05112878;
  const double MaxLatitude = 85.05112878;
  const double MinLongitude = -180;
  const double MaxLongitude = 180;
  const OdUInt32 TILE_SIZE = 256;
  const OdInt8 MIN_LOD = 1;
  const OdInt8 MAX_LOD = 21;

  double Clip(double n, double minValue, double maxValue);
  unsigned int MapSize(int levelOfDetail);
  double GroundResolution(double latitude, int levelOfDetail);
  double MapScale(double latitude, int levelOfDetail, int screenDpi);
  void LatLongToPixelXY(double latitude, double longitude, int levelOfDetail, OdInt32& pixelX, OdInt32& pixelY);
  void PixelXYToLatLong(OdInt32 pixelX, OdInt32 pixelY, int levelOfDetail, double& latitude, double& longitude);
  void PixelXYToTileXY(OdInt32 pixelX, OdInt32 pixelY, OdInt32& tileX, OdInt32& tileY);
  void TileXYToPixelXY(OdInt32 tileX, OdInt32 tileY, OdInt32& pixelX, OdInt32& pixelY);
  void TileXYToLatLong(OdInt32 iTileX, OdInt32 iTileY, int levelOfDetail, double& latitude, double& longitude);
  OdAnsiString TileXYToQuadKey(OdInt32 tileX, OdInt32 tileY, int levelOfDetail);
  void QuadKeyToTileXY(const OdString& quadKey, OdInt32& tileX, OdInt32& tileY, int& levelOfDetail);
  void LatLongToTileXY(double latitude, double longitude, int levelOfDetail, OdInt32& iTileX, OdInt32& iTileY);
  void GoogleBingToWGS84Mercator(double x, double y, double& lat, double& lon);
  void WGS84ToGoogleBing(double lat, double lon, double& x, double& y);

  bool isTileValid(int nLOD, OdInt32 tileX, OdInt32 tileY);
  void CorrectGeoPt(OdGePoint3d& res);
};

#endif // _OD_BINGMAPS_
