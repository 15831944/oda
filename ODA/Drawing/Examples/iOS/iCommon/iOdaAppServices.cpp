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

#include <OdaCommon.h>
#include <OdMutex.h>
#include "iOdaAppServices.h"
#include <StaticRxObject.h>

static OdMutex iOdaAppServices_classMutex;

iOdaAppServices* instance = NULL;

iOdaAppServices* iOdaAppServices::getInstance() {
	
	if (instance == NULL) {
		iOdaAppServices_classMutex.lock();
		if (instance == NULL) {
			instance = new iOdaAppServices();
#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED
			printf("iPhone OS version min required: %d\n", __IPHONE_OS_VERSION_MIN_REQUIRED);
#endif
			
#ifdef __APPLE__
			printf("__APPLE__\n");
#endif
			
#ifdef __GNUC__
			printf("__GNUC__\n");
#endif
		}
		iOdaAppServices_classMutex.unlock();
	}
	return instance;
	 
}


iOdaAppServices::~iOdaAppServices() {
}

iOdaAppServices::iOdaAppServices() : mCnt (0) {
}

void iOdaAppServices::addRef() {
	mutex.lock();
	mCnt++;
	mutex.unlock();
}

void iOdaAppServices::release() {
	mutex.lock();
	mCnt--;
	mutex.unlock();
	if (mCnt==0) delete this;
}

/*
OdStreamBufPtr iOdaAppServices::createFile(
								  const OdString& filename,                    
								  Oda::FileAccessMode accessMode,
								  Oda::FileShareMode  shareMode,                          
								  Oda::FileCreationDisposition creationDisposition)
{
	OdStreamBufPtr p;
	return p;
}

bool iOdaAppServices::accessFile(const OdString& filename, int accessMode) 
{
	return true;
}

OdInt64 iOdaAppServices::getFileCTime(const OdString& filename) 
{
	return 0;
}

OdInt64 iOdaAppServices::getFileMTime(const OdString& filename) 
{
	return 0;
}

OdInt64 iOdaAppServices::getFileSize(const OdString& filename)
{
	return 0;
}

OdString iOdaAppServices::formatMessage(unsigned int formatId, va_list* argList)
{
	OdString s = OD_T("");
	return s;
}

OdCodePageId iOdaAppServices::systemCodePage() const
{
	return CP_MACINTOSH;
}
*/

