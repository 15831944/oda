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




#ifndef _OD_DB_ELIPSE_H
#define _OD_DB_ELIPSE_H /*!DOM*/

#include "TD_PackPush.h"

#include "DbCurve.h"
#include "DwgDeclareMembers.h"

/** \details
  <group OdDb_Classes>

  This class implements the ellipse object (Ellipse entity) in the database. This class 
  inherits the base functionality of entities. The ellipse entity can represent either an 
  elliptical closed figure (ellipse) or unclosed elliptical segment (elliptical arc) in world
  space. When the start angle coincides with the end angle, the figure is closed and this 
  class represents an ellipse. When the start angle differs from the end angle on 1e-6, the 
  figure is unclosed and this class represents an elliptical arc. 

  \remarks
  The ellipse must have (StartAngle = 0) and (EndAngle = Oda2PI).
  The elliptical arc has (EndAngle - StartAngle) > 1e-6.
  The radius ratio must be in the range [1e-6 to 1.0].

  \sa
  <link db_ellipse_sample, Example of Working with the Ellipse Object>
  \sa
  <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
  \sa
  <link db_ellipse, Working with Ellipses>
  \sa
  <link OdDbCircle, OdDbCircle class> 
	\sa
	<link OdDbArc, OdDbArc class>
*/
class DBENT_EXPORT OdDbEllipse : public OdDbCurve
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbEllipse);

  /** \details
    Builds an instance of the ellipse object.
    
    \remarks
    Use the static pseudo-constructor instead it.
    See: <link db_ellipse_overview, Overview of Ellipses>
  */
  OdDbEllipse();


  /** \details
    Returns the WCS coordinates of the center for the elliptical figure (ellipse or arc) 
    (DXF 10) as three-dimensional point instance.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
    \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_specific, Specific Ellipse Properties>
		\sa
    <link OdDbEllipse::setCenter@OdGePoint3d&, OdDbEllipse::setCenter() method>
  */
  OdGePoint3d center() const;


  /** \details
    Sets the center of the elliptical figure (ellipse or arc) (DXF 10) in WCS.
    The initial value is (0,0,0) by default.
    
    \param center [in]  Three-dimensional point instance to set the center.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
    \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_specific, Specific Ellipse Properties>
    \sa
    <link OdDbEllipse::center@const, OdDbEllipse::center() method>
  */
  void setCenter( const OdGePoint3d& center );


  /** \details
    Returns the WCS unit vector which is the normal to the plane of the ellipse entity 
    (DXF 210). The normal defines the orientation of the ellipse plane in world space.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
    \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_specific, Specific Ellipse Properties>
  */
  OdGeVector3d normal() const;


  /** \details
    Returns the WCS major axis vector for the elliptical figure (ellipse or arc). The 
    major axis defines the major radius and rotation of the elliptical figure in the plane.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
    \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_specific, Specific Ellipse Properties>
    \sa
    <link OdDbEllipse::minorAxis@const, OdDbEllipse::minorAxis() method>
		\sa
		<link OdDbEllipse::radiusRatio@const, OdDbEllipse::radiusRatio() method>
  */
  OdGeVector3d majorAxis() const;


  /** \details
    Returns the WCS minor axis vector for the elliptical figure (ellipse or arc). 
    The minor axis defines the minor radius. The minor axis is perpendicular to the 
    major axis. The minor radius less than or equal the major radius.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
    \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_specific, Specific Ellipse Properties>
    \sa
		<link OdDbEllipse::majorAxis@const, OdDbEllipse::majorAxis() method>
    \sa
		<link OdDbEllipse::radiusRatio@const, OdDbEllipse::radiusRatio() method>
  */
  OdGeVector3d minorAxis() const;


  /** \details
    Returns the ratio of the minor radius to the major radius for the elliptical figure 
    (ellipse or arc). The radius ratio is in the range [1e-6 to 1.0].

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
    \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_specific, Specific Ellipse Properties>
		\sa
    <link OdDbEllipse::minorAxis@const, OdDbEllipse::minorAxis() method>
    \sa
		<link OdDbEllipse::majorAxis@const, OdDbEllipse::majorAxis() method>
		\sa
    <link OdDbEllipse::setRadiusRatio@double, OdDbEllipse::setRadiusRatio() method>
  */
  double radiusRatio() const;


  /** \details
    Sets the ratio of the minor radius to the major radius for the elliptical figure 
    (ellipse or arc) in the range [1e-6 to 1.0]. The initial value is 1.0 by default.

    \param radiusRatio [in]  Radius ratio as a Double value.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
    \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_specific, Specific Ellipse Properties>
		\sa
    <link OdDbEllipse::minorAxis@const, OdDbEllipse::minorAxis() method>
    \sa
		<link OdDbEllipse::majorAxis@const, OdDbEllipse::majorAxis() method>
		\sa
    <link OdDbEllipse::radiusRatio@const, OdDbEllipse::radiusRatio() method>
  */
  void setRadiusRatio( double radiusRatio );


  /** \details
    Returns the start angle of the elliptical figure (ellipse or arc) in radians (DXF 50).

    \remarks
    The start angle is measured counterclockwise from the major axis. When (StartAngle = 0) 
    and (EndAngle = Oda2PI) and (EndAngle - StartAngle) > 1e-6, the figure is closed, 
    that is an ellipse.

    \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_elliptical_arc_specific, Specific Elliptical Arc Properties>
    \sa
    <link OdDbEllipse::setStartAngle@double, OdDbEllipse::setStartAngle() method>
		\sa	
		<link OdDbEllipse::endAngle@const, OdDbEllipse::endAngle() method>
  */
  double startAngle() const;


  /** \details
    Sets the start angle for the elliptical figure (ellipse or arc) in the range ±2PI  
    radians (DXF 50). A positive value specifies the angle measured from the major axis  
    counterclockwise. A negative value specifies the angle measured from the major axis  
    clockwise. The initial value is zero by default.

    \param startAngle [in]  Start angle as a Double.
    
    \remarks
    If an angle value is negative, this method automatically converts it to the positive 
    equivalent. If an angle value is more than 2PI, this method automatically convertes 
    it to the range 0 to 2PI. When (StartAngle = 0) and (EndAngle = Oda2PI) and 
    (EndAngle - StartAngle) > 1e-6, the figure is closed, that is an ellipse.

    \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_elliptical_arc_specific, Specific Elliptical Arc Properties>
	  \sa
    <link OdDbEllipse::startAngle@const, OdDbEllipse::startAngle() method>
		\sa
		<link OdDbEllipse::setEndAngle@double, OdDbEllipse::setEndAngle() method>
  */
  void setStartAngle( double startAngle );


  /** \details
    Returns the end angle of the elliptical figure (ellipse or arc) in radians (DXF 51).

    \remarks
    The end angle is measured counterclockwise from the major axis. When (StartAngle = 0)  
    and (EndAngle = Oda2PI) and (EndAngle - StartAngle) > 1e-6, the figure is closed,  
    that is an ellipse.

    \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_elliptical_arc_specific, Specific Elliptical Arc Properties>
	  \sa
    <link OdDbEllipse::setEndAngle@double, OdDbEllipse::setEndAngle() method>
		\sa
		<link OdDbEllipse::startAngle@const, OdDbEllipse::startAngle() method>
  */
  double endAngle() const;


  /** \details
    Sets the end angle for the elliptical figure (ellipse or arc) in the range ±2PI 
    radians (DXF 51). A positive value specifies the angle measured from the major axis 
    counterclockwise. A negative value specifies the angle measured from the major axis 
    clockwise. The initial value is 2PI by default.

    \param endAngle [in]  End angle as a Double value.

    \remarks
    If an angle value is negative, this method automatically converts it to the positive 
    equivalent. If an angle value is more than 2PI, this method automatically convertes 
    it to the range 0 to 2PI. When (StartAngle = 0) and (EndAngle = Oda2PI) and 
    (EndAngle - StartAngle) > 1e-6, the figure is closed, that is an ellipse.

    \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_elliptical_arc_specific, Specific Elliptical Arc Properties>
	  \sa
    <link OdDbEllipse::setStartAngle@double, OdDbEllipse::setStartAngle() method>
		\sa
		<link OdDbEllipse::endAngle@const, OdDbEllipse::endAngle() method>
  */
  void setEndAngle( double endAngle );


  /** \details
    Returns the parameter value corresponding to the specified angle value.

    \param angle [in]  Angle as a Double value in radians.

    \remarks
    Can be used to retrieve the DXF 41 & 42 values.

    \sa
    <link db_elliptical_arc_specific, Specific Elliptical Arc Properties>
  */
  double paramAtAngle( double angle ) const;


  /** \details
    Returns the angle value corresponding to the specified parameter value.

    \param param [in]  Parameter as a Double value in radians.

    \sa
    <link db_elliptical_arc_specific, Specific Elliptical Arc Properties>
  */
  double angleAtParam( double param ) const;


  /** \details
    {NoAutoLink}
    Gets the properties for the ellipse entity. This method passes the properties 
    through own arguments.

    \param center [out]  Receives the WCS center.
    \param unitNormal [out]  Receives the WCS unit normal.
    \param majorAxis [out]  Receives the WCS major axis.
    \param radiusRatio [out]  Receives the radius ratio.
    \param startAngle [out]  Receives the start angle.
    \param endAngle [out]  Receives the end angle.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_specific, Specific Ellipse Properties>
	  \sa
    <link db_elliptical_arc_specific, Specific Elliptical Arc Properties>
  */
  void get(OdGePoint3d& center,
    OdGeVector3d& unitNormal,
    OdGeVector3d& majorAxis,
    double& radiusRatio,
    double& startAngle,
    double& endAngle) const;


  /** \details
    {NoAutoLink}
    Sets the properties for the ellipse entity.

    \param center [in]  WCS center.
    \param unitNormal [in]  WCS unit normal.
    \param majorAxis [in]  WCS major axis.
    \param radiusRatio [in]  Radius ratio.
    \param startAngle [in]  Start angle.
    \param endAngle [in]  End angle.    

    \remarks
    All angles are expressed in radians. A closed ellipse entity must have (StartAngle = 0)
    and (EndAngle = Oda2PI) and (EndAngle - StartAngle) > 1e-6. The radius ratio must be in 
    the range [1e-6 to 1.0]. The major axis must be perpendicular to the normal within 1e-6.
  
    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_specific, Specific Ellipse Properties>
	  \sa
    <link db_elliptical_arc_specific, Specific Elliptical Arc Properties>
  */
  void set(const OdGePoint3d& center,
    const OdGeVector3d& unitNormal,
    const OdGeVector3d& majorAxis,
    double radiusRatio,
    double startAngle = 0.0,
    double endAngle = Oda2PI);


  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;

  virtual bool isClosed() const;

  virtual bool isPeriodic() const;


  /** \details
    Determines whether the elliptical figure is planar as a Boolean value and returns True.
    This method is inherited from the OdDbEntity class.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
    \sa
    <link db_ellipse, Working with Ellipses>
	  \sa
    <link OdDbEllipse::getPlane@OdGePlane&@OdDb::Planarity&@const, OdDbEllipse::getPlane() method>
		\sa
		<link OdDbEntity::isPlanar@const, OdDbEntity::isPlanar() method>
  */
  virtual bool isPlanar() const;


  /** \details
    Gets a plane in which the ellipse entity places. This method is inherited from the  
    OdDbEntity class.

    \param gePlane [out]  Reference to a variable in which this method must save the plane parameters as 
                a plane instance.
    \param gePlanarity [out] Reference to a variable in which this method must save the plane type
                    (returns kPlanar - planar entity).

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
    \sa
    <link db_ellipse, Working with Ellipses>
	  \sa
    <link OdDbEllipse::isPlanar@const, OdDbEllipse::isPlanar() method>
		\sa
		<link OdDbEntity::getPlane@OdGePlane&@OdDb::Planarity&@const, OdDbEntity::getPlane() method>
  */
  virtual OdResult getPlane( OdGePlane& gePlane, OdDb::Planarity& gePlanarity ) const;


  /** \details
    Gets the start parameter value of the elliptical figure (ellipse or arc). The 
    OdDbEllipse::setStartParam() method sets this value. Parameter is an angle 
    measured counterclockwise from the major axis in the range 0 to 2PI radians as 
    if the elliptical figure was a circular figure with a major radius.

    \param startParam [out]  Reference to a variable in which this method must save the start    
                   parameter value as a Double value in radians.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
	  \sa
    <link db_elliptical_arc_specific, Specific Elliptical Arc Properties>
	  \sa
    <link OdDbEllipse::setStartParam@double, OdDbEllipse::setStartParam() method>
		\sa
		<link OdDbEllipse::getEndParam@double&@const, OdDbEllipse::getEndParam() method>
  */
  virtual OdResult getStartParam( double& startParam ) const;


  /** \details
    Sets the start parameter for the elliptical figure (ellipse or arc) (DXF 41).
    Parameter is an angle measured from the major axis in the range 0 to 2PI 
    radians as if the elliptical figure was a circular figure with a major radius.
    A positive value specifies the start parameter measured counterclockwise
    A negative value specifies the start parameter measured clockwise. The initial
    value is zero by default.

    \param startParam [in]  Start parameter as a Double value in radians.

    \remarks
    If an parameter value is negative, this method automatically converts it to the 
    positive equivalent. If an angle value is more than 2PI, this method automatically 
    convertes it to the range 0 to 2PI. When (StartParam = 0) and (EndParam = Oda2PI) and 
    (EndParam - StartParam) > 1e-6, the figure is closed, that is an ellipse.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
	  \sa
    <link db_elliptical_arc_specific, Specific Elliptical Arc Properties>
	  \sa
    <link OdDbEllipse::getStartParam@double&@const, OdDbEllipse::getStartParam() method>
		\sa
		<link OdDbEllipse::setEndParam@double, OdDbEllipse::setEndParam() method>
  */
  void setStartParam( double startParam );


  /** \details
    Gets the end parameter value of the elliptical figure (ellipse or arc). The 
    OdDbEllipse::setEndParam() method sets this value. Parameter is an angle 
    measured counterclockwise from the major axis in the range 0 to 2PI radians 
    as if the elliptical figure was a circular figure with a major radius.

    \param endParam [out]  Reference to a variable in which this method must save the end    
                 parameter value as a Double value in radians.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
	  \sa
    <link db_elliptical_arc_specific, Specific Elliptical Arc Properties>
	  \sa
		<link OdDbEllipse::setEndParam@double, OdDbEllipse::setEndParam() method>
		\sa
    <link OdDbEllipse::getStartParam@double&@const, OdDbEllipse::getStartParam() method>
  */
  virtual OdResult getEndParam( double& endParam ) const;


  /** \details
    Sets the end parameter for the elliptical figure (ellipse or arc) (DXF 42).
    Parameter is an angle measured from the major axis in the range 0 to 2PI 
    radians as if the elliptical figure was a circular figure with a major radius.
    A positive value specifies the end parameter measured counterclockwise
    A negative value specifies the end parameter measured clockwise. The initial
    value is 2PI by default.

    \param endParam [in]  End parameter as a Double value in radians.

    \remarks
    If an parameter value is negative, this method automatically converts it to the 
    positive equivalent. If an angle value is more than 2PI, this method automatically 
    convertes it to the range 0 to 2PI. When (StartParam = 0) and (EndParam = Oda2PI) and 
    (EndParam - StartParam) > 1e-6, the figure is closed, that is an ellipse.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
	  \sa
    <link db_elliptical_arc_specific, Specific Elliptical Arc Properties>
	  \sa
    <link OdDbEllipse::setStartParam@double, OdDbEllipse::setStartParam() method>
		\sa
		<link OdDbEllipse::getEndParam@double&@const, OdDbEllipse::getEndParam() method>
  */
  void setEndParam( double endParam );


  /** \details
    Computes the WCS coordinates of the start point on the elliptical figure (ellipse or arc) 
    for which the parameter equals the start value. When the elliptical figure is closed, the 
    end point coincides with the start point. When the elliptical figure is unclosed, the 
    start point corresponds to the start angle.

    \param startPoint [out]  Reference to a variable in which this method must save the start 
                   point as three-dimensional point instance.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
  */
  virtual OdResult getStartPoint( OdGePoint3d& startPoint ) const;


  /** \details
    Computes the WCS coordinates of the end point on the elliptical figure (ellipse or arc) 
    for which the parameter equals the end value. When the elliptical figure is closed, the 
    end point coincides with the start point. When the elliptical figure is unclosed, the 
    end point corresponds to the end angle.

    \param endPoint [out]  Reference to a variable in which this method must save the end  
                 point as three-dimensional point instance.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
  */
  virtual OdResult getEndPoint( OdGePoint3d& endPoint ) const;


  /** \details
    Computes the WCS coordinates of the point on the elliptical figure (ellipse or arc)  
    at the specified parameter value. Parameter is an angle measured counterclockwise  
    from the major axis in the range 0 to 2PI radians as if the elliptical figure was  
    a circular figure with a major radius.

    \param param [in]  Parameter value specifying the point on the elliptical curve as a Double 
              value in radians.
    \param pointOnCurve [out] Reference to a variable in which this method must save the 
                     three-dimensional point instance corresponding to the parameter.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
  */
  virtual OdResult getPointAtParam( double param, OdGePoint3d& pointOnCurve ) const;


  /** \details
    Computes the parameter at the point placed on the elliptical figure (ellipse or arc). 
    Parameter is an angle measured counterclockwise from the major axis in the range 
    0 to 2PI radians as if the elliptical figure was a circular figure with a major radius.

    \param pointOnCurve [in]  Three-dimensional point instance which specifies the point on 
                     the elliptical curve. 
    \param param [out] Reference to a variable in which this method must save the parameter   
              value as a Double value in radians.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
  */
  virtual OdResult getParamAtPoint( const OdGePoint3d& pointOnCurve, double& param ) const;


  /** \details
    Computes the distance along the elliptical figure (ellipse or arc) measured from the 
    start point to the point on the elliptical curve specified the parameter.

    \param param [in]  Parameter value specifying the point on the elliptical curve as a Double 
              value in radians. 
    \param dist  [out] Reference to a variable in which this method must save the distance value 
              as a Double value in drawing units.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
  */
  virtual OdResult getDistAtParam( double param, double& dist ) const;


  /** \details
    Computes the parameter at the distance along the elliptical figure (ellipse or arc). 
    Parameter is an angle measured counterclockwise from the major axis in the range  
    0 to 2PI radians as if the elliptical figure was a circular figure with a major radius.

    \param dist [in]   Distance value along the elliptical curve as a Double value in drawing units. 
    \param param [out]  Reference to a variable in which this method must save the parameter value  
              as a Double value in radians.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
  */
  virtual OdResult getParamAtDist( double dist, double& param ) const;


  /** \details
    Computes the first derivative for the specified parameter of the elliptical figure 
    (ellipse or arc) and returns the three-dimensional vector instance.

    \param param [in]  Parameter value specifying the point on the elliptical curve as a Double 
              value in radians. 
    \param firstDeriv [out] Reference to a variable in which this method must save the first  
                   derivative as three-dimensional vector instance.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	  \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
  */
  virtual OdResult getFirstDeriv( double param, OdGeVector3d& firstDeriv ) const;


  /** \details
    Computes the second derivative for the specified parameter of the elliptical figure   
    (ellipse or arc) and returns the three-dimensional vector instance.

    \param param [in]  Parameter value specifying the point on the elliptical curve as a Double 
              value in radians. 
    \param secondDeriv [out] Reference to a variable in which this method must save the second   
                    derivative as three-dimensional vector instance.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
    \sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
  */
  virtual OdResult getSecondDeriv( double param, OdGeVector3d& secondDeriv ) const;


  /** \details
    When an elliptical figure is unclosed, this method returns the area calculated for the 
    closed figure formed by the elliptical arc and a line passed from the end point to the 
    start point. When an elliptical figure is closed, this method returns the area of the 
    ellipse. 

    \param vArea [out]  Reference to a variable in which this method must save the area
              as a Double value.

    \sa
    <link db_ellipse_sample, Example of Working with the Ellipse Object>
	\sa
    <link db_elliptical_arc_sample, Example of Working with the Elliptical Arc>
    \sa
    <link db_ellipse_compute, Computed Ellipse and Elliptical Arc Properties>
  */
  virtual OdResult getArea( double& vArea ) const;


  TD_USING(OdDbCurve::getFirstDeriv);
  TD_USING(OdDbCurve::getSecondDeriv);

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  virtual OdResult getOdGeCurve(OdGeCurve3d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol) const;
  virtual OdResult setFromOdGeCurve(const OdGeCurve3d& geCurve, OdGeVector3d *normal = NULL, const OdGeTol& tol = OdGeContext::gTol);
  virtual OdResult reverseCurve();
};

/** \details
  The typified smart pointer for the ellipse object (Ellipse entity). This template  
  class is specialization of the OdSmartPtr class for the OdDbEllipse class.

  \sa
  <link smart_pointers, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbEllipse> OdDbEllipsePtr;

#include "TD_PackPop.h"

#endif //_OD_DB_ELIPSE_H

