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

//
// This sample demonstrates implementation of the AsdkSmiley custom object 
// from ObjectARX using ODA Drawings SDK. Implementation of the smiley object
// consists of several steps and each step implements some functionality of the
// object. The smiley object is recommended to develop step-by-step adding
// new methods to implemented ones. 
//
#ifndef SMILEY_INCLUDED
#define SMILEY_INCLUDED
#define kPi  3.14159265358979323846

//#define SMILEY_WITH_TEXT

class AsdkSmiley : public OdDbEntity
{
  private:
	OdGeVector3d    mnormal;            // smiley normal vector
    OdGeCircArc3d   mfacecircle;        // smiley face
    OdGeCircArc3d   mmoutharc;          // smiley mouth arc (not written out)
    double          meyesapart;         // smiley eyes apart
    double          meyesheight;        // smiley eyes height
    double          meyesize;           // smiley eyes size
	OdCmEntityColor mbackcolor;

#ifdef SMILEY_WITH_TEXT
  static const OdChar mTextFieldValue[];
  OdString mTextField;
#endif

protected:
	void ensureRadiusMouth();
	void ensureRadiusEyes();

  public:
/** Description:
Enumeration for Smiley GS mark type.
*/
  enum gsMarkType 
  {
    /** Description:
    No GS mark.
    */
    kNone               = 0,

    /** Description:
    GS mark of smiley face.
    */
    kFaceMark          = 1,

    /** Description:
    GS mark of left smiley eye.
    */
    kLeftEyeMark     = kFaceMark  + 1,

    /** Description:
    GS mark of right smiley eye.
    */
    kRightEyeMark     = kLeftEyeMark  + 1,

    /** Description:
    GS mark of smiley mouth.
    */
    kMouthMark         = kRightEyeMark + 1
  };

	ODDB_DECLARE_MEMBERS(AsdkSmiley);	// Declare default functions. It is necessary for 
										// all Drawings database objects.
	AsdkSmiley();
	virtual ~AsdkSmiley();
	// Step 1
	bool subWorldDraw(OdGiWorldDraw* mode) const;			// for drawing 
	OdResult dwgInFields(OdDbDwgFiler* pFiler);			// for loading from DWG file
	void dwgOutFields(OdDbDwgFiler* pFiler) const;		// for saving in DWG file
	// Step 2
	OdResult dxfInFields(OdDbDxfFiler *filer);			// for loading from DXF file
	void dxfOutFields(OdDbDxfFiler *filer) const;		// for saving in DXF file
	// Step 3
	OdResult subTransformBy(const OdGeMatrix3d& xform);	// for moving and rotating 
  void scaleRadius(const double r);					// for scalling
	// Step 4
  virtual OdResult subGetGripPoints(OdGePoint3dArray& gripPoints) const;	
  OdResult subMoveGripPointsAt(const OdGePoint3dArray& gripPoints, const OdIntArray& indices);
  virtual OdResult subMoveGripPointsAt( const OdIntArray& indices, const OdGeVector3d& offset );
	// Step 5
	virtual OdResult subGetOsnapPoints(OdDb::OsnapMode osnapMode, OdGsMarker gsSelectionMark,
							const OdGePoint3d& pickPoint, const OdGePoint3d& lastPoint,
							const OdGeMatrix3d& xfm, const OdGeMatrix3d& ucs,
							OdGePoint3dArray& snapPoints ) const;
	// Step 6
	virtual OdResult subExplode(OdRxObjectPtrArray& entitySet) const;	// to destroy the smiley object
	//
	// Smiley property functions
    //
	virtual OdCmEntityColor backColor()	const;		// smiley back color
	virtual void setBackColor(const OdCmEntityColor& color);

	virtual OdGeVector3d normal() const;			// smiley normal
	virtual void setNormal(const OdGeVector3d v);
   
  virtual OdGePoint3d center() const;				// smiley center
  virtual void setCenter(const OdGePoint3d c);

  virtual double radius() const;					// smiley radius
  virtual void setRadius(const double r);

	virtual double eyesApart() const;				// eyes apart
	virtual void setEyesApart(const double a);

	virtual double eyesHeight() const;				// eyes height
	virtual void setEyesHeight(const double h);

	virtual double eyeSize() const;					// eyes size
	virtual void setEyeSize(const double s);

	virtual OdGePoint3d leftEyeCenter() const;		// center of left eye
	virtual OdGePoint3d rightEyeCenter() const;		// center of right eye

  virtual double mouthRadius() const;				// radius of mouth arc
  virtual OdGePoint3d mouthCenter() const;		// center of mouth arc
  virtual OdGePoint3d mouthLeft() const;			// left point of mouth arc
  virtual OdGePoint3d mouthRight() const;			// right point of mouth arc
  virtual OdGePoint3d mouthBottom() const;		// bottom point of mouth arc
  // whole mouth
  virtual void setMouth( const OdGePoint3d& left, const OdGePoint3d& bottom, const OdGePoint3d& right);
	void moveMouthToPoint( const OdGePoint3d point );

  // Subentity functions

  virtual OdResult subGetGsMarkersAtSubentPath( const OdDbFullSubentPath& subPath, OdGsMarkerArray& gsMarkers ) const;

  virtual OdResult subGetSubentPathsAtGsMarker( OdDb::SubentType type, OdGsMarker gsMark, 
    const OdGePoint3d& pickPoint,const OdGeMatrix3d& xfm, 
    OdDbFullSubentPathArray& subentPaths, 
    const OdDbObjectIdArray* pEntAndInsertStack = 0) const;

  virtual OdResult subGetSubentClassId( const OdDbFullSubentPath& path, void* clsId ) const;

  virtual OdResult subGetSubentPathGeomExtents( const OdDbFullSubentPath& path, OdGeExtents3d& extents ) const;

  virtual OdDbEntityPtr subSubentPtr( const OdDbFullSubentPath& id ) const;

  virtual OdResult subGetGripPointsAtSubentPath( const OdDbFullSubentPath& path, OdDbGripDataPtrArray& grips,
    const double curViewUnitSize, const int gripSize,
    const OdGeVector3d& curViewDir, const OdUInt32 bitflags) const;

  virtual OdResult subMoveGripPointsAtSubentPaths( const OdDbFullSubentPathArray& paths, const OdDbVoidPtrArray& gripAppData,
    const OdGeVector3d& offset, const OdUInt32 bitflags );

  virtual OdResult subTransformSubentPathsBy( const OdDbFullSubentPathArray& paths, const OdGeMatrix3d& xform );

  virtual void composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);
};

#endif  // SMILEY_INCLUDED
