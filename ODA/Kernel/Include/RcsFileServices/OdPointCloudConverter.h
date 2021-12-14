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

#if !defined(OD_POINTCLOUDCONVERTER_H__1F251CD6_DB45_4534_9332_AE8CE3238D64__INCLUDED)
#define OD_POINTCLOUDCONVERTER_H__1F251CD6_DB45_4534_9332_AE8CE3238D64__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include "RootExport.h"
#include "SharedPtr.h"

/** <group OdRx_Classes>
  \details
  This is the interface class for working with source points.
*/
class OdSourcePoint
{
public:

  /** \details
    Constructor for the OdSourcePoint class.

    \remarks
    Constructs a source point of fully opaque white color and full intensity.
  */
  OdSourcePoint()
    : m_color(ODRGBA(255, 255, 255, 255)), m_intensity(255.0)
  {
  }

  /** \details
    Destructor for the OdSourcePoint class.
  */
  ~OdSourcePoint(){}

  /** Source point coordinates. */
  OdGePoint3d m_coord;
  /** Source point color. */
  ODCOLORREF  m_color;
  /** Source point intensity. */
  float       m_intensity;
};

/** \details
  A data type that represents a shared pointer to an
  <link OdSourcePoint, OdSourcePoint> object.
*/
typedef OdSharedPtr<OdSourcePoint> OdSourcePointPtr;

/** <group OdRx_Classes>
  \details
  This is the interface class for working with point cloud converter parameters.
*/
class OdPointCloudConverterParams
{
public:

  /** \details
    Constructor for the OdPointCloudConverterParams class.

    \remarks
    Constructs point cloud convertor parameters with 0.0 value for lower
    intensity threshold, 100.0 value for upper intensity threshold.
    The m_isTerrestrial flag specifies that input data is terrestrial if set to
    true, otherwise data is lidar.
  */
  OdPointCloudConverterParams()
    : m_intensityBottom(0.0), m_intensityUpper(100.0), m_isTerrestrial(true)
  {
  }

  /** \details
    Destructor for the OdPointCloudConverterParams class.
  */
  ~OdPointCloudConverterParams() {}

  /** Lower intensity threshold. */
  float m_intensityBottom;
  /** Upper intensity threshold. */
  float m_intensityUpper;
  /** A flag, indicating whether input data is terrestrial (true) or lidar (false). */
  bool  m_isTerrestrial;
};

/** \details
  A data type that represents a shared pointer to an
  <link OdPointCloudConverterParams, OdPointCloudConverterParams> object.
*/
typedef OdSharedPtr<OdPointCloudConverterParams> OdPointCloudConverterParamsPtr;

/** <group OdRx_Classes>
  \details
  This is an interface class to iterate through the source points.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdSourcePointIterator
{
public:

  /** \details
    Destructor for the OdSourcePointIterator class.
  */
  virtual ~OdSourcePointIterator(){}

  /** \details
    Moves the current iterator position to the beginning of the point data
    within the data source.
  */
  virtual void start() = 0;

  /** \details
    Indicates whether the traversal by this iterator object is complete.

    \returns
    True if the traversal by this iterator object is complete; false otherwise.
  */
  virtual bool done() const = 0;

  /** \details
    Gets the source point that is currently pointed at by the source point data
    iterator. The iterator object steps forward after getting the point.

    \param point [out] Resulting source point.

    \returns
    Point data correctness flag: true &ndash; data is correct; false &ndash;
    otherwise.
  */
  virtual bool getPoint(OdSourcePoint& point) = 0;
};

/** \details
  A data type that represents a shared pointer to an
  <link OdSourcePointIterator, OdSourcePointIterator> object.
*/
typedef OdSharedPtr<OdSourcePointIterator> OdSourcePointIteratorPtr;

/** <group OdRx_Classes>
  \details
  This is an interface class for representing cloud data sources.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdPointCloudDataSource
{
public:

  /** \details
    This enumeration represents measurement units for cloud data sources.
  */
  enum Units
  {
    /** Millimeters */
    kMillimeter = 0,
    /** Centimeters */
    kCentimeter,
    /** Meters */
    kMeter,
    /** Inches */
    kInch,
    /** Feet */
    kFeet,
    /** US feet */
    kUSFeet
  };

  /** \details
    Destructor for the OdPointCloudDataSource class.
  */
  virtual ~OdPointCloudDataSource(){}

  /** \details
    Returns the total number of points in the cloud data source.

    \returns
    Total number of points as an OdUInt64 value.
  */
  virtual OdUInt64 pointsCount() const = 0;

  /** \details
    Gets the measurement unit used in the cloud data source.

    \returns
    Measurement unit.
  */
  virtual Units getUnits() const = 0;

  /** \details
    Creates a new source point iterator.

    \returns
    Shared pointer to an OdSourcePointIterator object.
  */
  virtual OdSourcePointIteratorPtr newSourcePointIterator() const = 0;
};

/** \details
  A data type that represents a shared pointer to an
  <link OdPointCloudDataSource, OdPointCloudDataSource> object.
*/
typedef OdSharedPtr<OdPointCloudDataSource> OdPointCloudDataSourcePtr;

/** <group OdRcs_Classes>
  \details
  This is an interface class that provides functionality for converting a user
  point cloud to an .rcs file.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdPointCloudConverter
{
public:

  /** \details
    Destructor for the OdPointCloudConverter class.
  */
  virtual ~OdPointCloudConverter(){}

  /** \details
    Converts a point cloud to an .rcs file.

    \param rcsFilePath [in] Path to the resulting .rcs file.
  */
  virtual void convertToRcsFormat(const OdString& rcsFilePath, bool bMTMode = true) = 0;
};

/** \details
  A data type that represents a shared pointer to an
  <link OdPointCloudConverter, OdPointCloudConverter> object.
*/
typedef OdSharedPtr<OdPointCloudConverter> OdPointCloudConverterPtr;

#include "TD_PackPop.h"

#endif
