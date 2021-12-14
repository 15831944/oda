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

/////////////////////////////////////////////////////////////////////
//
// PdfAux.cpp
//
//////////////////////////////////////////////////////////////////////

#include "PdfExportCommon.h"
#include "PdfAux.h"
#include "OdPlatform.h"
#include "Pdf2dExportDevice.h"

#include "PdfDocument.h"
#include "PdfCatalogDictionary.h"
#include "PdfPageNodeDictionary.h"
#include "PdfPageDictionary.h"
#include "PdfOutputIntentsDictionary.h"
#include "PdfContentStream.h"
#include "PdfRGBStream.h"
#include "PdfExportParams.h"
#include "PdfDocumentInformation.h"
#include "PdfExportVersion.h"
#include "PdfToolkitVersion.h"
#include "PdfTempFileStream.h"
#include "PdfMetadataStream.h"
#include "PdfICCBasedStream.h"
#include "PdfViewportDictionary.h"
#include "PdfMeasureDictionary.h"
#include "PdfNumberFormatDictionary.h"
#include "PdfWatermarkAnnotationDictionary.h"
#include "PdfFixedPrintDictionary.h"
#include "PdfExtGState.h"

#include "MemoryStream.h"
#include "OdCharMapper.h"
#include "Gi/GiRasterWrappers.h"
#include "RxObjectImpl.h"

