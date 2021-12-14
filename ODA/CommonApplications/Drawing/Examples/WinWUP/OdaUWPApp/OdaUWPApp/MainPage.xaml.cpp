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

#include "WinRTAppService.h"

using namespace OdaUWPApp;

using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Platform::Collections;
using namespace concurrency;
using namespace std;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Interop;

#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExFieldEvaluatorModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);

#ifdef TA_MODULES_ENABLED
#include "AECAppStaticModules.h"
ODRX_DECLARE_ARCH_STATIC_MODULES_ENTRY_POINTS()
#endif

#ifdef TC_MODULES_ENABLED
#include "AECCAppStaticModules.h"
ODRX_DECLARE_CIVIL_STATIC_MODULES_ENTRY_POINTS()
ODRX_DECLARE_CIVIL_UI_STATIC_MODULES_ENTRY_POINTS()
#endif

ODRX_BEGIN_STATIC_MODULE_MAP()
ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName, ModelerModule)
ODRX_DEFINE_STATIC_APPLICATION(RX_RASTER_SERVICES_APPNAME, ExRasterModule)
ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName, OdRasterProcessingServicesImpl)
ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
ODRX_DEFINE_STATIC_APPLICATION(OdExFieldEvaluatorModuleName, ExFieldEvaluatorModule)
ODRX_DEFINE_STATIC_APPLICATION(OdWinBitmapModuleName, BitmapModule)
#ifdef TA_MODULES_ENABLED
ODRX_DEFINE_ARCH_STATIC_APPMODULES()
#endif
#ifdef TC_MODULES_ENABLED
ODRX_DEFINE_CIVIL_STATIC_APPMODULES()
ODRX_DEFINE_CIVIL_UI_STATIC_APPMODULES()
#endif

ODRX_END_STATIC_MODULE_MAP()
#endif

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409
MainPage^ MainPage::Current = nullptr;

static void _MyAssert(const char *expression, const char *fileName, int nLineNo)
{}


MainPage::MainPage()
{
	InitializeComponent();
#if !defined(_TOOLKIT_IN_DLL_)
	ODRX_INIT_STATIC_MODULE_MAP();
#endif
	odSetAssertFunc(_MyAssert);
	try
	{
		odInitialize(WinRTAppServices::getInstance());
	}
	catch (...)
	{
		return;
	}

	// This is a static public property that allows downstream pages to get a handle to the MainPage instance
	// in order to call methods that are in this class.
	MainPage::Current = this;
}

void MainPage::Unitialize()
{
	try
	{
		odUninitialize();
	}
	catch (...)
	{
		//TODO
	}
}
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void)e;	// Unused parameter
	StorageFolder^ downloadFolder = KnownFolders::PicturesLibrary;
	auto outputString = make_shared<wstring>();

	create_task(downloadFolder->GetItemsAsync())
		.then([this, outputString](IVectorView<IStorageItem^>^ items)
	{
		auto itemCollection = ref new Platform::Collections::Vector<Object^>();
		for (unsigned int i = 0; i < items->Size; i++)
		{
			if (!items->GetAt(i)->IsOfType(StorageItemTypes::Folder))
			{
				StorageFile^ file = safe_cast<StorageFile^>(items->GetAt(i));
				if (file->FileType == L".dwg" ||
					file->FileType == L".dxf")
				{
					TextBlock^ textBlock = ref new TextBlock();
					ListBoxItem^ item = ref new ListBoxItem();

					textBlock->Text = ref new String(items->GetAt(i)->Name->Data());

					item->Name = ref new String(items->GetAt(i)->Name->Data());
					item->Content = textBlock;
					itemCollection->Append(item);
				}
			}
		}
		// Set the newly created itemCollection as the ListBox ItemSource.
		FilesControl->ItemsSource = itemCollection;
	});

	SuspensionManager::RegisterFrame(ScenarioFrame, "scenarioFrame");
	int startingScenarioIndex;

	//if (Window::Current->Bounds.Width < 640)
	//{
		startingScenarioIndex = -1;
	//}
	//else
	//{
		//startingScenarioIndex = 0;
	//}

	FilesControl->SelectedIndex = startingScenarioIndex;
	FilesControl->ScrollIntoView(FilesControl->SelectedItem);
}


void MainPage::File_SelectionChanged(Object^ sender, SelectionChangedEventArgs^ e)
{

	ListBox^ filesListBox = safe_cast<ListBox^>(sender);
	ListBoxItem^ item = dynamic_cast<ListBoxItem^>(filesListBox->SelectedItem);

	if (item != nullptr)
	{
		// Get the returned file and copy it
		this->Filename = item->Name;
		String^ copyFileName = Filename;
		// Get the app's installation folder
		StorageFolder^ appFolder = KnownFolders::PicturesLibrary;
		StorageFolder^ targetFolder = ApplicationData::Current->LocalFolder;
		create_task(appFolder->GetFileAsync(Filename)).then([targetFolder, copyFileName](StorageFile^ sourceFile) {
			auto copyFileTask = sourceFile->CopyAsync(targetFolder, copyFileName, Windows::Storage::NameCollisionOption::ReplaceExisting);
			return create_task(copyFileTask);
		}).then([this](StorageFile^ file)
		{
			sampleFile = file;
			if (file != nullptr)
			{
				try
				{
					// Navigate to the selected scenario.
					TypeName scenarioType = { "OdaUWPApp.ViewFile", TypeKind::Custom };
					ScenarioFrame->Navigate(scenarioType, this);

					// Clear the selection before we navigate away
					//filesListBox->SelectedItem = nullptr;
					if (Window::Current->Bounds.Width < 640)
					{
						Splitter->IsPaneOpen = false;
					}
				}
				catch (COMException^ ex)
				{
					//TODO
				}
			}
		});
	}
}

void MainPage::Button_Click(Object^ sender, RoutedEventArgs^ e)
{
	Splitter->IsPaneOpen = !Splitter->IsPaneOpen;
}


