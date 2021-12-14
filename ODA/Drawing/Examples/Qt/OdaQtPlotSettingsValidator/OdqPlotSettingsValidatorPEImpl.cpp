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
// OdqPlotSettingsValidatorPEImpl.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui> 
#else
#include <QtWidgets>
#include <QtPrintSupport>
#endif
#include <QDir> 
#include <QSettings>
#include "OdqInterfaces.h"

#include "OdaCommon.h"
#include "OdqPlotSettingsValidatorPEImpl.h"
#include "OdString.h"

#include "DwfMediaList.h" // for DWF_DEVICE_NAME // L"DWF6 ePlot.pc3"
//#include "PdfMediaList.h" // TODO for PDF_DEVICE_NAME // L"DWG To PDF.pc3"

////#define NON_PORTABLE

//ODRX_NO_CONS_DEFINE_MEMBERS(OdqPlotSettingsValidatorPEImpl, OdRxObject)

static QPrinterInfo DeviceNameToPrinterInfo(const OdString& sDeviceName)
{
  QString qsDeviceName = getIApp()->toQString(sDeviceName);
  ODA_ASSERT_ONCE(!qsDeviceName.isEmpty());

  QPrinterInfo infoPrinter;
  foreach (QPrinterInfo info, QPrinterInfo::availablePrinters())
  {
    if (info.isNull() || !infoPrinter.isNull()) // problem on Mac Tiger without it
      continue;
    QString qsName = info.printerName();

    if (qsDeviceName.toLower() != qsName.toLower()) //if (qsDeviceName.compare(qsName, Qt::CaseInsensitive))
      continue;
    
    infoPrinter = info;
    break;
  }
  return infoPrinter;
}

