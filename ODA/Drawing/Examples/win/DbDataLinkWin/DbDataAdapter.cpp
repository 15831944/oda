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

#include "DbDataAdapter.h"
#include "DbTableContent.h"
#include "OdDToStr.h"

// #define ADO_USED
#define LibXL_USED

#ifdef ADO_USED

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "adoEOF") rename("BOF", "adoBOF")

// #import "C:\\Program Files (x86)\\Common Files\\Microsoft Shared\OFFICE15\\MSO.DLL" \
//   rename("RGB", "MSORGB")
// 
// using namespace Office;
// 
// #import "C:\\Program Files (x86)\\Common Files\\Microsoft Shared\\VBA\\VBA6\\VBE6EXT.OLB"
// 
// using namespace VBIDE;
// 
// #import "C:\\Program Files (x86)\\Microsoft Office\\OFFICE14\\EXCEL.EXE" \
//   rename("DialogBox", "ExcelDialogBox") \
//   rename("RGB", "ExcelRGB") \
//   rename("CopyFile", "ExcelCopyFile") \
//   rename("ReplaceText", "ExcelReplaceText")

class DbLinkConnection
{
  _ConnectionPtr pCon;
  _RecordsetPtr pSchema;
  _RecordsetPtr pRec;

  long m_records;
  long m_fields;

  OdString m_connectionStr;
public:
  DbLinkConnection() {
    pCon = NULL;
    pSchema = NULL;
    pRec = NULL;
    m_records = 0;
    m_fields = 0;
  };

  ~DbLinkConnection()
  {
    if (pCon != NULL && pCon->State == adStateOpen)
      pCon->Close();
    ::CoUninitialize();
  };

  OdString makeConnStr(OdString filename, bool header = true);
  OdString sqlSelectSheet(int sheetIndex);
  bool connect(OdString conectionStr);
  void selectRecordSet(OdString sheetName, OdString range);
  void getData(OdDbTableContentPtr content);
  void putData(OdDbTableContentPtr content);

  long numRows() { return m_records;}
  long numColumns() { return m_fields; }

};

OdString DbLinkConnection::makeConnStr(OdString filename, bool header)
{
  OdString hdr = header ? "YES" : "NO";

  if ( filename.isEmpty() )
    return OdString::kEmpty;

  OdString connStr;

  if (filename.find(L".xlsx") != -1)
  {
    m_connectionStr = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=";
    m_connectionStr += filename;
    m_connectionStr += L";Extended Properties=\"Excel 12.0 Xml;HDR=";
  }
  else
  {
    m_connectionStr = L"Provider='Microsoft.JET.OLEDB.4.0';Data Source=";
    m_connectionStr += filename;
    m_connectionStr += L";Extended Properties=\"Excel 8.0;HDR=";
  }
  if (header)
    m_connectionStr += L"YES";
  else
    m_connectionStr += L"NO";
  m_connectionStr += L"\"";

  return m_connectionStr;
}

bool DbLinkConnection::connect(OdString conectiobStr)
{
  try
  {
    if ( ::CoInitialize(NULL) < 0 ) return false;

//     // Load the Excel application in the background.
//     Excel::_ApplicationPtr pApplication;

//     if (FAILED(pApplication.CreateInstance(_T("Excel.Application"))))
//     {
//       Errorf(_T("Failed to initialize Excel::_Application!"));
//       return;
//     }

    if (pCon.CreateInstance(__uuidof(Connection)) < 0)
      return false;

    _bstr_t connStr(conectiobStr.c_str());
    if (pCon->Open(connStr, "", "", NULL) < 0)  // id, password
       return false;

    return true;
  }
  catch (_com_error &e)
  {
    _bstr_t bstrDescription(e.Description());
    CharToOem(bstrDescription, bstrDescription);
    return false;
  }
}

OdString DbLinkConnection::sqlSelectSheet(int sheetIndex)
{
  pSchema = pCon->OpenSchema(adSchemaTables);

  for (int i = 0; i < sheetIndex; i++) 
    pSchema->MoveNext();

  OdString sheetName = (char*)(_bstr_t)pSchema->Fields->GetItem("TABLE_NAME")->Value.bstrVal;

  OdString select;
  select = "SELECT * FROM [" + sheetName + "]";
  return select;
}

