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

#ifndef __OD_GS_FILER_H__
#define __OD_GS_FILER_H__

#include "TD_PackPush.h"

#include "OdStreamBuf.h"
#include "Gs/GsExport.h"
#include "OdBinaryData.h"
#include "OdVector.h"
#include "IntArray.h"
#include "UInt8Array.h"
#include "UInt32Array.h"
#include "UInt16Array.h"
#include "GsMarkerArray.h"
#include "DbStubPtrArray.h"
#include "Ge/GePoint2d.h"
#include "UInt64Array.h"

class OdGePoint2d;
class OdGeVector2d;
class OdGePoint3d;
class OdGeVector3d;
class OdGeMatrix3d;
class OdGeExtents3d;

class OdDbStub;
typedef OdRxObject OdDbBaseDatabase;

class OdGsView;
class OdGsCache;

class OdGsDCRect;
class OdGsDCRectDouble;
typedef OdArray<float, OdMemoryAllocator<float> > OdFloatArray;
typedef OdArray<OdGePoint2d, OdMemoryAllocator<OdGePoint2d> > OdGePoint2dArray;

/** <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsFiler : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGsFiler);

    /** \details
      Default constructor for the OdGsFiler class.
    */
    OdGsFiler() { }
    
    /** \details
      Destructor for the OdGsFiler class.
    */
    virtual ~OdGsFiler() { }
  public:
    /** \details
      Represents a file section.
    */
    enum Section
    {
      kEOFSection = -1,      // EOF
      // V1 sections
      kHeaderSection = 0,     // File header
      kDatabaseLinkSection,   // Database linkage section
      kGsModuleSection,       // Gs Module section
      kDeviceSection,         // Gs Device section
      kClientDeviceSection,   // Client Gs Device section
      kViewSection,           // Gs View section
      kClientViewSection,     // Client Gs View section
      kModelSection,          // Gs Model section
      kClientModelSection,    // Client Gs Model section
      kNodeSection,           // Gs Node section
      kClientNodeSection,     // Client Gs Node section
      kMetafileSection,       // Gs Metafile section
      kClientMetafileSection, // Client Gs Metafile section
      kClientMaterialSection, // Client Gs Material section
      kBlockRefImplSection,   // Block reference cache implementation section
      kLinkedDeviceSection,   // Linked Gs Device section
      kRenditionSection,      // Client visual rendition section
      // V2 sections
      kRuntimeChangesSection  // Runtime Gs modifications section
    };
    /** \details
      Represents a file version.
    */
    enum Version
    {
      kV1 = 100,
      kV2 = 200,
      kVLast = kV2
    };
    struct SubstitutionActuator
    {
      typedef void (*SetPtrFunc)(void *pPlace, const void *pValue);
      SubstitutionActuator() { }
      virtual ~SubstitutionActuator() { }
      virtual void applySubstitution(void *pPlace, const void *pValue, SetPtrFunc pSetFunc) { pSetFunc(pPlace, pValue); }
    };
    struct Substitutor
    {
      // Register known substitution
      virtual void registerSubstitution(const void *pValue, const void *pSubstitution, OdUInt32 size = sizeof(OdIntPtr), bool bRegister = true, bool bImmediate = false) = 0;
      // Register substitution request
      virtual void requestSubstitution(void *pPlace, const void *pValue, OdUInt32 size = sizeof(OdIntPtr), bool bRegister = true, bool bImmediate = true) = 0;
      // Register substitution request for smart pointer
      virtual void requestSubstitution(OdBaseObjectPtr *pPlace, const void *pValue, bool bRegister = true, bool bImmediate = true) = 0;
      // Register substitution with non-default behavior
      virtual void requestSubstitution(void *pPlace, OdGsFiler::SubstitutionActuator *pActuator, const void *pValue, OdUInt32 size = sizeof(OdIntPtr), bool bRegister = true, bool bImmediate = true) = 0;
      // Clear substitutions
      virtual void clearSubstitutions(const void *pValue = NULL, OdUInt32 size = 0) = 0;
      // Run substitutions
      virtual void runSubstitutions(const void *pValue = NULL, OdUInt32 size = 0, bool bClear = true) = 0;
    };
  public:
    // Attach stream and initiate read/write process
    /** \details
      Attaches a specified stream and opens it for read and potentially write operations.
      
      \param pStream  [in]  Pointer to a stream to set.
      \param bWrite   [in]  Flag that specifies whether to open stream for write operations.
    */
    virtual bool setStream(OdStreamBuf *pStream, bool bWrite = false) = 0;
    
    /** \details
      Retrieves a smart pointer to current stream.
    */
    virtual OdStreamBufPtr getStream() const = 0;

    // Setup working database
    /** \details
      Sets a specified database.
      
      \param pDb  [in]  Pointer to a database to set.
    */
    virtual void setDatabase(const OdDbBaseDatabase *pDb) = 0;
    
    /** \details
      Retrieves a pointer to a database that is used for this stream.
    */
    virtual OdDbBaseDatabase *getDatabase() const = 0;

    // File version control
    /** \details
      Sets a file version for this filer.
      
      \param nVersion  [in]  File version. See Version enum.
    */
    virtual void setVersion(OdUInt32 nVersion) = 0;
    
    /** \details
      Retrieves a file version for this filer.
      
      \returns
      one of the following values:
      * 100 - file version 1
      * 200 - file version 2
    */
    virtual OdUInt32 version() const = 0;

    // Database hash control
    virtual void wrDbHash(const OdDbBaseDatabase *pDb) = 0;
    virtual bool checkDbHash(const OdDbBaseDatabase *pDb) = 0;

    // Setup sections for writing
    /** \details
      Sets sections for write operations.
      
      \param nSections  [in]  Sections to set for writing.
    */
    virtual void setWriteSections(OdUInt64 nSections) = 0;
    
    /** \details
      Sets the specified section for write operations.
      
      \param section  [in]  Section to set.
      \param bSet     [in]  Flag that specifies whether to write to the section.
    */
    virtual void setWriteSection(Section section, bool bSet) = 0;
    
    /** \details
      Checks whether the specified section is opened for write operations.
      
      \param section  [in]  Section to check.
      \returns
      true if write operations are enabled for the section, false otherwise.
    */
    virtual bool isWriteSection(Section section) const = 0;

    // Setup sections for reading
    /** \details
      Sets sections for read operations.
      
      \param nSections  [in]  Sections to set for reading.
    */
    virtual void setReadSections(OdUInt64 nSections) = 0;
    
    /** \details
      Sets the specified section for read operations.
      
      \param section  [in]  Section to set.
      \param bSet     [in]  Flag that specifies whether to read the section.
    */
    virtual void setReadSection(Section section, bool bSet) = 0;
    
    /** \details
      Checks whether the specified section is opened for read operations.
      
      \param section  [in]  Section to check.
      \returns
      true if read operations are enabled for the section, false otherwise.
    */
    virtual bool isReadSection(Section section) const = 0;

    // Section writing
    /** \details
      Begins writing to the section.
      
      \param section  [in]  File section.
    */
    virtual void wrSectionBegin(Section section) = 0;
    
    /** \details
      Ends writing to the section.
      
      \param section  [in]  File section.
    */
    virtual void wrSectionEnd(Section section) = 0;
    
    /** \details
      Writes the end of file (EOF) section.
    */
    virtual void wrEOFSection() = 0;

    // Section reading
    /** \details
      Reads sections one by one and returns a current section for which read access is not enabled or the EOF section.
    */
    virtual Section rdSection() const = 0;
    
    /** \details
      Retrieves the current section.
    */
    virtual Section curSection() const = 0;
    
    /** \details
      Skips the current section and makes the following section current.
    */
    virtual void skipSection() const = 0;
    virtual void rdBackSection() const = 0;
    /** \details
      Checks whether currently read section is the end of file (EOF) section.
      \returns
      true if current section is EOF section, false otherwise.
    */
    virtual bool checkEOF() const = 0;

    // Substitutions processing
    virtual Substitutor *subst() const = 0;
    virtual void makeSubstitutions(bool bClear = true) const = 0;
    virtual OdRxObjectPtr getSubstitutor() const = 0;
    virtual void setSubstitutor(OdRxObject *pSubst) = 0;

    // Arbitrary data
    virtual void setArbitraryData(const OdChar *pName, OdRxObject *pObject) = 0;
    virtual OdRxObjectPtr getArbitraryData(const OdChar *pName) const = 0;
    virtual bool hasArbitraryData(const OdChar *pName) const = 0;
    virtual void clearArbitraryData() = 0;

    // Pointers registration
    virtual void registerPtr(const void *pPtr) = 0;
    virtual void unregisterPtr(const void *pPtr) = 0;
    virtual bool isPtrRegistered(const void *pPtr) const = 0;
    virtual void clearRegisteredPtrs() = 0;

    // Process handles
    /** \details
      Writes database handle to the stream.
      
      \param pHandle  [in]  Pointer to database handle.
    */
    virtual void wrHandle(OdDbStub *pHandle) = 0;
    
    /** \details
      Retrieves a pointer to database handle.
    */
    virtual OdDbStub *rdHandle() const = 0;

    // Process classes
    /** \details
      Writes a name of the class associated with passed object to the stream.
      
      \param pObj  [in]  Pointer to database handle.
    */
    virtual void wrClass(OdRxObject *pObj) = 0;
    
    /** \details
      Retrieves a smart pointer to a class at current stream position.
      
      \returns
      smart pointer to a class by read class name, if class not found by class name this method returns a smart pointer to a not valid class.
    */
    virtual OdRxObjectPtr rdClass() const = 0;

    // Process raw data
    /** \details
      Writes raw data to the stream.
      
      \param pData      [in]  Pointer to data (in bytes).
      \param nDataSize  [in]  Data size.
    */
    virtual void wrRawData(const void *pData, OdUInt32 nDataSize) = 0;
    
    /** \details
      Reads raw data from the stream.
      
      \param pData      [out]  Pointer to data (in bytes).
      \param nDataSize  [in]   Data size.
    */
    virtual void rdRawData(void *pData, OdUInt32 nDataSize) const = 0;

    // Process data primitives
    /** \details
      Writes data of bool type to the stream.
      
      \param bVal      [in]  Data to write.
    */
    virtual void wrBool(bool bVal);
    
    /** \details
      Reads data of bool type from the stream.
      
      \returns
      read bool value.
    */
    virtual bool rdBool() const;

    /** \details
      Writes data of int type to the stream.
      
      \param val      [in]  Data to write.
    */
    virtual void wrInt(int val);
    
    /** \details
      Reads data of int type from the stream.
      
      \returns
      read int value.
    */
    virtual int rdInt() const;

    /** \details
      Writes data of unsigned int type to the stream.
      
      \param val      [in]  Data to write.
    */
    virtual void wrUInt(unsigned int val);
    
    /** \details
      Reads data of unsigned int type from the stream.
      
      \returns
      read unsigned int value.
    */
    virtual unsigned int rdUInt() const;

    /** \details
      Writes data of char type to the stream.
      
      \param val      [in]  Data to write.
    */
    virtual void wrChar(char val);
    
    /** \details
      Reads data of char type from the stream.
      
      \returns
      read char value.
    */
    virtual char rdChar() const;

    /** \details
      Writes data of OdUInt8 type (unsigned int 8 bits) to the stream.
      
      \param val      [in]  Data to write.
    */
    virtual void wrUInt8(OdUInt8 val);
    
    /** \details
      Reads data of OdUInt8 type (unsigned int 8 bits) from the stream.
      
      \returns
      read OdUInt8 value.
    */
    virtual OdUInt8 rdUInt8() const;

    /** \details
      Writes data of OdInt16 type (int 16 bits) to the stream.
      
      \param val      [in]  Data to write.
    */
    virtual void wrInt16(OdInt16 val);
    
    /** \details
      Reads data of OdInt16 type (int 16 bits) from the stream.
      
      \returns
      read OdInt16 value.
    */
    virtual OdInt16 rdInt16() const;

    /** \details
      Writes data of OdUInt16 type (unsigned int 16 bits) to the stream.
      
      \param val      [in]  Data to write.
    */
    virtual void wrUInt16(OdUInt16 val);
    
    /** \details
      Reads data of OdUInt16 type (unsigned int 16 bits) from the stream.
      
      \returns
      read OdUInt16 value.
    */
    virtual OdUInt16 rdUInt16() const;

    /** \details
      Writes data of OdInt32 type (int 32 bits) to the stream.
      
      \param val      [in]  Data to write.
    */
    virtual void wrInt32(OdInt32 val);
    
    /** \details
      Reads data of OdInt32 type (int 32 bits) from the stream.
      
      \returns
      read OdInt32 value.
    */
    virtual OdInt32 rdInt32() const;

    /** \details
      Writes data of OdUInt32 type (unsigned int 32 bits) to the stream.
      
      \param val      [in]  Data to write.
    */
    virtual void wrUInt32(OdUInt32 val);
    
    /** \details
      Reads data of OdUInt32 type (unsigned int 32 bits) from the stream.
      
      \returns
      read OdUInt32 value.
    */
    virtual OdUInt32 rdUInt32() const;

    /** \details
      Reads data of OdInt64 type (int 64 bits) from the stream.
      
      \returns
      read OdInt64 value.
    */
    virtual void wrInt64(OdInt64 val);
    
    /** \details
      Reads data of OdInt64 type (int 64 bits) from the stream.
      
      \returns
      read OdInt64 value.
    */
    virtual OdInt64 rdInt64() const;

    /** \details
      Writes data of OdUInt64 type (unsigned int 64 bits) to the stream.
      
      \param val      [in]  Data to write.
    */
    virtual void wrUInt64(OdUInt64 val);
    
    /** \details
      Reads data of OdUInt64 type (unsigned int 64 bits) from the stream.
      
      \returns
      read OdUInt64 value.
    */
    virtual OdUInt64 rdUInt64() const;

    /** \details
      Writes data of OdIntPtr type (integer that is able to store a pointer (32 or 64 depending on platform)) to the stream.
      
      \param val      [in]  Data to write.
    */
    virtual void wrIntPtr(OdIntPtr val);
    
    /** \details
      Reads data of rdIntPtr type (integer that is able to store a pointer (32 or 64 depending on platform)) from the stream.
      
      \returns
      read OdIntPtr value.
    */
    virtual OdIntPtr rdIntPtr() const;

    /** \details
      Writes data defined by the passed pointer. Internally type is cast to OdIntPtr.
      
      \param pPtr      [in]  Pointer to the data to write.
    */
    void wrPtr(const void *pPtr) { wrIntPtr((OdIntPtr)pPtr); }
    
    /** \details
      Reads the data and returns pointer to it. Length of the read data equals to OdIntPtr data type length.
    */
    void *rdPtr() const { return (void*)rdIntPtr(); }

    /** \details
      Writes color value defined by the ODCOLORREF macro.
      
      \param val      [in]  Value to write.
    */
    virtual void wrCOLORREF(ODCOLORREF val);
    
    /** \details
      Reads the color data defined by the ODCOLORREF macro. Length of the read data equals to OdUInt32 data type length.
    */
    virtual ODCOLORREF rdCOLORREF() const;

    /** \details
      Writes data of float type to the stream.
      
      \param val      [in]  Data to write.
    */
    virtual void wrFloat(float val);
    
    /** \details
      Reads data of float type from the stream.
      
      \returns
      read float value.
    */
    virtual float rdFloat() const;

    /** \details
      Writes data of double type to the stream.
      
      \param val      [in]  Data to write.
    */
    virtual void wrDouble(double val);
    
    /** \details
      Reads data of double type from the stream.
      
      \returns
      read double value.
    */
    virtual double rdDouble() const;

    /** \details
      Writes data of OdGePoint2d type (2-dimensional point) to the stream.
      
      \param pt      [in]  Data to write.
    */
    virtual void wrPoint2d(const OdGePoint2d &pt);
    
    /** \details
      Reads data of OdGePoint2d type from the stream.
      
      \param pt      [out]  Receives 2D point.
    */
    virtual void rdPoint2d(OdGePoint2d &pt) const;

    /** \details
      Writes data of OdGeVector2d type (2-dimensional vector) to the stream.
      
      \param vec      [in]  Data to write.
    */
    virtual void wrVector2d(const OdGeVector2d &vec);
    
    /** \details
      Reads data of OdGeVector2d type from the stream.
      
      \param vec      [out]  Receives 2D vector.
    */
    virtual void rdVector2d(OdGeVector2d &vec) const;

    /** \details
      Writes data of OdGePoint3d type (3-dimensional point) to the stream.
      
      \param pt      [in]  Data to write.
    */
    virtual void wrPoint3d(const OdGePoint3d &pt);
    
    /** \details
      Reads data of OdGePoint3d type from the stream.
      
      \param pt      [out]  Receives 3D point.
    */
    virtual void rdPoint3d(OdGePoint3d &pt) const;

    /** \details
      Writes data of OdGeVector3d type (3-dimensional vector) to the stream.
      
      \param vec      [in]  Data to write.
    */
    virtual void wrVector3d(const OdGeVector3d &vec);
    
    /** \details
      Reads data of OdGeVector3d type from the stream.
      
      \param vec      [out]  Receives 3D vector.
    */
    virtual void rdVector3d(OdGeVector3d &vec) const;

    /** \details
      Writes data of OdGeMatrix3d type (3-dimensional transformation matrix) to the stream.
      
      \param mat      [in]  Data to write.
    */
    virtual void wrMatrix3d(const OdGeMatrix3d &mat);
    
    /** \details
      Reads data of OdGeMatrix3d type from the stream.
      
      \param mat      [out]  Receives 3D transformation matrix.
    */
    virtual void rdMatrix3d(OdGeMatrix3d &mat) const;

    /** \details
      Writes data of OdGeExtents3d type (3-dimensional extents) to the stream.
      
      \param ext      [in]  Data to write.
    */
    virtual void wrExtents3d(const OdGeExtents3d &ext);
    
    /** \details
      Reads data of OdGeExtents3d type from the stream.
      
      \param ext      [out]  Receives 3D extents.
    */
    virtual void rdExtents3d(OdGeExtents3d &ext) const;

    /** \details
      Writes data of OdAnsiString type (ANSI string) to the stream.
      
      \param str      [in]  Data to write.
    */
    virtual void wrAnsiString(const OdAnsiString &str);
    
    /** \details
      Reads data of OdAnsiString type from the stream.
      
      \param str      [out]  Receives ANSI string.
    */
    virtual void rdAnsiString(OdAnsiString &str) const;
    
    /** \details
      Reads and returns data of OdAnsiString type from the stream.
    */
    OdAnsiString rdAnsiStringRet() const { OdAnsiString str; rdAnsiString(str); return str; }

    /** \details
      Writes data of OdString type (string) to the stream.
      
      \param str      [in]  Data to write.
    */
    virtual void wrString(const OdString &str);
    
    /** \details
      Reads data of OdString type (string) from the stream.
      
      \param str      [out]  Receives a string.
    */
    virtual void rdString(OdString &str) const;
    
    /** \details
      Reads and returns data of OdString type from the stream.
    */
    OdString rdStringRet() const { OdString str; rdString(str); return str; }

    /** \details
      Writes an array of OdUInt8 values (unsigned int 8 bits) to the stream.
      
      \param arr      [in]  Array to write.
    */
    virtual void wrUInt8Array (const OdUInt8Array& arr);
    
    /** \details
      Writes an array of OdUInt16 values (unsigned int 16 bits) to the stream.
      
      \param arr      [in]  Array to write.
    */
    virtual void wrUInt16Array(const OdUInt16Array& arr);
    
    /** \details
      Writes an array of OdUInt16 values (unsigned int 16 bits) to the stream.
      
      \param count    [in]  Count of elements in the array.
      \param arr      [in]  Array to write.
    */
    virtual void wrUInt16Array(OdUInt32 count, const OdUInt16* arr);
    
    /** \details
      Writes an array of OdUInt32 values (unsigned int 32 bits) to the stream.
      
      \param arr      [in]  Array to write.
    */
    virtual void wrUInt32Array(const OdUInt32Array& arr);
    
    /** \details
      Writes a vector of OdUInt32 values (unsigned int 32 bits) to the stream.
      
      \param arr      [in]  Vector to write.
    */
    virtual void wrUInt32Array(const OdUInt32Vector& arr);
    
    /** \details
      Writes an array of OdUInt64 values (unsigned int 64 bits) to the stream.
      
      \param arr      [in]  Array to write.
    */
    virtual void wrUInt64Array(const OdUInt64Array& arr);
    
    /** \details
      Writes a vector of OdUInt64 values (unsigned int 64 bits) to the stream.
      
      \param vec      [in]  Vector to write.
    */
    virtual void wrUInt64Array(const OdUInt64Vector& vec);
    
    /** \details
      Writes an array of OdUInt64 values (unsigned int 16 bits) to the stream.
      
      \param count    [in]  Count of elements in the array.
      \param arr      [in]  Array to write.
    */
    virtual void wrUInt64Array(OdUInt32 count, const OdUInt64* arr);
    
    /** \details
      Writes an array of integer values to the stream.
      
      \param arr      [in]  Array to write.
    */
    virtual void wrIntArray   (const OdIntArray& arr);
    
    /** \details
      Writes an array of float values to the stream.
      
      \param arr      [in]  Array to write.
    */
    virtual void wrFloatArray (const OdFloatArray& arr);
    
    /** \details
      Writes an array of float values to the stream.
      
      \param count    [in]  Count of elements in the array.
      \param arr      [in]  Array to write.
    */
    virtual void wrFloatArray (OdUInt32 count, const float* arr);
    
    /** \details
      Writes an array of 2D points to the stream.
      
      \param arr      [in]  Array to write.
    */
    virtual void wrPoint2dArray  (const OdGePoint2dArray& arr);
    
    /** \details
      Writes an array of 3D points to the stream.
      
      \param arr      [in]  Array to write.
    */
    virtual void wrPoint3dArray  (const OdGePoint3dArray& arr);
    
    /** \details
      Writes an array of pointers to database-resident objects to the stream.
      
      \param arr      [in]  Array to write.
    */
    virtual void wrDbStubPtrArray(const OdDbStubPtrArray& arr);
    
    /** \details
      Writes an array of 2D device rectangles to the stream.
      
      \param rc      [in]  Array to write.
    */
    virtual void wrGsDCRect      (const OdGsDCRect& rc);
    
    /** \details
      Writes an array of 2D device rectangles (with coordinates represented by values of the double type) to the stream.
      
      \param rcd     [in]  Array to write.
    */
    virtual void wrGsDCRectDouble(const OdGsDCRectDouble& rcd);

    /** \details
      Reads an array of OdUInt8 values (unsigned int 8 bits) from the stream.
      
      \param arr     [out]  Receives an array of OdUInt8 values.
    */
    virtual void rdUInt8Array (OdUInt8Array& arr);
    
    /** \details
      Reads an array of OdUInt16 values (unsigned int 16 bits) from the stream.
      
      \param arr     [out]  Receives an array of OdUInt16 values.
    */
    virtual void rdUInt16Array(OdUInt16Array& arr);
    
    /** \details
      Reads an array of OdUInt16 values (unsigned int 16 bits) from the stream.
      
      \param count    [in]  Count of elements in the array.
      \param arr      [out]  Receives an array of OdUInt16 values.
    */
    virtual void rdUInt16Array(OdUInt32 count, OdUInt16* arr);
    
    /** \details
      Reads an array of OdUInt32 values (unsigned int 32 bits) from the stream.
      
      \param arr     [out]  Receives an array of OdUInt32 values.
    */
    virtual void rdUInt32Array(OdUInt32Array& arr);
    
    /** \details
      Reads a vector of OdUInt32 values (unsigned int 32 bits) from the stream.
      
      \param arr     [out]  Receives a vector of OdUInt32 values.
    */
    virtual void rdUInt32Array(OdUInt32Vector& arr);
    
    /** \details
      Reads an array of OdUInt64 values (unsigned int 64 bits) from the stream.
      
      \param arr     [out]  Receives an array of OdUInt64 values.
    */
    virtual void rdUInt64Array(OdUInt64Array& arr);
    
    /** \details
      Reads a vector of OdUInt64 values (unsigned int 64 bits) from the stream.
      
      \param arr     [out]  Receives a vector of OdUInt64 values.
    */
    virtual void rdUInt64Array(OdUInt64Vector& vec);
    
    /** \details
      Reads an array of OdUInt64 values (unsigned int 64 bits) from the stream.
      
      \param count    [in]  Count of elements in the array.
      \param arr      [out]  Receives an array of OdUInt64 values.
    */
    virtual void rdUInt64Array(OdUInt32 count, OdUInt64* arr);
    
    /** \details
      Reads an array of integer values from the stream.
      
      \param arr      [out]  Receives an array of integer values.
    */
    virtual void rdIntArray   (OdIntArray& arr);
    
    /** \details
      Reads an array of float values from the stream.
      
      \param arr      [out]  Receives an array of float values.
    */
    virtual void rdFloatArray (OdFloatArray& arr);
    
    /** \details
      Reads an array of float values from the stream.
      
      \param count    [in]   Count of elements in the array.
      \param arr      [out]  Receives an array of float values.
    */
    virtual void rdFloatArray (OdUInt32 count, float* arr);
    
    /** \details
      Reads an array of 2D points from the stream.
      
      \param arr      [out]  Receives an array of 2D points.
    */
    virtual void rdPoint2dArray  (OdGePoint2dArray& arr);
    
    /** \details
      Reads an array of 3D points from the stream.
      
      \param arr      [out]  Receives an array of 3D points.
    */
    virtual void rdPoint3dArray  (OdGePoint3dArray& arr);
    
    /** \details
      Reads an array of pointers to database-resident objects from the stream.
      
      \param arr      [out]  Receives an array of pointers to database-resident objects.
    */
    virtual void rdDbStubPtrArray(OdDbStubPtrArray& arr);
    
    /** \details
      Reads an array of 2D device rectangles from the stream.
      
      \param arr      [out]  Receives an array of 2D device rectangles.
    */
    virtual void rdGsDCRect      (OdGsDCRect& dcrc);
    
    /** \details
      Reads an array of 2D device rectangles (with coordinates represented by values of the double type) from the stream.
      
      \param arr      [out]  Receives an array of 2D device rectangles (with coordinates represented by values of the double type).
    */
    virtual void rdGsDCRectDouble(OdGsDCRectDouble& dcrcd);

    /** \details
      Creates a filer object and returns a smart pointer to it.
      
      \param pStream    [in]  Pointer to a stream buffer.
      \param bForWrite  [in]  Flag that specifies whether to open filer for write operations.
      \param pDb        [in]  Pointer to a database.
      \param nVersion   [in]  File version.
    */
    static OdSmartPtr<OdGsFiler> createObject(OdStreamBuf *pStream, bool bForWrite, const OdDbBaseDatabase *pDb, OdUInt32 nVersion = kVLast);
};

