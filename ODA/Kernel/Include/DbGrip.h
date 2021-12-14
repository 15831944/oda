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




#ifndef _ODDBGRIP_INCLUDED_
#define _ODDBGRIP_INCLUDED_ /*!DOM*/

#include "TD_PackPush.h"
#include "Ge/GePoint3d.h"
#include "SharedPtr.h"
#include "DbStubPtrArray.h"
#include "GsMarkerArray.h"

class OdDbGripData;
template <class T> class OdSharedPtr;

/** \details
  This template class is a specialization of the OdSharedPtr class template for
  OdDbGripData shared pointers.
*/
typedef OdSharedPtr<OdDbGripData> OdDbGripDataPtr;

/** \details
  This template class is a specialization of the OdArray class template for
  OdDbGripData pointers.
*/
typedef OdArray<OdDbGripDataPtr> OdDbGripDataPtrArray;

/** \details
  This template class is a specialization of the OdArray class template for
  void pointers and corresponding memory allocators.
*/
typedef OdArray<void*, OdMemoryAllocator<void*> > OdDbVoidPtrArray;

class OdDbDimData;

/** \details
  This template class is a specialization of the OdArray class template for
  OdDbDimData pointers and corresponding memory allocators.
*/
typedef OdArray<OdDbDimData*, OdMemoryAllocator<OdDbDimData*> > OdDbDimDataPtrArray;

class OdGiWorldDraw;
class OdGiViewportDraw;
class OdGiViewport;
class OdDbGripData;
class OdDbCustomOsnapMode;

/** \details
  This type defines the right-click menu.
*/
typedef void* ODHMENU;

/** \details
  This template class is a specialization of the OdArray class template for
  OdDbGripData pointers.
*/
typedef OdArray<OdDbGripData> OdDbGripDataArray;

/** \details
  This template class is a specialization of the OdSmartPtr class template for
  OdDbCustomOsnapMode smart pointers.
*/
typedef OdSmartPtr<OdDbCustomOsnapMode> OdDbCustomOsnapModePtr;

/** \details
  This namespace contains functions for working with OdDbEntity.

  <group TD_Namespaces>
*/
namespace OdDb
{
  /** \details
    This enumeration specifies drag states.
  */
  enum DragStat
  {
    /** Drag start */
    kDragStart = 0,
    /** Drag end */
    kDragEnd   = 1,
    /** Drag abort */
    kDragAbort = 2
  };

  /** \details
    This enumeration specifies grip states.
  */
  enum GripStat
  {
    /** Grips done */
    kGripsDone          = 0,
    /** Grips to be deleted */
    kGripsToBeDeleted   = 1,
    /** Dim data to be deleted */
    kDimDataToBeDeleted = 2
  };
}

/** \details
  This namespace contains grip operations definitions.

  <group TD_Namespaces>
*/
namespace OdDbGripOperations
{
  /** \details
    This enumeration specifies grip draw types.
  */
  enum DrawType
  {
    /** Warm grip */
    kWarmGrip = 0,
    /** Hover grip */
    kHoverGrip,
    /** Hot grip */
    kHotGrip,
    /** Drag image grip */
    kDragImageGrip
  };

  /** \details
    This enumeration specifies grip status types.
  */
  enum GripStatus
  {
    /** Grip start */
    kGripStart = 0,
    /** Grip end */
    kGripEnd,
    /** Grip abort */
    kGripAbort,
    /** Stretch */
    kStretch,
    /** Move */
    kMove,
    /** Rotate */
    kRotate,
    /** Scale */
    kScale,
    /** Mirror */
    kMirror,
    /** Dim focus changed */
    kDimFocusChanged,
    /** Pop-up menu */
    kPopUpMenu
  };

  /** \details
    This enumeration specifies grip flags.
  */
  enum GripFlags
  {
    /** Skip when shared */
    kSkipWhenShared           = 0x001,
    /** Disable rubber band line */
    kDisableRubberBandLine    = 0x002,
    /** Disable mode keywords */
    kDisableModeKeywords      = 0x004,
    /** Draw at drag image grip point */
    kDrawAtDragImageGripPoint = 0x008,
    /** Trigger grip */
    kTriggerGrip              = 0x010,
    /** Turn on forced pick */
    kTurnOnForcedPick         = 0x020,
    /** Maps grip hot to right-click */
    kMapGripHotToRtClk        = 0x040,
    /** Gizmos enabled */
    kGizmosEnabled            = 0x080,
    /** Grip is per viewport */
    kGripIsPerViewport        = 0x100
  };

  /** \details
    This enumeration specifies grip context flags.
  */
  enum GripContextFlags
  {
    /** Shared grip */
    kSharedGrip    = 0x01,
    /** Multi hot grip */
    kMultiHotGrip  = 0x02
  };

  /** \details
    This enumeration specifies get grip points flags.
  */
  enum GetGripPointsFlags
  {
    /** Grip points only */
    kGripPointsOnly    = 0x01,
    /** Cyclable grips only */
    kCyclableGripsOnly = 0x02,
    /** Dynamic dim mode */
    kDynamicDimMode    = 0x04
  };

