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

#include "Pc3Parser.h"
#include <iostream>
#include <algorithm>

namespace pc3
{ 
  /**********************************************************************/
  /* Replace all instances of  & " < > with their XML equivalents       */
  /**********************************************************************/
  std::string xml( const std::string& s )
  {
    std::string text = s;
    std::string::size_type i = std::string::npos;
    while( ( i = text.find( '&', i + 1 ) ) != -1 ) 
      text = text.substr( 0, i ) + "&amp;" + text.substr( i + 1 );
    while( ( i = text.find( '\"', i + 1) ) != -1 )
      text = text.substr( 0, i ) + "&quot;" + text.substr( i + 1 );
    while( ( i = text.find( '<', i + 1 ) ) != -1 )
      text = text.substr( 0, i ) + "&lt;" + text.substr( i + 1 );
    while( ( i = text.find( '>', i + 1 ) ) != -1 )
      text = text.substr( 0, i ) + "&gt;" + text.substr( i + 1 );
    return text;
  }

  /**********************************************************************/
  /* Insertion operator for Node objects                                */
  /**********************************************************************/
  std::ostream& operator << ( std::ostream& str, const Node& n )
  {
    return str << "<node name=\"" << xml( n._name ) 
      << "\" value=\"" << xml( n._value ) << "\"/>\n";
  }

  /**********************************************************************/
  /* Insertion operator for Group objects                               */
  /**********************************************************************/
  std::ostream& operator << ( std::ostream& str, const Group& group )
  {
    str << "<group name=\"" << xml( group._name ) << "\">\n";

    unsigned int i = 0;
    for ( ; i < group._nodes.size(); i++ )
      str << group._nodes[ i ];

    for ( i = 0; i < group._children.size(); i++ )
      str << *group._children[ i ];

    return str << "</group>\n";
  }


  /**********************************************************************/
  /* Outputs the version and XML tree                                   */
  /**********************************************************************/
  void Parser::saveXml( std::ostream& str )
  {
    str << "<?xml version=\"1.0\"?>\n" << _root;
  }
  /**********************************************************************/
  /* Start a new group as a child of current in the form <name>{        */
  /* Strip leading white space and trailing '{' from line to form       */
  /* group->_name                                                       */
  /**********************************************************************/
  static Group* startGroup( Group* current, const std::string& line )
  {
    Group* g = new Group( current );
    current->_children.push_back( g );
    std::string::size_type start = line.find_first_not_of( " \t" );
    g->_name = line.substr( start, line.size() - start - 1 );
    return g;
  }
  /**********************************************************************/
  /* Process instr, return true if and only if successful               */
  /**********************************************************************/
  bool Parser::loadText( std::istream& instr )
  {
    for ( std::string line; std::getline( instr, line ); )
    {
      if (std::find(line.begin(), line.end(), 0) != line.end())
        continue;
      else if ( line[line.size() - 1] == '{' )
        /****************************************************************/
        /* Begin a new group, and make it _current                      */
        /****************************************************************/ 
        _current = startGroup( _current, line );
      else if ( line[line.size() - 1] == '}' && _current->_parent )
        /****************************************************************/
        /* End a group, and make its parent _current                    */
        /****************************************************************/ 
        _current = _current->_parent;
      else 
        /****************************************************************/
        /* Add a node to the current group                              */
        /****************************************************************/ 
        if ( !_current->addNode( line ) ) 
          return false;
    }
    return true;
  }
  /**********************************************************************/
  /* Add a node in the form <name>=<value> to the current group         */
  /* Returns true if and only if successful                             */
  /**********************************************************************/
  bool Group::addNode( const std::string& line )
  {
    std::string::size_type i = line.find( '=' );
    /********************************************************************/
    /* Fail if line does not contain a '='                              */
    /********************************************************************/
    if ( i == std::string::npos ) return false;
    /********************************************************************/
    /* Add a node to the _nodes array                                   */
    /**********************************************************************/
    _nodes.resize( _nodes.size() + 1 );
    /********************************************************************/
    /* Strip the leading white space from line                          */
    /********************************************************************/
    std::string::size_type start = line.find_first_not_of( " \t" );
    /****************************(***************************************/
    /* Extract the name and value                                       */
    /********************************************************************/
    _nodes.back()._name = line.substr( start, i - start );
    _nodes.back()._value = line.substr( i + 1 );
    return true;
  }
}