void DbLinkConnection::selectRecordSet(OdString sheetName, OdString range)
{
  OdString select;
  select = "SELECT * FROM [" + sheetName + "$" + range + "]";
  try
  {
    pRec.CreateInstance(__uuidof(Recordset));
    pRec->Open(select.c_str(), m_connectionStr.c_str(), adOpenStatic, adLockOptimistic, adCmdText);

    m_records = pRec->GetRecordCount(); //  GetMaxRecords();
    m_fields = pRec->Fields->GetCount();
  }
  catch (_com_error &e)
  {
    _bstr_t bstrDescription(e.Description());
    CharToOem(bstrDescription, bstrDescription);
    return;
  }
}

void DbLinkConnection::getData(OdDbTableContentPtr content)
{
  OdInt32 valI = 0;
  double  valD = 0.0;
  OdString valStr;
  OdInt32 row = 0, col = 0;

  try
  {
    while (!pRec->adoEOF)
    {
      for (OdInt32 col = 0; col < pRec->Fields->GetCount(); col++)
      {
        _variant_t v = pRec->Fields->GetItem(col)->Value;
        unsigned char prec = pRec->Fields->GetItem(col)->GetPrecision();
        if (v.vt == VT_R8)
        {
          valD = v.dblVal;
          OdString valStr = odDToStr(valD, 'f', prec);
          int numTrailZeros = 0, length = valStr.getLength();
          for (int i = 0; i < length; i++)
          {
            if (valStr[length - i - 1] == '.')
            {
              numTrailZeros++;
              break;
            }
            else if (valStr[length - i - 1] == '0')
            {
              numTrailZeros++;
            }
            else
            {
              break;
            }
          }
          valStr = valStr.left(length - numTrailZeros);

          OdValue val;
          val.set(valStr);
          content->setValue(row, col, val);
        }
        else if (v.vt == VT_BSTR)
        {
          valStr = (char*)(_bstr_t)v.bstrVal;
          OdValue val;
          val.set(valStr);
          content->setValue(row, col, val);
        }
        else if (v.vt == VT_INT)
        {
          valI = v.dblVal;
          OdValue val;
          val.set(valI);
          content->setValue(row, col, val);
        }
      }
      pRec->MoveNext();
      row++;
    }
    pRec->Close();
  }
  catch (_com_error &e)
  {
    _bstr_t bstrDescription(e.Description());
    CharToOem(bstrDescription, bstrDescription);
    pRec->Close();
    return;
  }
}

void DbLinkConnection::putData(OdDbTableContentPtr content)
{
  OdInt32 valI = 0;
  double  valD = 0.0;
  OdString valStr;
  OdInt32 row = 0, col = 0;

  try
  {
    while (!pRec->adoEOF)
    {
      for (OdInt32 col = 0; col < pRec->Fields->GetCount(); col++)
      {
        OdValue val;
        val = content->getValue(row, col, 0, OdValue::kIgnoreMtextFormat);

        OdString strVal;
        val.get(strVal);

        _variant_t v = pRec->Fields->GetItem(col)->Value;
        DataTypeEnum dt;
        pRec->Fields->GetItem(col)->get_Type(&dt);
        if (dt == DataTypeEnum::adVarWChar)
        {
          pRec->Fields->GetItem(col)->Value = _variant_t(strVal.c_str());
        } else if (dt == DataTypeEnum::adDouble)
        {
          valD = odStrToD(strVal);
          pRec->Fields->GetItem(col)->Value = _variant_t(valD);

//           val = content->getValue(row, col, 0, OdValue::FormatOption(OdValue::kIgnoreMtextFormat | OdValue::kForExpression));
//           val.get(valD);
//           val.get(valI);
//           if ( valI == 0 )
//             pRec->Fields->GetItem(col)->Value = _variant_t(valD);
//           else
//             pRec->Fields->GetItem(col)->Value = _variant_t(valI);
        }
      }
      pRec->MoveNext();
      row++;
    }
    pRec->Close();
  }
  catch (_com_error &e)
  {
    _bstr_t bstrDescription(e.Description());
    CharToOem(bstrDescription, bstrDescription);
    pRec->Close();
    return;
  }
}

