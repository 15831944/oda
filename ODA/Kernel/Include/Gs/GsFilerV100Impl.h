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

#ifndef __OdGsFilerV100Impl_H__
#define __OdGsFilerV100Impl_H__

#include "TD_PackPush.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeExtents3d.h"
#include "DbBaseDatabase.h"
#include "GsFiler.h"
#include "OdStack.h"
#include "RxObjectImpl.h"


// Database linker
class OdGsFilerDbLinker : public OdRxObject
{
public:
  /** \details
    Default constructor for the OdGsFilerDbLinker class.
  */
  OdGsFilerDbLinker() { }
  
  /** \details
    Destructor for the OdGsFilerDbLinker class.
  */
  virtual ~OdGsFilerDbLinker() { }
public:
  /** \details
    Computes database hash.
    
    \param pDb        [in]  Pointer to a database.
    \returns
    database hash.
  */
  virtual OdBinaryData getDbHash(const OdDbBaseDatabase *pDb) const;

  // Transform database handle
  /** \details
    Retrieves a database handle by the specified database-resident object.
    
    \param pStub   [in]  Pointer to a database-resident object.
  */
  virtual OdUInt64 stubToDbHandle(OdDbStub *pStub) const = 0;
  
  /** \details
    Retrieves a database-resident object by the specified database handle.
    
    \param pStub   [in]  Pointer to a database-resident object.
  */
  virtual OdDbStub *handleToDbStub(OdUInt64 nHandle) const = 0;

  // Util methods
  /** \details
    Retrieves a pointer to a database associated with this object.
    
    \param pStub   [in]  Pointer to a database-resident object.
  */
  virtual OdDbBaseDatabase *getDatabase() const = 0;
  
  /** \details
    Retrieves a pointer to a database that owns the specified database-resident object.
    
    \param pStub   [in]  Pointer to a database-resident object.
  */
  virtual OdDbBaseDatabase *getDatabase(OdDbStub *pStub) const = 0;
};

/** \details
  A data type that represents a smart pointer to an <link OdGsFilerDbLinker, OdGsFilerDbLinker> object.
*/
typedef OdSmartPtr<OdGsFilerDbLinker> OdGsFilerDbLinkerPtr;



#define STL_USING_MAP
#define STL_USING_SET
#include "OdaSTL.h"

// Implement substitutor
struct GS_TOOLKIT_EXPORT OdGsFiler_SubstitutorImpl : public OdRxObject, public OdGsFiler::Substitutor
{
  template<OdUInt32 nBytes> struct DataTyp
  {
    OdUInt8 m_data[nBytes];
    static int compare(const DataTyp &t1, const DataTyp &t2)
    {
      for (OdUInt32 nb = 0; nb < nBytes; nb++)
      {
        if (t1.m_data[nb] != t2.m_data[nb])
          return int(t1.m_data[nb]) - int(t2.m_data[nb]);
      }
      return 0;
    }
    bool operator ==(const DataTyp &t2) const { return compare(*this, t2) == 0; }
    bool operator !=(const DataTyp &t2) const { return compare(*this, t2) != 0; }
    bool operator <(const DataTyp &t2) const { return compare(*this, t2) < 0; }
    bool operator <=(const DataTyp &t2) const { return compare(*this, t2) <= 0; }
    bool operator >(const DataTyp &t2) const { return compare(*this, t2) > 0; }
    bool operator >=(const DataTyp &t2) const { return compare(*this, t2) >= 0; }

    operator const void*() const { return this; }
  };
  struct VoidsSet
  {
    void *m_pVoid;
    OdGsFiler::SubstitutionActuator *m_pAct;
  };
  struct VoidArry : public VoidsSet
  {
    OdInt32 m_nVoids;
    VoidArry()
      : m_nVoids(0)
    {
      m_pVoid = NULL;
      m_pAct = NULL;
    }
    ~VoidArry() { clear(); }
    VoidsSet *getSet() { return (VoidsSet*)m_pVoid; }
    VoidsSet *at(OdInt32 n) {
      if (m_nVoids == 1)
        return this;
      return getSet() + n;
    }
    bool contains(void *pVoid);
    void add(void *pVoid, OdGsFiler::SubstitutionActuator *pAct);
    void kill(void *pVoid);
    void clear();
  };

  struct AbstractTyp
  {
    void *m_pContainer;
    AbstractTyp() : m_pContainer(NULL) { }
  };
  typedef std::map<OdUInt32, AbstractTyp> RegsMap;
  typedef std::map<OdUInt32, AbstractTyp> SubstsMap;
  RegsMap m_regs;
  SubstsMap m_substs;

