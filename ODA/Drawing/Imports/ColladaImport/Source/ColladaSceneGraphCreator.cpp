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
#include "ColladaSceneGraphCreator.h"
#include "COLLADAFWVisualScene.h"
#include "DbEntity.h"
#include "COLLADAFWNode.h"

namespace TD_COLLADA_IMPORT
{
  SceneGraphCreator::SceneGraphCreator(DocumentImporter* documentImporter)
    : ImporterBase(documentImporter)
  {}

  SceneGraphCreator::~SceneGraphCreator()
  {}

  bool SceneGraphCreator::create(const COLLADAFW::VisualScene* pVisualScene)
  {
    if ( !pVisualScene )
      return false;

    InternalNode parentNode;
    importNodes(pVisualScene->getRootNodes(), parentNode);
    return true;
  }


  void SceneGraphCreator::importNode(const COLLADAFW::Node* pNode, InternalNode& parentImportNode)
  {
    InternalNode importNode(parentImportNode);
    importNode.m_matTransformation = collectTransformation(pNode, importNode.m_matTransformation);

    //TODO: import cameras?
    importInstanceGeometries(pNode->getInstanceGeometries(), importNode);
    importInstanceLights(pNode->getInstanceLights(), importNode);
    importInstanceControllers(pNode->getInstanceControllers(), importNode);
    importNodes(pNode->getChildNodes(), importNode);
    // Append all nodes that are referenced by this node.
    importInstanceNodes(pNode->getInstanceNodes(), importNode);
  }


  //------------------------------
  bool SceneGraphCreator::importInstanceNodes(const COLLADAFW::InstanceNodePointerArray& instanceNodeArray, InternalNode& parentImportNode)
  {
    for ( size_t i = 0, count = instanceNodeArray.getCount(); i < count; ++i)
    {
      COLLADAFW::InstanceNode* instanceNode = instanceNodeArray[i];

      const COLLADAFW::UniqueId& uniqueId = instanceNode->getInstanciatedObjectId();
      OdDbObjectId objectId = getOdDbObjectIdByUniqueId(uniqueId);

      if (objectId.isNull())
      {
        const COLLADAFW::Node* instanciatedFWNode = getFWNodeByUniqueId(uniqueId);
        // check if the referenced node is in one of the already received library nodes
        if (instanciatedFWNode)
        {
          importNode(instanciatedFWNode, parentImportNode);
        }
      }
    }
    return true;
  }


  bool SceneGraphCreator::importNodes(const COLLADAFW::NodePointerArray& nodeArray, InternalNode& parentImportNode)
  {
    for (size_t i = 0, count = nodeArray.getCount(); i < count; ++i)
    {
      importNode(nodeArray[i], parentImportNode);
    }
    return true;
  }

  //------------------------------
  template<class Instance>
  bool SceneGraphCreator::importInstances(const COLLADAFW::PointerArray<Instance>& instanceArray,
    InternalNode& parentImportNode, void (SceneGraphCreator::*postProcess)(Instance*, OdDbObjectId, const COLLADAFW::UniqueId&))
  {
    for (size_t idxInstance = 0, count = instanceArray.getCount(); idxInstance < count; ++idxInstance)
    {
      Instance* instance = instanceArray[idxInstance];
      const COLLADAFW::UniqueId& uniqueId = instance->getInstanciatedObjectId();
      const COLLADAFW::Controller* pController = getFWControllerByUniqueId(uniqueId);
      SubUniqueIdIter first, last;
      if (pController)
      {
        COLLADAFW::UniqueId prevId(pController->getSource());
        COLLADAFW::UniqueId currId(uniqueId);
        while (pController->getSource().getClassId() == COLLADAFW::COLLADA_TYPE::CONTROLLER && prevId != currId)
        {
          prevId = pController->getSource();
          pController = getFWControllerByUniqueId(prevId);
          currId = pController->getSource();
        }
        getSubUniqueIdsByUniqueId(pController->getSource(), first, last);
      }
      else
      {
        getSubUniqueIdsByUniqueId(uniqueId, first, last);
      }
      for (SubUniqueIdIter it = first; it != last; ++it)
      {
        COLLADAFW::UniqueId subUniqueId = it->second;
        //clone object with transformation.
        OdDbObjectId oldObjectId = getOdDbObjectIdByUniqueId(subUniqueId);
        ODA_ASSERT_ONCE(!oldObjectId.isNull());
        OdDbEntityPtr pEnt = oldObjectId.safeOpenObject();
        OdDbEntityPtr pClone = pEnt->clone();
        if (pBTRTmp.isNull())
        {
          pBTRTmp = pEnt->database()->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
        }
        OdDbObjectId objectId = pBTRTmp->appendOdDbEntity(pClone);
        pClone->transformBy(parentImportNode.m_matTransformation);
        
        // post process the creation
        if (postProcess)
          (this->*postProcess)(instance, objectId, subUniqueId);
      }
    }
    return true;
  }

  bool SceneGraphCreator::importInstanceGeometries(const COLLADAFW::InstanceGeometryPointerArray& instanceGeometryArray, InternalNode& parentImportNode)
  {
    return importInstances<COLLADAFW::InstanceGeometry>(instanceGeometryArray, parentImportNode, &SceneGraphCreator::storeMaterialBinding<COLLADAFW::COLLADA_TYPE::INSTANCE_GEOMETRY>);
  }

  bool SceneGraphCreator::importInstanceLights(const COLLADAFW::InstanceLightPointerArray& instanceLightArray, InternalNode& parentImportNode)
  {
    return importInstances<COLLADAFW::InstanceLight>(instanceLightArray, parentImportNode);
  }

  bool SceneGraphCreator::importInstanceControllers(const COLLADAFW::InstanceControllerPointerArray& instanceControllerArray, InternalNode& parentImportNode)
  {
    return importInstances<COLLADAFW::InstanceController>(instanceControllerArray, parentImportNode, &SceneGraphCreator::storeMaterialBinding<COLLADAFW::COLLADA_TYPE::INSTANCE_CONTROLLER>);
  }

  OdGeMatrix3d SceneGraphCreator::collectTransformation(const COLLADAFW::Node* pNode, OdGeMatrix3d& matTransformation)
  {
    COLLADABU::Math::Matrix4 transformationMatrix;
    pNode->getTransformationMatrix(transformationMatrix);
    OdGeMatrix3d mat = OdGeMatrix3d::kIdentity;
    for(int i = 0; i < 4; ++i)
    {
      for(int j = 0; j < 4; ++j)
      {
        mat[i][j] = transformationMatrix[i][j];
      }
    }
    return matTransformation * mat;
  }
}
