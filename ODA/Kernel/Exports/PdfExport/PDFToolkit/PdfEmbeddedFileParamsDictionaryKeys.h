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

_PDF_DICT_KEYS_A_(Size         , PDFInteger    )  // (Optional) The size of the uncompressed embedded file, in bytes.
_PDF_DICT_KEYS_A_(CreationDate , PDFDate       )  // (Optional) The date and time when the embedded file was created.
_PDF_DICT_KEYS_A_(ModDate      , PDFDate       )  // (Optional) The date and time when the embedded file was last modified.
_PDF_DICT_KEYS_A_(Mac          , PDFDictionary )  // (Optional) A subdictionary containing additional information specific to Mac OS files
_PDF_DICT_KEYS_A_(CheckSum     , PDFTextString )  // (Optional) A 16-byte string that is the checksum of the bytes of the uncompressed embedded file.

#undef _PDF_DICT_KEYS_A_