  template<OdUInt32 nBytes> struct Procs
  {
    typedef OdGsFiler_SubstitutorImpl::DataTyp<nBytes> ProcTyp;
    typedef std::map<ProcTyp, ProcTyp> RegMap;
    typedef std::map<ProcTyp, VoidArry> SubstMap;
    static void makeSubst(void *pPlace, const void *pSubstitution)
    {
      *(ProcTyp*)(pPlace) = *(const ProcTyp*)(pSubstitution);
    }
    static void runSubst(void *pPlace, const void *pSubstitution, OdGsFiler::SubstitutionActuator *pActuator)
    {
      ODA_ASSERT(pActuator);
      pActuator->applySubstitution(pPlace, pSubstitution, makeSubst);
    }
    static void subst(OdGsFiler_SubstitutorImpl &impl, const void *pValue, void *pPlace, OdGsFiler::SubstitutionActuator *pAct, const void *pSubstitution, bool bClear)
    {
      if (pPlace && pSubstitution)
      {
        runSubst(pPlace, pSubstitution, pAct);
        return;
      }
      if (pValue)
      {
        if (pPlace)
        {
          RegMap *pRegMap = (RegMap*)impl.m_regs[nBytes].m_pContainer;
          OD_TYPENAME RegMap::iterator itReg = pRegMap->find(*(const ProcTyp*)pValue);
          if (itReg != pRegMap->end())
          {
            runSubst(pPlace, itReg->second, pAct);
            if (bClear)
              pRegMap->erase(itReg);
          }
        }
        else if (pSubstitution)
        {
          SubstMap *pSubMap = (SubstMap*)impl.m_substs[nBytes].m_pContainer;
          OD_TYPENAME SubstMap::iterator itSub = pSubMap->find(*(const ProcTyp*)pValue);
          if (itSub != pSubMap->end())
          {
            for (OdInt32 n = 0; n < itSub->second.m_nVoids; n++)
              runSubst(itSub->second.at(n)->m_pVoid, pSubstitution, itSub->second.at(n)->m_pAct);
            if (bClear)
              pSubMap->erase(itSub);
          }
        }
        else
        {
          RegMap *pRegMap = (RegMap*)impl.m_regs[nBytes].m_pContainer;
          SubstMap *pSubMap = (SubstMap*)impl.m_substs[nBytes].m_pContainer;
          if (pRegMap && pSubMap)
          {
            OD_TYPENAME RegMap::iterator itReg = pRegMap->find(*(const ProcTyp*)pValue);
            OD_TYPENAME SubstMap::iterator itSub = pSubMap->find(*(const ProcTyp*)pValue);
            if ((itReg != pRegMap->end()) && (itSub != pSubMap->end()))
            {
              for (OdInt32 n = 0; n < itSub->second.m_nVoids; n++)
                runSubst(itSub->second.at(n)->m_pVoid, itReg->second, itSub->second.at(n)->m_pAct);
            }
            if (bClear)
            {
              if (itReg != pRegMap->end())
                pRegMap->erase(itReg);
              if (itSub != pSubMap->end())
                pSubMap->erase(itSub);
            }
          }
          else if (bClear)
          {
            if (pRegMap)
            {
              OD_TYPENAME RegMap::iterator itReg = pRegMap->find(*(const ProcTyp*)pValue);
              if (itReg != pRegMap->end())
                pRegMap->erase(itReg);
            }
            if (pSubMap)
            {
              OD_TYPENAME SubstMap::iterator itSub = pSubMap->find(*(const ProcTyp*)pValue);
              if (itSub != pSubMap->end())
                pSubMap->erase(itSub);
            }
          }
        }
        return;
      }
      RegMap *pRegMap = (RegMap*)impl.m_regs[nBytes].m_pContainer;
      SubstMap *pSubMap = (SubstMap*)impl.m_substs[nBytes].m_pContainer;
      if (pRegMap && pSubMap)
      {
        OD_TYPENAME RegMap::iterator itReg = pRegMap->begin();
        while (itReg != pRegMap->end())
        {
          OD_TYPENAME SubstMap::iterator itSub = pSubMap->find(itReg->first);
          if (itSub != pSubMap->end())
          {
            for (OdInt32 n = 0; n < itSub->second.m_nVoids; n++)
              runSubst(itSub->second.at(n)->m_pVoid, itReg->second, itSub->second.at(n)->m_pAct);
          }
          itReg++;
        }
      }
      if (bClear)
      {
        if (pRegMap) { delete pRegMap; impl.m_regs[nBytes].m_pContainer = NULL; }
        if (pSubMap) { delete pSubMap; impl.m_substs[nBytes].m_pContainer = NULL; }
      }
    }
    static void reg(OdGsFiler_SubstitutorImpl &impl, const void *pValue, const void *pSubstitution, bool bRegister, bool bImmediate)
    {
      if (bRegister)
      {
        OdGsFiler_SubstitutorImpl::AbstractTyp &cont = impl.m_regs[nBytes];
        if (!cont.m_pContainer)
          cont.m_pContainer = new RegMap;
        RegMap &pMap = *(RegMap*)cont.m_pContainer;
        pMap[*(const ProcTyp*)pValue] = *(const ProcTyp*)pSubstitution;
      }
      if (bImmediate)
        subst(impl, pValue, NULL, NULL, pSubstitution, false);
    }
    static void req(OdGsFiler_SubstitutorImpl &impl, void *pPlace, OdGsFiler::SubstitutionActuator *pAct, const void *pValue, bool bRegister, bool bImmediate)
    {
      if (bRegister)
      {
        if (bImmediate)
        {
          RegMap *pRegMap = (RegMap*)impl.m_regs[nBytes].m_pContainer;
          if (pRegMap)
          {
            OD_TYPENAME RegMap::iterator itReg = pRegMap->find(*(const ProcTyp*)pValue);
            if (itReg != pRegMap->end())
            {
              runSubst(pPlace, itReg->second, pAct);
              return;
            }
          }
        }
        OdGsFiler_SubstitutorImpl::AbstractTyp &cont = impl.m_substs[nBytes];
        if (!cont.m_pContainer)
          cont.m_pContainer = new SubstMap;
        SubstMap &pMap = *(SubstMap*)cont.m_pContainer;
        pMap[*(const ProcTyp*)pValue].add(pPlace, pAct);
      }
      else if (bImmediate)
        subst(impl, pValue, pPlace, pAct, NULL, false);
    }
    static void clear(OdGsFiler_SubstitutorImpl &impl, const void *pValue)
    {
      RegMap *pRegMap = (RegMap*)impl.m_regs[nBytes].m_pContainer;
      SubstMap *pSubMap = (SubstMap*)impl.m_substs[nBytes].m_pContainer;
      if (pValue)
      {
        if (pRegMap)
        {
          OD_TYPENAME RegMap::iterator it = pRegMap->find(*(const ProcTyp*)pValue);
          if (it != pRegMap->end())
            pRegMap->erase(it);
        }
        if (pSubMap)
        {
          OD_TYPENAME SubstMap::iterator it = pSubMap->find(*(const ProcTyp*)pValue);
          if (it != pSubMap->end())
            pSubMap->erase(it);
        }
      }
      else
      {
        if (pRegMap) { delete pRegMap; impl.m_regs[nBytes].m_pContainer = NULL; }
        if (pSubMap) { delete pSubMap; impl.m_substs[nBytes].m_pContainer = NULL; }
      }
    }
  };

