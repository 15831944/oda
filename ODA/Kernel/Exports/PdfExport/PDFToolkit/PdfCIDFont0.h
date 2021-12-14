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




#ifndef _PDFCIDFONT0_INCLUDED_
#define _PDFCIDFONT0_INCLUDED_ /*!DOM*// 

#include "PdfCIDFont.h"
#include "UInt16Array.h"

namespace TD_PDF
{

class PDFEXPORT_TOOLKIT PDFCIDFont0 : public PDFCIDFont
{
  PDF_DECLARE_OBJECT(PDFCIDFont0, PDFCIDFont, kCIDFont0)

protected:
  virtual void InitObject();
public:
  virtual bool truncateFont(OdUInt16 nFirstChar, OdUInt16 nLastChar);
  virtual bool truncateFont(OdUInt16 /*nFirstChar*/, OdUInt16 /*nLastChar*/, const OdUInt16Array &/*fontUsedUnicodeChars*/);

#include "PdfDictKeysDefs.h"
#include "PdfCIDFont0Keys.h"
#include "PdfDictKeysUnDefs.h"

};

typedef PDFSmartPtr<PDFCIDFont0> PDFCIDFont0Ptr;

};

#endif //_PDFCIDFONT0_INCLUDED_
