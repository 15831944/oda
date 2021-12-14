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

//
// Copyright (c) 2004 by Tech Soft 3D, LLC.
// The information contained herein is confidential and proprietary to
// Tech Soft 3D, LLC., and considered a trade secret as defined under
// civil and criminal statutes.  Tech Soft 3D shall pursue its civil
// and criminal remedies in the event of unauthorized use or misappropriation
// of its trade secrets.  Use of this information by anyone other than
// authorized employees of Tech Soft 3D, LLC. is granted only under a
// written non-disclosure agreement, expressly prescribing the scope and
// manner of such use.
//
// $Header: /files/homes/master/cvs/hoops_master/hoops_appwizard/VCWizards/HOOPSAppWiz_NET2005/Templates/1033/MfcApp_3dgsOnly/HUtilityLocaleString.h,v 1.2 2006-08-07 20:38:55 stage Exp $
//

// HUtilityLocaleString.h : Utility classes to convert char strings to wchar_t strings and vice-versa


#ifndef _HUTILITY_LOCALTE_STRING_H__RAJESH_B__20040211__1110__
#define _HUTILITY_LOCALTE_STRING_H__RAJESH_B__20040211__1110__


// HUtilityUnicodeStr ////////////////////////////////////////////////////////
// Converts both 8-bit (foreign) and 16-bit (native) strings to 16-bit wideness

/*class HUtilityUnicodeStr 
{
public:
	
	// native and foreign constructors
    HUtilityUnicodeStr(const char *p8);
    HUtilityUnicodeStr(const wchar_t *p16);
	
	// non-virtual destructor (this class is concrete)
	~HUtilityUnicodeStr(void);
	
	// native conversion operator
	operator const wchar_t * (void) const;
	
	// protect against assignment!
	HUtilityUnicodeStr(const HUtilityUnicodeStr&);
    HUtilityUnicodeStr& operator=(const HUtilityUnicodeStr&);

private:
	
	// native wideness string
    wchar_t *m_sz;
	
	// is foreign??
    BOOL m_bIsForeign;
	
};

// native constructor is a pass-through
inline HUtilityUnicodeStr::HUtilityUnicodeStr(const wchar_t *p16)
: m_sz((wchar_t *)p16), m_bIsForeign(FALSE)
{
}

// simply give out the native wideness string
inline HUtilityUnicodeStr::operator const wchar_t * (void) const
{
	return m_sz;
}

// foreign constructor requires allocation of a native
// string and conversion
inline HUtilityUnicodeStr::HUtilityUnicodeStr(const char *p8)
: m_bIsForeign(TRUE)
{
	// calculate string length
	size_t len = strlen(p8);
	
	// calculate required buffer size (some characters may
	// already occupy 16-bits under DBCS)
	size_t size = mbstowcs(0, p8, len) + 1;
	
	// alloc native string and convert
	if (m_sz = new wchar_t[size])
		mbstowcs(m_sz, p8, size);
}

// delete native string only if synthesized in foreign constructor
inline HUtilityUnicodeStr::~HUtilityUnicodeStr(void) 
{
	if (m_bIsForeign)
		delete[] m_sz;
}




// HUtilityAsciiStr /////////////////////////////////////////////////////////

// Shim class that converts both 8-bit (native) and
// 16-bit (foreign) strings to 8-bit wideness

class HUtilityAsciiStr 
{

public:
	// native and foreign constructors
	HUtilityAsciiStr(const char *p8);
	HUtilityAsciiStr(const wchar_t *p16);
	
	// non-virtual destructor (this class is concrete)
	~HUtilityAsciiStr(void);
	
	// native conversion operator
	operator const char * (void) const;
	
	
private:
	// native wideness string
	char *m_sz;
	// is foreign??
	BOOL m_bIsForeign;
	
	// protect against assignment!
	HUtilityAsciiStr(const HUtilityAsciiStr&);
	HUtilityAsciiStr& operator=(const HUtilityAsciiStr&);
	
};

// native constructor is a pass-through
inline HUtilityAsciiStr::HUtilityAsciiStr(const char *p8) 
: m_sz((char *)p8), // casting away constness ONLY FOR CONVENIENCE!
m_bIsForeign(FALSE) 
{ 
}

// simply give out the native wideness string 
inline HUtilityAsciiStr::operator const char * (void) const 
{
	return m_sz;
}

// foreign constructor requires allocation of a native
// string and conversion
inline HUtilityAsciiStr::HUtilityAsciiStr(const wchar_t *p16)
: m_bIsForeign(TRUE) 
{
	// calculate string length
	size_t len = wcslen(p16);
	
	// calculate required buffer size (some characters may
	// require more than one byte under DBCS)
	size_t size = wcstombs(0, p16, len) + 1;
	
	// alloc native string and convert
	if (m_sz = new char[size])
		wcstombs(m_sz, p16, size);
}

// delete native string only if synthesized in foreign constructor
inline HUtilityAsciiStr::~HUtilityAsciiStr(void) {
	if (m_bIsForeign) 
		delete[] m_sz;
}


// H_TEXT - use this macro where-ever the api itself takes different type of argument 
// during compilation depending upon preprocessor
#ifdef _UNICODE
typedef HUtilityUnicodeStr	H_TEXT;
#else
typedef HUtilityAsciiStr H_TEXT;
#endif

typedef HUtilityUnicodeStr	H_UNICODE_TEXT;
typedef HUtilityAsciiStr		H_ASCII_TEXT;
*/
#endif	//_HUTILITY_LOCALTE_STRING_H__RAJESH_B__20040211__1110__

