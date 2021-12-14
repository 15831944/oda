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
// GLES2 metafile inverse converter

#ifndef ODTRVISMETAFILEPLAYBACK
#define ODTRVISMETAFILEPLAYBACK

#include "TD_PackPush.h"

#include "TrVisDefs.h"
#include "TrVisMetafileStream.h"
#include "Gi/GiConveyorGeometry.h"
#include "MetafileTransformStack.h"
#define STL_USING_SET
#include "OdaSTL.h"

class OdTrVisTtfFontsCacheClient;

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdTrVisMetafilePlaybackCallback
{
  public:
    virtual void metafilePlayMetafile(OdTrVisDisplayId /*metafileId*/) { }
    virtual void metafilePushTransform(const OdGeMatrix3d & /*mTransform*/) { }
    virtual void metafilePopTransform() { }
    virtual const OdTrVisTtfFontsCacheClient *metafileTtfFontsCache() { return NULL; }

    virtual void metafileProcessUserEntry(OdUInt32 /*dataLength*/, const OdUInt8 * /*pData*/) { }
    virtual void metafilePlayExtensionObject(OdTrVisExtensionId /*extId*/) { }

    virtual bool metafileSelectMarker(OdTrVisGsMarker /*gsMarker*/, const OdBaseMetafileContainer * /*pContainer*/) { return true; }

    virtual bool metafilePointProc(const OdGePoint3d * /*pVertex*/) { return true; }
    virtual bool metafilePolyPointProc(const OdGePoint3d * /*pVertex*/, const OdGsMarker * /*pSubEntMarkers*/) { return true; }
    virtual bool metafileLineProc(const OdGePoint3d * /*pVertex*/) { return true; }
    virtual bool metafileTriangleProc(const OdGePoint3d * /*pVertex*/) { return true; }
    virtual bool metafileGeomVisibility(OdTrVisGeomType /*geomType*/) { return true; }
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdTrVisMetafilePlayback
{
  public:
    enum PlayType
    {
      kPlayTypeUndefined = 0,
      kPlayForDisplaying,
      kPlayForSelection
    };
    enum PlayMode
    {
      kPlayModeUndefined = 0,
      kPlayMode2d,
      kPlayMode3d
    };
  protected:
    enum ConfFlags
    {
      kIgnoreSelGeom    = (1 << 0), // Ignore selection-only geometry (can't be set with kIgnoreNonSelGeom)
      kIgnoreNonSelGeom = (1 << 1), // Ignore geometry which can't be selected (can't be set with kIgnoreSelGeom)
      kClientStream     = (1 << 2), // Client-side metafile stream
      kIgnore2d         = (1 << 3), // Ignore geometry marked as 2d
      kIgnore3d         = (1 << 4)  // Ignore geometry marked as 3d
    };
  protected:
    // Player->Owner communication
    OdTrVisMetafilePlaybackCallback *m_pCallback;
    // Interprocess context for nested play() calls
    struct InprocRegisters
    {
      // Current selection markers
      const OdTrVisDefProcMark *pMarks[OdTrVisMetaRecMarkerType_NTypes];
      // Number of selection markers
      OdUInt32 nMarks[OdTrVisMetaRecMarkerType_NTypes];
      // Selection markers activity
      OdUInt8 bMarksActivity;
      // Suppress flags
      enum {
        kSuppressByGeom  = (1 << 0),
        kSuppressSelGeom = (1 << 1),
        kSuppressByMf    = (1 << 2)
      };
      OdUInt8 bSuppressFlags;

      // Default initializer
      InprocRegisters() : bMarksActivity(0), bSuppressFlags(0) { }
      OdTrVisMetafileId metafileMarker() const { return (OdTrVisMetafileId)pMarks[OdTrVisMetaRecMarkerType_Metafile]->m_nMark; }
      OdTrVisGsMarker selectionMarker() const { return (OdTrVisGsMarker)pMarks[OdTrVisMetaRecMarkerType_Selection]->m_nMark; }
    } m_inprocContext;
    friend struct OdTrVisMetafilePlaybackMarkersIterator;
    // Configuration flags
    OdUInt32 m_configFlags;
    // Inclusion metafile filers
    typedef std::set<OdTrVisMetafileId> FilterMfsMap;
    FilterMfsMap m_filterMfs;
    // Marker till which the Drawable PE will perform selection itself
    OdUInt64 m_nMarkerBorder;
  public:
    OdTrVisMetafilePlayback(OdTrVisMetafilePlaybackCallback *pCallback = NULL, PlayType playType = kPlayForSelection,
                            bool bClientStream = false, PlayMode playMode = kPlayModeUndefined)
      : m_pCallback(pCallback)
      , m_configFlags((bClientStream) ? kClientStream : 0)
      , m_nMarkerBorder(0)
    { setPlayType(playType); setPlayMode(playMode); }
    virtual ~OdTrVisMetafilePlayback()
    {
    }

    inline void setCallback(OdTrVisMetafilePlaybackCallback *pCallback)
    {
      m_pCallback = pCallback;
    }
    inline OdTrVisMetafilePlaybackCallback *callback() const
    {
      return m_pCallback;
    }

    inline void setPlayType(PlayType playType)
    { SETBIT_0(m_configFlags, kIgnoreSelGeom | kIgnoreNonSelGeom);
      switch (playType)
      { case kPlayForDisplaying: SETBIT_1(m_configFlags, kIgnoreSelGeom); break;
        case kPlayForSelection: SETBIT_1(m_configFlags, kIgnoreNonSelGeom); break;
        default: break;
      }
    }
    inline PlayType playType() const
    {
      return (PlayType)(m_configFlags & (kIgnoreSelGeom | kIgnoreNonSelGeom));
    }

    inline void setPlayMode(PlayMode playMode)
    { SETBIT_0(m_configFlags, kIgnore3d | kIgnore2d);
      switch (playMode)
      { case kPlayMode2d: SETBIT_1(m_configFlags, kIgnore3d); break;
        case kPlayMode3d: SETBIT_1(m_configFlags, kIgnore2d); break;
        default: break;
      };
    }
    inline PlayMode playMode() const
    {
      if (GETBIT(m_configFlags, kIgnore3d)) return kPlayMode2d;
      if (GETBIT(m_configFlags, kIgnore2d)) return kPlayMode3d;
      return kPlayModeUndefined;
    }

    inline void setFilterMetafile(OdTrVisMetafileId mfId) { m_filterMfs.clear(); m_filterMfs.insert(mfId); }
    inline void addFilterMetafile(OdTrVisMetafileId mfId) { m_filterMfs.insert(mfId); }
    inline void removeFilterMetafile(OdTrVisMetafileId mfId) { m_filterMfs.erase(mfId); }
    /////////////////////////////////////////////////////////////////////////////////////
    inline void setFilterMetafiles(const OdTrVisMetafileId *pMfIds, OdUInt32 nMfs)
    { m_filterMfs.clear(); for (OdUInt32 nMf = 0; nMf < nMfs; nMf++) m_filterMfs.insert(pMfIds[nMf]); }
    inline void resetFilterMetafiles() { m_filterMfs.clear(); }
    /////////////////////////////////////////////////////////////////////////////////////
    inline bool hasFilterMetafiles() const { return !m_filterMfs.empty(); }
    inline bool isMetafileFiltered(OdTrVisMetafileId mfId) const { return m_filterMfs.find(mfId) != m_filterMfs.end(); }

    bool play(const OdBaseMetafileContainer *pContainer, bool bCheckMarks = true, bool bHighlighted = false);
  protected:
    void annihilateInprocMarkers(InprocRegisters &inprocCtx, OdInt32 first, OdInt32 count);

    bool canDrawablePerformSelectionItself(OdUInt64* nMarkerBorder = NULL);

    OdGsMarker getCurrentSelectionMarker();
};

/** \details
  .

  Library: Source code provided.

  <group ExRender_Classes>
*/
class OdTrVisMetafileConveyorPlayback : public OdTrVisMetafilePlayback, OdTrVisMetafilePlaybackCallback
{
  protected:
    // Conveyor communication interfaces
    OdGiConveyorGeometry *m_pGeom;
    OdGiConveyorContext *m_pCtx;
    // Actual readdress callback
    OdTrVisMetafilePlaybackCallback *m_pActualCallback;
    // Callback redirection
    virtual void metafilePlayMetafile(OdTrVisDisplayId metafileId);
    virtual void metafilePushTransform(const OdGeMatrix3d &mtx);
    virtual void metafilePopTransform();
    virtual const OdTrVisTtfFontsCacheClient *metafileTtfFontsCache();
    virtual void metafileProcessUserEntry(OdUInt32 dataLength, const OdUInt8 *pData);
    virtual void metafilePlayExtensionObject(OdTrVisExtensionId extId);
    virtual bool metafileSelectMarker(OdTrVisGsMarker gsMarker, const OdBaseMetafileContainer *pContainer);
    // Internal callback implementations
    virtual bool metafilePointProc(const OdGePoint3d *pVertex);
    virtual bool metafilePolyPointProc(const OdGePoint3d * pVertex, const OdGsMarker * pSubEntMarkers);
    virtual bool metafileLineProc(const OdGePoint3d *pVertex);
    virtual bool metafileTriangleProc(const OdGePoint3d *pVertex);
    virtual bool metafileGeomVisibility(OdTrVisGeomType geomType);
  public:
    OdTrVisMetafileConveyorPlayback(OdTrVisMetafilePlaybackCallback *pCallback = NULL, PlayType playType = kPlayForSelection,
                                    bool bClientStream = false, PlayMode playMode = kPlayModeUndefined)
      : OdTrVisMetafilePlayback(NULL, playType, bClientStream, playMode)
      , m_pActualCallback(pCallback)
    {
      OdTrVisMetafilePlayback::setCallback(this);
    }
    virtual ~OdTrVisMetafileConveyorPlayback()
    {
    }

    inline void setCallback(OdTrVisMetafilePlaybackCallback *pCallback)
    {
      m_pActualCallback = pCallback;
    }
    inline OdTrVisMetafilePlaybackCallback *callback() const
    {
      return m_pActualCallback;
    }
    inline OdGiConveyorContext* context() const
    {
      return m_pCtx;
    }

    void play(const OdBaseMetafileContainer *pContainer, OdGiConveyorGeometry *pGeom, OdGiConveyorContext *pCtx,
              bool bCheckMarks = true, bool bHighlighted = false);
};

#include "TD_PackPop.h"

#endif // ODTRVISMETAFILEPLAYBACK
