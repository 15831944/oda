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

#ifndef __ODGITRANSIENTMANAGER_H__
#define __ODGITRANSIENTMANAGER_H__

#include <RxObject.h>
#include <Gi/GiExport.h>
#include <UInt32Array.h>

#include "TD_PackPush.h"

class OdGiDrawable;

/** \details
    This class provides transient drawables manager functionality.

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiTransientManager : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiTransientManager);

    /** \details
      Represents a drawing mode of a transient. Drawing modes can be used to specify the draw order. 
      For 2D graphics system the topmost level is kOdGiContrast while the bottom-most level is kAcGiMain. 
      For 3D graphics system kOdGiDirectShortTerm, kOdGiHighlight and kOdGiContrast members also indicate the draw order, the kOdGiDirectShortTerm mode is the bottom-most level.
    */
    enum OdGiTransientDrawingMode
    {
      /** Main drawing mode (for internal use). For a 3D graphics system, this mode uses the main Z-Buffer. */
      kOdGiMain = 0,
      /** Sprite drawing mode (for internal use). For a 3D graphics system, this mode uses a separate Z-Buffer for sprites. */
      kOdGiSprite,
      /** ShortTerm drawing mode. For a 3D graphics system, renders directly to the device. */
      kOdGiDirectShortTerm,
      /** Highlight drawing mode. For 2D and 3D graphics systems, renders transient directly to the device highlight effects. */
      kOdGiHighlight,
      /** Renders transient above all other render modes except the contrast mode. */
      kOdGiDirectTopmost,
      /** For 2D and 3D graphics systems renders transient directly to the device in contrast style. */
      kOdGiContrast,
      /** Count of drawing modes. */
      kOdGiDrawingModeCount
    };

    /** \details
        Adds new drawable into transient drawables manager.
        \param pDrawable [in]  Drawable pointer.
        \param mode [in]  Transient drawable mode.
        \param subMode [in]  Draw order index.
        \param viewportIds [in]  List of viewport indices.
        \returns
        true if the drawable is successfully added or false in one of the following cases:
        * The passed pointer does not point to a drawable;
        * The passed mode is greater or equals to the kOdGiDrawingModeCount;
        * The specified draw order index exceeds the maximum value of the signed integer;
    */
    virtual bool addTransient(OdGiDrawable *pDrawable,
                              OdGiTransientDrawingMode mode,
                              OdInt32 subMode,
                              const OdUInt32Array &viewportIds) = 0;

    /** \details
        Erases drawable from transient drawables manager.
        \param pDrawable [in]  Drawable pointer.
        \param viewportIds [in]  List of viewport indices.
        \returns
        true if the drawable is erased or false in one of the following cases:
        * The passed pointer does not point to a drawable;
        * The drawable could not be erased by a registered path;
    */
    virtual bool eraseTransient(OdGiDrawable *pDrawable,
                                const OdUInt32Array &viewportIds) = 0;

    /** \details
        Erases all drawables from specified drawable mode and order.
        \param mode [in]  Transient drawable mode.
        \param subMode [in]  Draw order index.
        \param viewportIds [in]  List of viewport indices.
        \returns
        true if the drawables are erased or false in one of the following cases:
        * The passed mode is greater or equals to the kOdGiDrawingModeCount;
        * The specified draw order index exceeds the maximum value of the signed integer;
    */
    virtual bool eraseTransients(OdGiTransientDrawingMode mode,
                                 OdInt32 subMode,
                                 const OdUInt32Array &viewportIds) = 0;

    /** \details
        Updates drawable in the transient drawables manager.
        \param pDrawable [in]  Drawable pointer.
        \param viewportIds [in]  List of viewport indices.
    */
    virtual void updateTransient(OdGiDrawable *pDrawable,
                                 const OdUInt32Array &viewportIds) const = 0;

    /** \details
        Appends child drawable to parent drawable in the transient drawables manager.
        \param pDrawable [in]  Drawable pointer.
        \param pParent [in]  Parent drawable pointer.
        \returns
        true in current implementation, after the transient is updated.
    */
    virtual bool addChildTransient(OdGiDrawable *pDrawable, OdGiDrawable *pParent) = 0;

    /** \details
        Erases child drawable from parent drawable in the transient drawables manager.
        \param pDrawable [in]  Drawable pointer.
        \param pParent [in]  Parent drawable pointer.
        \returns
        true in current implementation, after the transient is updated.
    */
    virtual bool eraseChildTransient(OdGiDrawable *pDrawable, OdGiDrawable *pParent) = 0;

    /** \details
        Updates child drawable of parent drawable in the transient drawables manager.
        \param pDrawable [in]  Drawable pointer.
        \param pParent [in]  Parent drawable pointer.
    */
    virtual void updateChildTransient(OdGiDrawable *pDrawable, OdGiDrawable *pParent) const = 0;

    /** \details
        Finds free order for specified drawable mode.
        \param mode [in]  Transient drawable mode.
        \param subMode [out]  Draw order index.
        \param viewportIds [in]  List of viewport indices.
        \returns
        Returns 0 if specified input draw order isn't free and transient manager couldn't find free draw order.
        Returns 1 if specified input draw order is free.
        Returns 2 if specified input draw order isn't free but transient manager found free draw order.
    */
    virtual OdInt32 getFreeSubDrawingMode(OdGiTransientDrawingMode mode,
                                          OdInt32 &subMode,
                                          const OdUInt32Array &viewportIds) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiTransientManager object pointers.
*/
typedef OdSmartPtr<OdGiTransientManager> OdGiTransientManagerPtr;

/** \details
    Extracts transient manager using transient manager protocol extension from specified object.
    \param pObject [in]  Pointer the object to be cast to the protocol extension.
*/
ODGI_EXPORT OdGiTransientManager *odgiGetTransientManager(const OdRxObject *pObject);
/** \details
    Sets transient manager using transient manager protocol extension for specified object.
    \param pManager [in]  Pointer the transient manager.
    \param pObject [in]  Pointer the object to be cast to the protocol extension.
*/
ODGI_EXPORT void odgiSetTransientManager(OdGiTransientManager *pManager, OdRxObject *pObject);

/** \details
    This class provides transient drawables manager protocol extension.

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiTransientManagerPE : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiTransientManagerPE);

    /** \details
        Returns transient manager for the specified object.
        \param pThis [in]  Pointer to the object for which this procol extension was specified.
    */
    virtual OdGiTransientManager *transientManager(const OdRxObject *pThis) const = 0;
    /** \details
        Sets transient manager to the specified object.
        \param pThis [in]  Pointer to the object for which this protocol extension was specified.
        \param pManager [in]  Pointer to the transient drawables manager.
    */
    virtual void setTransientManager(OdRxObject *pThis, OdGiTransientManager *pManager) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiTransientManagerPE object pointers.
*/
typedef OdSmartPtr<OdGiTransientManagerPE> OdGiTransientManagerPEPtr;

#include "TD_PackPop.h"

#endif  // __ODGITRANSIENTMANAGER_H__
