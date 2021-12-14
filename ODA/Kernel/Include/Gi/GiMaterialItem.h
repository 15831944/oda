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

// Material data entry for Gs material cache and textures processing.

#ifndef __ODGIMATERIALITEM_H__
#define __ODGIMATERIALITEM_H__

#include "GiExport.h"
#include "GiImage.h"
#include "GiMaterial.h"
#include "GiCommonDraw.h"

#include "RxObject.h"

#include "TD_PackPush.h"

class OdDbStub;

/** \details
    Internal texture data for device support.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTextureData : public OdRxObject
{
  public:
    // Simple variant type for external device information

    /** \details
      Simple variant type for storing external device information.
    */
    struct DevDataVariant
    {
      /** \details
        Device data variant type.
      */
      enum DevDataVariantType
      {
        /**Device data type is void pointer to NULL. */
        DevDataVariantType_Void = 0,
        /**Device data type is a void pointer to the data*/
        DevDataVariantType_Ptr,
        /**Device data type is an OdRxObject */
        DevDataVariantType_RxObject
      };

      /** \details
        Default constructor for the DevDataVariant struct. Sets object type to DevDataVariantType_Void and initializes
        pointer to a data to NULL.
      */
      DevDataVariant() : m_ddvType(DevDataVariantType_Void), m_pVoid(NULL)
      {
      }
      
      /** \details
        Constructor for the DevDataVariant struct. Sets object type to DevDataVariantType_Ptr and initializes
        pointer to the data. The data type can be reset if the passed pointer points to NULL.
      */
      DevDataVariant(void *ptr) : m_ddvType(DevDataVariantType_Ptr), m_pVoid(ptr)
      {
        if (m_pVoid == NULL)
          m_ddvType = DevDataVariantType_Void;
      }
      
      /** \details
        Constructor for the DevDataVariant struct. Sets object type to DevDataVariantType_RxObject and initializes
        pointer to the data.
      */
      DevDataVariant(OdRxObjectPtr ptr) : m_ddvType(DevDataVariantType_RxObject), m_pVoid(NULL)
      {
        m_pRxObject = ptr;
      }

      /** \details
        Retrieves the type of the device data variant.
        
        \returns
        type of the device data variant as a value from the DevDataVariantType enumeration.
      */
      DevDataVariantType type() const
      {
        return m_ddvType;
      }
      
      /** \details
        Retrieves the pointer to device data variant.
        
        \returns
        void pointer that stores data.
      */
      void *getPtr() const
      {
        return m_pVoid;
      }
      
      /** \details
        Retrieves the OdRxObject that stores device data variant.
        
        \returns
        smart pointer to the OdRxObject.
      */
      OdRxObjectPtr getRxObject() const
      {
        return m_pRxObject;
      }

      /** \details
        Assignment operator for the DevDataVariant.
        \param ptr [in]  Void pointer to the data.
        
        \returns
        the instance of DevDataVariant.
      */
      DevDataVariant &operator =(void *ptr)
      {
        return setPtr(ptr);
      }
      
      /** \details
        Assignment operator for the DevDataVariant.
        \param ptr [in]  Smart pointer to the OdRxObject that stores the data.
        
        \returns
        the instance of DevDataVariant.
      */
      DevDataVariant &operator =(OdRxObjectPtr ptr)
      {
        return setRxObject(ptr);
      }

      /** \details
        Sets the data pointed by the specified pointer.
        \param ptr [in]  Void pointer to the data.
        
        \returns
        the instance of DevDataVariant.
      */
      DevDataVariant &setPtr(void *ptr)
      {
        clear();
        if (ptr != NULL)
        {
          m_ddvType = DevDataVariantType_Ptr;
          m_pVoid = ptr;
        }
        return *this;
      }
      
      /** \details
        Sets the data pointed by the specified smart pointer to the OdRxObject.
        \param ptr [in]  Smart pointer to the OdRxObject.
        
        \returns
        the instance of DevDataVariant.
      */
      DevDataVariant &setRxObject(OdRxObjectPtr ptr)
      {
        clear();
        m_ddvType = DevDataVariantType_RxObject;
        m_pRxObject = ptr;
        return *this;
      }

      /** \details
        Erases the data, sets the device data variant type to DevDataVariantType_Void.
      */
      void clear()
      {
        m_ddvType = DevDataVariantType_Void;
        m_pVoid = NULL;
        if (m_pRxObject.isNull())
          m_pRxObject.release();
      }

    protected:
      DevDataVariantType m_ddvType;
      void *m_pVoid;
      OdRxObjectPtr m_pRxObject;
    };

  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialTextureData);
    friend class OdGiModuleObject;
    
    /** \details
      Set texture pixels from RGBA array.
      \param pDeviceInfo [in]  Optional external device information.
      \param image [in]  Input BGRA image.
    */
    virtual void setTextureData(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, const OdGiImageBGRA32 &image) = 0;
    /** \details
      Get texture pixels as array of RGBA colors.
      \param data [out]  Output BGRA colors array.
      \param width [out]  Width of texture.
      \param height [out]  Height of texture.
    */
    virtual void textureData(OdGiPixelBGRA32Array &data, OdUInt32 &width, OdUInt32 &height) const = 0;
    
    /** \details
      Return true if texture data was previously initialized.
    */
    virtual bool haveData() const = 0;

    /** \details
      Create default material texture data implementation that stores color array.
    */
    static OdSmartPtr<OdGiMaterialTextureData> createDefaultTextureDataImplementation();
    /** \details
      Retrieves the default texture data implementation description.
      \returns
      pointer to the OdRxClass instance that stores the description of default material texture data implementation.
    */
    static OdRxClass *defaultTextureDataImplementationDesc();

  private:
    /*!DOM*/
    static void rxInitDefaultTextureDataImplementation();
    /*!DOM*/
    static void rxUninitDefaultTextureDataImplementation();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialTextureData object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialTextureData> OdGiMaterialTextureDataPtr;

