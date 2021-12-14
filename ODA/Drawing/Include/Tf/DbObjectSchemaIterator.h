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
#ifndef _ODDBSCHEMAITERATOR_H_INCLUDED_
#define _ODDBSCHEMAITERATOR_H_INCLUDED_

#include "Tf/Tf.h"
#include "DbHostAppServices.h"


class TFDLL_EXPORT OdDbObjectSchemaIterator : public OdRxObject {
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectSchemaIterator);

  virtual bool seekClass(const OdRxClass* pClass, OdDbHostAppServices* pAppServices) = 0;
  virtual OdTf::TypesEnum  currentType() = 0;
  virtual void step() = 0;
  virtual void skipDefDouble() = 0;
  virtual OdString currentSubClass() = 0;
  virtual OdString currentSubObject() = 0;
  virtual OdString currentSubObjectName() = 0;

  virtual OdString seekFieldGetName() = 0;
  virtual OdString seekFieldGetName(const OdString& sValue) = 0;
  virtual OdString seekFieldGetName(const OdInt64& data) = 0;
  virtual OdString seekFieldGetName(const double& value) = 0;
  virtual OdString seekFieldGetName(const double& valueX, const double& valueY) = 0;
  virtual OdString seekFieldGetName(const double& valueX, const double& valueY, const double& valueZ) = 0;
  virtual OdString seekFieldGetName_onBinaryChunk() = 0;

  virtual bool done() const = 0;
};

typedef OdSmartPtr<OdDbObjectSchemaIterator> OdDbObjectSchemaIteratorPtr;


#endif //_ODDBSCHEMAITERATOR_H_INCLUDED_
