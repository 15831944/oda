/************************************************************************
* StructuredStorage.h
* Public Header file for Structured Storage library
*
* (c) Schema Software Inc., 2001-2004
* Authors: Bob Sabiston, Yuri Khramov, Mark Ambachtsheer
************************************************************************

$Revision: 1.19.2.2 $
$Date: 2004/02/26 19:19:45 $
*/
#ifndef _StructuredStorage_h
#define _StructuredStorage_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef SSTG_PALMOS
#include <VFSMgr.h>      /* For definition of FileRef */
#endif /* SSTG_PALMOS */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    /** Status codes -- return codes for open and create */
#define SSTG_OK 0
#define SSTG_ERROR_FILE_NOT_FOUND 1
#define SSTG_ERROR_ILLEGAL_ACCESS 2
#define SSTG_ERROR_FILE_NOT_SSTG  3
#define SSTG_ERROR_FILEIO 4
#define SSTG_ERROR_MEMORY 5
#define SSTG_ERROR_ILLEGAL_CALL 6
#define SSTG_ERROR_STG_LOCKED 7
#define SSTG_ERROR_CORRUPTED_FAT 8
#define SSTG_ERROR_NOT_STORAGE 9
#define SSTG_ERROR_NOT_STREAM 10
#define SSTG_ERROR_CHILD_NOT_FOUND 11
#define SSTG_ERROR_NOT_SUPPORTED 12
#define SSTG_ERROR_PROPERTY_NOT_FOUND 13
#define SSTG_ERROR_PROPERTY_FORMAT 14
#define SSTG_ERROR_END_OF_STREAM 15

    /* Access mode enumeration */
    typedef enum {STG_READ, STG_WRITE, STG_RW} StgMode;

#define STG_CANREAD( mode )  (( mode == STG_READ  ) || ( mode == STG_RW ))
#define STG_CANWRITE( mode ) (( mode == STG_WRITE ) || ( mode == STG_RW ))

    /* Define a standard 64-bit integer type.  Use #defines rather than 
     * typedefs so that the modifiers "unsigned" and "signed" can be used in 
     * conjunction with the resulting type. 
     * If this define does not work with your compiler, just change the 
     * definition here.
     */
#ifdef _MSC_VER /* Visual C++ */
    #define SSRW_INT64 __int64
#else /* !_MSC_VER, every other compiler */
    #define SSRW_INT64 long long
#endif /* !_MSC_VER */

#include "SsrwInputSource.h"

    /* Relative Seek position options enumeration */
    typedef enum {
        STG_START = 0,
        STG_CUR = 1,
        STG_END = 2 } SeekPos;
    /* Strucutred Storage Directory entry type enumeration */
    typedef enum { STORAGE, STREAM } StgDirType;

#ifndef PIDSI_TITLE
    /* Property IDs for for Summary Info
    we do not typedef them on purpose */
    enum
    {  PIDSI_TITLE = 0x02,           /* Title */
       PIDSI_SUBJECT =  0x00000003,  /* Subject */
       PIDSI_AUTHOR = 0x00000004,    /* Author */
       PIDSI_KEYWORDS = 0x00000005,  /* Keywords */
       PIDSI_COMMENTS = 0x00000006,  /* Comments */
       PIDSI_TEMPLATE = 0x00000007,  /* Template */
       PIDSI_LASTAUTHOR = 0x00000008,   /* Last Saved By */
       PIDSI_REVNUMBER = 0x00000009, /* Revision Number*/
       PIDSI_EDITTIME = 0x0000000A,  /* Total Editing Time */
       PIDSI_LASTPRINTED = 0x0000000B,  /* Last Printed */
       PIDSI_CREATE_DTM = 0x0000000C,   /* Create Time/Date */
       PIDSI_LASTSAVE_DTM = 0x0000000D,/* Last saved Time/Date */
       PIDSI_PAGECOUNT = 0x0000000E,
       PIDSI_WORDCOUNT = 0x0000000F,
       PIDSI_CHARCOUNT = 0x00000010,
       PIDSI_THUMBNAIL = 0x00000011,	/* used for previews by many applications */
       PIDSI_APPNAME = 0x00000012,      /* Name of Creating Application */
       PIDSI_SECURITY = 0x00000013
    };

    /* Property IDs for for Summary Info
    we do not typedef them on purpose */
    enum
    {
        PIDDSI_CATEGORY = 0x00000002,
        PIDDSI_PRESFORMAT = 0x00000003,
        PIDDSI_BYTECOUNT = 0x00000004,
        PIDDSI_LINECOUNT = 0x00000005,
        PIDDSI_PARCOUNT = 0x00000006,
        PIDDSI_SLIDECOUNT = 0x00000007,
        PIDDSI_NOTECOUNT = 0x00000008,
        PIDDSI_HIDDENCOUNT = 0x00000009,
        PIDDSI_MMCLIPCOUNT = 0x0000000A,
        PIDDSI_SCALE = 0x0000000B,
        /* not supported in this version
           PIDDSI_HEADINGPAIR = 0x0000000C,
           PIDDSI_DOCPARTS = 0x0000000D,
        */
        PIDDSI_MANAGER = 0x0000000E,
        PIDDSI_COMPANY = 0x0000000F,
        PIDDSI_LINKSDIRTY = 0x00000010
    };
