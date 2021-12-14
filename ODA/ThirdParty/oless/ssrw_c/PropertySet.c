/************************************************************************
* PropertySet.c
*
* PropertySet structure and function definitions
*
* (c) Schema Software Inc., 2001 - 2004
* 
*************************************************************************

$Revision: 1.13.2.1 $
$Date: 2004/02/24 19:09:25 $
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "StructuredStorage.h"
#include "RootStorage.h"
#include "Header.h"
#include "Directory.h"
#include "Stream.h"
#include "LEReadWrite.h"
#include "PropertySet.h"

#define PROPERTY_OS_VERSION 0x0204 /* as in all Microsaoft-produced files */
#define MAX_PROPERTY_NUM 20
#define PROPERTY_STREAM_HEADER_SIZE 0x030
#define FMTID_SIZE 16
#define SECTION_HEADER_SIZE 8

#ifndef _WIN32_WCE
typedef enum
{
    VT_EMPTY = 0,
    VT_NULL = 1,
    VT_I1 = 16,
    VT_UI1 = 17,
    VT_I2 = 2,
    VT_UI2 = 18,
    VT_I4 = 3,
    VT_INT = 22,
    VT_UI4 = 19,
    VT_UINT = 23,
    VT_I8 = 20,
    VT_UI8 = 21,
    VT_R4 = 4,
    VT_R8 = 5,
    VT_CY = 6,
    VT_DATE = 7,
    VT_BSTR = 8,
    VT_BOOL = 11,
    VT_ERROR = 10,
    VT_FILETIME = 64,
    VT_LPSTR = 30,
    VT_LPWSTR = 31,
    VT_CLSID = 72,
    VT_CF = 71,
    VT_BLOB = 65,
    VT_BLOBOBJECT = 70,
    VT_STREAM = 66,
    VT_STREAMED_OBJECT = 68,
    VT_STORAGE = 67,
    VT_STORED_OBJECT = 69,
    VT_DECIMAL = 14,
    VT_VECTOR = 0x1000,
    VT_ARRAY = 0x2000,
    VT_BYREF = 0x4000,
    VT_VARIANT = 12,
    VT_TYPEMASK = 0xFFF
} VT_type;
#else
#include <wtypes.h>
typedef enum VARENUM VT_type;
#endif



/* Internal record describing one property:
 * id + offset in the file for type-value pair (on input) or 
 * pointer to the value (on output) */
typedef struct _PropertyRecord
{
    UINT4 propertyId;
    UINT4 offset;
    UINT4 len;
    void* pValue;
}
PropertyRecord;

/* Table containing max 20 property records
 * and the count of really set/read properties */
typedef struct _PropertyTable
{
    PropertyRecord table[MAX_PROPERTY_NUM];
    UINT4 count;
}
PropertyTable;

/* Property Set description structure */
struct _PropertySet
{
    PropertyTable propTable;
    PropertyStreamType id;
    Stream* pStream;
    UINT4 sectionOffset;
    StgMode mode;
};

/* Internal functions - forwad declaration */
static SINT4 propertySetOffsetById(PropertySet* in_pSet, UINT4 in_id, UINT4* out_pOffset);
static VT_type typeof_Property(PropertyStreamType strId, int propId);
static SINT4 propertySetAddProperty(PropertySet* in_pSet, SINT4 in_id);
static SINT4 propertySetPutValue(PropertySet* in_pSet, 
                                 UINT4 in_id, 
                                 void* in_buffer, 
                                 UINT4 in_ulLen);
static UINT4 propertySetSectionLength(PropertySet* in_pSet);
static UINT4 propertySetValueLength(PropertySet* in_pSet, UINT4 index);
static SINT4 propertySetReadHeader(Stream* in_pStream, UINT4* out_pSectionOffset);
static SINT4 propertySetReadClipboardFormat(PropertySet* in_pSet, 
                                            UINT4* out_pulLen, 
                                            void** out_ppValue);
static SINT4 propertySetLoadValue(PropertySet* in_pSet,
                                  SINT4 in_id,
                                  UINT4* out_pulLen,
                                  void** out_ppValue);
static SINT4 propertySetSaveValue(SINT4 in_id,
                                  VT_type in_type,
                                  const void* in_pValue,
                                  void* out_pBuf,
                                  UINT4* out_pulLen);
static SINT4 propertySetCompress(PropertySet* in_pSet);
static TypeTag propertySetTypeFromVariantType(UINT4 in_vt);

/* Constant - property sections format identifiers */
#define FMTID_SUMMARY_INFO     "\xe0\x85\x9f\xf2\xf9\x4f\x68\x10\xab\x91\x08\x00\x2b\x27\xb3\xd9"
#define FMTID_DOC_SUMMARY_INFO "\x02\xd5\xcd\xd5\x9c\x2e\x1b\x10\x93\x97\x08\x00\x2b\x2c\xf9\xae"
#define FMTID_USER_PROPERTIES  "\x05\xd5\xcd\xd5\x9c\x2e\x1b\x10\x93\x97\x08\x00\x2b\x2c\xf9\xae"