namespace TD_PDF_2D_EXPORT {

void PDFAUX::createBasePDF4DWG(PDFDocument &PDFDoc, const OdArray<OdGsPageParams> &pageParams, const PDFExportParams &pParams)
{
  PDFPageNodeDictionaryPtr pRootPageNode = PDFPageNodeDictionary::createObject(PDFDoc, true);
  
  for(OdUInt32 f=0; f<pageParams.size(); ++f)
  {
    PDFPageDictionaryPtr pCurPage( PDFPageDictionary::createObject(PDFDoc, true) );
  
    PDFRectanglePtr pRect( PDFRectangle::createObject(PDFDoc) );
    pRect->set(0, 0, OdRound(pageParams[f].getPaperWidth()), OdRound(pageParams[f].getPaperHeight()));
    pCurPage->setMediaBox(pRect);

    PDFContentStreamPtr pCStream = PDFContentStream::createObject(PDFDoc, true);
    PDFArrayPtr pContentArray = PDFArray::createObject(PDFDoc);
    pContentArray->append(pCStream);

    pCurPage->setContents(pContentArray);

    if (pParams.archived() == PDFExportParams::kPDFA_2b)
    {
      pCStream->CS(PDFName::createObject(PDFDoc, ("DeviceRGB")));
      pCStream->cs(PDFName::createObject(PDFDoc, ("DeviceRGB")));

      PDFResourceDictionaryPtr pRes = createResDictionary4DWG(PDFDoc);
      pCurPage->setResources(pRes);
    }

    pRootPageNode->AddKids(pCurPage);
  }

  PDFCatalogDictionaryPtr pCatalog = PDFCatalogDictionary::createObject(PDFDoc, true); PDFDoc.setRoot(pCatalog);
  pCatalog->setPages( pRootPageNode );
}

OdGsDCPoint getOffsetAndRotation(const Watermark& wm, const OdGsDCRect& bbox, OdGeMatrix2d& rotation, OdUInt16& fontSize)
{
  OdGsDCPoint res;
  rotation.setToIdentity();

  PDFType1Font::StandardType1FontsEnum font_type = (PDFType1Font::StandardType1FontsEnum)wm.font;
  fontSize = wm.fontSize;

  double textWidth = PDFType1Font::getTextBaseWidth(font_type, wm.text) * fontSize;
  ODRECT textBox = PDFType1Font::getTextBBox(font_type);
  double descender = fabs((double)textBox.top / 1000. * (double)fontSize);

  if (wm.scaleToPage)
  {
    long mediaWidth = bbox.m_max.x - bbox.m_min.x;
    if (wm.position == Watermark::kLowerLeftToUpperRight || wm.position == Watermark::kUpperLeftToLowerRight)
    {
      mediaWidth = sqrt((double)(bbox.m_max.x - bbox.m_min.x)*(double)(bbox.m_max.x - bbox.m_min.x) + (double)(bbox.m_max.y - bbox.m_min.y)*(double)(bbox.m_max.y - bbox.m_min.y));
      mediaWidth = mediaWidth - mediaWidth / 10.;
    }
    if (textWidth >= mediaWidth)
    {
      while (textWidth >= mediaWidth)
      {
        fontSize--;
        textWidth = PDFType1Font::getTextBaseWidth(font_type, wm.text) * fontSize;
      }
    } 
    else
    {
      while (textWidth < mediaWidth)
      {
        fontSize++;
        textWidth = PDFType1Font::getTextBaseWidth(font_type, wm.text) * fontSize;
      }
      fontSize--;
      textWidth = PDFType1Font::getTextBaseWidth(font_type, wm.text) * fontSize;
    }
  }
  double textHeight = (double)(textBox.bottom - Od_abs(textBox.top) + 1) / 1000. * fontSize;

  OdGePoint2d start, center;
  double ang = 0.;
  switch (wm.position)
  {
  case Watermark::kLowerLeft:
    res.x = bbox.m_min.x;
    res.y = bbox.m_min.y + descender;
    break;
  case Watermark::kLowerRight:
    res.x = bbox.m_max.x - textWidth;
    res.y = bbox.m_min.y + descender;
    break;
  case Watermark::kUpperLeft:
    res.x = bbox.m_min.x;
    res.y = bbox.m_max.y - textHeight;
    break;
  case Watermark::kUpperRight:
    res.x = bbox.m_max.x - textWidth;
    res.y = bbox.m_max.y - textHeight;
    break;
  case Watermark::kLeftToRight:
    res.x = (double)(bbox.m_max.x + bbox.m_min.x) / 2. - textWidth / 2.;
    res.y = (double)(bbox.m_max.y + bbox.m_min.y) / 2. - textHeight / 2.;
    break;
  case Watermark::kLowerLeftToUpperRight:
  case Watermark::kUpperLeftToLowerRight:
    ang = atan(double(bbox.m_max.y - bbox.m_min.y) / double(bbox.m_max.x - bbox.m_min.x));
    center = OdGePoint2d(double(bbox.m_max.x + bbox.m_min.x) / 2., double(bbox.m_max.y + bbox.m_min.y) / 2.);
    start = OdGePoint2d((double)(bbox.m_max.x + bbox.m_min.x) / 2. - textWidth / 2., (double)(bbox.m_max.y + bbox.m_min.y) / 2. - textHeight / 2.);

    if (wm.position == Watermark::kLowerLeftToUpperRight)
      rotation.setToRotation(ang, center);
    else if (wm.position == Watermark::kUpperLeftToLowerRight)
      rotation.setToRotation(-ang, center);

    start.transformBy(rotation);
    res.x = start.x;
    res.y = start.y;
    break;
  default:
    break;
  }
  return res;
}

void PDFAUX::CreateWatermark(int WMIndex, const OdGsDCRect& bbox, PDFDocument &PDFDoc, PDFPageDictionary* pPage, const PDFExportParams &pParams)
{
  const Watermark wm = pParams.watermarks().at(WMIndex);
  if (wm.text.getLength() == 0 || !pPage)
    return;

  PDFType1Font::StandardType1FontsEnum font_type = (PDFType1Font::StandardType1FontsEnum)wm.font;
  OdUInt16 fontSize = wm.fontSize;
  OdUInt16 opacity = wm.opacity;
  ODCOLORREF textColor = wm.color;
  Watermark::WatermarkPosition pos = wm.position;

  double rTxt = double(ODGETRED(textColor)) / 255.;
  double gTxt = double(ODGETGREEN(textColor)) / 255.;
  double bTxt = double(ODGETBLUE(textColor)) / 255.;

  //PDF 32000-1:2008 - "many printing devices have non printable margins,
  //such margins should be taken into consideration when positioning watermark annotations near the edge of a page"
  //let's take them about 5% of the page dimension
  int marginX = (bbox.m_max.x - bbox.m_min.x) / 20;
  int marginY = (bbox.m_max.y - bbox.m_min.y) / 20;
  OdGsDCRect boxWithMargins(bbox.m_min.x + marginX, bbox.m_max.x - marginX, bbox.m_min.y + marginY, bbox.m_max.y - marginY);

  PDFArrayPtr annots = pPage->getAnnots();
  PDFWatermarkAnnotationDictionaryPtr pDict = PDFWatermarkAnnotationDictionary::createObject(PDFDoc, true);

  PDFRectanglePtr rect = PDFRectangle::createObject(PDFDoc);
  rect->set(boxWithMargins.m_min.x, boxWithMargins.m_min.y, boxWithMargins.m_max.x, boxWithMargins.m_max.y);
  pDict->setRect(rect);
  pDict->setP(pPage);

  PDFFixedPrintDictionaryPtr fpDict = PDFFixedPrintDictionary::createObject(PDFDoc);
  fpDict->setH(PDFNumber::createObject(PDFDoc, 0.));
  fpDict->setV(PDFNumber::createObject(PDFDoc, 0.));
  PDFArrayPtr pMatrix = PDFArray::createObject(PDFDoc);
  pMatrix->push_number(1);  pMatrix->push_number(0);  pMatrix->push_number(0);
  pMatrix->push_number(1);  pMatrix->push_number(0);  pMatrix->push_number(0);
  fpDict->setMatrix(pMatrix);
  pDict->setFixedPrint(fpDict);

  PDFIntegerPtr pInt = PDFInteger::createObject(PDFDoc, 196, false); //print (4) & read only (64) & locked (128)
  pDict->setF(pInt);

  PDFSubDictionaryPtr pAp = PDFSubDictionary::createObject(PDFDoc);
  PDFXObjectFormPtr pN = PDFXObjectForm::createObject(PDFDoc, true);
  pN->dictionary()->setBBox(rect);
  pAp->AddItem("N", pN);
  pDict->setAP(pAp);

  PDFFontPtr pOutFont;
  PDFResourceDictionaryPtr pResources = pN->dictionary()->getResources();
  OdAnsiString fontName = TD_PDF_HELPER_FUNCS::addType1Font(PDFDoc, font_type, &pOutFont, pResources);

  OdGeMatrix2d rotation;
  OdGsDCPoint offset = getOffsetAndRotation(wm, boxWithMargins, rotation, fontSize);

  pN->q();
  pN->n();
  pN->rg(rTxt, gTxt, bTxt);
  pN->RG(rTxt, gTxt, bTxt);

  if (opacity < 100)
  {
    OdAnsiString lwstr;
    odDToStr(lwstr.getBufferSetLength(512), opacity, 'f', 0);
    lwstr.releaseBuffer();

    PDFResourceDictionaryPtr pResDict(PDFPageNodeDictionaryPtr(PDFDoc.Root()->getPages())->getResources());
    PDFExtGStateSubDictionaryPtr pExtGsSub = pResDict->getExtGState();
    OdAnsiString opacityGroup = OdAnsiString("WatermarkGS"+ lwstr);
    PdfExtGStatePtr pExtGState;
    if (!pExtGsSub->HasItem(opacityGroup))
    {
      pExtGState = PdfExtGState::createObject(PDFDoc, true);
      //pExtGState->setBM(PDFName::createObject(PDFDoc, "Normal"));
      pExtGState->setType(PDFName::createObject(PDFDoc, "ExtGState"));
      PDFNumberPtr pCa = PDFNumber::createObject(PDFDoc, (double)opacity / 100.);
      pExtGState->setCA(pCa);
      pExtGState->setca(pCa);
      pExtGsSub->AddItem(opacityGroup, pExtGState);
    }
    else
      pExtGState = pExtGsSub->Find(opacityGroup);

    PDFExtGStateSubDictionaryPtr pExtGsSubAnnot = pResources->getExtGState();
    if (!pExtGsSubAnnot->HasItem(opacityGroup))
      pExtGsSubAnnot->AddItem(opacityGroup, pExtGState);
    pN->gs(PDFName::createObject(PDFDoc, opacityGroup.c_str()));
  }

  pN->BT();
  pN->Tf(PDFName::createObject(PDFDoc, fontName, false), fontSize);
  pN->Tm(rotation.entry[0][0], rotation.entry[1][0], rotation.entry[0][1], rotation.entry[1][1], offset.x, offset.y);

  PDFTextStringPtr pText = PDFTextString::createObject(PDFDoc);
  OdAnsiString str(wm.text);
  pText->set(str);
  pN->Tj(pText);
  pN->ET();
  pN->Q();

  annots->push_back(pDict);
}

void PDFAUX::createMeasuringViewPort(const OdGsDCRect& bbox, PDFDocument &PDFDoc, PDFPageDictionary* pPage, double iMeasureScale)
{
  PDFArrayPtr pVPArray = PDFArray::createObject(PDFDoc);
  PDFViewportDictionaryPtr pVP(PDFViewportDictionary::createObject(PDFDoc));

  PDFRectanglePtr rect = PDFRectangle::createObject(PDFDoc);
  rect->set(bbox.m_min.x, bbox.m_min.y, bbox.m_max.x, bbox.m_max.y);
  pVP->setBBox(rect);

  PDFMeasureDictionaryPtr pMeasure = PDFMeasureDictionary::createObject(PDFDoc);
  pMeasure->setSubtype(PDFName::createObject(PDFDoc, "RL"));
  PDFTextStringPtr pEmptyStr = PDFTextString::createObject(PDFDoc, L"", false);
  pMeasure->setR(pEmptyStr);

  PDFArrayPtr pADArray = PDFArray::createObject(PDFDoc);
  PDFArrayPtr pXArray = PDFArray::createObject(PDFDoc);
  PDFNumberFormatDictionaryPtr pNumF1 = PDFNumberFormatDictionary::createObject(PDFDoc);
  PDFNumberFormatDictionaryPtr pNumF2 = PDFNumberFormatDictionary::createObject(PDFDoc);
  pNumF1->setU(pEmptyStr);
  pNumF1->setC(PDFNumber::createObject(PDFDoc, 1, false));
  pNumF2->setU(pEmptyStr);
  pNumF2->setC(PDFNumber::createObject(PDFDoc, iMeasureScale, false));
  pADArray->push_back(pNumF1);
  pXArray->push_back(pNumF2);

  pMeasure->setA(pADArray);
  pMeasure->setD(pADArray);
  pMeasure->setX(pXArray);
  pVP->setMeasure(pMeasure);
  pVPArray->append(pVP);
  pPage->setVP(pVPArray);
}

void PDFAUX::updateContentStreamWithCM(PDFDocument &PDFDoc, PDFPageDictionary* pPage, const PDFExportParams &pParams)
{
  if (!pPage || pParams.export2XObject()) //recording disabled
  {
    return;
  }

  PDFArrayPtr pContent = pPage->getContents();
  if (pContent->size() == 1) //now we can have only 1 CS for page
  {
    PDFContentStreamPtr pCStream = PDFContentStream::createObject(PDFDoc, true);

    OdGeMatrix2d m;
    OdUInt16 geomDPI = pParams.getGeomDPI();
    m.setToScaling(72. / (double)geomDPI);

    pCStream->cm(m.entry[0][0], m.entry[1][0], m.entry[0][1], m.entry[1][1], m.entry[0][2], m.entry[1][2]);
    pContent->insert(pContent->begin(), pCStream);
  }
}

void PDFAUX::createDocumentInformation(PDFDocument &PDFDoc, const PDFExportParams &pParams)
{
  PDFDocumentInformationPtr pDI = PDFDocumentInformation::createObject(PDFDoc, true);
  PDFMetadataStreamPtr pMD;
  if (PDFDoc.isPdfA())
    pMD = PDFMetadataStream::createObject(PDFDoc, true);

  if (!pParams.title().isEmpty())
  {
    PDFTextStringPtr pTitle = PDFTextString::createObject(PDFDoc, false);
    TD_PDF_HELPER_FUNCS::getUnicodeTextString(pParams.title(), pTitle);
    pTitle->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'
    pDI->setTitle(pTitle);
    if (!pMD.isNull())
      pMD->setTitle(pParams.title());
  }

  if (!pParams.author().isEmpty())
  {
    PDFTextStringPtr pAuthor = PDFTextString::createObject(PDFDoc, false);
    TD_PDF_HELPER_FUNCS::getUnicodeTextString(pParams.author(), pAuthor);
    pAuthor->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'
    pDI->setAuthor(pAuthor);
    if (!pMD.isNull())
      pMD->setAuthor(pParams.author());
  }

  if (!pParams.subject().isEmpty())
  {
    PDFTextStringPtr pSubject = PDFTextString::createObject(PDFDoc, false);
    TD_PDF_HELPER_FUNCS::getUnicodeTextString(pParams.subject(), pSubject);
    pSubject->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'
    pDI->setSubject(pSubject);
    if (!pMD.isNull())
      pMD->setSubject(pParams.subject());
  }

  if (!pParams.keywords().isEmpty())
  {
    PDFTextStringPtr pKeywords = PDFTextString::createObject(PDFDoc, false);
    TD_PDF_HELPER_FUNCS::getUnicodeTextString(pParams.keywords(), pKeywords);
    pKeywords->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'
    pDI->setKeywords(pKeywords);
    if (!pMD.isNull())
      pMD->setKeywords(pParams.keywords());
  }

  if (!pParams.creator().isEmpty())
  {
    PDFTextStringPtr pCreator = PDFTextString::createObject(PDFDoc, false);
    TD_PDF_HELPER_FUNCS::getUnicodeTextString(pParams.creator(), pCreator);
    pCreator->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'
    pDI->setCreator(pCreator);
    if (!pMD.isNull())
      pMD->setCreator(pParams.creator());
  }

  if (!pParams.producer().isEmpty())
  {
    PDFTextStringPtr pProducer = PDFTextString::createObject(PDFDoc, false);
    TD_PDF_HELPER_FUNCS::getUnicodeTextString(pParams.producer(), pProducer);
    pProducer->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'
    pDI->setProducer(pProducer);
    if (!pMD.isNull())
      pMD->setProducer(pParams.producer());
  }
  else
  {
    OdAnsiString str;
    str.format(("ODA PDF Export v%d.%d.%d.%d (v%d.%d.%d.%d)"), PDF_EXPORT_MAJOR_VERSION, PDF_EXPORT_MINOR_VERSION, PDF_EXPORT_MAJOR_BUILD_VERSION, PDF_EXPORT_MINOR_BUILD_VERSION, PDF_TOOLKIT_MAJOR_VERSION, PDF_TOOLKIT_MINOR_VERSION, PDF_TOOLKIT_MAJOR_BUILD_VERSION, PDF_TOOLKIT_MINOR_BUILD_VERSION);
    pDI->setProducer(PDFTextString::createObject(PDFDoc, str, false));
    if (!pMD.isNull())
      pMD->setProducer(str);
  }

  PDFDatePtr pCreationDate = PDFDate::createObject(PDFDoc);
  OdTimeStamp curDate(OdTimeStamp::kInitUniversalTime);
  pCreationDate->set(curDate, 0, 0);

  pDI->setCreationDate(pCreationDate);
  if (!pMD.isNull())
  {
    pMD->setCreationDate(curDate);

    pMD->setPdfALevelConf((OdUInt16)pParams.archived(), 2);//conformance B is hardcoded
    PDFCatalogDictionaryPtr pCatalog = PDFDoc.Root();
    pCatalog->setMetadata(pMD);
  }

  PDFDoc.setDocumentInformation(pDI);
}

void PDFAUX::CreateOutputIntent(PDFDocument &PDFDoc)
{
  PDFCatalogDictionaryPtr pCatalog = PDFDoc.Root();

  PDFArrayPtr pIntents = pCatalog->Find("OutputIntents");
  if (pIntents.isNull())
  {
    PDFOutputIntentsDictionaryPtr pIntent = PDFOutputIntentsDictionary::createObject(PDFDoc);
    PDFICCBasedStreamPtr pDestOutProfile = PDFICCBasedStream::createObject(PDFDoc, true);
    pIntent->setDestOutputProfile(pDestOutProfile);

    pIntents = PDFArray::createObject(PDFDoc);
    pIntents->append(pIntent);
    pCatalog->setOutputIntents(pIntents);
  }
}

PDFIndexedRGBColorSpacePtr PDFAUX::CreateIndexedRGBColorSpace(const ODCOLORREF *pRGB, OdUInt32 num, PDFDocument &PDFDoc)
{
  PDFIndexedRGBColorSpacePtr pIndexed = PDFIndexedRGBColorSpace::createObject(PDFDoc, true);

  PDFRGBStreamPtr pRGBStream( pIndexed->getLookup() );

  for(OdUInt32 f=0; f<num; ++f, ++pRGB)
  {
    pRGBStream->addRGB(ODGETRED(*pRGB), ODGETGREEN(*pRGB), ODGETBLUE(*pRGB));
  }

  return pIndexed;
}

PDFIndexedRGBColorSpacePtr PDFAUX::CreateIndexedRGBColorSpace(const OdUInt8 *pRGB, OdUInt32 num, PDFDocument &PDFDoc)
{
  PDFIndexedRGBColorSpacePtr pIndexed = PDFIndexedRGBColorSpace::createObject(PDFDoc, true);

  PDFRGBStreamPtr(pIndexed->getLookup())->addRGB(pRGB, num);

  return pIndexed;
}

PDFResourceDictionaryPtr PDFAUX::createResDictionary4DWG(PDFDocument &PDFDoc)
{
  PDFResourceDictionaryPtr pResDic = PDFResourceDictionary::createObject(PDFDoc, true);
  return pResDic;
}

void PDFAUX::createIndexedDWGPalette(PDFResourceDictionaryPtr pResDic, const ODCOLORREF *pRGB, OdUInt32 num, PDFDocument &PDFDoc)
{
  PDFSubDictionaryPtr pSubColorDict = pResDic->getColorSpace();
  PDFIndexedRGBColorSpacePtr pIndexed = CreateIndexedRGBColorSpace(pRGB, num, PDFDoc);
  pSubColorDict->AddItem(("DWGPalette"), pIndexed );
}

class OdGiRasterImageMaskWrapper : public OdGiRasterImageWrapper
{
public:
  virtual OdGiRasterImage::PixelFormatInfo pixelFormat() const
  {
    OdGiRasterImage::PixelFormatInfo pf = OdGiRasterImageWrapper::pixelFormat();
    return pf;
  }

  virtual OdUInt32 scanLinesAlignment() const 
  { 
    return 1; 
  }
};

}
