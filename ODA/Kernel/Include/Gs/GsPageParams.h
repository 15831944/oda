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

#ifndef _GSPAGEPARAMS_INCLUDED_
#define _GSPAGEPARAMS_INCLUDED_ / 

#include "TD_PackPush.h"

/** \details
  This class stores printing page size and margins.

  \remarks
  All values are in millimeters.

  \sa
  TD_Gs

  <group OdGs_Classes> 
*/
class OdGsPageParams
{
  double m_dPaperWidth;  // All are in mm.
  double m_dPaperHeight;
  
  double m_dLeftMargin;
  double m_dRightMargin;
  double m_dTopMargin;
  double m_dBottomMargin;
public:
  /** \details
    Default constructor for the OdGsPageParams class. Sets the following data members:
    <table>
    Data member           Value        Description
    m_dPaperWidth         210.         Paper width of the page.
    m_dPaperHeight        297.         Paper height of the page.
    m_dLeftMargin         0.           Left margin of the page.
    m_dRightMargin        0.           Right margin of the page.
    m_dTopMargin          0.           Top margin of the page.
    m_dBottomMargin       0.           Bottom margin of the page.
    </table>
  */
  OdGsPageParams()
    : m_dPaperWidth(210.)
    , m_dPaperHeight(297.)
    , m_dLeftMargin(0.)
    , m_dRightMargin(0.)
    , m_dTopMargin(0.)
    , m_dBottomMargin(0.)
  {
  }

  /** \details
    Constructor for the OdGsPageParams class. Sets the specified page width and height. Page margin parameters are set to 0.
    
    \param dPaperWidth    [in]  Page width.
    \param dPaperHeight   [in]  Page height.
  */
  OdGsPageParams(double dPaperWidth, double dPaperHeight)
    : m_dPaperWidth(dPaperWidth)
    , m_dPaperHeight(dPaperHeight)
    , m_dLeftMargin(0.)
    , m_dRightMargin(0.)
    , m_dTopMargin(0.)
    , m_dBottomMargin(0.)
  {
  }

  /** \details
    Destructor for the OdGsOverlayDataContainer class.
  */
  ~OdGsPageParams()
  {
  }

  /** \details
    Copy constructor for the OdGsPageParams class. Copies the passed object with page parameters.
    
    \param data    [in]  Page parameters object to copy.
  */
  OdGsPageParams(const OdGsPageParams& data)
  {
    *this = data;
  }

  /** \details
    Assignment operator for the OdGsPageParams class.
    
    \param from    [in]  Object from which to assign page parameters.
  */
  OdGsPageParams& operator=(const OdGsPageParams& from)
  {
   m_dPaperWidth   = from.m_dPaperWidth;  // All are in mm.
   m_dPaperHeight  = from.m_dPaperHeight;
   m_dLeftMargin   = from.m_dLeftMargin;
   m_dRightMargin  = from.m_dRightMargin;
   m_dTopMargin    = from.m_dTopMargin;
   m_dBottomMargin = from.m_dBottomMargin;
   return *this;
  }

  /** \details
    Retrieves page paper width in mm.
  */
  double getPaperWidth() const   { return m_dPaperWidth;   }
  
  /** \details
    Retrieves page paper height in mm.
  */
  double getPaperHeight() const  { return m_dPaperHeight;  }
  
  /** \details
    Retrieves page left margin in mm.
  */
  double getLeftMargin() const   { return m_dLeftMargin;   }
  
  /** \details
    Retrieves page right margin in mm.
  */
  double getRightMargin() const  { return m_dRightMargin;  }
  
  /** \details
    Retrieves page top margin in mm.
  */
  double getTopMargin() const    { return m_dTopMargin;    }
  
  /** \details
    Retrieves page bottom margin in mm.
  */
  double getBottomMargin() const { return m_dBottomMargin; }

  /** \details
    Sets page parameters.
    
    \param dPaperWidth    [in]  Page width.
    \param dPaperHeight   [in]  Page height.
    \param dLeftMargin    [in]  Left margin of the page.
    \param dRightMargin   [in]  Right margin of the page.
    \param dTopMargin     [in]  Top margin of the page.
    \param dBottomMargin  [in]  Bottom margin of the page.
  */
  void set(
    double dPaperWidth,
    double dPaperHeight,
    double dLeftMargin = 0.,
    double dRightMargin = 0.,
    double dTopMargin = 0.,
    double dBottomMargin = 0.
    )
  {
    const double dTol = OdGeContext::gZeroTol.equalPoint();
    if (OdNegative(dPaperWidth-dTol) || OdNegative(dPaperHeight-dTol) || OdNegative(dLeftMargin) || OdNegative(dRightMargin) || OdNegative(dTopMargin) || OdNegative(dBottomMargin))
    {
      ODA_FAIL_ONCE();
      return;
    }

    if (dLeftMargin + dRightMargin >= dPaperWidth - dTol)
    {
      ODA_FAIL_ONCE();
      return;
    }

    if (dTopMargin + dBottomMargin >= dPaperHeight - dTol)
    {
      ODA_FAIL_ONCE();
      return;
    }

    m_dPaperWidth = dPaperWidth;
    m_dPaperHeight = dPaperHeight;
    m_dLeftMargin = dLeftMargin;
    m_dRightMargin = dRightMargin;
    m_dTopMargin = dTopMargin;
    m_dBottomMargin = dBottomMargin;
  }

  /** \details
    Scales page parameters.
    
    \param dScale    [in]  Scale factor.
  */
  void scale(double dScale)
  {
    if (dScale > 1e-8)
    {
      m_dPaperWidth *= dScale;
      m_dPaperHeight *= dScale;
      m_dLeftMargin *= dScale;
      m_dRightMargin *= dScale;
      m_dTopMargin *= dScale;
      m_dBottomMargin *= dScale;
    }
  }
};

#include "TD_PackPop.h"

#endif //_GSPAGEPARAMS_INCLUDED_
