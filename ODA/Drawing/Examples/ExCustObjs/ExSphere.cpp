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

/************************************************************************/
/* ExSphere.cpp: implementation of the ExSphere class, a custom         */
/* OdDbEntity                                                           */                                 
/************************************************************************/

#include "OdaCommon.h"
#include "ExSphere.h"
#include "Gi/GiWorldDraw.h"
#include "DbFiler.h"
#include "DbCommandContext.h"
#include "DbDatabase.h"
#include "DbBlockTableRecord.h"
#include "DbProxyEntity.h"
#include "UInt8Array.h"
#include "Int32Array.h"
#include "CmEntityColorArray.h"
#include "DbUserIO.h"

const int ExSphere::lastKnownVersion = 1;

ODRX_DXF_DEFINE_MEMBERS(ExSphere,                                                              // ClassName
                        OdDbEntity,                                                            // ParentClass
                        DBOBJECT_CONSTR,                                                       // DOCREATE
                        OdDb::vAC15,                                                           // DwgVer
                        OdDb::kMRelease0,                                                      // MaintVer
                        OdDbProxyEntity::kAllAllowedBits ^ OdDbProxyEntity::kTransformAllowed, // nProxyFlags
                        EXSPHERE,                                                              // DxfName
                        ExCustObjs|Description: Teigha Run-time Extension Example)          // AppName


ExSphere::ExSphere()
{
}

ExSphere::~ExSphere()
{
}

/************************************************************************/
/* Read the DWG format data of this object from the specified file.     */
/************************************************************************/
OdResult ExSphere::dwgInFields(OdDbDwgFiler* pFiler)
{
  /**********************************************************************/
  /* Read the fields common to all entities                             */
  /**********************************************************************/
  OdResult res = OdDbEntity::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  /**********************************************************************/
  /* Read the version of the ExSphere, and indicate create if a proxy   */
  /* image is required.                                                 */
  /**********************************************************************/
  int nVersion = pFiler->rdInt16();
  if (nVersion > lastKnownVersion)
    return eMakeMeProxy;

  /**********************************************************************/
  /* Read and set the remaining properties unique to ExSphere.          */
  /**********************************************************************/
  setCenter(pFiler->rdPoint3d());
  setRadius(pFiler->rdDouble());
  return eOk;
}

/************************************************************************/
/* Write the DWG format data of this object to the specified file.      */
/************************************************************************/
void ExSphere::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  /**********************************************************************/
  /* Write the fields common to all entities.                           */
  /**********************************************************************/
  OdDbEntity::dwgOutFields(pFiler);
  /**********************************************************************/
  /* Write the properties unique to ExSphere.                           */
  /**********************************************************************/
  pFiler->wrInt16((OdInt16)getVersion());
  pFiler->wrPoint3d(center());
  pFiler->wrDouble(radius());
}

/************************************************************************/
/* Read the DXF format data of this object from the specified file.     */
/************************************************************************/
OdResult ExSphere::dxfInFields(OdDbDxfFiler* pFiler)
{
  /**********************************************************************/
  /* Read the fields common to all entities                             */
  /**********************************************************************/
  OdResult res = OdDbEntity::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(desc()->name()))
    return eBadDxfSequence;

  /**********************************************************************/
  /* Read the version of the ExSphere, and indicate create a proxy      */
  /* is required.                                                       */
  /**********************************************************************/
  int nVersion = pFiler->nextRb()->getInt16();
  if (nVersion > lastKnownVersion)
    return eMakeMeProxy;

  /**********************************************************************/
  /* Read and Set the remaining properties unique to ExSphere.          */
  /**********************************************************************/
  setCenter(pFiler->nextRb()->getPoint3d());
  setRadius(pFiler->nextRb()->getDouble());
  return eOk;
}

/************************************************************************/
/* Write the DXF format data of this object to the specified file.      */
/************************************************************************/
void ExSphere::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  /**********************************************************************/
  /* Write the fields common to all entities.                           */
  /**********************************************************************/
  OdDbEntity::dxfOutFields(pFiler);

  /**********************************************************************/
  /* Write the properties unique to ExSphere.                           */
  /**********************************************************************/
  pFiler->wrSubclassMarker(desc()->name());
  pFiler->wrInt16(70, (OdInt16)getVersion());
  pFiler->wrPoint3d(10, center());
  pFiler->wrDouble(40, radius());
}

