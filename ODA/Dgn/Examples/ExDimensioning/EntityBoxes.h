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
#ifndef __EX_ENTITYBOXES_H__
#define __EX_ENTITYBOXES_H__

#define WIDTH_BOX   2.25
#define HEIGHT_BOX  3.25

#define HOR_SPACE   0.625
#define VER_SPACE   0.375

#define HOR_BOXES   11
#define VER_BOXES   7


#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#include "Ge/GePoint3d.h"

class EntityBoxes
{
  static const int BoxSizes[VER_BOXES][HOR_BOXES];
  
public:
  EntityBoxes(){}
  ~EntityBoxes(){}

  /**********************************************************************/
  /* Return the width of the specified box                              */
  /**********************************************************************/
  double getWidth ( int row, int col ) const
  {
    return BoxSizes[row][col]*WIDTH_BOX + (BoxSizes[row][col] - 1 )*HOR_SPACE;
  }
  /**********************************************************************/
  /* Return the height of specified box                                 */
  /**********************************************************************/
  double getHeight () const
  {
    return HEIGHT_BOX;
  }
  /**********************************************************************/
  /* Return true if and only if the specified box is a box              */
  /**********************************************************************/
  bool isBox(int row, int col)
  {
    return BoxSizes[row][col] > 0 ? true : false;
  }
  
  /**********************************************************************/
  /* Return the upper-left corner of the specified box                  */
  /**********************************************************************/
  OdGePoint3d getBox( int row, int col ) const
  {
    OdGePoint3d point;
    if ( col > HOR_BOXES-1 )
      return point;

    point = OdGePoint3d(0, HEIGHT_BOX * VER_BOXES + VER_SPACE * (VER_BOXES-1), 0);
    
    for (int i=0; i < col;  i++ )
    {
      point.x += BoxSizes[row][i]*WIDTH_BOX;
      point.x += (BoxSizes[row][i])*HOR_SPACE;
    }
    point.y -= row*HEIGHT_BOX;
    point.y -= row*VER_SPACE;
    return point;
  }

  /**********************************************************************/
  /* Return the center of the specified box                             */
  /**********************************************************************/
  OdGePoint3d getBoxCenter(int row, int col) const
  {
    OdGePoint3d point = getBox(row,col);
    double w = getWidth(row,col);
    point.x += w/2.0;
    point.y -= HEIGHT_BOX/2.0;
    return point;
  }

  /**********************************************************************/
  /* Return the size of the box array                                   */
  /**********************************************************************/
  OdGeVector3d getArraySize() const
  {
    return OdGeVector3d(WIDTH_BOX * HOR_BOXES + HOR_SPACE * (HOR_BOXES-1), 
                       -(HEIGHT_BOX * VER_BOXES + VER_SPACE * (VER_BOXES-1)),
                       0);
  }

  /**********************************************************************/
  /* Return the center of the box array                                 */
  /**********************************************************************/
  OdGePoint3d getArrayCenter() const
  {
    return OdGePoint3d(0.5 * (WIDTH_BOX * HOR_BOXES + HOR_SPACE * (HOR_BOXES-1)), 
                       0.5 * (HEIGHT_BOX * VER_BOXES + VER_SPACE * (VER_BOXES-1)),
                       0);
  }
};

#endif // __EX_ENTITYBOXES_H__
