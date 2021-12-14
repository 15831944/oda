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
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <ppltasks.h>
#include <stdio.h>
#include <stdlib.h>

#include "StaticRxObject.h"
#include "RxInit.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "DgDatabase.h"
#include "RxDynamicModule.h"

#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"

#include "OdPlatformStreamer.h"
#include "RxObjectImpl.h"
#include "RxVariantValue.h"
#include "DgGsManager.h"
#include "DgGiContext.h"
#include "Gi/GiRasterImage.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseInclude.h"
#include "ColorMapping.h"
#include "RxRasterServices.h"

using namespace OdaDgnWinRTApp;

using namespace Platform;
using namespace concurrency;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Foundation::Metadata;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::ViewManagement;

#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExFieldEvaluatorModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
//Dgn Import
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,     ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(RX_RASTER_SERVICES_APPNAME,      ExRasterModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName,     OdRasterProcessingServicesImpl)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName,   OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdExFieldEvaluatorModuleName,    ExFieldEvaluatorModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdWinBitmapModuleName,           BitmapModule)

  //Dgn
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db",              OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO",          OdDgn7IOModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_ModelerGeometry",   DgModelerModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdDbModuleName,          OdDwgModule)
ODRX_END_STATIC_MODULE_MAP()
#endif
#include "WinRTAppService.h"

static MainPage^ root = nullptr;

/* Assert*/
static void _MyAssert(const char *expression, const char *fileName, int nLineNo)
{
//todo
}
MainPage::MainPage()
{
  InitializeComponent();
  if (root == nullptr)
  {
    root = this;
  }
  ODRX_INIT_STATIC_MODULE_MAP();
  odSetAssertFunc(_MyAssert);
  try 
  {
    odrxInitialize(WinRTAppServices::getInstance());
    Results->Text = L"OdInitialize OK";
  }
  catch (...)
  {
    Results->Text = L"OdInitialize failed with error";
    return;
  }
  gsDevice = NULL;
 //   odgsInitialize();
  ::odrxDynamicLinker()->loadModule(L"TG_Db", false);
}

void MainPage::Unitialize()
{
  try
  {
    odrxUninitialize();
    Results->Text = L"ODA Platform uninitialized\n";
  }
  catch(...)
  {
    Results->Text = L"odUninitialize() failed\n";
  }
}
void MainPage::clearDevice()
{
  if (!gsDevice.isNull())
  {
    gsDevice.release();
  }
  if (!pDb.isNull())
  {
    pDb.release();
  }
}
/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
    orientationChangedEventToken = DisplayProperties::OrientationChanged::add(
        ref new DisplayPropertiesEventHandler(this, &MainPage::OnOrientationChanged)
        );
}

void MainPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
    DisplayProperties::OrientationChanged::remove(orientationChangedEventToken);
}

