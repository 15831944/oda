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

#ifdef ANDROID

#include "OdaCommon.h"
#include "OdAlloc.h"
#include "OdPlatform.h"
#include "OdCharMapper.h"
#include "DbSystemServices.h"

#include <wchar_.h> 

#undef btowc
#undef mbrlen
#undef mbrtowc
#undef mbsinit
#undef mbsrtowcs
#undef wcrtomb
#undef wcsrtombs
#undef wcstombs

extern "C" {

wint_t btowc(int);
size_t mbrlen(const char * __restrict, size_t, mbstate_t * __restrict);
size_t mbrtowc(wchar_t * __restrict, const char * __restrict, size_t,
	             mbstate_t * __restrict);
int	mbsinit(const mbstate_t *);
size_t mbsrtowcs(wchar_t * __restrict, const char ** __restrict, size_t,
	               mbstate_t * __restrict);
size_t wcrtomb(char * __restrict, wchar_t, mbstate_t * __restrict);
size_t wcsrtombs(char * __restrict, const wchar_t ** __restrict, size_t,
                 mbstate_t * __restrict);
size_t wcstombs(char *dest, const wchar_t *src, size_t n);

}

static void catchFn()
{
}

static wint_t Od_btowc(int c)
{
  size_t cnt = 0; 
  catchFn();

  cnt = btowc(c);
  return cnt; 
}

static size_t Od_mbrlen(const char *s, size_t n, mbstate_t *ps)
{
  size_t cnt = 0; 
  catchFn();

  cnt = mbrlen(s, n, ps);
  return cnt;
}

static int Od_mbsinit(const mbstate_t *ps)
{
  catchFn();

  int res = mbsinit(ps);
  return res;
}

static size_t Od_mbrtowc(wchar_t* pwc, const char* s, size_t n, mbstate_t* ps)
{
  size_t cnt = 0; 
  catchFn();
  //return mbrtowc(pwc, s, n, ps); // test

  OdCharArray buf; // ? OdArray<wchar_t, OdMemoryAllocator<wchar_t> > buf;
  //OdCodePageId codePage = odSystemServices()->systemCodePage();
  //OdCharMapper::multiByteToWideChar(codePage, *src, len, buf);
  OdAnsiCharArray bufChar;
  memcpy(bufChar.setLogicalLength(n + 1).asArrayPtr(), s, n); 
  bufChar[n] = '\0';
  OdCharMapper::utf8ToUnicode(bufChar.getPtr(), n, buf);
  cnt = buf.size();
  if (pwc && cnt)
    *pwc = *buf.getPtr();
  if (!cnt)
    return 0;
  wchar_t wc_[2];
  wc_[0] = buf[0];
  wc_[1] = L'\0';
  bufChar.resize(0);
  OdCharMapper::unicodeToUtf8(wc_, 1, bufChar);
  cnt = bufChar.size() - 1; // without terminated zero
  return cnt;
}

static size_t Od_mbsrtowcs(wchar_t* dest, const char** src, size_t len, mbstate_t* ps)
{
  size_t cnt = 0; 
  catchFn();
  //cnt = mbsrtowcs(dest, src, len, ps); // test (even breaks input data)

  OdCharArray buf; // ? OdArray<wchar_t, OdMemoryAllocator<wchar_t> > buf;
  //OdCodePageId codePage = odSystemServices()->systemCodePage();
  //OdCharMapper::multiByteToWideChar(codePage, *src, len, buf);
  OdAnsiCharArray bufIn;
  memcpy(bufIn.setLogicalLength(len + 1).asArrayPtr(), *src, len); 
  bufIn[len] = '\0';
  OdCharMapper::utf8ToUnicode(bufIn.getPtr(), len, buf);
  cnt = buf.size();
  if (dest && cnt)
    memcpy(dest, buf.getPtr(), cnt * sizeof(wchar_t));
  return --cnt; // without terminated zero
}

static size_t Od_wcrtomb(char *s, wchar_t wc, mbstate_t *ps)
{
  size_t cnt = 0; 
  catchFn();
  //cnt = wcrtomb(s, wc,ps); // test

  OdAnsiCharArray buf;
  //OdCodePageId codePage = odSystemServices()->systemCodePage();
  //OdCharMapper::wideCharToMultiByte(codePage, &wc, 1, buf);
  wchar_t wc_[2];
  wc_[0] = wc;
  wc_[1] = L'\0';
  OdCharMapper::unicodeToUtf8(wc_, 1, buf);
  cnt = buf.size() - 1; // without terminated zero
  if (cnt)
    memcpy(s, buf.getPtr(), cnt);
  return cnt;
}

static size_t Od_wcsrtombs(char *dest, const wchar_t **src,
                           size_t len, mbstate_t *ps)
{
  size_t cnt = 0; 
  catchFn();
  //cnt = wcsrtombs(dest, src, len, ps); // test (general exception)

  OdAnsiCharArray buf;
  len = Citrus::wcslen(*src);
  //OdCodePageId codePage = odSystemServices()->systemCodePage();
  //OdCharMapper::wideCharToMultiByte(codePage, *src, (int) len, buf);
  OdCharMapper::unicodeToUtf8(*src, (int) len, buf);
  cnt = buf.size();
  if (dest && cnt)
    memcpy(dest, buf.getPtr(), cnt);
  return --cnt; // without terminated zero
}

static size_t Od_wcstombs(char *dest, const wchar_t *src, size_t n)
{
  size_t cnt = 0; 
  catchFn();
  //cnt = wcstombs(dest, src, n); // test

  OdAnsiCharArray buf;
  int len = Citrus::wcslen(src);
  //OdCodePageId codePage = odSystemServices()->systemCodePage();
  //OdCharMapper::wideCharToMultiByte(codePage, src, len, buf);
  OdCharMapper::unicodeToUtf8(src, len, buf);
  cnt = buf.size();
  if (dest && cnt)
    memcpy(dest, buf.getPtr(), cnt);
  return --cnt; // without terminated zero
}

void initMbWcConv()
{
  Citrus::fn_btowc = Od_btowc;
  Citrus::fn_mbrlen = Od_mbrlen;
  Citrus::fn_mbsinit = Od_mbsinit;

  //Citrus::fn_mbrtowc = Od_mbrtowc;
  //Citrus::fn_mbsrtowcs = Od_mbsrtowcs;
  //Citrus::fn_wcrtomb = Od_wcrtomb;
  //Citrus::fn_wcsrtombs = Od_wcsrtombs;
  //Citrus::fn_wcstombs = Od_wcstombs;
}

#endif // ANDROID