/** \details
    Protocol extension for OdGiMaterialTextureEntry texture loading information.

    \sa
    TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTextureLoadPE : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialTextureLoadPE);

    /** \details
      Default constructor for the OdGiMaterialTextureLoadPE class.
    */
    OdGiMaterialTextureLoadPE();

    /** \details
      Called by texture loader before file loading.
      Can be used for file name correction before texture loading.
      \param fileName [in] Corrected file name of the texture.
      \param pDb [in] Pointer to the database.
    */
    virtual void startTextureLoading(OdString &fileName, OdDbBaseDatabase* pDb) = 0;

    /** \details
      Called by texture loader after file loading, only if texture loading success.
      \param fileName [in] File name of the texture.
      \param pDb [in] Pointer to the database.
    */
    virtual void textureLoaded(const OdString &fileName, OdDbBaseDatabase* pDb) = 0;
    /** \details
      Called by texture loader after file loading, only if texture loading failed.
      \param fileName [in] File name of the texture.
      \param pDb [in] Pointer to the database.
    */
    virtual void textureLoadingFailed(const OdString &fileName, OdDbBaseDatabase* pDb) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialTextureLoadPE object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialTextureLoadPE> OdGiMaterialTextureLoadPEPtr;

class OdGiMaterialTextureEntry;

/** \details
    Extends device by texture loading functionality.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTextureLoaderExt : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialTextureLoaderExt);

    /** \details
      Checks whether the device supports loading of the specified kind of texture.
      \param pTexture [in]  Pointer to material texture.
      \returns
      true if device supports loading of the specified kind of texture, false otherwise.
    */
    virtual bool allowTextureLoading(OdGiMaterialTexturePtr pTexture) = 0;

    /** \details
      Loads material texture.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation.
      \param giCtx [in]  Gi context.
      \param pEntry [in]  Texture entry.
      \param pTexture [in]  Pointer to material texture.
      \returns
      smart pointer to the material texture data.
    */
    virtual OdGiMaterialTextureDataPtr loadTexture(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, OdGiMaterialTextureEntry *pEntry, OdGiMaterialTexturePtr pTexture) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialTextureLoaderExt object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialTextureLoaderExt> OdGiMaterialTextureLoaderExtPtr;

