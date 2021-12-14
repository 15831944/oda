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
//  PdfExportParamsHolder.h : definition of the PdfExportParamsHolder class
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _2DEXPORT_UTILS_DEFINED
#define _2DEXPORT_UTILS_DEFINED

#include "Gs/GsBaseModel.h"
typedef OdSmartPtr<OdGsBaseModel> OdGsBaseModelPtr;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT
{

class SavedGsModelData
{
public:
  SavedGsModelData() {}
  SavedGsModelData(OdGiDrawable* pDbAsDrawable) { save(pDbAsDrawable); }
  SavedGsModelData(const SavedGsModelData& o)
  {
    m_model = o.m_model;
    m_savedData = o.m_savedData;
  }

  bool isValid() const { return !!m_model.get(); }
  void save(OdGiDrawable* pDbAsDrawable);
  void restore(OdGiDrawable* pDbAsDrawable);

private:
  OdGsBaseModelPtr m_model;
  OdRxObjectPtr m_savedData;
};

class AutoSavedGsModelData
{
public:
  AutoSavedGsModelData(OdGiDrawable* pDbAsDrawable): m_data(pDbAsDrawable), m_drw(pDbAsDrawable){}
  ~AutoSavedGsModelData() { m_data.restore(m_drw); }

private:
  SavedGsModelData m_data;
  OdGiDrawablePtr m_drw;
};

class AutoRestoreGsModelData
{
public:
  AutoRestoreGsModelData(SavedGsModelData& data, OdGiDrawable* pDbAsDrawable): 
    m_data(data), m_drw(pDbAsDrawable)
  {
    m_data.restore(m_drw);
  }
  ~AutoRestoreGsModelData()
  {
    m_data.save(m_drw);
  }
private:
  SavedGsModelData& m_data;
  OdGiDrawablePtr m_drw;
};

}
#endif //_2D_EXPORT_UTILS_DEFINED
