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


// to satisfy VC2005, else it refuses to compile WinDirectX project
#if !defined(_WIN32_WCE) && (_MSC_VER >= 1400)
  #define POINTER_64 __ptr64
#endif

#include "OdaCommon.h"
#include "OdString.h" // else undefined in rxvariantvalue.h (?)
#include "ExGsWin32Device.h"

//Implementation of class ExGsWin32Device
// AE 11.02.2003 - Begin
//*******************************************************************************/
//  Constructor()
//*******************************************************************************/
ExGsWin32Device::ExGsWin32Device()
    : m_hWnd(NULL)
    , m_pOldPal(NULL)
    , m_pPal(NULL)
    , m_hDC(NULL)
    , m_nColorDepth(24)
    , m_bColorIndexMode(false)
    , m_nDPIOverride(0)
{
}

ODRX_DECLARE_PROPERTY(WindowHDC)
ODRX_DECLARE_PROPERTY(WindowHWND)
ODRX_DECLARE_PROPERTY(DPI)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExGsWin32Device);
  ODRX_INHERIT_PROPERTIES(OdCommonDeviceProps)
  ODRX_GENERATE_PROPERTY(WindowHDC)
  ODRX_GENERATE_PROPERTY(WindowHWND)
  ODRX_GENERATE_PROPERTY(DPI)
ODRX_END_DYNAMIC_PROPERTY_MAP(ExGsWin32Device);

ODRX_DEFINE_PROPERTY_METHODS(WindowHDC,               ExGsWin32Device,  getHDC,                   setHDC,  getIntPtr);
ODRX_DEFINE_PROPERTY_METHODS(WindowHWND,              ExGsWin32Device, getHWND,                   setHWND, getIntPtr);
ODRX_DEFINE_PROPERTY_METHODS(DPI,                     ExGsWin32Device,     DPI,                   setDPI,  getInt32);

//*******************************************************************************/
//  Destructor()
//*******************************************************************************/
ExGsWin32Device::~ExGsWin32Device()
{
  deleteContext();
}


//*******************************************************************************/
//  deleteContext()
//*******************************************************************************/
void ExGsWin32Device::deleteContext()
{
  if (m_pOldPal)
        {
                // Select in old palette.
    SelectPalette(m_hDC, m_pOldPal, 0);
                m_pOldPal = NULL ;
        }

        if (m_pPal)
        {
                DeleteObject(m_pPal);
                m_pPal = NULL ;
        }
}

//*******************************************************************************/
//  getDeviceColorDepth()
//*******************************************************************************/
int ExGsWin32Device::getDeviceColorDepth() const
{
  return ::GetDeviceCaps(m_hDC, BITSPIXEL);
}


