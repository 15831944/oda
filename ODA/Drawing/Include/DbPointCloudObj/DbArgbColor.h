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

#ifndef _OD_DB_ARGBCOLOR_
#define _OD_DB_ARGBCOLOR_

#include "TD_PackPush.h"
#include "DbPointCloudObjExports.h"
#include "DbObject.h"


/** \details
  This class represents the HSL color for point cloud entities.

  Corresponding C++ library: AcDbPointCloudObj

  <group OdDbPointCloud_Classes>
*/
struct DBPOINTCLOUDOBJ_EXPORT ColorModelHSL 
{
  /** \details
    Default constructor. Creates an HSL color with zeroed fields.
  */
  ColorModelHSL();

  /** \details
    Constructor. Creates an HSL color with passed in parameters.

    \param hue [in] Hue value from 1 to 360.
    \param saturation [in] Saturation value from 0 to 1.
    \param luminance [in] Luminance value from 0 to 1.
    \param alpha [in] Alpha value from 0 to 1.
  */
  ColorModelHSL(double hue, double saturation, double luminance, double alpha);

  /** \details
    Equality operator. Checks if lhs color is equal to rhs color. 
    Compares colors by hue, saturation, luminance, alpha value.

    \param color [in] rhs color to be compared.
    \returns true if the colors are identical, false - otherwise.
  */
  bool operator == (const ColorModelHSL& color) const;

  double hue; // 0-360
  double saturation; // 0-1
  double luminance; // 0-1
  double alpha; // 0-1
};

/** \details
  This class represents the color for point cloud entities.

  Corresponding C++ library: AcDbPointCloudObj

  <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT ArgbColor 
{
public:
  /** \details
    Default constructor.
  */
  ArgbColor();
  
  /** \details
    Constructor. Creates an ArgbColor object from an HSL color.

    \param color [in] Color to be set to a new object.
  */
  ArgbColor(ColorModelHSL color);

  /** \details
    Constructor. Creates an ArgbColor object from color reference passed in.

    \param color [in] Color reference.
  */
  ArgbColor(ODCOLORREF color);

  /** \details
    Sets a color to this ArgbColor object.

    \param color [in] New HSL color to be set.
  */
  void setColor(const ColorModelHSL & color);

  /** \details
    Returns the current HSL color set to this ArgbColor object.
  */
  ColorModelHSL color() const;

  /** \details
    Casts color from COLORREF to ArgbColor using a special constructor.

    \param color [in] Input color as ODCOLORREF.
    \returns Same color as ArgbColor.
  */
  static ArgbColor COLORREFToArgb(ODCOLORREF color);

  /** \details
    Creates a gradient from current color to endColor at a given rate.

    \param endColor [in] Color to which a gradient will fade.
    \param ratio [in] Parameter which defines how quickly a gradient fades.
  */
  ArgbColor gradientRgb(const ArgbColor & endColor, const double ratio) const;

  /** \details
    Creates a gradient from current color to endColor at a given rate.

    \param endColor [in] HSL color to which a gradient will fade.
    \param ratio [in] Parameter which defines how quickly a gradient fades.
  */
  ArgbColor gradientRgb(const ColorModelHSL & endColor, const double ratio) const;

  /** \details
    Converts ODCOLORREF to HSL color.

    \param color [in] Color as ODCOLORREF.
    \remarks
      The conversion is complete by retrieving RGB values from ODCOLORREF 
      and then they are transformed into HSL using special formulas.
  */
  void fromCOLORREF(ODCOLORREF);

  /** \details
    Converts HSL color to ODCOLORREF.

    \remarks
      The conversion is complete by using special formulas that
      convert HSL to RGB, and ODCOLORREF is assembled from RGB.
  */
  ODCOLORREF toCOLORREF() const;

private:
  ColorModelHSL m_color;

};

#include "TD_PackPop.h"

#endif
