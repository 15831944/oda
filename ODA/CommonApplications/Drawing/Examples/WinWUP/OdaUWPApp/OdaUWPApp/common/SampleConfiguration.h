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
// SampleConfiguration.h.
// Declaration of the MainPage class
//

#pragma once

#include <collection.h>

namespace OdaUWPApp
{
	public value struct Scenario
	{
		Platform::String^ Title;
		Platform::String^ ClassName;
	};

	partial ref class MainPage
	{
	public:

		static property Platform::Array<Scenario>^ scenarios
		{
			Platform::Array<Scenario>^ get()
			{
				return scenariosInner;
			}
		}
		property Platform::String^ Filename
		{
			Platform::String^ get()
			{
				return fileName;
			}
			void set(Platform::String^ value)
			{
				fileName = value;
			}
		}
		property Windows::Storage::StorageFile^ SampleFile
		{
			Windows::Storage::StorageFile^ get()
			{
				return sampleFile;
			}
			void set(Windows::Storage::StorageFile^ value)
			{
				sampleFile = value;
			}
		}
	internal:
		void Initialize();
	private:
		static Platform::Array<Scenario>^ scenariosInner;
		Windows::Storage::StorageFile^ sampleFile;
		Platform::String^ fileName;
	};
}
