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

#include "MemoryStream.h"
#include "Db3dSolid.h"
#include "DbBlockReference.h"
#include "DbCommandContext.h"
#include "DbDatabase.h"
#include "DbBlockTableRecord.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdCommandStack.h"
#include "FMDataSerialize.h"
#include "Gs/GsModel.h"
#include "DbCircle.h"
#include "DbSubDMesh.h"
#include "../../../../Components/Imports/STLImport/Include/STLImport.h"
#include "Ge/GeTrMeshSimplification.h"

static void FMCreateBody(OdEdCommandContext* pCmdCtx, ExFmBody* pMyEnt, int color = 0)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();

  OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject(OdDb::kForWrite);

  if (color != 0)
  {
    pMyEnt->setColor(color);
  }

  struct InsertionPointTracker : OdStaticRxObject<OdEdPointTracker>
  {
    ExFmBodyPtr _ent;
    OdGePoint3d _origin;
    const ExFmBody& _body;
    InsertionPointTracker(const ExFmBody& body, OdDbDatabase* pDb)
      : _body(body)
    {
      _ent = ExFmBody::createObject();
      _ent->setDatabaseDefaults(pDb);
    }

    virtual void setValue(const OdGePoint3d& value)
    {
      OdGeMatrix3d mat;
      mat.setToTranslation(value.asVector());
      _ent->copyBodiesFrom(_body);
      _ent->transformBy(mat);
    }

    virtual int addDrawables(OdGsView* pView)
    {
      pView->add(_ent, 0);
      return 1;
    }

    virtual void removeDrawables(OdGsView* pView)
    {
      pView->erase(_ent);
    }
  }
  trackerInsertion(*pMyEnt, pDb);

  OdGePoint3d pPoint = OdGePoint3d();
  OdGePoint3d ptInsert = pIO->getPoint(OD_T("Enter insertion point: "), OdEd::kGdsFromLastPoint, &pPoint, OdString::kEmpty, &trackerInsertion);
  pMyEnt->transformBy(OdGeMatrix3d::translation(ptInsert.asVector()));

  struct ScaleTracker : OdStaticRxObject<OdEdRealTracker>
  {
    ExFmBodyPtr _ent;
    OdGePoint3d _origin;
    const ExFmBody& _body;
    ScaleTracker(const ExFmBody& body, OdGePoint3d& origin, OdDbDatabase* pDb)
      : _body(body)
      , _origin(origin)
    {
      _ent = ExFmBody::createObject();
      _ent->setDatabaseDefaults(pDb);
    }

    virtual void setValue(double r)
    {
      OdGeMatrix3d mat;
      mat.setToScaling(r, _origin);
      _ent->copyBodiesFrom(_body);
      _ent->transformBy(mat);
    }

    virtual int addDrawables(OdGsView* pView)
    {
      pView->add(_ent, 0);
      return 1;
    }

    virtual void removeDrawables(OdGsView* pView)
    {
      pView->erase(_ent);
    }
  }
  trackerScale(*pMyEnt, ptInsert, pDb);

  double scale = pIO->getDist(OD_T("\nSpecify scale: "), OdEd::kGdsFromLastPoint, 1, OdString::kEmpty, &trackerScale);
  pMyEnt->transformBy(OdGeMatrix3d::scaling(scale, ptInsert));

  pMyEnt->setDatabaseDefaults(pDb);
  pMS->appendOdDbEntity(pMyEnt);
}

#include "../examples/FMCreate/BodyExample.cpp" // TODO: export from FMCreate sample

void _FMCreateBody_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();

  const int iModel = pIO->getInt(OD_T("\nSpecify predefined model index (1-14): ")) - 1;
  if (iModel < kCube || iModel > kMeshTetra)
  {
    pIO->putError(OD_T("Wrong number"));
    return;
  }

  FacetModeler::DeviationParams devParams;
  OdStreamBufPtr memStream = OdMemoryStream::createNew();
  CreateBodyExample(devParams, memStream, CreationMode(iModel));
  memStream->rewind();

  ExFmBodyPtr pMyEnt = ExFmBody::createObject();
  if (pMyEnt->readBody(memStream) != eOk)
  {
    pIO->putError("Failed to read body");
    return;
  }
  pMyEnt->transformBy(OdGeMatrix3d::scaling(0.003));

  FMCreateBody(pCmdCtx, pMyEnt, OdCmEntityColor::kACIRed + iModel % 7);
}

