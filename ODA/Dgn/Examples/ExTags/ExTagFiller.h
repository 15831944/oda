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
#ifndef __EX_TAG_FILLER_H__
#define __EX_TAG_FILLER_H__

#include "DgModel.h"
#include "DgTagElement.h"

class OdExTagFiller
{
public:
  OdExTagFiller();
  
  void fillModel(OdDgModel* pModel);

private:

  void FillTable();
  void CreateTextElementAsParent();
  OdDgTagElementPtr CreateTag( OdUInt16 definitionID );

  void addFontTableRecord();

  void createTags_offset();
  void createTags_rotation();
  void createTags_associationFlag();
  void createTags_types();
  void createTags_formattingFlags();
  void createTags_slant();
  void createTags_characterSpacing();
  void createTags_underlineSpacing();
  void createTags_sizeMultiplier();
  void createTags_textStyle();
  void createTags_font();
  void createTags_justification();

  OdDgModel* m_pModel3d;
  OdDgElementId m_ParentElementID;
  OdDgElementId m_DefinitionSetID;

  OdUInt16 m_stringDefinitionId;
  OdUInt16 m_int16DefinitionId;
  OdUInt16 m_int32DefinitionId;
  OdUInt16 m_doubleDefinitionId;
  OdUInt16 m_binaryDefinitionId;

};

#endif // __EX_TAG_FILLER_H__
