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
#include "ColladaMaterialCreator.h"
#include "DbDictionary.h"
#include "DbMaterial.h"
#include "DbEntity.h"


namespace TD_COLLADA_IMPORT
{
  MaterialCreator::MaterialCreator(DocumentImporter* documentImporter)
    : ImporterBase(documentImporter)
  {
  }

  MaterialCreator::~MaterialCreator()
  {
  }

  bool MaterialCreator::create()
  {
    const DocumentImporter::EntityMaterialBindingsArray& materialBindings = getEntityMaterialBindings();
    bool bRes = true;

    for (unsigned int i = 0; i < materialBindings.size(); i++)
    {
      OdDbObjectId objId = materialBindings[i].objId;
      createMaterialResults res = createAndAssingMaterial(objId, materialBindings[i].materialBinding);
      if (res != errOk && res != errNoTexture)
      {
        bRes = false;
      }
    }
    return bRes;
  }

  MaterialCreator::createMaterialResults MaterialCreator::createAndAssingMaterial(OdDbObjectId objId, const COLLADAFW::MaterialBinding& materialBinding)
  {
    createMaterialResults crMatRes = errOk;
    OdDbDictionaryPtr pMatDic = this->getDocumentImporter()->getDatabase()->getMaterialDictionaryId().safeOpenObject(OdDb::kForWrite);

    const COLLADAFW::UniqueId& materialUniqueId = materialBinding.getReferencedMaterial();
    if (materialUniqueId.isValid())
    {
      const COLLADAFW::Material* pMaterial = getFWMaterialByUniqueId(materialUniqueId);
      if (pMaterial)
      {
        OdDbObjectId objIdMat = pMatDic->getAt(pMaterial->getName().c_str());
        if (objIdMat.isNull())
        {
          const COLLADAFW::Effect* effect = getEffect(pMaterial);
          if (!effect)
            return crMatRes;
          crMatRes = createTeighaMaterial(*effect, pMaterial->getName(), pMatDic, objIdMat);
        }
        if (!objId.isNull())
        {
          OdDbEntityPtr ent = objId.safeOpenObject(OdDb::kForWrite);
          if (eOk != ent->setMaterial(objIdMat))
          {
            crMatRes = errSetMatState;
          }
        }
      }
    }
    else
    {
      crMatRes = errUnknown;
    }
    return crMatRes;
  }

  const COLLADAFW::Effect* MaterialCreator::getEffect( const COLLADAFW::Material* pMaterial )
  {
    const COLLADAFW::UniqueId& effectUniqueId = pMaterial->getInstantiatedEffect();
    if (!effectUniqueId.isValid())
      return 0;

    return getFWEffectByUniqueId(effectUniqueId);
  }
  

  void MaterialCreator::setTexture( const COLLADAFW::EffectCommon& effectCommon, const COLLADAFW::Texture& texture, OdGiMaterialMap& materialMap )
  {
    COLLADAFW::SamplerID samplerId = texture.getSamplerId();
    const COLLADAFW::Sampler* sampler = effectCommon.getSamplerPointerArray()[ samplerId ];

    const COLLADAFW::UniqueId& imageUniqueId = sampler->getSourceImage();
    const COLLADAFW::Image* image = getFWImageByUniqueId( imageUniqueId );
    if (image)
    {
      COLLADABU::URI imageUri(getFileInfo().absoluteFileUri, image->getImageURI().getURIString());
      OdString strImageFileName;
      try
      {
        COLLADABU::NativeString imageFileNameUTF8(imageUri.toNativePath().c_str(), COLLADABU::NativeString::ENCODING_UTF8);
        strImageFileName = imageFileNameUTF8.c_str();
      }
      catch (const COLLADABU::Exception& e)
      {
        if (e.getType() == COLLADABU::Exception::ERROR_UTF8_2_WIDE)
        {
          COLLADABU::NativeString imageFileNameNative(imageUri.toNativePath().c_str());
          strImageFileName = imageFileNameNative.c_str();
        }
      }
      materialMap.setSourceFileName(strImageFileName);
      OdGiMapper mapper;
      mapper.setUTiling(OdGiMapper::kInheritTiling);
      mapper.setVTiling(OdGiMapper::kInheritTiling);
      materialMap.setMapper(mapper);
    }
  }

