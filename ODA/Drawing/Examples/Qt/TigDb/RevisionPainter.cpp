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
// RevisionPainter.cpp
// 

#include "OdaCommon.h"
#define STL_USING_ALGORITHM
#include "OdaSTL.h"

#include "RevisionPainter.h"

RevisionPainter::RevisionPainter()
  : spaceX(20)
  , spaceY(30)
  , branchWidth(6)
  , commitRadius(16)
  , cornerRadius(0)
{
}

void RevisionPainter::setBranchWidth(int size) 
{
  branchWidth = size;
}

void RevisionPainter::setSpace(int xSpace, int ySpace)
{
  spaceX = xSpace;
  spaceY = ySpace;
  if (cornerRadius > (spaceY / 2))
    cornerRadius = spaceY / 2;
}

void RevisionPainter::setRadius(int cmtRadius, 
                                int cnrRadius) // = 0 
{
  commitRadius = cmtRadius;
  cornerRadius = cnrRadius;
  if (cornerRadius > (spaceY / 2))
    cornerRadius = spaceY / 2;
}

void RevisionPainter::setData(const OdStringArray& branches, const OdCommitDescriptionList& revisions, 
                              int ver) // = -1 // latest
{
  switch (ver)
  {
  default:
    ODA_FAIL_ONCE(); // TODO
  case -1:
  case 2:
    setData2(branches, revisions);
    break;
  // initial version (was used on conference) :
  case 1:
    setData1(branches, revisions);
    break;
  case 0:
    setData0(branches, revisions);
    break;
  }
}

void RevisionPainter::setData0(const OdStringArray& arrBranches, const OdCommitDescriptionList& revisions)
{
  nodes.clear();
  mapIdToIndex.clear();

  branches = arrBranches;

  auto itr = revisions.begin();
  auto end = revisions.end();
  // assume that "revisions" is a correct list of revisions to render
  int columns = 0,
      index = 0;

  for (; itr != end; itr ++, index++) 
  {
    RevGuiNode node(&*itr);
    node.y = index;

    for (auto i = nodes.begin(); i != nodes.end(); ++i)
    {
      auto p = i->data().parents.begin();
      if (p == i->data().parents.end())
        continue;
      if (*p == node.data().id) // there is a node that has this as a first parent - this node is at the same x
      {
        node.x = i->x;
        break;
      }
      for (; p != i->data().parents.end(); ++p) // node is a second parent - shift off
      {
        if (*p == node.data().id)
        {
          node.x = columns++;
          break;
        }
      }
    }
    if (node.x < 0) // new top
      node.x = columns++;

    nodes.push_back(node);
    mapIdToIndex[node.data().id] = nodes.size() - 1;
  }
}

void RevisionPainter::setData1(const OdStringArray& arrBranches, const OdCommitDescriptionList& revisions)
{
  nodes.clear();
  mapIdToIndex.clear();

  branches = arrBranches;

  // assume that "revisions" is a correct list of revisions to render

  int column = 0;
  for (OdUInt32 idxRev = 0, szRev = revisions.size(); idxRev < szRev; idxRev++)
  {
    const OdCommitDescription& rev = revisions[idxRev];

    RevGuiNode node(&rev);
    node.y = (int) idxRev;

    bool isByFirst = false;
    for (OdUInt32 idxNds = 0, szNds = nodes.size(); idxNds < szNds; idxNds++)
    {
      const RevGuiNode& nd = nodes[idxNds];

      OdUInt32 sz = nd.data().parents.size();
      for (OdUInt32 idx = 0; idx < sz; idx++)
      {
        const OdString& parent = nd.data().parents[idx];
        if (parent != node.data().id) 
          continue;

        if (!idx)
        {
          isByFirst = true;
          node.x = nd.x;  // there is a node that has this as a first parent - this node is at the same x
        }
        else
          node.x = column++; // node is a second parent - shift off
        break;
      }
      if (isByFirst)
        break;
    }
    if (node.x < 0)
      node.x = column++;

    nodes.push_back(node);
    mapIdToIndex[node.data().id] = nodes.size() - 1;
  }
}

//static int numRevisiomsToBranch(NodeList& nodes, int indexStart,
//                                const OdIntArray* pOnlyforindexes = NULL)
//{
//  return -1; // TODO
//}

