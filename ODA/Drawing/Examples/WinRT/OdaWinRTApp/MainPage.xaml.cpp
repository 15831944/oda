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

using namespace OdaWinRTApp;

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



ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,     ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(RX_RASTER_SERVICES_APPNAME,      ExRasterModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName,     OdRasterProcessingServicesImpl)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName,   OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdExFieldEvaluatorModuleName,    ExFieldEvaluatorModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdWinBitmapModuleName,           BitmapModule)
ODRX_END_STATIC_MODULE_MAP()

#endif
#include "WinRTAppService.h"
#include <DbDumper.h>


static MainPage^ root = nullptr;
/* Helper function to write dump results*/
//typedef void (*PrintConsoleFunc) (const wchar_t* fmt, ...);
void winrtPrint(const wchar_t* fmt, ...)
{
    if (root)
    {
        va_list argList;
        va_start(argList, fmt);
        OdString lineToTrace;
        lineToTrace.formatV(fmt, argList);
        va_end(argList);
        root->addDumpText(ref new String((wchar_t*)lineToTrace.c_str()));
    }
}

/* Assert*/
static void _MyAssert(const char *expression, const char *fileName, int nLineNo)
{
    winrtPrint(L"MyAssert: assertion happened");
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
        odInitialize(WinRTAppServices::getInstance());
        Results->Text = L"OdInitialize OK";
    }
    catch (...)
    {
        Results->Text = L"OdInitialize failed with error";
        return;
    }
    gsDevice = NULL;
    odgsInitialize();
    whatEx = OdaWinRTApp::OdExample::none;
}

void MainPage::Unitialize()
{
    try
    {
      odUninitialize();
      Results->Text = L"Teigha uninitialized\n";
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
    if (whatEx == OdaWinRTApp::OdExample::OdViewEx)
        drawBmpImage();
}

void MainPage::addDumpText(String^ text)
{
    ExampleDump->Text += text + L"\n";
}

void MainPage::OnReadEx(Platform::Object^ sender, 
                    Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if (whatEx == OdaWinRTApp::OdExample::OdViewEx)
    {
        auto bitmapImage = ref new BitmapImage();
        bitmapImage->DecodePixelHeight = 0;
        bitmapImage->DecodePixelWidth = 0;
        ExampleImage->Source = bitmapImage;
    }
    if (whatEx !=  OdaWinRTApp::OdExample::none)
    {
        clearDevice();
    }
    ExampleDump->Text = "";
    whatEx = OdaWinRTApp::OdExample::OdReadEx;
    if (root == nullptr)
    {
        root = this;
    }

    auto localFilePath = Windows::Storage::ApplicationData::Current->LocalFolder->Path + "//" + mSelectedFileName;
    Results->Text = L"ReadEx: " + mSelectedFileName + "\n";

    try 
    {
      ::odrxDynamicLinker()->loadModule(OdExFieldEvaluatorModuleName);
      ::odrxDynamicLinker()->loadModule(OdDynBlocksModuleName);
      /******************************************************************/
      /* Create a database and load the drawing into it.                */
      /*                                                                */
      /* Specified arguments are as followed:                           */
      /*     filename, allowCPConversion, partialLoad, openMode         */      
      /******************************************************************/
        pDb = WinRTAppServices::getInstance()->readFile(
            localFilePath->Data(), true, false, Oda::kShareDenyNo); 

        if (!pDb.isNull())
        {
            oddbEvaluateFields(pDb, OdDbField::kOpen);
            /****************************************************************/
            /* Display the File Version                                     */
            /****************************************************************/
            int version__ = pDb->originalFileVersion(); 

            ExProtocolExtension pext;
            pext.initialize();
    
            OdString str;
            str.format(L"\nFile Version: %d \n", pDb->originalFileVersion());
            ExampleDump->Text += ref new String((wchar_t*)str.c_str());

            DbDumper dumper;
            odExSetPrintConsoleFunc(&(winrtPrint));
            dumper.dump(pDb);
        }
    }
    catch (OdError& e)
    {
        ExampleDump->Text += L"OdError: " + ref new String((wchar_t*)e.description().c_str());
    }
    catch (...)
    {
      ExampleDump->Text += L"\nUnknown Error\n";
    }
}

void MainPage::drawBmpImage()
{
    // Resize
    gsDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(24)));
    // Set the screen size for the generated geometry.
    int backingHeight = 600,  backingWidth = 740;
    OdGsDCRect screenRect( OdGsDCPoint(0, backingHeight), OdGsDCPoint(backingWidth, 0) );
    gsDevice->onSize(screenRect);

    // Zoom to extents
    OdAbstractViewPEPtr(gsDevice->viewAt(0))->zoomExtents(gsDevice->viewAt(0));
    gsDevice->update();
            
    OdGiRasterImagePtr ptrImage = gsDevice->properties()->getAt( OD_T("RasterImage") );

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
void MainPage::OnViewEx(Platform::Object^ sender, 
                    Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if (whatEx == OdaWinRTApp::OdExample::OdReadEx)
    {
        ExampleDump->Text = "";
    }
    if (whatEx !=  OdaWinRTApp::OdExample::none)
    {
        clearDevice();
    }
    whatEx = OdaWinRTApp::OdExample::OdViewEx;

    try 
    {
        auto localFilePath = Windows::Storage::ApplicationData::Current->LocalFolder->Path + "//" + mSelectedFileName;
		Results->Text = L"ViewEx: " + mSelectedFileName + "\n";
        // Load database
        pDb = WinRTAppServices::getInstance()->readFile(
            localFilePath->Data(), true, false, Oda::kShareDenyNo); 
        if ( pDb.isNull() )
        {
            // load database failed
            ExampleDump->Text = L"Load database failed";
            return;
        }
        else 
        {
            // Create the rendering context.
            OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();
            
            // Create the custom rendering device and set the output stream for the device.
            if ( !gsDevice.isNull() ) 
            {
                gsDevice.release();
            }
            
            if ( gsDevice.isNull() ) 
            {
                OdGsModulePtr m = ::odrxDynamicLinker()->loadModule(OdWinBitmapModuleName);
                gsDevice = m->createBitmapDevice();
            }                        
            
            // Set the database to be vectorized.
            pDwgContext->setDatabase(pDb);
            
            // Prepare the device to render the active layout in this database.            
            gsDevice = OdDbGsManager::setupActiveLayoutViews(gsDevice, pDwgContext);                         
            
            // Setup device palette
            if (pDb->getTILEMODE())//OdGsPaperLayoutHelper::cast(gsDevice).isNull())
            {
                // ModelSpace
                gsDevice->setLogicalPalette(::odcmAcadDarkPalette(), 256);
                gsDevice->setBackgroundColor(ODRGB(0, 0, 0));
                pDwgContext->setPaletteBackground(ODRGB(0, 0, 0));
            }
            else
            {
                gsDevice->setLogicalPalette(::odcmAcadLightPalette(), 256);
                gsDevice->setBackgroundColor(ODRGB(168, 168, 168));
                pDwgContext->setPaletteBackground(ODRGB(255, 255, 255));
            }
            drawBmpImage();
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
    open->FileTypeFilter->Append(".dwg");
    
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