  /** \details
    This enumeration specifies move grip points flags.
  */
  enum MoveGripPointsFlags
  {
    /** Osnapped */
    kOsnapped  = 0x01,
    /** Polar */
    kPolar     = 0x02,
    /** Otrack */
    kOtrack    = 0x04,
    /** Zdir */
    kZdir      = 0x08,
    /** Keyboard */
    kKeyboard  = 0x10
  };
}

/** \details
  This namespace contains functions for working with OdDbEntity.

  <group TD_Namespaces>
*/
namespace OdDb
{
  /** \details
    This enumeration specifies osnap (object snap) mask types.
  */
  enum OsnapMask
  {
    /** Endpoint */
    kOsMaskEnd          = 0x000001,
    /** Midpoint */
    kOsMaskMid          = 0x000002,
    /** Center */
    kOsMaskCen          = 0x000004,
    /** Node */
    kOsMaskNode         = 0x000008,
    /** Quadrant */
    kOsMaskQuad         = 0x000010,
    /** Intersection */
    kOsMaskInt          = 0x000020,
    /** Insertion */
    kOsMaskIns          = 0x000040,
    /** Perpendicular */
    kOsMaskPerp         = 0x000080,
    /** Tangent */
    kOsMaskTan          = 0x000100,
    /** Nearest */
    kOsMaskNear         = 0x000200,
    /** Quck */
    kOsMaskQuick        = 0x000400,
    /** Apparent intersection */
    kOsMaskApint        = 0x000800,
    /** Immediate */
    kOsMaskImmediate    = 0x010000,
    /** Allow tangent */
    kOsMaskAllowTan     = 0x020000,
    /** Disable perpendicular */
    kOsMaskDisablePerp  = 0x040000,
    /** Relative certesian */
    kOsMaskRelCartesian = 0x080000,
    /** Relative polar */
    kOsMaskRelPolar     = 0x100000,
    /** None override */
    kOsMaskNoneOverride = 0x200000
  };
}

/** \details
  This type represents functions that are called to draw non-viewport-specific
  grip's graphics.
*/
typedef bool (*GripWorldDrawPtr)(
  OdDbGripData* pThis,
  OdGiWorldDraw* pWd,
  OdDbStub* entId,
  OdDbGripOperations::DrawType type,
  OdGePoint3d* imageGripPoint,
  double dGripSize );

/** \details
  This type represents functions that are called to draw viewport-specific
  grip's graphics.
*/
typedef void (*GripViewportDrawPtr)(
  OdDbGripData* pThis,
  OdGiViewportDraw* pWd,
  OdDbStub* entId,
  OdDbGripOperations::DrawType type,
  OdGePoint3d* imageGripPoint,
  int gripSize );

/** \details
  This type represents functions that are called when a grip becomes hot or is
  hovered over.
*/
typedef OdResult (*GripOperationPtr)(
  OdDbGripData* pThis,
  OdDbStub* entId,
  int iContextFlags );

/** \details
  This type represents functions that notify about the edit status of a grip.
*/
typedef void (*GripOpStatusPtr)(
  OdDbGripData* pThis,
  OdDbStub* entId,
  OdDbGripOperations::GripStatus status );

/** \details
  This type represents functions that get the tooltip string of a grip.
*/
typedef const OdChar* (*GripToolTipPtr)( OdDbGripData* pThis );

/** \details
  This type represents the context menu item index.
*/
typedef void (*ContextMenuItemIndexPtr)( unsigned itemIndex );

/** \details
  This type represents functions that get the dynamic dimensions of a grip
  when it becomes hot or is hovered over.
*/
typedef void (*GripDimensionPtr)(
  OdDbGripData* pThis,
  OdDbStub* entId,
  double dimScale,
  OdDbDimDataPtrArray& dimData );

/** \details
  This type represents functions that are called when a grip is right-clicked.
*/
typedef OdResult (*GripRtClkHandler)(
  OdDbGripDataArray& hotGrips,
  const OdDbStubPtrArray& ents, 
  OdString& menuName, ODHMENU& menu,
  ContextMenuItemIndexPtr& cb );

/** \details
  This type represents functions that are called when on the certain step of the
  grip editing operation the input point is received. Used for custom entities.
*/
typedef OdResult (*GripInputPointPtr)(
  OdDbGripData* pThis, OdDbStub* entId, bool& changedPoint,
  OdGePoint3d& newPoint, const OdGiViewport& viewContext,
  OdDbStub* viewportId, bool pointComputed, int history,
  const OdGePoint3d& lastPoint, const OdGePoint3d& rawPoint,
  const OdGePoint3d& grippedPoint, const OdGePoint3d& cartesianSnappedPoint,
  const OdGePoint3d& osnappedPoint, OdDb::OsnapMask osnapMask,
  const OdArray<OdDbCustomOsnapModePtr>& customOsnapModes,
  OdDb::OsnapMask osnapOverrides,
  const OdArray<OdDbCustomOsnapModePtr>& customOsnapOverrides,
  const OdDbStubPtrArray& pickedEntities,
  const OdArray<OdDbStubPtrArray>& nestedPickedEntities,
  const OdGsMarkerArray& gsSelectionMark,
  const OdDbStubPtrArray& keyPointEntities,
  const OdArray<OdDbStubPtrArray>& nestedKeyPointEntities,
  const OdGsMarkerArray& keyPointGsSelectionMark,
  const OdArray<OdSharedPtr<OdGeCurve3d> >& alignmentPaths, const OdGePoint3d& computedPoint); 