/************************************************************************/
/* Creates a viewport-independent geometric representation of the       */
/* specified OdGeSurface object.                                        */
/************************************************************************/
void buildMesh(OdGiWorldDraw* pWd, const OdGeSurface& surf)
{
  OdGeInterval U, V;
  surf.getEnvelope(U, V);
  /**********************************************************************/
  /* Create a 20 x 20 mesh                                              */
  /**********************************************************************/
  int nu = 20, nv = 20; 

  OdGiEdgeData edgeData;
  OdUInt8Array edgeVis(nu * nv);
  double du = (U.upperBound() - U.lowerBound()) / nu, dv = (V.upperBound() - V.lowerBound()) / nv; 

  OdInt32Array faceList(nu * nv * 5);
  ++nu;
  ++nv;
  OdGePoint3dArray points(nu * nv);
  OdGiVertexData vertexData;
  OdGeVector3dArray vertexNormals(nu * nv);
  vertexNormals.resize(nu * nv);
  vertexData.setNormals(vertexNormals.getPtr());
  OdGeVector3dArray derivatives;

  /**********************************************************************/
  /* Create the face list                                               */
  /**********************************************************************/
  OdGePoint2d param;
  param.y = V.lowerBound();
  int n = 0;
  for(int v=0; v<nv; ++v, param.y+=dv)
  {
    param.x = U.lowerBound();
    for(int u=0; u<nu; ++u, param.x+=du)
    {
      points.append(surf.evalPoint(param, 0, derivatives, vertexNormals[n]));
      if( v > 0 && u > 0)
      {
        faceList.push_back(4);
        faceList.push_back(n-1-nu);
        faceList.push_back(n  -nu);
        faceList.push_back(n);
        faceList.push_back(n-1);
      }
      ++n;
    }
  }
  /**********************************************************************/
  /* Populate the Vertex Colors                                         */
  /**********************************************************************/
  OdCmEntityColorArray vertexColors;
  OdCmEntityColor vertexColor;
  OdUInt32 colorStep = 0xFF000000 / n;
  OdUInt32 red = 0xFF000000, green = 0, blue = 0;
  vertexColors.reserve(n);
  for(int i=0; i<n; ++i)
  {
    vertexColor.setRGB(OdUInt8(red >> 24), OdUInt8(green >> 24), OdUInt8(blue >> 24));
    vertexColors.append(vertexColor);
    red   -= colorStep;
    green += colorStep;
    //blue  += colorStep;
  }
  ODA_ASSERT(OdUInt32(nu * nv) == points.size());
  /**********************************************************************/
  /* Set the edge visibilities                                       */
  /**********************************************************************/
  edgeVis.insert(edgeVis.end(), nu * nv * 4, kOdGiSilhouette);
  edgeData.setVisibility(edgeVis.getPtr());

  /**********************************************************************/
  /* Set the vertex orientations and colors                             */
  /**********************************************************************/
  vertexData.setOrientationFlag(kOdGiClockwise);
  vertexData.setTrueColors(vertexColors.getPtr());
  
  /**********************************************************************/
  /* Vectorize the mesh                                                 */
  /**********************************************************************/
  pWd->geometry().shell(n, points.getPtr(), faceList.size(), faceList.getPtr(), &edgeData, 0, &vertexData);
}

/************************************************************************/
/* Creates a viewport-independent geometric representation of the       */
/* specified OdGeSurface object.                                        */
/************************************************************************/
bool ExSphere::subWorldDraw(OdGiWorldDraw* pWd) const
{
  ::buildMesh(pWd, m_impl);
  return true;
}

/************************************************************************/
/* Properties unique to ExSphere                                        */
/************************************************************************/
OdGePoint3d ExSphere::center() const
{
  assertReadEnabled();
  return m_impl.center();
}

void ExSphere::setCenter(const OdGePoint3d& center)
{
  assertWriteEnabled();
  m_impl.set(m_impl.radius(), center);
}

double ExSphere::radius() const
{
  assertReadEnabled();
  return m_impl.radius();
}

void ExSphere::setRadius(double rad)
{
  assertWriteEnabled();
  m_impl.set(rad, m_impl.center());
}


/************************************************************************/
/* Create ExSphere command                                              */
/************************************************************************/
void _ExCreateSphere_func(OdEdCommandContext* pCmdCtx)
{
  /**********************************************************************/
  /* Establish command context, database, and UserIO                    */
  /**********************************************************************/
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  
  /**********************************************************************/
  /* Open Model Space for writing                                       */
  /**********************************************************************/
  OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Create the ExSphere                                                */
  /**********************************************************************/
  ExSpherePtr pSphere = ExSphere::createObject();

  /**********************************************************************/
  /* Set the default properties common all entities                     */
  /**********************************************************************/
  pSphere->setDatabaseDefaults(pDb);

  /**********************************************************************/
  /* Set the properties unique to ExSphere.                             */
  /**********************************************************************/
  pSphere->setCenter( pIO->getPoint(OD_T("\nSpecify center of sphere: ")) );
  pSphere->setRadius( pIO->getDist(OD_T("\nSpecify radius of sphere: ")) );

  /**********************************************************************/
  /* Add the ExSphere to Model Space                                    */
  /**********************************************************************/
  pMS->appendOdDbEntity(pSphere);
}