void _FMReadBody_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();

  OdString fName = pIO->getFilePath("Enter file name:");

  ExFmBodyPtr pMyEnt = ExFmBody::createObject();

  if (fName.right(4).makeLower() == OD_T(".stl"))
  {
    STLFileImport::OdStlImportModulePtr stlModule = odrxDynamicLinker()->loadModule(OdStlImportModuleName);
    STLFileImport::OdStlImportPtr stlImport = stlModule->create();

    stlImport->import(fName);

    OdGePoint3dArray vertices;
    OdUInt32Array faces;
    OdGeVector3dArray normals;
    stlImport->getShell(vertices, faces, normals);

    std::vector<OdGePoint3d> vertVect(vertices.begin(), vertices.end());
    std::vector<OdInt32> faceData(faces.begin(), faces.end());
    FacetModeler::Body body = FacetModeler::Body::createFromMesh(vertVect, faceData);

    if(body.faceCount() == 0)
    {
      pIO->putError("Failed to create body from STL");
      return;
    }

    pMyEnt->setBody(body);
  }
  else if (pMyEnt->readBody(fName) != eOk)
  {
    pIO->putError("Failed to read body");
    return;
  }

  OdArray<FacetModeler::Body> bodies;
  pMyEnt->getBodies(bodies);
  if (bodies.isEmpty())
  {
    pIO->putString("WARNING! Empty body is read.");
  }
  else if (!bodies[0].isClosed())
  {
    pIO->putString("WARNING! Open body is read.");
  }

  FMCreateBody(pCmdCtx, pMyEnt);
}

#if 0 // for internal usage
void _FMReadFolder_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();

#ifdef _MSC_VER
  OdString rootPath = pIO->getString("Enter folder path");
  if (rootPath.getAt(rootPath.getLength() - 1) != '\\')
  {
    rootPath += '\\';
  }

  WIN32_FIND_DATA findData;
  HANDLE dir;
  dir = ::FindFirstFile(rootPath + L"*.*", &findData);
  if (dir == INVALID_HANDLE_VALUE)
  {
    pIO->putError("Failed to open the folder");
    return;
  }

  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject(OdDb::kForWrite);

  OdArray<FacetModeler::Body> bodies;
  for (BOOL resFind = TRUE; resFind; resFind = FindNextFile(dir, &findData))
  {
    if (findData.cFileName == OdString(L".") || findData.cFileName == OdString(L".."))
    {
      continue;
    }
    if (findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
    {
      continue;
    }
    OdString fName = findData.cFileName;

    try
    {
      FacetModeler::BinaryStream sin;
      sin.Open(rootPath + OdString(findData.cFileName));
      FacetModeler::Body* body = bodies.append();
      sin.Read(*body);
      /*if (body->faceCount() > 0 && body->faceList()->color() == 0)
      {
        if (fName.find(OD_T("railing")) != -1)
        {
          body->setColor(OdCmEntityColor::kACIMagenta);
        }
        else if (fName.find(OD_T("slab")) != -1)
        {
          body->setColor(OdCmEntityColor::kACIGreen);
        }
        else
        {
          body->setColor(OdCmEntityColor::kACIYellow);
        }
      }*/
    }
    catch (...)
    {
      pIO->putError("Failed to read file");
      return;
    }
  }

  ExFmBodyPtr pMyEnt = ExFmBody::createObject();
  pMyEnt->setDatabaseDefaults(pDb);

  pMyEnt->setBodies(bodies);

  pMS->appendOdDbEntity(pMyEnt);
#else
  pIO->putError("This command works only at Windows platform");
#endif
}
#endif

void _FMBodyBoolean_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();

  OdDbSelectionSetIteratorPtr pIter = pIO->select(OD_T("Select two FM bodies"))->newIterator();
  OdArray<ExFmBodyPtr> aBody;
  for (; !pIter->done(); pIter->next())
  {
    ExFmBodyPtr body = ExFmBody::cast(pIter->objectId().openObject(OdDb::kForWrite));
    if (body.isNull())
      continue;
    aBody.push_back(body);
  }

  if (aBody.logicalLength() != 2)
  {
    pIO->putError("Two FM bodies must be selected");
    return;
  }

  const int iOp = pIO->getInt(OD_T("\nSpecify type of operation (0 - eUnion, 1 - eIntersection, 2 - eDifference, 3 - eXOR): "));

  ExFmBodyPtr pMyEnt = aBody[0]->boolOper(*aBody[1], FacetModeler::BooleanOperation(iOp));

  aBody[0]->erase();
  aBody[1]->erase();

  OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject(OdDb::kForWrite);
  pMyEnt->setDatabaseDefaults(pDb);
  pMS->appendOdDbEntity(pMyEnt);
}

