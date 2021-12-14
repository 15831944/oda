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

// ExText.cpp: implementation of the TEXT command.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DbText.h"

#include "DbSymUtl.h"
#include "DbTextStyleTable.h"


//---------------------------------------------------------------------------------------------------------------------

void _Text_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr  pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr        pDb = pDbCmdCtx->database();

  OdGeVector3d vNormal(ExCommandsUtils::ucsNormal(pDb));

  OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);

  OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

  OdGePoint3d ptPosition, ptAlignment;
  int nKeyIndex;

  for (;;) // While point or "Justify" option are not specified
  {
    nKeyIndex = -1;
    try
    {
      ptPosition = pIO->getPoint(L"Specify start point of text or [Justify/Style]: ", OdEd::kGptDefault,
                                  NULL, L"Justify Style");
    }
    catch(const OdEdKeyword &kwd)
    {
      nKeyIndex = kwd.keywordIndex();
    }

    if (nKeyIndex != 1)
    { // Point was specified or Justification is to be set
      break;
    }

    // select Style
    OdDbObjectId idStyle = pDb->getTEXTSTYLE(); // default text style
    OdDbTextStyleTableRecordPtr pStyle = idStyle.safeOpenObject();

    OdDbTextStyleTablePtr pTextStyleTable = pDb->getTextStyleTableId().openObject();
    OdDbSymbolTableIteratorPtr pIter = pTextStyleTable->newIterator();

    for (;;)
    {
      OdString sName = pIO->getString(L"Enter style name or [?] <" + pStyle->getName() + L">: ");

      if (sName.isEmpty())
        break;           // user pressed Enter, i.e. has left default Style

      if (sName == L"?")          {
        OdString styleNames = L"Text styles: ";
        for(pIter->start(); !pIter->done(); pIter->step())
        {
          styleNames += L"\n" + pIter->getRecord()->getName();
          if (idStyle == pIter->getRecord()->objectId())
            styleNames += L" (current text style)";
        }
        pIO->putString(styleNames);
      }
      else
      {
        idStyle = OdDbSymUtil::getTextStyleId(sName, pDb);
        if (!idStyle.isNull())
        {
          pDb->setTEXTSTYLE(idStyle); // Command changes database variable
          break;
        }
      }
    }
  }

  if (nKeyIndex == 0)
  { // Justification
    nKeyIndex = pIO->getKeyword(L"Enter an option [Align/Fit/Center/Midle/Right/TL/TC/TR/ML/MC/MR/BL/BC/BR]: ",
                                                 L"Align Fit Center Midle Right TL TC TR ML MC MR BL BC BR");
  }

  OdDbTextPtr pText = OdDbText::createObject();
  pText->setDatabaseDefaults(pDb);

  const OdChar* strFirstPoint =  L"Specify first end point of text base line: ";
  const OdChar* strSecondPoint = L"Specify second end point of text base line: ";
  OdDb::TextHorzMode hMode = OdDb::kTextLeft;
  OdDb::TextVertMode vMode = OdDb::kTextBase;
  bool bHeightRequired = true;
  bool bAngleRequired = true;

  switch (nKeyIndex)
  {
  case -1:  // Left - Base line
    pText->setPosition(ptPosition);
    break;

  case 0:   // Aligned
    hMode = OdDb::kTextAlign;
    pText->setPosition(pIO->getPoint(strFirstPoint));
    pText->setAlignmentPoint(pIO->getPoint(strSecondPoint, OdEd::kGptRubberBand));
    bHeightRequired = false;
    bAngleRequired = false;
    break;

  case 1:   // Fit
    hMode = OdDb::kTextFit;
    pText->setPosition(pIO->getPoint(strFirstPoint));
    pText->setAlignmentPoint(pIO->getPoint(strSecondPoint, OdEd::kGptRubberBand));
    bAngleRequired = false;
    break;

  case 2: // Center
    hMode = OdDb::kTextCenter;
    pText->setAlignmentPoint(pIO->getPoint(L"Specify center point of text: "));
    break;

  case 3: // Middle
    hMode = OdDb::kTextMid;
    pText->setAlignmentPoint(pIO->getPoint(L"Specify middle point of text: "));
    break;

  case 4: // Right
    hMode = OdDb::kTextRight;
    pText->setAlignmentPoint(pIO->getPoint(L"Specify right endpoint of text baseline: "));
    break;

  case 5: // Top left
    hMode = OdDb::kTextLeft;
    vMode = OdDb::kTextTop;
    pText->setAlignmentPoint(pIO->getPoint(L"Specify top-left point of text: "));
    break;

  case 6: // Top center
    hMode = OdDb::kTextCenter;
    vMode = OdDb::kTextTop;
    pText->setAlignmentPoint(pIO->getPoint(L"Specify top-center point of text: "));
    break;

  case 7: // Top right
    hMode = OdDb::kTextRight;
    vMode = OdDb::kTextTop;
    pText->setAlignmentPoint(pIO->getPoint(L"Specify top-right point of text: "));
    break;

  case 8: // Middle left
    hMode = OdDb::kTextLeft;
    vMode = OdDb::kTextVertMid;
    pText->setAlignmentPoint(pIO->getPoint(L"Specify middle-left point of text: "));
    break;

  case 9: // Middle Center
    hMode = OdDb::kTextCenter;
    vMode = OdDb::kTextVertMid;
    pText->setAlignmentPoint(pIO->getPoint(L"Specify middle point of text: "));
    break;

  case 10: // Middle right
    hMode = OdDb::kTextRight;
    vMode = OdDb::kTextVertMid;
    pText->setAlignmentPoint(pIO->getPoint(L"Specify middle-right point of text: "));
    break;

  case 11: // Bottom left
    hMode = OdDb::kTextLeft;
    vMode = OdDb::kTextBottom;
    pText->setAlignmentPoint(pIO->getPoint(L"Specify bottom-left point of text: "));
    break;

  case 12: // Bottom center
    hMode = OdDb::kTextCenter;
    vMode = OdDb::kTextBottom;
    pText->setAlignmentPoint(pIO->getPoint(L"Specify bottom-center point of text: "));
    break;

  case 13: // Bottom right
    hMode = OdDb::kTextRight;
    vMode = OdDb::kTextBottom;
    pText->setAlignmentPoint(pIO->getPoint(L"Specify bottom-right point of text: "));
    break;
  }

  pText->setHorizontalMode(hMode);
  pText->setVerticalMode  (vMode);

  if (bHeightRequired) // Not required for Aligned justification
  {
    OdDbTextStyleTableRecordPtr pTextStyle = pDb->getTEXTSTYLE().openObject();
    double dHeightToSet = pTextStyle->textSize();
    double  dDbHeight = pDb->getTEXTSIZE();
    if (dHeightToSet == 0.)
    { // Height is not specified in Text Style
      dHeightToSet = dDbHeight;
      // Note: Formatter presents double according to Drawing rules (Architectural, Fractional, ...
      OdString strHeightPrompt(L"Specify height <");
      strHeightPrompt += pDb->formatter().formatLinear(dHeightToSet);
      strHeightPrompt += L">: ";     

      dHeightToSet = pIO->getDist(strHeightPrompt, OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand, dHeightToSet);
    }
    if (dHeightToSet != dDbHeight)
    {
      pDb->setTEXTSIZE(dHeightToSet);  // the command really changes Db variable and prior size in current Text Style
    }
    pText->setHeight(dHeightToSet);
  }

  if (bAngleRequired)      // rotation, for Align and Fit not needed
  {
    double dAngle  = pIO->getAngle(L"Specify rotation angle of text <0>: ",
                                    OdEd::kGanFromLastPoint|OdEd::kGptRubberBand, 0.);
    pText->setRotation(dAngle);
  }

  struct StringTracker : OdStaticRxObject<OdEdStringTracker>
  {     
  private:
    OdDbText*     m_pText;
    OdDbTextPtr   m_pCursor;
    bool          m_bCursor;

  public:
    StringTracker(OdDbText* pText)
                 : m_pText(pText)
                 , m_bCursor(false)
    {
      m_pCursor = pText->clone();
      m_pCursor->setHorizontalMode(OdDb::kTextLeft);
      m_pCursor->setVerticalMode(OdDb::kTextBottom);
    }                                          

    virtual void setValue(const OdString& value)
    {
      m_pText->setTextString(value);
      m_pText->adjustAlignment();

      m_pCursor->setTextString(m_bCursor ? OdString(L"I") : OdString::kEmpty);

      if (m_bCursor)
      {
        OdGePoint3dArray bounds;
        m_pText->getBoundingPoints(bounds);
        m_pCursor->setPosition(bounds[3]);
      }
      m_bCursor = !m_bCursor;
    }

    virtual void setCursor(const bool& bShowCursor)
    {
      OdEdStringTracker::setCursor(bShowCursor);
      m_pCursor->setTextString(bShowCursor ? OdString(L"I") : OdString::kEmpty);
      if (m_bCursor = bShowCursor)
      {
        OdGePoint3dArray bounds;
        m_pText->getBoundingPoints(bounds);
        m_pCursor->setPosition(bounds[3]);
      }
    }

    virtual int addDrawables(OdGsView* pView)
    {
      pView->add(m_pText, 0);
      pView->add(m_pCursor, 0);
      return 1;
    }

    virtual void removeDrawables(OdGsView* pView)
    {
      pView->erase(m_pCursor);
      pView->erase(m_pText);
    }
  }

  stracker(pText);
  OdString strRes = pIO->getString(L"Specify text string: ",
                                  OdEd::kGstAllowSpaces | OdEd::kGstNoEmpty,
                                  OdString::kEmpty, OdString::kEmpty, &stracker);
  pText->setTextString(strRes); // This is required at least in case if tracker is not supported
  pSpace->appendOdDbEntity(pText);
}
