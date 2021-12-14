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
#ifndef _COLLADA_SCENEGRAPHCREATOR_H_
#define _COLLADA_SCENEGRAPHCREATOR_H_

#include "ColladaImporterBase.h"
#include "COLLADAFWNode.h"
#include "Math/COLLADABUMathMatrix4.h"


/** \details
  <group OdImport_Classes> 
*/
namespace TD_COLLADA_IMPORT
{

  struct InternalNode
  {
    OdGeMatrix3d m_matTransformation;
    InternalNode()
    {
      m_matTransformation = OdGeMatrix3d::kIdentity;
    }
  };

  /** \details
    This class creates the scene graph for the importer.
  */
  class SceneGraphCreator : public ImporterBase 
  {
  public:
    /** Constructor. 
    */
    SceneGraphCreator(DocumentImporter* documentImporter);

    /** Destructor. 
    */
    virtual ~SceneGraphCreator();

    /** Creates the scene graph.
    */
    bool create(const COLLADAFW::VisualScene* pVisualScene);

  private:
    /** Recursively imports @a node.
    */
    void importNode( const COLLADAFW::Node* node, InternalNode& parentImportNode );

    /** Imports all nodes in  @a nodeArray
    */
    bool importNodes(const COLLADAFW::NodePointerArray& nodeArray, InternalNode& parentImportNode);

    template<class Instance>
    bool importInstances(const COLLADAFW::PointerArray<Instance>& instanceArray, InternalNode& parentImportNode, void (SceneGraphCreator::*postProcess)(Instance*, OdDbObjectId, const COLLADAFW::UniqueId&) = NULL);

    bool importInstanceGeometries(const COLLADAFW::InstanceGeometryPointerArray& instanceGeometryArray, InternalNode& parentImportNode);

    bool importInstanceLights(const COLLADAFW::InstanceLightPointerArray& instanceLightArray, InternalNode& parentImportNode);

    bool importInstanceControllers(const COLLADAFW::InstanceControllerPointerArray& instanceControllerArray, InternalNode& parentImportNode);

    //void setNodeProperties( const COLLADAFW::Node* node, InternalNode& parentImportNode);
    OdGeMatrix3d collectTransformation(const COLLADAFW::Node* pNode, OdGeMatrix3d& matTransformation);

    bool importInstanceNodes(const COLLADAFW::InstanceNodePointerArray& instanceNodeArray, InternalNode& parentImportNode);

    template<COLLADAFW::ClassId classId>
    void storeMaterialBinding(COLLADAFW::InstanceBindingBase<classId>* instanceGeometry, OdDbObjectId objId, const COLLADAFW::UniqueId& uniqueId)
    {
      COLLADAFW::MaterialBindingArray& materialBindings = instanceGeometry->getMaterialBindings();
      COLLADAFW::MaterialId matId = getMaterialIdByUniqueId(uniqueId);
      if (matId != INVALID_MAT_ID)
      {
        for (size_t matBindIdx = 0; matBindIdx < materialBindings.getCount(); ++matBindIdx)
        {
          if (materialBindings[matBindIdx].getMaterialId() == matId)
          {
            DocumentImporter::EntityMaterialBindingPair materialBindingsPair(objId, materialBindings[matBindIdx]);
            appendMaterialBindingsPair(materialBindingsPair);
            return;
          }
        }
      }
      ODA_FAIL_ONCE();
      //if we are here - material bindings in .dae file are incorrect
      //this can happen if <geometry> has different materials than in correspondent <bind_material>
      //ODA Collada Export could have created such files previously, so try to regenerate .dae file
      //I use the first material here - it will work like previous version and will guess correctly most of the time
      DocumentImporter::EntityMaterialBindingPair materialBindingsPair(objId, materialBindings[0]);
      appendMaterialBindingsPair(materialBindingsPair);
    }

  private:
    OdDbBlockTableRecordPtr pBTRTmp; //Temporary object. Optimization
    /** Disable default copy ctor. 
    */
    SceneGraphCreator(const SceneGraphCreator& pre);
    /** Disable default assignment operator. 
    */
    const SceneGraphCreator& operator= ( const SceneGraphCreator& pre );
  };
}

#endif // _COLLADA_SCENEGRAPHCREATOR_H_