/** \details
    Manage scene textures.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTextureManager : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialTextureManager);

    /** \details
        Management type.
    */
    enum ManageType
    {
      kFileTexturesOnly = 0, // @def
      kFileAndProceduralTextures, // not efficient for search
      kDisable // only loading from LoaderExt will be active
    };

    /** \details
      Sets type of textures management.
      \param type [in]  New management type.
    */
    virtual void setManageType(ManageType type) = 0;
    /** \details
      Retrieves the current management type.
      \returns
      current management type as a value from the ManageType enumeration.
    */
    virtual ManageType manageType() const = 0;

    /** \details
      Sets texture loading extension.
      \param pExt [in]  New extentsion pointer.
    */
    virtual void setMaterialLoaderExt(OdGiMaterialTextureLoaderExt *pExt) = 0;
    /** \details
      Retrieves the current texture loading extension.
      
      \returns
      smart pointer to the material texture loader extension.
    */
    virtual OdGiMaterialTextureLoaderExtPtr materialLoaderExt() const = 0;

    /** \details
      Searches texture in cache.
      \param matMap [in]  Material map.
      \returns
      smart pointer to the material texture data.
    */
    virtual OdGiMaterialTextureDataPtr searchTexture(const OdGiMaterialMap &matMap) = 0;
    /** \details
      Searches texture in cache.
      \param pTexture [in]  Material texture pointer.
      \returns
      smart pointer to the material texture data.
    */
    virtual OdGiMaterialTextureDataPtr searchTexture(OdGiMaterialTexturePtr pTexture) = 0;
    /** \details
      Searches texture in cache.
      \param fileName [in]  Texture file name.
      \returns
      smart pointer to the material texture data.
    */
    virtual OdGiMaterialTextureDataPtr searchTexture(const OdString &fileName) = 0;

    /** \details
      Tries to load texture if it isn't available in cache.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation.
      \param giCtx [in]  Gi context.
      \param pEntry [in]  Texture entry.
      \param matMap [in]  Material map.
      \returns
      smart pointer to the material texture data.
    */
    virtual OdGiMaterialTextureDataPtr tryToLoad(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, OdGiMaterialTextureEntry *pEntry, const OdGiMaterialMap &matMap) = 0;
    /** \details
      Tries to load texture if it isn't available in cache.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation.
      \param giCtx [in]  Gi context.
      \param pEntry [in]  Texture entry.
      \param pTexture [in]  Material texture pointer.
      \returns
      smart pointer to the material texture data.
    */
    virtual OdGiMaterialTextureDataPtr tryToLoad(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, OdGiMaterialTextureEntry *pEntry, OdGiMaterialTexturePtr pTexture) = 0;
    /** \details
      Tries to load texture if it isn't available in cache.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation.
      \param giCtx [in]  Gi context.
      \param pEntry [in]  Texture entry.
      \param fileName [in]  Texture file name.
      \returns
      smart pointer to the material texture data.
    */
    virtual OdGiMaterialTextureDataPtr tryToLoad(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, OdGiMaterialTextureEntry *pEntry, const OdString &fileName) = 0;

    /** \details
      Links texture to current cache.
      \param matMap [in]  Material map.
      \param pData [in]  Texture data pointer
    */
    virtual void linkTexture(const OdGiMaterialMap &matMap, OdGiMaterialTextureDataPtr pData) = 0;
    /** \details
      Links texture to current cache.
      \param pTexture [in]  Material texture pointer.
      \param pData [in]  Texture data pointer
    */
    virtual void linkTexture(OdGiMaterialTexturePtr pTexture, OdGiMaterialTextureDataPtr pData) = 0;
    /** \details
      Links texture to current cache.
      \param fileName [in]  Texture file name.
      \param pData [in]  Texture data pointer
    */
    virtual void linkTexture(const OdString &fileName, OdGiMaterialTextureDataPtr pData) = 0;

    /** \details
      Checks whether the specified texture data is available in cache.
      \param pData [in]  Texture data pointer.
      \returns
      true if specified texture data is available in cache, false otherwise.
    */
    virtual bool isAvailable(const OdGiMaterialTextureData *pData) const = 0;

    /** \details
      Removes the specified texture data from cache.
      \param pData [in]  Texture data pointer.
    */
    virtual void unlinkTexture(OdGiMaterialTextureData *pData) = 0;

    /** \details
      Clears cache.
    */
    virtual void clear() = 0;

    /** \details
      Retrieves amount of links to this texture. 
      \returns
      number of texture linkages as OdUInt32 value.
    */
    virtual OdUInt32 numTextureEntries() const = 0;

    /** \details
      Returns texture linkage by index.
      \param nIndex [in]  Texture linkage index.
      \returns
      smart pointer to the material texture data.
    */
    virtual OdGiMaterialTexturePtr textureEntryKey(OdUInt32 nIndex) const = 0;

    /** \details
      Returns texture linkage data by index.
      \param nIndex [in]  Texture linkage index.
      \returns
      smart pointer to the material texture data.
    */
    virtual OdGiMaterialTextureDataPtr textureEntryData(OdUInt32 nIndex) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialTextureManager object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialTextureManager> OdGiMaterialTextureManagerPtr;

