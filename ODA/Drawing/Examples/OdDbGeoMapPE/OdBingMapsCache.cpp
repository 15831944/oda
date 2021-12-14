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

#include "OdBingMapsCache.h"
#include "OdBingMaps.h"
#include "RxRasterServices.h"
#include "FlatMemStream.h"
#include "DbGeoMapPEImpl.h"

//////////////////////////
// OdDbGeoMapTile
//////////////////////////
OdDbGeoMapTile::OdDbGeoMapTile()
  : m_cMapTypeId('a')
  , m_nLOD(1)
  , m_nTileX(0)
  , m_nTileY(0)
{
}

OdDbGeoMapTile::OdDbGeoMapTile(char cMapTypeId, int nLOD, OdInt32 nTileX, OdInt32 nTileY)
  : m_cMapTypeId(cMapTypeId)
  , m_nLOD(nLOD)
  , m_nTileX(nTileX)
  , m_nTileY(nTileY)
{
}

void OdDbGeoMapTile::set(char cMapTypeId, int nLOD, OdInt32 nTileX, OdInt32 nTileY)
{
  m_cMapTypeId = cMapTypeId;
  m_nLOD = nLOD;
  m_nTileX = nTileX;
  m_nTileY = nTileY;
}

void OdDbGeoMapTile::getUrl(OdAnsiString & sUrl) const
{
  OdAnsiString sQuadKey = OdBingMaps::TileXYToQuadKey(m_nTileX, m_nTileY, m_nLOD);
  sUrl.format("http://tiles.virtualearth.net/tiles/%c%s?g=1&shading=hill&n=z", m_cMapTypeId, sQuadKey.c_str());
}

void OdDbGeoMapTile::getParentTile(OdDbGeoMapTile & tile) const
{
  tile.set(m_cMapTypeId, m_nLOD, m_nTileX, m_nTileY);
  tile.makeParentTile();
}

int OdDbGeoMapTile::getLOD() const
{
  return m_nLOD;
}

OdInt32 OdDbGeoMapTile::getX() const
{
  return m_nTileX;
}

OdInt32 OdDbGeoMapTile::getY() const
{
  return m_nTileY;
}

bool OdDbGeoMapTile::makeParentTile()
{
  if (1 == m_nLOD)
  {
    return false;
  }
  --m_nLOD;
  m_nTileX /= 2;
  m_nTileY /= 2;
  return true;
}

bool OdDbGeoMapTile::operator< (const OdDbGeoMapTile& tile) const
{
  if (m_nTileX < tile.m_nTileX)
  {
    return true;
  }
  else if (m_nTileX > tile.m_nTileX)
  {
    return false;
  }

  if (m_nTileY < tile.m_nTileY)
  {
    return true;
  }
  else if (m_nTileY > tile.m_nTileY)
  {
    return false;
  }

  if (m_nLOD < tile.m_nLOD)
  {
    return true;
  }
  else if (m_nLOD > tile.m_nLOD)
  {
    return false;
  }

  if (m_cMapTypeId < tile.m_cMapTypeId)
  {
    return true;
  }

  return false;
}

bool OdDbGeoMapTile::operator== (const OdDbGeoMapTile& tile) const
{
  return m_nTileX == tile.m_nTileX
    && m_nTileY == tile.m_nTileY
    && m_nLOD == tile.m_nLOD
    && m_cMapTypeId == tile.m_cMapTypeId;
}
//////////////////////////
// OdDbGeoMapTile end
//////////////////////////


//////////////////////////
// OdBingMapsCache
//////////////////////////
OdBingMapsCache* g_OdBingMapsCache = NULL;

OdBingMapsCache::OdBingMapsCache()
{
}