/** \details
  This class passes parameters for the getGripPoints and moveGripPoints
  functions of OdDbEntity.

  \sa
  TD_Db

  <group OdDb_Classes>
*/
class OdDbGripData
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Default constructor. Creates a new object of the OdDbGripData class.
  */
  OdDbGripData();

  /** \details
    Copy constructor. Creates a clone of an existing object of the OdDbGripData
    class.

    \param OdGripData [in] Object of the OdDbGripData class to be cloned.
  */
  OdDbGripData(const OdDbGripData&);

  /** \details
    Constructor. Creates a new object of the OdDbGripData class and sets its
    data.

    \param pt             [in] Grip point.
    \param AppData        [in] Application-specific data.
    \param hotGrip        [in] Function to call when the grip becomes hot.
    \param hoverGrip      [in] Function to call when the grip is hovered over.
    \param RtClk          [in] Function to call when the grip is right-clicked.
    \param wd             [in] Function to draw grip's graphics.
    \param vd             [in] Function to draw viewport-specific grip's graphics.
    \param stat           [in] Function that notifies about the edit status of
                               the grip.
    \param tt             [in] Function that gets the tooltip string of the grip.
    \param hoverDim       [in] Function that gets the dynamic dimensions of the
                               grip when it is hovered over.
    \param hotGripDim     [in] Function that gets the dynamic dimensions of the
                               grip when it becomes hot.
    \param bitFlags       [in] Flags used to modify the grip's behaviour.
                               Specified by the GripFlags enumeration.
    \param altBasePt      [in] Base point of the grip operation.
    \param inputPointFunc [in] Function to call when on the certain step of the
                               grip editing operation the input point is
                               received. Used for custom entities.
  */
  OdDbGripData(const OdGePoint3d& pt, void* AppData,
    GripOperationPtr hotGrip, GripOperationPtr hoverGrip,
    GripRtClkHandler RtClk, GripWorldDrawPtr wd,
    GripViewportDrawPtr vd, GripOpStatusPtr stat,
    GripToolTipPtr tt, GripDimensionPtr hoverDim,
    GripDimensionPtr hotGripDim,
    unsigned bitFlags, OdGePoint3d* altBasePt,
    GripInputPointPtr inputPointFunc);

  /** \details
    Constructor. Creates a new object of the OdDbGripData class and sets its
    data including the OdRxClass object associated with this class.

    \param pt             [in] Grip point.
    \param AppData        [in] Application-specific data.
    \param pAppDataClass  [in] OdRxClass object associated with this
                               OdDbGripData object.
    \param hotGrip        [in] Function to call when the grip becomes hot.
    \param hoverGrip      [in] Function to call when the grip is hovered over.
    \param RtClk          [in] Function to call when the grip is right-clicked.
    \param wd             [in] Function to draw grip's graphics.
    \param vd             [in] Function to draw viewport-specific grip's graphics.
    \param stat           [in] Function that notifies about the edit status of
                               the grip.
    \param tt             [in] Function that gets the tooltip string of the grip.
    \param hoverDim       [in] Function that gets the dynamic dimensions of the
                               grip when it is hovered over.
    \param hotGripDim     [in] Function that gets the dynamic dimensions of the
                               grip when it becomes hot.
    \param bitFlags       [in] Flags used to modify the grip's behaviour.
                               Specified by the GripFlags enumeration.
    \param altBasePt      [in] Base point of the grip operation.
    \param inputPointFunc [in] Function to call when on the certain step of the
                               grip editing operation the input point is
                               received. Used for custom entities.
  */
  OdDbGripData(const OdGePoint3d& pt, void* AppData,
    OdRxClass* pAppDataClass,
    GripOperationPtr hotGrip, GripOperationPtr hoverGrip,
    GripRtClkHandler RtClk, GripWorldDrawPtr wd,
    GripViewportDrawPtr vd, GripOpStatusPtr stat,
    GripToolTipPtr tt, GripDimensionPtr hoverDim,
    GripDimensionPtr hotGripDim,
    unsigned bitFlags, OdGePoint3d* altBasePt,
    GripInputPointPtr inputPointFunc);

  /** \details
    Sets all data of this OdDbGripData object.

    \param pt             [in] Grip point.
    \param AppData        [in] Application-specific data.
    \param hotGrip        [in] Function to call when the grip becomes hot.
    \param hoverGrip      [in] Function to call when the grip is hovered over.
    \param RtClk          [in] Function to call when the grip is right-clicked.
    \param wd             [in] Function to draw grip's graphics.
    \param vd             [in] Function to draw viewport-specific grip's graphics.
    \param stat           [in] Function that notifies about the edit status of
                               the grip.
    \param tt             [in] Function that gets the tooltip string of the grip.
    \param hoverDim       [in] Function that gets the dynamic dimensions of the
                               grip when it is hovered over.
    \param hotGripDim     [in] Function that gets the dynamic dimensions of the
                               grip when it becomes hot.
    \param bitFlags       [in] Flags used to modify the grip's behaviour.
                               Specified by the GripFlags enumeration.
    \param altBasePt      [in] Base point of the grip operation.
    \param inputPointFunc [in] Function to call when on the certain step of the
                               grip editing operation the input point is
                               received. Used for custom entities.
    \param pAppDataClass  [in] OdRxClass object associated with this
                               OdDbGripData object.
  */
  void setAllData(const OdGePoint3d& pt, void* AppData,
    GripOperationPtr hotGrip, GripOperationPtr hoverGrip,
    GripRtClkHandler RtClk, GripWorldDrawPtr wd,
    GripViewportDrawPtr vd, GripOpStatusPtr stat,
    GripToolTipPtr tt, GripDimensionPtr hoverDim,
    GripDimensionPtr hotGripDim,
    unsigned bitFlags, OdGePoint3d* altBasePt,
    GripInputPointPtr inputPointFunc, 
    OdRxClass* pAppDataClass);

  /** \details
    Comparison operator. Compares this OdDbGripData object with another
    OdDbGripData object.

    \param OdDbGripData [in] Object of the OdDbGripData class to be compared
                             with this object.
    \returns
    True if this object is equal to the compared object; false otherwise.
  */
  OdDbGripData& operator = (const OdDbGripData&);

  /** \details
    Returns the grip point of this OdDbGripData object.
  */
  const OdGePoint3d& gripPoint() const;

  /** \details
    Sets a new grip point for this OdDbGripData object.

    \param pt [in] New grip point.
  */
  void setGripPoint( const OdGePoint3d& pt );

  /** \details
    Returns the application-specific data structure of this OdDbGripData object.
  */
  void* appData() const;

  /** \details
    Sets the application-specific data structure for this OdDbGripData object.

    \param pAppData [in] Pointer to the application-specific data structure.
  */
  void setAppData( void* pAppData );

  /** \details
    Returns the OdRxClass object associated with this OdDbGripData object.
  */
  OdRxClass* appDataOdRxClass() const;

  /** \details
    Associates an OdRxClass object with this OdDbGripData object.

    \param pClass [in] Pointer to the OdRxClass object to associate this
                       OdDbGripData object with.
  */
  void setAppDataOdRxClass(OdRxClass* pClass);

  /** \details
    Returns the pointer to the function that is called when the grip becomes
    hot.
  */
  GripOperationPtr hotGripFunc() const;

  /** \details
    Assigns the function for this OdDbGripData object that is called when the
    grip becomes hot.

    \param pf [in] Pointer to the function that is called when the grip becomes
                   hot.
  */
  void setHotGripFunc( GripOperationPtr pf );

  /** \details
    Returns the pointer to the function that is called when the grip is hovered
    over.
  */
  GripOperationPtr hoverFunc() const;

  /** \details
    Assigns the function for this OdDbGripData object that is called when the
    grip is hovered over.

    \param pf [in] Pointer to the function that is called when the grip is
                   hovered over.
  */
  void setHoverFunc( GripOperationPtr pf );

  /** \details
    Returns the pointer to the function that is called to draw
    non-viwport-specific grip's graphics.
  */
  GripWorldDrawPtr worldDraw() const;

  /** \details
    Assigns the function for this OdDbGripData object that is called to draw
    non-viwport-specific grip's graphics.

    \param pf [in] Pointer to the function that is called to draw
                   non-viwport-specific grip's graphics.
  */
  void setWorldDraw( GripWorldDrawPtr pf );

  /** \details
    Returns the pointer to the function that is called to draw viwport-specific
    grip's graphics.
  */
  GripViewportDrawPtr viewportDraw() const;

  /** \details
    Assigns the function for this OdDbGripData object that is called to draw
    viwport-specific grip's graphics.

    \param pf [in] Pointer to the function that is called to draw
                   viwport-specific grip's graphics.
  */
  void setViewportDraw( GripViewportDrawPtr pf );

  /** \details
    Returns the pointer to the function that notifies about the edit status of
    the grip.
  */
  GripOpStatusPtr gripOpStatFunc() const;

  /** \details
    Assigns the function for this OdDbGripData object that notifies about the
    edit status of the grip.

    \param pf [in] Pointer to the function that notifies about the edit status
                   of the grip.
  */
  void setGripOpStatFunc( GripOpStatusPtr pf );

  /** \details
    Returns the pointer to the function that gets the tooltip string of the
    grip.
  */
  GripToolTipPtr toolTipFunc() const;

  /** \details
    Assigns the function for this OdDbGripData object that gets the tooltip
    string of the grip.

    \param pf [in] Pointer to the function that gets the tooltip string of the
                   grip.
  */
  void setToolTipFunc( GripToolTipPtr pf );

  /** \details
    Returns the base point of the grip operation for this OdDbGripData object.
  */
  OdGePoint3d* alternateBasePoint() const;

  /** \details
    Sets the base point of the grip operation for this OdDbGripData object.

    \param altBasePt [in] Base point of the grip operation.
  */
  void setAlternateBasePoint( OdGePoint3d* altBasePt );

  /** \details
    Returns the flags that modify the behaviour of this OdDbGripData object.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  unsigned bitFlags() const;

  /** \details
    Sets the flags that modify the behaviour of this OdDbGripData object.

    \param flags [in] Flags value.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  void setBitFlags( unsigned flags );

  /** \details
    Indicates whether the kSkipWhenShared flag is set.

    \returns
    True if the kSkipWhenShared flag is set; false otherwise.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  bool skipWhenShared() const;

  /** \details
    Sets or clears the kSkipWhenShared flag.

    \param skip [in] True to set the flag; false to clear the flag.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  void setSkipWhenShared( bool skip );

  /** \details
    Indicates whether the kDisableRubberBandLine flag is set.

    \returns
    True if the kDisableRubberBandLine flag is set; false otherwise.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  bool isRubberBandLineDisabled() const;

  /** \details
    Sets or clears the kDisableRubberBandLine flag.

    \param disable [in] True to set the flag; false to clear the flag.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  void disableRubberBandLine( bool disable );

  /** \details
    Indicates whether the kDisableModeKeywords flag is set.

    \returns
    True if the kDisableModeKeywords flag is set; false otherwise.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  bool areModeKeywordsDisabled() const;

  /** \details
    Sets or clears the kDisableModeKeywords flag.

    \param disable [in] True to set the flag; false to clear the flag.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  void disableModeKeywords( bool disable );

  /** \details
    Indicates whether the kDrawAtDragImageGripPoint flag is set.

    \returns
    True if the kDrawAtDragImageGripPoint flag is set; false otherwise.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  bool drawAtDragImageGripPoint() const;

  /** \details
    Sets or clears the kDrawAtDragImageGripPoint flag.

    \param atDragPoint [in] True to set the flag; false to clear the flag.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  void setDrawAtDragImageGripPoint( bool atDragPoint );

  /** \details
    Indicates whether the kTriggerGrip flag is set.

    \returns
    True if the kTriggerGrip flag is set; false otherwise.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  bool triggerGrip() const;

  /** \details
    Sets or clears the kTriggerGrip flag.

    \param trigger [in] True to set the flag; false to clear the flag.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  void setTriggerGrip( bool trigger );

  /** \details
    Indicates whether the kTurnOnForcedPick flag is set.

    \returns
    True if the kTurnOnForcedPick flag is set; false otherwise.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  bool forcedPickOn() const;

  /** \details
    Sets or clears the kTurnOnForcedPick flag.

    \param on [in] True to set the flag; false to clear the flag.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  void setForcedPickOn( bool on );

  /** \details
    Indicates whether the kMapGripHotToRtClk flag is set.

    \returns
    True if the kMapGripHotToRtClk flag is set; false otherwise.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  bool mapGripHotToRtClk() const;

  /** \details
    Sets or clears the kMapGripHotToRtClk flag.

    \param on [in] True to set the flag; false to clear the flag.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  void setMapGripHotToRtClk( bool on );

  /** \details
    Indicates whether the kGizmosEnabled flag is set.

    \returns
    True if the kGizmosEnabled flag is set; false otherwise.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  bool gizmosEnabled() const;

  /** \details
    Sets or clears the kGizmosEnabled flag.

    \param on [in] True to set the flag; false to clear the flag.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  void setGizmosEnabled( bool on );

  /** \details
    Indicates whether the kGripIsPerViewport flag is set.

    \returns
    True if the kGripIsPerViewport flag is set; false otherwise.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  bool gripIsPerViewport() const;

  /** \details
    Sets or clears the kGripIsPerViewport flag.

    \param on [in] True to set the flag; false to clear the flag.

    \remarks
    Flags values are specified by the GripFlags enumeration.
  */
  void setGripIsPerViewport(bool on);

  /** \details
    Returns the pointer to the function that gets the dynamic dimensions of the
    grip when it is hovered over.
  */
  GripDimensionPtr hoverDimensionFunc() const;

  /** \details
    Assigns the function for this OdDbGripData object that is called to get the
    dynamic dimensions of the grip when it is hovered over.

    \param pf [in] Pointer to the function that gets the dynamic dimensions of
                   the grip when it is hovered over.
  */
  void setHoverDimensionFunc( GripDimensionPtr pf );

  /** \details
    Returns the pointer to the function that gets the dynamic dimensions of the
    grip when it becomes hot.
  */
  GripDimensionPtr hotGripDimensionFunc() const;

  /** \details
    Assigns the function for this OdDbGripData object that is called to get the
    dynamic dimensions of the grip when it becomes hot.

    \param pf [in] Pointer to the function that gets the dynamic dimensions of
                   the grip when it becomes hot.
  */
  void setHotGripDimensionFunc( GripDimensionPtr pf );

  /** \details
    Returns the pointer to the function that is called when the grip is
    right-clicked.
  */
  GripRtClkHandler rtClk() const;

  /** \details
    Assigns the function for this OdDbGripData object that is called when the
    grip is right-clicked.

    \param pf [in] Pointer to the function that is called when the grip is
                   right-clicked.
  */
  void setRtClk( GripRtClkHandler pf );

  /** \details
    Returns the pointer to the function that is called when on the certain step
    of the grip editing operation the input point is received. Used for custom
    entities.
  */
  GripInputPointPtr inputPointFunc() const;

  /** \details
    Assigns the function for this OdDbGripData object that is called when the
    certain step of the grip editing operation the input point is received. Used
    for custom entities.

    \param pf [in] Pointer to the function that is called when the certain step
                   of the grip editing operation the input point is received.
  */
  void setInputPointFunc( GripInputPointPtr pf );

