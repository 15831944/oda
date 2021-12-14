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


///////////////////////////////////////////////////////////////////////////////
//
// 2dExportUtils.cpp : implementation of 2d export utility classes
//
///////////////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
#include "2dExportUtils.h"

namespace TD_PDF_2D_EXPORT
{

void SavedGsModelData::save(OdGiDrawable* pDbAsDrawable)
{
#ifdef BS_CHANGES_ENABLED
  ODA_ASSERT(m_model.isNull());
  ODA_ASSERT(m_savedData.isNull());
  if(pDbAsDrawable && pDbAsDrawable->gsNode() && pDbAsDrawable->gsNode()->model())
  {
    OdGsBaseModel* pBaseModel = dynamic_cast<OdGsBaseModel*>(pDbAsDrawable->gsNode()->model());
    if(pBaseModel)
    {
      m_model = pBaseModel;
      m_savedData = pBaseModel->saveAndDetach(*pDbAsDrawable);
    }
  }
#endif //BS_CHANGES_ENABLED
}

void SavedGsModelData::restore(OdGiDrawable* pDbAsDrawable)
{
#ifdef BS_CHANGES_ENABLED
  if(m_model.get())
  {
    m_model->restoreAndAttach(*pDbAsDrawable, m_savedData.get());
    m_model = nullptr;
  }
  m_savedData = nullptr;
#endif //BS_CHANGES_ENABLED
}

}