/** \details
    Material texture entry that contains, generates, and loads texture data.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTextureEntry : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialTextureEntry);

    /** \details
      Retrieves pointer to device-dependent texture data container.
      \returns
      smart pointer to the material texture data.
    */
    virtual OdGiMaterialTextureDataPtr textureData() const = 0;
    /** \details
      Checks whether the texture data is initialized.
      \returns
      true if texture data is initialized, false otherwise.
    */
    virtual bool isTextureInitialized() const = 0;

    /** \details
      Retrieves the width of procedurally generated textures.
      \returns
      width of procedurally generated textures as OdUInt32 value.
    */
    virtual OdUInt32 proceduralTextureWidth() const = 0;
    /** \details
      Set width of procedurally generated textures.
      \param width [in]  width of the generated textures.
    */
    virtual void setProceduralTextureWidth(OdUInt32 width) = 0;
    /** \details
      Retrieves the height of procedurally generated textures.
      \returns
      height of procedurally generated textures as OdUInt32 value.
    */
    virtual OdUInt32 proceduralTextureHeight() const = 0;
    /** \details
      Set height of procedurally generated textures.
      \param height [in]  height of the generated textures.
    */
    virtual void setProceduralTextureHeight(OdUInt32 height) = 0;
    /** \details
      Set width and height of procedurally generated textures.
      \param width [in]  width of the generated textures.
      \param height [in]  height of the generated textures.
    */
    virtual void setProceduralTextureResolution(OdUInt32 width, OdUInt32 height) = 0;
    
    /** \details
      Retrieves the procedural textures quality coefficient.
      \returns
      procedural textures quality coefficient as a value of the double type.
    */
    virtual double proceduralTextureQuality() const = 0;
    /** \details
      Sets procedural textures quality coefficient.
      \param coef [in]  textures quality coefficient.
    */
    virtual void setProceduralTextureQuality(double coef) = 0;

    /** \details
      Load texture from material map.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation (must not be NULL)
      \param giCtx [in]  Gi context.
      \param matMap [in]  Input material map reference.
      \param pManager [in]  Optional textures manager.
      \returns
      true if either procedural texture or texture from file is set successfully, false otherwise.
    */
    virtual bool setGiMaterialTexture(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, const OdGiMaterialMap &matMap, OdGiMaterialTextureManager *pManager = NULL) = 0;

    /** \details
      Load texture from material texture pointer.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation (must not be NULL).
      \param giCtx [in]  Gi context.
      \param pTexture [in]  Pointer to material texture.
      \param pManager [in]  Optional textures manager.
      \returns
      true if either procedural texture or texture from file is set successfully, false otherwise.
    */
    virtual bool setGiMaterialTexture(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, OdGiMaterialTexturePtr pTexture, OdGiMaterialTextureManager *pManager = NULL) = 0;
    // Load texture from raster file
    /** \details
      Load texture from raster file.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation (must not be NULL).
      \param giCtx [in]  Gi context.
      \param fileName [in]  Name of file that contains raster image.
      \param pManager [in]  Optional textures manager.
      \returns
      true if texture is set successfully from the image file, false otherwise.
    */
    virtual bool setGiMaterialTexture(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, const OdString &fileName, OdGiMaterialTextureManager *pManager = NULL) = 0;
    /** \details
      Generate texture from material opacity value.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation (must not be NULL).
      \param giCtx [in]  Gi context.
      \param opacity [in]  Material opacity value (0.0 - 1.0).
      \param pManager [in]  Optional textures manager.
      \returns
      true if texture is set successfully set, false otherwise.
    */
    virtual bool setGiMaterialTexture(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, double opacity, OdGiMaterialTextureManager *pManager = NULL) = 0;

    /** \details
      Sets texture data explicitly.
      \param pTextureData [in]  Pointer to texture data.
      \param pManager [in]  Optional textures manager.
    */
    virtual void setTextureData(OdGiMaterialTextureData *pTextureData, OdGiMaterialTextureManager *pManager = NULL) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialTextureEntry object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialTextureEntry> OdGiMaterialTextureEntryPtr;

