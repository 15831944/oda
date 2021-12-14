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

#ifndef __FMMDLTAGTOOLS_H__
#define __FMMDLTAGTOOLS_H__

#include "OdArray.h"

namespace FacetModeler {


struct TaggedData {
  Entity*     m_pEntity;
  TagType     m_savedTag;

  void detach() {
    if( m_pEntity ) {
      m_pEntity->setTag( m_savedTag );
      m_pEntity = 0;
    }
  }

  void attach( Entity* pEntity ) {
    m_pEntity = pEntity;
    if ( pEntity ) 
    {
        m_savedTag = pEntity->tag();
        pEntity->setTag( (TagType)this );
    }
  }

  static TaggedData* castTag( Entity* pEntity ) {
    return (TaggedData*)pEntity->tag();
  }
};

template <class TDataEntry = TaggedData, class TAllocator = OdObjectsAllocator<TDataEntry> >
class TaggedDataHeap {
  OdArray<TDataEntry, TAllocator> m_entries;
public:
  TaggedDataHeap() {
  }
  ~TaggedDataHeap() {
    restoreTags();
  }
  void setMaxSize( OdUInt32 nStorageMaxSize ) {
    restoreTags();
    m_entries.reserve( nStorageMaxSize );
  }
  TDataEntry* append( Entity* pEntity ) {
    ODA_ASSERT( m_entries.physicalLength() > m_entries.size() ); // make sure that entry pointers are not changed
    TDataEntry* pNew = m_entries.append();
    pNew->attach( pEntity );
    return pNew;
  }

  void restoreTags() {
    while( m_entries.size() ) {
      m_entries.last().detach();
      m_entries.removeLast();
    }
  }
};

}

#endif //__FMTAGTOOLS_H__