  static OdGsFiler::SubstitutionActuator g_defActuator;
  static struct OdRxObjectSubstitutionActuator : public OdGsFiler::SubstitutionActuator
  {
    OdRxObjectSubstitutionActuator() { }
    virtual void applySubstitution(void *pPlace, const void *pValue, SetPtrFunc pSetFunc)
    {
      OdGsFiler::SubstitutionActuator::applySubstitution(pPlace, pValue, pSetFunc);
      if (pPlace)
        (*reinterpret_cast<OdRxObjectPtr*>(pPlace))->addRef();
    }
  } g_rxObjectActuator;

  // Register known substitution
  virtual void registerSubstitution(const void *pValue, const void *pSubstitution, OdUInt32 size = sizeof(OdIntPtr), bool bRegister = true, bool bImmediate = false);
  // Register substitution request
  virtual void requestSubstitution(void *pPlace, const void *pValue, OdUInt32 size = sizeof(OdIntPtr), bool bRegister = true, bool bImmediate = true);
  // Register substitution request for smart pointer
  virtual void requestSubstitution(OdBaseObjectPtr *pPlace, const void *pValue, bool bRegister = true, bool bImmediate = true);
  // Register substitution with non-default behavior
  virtual void requestSubstitution(void *pPlace, OdGsFiler::SubstitutionActuator *pActuator, const void *pValue, OdUInt32 size = sizeof(OdIntPtr), bool bRegister = true, bool bImmediate = true);
  // Clear substitutions
  virtual void clearSubstitutions(const void *pValue = NULL, OdUInt32 size = 0);
  // Run substitutions
  virtual void runSubstitutions(const void *pValue = NULL, OdUInt32 size = 0, bool bClear = true);
#undef REG_FUNCCALL
  ~OdGsFiler_SubstitutorImpl() { clearSubstitutions(); }
};


