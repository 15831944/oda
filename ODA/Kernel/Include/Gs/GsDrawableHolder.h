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
#ifndef ODGSDRAWABLEHOLDER_INC
#define ODGSDRAWABLEHOLDER_INC

#include "TD_PackPush.h"
#include "Ge/GeExtents3d.h"
#include "IntArray.h"
#include "Gi/GiDrawable.h"
#include "Gs/GsExport.h"

class OdGsNode;
class OdGsBaseModel;
class OdDbStub;


/** \details
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsUpdateExtents
{
  protected:
    OdGeExtents3d m_extents;
    OdDb::LineWeight m_lineweight;
  public:
    /** Invalid extents. */
    GS_STATIC_EXPORT static const OdGsUpdateExtents kInvalid;
  public:
    /** \details
      Default constructor for the OdGsUpdateExtents class. Initializes lineweight to zero lineweight.
    */
    OdGsUpdateExtents()
      : m_lineweight(OdDb::kLnWt000)
    {
    }
    /** \details
      Constructor for the OdGsUpdateExtents class. Creates an instance based on the passed extents and lineweight.
      
      \param extents  [in]  Extents.
      \param lw       [in]  Lineweight.
    */
    OdGsUpdateExtents(const OdGeExtents3d &extents, OdDb::LineWeight lw = OdDb::kLnWt000)
      : m_extents(extents)
      , m_lineweight(lw)
    {
    }

    /** \details
      Returns extents of this object.
    */
    const OdGeExtents3d &getExtents() const
    {
      return m_extents;
    }
    
    /** \details
      Returns extents of this object.
    */
    OdGeExtents3d &accessExtents()
    {
      return m_extents;
    }
    
    /** \details
      Sets extents of this object.
      \param extents  [in]  Extents.
    */
    void setExtents(const OdGeExtents3d &extents)
    {
      m_extents = extents;
    }
    
    /** \details
      Resets extents of this object to invalid extents.
    */
    void resetExtents()
    {
      m_extents = OdGeExtents3d::kInvalid;
    }
    
    /** \details
      Expands the available extents of this object.
      \param extents  [in]  Extents.
    */
    void addExtents(const OdGeExtents3d &extents)
    {
      if (extents.isValidExtents())
        m_extents.addExt(extents);
    }

    /** \details
      Checks whether this object has non-zero lineweight.
      \returns
      true if this object has non-zero lineweight, false otherwise.
    */
    bool hasLineweight() const
    {
      return (m_lineweight > OdDb::kLnWt000);
    }
    
    /** \details
      Returns lineweight of this object.
    */
    OdDb::LineWeight getLineweight() const
    {
      return m_lineweight;
    }
    
    /** \details
      Returns lineweight of this object.
    */
    OdDb::LineWeight &accessLineweight()
    {
      return m_lineweight;
    }
    
    /** \details
      Sets lineweight of this object.
      \param lw  [in]  Lineweight.
    */
    void setLineweight(OdDb::LineWeight lw)
    {
      m_lineweight = lw;
    }
    
    /** \details
      Resets lineweight of this object to zero lineweight.
    */
    void resetLineweight()
    {
      m_lineweight = OdDb::kLnWt000;
    }
    
    /** \details
      Replaces lineweight of this object with the passed lineweight if the passed lineweight has greater width.
      \param lw  [in]  Lineweight.
    */
    void addLineweight(OdDb::LineWeight lw)
    {
      if (m_lineweight < lw)
        m_lineweight = lw;
    }

    /** \details
      Retrieves parameters of this object.
      \param extents  [out]  Receives extents of this object.
      \param lw       [out]  Receives lineweight of this object.
    */
    void get(OdGeExtents3d &extents, OdDb::LineWeight &lw) const
    {
      extents = m_extents;
      lw = m_lineweight;
    }
    
    /** \details
      Sets parameters for this object.
      \param extents  [in]  Extents to set.
      \param lw       [in]  Lineweight to set.
    */
    void set(const OdGeExtents3d &extents, OdDb::LineWeight lw)
    {
      m_extents = extents;
      m_lineweight = lw;
    }
    
    /** \details
      Resets extents of this object to invalid extents and lineweight of this object to zero lineweight.
    */
    void reset()
    {
      resetExtents();
      resetLineweight();
    }
    
    /** \details
      Expands extents of this object and replaces lineweight of this object if the passed lineweight has greater width.
      \param extents  [in]  Extents.
      \param lw       [in]  Lineweight.
    */
    void add(const OdGeExtents3d &extents, OdDb::LineWeight lw)
    {
      addExtents(extents);
      addLineweight(lw);
    }
    
    /** \details
      Expands extents of this object and replaces lineweight of this object if the passed object has with greater width.
      
      \param extents  [in]  Other object to add.
    */
    void add(const OdGsUpdateExtents &extents)
    {
      add(extents.getExtents(), extents.getLineweight());
    }

    /** \details
      Checks whether extents of this object is valid.
      
      \returns
      true if extents of this object is valid, false otherwise.
    */
    bool isValid() const
    {
      return m_extents.isValidExtents();
    }
};

/** \details
  <group OdGs_Classes> 
    
  Corresponding C++ library: TD_Gs
*/
struct DrawableHolder
{
  DrawableHolder(): m_drawableId(0), m_pGsRoot(0)
  {
  }
  OdDbStub* m_drawableId;
  OdGiDrawablePtr m_pDrawable;
  OdSmartPtr<OdGsBaseModel> m_pGsModel;
  OdGsNode* m_pGsRoot;
  OdRxObjectPtr m_pMetafile;
  OdGsUpdateExtents m_lastExt;
};
typedef OdArray<DrawableHolder> DrawableHolderArray;

#include "TD_PackPop.h"

#endif // ODGSDRAWABLEHOLDER_INC