static QString PaperSizeToString(QPrinter::PaperSize paperSize,
                                 int idx = 0)
{
  ODA_ASSERT_ONCE(paperSize >= 0 && paperSize <= QPrinter::NPaperSize && idx >= 0);

  static QMap<QPrinter::PaperSize, QString> s_map;
  if (s_map.isEmpty())
  {
    // =   name 
    //   + ;0 (0 - kInches or 1 - kMillimeters) 
    //   + ;width (10 * mm)
    //   + ;height (10 * mm)
    s_map[QPrinter::A0] = QObject::tr("ISO A0 (841 x 1189 mm)") + ";1;8410;11890";
    s_map[QPrinter::A1] = QObject::tr("ISO A1 (594 x 841 mm)") + ";1;5940;8410";
    s_map[QPrinter::A2] = QObject::tr("ISO A2 (420 x 594 mm)") + ";1;4200;5940";
    s_map[QPrinter::A3] = QObject::tr("ISO A3 (297 x 420 mm)") + ";1;2970;4200";
    s_map[QPrinter::A4] = QObject::tr("ISO A4 (210 x 297 mm)") + ";1;2100;2970"; // (8.26 x 11.7 inches)
    s_map[QPrinter::A5] = QObject::tr("ISO A5 (148 x 210 mm)") + ";1;1480;2100";
    s_map[QPrinter::A6] = QObject::tr("ISO A6 (105 x 148 mm)") + ";1;1050;1480";
    s_map[QPrinter::A7] = QObject::tr("ISO A7 (74 x 105 mm)") + ";1;740;1050";
    s_map[QPrinter::A8] = QObject::tr("ISO A8 (52 x 74 mm)") + ";1;520;740";
    s_map[QPrinter::A9] = QObject::tr("ISO A9 (37 x 52 mm)") + ";1;370;520";
    s_map[QPrinter::B0] = QObject::tr("ISO B0 (1000 x 1414 mm)") + ";1;10000;14140";
    s_map[QPrinter::B1] = QObject::tr("ISO B1 (707 x 1000 mm)") + ";1;7070;10000";
    s_map[QPrinter::B2] = QObject::tr("ISO B2 (500 x 707 mm)") + ";1;5000;7070";
    s_map[QPrinter::B3] = QObject::tr("ISO B3 (353 x 500 mm)") + ";1;3530;5000";
    s_map[QPrinter::B4] = QObject::tr("ISO B4 (250 x 353 mm)") + ";1;2500;3530"; 
    s_map[QPrinter::B5] = QObject::tr("ISO B5 (176 x 250 mm)") + ";1;1760;2500"; // (6.93 x 9.84 inches) // // 182 x 257
    s_map[QPrinter::B6] = QObject::tr("ISO B6 (125 x 176 mm)") + ";1;1250;1760";
    s_map[QPrinter::B7] = QObject::tr("ISO B7 (88 x 125 mm)") + ";1;880;1250";
    s_map[QPrinter::B8] = QObject::tr("ISO B8 (62 x 88 mm)") + ";1;620;880";
    s_map[QPrinter::B9] = QObject::tr("ISO B9 (44 x 62 mm)") + ";1;440;620";
    s_map[QPrinter::B10] = QObject::tr("ISO B10 (31 x 44 mm)") + ";1;310;440";
    s_map[QPrinter::C5E] = QObject::tr("C5E (163 x 229 mm)") + ";1;1630;2290";
    s_map[QPrinter::Comm10E] = QObject::tr("US Common #10 Envelope (105 x 241 mm)") + ";1;1050;2410";
    s_map[QPrinter::DLE] = QObject::tr("DLE (110 x 220 mm)") + ";1;1100;2200";
    s_map[QPrinter::Executive] = QObject::tr("Executive (7.5 x 10 inches, 191 x 254 mm)") + ";1;1910;2540";
    //s_map[QPrinter::Executive] = QObject::tr("Executive (7.25 x 10.5 inches. 184 x 267 mm)") + ";1;1840;2670";
    s_map[QPrinter::Folio] = QObject::tr("Folio (210 x 330 mm)") + ";1;2100;3300"; // 8.5 x 13 inches
    s_map[QPrinter::Ledger] = QObject::tr("Ledger (432 x 279 mm)") + ";1;4320;2790"; // 17 x 11 inches
    s_map[QPrinter::Legal] = QObject::tr("Legal (8.5 x 14 inches, 216 x 356 mm)") + ";1;2160;3560"; // 8.5 x 14 inches
    s_map[QPrinter::Letter] = QObject::tr("Letter (8.5 x 11 inches, 216 x 279 mm)") + ";1;2160;2790";
    s_map[QPrinter::Tabloid] = QObject::tr("Tabloid (279 x 432 mm)") + ";1;2790;4320"; // 11 x 17 inches
    s_map[QPrinter::Custom] = QObject::tr("User defined size");
#if 0 // (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    // TODO for INT-7647 gg for  + ";1;?*10;?*10";
    // New values derived from PPD standard
    s_map[QPrinter::A10] = QObject::tr("A10") + ";1;260;370";
    s_map[QPrinter::A3Extra] = QObject::tr("A3 Extra") + ";1;3220;4450";
    s_map[QPrinter::A4Extra] = QObject::tr("A4 Extra") + ";1;;"; //235 mm x 322 mm
    s_map[QPrinter::A4Plus] = QObject::tr("A4 Plus") + ";1;;";
    s_map[QPrinter::A4Small] = QObject::tr("A4 Small") + ";1;;";
    s_map[QPrinter::A5Extra] = QObject::tr("A5 Extra") + ";1;;";
    s_map[QPrinter::B5Extra] = QObject::tr("B5 Extra") + ";1;;";
    //
    s_map[QPrinter::JisB0] = QObject::tr("Jis B0") + ";1;;";
    s_map[QPrinter::JisB1] = QObject::tr("Jis B1") + ";1;;";
    s_map[QPrinter::JisB2] = QObject::tr("Jis B2") + ";1;;";
    s_map[QPrinter::JisB3] = QObject::tr("Jis B3") + ";1;;";
    s_map[QPrinter::JisB4] = QObject::tr("Jis B4") + ";1;;";
    s_map[QPrinter::JisB5] = QObject::tr("Jis B5") + ";1;;";
    s_map[QPrinter::JisB6] = QObject::tr("Jis B6") + ";1;;";
    s_map[QPrinter::JisB7] = QObject::tr("Jis B7") + ";1;;";
    s_map[QPrinter::JisB8] = QObject::tr("Jis B8") + ";1;;";
    s_map[QPrinter::JisB9] = QObject::tr("Jis B9") + ";1;;";
    s_map[QPrinter::JisB10] = QObject::tr("Jis B10") + ";1;;";
    // AnsiA = Letter
    // AnsiB = Ledger
    s_map[QPrinter::AnsiC] = QObject::tr("Ansi C") + ";1;;";
    s_map[QPrinter::AnsiD] = QObject::tr("Ansi D") + ";1;;";
    s_map[QPrinter::AnsiE] = QObject::tr("Ansi E") + ";1;;";
    s_map[QPrinter::LegalExtra] = QObject::tr("Legal Extra") + ";1;;";
    s_map[QPrinter::LetterExtra] = QObject::tr("Letter Extra") + ";1;;";
    s_map[QPrinter::LetterPlus] = QObject::tr("Letter Plus") + ";1;;";
    s_map[QPrinter::LetterSmall] = QObject::tr("Letter Small") + ";1;;";
    s_map[QPrinter::TabloidExtra] = QObject::tr("Tabloid Extra") + ";1;;";
    //
    s_map[QPrinter::ArchA] = QObject::tr("Arch A") + ";1;;";
    s_map[QPrinter::ArchB] = QObject::tr("Arch B") + ";1;;";
    s_map[QPrinter::ArchC] = QObject::tr("Arch C") + ";1;;";
    s_map[QPrinter::ArchD] = QObject::tr("Arch D") + ";1;;";
    s_map[QPrinter::ArchE] = QObject::tr("Arch E") + ";1;;";
    //  
    s_map[QPrinter::Imperial7x9] = QObject::tr("Imperia l7 x 9") + ";1;;";
    s_map[QPrinter::Imperial8x10] = QObject::tr("Imperia l8 x 10") + ";1;;";
    s_map[QPrinter::Imperial9x11] = QObject::tr("Imperia l9 x 11") + ";1;;";
    s_map[QPrinter::Imperial9x12] = QObject::tr("Imperia l9 x 12") + ";1;;";
    s_map[QPrinter::Imperial10x11] = QObject::tr("Imperial10x11") + ";1;;";
    s_map[QPrinter::Imperial10x13] = QObject::tr("Imperial 10 x 13") + ";1;;";
    s_map[QPrinter::Imperial10x14] = QObject::tr("Imperial 10 x 14") + ";1;;";
    s_map[QPrinter::Imperial12x11] = QObject::tr("Imperial 12 x 11") + ";1;;";
    s_map[QPrinter::Imperial15x11] = QObject::tr("Imperial 15 x 11") + ";1;;";
    //
    s_map[QPrinter::ExecutiveStandard] = QObject::tr("Executive Standard") + ";1;;";
    s_map[QPrinter::Note] = QObject::tr("Note") + ";1;2160;2800"; //8.5 x 11 inches
    s_map[QPrinter::Quarto] = QObject::tr("Quarto") + ";1;;";
    s_map[QPrinter::Statement] = QObject::tr("Statement") + ";1;1400;2160"; // 5.5 x 8.5 inches
    s_map[QPrinter::SuperA] = QObject::tr("Super A") + ";1;;";
    s_map[QPrinter::SuperB] = QObject::tr("Super B") + ";1;;";
    s_map[QPrinter::Postcard] = QObject::tr("Postcard") + ";1;;";
    s_map[QPrinter::DoublePostcard] = QObject::tr("Double Postcard") + ";1;;";
    s_map[QPrinter::Prc16K] = QObject::tr("Prc 16K") + ";1;;";
    s_map[QPrinter::Prc32K] = QObject::tr("Prc 32K") + ";1;;";
    s_map[QPrinter::Prc32KBig] = QObject::tr("Prc 32K Big") + ";1;;";
    //
    s_map[QPrinter::FanFoldUS] = QObject::tr("Fan Fold US") + ";1;;";
    s_map[QPrinter::FanFoldGerman] = QObject::tr("Fan Fold German") + ";1;;";
    s_map[QPrinter::FanFoldGermanLegal] = QObject::tr("Fan Fold German Legal") + ";1;;";
    //
    s_map[QPrinter::EnvelopeB4] = QObject::tr("Envelope B4") + ";1;;";
    s_map[QPrinter::EnvelopeB5] = QObject::tr("Envelope B5") + ";1;;";
    s_map[QPrinter::EnvelopeB6] = QObject::tr("Envelope B6") + ";1;;";
    s_map[QPrinter::EnvelopeC0] = QObject::tr("Envelope C0") + ";1;;";
    s_map[QPrinter::EnvelopeC1] = QObject::tr("Envelope C1") + ";1;;";
    s_map[QPrinter::EnvelopeC2] = QObject::tr("Envelope C2") + ";1;;";
    s_map[QPrinter::EnvelopeC3] = QObject::tr("Envelope C3") + ";1;;";
    s_map[QPrinter::EnvelopeC4] = QObject::tr("Envelope C4") + ";1;;";
    // EnvelopeC5 = C5E
    s_map[QPrinter::EnvelopeC6] = QObject::tr("Envelope C6") + ";1;;";
    s_map[QPrinter::EnvelopeC65] = QObject::tr("Envelope C65") + ";1;;";
    s_map[QPrinter::EnvelopeC7] = QObject::tr("Envelope C7") + ";1;;";
    // EnvelopeDL = DLE
    //
    s_map[QPrinter::Envelope9] = QObject::tr("Envelope1 9") + ";1;;";
    // Envelope10 = Comm10E
    s_map[QPrinter::Envelope11] = QObject::tr("Envelope1 11") + ";1;;";
    s_map[QPrinter::Envelope12] = QObject::tr("Envelope1 12") + ";1;;";
    s_map[QPrinter::Envelope14] = QObject::tr("Envelope1 14") + ";1;;";
    s_map[QPrinter::EnvelopeMonarch] = QObject::tr("Envelope Monarch") + ";1;;";
    s_map[QPrinter::EnvelopePersonal] = QObject::tr("Envelope Personal") + ";1;;";
    //
    s_map[QPrinter::EnvelopeChou3] = QObject::tr("Envelope Chou 3") + ";1;;";
    s_map[QPrinter::EnvelopeChou4] = QObject::tr("Envelope Chou 4") + ";1;;";
    s_map[QPrinter::EnvelopeInvite] = QObject::tr("Envelope Invite") + ";1;;";
    s_map[QPrinter::EnvelopeItalian] = QObject::tr("Envelope Italian") + ";1;;";
    s_map[QPrinter::EnvelopeKaku2] = QObject::tr("Envelope Kaku 2") + ";1;;";
    s_map[QPrinter::EnvelopeKaku3] = QObject::tr("Envelope Kaku 3") + ";1;;";
    s_map[QPrinter::EnvelopePrc1] = QObject::tr("Envelope Prc 1") + ";1;;";
    s_map[QPrinter::EnvelopePrc2] = QObject::tr("Envelope Prc 2") + ";1;;";
    s_map[QPrinter::EnvelopePrc3] = QObject::tr("Envelope Prc 3") + ";1;;";
    s_map[QPrinter::EnvelopePrc4] = QObject::tr("Envelope Prc 4") + ";1;;";
    s_map[QPrinter::EnvelopePrc5] = QObject::tr("Envelope Prc 5") + ";1;;";
    s_map[QPrinter::EnvelopePrc6] = QObject::tr("Envelope Prc 6") + ";1;;";
    s_map[QPrinter::EnvelopePrc7] = QObject::tr("Envelope Prc 7") + ";1;;";
    s_map[QPrinter::EnvelopePrc8] = QObject::tr("Envelope Prc 8") + ";1;;";
    s_map[QPrinter::EnvelopePrc9] = QObject::tr("Envelope Prc 9") + ";1;;";
    s_map[QPrinter::EnvelopePrc10] = QObject::tr("Envelope Prc 10") + ";1;;";
    s_map[QPrinter::EnvelopeYou4] = QObject::tr("Envelope You 4") + ";1;;";
#endif
  }

  QMap<QPrinter::PaperSize, QString>::Iterator itr = s_map.find(paperSize);
  if (itr == s_map.end())
  {
#  if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    ODA_FAIL_ONCE();
#  endif 
    return "";
  }

  QStringList lstValues = itr.value().split(";");
  if (idx >= lstValues.size() || idx < 0)
  {
    ODA_FAIL_ONCE();
    return "";
  }

  QString qsValue = lstValues.at(idx);
  return qsValue;
}

typedef enum {
  enPaperSize_Unit,
  enPaperSize_Width,
  enPaperSize_Height
} TPaperSizeDataValue;
static int PaperSizeToValue(QPrinter::PaperSize paperSize,
                            TPaperSizeDataValue typeValue)
{
  QString qsData = PaperSizeToString(paperSize, typeValue + 1);
  bool bOk = false;
  int nValue = 0;
  if (   qsData.isEmpty()
      || (nValue = qsData.toInt(&bOk), !bOk))
  {
    ODA_FAIL_ONCE();
    return 0;
  }
  return nValue;
}

