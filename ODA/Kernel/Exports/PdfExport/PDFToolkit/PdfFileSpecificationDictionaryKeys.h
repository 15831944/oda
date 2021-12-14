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


// Entries in the catalog, PDFDictionary)

#ifndef _PDF_DICT_KEYS_A_
#define _PDF_DICT_KEYS_A_(KEYNAME, KEYTYPE)
#endif

// #ifdef IN
// #undef IN
// #endif

_PDF_DICT_KEYS_A_(Type       , PDFName          )//(Required if an EF or RF entry is present; recommended always) The type of PDF object that this dictionary describes; shall be Filespec for a file specification dictionary.
_PDF_DICT_KEYS_A_(FS         , PDFName          )//(Optional) The name of the file system that shall be used to interpret this file specification.
_PDF_DICT_KEYS_A_(F          , PDFName          )//(Required) A file specification string of the form described in 7.11.2, "File Specification Strings
_PDF_DICT_KEYS_A_(UF         , PDFTextString    )//(Required) Unicode text string that provides file specification of the form described in 7.11.2, "File Specification Strings.
_PDF_DICT_KEYS_A_(DOS        , PDFTextString    )//(Optional) A file specification string (see 7.11.2, "File Specification Strings") representing a DOS file name.
_PDF_DICT_KEYS_A_(Mac        , PDFTextString    )//(Optional) A file specification string (see 7.11.2, "File Specification Strings") representing a Mac OS file name.
_PDF_DICT_KEYS_A_(Unix       , PDFTextString    )//(Optional) A file specification string (see 7.11.2, "File Specification Strings") representing a UNIX file name.
_PDF_DICT_KEYS_A_(ID         , PDFArray         )//(Optional) An array of two byte strings constituting a file identifier (see 14.4, "File Identifiers") that should be included in the referenced file.
_PDF_DICT_KEYS_A_(V          , PDFBoolean       )//(Optional; PDF 1.2) A flag indicating whether the file referenced by the file specification is volatile (changes frequently with time).
_PDF_DICT_KEYS_A_(EF         , PDFDictionary    )//(Required if RF is present; PDF 1.3; amended to include the UF key in PDF 1.7) A dictionary containing a subset of the keys F, UF, DOS, Mac, and Unix, corresponding to the entries by those names in the file specification dictionary.
_PDF_DICT_KEYS_A_(RF         , PDFDictionary    )//(Optional; PDF 1.3) A dictionary with the same structure as the EF dictionary, which shall be present.
_PDF_DICT_KEYS_A_(Desc       , PDFTextString    )//(Optional; PDF 1.6) Descriptive text associated with the file specification.
_PDF_DICT_KEYS_A_(Cl         , PDFDictionary    )//(Optional; shall be indirect reference; PDF 1.7) A collection item dictionary, which shall be used to create the user interface for portable collections (see 7.11.6, "Collection Items").

#undef _PDF_DICT_KEYS_A_
