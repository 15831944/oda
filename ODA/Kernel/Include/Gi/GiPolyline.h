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




#ifndef __ODGIPOLYLINE_H__
#define __ODGIPOLYLINE_H__

class OdGeVector3d;
class OdGeLineSeg2d;
class OdGeLineSeg3d;
class OdGeCircArc2d;
class OdGeCircArc3d;

#include "TD_PackPush.h"

#include "Gi/GiExport.h"

/** \details
    This class implements the OdGi equivalent of an OdDbPolyline or "lightweight" polyline.
    
    \remarks
    Instances of this class are used to pass lightweight polyline data through the 
    ODA Platform vectorization framework.

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiPolyline : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiPolyline);

  /** 
    Represents segment types.
  */
  enum SegType
  {
    kLine,        // Straight segment with length greater than 0.
    kArc,         // Arc segment with length greater than 0.
    kCoincident,  // Segment with 0 length.
    kPoint,       // Polyline with a single vertex.
    kEmpty        // No vertices are present.
  };

  /** \details
      Retrieves the number of vertices in the entity.
      returns
      quantity of vertices in the entity.
  */
  virtual unsigned int numVerts() const = 0;

  /** \details
      Checks whether this polyline is closed.
      \returns
      true if polyline is closed, false otherwise
  */
  virtual bool isClosed() const = 0;

  /** \details
      Retrieves the normal vector for this entity.
      \returns
      normal to this entity.
  */
  virtual OdGeVector3d normal() const = 0;

  /** \details
      Retrieves the thickness for this entity.
      \returns
      thickness as a value of the double type.
  */
  virtual double thickness() const = 0;

  /** \details
      Retrieves the constant width for this entity.
      \returns
      constant width for this entity as a value of the double type.
  */
  virtual double getConstantWidth() const = 0;

  /** \details
      Checks whether this entity has width data.
      \returns
      true if the vertices in this entity contain start and end width data, false otherwise.
  */
  virtual bool hasWidth() const = 0;

  /** \details
      Retrieves the PLINEGEN flag for this entity. If PLINEGEN is true,
      linetype generation will be patterned across the entire polyline, rather than 
      being done for each segment individually.
      \returns
      true if this entity has the PLINEGEN flag set to true, false otherwise.
  */
  virtual bool hasPlinegen() const = 0;

  /** \details
      Retrieves the elevation for this entity.
      \returns
      an elevation as a value of the double type.
  */
  virtual double elevation() const = 0;

  /** \details
      Retrieves the segment type at the specified index.
      
      \param index [in]   Segment index.
      
      \returns
      segment type as a value from the SegType enumeration.
  */
  virtual SegType segType(unsigned int index) const = 0;
  
  /** \details
    Retrieves the line segment starting at the specified index.
    \param index [in]   Segment index.
    \param ln [out]   2D line segment to get.
  */
  virtual void getLineSegAt(unsigned int index, OdGeLineSeg2d& ln) const = 0;

  /** \details
    Retrieves the line segment starting at the specified index.
    \param index [in]   Segment index.
    \param ln [out]   3D line segment to get.
  */
  virtual void getLineSegAt(unsigned int index, OdGeLineSeg3d& ln) const = 0;

  /** \details
      Retrieves the arc segment starting at the specified index.
      \param index [in]   Segment index.
      \param arc [out]   2D arc segment to get.
  */
  virtual void getArcSegAt(unsigned int index, OdGeCircArc2d& arc) const = 0;

  /** \details
      Retrieves the arc segment starting at the specified index.
      \param index [in]   Segment index.
      \param arc [out]   3D arc segment to get.
  */
  virtual void getArcSegAt(unsigned int index, OdGeCircArc3d& arc) const = 0;

  /** \details
      Retrieves a specified point from this entity.
      \param index [in]   Segment index.
      \param arc [out]   2D point to get.
  */
  virtual void getPointAt(unsigned int index, OdGePoint2d& pt) const = 0;

  /** \details
      Retrieves the bulge value for a specified point.
      \param index [in]   Segment index.
      \returns
      bulge as a value of the double type.
  */
  virtual double getBulgeAt(unsigned int index) const = 0;

  /** \details
      Retrieves the start and end widths for a specified point.
      \param index [in]   Segment index.
      \param startWidth [out]   Start width.
      \param endWidth [out]   End width.
  */
  virtual void getWidthsAt(unsigned int index, double& startWidth,  double& endWidth) const = 0;

  /** \details
      Retrieves the original OdDbPolyline object from which this OdGiPolyline was 
      created.
      \returns
      smart pointer to the underlaying OdDbPolyline object.
  */
  virtual OdRxObjectPtr getDbPolyline() const = 0;
};

/** \details
  A data type that represents a smart pointer to an <link OdGiPolyline, OdGiPolyline> object.
*/
typedef OdSmartPtr<OdGiPolyline> OdGiPolylinePtr;

#include "TD_PackPop.h"

#endif  // __ODGIPOLYLINE_H__

