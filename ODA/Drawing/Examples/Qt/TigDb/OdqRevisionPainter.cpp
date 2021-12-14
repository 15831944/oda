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
// OdqRevisionPainter.cpp
// 

#include "OdaCommon.h"

#include "OdqRevisionPainter.h"

OdqRevisionPainter::OdqRevisionPainter(QGraphicsScene *scene)
  : scene(scene) 
{
  branchesColors.append(QColor::fromRgb(44, 62, 80));
  branchesColors.append(QColor::fromRgb(39, 174, 96));
  branchesColors.append(QColor::fromRgb(230, 126, 34));
  branchesColors.append(QColor::fromRgb(41, 128, 185));
  branchesColors.append(QColor::fromRgb(231, 76, 60));
  branchesColors.append(QColor::fromRgb(142, 68, 173));
  branchesColors.append(QColor::fromRgb(255, 140, 0));
  branchesColors.append(QColor::fromRgb(149, 165, 166));
  branchesColors.append(QColor::fromRgb(192, 57, 43));
  branchesColors.append(QColor::fromRgb(127, 140, 141));
}

void OdqRevisionPainter::draw()
{
  scene->addLine(spaceX, 0, spaceX + commitRadius / 2.0, spaceY * nodes.size(), QPen(QColor(Qt::white))); // fix minor vertical alignment problem in control
  RevisionPainter::draw();
}

QColor OdqRevisionPainter::getBranchColor(const RevGuiNode& node)
{
  int idxColor = node.x;
  ODA_ASSERT_ONCE(idxColor >= 0);
  if (idxColor < 0)
    idxColor = 0;
  int sz = (int)branchesColors.size();
  if (idxColor >= sz)
    idxColor = 1 + (idxColor - 1) % (sz - 1); // cycled except black color

  return branchesColors[idxColor];
}

void OdqRevisionPainter::drawNode(const RevGuiNode& node)
{
  QColor color = getBranchColor(node);
  QPen   pen(color);
  QBrush brush(color);
  scene->addEllipse(node.x * spaceX - commitRadius / 2.0 , (node.y + 0.5) * spaceY - commitRadius / 2.0, 
                    commitRadius, commitRadius, pen, brush);
}

void OdqRevisionPainter::drawBranch(const RevGuiNode& node,
                                    const RevGuiNode& parentNode)
{
  QPen   pen;
  QBrush brush;
  QColor color = getBranchColor((node.x < parentNode.x) ? parentNode : node);

  pen.setColor(color);
  brush.setColor(color);
  pen.setWidth(branchWidth);

  if (node.x == parentNode.x) {
    scene->addLine(node.x * spaceX, (node.y + 0.5) * spaceY, parentNode.x * spaceX, (parentNode.y + 0.5) * spaceY, pen);
  } else if (node.x < parentNode.x) {
    QPainterPath path;
    if (cornerRadius <= 1e-7)
    {
      path.moveTo(node.x * spaceX, (node.y + 0.5) * spaceY);
      path.cubicTo(node.x * spaceX - 10, (node.y + 0.5) * spaceY + 15,
                   (node.x + 1) * spaceX + 5, (node.y + 1.0) * spaceY,
                   parentNode.x * spaceX, (parentNode.y + 0.5) * spaceY);
      scene->addPath(path, pen, brush);
    }
    else
    {
      scene->addLine(parentNode.x * spaceX, (parentNode.y + 0.5) * spaceY,
                     parentNode.x * spaceX, (node.y + 0.5) * spaceY + cornerRadius, pen);

      path.moveTo(parentNode.x * spaceX, (node.y + 0.5) * spaceY + cornerRadius);
      path.arcTo(parentNode.x * spaceX - cornerRadius * 2, (node.y + 0.5) * spaceY,
                 cornerRadius * 2, cornerRadius * 2, 0, 90);
      scene->addPath(path, pen, brush);

      if ((node.x * spaceX) < (parentNode.x * spaceX - cornerRadius))
        scene->addLine(parentNode.x * spaceX - cornerRadius, (node.y + 0.5) * spaceY,
                       node.x * spaceX, (node.y + 0.5) * spaceY, pen);
    }
  } else {
    QPainterPath path;
    if (cornerRadius <= 1e-7)
    {
      path.moveTo(node.x * spaceX, (node.y + 0.5) * spaceY);
      path.cubicTo(node.x * spaceX + 10, (node.y + 0.5) * spaceY + 12,
                   (node.x - 1) * spaceX - 5, (node.y + 1.0) * spaceY,
                   parentNode.x * spaceX, (parentNode.y + 0.5) * spaceY);
      scene->addPath(path, pen, brush);
    }
    else
    {
      scene->addLine(node.x * spaceX, (node.y + 0.5) * spaceY, 
                     node.x * spaceX, (parentNode.y + 0.5) * spaceY - cornerRadius, pen);

      path.moveTo(node.x * spaceX, (parentNode.y + 0.5) * spaceY - cornerRadius);
      path.arcTo(node.x * spaceX - cornerRadius * 2, (parentNode.y + 0.5) * spaceY - cornerRadius * 2,
        cornerRadius * 2, cornerRadius * 2, 0, -90);
      scene->addPath(path, pen, brush);

      if ((parentNode.x * spaceX) < (node.x * spaceX - cornerRadius))
        scene->addLine(node.x * spaceX - cornerRadius, (parentNode.y + 0.5) * spaceY,
                       parentNode.x * spaceX, (parentNode.y + 0.5) * spaceY, pen);
    }
  }
}
