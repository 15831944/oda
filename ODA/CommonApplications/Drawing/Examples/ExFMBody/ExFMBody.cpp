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
#include "ExFMBody.h"

#include "DbFiler.h"
#include "Gi/GiWorldDraw.h"
#include "DbProxyEntity.h"
#include "StaticRxObject.h"
#include "Modeler/FMMdlIterators.h"
#include "FMDataSerialize.h"
#include "MemoryStream.h"
#include "FlatMemStream.h"
#include "NextCodeDefs.h"
#include "CmTransparencyArray.h"
#include <memory>
#include "Wr/wrTriangulationParams.h"
#include "DbSubDMesh.h"
#include "Modeler/FMDrawBody.h"

// Usually ODRX_DEFINE_MEMBERS_EX is enough, but to declare custom properties 
// we use the most complicated class registration form

ODRX_DEFINE_INIT_MEMBERS(ExFmBody,                                                  // ClassName
                        OdDbEntity,                                                     // ParentClass
                        ExFmBody::pseudoConstructor,                                // DOCREATE
                        OdDb::vAC15,                                                    // DwgVer
                        OdDb::kMRelease0,                                               // MaintVer
                        OdDbProxyEntity::kAllButCloningAllowed,                         // NProxyFlags
                        L"ExFmBody",                                                // DWG class name
                        L"EXFMBODY",                                                // DxfName
                        L"ExFMBody|Description: Teigha Run-time Extension Example",   // AppName
                        OdRx::kMTLoading | OdRx::kMTRender | OdRx::kMTRenderInBlock, 0, 0   // Custom flags
                        
);
ODRX_DEFINE_RTTI_MEMBERS(ExFmBody, OdDbEntity)
ODRX_DEFINE_PSEUDOCONSTRUCTOR(ExFmBody, DBOBJECT_CONSTR)

const OdInt16 ExFmBody::lastKnownVersion = 1;
const int ExFmBody::tagSection = 123123;
const int ExFmBody::tagTransparant = 456456;

ExFmBody::ExFmBody()
{
}

ExFmBody::~ExFmBody()
{
}

OdInt16 ExFmBody::getVersion()
{
  return lastKnownVersion;
}

OdResult ExFmBody::subGetGeomExtents(OdGeExtents3d& ext) const
{
  assertReadEnabled();
  ext = OdGeExtents3d::kInvalid;
  for (unsigned int i = 0; i < m_aBody.logicalLength(); ++i)
  {
    ext.addExt(m_aBody[i].interval());
  }
  return eOk;
}

/************************************************************************/
/* Read the DWG format data of this object from the specified file.     */
/************************************************************************/
OdResult ExFmBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbEntity::dwgInFields(pFiler);
  if (res != eOk)
  {
    return res;
  }
  int nVersion = pFiler->rdInt16();
  if (nVersion > lastKnownVersion)
  {
    return eMakeMeProxy;
  }

  try
  {
    OdInt32 nBody = pFiler->rdInt32();
    OdInt32 streamLen = pFiler->rdInt32();
    std::auto_ptr<char> buffer(new char[streamLen]);
    pFiler->rdBytes(buffer.get(), streamLen);
    OdStreamBufPtr memStream = OdFlatMemStream::createNew(buffer.get(), streamLen);
    FacetModeler::BinaryStream sin;
    sin.Open(memStream);
    m_aBody.setLogicalLength(nBody);
    for (int i = 0; i < nBody; ++i)
    {
      sin.Read(m_aBody[i]);
    }
  }
  catch (...)
  {
    return eMakeMeProxy;
  }

  return eOk;
}

/************************************************************************/
/* Write the DWG format data of this object to the specified file.      */
/************************************************************************/
void ExFmBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbEntity::dwgOutFields(pFiler);

  pFiler->wrInt16(getVersion());
  pFiler->wrInt32(m_aBody.logicalLength());
  OdStreamBufPtr memStream = OdMemoryStream::createNew();
  FacetModeler::BinaryStream sout;
  sout.Create(memStream);
  for (unsigned int i = 0; i < m_aBody.logicalLength(); ++i)
  {
    sout.Write(m_aBody[i]);
  }
  memStream->rewind();
  OdInt32 streamLen = memStream->length();
  std::auto_ptr<char> buffer(new char[streamLen]);
  memStream->getBytes(buffer.get(), streamLen);
  pFiler->wrInt32(streamLen);
  pFiler->wrBytes(buffer.get(), streamLen);
}