private:
  void*               m_pAppData;
  OdRxClass*          m_pAppDataClass;
  OdGePoint3d         m_gripPt;
  OdGePoint3d*        m_pAltBasePt;
  GripOperationPtr    m_pHotGripFunc;
  GripOperationPtr    m_pHoverFunc;
  GripWorldDrawPtr    m_pWorldDraw;
  GripViewportDrawPtr m_pViewportDraw;
  GripOpStatusPtr     m_pGripOpStatFunc;
  GripToolTipPtr      m_pToolTipFunc;
  unsigned        m_bitFlags;
  GripInputPointPtr   m_pInputPointFunc;
  GripRtClkHandler    m_pRtClk;
  GripDimensionPtr    m_pHoverDimensionFunc;
  GripDimensionPtr    m_pHotGripDimensionFunc;
};

//
// Inline methods
//

inline OdDbGripData::OdDbGripData()
  : m_pAppData(0)
  , m_pAppDataClass(0)
  , m_pAltBasePt(0)
  , m_pHotGripFunc(0)
  , m_pHoverFunc(0)
  , m_pWorldDraw(0)
  , m_pViewportDraw(0)
  , m_pGripOpStatFunc(0)
  , m_pToolTipFunc(0)
  , m_bitFlags(0)
  , m_pInputPointFunc(0)
  , m_pRtClk(0)
  , m_pHoverDimensionFunc(0)
  , m_pHotGripDimensionFunc(0)
{
}

