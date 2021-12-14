/************************************************************************
* DocumentProperties.h
*
* Prototypes and external declarations for DocumentProperties
*
* (c) Schema Software Inc., 2001
* 
*************************************************************************

$Revision: 1.3 $
$Date: 2002/10/09 22:19:32 $
*/

#ifndef __DocumentProperties_H__
#define __DocumentProperties_H__

_OLESS_API SINT4 documentPropertiesConstruct(DocumentProperties** out_ppDoc);
_OLESS_API SINT4 documentPropertiesDestroy(DocumentProperties** in_ppDoc);
_OLESS_API SINT4 documentPropertiesWrite(DocumentProperties* in_pDoc);

#endif