OdResult ExFmBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbEntity::dxfInFields(pFiler);
  if (res != eOk)
    return res;
  if (!pFiler->atSubclassData(desc()->name()))
    return eBadDxfSequence;

  OdInt32 streamLen = 0, nBody = 0;
  NEXT_CODE(70);
  int nVersion = pFiler->rdInt16();
  if (nVersion > lastKnownVersion)
  {
    return eMakeMeProxy;
  }

  NEXT_CODE(90);
  nBody = pFiler->rdInt32();

  NEXT_CODE(90);
  streamLen = pFiler->rdInt32();

  OdBinaryData binData;
  binData.reserve(streamLen);
  OdBinaryData chunk;
  while (binData.size() < (unsigned int)streamLen)
  {
    NEXT_CODE(310);
    pFiler->rdBinaryChunk(chunk);
    binData.append(chunk);
  }

  try
  {
    OdStreamBufPtr memStream = OdFlatMemStream::createNew(binData.begin(), binData.logicalLength());
    FacetModeler::BinaryStream sin;
    sin.Open(memStream);
    m_aBody.setLogicalLength(nBody);
    for (int i = 0; i < nBody; ++i)
    {
      sin.Read(m_aBody[i]);
    }
  }
  catch (...)
  {
    return eMakeMeProxy;
  }

  return eOk;
}

/************************************************************************/
/* Write the DXF format data of this object to the specified file.      */
/************************************************************************/
void ExFmBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbEntity::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(desc()->name());
  pFiler->wrInt16(70, getVersion());
  pFiler->wrInt32(90, m_aBody.logicalLength());

  OdStreamBufPtr memStream = OdMemoryStream::createNew();
  FacetModeler::BinaryStream sout;
  sout.Create(memStream);
  for (unsigned int i = 0; i < m_aBody.logicalLength(); ++i)
  {
    sout.Write(m_aBody[i]);
  }
  memStream->rewind();
  OdInt32 streamLen = memStream->length();
  std::auto_ptr<OdUInt8> buffer(new OdUInt8[streamLen]);
  memStream->getBytes(buffer.get(), streamLen);

  pFiler->wrInt32(90, streamLen);
  pFiler->wrBinaryChunk(310, buffer.get(), streamLen);
}

static void bodyGenMesh(OdGePoint3dArray& aVxFace, OdInt32Array& aFaceIndex, OdIntArray& tags, const FacetModeler::Body& body)
{
  aVxFace.clear();
  aFaceIndex.clear();

  FacetModeler::FaceIterator itF(&body);
  for (; !itF.done(); itF.next())
  {
    FacetModeler::Face* pFace = itF.get();

    const OdGePlane& plane = pFace->plane();
    FacetModeler::Profile2D profile;
    pFace->projection(plane, profile);
    for (unsigned int i = 0; i < profile.size(); ++i)
    {
      const FacetModeler::Contour2D& contour = profile[i];
      const int nVt = contour.numVerts();
      const bool bHole = !contour.isCCW();
      aFaceIndex.push_back(bHole ? -nVt : nVt);
      for (int j = 0; j < nVt; ++j)
      {
        const int iVx = aVxFace.logicalLength();
        aVxFace.resize(iVx + 1);
        OdGePoint2d pt;
        contour.getVertexAt(j, &pt);
        aVxFace[iVx] = plane.evalPoint(pt);
        aFaceIndex.push_back(iVx);
      }
    }

    int color = pFace->color();
    int tag;
    if (pFace->tag() == ExFmBody::tagSection)
    {
      tag = ExFmBody::tagSection;
    }
    else
    {
      tag = color;
    }
    tags.push_back(tag);
  }
}