// Implement V1 version filer
class GS_TOOLKIT_EXPORT OdGsFilerV100Impl : public OdGsFiler {
protected:
  enum Flags {
    kOpenedForWrite = (1 << 0),

    kLastFlag = kOpenedForWrite
  };
protected:
  mutable OdStreamBufPtr  m_pStream;
  OdGsFilerDbLinkerPtr    m_pLinker;
  OdUInt64                m_nSectionsToWrite;
  OdUInt64                m_nSectionsToRead;
  OdUInt32                m_nFlags;
  OdUInt32                m_nVersion;
  mutable Section         m_nCurSection;
  mutable OdUInt32        m_nCurSectionSize;
  struct WrSectionData {
    OdGsFiler::Section  m_type;
    OdUInt64            m_begin;
    WrSectionData()
      : m_type(OdGsFiler::kEOFSection)
      , m_begin(0)
    { }
    WrSectionData(OdGsFiler::Section section, OdUInt32 tell)
      : m_type(section)
      , m_begin(tell)
    { }
  };
  OdStack<WrSectionData> m_sectionStack;
  mutable OdSmartPtr<OdGsFiler_SubstitutorImpl> m_subst;
  typedef std::map<OdString, OdRxObjectPtr> ArbDataMap;
  ArbDataMap m_arbData;
  typedef std::set<const void *> RegPtrsSet;
  RegPtrsSet m_regPtrs;
protected:
  bool isWriting() const { return GETBIT(m_nFlags, kOpenedForWrite); }
  void setWriting(bool bSet) { SETBIT(m_nFlags, kOpenedForWrite, bSet); }

  void assertWr() const { if (!isWriting()) throw OdError(eNotOpenForWrite); }
  void assertRd() const { if (isWriting()) throw OdError(eNotOpenForRead); }
public:
  /** \details
    Default constructor for the OdGsFilerV100Impl class. Sets the following data members:
    
    <table>
      Data member                 Value                Description
      m_nSectionsToWrite          -1                   Sections for which write operations are enabled.
      m_nSectionsToRead           -1                   Sections for which read operations are enabled.
      m_nFlags                    0                    Filer flags (indicates whether filer is opened for write operations).
      m_nVersion                  kV2                  File version.
      m_nCurSection               kEOFSection          Current file section.
      m_nCurSectionSize           0                    Current file section size.
    </table>
  */
  OdGsFilerV100Impl()
    : m_nSectionsToWrite(OdUInt64(-1))
    , m_nSectionsToRead(OdUInt64(-1))
    , m_nFlags(0)
    , m_nVersion(kV2)
    , m_nCurSection(kEOFSection)
    , m_nCurSectionSize(0)
  {
    m_subst = OdRxObjectImpl<OdGsFiler_SubstitutorImpl>::createObject();
  }
  
  /** \details
    Destructor for the OdGsFilerV100Impl class.
  */
  ~OdGsFilerV100Impl()
  {
  }

  OdUInt32 headerTestChunk() const;
  /** \details
    Reads a header section.
    \returns
    false if current section is not header section or if header test chunk is not read successfully or if after setting version checkEOF() call returns false.
  */
  bool rdHeaderSection();
  
  /** \details
    Writes a header test chunk to the header section.
  */
  void wrHeaderSection();

  /** \details
    Attaches a specified stream and opens it for read and potentially write operations.
    
    \param pStream  [in]  Pointer to a stream to set.
    \param bWrite   [in]  Flag that specifies whether to open stream for write operations.
  */
  virtual bool setStream(OdStreamBuf *pStream, bool bWrite = false);
  
  /** \details
    Retrieves a smart pointer to current stream.
  */
  virtual OdStreamBufPtr getStream() const;

  /** \details
    Sets a specified database.
    
    \param pDb  [in]  Pointer to a database to set.
  */
  virtual void setDatabase(const OdDbBaseDatabase *pDb);
  
  /** \details
    Retrieves a pointer to a database that is used for this stream.
  */
  virtual OdDbBaseDatabase *getDatabase() const;

  /** \details
    Sets a file version for this filer.
    
    \param nVersion  [in]  File version. See Version enum.
  */
  virtual void setVersion(OdUInt32 nVersion);
  