void _FMBodySection_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();

  OdDbSelectionSetIteratorPtr pIter = pIO->select(OD_T("Select FM bodies"))->newIterator();
  OdArray<ExFmBodyPtr> aBody;
  OdArray<OdDbObjectId> aBodyId;
  for (; !pIter->done(); pIter->next())
  {
    ExFmBodyPtr body = ExFmBody::cast(pIter->objectId().openObject(OdDb::kForWrite));
    if (body.isNull())
      continue;
    aBody.push_back(body);
    aBodyId.push_back(pIter->objectId());
  }

  if (aBody.logicalLength() == 0)
  {
    pIO->putError("No bodies are selected");
    return;
  }

  OdAnsiString sAxis;
  sAxis = pIO->getString(OD_T("\nSpecify section plane normal (X, Y, Z): "));
  if (sAxis.getLength() != 1)
  {
    pIO->putError("Wrong axis");
    return;
  }
  sAxis.makeLower();
  char axis = sAxis.getAt(0);

  OdGeExtents3d ext;
  aBody[0]->getGeomExtents(ext);
  OdGePlane plane;
  plane.set(ext.center(), axis == 'x' ? OdGeVector3d::kXAxis : (axis == 'y' ? OdGeVector3d::kYAxis : OdGeVector3d::kZAxis));

  struct PlaneTracker : OdStaticRxObject<OdEdPointTracker>
  {
    const OdArray<ExFmBodyPtr>& _aBody;
    OdGePlane _plane;
    OdArray<ExFmBodyPtr> _ent;
    OdGePoint3d _origin;
    OdArray<OdDbCirclePtr> _circle;

    PlaneTracker(const OdArray<ExFmBodyPtr>& aBody, const OdGePlane& plane, OdDbDatabase* pDb)
      : _aBody(aBody)
    {
      _plane = plane;
      OdGeVector3d u, v;
      plane.get(_origin, u, v);
      const int nBody = _aBody.logicalLength();
      _ent.setLogicalLength(nBody);
      _circle.setLogicalLength(nBody);
      for (int i = 0; i < nBody; ++i)
      {
        OdGeExtents3d ext;
        aBody[i]->getGeomExtents(ext);
        _ent[i] = ExFmBody::createObject();
        _ent[i]->setDatabaseDefaults(pDb);
        _circle[i] = OdDbCircle::createObject();
        _circle[i]->setDatabaseDefaults(pDb);
        _circle[i]->setNormal(_plane.normal());
        _circle[i]->setRadius(ext.diagonal().length() * 0.5);
        _circle[i]->setCenter(ext.center());
      }
    }

    virtual void setValue(const OdGePoint3d& pt)
    {
      OdGePlane planeMove;
      planeMove.set(_origin, _plane.normal());
      double r = planeMove.signedDistanceTo(pt);

      OdGePoint3d center = _origin + _plane.normal() * r;
      _plane.set(center, _plane.normal());

      for (unsigned int i = 0; i < _ent.logicalLength(); ++i)
      {
        ExFmBodyPtr pSlice = _aBody[i]->slice(_plane, ExFmBody::tagSection);
        _ent[i]->copyBodiesFrom(*pSlice);
        OdGePoint3d circCenter;
        _plane.project(_circle[i]->center(), circCenter);
        _circle[i]->setCenter(circCenter);
      }
    }

    virtual int addDrawables(OdGsView* pView)
    {
      for (unsigned int i = 0; i < _ent.logicalLength(); ++i)
      {
        pView->add(_circle[i], 0);
        pView->add(_ent[i], 0);
      }
      return 1;
    }

    virtual void removeDrawables(OdGsView* pView)
    {
      for (unsigned int i = 0; i < _ent.logicalLength(); ++i)
      {
        pView->erase(_circle[i]);
        pView->erase(_ent[i]);
      }
    }
  }
  trackerPlane(aBody, plane, pDb);

  for (unsigned int i = 0; i < aBody.logicalLength(); ++i)
  {
    OdDbEntityPtr ent = aBody[i];
    ent->setVisibility(OdDb::kInvisible);
    if (ent->gsNode())
    {
      ent->gsNode()->model()->onModified(ent.get(), ent->ownerId());
    }
  }

  OdGePoint3d ptPlane = pIO->getPoint(OD_T("Specify plane position: "), OdEd::kGdsFromLastPoint | OdEd::kGptNoUCS, 0, OdString::kEmpty, &trackerPlane);
  OdGePlane planeMove;
  planeMove.set(ext.center(), plane.normal());
  double scale = planeMove.signedDistanceTo(ptPlane);
  plane.set(ext.center() + plane.normal() * scale, plane.normal());

  for (unsigned int i = 0; i < aBody.logicalLength(); ++i)
  {
    ExFmBodyPtr pEnt = aBody[i];
    ExFmBodyPtr pMyEnt = pEnt->slice(plane, ExFmBody::tagSection);

    pEnt->erase();

    if (pMyEnt->nBodies() > 0)
    {
      OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject(OdDb::kForWrite);
      pMyEnt->setDatabaseDefaults(pDb);
      pMS->appendOdDbEntity(pMyEnt);
    }
  }
}

