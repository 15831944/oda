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
#include "DbSystemServices.h"
#include "DbHostAppServices.h"

#include "OdGeoDataPEImpl.h"
#include "OdDbGeoCoordinateSystem.h"
#include "Ge/GeMatrix3d.h"
#include <tinyxml.h>

ODRX_CONS_DEFINE_MEMBERS(OdDbGeoDataPEImpl, OdDbGeoDataPE, RXIMPL_CONSTR);

OdDbGeoDataPEImpl::~OdDbGeoDataPEImpl()
{
}

OdResult OdDbGeoDataPEImpl::transformFromLonLatAlt(const OdDbGeoData* pGeoData, const double & dblLongitude, const double & dblLatitude, const double & dblAltitude,
                                                   double & dblDwgX, double & dblDwgY, double & dblDwgZ)
{
  if (pGeoData == NULL)
  {
    return eNullPtr;
  }
  
  OdGePoint3d res;
  OdDbGeoCoordinateSystemTransformerPtr pTransformer;
  if (eOk == pGeoData->getSimpleTransformerFromLLA(pTransformer))
  {
    OdGePoint3d lla(dblLongitude, dblLatitude, dblAltitude);
    if (eOk == pTransformer->transformPoint(lla, res))
    {
      if (pGeoData->coordinateType() == OdDbGeoData::kCoordTypLocal)
      {
        res.transformBy(pGeoData->getMatrixLocalFromLLA());
      }
      dblDwgX = res.x;
      dblDwgY = res.y;
      dblDwgZ = res.z;
      return eOk;
    }
  }

  if (pGeoData->numMeshPoints() == 0 || pGeoData->numMeshFaces() == 0)
  {
    OdGePoint3d lla(dblLongitude, dblLatitude, dblAltitude);
    lla.transformBy(pGeoData->getMatrixFromLLA());
    res = lla + pGeoData->designPoint().asVector();
    dblDwgX = res.x;
    dblDwgY = res.y;
    dblDwgZ = res.z;
    return eOk;
  }

  OdGePoint2d ptXY;
  OdResult errCode = pGeoData->meshTransformFromLLA(OdGePoint2d(dblLongitude, dblLatitude), ptXY);
  dblDwgX = ptXY.x;
  dblDwgY = ptXY.y;
  dblDwgZ = dblAltitude;
  return errCode;
}


OdResult OdDbGeoDataPEImpl::transformToLonLatAlt(const OdDbGeoData* pGeoData, const double & dblDwgX, const double & dblDwgY, const double & dblDwgZ,
                                                 double & dblLongitude, double & dblLatitude, double & dblAltitude)
{
  if(pGeoData == NULL)
  {
    return eNullPtr;
  }

  OdGePoint3d res;
  OdDbGeoCoordinateSystemTransformerPtr pTransformer;
  if (eOk == pGeoData->getSimpleTransformerToLLA(pTransformer))
  {
    OdGePoint3d ptOld(dblDwgX, dblDwgY, dblDwgZ);
    if (pGeoData->coordinateType() == OdDbGeoData::kCoordTypLocal)
    {
      ptOld.transformBy(pGeoData->getMatrixLocalToLLA());
    }

    if (eOk == pTransformer->transformPoint(ptOld, res))
    {
      dblLongitude = res.x;
      dblLatitude = res.y;
      dblAltitude = res.z;
      return eOk;
    }
  }
  
  if (pGeoData->numMeshPoints() == 0 || pGeoData->numMeshFaces() == 0)
  {
    OdGePoint3d ptOld(dblDwgX, dblDwgY, dblDwgZ);
    res = ptOld - pGeoData->designPoint().asVector();
    res.transformBy(pGeoData->getMatrixToLLA());

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

    dblLongitude = res.x;
    dblLatitude = res.y;
    dblAltitude = res.z;
    return eOk;
  }

  OdGePoint2d ptGeo;
  OdResult errCode = pGeoData->meshTransformToLLA(OdGePoint2d(dblDwgX, dblDwgY), ptGeo);
  dblLongitude = ptGeo.x;
  dblLatitude = ptGeo.y;
  dblAltitude = dblDwgZ;
  return errCode;
}

bool OdDbGeoDataPEImpl::validateCs(const OdString & coordinateSystem)
{
  //validation presented in OdDbGeoCoordinateSystem.h classes
  //this function is for user validation stuff
  return true;
}

OdResult OdDbGeoDataPEImpl::parseCsName(const OdDbGeoData* pGeoData, const OdString & sInputName, OdString & sResultName)
{
  if(pGeoData == NULL)
  {
    return eNullPtr;
  }
  if (sInputName.isEmpty())
  {
    sResultName.empty();
    return eOk;
  }
  OdDbGeoCoordinateSystemPtr pCs;
  OdResult status = OdDbGeoCoordinateSystem::create(sInputName, pCs);
  if (status != eOk)
  {
    return status;
  }

  return pCs->getXmlRepresentation(sResultName);
}

OdResult OdDbGeoDataPEImpl::geoCoordinateSystemWillChange(OdDbGeoData* pGeoData, const OdString & newName)
{
  if(pGeoData == NULL)
  {
    return eNullPtr;
  }
  return eOk;
}

OdResult OdDbGeoDataPEImpl::geoCoordinateSystemChanged(OdDbGeoData* pGeoData, const OdString & oldName)
{
  if(pGeoData == NULL)
  {
    return eNullPtr;
  }
  return eOk;
}

OdString OdDbGeoDataPEImpl::parseIdFromXml(const OdString & xml)
{
  TiXmlDocument doc;
  doc.Parse(xml, 0, TIXML_ENCODING_UTF8);
  TiXmlHandle docHandle(&doc);
  
  TiXmlElement * el = docHandle.FirstChild("Dictionary").FirstChild().ToElement();
  if(el == NULL)
  {
    return OdString();
  }
  return el->Attribute("id");
}
