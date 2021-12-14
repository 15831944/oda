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

#ifndef _Pc3Parser_h_included_
#define _Pc3Parser_h_included_

#ifdef _MSC_VER
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4146)  // unary minus operator applied to unsigned type, result still unsigned
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4511)  // 'class' : copy constructor could not be generated
#pragma warning(disable: 4512)  // 'class' : assignment operator could not be generated
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize class template 'vector'
#endif

#include <vector>
#include <string>
#include <iostream>

namespace pc3
{
  struct Node
  {
    std::string _name;
    std::string _value;
  };

  struct Group
  {
    std::string _name;
    std::vector<Group*> _children;
    Group* _parent;
    std::vector<Node> _nodes;
    Group( Group* parent) : _parent( parent ) {}
    ~Group()
    {
      for ( unsigned int i = 0; i < _children.size(); i++ )
        delete _children[i];
    }
    bool addNode( const std::string& line );
  };
  
  class Parser 
  {
    Group _root;
    Group* _current;
  public:
    Parser() : _root( 0 ) { _current = &_root; }
    bool loadText( std::istream& );
    void saveXml( std::ostream& );
  };
}

#endif // _Pc3Parser_h_included_