OdError DbDataAdapter::updateLink(OdDbDataLink* pDataLink, OdDb::UpdateDirection nDir, OdDb::UpdateOption nOption)
{
  OdString connStr = pDataLink->connectionString();
  OdString fileName;
  int idx = connStr.find('!');
  if (idx != -1)
   fileName = connStr.left(idx);

  int idx2 = connStr.find('!', idx+1);
  OdString sheetName;
  OdString range;

  if (idx2 != -1)
  {
    sheetName = connStr.mid(idx+1, idx2 - idx - 1);
    range = connStr.right(connStr.getLength() - idx2 - 1);
  } else
  {
    sheetName = connStr.right(connStr.getLength() - idx - 1);
  }

  // TODO: check file 
  DbLinkConnection lnkConnect;
  OdString cs = lnkConnect.makeConnStr(fileName, false);
  if (!lnkConnect.connect(cs))
    return OdError(eDataLinkConnectionFailed);

  lnkConnect.selectRecordSet(sheetName, range);

  OdDbTableContentPtr tblContent = pDataLink->contentId().safeOpenObject(OdDb::kForWrite);

  if (nDir == OdDb::kUpdateDirectionSourceToData)
  {
    if (lnkConnect.numRows() != tblContent->numRows() ||
      lnkConnect.numColumns() != tblContent->numColumns())
    {
      tblContent->setSize(lnkConnect.numRows(), lnkConnect.numColumns());
    }
    lnkConnect.getData(tblContent);
  }
  else
  {
    if (lnkConnect.numRows() < tblContent->numRows() ||
      lnkConnect.numColumns() < tblContent->numColumns())
      return OdError(eDataLinkOtherError);

    lnkConnect.putData(tblContent);
  }
  return OdError(eOk);
}
#endif

OdError DbDataAdapter::repathSourceFiles(OdString& sBasePath, OdDb::PathOption nOption, OdString& path)
{
  path = sBasePath;
  return OdError(eOk);
}

#ifdef LibXL_USED
#include "include_cpp\libxl.h"

using namespace libxl;

bool getCmColor(libxl::Color clr, OdCmColor& cmColor);
OdString getColorSwitch(libxl::Color clr);
OdDb::CellAlignment getAlignment(AlignH aH, AlignV aV);

class LibXLDbLinkConnection
{
  long m_records;
  long m_fields;
  OdString m_fileName;
  Book* m_book;
  Sheet* m_sheet;

  OdDb::UpdateOption m_options;
  OdString m_connectionStr;

public:
  LibXLDbLinkConnection(bool bXLSX) {
    if ( bXLSX )
      m_book = xlCreateXMLBook();
    else
      m_book = xlCreateBook();
#include "libxl.lic"
    m_records = 0;
    m_fields = 0;
  };

  ~LibXLDbLinkConnection()
  {
    m_book->release();
  };

  bool connect(OdString conectionStr);
  void selectRecordSet(OdString sheetName, OdString range);
  void getData(OdDbTableContentPtr content);
  void putData(OdDbTableContentPtr content);

  long numRows() { return m_records; }
  long numColumns() { return m_fields; }
  bool saveFile() { return m_book->save(m_fileName.c_str()); }
  void setUpdateOptions(OdDb::UpdateOption options) { m_options = options; }
};

bool LibXLDbLinkConnection::connect(OdString conectionStr)
{
  m_fileName = conectionStr;
  if ( m_book->load(conectionStr) )
  { 
    m_book->setRgbMode();
    return true;
  }
  return false;
}

void LibXLDbLinkConnection::selectRecordSet(OdString sheetName, OdString range)
{
  int cnt = m_book->sheetCount();
  int shInd = -1;
  for (int i = 0; i < cnt; i++)
  {
    if (sheetName == OdString(m_book->getSheet(i)->name()))
    {
      shInd = i;
      break;
    }
  }
  if (shInd != -1)
  {
    m_sheet = m_book->getSheet(shInd);
    int firstR = m_sheet->firstRow();
    int firstÑ = m_sheet->firstCol();
    m_records = m_sheet->lastRow(); // -1; ??
    m_fields = m_sheet->lastCol(); // -1; ??
  }

  if (!range.isEmpty())
  {
    int idx = range.find(':');
  }
}

