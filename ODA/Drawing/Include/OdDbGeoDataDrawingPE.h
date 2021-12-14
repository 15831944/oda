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

#ifndef OD_DBGEODATADRAWINGPE_H
#define OD_DBGEODATADRAWINGPE_H

#include "Gi/GiDrawable.h"
#include "Gs/GsModel.h"

/** \details
An abstract class that represents a protocol extension for an OdDbGeoData object to draw chunks images for OdDbGeoData.

<group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGeoDataChunksPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoDataChunksPE);

  /** \details
    Virtual destructor.
  */
  virtual ~OdDbGeoDataChunksPE() {};
  /** \details
    Draws chunks images for OdDbGeoData.
    
    \param pVd [in] Pointer to the OdGiViewportDraw interface.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult drawGeoDataChunks(OdGiViewportDraw* pVd) const = 0;

  /** \details
    Draws providers string for currently drawn OdDbGeoData chuncks.
    
    \param pVd [in] Pointer to the OdGiViewportDraw interface.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult drawGeoDataProvidersString(OdGiViewportDraw* pVd) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbGeoDataChunksPE object pointers.
*/
typedef OdSmartPtr<OdDbGeoDataChunksPE> OdDbGeoDataChunksPEPtr;

class OdDbGeoDataChunks;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbGeoDataChunks object pointers.
*/
typedef OdSmartPtr<OdDbGeoDataChunks> OdDbGeoDataChunksPtr;

/** \details
  This class represents implementation of OdGiDrawable, that is used to draw chucks images of OdDbGeoData.

  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGeoDataChunks : public OdGiDrawable
{
  OdDbGeoDataChunks& operator = (const OdDbGeoDataChunks&);
  OdGsCache* m_pGsNode;
  OdRefCounter m_nRefCounter;
protected:
  OdDbGeoDataChunks();
public:
  void addRef()
  {
    ++m_nRefCounter;
  }
  void release()
  {
    ODA_ASSERT((m_nRefCounter > 0));
    if (!(--m_nRefCounter))
    {
      delete this;
    }
  }
  long numRefs() const { return m_nRefCounter; }

  void setGsNode(OdGsCache* pGsNode) { m_pGsNode = pGsNode; }
  OdGsCache* gsNode() const { return m_pGsNode; }
  bool isPersistent() const { return false; }
  OdDbStub* id() const { return 0; }

public:
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw(OdGiViewportDraw* pVd) const;

  /** \details
    Virtual destructor.
  */
  ~OdDbGeoDataChunks();

  /** \details
    It is the pseudo-constructor that creates an instance of this class and returns the typified smart pointer to it.
  */
  static OdDbGeoDataChunksPtr createObject()
  {
    return OdRxObjectImpl<OdDbGeoDataChunks>::createObject();
  }
};


class OdDbGeoDataProvidersString;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbGeoDataProvidersString object pointers.
*/
typedef OdSmartPtr<OdDbGeoDataProvidersString> OdDbGeoDataProvidersStringPtr;

/** \details
  This class represents implementation of OdGiDrawable, that is used to draw providers string for OdDbGeoData.

  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGeoDataProvidersString : public OdGiDrawable
{
  OdDbGeoDataProvidersString& operator = (const OdDbGeoDataProvidersString&);
  OdGsCache* m_pGsNode;
  OdRefCounter m_nRefCounter;
protected:
  OdDbGeoDataProvidersString();
public:
  void addRef()
  {
    ++m_nRefCounter;
  }
  void release()
  {
    ODA_ASSERT((m_nRefCounter > 0));
    if (!(--m_nRefCounter))
    {
      delete this;
    }
  }
  long numRefs() const { return m_nRefCounter; }

  void setGsNode(OdGsCache* pGsNode) { m_pGsNode = pGsNode; }
  OdGsCache* gsNode() const { return m_pGsNode; }
  bool isPersistent() const { return false; }
  OdDbStub* id() const { return 0; }

public:
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw(OdGiViewportDraw* pVd) const;

  /** \details
    Virtual destructor.
  */
  ~OdDbGeoDataProvidersString();

  /** \details
    It is the pseudo-constructor that creates an instance of this class and returns the typified smart pointer to it.
  */
  static OdDbGeoDataProvidersStringPtr createObject()
  {
    return OdRxObjectImpl<OdDbGeoDataProvidersString>::createObject();
  }
};
#endif
