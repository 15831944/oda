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
#ifndef __COLLADA_IMPORTERBASE_H__
#define __COLLADA_IMPORTERBASE_H__

#include "OdaCommon.h"
#include "COLLADAFWLight.h"
#include "ColladaDocumentImporter.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_COLLADA_IMPORT
{
        /** \details
          This is the base class for Collada importer.
        */
  class ImporterBase
  {
  private:
    /** The collada importer the importer belongs to.
    */
    DocumentImporter* m_DocumentImporter;

  protected:
    /** \details 
      Constructor. 
      \param colladaImporter [in] The collada importer the importer belongs to.
    */
    ImporterBase(DocumentImporter* colladaImporter);

    /** \details
      Destructor. 
    */
    virtual ~ImporterBase();
 
    DocumentImporter* getDocumentImporter() { return m_DocumentImporter; }
 
    void addUniqueIdFWImagePair( const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Image& image );
 
    void addUniqueIdFWEffectPair( const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Effect& effect );
 
    void addUniqueIdFWMaterialPair( const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Material& material );

    void addUniqueIdFWNodePair( const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Node* node );

    void addVisualScene( const COLLADAFW::VisualScene* visualScene );

    void addLibraryNodes( const COLLADAFW::LibraryNodes* libraryNodes );

    void addUniqueIdObjectIdPair(const COLLADAFW::UniqueId& uniqueId, OdDbObjectId objectId);

    void addUniqueIdMaterialIdPair(const COLLADAFW::UniqueId& uniqueId, COLLADAFW::MaterialId materialId);

    template<class FWObject>
    bool handleObjectReferences(FWObject* pFWobject, OdDbObjectId objectId, COLLADAFW::MaterialId materialId);

    void addUniqueIdFWControllerPair(const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::Controller& controller);

    const COLLADAFW::Node* getFWNodeByUniqueId(const COLLADAFW::UniqueId& uniqueId);

    OdDbObjectId getOdDbObjectIdByUniqueId(const COLLADAFW::UniqueId& uniqueId);

    COLLADAFW::MaterialId getMaterialIdByUniqueId(const COLLADAFW::UniqueId& uniqueId);

    void appendMaterialBindingsPair(const DocumentImporter::EntityMaterialBindingPair& materialBindingPair);

    void addUniqueIdSubUniqueIdPair(const COLLADAFW::UniqueId& uniqueId, const COLLADAFW::UniqueId& subUniqueId);

    void addUniqueIdOdDbObjectIdPair(const COLLADAFW::UniqueId& uniqueId, OdDbObjectId objectId);

    const COLLADAFW::Image* getFWImageByUniqueId(const COLLADAFW::UniqueId& uniqueId);

    const COLLADAFW::Controller* getFWControllerByUniqueId(const COLLADAFW::UniqueId& uniqueId);

    const DocumentImporter::FileInfo& getFileInfo() const;

    const bool isImportTextures();

    const DocumentImporter::UniqueIdFWMaterialMap& getUniqueIdFWMaterialMap();

    typedef DocumentImporter::UniqueIdSubUniqueIdMultiMap::const_iterator SubUniqueIdIter;

    void getSubUniqueIdsByUniqueId(const COLLADAFW::UniqueId& uniqueId, SubUniqueIdIter& first, SubUniqueIdIter& last);

    const DocumentImporter::EntityMaterialBindingsArray& getEntityMaterialBindings();

    const COLLADAFW::Material* getFWMaterialByUniqueId(const COLLADAFW::UniqueId& uniqueId);

    const COLLADAFW::Effect* getFWEffectByUniqueId(const COLLADAFW::UniqueId& uniqueId);

    static const COLLADAFW::MaterialId INVALID_MAT_ID = ~1;
  private:
    /** Disable default copy ctor. 
    */
    ImporterBase( const ImporterBase& pre );
    /** Disable default assignment operator. 
    */
    const ImporterBase& operator= (const ImporterBase& pre);
  };


  template<class FWObject>
  bool ImporterBase::handleObjectReferences(FWObject* pFWobject, OdDbObjectId objectId, COLLADAFW::MaterialId materialId)
  {
    const COLLADAFW::UniqueId& uniqueId = pFWobject->getUniqueId();
    addUniqueIdOdDbObjectIdPair(uniqueId, objectId);
    addUniqueIdMaterialIdPair(uniqueId, materialId);
    return true;
  }
}

#endif // __COLLADA_IMPORTERBASE_H__
