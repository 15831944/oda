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
// SuspensionManager.h
// Declaration of the SuspensionManager class
//

#pragma once

namespace OdaUWPApp
{
	/// <summary>
	/// SuspensionManager captures global session state to simplify process lifetime management
	/// for an application.  Note that session state will be automatically cleared under a variety
	/// of conditions and should only be used to store information that would be convenient to
	/// carry across sessions, but that should be disacarded when an application crashes or is
	/// upgraded.
	/// </summary>
	class SuspensionManager sealed
	{
	public:
		static void RegisterFrame(Windows::UI::Xaml::Controls::Frame^ frame, Platform::String^ sessionStateKey, Platform::String^ sessionBaseKey = nullptr);
		static void UnregisterFrame(Windows::UI::Xaml::Controls::Frame^ frame);
		static concurrency::task<void> SaveAsync();
		static concurrency::task<void> RestoreAsync(Platform::String^ sessionBaseKey = nullptr);
		static Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ SessionState();
		static Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ SessionStateForFrame(
			Windows::UI::Xaml::Controls::Frame^ frame);

	private:
		static void RestoreFrameNavigationState(Windows::UI::Xaml::Controls::Frame^ frame);
		static void SaveFrameNavigationState(Windows::UI::Xaml::Controls::Frame^ frame);

		static Platform::Collections::Map<Platform::String^, Platform::Object^>^ _sessionState;
		static const wchar_t* sessionStateFilename;

		static std::vector<Platform::WeakReference> _registeredFrames;
		static Windows::UI::Xaml::DependencyProperty^ FrameSessionStateKeyProperty;
		static Windows::UI::Xaml::DependencyProperty^ FrameSessionBaseKeyProperty;
		static Windows::UI::Xaml::DependencyProperty^ FrameSessionStateProperty;
	};
}