void _FMBodyConvert_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();

  OdDbSelectionSetIteratorPtr pIter = pIO->select(OD_T("Select solid entity"))->newIterator();
  OdArray<OdDb3dSolidPtr> aSolid;
  for (; !pIter->done(); pIter->next())
  {
    OdDbEntityPtr pEntity = pIter->objectId().openObject(OdDb::kForWrite);
    if (pEntity.isNull())
    {
      continue;
    }
    if (pEntity->isKindOf(OdDbBlockReference::desc()))
    {
      OdDbBlockReferencePtr pBlockRef = pEntity;
      OdDbBlockTableRecordPtr bTR = pBlockRef->blockTableRecord().openObject(OdDb::kForWrite);
      OdDbObjectIteratorPtr i = bTR->newIterator();
      for (; !i->done(); i->step())
      {
        if (i->entity()->isKindOf(OdDb3dSolid::desc()))
        {
          aSolid.push_back(i->entity());
        }
      }
    }
    else if (pEntity->isKindOf(OdDb3dSolid::desc()))
    {
      aSolid.push_back(pEntity);
    }
  }

  if (aSolid.logicalLength() < 1)
  {
    pIO->putError("No solids are selected");
    return;
  }

  const double surfaceTolerance = pIO->getReal(OD_T("\nSpecify surface tolerance: "));

  ExFmBodyPtr pMyEnt = ExFmBody::createObject();
  pMyEnt->convertBodiesFrom(aSolid, surfaceTolerance);

  for (unsigned int i = 0; i < aSolid.logicalLength(); ++i)
  {
    aSolid[i]->erase();
  }

  pIO->putString(OdString().format(OD_T("Successfully converted %d solids of %d"), aSolid.logicalLength(), pMyEnt->nBodies()));
  OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject(OdDb::kForWrite);
  pMyEnt->setDatabaseDefaults(pDb);
  pMS->appendOdDbEntity(pMyEnt);
}

static FacetModeler::Body createCylinder(const OdGePoint3d& origin, double r, double height, double rHole = -1)
{
  FacetModeler::Contour2D contour;
  contour.appendVertex(origin.convert2d() - OdGeVector2d::kXAxis * r, 1);
  contour.appendVertex(origin.convert2d() + OdGeVector2d::kXAxis * r, 1);
  contour.setClosed();
  contour.makeCCW();

  FacetModeler::Profile2D profile;
  profile.push_back(contour);

  if (rHole > 0)
  {
    FacetModeler::Contour2D hole;
    hole.appendVertex(origin.convert2d() + OdGeVector2d::kXAxis * 15, 1);
    hole.appendVertex(origin.convert2d() - OdGeVector2d::kXAxis * 15, 1);
    hole.setClosed();
    hole.makeCCW(false);
    profile.push_back(hole);
  }

  FacetModeler::DeviationParams dev;
  dev.MinPerCircle = 40;
  FacetModeler::Body res = FacetModeler::Body::extrusion(profile, OdGeVector3d::kZAxis * height, dev);
  res.transform(OdGeMatrix3d::translation(OdGeVector3d(0, 0, origin.z)));
  return res;
}