//*******************************************************************************/
//  createPalette()
//*******************************************************************************/
void ExGsWin32Device::createPalette(ODGSPALETTE *logPalette)
{
  OdUInt32 i;
  if (!m_bColorIndexMode)
  {
    return;
  }

  int nLogPaletteSz = (1 << m_nColorDepth);
  int iPalSize = sizeof(LOGPALETTE) + nLogPaletteSz * sizeof(PALETTEENTRY);

  OdUInt8Array palBuf;
  palBuf.resize(iPalSize);
  LOGPALETTE* pPal = (LOGPALETTE*)palBuf.asArrayPtr();
  if(!pPal)
  {
    ODA_TRACE0("Out of memory for logpal");
    return;
  }
  pPal->palVersion = 0x300; // Windows 3.0
  pPal->palNumEntries = (WORD)nLogPaletteSz; // table size

  if (nLogPaletteSz > 16)
  {
    for(i = 0; i < odmin(logPalette->size(), (OdUInt32)nLogPaletteSz); ++i)
    { const ODCOLORREF palColor = logPalette->getAt(i);
      pPal->palPalEntry[i].peRed    = ODGETRED(palColor);
      pPal->palPalEntry[i].peGreen  = ODGETGREEN(palColor);
      pPal->palPalEntry[i].peBlue   = ODGETBLUE(palColor);
      pPal->palPalEntry[i].peFlags  = ODGETALPHA(palColor);
    }
  }
  else if (nLogPaletteSz > 2)
  { pPal->palPalEntry[0].peRed = 0,    pPal->palPalEntry[0].peGreen = 0,    pPal->palPalEntry[0].peBlue = 0,    pPal->palPalEntry[0].peFlags = 0;
    pPal->palPalEntry[1].peRed = 128,  pPal->palPalEntry[1].peGreen = 128,  pPal->palPalEntry[1].peBlue = 128,  pPal->palPalEntry[1].peFlags = 0;
    pPal->palPalEntry[2].peRed = 128,  pPal->palPalEntry[2].peGreen = 0,    pPal->palPalEntry[2].peBlue = 0,    pPal->palPalEntry[2].peFlags = 0;
    pPal->palPalEntry[3].peRed = 255,  pPal->palPalEntry[3].peGreen = 0,    pPal->palPalEntry[3].peBlue = 0,    pPal->palPalEntry[3].peFlags = 0;
    pPal->palPalEntry[4].peRed = 0,    pPal->palPalEntry[4].peGreen = 128,  pPal->palPalEntry[4].peBlue = 0,    pPal->palPalEntry[4].peFlags = 0;
    pPal->palPalEntry[5].peRed = 0,    pPal->palPalEntry[5].peGreen = 255,  pPal->palPalEntry[5].peBlue = 0,    pPal->palPalEntry[5].peFlags = 0;
    pPal->palPalEntry[6].peRed = 128,  pPal->palPalEntry[6].peGreen = 128,  pPal->palPalEntry[6].peBlue = 0,    pPal->palPalEntry[6].peFlags = 0;
    pPal->palPalEntry[7].peRed = 255,  pPal->palPalEntry[7].peGreen = 255,  pPal->palPalEntry[7].peBlue = 0,    pPal->palPalEntry[7].peFlags = 0;
    pPal->palPalEntry[8].peRed = 0,    pPal->palPalEntry[8].peGreen = 0,    pPal->palPalEntry[8].peBlue = 128,  pPal->palPalEntry[8].peFlags = 0;
    pPal->palPalEntry[9].peRed = 0,    pPal->palPalEntry[9].peGreen = 0,    pPal->palPalEntry[9].peBlue = 255,  pPal->palPalEntry[9].peFlags = 0;
    pPal->palPalEntry[10].peRed = 0,   pPal->palPalEntry[10].peGreen = 128, pPal->palPalEntry[10].peBlue = 128, pPal->palPalEntry[10].peFlags = 0;
    pPal->palPalEntry[11].peRed = 0,   pPal->palPalEntry[11].peGreen = 255, pPal->palPalEntry[11].peBlue = 255, pPal->palPalEntry[11].peFlags = 0;
    pPal->palPalEntry[12].peRed = 128, pPal->palPalEntry[12].peGreen = 0,   pPal->palPalEntry[12].peBlue = 128, pPal->palPalEntry[12].peFlags = 0;
    pPal->palPalEntry[13].peRed = 255, pPal->palPalEntry[13].peGreen = 0,   pPal->palPalEntry[13].peBlue = 255, pPal->palPalEntry[13].peFlags = 0;
    pPal->palPalEntry[14].peRed = 192, pPal->palPalEntry[14].peGreen = 192, pPal->palPalEntry[14].peBlue = 192, pPal->palPalEntry[14].peFlags = 0;
    pPal->palPalEntry[15].peRed = 255, pPal->palPalEntry[15].peGreen = 255, pPal->palPalEntry[15].peBlue = 255, pPal->palPalEntry[15].peFlags = 0;
  } else
  { pPal->palPalEntry[0].peRed = pPal->palPalEntry[0].peGreen = pPal->palPalEntry[0].peBlue =
    pPal->palPalEntry[0].peFlags = pPal->palPalEntry[1].peFlags = 0;
    pPal->palPalEntry[1].peRed = pPal->palPalEntry[1].peGreen = pPal->palPalEntry[1].peBlue = 255;
  }
  if (m_pPal)
  {
    DeleteObject(m_pPal);
  }
  m_pPal = ::CreatePalette(pPal);
}
// AE - End

void ExGsWin32Device::setDPI(long dpi)
{
  // #9633 (1)
  m_nDPIOverride = dpi;
}