/*============================================================================
  Description:  Allocates, initializes and returns new PropertySet structure
  Arguments: 
   out_ppSet - handle to the allocated strucure
  Return:
   Status code
  ==========================================================================*/
SINT4
propertySetConstruct(PropertyStreamType in_id, StgMode in_mode, PropertySet** out_ppSet)
{
    *out_ppSet = (PropertySet*)malloc(sizeof(PropertySet));
    if (*out_ppSet == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset( *out_ppSet, 0, sizeof(PropertySet));

    (*out_ppSet)->propTable.count = 0;
    (*out_ppSet)->pStream = NULL;
    (*out_ppSet)->id = in_id;
    (*out_ppSet)->mode = in_mode;

    return SSTG_OK;
}


/*============================================================================
  Description:  cleans up, nullifies, etc.
  Arguments: 
   out_ppSet - handle to the allocated strucure
  Return:
   Status code
  ==========================================================================*/
SINT4
propertySetDestroy(PropertySet** in_ppSet)
{
    UINT4           i =    0;
    PropertyRecord* pRec = NULL;

    for (i = 0; i < (*in_ppSet)->propTable.count; i++)
    {
        pRec = &((*in_ppSet)->propTable.table[i]);
        if ((pRec->pValue) != NULL)
        {
            free(pRec->pValue);
            pRec->pValue = NULL;
        }
    }

    free(*in_ppSet);
    *in_ppSet = NULL;

    return SSTG_OK;
}


/*============================================================================
  Description:  Reads in the property table - necessary to find
                properties in the stream
  Arguments: 
   in_pSet - set to read
   in_pStream - sream to read from
  Return:
   Status code
  Note: the function changes the status of propertySet to be "read"
   by assigning non-null value to ist pStream member
 ==========================================================================*/
SINT4
propertySetRead(PropertySet* in_pSet, Stream* in_pStream)
{
    SINT4           iRet =          SSTG_OK;
    SINT4           sectionSize =   0;
    SINT4           propertyCount = 0;
    int             i =             0;
    UINT4*          pOffset =       NULL;
    UINT4*          pId =           NULL;
    SINT4            lTmp =          0;
    UINT4   ulTmp =         0;
    SINT8           llSeekLoc =     0;

    /* Read in and set the offset for the section that contains property table */
    iRet = propertySetReadHeader(in_pStream, &(in_pSet->sectionOffset));
    if (iRet != SSTG_OK)
    {
        in_pSet->pStream = NULL;
        return iRet;
    }
    /* store the stream pointer */
    in_pSet->pStream = in_pStream;

    /* read in size of the section and number of properties in it */
    streamSeek(in_pStream, in_pSet->sectionOffset, STG_START);
    iRet = readLElong(in_pStream, &lTmp);
    sectionSize = (SINT4)lTmp;
    iRet = readLElong(in_pStream, &lTmp);
    propertyCount = (SINT4)lTmp;

    /* Too many properties - no suport in the current version */
    if (propertyCount > MAX_PROPERTY_NUM)
    {
        return SSTG_ERROR_NOT_SUPPORTED;
    }
    in_pSet->propTable.count = propertyCount;

    for (i = 0; i < propertyCount; i++)
    {
        pId = &(in_pSet->propTable.table[i].propertyId);
        iRet = readLEdword(in_pStream, &ulTmp);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
        *pId = (UINT4)ulTmp;

        pOffset = &(in_pSet->propTable.table[i].offset);
        iRet = readLElong(in_pStream, &lTmp);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
        *pOffset = (UINT4)lTmp;

        iRet = streamGetPos64(in_pStream, (UINT8*)&llSeekLoc);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }

        iRet = propertySetLoadValue(in_pSet, 
                                    *pId, 
                                    &(in_pSet->propTable.table[i].len),
                                    &(in_pSet->propTable.table[i].pValue));
        /* Ignore errors--pValue will be set to NULL and len to 0 */

        iRet = streamSeek64(in_pStream, llSeekLoc, STG_START);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    return SSTG_OK;
}


/*============================================================================
  Description:  Reads header of the property sream and returns the offset
                where the property table starts
  Arguments: 
   io_pStream - stream to read
   out_pSectionOffset - offest of the property table
  Return:
   Status code
  Note:  The only thing we use from there is SectionsOffset. We can skip
   all the rest.
  ==========================================================================*/
static SINT4
propertySetReadHeader(Stream* in_pStream, UINT4* out_pSectionOffset)
{
    SINT2           byteOrder =         0;
    SINT2           formatVersion =     0;
    SINT4           osVersion =         0;
    SINT4           reserved =          0;
    SINT4           iRet =              SSTG_OK;
    UINT4   len =               0;
    SINT4            lTmp =              0;
    UINT4   ulTmp =             0;
    SSRW_CLSID      clsid;
    CHAR            fmtid[FMTID_SIZE];

    iRet = readLEshort(in_pStream, &byteOrder);
    iRet = readLEshort(in_pStream, &formatVersion);
    iRet = readLElong(in_pStream, &lTmp);
    osVersion = (SINT4)lTmp;
    iRet = readLEdword(in_pStream, &(clsid.data1));
    iRet = readLEword(in_pStream, &(clsid.data2));
    iRet = readLEword(in_pStream, &(clsid.data3));
    len = sizeof(clsid.data4);
    iRet = streamRead(in_pStream, clsid.data4, &len);
    iRet = readLElong(in_pStream, &lTmp);
    reserved = (SINT4)lTmp;

    len = FMTID_SIZE;
    iRet = streamRead (in_pStream, fmtid, &len);
    iRet = readLEdword(in_pStream, &ulTmp);
    *out_pSectionOffset = (UINT4)ulTmp;

    return iRet;
}


/*============================================================================
  Description:  returns type and value of a requested property. The space for 
                the value is allocated in this function. Necessary conversion 
                are made.
  Arguments: 
   in_pSet - set from which to read
   in_id - id of the property to look for
   out_pType   - pointer to the enumerated value for the type 
   out_pulLen -  pointer to the length of the buffer required for the property
   out_ppValue - handle to the value. The value will be converted
  Return:
   Status code
  Pre-condition: PropertySet should be initialized, i.e. stream should be
   open and the table is read
 ==========================================================================*/
static SINT4
propertySetLoadValue(PropertySet* in_pSet,
                     SINT4 in_id,
                     UINT4* out_pulLen,
                     void** out_ppValue)
{
    SINT4           iRet =      SSTG_OK;
    UINT4           ulType =    0;
    SINT4            lTmp =      0;
    short           sTmp =      0;
    UINT4   ulTmp =     0;
    UINT4           offset;
    UINT4           len;
    WinTime         winTime;

    if (in_pSet->pStream == NULL)
    {
        /* Property is not read in yet! */
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    iRet = propertySetOffsetById(in_pSet, in_id, &offset);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    streamSeek(in_pSet->pStream, in_pSet->sectionOffset + offset, STG_START);
    /* read in the property type */
    iRet = readLEdword(in_pSet->pStream, &ulTmp);
    ulType = (UINT4)ulTmp;
    *out_ppValue = NULL;

    switch (ulType)
    {
    case VT_I2:
        *out_pulLen = sizeof(SINT2);
        *out_ppValue = malloc(sizeof(SINT2));
        if (*out_ppValue == NULL)
        {
            return SSTG_ERROR_MEMORY;
        }
        iRet = readLEshort(in_pSet->pStream, &sTmp);
        *((SINT2*)(*out_ppValue)) = (SINT2)sTmp;
        break;
    case VT_INT:
    case VT_I4:
        *out_pulLen = sizeof(SINT4);
        *out_ppValue = malloc(sizeof(SINT4));
        if (*out_ppValue == NULL)
        {
            return SSTG_ERROR_MEMORY;
        }
        iRet = readLElong(in_pSet->pStream, &lTmp);
        *((SINT4*)(*out_ppValue)) = (SINT4)lTmp;
        break;
    case VT_LPSTR:
        /* This is a"narrow"  OLE string: the length in front (which includes the
         * null terminator), trailing zero at the end, 8-bit chars */
        iRet = readLEdword(in_pSet->pStream, &ulTmp);
        len = (UINT4)ulTmp;
        if (iRet != SSTG_OK)
            break;
        if (len <= 0)
        {
            iRet = SSTG_ERROR_PROPERTY_FORMAT;
            break;
        }
        *out_pulLen = len;
        *out_ppValue = malloc(len);
        if (*out_ppValue == NULL)
        {
            return SSTG_ERROR_MEMORY;
        }

        memset(*out_ppValue, 0, len);

        ulTmp = (unsigned long)len;
        iRet = streamRead(in_pSet->pStream, (SINT4*)(*out_ppValue), &ulTmp);

        break;
    case VT_FILETIME:
        /* this is an 8 byte FILETIME Win32 structure where the time
         is stored in UTC format */
        *out_pulLen = sizeof(time_t);
        *out_ppValue = (time_t *)malloc(sizeof(time_t));
        if (*out_ppValue == NULL)
        {
            return SSTG_ERROR_MEMORY;
        }

        memset( *out_ppValue, 0, sizeof(time_t));

        iRet = readLEdword(in_pSet->pStream, &ulTmp);
        if (iRet != SSTG_OK)
        {
            break;
        }
        winTime.ulLowDateTime = (UINT4)ulTmp;
        iRet = readLEdword(in_pSet->pStream, &ulTmp);
        if (iRet != SSTG_OK)
        {
            break;
        }
        winTime.ulHighDateTime = (UINT4)ulTmp;

        time_tFromWindowsTime(&winTime, (time_t*)(*out_ppValue) );
        break;

    case VT_CF:
        /* Clipboard format case  - the support is necessary for preview files */
        iRet = propertySetReadClipboardFormat(in_pSet, out_pulLen, out_ppValue);
        break;

    case VT_BOOL:
        *out_pulLen = sizeof(SINT2);
        *out_ppValue = malloc(sizeof(SINT2));
        if (*out_ppValue == NULL)
        {
            return SSTG_ERROR_MEMORY;
        }
        iRet = readLEshort(in_pSet->pStream, &sTmp);
        *((SINT2*)(*out_ppValue)) = (SINT2)sTmp;
        break;
    default:
        *out_pulLen = 0;
        *out_ppValue = NULL;
        iRet = SSTG_ERROR_NOT_SUPPORTED;
        break;
    }

    /* release the memory on error */
    if (iRet != SSTG_OK)
    {
        if (*out_ppValue != NULL)
        {
            free(*out_ppValue);
            *out_ppValue = NULL;
        }
        *out_pulLen = 0;
    }

    return iRet;
}


/*============================================================================
  Description:  Retrieves a value from the loaded property set
  Arguments: 
   in_pSet -     set from which to read
   in_id -       id of the property to look for
   out_pType   - pointer to the enumerated value for the type 
   out_ppValue - handle to the value. The value will be converted
  Return:       
   Status code
  Note: the function changes the status of propertySet to be "read"
   by assigning non-null value to ist pStream member
 ==========================================================================*/
SINT4 
propertySetGet (PropertySet* in_pSet,
                SINT4 in_id,
                TypeTag* out_pType,
                void** out_ppValue)
{
    SINT4           id =   0;
    UINT4           i =    0;
    PropertyRecord* pRec = NULL;
    UINT4           vt =   VT_I2;

    for (i = 0; i < in_pSet->propTable.count; i++)
    {
        pRec = &(in_pSet->propTable.table[i]);
        id = pRec->propertyId;
        if (id == in_id)
        {
            if ((pRec->len == 0) || (pRec->pValue == NULL))
            {
                return SSTG_ERROR_PROPERTY_NOT_FOUND;
            }

            *out_ppValue = malloc(pRec->len);
            if (*out_ppValue == NULL)
            {
                return SSTG_ERROR_MEMORY;
            }

            memcpy(*out_ppValue, pRec->pValue, pRec->len);
            vt = typeof_Property(in_pSet->id, pRec->propertyId);
            *out_pType = propertySetTypeFromVariantType(vt);
                    
            return SSTG_OK;
        }
    }

    return SSTG_ERROR_PROPERTY_NOT_FOUND;
}


/*============================================================================
  Description:  Auxiliary - reads in clipboard header and determines 
                clipboard formats and header length
  Arguments: 
   in_pSet -     set to read
   out_p
   out_ppValue - on success, points to the clipboard format data buffer
  Return:       
   Status code
  Note: the function changes the status of propertySet to be "read"
   by assigning non-null value to ist pStream member
 ==========================================================================*/
static SINT4
propertySetReadClipboardFormat(PropertySet* in_pSet, 
                               UINT4*  out_pulLen,
                               void ** out_ppValue)
{
    SINT4           iRet =      SSTG_OK;
    UINT4   len =       0;
    unsigned long   iPos;

    /* Read in clipboard data length */
    iRet = streamGetPos(in_pSet->pStream, &iPos);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    iRet = readLEdword(in_pSet->pStream, &len);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    if (len <= 0)
    {
        return  SSTG_ERROR_PROPERTY_FORMAT;
    }

    /* return back to the beginning of the property */
    iRet = streamSeek(in_pSet->pStream, iPos, STG_START);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    /* allocate space for the data */
    *out_pulLen = len + sizeof(UINT4);
    *out_ppValue = malloc(*out_pulLen);
    if (*out_ppValue == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(*out_ppValue, 0, *out_pulLen);

    iRet = streamRead(in_pSet->pStream, *out_ppValue, out_pulLen);

    return iRet;
}

/*============================================================================
  Description:  Computes combined size necessary to write property values and 
                types
  Arguments: 
   in_pSet - set to add to
  Return:  
   the size
 ==========================================================================*/
static UINT4
propertySetSectionLength(PropertySet* in_pSet)
{
    unsigned int len = 0;
    unsigned int i;

    /* For every property, we keep id, offset and type tag
    - all 4 bytes long */
    len = in_pSet->propTable.count * 3 * sizeof(SINT4);

    /* Section has header - it contains section length (4 bytes)
    and number of properties (4 bytes) */
    len += (sizeof(SINT4) * 2);

    /* Length needed for property values */
    for (i = 0; i < in_pSet->propTable.count; i++)
    {
        len += propertySetValueLength(in_pSet, i);
    }

    return len;
}


/*============================================================================
  Description:  Converts a variant type tag to a property set type tag as per
                the structured storage library's API
  Arguments: 
   in_vt - variant type
  Return:
   Status code
 ==========================================================================*/
static TypeTag 
propertySetTypeFromVariantType(UINT4 in_vt)
{
    TypeTag type = INT4_TYPE;
    switch(in_vt)
    {
    case VT_I2:
        type = INT2_TYPE;
        break;
    case VT_I4:
        type = INT4_TYPE;
        break;
    case VT_CF:
        type = CF_TYPE;
        break;
    case VT_BOOL:
        type = BOOL_TYPE;
        break;
    case VT_LPSTR:
        type = STRING_TYPE;
        break;
    case VT_FILETIME:
        type = TIME_TYPE;
        break;
    default:
        ASSERT(0);
        type = (TypeTag)-1;
        break;
    }

    return type;
}

/*============================================================================
  Description:  Returns the length of the i-th value in the property set
  Arguments: 
   in_pSet - set to look up
   index - property index in the set
  Return:
   Status code
 ==========================================================================*/
static UINT4
propertySetValueLength(PropertySet* in_pSet, UINT4 index)
{
    unsigned int  len =         0;
    SINT4         id =          0;
    VT_type       propType;
    char*         pstr =        0;

    id = in_pSet->propTable.table[index].propertyId;
    propType = typeof_Property(in_pSet->id, id);
    switch (propType)
    {
    case VT_I2:
        len = 2;
        break;
    case VT_I4:
        len = 4;
        break;
    case VT_FILETIME:
        len = 8;
        break;
    case VT_LPSTR:
        /* Note: we compensate for the fact that there is a DWORD
        at the beginning of the string */
        pstr = (char*)(in_pSet->propTable.table[index].pValue);
        len = (strlen(pstr) + 1) + sizeof(SINT4);
        break;
    case VT_BOOL:
        len = 2;
        break;
    case VT_CF:
        len = leReadUnsignedLong(in_pSet->propTable.table[index].pValue) + sizeof(UINT4);
        break;
    default:
        /* we will not put in anything else*/
        break;

    }
    ASSERT(len > 0);

    return len;
}


/*============================================================================
  Description:  Auxiliary: returns Offset in the section for given property Id
  Arguments: 
   in_pSet - set from which to read
   in_id - id of the property to look for
   out_pOffset - pointer to the offset value
   out_ppValue - handle to the value. The value will be converted
  Return:
   Status code
 ==========================================================================*/
static SINT4 propertySetOffsetById(PropertySet* in_pSet, UINT4 in_id, UINT4* out_pOffset)
{
    UINT4           id;
    unsigned int    i;

    for (i = 0; i < in_pSet->propTable.count; i++)
    {
        id = in_pSet->propTable.table[i].propertyId;
        if (id == in_id)
        {
            *out_pOffset = in_pSet->propTable.table[i].offset;
            return SSTG_OK;
        }
    }

    return SSTG_ERROR_PROPERTY_NOT_FOUND;
}


/*============================================================================
  Description:  Auxiliary: returns a VT_Type of the property Id
  Arguments: 
   in_pSet - set from which to read
   in_id - id of the property to look for
   out_pOffset - pointer to the offset value
   out_ppValue - handle to the value. The value will be converted
  Return:
   Status code
 ==========================================================================*/
static VT_type
typeof_Property(PropertyStreamType strId, int propId)
{
    if (strId == SUMMARY_INFO)
    {
        switch (propId)
        {
        case PIDSI_TITLE:           /* Title */
        case PIDSI_SUBJECT:   /* Subject */
        case PIDSI_AUTHOR:       /* Author */
        case PIDSI_KEYWORDS:  /* Keywords */
        case PIDSI_COMMENTS:  /* Comments */
        case PIDSI_TEMPLATE:  /* Template */
        case PIDSI_LASTAUTHOR:   /* Last Saved By */
        case PIDSI_REVNUMBER:    /* Revision Number*/
        case PIDSI_APPNAME:
            return VT_LPSTR;
        case PIDSI_EDITTIME:  /* Total Editing Time */
        case PIDSI_LASTPRINTED:  /* Last Printed */
        case PIDSI_CREATE_DTM:   /* Create Time/Date */
        case PIDSI_LASTSAVE_DTM: /* Last saved Time/Date */
            return VT_FILETIME;
        case PIDSI_PAGECOUNT:
        case PIDSI_WORDCOUNT:
        case PIDSI_CHARCOUNT:
        case PIDSI_SECURITY:
            return VT_I4;
        case PIDSI_THUMBNAIL:
            return VT_CF;
        }
    }
    else
    {
        switch (propId)
        {
        case PIDDSI_CATEGORY:
        case PIDDSI_PRESFORMAT:
        case PIDDSI_MANAGER:
        case PIDDSI_COMPANY:
            return VT_LPSTR;
        case PIDDSI_BYTECOUNT:
        case PIDDSI_LINECOUNT:
        case PIDDSI_PARCOUNT:
        case PIDDSI_SLIDECOUNT:
        case PIDDSI_NOTECOUNT:
        case PIDDSI_HIDDENCOUNT:
        case PIDDSI_MMCLIPCOUNT:
            return VT_I4;
        case PIDDSI_SCALE:
        case PIDDSI_LINKSDIRTY:
            return VT_BOOL;
        default:
            return VT_ERROR;
        }
    }

    return VT_ERROR;
}

/*============================================================================
  Description:  Compresses a property table so that properties for which 
                there are errors, or which were not understood, are removed.
  Arguments: 
   io_pSet -    property set to modify
  Return:       
   Status code
 ==========================================================================*/
static SINT4 
propertySetCompress(PropertySet* in_pSet)
{
    UINT4           i =    0;
    UINT4           j =    0;
    PropertyRecord* pRec = NULL;
    VT_type         type = VT_ERROR;

    for (i = 0; i < in_pSet->propTable.count; i++)
    {
        pRec = &(in_pSet->propTable.table[i]);

        type = typeof_Property(in_pSet->id, pRec->propertyId);
        if ((pRec->len == 0) || (pRec->pValue == NULL) || (type == VT_ERROR))
        {
            if (pRec->pValue != NULL)
            {
                free(pRec->pValue);
            }
            for (j = i; j < in_pSet->propTable.count - 1; j++)
            {
                in_pSet->propTable.table[j] = in_pSet->propTable.table[j + 1];
            }
            (in_pSet->propTable.count)--;
            i--;
        }
    }

    return SSTG_OK;
}


/*============================================================================
  Description:  Writes in the property table - necessary to find
                properties in the stream
  Arguments: 
   in_pSet -    set to read
   in_pStream - sream to read from
  Return:       
   Status code
  Note: the function changes the status of propertySet to be "read"
   by assigning non-null value to ist pStream member
 ==========================================================================*/
SINT4
propertySetWrite(PropertySet* in_pSet, Stream* in_pStream)
{
    SINT4           iRet;
    UINT4           sectionSize, headerSize;
    SINT4           propertyCount =             0;
    int             i;
    UINT4           offset;
    SINT4           id;
    BYTE*           buffer;
    BYTE*           ptr;
    VT_type         propType;
    PropertyRecord* pRec =                      NULL;
    UINT4   len;
    UINT2           byteOrder =                 SSTG_BYTE_ORDER;
    UINT2           formatVersion =             0;  /* always 0 */
    UINT4           osVersion =                 PROPERTY_OS_VERSION;
    UINT4 reserved = 1; /* (MarkA) This variable used to be initialized to 0.
                         * Setting it to 1 caused certain properties, such as
                         * the document author, to appear under Windows.  We're
                         * not sure why. */
    BYTE*           fmtid;
    SSRW_CLSID      clsid =                 {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0} };
    UINT4   ulTmp =                     0;

    if (in_pStream == NULL)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }
    in_pSet->pStream = in_pStream;
    iRet = streamSeek(in_pSet->pStream, 0, STG_START);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* Remove invalid entries from the table */
    iRet = propertySetCompress(in_pSet);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    propertyCount = in_pSet->propTable.count;

    /* First, get the section length (we support only one section per stream fo now) */
    sectionSize = propertySetSectionLength(in_pSet);

    /* calculate header length - it depends on the type of the stream */
    headerSize = in_pSet->id == SUMMARY_INFO ?
                 PROPERTY_STREAM_HEADER_SIZE : PROPERTY_STREAM_HEADER_SIZE + FMTID_SIZE + sizeof(SINT4);

    /* Write header to the buffer */
    iRet = writeLEshort(in_pStream, byteOrder);     /* Byte order */
    iRet = writeLEshort(in_pStream, formatVersion);
    iRet = writeLElong(in_pStream, osVersion);
    len = (unsigned long)CLSID_SIZE;
    iRet = writeLEdword(in_pStream, clsid.data1);
    iRet = writeLEword(in_pStream, clsid.data2);
    iRet = writeLEword(in_pStream, clsid.data3);
    len = sizeof(clsid.data4);
    iRet = streamWrite(in_pStream, clsid.data4, &len);
    iRet = writeLElong(in_pStream, reserved);
    fmtid = (in_pSet->id == SUMMARY_INFO) ?
            (BYTE*)FMTID_SUMMARY_INFO :
            (BYTE*)FMTID_DOC_SUMMARY_INFO;
    len = (unsigned long)FMTID_SIZE;
    iRet = streamWrite(in_pStream, fmtid, &len);
    iRet = writeLEdword(in_pStream, headerSize); /* section offset equals header size ! */

    /* Fo Document summary, write down format Id and offset for the second section */
    if (in_pSet->id == DOC_SUMMARY_INFO)
    {
        len = (unsigned long)FMTID_SIZE;
        iRet = streamWrite(in_pStream, FMTID_USER_PROPERTIES, &len);
        iRet = writeLElong(in_pStream, headerSize + sectionSize);
    }

    /* Write down section header */
    iRet = writeLElong(in_pStream, sectionSize);
    iRet = writeLElong(in_pStream, in_pSet->propTable.count);

    /* allocate memory for teh section buffer*/
    ptr = (BYTE*) malloc(sectionSize - SECTION_HEADER_SIZE);
    if (ptr == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(ptr, 0, sectionSize - SECTION_HEADER_SIZE);


    /* Write down property table, values in one pass - bu to the buffer */
    offset = propertyCount * 2 * sizeof(SINT4);
    buffer = ptr;
    for (i = 0; i < propertyCount; i++)
    {
        pRec = &(in_pSet->propTable.table[i]);

        /* Don't write properties that weren't understood on read */
        ASSERT ((pRec->len != 0) && (pRec->pValue != NULL));

        /* pair id - offset is written to the table */
        id = pRec->propertyId;
        leWriteLong(id, buffer);
        buffer += sizeof(SINT4);
        leWriteLong(offset + SECTION_HEADER_SIZE, buffer);
        buffer += sizeof(SINT4);

        /* Type tag is written at the offset */
        propType = typeof_Property(in_pSet->id, id);
        leWriteLong(propType, ptr + offset);
        offset += sizeof(SINT4);

        /* finally, the value is written */
        iRet = propertySetSaveValue(pRec->propertyId,
                                    propType,
                                    pRec->pValue,
                                    ptr + offset,
                                    &len);
        offset += (UINT4)len;
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    /* Now, write this buffer to the stream */
    sectionSize -= SECTION_HEADER_SIZE;
    ulTmp = (unsigned long)sectionSize;
    iRet = streamWrite(in_pStream, ptr, &ulTmp);
    free(ptr);

    return SSTG_OK;
}


/*============================================================================
  Description:  Saves an in-memory property value into a disk-format buffer.
  Arguments: 
   in_id -      id of the property to save
   in_type   -  the enumerated value for the data type 
   in_pValue -  pointer to the value to be converted and written
   out_pBuf -   buffer to write to
   out_pulLen - number of bytes written
  Return:
   Status code
 ==========================================================================*/
static SINT4
propertySetSaveValue(SINT4 in_id,
                     VT_type in_type,
                     const void* in_pValue,
                     void* out_pBuf,
                     UINT4* out_pulLen)
{
    SINT4               iRet =          SSTG_OK;
    UINT4               len =           0;
    SINT4               ldata =         0;
    SINT2               sdata =         0;
    UINT4               ulValLen =      0;
    const SINT4*        pldata =        NULL;
    const SINT2*        psdata =        NULL;
    const time_t*       ptdata =        NULL;
    WinTime             winTime;

    switch (in_type)
    {
    case VT_I2:
        psdata = (const SINT2*)in_pValue;
        sdata = *psdata;

        ulValLen = sizeof(SINT2);
        leWriteShort(sdata, out_pBuf);
        break;
    case VT_INT:
    case VT_I4:
        pldata = (const SINT4*)in_pValue;
        ldata = *pldata;

        ulValLen = sizeof(SINT4);
        leWriteLong(ldata, out_pBuf);
        break;
    case VT_LPSTR:
        /* This is a"narrow"  OLE string: the length in front, trailing zero at the end,
         8-bit chars */
        len = strlen((const char*)in_pValue);
        ulValLen = len + sizeof(SINT4) + 1;
        memset(out_pBuf, 0, ulValLen);

        leWriteLong(len + 1, out_pBuf);
        strcpy(((char*)out_pBuf) + sizeof(SINT4), (const char*)in_pValue);
        break;
    case VT_FILETIME:
        /* we get a time_t data and convert it
        to  an 8 byte FILETIME Win32 structure where the time
         is stored in UTC format */
        ptdata = (const time_t*)in_pValue;
        iRet = WindowsTimeFromTime_t(*ptdata, &winTime);
        if (iRet != SSTG_OK)
            break;

        ulValLen = sizeof(WinTime);
        leWriteLong(winTime.ulLowDateTime, out_pBuf);
        leWriteLong(winTime.ulHighDateTime, ((char*)out_pBuf) + sizeof(SINT4));
        break;
    case VT_CF:
        /* Clipboard file has to be already completely ready for saving -
        - we will just write it "as is"  - but we have to copy it, since
        this is the way this function works*/
        ulValLen = (UINT4)leReadUnsignedLong(in_pValue) + sizeof(UINT4);
        memcpy(out_pBuf, in_pValue, ulValLen);
        break;
    case VT_BOOL:
        psdata = (const SINT2*)in_pValue;
        sdata = *psdata;
        ulValLen = sizeof(SINT2);
        leWriteShort(sdata, out_pBuf);
        break;
    default:
        iRet = SSTG_ERROR_NOT_SUPPORTED;
        break;
    }

    if (iRet == SSTG_OK)
    {
        *out_pulLen = ulValLen;
    }

    return iRet;
}


/*============================================================================
  Description:  returns type and value of a requested property. The space for 
                the value is allocated in this function. Necessary conversion 
                are made.
  Arguments: 
   in_pSet - set from which to read
   in_id - id of the property to look for
   in_type   - the enumerated value for the type 
   in_pValue - pointer to the value to be converted and written
  Return:
   Status code
  Pre-condition: PropertySet should be initialized, i.e. stream should be
   open and the table is read
 ==========================================================================*/
SINT4
propertySetPut(PropertySet* in_pSet,
               SINT4 in_id,
               TypeTag in_type,
               const void* in_pValue)
{
    SINT4               iRet =          SSTG_OK;
    UINT4               offset;
    UINT4               ulValLen =      0;
    VT_type             targetType;
    BYTE*               buffer =        NULL;

    targetType = typeof_Property(in_pSet->id, in_id);
    if (targetType == VT_ERROR)
    {
        return SSTG_ERROR_NOT_SUPPORTED;
    }

    switch (targetType)
    {
    case VT_INT:
    case VT_I4:
        if (in_type != INT4_TYPE)
        {
            iRet = SSTG_ERROR_PROPERTY_FORMAT;
        }
        ASSERT(in_type == INT4_TYPE);
        ulValLen = 4;
        break;
    case VT_LPSTR:
        if (in_type != STRING_TYPE)
        {
            iRet = SSTG_ERROR_PROPERTY_FORMAT;
        }
        ulValLen = strlen((const char*)in_pValue) + 1;
        break;
    case VT_FILETIME:
        if (in_type != TIME_TYPE)
        {
            iRet =  SSTG_ERROR_PROPERTY_FORMAT;
        }
        ulValLen = sizeof(time_t);
        break;
    case VT_CF:
        if (in_type != CF_TYPE)
        {
            iRet =  SSTG_ERROR_PROPERTY_FORMAT;
        }
        ulValLen = (unsigned long)leReadUnsignedLong(in_pValue) + sizeof(UINT4);
        break;
    case VT_BOOL:
        if (in_type != BOOL_TYPE)
        {
            iRet = SSTG_ERROR_PROPERTY_FORMAT;
        }
        ulValLen = sizeof(SINT2);
        break;
    default:
        iRet = SSTG_ERROR_NOT_SUPPORTED;
        break;
    }

    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    buffer = (BYTE*)malloc(ulValLen);
    if (buffer == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memcpy(buffer, in_pValue, ulValLen);

    iRet = propertySetOffsetById(in_pSet, in_id, &offset);
    if (iRet != SSTG_OK)
    {
        iRet = propertySetAddProperty(in_pSet, in_id);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    iRet = propertySetPutValue(in_pSet, in_id, buffer, ulValLen);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    return iRet;
}


/*============================================================================
  Description:  Adds new property to the property set. Does not set the value.
  Arguments: 
   in_pSet - set to add to
   in_id - id of the property to add   
  Return:
   Status code
 ==========================================================================*/
static SINT4
propertySetAddProperty(PropertySet* in_pSet, SINT4 in_id)
{
    UINT4 i = in_pSet->propTable.count;

    if (i >= 20)
    {
        return SSTG_ERROR_NOT_SUPPORTED;
    }

    in_pSet->propTable.table[i].propertyId = in_id;
    in_pSet->propTable.table[i].pValue = NULL;
    (in_pSet->propTable.count)++;
    return SSTG_OK;
}


/*============================================================================
  Description:  Puts value for property - always for writing
  Arguments: 
   in_pSet - set to update
   in_id - id of the property to set value for
   in_buffer   - pointer to the  value
  Return:
   Status code
 ==========================================================================*/
static SINT4
propertySetPutValue(PropertySet* in_pSet, 
                    UINT4 in_id, 
                    void* in_buffer, 
                    UINT4 in_ulLen)
{
    UINT4           id =   0;
    unsigned int    i =    0;
    PropertyRecord* pRec = NULL;

    for (i = 0; i < in_pSet->propTable.count; i++)
    {
        pRec = &(in_pSet->propTable.table[i]);
        id = pRec->propertyId;
        if (id == in_id)
        {
            if (pRec->pValue != NULL)
            {
                free(pRec->pValue);
            }
            pRec->pValue = in_buffer;
            pRec->len = in_ulLen;

            return SSTG_OK;
        }
    }

    return SSTG_ERROR_PROPERTY_NOT_FOUND;
}

