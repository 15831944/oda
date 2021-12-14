/************************************************************************
* PropertySet.h
*
* Prototypes and external declarations for PropertySet structure
*
* (c) Schema Software Inc., 2001
* 
*************************************************************************

$Revision: 1.4 $
$Date: 2003/02/01 01:14:49 $
*/

#ifndef __PropertySet_H__
#define __PropertySet_H__

struct _PropertySet;
typedef struct _PropertySet PropertySet;

_OLESS_API SINT4 propertySetConstruct(PropertyStreamType in_id, StgMode in_mode, PropertySet** out_ppSet);
_OLESS_API SINT4 propertySetDestroy(PropertySet** in_ppSet);
_OLESS_API SINT4 propertySetRead(PropertySet* in_pSet, Stream* in_pStream);
_OLESS_API SINT4 propertySetGet(PropertySet* in_pSet, SINT4 in_id, TypeTag* out_pType, void** out_ppValue);
_OLESS_API SINT4 propertySetPut(PropertySet* in_pSet, SINT4 in_id, TypeTag in_type, const void* in_pValue);
_OLESS_API SINT4 propertySetWrite(PropertySet* in_pSet, Stream* in_pStream);

#endif

