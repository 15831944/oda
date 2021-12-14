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


#if defined(_MSC_VER)
// warning C4290: C++ Exception Specification ignored
#pragma warning ( disable : 4290 )
#if _MSC_VER <= 1200
// compiler stack overflows if these warning is enabled
#pragma warning ( disable : 4786)
#endif
#endif

#include "OdaCommon.h"
#include "RxDispatchImpl.h"
#include "RxVariantValue.h"
#include "DbDictionary.h"
#include "RxDynamicModule.h"
#include "Ed/EdCommandStack.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "DbCommandContext.h"
#include "StaticRxObject.h"
#include "PdfProperties.h"
#include "PdfImportImpl.h"
#include "DbDatabase.h"
#include "DbHostAppServices.h"
#include "DbBlockTableRecord.h"
#include "DbUnderlayReference.h"
#include "DbUnderlayDefinition.h"
#include "Ge/GeScale3d.h"
#include "OdPath.h"
#include "PdfImportCmd.h"
#include "OdUtilAds.h"

#include "DbPdfImportObjectGrabber.h"
#include "DbProgressMeterGrabber.h"

#include <memory>
#include <functional>
#include "OdPlatform.h"
#include <set>


OdRxDictionaryPtr PdfProperties::createObject()
{
  return OdRxObjectImpl<PdfProperties, OdRxDictionary>::createObject();
}


ODRX_DECLARE_PROPERTY(PdfPath)
ODRX_DECLARE_PROPERTY(Password)
ODRX_DECLARE_PROPERTY(Database)
ODRX_DECLARE_PROPERTY(PageNumber)
ODRX_DECLARE_PROPERTY(LayersUseType)
ODRX_DECLARE_PROPERTY(ImportVectorGeometry)
ODRX_DECLARE_PROPERTY(ImportSolidFills)
ODRX_DECLARE_PROPERTY(ImportTrueTypeText)
ODRX_DECLARE_PROPERTY(ImportRasterImages)
ODRX_DECLARE_PROPERTY(ImportAsBlock)
ODRX_DECLARE_PROPERTY(JoinLineAndArcSegments)
ODRX_DECLARE_PROPERTY(ConvertSolidsToHatches)
ODRX_DECLARE_PROPERTY(ApplyLineweight)
ODRX_DECLARE_PROPERTY(InsertionPointX)
ODRX_DECLARE_PROPERTY(InsertionPointY)
ODRX_DECLARE_PROPERTY(Scaling)
ODRX_DECLARE_PROPERTY(Rotation)
ODRX_DECLARE_PROPERTY(ImportTrueTypeTextAsGeometry)
ODRX_DECLARE_PROPERTY(ImportGradientFills)
ODRX_DECLARE_PROPERTY(UseProgressMeter)
ODRX_DECLARE_PROPERTY(UseGeometryOptimization)
ODRX_DECLARE_PROPERTY(ImagePath)
ODRX_DECLARE_PROPERTY(ImportType3AsTrueTypeText)
ODRX_DECLARE_PROPERTY(UseRgbColor)
ODRX_DECLARE_PROPERTY(ImportInvalidTTTextAsGeometry)
ODRX_DECLARE_PROPERTY(UseClipping)
ODRX_DECLARE_PROPERTY(UsePageBorderClipping)
ODRX_DECLARE_PROPERTY(TakeAddMeasurementFactors)
ODRX_DECLARE_PROPERTY(CombineTextObjects)
ODRX_DECLARE_PROPERTY(CharSpaceFactor)
ODRX_DECLARE_PROPERTY(SpaceFactor)
ODRX_DECLARE_PROPERTY(UsePdfTransparency)
ODRX_DECLARE_PROPERTY(ImportHyperlinkMethod)
ODRX_DECLARE_PROPERTY(ImportWidgets)
ODRX_DECLARE_PROPERTY(UseImageClipping)
ODRX_DECLARE_PROPERTY(ConsiderINSUNITS)
ODRX_DECLARE_PROPERTY(Type3TextDefaultFont)
ODRX_DECLARE_PROPERTY(ExtractEmbeddedFonts)
ODRX_DECLARE_PROPERTY(EbeddedFontsPath)


