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

#include "OdBingMaps.h"

namespace OdBingMaps
{
  void GoogleBingToWGS84Mercator(double x, double y, double& lat, double& lon)
  {
    lon = (x / EarthRadius / OdaPI) * 180;
    lat = (y / EarthRadius / OdaPI) * 180;
    lat = 180 / OdaPI * (2 * atan(exp(lat * OdaPI / 180)) - OdaPI / 2);
  }

  void WGS84ToGoogleBing(double lat, double lon, double& x, double& y)
  {
    x = lon * EarthRadius * OdaPI / 180;
    y = log(tan((90 + lat) * OdaPI / 360)) / (OdaPI / 180);
    y = y * EarthRadius * OdaPI / 180;
  }

  double Clip(double n, double minValue, double maxValue)
  {
    return odmin(odmax(n, minValue), maxValue);
  }

  unsigned int MapSize(int levelOfDetail)
  {
    return (unsigned int)TILE_SIZE << levelOfDetail;
  }

  double GroundResolution(double latitude, int levelOfDetail)
  {
    latitude = Clip(latitude, MinLatitude, MaxLatitude);
    return cos(latitude * OdaPI / 180) * 2 * OdaPI * EarthRadius / MapSize(levelOfDetail);
  }

  double MapScale(double latitude, int levelOfDetail, int screenDpi)
  {
    return GroundResolution(latitude, levelOfDetail) * screenDpi / 0.0254;
  }

  void LatLongToPixelXY(double latitude, double longitude, int levelOfDetail, OdInt32& pixelX, OdInt32& pixelY)
  {
    latitude = Clip(latitude, MinLatitude, MaxLatitude);
    longitude = Clip(longitude, MinLongitude, MaxLongitude);

    double x = (longitude + 180) / 360;
    double sinLatitude = sin(latitude * OdaPI / 180);
    double y = 0.5 - log((1 + sinLatitude) / (1 - sinLatitude)) / (4 * OdaPI);

    unsigned int mapSize = MapSize(levelOfDetail);
    pixelX = Clip(x * mapSize + 0.5, 0, mapSize - 1);
    pixelY = Clip(y * mapSize + 0.5, 0, mapSize - 1);
  }

  void PixelXYToLatLong(OdInt32 pixelX, OdInt32 pixelY, int levelOfDetail, double& latitude, double& longitude)
  {
    double mapSize = MapSize(levelOfDetail);
    double x = (Clip(pixelX, 0, mapSize - 1) / mapSize) - 0.5;
    double y = 0.5 - (Clip(pixelY, 0, mapSize - 1) / mapSize);

    latitude = 90 - 360 * atan(exp(-y * 2 * OdaPI)) / OdaPI;
    longitude = 360 * x;
  }

  void PixelXYToTileXY(OdInt32 pixelX, OdInt32 pixelY, OdInt32& tileX, OdInt32& tileY)
  {
    tileX = pixelX / TILE_SIZE;
    tileY = pixelY / TILE_SIZE;
  }

  void TileXYToPixelXY(OdInt32 tileX, OdInt32 tileY, OdInt32& pixelX, OdInt32& pixelY)
  {
    pixelX = tileX * TILE_SIZE;
    pixelY = tileY * TILE_SIZE;
  }

  void TileXYToLatLong(OdInt32 iTileX, OdInt32 iTileY, int levelOfDetail, double& latitude, double& longitude)
  {
    OdInt32 nPixelX, nPixelY;
    TileXYToPixelXY(iTileX, iTileY, nPixelX, nPixelY);
    PixelXYToLatLong(nPixelX, nPixelY, levelOfDetail, latitude, longitude);
  }

  OdAnsiString TileXYToQuadKey(OdInt32 tileX, OdInt32 tileY, int levelOfDetail)
  {
    OdAnsiString quadKey;
    for (int i = levelOfDetail; i > 0; --i)
    {
      char digit = '0';
      int mask = 1 << (i - 1);
      if ((tileX & mask) != 0)
      {
        digit++;
      }
      if ((tileY & mask) != 0)
      {
        digit += 2;
      }
      quadKey += digit;
    }
    return quadKey;
  }

  void QuadKeyToTileXY(const OdString& quadKey, OdInt32& tileX, OdInt32& tileY, int& levelOfDetail)
  {
    tileX = tileY = 0;
    levelOfDetail = quadKey.getLength();
    for (int i = levelOfDetail; i > 0; --i)
    {
      int mask = 1 << (i - 1);
      switch (quadKey[levelOfDetail - i])
      {
      case '0':
        break;

      case '1':
        tileX |= mask;
        break;

      case '2':
        tileY |= mask;
        break;

      case '3':
        tileX |= mask;
        tileY |= mask;
        break;

      default:
        break;
      }
    }
  }

  void LatLongToTileXY(double latitude, double longitude, int levelOfDetail, OdInt32& iTileX, OdInt32& iTileY)
  {
    OdInt32 iPixelX, iPixelY;
    LatLongToPixelXY(latitude, longitude, levelOfDetail, iPixelX, iPixelY);
    PixelXYToTileXY(iPixelX, iPixelY, iTileX, iTileY);
  }

  bool isTileValid(int nLOD, OdInt32 nTileX, OdInt32 nTileY)
  {
    if (nTileX < 0 || nTileY < 0)
    {
      return false;
    }
    OdInt32 nTilesCountPerSide = 1 << nLOD;
    return nTileX < nTilesCountPerSide && nTileY < nTilesCountPerSide;
  }

  void CorrectGeoPt(OdGePoint3d& res)
  {
    //X corrections
    if (fabs(res.x) > 180.)
    {
      double dX = res.x;
      if (dX <= 0.)
      {
        if (dX < -180.)
        {
          dX += 360.;
        }
      }
      else if (dX > 180.)
      {
        dX -= 360.;
      }
      res.x = dX;
    }

    //Y corrections
    if (fabs(res.y) > 90.)
    {
      double dY = res.y;
      if (dY <= 0.)
      {
        if (dY >= -270.)
        {
          if (dY < -90.)
          {
            dY = -180. - dY;
          }
        }
        else
        {
          dY += 360.;
        }
      }
      else if (dY >= 90.)
      {
        if (dY >= 270.)
        {
          dY -= 360.;
        }
        else
        {
          dY = 180. - dY;
        }
      }
      res.y = dY;
    }
  }

} //OdBingMaps namespace