/************************************************************************/
/* Creates a viewport-independent geometric representation              */
/************************************************************************/
bool ExFmBody::subWorldDraw(OdGiWorldDraw * pWd) const
{
  assertReadEnabled();

  if (pWd->regenAbort())
  {
    return true;
  }

  const unsigned int nBody = m_aBody.logicalLength();

  switch (pWd->regenType())
  {
  case kOdGiHideOrShadeCommand:
  case kOdGiRenderCommand:
  case kOdGiSaveWorldDrawForProxy:
  {
#if 1
    for (unsigned int i = 0; i < m_aBody.logicalLength(); ++i)
    {
      FacetModeler::DrawBody(pWd, m_aBody[i]);
    }
#else
    for (unsigned int i = 0; i < nBody; ++i)
    {
      OdGePoint3dArray vertices;
      OdInt32Array faces;
      OdIntArray tags;
      bodyGenMesh(vertices, faces, tags, m_aBody[i]);

      OdUInt16Array aColors;
      aColors.setLogicalLength(tags.logicalLength());
      OdCmTransparencyArray faceTransparencies;
      faceTransparencies.setLogicalLength(tags.logicalLength());

      const bool bTransparent = m_aBody[i].tag() == ExFmBody::tagTransparant;
      for (unsigned int i = 0; i < tags.length(); i++)
      {
        faceTransparencies[i] = bTransparent ? 0.5 : 1.;
        if (tags[i] == tagSection)
        {
          faceTransparencies[i] = 0.5;
          aColors[i] = OdCmEntityColor::kACIRed;
        }
        else
        {
          aColors[i] = tags[i];
        }
      }

      OdGiFaceData faceData;
      faceData.setColors(aColors.getPtr());
      faceData.setTransparency(faceTransparencies.getPtr());
      pWd->geometry().shell(vertices.length(), vertices.asArrayPtr(), faces.length(), faces.asArrayPtr(), 0, &faceData);
    }
#endif
  }
  break;
  case kOdGiStandardDisplay:
  {
    for (unsigned int i = 0; i < nBody; ++i)
    {
      FacetModeler::EdgeBodyIterator itE(&m_aBody[i]);
      while (!itE.done())
      {
        FacetModeler::Edge* pEdge = itE.get();
        if (pEdge != NULL)
        {
          OdGePoint3d aVx[2] = { pEdge->startPoint(), pEdge->endPoint() };

          OdUInt32 fmColor = pEdge->face()->color();
          if (fmColor != 0)
          {
            OdCmEntityColor color;
            color.setColorIndex(fmColor);
            pWd->subEntityTraits().setTrueColor(color);
          }
          pWd->geometry().polyline(2, aVx);
        }
        itE.next();
      }
    }
  }
  break;
  case kOdGiForExtents:
  {
    for (unsigned int i = 0; i < nBody; ++i)
    {
      const unsigned int nVt = m_aBody[i].vertexCount();
      OdGePoint3dArray aPt(nVt);
      FacetModeler::VertexIterator itV(&m_aBody[i]);
      while (!itV.done())
      {
        FacetModeler::Vertex* pVertex = itV.get();
        aPt.push_back(pVertex->point());
        itV.next();
      }
      pWd->geometry().polyline(nVt, aPt.begin());
    }
  }
  break;
  default:
    return false;
  }

  return true;
}

OdResult ExFmBody::subTransformBy(const OdGeMatrix3d& xform)
{
  assertWriteEnabled();
  for (unsigned int i = 0; i < m_aBody.logicalLength(); ++i)
  {
    m_aBody[i].transform(xform);
  }
  xDataTransformBy(xform);
  return eOk;
}

void ExFmBody::setBody(const FacetModeler::Body& body)
{
  assertWriteEnabled();
  m_aBody.setLogicalLength(1);
  m_aBody[0] = body;
}

void ExFmBody::setBodies(const OdArray<FacetModeler::Body>& bodies)
{
  assertWriteEnabled();
  m_aBody = bodies;
}

void ExFmBody::getBodies(OdArray<FacetModeler::Body>& bodies) const
{
  assertReadEnabled();
  bodies = m_aBody;
}

unsigned int ExFmBody::nBodies() const
{
  assertReadEnabled();
  return m_aBody.logicalLength();
}

OdResult ExFmBody::readBody(OdStreamBuf* pStreamBuff)
{
  assertWriteEnabled();
  FacetModeler::BinaryStream sin;
  if (!sin.Open(pStreamBuff))
  {
    return eInvalidInput;
  }

  FacetModeler::Body bodyAux;
  try
  {
    sin.Read(bodyAux);
  }
  catch (...)
  {
    return eInvalidInput;
  }

  setBody(bodyAux);
  return eOk;
}

OdResult ExFmBody::readBody(const OdString& fName)
{
  assertWriteEnabled();
  FacetModeler::BinaryStream sin;
  if (!sin.Open(fName))
  {
    return eInvalidInput;
  }

  FacetModeler::Body bodyAux;
  try
  {
    sin.Read(bodyAux);
  }
  catch (...)
  {
    return eInvalidInput;
  }

  setBody(bodyAux);
  return eOk;
}