void LibXLDbLinkConnection::getData(OdDbTableContentPtr content)
{
  OdString valStr;
  OdValue val;
  OdString fontSwitch;
  OdString colorSwitch;
  double fontSize = 0;
  for (int r = 0; r < m_records; r++)
  {
    double rH = m_sheet->rowHeight(r);
    for (int c = 0; c < m_fields; c++)
    {
      double cW = m_sheet->colWidth(c);
      // px = ([100 * {width}-0.5] * {Maximum Digit Width}) / 100 + 5,
      // where {Maximum Digit Width} = 7 for a default font.
      fontSize = 0.0;
      CellType type = m_sheet->cellType(r, c);
      Format* format = m_sheet->cellFormat(r, c);
      if (format)
      {
        OdCmColor cmColor;
        cmColor.setColorMethod(OdCmEntityColor::kNone);

        libxl::Color bgClr = format->patternForegroundColor();
        if ( bgClr != 0xFFFFFFFF && bgClr != COLOR_DEFAULT_FOREGROUND )
        {
          int red, green, blue;
          m_book->colorUnpack(bgClr, &red, &green, &blue);
          cmColor.setRGB(red, green, blue);
        }
        // getCmColor(bgClr, cmColor);
        content->setBackgroundColor(r, c, cmColor);

        FillPattern patt = format->fillPattern();
        OdDb::CellAlignment algn =  getAlignment(format->alignH(), format->alignV());
        content->setAlignment(r, c, algn);
        Font* fnt = format->font();
        if (fnt)
        {
          fontSwitch.format(OD_T("\\f%ls|b%ls|i%ls|c0;"), fnt->name()
            , fnt->bold() ? OD_T("1") : OD_T("0")
            , fnt->italic() ? OD_T("1") : OD_T("0") );
          if ( fnt->strikeOut() )
            fontSwitch += OD_T("\\K");
          if (fnt->underline())
            fontSwitch += OD_T("\\L");

          libxl::Color txtClr = fnt->color();
          // colorSwitch = getColorSwitch(txtClr);
          colorSwitch.empty();
          if (txtClr != 0xFFFFFFFF && txtClr != COLOR_NONE && txtClr != COLOR_AUTO )
          { 
            OdUInt32 swColor = 0;
            int red, green, blue;
            m_book->colorUnpack(txtClr, &red, &green, &blue);
            swColor = red + (green << 8) + (blue << 16);
            if ( swColor )
              colorSwitch.format(OD_T("\\c%d;"), swColor);
          } 
          fontSize = fnt->size() * 0.24;
        } else
          fontSwitch.empty();
        fnt = 0;
      }
      else
        fontSwitch.empty();
      format = 0;
      switch (type)
      {
        case libxl::CELLTYPE_EMPTY:
          valStr.empty();
          val.set(valStr);
          content->setValue(r, c, val);
          break;
        case libxl::CELLTYPE_NUMBER:
        {
          double valDouble = m_sheet->readNum(r,c);
          // OdString valStr = odDToStr(valDouble, 'f', 3);
          valStr.format(OD_T("%f"), valDouble);
          int numTrailZeros = 0, length = valStr.getLength();
          for (int i = 0; i < length; i++)
          {
            if (valStr[length - i - 1] == '.')
            {
              numTrailZeros++;
              break;
            }
            else if (valStr[length - i - 1] == '0')
            {
              numTrailZeros++;
            }
            else
            {
              break;
            }
          }
          valStr = valStr.left(length - numTrailZeros);
          valStr = fontSwitch + colorSwitch + valStr;
          val.set(valStr);
          content->setValue(r, c, val);
          break;
        }
        case libxl::CELLTYPE_STRING:
        {
          valStr = m_sheet->readStr(r, c);
          valStr = fontSwitch + colorSwitch + valStr;
          val.set(valStr);
          content->setValue(r, c, val);
          break;
        }
        case libxl::CELLTYPE_BOOLEAN:
        {
          bool b = m_sheet->readBool(r, c);
          break;
        }
        case libxl::CELLTYPE_BLANK:
          valStr.empty();
          val.set(valStr);
          content->setValue(r, c, val);
          break;
        case libxl::CELLTYPE_ERROR:
          break;
        default:
          break;
      }

//       if ( OdNonZero(fontSize) )
//         content->setTextHeight(r, c, 0, fontSize);
    }
  }
}

void LibXLDbLinkConnection::putData(OdDbTableContentPtr content)
{
  OdInt32 rows = content->numRows();
  OdInt32 cols = content->numColumns();
  OdValue val;
  OdString strVal;
  double valD = 0.0;

  for (int r = 0; r < rows; r++)
  {
    for (int c = 0; c < cols; c++)
    {
      val = content->getValue(r, c, 0, OdValue::kIgnoreMtextFormat);
      val.get(strVal);
      CellType type = m_sheet->cellType(r, c);
      switch (type)
      {
        case libxl::CELLTYPE_EMPTY:
          m_sheet->writeStr(r, c, strVal.c_str());
          break;
        case libxl::CELLTYPE_NUMBER:
          valD = odStrToD(strVal);
          m_sheet->writeNum(r, r, valD);
          break;
        case libxl::CELLTYPE_STRING:
          m_sheet->writeStr(r, c, strVal.c_str());
          break;
        case libxl::CELLTYPE_BOOLEAN:
          break;
        case libxl::CELLTYPE_BLANK:
          m_sheet->writeStr(r, c, strVal.c_str());
          break;
        case libxl::CELLTYPE_ERROR:
          break;
        default:
          break;
      }
    }
  }
}

