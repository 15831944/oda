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




#ifndef _PDFEmbeddedFileSTREAM_INCLUDED_
#define _PDFEmbeddedFileSTREAM_INCLUDED_ /*!DOM*/ 

#include "PdfStream.h"
#include "PdfEmbeddedFileStreamDictionary.h"

namespace TD_PDF
{

  class PDFIStream;
  class PDFVersion;

  class PDFEXPORT_TOOLKIT PDFEmbeddedFileStream : public PDFStream
  {
    PDF_DECLARE_OBJECT(PDFEmbeddedFileStream, PDFStream, kEmbeddedFileStream)

  protected:
    virtual void InitObject();
  public:
    PDFEmbeddedFileStreamDictionaryPtr getDictionary();

    virtual void putData(const char *pBuf, OdUInt32 nSize);
    virtual void getData(char *pBuf, OdUInt32 nSize);

    virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);
  };

  typedef PDFSmartPtr<PDFEmbeddedFileStream> PDFEmbeddedFileStreamPtr;

};

#endif //_PDFEmbeddedFileSTREAM_INCLUDED_

