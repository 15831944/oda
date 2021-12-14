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

#ifndef __OD_GI_METAFILER__
#define __OD_GI_METAFILER__

#include "Gi/GiConveyorNode.h"
#include "ChunkAllocator.h"
#include "Ge/GeDoubleArray.h"

class OdGiDeviation;

#include "TD_PackPush.h"

/** \details
  Geometry metafile class.

  <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiGeometryMetafile : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdGiGeometryMetafile);

  ODCA_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Geometry metafile record class.
  */
  class Record
  {
  protected:
    //DOM-IGNORE-BEGIN
    Record* m_pTail;
  public:
    ODRX_HEAP_OPERATORS();
    //DOM-IGNORE-END

    /** \details
      Default constructor for objects of the geometry metafile record class.
    */
    Record()
      : m_pTail(0)
    {}

    /** \details
      Destructor for objects of the geometry metafile record class.
    */
    virtual ~Record() {}

    /** \details
      Retrieves the tail record of this record.

      \returns
      Tail record of this record.
    */
    Record* tail()
    {
      return m_pTail;
    }

    /** \details
      Retrieves the tail record of this record.

      \returns
      Tail record of this record.
    */
    const Record* tail() const
    {
      return m_pTail;
    }

    /** \details
      Sets the tail record of this record.

      \param pTail [in] New tail record to set.
    */
    void setTail(Record* pTail)
    {
      m_pTail = pTail;
    }

    /** \details
      Deletes the list of records starting from this record.
    */
    inline void deleteList()
    {
      Record* pCurr = this;
      while(pCurr)
      {
        Record* pTail = pCurr->m_pTail;
        delete pCurr;
        pCurr = pTail;
      }
    }

    /** \details
      Plays this record.

      \param pGeom [in] Conveyor geometry.
      \param pCtx  [in] Conveyor context.
    */
    virtual void play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const = 0;

    /** \details
      Retrieves the size of this record.

      \returns
      Size of this record.
    */
    virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
  };

private:
  //DOM-IGNORE-BEGIN
  Record*               m_pHead;

  void destroyRecords()
  {
    if(m_pHead)
    {
      m_pHead->deleteList();
      m_pHead = NULL;
    }
  }
  //DOM-IGNORE-END
public:

  /** \details
    Default constructor for objects of the OdGiGeometryMetafile class.
  */
  OdGiGeometryMetafile();

  /** \details
    Destructor for objects of the OdGiGeometryMetafile class.
  */
  virtual ~OdGiGeometryMetafile();

  /** \details
    Clears all records of this geometry metafile object.
  */
  void clear();

  /** \details
    Plays this geometry metafile object.

    \param pGeom [in] Conveyor geometry.
    \param pCtx  [in] Conveyor context.
  */
  void play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const;

  /** \details
    Plays this geometry metafile object.

    \param output [in] Conveyor output.
    \param pCtx   [in] Conveyor context.
  */
  void play(OdGiConveyorOutput& output, OdGiConveyorContext* pCtx) const;

  /** \details
    Retrieves the size of this geometry metafile object.

    \returns
    Size of this geometry metafile object.
  */
  virtual OdUInt64 metafileSize() const;

  /** \details
    Sets records for this geometry metafile object.

    \param pRec [in] Head record of the list of records to be set.
  */
  void setRecords(Record* pRec)
  {
    ODA_ASSERT(m_pHead == 0);
    m_pHead = pRec;
  }

  /** \details
    Indicates whether this geometry metafile object is empty.

    \returns
    True if this geometry metafile object is empty; false otherwise.
  */
  bool isEmpty() const { return m_pHead == 0; }

  /** \details
    Retrieves the head record of this geometry metafile object.

    \returns
    Head record of this geometry metafile object.
  */
  Record *firstRecord() const { return m_pHead; }

  /** \details
    Retrieves the last record of this geometry metafile object.

    \returns
    Last record of this geometry metafile object.
  */
  Record *lastRecord() const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdGiGeometryMetafile object pointers.
*/
typedef OdSmartPtr<OdGiGeometryMetafile> OdGiGeometryMetafilePtr;

