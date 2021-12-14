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

#include <OdaCommon.h>
#include "DgnExportText.h"
#include "DgnExportCommon.h"
#include "DgnExportContext.h"
#include <DgText.h>
#include <DbText.h>
#include <DgShape.h>
#include <DbMText.h>
#include <DgLineString.h>
#include <FdField.h>
#include <DbFcf.h>

namespace TD_DGN_EXPORT {

//---------------------------------------------------------------------------------------------------

void OdDbTextExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbText* pDbText = (OdDbText*)(pDbEntity);

  if (pDbText->hasFields())
  {
    ::odrxDynamicLinker()->loadApp(OdExFieldEvaluatorModuleName).release();
    pDbText->upgradeOpen();
    OdDbObjectIdArray fields;
    fields.append(pDbEntity->objectId());
    ::oddbEvaluateFields(pDbEntity->database(), 0xFF, &fields, 0, OdString::kEmpty);
    pDbText->convertFieldToText();
    pDbText->downgradeOpen();
  }

  OdDgGraphicsElementPtr pDgText;

  if( OdDgnExportContext::is3d() )
  {
    OdDgText3dPtr pDgText3d = OdDgText3d::createObject();
    pDgText3d->setDatabaseDefaults(owner->database());
    pDgText3d->setText(pDbText->textString());

    OdDgElementId idDgTextStyle = OdDgnExportContext::getElementId(pDbText->textStyle());

    if( !idDgTextStyle.isNull() )
      pDgText3d->applyTextStyle(idDgTextStyle, false, 1.0 / OdDgnExportContext::getMasterToUORsScale() );

    pDgText3d->setHeightMultiplier(pDbText->height());
    pDgText3d->setLengthMultiplier(pDgText3d->getHeightMultiplier() * pDbText->widthFactor());
    pDgText3d->setSlant(pDbText->oblique());
      
    OdGeMatrix3d matRotation;
    OdGeVector3d vrZAxis = pDbText->normal();
    OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
    double dRotation = pDbText->rotation();

    if( fabs(dRotation) < 1.e16 )
      vrXAxis.rotateBy(dRotation, OdGeVector3d::kZAxis);

    vrXAxis.transformBy(OdGeMatrix3d::planeToWorld(vrZAxis));
    matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrXAxis, vrZAxis.crossProduct(vrXAxis), vrZAxis);
    OdGeQuaternion quatRot;
    quatRot.set(matRotation);
    pDgText3d->setRotation(quatRot);
    OdDg::TextJustification uDgJust = getDgTextJustification(pDbText->horizontalMode(), pDbText->verticalMode());
    pDgText3d->setJustification(uDgJust);

    if (pDbText->isMirroredInX())
      pDgText3d->setBackwardsFlag(true);

    if (pDbText->isMirroredInY())
      pDgText3d->setUpsidedownFlag(true);

    pDgText3d->setOrigin(pDbText->position());