inline OdDbGripData::OdDbGripData(const OdDbGripData& srcData)
{
  m_gripPt                = srcData.gripPoint();
  m_pAppData              = srcData.appData();
  m_pAppDataClass         = srcData.appDataOdRxClass();
  m_pHotGripFunc          = srcData.hotGripFunc();
  m_pHoverFunc            = srcData.hoverFunc();
  m_pRtClk                = srcData.rtClk();
  m_pWorldDraw            = srcData.worldDraw();
  m_pViewportDraw         = srcData.viewportDraw();
  m_pGripOpStatFunc       = srcData.gripOpStatFunc();
  m_pToolTipFunc          = srcData.toolTipFunc();
  m_pHoverDimensionFunc   = srcData.hoverDimensionFunc();
  m_pHotGripDimensionFunc = srcData.hotGripDimensionFunc();
  m_bitFlags              = srcData.bitFlags();
  m_pAltBasePt            = srcData.alternateBasePoint();
  m_pInputPointFunc       = srcData.inputPointFunc();
}

inline
OdDbGripData::OdDbGripData(const OdGePoint3d& pt, void* AppData,
                           GripOperationPtr hotGrip, GripOperationPtr hoverGrip,
                           GripRtClkHandler RtClk, GripWorldDrawPtr wd,
                           GripViewportDrawPtr vd, GripOpStatusPtr stat,
                           GripToolTipPtr tt, GripDimensionPtr hoverDim,
                           GripDimensionPtr hotGripDim,
                           unsigned bitFlags, OdGePoint3d* altBasePt,
                           GripInputPointPtr inputPointFunc)
  : m_pAppData(AppData)
  , m_pAppDataClass(0)
  , m_gripPt(pt)
  , m_pAltBasePt(altBasePt)
  , m_pHotGripFunc(hotGrip)
  , m_pHoverFunc(hoverGrip)
  , m_pWorldDraw(wd)
  , m_pViewportDraw(vd)
  , m_pGripOpStatFunc(stat)
  , m_pToolTipFunc(tt)
  , m_bitFlags(bitFlags)
  , m_pInputPointFunc(inputPointFunc)
  , m_pRtClk(RtClk)
  , m_pHoverDimensionFunc(hoverDim)
  , m_pHotGripDimensionFunc(hotGripDim)
{
}

