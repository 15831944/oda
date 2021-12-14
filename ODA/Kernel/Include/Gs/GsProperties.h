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

#ifndef _ODGSPROPERTIES_H_INCLUDED_
#define _ODGSPROPERTIES_H_INCLUDED_

#include "OdaCommon.h"
#include "RxObject.h"
#include "Gi/GiDrawable.h"
#include "Gi/GiCommonDraw.h"
#include "Gs/GsExport.h"
#include "CmColorBase.h"

class OdDbStub;

class OdGiRasterImage;
class OdGsBaseVectorizer;
class OdGsViewImpl;

#include "TD_PackPush.h"

/** \details
    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class OdGsPropertiesDirectRenderOutput
{
  public:
    enum DirectRenderFlags
    {
      DirectRender_Point           = 1,
      DirectRender_LineFlat        = (1 << 1),
      DirectRender_LineGouraud     = (1 << 2),
      DirectRender_TriangleFlat    = (1 << 3),
      DirectRender_TriangleGouraud = (1 << 4),
      DirectRender_PolygoneFlat    = (1 << 5),
      DirectRender_PolygoneGouraud = (1 << 6),
      DirectRender_Image           = (1 << 7)
    };
    struct DirectRenderImageUV
    {
      double u, v;
    };
    struct DirectRenderImageParams
    {
      DirectRenderImageUV *uvCoords;
      const OdGiDrawable *pDrawable; // For images caching
    };
  public:
    virtual void directRenderOutputPoint(const OdGePoint3d &/*point*/,
                                         const OdCmEntityColor &/*color*/)
    {
    }
    virtual void directRenderOutputLineFlat(const OdGePoint3d &/*pt1*/, const OdGePoint3d &/*pt2*/,
                                            const OdCmEntityColor &/*color*/)
    {
    }
    virtual void directRenderOutputLineGouraud(const OdGePoint3d &/*pt1*/, const OdGePoint3d &/*pt2*/,
                                               const OdCmEntityColor &/*clr1*/, const OdCmEntityColor &/*clr2*/)
    {
    }
    virtual void directRenderOutputTriangleFlat(const OdGePoint3d &/*pt1*/, const OdGePoint3d &/*pt2*/, const OdGePoint3d &/*pt3*/,
                                                const OdCmEntityColor &/*color*/)
    {
    }
    virtual void directRenderOutputTriangleGouraud(const OdGePoint3d &/*pt1*/, const OdGePoint3d &/*pt2*/, const OdGePoint3d &/*pt3*/,
                                                   const OdCmEntityColor &/*clr1*/, const OdCmEntityColor &/*clr2*/, const OdCmEntityColor &/*clr3*/)
    {
    }
    virtual void directRenderOutputPolygoneFlat(OdUInt32 /*nPts*/, const OdGePoint3d * /*pPts*/,
                                                const OdCmEntityColor &/*color*/)
    {
    }
    virtual void directRenderOutputPolygoneGouraud(OdUInt32 /*nPts*/, const OdGePoint3d * /*pPts*/,
                                                   const OdCmEntityColor * /*pClrs*/)
    {
    }
    virtual void directRenderOutputImage(const OdGePoint3d * /*pRect*/, const OdGiRasterImage * /*pImage*/,
                                         const OdGsPropertiesDirectRenderOutput::DirectRenderImageParams &/*driParams*/)
    {
    }

    virtual OdUInt32 directRenderOutputFlags() const = 0;
    
    virtual ~OdGsPropertiesDirectRenderOutput() {}
};

/** \details
    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsProperties : public OdRxObject
{
public:
  /** \details
    Represents property types.
  */
  enum PropertiesType
  {
    /** Represents a property type for a graphics viewport. */
    kViewport          = 1,
    
    /** Represents a property type for a view background. */
    kBackground        = (1 << 1),
    
    /** Represents a property type for a visual style. */
    kVisualStyle       = (1 << 2),
    
    /** Represents a property type for a render environment. */
    kRenderEnvironment = (1 << 3),
    
    /** Represents a property type for render settings. */
    kRenderSettings    = (1 << 4),
    
    /** Represents a properties for a simple device. */
    kDeviceSimple      = 0,

    /** Represents a properties for a background-compatible device. */
    kDeviceBackground  = 3, // kViewport | kBackground

    /** Represents a properties for a normal vectorization device. */
    kDeviceNormal      = 7, // kViewport | kBackground | kVisualStyle

    /** Represents a properties for a rendering device. */
    kDeviceRender      = 31, // kAll

    /** Represents all properties combined. */
    kAll               = 31
  };