OdError DbDataAdapter::updateLink(OdDbDataLink* pDataLink, OdDb::UpdateDirection nDir, OdDb::UpdateOption nOption)
{
  OdString connStr = pDataLink->connectionString();
  OdString fileName;
  int idx = connStr.find('!');
  if (idx != -1)
    fileName = connStr.left(idx);

  int idx2 = connStr.find('!', idx + 1);
  OdString sheetName;
  OdString range;

  if (idx2 != -1)
  {
    sheetName = connStr.mid(idx + 1, idx2 - idx - 1);
    range = connStr.right(connStr.getLength() - idx2 - 1);
  }
  else
  {
    sheetName = connStr.right(connStr.getLength() - idx - 1);
  }

  bool bXml = ( fileName.find(L".xlsx") != -1 );
  // TODO: check file 
  LibXLDbLinkConnection lnkConnect(bXml);

  if ( !lnkConnect.connect(fileName) )
    return OdError(eDataLinkSourceNotFound);

  lnkConnect.setUpdateOptions(nOption);

  lnkConnect.selectRecordSet(sheetName, range);

  OdDbTableContentPtr tblContent = pDataLink->contentId().safeOpenObject(OdDb::kForWrite);

  if (nDir == OdDb::kUpdateDirectionSourceToData)
  {
    if (lnkConnect.numRows() != tblContent->numRows() ||
      lnkConnect.numColumns() != tblContent->numColumns())
    {
      tblContent->setSize(lnkConnect.numRows(), lnkConnect.numColumns());
    }
    lnkConnect.getData(tblContent);
  }
  else
  {
    if (lnkConnect.numRows() < tblContent->numRows() ||
      lnkConnect.numColumns() < tblContent->numColumns())
      return OdError(eDataLinkOtherError);

    lnkConnect.putData(tblContent);
    if ( !lnkConnect.saveFile() )
      return OdError(eDataLinkSourceIsWriteProtected);
  }
  return OdError(eOk);
}

struct mapCOLOR
{
  libxl::Color m_clr;
  OdUInt16 m_red;
  OdUInt16 m_green;
  OdUInt16 m_blue;
} LibXLColorMap[]= {
{ COLOR_BLACK, 0, 0, 0 },
{ COLOR_WHITE, 255, 255, 255 },
{ COLOR_RED, 255, 0, 0 },
{ COLOR_BRIGHTGREEN, 0, 255, 255 },
{ COLOR_BLUE, 0, 0, 255 },
{ COLOR_YELLOW, 255, 255, 0 },
{ COLOR_PINK, 255, 0, 255 },
{ COLOR_TURQUOISE, 0, 255, 255 },
{ COLOR_DARKRED, 128, 0, 0 },
{ COLOR_GREEN, 0, 128, 0 },
{ COLOR_DARKBLUE, 0, 0, 128 },
{ COLOR_DARKYELLOW, 128, 128, 0 },
{ COLOR_VIOLET, 128, 0, 128 },
{ COLOR_TEAL, 0, 128, 128 },
{ COLOR_GRAY25, 192, 192, 192 },
{ COLOR_GRAY50, 128, 128, 128 },
{ COLOR_PERIWINKLE_CF, 153, 153, 255 },
{ COLOR_PLUM_CF, 153, 51, 102 },
{ COLOR_IVORY_CF, 255, 255, 204 },
{ COLOR_LIGHTTURQUOISE_CF, 204, 255, 255 },
{ COLOR_DARKPURPLE_CF, 102, 0, 102 },
{ COLOR_CORAL_CF, 255, 128, 128 },
{ COLOR_OCEANBLUE_CF, 0, 102, 204 },
{ COLOR_ICEBLUE_CF, 204, 204, 255 },
{ COLOR_DARKBLUE_CL, 0, 0, 128 },
{ COLOR_PINK_CL, 255, 0, 255 },
{ COLOR_YELLOW_CL, 255, 255, 0 },
{ COLOR_TURQUOISE_CL, 0, 255, 255 },
{ COLOR_VIOLET_CL, 128, 0, 128 },
{ COLOR_DARKRED_CL, 128, 0, 0 },
{ COLOR_TEAL_CL, 128, 0, 0 },
{ COLOR_BLUE_CL, 0, 0, 255 },
{ COLOR_SKYBLUE, 0, 204, 255 },
{ COLOR_LIGHTTURQUOISE, 204, 255, 255 },
{ COLOR_LIGHTGREEN, 204, 255, 204 },
{ COLOR_LIGHTYELLOW, 255, 255, 153 },
{ COLOR_PALEBLUE, 153, 204, 255 },
{ COLOR_ROSE, 255, 153, 204 },
{ COLOR_LAVENDER, 204, 153, 255 },
{ COLOR_TAN, 255, 204, 153 },
{ COLOR_LIGHTBLUE, 51, 102, 255 },
{ COLOR_AQUA, 51, 204, 204 },
{ COLOR_LIME, 153, 204, 0 },
{ COLOR_GOLD, 255, 204, 0 },
{ COLOR_LIGHTORANGE, 255, 153, 0 },
{ COLOR_ORANGE, 255, 102, 0 },
{ COLOR_BLUEGRAY, 102, 102, 153 },
{ COLOR_GRAY40, 150, 150, 150 },
{ COLOR_DARKTEAL, 0, 51, 102 },
{ COLOR_SEAGREEN, 51, 153, 102 },
{ COLOR_DARKGREEN, 0, 51, 0 },
{ COLOR_OLIVEGREEN, 51, 51, 0 },
{ COLOR_BROWN, 153, 51, 0 },
{ COLOR_PLUM, 153, 51, 102 },
{ COLOR_INDIGO, 51, 51, 153 },
{ COLOR_GRAY80, 51, 51, 51 }
};


