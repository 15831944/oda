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




#ifndef _ODA_LISTT_H_
#define _ODA_LISTT_H_

#define STL_USING_LIST
#include "OdaSTL.h"

#include "OdMemory.h"


#ifdef OD_STD_ALLOCATOR_AS_IS
/** \details
    This template class extends the functionality of the standard STL class std::list.
    <group Other_Classes>
*/
template<class T, class A = OD_TYPENAME3 std::allocator<T> >
#else
/** \details
    This template class extends the functionality of the standard STL class std::list.
    <group Other_Classes>
*/
template<class T, class A = OdAllocator<T> >
#endif
class OdList : public std::list<T, A>
{
public:
	typedef typename std::list<T,A>::iterator iterator;
  
  /** \details
    Default constructor for the OdList class.
  */
	OdList() {}
  
  /** \details
    Destructor for the OdList class.
  */
	virtual ~OdList() {}
	/** \details
	  Appends an item to the end of the list.
	  
	  \returns
	  Returns an iterator pointing to the new member.
	*/
	iterator append() { return this->insert(this->end(), T()); }
};


template<class T, class A> class OdList;

#endif //_ODA_LISTT_H_