    MaterialCreator::createMaterialResults MaterialCreator::createTeighaMaterial(const COLLADAFW::Effect& effect, const COLLADAFW::String& name, OdDbDictionaryPtr pMatDic, OdDbObjectId& objectId)
	{
    MaterialCreator::createMaterialResults res = MaterialCreator::errOk;
    const COLLADAFW::CommonEffectPointerArray& commonEffects = effect.getCommonEffects();
    if (commonEffects.getCount() > 0)
    {
      const COLLADAFW::EffectCommon& effectCommon = *commonEffects[0];

      OdDbMaterialPtr pMaterial = OdDbMaterial::createObject();

      pMaterial->setName(name.c_str());    

      OdGiMaterialMap materialMap;
      OdGiMaterialColor diffuseColor;
      diffuseColor.setMethod(OdGiMaterialColor::kOverride);
      materialMap.setSource(OdGiMaterialMap::kFile);

      unsigned int channelFlags = OdGiMaterialTraits::kNone;

      const COLLADAFW::ColorOrTexture& diffuse = effectCommon.getDiffuse();
      if (diffuse.isValid())
      {
        if (diffuse.isColor())
        {
          const COLLADAFW::Color& difColor = diffuse.getColor();
          diffuseColor.color() = OdCmEntityColor((OdUInt8)(difColor.getRed()*255.), (OdUInt8)(difColor.getGreen()*255.), (OdUInt8)(difColor.getBlue()*255.));
          pMaterial->setDiffuse(diffuseColor, materialMap);
        }
        else
        {
          SETBIT_1(channelFlags, OdGiMaterialTraits::kUseDiffuse);
          OdGiMaterialMap diffuseMap(materialMap);
          setTexture( effectCommon, diffuse.getTexture(), diffuseMap );
          pMaterial->setDiffuse(diffuseColor, diffuseMap);
          OdString sTextureFName = diffuseMap.sourceFileName();
          if (sTextureFName.getLength() > 0)//Test whether texture exists
          {
            if (FILE *file = fopen(sTextureFName, "r"))
            {
              fclose(file);
            }
            else
            {
              res = MaterialCreator::errNoTexture;
            }
          }
        }
      }

      const COLLADAFW::ColorOrTexture& emission = effectCommon.getEmission();
      if (emission.isValid())
      {
        if (emission.isColor())
        {
          const COLLADAFW::Color& emColor = emission.getColor();
          pMaterial->setSelfIllumination(emColor.getAlpha());
        }
        else
        {
          pMaterial->setSelfIllumination(0);
        }
      }

      const COLLADAFW::ColorOrTexture& specular = effectCommon.getSpecular();
      if (specular.isColor())
      {
        const COLLADAFW::Color& specColor = specular.getColor();
        OdGiMaterialColor specularColor;
        specularColor.setMethod(OdGiMaterialColor::kOverride);
        specularColor.color() = OdCmEntityColor((OdUInt8)(specColor.getRed()*255.), (OdUInt8)(specColor.getGreen()*255.), (OdUInt8)(specColor.getBlue()*255.));
        const COLLADAFW::FloatOrParam& shininessFloatOrParam = effectCommon.getShininess();
        double dGlossFactor = 1;
        if ( shininessFloatOrParam.getType() == COLLADAFW::FloatOrParam::FLOAT )
        {
          dGlossFactor = shininessFloatOrParam.getFloatValue();
        }
        pMaterial->setSpecular(specularColor, materialMap, dGlossFactor);
      }


      const COLLADAFW::ColorOrTexture& opacity = effectCommon.getOpacity();
      if (opacity.isColor())
      {
        //transparency
        const COLLADAFW::Color& opacityColor = opacity.getColor(); 
        float fAverageTranslucence = (float)(opacityColor.getRed() + opacityColor.getGreen() + opacityColor.getBlue())/3; 
        pMaterial->setOpacity(fAverageTranslucence, materialMap);
      }

      const COLLADAFW::FloatOrParam& indexOfRefraction = effectCommon.getIndexOfRefraction();
      if (indexOfRefraction.getType() == COLLADAFW::FloatOrParam::FLOAT)
      {
        pMaterial->setRefraction(indexOfRefraction.getFloatValue(), materialMap);
      }

      const COLLADAFW::FloatOrParam& reflectivity = effectCommon.getReflectivity();
      if (reflectivity.getType() == COLLADAFW::FloatOrParam::FLOAT)
      {
        pMaterial->setReflectivity(reflectivity.getFloatValue());
      }
      
      const COLLADAFW::ColorOrTexture& ambient = effectCommon.getAmbient();
      if (ambient.isColor())
      {
        const COLLADAFW::Color& ambColor = ambient.getColor();
        OdGiMaterialColor ambientColor;
        ambientColor.setMethod(OdGiMaterialColor::kOverride);
        ambientColor.color() = OdCmEntityColor((OdUInt8)(ambColor.getRed()*255.), (OdUInt8)(ambColor.getGreen()*255.), (OdUInt8)(ambColor.getBlue()*255.));
        pMaterial->setAmbient(ambientColor);
      }
      pMaterial->setChannelFlags(OdGiMaterialTraits::ChannelFlags(channelFlags));
      objectId = pMatDic->setAt(name.c_str(), pMaterial);
      return res;
    }
    objectId = OdDbObjectId();
    return res;
	}
}