void _FMBooleanDemo_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();
  OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject(OdDb::kForWrite);

  const OdGePoint3d origin(0, 0, 0);

  OdDbObjectId idAux, idCut, idMain;

  FacetModeler::Body bodyAux = createCylinder(origin + OdGeVector3d(0, 0, 10), 30, 21);
  bodyAux.setColor(OdCmEntityColor::kACIBlue);
  {
    ExFmBodyPtr pAuxEnt = ExFmBody::createObject();
    pAuxEnt->setDatabaseDefaults(pDb);
    pAuxEnt->setBody(bodyAux);
    pMS->appendOdDbEntity(pAuxEnt);
    idAux = pAuxEnt->objectId();
    if (pAuxEnt->gsNode())
    {
      pAuxEnt->gsNode()->model()->onModified(pAuxEnt.get(), pAuxEnt->ownerId());
    }
  }

  pIO->getString("Step 1. Press Enter.");

  FacetModeler::Body bodyCut1 = createCylinder(origin + OdGeVector3d(0, 25, 9), 10, 23);
  bodyCut1.setColor(OdCmEntityColor::kACIRed);
  {
    ExFmBodyPtr pCutEnt = ExFmBody::createObject();
    pCutEnt->setDatabaseDefaults(pDb);
    pCutEnt->setBody(bodyCut1);
    pMS->appendOdDbEntity(pCutEnt);
    idCut = pCutEnt->objectId();
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  pIO->getString("Step 2. Press Enter.");

  {
    ExFmBodyPtr pAuxEnt = idAux.safeOpenObject(OdDb::kForWrite);
    bodyAux = FacetModeler::Body::boolOper(FacetModeler::eDifference, bodyAux, bodyCut1);
    pAuxEnt->setBody(bodyAux);
    ExFmBodyPtr pCutEnt = idCut.safeOpenObject(OdDb::kForWrite);
    pCutEnt->erase();
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  pIO->getString("Step 3. Press Enter.");

  FacetModeler::Body bodyCut2 = createCylinder(origin + OdGeVector3d(0, -25, 9), 10, 23);
  bodyCut2.setColor(OdCmEntityColor::kACIRed);
  {
    ExFmBodyPtr pCutEnt = ExFmBody::createObject();
    pCutEnt->setDatabaseDefaults(pDb);
    pCutEnt->setBody(bodyCut2);
    pMS->appendOdDbEntity(pCutEnt);
    idCut = pCutEnt->objectId();
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  pIO->getString("Step 4. Press Enter.");

  {
    ExFmBodyPtr pAuxEnt = idAux.safeOpenObject(OdDb::kForWrite);
    bodyAux = FacetModeler::Body::boolOper(FacetModeler::eDifference, bodyAux, bodyCut2);
    pAuxEnt->setBody(bodyAux);
    ExFmBodyPtr pCutEnt = idCut.safeOpenObject(OdDb::kForWrite);
    pCutEnt->erase();
    if (pCutEnt->gsNode())
      ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  pIO->getString("Step 5. Press Enter.");

  FacetModeler::Body bodyMain = createCylinder(origin, 40, 30, 15);
  bodyMain.setColor(OdCmEntityColor::kACIYellow);
  bodyMain.setTag(ExFmBody::tagTransparant);
  {
    ExFmBodyPtr pMainEnt = ExFmBody::createObject();
    pMainEnt->setDatabaseDefaults(pDb);
    pMainEnt->setBody(bodyMain);
    pMS->appendOdDbEntity(pMainEnt);
    idMain = pMainEnt->objectId();
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  pIO->getString("Step 6. Press Enter.");

  bodyMain.setTag(0);
  {
    ExFmBodyPtr pMainEnt = idMain.safeOpenObject(OdDb::kForWrite);
    bodyMain = FacetModeler::Body::boolOper(FacetModeler::eDifference, bodyMain, bodyAux);
    pMainEnt->setBody(bodyMain);
    ExFmBodyPtr pAuxEnt = idAux.safeOpenObject(OdDb::kForWrite);
    pAuxEnt->erase();
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  pIO->getString("Step 7. Press Enter.");

  FacetModeler::Body box = FacetModeler::Body::box(OdGePoint3d(-40, -15, 20), OdGeVector3d(80, 30, 11));
  box.setColor(OdCmEntityColor::kACIGreen);
  {
    ExFmBodyPtr pCutEnt = ExFmBody::createObject();
    pCutEnt->setDatabaseDefaults(pDb);
    pCutEnt->setBody(box);
    pMS->appendOdDbEntity(pCutEnt);
    idCut = pCutEnt->objectId();
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  pIO->getString("Step 8. Press Enter.");

  {
    ExFmBodyPtr pMainEnt = idMain.safeOpenObject(OdDb::kForWrite);
    bodyMain = FacetModeler::Body::boolOper(FacetModeler::eDifference, bodyMain, box);
    pMainEnt->setBody(bodyMain);
    ExFmBodyPtr pCutEnt = idCut.safeOpenObject(OdDb::kForWrite);
    pCutEnt->erase();
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  pIO->getString("Step 9. Press Enter.");

  box = FacetModeler::Body::box(OdGePoint3d(-27.5, -5, 5), OdGeVector3d(10, 10, 10));
  box.setColor(OdCmEntityColor::kACIMagenta);
  {
    ExFmBodyPtr pCutEnt = ExFmBody::createObject();
    pCutEnt->setDatabaseDefaults(pDb);
    pCutEnt->setBody(box);
    pMS->appendOdDbEntity(pCutEnt);
    idCut = pCutEnt->objectId();
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  pIO->getString("Step 10. Press Enter.");

  {
    ExFmBodyPtr pMainEnt = idMain.safeOpenObject(OdDb::kForWrite);
    bodyMain = FacetModeler::Body::boolOper(FacetModeler::eDifference, bodyMain, box);
    pMainEnt->setBody(bodyMain);
    ExFmBodyPtr pCutEnt = idCut.safeOpenObject(OdDb::kForWrite);
    pCutEnt->erase();
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  pIO->getString("Step 11. Press Enter.");

  box = FacetModeler::Body::box(OdGePoint3d(+27.5, -5, 5), OdGeVector3d(-10, 10, 10));
  box.setColor(OdCmEntityColor::kACIMagenta);
  {
    ExFmBodyPtr pCutEnt = ExFmBody::createObject();
    pCutEnt->setDatabaseDefaults(pDb);
    pCutEnt->setBody(box);
    pMS->appendOdDbEntity(pCutEnt);
    idCut = pCutEnt->objectId();
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  pIO->getString("Step 12. Press Enter.");

  {
    ExFmBodyPtr pMainEnt = idMain.safeOpenObject(OdDb::kForWrite);
    bodyMain = FacetModeler::Body::boolOper(FacetModeler::eDifference, bodyMain, box);
    pMainEnt->setBody(bodyMain);
    ExFmBodyPtr pCutEnt = idCut.safeOpenObject(OdDb::kForWrite);
    pCutEnt->erase();
    ::odedRegCmds()->executeCommand(L"REGEN", pCmdCtx);
  }

  pIO->putString("Done.");
}

void _FMBodyShaded_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();

  OdDbSelectionSetIteratorPtr pIter = pIO->select(OD_T("Select FM bodies"))->newIterator();
  OdArray<ExFmBodyPtr> aBody;
  for (; !pIter->done(); pIter->next())
  {
    ExFmBodyPtr body = ExFmBody::cast(pIter->objectId().openObject(OdDb::kForWrite));
    if (body.isNull())
      continue;
    aBody.push_back(body);
  }

  if (aBody.logicalLength() == 0)
  {
    pIO->putError("No bodies are selected");
    return;
  }

  double angleTol = pIO->getAngle(OD_T("\nSpecify angle tolerance: "), OdEd::kInpDefault, OdaPI * 0.1, OdString::kEmpty);
  
  for (unsigned int i = 0; i < aBody.logicalLength(); ++i)
  {
    ExFmBodyPtr pEnt = aBody[i];
    pEnt->enableShadedRendering(angleTol);
  }
}

void _FMBodySimplification_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();

  OdDbSelectionSetIteratorPtr pIter = pIO->select(OD_T("Select FM bodies"))->newIterator();
  OdArray<ExFmBodyPtr> aBody;
  for (; !pIter->done(); pIter->next())
  {
    ExFmBodyPtr body = ExFmBody::cast(pIter->objectId().openObject(OdDb::kForWrite));
    if (body.isNull())
      continue;
    aBody.push_back(body);
  }

  if (aBody.logicalLength() == 0)
  {
    pIO->putError("No bodies are selected");
    return;
  }

  const double param = pIO->getDist(OD_T("\nSpecify simplification percent (or tolerance if negative): "), OdEd::kInpDefault, 30., OdString::kEmpty);

  for (unsigned int i = 0; i < aBody.logicalLength(); ++i)
  {
    ExFmBodyPtr pEnt = aBody[i];
    pEnt->simplification(param);
  }
}

void _FMBodyConvert2SubDMesh_func(OdEdCommandContext* pCmdCtx)
{
  // Get user IO
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = (OdDbUserIO*)pDbCmdCtx->userIO();

  // Get selection of entities with FacetModeler bodies inside
  OdDbSelectionSetIteratorPtr pIter = pIO->select(OD_T("Select FM bodies"))->newIterator();
  OdArray<ExFmBodyPtr> aBody;
  for (; !pIter->done(); pIter->next())
  {
    ExFmBodyPtr body = ExFmBody::cast(pIter->objectId().openObject(OdDb::kForWrite));
    if (body.isNull())
      continue;
    aBody.push_back(body);
  }

  if (aBody.logicalLength() == 0)
  {
    pIO->putError("No bodies are selected");
    return;
  }

  // Process entities in loop
  for (unsigned int i = 0; i < aBody.logicalLength(); ++i)
  {
    ExFmBodyPtr body = aBody[i];

    // Get FacetModeler bodies
    OdArray<FacetModeler::Body> bodies;
    body->getBodies(bodies);

    // Process bodies in loop
    for (unsigned int iBody = 0; iBody < bodies.logicalLength(); ++iBody)
    {
      // Generate mesh from body with Body::generateMesh method
      GeMesh::OdGeTrMesh mesh;
      bodies[iBody].generateMesh(mesh);
      if (mesh.m_aTr.isEmpty())
      {
        continue;
      }

      // Convert GeMesh::OdGeTrMesh to faceData and colors arrays
      bool hasColors = false;
      OdInt32Array faceArray;
      std::vector<OdUInt32> faceColors;
      faceArray.reserve(mesh.m_aTr.logicalLength() * 4);
      faceColors.reserve(mesh.m_aTr.logicalLength());
      for (unsigned int j = 0; j < mesh.m_aTr.logicalLength(); ++j)
      {
        const GeMesh::OdGeTr& tr = mesh.m_aTr[j];
        faceArray.push_back(3);
        faceArray.push_back(tr.tr[0]);
        faceArray.push_back(tr.tr[1]);
        faceArray.push_back(tr.tr[2]);
        faceColors.push_back(tr.tagFace);
        if (tr.tagFace != 0)
        {
          hasColors = true;
        }
      }

      // Create OdDbSubDMesh and init with vertices, faceData arrays
      OdDbSubDMeshPtr pMesh = OdDbSubDMesh::createObject();
      OdResult res = pMesh->setSubDMesh(mesh.m_aVx, faceArray, 0); // 0 - no smoothing
      if (res != eOk)
      {
        OdString tmp;
        tmp.format(OD_T("Error of setSubDMesh : %s\n"), 
          OdError(res).description().c_str());
        pIO->putString(tmp);
        continue;
      }

      // Apply colors on faces 
      if (hasColors)
      {
        OdInt32 facesCnt;
        pMesh->numOfFaces(facesCnt);
        ODA_ASSERT(facesCnt == mesh.m_aTr.logicalLength());

        OdDbSubentId subId;
        OdCmColor clr;
        subId.setType(OdDb::kFaceSubentType);
        for (int k = 0; k < facesCnt; ++k)
        {
          if (faceColors[k] == 0)
          {
            continue;
          }
          subId.setIndex(k);
          /* Depending on the nature of the body, it may be necessary
           * to interpret the color differently.
           * For example, call setColor instead of setColorIndex. */
          clr.setColorIndex(static_cast<OdUInt16>(faceColors[k]));
          res = pMesh->setSubentColor(subId, clr);
          if (res != eOk)
          {
            OdString tmp;
            tmp.format(OD_T("Error during setting color for face: %s\n"), 
              OdError(res).description().c_str());
            pIO->putString(tmp);
          }
        }
      }

      // Append created OdDbSubDMesh entity to the ModelSpace
      OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().openObject(OdDb::kForWrite);
      pMesh->setDatabaseDefaults(pDb);
      pMS->appendOdDbEntity(pMesh);
    }

    // Remove original entity
    body->erase();
  }
}
