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


#ifndef __OD_GI_PLOTGENERATOR__
#define __OD_GI_PLOTGENERATOR__

#include "Gi/GiConveyorNode.h"

#include "TD_PackPush.h"
#include "Ps/PlotStyles.h"
#include "Gs/GsExport.h"

/** \details
    This class is the base class for custom classes that implements software plot generation for OdGiConveyorGeometry objects.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiPlotGenerator : public OdGiConveyorNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGiPlotGenerator);

  /** \details
    Sets max deviation for curve tesselation.
    \param deviations [in]  Array with deviation values.
  */
  virtual void setDeviation(const OdGeDoubleArray& deviations) = 0;

  /** \details
    Sets deviation object to obtain max deviation for curve tesselation.
    \param pDeviation [in]  Pointer to the deviation object.
  */
  virtual void setDeviation(const OdGiDeviation* pDeviation) = 0;

  /** \details
    Sets the draw context associated with this object.
    \param pDrawContext [in]  Pointer to the draw context.
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawContext) = 0;

  /** \details
    Enable/Disable plot generation.
    \param bEnable [in]  Flag to set.
  */
  virtual void enable(bool bEnable) = 0;

  /** \details
    Checks whether plot generation is enabled.
    \returns 
    true if software plot generation is enabled, false otherwise.
  */
  virtual bool enabled() const = 0;

  /** \details
    Sets software plot generation output DPI.
    \param dpi [in]  Value to set.
  */
  virtual void dot_per_inch(double dpi) = 0;
  /** \details
    Retrieves current software plot generation output DPI.
    \returns
    output DPI as a value of double type.
  */
  virtual double dot_per_inch() const = 0;

  /** \details
    Sets software plot generation output DPI and millimeters.
    \param dpmm [in]  DPI in millimeters to set.
  */
  void dot_per_mm(double dpmm);
  /** \details
    Retrieves current software plot generation output DPI as millimeters.
    \returns
    plot generation output DPI as millimeters.
  */
  double dot_per_mm() const;

  /** \details
    Sets current plot style.
    \param psd [in]  Plot style to set.
  */
  virtual void setPlotStyle(const OdPsPlotStyleData& psd) = 0;

  /** \details
    Abstract interface to be used as output for polylineOut calls with extended style.
  */
  class ODGI_EXPORT PolylineOut
  {
    public:
      /** \details
        Abstract method to be called by plot generator for applications which supports extended styles.
        \param pPoints [in]  Points in display coordinate system.
        \param nPoints [in]  Count of points in array.
        \param capStyle [in]  Style for start/end caps.
        \param joinStyle [in]  Joins style.
        \param fLwd [in]  Lineweight in pixels.
        \returns
        Must return true if call is correctly processed. Implementation could return false if such
        type of data cannot be processed. False instructs caller to process data oneself.
      */
      virtual bool plotGeneratorPolylineOut(const OdGePoint3d *pPoints, OdUInt32 nPoints,
        OdPs::LineEndStyle capStyle, OdPs::LineJoinStyle joinStyle, double fLwd) = 0;
  };
  /** \details
    Sets external interface for extended line style emulation.
    \param pPolylineOut [in]  Pointer to implementation of PolylineOut interface.
  */
  virtual void setExternalPolylineOut(PolylineOut *pPolylineOut) = 0;
  /** \details
    Returns current interface for extended line style emulation.
    \returns
    By default returns Null (unless PolylineOut interface is set). This is means that
    software plot generator handles extended line style internally.
  */
  virtual PolylineOut *externalPolylineOut() const = 0;

  class ODGI_EXPORT EllipseOut
  {
    public:
      virtual bool plotGeneratorEllipseOut(const OdGeEllipArc3d& arc, double width) = 0;
  };
  virtual void setExternalEllipseOut(EllipseOut *pEllipseOut) = 0;
  virtual EllipseOut *externalEllipseOut() const = 0;
};

inline void
OdGiPlotGenerator::dot_per_mm(double dpmm)
{
  dot_per_inch(dpmm * 25.4);
}

inline double
OdGiPlotGenerator::dot_per_mm() const
{
  return dot_per_inch() / 25.4;
}

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiPlotGenerator object pointers.
*/
typedef OdSmartPtr<OdGiPlotGenerator> OdGiPlotGeneratorPtr;

#include "TD_PackPop.h"

#endif //#ifndef __OD_GI_PLOTGENERATOR__