ODRX_DEFINE_PROPERTY(PdfPath, PdfProperties, getString)
ODRX_DEFINE_PROPERTY(Password, PdfProperties, getString)
ODRX_DEFINE_PROPERTY(PageNumber, PdfProperties, getInt32)
ODRX_DEFINE_PROPERTY_OBJECT(Database, PdfProperties,  get_Database, put_Database, OdDbDatabase)
ODRX_DEFINE_PROPERTY(LayersUseType, PdfProperties, getUInt8)
ODRX_DEFINE_PROPERTY(ImportVectorGeometry, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportSolidFills, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportTrueTypeText, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportRasterImages, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportAsBlock, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(JoinLineAndArcSegments, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ConvertSolidsToHatches, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ApplyLineweight, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(InsertionPointX, PdfProperties, getDouble)
ODRX_DEFINE_PROPERTY(InsertionPointY, PdfProperties, getDouble)
ODRX_DEFINE_PROPERTY(Scaling, PdfProperties, getDouble)
ODRX_DEFINE_PROPERTY(Rotation, PdfProperties, getDouble)
ODRX_DEFINE_PROPERTY(ImportTrueTypeTextAsGeometry, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportGradientFills, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(UseProgressMeter, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(UseGeometryOptimization, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImagePath, PdfProperties, getString)
ODRX_DEFINE_PROPERTY(ImportType3AsTrueTypeText, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(UseRgbColor, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportInvalidTTTextAsGeometry, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(UseClipping, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(UsePageBorderClipping, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(TakeAddMeasurementFactors, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(CombineTextObjects, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(CharSpaceFactor, PdfProperties, getDouble)
ODRX_DEFINE_PROPERTY(SpaceFactor, PdfProperties, getDouble)
ODRX_DEFINE_PROPERTY(UsePdfTransparency, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportHyperlinkMethod, PdfProperties, getUInt8)
ODRX_DEFINE_PROPERTY(ImportWidgets, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(UseImageClipping, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ConsiderINSUNITS, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(Type3TextDefaultFont, PdfProperties, getString)
ODRX_DEFINE_PROPERTY(ExtractEmbeddedFonts, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(EbeddedFontsPath, PdfProperties, getString)


ODRX_BEGIN_DYNAMIC_PROPERTY_MAP( PdfProperties );
  ODRX_GENERATE_PROPERTY( PdfPath )
  ODRX_GENERATE_PROPERTY( Password )
  ODRX_GENERATE_PROPERTY( Database )
  ODRX_GENERATE_PROPERTY(PageNumber)
  ODRX_GENERATE_PROPERTY(LayersUseType)
  ODRX_GENERATE_PROPERTY(ImportVectorGeometry)
  ODRX_GENERATE_PROPERTY(ImportSolidFills)
  ODRX_GENERATE_PROPERTY(ImportTrueTypeText)
  ODRX_GENERATE_PROPERTY(ImportRasterImages)
  ODRX_GENERATE_PROPERTY(ImportAsBlock)
  ODRX_GENERATE_PROPERTY(JoinLineAndArcSegments)
  ODRX_GENERATE_PROPERTY(ConvertSolidsToHatches)
  ODRX_GENERATE_PROPERTY(ApplyLineweight)
  ODRX_GENERATE_PROPERTY(InsertionPointX)
  ODRX_GENERATE_PROPERTY(InsertionPointY)
  ODRX_GENERATE_PROPERTY(Scaling)
  ODRX_GENERATE_PROPERTY(Rotation)
  ODRX_GENERATE_PROPERTY(ImportTrueTypeTextAsGeometry)
  ODRX_GENERATE_PROPERTY(ImportGradientFills)
  ODRX_GENERATE_PROPERTY(UseProgressMeter)
  ODRX_GENERATE_PROPERTY(UseGeometryOptimization)
  ODRX_GENERATE_PROPERTY(ImagePath)
  ODRX_GENERATE_PROPERTY(ImportType3AsTrueTypeText)
  ODRX_GENERATE_PROPERTY(UseRgbColor)
  ODRX_GENERATE_PROPERTY(ImportInvalidTTTextAsGeometry)
  ODRX_GENERATE_PROPERTY(UseClipping)
  ODRX_GENERATE_PROPERTY(UsePageBorderClipping)
  ODRX_GENERATE_PROPERTY(TakeAddMeasurementFactors)
  ODRX_GENERATE_PROPERTY(CombineTextObjects)
  ODRX_GENERATE_PROPERTY(CharSpaceFactor)
  ODRX_GENERATE_PROPERTY(SpaceFactor)
  ODRX_GENERATE_PROPERTY(UsePdfTransparency)
  ODRX_GENERATE_PROPERTY(ImportHyperlinkMethod)
  ODRX_GENERATE_PROPERTY(ImportWidgets)
  ODRX_GENERATE_PROPERTY(UseImageClipping)
  ODRX_GENERATE_PROPERTY(ConsiderINSUNITS)
  ODRX_GENERATE_PROPERTY(Type3TextDefaultFont)
  ODRX_GENERATE_PROPERTY(ExtractEmbeddedFonts)
  ODRX_GENERATE_PROPERTY(EbeddedFontsPath)
ODRX_END_DYNAMIC_PROPERTY_MAP( PdfProperties );

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#pragma warning(disable:4355)
#endif

PdfImporter::PdfImporter() 
  : m_pProperties( PdfProperties::createObject() )
  , m_pDocument(nullptr)
  , m_Page(nullptr)
  , m_PageNum(-1)
  , m_isClipBoundaryInverted(false)
  , m_PreferableFontCallback(nullptr)
{
}

PdfImporter::~PdfImporter()
{
  if (isPageLoaded())
  {
    FPDF_ClosePage(m_Page);
  }
  if (isDocumentLoaded())
  {
    FPDF_CloseDocument(m_pDocument);
  }
}

OdRxDictionaryPtr PdfImporter::properties() { return m_pProperties; }

#define TOCHAR(a) ((char *)OdAnsiString(OdString(a)).c_str())

double PdfImporter::getPageWidth() const
{
  return FPDF_GetPageWidth(m_Page);
}

void PdfImporter::setClipBoundary(const OdGePoint2dArray& clipBoundary, const bool is_clip_boundary_inverted)
{
  m_ClipBoundary = clipBoundary;
  m_isClipBoundaryInverted = is_clip_boundary_inverted;
}

void PdfImporter::setLayersInfo(std::map<std::wstring, bool>& layers_info)
{
  m_LayersInfo = layers_info;
}

void PdfImporter::clearUnderlayInfo()
{
  m_ClipBoundary.clear();
  m_LayersInfo.clear();
  m_isClipBoundaryInverted = false;
}

void PdfImporter::getImportedObjectsInfo(size_t& object_count, size_t& error_count)
{
  if (m_Import_info)
  {
    object_count = m_Import_info->m_ObjectCounter;
    error_count = m_Import_info->m_ErrorCounter;
  }
  else
  {
    object_count = 0;
    error_count = 0;
  }
}

class BaseFontEnumPdfImportObjectGrabber : public OdPdfImportBaseObjectGrabber
{
public:

  BaseFontEnumPdfImportObjectGrabber()
  {

  }

  virtual bool needToAddGrathics(bool* ret)
  {
    return false;
  }

  virtual bool needToAddImage(bool* ret)
  {
    return false;
  }


  virtual bool needToAddShading(bool* ret)
  {
    return false;
  }


  virtual bool needToAddText(bool* ret)
  {
    return true;
  }

  virtual float getWidth()
  {
    return 100;
  }
  virtual float getHeight()
  {
    return 100;
  }
};


class FontEnumPdfImportObjectGrabber : public BaseFontEnumPdfImportObjectGrabber
{
public:

  explicit FontEnumPdfImportObjectGrabber(std::map<OdAnsiString, OdAnsiString>& font_map)
    :m_FontMap(font_map)
  {

  }

  virtual bool getPreferableFont(const char* font_name, const char* font_family, const bool is_found, char* preferable_font_name, unsigned int& font_name_size)
  {
    m_FontMap[font_name] = font_family;
    return false;
  };

  std::map<OdAnsiString, OdAnsiString>& m_FontMap;
};

class EmbeddedFontEnumPdfImportObjectGrabber : public BaseFontEnumPdfImportObjectGrabber
{
public:

  explicit EmbeddedFontEnumPdfImportObjectGrabber(const OdString& save_path, const OdString& pdf_file_name, std::set<OdString>& font_list, bool& is_ok)
    :m_FileSet(font_list)
    ,m_EbeddedFontsPath(save_path)
    ,m_PdfFileName(pdf_file_name)
    ,m_IsOk(is_ok)
  {
    m_IsOk = true;
  }

  virtual void addText(const wchar_t* text, const PdfImportFontParams* const font_params, const OdPdfImportColor* color,
    const OdPdfImportMatrix* object_matrix, const bool is_object_visible, const wchar_t* layer_name, const char* hyperlink = nullptr)
  {
    if (nullptr != font_params->embedded_font_data)
    {
      OdString font_file_name = m_EbeddedFontsPath + m_PdfFileName + L"_" + font_params->font_file_name + ".ttf";
      
      if (m_FileSet.end() == m_FileSet.find(font_file_name))
      {

        try
        {
          OdStreamBufPtr font_file = odSystemServices()->createFile(
            font_file_name, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
          font_file->putBytes(font_params->embedded_font_data, font_params->embedded_font_data_size);
          if (font_file.isNull())
            m_IsOk = false;
        }
        catch (...)
        {
          m_IsOk = false;
        }
        m_FileSet.insert(font_file_name);
      }
    }
  }

  virtual bool extractEmbeddedFonts() const
  {
    return true;
  }

  std::set<OdString>& m_FileSet;
  OdString m_EbeddedFontsPath;
  OdString m_PdfFileName;
  bool& m_IsOk;
};

OdPdfImport::ImportResult PdfImporter::fillFontsInfo(std::map<OdAnsiString, OdAnsiString>& font_map)
{
  if(!isDocumentLoaded() || !isPageLoaded())
    return OdPdfImport::fail;

  auto pGraber = std::make_shared<FontEnumPdfImportObjectGrabber>(font_map);

  long options(0);
  SETBIT(options, kImportTTFontAsGeometry, m_pProperties->get_ImportTrueTypeTextAsGeometry());
  SETBIT(options, kImportType3FontAsTT, m_pProperties->get_ImportType3AsTrueTypeText());
  SETBIT(options, kImportInvalidFontAsGeometry, m_pProperties->get_ImportInvalidTTTextAsGeometry());
  SETBIT(options, kImportWidgets, m_pProperties->get_ImportWidgets());

  FPDF_ImportPage(m_Page, pGraber.get(), options);

  return OdPdfImport::success;
}

void PdfImporter::setPreferableFontCallback(PreferableFontCallback callback)
{
  m_PreferableFontCallback = callback;
}

OdPdfImport::ImportResult PdfImporter::extractEmbededFonts(OdStringArray& font_list) const
{
  if (!isDocumentLoaded() || !isPageLoaded())
    return OdPdfImport::fail;

  OdString ebedded_fonts_path = m_pProperties->get_EbeddedFontsPath();
  if (ebedded_fonts_path.isEmpty())
  {
    OdDbDatabasePtr pDb = OdDbDatabasePtr(m_pProperties->get_Database());
    ebedded_fonts_path = pDb->appServices()->getTempPath();
  }
  if (0 != ebedded_fonts_path.getLength() && ebedded_fonts_path[ebedded_fonts_path.getLength() - 1] != pathChar)
    ebedded_fonts_path += pathChar;

  std::set<OdString> file_list;
  bool is_ok;
  auto pGraber = std::make_shared<EmbeddedFontEnumPdfImportObjectGrabber>(ebedded_fonts_path,
    OdDbPdfImportObjectGrabber::extractFileName(m_pProperties->get_PdfPath()), file_list, is_ok);

  long options(0);
  SETBIT(options, kImportTTFontAsGeometry, m_pProperties->get_ImportTrueTypeTextAsGeometry());
  SETBIT(options, kImportType3FontAsTT, m_pProperties->get_ImportType3AsTrueTypeText());
  SETBIT(options, kImportInvalidFontAsGeometry, m_pProperties->get_ImportInvalidTTTextAsGeometry());
  SETBIT(options, kImportWidgets, m_pProperties->get_ImportWidgets());

  FPDF_ImportPage(m_Page, pGraber.get(), options);

  std::copy(file_list.begin(), file_list.end(), std::back_inserter(font_list));
  
  if (is_ok)
    return OdPdfImport::success;
  else
    return OdPdfImport::font_file_error;
}

double PdfImporter::getPageHeight() const
{
  return FPDF_GetPageHeight(m_Page);
}

double PdfImporter::getMeasureDictInfo()
{
// CORE-16156 : Behavior is identical to A... 2021:
//  * If a pdf document contains one viewport, the biggest measuring coefficient from the page measuring coefficientand the measuring coefficient of the viewport is taken.
//  * If a pdf document contains more than one viewport, the page measuring coefficient is taken.

  std::vector<FPDF_PageMeasureDictInfo> measure_info;
  FPDF_GetMeasureDictInfo(m_pDocument, m_PageNum, measure_info);
  if (0 == measure_info.size())
    return 1.;

  if (m_pProperties->get_TakeAddMeasurementFactors() && 2 == measure_info.size())
  {
    return odmax(measure_info[0].measure_factor, measure_info[1].measure_factor)*72.0;
  }

  return measure_info[0].measure_factor*72.0;
}

OdPdfImport::ImportResult PdfImporter::fillLayersInfo(std::map<std::wstring, bool>& layers_info)
{

  FPDF_ARRAY pONLayers = 0, pOFFLayers = 0, pLayers = 0;
  if (!FPDF_LoadLayers(m_pDocument, pLayers, pONLayers, pOFFLayers))
    return OdPdfImport::fail;

  if (!FPDF_GetPageLayers(m_pDocument, m_Page, m_pProperties->get_PageNumber() - 1, m_PageInfo))
    return OdPdfImport::fail;

  for (int i = 0; i < m_PageInfo.nol; ++i)
  {
    layers_info.emplace(m_PageInfo.names[i], FPDF_IsLayerEnabled(pOFFLayers, m_PageInfo.names[i]));
  }

  return OdPdfImport::success;
}

OdPdfImport::ImportResult PdfImporter::import()
{
  OdPdfImport::ImportResult result(OdPdfImport::success);
  try
  {
    result = loadDocument();
    if (OdPdfImport::success != result)
      return result;
    result = loadPage();
    if (OdPdfImport::success != result)
      return result;

    m_Import_info = std::make_shared<PdfImportInfoHolder>(result);

    OdRxDictionaryIteratorPtr pIt = m_pProperties->newIterator();
    while (!pIt->done())
    {
      m_Import_info->m_pProperties->putAt(pIt->getKey(), pIt->object());
      pIt->next();
    }

    OdDbDatabasePtr pDb = OdDbDatabasePtr(m_Import_info->m_pProperties->get_Database());

    OdString image_path = m_Import_info->m_pProperties->get_ImagePath();
    if (image_path.isEmpty())
      image_path =  pDb->appServices()->getPDFIMPORTIMAGEPATH();
    if(image_path.isEmpty())
      image_path = pDb->appServices()->getTempPath();
    if (0 != image_path.getLength() && image_path[image_path.getLength() - 1] != pathChar)
      image_path += pathChar;

    double insunits_scale(1.0);
    if (m_pProperties->get_ConsiderINSUNITS())
    {
      OdDb::UnitsValue base_unit = pDb->getINSUNITS();
      if (base_unit > OdDb::kUnitsUndefined && base_unit <= OdDb::kUnitsMax)
      {
        insunits_scale = oddbGetUnitsConversion(OdDb::kUnitsInches, base_unit);
      }
    }

    const double measure = getMeasureDictInfo();
    m_Import_info->m_pProperties->put_Scaling(m_pProperties->get_Scaling() * measure * insunits_scale);

    m_Import_info->m_Width = getPageWidth();
    m_Import_info->m_Height = -getPageHeight();

    std::map<std::wstring, bool> layers_info(m_LayersInfo);
    result = fillLayersInfo(layers_info);
    if (OdPdfImport::success != result)
      return result;

    OdGePoint2dArray clip_boundary(m_ClipBoundary);
    for(auto& elem : clip_boundary)
      elem /= measure;

    long options(0);
    SETBIT(options, kImportTTFontAsGeometry, m_Import_info->m_pProperties->get_ImportTrueTypeTextAsGeometry());
    SETBIT(options, kImportType3FontAsTT, m_Import_info->m_pProperties->get_ImportType3AsTrueTypeText());
    SETBIT(options, kImportInvalidFontAsGeometry, m_Import_info->m_pProperties->get_ImportInvalidTTTextAsGeometry());
    SETBIT(options, kImportWidgets, m_Import_info->m_pProperties->get_ImportWidgets());

    if(m_Import_info->m_pProperties->get_UseProgressMeter())
    {
      auto pGraber = std::make_shared<DbProgressMeterGrabber>(m_Import_info);

      FPDF_ImportPage(m_Page, pGraber.get(), options);

      if (0 == m_Import_info->m_ObjectCounter)
        return OdPdfImport::no_objects_imported;

      m_Import_info->InitProgressMeter();
      m_Import_info->ProgressMeterSetLimit();
      OdString cmd_line(OD_T("Importing pdf file..."));
      m_Import_info->StartProgressMeter(cmd_line);
    }

    auto pGraber = std::make_shared<OdDbPdfImportObjectGrabber>(m_Import_info, layers_info, clip_boundary, m_isClipBoundaryInverted, image_path, m_PreferableFontCallback);

    FPDF_ImportPage(m_Page, pGraber.get(), options);

    if (m_Import_info->m_pProperties->get_UseProgressMeter())
    {
      m_Import_info->StopProgressMeter();
    }

    result = m_Import_info->m_ImageError;
    if (0 == m_Import_info->m_ObjectCounter || m_Import_info->m_ObjectCounter == m_Import_info->m_ErrorCounter)
      result = OdPdfImport::no_objects_imported;
  }
  catch (...)
  {
    return OdPdfImport::fail;
  }

  return result;
}

OdPdfImport::ImportResult PdfImporter::loadDocument()
{
  if (isDocumentLoaded())
    return OdPdfImport::success;

  OdString path(m_pProperties->get_PdfPath());

  OdString password = m_pProperties->get_Password();

  m_pDocument = FPDF_LoadDocument(path.c_str(), TOCHAR(password.c_str()));

  if (nullptr == m_pDocument)
  {
    unsigned long error = FPDF_GetLastError();
    if (FPDF_ERR_PASSWORD == error)
      return OdPdfImport::bad_password;
    return OdPdfImport::fail;
  }
  return OdPdfImport::success;
}

bool PdfImporter::isDocumentLoaded() const
{
  return (nullptr != m_pDocument);
}

OdPdfImport::ImportResult PdfImporter::loadPage()
{
  if (isPageLoaded())
    return OdPdfImport::success;
  if (!isDocumentLoaded())
    return OdPdfImport::fail;

  m_PageNum = m_pProperties->get_PageNumber() - 1;

  if (m_PageNum >= getPageCount())
    return OdPdfImport::invalid_page_number;

  m_Page = FPDF_LoadPage(m_pDocument, m_PageNum);

  if (nullptr == m_Page)
    return OdPdfImport::fail;

  return OdPdfImport::success;
}

bool PdfImporter::isPageLoaded() const
{
  return (nullptr != m_Page);
}

OdUInt32 PdfImporter::getPageCount() const
{
  if (!isDocumentLoaded())
    return 0;
  return FPDF_GetPageCount(m_pDocument);
}

class InitFontsHostApp : public OdDbBaseHostAppServices
{
public:
  virtual const OdString program() { return OdString::kEmpty; }
  virtual const OdString product() { return OdString::kEmpty; }
  virtual const OdString companyName() { return OdString::kEmpty; }
  virtual OdString getAlternateFontName() const { return OdString::kEmpty; }
  virtual OdString getFontMapFileName() const { return OdString::kEmpty; }
  virtual OdString getPreferableFont(
    const OdString& fontName,
    OdFontType fontType) {
    return OdString::kEmpty;
  }
  virtual OdString getSubstituteFont(
    const OdString& fontName,
    OdFontType fontType) {
    return OdString::kEmpty;
  }

  virtual OdString findFile(const OdString& filename, OdDbBaseDatabase* pDb = 0, FindFileHint hint = kDefault)
  {
    return OdString::kEmpty;
  }
};


class PdfImportModule : public OdPdfImportModule
{
  OdStaticRxObject<PdfImportCommand> m_Command;
public:
  virtual void initApp()
  {
    OdStaticRxObject<InitFontsHostApp> host_app;

    OdStringArray font_folders;
    host_app.getSystemFontFolders(font_folders);

    std::vector<const char*> font_folders_array(font_folders.length());
    for (OdStringArray::size_type i = 0; i < font_folders.size(); ++i)
    {
      font_folders_array[i] = font_folders[i].operator const char* ();
    }

    font_folders_array.push_back(nullptr);

    FPDF_LIBRARY_CONFIG cfg;
    cfg.m_pIsolate = nullptr;
    cfg.version = 2;
    cfg.m_v8EmbedderSlot = 0;
    cfg.m_pUserFontPaths = font_folders_array.data();

    FPDF_InitLibraryWithConfig(&cfg);
    odedRegCmds()->addCommand(&m_Command);
  }
  virtual void uninitApp()
  {
    FPDF_DestroyLibrary();
    odedRegCmds()->removeCmd(m_Command.groupName(), m_Command.globalName());
  }

  virtual OdPdfImportPtr create()
  {
    return OdRxObjectImpl<PdfImporter>::createObject();
  }

};

ODRX_DEFINE_DYNAMIC_MODULE(PdfImportModule)