typedef OdSmartPtr<OdGsFiler> OdGsFilerPtr;

#define OdGsFiler_wrArray(filer, arry, wrFunc) \
  { \
    filer.wrUInt32(arry.size()); \
    for (OdUInt32 nVal = 0; nVal < arry.size(); nVal++) \
      filer.wrFunc(arry.getPtr()[nVal]); \
  }
#define OdGsFilerPtr_wrArray(pFiler, arry, wrFunc) \
  OdGsFiler_wrArray((*pFiler), arry, wrFunc)

#define OdGsFiler_wrArrayRaw(filer, arry, typSz) \
  { \
    filer.wrUInt32(arry.size()); \
    if (!arry.isEmpty()) \
      filer.wrRawData(arry.getPtr(), arry.size() * typSz); \
  }
#define OdGsFilerPtr_wrArrayRaw(pFiler, arry, typSz) \
  OdGsFiler_wrArrayRaw((*pFiler), arry, typSz)

#define OdGsFiler_rdArray(filer, arry, rdFunc) \
  { \
    arry.clear(); \
    OdUInt32 nElems = filer.rdUInt32(); \
    arry.setPhysicalLength(nElems); \
    for (OdUInt32 nElem = 0; nElem < nElems; nElem++) \
      arry.push_back(filer.rdFunc()); \
  }
