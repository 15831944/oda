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

#ifndef _BASEDEVICEDRIVER_INCLUDED_
#define _BASEDEVICEDRIVER_INCLUDED_

#include "TD_PackPush.h"
#include "PdfProperties.h"
#include "PdfImport.h"

#include <memory>
#include <vector>

#include "oda_files/PdfImportBaseObjectGrabber.h"

class OdDbHostAppProgressMeter;

class PdfImportInfoHolder
{
public:
  explicit PdfImportInfoHolder(OdPdfImport::ImportResult& result);

  ~PdfImportInfoHolder();

  float                               m_Width{0.};
  float                               m_Height{0.};
  PdfPropertiesPtr                    m_pProperties;
  size_t                              m_ObjectCounter{0};
  size_t                              m_ErrorCounter{0};
  OdPdfImport::ImportResult&          m_ImageError;

  void InitProgressMeter();
  void StartProgressMeter(OdString& displayString);
  void StopProgressMeter();
  void ProgressMeterSetLimit();
  void MeterProgress();

private:
  OdDbHostAppProgressMeter*           m_pProgressMeter{ NULL };
};

using PdfImportInfoHolderPtr = std::shared_ptr<PdfImportInfoHolder>;


class OdDbBasePdfImportObjectGrabber : public OdPdfImportBaseObjectGrabber
{
public:
  OdDbBasePdfImportObjectGrabber(PdfImportInfoHolderPtr info);
  virtual ~OdDbBasePdfImportObjectGrabber();

  virtual bool needToAddGrathics(bool* ret);

  virtual bool needToAddImage(bool* ret);

  virtual bool needToAddText(bool* ret);

  virtual bool needToAddShading(bool* ret);

  virtual float getWidth();
  virtual float getHeight();

protected:
  PdfImportInfoHolderPtr              m_Info;

};

#include "TD_PackPop.h"

#endif // _BASEDEVICEDRIVER_INCLUDED_
