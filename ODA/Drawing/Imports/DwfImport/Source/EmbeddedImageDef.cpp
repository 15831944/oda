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

#include "StdAfx.h"
#include "OdaCommon.h"
#include "DbObject.h"
#include "EmbeddedImageDef.h"
#include "DbSystemServices.h"
#include "DbMaterial.h"
#include "DwfImportImpl.h"

//void OD_TOOLKIT_IMPORT OdDbObject_clearModified(OdDbObject* pObj);

namespace TD_DWF_IMPORT {

EmbeddedImageDef::EmbeddedImageDef()
{
}

EmbeddedImageDef::~EmbeddedImageDef()
{
}

static bool updateTexturePath(OdGiMaterialMap& materialMap,
                              const OdString& sOldPrefix, const OdString& sNewPrefix,
                              bool& bAlreadyDone)
{
  OdString sFileName = materialMap.sourceFileName();
  if (sFileName.isEmpty())
    return false;
  if (!sFileName.find(sNewPrefix))
  {
    bAlreadyDone = true;
    return false;
  }
  if (sFileName.find(sOldPrefix))
    return false;
  sFileName = sNewPrefix + sFileName.mid(sOldPrefix.getLength());
  materialMap.setSourceFileName(sFileName);
  return true;
}

void EmbeddedImageDef::updateTexturePathOfMatrials(OdDbDatabase* pDb, const OdString& sOldPrefix, const OdString& sNewPrefix)
{
  ODA_ASSERT_ONCE_X(TDWF, pDb && !isDBRO());
  if (sNewPrefix == sOldPrefix)
    return;

  OdDbDictionaryPtr pMatDict = pDb->getMaterialDictionaryId().safeOpenObject();
  if (!pMatDict->numEntries())
    return;
  bool bAlreadyDone = false;
  for (OdDbDictionaryIteratorPtr pItr = pMatDict->newIterator(); !bAlreadyDone && !pItr->done(); pItr->next())
  {
    OdDbMaterialPtr pMaterial = pItr->getObject();
    ODA_ASSERT_ONCE_X(TDWF, pMaterial.get());
    OdGiMaterialColor color; OdGiMaterialMap materialMap; double value = 0.0;

    pMaterial->diffuse(color, materialMap);
    if (updateTexturePath(materialMap, sOldPrefix, sNewPrefix, bAlreadyDone))
      pMaterial->upgradeOpen(), pMaterial->setDiffuse(color, materialMap);
    if (bAlreadyDone)
      return;

    pMaterial->specular(color, materialMap, value);
    if (updateTexturePath(materialMap, sOldPrefix, sNewPrefix, bAlreadyDone))
      pMaterial->upgradeOpen(), pMaterial->setSpecular(color, materialMap, value);
    if (bAlreadyDone)
      return;

    pMaterial->opacity(value, materialMap);
    if (updateTexturePath(materialMap, sOldPrefix, sNewPrefix, bAlreadyDone))
      pMaterial->upgradeOpen(), pMaterial->setOpacity(value, materialMap);
    if (bAlreadyDone)
      return;

    //pMaterial->emission(color, materialMap);
    //if (updateTexturePath(materialMap, sOldPrefix, sNewPrefix, bAlreadyDone))
    //  pMaterial->upgradeOpen(), pMaterial->setEmission(color, materialMap);
    //if (bAlreadyDone)
    //  return;

    pMaterial->refraction(value, materialMap);
    if (updateTexturePath(materialMap, sOldPrefix, sNewPrefix, bAlreadyDone))
      pMaterial->upgradeOpen(), pMaterial->setRefraction(value, materialMap);
    if (bAlreadyDone)
      return;

    pMaterial->bump(materialMap);
    if (updateTexturePath(materialMap, sOldPrefix, sNewPrefix, bAlreadyDone))
      pMaterial->upgradeOpen(), pMaterial->setBump(materialMap);
    if (bAlreadyDone)
      return;

    pMaterial->reflection(materialMap);
    if (updateTexturePath(materialMap, sOldPrefix, sNewPrefix, bAlreadyDone))
      pMaterial->upgradeOpen(), pMaterial->setReflection(materialMap);
    if (bAlreadyDone)
      return;

    OdGiMaterialTraits::NormalMapMethod method;
    pMaterial->normalMap(materialMap, method, value);
    if (updateTexturePath(materialMap, sOldPrefix, sNewPrefix, bAlreadyDone))
      pMaterial->upgradeOpen(), pMaterial->setNormalMap(materialMap, method, value);

    //if (pMaterial->isWriteEnabled())
    //{
    //  // bad workaround for VIS-886 Crash after changing material property
    //  OdDbObject_clearModified(pMaterial);
    //  pMaterial->downgradeOpen();
    //}
  }
}

void EmbeddedImageDef::beginSave(OdDbDatabase* pDb, const OdString& pIntendedName)
{
  if (pIntendedName.isEmpty())
    return;
  // extract base path
  OdString path = pIntendedName;
  OdString shortFileName, shortFileNameOld;
  // under windows in the path there may be both '\' and '/' separators
  {
    int i1 = path.reverseFind('\\');
    int i2 = path.reverseFind('/');
    int i = odmax(i1, i2);
    shortFileName = path.mid(i+1);
    path = path.left(i+1);
    i = shortFileName.find(L'.');
    if (i != -1)
      shortFileName = shortFileName.left(i);
  }
  // materialize image to the file by the path
  OdChar filename[1024];
  if (_name.isEmpty())
  {
    int imageNum = 0;
    for (;;)
    {
      odSprintf(filename, 1024, OD_T("%simage%d%ls"), (const OdChar*)path, imageNum++, (const OdChar*)_ext);
      if (!odSystemServices()->accessFile(filename, 0))
        break;
    }
  }
  else
  {
    OdString sName = _name;
    int pos = 0;
    if (_pTexture.get() && (pos = sName.find(L'_')) > 0)
    {
      // CORE-14394
      ODA_ASSERT_ONCE_X(TDWF, !isDBRO());
      shortFileNameOld = sName.left(pos);
      sName = sName.mid(++pos);
#    if defined(DWFIMPORT_TEXTURE_PREFIX_BY_SOURCE_DWF_FILE)
      shortFileName = shortFileNameOld;
#    endif
    }
    odSprintf(filename, 1024, OD_T("%ls%ls_%ls%ls"), (const OdChar*)path, (const OdChar*)shortFileName, (const OdChar*)sName, (const OdChar*)_ext);
  }
  try
  {
    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if (_ext == L".png" && image().get() && image()->scanLineSize() && pRasSvcs.get())
    {
      if (!pRasSvcs->saveRasterImage(image(), filename)) // CORE-13609
        throw OdError(eFileAccessErr);
    }
    else
    {
      OdStreamBufPtr f = odSystemServices()->createFile(
        filename, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
      if (f.isNull())
        throw OdError(eFileAccessErr);
      _data->rewind();
      _data->copyDataTo(f);
    }
    if (!isDBRO())
    {
      ODA_ASSERT_ONCE_X(TDWF, !shortFileNameOld.isEmpty());
      updateTexturePathOfMatrials(pDb, shortFileNameOld + L"_", shortFileName + L"_");
    }
    else
    {
      // set this materialized file path to OdDbRasterImageDef
      upgradeOpen();
      setSourceFileName(filename);
      if (image().isNull() || image()->scanLineSize() == 0)
      {
        if (!pRasSvcs.isNull())
        {
          OdGiRasterImagePtr img = pRasSvcs->loadRasterImage(filename);
          if (!img.isNull())
            setImage(img);
        }
      }
      downgradeOpen();
    }
  }
  catch (OdError&)
  {
    ODA_FAIL_ONCE_X(TDWF);
  }
}
}