void MainPage::OnOrientationChanged(Object^ sender)
{
        drawBmpImage();
}
void MainPage::drawBmpImage()
{
  if (!pDevice.isNull())
  {
    OdGsDCRect screenRect(OdGsDCPoint(0, 768), OdGsDCPoint(1024, 0));
    pDevice->onSize(screenRect);

    pDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(24)));

    pDevice->update();

    OdGiRasterImagePtr ptrImage = pDevice->properties()->getAt( OD_T("RasterImage") );

    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);

    if (!pRasSvcs.isNull())
    {
      auto str = Windows::Storage::ApplicationData::Current->LocalFolder->Path + L"//" + mSelectedFileName + L".bmp";
      OdString dstFileName(str->Data());
      pRasSvcs->saveRasterImage(OdGiRasterImagePtr(gsDevice->properties()->getAt(OD_T("RasterImage"))), dstFileName);

      // Set the image source to the selected bitmap
      auto fp = L"ms-appdata:///local/" + mSelectedFileName + L".bmp";
      Uri^ uri = ref new Uri(fp);
      create_task(StorageFile::GetFileFromApplicationUriAsync(uri)).then([this](StorageFile^ file)
      {
        if (file)
        {
          // Ensure the stream is disposed once the image is loaded
          create_task(file->OpenAsync(Windows::Storage::FileAccessMode::Read)).then([this](IRandomAccessStream^ fileStream)
          {
            //auto bitmapImage = ref new BitmapImage(uri);
            //ExampleImage->Source = bitmapImage;

            auto bitmapImage = ref new BitmapImage();
            switch (DisplayProperties::CurrentOrientation)
            {
              case DisplayOrientations::Landscape:
              case DisplayOrientations::LandscapeFlipped:
                bitmapImage->DecodePixelHeight = 600;
                bitmapImage->DecodePixelWidth = 1200;
                break;
              case DisplayOrientations::Portrait:
              case DisplayOrientations::PortraitFlipped:
              default:
                bitmapImage->DecodePixelHeight = 700;
                bitmapImage->DecodePixelWidth = 700;
                break;
            }
           
         
            bitmapImage->SetSource(fileStream);
            ExampleImage->Source = bitmapImage;
          });
        }
      });
    }
    else
    {
      Results->Text = L"Failed to created bmp file";
    }
  }
}
void MainPage::OnViewEx(Platform::Object^ sender, 
          Windows::UI::Xaml::RoutedEventArgs^ e)
{
  try 
  {
    auto localFilePath = Windows::Storage::ApplicationData::Current->LocalFolder->Path + "//" + mSelectedFileName;
    Results->Text = L"ViewEx: " + mSelectedFileName + "\n";

    /////////////////////////////////
    // Load database
    pDb = WinRTAppServices::getInstance()->readFile(localFilePath->Data()); 
    if ( pDb.isNull() )
    {
      // load database failed
      ExampleDump->Text = L"Load database failed";
      return;
    }
    else 
    {
      // Extended check for bad files
      OdDgModelPtr pModel;
      {
        OdDgElementId activeModelId = pDb->getActiveModelId();

        if( activeModelId.isNull() )
        {
          activeModelId = pDb->getDefaultModelId();
          pDb->setActiveModelId (activeModelId);
        }

        if( activeModelId.isNull() )
        {
          ExampleDump->Text = L"Incorrect Active Model Id\n";
          return;
        }

        pModel = OdDgModel::cast( activeModelId.openObject( OdDg::kForRead ) );
        if( pModel.isNull() )
        {
          ExampleDump->Text = L"Can not open Active Model\n";
          return;
        }
      }


            // Create the custom rendering device and set the output stream for the device.
      if ( !gsDevice.isNull() ) 
      {
        gsDevice.release();
      }
      
      if ( gsDevice.isNull() ) 
      {
        OdGsModulePtr pGsModule = ::odrxDynamicLinker()->loadModule(OdWinBitmapModuleName);
        gsDevice = pGsModule->createBitmapDevice();
      }
      
      if ( !gsDevice.isNull() ) 
      {
        // Create the rendering context.
        OdGiContextForDgDatabasePtr pDgnContext = OdGiContextForDgDatabase::createObject();
        pDgnContext->setDatabase(pDb);

        OdDgElementId vectorizedViewId;
        OdDgViewGroupPtr pViewGroup = pDb->getActiveViewGroupId().openObject();
        if( pViewGroup.isNull() )
        {
          //  Some files can have invalid id for View Group. Try to get & use a valid (recommended) View Group object.
          pViewGroup = pDb->recommendActiveViewGroupId().openObject();
          if( pViewGroup.isNull() )
          {
            // Add View group
            pModel->createViewGroup();
            pModel->fitToView();
            pViewGroup = pDb->recommendActiveViewGroupId().openObject();
          }
        }
        if( !pViewGroup.isNull() )
        {
          OdDgElementIteratorPtr pIt = pViewGroup->createIterator();
          for( ; !pIt->done(); pIt->step() )
          {
            OdDgViewPtr pView = OdDgView::cast( pIt->item().openObject() );
            if( pView.get() && pView->getVisibleFlag() )
            {
              vectorizedViewId = pIt->item();
              break;
            }
          }
        }

        if( vectorizedViewId.isNull() )
        {
          ExampleDump->Text = L"Can not find an active view group or all its views are disabled\n";
          return;
        }

        pDgnContext->setView( vectorizedViewId.openObject( OdDg::kForRead ) );

        pDevice = OdGsDeviceForDgModel::setupModelView(pDb->getActiveModelId(), vectorizedViewId, gsDevice, pDgnContext);
        const ODCOLORREF* refColors = OdDgColorTable::currentPalette( pDb );
        ODGSPALETTE pPalCpy;
        pPalCpy.insert(pPalCpy.begin(), refColors, refColors + 256);
        OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject();
        ODCOLORREF background = pModel->getBackground();
        // Color with #255 always defines backround. The background of the active model must be considered in the device palette.
        pPalCpy[255] = background;
        // Note: This method should be called to resolve "white background issue" before setting device palette
        bool bCorrected = OdDgColorTable::correctPaletteForWhiteBackground(pPalCpy.asArrayPtr());
        pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
        pDevice->setBackgroundColor( background );
        pDgnContext->setPaletteBackground( background );
        drawBmpImage();
      }
    }
  }
  catch (OdError& e)
  {
    ExampleDump->Text += L"OdError: " + ref new String((wchar_t*)e.description().c_str());

  }
  catch (...)
  {
    Results->Text = L"\nUnknown Exception Caught\n";
  }
}

void MainPage::OnSelectFile(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
  // Initialize file picker
  auto open = ref new FileOpenPicker();
  open->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;//PicturesLibrary;
  open->ViewMode = PickerViewMode::Thumbnail;
  
  // Filter to include a sample subset of file types
  open->FileTypeFilter->Clear();
  open->FileTypeFilter->Append(".bmp");
  open->FileTypeFilter->Append(".dgn");
  
  // Open a stream for the selected file
  create_task(open->PickSingleFileAsync()).then([this](StorageFile^ file)
  {
    if (file)
    {
      auto str = file->Path;
      mSelectedFileName = file->Name;
      this->Results->Text = file->Name;
      file->CopyAsync(Windows::Storage::ApplicationData::Current->LocalFolder);
    }
  });
}