#else
#if defined(_WIN32_WCE)
#ifndef PIDSI_SECURITY
    enum
    { 
       PIDSI_SECURITY = 0x00000013
    };
#endif
#ifndef PIDDSI_CATEGORY
    /* Property IDs for for Summary Info
    we do not typedef them on purpose */
    enum
    {
        PIDDSI_CATEGORY = 0x00000002,
        PIDDSI_PRESFORMAT = 0x00000003,
        PIDDSI_BYTECOUNT = 0x00000004,
        PIDDSI_LINECOUNT = 0x00000005,
        PIDDSI_PARCOUNT = 0x00000006,
        PIDDSI_SLIDECOUNT = 0x00000007,
        PIDDSI_NOTECOUNT = 0x00000008,
        PIDDSI_HIDDENCOUNT = 0x00000009,
        PIDDSI_MMCLIPCOUNT = 0x0000000A,
        PIDDSI_SCALE = 0x0000000B,
        /* not supported in this version
           PIDDSI_HEADINGPAIR = 0x0000000C,
           PIDDSI_DOCPARTS = 0x0000000D,
        */
        PIDDSI_MANAGER = 0x0000000E,
        PIDDSI_COMPANY = 0x0000000F,
        PIDDSI_LINKSDIRTY = 0x00000010
    };
#endif
#endif //WINCE
#endif
    /* Enumeration of supported property streams */
    typedef enum
    {
        SUMMARY_INFO = 0x001,
        DOC_SUMMARY_INFO = 0x002
    } PropertyStreamType;

    /* Enumeration for type tags */
    typedef enum
    {
        INT2_TYPE,
        INT4_TYPE,
        STRING_TYPE,
        TIME_TYPE,
        CF_TYPE,
        BOOL_TYPE
    } TypeTag;

    /* just in case */
