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
// ExCommandsUtils.h: Declarations of the ExCommands utility functions.
//
//////////////////////////////////////////////////////////////////////

#ifndef EXCOMMANDSUTILS_INCLUDED
#define EXCOMMANDSUTILS_INCLUDED

class OdDbDatabase;

namespace ExCommandsUtils
{
  // Returns Block Record id entity is to be appended
  OdDbObjectId activeBlockTableRecord(const OdDbDatabase* pDb);

  OdGeVector3d ucsNormal(const OdDbDatabase * pDb);
};


////////////////////////////////////////////////////////////////////////////
// Iterator returning entities from current space
// except presenting in Selection Set

class OdDbInvertedSelectionSetIterator : protected OdDbSelectionSetIterator
{
public:
  OdDbInvertedSelectionSetIterator()  {}
  
  void init(OdDbSelectionSet* pSSet);
  OdDbObjectId objectId() const;
  OdDbSelectionMethodPtr method() const;
  bool done() const;
  bool next();
  OdUInt32 subentCount();
  bool getSubentity(OdUInt32 i, OdDbBaseFullSubentPath& path);
  OdDbStub* id() const;

protected:
  void checkPos();
  OdDbObjectIteratorPtr m_pBlkIter;
  OdDbSelectionSet*     m_pSSet;
};

#endif