#define OdGsFilerPtr_rdArray(pFiler, arry, rdFunc) \
  OdGsFiler_rdArray((*pFiler), arry, rdFunc)

#define OdGsFiler_rdArrayArg(filer, arry, rdFunc) \
  { \
    OdUInt32 nElems = filer.rdUInt32(); \
    arry.resize(nElems); \
    for (OdUInt32 nElem = 0; nElem < nElems; nElem++) \
      filer.rdFunc(arry[nElem]); \
  }
#define OdGsFilerPtr_rdArrayArg(pFiler, arry, rdFunc) \
  OdGsFiler_rdArrayArg((*pFiler), arry, rdFunc)

#define OdGsFiler_rdArrayRaw(filer, arry, typSz) \
  { \
    OdUInt32 nElems = filer.rdUInt32(); \
    arry.resize(nElems); \
    if (nElems) \
      filer.rdRawData(arry.asArrayPtr(), nElems * typSz); \
  }
#define OdGsFilerPtr_rdArrayRaw(pFiler, arry, typSz) \
  OdGsFiler_rdArrayRaw((*pFiler), arry, typSz)

/** <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsFilerCreatorPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGsFilerCreatorPE);
public:
  /** \details
    Creates a GS filer object and returns a smart pointer to it.
    
    \param pStream    [in]  Pointer to a stream buffer.
    \param bForWrite  [in]  Flag that specifies whether to open filer for write operations.
    \param pDb        [in]  Pointer to a database.
  */
  virtual OdGsFilerPtr createGsFiler(OdStreamBuf *pStream, bool bForWrite, const OdDbBaseDatabase *pDb) = 0;
};

/** \details
  A data type that represents a smart pointer to an <link OdGsFilerCreatorPE, OdGsFilerCreatorPE> object.
*/
typedef OdSmartPtr<OdGsFilerCreatorPE> OdGsFilerCreatorPEPtr;

/** <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsFilerDbHashPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGsFilerDbHashPE);
public:
  /** \details
    Computes database hash.
    
    \param pDb        [in]  Pointer to a database.
    \returns
    database hash.
  */
  virtual OdBinaryData computeDbHash(const OdDbBaseDatabase *pDb) = 0;
};

/** \details
  A data type that represents a smart pointer to an <link OdGsFilerDbHashPE, OdGsFilerDbHashPE> object.
*/
typedef OdSmartPtr<OdGsFilerDbHashPE> OdGsFilerDbHashPEPtr;

#include "TD_PackPop.h"

#endif // __OD_GS_FILER_H__