OdString getColorSwitch(libxl::Color clr)
{
  OdString retStr;
  int sizeMap = sizeof(LibXLColorMap) / sizeof(mapCOLOR);
  int i = 0;
  for ( i = 0; i < sizeMap; i++)
  {
    if ( LibXLColorMap[i].m_clr == clr )
      break;
  }
  if (i != sizeMap)
  {
    OdUInt32 swColor = 0;
    swColor = LibXLColorMap[i].m_red + ( LibXLColorMap[i].m_green << 8 ) + ( LibXLColorMap[i].m_blue << 16 );
    if (swColor)
      retStr.format(OD_T("\\c%d;"), swColor);
  }
  return retStr;
}

bool getCmColor(libxl::Color clr, OdCmColor& cmColor)
{
  cmColor.setColorMethod(OdCmEntityColor::kNone);
  int sizeMap = sizeof(LibXLColorMap) / sizeof(mapCOLOR);
  int i = 0;
  for (i = 0; i < sizeMap; i++)
  {
    if (LibXLColorMap[i].m_clr == clr)
      break;
  }
  if (i != sizeMap)
  {
    OdUInt32 swColor = 0;
    swColor = LibXLColorMap[i].m_red + (LibXLColorMap[i].m_green << 8) + (LibXLColorMap[i].m_blue << 16);
    if (swColor)
    {
      cmColor.setRGB(LibXLColorMap[i].m_red, LibXLColorMap[i].m_green, LibXLColorMap[i].m_blue);
      return true;
    }
  }
  return false;
}

OdDb::CellAlignment getAlignment(AlignH aH, AlignV aV)
{
  OdDb::CellAlignment algn = OdDb::CellAlignment::kMiddleLeft;
  if ( aV == ALIGNV_TOP )
  {
    if ( aH == ALIGNH_CENTER )
      return OdDb::CellAlignment::kTopCenter;
    else if (aH == ALIGNH_RIGHT)
      return OdDb::CellAlignment::kTopRight;
    else
      return OdDb::CellAlignment::kTopLeft;
  } else if ( aV == ALIGNV_CENTER)
  {
    if (aH == ALIGNH_CENTER)
      return OdDb::CellAlignment::kMiddleCenter;
    else if (aH == ALIGNH_RIGHT)
      return OdDb::CellAlignment::kMiddleRight;
    else
      return OdDb::CellAlignment::kMiddleLeft;
  }
  else if (aV == ALIGNV_BOTTOM)
  {
    if (aH == ALIGNH_CENTER)
      return OdDb::CellAlignment::kBottomCenter;
    else if (aH == ALIGNH_RIGHT)
      return OdDb::CellAlignment::kBottomRight;
    else
      return OdDb::CellAlignment::kBottomLeft;
  }
  return algn;
}

#endif
