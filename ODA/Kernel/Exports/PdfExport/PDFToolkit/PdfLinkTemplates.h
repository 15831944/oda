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




#ifndef _PDFLINKTEMPLATES_INCLUDED_
#define _PDFLINKTEMPLATES_INCLUDED_ /*!DOM*// 

#include "PdfCommon.h"
#include "PdfObjectID.h"
#include "PdfDocument.h"
#include "PdfIStream.h"

namespace TD_PDF
{

#define CONST_STR_LITERAL_LEN(str) (sizeof(str) / sizeof(char) - 1)
#define PUT_STRING_TO_PSTRIM_CONST_STR_LITERAL(str) pStream->putBytes(str, CONST_STR_LITERAL_LEN(str));

template <class T> class PDFIndirectLinkedObj : public T
{
  mutable PDFObjectID m_ObjectId;

  void setObjectId() const
  {
    if (m_ObjectId.isNull())
    {
      ODA_ASSERT(this->document());
      if (this->document())
      {
        m_ObjectId = this->document()->getNextObjectID();
      }
    }
  }

public:

  virtual bool isIndirect() const { return true; }
  virtual void IndirectObjectID(PDFObjectID &id) const
  {
    setObjectId();

    id = m_ObjectId;
  }
  virtual bool ExportLikeRef(PDFIStream *pStream, const PDFVersion &ver) 
  { 
    setObjectId();

    m_ObjectId.Export(pStream, ver);
    PUT_STRING_TO_PSTRIM_CONST_STR_LITERAL(" R");
    
    return true;
  }

  virtual bool ExportLikeRefObj(PDFIStream * pStream, const PDFVersion &ver) 
  { 
    setObjectId();

    m_ObjectId.Export(pStream, ver);
    PUT_STRING_TO_PSTRIM_CONST_STR_LITERAL(" obj");
    pStream->putEOL();
    
    T::Export(pStream, ver);
    
    pStream->putEOL();
    PUT_STRING_TO_PSTRIM_CONST_STR_LITERAL("endobj");

    return true; 
  }
};

template <class T> class PDFDirectLinkedObj : public T
{
public:
  virtual bool isIndirect() const { return false; }
  virtual bool ExportLikeRef(PDFIStream * /*pStream*/, const PDFVersion &/*ver*/) { return false; }
};

};

#endif //_PDFLINKTEMPLATES_INCLUDED_

