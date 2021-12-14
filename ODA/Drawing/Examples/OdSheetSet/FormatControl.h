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

#ifndef _FormatControl_h_Included_
#define _FormatControl_h_Included_

#include "OdString.h"

/************************************************************************/
/* Maintains the indentation for printing                               */
/************************************************************************/
class offset
{
  static int _offset;
public:
  static const int TABSIZE;
  /**********************************************************************/
  /* Increase the offset by TABSIZE                                     */
  /**********************************************************************/
  static void increase( int value = TABSIZE )
  {
    _offset += value;
  }
  /**********************************************************************/
  /* Decrease the offset by TABSIZE                                     */
  /**********************************************************************/
  static void decrease( int value = TABSIZE )
  {
    _offset -= value;
  }
  static int getOffset()
  {
    return _offset;
  }
};
/************************************************************************/
/* Inserter that inserts _offset blanks                                 */
/************************************************************************/
std::wostream& operator << ( std::wostream& o, const offset& )
{
  for ( int i = 0; i < offset::getOffset(); ++i )
    o << " ";
  return o;
}
/************************************************************************/
/* Increase _offset for the scope of the instance                       */
/************************************************************************/
class OffsetShift
{
  int _ofs;
public:
  OffsetShift( int ofs = offset::TABSIZE ) : _ofs( ofs )
  {
    offset::increase( _ofs );
  }
  ~OffsetShift()
  {
    offset::decrease( _ofs );
  }
};

/************************************************************************/
/* Inserter that inserts on OdString                                    */
/************************************************************************/
std::wostream& operator << ( std::wostream& o, const OdString& s )
{
  return o << s.c_str();
}

#endif //_FormatControl_h_Included_
