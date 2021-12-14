/************************************************************************
* UTF8.h
*
* Contains prototypes for functions that convert from UTF-8 <-> UCS-2
*
* (c) Schema Software Inc., 2004
*
*************************************************************************

$Revision: $
$Date: $
*/

#ifndef __UTF8_H__
#define __UTF8_H__

_OLESS_API int wctou8(char *out_pm, wchar_t in_wch);

_OLESS_API int wcu8len(const wchar_t in_wch);

_OLESS_API int wcsu8slen(const wchar_t *in_pwch);

_OLESS_API size_t wcstou8s(char *out_pu, const wchar_t *in_pwch, size_t in_count);

_OLESS_API int u8len(const char *in_pu, size_t in_count);

_OLESS_API int u8towc(wchar_t *out_pwch, const char *in_pu, size_t in_count);

_OLESS_API int u8swcslen(const char* in_pu);

_OLESS_API size_t u8stowcs( wchar_t *pw, const char *pu, size_t count );

#endif /* __UTF8_H__ */
