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

#ifndef __FMMDL_ENTITY_H__
#define __FMMDL_ENTITY_H__

#include "Modeler/FMMdlBase.h"

namespace FacetModeler
{

  /** \details
     Gets a bit mask for the flag by its index.

     \param i [in] Index of the flag.
     \returns Bit mask as an OdUInt32 value.
   */
  OdUInt32 inline FLAG(OdUInt32 i) { return ((OdUInt32)1 << i); }

  /** \details
     This is the base class for all modeler primitives.
  */
  class FMGEOMETRY_API Entity
  {
  public:
    /** \details
       Default constructor.
    */
    Entity();

    /** \details
       Copy constructor. Creates an Entity object and initializes it with the given object.
      
       \param ent [in] Entity to copy from.
    */
    Entity(const Entity& ent);

    /** \details
       Assignment operator.

       \param entity [in] Entity to assign.
       \returns Reference to the resulting entity.
    */
    Entity& operator =(const Entity& entity);

    /** \details
       Gets all flags.

       \returns All of the entity's flags as an OdUInt32 value.
     */
    OdUInt32 flags() const;

    /** \details
       Sets all flags.

       \param iFlags [in] All flags as an OdUInt32 number.
    */
    void setFlags(OdUInt32 iFlags);

    /** \details
       Checks whether the specified flag is on.

       \param iFlag [in] Bit mask of the flag to check.
       \returns true if the flag is on, false otherwise.
     */
    bool isFlagOn(OdUInt32 iFlag) const;

    /** \details
       Sets the specified flag to the specified state.

       \param iFlag [in] Bit mask of the flag to set.
       \param bOn   [in] New state to set.
     */
    void setIsFlagOn(OdUInt32 iFlag, bool bOn);

    /** \details
       Gets an entity tag.

       \returns Entity tag as a TagType value.
     */
    TagType tag() const;

    /** \details
       Sets the tag to the specified one.

       \param nTag [in] Tag to set.
     */
    void setTag(TagType nTag);
    
    /** \details 
      Destructor.
    */
    virtual ~Entity();

  private:
    OdUInt32    m_flags; // Flags bit fields.
    TagType     m_nTag;  // Tag data.
  };


  inline Entity::Entity() {
    m_flags = 0;
    m_nTag = 0;
  }

  inline Entity::Entity(const Entity& ent) {
    m_nTag = ent.m_nTag;
    m_flags = ent.m_flags;
    *this = ent;
  }

  inline Entity& Entity::operator =(const Entity& ent) {
    setTag(ent.tag());
    m_flags = ent.m_flags;
    return *this;
  }

  inline Entity::~Entity() {
  }

  inline OdUInt32 Entity::flags() const {
    return m_flags;
  }

  inline void Entity::setFlags(OdUInt32 iFlags) {
    m_flags = iFlags;
  }

  inline bool Entity::isFlagOn(OdUInt32 iFlag) const {
    return GETBIT(m_flags, iFlag);
  }

  inline void Entity::setIsFlagOn(OdUInt32 iFlag, bool bOn) {
    SETBIT(m_flags, iFlag, bOn);
  }

  inline TagType Entity::tag() const {
    return m_nTag;
  }

  inline void Entity::setTag(TagType nTag) {
    m_nTag = nTag;
  }

}

#endif //__FMMDL_ENTITY_H__