#ifndef boolean
#define boolean char
#endif
#define SSRW_FALSE 0
#define SSRW_TRUE (!SSRW_FALSE)

    /* Structures keeping the information about SStorage objects
      - their members should not be used in the caller programs */
    struct _RootStorage;
    struct _Stream;
    struct _Storage;
    typedef struct _Stream Stream;
    typedef struct _Storage Storage;
    typedef struct _RootStorage RootStorage;

    /* Corresponding structures to Windows GUID, CLSID */
    typedef struct _SSRW_GUID
    {
        unsigned int data1;
        unsigned short data2;
        unsigned short data3;
        unsigned char data4[8];
    }
    SSRW_GUID;
    typedef SSRW_GUID SSRW_CLSID;

    /** Statistic data structure - contains information about storages and streams */
    typedef struct _StgInfo
    {
        wchar_t* sName;
        StgDirType type;
        unsigned long userFlags;
        unsigned SSRW_INT64 size;
        time_t creationTime;
        time_t modificationTime;
        StgMode accessMode;
        SSRW_CLSID clsid;
    }
    StgInfo;

    /* Prototype for a function pointer matching the standard C library's realloc
     * function definition, needed for creating structured storage files in buffers 
     */
    typedef void* (*SsrwReallocFunc)(
        void* in_ptr,
        size_t in_size);

    /* RootStorage object functions available for caller programs */

    extern _OLESS_API int openStructuredStorage (
            const char* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern _OLESS_API int openStructuredStorageEx (
            const wchar_t* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern _OLESS_API int openStructuredStorageInBuf (
            const void** in_ppBuf,
            unsigned long in_ulBufSize,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern _OLESS_API int openStructuredStorageInSource (
            SSRWIS* in_pSSSRWIS,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern _OLESS_API int getStorageFromRoot(
            RootStorage* in_pRoot,
            Storage** out_ppStorage);

    extern _OLESS_API int closeStructuredStorage (
            RootStorage** in_pRoot);

    extern _OLESS_API int structuredStorageGetSectorSize(
            const RootStorage* in_pRoot,
            unsigned long* out_pSectorSize);

#ifdef SSTG_PALMOS

    extern _OLESS_API int openStructuredStorageInPalmStream (
            FileHand in_file,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern _OLESS_API int openStructuredStorageInPalmVFSStream (
            FileRef in_file,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern _OLESS_API int openStructuredStorageInVFS (
            const char* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned short in_volRef);

#else /* !SSTG_PALMOS */

    extern _OLESS_API int openStructuredStorageInStream (
            FILE* in_pFile,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

#endif /* !SSTG_PALMOS */

    /* Storage object funcions available for caller programs*/
    extern _OLESS_API int openStorage (
            Storage* in_this,
            const wchar_t* in_name,
            Storage** out_ppChild);

    extern _OLESS_API int openStream (
            Storage* in_this,
            const wchar_t* in_name,
            Stream** out_ppChild);

    extern _OLESS_API int closeStorage (
            Storage** io_pThis);

    extern _OLESS_API int getStorageInfo(
            Storage* in_this,
            StgInfo** out_ppInfo );

	extern _OLESS_API int countChildren(
			Storage* in_this,
			int* out_pCount);

    extern _OLESS_API int getChildrenInfo(
            Storage* in_this,
            StgInfo** out_paInfo,
            int* out_pCount );

    extern _OLESS_API int destroy(
            Storage* io_pParent,
            const wchar_t* in_pName);

    extern _OLESS_API int streamSetClass(
            Stream* io_this,
            const SSRW_CLSID* in_pCLSID);

    extern _OLESS_API int storageSetClass(
            Storage* io_this,
            const SSRW_CLSID* in_pCLSID);

    /* Stream  object functions available for caller programs*/
    extern _OLESS_API int streamSeek (
            Stream* in_this,
            long in_offset,
            SeekPos in_pos);

    extern _OLESS_API int streamGetPos (
            Stream* in_this,
            unsigned long* out_offset );

    extern _OLESS_API int streamGetBytesAvailableToRead(
            Stream* in_pStream,
            unsigned long* out_pOffsetFromEnd);

    extern _OLESS_API int streamSeek64 (
            Stream* in_this,
            SSRW_INT64 in_offset,
            SeekPos in_pos);

    extern _OLESS_API int streamGetPos64 (
            Stream* in_this,
            unsigned SSRW_INT64* out_offset);

    extern _OLESS_API int streamGetBytesAvailableToRead64(
            Stream* in_pStream,
            unsigned SSRW_INT64* out_pOffsetFromEnd);

    extern _OLESS_API int streamRead (
            Stream* in_this,
            void* out_buffer,
            unsigned int * in_pCount);

    extern _OLESS_API int readLEshort(
            Stream* in_pStream,
            short* out_psVal);

    extern _OLESS_API int readLEword(
            Stream* in_pStream,
            unsigned short* out_pwVal);

    extern _OLESS_API int readLElong(
            Stream* in_pStream,
            int* out_plVal );

    extern _OLESS_API int readLEdword(
            Stream* in_pStream,
            unsigned int* out_pdwVal);

    extern _OLESS_API int readLEfloat(
            Stream* in_pStream,
            float* out_pfVal);

    extern _OLESS_API int readLEdouble(
            Stream* in_pStream,
            double* out_pdVal);

    extern _OLESS_API int readLEwchar(
            Stream* in_pStream,
            wchar_t* out_pwcVal);

    extern _OLESS_API int readLEwstring(
            Stream* in_pStream,
            int* io_piCount,
            wchar_t* out_strString);

    extern _OLESS_API int closeStream(
            Stream** io_ppStream);

    extern _OLESS_API int getStreamInfo(
            Stream* in_pStream,
            StgInfo** out_ppInfo);

    /* StgInfo object functions */
    extern _OLESS_API int freeInfo(
            StgInfo** io_paInfo,
            int in_count);

    /* SummaryInfo and DocumentSummaryInfo properties management */
    extern _OLESS_API int getDocumentProperty(
            RootStorage* pRoot,
            PropertyStreamType streamId,
            int propertyId,
            TypeTag* pType,
            void** ppValue );

    extern _OLESS_API int freeDocumentProperty(
            void** ppValue);

#ifndef SSRW_READ_ONLY

    extern _OLESS_API int createStructuredStorage (
            const char* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern _OLESS_API int createStructuredStorageEx (
            const wchar_t* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned long in_sectorSize);

    extern _OLESS_API int createStructuredStorageInBuf (
            void** in_ppBuf,
            unsigned long in_ulBufSize,
            SsrwReallocFunc in_pRealloc,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern _OLESS_API int createStructuredStorageInBufEx (
            void** in_ppBuf,
            unsigned long in_ulBufSize,
            SsrwReallocFunc in_pRealloc,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned long in_sectorSize);

    extern _OLESS_API int createStructuredStorageInSource (
            SSRWIS* in_pSSRWIS,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned int in_sectorSize);

#ifdef SSTG_PALMOS

    extern _OLESS_API int createStructuredStorageInPalmStream (
            FileHand in_file,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern _OLESS_API int createStructuredStorageInPalmStreamEx (
            FileHand in_file,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned long in_sectorSize);

    extern _OLESS_API int createStructuredStorageInPalmVFSStream (
            FileRef in_file,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern _OLESS_API int createStructuredStorageInPalmVFSStreamEx (
            FileRef in_file,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned long in_sectorSize);

    extern _OLESS_API int createStructuredStorageInVFS (
            const char* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned short in_volRef);

    extern _OLESS_API int createStructuredStorageInVFSEx (
            const char* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned short in_volRef,
            unsigned long in_sectorSize);

#else /* !SSTG_PALMOS */

    extern _OLESS_API int createStructuredStorageInStream (
            FILE* in_pFile,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern _OLESS_API int createStructuredStorageInStreamEx (
            FILE* in_pFile,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned long in_sectorSize);

#endif /* !SSTG_PALMOS */

    extern _OLESS_API int flushStream(Storage* in_pStorage,
                           const wchar_t* in_wszStreamName,
                           const void* in_pvBuf,
                           unsigned long* in_pcbBufSize);

    extern _OLESS_API int createStorage (
            Storage* in_this,
            const wchar_t* in_name,
            Storage** out_ppChild);

    extern _OLESS_API int createStream (
            Storage* in_this,
            const wchar_t* in_name,
            Stream** out_ppChild);

    extern _OLESS_API int streamResize(
            Stream* in_pStream,
            SSRW_INT64 in_llNewSize); 

    extern _OLESS_API int streamWrite(
            Stream* in_pStream,
            const void* in_pBuffer,
            unsigned int * io_pCount);

    extern _OLESS_API int writeLEshort(
            Stream* in_pStream,
            short out_val);

    extern _OLESS_API int writeLEword(
            Stream* in_pStream,
            unsigned short out_val);

    extern _OLESS_API int writeLElong(
            Stream* in_pStream,
            int in_val);

    extern _OLESS_API int writeLEdword(
            Stream* in_pStream,
            unsigned int in_val);

    extern _OLESS_API int writeLEfloat(
            Stream* in_pStream,
            float in_val);

    extern _OLESS_API int writeLEdouble(
            Stream* in_pStream,
            double in_val);

    extern _OLESS_API int writeLEwchar(
            Stream* in_pStream,
            wchar_t in_val);

    extern _OLESS_API int writeLEwstring(
            Stream* in_pStream,
            const wchar_t* in_wszString,
            int* io_piCount);

    extern _OLESS_API int setDocumentProperty(
            RootStorage* pRoot,
            PropertyStreamType streamId,
            int propertyId,
            TypeTag srcType,
            const void* pValue);

    extern _OLESS_API int setDefaultDocumentProperties(
            RootStorage* pRoot);

#endif /* !SSRW_READ_ONLY */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