/** \details
    Container item for material data storage.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialItem : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialItem);

    /** \details
      Retrieves diffuse texture pointer.
      \returns
      smart pointer to the diffuse texture.
    */
    virtual OdGiMaterialTextureEntryPtr diffuseTexture() const = 0;
    /** \details
      Retrieves diffuse texture pointer.
      \returns
      smart pointer to the diffuse texture.
    */
    virtual OdGiMaterialTextureEntryPtr diffuseTexture() = 0;
    /** \details
      Creates and returns diffuse texture pointer.
      \returns
      smart pointer to the created diffuse texture.
    */
    virtual OdGiMaterialTextureEntryPtr createDiffuseTexture() = 0;
    /** \details
      Destroys diffuse texture.
    */
    virtual void removeDiffuseTexture() = 0;
    /** \details
      Checks whether the diffuse texture is created.
      \returns
      true if this material item has a diffuse texture, false otherwise.
    */
    virtual bool haveDiffuseTexture() const = 0;

    // Container data

    /** \details
      Retrieves pointer to cached data object.
      \returns
      smart pointer to the OdRxObject cache.
    */
    virtual OdRxObjectPtr cachedData() const = 0;
    /** \details
      Set pointer to cached data object.
      \param data [in]  Pointer to cached data.
    */
    virtual void setCachedData(OdRxObjectPtr data) = 0;

    /** \details
      Retrieves material ID.
      \returns
      pointer to the material identifier.
    */
    virtual const OdDbStub *materialId() const = 0;
    /** \details
      Set material ID.
      \param matId [in]  Pointer to the material identifier.
    */
    virtual void setMaterialId(const OdDbStub *matId = NULL) = 0;
    /** \details
      Checks whether stored material ID is equal to matId.
      \returns
      true if the specified material ID is equal to this material ID, false otherwise.
    */
    virtual bool isMaterialIdValid(const OdDbStub *matId) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialItem object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialItem> OdGiMaterialItemPtr;

