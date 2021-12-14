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

#ifndef EX_H_COLORCONVERTERCALLBACK
#define EX_H_COLORCONVERTERCALLBACK

#include "TD_PackPush.h"

#include "RxObject.h"

/** <group ExRender_Classes>
  \details
  This is the interface class for objects of color converter types.

  \sa
  <exref target="https://www.opendesign.com/blog/2017/october/frequently-asked-how-can-i-modify-drawn-colors-my-application-during-display">Frequently Asked: How can I modify drawn colors from my application during display?</exref>
*/
class OdColorConverterCallback : public OdRxObject
{
  public:
    /** \details
      This method is intended for color conversion.

      \param originalColor [in] Original color.

      \returns
      Converted color.
    */
    virtual ODCOLORREF convert(ODCOLORREF originalColor) = 0;

    /** \details
      This method determines whether the convert() method should be called to
      convert the background color.

      \returns
      True if conversion for the background color is required; false otherwise.
    */
    virtual bool convertBackgroundColors() const { return true; }
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdColorConverterCallback object pointers.
*/
typedef OdSmartPtr<OdColorConverterCallback> OdColorConverterCallbackPtr;

#include "TD_PackPop.h"

#endif // EX_H_COLORCONVERTERCALLBACK