    pDgText = pDgText3d;
  }
  else
  {
    OdDgText2dPtr pDgText2d = OdDgText2d::createObject();
    pDgText2d->setDatabaseDefaults(owner->database());
    pDgText2d->setText(pDbText->textString());

    OdDgElementId idDgTextStyle = OdDgnExportContext::getElementId(pDbText->textStyle());

    if (!idDgTextStyle.isNull())
      pDgText2d->applyTextStyle(idDgTextStyle, false, 1.0 / OdDgnExportContext::getMasterToUORsScale());

    pDgText2d->setHeightMultiplier(pDbText->height());
    pDgText2d->setLengthMultiplier(pDgText2d->getHeightMultiplier() * pDbText->widthFactor());

    pDgText2d->setSlant(pDbText->oblique());
    pDgText2d->setRotation(pDbText->rotation());
    OdDg::TextJustification uDgJust = getDgTextJustification(pDbText->horizontalMode(), pDbText->verticalMode());
    pDgText2d->setJustification(uDgJust);

    if (pDbText->isMirroredInY())
      pDgText2d->setUpsidedownFlag(true);

    if (pDbText->isMirroredInX())
      pDgText2d->setBackwardsFlag(true);

    pDgText2d->setOrigin(pDbText->position().convert2d());
    pDgText = pDgText2d;
  }

  if( !pDgText.isNull() )
  {
    pDgText->setThickness(pDbText->thickness());
    addElementToDatabase(pDgText, owner);
	  copyElementProperties(pDgText, pDbText, owner->database());
	  appendOdDgElementToOwner(owner, pDgText);

	  OdDgnExportContext::addDgnElement(pDbText->objectId(), pDgText->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

struct OdDgnExportMTextItem
{
  enum OdDgnExportMTextItemType
  {
    kText       = 0,
    kNewLine    = 1,
    kTab        = 2,
    kParagraph  = 3
  };

  OdDgnExportMTextItemType m_uType;
  OdDgGraphicsElementPtr   m_pText;
  OdDbMTextIndent          m_paragraphIndent;
};

struct OdDgnExportMTextContext
{
  OdDgDatabase*                   m_pDb;
  OdDbMText*                      m_pMText;
  OdDbMTextIndents                m_indents;
  OdArray<OdDgnExportMTextItem>   m_arrItems;
  OdUInt32                        m_uParagraphIndex;
  OdGeMatrix3d                    m_matRotation;
  OdUInt32                        m_nTexts;
};

//---------------------------------------------------------------------------------------------------

static int mTextFragmentProcessing(OdDbMTextFragment *pFragment, void *out)
{
  OdDgnExportMTextContext* pMTextContext = (OdDgnExportMTextContext*)out;

  // add paragraph

  if( pFragment->newParagraph )
  {
    OdDgnExportMTextItem itemParagraph;
    itemParagraph.m_uType = OdDgnExportMTextItem::kParagraph;

    if( pMTextContext->m_uParagraphIndex >= pMTextContext->m_indents.size() )
      itemParagraph.m_paragraphIndent = pMTextContext->m_indents.last();
    else
      itemParagraph.m_paragraphIndent = pMTextContext->m_indents[pMTextContext->m_uParagraphIndex];

    pMTextContext->m_arrItems.push_back(itemParagraph);

    pMTextContext->m_uParagraphIndex++;
  }

  if(pFragment->text == L"\t")
  {
    OdDgnExportMTextItem itemTab;
    itemTab.m_uType = OdDgnExportMTextItem::kTab;
    pMTextContext->m_arrItems.push_back(itemTab);
  }
  else if( !pFragment->text.isEmpty() )
  {
    OdDgGraphicsElementPtr pGrElm;

    if( OdDgnExportContext::is3d() )
    {
      OdDgText3dPtr pText3d = OdDgText3d::createObject();
      pText3d->setDatabaseDefaults(pMTextContext->m_pDb);
      copyElementProperties(pText3d, pMTextContext->m_pMText, pMTextContext->m_pDb, false);

      pText3d->setOrigin(pFragment->location);

      OdGeQuaternion quatRot;
      quatRot.set(pMTextContext->m_matRotation);

      pText3d->setRotation(quatRot);

      pText3d->setText(pFragment->text);
      pText3d->setHeightMultiplier(pFragment->capsHeight);
      pText3d->setLengthMultiplier(pFragment->capsHeight * pFragment->widthFactor);
      pText3d->setSlant(pFragment->obliqueAngle);
      pText3d->setSpacing(pFragment->trackingFactor, OdDgTextExtendedProperties::sAcadInterCharSpacing);
      pText3d->setVerticalFlag(pFragment->vertical);
      pText3d->setUnderlineFlag(pFragment->underlined);
      pText3d->setOverlineFlag(pFragment->overlined);
      pText3d->setColorIndex(getDgnEntityColorIndex(pFragment->color, pMTextContext->m_pDb));

      if( pFragment->stackTop  )
      {
        pText3d->setStackedFractionSection(1);
        pText3d->setStackedFractionType(2);
      }
      else
        pText3d->setStackedFractionSection(0);

      
      if (!pFragment->fontname.isEmpty())
      {
        pText3d->setFontEntryId(getDgnFontEntryId(pFragment->fontname, pMTextContext->m_pDb, false));
        pText3d->setBoldFlag(pFragment->bold);
        pText3d->setItalics(pFragment->italic);
      }
      else if (!pFragment->font.isEmpty())
      {
        pText3d->setFontEntryId(getDgnFontEntryId(pFragment->font, pMTextContext->m_pDb, true));

        if (!pFragment->bigfont.isEmpty())
          pText3d->setShxBigFontEntryId(getDgnFontEntryId(pFragment->bigfont, pMTextContext->m_pDb, true));
      }

      pGrElm = pText3d;
    }
    else
    {
      OdDgText2dPtr pText2d = OdDgText2d::createObject();

      pText2d->setDatabaseDefaults(pMTextContext->m_pDb);
      copyElementProperties(pText2d, pMTextContext->m_pMText, pMTextContext->m_pDb, false);

      pText2d->setOrigin(pFragment->location.convert2d());
      pText2d->setRotation(pMTextContext->m_pMText->rotation());

      pText2d->setText(pFragment->text);
      pText2d->setHeightMultiplier(pFragment->capsHeight);
      pText2d->setLengthMultiplier(pFragment->capsHeight * pFragment->widthFactor);
      pText2d->setSlant(pFragment->obliqueAngle);
      pText2d->setSpacing(pFragment->trackingFactor, OdDgTextExtendedProperties::sAcadInterCharSpacing);
      pText2d->setVerticalFlag(pFragment->vertical);
      pText2d->setUnderlineFlag(pFragment->underlined);
      pText2d->setOverlineFlag(pFragment->overlined);
      pText2d->setColorIndex(getDgnEntityColorIndex(pFragment->color, pMTextContext->m_pDb));

      if (pFragment->stackTop)
      {
        pText2d->setStackedFractionSection(1);
        pText2d->setStackedFractionType(2);
      }
      else
        pText2d->setStackedFractionSection(0);

      if (!pFragment->fontname.isEmpty())
      {
        pText2d->setFontEntryId(getDgnFontEntryId(pFragment->fontname, pMTextContext->m_pDb, false));
        pText2d->setBoldFlag(pFragment->bold);
        pText2d->setItalics(pFragment->italic);
      }
      else if (!pFragment->font.isEmpty())
      {
        pText2d->setFontEntryId(getDgnFontEntryId(pFragment->font, pMTextContext->m_pDb, true));

        if (!pFragment->bigfont.isEmpty())
          pText2d->setShxBigFontEntryId(getDgnFontEntryId(pFragment->bigfont, pMTextContext->m_pDb, true));
      }

      pGrElm = pText2d;
    }

    if( !pGrElm.isNull() )
    {
      OdDgnExportMTextItem itemText;
      itemText.m_uType = OdDgnExportMTextItem::kText;
      itemText.m_pText = pGrElm;
      pMTextContext->m_arrItems.push_back(itemText);
      pMTextContext->m_nTexts++;
    }
  }

  if( pFragment->lineBreak )
  {
    OdDgnExportMTextItem itemNewLine;
    itemNewLine.m_uType = OdDgnExportMTextItem::kNewLine;
    pMTextContext->m_arrItems.push_back(itemNewLine);
  }

  return 1;
}

//---------------------------------------------------------------------------------------------------

OdGePoint3d correctMTextLocation( const OdGePoint3d& ptOrigin,
                                    const OdGeVector3d& vrXDir,
                                      const OdGeVector3d& vrYDir,
                                        const OdDbMText::AttachmentPoint& uAttach,
                                          double dWidth,
                                            double dHeight,
                                              const OdDg::TextJustification& uDgJust)
{
  OdGePoint3d ptRet = ptOrigin;

  switch( uAttach )
  {
    case OdDbMText::kTopLeft:
    {
      switch( uDgJust )
      {
        case OdDg::kLeftTop: 
          break;

        case OdDg::kLeftCenter:
          ptRet -= vrYDir * dHeight / 2.0; break;

        case OdDg::kLeftBottom:
        case OdDg::kLeftDescender:
          ptRet -= vrYDir * dHeight; break;

        case OdDg::kCenterTop:
          ptRet += vrXDir * dWidth / 2.0; break;

        case OdDg::kCenterCenter:
        {
          ptRet += vrXDir * dWidth / 2.0;
          ptRet -= vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kCenterBottom:
        case OdDg::kCenterDescender:
        {
          ptRet += vrXDir * dWidth / 2.0;
          ptRet -= vrYDir * dHeight;
        } break;

        case OdDg::kRightTop:
          ptRet += vrXDir * dWidth; break;

        case OdDg::kRightCenter:
        {
          ptRet += vrXDir * dWidth;
          ptRet -= vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kRightBottom:
        case OdDg::kRightDescender:
        {
          ptRet += vrXDir * dWidth;
          ptRet -= vrYDir * dHeight;
        } break;
      }
    } break;

    case OdDbMText::kTopCenter:
    {
      switch (uDgJust)
      {
        case OdDg::kLeftTop:
          ptRet -= vrXDir * dWidth / 2.0; break;

        case OdDg::kLeftCenter:
        {
          ptRet -= vrXDir * dWidth / 2.0;
          ptRet -= vrYDir * dHeight / 2.0; 
        } break;

        case OdDg::kLeftBottom:
        case OdDg::kLeftDescender:
        {
          ptRet -= vrXDir * dWidth / 2.0;
          ptRet -= vrYDir * dHeight;
        } break;

        case OdDg::kCenterTop:
          break;

        case OdDg::kCenterCenter:
          ptRet -= vrYDir * dHeight / 2.0; break;

        case OdDg::kCenterBottom:
        case OdDg::kCenterDescender:
          ptRet -= vrYDir * dHeight; break;

        case OdDg::kRightTop:
          ptRet += vrXDir * dWidth / 2.0; break;

        case OdDg::kRightCenter:
        {
          ptRet += vrXDir * dWidth / 2.0;
          ptRet -= vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kRightBottom:
        case OdDg::kRightDescender:
        {
          ptRet += vrXDir * dWidth / 2.0;
          ptRet -= vrYDir * dHeight;
        } break;
      }
    } break;

    case OdDbMText::kTopRight:
    {
      switch (uDgJust)
      {
        case OdDg::kLeftTop:
          ptRet -= vrXDir * dWidth; break;

        case OdDg::kLeftCenter:
        {
          ptRet -= vrXDir * dWidth;
          ptRet -= vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kLeftBottom:
        case OdDg::kLeftDescender:
        {
          ptRet -= vrXDir * dWidth;
          ptRet -= vrYDir * dHeight;
        }break;

        case OdDg::kCenterTop:
        {
          ptRet -= vrXDir * dWidth / 2.0;
        } break;

        case OdDg::kCenterCenter:
        {
          ptRet -= vrXDir * dWidth / 2.0;
          ptRet -= vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kCenterBottom:
        case OdDg::kCenterDescender:
        {
          ptRet -= vrXDir * dWidth / 2.0;
          ptRet -= vrYDir * dHeight;
        } break;

        case OdDg::kRightTop:
          {}break;

        case OdDg::kRightCenter:
          ptRet -= vrYDir * dHeight / 2.0; break;

        case OdDg::kRightBottom:
        case OdDg::kRightDescender:
          ptRet -= vrYDir * dHeight; break;
      }
    } break;

    case OdDbMText::kMiddleLeft:
    {
      switch (uDgJust)
      {
        case OdDg::kLeftTop:
          ptRet += vrYDir * dHeight / 2.0; break;

        case OdDg::kLeftCenter:
          break;

        case OdDg::kLeftBottom:
        case OdDg::kLeftDescender:
          ptRet -= vrYDir * dHeight / 2.0; break;

        case OdDg::kCenterTop:
        {
          ptRet += vrXDir * dWidth / 2.0;
          ptRet += vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kCenterCenter:
          ptRet += vrXDir * dWidth / 2.0; break;

        case OdDg::kCenterBottom:
        case OdDg::kCenterDescender:
        {
          ptRet += vrXDir * dWidth / 2.0;
          ptRet -= vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kRightTop:
        {
          ptRet += vrYDir * dHeight / 2.0;
          ptRet += vrXDir * dWidth;
        } break;

        case OdDg::kRightCenter:
          ptRet += vrXDir * dWidth; break;

        case OdDg::kRightBottom:
        case OdDg::kRightDescender:
        {
          ptRet += vrXDir * dWidth;
          ptRet -= vrYDir * dHeight / 2.0;
        } break;
      }
    } break;

    case OdDbMText::kMiddleCenter:
    {
      switch (uDgJust)
      {
        case OdDg::kLeftTop:
        {
          ptRet += vrYDir * dHeight / 2.0;
          ptRet -= vrXDir * dWidth / 2.0;
        } break;

        case OdDg::kLeftCenter:
          ptRet -= vrXDir * dWidth / 2.0; break;

        case OdDg::kLeftBottom:
        case OdDg::kLeftDescender:
        {
          ptRet -= vrXDir * dWidth / 2.0;
          ptRet -= vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kCenterTop:
          ptRet += vrYDir * dHeight / 2.0; break;

        case OdDg::kCenterCenter:
          break;

        case OdDg::kCenterBottom:
        case OdDg::kCenterDescender:
          ptRet -= vrYDir * dHeight / 2.0; break;

        case OdDg::kRightTop:
        {
          ptRet += vrYDir * dHeight / 2.0;
          ptRet += vrXDir * dWidth / 2.0;
        } break;

        case OdDg::kRightCenter:
          ptRet += vrXDir * dWidth / 2.0; break;

        case OdDg::kRightBottom:
        case OdDg::kRightDescender:
        {
          ptRet += vrXDir * dWidth / 2.0;
          ptRet -= vrYDir * dHeight / 2.0;
        } break;
      }
    } break;

    case OdDbMText::kMiddleRight:
    {
      switch (uDgJust)
      {
        case OdDg::kLeftTop:
        {
          ptRet += vrYDir * dHeight / 2.0;
          ptRet -= vrXDir * dWidth;
        } break;

        case OdDg::kLeftCenter:
          ptRet -= vrXDir * dWidth; break;

        case OdDg::kLeftBottom:
        case OdDg::kLeftDescender:
        {
          ptRet -= vrXDir * dWidth;
          ptRet -= vrYDir * dHeight / 2.0;
        }break;

        case OdDg::kCenterTop:
        {
          ptRet += vrYDir * dHeight / 2.0;
          ptRet -= vrXDir * dWidth / 2.0;
        } break;

        case OdDg::kCenterCenter:
          ptRet -= vrXDir * dWidth / 2.0; break;

        case OdDg::kCenterBottom:
        case OdDg::kCenterDescender:
        {
          ptRet -= vrXDir * dWidth / 2.0;
          ptRet -= vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kRightTop:
          ptRet += vrYDir * dHeight / 2.0; break;

        case OdDg::kRightCenter:
          break;

        case OdDg::kRightBottom:
        case OdDg::kRightDescender:
          ptRet -= vrYDir * dHeight / 2.0; break;
      }
    } break;

    case OdDbMText::kBottomLeft:
    {
      switch (uDgJust)
      {
        case OdDg::kLeftTop:
          ptRet += vrYDir * dHeight; break;

        case OdDg::kLeftCenter:
          ptRet += vrYDir * dHeight / 2.0; break;

        case OdDg::kLeftBottom:
        case OdDg::kLeftDescender:
          break;

        case OdDg::kCenterTop:
        {
          ptRet += vrXDir * dWidth / 2.0;
          ptRet += vrYDir * dHeight;
        } break;

        case OdDg::kCenterCenter:
        {
          ptRet += vrXDir * dWidth / 2.0;
          ptRet += vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kCenterBottom:
        case OdDg::kCenterDescender:
          ptRet += vrXDir * dWidth / 2.0; break;

        case OdDg::kRightTop:
        {
          ptRet += vrYDir * dHeight;
          ptRet += vrXDir * dWidth;
        } break;

        case OdDg::kRightCenter:
        {
          ptRet += vrXDir * dWidth;
          ptRet += vrYDir * dHeight;
        } break;

        case OdDg::kRightBottom:
        case OdDg::kRightDescender:
          ptRet += vrXDir * dWidth; break;
      }
    } break;

    case OdDbMText::kBottomCenter:
    {
      switch (uDgJust)
      {
        case OdDg::kLeftTop:
        {
          ptRet += vrYDir * dHeight;
          ptRet -= vrXDir * dWidth / 2.0;
        } break;

        case OdDg::kLeftCenter:
        {
          ptRet -= vrXDir * dWidth / 2.0;
          ptRet += vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kLeftBottom:
        case OdDg::kLeftDescender:
          ptRet -= vrXDir * dWidth / 2.0; break;

        case OdDg::kCenterTop:
          ptRet += vrYDir * dHeight; break;

        case OdDg::kCenterCenter:
          ptRet += vrYDir * dHeight / 2.0; break;

        case OdDg::kCenterBottom:
        case OdDg::kCenterDescender:
          break;

        case OdDg::kRightTop:
        {
          ptRet += vrYDir * dHeight;
          ptRet += vrXDir * dWidth / 2.0;
        }break;

        case OdDg::kRightCenter:
        {
          ptRet += vrXDir * dWidth / 2.0;
          ptRet += vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kRightBottom:
        case OdDg::kRightDescender:
          ptRet += vrXDir * dWidth / 2.0; break;
      }
    } break;

    case OdDbMText::kBottomRight:
    {
      switch (uDgJust)
      {
        case OdDg::kLeftTop:
        {
          ptRet += vrYDir * dHeight;
          ptRet -= vrXDir * dWidth;
        }break;

        case OdDg::kLeftCenter:
        {
          ptRet -= vrXDir * dWidth;
          ptRet += vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kLeftBottom:
        case OdDg::kLeftDescender:
          ptRet -= vrXDir * dWidth; break;

        case OdDg::kCenterTop:
        {
          ptRet += vrYDir * dHeight;
          ptRet -= vrXDir * dWidth / 2.0;
        } break;

        case OdDg::kCenterCenter:
        {
          ptRet -= vrXDir * dWidth / 2.0;
          ptRet += vrYDir * dHeight / 2.0;
        } break;

        case OdDg::kCenterBottom:
        case OdDg::kCenterDescender:
          ptRet -= vrXDir * dWidth / 2.0; break;

        case OdDg::kRightTop:
          ptRet += vrYDir * dHeight; break;

        case OdDg::kRightCenter:
          ptRet += vrYDir * dHeight / 2.0; break;

        case OdDg::kRightBottom:
        case OdDg::kRightDescender:
          break;
      }
    } break;
  }

  return ptRet;
}

//---------------------------------------------------------------------------------------------------

bool isEqualIndent(const OdDbMTextIndent& indent1, const OdDbMTextIndent& indent2)
{
  if( !OdEqual(indent1.firstLineInd, indent2.firstLineInd) )
    return false;

  if (!OdEqual(indent1.paragraphInd, indent2.paragraphInd))
    return false;

  if ( indent1.tabs.size() != indent2.tabs.size() )
    return false;

  for( OdUInt32 i = 0; i < indent1.tabs.size(); i++ )
  {
    if (!OdEqual(indent1.tabs[i], indent2.tabs[i]))
      return false;
  }

  return true;
}

//---------------------------------------------------------------------------------------------------

void setIndent( OdDgGraphicsElement* pTextElm, const OdDbMTextIndent& indent )
{
  bool bDefaultIdent = false;

  if( OdZero(indent.firstLineInd) && OdZero(indent.paragraphInd) )
  {
    if ((indent.tabs.size() == 0) || (indent.tabs.size() == 1 && OdZero(indent.tabs[0])))
      bDefaultIdent = true;
  }

  if (pTextElm->isKindOf(OdDgText2d::desc()))
  {
    OdDgText2d* pText2d = (OdDgText2d*)(pTextElm);
    pText2d->setParagraphIndentationOverride(indent.paragraphInd);
    pText2d->setFirstLineIndentationOverride(indent.firstLineInd);
    pText2d->setTabStopArrayOverride(indent.tabs);
  }
  else if (pTextElm->isKindOf(OdDgText3d::desc()))
  {
    OdDgText3d* pText3d = (OdDgText3d*)(pTextElm);
    pText3d->setParagraphIndentationOverride(indent.paragraphInd);
    pText3d->setFirstLineIndentationOverride(indent.firstLineInd);
    pText3d->setTabStopArrayOverride(indent.tabs);
  }
  else if (pTextElm->isKindOf(OdDgTextNode2d::desc()) && !bDefaultIdent )
  {
    OdDgTextNode2d* pTextNode2d = (OdDgTextNode2d*)(pTextElm);
    pTextNode2d->setParagraphIndentation(indent.paragraphInd);
    pTextNode2d->setFirstLineIndentation(indent.firstLineInd);
    pTextNode2d->setTabStopArray(indent.tabs);
  }
  else if (pTextElm->isKindOf(OdDgTextNode3d::desc()) && !bDefaultIdent )
  {
    OdDgTextNode3d* pTextNode3d = (OdDgTextNode3d*)(pTextElm);
    pTextNode3d->setParagraphIndentation(indent.paragraphInd);
    pTextNode3d->setFirstLineIndentation(indent.firstLineInd);
    pTextNode3d->setTabStopArray(indent.tabs);
  }
}

//---------------------------------------------------------------------------------------------------

void addExtraSymbolsToText(OdDgGraphicsElement* pTextElm, const OdString& strSymbols)
{
  OdArray<OdDgTextExtendedProperties::OdDgSpecialSymbol> arrSymbols;

  bool bHasNewLines = false;
  bool bHasTabs     = false;

  for( OdInt32 i = 0; i < strSymbols.getLength(); i++ )
  {
    if (strSymbols.getAt(i) == L't')
    {
      arrSymbols.push_back(OdDgTextExtendedProperties::kTab);
      bHasTabs = true;
    }
    else
    {
      arrSymbols.push_back(OdDgTextExtendedProperties::kNewLine);
      bHasNewLines = true;
    }
  }

  if( pTextElm->isKindOf(OdDgText2d::desc()) )
  {
    OdDgText2d* pText2d = (OdDgText2d*)(pTextElm);

    if( bHasTabs )
    {
      pText2d->setSpecialSymbolsAfterText(arrSymbols);

      if( bHasNewLines )
        pText2d->setCrCount(pText2d->getCrCount() + 1);
    }
    else
      pText2d->setCrCount(arrSymbols.size());
  }
  else if (pTextElm->isKindOf(OdDgText3d::desc()))
  {
    OdDgText3d* pText3d = (OdDgText3d*)(pTextElm);

    if(bHasTabs)
    {
      pText3d->setSpecialSymbolsAfterText(arrSymbols);

      if( bHasNewLines )
        pText3d->setCrCount(pText3d->getCrCount() + 1);
    }
    else
      pText3d->setCrCount(arrSymbols.size());
  }
  else if (pTextElm->isKindOf(OdDgTextNode2d::desc()))
  {
    OdDgTextNode2d* pTextNode2d = (OdDgTextNode2d*)(pTextElm);
    pTextNode2d->setTextNodeStartSpecialSymbols(arrSymbols);
  }
  else if (pTextElm->isKindOf(OdDgTextNode3d::desc()))
  {
    OdDgTextNode3d* pTextNode3d = (OdDgTextNode3d*)(pTextElm);
    pTextNode3d->setTextNodeStartSpecialSymbols(arrSymbols);
  }
}

//---------------------------------------------------------------------------------------------------

OdDgGraphicsElementPtr convertMText(OdDbMText* pDbMText, OdDgDatabase* pDgDb, bool bForceMText )
{
  OdDgGraphicsElementPtr pDgMText;

  OdDbMTextIndents arrIndents;
  pDbMText->getParagraphsIndent(arrIndents);
  OdGeMatrix3d matRotation = OdGeMatrix3d::planeToWorld(pDbMText->normal());
  matRotation = OdGeMatrix3d::rotation( pDbMText->rotation(), pDbMText->normal()) * matRotation;
  matRotation.setTranslation(OdGeVector3d::kIdentity);

  OdGeVector3d vrX = OdGeVector3d::kXAxis, vrY = OdGeVector3d::kYAxis, vrZ = OdGeVector3d::kZAxis;

  if (matRotation != OdGeMatrix3d::kIdentity)
  {
    vrX.transformBy(matRotation);
    vrY.transformBy(matRotation);
    vrZ.transformBy(matRotation);
    vrX.normalize();
    vrY.normalize();
    vrZ.normalize();
    matRotation.setCoordSystem(OdGePoint3d::kOrigin, vrX, vrY, vrZ);
  }

  OdDgnExportMTextContext mTextContext;
  mTextContext.m_pDb = pDgDb;
  mTextContext.m_uParagraphIndex = 0;
  mTextContext.m_indents = arrIndents;
  mTextContext.m_matRotation = matRotation;
  mTextContext.m_pMText = pDbMText;
  mTextContext.m_nTexts = 0;

  pDbMText->explodeFragments(mTextFragmentProcessing, &mTextContext);

  OdDgGraphicsElementPtr pFirstText;

  for (OdUInt32 k = 0; k < mTextContext.m_arrItems.size(); k++)
  {
    if (mTextContext.m_arrItems[k].m_uType != OdDgnExportMTextItem::kText)
      continue;

    pFirstText = mTextContext.m_arrItems[k].m_pText;

    if (!pFirstText.isNull())
      break;
  }

  if( (mTextContext.m_nTexts == 1) && !bForceMText )
  {
    pDgMText = pFirstText;
  }
  else if (mTextContext.m_nTexts > 0)
  {
    if (OdDgnExportContext::is3d())
    {
      OdDgTextNode3dPtr pTextNode = OdDgTextNode3d::createObject();
      pTextNode->setDatabaseDefaults(pDgDb);
      copyElementProperties(pTextNode, pDbMText, pDgDb);

      OdDgElementId idDgTextStyle = OdDgnExportContext::getElementId(pDbMText->textStyle());

      if (!idDgTextStyle.isNull())
        pTextNode->applyTextStyle(idDgTextStyle, false, 1.0 / OdDgnExportContext::getMasterToUORsScale());

      OdGeQuaternion quatRot;
      quatRot.set(matRotation);
      pTextNode->setRotation(quatRot);
      OdDg::TextJustification uDgJust = getDgTextJustification(pDbMText->horizontalMode(), pDbMText->verticalMode());
      pTextNode->setJustification(uDgJust);

      OdGePoint3d ptOrigin = pDbMText->location();
      ptOrigin = correctMTextLocation(ptOrigin, vrX, vrY, pDbMText->attachment(), pDbMText->width(), pDbMText->height(), uDgJust);

      pTextNode->setOrigin(ptOrigin);
      pTextNode->setWordWrapTextNodeFlag(true);
      pTextNode->setTextnodeWordWrapLength(pDbMText->width());

      if (pDbMText->lineSpacingStyle() == OdDb::kAtLeast)
        pTextNode->setLineSpacingType(OdDgTextExtendedProperties::kAtLeast);
      else
        pTextNode->setLineSpacingType(OdDgTextExtendedProperties::kExact);

      pTextNode->setLineSpacing(pDbMText->lineSpacingFactor());
      pTextNode->setLengthMultiplier(pDbMText->textHeight() * pTextNode->getLengthMultiplier() / pTextNode->getHeightMultiplier());
      pTextNode->setHeightMultiplier(pDbMText->textHeight());

      pDgMText = pTextNode;
    }
    else
    {
      OdDgTextNode2dPtr pTextNode = OdDgTextNode2d::createObject();
      pTextNode->setDatabaseDefaults(pDgDb);
      copyElementProperties(pTextNode, pDbMText, pDgDb);

      OdDgElementId idDgTextStyle = OdDgnExportContext::getElementId(pDbMText->textStyle());

      if (!idDgTextStyle.isNull())
        pTextNode->applyTextStyle(idDgTextStyle, false, 1.0 / OdDgnExportContext::getMasterToUORsScale());

      pTextNode->setRotation(pDbMText->rotation());
      OdDg::TextJustification uDgJust = getDgTextJustification(pDbMText->horizontalMode(), pDbMText->verticalMode());
      pTextNode->setJustification(uDgJust);

      OdGePoint3d ptOrigin = pDbMText->location();
      ptOrigin = correctMTextLocation(ptOrigin, vrX, vrY, pDbMText->attachment(), pDbMText->width(), pDbMText->height(), uDgJust);

      pTextNode->setOrigin(ptOrigin.convert2d());
      pTextNode->setWordWrapTextNodeFlag(true);
      pTextNode->setTextnodeWordWrapLength(pDbMText->width());

      if (pDbMText->lineSpacingStyle() == OdDb::kAtLeast)
        pTextNode->setLineSpacingType(OdDgTextExtendedProperties::kAtLeast);
      else
        pTextNode->setLineSpacingType(OdDgTextExtendedProperties::kExact);

      pTextNode->setLineSpacing(pDbMText->lineSpacingFactor());
      pTextNode->setLengthMultiplier(pDbMText->textHeight() * pTextNode->getLengthMultiplier() / pTextNode->getHeightMultiplier());
      pTextNode->setHeightMultiplier(pDbMText->textHeight());

      pDgMText = pTextNode;
    }

    OdDgGraphicsElementPtr pLastText = pDgMText;
    OdDbMTextIndent        lastIndent;
    bool                   bInitLastIndent = false;
    OdString               strExtraSymbols;

    for (OdUInt32 k = 0; k < mTextContext.m_arrItems.size(); k++)
    {
      if (mTextContext.m_arrItems[k].m_uType == OdDgnExportMTextItem::kParagraph)
      {
        if (!bInitLastIndent)
        {
          lastIndent = mTextContext.m_arrItems[k].m_paragraphIndent;
          bInitLastIndent = true;
          setIndent(pLastText, lastIndent);
        }
        else
        {
          if (!isEqualIndent(lastIndent, mTextContext.m_arrItems[k].m_paragraphIndent))
          {
            lastIndent = mTextContext.m_arrItems[k].m_paragraphIndent;
            setIndent(pLastText, lastIndent);
          }
        }
      }
      else if (mTextContext.m_arrItems[k].m_uType == OdDgnExportMTextItem::kTab)
      {
        strExtraSymbols += L"t";
      }
      else if (mTextContext.m_arrItems[k].m_uType == OdDgnExportMTextItem::kNewLine)
      {
        strExtraSymbols += L"n";
      }
      else
      {
        if (!strExtraSymbols.isEmpty())
          addExtraSymbolsToText(pLastText, strExtraSymbols);

        strExtraSymbols.empty();

        appendOdDgElementToOwner(pDgMText, mTextContext.m_arrItems[k].m_pText);
        pLastText = mTextContext.m_arrItems[k].m_pText;
      }
    }

    if (!strExtraSymbols.isEmpty())
      addExtraSymbolsToText(pLastText, strExtraSymbols);
  }

  return pDgMText;
}

//---------------------------------------------------------------------------------------------------

void OdDbMTextExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbMText* pDbMText = (OdDbMText*)(pDbEntity);

  if( pDbMText->hasFields() )
  {
    ::odrxDynamicLinker()->loadApp(OdExFieldEvaluatorModuleName).release();
    pDbMText->upgradeOpen();
    OdDbObjectIdArray fields;
    fields.append(pDbEntity->objectId());
    ::oddbEvaluateFields(pDbEntity->database(), 0xFF, &fields, 0, OdString::kEmpty);
    pDbMText->convertFieldToText();
    pDbMText->downgradeOpen();
  }

  OdDgGraphicsElementPtr pDgMText = convertMText(pDbMText, owner->database());

  if (!pDgMText.isNull())
  {
    if( pDbMText->backgroundFillOn() )
    {
      double dOffset = pDbMText->textHeight()*(fabs(pDbMText->getBackgroundScaleFactor()) - 1.0);

      if( pDgMText->isKindOf(OdDgText2d::desc()) )
      {
        OdDgText2dPtr pText2d = pDgMText;
        OdUInt32 uBgColorIndex = 255;

        if( !pDbMText->useBackgroundColorOn() )
          uBgColorIndex = getDgnColorIndex(pDbMText->getBackgroundFillColor(), owner->database());

        pText2d->setBackground(0, 0, uBgColorIndex, uBgColorIndex, OdGePoint2d(dOffset, dOffset));
        pText2d->setBgColorFlag(true);
      }
      else if (pDgMText->isKindOf(OdDgText3d::desc()))
      {
        OdDgText3dPtr pText3d = pDgMText;
        OdUInt32 uBgColorIndex = 255;

        if (!pDbMText->useBackgroundColorOn())
          uBgColorIndex = getDgnColorIndex(pDbMText->getBackgroundFillColor(), owner->database());

        pText3d->setBackground(0, 0, uBgColorIndex, uBgColorIndex, OdGePoint2d(dOffset, dOffset));
        pText3d->setBgColorFlag(true);
      }
      else
      {
        OdGePoint3dArray arrBoundingPoints;
        pDbMText->getBoundingPoints(arrBoundingPoints);

        OdGeVector3d vrTextDirX = arrBoundingPoints[1] - arrBoundingPoints[0];
        OdGeVector3d vrTextDirY = arrBoundingPoints[2] - arrBoundingPoints[0];

        if (!vrTextDirX.isZeroLength())
          vrTextDirX.normalize();

        if (!vrTextDirY.isZeroLength())
          vrTextDirY.normalize();

        arrBoundingPoints[0] = arrBoundingPoints[0] - vrTextDirX * dOffset - vrTextDirY * dOffset;
        arrBoundingPoints[1] = arrBoundingPoints[1] + vrTextDirX * dOffset - vrTextDirY * dOffset;
        arrBoundingPoints[2] = arrBoundingPoints[2] - vrTextDirX * dOffset + vrTextDirY * dOffset;
        arrBoundingPoints[3] = arrBoundingPoints[3] + vrTextDirX * dOffset + vrTextDirY * dOffset;

        OdDgGraphicsElementPtr pShape;

        if (OdDgnExportContext::is3d())
        {
          OdDgShape3dPtr pBgShape = OdDgShape3d::createObject();
          pBgShape->setDatabaseDefaults(owner->database());

          pBgShape->addVertex(arrBoundingPoints[0]);
          pBgShape->addVertex(arrBoundingPoints[1]);
          pBgShape->addVertex(arrBoundingPoints[3]);
          pBgShape->addVertex(arrBoundingPoints[2]);
          pBgShape->addVertex(arrBoundingPoints[0]);

          pShape = pBgShape;
        }
        else
        {
          OdDgShape2dPtr pBgShape = OdDgShape2d::createObject();
          pBgShape->setDatabaseDefaults(owner->database());

          pBgShape->addVertex(arrBoundingPoints[0].convert2d());
          pBgShape->addVertex(arrBoundingPoints[1].convert2d());
          pBgShape->addVertex(arrBoundingPoints[3].convert2d());
          pBgShape->addVertex(arrBoundingPoints[2].convert2d());
          pBgShape->addVertex(arrBoundingPoints[0].convert2d());

          pShape = pBgShape;
        }

        copyElementProperties(pShape, pDbMText, owner->database(), false);

        pShape->setColorIndex(getDgnColorIndex(pDbMText->getBackgroundFillColor(), owner->database()));

        if (!pDbMText->useBackgroundColorOn())
          pShape->setFillColor(pShape->getColorIndex());
        else
          pShape->setFillColor(255);

        addElementToDatabase(pShape, owner);
        appendOdDgElementToOwner(owner, pShape);
      }
    }

    addElementToDatabase(pDgMText, owner);
    appendOdDgElementToOwner(owner, pDgMText);

    OdDgnExportContext::addDgnElement(pDbMText->objectId(), pDgMText->elementId());
  }
}

//---------------------------------------------------------------------------------------------------

void OdDbFcfExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbFcf* pDbFcf = (OdDbFcf*)(pDbEntity);

  OdDgGraphicsElementPtr pCell;

  if (OdDgnExportContext::is3d())
    pCell = OdDgCellHeader3d::createObject();
  else
    pCell = OdDgCellHeader2d::createObject();

  addElementToDatabase(pCell, owner);
  copyElementProperties(pCell, pDbFcf, owner->database());
  appendOdDgElementToOwner(owner, pCell);

  OdDgnExportPE::subExportElement(pDbEntity, pCell);

  OdDgnExportContext::addDgnElement(pDbFcf->objectId(), pCell->elementId());
}

//---------------------------------------------------------------------------------------------------

}
