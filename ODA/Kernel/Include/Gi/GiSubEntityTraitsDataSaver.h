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

#ifndef __ODGISUBENTITYTRAITSDATASAVER_H__
#define __ODGISUBENTITYTRAITSDATASAVER_H__

#include "TD_PackPush.h"

#include "Gi/GiSubEntityTraitsData.h"
#include "Gi/GiDgLinetype.h"
#include "Gi/GiFill.h"

/** \details

    <group OdGi_Classes> 
*/
struct OdGiSubEntityTraitsDataSaver : public OdGiSubEntityTraitsData
{
  ODRX_HEAP_OPERATORS();

  /** \details
    Default constructor for the OdGiSubEntityTraitsDataSaver structure.
  */
  OdGiSubEntityTraitsDataSaver() : OdGiSubEntityTraitsData() { }
  
  /** \details
    Constructor for the OdGiSubEntityTraitsDataSaver structure. Sets the subentity traits data to save.
    \param from [in]  Subentity traits data to save.
  */
  OdGiSubEntityTraitsDataSaver(const OdGiSubEntityTraitsData &from) : OdGiSubEntityTraitsData(from) { allocPtrs(); }
  
  /** \details
    Copy constructor for the OdGiSubEntityTraitsDataSaver structure.
    \param from [in]  Traits saver to copy.
  */
  OdGiSubEntityTraitsDataSaver(const OdGiSubEntityTraitsDataSaver &from) : OdGiSubEntityTraitsData(from) { allocPtrs(); }

  /** \details
    Destructor for the OdGiSubEntityTraitsDataSaver structure. Deletes all the allocated data.
  */
  ~OdGiSubEntityTraitsDataSaver() { deletePtrs(); }

  /** \details
      Sets a mapper.
      \param pMapper [in]  Pointer to the mapper object.
  */
  void setMapper(const OdGiMapper* pMapper)
  {
    if (mapper()) delete mapper();
    if (pMapper) OdGiSubEntityTraitsData::setMapper(new OdGiMapper(*pMapper));
    else OdGiSubEntityTraitsData::setMapper(NULL);
  }
  /** \details
      Sets line style modifiers.
      \param pLSMod [in]  Pointer to the line style modifiers object.
  */
  void setLineStyleModifiers(const OdGiDgLinetypeModifiers* pLSMod)
  {
    if (lineStyleModifiers()) delete lineStyleModifiers();
    if (pLSMod) OdGiSubEntityTraitsData::setLineStyleModifiers(new OdGiDgLinetypeModifiers(*pLSMod));
    else OdGiSubEntityTraitsData::setLineStyleModifiers(NULL);
  }
  
  /** \details
      Sets the object fill.
      \param pFill [in]  Pointer to the fill object.
  */
  void setFill(const OdGiFill* pFill)
  {
    if (fill()) (OdGiFillPtr(fill(), kOdRxObjAttach));
    if (pFill) OdGiSubEntityTraitsData::setFill(static_cast<OdGiFill*>(pFill->clone().detach()));
    else OdGiSubEntityTraitsData::setFill(NULL);
  }

  /** \details
      Assignment operator, deletes previously allocated data, assigns the passed object and allocates pointers for new data.
      \param pFill [in]  Pointer to the fill object.
  */
  OdGiSubEntityTraitsData &operator =(const OdGiSubEntityTraitsData &data)
  {
    deletePtrs();
    static_cast<OdGiSubEntityTraitsData&>(*this) = data;
    allocPtrs();
    return *this;
  }

protected:
  void deletePtrs()
  {
    if (mapper()) delete mapper();
    if (lineStyleModifiers()) delete lineStyleModifiers();
    if (fill()) (OdGiFillPtr(fill(), kOdRxObjAttach));
  }
  void allocPtrs()
  {
    if (mapper()) OdGiSubEntityTraitsData::setMapper(new OdGiMapper(*mapper()));
    if (lineStyleModifiers()) OdGiSubEntityTraitsData::setLineStyleModifiers(new OdGiDgLinetypeModifiers(*lineStyleModifiers()));
    if (fill()) OdGiSubEntityTraitsData::setFill(static_cast<OdGiFill*>(fill()->clone().detach()));
  }
};

#include "TD_PackPop.h"

#endif // __ODGISUBENTITYTRAITSDATASAVER_H__
