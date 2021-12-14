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
#include "PdfImportCmd.h"
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

#include "DbBasePdfImportObjectGrabber.h"
#include "DbPdfImportObjectGrabber.h"
#include "DbProgressMeterGrabber.h"

#include <memory>
#include <functional>
#include "OdPlatform.h"
#include "Gi/TtfDescriptor.h"


class PdfImportPointTracker : public OdStaticRxObject<OdEdPointTracker>
{
  OdDbDatabase*  m_pDb;
  OdDb3dSolidPtr m_ent;
  const double   m_Width;
  const double   m_Height;
  const double   m_Angle;

public:
  PdfImportPointTracker(OdDbDatabase* pDb, OdDb3dSolidPtr ent, const double width, const double height, const double angle)
    : m_pDb(pDb)
    , m_ent(ent)
    , m_Width(width)
    , m_Height(height)
    , m_Angle(angle)
  {
  }

  static OdGeMatrix3d getUcsToWcsMatrix(OdDbDatabase* pDb)
  {
    OdGeMatrix3d ucsToWcsMatrix;
    ucsToWcsMatrix.setCoordSystem(pDb->getUCSORG(), pDb->getUCSXDIR(), pDb->getUCSYDIR(),
      pDb->getUCSXDIR().crossProduct(pDb->getUCSYDIR()));
    return ucsToWcsMatrix;
  }

  virtual int addDrawables(OdGsView* pView)
  {
    pView->add(m_ent, 0);
    return 1;
  }

  virtual void removeDrawables(OdGsView* pView)
  {
    pView->erase(m_ent);
  }

  virtual void setValue(const OdGePoint3d& value)
  {
    m_ent->createBox(m_Width, m_Height, 1);
    OdGePoint3d center = value + OdGeVector3d(m_Width / 2.0, m_Height / 2.0, 0);
    m_ent->transformBy(OdGeMatrix3d::translation(center.asVector()));
    m_ent->transformBy(OdGeMatrix3d::rotation(m_Angle, OdGeVector3d::kZAxis, value));
    m_ent->transformBy(getUcsToWcsMatrix(m_pDb));
  }

};

class PdfImportSourceTracker : public OdStaticRxObject<OdEdSSetTracker>
{

public:
  PdfImportSourceTracker()
  {}
  virtual int addDrawables(OdGsView*) { return 0; }
  virtual void removeDrawables(OdGsView*) {}

  virtual bool append(const OdDbObjectId& entId, const OdDbSelectionMethod*)
  {
    OdDbObjectPtr obj = entId.openObject();
    return !obj.isNull() && obj->isKindOf(OdDbPdfReference::desc());
  }
  virtual bool remove(const OdDbObjectId&, const OdDbSelectionMethod*)
  {
    return true;
  }
};

class EmbeddedFontHandler
{
public:
  EmbeddedFontHandler() {};
  ~EmbeddedFontHandler()
  {
#if defined(OD_WINDOWS_DESKTOP)
    for (auto& elem : m_FontsList)
    {
      RemoveFontResourceW(elem.c_str());
    }
#endif
  }
#if defined(OD_WINDOWS_DESKTOP)
  bool AddFontList(const OdStringArray& font_files_name)
  {
    bool ret = true;
    for (auto& file_name : font_files_name)
    {
      if(0 == AddFontResourceW(file_name.c_str()))
        ret = false;
      m_FontsList.push_back(file_name);
    }
    return ret;
  }
#else
  bool AddFontList(OdStringArray& font_files_name, OdDbHostAppServices* pHostApp)
  {
    bool ret = true;
    for (auto& file_name : font_files_name)
    {
      OdTtfDescriptor descr;
      if (!pHostApp->ttfFileNameByDescriptor(descr, file_name))
        ret = false;
      m_FontsList.push_back(file_name);
    }
    return ret;
  }
#endif

  OdStringArray m_FontsList;
};

EmbeddedFontHandler embedded_font_handler;

