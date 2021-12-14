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
// App.xaml.cpp
// Implementation of the App class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace OdaDgnWinRTApp;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Application template is documented at http://go.microsoft.com/fwlink/?LinkId=234227

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
  InitializeComponent();
  Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used when the application is launched to open a specific file, to display
/// search results, and so forth.
/// </summary>
/// <param name="args">Details about the launch request and process.</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args)
{
  auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

  // Do not repeat app initialization when the Window already has content,
  // just ensure that the window is active
  if (rootFrame == nullptr)
  {
  // Create a Frame to act as the navigation context and associate it with
  // a SuspensionManager key
  rootFrame = ref new Frame();

  if (args->PreviousExecutionState == ApplicationExecutionState::Terminated)
  {
    // TODO: Restore the saved session state only when appropriate, scheduling the
    // final launch steps after the restore is complete

  }

  if (rootFrame->Content == nullptr)
  {
    // When the navigation stack isn't restored navigate to the first page,
    // configuring the new page by passing required information as a navigation
    // parameter
    if (!rootFrame->Navigate(TypeName(MainPage::typeid), args->Arguments))
    {
      throw ref new FailureException("Failed to create initial page");
    }
  }
  // Place the frame in the current Window
  Window::Current->Content = rootFrame;
  // Ensure the current window is active
  Window::Current->Activate();
  }
  else
  {
    if (rootFrame->Content == nullptr)
    {
      // When the navigation stack isn't restored navigate to the first page,
      // configuring the new page by passing required information as a navigation
      // parameter
      if (!rootFrame->Navigate(TypeName(MainPage::typeid), args->Arguments))
      {
        throw ref new FailureException("Failed to create initial page");
      }
    }
    // Ensure the current window is active
    Window::Current->Activate();
  }
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
  (void) sender;  // Unused parameter
  (void) e;  // Unused parameter

  //TODO: Save application state and stop any background activity
}