static int setNodeOffsets(NodeList& nodes,
                          int indexStart, int xOffset, 
                          int& xWidth, // initially 0 
                          const OdIntArray* pOnlyforindexes = NULL,
                          const OdIntArray* pIndexesToJoin = NULL,
                          OdIntArray* pOtherConnectedIndexes = NULL)
{
  int num = 0,
      index = indexStart;
  OdIntArray nextIndexes;

  if (pIndexesToJoin && pOnlyforindexes)
  {
    RevGuiNode& firstNode = nodes[pIndexesToJoin->first()];
    for (int idxKid = 0, szKid = (int)firstNode.kidIndexes.size(); idxKid < szKid; idxKid++)
    {
      int indexKid = firstNode.kidIndexes[idxKid];
      if (!pOnlyforindexes->contains(indexKid))
        continue;

      index = indexKid;
      if (indexStart != indexKid)
        nextIndexes.push_back(indexStart);
      break;
    }
  }

  // via kids
  for (; index >= 0 && (!pOnlyforindexes || pOnlyforindexes->contains(index)); num++)
  {
    RevGuiNode& node = nodes[index];
    if (node.x >= 0)
      break;

    int nextIndex = -1;
    //int nextToBr = -1;
    for (int idxKid = 0, szKid = (int)node.kidIndexes.size(); idxKid < szKid; idxKid++)
    {
      int indexKid = node.kidIndexes[idxKid];
      if (nextIndex < 0)
      {
        if (!pOnlyforindexes || pOnlyforindexes->contains(indexKid))
        {
          nextIndex = indexKid;
          //nextToBr = numRevisiomsToBranch(nodes, nextIndex, pOnlyforindexes);
        }
        else if (pOtherConnectedIndexes && !pOtherConnectedIndexes->contains(indexKid))
          pOtherConnectedIndexes->push_back(indexKid);
        continue;
      }

      if ((!pOnlyforindexes || pOnlyforindexes->contains(indexKid)) && !nextIndexes.contains(indexKid))
      {
        //int nextKidToBr = numRevisiomsToBranch(nodes, nextIndex, pOnlyforindexes);
        //if (nextKidToBr >= 0 && ((nextToBr < 0) || nextKidToBr < nextToBr)
        //{
        //  nextIndexes.push_back(nextIndex);
        //  nextIndex = indexKid;
        //  nextToBr = nextKidToBr;
        //}
        //else
          nextIndexes.push_back(indexKid);
      }
      else if (pOtherConnectedIndexes && !pOtherConnectedIndexes->contains(indexKid))
        pOtherConnectedIndexes->push_back(indexKid);
    }

    nodes[index].x = xOffset;
    index = nextIndex;
  }

  if (!pIndexesToJoin && num)
    xWidth++;

  for (int idxNext = 0, szNext = (int) nextIndexes.size(); idxNext < szNext; idxNext++)
  {
    index = nextIndexes[idxNext];
    int svXWidth = xWidth;
    num += setNodeOffsets(nodes, index, xOffset + xWidth - 1, xWidth, pOnlyforindexes, NULL, pOtherConnectedIndexes);
    if (pIndexesToJoin)
      xWidth = svXWidth;
  }

  return num;
}

static bool isColomnToJoin(const NodeList& nodes, std::map<OdString, OdIntArray>& mapColumnToNodeIndexes,
                           const OdString& colBase, const OdString& col, const OdStringArray& branches,
                           int priority = 0)
{
  OdIntArray& indexesBase = mapColumnToNodeIndexes[colBase];
  const RevGuiNode& nodeBase = nodes[indexesBase.first()];
  OdIntArray& indexes = mapColumnToNodeIndexes[col];
  //const RevGuiNode& node = nodes[indexes.last()];

  if (!priority || priority == 1)
  {
    // hardcoded by merge in commit descriprion
    if (colBase.find(L"+master+") >= 0 && col.find(L"+master+") >= 0)
    {
      if (!priority && nodeBase.kidIndexes.contains(indexes.last()))
        return true;

      const RevGuiNode& nodeFirst = nodes[indexes.first()];
      if (priority && nodeFirst.data().msg.find(L"Merge") >= 0)
        return true;
      if (priority && col == L"+master+")
        return true;
    }
    return false;
  }

  if (!colBase.find(col) && colBase.find(L"+master+") < 0)
  {
    if (col.find(L'+', 1) == (col.getLength() - 1)) // single only
      return true;
  }
  //int numDif = 0;
  //for (int idx = 0, sz = (int) branches.size(); idx < sz; idx++)
  //{
  //  OdString br = L"+" + branches[idx] + L"+";
  //  bool presentInBase = (colBase.find(br) >= 0),
  //       presentIn = (col.find(br) >= 0);
  //  if (br == L"+master+" && presentInBase != presentIn)
  //    return false;
  //  if (!presentInBase && presentIn)
  //    return false;
  //  if (presentInBase && !presentIn)
  //    numDif++;
  //}
  //if (numDif == 1)
  //  return true;
  return false;
}

