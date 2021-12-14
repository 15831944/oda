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

#ifndef EX_H_HARDWARESECTIONS
#define EX_H_HARDWARESECTIONS

#include "TD_PackPush.h"

#include "RxObject.h"
#include "Ge/GeMatrix3d.h"
#include "Ps/PlotStyles.h"
#include "StaticRxObject.h"

class OdGsView;

/** \details
    <group ExRender_Classes>
*/
class OdHardwareSectionsInterface : public OdStaticRxObject<OdRxObject>
{
  public:
    struct SectionDef
    {
      OdGePoint3d m_position;
      OdGeVector3d m_normal;

      SectionDef() {}
      SectionDef(const OdGePoint3d &pos, const OdGeVector3d &norm)
        : m_position(pos), m_normal(norm) {}

      void setPosition(const OdGePoint3d &pos) { m_position = pos; }
      const OdGePoint3d &position() const { return m_position; }

      void setNormal(const OdGeVector3d &norm) { m_normal = norm; }
      const OdGeVector3d &normal() const { return m_normal; }
    };
  public:
    OdHardwareSectionsInterface() {}

    virtual void addSectionPlane(OdGsView *pView, const SectionDef &secDef) = 0;
    virtual void removeSectionPlane(OdGsView *pView, OdUInt32 nPlane) = 0;
    virtual void updateCuttingPlane(OdGsView* pView, OdUInt32 nPlane, const SectionDef& secDef) = 0;

    virtual OdUInt32 numSectionPlanes(const OdGsView *pView) const = 0;
    virtual void getSectionPlane(const OdGsView *pView, OdUInt32 nPlane, SectionDef &secDef) const = 0;

    virtual void clearSectionPlanes(OdGsView *pView) = 0;
    virtual void transformSectionPlanes(OdGsView *pView, const OdGeMatrix3d &xForm) = 0;
    virtual void transformSectionPlane(OdGsView* pView, OdUInt32 nPlane, const OdGeMatrix3d& xForm) = 0;

    virtual void setEnableSectionsFill(OdGsView *pView, bool bEnable) = 0;
    virtual bool isSectionsFillEnabled(const OdGsView *pView) const = 0;

    virtual void setSectionsFillColor(OdGsView *pView, ODCOLORREF color) = 0;
    virtual ODCOLORREF sectionsFillColor(const OdGsView *pView) const = 0;

    virtual void setEnableSectionsFillPattern(OdGsView *pView, bool bEnable) = 0;
    virtual bool isSectionsFillPatternEnabled(const OdGsView *pView) const = 0;

    virtual void setSectionsFillPattern(OdGsView *pView, OdPs::FillStyle pattern) = 0;
    virtual OdPs::FillStyle sectionsFillPattern(const OdGsView *pView) const = 0;

    virtual void setSectionsFillPatternColor(OdGsView *pView, ODCOLORREF color) = 0;
    virtual ODCOLORREF sectionsFillPatternColor(const OdGsView *pView) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdHardwareSectionsInterface object pointers. 
*/
typedef OdSmartPtr<OdHardwareSectionsInterface> OdHardwareSectionsInterfacePtr;

#include "TD_PackPop.h"

#endif // EX_H_HARDWARESECTIONS