protected:
  void *m_pUnderlyingDrawable;
  const OdGiDrawable *m_pInconsistentDrawable;
  OdGiContext* m_context;

  OdGiDrawablePtr (*m_pOpenDrawableFn)(OdDbStub* id);
  bool m_bDrawablePersistent;
  // For optimize
  OdGiDrawable::DrawableType m_dType;
public:
  ODRX_DECLARE_MEMBERS(OdGsProperties);

  /** \details
    Default constructor for the OdGsProperties class. Sets the following data member:
    <table>
    Data member                             Value                                             Description
    m_pUnderlyingDrawable                   NULL                                              Underlying drawable
    m_pInconsistentDrawable                 NULL                                              Inconsistent (transient) drawable
    m_context                               NULL                                              Gi context
    m_bDrawablePersistent                   false                                             Flag that shows whether persistent (database-resident) drawable is used
    m_dType                                 (OdGiDrawable::DrawableType)0xFFFFFFFF            Drawable type
    </table>
  */
  OdGsProperties();

  void setOpenDrawableFn(OdGiDrawablePtr (*fn)(OdDbStub*));

  /** \details
    Sets underlying drawable.
    
    \param pUnderlyingDrawable  [in]  Pointer to the underlying drawable.
    \param ctx                  [in]  Pointer to the GI context.
  */
  void setUnderlyingDrawable(const OdGiDrawable *pUnderlyingDrawable, OdGiContext* ctx);
  
  /** \details
    Retrieves a smart pointer to n underlying drawable.
  */
  OdGiDrawablePtr underlyingDrawable() const;
  
  /** \details
    Checks whether associated underlying drawable has changed.
    
    \returns
    true if associated underlying drawable has changed, false otherwise.
  */
  bool isUnderlyingDrawableChanged(const OdGiDrawable *pUnderlyingDrawable) const;
  
  /** \details
    Checks whether this object has an associated underlying drawable.
    
    \returns
    true if this object has an associated underlying drawable, false otherwise.
  */
  bool hasUnderlyingDrawable() const;
 
  /** \details
    Checks whether an associated underlying drawable is persistent (database-resident).
    
    \returns
    true if an associated underlying drawable is persistent, false otherwise.
  */
  bool isUnderlyingDrawablePersistent() const;
  
  /** \details
    Retrieves a pointer to a persistent object ID of an underlying drawable.
  */
  OdDbStub* underlyingDrawableId() const;

  /** \details
    Retrieves a type of an underlying drawable.
  */
  OdGiDrawable::DrawableType underlyingDrawableType() const;

  /** \details
    Retrieves a type of properties.
  */
  virtual PropertiesType propertiesType() const = 0;

  // Search down in hierarchy for appropriate properties
  /** \details
    Searches in a hierarchy corresponding properties by the specified properties type.
    and returns a smart pointer to the searched properties. 
  */
  virtual OdSmartPtr<OdGsProperties> propertiesForType(PropertiesType /*type*/) const;

  // Update attributes
  virtual void update(const OdGiDrawable* pUnderlyingDrawable,
      OdGsViewImpl* view, OdUInt32 incFlags = kAll) = 0;
  // Display attributed data
  virtual void display(OdGsBaseVectorizer& view,
      OdGsPropertiesDirectRenderOutput *pdro = NULL, OdUInt32 incFlags = kAll) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsProperties object pointers.
*/
typedef OdSmartPtr<OdGsProperties> OdGsPropertiesPtr;

#include "TD_PackPop.h"

#endif // _ODGSPROPERTIES_H_INCLUDED_