void RevisionPainter::setData2(const OdStringArray& arrBranches, const OdCommitDescriptionList& revisions)
{
  nodes.clear();
  mapIdToIndex.clear();

  branches = arrBranches;

  // assume that "revisions" is a correct list of revisions to render

  int idxRev = 0,
      szRev = revisions.size();

  // inline column groups
  std::map<OdString, OdIntArray> mapColumnToNodeIndexes;
  OdStringArray columns;

  for (; idxRev < szRev; idxRev++)
  {
    const OdCommitDescription& rev = revisions[idxRev];
   
    RevGuiNode node(&rev);
    node.y = idxRev;

    OdString column;
    if (!node.data().branchByPrefix.isEmpty())
      column = L"+" + node.data().branchByPrefix + L"+"; // in +'ses for convenient seach only
    //else if (node.data().branches.size() == 1)
    //  column = node.data().branches[0];
    else
    {
      for (int idxBr = 0, szBr = (int)node.data().branches.size(); idxBr < szBr; idxBr++)
        column += L"+" + node.data().branches[idxBr];
      column += L"+";
    }

    if (!column.isEmpty())
    {
      if (!columns.contains(column))
        columns.push_back(column);
      OdIntArray& indexes = mapColumnToNodeIndexes[column];
      indexes.push_back(idxRev);
    }

    nodes.push_back(node);
    mapIdToIndex[node.data().id] = idxRev;
  }

  struct ColumComparator 
  {
    bool operator() (const OdString& cl1, const OdString& cl2) const
    { 
      return cl1.getLength() > cl2.getLength();
    }
  } columComparator;
  std::sort(columns.begin(), columns.end(), columComparator);

  for (idxRev = 0; idxRev < szRev; idxRev++)
  {
    RevGuiNode& node = nodes[idxRev];
    for (OdUInt32 idxPrnt = 0, szPrnt = node.data().parents.size(); idxPrnt < szPrnt; idxPrnt++)
    {
      const OdString& idParent = node.data().parents[idxPrnt];

      int index = mapIdToIndex[idParent];
      node.parentIndexes.push_back(index);

      RevGuiNode& parentNode = nodes[index];
      parentNode.kidIndexes.push_back(idxRev);
    }
  }

  int xOffset = 0,
      xWidth = 0,
      numAll = 0;
//#if 0
//  // test
//  ODA_ASSERT_VAR(int num = )
//  setNodeOffsets(nodes, szRev - 1, xOffset, xWidth);
//  ODA_ASSERT_ONCE(num == szRev)
//#else  
  for (int idxCol = 0, szCol = (int) columns.size(); idxCol < szCol; idxCol++)
  {
    OdString col = columns[idxCol];

    OdIntArray& indexes = mapColumnToNodeIndexes[col];
    int szIndexes = indexes.size();
    ODA_ASSERT_ONCE(szIndexes);
    int indexStart = indexes.last();
    xWidth = 0;

    //OdIntArray connectedIndexes;
    int num = setNodeOffsets(nodes, 
                             indexStart, xOffset, xWidth,
                             &indexes); // onlyforindexes,
                             //NULL, &connectedIndexes);

    if (!num)
      continue; //already joined by next
    for (int priority = 0; priority < 3; priority++)
    {
      for (int idxColJoin = idxCol + 1; idxColJoin < szCol; idxColJoin++)
      {
        if (!isColomnToJoin(nodes, mapColumnToNodeIndexes, columns[idxCol], columns[idxColJoin], 
                            branches, priority))
          continue;

        OdIntArray& indexes = mapColumnToNodeIndexes[columns[idxColJoin]];
        szIndexes = indexes.size();
        ODA_ASSERT_ONCE(szIndexes);
        indexStart = indexes.last();

        OdIntArray& indexesToJoin = mapColumnToNodeIndexes[col];
        int numJoined = setNodeOffsets(nodes, indexStart, xOffset, xWidth, &indexes, &indexesToJoin);
        if (numJoined)
        {
          num += numJoined;
          col = columns[idxColJoin];
          if (idxColJoin == (idxCol + 1))
            idxCol = idxColJoin;
          priority = 0;
        }
      }
    }

    if (num)
      xOffset += xWidth;
    numAll += num;
  }

  if (numAll < szRev)
  {
    // try to find missing node & set something for it too
    xWidth = 0;
    for (idxRev = szRev - 1; idxRev >= 0 && numAll < szRev; idxRev--)
    {
      RevGuiNode& node = nodes[idxRev];
      if (node.x >= 0)
        continue;

      numAll += setNodeOffsets(nodes, idxRev, xOffset, xWidth);
      xOffset += xWidth;
    }
  }
//#endif
}

void RevisionPainter::draw()
{
  for (auto itr = nodes.begin(); itr != nodes.end(); itr ++) {
    RevGuiNode& node = *itr;

    auto prntItr = node.data().parents.begin();
    auto end = node.data().parents.end();

    for (; prntItr != end; prntItr ++) {
       if (mapIdToIndex.find((*prntItr)) != mapIdToIndex.end()) {
         RevGuiNode& prnt = nodes[mapIdToIndex[(*prntItr)]];
         drawBranch(node, prnt);
       }
    }

    drawNode(node);
  }
}
