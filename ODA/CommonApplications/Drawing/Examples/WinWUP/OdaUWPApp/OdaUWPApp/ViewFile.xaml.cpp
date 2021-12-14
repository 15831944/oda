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
// ViewFile.xaml.cpp
// Implementation of the ViewFile class
//

#include "pch.h"
#include "ViewFile.xaml.h"

#include "WinRTAppService.h"

using namespace OdaUWPApp;

using namespace Platform;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media::Imaging;

ViewFile::ViewFile() : rootPage(MainPage::Current)
{
	InitializeComponent();
	gsDevice = NULL;
	odgsInitialize();

	processFile();
}

void ViewFile::drawBmpImage()
{
	// Resize
	gsDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(24)));
	// Set the screen size for the generated geometry.
	int backingHeight = 768, backingWidth = 1024; //1024, 768
	OdGsDCRect screenRect(OdGsDCPoint(0, backingHeight), OdGsDCPoint(backingWidth, 0));
	gsDevice->onSize(screenRect);

	// Zoom to extents
	OdAbstractViewPEPtr(gsDevice->viewAt(0))->zoomExtents(gsDevice->viewAt(0));
	gsDevice->update();

	OdGiRasterImagePtr ptrImage = gsDevice->properties()->getAt(OD_T("RasterImage"));

	OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
	if (!pRasSvcs.isNull())
	{
		auto str = Windows::Storage::ApplicationData::Current->LocalFolder->Path + L"//" + rootPage->Filename + L".bmp";
		OdString dstFileName(str->Data());
		pRasSvcs->saveRasterImage(OdGiRasterImagePtr(gsDevice->properties()->getAt(OD_T("RasterImage"))), dstFileName);

		// Set the image source to the selected bitmap
		auto fp = L"ms-appdata:///local/" + rootPage->Filename + L".bmp";
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
					bitmapImage->SetSource(fileStream);
					bmpExport->Source = bitmapImage;
				});
			}
		});
	}
}

void ViewFile::processFile()
{
	try
	{
		auto localFilePath = rootPage->SampleFile->Path;
		// Load database
		pDb = WinRTAppServices::getInstance()->readFile(
			localFilePath->Data(), true, false, Oda::kShareDenyNo);
		if (!pDb.isNull())
		{
			// Create the rendering context.
			OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();

			// Create the custom rendering device and set the output stream for the device.
			if (!gsDevice.isNull())
			{
				gsDevice.release();
			}

			if (gsDevice.isNull())
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
		//TODO
	}
	catch (...)
	{
		//TODO
	}
}
