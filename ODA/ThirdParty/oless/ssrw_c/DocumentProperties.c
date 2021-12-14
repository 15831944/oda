/************************************************************************
* DocumentProperties.c
*
* High-level structures and functions to manage 
* DocumentProperties
*
* (c) Schema Software Inc., 2001 - 2002
* 
*************************************************************************

$Revision: 1.11.2.1 $
$Date: 2004/02/24 19:09:25 $
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "StructuredStorage.h"
#include "RootStorage.h"
#include "Directory.h"
#include "Stream.h"
#include "DocumentProperties.h"
#include "PropertySet.h"

struct _DocumentProperties
{
    PropertySet* pSummaryInfo;
    PropertySet* pDocSummaryInfo;
    Stream* pSummaryInfoStream;
    Stream* pDocSummaryInfoStream;
};

#define DOC_SUMMARY_INFO_NAME L"\005DocumentSummaryInformation"
#define SUMMARY_INFO_NAME L"\005SummaryInformation"


/*============================================================================
  Description:  Allocates, initializes and returns new DocumentProperties 
                structure
  Arguments: 
   in_pRoot -  struct. storage
   out_ppDoc - handle to the allocated strucure
  Return:
   Status code
  ==========================================================================*/
SINT4
documentPropertiesConstruct(DocumentProperties** out_ppDoc)
{
    *out_ppDoc = (DocumentProperties*)malloc(sizeof(DocumentProperties));
    if (*out_ppDoc == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset( *out_ppDoc, 0, sizeof(DocumentProperties) );


    (*out_ppDoc)->pSummaryInfo = NULL;
    (*out_ppDoc)->pSummaryInfoStream = NULL;
    (*out_ppDoc)->pDocSummaryInfo = NULL;
    (*out_ppDoc)->pDocSummaryInfoStream = NULL;

    return SSTG_OK;
}


/*============================================================================
  Description:  Frees the DocumentSummary structure and all its components
  Arguments: 
   in_ppDoc - handle to the structure to be deleted
  Return:
   Status code
  ==========================================================================*/
SINT4
documentPropertiesDestroy(DocumentProperties** in_ppDoc)
{
    if ((*in_ppDoc)->pSummaryInfo != NULL)
        propertySetDestroy(&((*in_ppDoc)->pSummaryInfo));
    if ((*in_ppDoc)->pDocSummaryInfo != NULL)
        propertySetDestroy(&((*in_ppDoc)->pDocSummaryInfo));
    free(*in_ppDoc);
    *in_ppDoc = NULL;
    return SSTG_OK;
}


/*============================================================================
  Description:  Writes both document property streams to disk, if they exist
  Arguments: 
   in_pDoc - pointer to document property manager structure
  Return:
   Status code
  ==========================================================================*/
SINT4
documentPropertiesWrite(DocumentProperties* in_pDoc)
{
    SINT4 iRet = SSTG_OK;

    if (in_pDoc->pSummaryInfo != NULL)
    {
        iRet = propertySetWrite(in_pDoc->pSummaryInfo, in_pDoc->pSummaryInfoStream);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }


    if (in_pDoc->pDocSummaryInfo != NULL)
    {
        iRet = propertySetWrite(in_pDoc->pDocSummaryInfo, in_pDoc->pDocSummaryInfoStream);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    return iRet;
}


/*============================================================================
  Description:  Reads a property set if it already exists in the structured
                storage file.  Otherwise, attempts to create one.
  Arguments: 
   in_pRoot -          pointer to the structured storage root
   in_streamId -       stream identifier of property set to get, either 
                       SUMMARY_INFO or DOC_SUMMARY_INFO
   out_ppPropertySet - on success, points to the property set requested
  Return:
   Status code
  ==========================================================================*/
SINT4 
getPropertySet(RootStorage* in_pRoot, 
               PropertyStreamType in_streamId, 
               PropertySet** out_ppPropertySet)
{
    SINT4               iRet =      SSTG_OK;
    Storage*            pRootDir =  NULL;
    PropertySet**       ppSet =     NULL;
    Stream**            ppStream =  NULL;
    StgMode             mode =      STG_READ;
    const wchar_t*      pwName =    NULL;
    boolean             bRead =     SSRW_FALSE;
    DocumentProperties* pDoc =      NULL;
    
    pDoc = rootStorageGetDocProperties(in_pRoot);
    ppSet = (in_streamId == SUMMARY_INFO) ? 
            &(pDoc->pSummaryInfo) : 
            &(pDoc->pDocSummaryInfo);
    ppStream = (in_streamId == SUMMARY_INFO) ? 
            &(pDoc->pSummaryInfoStream) : 
            &(pDoc->pDocSummaryInfoStream);
    mode = rootStorageGetMode(in_pRoot);

    if ((*ppStream == NULL) && (*ppSet == NULL))
    {
        iRet = getStorageFromRoot (in_pRoot, &pRootDir);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
        pwName = (in_streamId == SUMMARY_INFO) ? 
                SUMMARY_INFO_NAME : 
                DOC_SUMMARY_INFO_NAME;
        if (STG_CANREAD(mode))
        {
            iRet = openStream (pRootDir, pwName, ppStream);
            if (iRet == SSTG_OK)
            {
                bRead = SSRW_TRUE;
            }
        }
        if ((*ppStream == NULL) && STG_CANWRITE(mode))
        {
            iRet = createStream(pRootDir, pwName, ppStream);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }
        if (*ppStream == NULL)
        {
            return SSTG_ERROR_PROPERTY_NOT_FOUND;
        }
    }

    if (*ppSet == NULL)
    {
        iRet = propertySetConstruct(in_streamId, mode, ppSet);
        if (iRet != SSTG_OK) 
        {
            return iRet;
        }
    }

    if (bRead)
    {
        iRet = propertySetRead(*ppSet, *ppStream);
        if (iRet != SSTG_OK) 
        {
            return iRet;
        }
    }

    *out_ppPropertySet = *ppSet;

    return iRet;
}


/*============================================================================
  Description:  gets value and type of the specified property
  Arguments: 
   in_pRoot -    struct. storage
   streamId -    SUMMARY_INFO or DOC_SUMMARY_INFO, selects between SummaryInfo 
                 and DocSummaryInfo streams.
   propertyId -  ID of property
   pType -       type of property value (INT2_TYPE, etc.)
   out_ppValue - handle to the allocated structure
  Note:
   If this functions fails, it is not fatal, since many files do not provide
   valid document property structures
  Return:
   Status code
  ==========================================================================*/
int
getDocumentProperty(RootStorage* in_pRoot,
                    PropertyStreamType streamId,
                    int propertyId,
                    TypeTag* pType,
                    void** ppValue)
{
    SINT4               iRet =      SSTG_OK;
    PropertySet*        pSet =      NULL;

    ASSERT (in_pRoot != NULL);
    ASSERT (streamId == SUMMARY_INFO || streamId == DOC_SUMMARY_INFO);
    ASSERT (pType != NULL);
    ASSERT (ppValue != NULL);
    if ((in_pRoot == NULL) ||
        (streamId != SUMMARY_INFO && streamId != DOC_SUMMARY_INFO) ||
        (pType == NULL) ||
        (ppValue == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    iRet = getPropertySet(in_pRoot, streamId, &pSet);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = propertySetGet(pSet, propertyId, pType, ppValue);

    return (int)iRet;
}


/*============================================================================
  Description:  Deallaocates the specified property, which was allocated by
                the structured storage library
  Arguments: 
    ppValue - handle to the value to be freed
  Return:
   Status code
  ==========================================================================*/
int
freeDocumentProperty(void** ppValue)
{
    free(*ppValue);
    *ppValue = NULL;
    return SSTG_OK;
}


/*============================================================================
  Description:  Sets value  of the specified property
  Arguments: 
   in_pRoot -   struct. storage
   streamId -   SUMMARY_INFO or DOC_SUMMARY_INFO, selects between SummaryInfo 
                and DocSummaryInfo streams.
   propertyId - ID of property
   srcType -    type of value to set (INT2_TYPE, etc.)
   pValue -     value to set
  Note:
   If this functions fails, it is not fatal, since many files do not provide
   valid document property structures
  Return:
   Status code
  ==========================================================================*/
int
setDocumentProperty(RootStorage* in_pRoot,
                    PropertyStreamType streamId,
                    int propertyId,
                    TypeTag srcType,
                    const void* pValue)
{
    SINT4               iRet =      SSTG_OK;
    PropertySet*        pSet =      NULL;

    ASSERT (in_pRoot != NULL);
    ASSERT (streamId == SUMMARY_INFO || streamId == DOC_SUMMARY_INFO);
    ASSERT (pValue != NULL);
    if ((in_pRoot == NULL) ||
        (streamId != SUMMARY_INFO && streamId != DOC_SUMMARY_INFO) ||
        (pValue == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    /* Make sure the file is updated on close */
    iRet = rootStorageSetDirty(in_pRoot);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = getPropertySet(in_pRoot, streamId, &pSet);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = propertySetPut(pSet, propertyId, srcType, pValue);

    return (int)iRet;
}


/*============================================================================
  Description:  sets default valuse for expected properties in the summary info
                and document summary
  Arguments: 
   in_pRoot - struct. storage to set info to.
  Note:
   The functions first checks for the presense of at least one property set.
   If it can find one, it aborts.
  Return:
   Status code
  ==========================================================================*/
int
setDefaultDocumentProperties(RootStorage* in_pRoot)
{
    DocumentProperties*     pDoc =              NULL;
    SINT4                   iRet =              SSTG_OK;
    Storage*                pRootDir =          NULL;

    char                    title [] =          "No title";
    char                    author [] =         " Auto-generated";
    char                    subject [] =        "unknown";
    char                    appname [] = "SchemaSoft Structured Storage Library v 3.0";
    SINT4                   securtityLevel =    0; /* no security */
    time_t                  currentTime =       SsrwTime(NULL);

    ASSERT (in_pRoot != NULL);
    if (in_pRoot == NULL)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    /* Make sure the file is updated on close */
    iRet = rootStorageSetDirty(in_pRoot);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    pDoc = rootStorageGetDocProperties(in_pRoot);
    if ((pDoc->pSummaryInfo != NULL) || (pDoc->pDocSummaryInfo != NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    iRet = (SINT4)getStorageFromRoot (in_pRoot, &pRootDir);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }

    /* Set up Summary Info */
    iRet = createStream(pRootDir, SUMMARY_INFO_NAME, &pDoc->pSummaryInfoStream);
    if (iRet != SSTG_OK) return iRet;

    iRet = propertySetConstruct(SUMMARY_INFO, STG_WRITE, &pDoc->pSummaryInfo);
    if (iRet != SSTG_OK) return iRet;

    iRet = propertySetPut(pDoc->pSummaryInfo, PIDSI_TITLE, STRING_TYPE, title);
    iRet = propertySetPut(pDoc->pSummaryInfo, PIDSI_AUTHOR, STRING_TYPE, author);
    iRet = propertySetPut(pDoc->pSummaryInfo, PIDSI_SUBJECT, STRING_TYPE, subject);
    iRet = propertySetPut(pDoc->pSummaryInfo, PIDSI_APPNAME, STRING_TYPE, appname);

    iRet = propertySetPut(pDoc->pSummaryInfo, PIDSI_SECURITY, INT4_TYPE, &securtityLevel);
    iRet = propertySetPut(pDoc->pSummaryInfo, PIDSI_CREATE_DTM, TIME_TYPE, &currentTime);

    return iRet;
}