static QPrinter::Unit PlotPaperUnitsToQPrinterUnit(OdDbPlotSettings::PlotPaperUnits units)
{
  switch (units)
  {
  case OdDbPlotSettings::kMillimeters:
    return QPrinter::Millimeter;
  case OdDbPlotSettings::kInches:
    return QPrinter::Inch;
  case OdDbPlotSettings::kPixels:
    return QPrinter::DevicePixel;
  }
  ODA_FAIL_ONCE();
  return QPrinter::Millimeter;
}

///////////////////////////////////////////////////////////////////////////////

//#if defined(NON_PORTABLE)
//
//#include <winspool.h>
//#include <commdlg.h>
//#include <tchar.h>
//
//#define STL_USING_MEMORY
//#define STL_USING_ALGORITHM
//#include "OdaSTL.h"
//
//namespace PE_PAPER
//{
//
//struct predefinedInfo
//{
//  OdDbPlotSettings::PlotPaperUnits _units;
//  const OdChar* _CanonicalName;
//};
//
//predefinedInfo predefinedPaperInfo[] =
//{
//  OdDbPlotSettings::kInches,      /* 0 element */ OD_T(""),
//  OdDbPlotSettings::kInches,      /* DMPAPER_LETTER               1  */ OD_T("Letter") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_LETTERSMALL          2  */ OD_T("Letter Small") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_TABLOID              3  */ OD_T("Tabloid") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_LEDGER               4  */ OD_T("Ledger") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_LEGAL                5  */ OD_T("Legal") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_STATEMENT            6  */ OD_T("Statement") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_EXECUTIVE            7  */ OD_T("Executive") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A3                   8  */ OD_T("A3") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A4                   9  */ OD_T("A4") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A4SMALL             10  */ OD_T("A4 Small") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A5                  11  */ OD_T("A5") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_B4                  12  */ OD_T("B4 (JIS)") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_B5                  13  */ OD_T("B5 (JIS)") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_FOLIO               14  */ OD_T("Folio") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_QUARTO              15  */ OD_T("Quarto") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_10X14               16  */ OD_T("10x14") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_11X17               17  */ OD_T("11x17") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_NOTE                18  */ OD_T("Note") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_9               19  */ OD_T("Envelope #9") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_10              20  */ OD_T("Envelope #10") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_11              21  */ OD_T("Envelope #11") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_12              22  */ OD_T("Envelope #12") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_14              23  */ OD_T("Envelope #14") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_CSHEET              24  */ OD_T("C size sheet") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_DSHEET              25  */ OD_T("D size sheet") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_ESHEET              26  */ OD_T("E size sheet") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_DL              27  */ OD_T("Envelope DL") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_C5              28  */ OD_T("Envelope C5") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_C3              29  */ OD_T("Envelope C3") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_C4              30  */ OD_T("Envelope C4") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_C6              31  */ OD_T("Envelope C6") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_C65             32  */ OD_T("Envelope C65") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_B4              33  */ OD_T("Envelope B4") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_B5              34  */ OD_T("Envelope B5") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_B6              35  */ OD_T("Envelope B6") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_ITALY           36  */ OD_T("Envelope") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_MONARCH         37  */ OD_T("Envelope Monarch") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_ENV_PERSONAL        38  */ OD_T("6 3/4 Envelope") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_FANFOLD_US          39  */ OD_T("US Std Fanfold") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_FANFOLD_STD_GERMAN  40  */ OD_T("German Std Fanfold") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_FANFOLD_LGL_GERMAN  41  */ OD_T("German Legal Fanfold") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_ISO_B4              42  */ OD_T("B4 (ISO)") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_JAPANESE_POSTCARD   43  */ OD_T("Japanese Postcard") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_9X11                44  */ OD_T("9x11") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_10X11               45  */ OD_T("10x11") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_15X11               46  */ OD_T("15x11") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_ENV_INVITE          47  */ OD_T("Envelope Invite") ,
//  OdDbPlotSettings::kInches,      /* 48 element */ OD_T("Reserved48") ,
//  OdDbPlotSettings::kInches,      /* 49 element */ OD_T("Reserved49") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_LETTER_EXTRA        50  */ OD_T("Letter Extra") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_LEGAL_EXTRA         51  */ OD_T("Legal Extra") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_TABLOID_EXTRA       52  */ OD_T("Tabloid Extra") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_A4_EXTRA            53  */ OD_T("A4 Extra") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_LETTER_TRANSVERSE   54  */ OD_T("Letter Transverse") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A4_TRANSVERSE       55  */ OD_T("A4 Transverse") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_LETTER_EXTRA_TRANSVERSE 56 */ OD_T("Letter Extra Transverse") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A_PLUS              57  */ OD_T("Super A") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_B_PLUS              58  */ OD_T("Super B") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_LETTER_PLUS         59  */ OD_T("Letter Plus") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A4_PLUS             60  */ OD_T("A4 Plus") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A5_TRANSVERSE       61  */ OD_T("A5 Transverse") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_B5_TRANSVERSE       62  */ OD_T("B5 (JIS) Transverse") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A3_EXTRA            63  */ OD_T("A3 Extra") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A5_EXTRA            64  */ OD_T("A5 Extra") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_B5_EXTRA            65  */ OD_T("B5 (ISO) Extra") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A2                  66  */ OD_T("A2") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A3_TRANSVERSE       67  */ OD_T("A3 Transverse") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A3_EXTRA_TRANSVERSE 68  */ OD_T("A3 Extra Transverse") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_DBL_JAPANESE_POSTCARD 69 */ OD_T("Japanese Double Postcard") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A6                  70  */ OD_T("A6") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_KAKU2          71  */ OD_T("Japanese Envelope Kaku #2") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_KAKU3          72  */ OD_T("Japanese Envelope Kaku #3") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_CHOU3          73  */ OD_T("Japanese Envelope Chou #3") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_CHOU4          74  */ OD_T("Japanese Envelope Chou #4") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_LETTER_ROTATED      75  */ OD_T("Letter Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A3_ROTATED          76  */ OD_T("A3 Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A4_ROTATED          77  */ OD_T("A4 Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A5_ROTATED          78  */ OD_T("A5 Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_B4_JIS_ROTATED      79  */ OD_T("B4 (JIS) Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_B5_JIS_ROTATED      80  */ OD_T("B5 (JIS) Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_JAPANESE_POSTCARD_ROTATED 81 */ OD_T("Japanese Postcard Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_DBL_JAPANESE_POSTCARD_ROTATED 82 */ OD_T("Double Japan Postcard Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_A6_ROTATED          83  */ OD_T("A6 Rotated") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_KAKU2_ROTATED  84  */ OD_T("Japan Envelope Kaku #2 Rotated") , // ????
//  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_KAKU3_ROTATED  85  */ OD_T("Japan Envelope Kaku #3 Rotated") , // ????
//  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_CHOU3_ROTATED  86  */ OD_T("Japan Envelope Chou #3 Rotated") , // ????
//  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_CHOU4_ROTATED  87  */ OD_T("Japan Envelope Chou #4 Rotated") , // ????
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_B6_JIS              88  */ OD_T("B6 (JIS)") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_B6_JIS_ROTATED      89  */ OD_T("B6 (JIS) Rotated") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_12X11               90  */ OD_T("12x11") ,
//  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_YOU4           91  */ OD_T("Japan Envelope You #4") , // ????
//  OdDbPlotSettings::kInches,      /* DMPAPER_JENV_YOU4_ROTATED   92  */ OD_T("Japan Envelope You #4 Rotated") , // ????
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_P16K                93  */ OD_T("PRC 16K") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_P32K                94  */ OD_T("PRC 32K") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_P32KBIG             95  */ OD_T("PRC 32K(Big)") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_1              96  */ OD_T("PRC Envelope #1") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_2              97  */ OD_T("PRC Envelope #2") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_3              98  */ OD_T("PRC Envelope #3") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_4              99  */ OD_T("PRC Envelope #4") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_5              100 */ OD_T("PRC Envelope #5") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_6              101 */ OD_T("PRC Envelope #6") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_7              102 */ OD_T("PRC Envelope #7") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_8              103 */ OD_T("PRC Envelope #8") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_9              104 */ OD_T("PRC Envelope #9") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_10             105 */ OD_T("PRC Envelope #10") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_P16K_ROTATED        106 */ OD_T("PRC 16K Rotated") , // ????
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_P32K_ROTATED        107 */ OD_T("PRC 32K Rotated") , // ????
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_P32KBIG_ROTATED     108 */ OD_T("PRC 32K(Big) Rotated") , // ????
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_1_ROTATED      109 */ OD_T("PRC Envelope #1 Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_2_ROTATED      110 */ OD_T("PRC Envelope #2 Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_3_ROTATED      111 */ OD_T("PRC Envelope #3 Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_4_ROTATED      112 */ OD_T("PRC Envelope #4 Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_5_ROTATED      113 */ OD_T("PRC Envelope #5 Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_6_ROTATED      114 */ OD_T("PRC Envelope #6 Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_7_ROTATED      115 */ OD_T("PRC Envelope #7 Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_8_ROTATED      116 */ OD_T("PRC Envelope #8 Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_9_ROTATED      117 */ OD_T("PRC Envelope #9 Rotated") ,
//  OdDbPlotSettings::kMillimeters, /* DMPAPER_PENV_10_ROTATED     118 */ OD_T("PRC Envelope #10 Rotated")
//};
//
//enum kPaperType
//{
//  eDeviceDefined = 0,
//  eUserDefined = 1
//};
//
//const OdUInt32 predefinedPaperInfoMax = sizeof(predefinedPaperInfo) / sizeof(predefinedPaperInfo[0]);
//
//OdString getCanonicalNameForDevice(OdUInt16 paperId, PE_PAPER::kPaperType mediaType, OdString localeName)
//{
//  if (mediaType == PE_PAPER::eDeviceDefined && paperId < PE_PAPER::predefinedPaperInfoMax)
//  {
//    //ODA_ASSERT(!localeName.compare( PE_PAPER::predefinedPaperInfo[paperId]._CanonicalName ));
//    return PE_PAPER::predefinedPaperInfo[paperId]._CanonicalName;
//  }
//  
//  return (mediaType == PE_PAPER::eUserDefined) ? OdString().format(OD_T("USER%d"), paperId) : localeName;
//}
//
//OdDbPlotSettings::PlotPaperUnits getUnitsForDevice(OdUInt16 paperId)
//{
//  return (paperId >= PE_PAPER::predefinedPaperInfoMax) ? OdDbPlotSettings::kMillimeters : PE_PAPER::predefinedPaperInfo[paperId]._units;
//}
//
//}; // namespace PE_PAPER
//
//
//static OdResult getSystemDeviceList(OdArray<OdString> &pDeviceList)
//{
//  // add system installed printer devices
//  DWORD pcbNeeded(0), pcReturned(0);
//  DWORD dwFlags( PRINTER_ENUM_CONNECTIONS|PRINTER_ENUM_LOCAL);
//  BOOL bRes = EnumPrinters( dwFlags, NULL, 4, NULL, 0, &pcbNeeded, &pcReturned );
//
//  std::auto_ptr<BYTE> pPrinterEnum(new BYTE[pcbNeeded]);
//
//  if (!pPrinterEnum.get())
//    return eOutOfMemory;
//
//  bRes = EnumPrinters( dwFlags, NULL, 4, pPrinterEnum.get(), pcbNeeded, &pcbNeeded, &pcReturned );
//  if (!bRes)
//    return eExtendedError;
//
//  PRINTER_INFO_4 *pPrinterInfo = (PRINTER_INFO_4 *)pPrinterEnum.get();
//
//  DWORD n = pDeviceList.size();
//  pDeviceList.resize(n + pcReturned);
//  for(DWORD f=0; f<pcReturned; ++f)
//  {
//    pDeviceList[n+f] = pPrinterInfo[f].pPrinterName;
//  }
//
//  return eOk;
//}
//
//class AutoClosePrinter
//{
//public:
//  HANDLE m_hPrinter;
//
//  AutoClosePrinter( ) : m_hPrinter( 0 ) { }
//  AutoClosePrinter(HANDLE hPrinter) : m_hPrinter(hPrinter) { }
//  operator HANDLE&() { return m_hPrinter; }
//  operator LPHANDLE() { return &m_hPrinter; }
//
//  virtual ~AutoClosePrinter()
//  {
//    ClosePrinter(m_hPrinter);
//  }
//};
//
//struct TCHAR64
//{
//  TCHAR tch[64];
//
//  OdString toOdString() const
//  {
//    return OdString(tch).left(64);
//  }
//
//  static OdString toOdString(LPCTSTR str)
//  {
//    return OdString(str).left(64);
//  }
//};
//
//class CDoubleRect
//{
//public:
//  double left;
//  double right;
//  double top;
//  double bottom;
//
//  CDoubleRect(const CDoubleRect &cdr)
//  {
//    left = cdr.left;
//    right = cdr.right;
//    top = cdr.top;
//    bottom = cdr.bottom;
//  }
//
//  CDoubleRect()
//    : left(0.), right(0.), top(0.), bottom(0.)
//  {}
//};
//
//class CPrinterInfo : public AutoClosePrinter
//{
//  OdString m_DriverName;
//  OdString m_DeviceName;
//  OdString m_PortName;
//public:
//  CPrinterInfo(OdString deviceName)
//    : m_DeviceName(deviceName)
//  {
//  }
//
//  OdResult openPrinter()
//  {
//    if (!OpenPrinter((LPTSTR)(LPCTSTR)m_DeviceName, &m_hPrinter, 0))
//    {
//      //DWORD err = GetLastError();
//      return eInvalidInput;
//    }
//
//    m_DriverName = OD_T("WINSPOOL");
//
//    return getPrinterPortName(m_PortName);
//  }
//protected:
//  OdResult getPrinterPortName(OdString &portName)
//  {
//    DWORD neeeded;
//
//    GetPrinter(m_hPrinter, 2, 0, 0, &neeeded);
//
//    //DWORD err = GetLastError();
//
//    std::auto_ptr<BYTE> pi2( new BYTE[neeeded] );
//    if (!pi2.get())
//      return eOutOfMemory;
//
//    if (!GetPrinter(m_hPrinter, 2, pi2.get(), neeeded, &neeeded))
//      return eInvalidInput;
//
//    portName = ((PRINTER_INFO_2 *)pi2.get())->pPortName;
//
//    return eOk;
//  }
//
//private:
//  static bool twice (const FORM_INFO_1 &elem1, const TCHAR64 &elem2 )
//  {
//    OdString s1 = TCHAR64::toOdString(elem1.pName);
//    OdString s2 = elem2.toOdString();
//    return !s1.iCompare(s2);
//  }
//
//protected:
//  OdResult getMediaTypes(const OdArray<TCHAR64> &paperNames, const OdArray <WORD> &paperIDs, OdArray<PE_PAPER::kPaperType> &mediaTypes)
//  {
//    DWORD pcReturned, pcbNeeded;
//    EnumForms(
//      m_hPrinter,    // handle to printer object
//      1,        // data level
//      0,       // form information buffer
//      0,        // size of form information buffer
//      &pcbNeeded,  // bytes received or required
//      &pcReturned  // number of forms received
//      );
//
//    OdArray<BYTE> buf; buf.resize(pcbNeeded, 0);
//
//    EnumForms(
//      m_hPrinter,    // handle to printer object
//      1,        // data level
//      buf.asArrayPtr(), // form information buffer
//      pcbNeeded,        // size of form information buffer
//      &pcbNeeded,  // bytes received or required
//      &pcReturned  // number of forms received
//      );
//
//    PFORM_INFO_1 pForm1 = (PFORM_INFO_1)buf.asArrayPtr();
//    mediaTypes.resize(paperNames.size(), PE_PAPER::eDeviceDefined);
//
//    for(OdUInt32 index = 0; index < paperNames.size(); ++index)
//    {
//      if (paperIDs[index] >= PE_PAPER::predefinedPaperInfoMax)
//      {
//        mediaTypes[index] = PE_PAPER::eUserDefined;
//      }
//      else
//      {
//        PFORM_INFO_1 pFounded = 
//          std::search(pForm1, pForm1 + pcReturned, paperNames.begin() + index, paperNames.begin() + index + 1, CPrinterInfo::twice);
//        if (pFounded == pForm1 + pcReturned)
//          mediaTypes[index] = PE_PAPER::eUserDefined;
//      }
//    }
//
//    return eOk;
//  }
//
//  OdResult getPaperNames(OdArray<TCHAR64> &paperNames)
//  {
//    DWORD dw, numPapers;
//
//    // got paper sizes name
//    numPapers = DeviceCapabilities(
//      m_DeviceName, m_PortName,
//      DC_PAPERNAMES, 0, NULL );
//
//    //dw = GetLastError();
//
//    if (numPapers == -1)
//      return eInvalidInput;
//
//    paperNames.resize(numPapers);
//
//    dw = DeviceCapabilities(
//      m_DeviceName, m_PortName,
//      DC_PAPERNAMES, (LPTSTR)paperNames.asArrayPtr(), NULL );
//    if (dw == -1)
//      return eInvalidInput;
//
//    return eOk;
//  }
//
//  OdResult getPaperSizeId(OdArray<WORD> &paperIDs)
//  {
//    DWORD dw, numStandardPaperSizes;
//
//    // got paper sizes id
//    numStandardPaperSizes = DeviceCapabilities(
//      m_DeviceName, m_PortName,
//      DC_PAPERS, 0, NULL );
//
//    paperIDs.resize(numStandardPaperSizes);
//
//    dw = DeviceCapabilities(
//      m_DeviceName, m_PortName,
//      DC_PAPERS, (LPTSTR)paperIDs.asArrayPtr(), NULL );
//    if (dw == -1)
//      return eInvalidInput;
//
//    return eOk;
//  }
//
//  OdResult getPaperSizes(OdArray<POINT> &paperSizes)
//  {
//    DWORD dw, numMMPapers;
//
//    // got paper sizes in tenths of a millimeter
//    numMMPapers = DeviceCapabilities(
//      m_DeviceName, m_PortName,
//      DC_PAPERSIZE, 0, NULL );
//
//    paperSizes.resize(numMMPapers);
//
//    dw = DeviceCapabilities(
//      m_DeviceName, m_PortName,
//      DC_PAPERSIZE, (LPTSTR)paperSizes.asArrayPtr(), NULL );
//    if (dw == -1)
//      return eInvalidInput;
//
//    return eOk;
//  }
//
//  OdResult getPaperMargins(const OdArray<WORD> &paperIDs, OdArray< CDoubleRect > &paperMargins)
//  {
//    paperMargins.resize(paperIDs.size());
//
//    // To prepare DEVMODE structure that specifies information 
//    //  about the device initialization and environment of a printer.
//
//    LONG
//      numberOfBytes = DocumentProperties(
//      NULL, m_hPrinter,	              // handle of printer object
//      (LPTSTR)(LPCTSTR)m_DeviceName,	// address of device name
//      0, 0, 0); 	  // mode flag
//    if (numberOfBytes<0)
//      return eInvalidInput;
//
//    std::auto_ptr<BYTE> pNewDefModeBuf(new BYTE[numberOfBytes]);
//    PDEVMODE pNewDefMode = (PDEVMODE)pNewDefModeBuf.get();
//
//    LONG
//    nResult = DocumentProperties(
//      NULL, m_hPrinter,	              // handle of printer object
//      (LPTSTR)(LPCTSTR)m_DeviceName,	// address of device name
//      pNewDefMode, 0, DM_OUT_BUFFER); 	  // mode flag
//    
//    if (nResult < 0)
//      return eInvalidInput;
//
//    pNewDefMode->dmFields |= DM_ORIENTATION | DM_PAPERSIZE;
//    pNewDefMode->dmOrientation = (short)DMORIENT_PORTRAIT;
//
//    HDC m_hPrinterDC = CreateDC( m_DriverName, m_DeviceName, NULL, pNewDefMode );
//    if (!m_hPrinterDC)
//      return eInvalidInput;
//
//    OdUInt32 index, k = paperIDs.size();
//    for(index = 0; index < k && nResult >= 0; ++index)
//    {
//      // To change Printer settings
//
//      if (paperIDs[index] == DMPAPER_USER)
//      {           
//        paperMargins[index].left   = 0.;
//        paperMargins[index].right  = 0.;
//        paperMargins[index].top    = 0.;
//        paperMargins[index].bottom = 0.;
//      }
//      else
//      {
//        pNewDefMode->dmPaperSize = paperIDs[index];
//
//
//        LONG nResult = DocumentProperties(
//          NULL, m_hPrinter,	              // handle of printer object
//          (LPTSTR)(LPCTSTR)m_DeviceName,	// address of device name
//          pNewDefMode, pNewDefMode, DM_OUT_BUFFER|DM_IN_BUFFER); 	  // mode flag
//
//        if (nResult < 0)
//          return eInvalidInput;
//
//        m_hPrinterDC = ResetDC(m_hPrinterDC, pNewDefMode);
//
//        double dLogPixelX           = GetDeviceCaps(m_hPrinterDC, LOGPIXELSX);
//        double dLogPixelY           = GetDeviceCaps(m_hPrinterDC, LOGPIXELSY);
//        double koeffX               = dLogPixelX / kMmPerInch;
//        double koeffY               = dLogPixelY / kMmPerInch;
//
//        double dPrinterWidth        = GetDeviceCaps(m_hPrinterDC, PHYSICALWIDTH) / koeffX;
//        double dPrinterHeight       = GetDeviceCaps(m_hPrinterDC, PHYSICALHEIGHT) / koeffY;
//
//        ODA_ASSERT(!OdZero(dPrinterWidth) & !OdZero(dPrinterHeight));
//
//        double dPrinterLeftMargin   = GetDeviceCaps(m_hPrinterDC, PHYSICALOFFSETX) / koeffX;
//        double dPrinterTopMargin    = GetDeviceCaps(m_hPrinterDC, PHYSICALOFFSETY) / koeffY;
//        double dPrinterMarginWidth  = GetDeviceCaps(m_hPrinterDC, HORZRES) / koeffX;
//        double dPrinterMarginHeight = GetDeviceCaps(m_hPrinterDC, VERTRES) / koeffY;
//        double dPrinterRightMargin  = dPrinterWidth  - dPrinterMarginWidth  - dPrinterLeftMargin;
//        double dPrinterBottomMargin = dPrinterHeight - dPrinterMarginHeight - dPrinterTopMargin;
//
//        paperMargins[index].left   = dPrinterLeftMargin;
//        paperMargins[index].right  = dPrinterRightMargin;
//        paperMargins[index].top    = dPrinterTopMargin;
//        paperMargins[index].bottom = dPrinterBottomMargin;
//      }
//    }
//    DeleteDC(m_hPrinterDC);
//
//    return eOk;
//  }
//
//public:
//  
//  OdResult getPrinterPaperInfo(OdArray<TCHAR64> &paperNames, OdArray<WORD> &paperIDs, OdArray<POINT> &paperSizes, OdArray< CDoubleRect > &paperMargins, OdArray<PE_PAPER::kPaperType> &mediaTypes, bool bUpdateMediaMargins, OdDbPlotSettingsValidatorPE::psvPaperInfo *pRequestedMediaInfo)
//  {
//    OdResult res;
//
//    if (pRequestedMediaInfo)
//    {
//      res = getPaperNames(paperNames);
//      if (res == eOk)
//      {
//        res = getPaperSizeId(paperIDs);
//        if (res == eOk)
//        {
//          for(OdUInt32 index = 0; index < paperNames.size(); ++index)
//          {
//            if (paperNames[index].toOdString() == pRequestedMediaInfo->localeName)
//            {
//              OdArray<WORD> singleIDs;
//              singleIDs.push_back( paperIDs[index] );
//              res = getPaperMargins(singleIDs, paperMargins);
//              return res;
//            }
//          }
//        }
//      }
//      return eInvalidInput;
//    }
//    else
//    {
//      res = getPaperNames(paperNames);
//      if (res == eOk)
//      {
//        res = getPaperSizeId(paperIDs);
//        if (res == eOk)
//        {
//          res = getPaperSizes(paperSizes);
//          if (res == eOk)
//          {
//            if (bUpdateMediaMargins)
//              res = getPaperMargins(paperIDs, paperMargins);
//            if (res == eOk)
//            {
//              res = getMediaTypes(paperNames, paperIDs,  mediaTypes);
//            }
//          }
//        }
//      }
//    }
//
//    return res;
//  }
//};
//
//static OdResult getSystemDeviceMediaList(const OdString& sDeviceName,
//                                         OdArray<OdDbPlotSettingsValidatorPE::psvPaperInfo>& listMedia,
//                                         bool bUpdateMediaMargins,
//                                         OdDbPlotSettingsValidatorPE::psvPaperInfo* pRequestedMediaInfo)
//{
//  OdResult res;
//  CPrinterInfo hPrinter(sDeviceName);
//  res = hPrinter.openPrinter();
//  if (res != eOk)
//    return res;
//  OdArray<TCHAR64> paperNames;
//  OdArray<WORD> paperIDs;
//  OdArray<POINT> paperSizes;
//  OdArray<CDoubleRect> paperMargins;
//  OdArray<PE_PAPER::kPaperType> mediaTypes;
//
//  if (pRequestedMediaInfo)
//  {
//    res = hPrinter.getPrinterPaperInfo(paperNames, paperIDs, paperSizes, paperMargins, mediaTypes, false, pRequestedMediaInfo);
//    if (res == eOk)
//    {
//      pRequestedMediaInfo->left = paperMargins.first().left;
//      pRequestedMediaInfo->right = paperMargins.first().right;
//      pRequestedMediaInfo->top = paperMargins.first().top;
//      pRequestedMediaInfo->bottom = paperMargins.first().bottom;
//      if (pRequestedMediaInfo->units == OdDbPlotSettings::kInches)
//      {
//        pRequestedMediaInfo->left /= kMmPerInch;;
//        pRequestedMediaInfo->right /= kMmPerInch;;
//        pRequestedMediaInfo->top /= kMmPerInch;;
//        pRequestedMediaInfo->bottom /= kMmPerInch;;
//      }
//    }
//    else
//    {
//      pRequestedMediaInfo->left = -HUGE_VAL;
//      pRequestedMediaInfo->right = -HUGE_VAL;
//      pRequestedMediaInfo->top = -HUGE_VAL;
//      pRequestedMediaInfo->bottom = -HUGE_VAL;
//    }
//    return res;
//  }
//
//  res = hPrinter.getPrinterPaperInfo(paperNames, paperIDs, paperSizes, paperMargins, mediaTypes, bUpdateMediaMargins, 0);
//  if (res != eOk)
//    return res;
//
//  // fill media list
//  listMedia.resize(paperNames.size());
//  for(OdUInt32 index = 0; index < listMedia.size(); index++)
//  {
//
//    OdDbPlotSettingsValidatorPE::psvPaperInfo& infoMedia = listMedia[index];
//
//
//    infoMedia.localeName    = paperNames[index].toOdString();
//    infoMedia.canonicalName = PE_PAPER::getCanonicalNameForDevice(paperIDs[index], mediaTypes[index], infoMedia.localeName);
//    infoMedia.units         = PE_PAPER::getUnitsForDevice(paperIDs[index]);
//    infoMedia.w = paperSizes[index].x / 10.;
//    infoMedia.h = paperSizes[index].y / 10.;
//    if (infoMedia.units == OdDbPlotSettings::kInches)
//    {
//      infoMedia.w /= kMmPerInch;
//      infoMedia.h /= kMmPerInch;
//    }
//
//    if (bUpdateMediaMargins)
//    {
//      infoMedia.left   = paperMargins[index].left;
//      infoMedia.right  = paperMargins[index].right;
//      infoMedia.top    = paperMargins[index].top;
//      infoMedia.bottom = paperMargins[index].bottom;
//      if (bUpdateMediaMargins && infoMedia.units == OdDbPlotSettings::kInches)
//      {
//        infoMedia.left /= kMmPerInch;
//        infoMedia.right /= kMmPerInch;
//        infoMedia.top /= kMmPerInch;
//        infoMedia.bottom /= kMmPerInch;
//      }
//    }
//    else
//    {
//      infoMedia.left   = -HUGE_VAL;
//      infoMedia.right  = -HUGE_VAL;
//      infoMedia.top    = -HUGE_VAL;
//      infoMedia.bottom = -HUGE_VAL;
//    }
//  }
//
//  return eOk; // return res;
//}
//
//#endif 

static bool findFilesByPath(const QString& qsPath, 
                            const QStringList& filters, 
                            OdArray<OdString>& pPlotStyleSheetList) // out
{
  QFileInfoList lstInfo = QDir(qsPath).entryInfoList(filters, 
                                                     QDir::Files | QDir::Readable | QDir::NoSymLinks);
  bool bRes = false;
  foreach (QFileInfo info, lstInfo)
  {
    ODA_ASSERT_ONCE(info.isFile());

    OdString sFileName = getIApp()->toOdString(info.fileName());
    pPlotStyleSheetList.push_back(sFileName);
    bRes = true;
  }
  return bRes;
}

OdResult OdqPlotSettingsValidatorPEImpl::getMediaMargins(const OdString& sDeviceName, psvPaperInfo& infoMedia)
{
//#if defined(NON_PORTABLE)
//  OdArray<OdDbPlotSettingsValidatorPE::psvPaperInfo> listMedia; // dummy
//  OdResult res = getSystemDeviceMediaList(sDeviceName, listMedia, false, &infoMedia);
//  return res;
//#endif

  QPrinterInfo infoPrinter = DeviceNameToPrinterInfo(sDeviceName);
  QPrinter printer(infoPrinter); // , QPrinter::HighResolution); // QPrinter::PrinterResolution);
  if (infoPrinter.isNull() || !printer.isValid())
  {
    ODA_FAIL_ONCE();
    return eInvalidInput;
  }

  printer.getPageMargins(&infoMedia.left, &infoMedia.top, &infoMedia.right, &infoMedia.bottom, 
                         PlotPaperUnitsToQPrinterUnit(infoMedia.units));
  return eOk;
}


OdResult OdqPlotSettingsValidatorPEImpl::getPlotStyleSheetList(OdArray<OdString>& pPlotStyleSheetList, OdDbBaseHostAppServices *pHostApp)
{
  OdString sPathPS = getIAppProps()->toString(OD_T("Vars/ddplotstylepaths"), 
                                              getIApp()->toOdString(getenv("DDPLOTSTYLEPATHS"))); //getenv() is a standard ISO C function from stdlib.h
  QString qsPathPS = getIApp()->toQString(sPathPS);  

 //#if defined(Q_OS_WIN32) // && defined(_DEBUG) // !defined(__APPLE__) && !defined(Q_OS_LINUX)
  if (qsPathPS.isEmpty())
  {
    // try to get it via the installed ACAD on computer
    foreach(QString qsKey, getIApp()->getAcadRegProfileGeneralKeys())
    {
      QSettings settings(qsKey, QSettings::NativeFormat);
      qsPathPS = settings.value("PrinterStyleSheetDir", "").toString();
      if (!qsPathPS.isEmpty())
      {
        getIApp()->substituteEnvVariables(qsPathPS);
        break;
      }
    }
  }
 //#endif
  if (!qsPathPS.isEmpty())
  {
    QStringList lstPaths = qsPathPS.split(chPathSeparator, QString::SkipEmptyParts);
    foreach (QString qsPath, lstPaths)
    {
      bool bFound = findFilesByPath(qsPath, QStringList() << "*.ctb", pPlotStyleSheetList);
      bFound |= findFilesByPath(qsPath, QStringList() << "*.stb", pPlotStyleSheetList);
      if (bFound)
        break;
    }
  }

  return eOk;
}

//static OdResult getPDFMediaList(OdArray<OdDbPlotSettingsValidatorPE::psvPaperInfo>& listMedia)
//{
//  struct PdfPaper
//  {
//    const OdChar* _localeName;
//    const OdChar* _canonicalName;
//    double _w;
//    double _h;
//    OdDbPlotSettings::PlotPaperUnits _units;
//
//    double _xminMargin;
//    double _yminMargin;
//    double _xmaxMargin;
//    double _ymaxMargin;
//  };
//
//  static PdfPaper pdfPapers [] = 
//  {
//    { OD_T("ISO expand A0 (841.00 x 1189.00 MM)")       , OD_T("ISO_expand_A0_(841.00_x_1189.00_MM)")       , 841.0   , 1189.0  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793701171875000 } ,
//    { OD_T("ISO A0 (841.00 x 1189.00 MM)")              , OD_T("ISO_A0_(841.00_x_1189.00_MM)")              , 841.0   , 1189.0  , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793701171875000 } ,
//    { OD_T("ISO expand A1 (841.00 x 594.00 MM)")        , OD_T("ISO_expand_A1_(841.00_x_594.00_MM)")        , 841.0   , 594.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
//    { OD_T("ISO expand A1 (594.00 x 841.00 MM)")        , OD_T("ISO_expand_A1_(594.00_x_841.00_MM)")        , 594.0   , 841.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
//    { OD_T("ISO A1 (841.00 x 594.00 MM)")               , OD_T("ISO_A1_(841.00_x_594.00_MM)")               , 841.0   , 594.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
//    { OD_T("ISO A1 (594.00 x 841.00 MM)")               , OD_T("ISO_A1_(594.00_x_841.00_MM)")               , 594.0   , 841.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
//    { OD_T("ISO expand A2 (594.00 x 420.00 MM)")        , OD_T("ISO_expand_A2_(594.00_x_420.00_MM)")        , 594.0   , 420.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
//    { OD_T("ISO expand A2 (420.00 x 594.00 MM)")        , OD_T("ISO_expand_A2_(420.00_x_594.00_MM)")        , 420.0   , 594.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
//    { OD_T("ISO A2 (594.00 x 420.00 MM)")               , OD_T("ISO_A2_(594.00_x_420.00_MM)")               , 594.0   , 420.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
//    { OD_T("ISO A2 (420.00 x 594.00 MM)")               , OD_T("ISO_A2_(420.00_x_594.00_MM)")               , 420.0   , 594.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
//    { OD_T("ISO expand A3 (420.00 x 297.00 MM)")        , OD_T("ISO_expand_A3_(420.00_x_297.00_MM)")        , 420.0   , 297.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
//    { OD_T("ISO expand A3 (297.00 x 420.00 MM)")        , OD_T("ISO_expand_A3_(297.00_x_420.00_MM)")        , 297.0   , 420.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793762207031250 } ,
//    { OD_T("ISO A3 (420.00 x 297.00 MM)")               , OD_T("ISO_A3_(420.00_x_297.00_MM)")               , 420.0   , 297.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
//    { OD_T("ISO A3 (297.00 x 420.00 MM)")               , OD_T("ISO_A3_(297.00_x_420.00_MM)")               , 297.0   , 420.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793762207031250 } ,
//    { OD_T("ISO expand A4 (297.00 x 210.00 MM)")        , OD_T("ISO_expand_A4_(297.00_x_210.00_MM)")        , 297.0   , 210.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793762207031250 , 10.793746948242188 } ,
//    { OD_T("ISO expand A4 (210.00 x 297.00 MM)")        , OD_T("ISO_expand_A4_(210.00_x_297.00_MM)")        , 210.0   , 297.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 10.793749809265137, 5.793746948242188 , 10.793762207031250 } ,
//    { OD_T("ISO A4 (297.00 x 210.00 MM)")               , OD_T("ISO_A4_(297.00_x_210.00_MM)")               , 297.0   , 210.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793762207031250 , 17.793746948242188 } ,
//    { OD_T("ISO A4 (210.00 x 297.00 MM)")               , OD_T("ISO_A4_(210.00_x_297.00_MM)")               , 210.0   , 297.0   , OdDbPlotSettings::kMillimeters , 5.793749809265137 , 17.793750762939453, 5.793746948242188 , 17.793762207031250 } ,
//    { OD_T("ARCH expand E1 (30.00 x 42.00 Inches)")     , OD_T("ARCH_expand_E1_(30.00_x_42.00_Inches)")     , 30.0    , 42.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424948865034449 } ,
//    { OD_T("ARCH E1 (30.00 x 42.00 Inches)")            , OD_T("ARCH_E1_(30.00_x_42.00_Inches)")            , 30.0    , 42.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700539416215551 } ,
//    { OD_T("ARCH expand E (36.00 x 48.00 Inches)")      , OD_T("ARCH_expand_E_(36.00_x_48.00_Inches)")      , 36.0    , 48.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424948865034449 } ,
//    { OD_T("ARCH E (36.00 x 48.00 Inches)")             , OD_T("ARCH_E_(36.00_x_48.00_Inches)")             , 36.0    , 48.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700539416215551 } ,
//    { OD_T("ARCH expand D (36.00 x 24.00 Inches)")      , OD_T("ARCH_expand_D_(36.00_x_24.00_Inches)")      , 36.0    , 24.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
//    { OD_T("ARCH expand D (24.00 x 36.00 Inches)")      , OD_T("ARCH_expand_D_(24.00_x_36.00_Inches)")      , 24.0    , 36.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
//    { OD_T("ARCH D (36.00 x 24.00 Inches)")             , OD_T("ARCH_D_(36.00_x_24.00_Inches)")             , 36.0    , 24.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
//    { OD_T("ARCH D (24.00 x 36.00 Inches)")             , OD_T("ARCH_D_(24.00_x_36.00_Inches)")             , 24.0    , 36.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
//    { OD_T("ARCH expand C (24.00 x 18.00 Inches)")      , OD_T("ARCH_expand_C_(24.00_x_18.00_Inches)")      , 24.0    , 18.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
//    { OD_T("ARCH expand C (18.00 x 24.00 Inches)")      , OD_T("ARCH_expand_C_(18.00_x_24.00_Inches)")      , 18.0    , 24.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
//    { OD_T("ARCH C (24.00 x 18.00 Inches)")             , OD_T("ARCH_C_(24.00_x_18.00_Inches)")             , 24.0    , 18.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
//    { OD_T("ARCH C (18.00 x 24.00 Inches)")             , OD_T("ARCH_C_(18.00_x_24.00_Inches)")             , 18.0    , 24.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
//    { OD_T("ANSI expand E (34.00 x 44.00 Inches)")      , OD_T("ANSI_expand_E_(34.00_x_44.00_Inches)")      , 34.0    , 44.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424948865034449 } ,
//    { OD_T("ANSI E (34.00 x 44.00 Inches)")             , OD_T("ANSI_E_(34.00_x_44.00_Inches)")             , 34.0    , 44.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700539416215551 } ,
//    { OD_T("ANSI expand D (34.00 x 22.00 Inches)")      , OD_T("ANSI_expand_D_(34.00_x_22.00_Inches)")      , 34.0    , 22.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
//    { OD_T("ANSI expand D (22.00 x 34.00 Inches)")      , OD_T("ANSI_expand_D_(22.00_x_34.00_Inches)")      , 22.0    , 34.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
//    { OD_T("ANSI D (34.00 x 22.00 Inches)")             , OD_T("ANSI_D_(34.00_x_22.00_Inches)")             , 34.0    , 22.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
//    { OD_T("ANSI D (22.00 x 34.00 Inches)")             , OD_T("ANSI_D_(22.00_x_34.00_Inches)")             , 22.0    , 34.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
//    { OD_T("ANSI expand C (22.00 x 17.00 Inches)")      , OD_T("ANSI_expand_C_(22.00_x_17.00_Inches)")      , 22.0    , 17.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
//    { OD_T("ANSI expand C (17.00 x 22.00 Inches)")      , OD_T("ANSI_expand_C_(17.00_x_22.00_Inches)")      , 17.0    , 22.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
//    { OD_T("ANSI C (22.00 x 17.00 Inches)")             , OD_T("ANSI_C_(22.00_x_17.00_Inches)")             , 22.0    , 17.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
//    { OD_T("ANSI C (17.00 x 22.00 Inches)")             , OD_T("ANSI_C_(17.00_x_22.00_Inches)")             , 17.0    , 22.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
//    { OD_T("ANSI expand B (17.00 x 11.00 Inches)")      , OD_T("ANSI_expand_B_(17.00_x_11.00_Inches)")      , 17.0    , 11.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
//    { OD_T("ANSI expand B (11.00 x 17.00 Inches)")      , OD_T("ANSI_expand_B_(11.00_x_17.00_Inches)")      , 11.0    , 17.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424951267993356 } ,
//    { OD_T("ANSI B (17.00 x 11.00 Inches)")             , OD_T("ANSI_B_(17.00_x_11.00_Inches)")             , 17.0    , 11.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
//    { OD_T("ANSI B (11.00 x 17.00 Inches)")             , OD_T("ANSI_B_(11.00_x_17.00_Inches)")             , 11.0    , 17.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541819174459 } ,
//    { OD_T("ANSI expand A (11.00 x 8.50 Inches)")       , OD_T("ANSI_expand_A_(11.00_x_8.50_Inches)")       , 11.0    , 8.5     , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100874292569 , 0.424950667253629 } ,
//    { OD_T("ANSI expand A (8.50 x 11.00 Inches)")       , OD_T("ANSI_expand_A_(8.50_x_11.00_Inches)")       , 8.5     , 11.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.424950779892328 , 0.228100273552842 , 0.424951267993356 } ,
//    { OD_T("ANSI A (11.00 x 8.50 Inches)")              , OD_T("ANSI_A_(11.00_x_8.50_Inches)")              , 11.0    , 8.5     , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100874292569 , 0.700541218434732 } ,
//    { OD_T("ANSI A (8.50 x 11.00 Inches)")              , OD_T("ANSI_A_(8.50_x_11.00_Inches)")              , 8.5     , 11.0    , OdDbPlotSettings::kInches      , 0.228100386191541 , 0.700541368619664 , 0.228100273552842 , 0.700541819174459 } ,
//  };
//
//  OdUInt32 numPapers = sizeof(pdfPapers)/sizeof(pdfPapers[0]);
//  OdUInt32 index = listMedia.size();
//  listMedia.resize(index + numPapers);
//  for(OdUInt32 i=0; i<numPapers; ++i,++index)
//  {
//    listMedia[index].canonicalName = pdfPapers[i]._canonicalName;
//    listMedia[index].localeName    = pdfPapers[i]._localeName;
//    listMedia[index].w             = pdfPapers[i]._w;
//    listMedia[index].h             = pdfPapers[i]._h;
//    listMedia[index].left          = pdfPapers[i]._xminMargin;
//    listMedia[index].top           = pdfPapers[i]._ymaxMargin;
//    listMedia[index].right         = pdfPapers[i]._xmaxMargin;
//    listMedia[index].bottom        = pdfPapers[i]._yminMargin;
//    listMedia[index].units         = pdfPapers[i]._units;
//  }
//
//  return eOk;
//}

OdResult OdqPlotSettingsValidatorPEImpl::getDeviceList(OdArray<OdString>& listDevices)
{
 //#ifndef _DEBUG // TODO remove the next if PDF ready
  listDevices.clear();
  listDevices.push_back(OD_T("None"));
 //#endif

  // add system installed printers
  foreach (QPrinterInfo info, QPrinterInfo::availablePrinters())
  {
    OdString sDevice = getIApp()->toOdString(info.printerName());
    listDevices.push_back(sDevice);
  }

 #ifndef _DEBUG // TODO remove the next if PDF ready
  return eOk;
 #endif

#ifdef PDF_DEVICE_NAME
  listDevices.append(PDF_DEVICE_NAME); // add PDF device
#endif

#ifdef DWF_DEVICE_NAME
  listDevices.append(DWF_DEVICE_NAME); // add DWF device
#endif

  return eOk;
}

OdResult OdqPlotSettingsValidatorPEImpl::getMediaList(const OdString& sDeviceName,
                                                      OdArray<psvPaperInfo>& listMedia,
                                                      bool bUpdateMediaMargins)
{
#ifdef PDF_DEVICE_NAME
  // User defined device sample. Add papers for PDF device.
  if (!sDeviceName.iCompare(PDF_DEVICE_NAME))
    return getPDFMediaList(listMedia);
#endif
#ifdef DWF_DEVICE_NAME
  // User defined device sample. Add papers for PDF device.
  if (!sDeviceName.iCompare(DWF_DEVICE_NAME))
    return getDWFMediaList(listMedia);
#endif
  
//#if defined(NON_PORTABLE)
//  // Add media list for system installed devices.
//  return getSystemDeviceMediaList(sDeviceName, listMedia, bUpdateMediaMargins, NULL);
//#endif

  QPrinterInfo infoPrinter = DeviceNameToPrinterInfo(sDeviceName);
  QPrinter printer(infoPrinter); // , QPrinter::HighResolution); // QPrinter::PrinterResolution);
  if (infoPrinter.isNull() || !printer.isValid())
  {
    ODA_FAIL_ONCE();
    return eInvalidInput;
  }

  QList<QPrinter::PaperSize> lstDirtySizes = infoPrinter.supportedPaperSizes(), // strange but it contains duplicates
                             lstSizes;
  if (lstDirtySizes.isEmpty())
  {
    // Not all printer drivers support query of supportedPaperSizes, so the list may be empty.
    // On Mac OS X 10.3, this function always returns an empty list.
    // 
    // there is enough to take full possible list here (it will be filtered on next steps)
    lstDirtySizes << QPrinter::Letter << QPrinter::Legal << QPrinter::Executive
                  << QPrinter::A0 << QPrinter::A1 << QPrinter::A2 << QPrinter::A3
                  << QPrinter::A4 << QPrinter::A5 << QPrinter::A6 << QPrinter::A7 
                  << QPrinter::A8 << QPrinter::A9
                  << QPrinter::B0 << QPrinter::B1 << QPrinter::B2 << QPrinter::B3 
                  << QPrinter::B4 << QPrinter::B5 << QPrinter::B6 << QPrinter::B7
                  << QPrinter::B8 << QPrinter::B9 << QPrinter::B10
                  << QPrinter::C5E << QPrinter::Comm10E << QPrinter::DLE
                  << QPrinter::Folio << QPrinter::Ledger << QPrinter::Tabloid;
  }

  QPrinter::PaperSize paperSize;
  foreach(paperSize, lstDirtySizes)
  {
    if (lstSizes.indexOf(paperSize) < 0)
      lstSizes << paperSize;
  }

  // fill media list
  listMedia.clear();

  OdUInt32 index = -1;
  foreach(paperSize, lstSizes)
  {
    QString qsName = PaperSizeToString(paperSize);
    if (qsName.isEmpty()) // skipped
      continue;
    OdDbPlotSettings::PlotPaperUnits units = OdDbPlotSettings::kMillimeters;
    printer.setPaperSize(paperSize);
    if (paperSize != printer.paperSize())
    {
      // TODO check it inside Qt source .. ODA_FAIL_ONCE();
      continue;
    }

    QRectF rect = printer.paperRect(PlotPaperUnitsToQPrinterUnit(units)); // paperRect pageRect
    //QRectF rect1 = printer.pageRect(PlotPaperUnitsToQPrinterUnit(units));
    double width = rect.width(),
           height = rect.height();
    if (qsName != QObject::tr("User defined size"))
    {
      units = OdDbPlotSettings::PlotPaperUnits(PaperSizeToValue(paperSize, enPaperSize_Unit));

     #ifdef _DEBUG
      double width_ = PaperSizeToValue(paperSize, enPaperSize_Width) / 10.,
             height_ = PaperSizeToValue(paperSize, enPaperSize_Height) / 10.;
      ODA_ASSERT_ONCE(   (   int(width + 0.5) == int(width_ + 0.5) 
                          && int(height + 0.5) == int(height_ + 0.5))
                      // TODO
                      || paperSize == QPrinter::Executive // 184 x 267
                      || paperSize == QPrinter::B4 // 257 x 363
                      || paperSize == QPrinter::B5 // 182 x 257
                      || paperSize == QPrinter::Folio // 216 x 330 
                      || paperSize == QPrinter::C5E // 324 x 459 
                      );
     #endif

      if (units == OdDbPlotSettings::kInches)
      {
        width /= kMmPerInch;
        height /= kMmPerInch;
      }
    }
    else 
      qsName += QObject::tr(" (%1 x %2 %3)")
                  .arg(width, 0, 'f', (units == OdDbPlotSettings::kMillimeters) 
                                        ? 0 : (units == OdDbPlotSettings::kInches) ? 1 : 2)
                  .arg(height, 0, 'f', (units == OdDbPlotSettings::kMillimeters) 
                                        ? 0 : (units == OdDbPlotSettings::kInches) ? 1 : 2)
                  .arg((units == OdDbPlotSettings::kMillimeters) 
                                        ? "mm" : (units == OdDbPlotSettings::kInches) ? "Inches" : "Pixels");

    listMedia.push_back(OdDbPlotSettingsValidatorPE::psvPaperInfo());
    OdDbPlotSettingsValidatorPE::psvPaperInfo& infoMedia = listMedia[++index];

    infoMedia.localeName = getIApp()->toOdString(qsName);
    qsName.replace(" ","_");
    infoMedia.canonicalName = getIApp()->toOdString(qsName);
    infoMedia.units = units;
    infoMedia.w = width;
    infoMedia.h = height;

    if (bUpdateMediaMargins)
      printer.getPageMargins(&infoMedia.left, &infoMedia.top, &infoMedia.right, &infoMedia.bottom, 
                             PlotPaperUnitsToQPrinterUnit(units));
    else
    {
      infoMedia.left   = -HUGE_VAL;
      infoMedia.right  = -HUGE_VAL;
      infoMedia.top    = -HUGE_VAL;
      infoMedia.bottom = -HUGE_VAL;
    }
  }

  return eOk;
}

OdResult OdqPlotSettingsValidatorPEImpl::getDefaultMedia(const OdString&, // sDeviceName
                                                         OdString&) // sDefaultMedia
{
  return eNotImplementedYet;
}