OdResult OdBingMapsCache::getTiles(const std::set<OdDbGeoMapTile> & arrTiles, OdArray<OdGiRasterImagePtr>& arrRes)
{
  TD_AUTOLOCK(m_mutex);
  const OdUInt32 nSize = OdUInt32(arrTiles.size());
  arrRes.resize(nSize);

  //do not request images, that are already in cache:
  OdAnsiStringArray arrAbsentAdresses;
  arrAbsentAdresses.reserve(nSize);

  OdArray<OdDbGeoMapTile> arrAbsentTiles;
  arrAbsentTiles.reserve(nSize);

  OdInt32Array arrAbsenceIndexes;
  arrAbsenceIndexes.reserve(nSize);
  OdAnsiString sAdress;
  std::set<OdDbGeoMapTile>::const_iterator pItTiles = arrTiles.begin();
  for (OdUInt32 i = 0; i < nSize; ++i, ++pItTiles)
  {
    pItTiles->getUrl(sAdress);
    Map::iterator pIt = m_map.find(*pItTiles);
    if (m_map.end() == pIt)
    {
      arrAbsentTiles.push_back(*pItTiles);
      arrAbsentAdresses.push_back(sAdress);
      arrAbsenceIndexes.push_back(i);
    }
    else
    {
      //assigne result:
      arrRes[i] = pIt->second;
    }
  }

  OdArray<OdGiRasterImagePtr> arrAbsentImages;
  OdResult res = ProviderHelper::multiRequst(arrAbsentAdresses, arrAbsentImages);
  if (eOk != res)
  {
    return res;
  }

  for (OdUInt32 i = 0; i < arrAbsentImages.size(); ++i)
  {
    OdGiRasterImagePtr pImage = arrAbsentImages[i];

    if (pImage.get())
    {
      OdSmartPtr<OdGiRasterImageDesc> pDesc = OdGiRasterImageDesc::createObject(pImage);
      pImage = pImage->convert(true, 50., 50., 0.0, 0, false, true, false, pDesc);
    }

    //assigne result:
    arrRes[arrAbsenceIndexes[i]] = pImage;

    //put result in cache:
    const OdDbGeoMapTile& tile = arrAbsentTiles[i];
    m_map.insert(Map::value_type(tile, pImage));
    m_deque.push_back(tile);

    //check cache limit:
    if (m_deque.size() > MAX_TILES_COUNT)
    {
      const OdDbGeoMapTile & tile = m_deque.front();
      m_map.erase(tile);
      m_deque.pop_front();
    }
  }

  pItTiles = arrTiles.begin();
  for (OdUInt32 i = 0; i < arrRes.size(); ++i, ++pItTiles)
  {
    if (arrRes[i].isNull())
    {
      // take image subpart of parent tile
      OdArray<OdGiRasterImagePtr> arrBadTilesImages;

      OdDbGeoMapTile tile = *pItTiles;
      OdInt32 nParentTileX = tile.getX();
      OdInt32 nParentTileY = tile.getY();
      OdInt32 nParentSteps = 0;
      do
      {
        if (!tile.makeParentTile())
        {
          return eInternetFileNotFound;
        }
        std::set<OdDbGeoMapTile> arrBadTiles;
        arrBadTiles.insert(tile);
        res = getTiles(arrBadTiles, arrBadTilesImages);
        if (eOk != res)
        {
          return res;
        }
        ++nParentSteps;
      } while (arrBadTilesImages[0].isNull());

      OdGiRasterImagePtr pRaster = arrBadTilesImages[0];

      //crop subImage from parent tile
      OdInt32 chunkDifference = 1 << nParentSteps; // pow(2, nParentSteps)
      OdInt32 pixelX, pixelY;
      OdBingMaps::TileXYToPixelXY(nParentTileX, nParentTileY, pixelX, pixelY);
      OdInt32 chunkSize = OdBingMaps::TILE_SIZE * chunkDifference;
      pixelX %= chunkSize;
      pixelY %= chunkSize;
      pixelX /= chunkDifference;
      pixelY /= chunkDifference;
      pRaster = pRaster->crop(pixelX, pixelY, OdBingMaps::TILE_SIZE / chunkDifference, OdBingMaps::TILE_SIZE / chunkDifference);
      OdSmartPtr<OdGiRasterImageDesc> pDesc = OdGiRasterImageDesc::createObject(pRaster);
      pDesc->setPixelWidth(OdBingMaps::TILE_SIZE);
      pDesc->setPixelHeight(OdBingMaps::TILE_SIZE);

      //assigne result:
      pRaster = pRaster->convert(true, 50., 50., 0.0, 0, false, false, false, pDesc);
      arrRes[i] = pRaster;
    }
  }

  return eOk;
}