inline
OdDbGripData::OdDbGripData(const OdGePoint3d& pt, void* AppData,
                           OdRxClass* pAppDataClass,
                           GripOperationPtr hotGrip, GripOperationPtr hoverGrip,
                           GripRtClkHandler RtClk, GripWorldDrawPtr wd,
                           GripViewportDrawPtr vd, GripOpStatusPtr stat,
                           GripToolTipPtr tt, GripDimensionPtr hoverDim,
                           GripDimensionPtr hotGripDim,
                           unsigned bitFlags, OdGePoint3d* altBasePt,
                           GripInputPointPtr inputPointFunc)
  : m_pAppData(AppData)
  , m_pAppDataClass(pAppDataClass)
  , m_gripPt(pt)
  , m_pAltBasePt(altBasePt)
  , m_pHotGripFunc(hotGrip)
  , m_pHoverFunc(hoverGrip)
  , m_pWorldDraw(wd)
  , m_pViewportDraw(vd)
  , m_pGripOpStatFunc(stat)
  , m_pToolTipFunc(tt)
  , m_bitFlags(bitFlags)
  , m_pInputPointFunc(inputPointFunc)
  , m_pRtClk(RtClk)
  , m_pHoverDimensionFunc(hoverDim)
  , m_pHotGripDimensionFunc(hotGripDim)
{
}