void ExFmBody::setColor(OdUInt32 color)
{
  assertWriteEnabled();
  for (unsigned int i = 0; i < m_aBody.logicalLength(); ++i)
  {
    m_aBody[i].setColor(color);
  }
}

ExFmBodyPtr ExFmBody::boolOper(const ExFmBody& body, FacetModeler::BooleanOperation operation) const
{
  assertReadEnabled();

  OdArray<FacetModeler::Body> bodiesA;
  getBodies(bodiesA);
  OdArray<FacetModeler::Body> bodiesB;
  body.getBodies(bodiesB);

  FacetModeler::Body opA, opB;
  for (unsigned int i = 0; i < bodiesA.logicalLength(); ++i)
  {
    opA = opA.combine(bodiesA[i]);
  }
  for (unsigned int i = 0; i < bodiesB.logicalLength(); ++i)
  {
    opB = opB.combine(bodiesB[i]);
  }

  FacetModeler::Body result = FacetModeler::Body::boolOper(operation, opA, opB, true);

  ExFmBodyPtr pResult = ExFmBody::createObject();
  pResult->setBody(result);
  return pResult;
}

ExFmBodyPtr ExFmBody::slice(const OdGePlane& cutter, FacetModeler::TagType tag /*= 0*/) const
{
  assertReadEnabled();

  OdArray<FacetModeler::Body> bodies;
  getBodies(bodies);
  OdArray<FacetModeler::Body> bodiesResult;
  for (unsigned int j = 0; j < bodies.logicalLength(); ++j)
  {
    FacetModeler::Body result;
    bodies[j].slice(cutter, result, tag);
    if (result.faceCount() > 0)
    {
      *bodiesResult.append() = result;
    }
  }

  ExFmBodyPtr pResult = ExFmBody::createObject();
  pResult->setBodies(bodiesResult);
  return pResult;
}

void ExFmBody::copyBodiesFrom(const ExFmBody& body)
{
  assertWriteEnabled();
  OdArray<FacetModeler::Body> bodies;
  body.getBodies(bodies);
  m_aBody = bodies;
}

void ExFmBody::convertBodiesFrom(const OdArray<OdDb3dSolidPtr>& aSolid, double surfaceTol)
{
  wrTriangulationParams params(true);
  OdDbFaceterSettings faceter;
  faceter.faceterDevNormal = params.normalTolerance;
  faceter.faceterGridRatio = params.gridAspectRatio;
  faceter.faceterMaxEdgeLength = params.maxFacetEdgeLength;
  faceter.faceterMaxGrid = params.maxNumGridLines;
  faceter.faceterDevSurface = params.surfaceTolerance = surfaceTol;

  OdArray<FacetModeler::Body> bodiesResult;
  for (unsigned int i = 0; i < aSolid.logicalLength(); ++i)
  {
    OdDb3dSolidPtr pSolid = aSolid[i];
    OdGePoint3dArray vertexArray;
    OdInt32Array faceArray;
    OdGiFaceData* pFaceData = NULL;
    if (oddbGetObjectMesh(pSolid, &faceter, vertexArray, faceArray, pFaceData) == eOk)
    {
      FacetModeler::Body body = FacetModeler::Body::createFromMesh(std::vector<OdGePoint3d>(vertexArray.begin(), vertexArray.end()),
        std::vector<OdInt32>(faceArray.begin(), faceArray.end()));
      if (!body.isNull())
      {
        *bodiesResult.append() = body;
      }
      delete pFaceData;
    }
  }

  setBodies(bodiesResult);
}

void ExFmBody::enableShadedRendering(double angleTol)
{
  assertWriteEnabled();
  for (unsigned int i = 0; i < m_aBody.logicalLength(); ++i)
  {
    FacetModeler::ClearShadedSurfaces(m_aBody[i]);
    if (!OdZero(angleTol))
    {
      FacetModeler::MarkShadedSurfaces(m_aBody[i], angleTol, false); 
    }
  }
}

void ExFmBody::simplification(double param)
{
  assertWriteEnabled();
  for (unsigned int i = 0; i < m_aBody.logicalLength(); ++i)
  {
    m_aBody[i].simplify(param);
  }
}

int ExFmBody::numFaces() const
{
  assertReadEnabled();
  int nFaces = 0;
  for (unsigned int i = 0; i < m_aBody.logicalLength(); ++i)
  {
    nFaces += m_aBody[i].faceCount();
  }
  return nFaces;
}