//////////////////////////
// OdBingMapsCache end
//////////////////////////


//////////////////////////
// ProviderHelper
//////////////////////////

size_t ProviderHelper::Callback(void* buffer, size_t size, size_t nmemb, void* param)
{
  //param - input to CURLOPT_WRITEDATA option
  OdBinaryData* binData = (OdBinaryData*)param;
  binData->setLogicalLength(binData->logicalLength() + unsigned(size * nmemb));
  memcpy(binData->asArrayPtr() + binData->logicalLength() - size * nmemb, buffer, size * nmemb * sizeof(OdInt8));
  return size * nmemb;
}

OdResult ProviderHelper::multiRequst(const OdAnsiStringArray & addr, OdArray<OdGiRasterImagePtr>& arrRes)
{
  OdResult res = eOk;
  const unsigned int nRequestCount = addr.size();
  OdArray<OdBinaryData> arrBufRes;
  arrBufRes.resize(nRequestCount);

  curl_global_init(CURL_GLOBAL_DEFAULT);

  //array of addresses can be bigger than GEO_CURL_MAX_CONNECTION_NUM, so we must do request in several steps
  for (unsigned int nShift = 0; nShift < nRequestCount; nShift += GEO_CURL_MAX_CONNECTION_NUM)
  {
    const int nConnectionsNumber = odmin(nRequestCount - nShift, GEO_CURL_MAX_CONNECTION_NUM);

    CURL* handles[GEO_CURL_MAX_CONNECTION_NUM];
    /* Allocate one CURL handle per transfer */
    for (int i = 0; i < nConnectionsNumber; ++i)
    {
      CURL* eh = curl_easy_init();
      curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, Callback);
      curl_easy_setopt(eh, CURLOPT_HEADER, 0L);
      curl_easy_setopt(eh, CURLOPT_URL, addr[i + nShift].c_str());
      curl_easy_setopt(eh, CURLOPT_PRIVATE, addr[i + nShift].c_str());
      curl_easy_setopt(eh, CURLOPT_VERBOSE, 0L);
      curl_easy_setopt(eh, CURLOPT_WRITEDATA, &arrBufRes[i + nShift]);
      handles[i] = eh;
    }

    /* init a multi stack */
    CURLM* multi_handle = curl_multi_init();

    //we can optionally limit the total amount of connections this multi handle uses 
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, GEO_CURL_MAX_CONNECTION_NUM);

    /* add the individual transfers */
    for (int i = 0; i < nConnectionsNumber; ++i)
    {
      curl_multi_add_handle(multi_handle, handles[i]);
    }

    /* we start some action by calling perform right away */
    int still_running = 0; /* keep number of running handles */
    curl_multi_perform(multi_handle, &still_running);

    while (still_running)
    {
      fd_set fdread;
      fd_set fdwrite;
      fd_set fdexcep;
      FD_ZERO(&fdread);
      FD_ZERO(&fdwrite);
      FD_ZERO(&fdexcep);

      struct timeval timeout;
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;
      long curl_timeo = -1;

      curl_multi_timeout(multi_handle, &curl_timeo);
      if (curl_timeo >= 0)
      {
        timeout.tv_sec = curl_timeo / 1000;
        if (timeout.tv_sec > 1)
        {
          timeout.tv_sec = 1;
        }
        else
        {
          timeout.tv_usec = (curl_timeo % 1000) * 1000;
        }
      }

      /* get file descriptors from the transfers */
      int maxfd = -1;
      CURLMcode mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

      if (mc != CURLM_OK)
      {
        fprintf(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
        break;
      }

      /* On success the value of maxfd is guaranteed to be >= -1. We call
       select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
       no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
       to sleep 100ms, which is the minimum suggested value in the
       curl_multi_fdset() doc. */

      int rc; /* select() return code */
      if (maxfd == -1)
      {
#ifdef _WIN32
        Sleep(100);
        rc = 0;
#else
        /* Portable sleep for platforms other than Windows. */
        struct timeval wait = { 0, 100 * 1000 }; /* 100ms */
        rc = select(0, NULL, NULL, NULL, &wait);
#endif
      }
      else
      {
        /* Note that on some platforms 'timeout' may be modified by select().
           If you need access to the original value save a copy beforehand. */
        rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
      }

      switch (rc)
      {
      case -1:
        /* select error */
        break;
      case 0: /* timeout */
      default: /* action */
        curl_multi_perform(multi_handle, &still_running);
        break;
      }
    } //while (still_running)

    /* See how the transfers went */
    CURLMsg* msg; /* for picking up messages with the transfer status */
    int msgs_left; /* how many messages are left */
    while ((msg = curl_multi_info_read(multi_handle, &msgs_left)))
    {
      if (msg->msg == CURLMSG_DONE)
      {
        if (CURLE_OK != msg->data.result)
        {
          fprintf(stderr, "HTTP transfer completed with status %d\n", msg->data.result);
          res = eInternetUnknownError;
        }
      }
    }

    curl_multi_cleanup(multi_handle);

    for (int i = 0; i < nConnectionsNumber; ++i)
    {
      curl_easy_cleanup(handles[i]);
    }
  }
  curl_global_cleanup();

  //put Images as result:
  OdRxRasterServicesPtr pRasterSvcs = odrxDynamicLinker()->loadModule(RX_RASTER_SERVICES_APPNAME);
  arrRes.resize(nRequestCount);
  for (OdUInt32 i = 0; i < nRequestCount; ++i)
  {
    const OdBinaryData & imageData = arrBufRes[i];
    if (imageData.size())
    {
      OdStreamBufPtr pStream = OdFlatMemStream::createNew((void *)imageData.asArrayPtr(), imageData.size());
      arrRes[i] = pRasterSvcs->loadRasterImage(pStream);
    }
  }
  return res;
}

