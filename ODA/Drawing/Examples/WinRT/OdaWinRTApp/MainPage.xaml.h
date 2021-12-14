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
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "OdaCommon.h"
#include "diagnostics.h"
#include "DbDatabase.h"
#include "DbEntity.h"
#include "DbDimAssoc.h"

#include "OdCharMapper.h"
#include "RxObjectImpl.h"

#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "OdFileBuf.h"
#include "RxDynamicModule.h"
#include "FdField.h"

#include <Gs/Gs.h>
#include <DbDatabase.h>
#include <GiContextForDbDatabase.h>
#include <DbGsManager.h>
#include <Gs/GsBaseVectorizer.h>
#include <RxVariantValue.h>
#include <RxDictionary.h>
#include <ColorMapping.h>
#include <AbstractViewPE.h>
#include "Gi/GiRasterImage.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseInclude.h"
#include "ColorMapping.h"
#include "RxRasterServices.h"
#include "toString.h"

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Popups;

namespace OdaWinRTApp
{
    enum OdExample{none = 0, OdReadEx, OdViewEx};

    public ref class MainPageSizeChangedEventArgs sealed
    {
    public:
        property Windows::UI::ViewManagement::ApplicationViewState ViewState
        {
            Windows::UI::ViewManagement::ApplicationViewState get()
            {
                return viewState;
            }

            void set(Windows::UI::ViewManagement::ApplicationViewState value)
            {
                viewState = value;
            }
        }

    private:
        Windows::UI::ViewManagement::ApplicationViewState viewState;
    };

    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public ref class MainPage sealed
    {
    public:
        MainPage();
        void addDumpText(Platform::String^ text);

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:
        /* For orientation purposes*/
        void OnOrientationChanged(Platform::Object^ sender);
        Windows::Foundation::EventRegistrationToken orientationChangedEventToken;

        /* Initialize device*/
        void Unitialize();
        /* Clear device*/
        void clearDevice();

        void OnReadEx(Platform::Object^ sender, 
              RoutedEventArgs^ e);
        void OnViewEx(Platform::Object^ sender, 
              RoutedEventArgs^ e);
        void OnSelectFile(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        void drawBmpImage();
        OdGsDevicePtr gsDevice;
        OdDbDatabasePtr pDb;
        Platform::String^ mSelectedFileName;
        OdExample whatEx;
    };
}
