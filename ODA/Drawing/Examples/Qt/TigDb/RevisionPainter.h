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
//
// RevisionPainter.h
// 

#ifndef ODA_REVISIONPAINTER_H_
#define ODA_REVISIONPAINTER_H_

#include "IntArray.h"
#define STL_USING_MAP
#include "OdaSTL.h"

#include "RevisionController.h"

class RevGuiNode 
{
  const OdCommitDescription* _pData;
public:
  int x; // in steps
  int y;

  OdIntArray parentIndexes,
             kidIndexes;
  
  RevGuiNode(const OdCommitDescription* pData = NULL)
    : _pData(pData)
    , x(-1) // undef
    , y(0)
  {
  }

  const OdCommitDescription& data() const
  {
    ODA_ASSERT_ONCE(_pData);
    return *_pData;
  }
};

typedef OdArray<RevGuiNode> NodeList;
typedef std::map<OdString, int> MapNodeIdToIndex;

class ODRX_ABSTRACT RevisionPainter
{
protected:
  NodeList nodes;
  MapNodeIdToIndex mapIdToIndex;
  OdStringArray branches;

  int spaceX,
      spaceY,
      commitRadius,
      cornerRadius,
      branchWidth;

public:
  RevisionPainter();

  void setBranchWidth(int size);
  void setSpace(int spaceX, int spaceY);
  void setRadius(int commitRadius, int cornerRadius = 0);

  void setData(const OdStringArray& branches, const OdCommitDescriptionList& revisions, 
               int ver = -1); // latest

  virtual void draw();
private:
  virtual void drawNode(const RevGuiNode& node) = 0;
  virtual void drawBranch(const RevGuiNode& node, const RevGuiNode &mergNode) = 0;

  //QColor getBranchColor(RevGuiNode& node);

  void setData0(const OdStringArray& branches, const OdCommitDescriptionList& revisions);
  void setData1(const OdStringArray& branches, const OdCommitDescriptionList& revisions);
  void setData2(const OdStringArray& branches, const OdCommitDescriptionList& revisions);
};

#endif // ODA_REVISIONPAINTER_H_