OdResult ProviderHelper::simpleRequst(const OdAnsiString& addr, OdBinaryData & buffer)
{
  curl_global_init(CURL_GLOBAL_DEFAULT);
  CURL* curl = curl_easy_init();
  if (!curl)
  {
    return eInvalidContext;
  }
  curl_easy_setopt(curl, CURLOPT_URL, addr.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
  OdResult res = eOk;
  if (CURLE_OK != curl_easy_perform(curl))
  {
    res = eInternetUnknownError;
  }
  curl_easy_cleanup(curl);
  curl_global_cleanup();
  return res;
}

OdResult ProviderHelper::getBrandLogo(OdGiRasterImagePtr & pRasterLogo, bool bReset)
{
  static OdGiRasterImagePtr pRasterLogoGlobal;
  static OdMutex mutex;
  TD_AUTOLOCK(mutex);
  if (bReset || pRasterLogoGlobal.isNull())
  {
    //BrandLogoUri - link from metadata, but it's always the same:
    OdBinaryData logo;
    OdAnsiString imageUrl("http://dev.virtualearth.net/Branding/logo_powered_by.png");
    OdResult res = ProviderHelper::simpleRequst(imageUrl, logo);
    if (eOk != res)
    {
      return res;
    }
    OdRxRasterServicesPtr pRasterSvcs = odrxDynamicLinker()->loadModule(RX_RASTER_SERVICES_APPNAME);
    OdFlatMemStreamPtr pStreamLogo = OdFlatMemStream::createNew((void*)logo.asArrayPtr(), logo.size());
    pRasterLogoGlobal = pRasterSvcs->loadRasterImage(pStreamLogo);
  }
  pRasterLogo = pRasterLogoGlobal->clone();
  return eOk;
}

OdResult ProviderHelper::getProviderMetadata(OdGeoMapType eMapType, OdBinaryData & metadata, bool bReset)
{
  static OdBinaryData metadataAerial; //kAerial
  static OdBinaryData metadataRoad; //kRoad
  static OdBinaryData metadatakHybrid; //kHybrid

  static OdMutex mutex;
  TD_AUTOLOCK(mutex);

  OdString sBingMapsKey;
  odrxSystemServices()->getEnvVar(L"CS_MAP_KEY", sBingMapsKey);
  if (sBingMapsKey.isEmpty())
  {
    ODA_ASSERT_ONCE(!"CS_MAP_KEY is not specified!");
    return eInvalidKey;
  }
  
  OdAnsiString sImageryProvidersUrl;

  switch (eMapType)
  {
  case kAerial:
    if (bReset || metadataAerial.isEmpty())
    {
      metadataAerial.clear();
      sImageryProvidersUrl.format("http://dev.virtualearth.net/REST/v1/Imagery/Metadata/2?incl=ImageryProviders&o=xml&key=%ls", sBingMapsKey.c_str());
      ProviderHelper::simpleRequst(sImageryProvidersUrl, metadataAerial);
    }
    metadata = metadataAerial;
    break;
  case kRoad:
    if (bReset || metadataRoad.isEmpty())
    {
      metadataRoad.clear();
      sImageryProvidersUrl.format("http://dev.virtualearth.net/REST/v1/Imagery/Metadata/0?incl=ImageryProviders&o=xml&key=%ls", sBingMapsKey.c_str());
      ProviderHelper::simpleRequst(sImageryProvidersUrl, metadataRoad);
    }
    metadata = metadataRoad;
    break;
  case kHybrid:
    if (bReset || metadatakHybrid.isEmpty())
    {
      metadatakHybrid.clear();
      sImageryProvidersUrl.format("http://dev.virtualearth.net/REST/v1/Imagery/Metadata/3?incl=ImageryProviders&o=xml&key=%ls", sBingMapsKey.c_str());
      ProviderHelper::simpleRequst(sImageryProvidersUrl, metadatakHybrid);
    }
    metadata = metadatakHybrid;
    break;
  default:
    return eInvalidInput;
  }
  return eOk;
}

OdResult ProviderHelper::getProvidersString(OdGeoMapType eMapType, const OdDbGeoData * pGeoData, int nLOD, const OdGePoint2dArray & arrImageArea, OdStringArray & arrCopyrightStrings)
{
  arrCopyrightStrings.clear();

  OdBinaryData metadata;
  ProviderHelper::getProviderMetadata(eMapType, metadata);
  if (metadata.empty())
  {
    return eInvalidInput;
  }

  TiXmlDocument doc;
  doc.Parse((const char *)metadata.asArrayPtr(), 0, TIXML_ENCODING_UTF8);

  TiXmlHandle docHandle(&doc);
  TiXmlElement* imageryMetadata = docHandle.
    FirstChild("Response").
    FirstChild("ResourceSets").
    FirstChild("ResourceSet").
    FirstChild("Resources").
    FirstChild("ImageryMetadata").
    ToElement();

  if (imageryMetadata)
  {
    double dZ = pGeoData->referencePoint().z;
    OdGePoint2dArray arrExtents;
    arrExtents.resize(4);
    OdGePoint2dArray arrExtentsRes36;
    OdGePoint2d ptIntersect;

    OdDbGeoCoordinateSystemPtr pCS;
    if (eOk != OdDbGeoCoordinateSystem::create(pGeoData->coordinateSystem(), pCS) || pCS.isNull())
    {
      return eInvalidInput;
    }
    OdGeExtents2d geoExts;
    pCS->getGeodeticExtents(geoExts);

    for (TiXmlElement* imageryProvider = imageryMetadata->FirstChildElement("ImageryProvider");
      imageryProvider != NULL;
      imageryProvider = imageryProvider->NextSiblingElement("ImageryProvider"))
    {
      for (TiXmlElement* coverageArea = imageryProvider->FirstChildElement("CoverageArea");
        coverageArea != NULL;
        coverageArea = coverageArea->NextSiblingElement("CoverageArea"))
      {
        int iZoomMin = atoi(coverageArea->FirstChildElement("ZoomMin")->GetText());
        int iZoomMax = atoi(coverageArea->FirstChildElement("ZoomMax")->GetText());
        if (nLOD >= iZoomMin && nLOD <= iZoomMax)
        {
          bool bAddProvider = false;
          for (TiXmlElement* boundingBox = coverageArea->FirstChildElement("BoundingBox");
            boundingBox != NULL;
            boundingBox = boundingBox->NextSiblingElement("BoundingBox"))
          {
            OdGeExtents3d providerExtents;

            double dWestLongitude = odStrToD(boundingBox->FirstChildElement("WestLongitude")->GetText());
            double dSouthLatitude = odStrToD(boundingBox->FirstChildElement("SouthLatitude")->GetText());
            
            double dEastLongitude = odStrToD(boundingBox->FirstChildElement("EastLongitude")->GetText());
            double dNorthLatitude = odStrToD(boundingBox->FirstChildElement("NorthLatitude")->GetText());

            //current CS extent correction
            dWestLongitude = odmin(odmax(dWestLongitude, geoExts.minPoint().x), geoExts.maxPoint().x);
            dSouthLatitude = odmin(odmax(dSouthLatitude, geoExts.minPoint().y), geoExts.maxPoint().y);

            dEastLongitude = odmin(odmax(dEastLongitude, geoExts.minPoint().x), geoExts.maxPoint().x);
            dNorthLatitude = odmin(odmax(dNorthLatitude, geoExts.minPoint().y), geoExts.maxPoint().y);

            arrExtents[0].set(dWestLongitude, dSouthLatitude);
            arrExtents[1].set(dEastLongitude, dSouthLatitude);
            arrExtents[2].set(dEastLongitude, dNorthLatitude);
            arrExtents[3].set(dWestLongitude, dNorthLatitude);
            OdDbGeoMapHelper::createLLAExtentsBy4Points(arrExtents, arrExtentsRes36);

            OdGePoint3d ptBuf;
            for (OdUInt32 i = 0; i < arrExtentsRes36.size(); ++i)
            {
              pGeoData->transformFromLonLatAlt(OdGePoint3d(arrExtentsRes36[i].x, arrExtentsRes36[i].y, dZ), ptBuf);
              arrExtentsRes36[i] = ptBuf.convert2d();
            }

            if (OdDbGeoMapHelper::getPolygonsPoint(arrExtentsRes36, arrImageArea, ptIntersect))
            {
              bAddProvider = true;
              break;
            }
          }

          if (bAddProvider)
          {
            OdAnsiString sImageryProvider = imageryProvider->FirstChildElement("Attribution")->GetText();
            OdCharArray arrBuf;
            OdCharMapper::utf8ToUnicode(sImageryProvider.c_str(), sImageryProvider.getLength(), arrBuf);
            arrCopyrightStrings.append(arrBuf.getPtr());
            break;
          }
        }
      }
    }
  }
  else
  {
    return eInvalidInput;
  }
  return eOk;
}

//////////////////////////
// ProviderHelper end
//////////////////////////