inline OdDbGripData& OdDbGripData::operator = (const OdDbGripData& rightSide)
{
  m_gripPt                = rightSide.gripPoint();
  m_pAppData              = rightSide.appData();
  m_pAppDataClass         = rightSide.appDataOdRxClass();
  m_pHotGripFunc          = rightSide.hotGripFunc();
  m_pHoverFunc            = rightSide.hoverFunc();
  m_pRtClk                = rightSide.rtClk();
  m_pWorldDraw            = rightSide.worldDraw();
  m_pViewportDraw         = rightSide.viewportDraw();
  m_pGripOpStatFunc       = rightSide.gripOpStatFunc();
  m_pToolTipFunc          = rightSide.toolTipFunc();
  m_pHoverDimensionFunc   = rightSide.hoverDimensionFunc();
  m_pHotGripDimensionFunc = rightSide.hotGripDimensionFunc();
  m_bitFlags              = rightSide.bitFlags();
  m_pAltBasePt            = rightSide.alternateBasePoint();
  m_pInputPointFunc       = rightSide.inputPointFunc();

  return *this;
}


inline void
OdDbGripData::setAllData(const OdGePoint3d& pt, void* pAppData,
                         GripOperationPtr hotGrip, GripOperationPtr hoverGrip,
                         GripRtClkHandler RtClk, GripWorldDrawPtr wd,
                         GripViewportDrawPtr vd, GripOpStatusPtr stat,
                         GripToolTipPtr tt, GripDimensionPtr hoverDim,
                         GripDimensionPtr hotGripDim,
                         unsigned bitFlags, OdGePoint3d* altBasePt,
                         GripInputPointPtr inputPointFunc, 
                         OdRxClass* pAppDataClass)
{
  m_gripPt = pt;
  m_pAppData = pAppData;
  m_pAppDataClass = pAppDataClass;
  m_pHotGripFunc = hotGrip;
  m_pHoverFunc = hoverGrip;
  m_pRtClk = RtClk;
  m_pWorldDraw = wd;
  m_pViewportDraw = vd;
  m_pGripOpStatFunc = stat;
  m_pToolTipFunc= tt;
  m_pHoverDimensionFunc = hoverDim;
  m_pHotGripDimensionFunc = hotGripDim;
  m_bitFlags = bitFlags;
  m_pAltBasePt = altBasePt;
  m_pInputPointFunc = inputPointFunc;
}

inline const OdGePoint3d& OdDbGripData::gripPoint() const
{
  return m_gripPt;
}

inline void OdDbGripData::setGripPoint( const OdGePoint3d& pt )
{
  m_gripPt = pt;
}

inline void* OdDbGripData::appData() const
{
  return m_pAppData;
}

inline void OdDbGripData::setAppData( void* pAppData )
{
  m_pAppData = pAppData;
}

inline OdRxClass* OdDbGripData::appDataOdRxClass() const
{
    return m_pAppDataClass;
}

inline void OdDbGripData::setAppDataOdRxClass(OdRxClass* appDataClass)
{
    m_pAppDataClass = appDataClass;
}

inline GripOperationPtr OdDbGripData::hotGripFunc() const
{
  return m_pHotGripFunc;
}

inline void OdDbGripData::setHotGripFunc( GripOperationPtr pf )
{
  m_pHotGripFunc = pf;
}

inline GripOperationPtr OdDbGripData::hoverFunc() const
{
  return m_pHoverFunc;
}

inline void OdDbGripData::setHoverFunc( GripOperationPtr pf )
{
  m_pHoverFunc = pf;
}

inline GripWorldDrawPtr OdDbGripData::worldDraw() const
{
  return m_pWorldDraw;
}

inline void OdDbGripData::setWorldDraw( GripWorldDrawPtr pf )
{
  m_pWorldDraw = pf;
}

inline GripViewportDrawPtr OdDbGripData::viewportDraw() const
{
  return m_pViewportDraw;
}

inline void OdDbGripData::setViewportDraw( GripViewportDrawPtr pf )
{
  m_pViewportDraw = pf;
}

inline GripOpStatusPtr OdDbGripData::gripOpStatFunc() const
{
  return m_pGripOpStatFunc;
}

inline void OdDbGripData::setGripOpStatFunc( GripOpStatusPtr pf )
{
  m_pGripOpStatFunc = pf;
}

inline GripToolTipPtr OdDbGripData::toolTipFunc() const
{
  return m_pToolTipFunc;
}

inline void OdDbGripData::setToolTipFunc( GripToolTipPtr pf )
{
  m_pToolTipFunc = pf;
}