/** \details
  Metafiler class.

  <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMetafiler : public OdGiConveyorNode
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdGiMetafiler);
  //DOM-IGNORE-END

  /** \details
    Sets a metafile.

    \param pMetafile [in] Pointer to a geometry metafile.
  */
  virtual void setMetafile(OdGiGeometryMetafile* pMetafile) = 0;

  /** \details
    Retrieves a metafile associated with this object.

    \returns
    Pointer to the geometry metafile.
  */
  virtual OdGiGeometryMetafile* metafile() = 0;

  /** \details
    Replaces a metafile, sets the last metafile record to be the current.

    \param pMetafile [in] Pointer to a geometry metafile.
  */
  virtual void exchangeMetafile(OdGiGeometryMetafile* pMetafile) = 0;

  /** \details
    Sets the max deviation for curve tesselation.

    \param deviations [in] Array of deviation values.
  */
  virtual void setDeviation(const OdGeDoubleArray& deviations) = 0;

  /** \details
    Sets a deviation object to obtain max deviation for curve tesselation.

    \param pDeviation [in] Pointer to the deviation object.
  */
  virtual void setDeviation(const OdGiDeviation* pDeviation) = 0;

  /** \details
    Sets the draw context object (to access traits, etc).

    \param pDeviation [in] Pointer to the deviation object.
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawCtx) = 0;

  /** \details
    Coordinates types for geometry.
  */
  enum CoordType
  {
    /** Unknown. */
    kUnknown = 0,
    /** Model space coordinates. */
    kModel,
    /** World space coordinates. */
    kWorld,
    /** Eye space coordinates. */
    kEye,
    /** Device space coordinates. */
    kDevice
  };

  /** \details
    Sets the coordinates type for geometry stored by the metafiler.

    \param ct [in] Coordinates type.
  */
  virtual void setCoordinatesType(CoordType ct) = 0;

  /** \details
    Retrieves the coordinates type used by this metafiler object.

    \returns
    Value from the CoordType enumeration that represents coordinates type for
    geometry.
  */
  virtual CoordType coordinatesType() const = 0;

  /** \details
    Saves traits into the metafile. Optionally saves ByBlock traits.

    \param entTraits     [in] Entity traits to save.
    \param byBlockTraits [in] Pointer to optional byBlock traits to save.

    \returns
    True if the underlying metafile is not NULL, false otherwise.
  */
  virtual bool saveTraits(const OdGiSubEntityTraitsData& entTraits, const OdGiSubEntityTraitsData *byBlockTraits = NULL) = 0;

  /** \details
    Saves ByBlock traits into the metafile.

    \param byBlockTraits [in] Pointer to ByBlock traits to save.

    \returns
    True if the underlying metafile is not NULL, false otherwise.
  */
  virtual bool saveTraits(const OdGiSubEntityTraitsData *byBlockTraits) = 0;

  /** \details
    Saves effective traits only into the metafile.

    \returns
    True if the underlying metafile is not NULL, false otherwise.
  */
  virtual bool saveTraits() = 0;

  /** \details
    Flushes all data to the metafile.

    \param bForceTraits [in] Flag that specifies whether data should be flushed even if geometry buffers are empty.
  */
  virtual void flush(bool bForceTraits = true) = 0;

  /** \details
    Adds a metafile record to the records list.

    \param pRec [in] Pointer to the metafile record to be added.
  */
  virtual void add(OdGiGeometryMetafile::Record* pRec) = 0;

  /** \details
    Metafiler storage options.
  */
  enum EOptions
  {
    /** Keep text as text, not converted into geometry. */
    kTextAsText = 1,
    /** Keep nurbs as nurbs, not simplified. */
    kNurbsAsNurbs = 2,
    /** Keep pline as pline, not simplified. */
    kPlineAsPline = 4,
    /** Disable image conversion. */
    kDisableImageConversion = 8
  };

  /** \details
    Sets the metafiler options.

    \param options [in] Metafiler options.
  */
  virtual void setOptions(OdUInt32 options) {}

  /** \details
    Retrieves options used by this metafiler object.

    \returns
    Options used by this metafiler object.
  */
  virtual OdUInt32 options() const { return 0; }
};

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdGiMetafiler object pointers.
*/
typedef OdSmartPtr<OdGiMetafiler> OdGiMetafilerPtr;

class ODGI_EXPORT OdGiShell
{
public:
  OdGiShell(OdInt32 nbVertex,
    const OdGePoint3d* pVertexList,
    OdInt32 faceListSize,
    const OdInt32* pFaceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData);
  ~OdGiShell();

  OdInt32 numVertices() const { return m_nbVertex; }
  OdInt32 faceListSize() const { return m_faceListSize; }

  const OdGePoint3d* vertices() const { return m_pVertexList; }
  const OdInt32* faceList() const { return m_pFaceList; }

  const OdGiEdgeData* edgeData() const { return m_pEdgeData; }
  const OdGiFaceData* faceData() const { return m_pFaceData; }
  const OdGiVertexData* vertexData() const { return m_pVertexData; }

private:
  OdInt32 m_nbVertex;
  OdInt32 m_faceListSize;

  OdGePoint3d* m_pVertexList;
  OdInt32* m_pFaceList;

  OdGiEdgeData* m_pEdgeData;
  OdGiFaceData* m_pFaceData;
  OdGiVertexData* m_pVertexData;

  OdGiEdgeData m_edgeData;
  OdGiFaceData m_faceData;
  OdGiVertexData m_vertexData;
};

#include "TD_PackPop.h"

#endif //#ifndef __OD_GI_METAFILER__