/** \details
    Container item for material data store (for render devices).

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialRenderItem : public OdGiMaterialItem
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialRenderItem);

    /** \details
      Retrieves specular texture pointer.
      \returns
      smart pointer to the specular texture.
    */
    virtual OdGiMaterialTextureEntryPtr specularTexture() const = 0;
    /** \details
      Retrieves specular texture pointer.
      \returns
      smart pointer to the specular texture.
    */
    virtual OdGiMaterialTextureEntryPtr specularTexture() = 0;
    /** \details
      Creates and returns specular texture pointer.
      \returns
      smart pointer to the created specular texture.
    */
    virtual OdGiMaterialTextureEntryPtr createSpecularTexture() = 0;
    /** \details
      Destroys specular texture.
    */
    virtual void removeSpecularTexture() = 0;
    /** \details
      Checks whether this material render item has a specular texture.
      \returns
      true if specular texture created, false otherwise.
    */
    virtual bool haveSpecularTexture() const = 0;

    /** \details
      Retrieves reflection texture pointer.
      \returns
      smart pointer to the reflection texture.
    */
    virtual OdGiMaterialTextureEntryPtr reflectionTexture() const = 0;
    /** \details
      Retrieves reflection texture pointer.
      \returns
      smart pointer to the reflection texture.
    */
    virtual OdGiMaterialTextureEntryPtr reflectionTexture() = 0;
    /** \details
      Creates and returns reflection texture pointer.
      \returns
      smart pointer to the created reflection texture.
    */
    virtual OdGiMaterialTextureEntryPtr createReflectionTexture() = 0;
    /** \details
      Destroy reflection texture.
    */
    virtual void removeReflectionTexture() = 0;
    /** \details
      Checks whether the reflection texture is created.
      \returns
      true if this material item has a reflection texture, false otherwise.
    */
    virtual bool haveReflectionTexture() const = 0;

    /** \details
      Retrieves opacity texture pointer.
      \returns
      smart pointer to the opacity texture.
    */
    virtual OdGiMaterialTextureEntryPtr opacityTexture() const = 0;
    /** \details
      Retrieves opacity texture pointer.
      \returns
      smart pointer to the opacity texture.
    */
    virtual OdGiMaterialTextureEntryPtr opacityTexture() = 0;
    /** \details
      Creates and returns opacity texture pointer.
      \returns
      smart pointer to the created opacity texture.
    */
    virtual OdGiMaterialTextureEntryPtr createOpacityTexture() = 0;
    /** \details
      Destroys opacity texture.
    */
    virtual void removeOpacityTexture() = 0;
    /** \details
      Checks whether the opacity texture is created.
      \returns
      true if this material item has an opacity texture, false otherwise.
    */
    virtual bool haveOpacityTexture() const = 0;

    /** \details
      Retrieves bump texture pointer.
      \returns
      smart pointer to the bump texture.
    */
    virtual OdGiMaterialTextureEntryPtr bumpTexture() const = 0;
    /** \details
      Retrieves bump texture pointer.
      \returns
      smart pointer to the bump texture.
    */
    virtual OdGiMaterialTextureEntryPtr bumpTexture() = 0;
    /** \details
      Creates and returns bump texture pointer.
      \returns
      smart pointer to the created bump texture.
    */
    virtual OdGiMaterialTextureEntryPtr createBumpTexture() = 0;
    /** \details
      Destroys bump texture.
    */
    virtual void removeBumpTexture() = 0;
    /** \details
      Checks whether the bump texture is created.
      \returns
      true if this material item has a bump texture, false otherwise.
    */
    virtual bool haveBumpTexture() const = 0;

    /** \details
      Retrieves refraction texture pointer.
      \returns
      smart pointer to the refraction texture.
    */
    virtual OdGiMaterialTextureEntryPtr refractionTexture() const = 0;
    /** \details
      Retrieves refraction texture pointer.
      \returns
      smart pointer to the refraction texture.
    */
    virtual OdGiMaterialTextureEntryPtr refractionTexture() = 0;
    /** \details
      Creates and returns refraction texture pointer.
      \returns
      smart pointer to the created refraction texture.
    */
    virtual OdGiMaterialTextureEntryPtr createRefractionTexture() = 0;
    /** \details
      Destroys refraction texture.
    */
    virtual void removeRefractionTexture() = 0;
    /** \details
      Checks whether the refraction texture is created.
      \returns
      true if this material item has a refraction texture, false otherwise.
    */
    virtual bool haveRefractionTexture() const = 0;

    /** \details
      Retrieves normal map texture pointer.
      \returns
      smart pointer to the normal map texture.
    */
    virtual OdGiMaterialTextureEntryPtr normalMapTexture() const = 0;
    /** \details
      Retrieves normal map texture pointer.
      \returns
      smart pointer to the normal map texture.
    */
    virtual OdGiMaterialTextureEntryPtr normalMapTexture() = 0;
    /** \details
      Creates and returns normal map texture pointer.
      \returns
      smart pointer to the created diffuse texture.
    */
    virtual OdGiMaterialTextureEntryPtr createNormalMapTexture() = 0;
    /** \details
      Destroys normal map texture.
    */
    virtual void removeNormalMapTexture() = 0;
    /** \details
      Checks whether the normal map texture is created.
      \returns
      true if this material item has a normal map texture, false otherwise.
    */
    virtual bool haveNormalMapTexture() const = 0;

    /** \details
      Retrieves emission texture pointer.
      \returns
      smart pointer to the emission texture.
    */
    virtual OdGiMaterialTextureEntryPtr emissionTexture() const = 0;
    /** \details
      Retrieves emission texture pointer.
      \returns
      smart pointer to the emission texture.
    */
    virtual OdGiMaterialTextureEntryPtr emissionTexture() = 0;
    /** \details
      Creates and returns emission texture pointer.
      \returns
      smart pointer to the created emission texture.
    */
    virtual OdGiMaterialTextureEntryPtr createEmissionTexture() = 0;
    /** \details
      Destroys emission texture.
    */
    virtual void removeEmissionTexture() = 0;
    /** \details
      Checks whether the emission texture is created.
      \returns
      true if this material item has an emission texture, false otherwise.
    */
    virtual bool haveEmissionTexture() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialRenderItem object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialRenderItem> OdGiMaterialRenderItemPtr;

#include "TD_PackPop.h"

#endif // __ODGIMATERIALITEM_H__