inline OdGePoint3d* OdDbGripData::alternateBasePoint() const
{
  return m_pAltBasePt;
}

inline void OdDbGripData::setAlternateBasePoint( OdGePoint3d* altBasePt )
{
  m_pAltBasePt = altBasePt;
}

inline unsigned OdDbGripData::bitFlags() const
{
  return m_bitFlags;
}

inline void OdDbGripData::setBitFlags( unsigned flags )
{
  m_bitFlags = flags;
}

inline bool OdDbGripData::skipWhenShared() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kSkipWhenShared);
}

inline void OdDbGripData::setSkipWhenShared( bool skip )
{
  if (skip)
    m_bitFlags |= OdDbGripOperations::kSkipWhenShared;
  else
    m_bitFlags &= ~OdDbGripOperations::kSkipWhenShared;
}

inline bool OdDbGripData::isRubberBandLineDisabled() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kDisableRubberBandLine);
}

inline void OdDbGripData::disableRubberBandLine( bool disable )
{
  if (disable)
    m_bitFlags |= OdDbGripOperations::kDisableRubberBandLine;
  else
    m_bitFlags &= ~OdDbGripOperations::kDisableRubberBandLine;
}

inline bool OdDbGripData::areModeKeywordsDisabled() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kDisableModeKeywords);
}

inline void OdDbGripData::disableModeKeywords( bool disable )
{
  if (disable)
    m_bitFlags |= OdDbGripOperations::kDisableModeKeywords;
  else
    m_bitFlags &= ~OdDbGripOperations::kDisableModeKeywords;
}

inline bool OdDbGripData::drawAtDragImageGripPoint() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kDrawAtDragImageGripPoint);
}

inline void OdDbGripData::setDrawAtDragImageGripPoint( bool atDragPoint )
{
  if (atDragPoint)
    m_bitFlags |= OdDbGripOperations::kDrawAtDragImageGripPoint;
  else
    m_bitFlags &= ~OdDbGripOperations::kDrawAtDragImageGripPoint;
}

inline bool OdDbGripData::triggerGrip() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kTriggerGrip);
}

inline void OdDbGripData::setTriggerGrip( bool trigger )
{
  if (trigger)
    m_bitFlags |= OdDbGripOperations::kTriggerGrip | OdDbGripOperations::kSkipWhenShared;
  else
    m_bitFlags &= ~(OdDbGripOperations::kTriggerGrip | OdDbGripOperations::kSkipWhenShared);
}

inline bool OdDbGripData::forcedPickOn() const
{
    return 0 != (m_bitFlags & OdDbGripOperations::kTurnOnForcedPick);
}

inline void OdDbGripData::setForcedPickOn( bool on )
{
  if (on)
    m_bitFlags |= OdDbGripOperations::kTurnOnForcedPick;
  else
    m_bitFlags &= ~OdDbGripOperations::kTurnOnForcedPick;
}

inline bool OdDbGripData::mapGripHotToRtClk() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kMapGripHotToRtClk);
}

inline void OdDbGripData::setMapGripHotToRtClk( bool on )
{
  if (on)
    m_bitFlags |= OdDbGripOperations::kMapGripHotToRtClk;
  else
    m_bitFlags &= ~OdDbGripOperations::kMapGripHotToRtClk;
}

inline bool OdDbGripData::gizmosEnabled() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kGizmosEnabled);
}

inline void OdDbGripData::setGizmosEnabled( bool on )
{
  if (on)
    m_bitFlags |= OdDbGripOperations::kGizmosEnabled;
  else
    m_bitFlags &= ~OdDbGripOperations::kGizmosEnabled;
}

inline bool OdDbGripData::gripIsPerViewport() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kGripIsPerViewport);
}

inline void OdDbGripData::setGripIsPerViewport(bool on)
{
  if (on)
    m_bitFlags |= OdDbGripOperations::kGripIsPerViewport;
  else
    m_bitFlags &= ~OdDbGripOperations::kGripIsPerViewport;
}

inline GripDimensionPtr OdDbGripData::hoverDimensionFunc() const
{
  return m_pHoverDimensionFunc;
}

inline void OdDbGripData::setHoverDimensionFunc(GripDimensionPtr pf)
{
  m_pHoverDimensionFunc = pf;
}

inline GripDimensionPtr OdDbGripData::hotGripDimensionFunc() const
{
  return m_pHotGripDimensionFunc;
}

inline void OdDbGripData::setHotGripDimensionFunc(GripDimensionPtr pf)
{
  m_pHotGripDimensionFunc = pf;
}

inline GripRtClkHandler OdDbGripData::rtClk() const
{
  return m_pRtClk;
}

inline void OdDbGripData::setRtClk(GripRtClkHandler pf)
{
  m_pRtClk = pf;
}

inline GripInputPointPtr OdDbGripData::inputPointFunc() const
{
  return m_pInputPointFunc;
}

inline void OdDbGripData::setInputPointFunc(GripInputPointPtr pf)
{
  m_pInputPointFunc = pf;
}

#include "TD_PackPop.h"

#endif // _ODDBGRIP_INCLUDED_