  /** \details
    Retrieves a file version for this filer.
    
    \returns
    one of the following values:
    * 100 - file version 1
    * 200 - file version 2
  */
  virtual OdUInt32 version() const;

  virtual void wrDbHash(const OdDbBaseDatabase *pDb);
  virtual bool checkDbHash(const OdDbBaseDatabase *pDb);

  /** \details
    Sets sections for write operations.
    
    \param nSections  [in]  Sections to set for writing.
  */
  virtual void setWriteSections(OdUInt64 nSections);
  
  /** \details
    Sets the specified section for write operations.
    
    \param section  [in]  Section to set.
    \param bSet     [in]  Flag that specifies whether to write to the section.
  */
  virtual void setWriteSection(Section section, bool bSet);
  
  /** \details
    Checks whether the specified section is opened for write operations.
    
    \param section  [in]  Section to check.
    \returns
    true if write operations are enabled for the section, false otherwise.
  */
  virtual bool isWriteSection(Section section) const;

  /** \details
    Sets sections for read operations.
    
    \param nSections  [in]  Sections to set for reading.
  */
  virtual void setReadSections(OdUInt64 nSections);
  
  /** \details
    Sets the specified section for read operations.
    
    \param section  [in]  Section to set.
    \param bSet     [in]  Flag that specifies whether to read the section.
  */
  virtual void setReadSection(Section section, bool bSet);
  
  /** \details
    Checks whether the specified section is opened for read operations.
    
    \param section  [in]  Section to check.
    \returns
    true if read operations are enabled for the section, false otherwise.
  */
  virtual bool isReadSection(Section section) const;

  /** \details
    Begins writing to the section.
    
    \param section  [in]  File section.
  */
  virtual void wrSectionBegin(Section section);
  
  /** \details
    Ends writing to the section.
    
    \param section  [in]  File section.
  */
  virtual void wrSectionEnd(Section section);
  
  /** \details
    Writes the end of file (EOF) section.
  */
  virtual void wrEOFSection();

  /** \details
    Reads sections one by one and returns a current section for which read access is not enabled or the EOF section.
  */
  virtual Section rdSection() const;
  
  /** \details
    Retrieves the current section.
  */
  virtual Section curSection() const;
  
  /** \details
    Skips the current section and makes the following section current.
  */
  virtual void skipSection() const;
  virtual void rdBackSection() const;
  
  /** \details
    Checks whether currently read section is the end of file (EOF) section.
    \returns
    true if current section is EOF section, false otherwise.
  */
  virtual bool checkEOF() const;

  virtual Substitutor *subst() const;
  virtual void makeSubstitutions(bool bClear = true) const;
  virtual OdRxObjectPtr getSubstitutor() const;
  virtual void setSubstitutor(OdRxObject *pSubst);

  virtual void setArbitraryData(const OdChar *pName, OdRxObject *pObject);
  virtual OdRxObjectPtr getArbitraryData(const OdChar *pName) const;
  virtual bool hasArbitraryData(const OdChar *pName) const;
  virtual void clearArbitraryData();

  virtual void registerPtr(const void *pPtr);
  virtual void unregisterPtr(const void *pPtr);
  virtual bool isPtrRegistered(const void *pPtr) const;
  virtual void clearRegisteredPtrs();

  /** \details
    Writes database handle to the stream.
    
    \param pHandle  [in]  Pointer to database handle.
  */
  virtual void wrHandle(OdDbStub *pHandle);
  
  /** \details
    Retrieves a pointer to database handle.
  */
  virtual OdDbStub *rdHandle() const;

  /** \details
    Writes a name of the class associated with passed object to the stream.
    
    \param pObj  [in]  Pointer to database handle.
  */
  virtual void wrClass(OdRxObject *pObj);
  
  /** \details
    Retrieves a smart pointer to a class at current stream position.
    
    \returns
    smart pointer to a class by read class name, if class not found by class name this method returns a smart pointer to a not valid class.
  */
  virtual OdRxObjectPtr rdClass() const;

  /** \details
    Writes raw data to the stream.
    
    \param pData      [in]  Pointer to data (in bytes).
    \param nDataSize  [in]  Data size.
  */
  virtual void wrRawData(const void *pData, OdUInt32 nDataSize);
  
  /** \details
    Reads raw data from the stream.
    
    \param pData      [out]  Pointer to data (in bytes).
    \param nDataSize  [in]   Data size.
  */
  virtual void rdRawData(void *pData, OdUInt32 nDataSize) const;
};

#include "TD_PackPop.h"

#endif // __OdGsFilerV100Impl_H__