void PdfImportCommand::execute(OdEdCommandContext* pCmdCtx)
{
  std::function<void(OdPdfImport::ImportResult res)> error_func = [](OdPdfImport::ImportResult res)
  {
    switch (res)
    {
    case OdPdfImport::fail:
      throw OdError(eTxError);
    case OdPdfImport::bad_password:
      throw OdError(eInvalidPassword);
    case OdPdfImport::bad_file:
      throw OdError(eFileNotFound);
    case OdPdfImport::bad_database:
      throw OdError(eWrongDatabase);
    case OdPdfImport::encrypted_file:
      throw OdError(eDecryptionError);
    case OdPdfImport::invalid_page_number:
      throw OdError(eInvalidIndex);
    case OdPdfImport::image_file_error:
      throw OdError(eFilerError);
    case OdPdfImport::no_objects_imported:
      throw OdError(eEmptySet);
    }
  };

  PdfImporterExPtr importer = createImporter();
  OdRxDictionaryPtr pProps = importer->properties();

  OdDbUserIOPtr pIO = OdDbCommandContextPtr(pCmdCtx)->dbUserIO();

  std::function<void(PdfImporterPtr importer)> find_password_func = [&error_func, &pCmdCtx, &pIO](PdfImporterPtr importer)
  {
    OdPdfImport::ImportResult load_res = importer->loadDocument();
    if (OdPdfImport::success != load_res)
    {
      if (OdPdfImport::bad_password != load_res)
        error_func(load_res);
      //check password cache for the valid password.
      OdPwdCachePtr pCache = OdDbCommandContextPtr(pCmdCtx)->database()->appServices()->getPasswordCache();
      OdPassword cache_password;
      if (!pCache.isNull())
      {
        OdPwdIteratorPtr pIter = pCache->newIterator();
        if (!pIter.isNull())
        {
          for (; !pIter->done() && load_res != OdPdfImport::success; pIter->next())
          {
            pIter->get(cache_password);
            importer->properties()->putAt(OD_T("Password"), OdRxVariantValue(cache_password));
            load_res = importer->loadDocument();
          }
        }
      }
      if (OdPdfImport::bad_password == load_res)
      {
        // ask user about valid password
        cache_password = pIO->getString(OD_T("password <>:"));

        // try to load again with new password
        importer->properties()->putAt(OD_T("Password"), OdRxVariantValue(cache_password));
        load_res = importer->loadDocument();
        if (OdPdfImport::bad_password == load_res)
        {
          throw OdError(eInvalidPassword);
        }
        else
        {
          pCache->add(cache_password);
        }
      }
    }
  };

  try
  {

    PdfImportSourceTracker trecker;

    OdDbSelectionSetPtr pSSet;
    OdDbPdfReferencePtr pPdfRef;
    OdDbPdfDefinitionPtr pPdfDef;

    bool is_file_import(false);
    do 
    {
      is_file_import = pIO->getKeyword(L"Enter source for import [File/Underlay]", L"File Underlay", 0) == 0;
      if (!is_file_import)
      {
        pSSet = pIO->select(OD_T("Select PDF underlay:"), OdEd::kSelSingleEntity| OdEd::kSelSinglePass, NULL, OdString::kEmpty, &trecker);
        if(pSSet->numEntities())
          break;
      }
    } 
    while (!is_file_import);

    // Target database
    pProps->putAt(OD_T("Database"), OdDbCommandContextPtr(pCmdCtx)->database());

    OdString path;
    OdString password;
    OdUInt32 page_num(0);
    double scale(1.0), rotation(0.0);
    if (is_file_import)
    {
      path = pIO->getFilePath(OD_T("Enter file name:"), OdEd::kGfpForOpen, OD_T("Pdf file to load"),
        OD_T("pdf"), OdString::kEmpty, OD_T("Design Web Format (*.pdf)|*.pdf|PDF file (*.pdf)||"));
      password = pIO->getString(OD_T("password <>:"));

      // Source PDF file
      pProps->putAt(OD_T("PdfPath"), OdRxVariantValue(path));
      // PDF may be password protected
      pProps->putAt(OD_T("Password"), OdRxVariantValue(password));

      importer->loadDocument();
      OdUInt32 page_count = importer->getPageCount();

      std::function<bool(const OdChar* s)> isNumber = [](const OdChar* s)
      {
        while (*s && Od_iswspace(*s))
          ++s;
        while (*s && Od_iswdigit(*s))
          ++s;
        while (*s && Od_iswspace(*s))
          ++s;
        return *s == 0;
      };
      while (true)
      {
        OdString page_num_str = pIO->getString(OD_T("Enter page number or [?] <1>:"), OdEd::kInpDefault, OD_T("1"));
        if (0 == page_num_str.compare(OD_T("?")))
        {
          bool page_list_not_intered = true;
          while (true)
          {
            page_num_str = pIO->getString(OD_T("Enter page to list <*>:"), OdEd::kInpDefault, OD_T("*"));
            if (0 == page_num_str.compare(OD_T("*")))
            {
              for (OdUInt32 i = 1; i < page_count + 1; ++i)
                pIO->putString(OdString().format(OD_T("%d"), i));
              break;
            }
            else
            {
              if (!isNumber(page_num_str))
              {
                pIO->putString(OD_T("Please enter page number or *."));
                continue;
              }
              OdUInt32 page_in_list = ::odStrToInt(page_num_str);
              if (page_in_list > 0 && page_count + 1 > page_in_list)
              {
                pIO->putString(OD_T("Found page ") + page_num_str);
              }
              else
              {
                pIO->putString(OD_T("No matching page found."));
              }
              break;
            }
          }
        }
        else
        {
          if (!isNumber(page_num_str))
          {
            pIO->putString(OD_T("Please enter page number or ?."));
            continue;
          }
          OdUInt32 page_intered = ::odStrToInt(page_num_str);
          if (page_intered > 0 && page_count + 1 > page_intered)
          {
            page_num = page_intered;
            break;
          }
          else
          {
            pIO->putString(OdString().format(OD_T("There is no %d in "), page_intered) + path);
            continue;
          }
        }
      }

        
      //page_num = (OdInt32)pIO->getInt(OD_T("Select page number to import <1>:"), OdEd::kInpNonZero | OdEd::kInpNonNeg, 1);
      rotation = pIO->getReal(OD_T("Select rotation <0.>:"), OdEd::kInpDefault, 0.0);
      scale = pIO->getReal(OD_T("Select scale <1.>:"), OdEd::kInpNonNeg, 1.0);

      importer->clearUnderlayInfo();


    }
    else
    {
      pPdfRef = OdDbPdfReference::cast(pSSet->objectIdArray().first().openObject(OdDb::kForWrite));
      if (pPdfRef.isNull())
        return;
      pPdfDef = OdDbPdfDefinition::cast(pPdfRef->definitionId().openObject(OdDb::kForWrite));
      if (pPdfDef.isNull())
        return;

      path = pPdfDef->getActiveFileName();
      page_num = odStrToUInt(pPdfDef->getItemName());
      scale = odmax(pPdfRef->scaleFactors().sx, pPdfRef->scaleFactors().sy);
      rotation = pPdfRef->rotation();
      OdGePoint3d insertion_point = pPdfRef->position();
      pProps->putAt(OD_T("InsertionPointX"), OdRxVariantValue(insertion_point.x));
      pProps->putAt(OD_T("InsertionPointY"), OdRxVariantValue(insertion_point.y));

      if (2 == pPdfRef->clipBoundary().size())
      {
        OdGePoint2dArray clip_boundary = pPdfRef->clipBoundary();
        if (clip_boundary[0].x > clip_boundary[1].x)
          std::swap(clip_boundary[0].x, clip_boundary[1].x);
        if (clip_boundary[0].y > clip_boundary[1].y)
          std::swap(clip_boundary[0].y, clip_boundary[1].y);

        OdGePoint2d pt0 = clip_boundary[0], pt1 = clip_boundary[1];
        clip_boundary.resize(4);
        clip_boundary[0].set(pt0.x, pt0.y);
        clip_boundary[1].set(pt0.x, pt1.y);
        clip_boundary[2].set(pt1.x, pt1.y);
        clip_boundary[3].set(pt1.x, pt0.y);
        importer->setClipBoundary(clip_boundary, pPdfRef->isClipInverted());
      }
      else
        importer->setClipBoundary(pPdfRef->clipBoundary(), pPdfRef->isClipInverted());

      std::map<std::wstring, bool> layers_info;
      for (OdUInt32 i = 0; i < pPdfRef->underlayLayerCount(); ++i)
      {
        OdUnderlayLayer layer;
        pPdfRef->getUnderlayLayer(i, layer);
        std::wstring layer_name = layer.name().c_str();
        layers_info.emplace(std::make_pair(layer_name.c_str(), OdUnderlayLayer::kOn == layer.state()));
      }
      importer->setLayersInfo(layers_info);

    }
    if (path.isEmpty())
      return;


    // Source PDF file
    pProps->putAt(OD_T("PdfPath"), OdRxVariantValue(path));
    // PDF may be password protected
    pProps->putAt(OD_T("Password"), OdRxVariantValue(password));

    // PDF may contain many sheets. You may select which one to import
    pProps->putAt(OD_T("PageNumber"), OdRxVariantValue(page_num));
    pProps->putAt(OD_T("Rotation"), OdRxVariantValue(rotation));
    pProps->putAt(OD_T("Scaling"), OdRxVariantValue(scale));


    find_password_func(importer);

    bool change_settings = pIO->getKeyword(L"Change default settings [Yes/No]", L"Yes No", 1) == 0;

    if (change_settings)
    {
      pProps->putAt(OD_T("LayersUseType"), OdRxVariantValue(
        (OdUInt8)pIO->getKeyword(L"Enter method to apply for assigning imported objects to layers[UsePDFLayers/ObjectLayers/CurrentLayer]",
          L"UsePDFLayers ObjectLayers CurrentLayer", 1)));

      pProps->putAt(OD_T("ImportVectorGeometry"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable import of vector geometry [Yes/No]"), OD_T("Yes No"), 0) == 0)));
      pProps->putAt(OD_T("ImportSolidFills"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable import of solid fills [Yes/No]"), OD_T("Yes No"), 0) == 0)));
      pProps->putAt(OD_T("ImportTrueTypeText"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable import of TrueType text [Yes/No]"), OD_T("Yes No"), 0) == 0)));
      pProps->putAt(OD_T("ImportRasterImages"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable import of raster images [Yes/No]:"), OD_T("Yes No"), 0) == 0)));
      pProps->putAt(OD_T("ImportGradientFills"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable import of shading objects [Yes/No]"), OD_T("Yes No"), 0) == 0)));
      pProps->putAt(OD_T("ImportWidgets"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable import of widgets objects [Yes/No]"), OD_T("Yes No"), 0) == 0)));
      pProps->putAt(OD_T("ImportAsBlock"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable import as block [Yes/No]"), OD_T("Yes No"), 1) == 0)));
      pProps->putAt(OD_T("JoinLineAndArcSegments"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable joins contiguous segments into a polyline where possible [Yes/No]"), OD_T("Yes No"), 1) == 0)));
      pProps->putAt(OD_T("ConvertSolidsToHatches"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable converts 2D solid objects into solid-filled hatches [Yes/No]"), OD_T("Yes No"), 0) == 0)));
      pProps->putAt(OD_T("ApplyLineweight"), OdRxVariantValue((pIO->getKeyword(OD_T("Apply lineweight properties [Yes/No]"), OD_T("Yes No"), 0) == 0)));
      pProps->putAt(OD_T("ImportTrueTypeTextAsGeometry"), OdRxVariantValue((pIO->getKeyword(OD_T("Import TrueType text as geometry [Yes/No]"), OD_T("Yes No"), 1) == 0)));
      pProps->putAt(OD_T("UseGeometryOptimization"), OdRxVariantValue((pIO->getKeyword(OD_T("Use 2D geometry optimization [Yes/No]"), OD_T("Yes No"), 0) == 0)));
      pProps->putAt(OD_T("ImportType3AsTrueTypeText"), OdRxVariantValue((pIO->getKeyword(OD_T("Import Type3 text as TrueType text [Yes/No]"), OD_T("Yes No"), 0) == 0)));
      pProps->putAt(OD_T("UseRgbColor"), OdRxVariantValue((pIO->getKeyword(OD_T("Import color as RGB color [Yes/No]"), OD_T("Yes No"), 0) == 0)));
      pProps->putAt(OD_T("ImportInvalidTTTextAsGeometry"), OdRxVariantValue((pIO->getKeyword(OD_T("Import invalid TrueType text as geometry [Yes/No]"), OD_T("Yes No"), 0) == 0)));
      pProps->putAt(OD_T("UseClipping"), OdRxVariantValue((pIO->getKeyword(OD_T("Use Clipping [Yes/No]"), OD_T("Yes No"), 1) == 0)));
      pProps->putAt(OD_T("UsePageBorderClipping"), OdRxVariantValue((pIO->getKeyword(OD_T("Use Clipping [Yes/No]"), OD_T("Yes No"), 1) == 0)));
      pProps->putAt(OD_T("UseImageClipping"), OdRxVariantValue((pIO->getKeyword(OD_T("Use Clipping [Yes/No]"), OD_T("Yes No"), 1) == 0)));
      pProps->putAt(OD_T("CombineTextObjects"), OdRxVariantValue((pIO->getKeyword(OD_T("Combine text objects [Yes/No]"), OD_T("Yes No"), 1) == 0)));
      pProps->putAt(OD_T("UsePdfTransparency"), OdRxVariantValue((pIO->getKeyword(OD_T("Use transparency from PDF stream [Yes/No]"), OD_T("Yes No"), 1) == 0)));
      pProps->putAt(OD_T("ImportHyperlinkMethod"), OdRxVariantValue(
        (OdUInt8)pIO->getKeyword(L"Enter method to export hyperlink[ImportAsObjectsHyperlink/ImportAsTrasparentHatch/MixedMethod]",
          L"ImportAsObjectsHyperlink ImportAsTrasparentHatch MixedMethod", 0)));
      pProps->putAt(OD_T("ConsiderINSUNITS"), OdRxVariantValue((pIO->getKeyword(OD_T("Consider INSUNITS from database [Yes/No]"), OD_T("Yes No"), 1) == 0)));
      bool font_replace_settings = pIO->getKeyword(L"Apply font replacement mechanisms [Yes/No]", L"Yes No", 1) == 0;
      if (font_replace_settings)
      {
        pProps->putAt(OD_T("Type3TextDefaultFont"), OdRxVariantValue(pIO->getString(OD_T("Default font for Type3 fonts"), OdEd::kGstAllowSpaces)));
        bool replace_not_found_font = pIO->getKeyword(L"Replace the preferred font name for fonts that are not found [Yes/No]", L"Yes No", 1) == 0;
        OdAnsiString nf_new_font_str;
        if (replace_not_found_font)
        {
          nf_new_font_str = pIO->getString(OD_T("Specify the preferred font name for fonts that are not found"), OdEd::kGstAllowSpaces);
        }

        struct FontInfoData
        {
          OdAnsiString font_name;
          OdAnsiString font_family;
          OdAnsiString preferred_font_name;
        };
        std::vector<FontInfoData> font_info_data;

        bool font_replace_settings = pIO->getKeyword(L"Select the preferred font name for specific fonts [Yes/No]", L"Yes No", 1) == 0;
        while (font_replace_settings)
        {
          FontInfoData font_data;
          font_data.font_name = pIO->getString(OD_T("Specify the FontName to be replaced"), OdEd::kGstAllowSpaces);
          font_data.font_family = pIO->getString(OD_T("Specify the FontFamily to be replaced"), OdEd::kGstAllowSpaces);
          if (font_data.font_name.isEmpty() && font_data.font_family.isEmpty())
          {
            pIO->putString(OD_T("Specify a non-empty FontName or FontFamily"));
            continue;
          }

          font_data.preferred_font_name = pIO->getString(OD_T("Specify the preferred font name to replace"), OdEd::kGstAllowSpaces);
          if (font_data.preferred_font_name.isEmpty())
          {
            pIO->putString(OD_T("Specify a non-empty preferred font name"));
            continue;
          }

          font_info_data.emplace_back(font_data);

          font_replace_settings = pIO->getKeyword(L"Select next the preferred font name for specific fonts [Yes/No]", L"Yes No", 1) == 0;

        }
        if (replace_not_found_font || 0 != font_info_data.size())
        {
          importer->setPreferableFontCallback([replace_not_found_font, nf_new_font_str, font_info_data](const char* font_name, const char* font_family, const bool is_found)
            {
              OdAnsiString ret;
              if (replace_not_found_font && !nf_new_font_str.isEmpty() && !is_found)
                ret = nf_new_font_str;
              auto iter = std::find_if(font_info_data.begin(), font_info_data.end(), [font_name, font_family](const FontInfoData& elem)
                {
                  if (elem.font_name.isEmpty())
                  {
                    return (0 == elem.font_family.compare(font_family));
                  }
                  else
                  {
                    if (elem.font_family.isEmpty())
                      return (0 == elem.font_name.compare(font_name));
                    else
                      return (0 == elem.font_name.compare(font_name)) && (0 == elem.font_family.compare(font_family));
                  }
                });
              if (font_info_data.end() != iter)
                ret = iter->preferred_font_name;
              return ret;
            });
        }
      }

      pProps->putAt(OD_T("ExtractEmbeddedFonts"), OdRxVariantValue((pIO->getKeyword(OD_T("Extract Embedded Fonts from PDF [Yes/No]"), OD_T("Yes No"), 1) == 0)));

    }

    if (is_file_import)
    {
      error_func(importer->loadPage());

      const double measure = importer->getMeasureDictInfo();
      OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
      pSolid->setDatabaseDefaults(OdDbCommandContextPtr(pCmdCtx)->database());
      PdfImportPointTracker tracker(OdDbCommandContextPtr(pCmdCtx)->database(), pSolid, importer->getPageWidth()*measure / 72.,
        importer->getPageHeight()*measure / 72., OdRxVariantValue(pProps->getAt(OD_T("Rotation"))));
      OdGePoint3d insertion_point;
      insertion_point = pIO->getPoint(OD_T("Specify insertion point: "), OdEd::kGptDefault, &insertion_point, OD_T("Insertion point"), &tracker);
      pProps->putAt(OD_T("InsertionPointX"), OdRxVariantValue(insertion_point.x));
      pProps->putAt(OD_T("InsertionPointY"), OdRxVariantValue(insertion_point.y));
    }
    else
    {
      pProps->putAt(OD_T("TakeAddMeasurementFactors"), OdRxVariantValue(false));
      OdUInt8 post_action = pIO->getKeyword(L"Keep, Detach or Unload PDF underlay? [Keep/Detach/Unload]", L"Keep Detach Unload", 2);
      switch (post_action)
      {
      case 1:
        {
          OdDbBlockTableRecordPtr pBlock = OdDbCommandContextPtr(pCmdCtx)->database()->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);
          OdDbObjectIteratorPtr pIter = pBlock->newIterator();
          for (pIter->start(); !pIter->done(); pIter->step())
          {
            OdDbEntityPtr pEntity = pIter->entity();
            OdDbPdfReferencePtr pRef = OdDbPdfReference::cast(pEntity);
            if (!pRef.isNull() && pRef->definitionId() == pPdfDef->objectId())
            {
              pRef->upgradeOpen();
              pRef->erase();
            }
          }
          pPdfDef->erase();
        }
        break;
      case 2:
        pPdfDef->unload();
        break;
      }
    }
  }
  catch (const OdEdKeyword&)
  {
  }

  if (OdRxVariantValue(importer->properties()->getAt(L"ExtractEmbeddedFonts"))->getBool())
  {
    OdStringArray font_files_name;
    importer->extractEmbededFonts(font_files_name);
#if defined(OD_WINDOWS_DESKTOP)
    embedded_font_handler.AddFontList(font_files_name);
#else
    embedded_font_handler.AddFontList(font_files_name, OdDbCommandContextPtr(pCmdCtx)->database()->appServices());
#endif
  }

  OdPdfImport::ImportResult res = importer->import();

  pIO->putString(OD_T("PdfImport complete"));
  error_func(res);
}

