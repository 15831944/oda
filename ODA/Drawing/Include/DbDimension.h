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




#ifndef OD_DBDIM_H
#define OD_DBDIM_H

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DbMText.h"
#include "Ge/GeScale3d.h"

class OdDbDimStyleTableRecord;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbDimStyleTableRecord object pointers.
*/
typedef OdSmartPtr<OdDbDimStyleTableRecord> OdDbDimStyleTableRecordPtr;

/** \details
    This class is the base class for all Dimension classes in an OdDbDatabase instance.
    
    
    <group OdDb_Classes>
	
	\sa 
	<link db_dimensions, Working with dimensions> 
*/
class DBDIM_EXPORT OdDbDimension : public OdDbEntity
{
public:
  //DOM-IGNORE-BEGIN
  DWGMAP_DECLARE_MEMBERS(OdDbDimension);
  //DOM-IGNORE-END

  /** \details
    Constructor. Initializes fields with default values.
  */
  OdDbDimension();

  /** \details
    Returns the position of the dimension text for this Dimension entity (DXF 11 as WCS).

    \remarks
    This position is the middle center point of the dimension text, which is itself a 
    middle-center justified MText entity.  
  */
  OdGePoint3d textPosition() const;
  
  /** \details
    Sets the position of the dimension text for this Dimension entity (DXF 11 as WCS).

    \param textPosition [in]  Text position.  
    \remarks
    This position is the middle center point of the dimension text, which is itself a 
    middle-center justified MText entity.  
  */
  void setTextPosition(
    const OdGePoint3d& textPosition);

  /** \details
    Returns true if and only if the dimension text for this Dimension entity
    is in the default position (DXF 70, bit 0x80 == 0).
  */
  bool isUsingDefaultTextPosition() const;

  /** \details
    Sets the dimension text for this Dimension entity to not use the default position (DXF 70, sets bit 0x80).
  */
  void useSetTextPosition();

  /** \details
    Sets the dimension text for this Dimension entity to use the default position (DXF 70, clears bit 0x80).
  */
  void useDefaultTextPosition();

  /** \details
    Returns the WCS normal to the plane of this entity (DXF 210).
  */
  OdGeVector3d normal() const;

  /** \details
    Sets the WCS normal to the plane of this entity (DXF 210).

    \param normal [in]  Normal.
  */
  void setNormal(
    const OdGeVector3d& normal);

  /** \details
    Checks if this dimension is planar. 
    \returns true by default since built-in dimensions are planar.
  */
  virtual bool isPlanar() const { return true; }

  /** \details
    Retrieves the WCS plane that this dimension contains. Sets planarity flag to OdDb::kPlanar.

    \param plane     [out] WCS plane that this dimension contains.
    \param planarity [out] Flag that states whether this dimension is planar or not.
    \returns eOk.
  */
  virtual OdResult getPlane(
    OdGePlane& plane, 
    OdDb::Planarity& planarity) const;

  /** \details
    Returns the elevation of this entity in the OCS (DXF 30).
    
    \remarks
    The elevation is the distance from the WCS origin to the plane of this entity.
  */
  double elevation() const;

  /** \details
    Sets the elevation of this entity in the OCS (DXF 30).

    \param elevation [in]  Elevation.    
    \remarks
    The elevation is the distance from the WCS origin to the plane of this entity.
  */
  void setElevation(
    double elevation);

  /** \details
    Returns the user-supplied dimension text for this Dimension Entity (DXF 1).

    \remarks
    * If no text is to appear, "." will be returned.
    * If only the default text is to appear, "" will be returned.
    * The default text is represented as "<>" within the returned text.
  */
  const OdString dimensionText() const;

  /** \details
    Sets the user-supplied dimension text for this Dimension Entity (DXF 1).
    
    \param dimensionText [in]  Dimension text.
    \remarks
    * If no text is to appear, supply ".".
    * If only the default text is to appear, supply an empty string "".
    * The default text is represented as "<>" within the supplied text.
     
  */
  void setDimensionText(
    const OdString& dimensionText);
    
  /** \details
    Returns the rotation angle for the dimension text for this Dimension entity (DXF 53).

    \remarks
    All angles are expressed in radians.
  */
  double textRotation() const;

  /** \details
    Sets the rotation angle for the dimension text for this Dimension entity (DXF 53).
    
    \param textRotation [in]  Text rotation.
    \remarks
    All angles are expressed in radians.
  */
  void setTextRotation(
    double textRotation);

  /** \details
    Returns the Object ID of the dimension style (OdDbDimStyleTableRecord) for this Dimension entity (DXF 3).
  */
  OdDbObjectId dimensionStyle() const;

  /** \details
    Sets the Object ID of the dimension style (OdDbDimStyleTableRecord) for this Dimension entity (DXF 3).
    \param objectID [in]  Object ID.
  */
  void setDimensionStyle(
    OdDbObjectId objectID);

  /** \details
    Returns the dimension text attachment point for this Dimension entity (DXF 71).
  */
  OdDbMText::AttachmentPoint textAttachment() const;

  /** \details
    Sets the dimension text attachment point for this Dimension entity (DXF 71).
    \param attachmentPoint [in]  Attachment Point.
  */
  void setTextAttachment(
    OdDbMText::AttachmentPoint attachmentPoint);

  /** \details
    Returns the dimension text line spacing style for this Dimension entity (DXF 72).
	
	\sa
    <exref target="https://docs.opendesign.com/tkernel/OdDb__LineSpacingStyle.html">OdDb::LineSpacingStyle enum</exref>
  */
  OdDb::LineSpacingStyle textLineSpacingStyle() const;

  /** \details
    Sets the dimension text line spacing style for this Dimension entity (DXF 72).
    \param lineSpacingStyle [in]  Line spacing style.
	
	\sa
    <exref target="https://docs.opendesign.com/tkernel/OdDb__LineSpacingStyle.html">OdDb::LineSpacingStyle enum</exref>
  */
  void setTextLineSpacingStyle(
    OdDb::LineSpacingStyle lineSpacingStyle);

  /** \details
    Returns the dimension text line spacing factor for this Dimension entity (DXF 41).
    
    \sa
    <exref target="https://docs.opendesign.com/tkernel/OdDb__LineSpacingStyle.html">OdDb::LineSpacingStyle enum</exref>
  */
  double textLineSpacingFactor() const;

  /** \details
    Sets the dimension text line spacing factor for this Dimension entity (DXF 41).
    
    \param lineSpacingFactor [in]  Line spacing factor. [0.25 .. 4.0]
    \sa
    <exref target="https://docs.opendesign.com/tkernel/OdDb__LineSpacingStyle.html">OdDb::LineSpacingStyle enum</exref>
  */
  void setTextLineSpacingFactor(
    double lineSpacingFactor);

  /** \details
    Copies the dimension style settings, including overrides, of this Dimension entity into the specified
    dimension style table record.
    
    \param pRecord [out]  Receives the effective dimension style data associated with entity.
    \remarks
    The copied data includes the dimension style data with all applicable overrides. 
  */
  void getDimstyleData(
    OdDbDimStyleTableRecord *pRecord) const;

  /** \details
    Copies the dimension style settings, including overrides, from the specified
    dimension style table record to this Dimension entity.
    
    \param pDimstyle [in]  Pointer to a non- database -resident dimension style record.
    \remarks
    The copied data includes the dimension style with all applicable overrides. 
  */
  void setDimstyleData(
    const OdDbDimStyleTableRecord* pDimstyle);
    
  /** \details
    Copies the dimension style settings, including overrides, from the specified
    dimension style table record to this Dimension entity.
    
    \param dimstyleID [in]  Database-resident dimension style record.
    \remarks
    The copied data includes the dimension style with all applicable overrides. 
  */
  void setDimstyleData(
    OdDbObjectId dimstyleID);

  /** \details
    Returns the horizontal rotation angle for this Dimension entity (DXF 51).
    \remarks
    All angles are expressed in radians.
  */
  double horizontalRotation() const;

  /** \details
    Sets the horizontal rotation angle for this Dimension entity (DXF 51).
    \param horizontalRotation [in]  Horizontal rotation angle.
    \remarks
    All angles are expressed in radians.
  */
  void setHorizontalRotation(
    double horizontalRotation);


  /** \details
    Returns the Object ID of the dimension block (OdDbBlockTableRecord) associated with this Dimension entity (DXF 2).
  */
  OdDbObjectId dimBlockId() const;

  /** \details
    Sets the Object ID of the dimension block (OdDbBlockTableRecord) associated with this Dimension entity.
   
    \param dimBlockId [in]  Object ID of the OdDbBlockTableRecord.
    \param singleReferenced [in]  True if and only if the dimension block is referenced only by this Dimension entity (DXF 70 bit 0x20).
  */
  void setDimBlockId(
    const OdDbObjectId& dimBlockId, 
    bool singleReferenced = true);

  /** \details
    Returns true and only if this Dimension entity has the only reference to its 
    associated OdDbBlockTableRecord (DXF 70 bit 0x20).
  */
  bool isSingleDimBlockReference() const;

  /** \details
    Returns the WCS relative position of the block associated with this Dimension entity (DXF 12).
      
    \remarks
    dimBlockPosition is the insertion point of the block with respect to the primary
    definition point (DXF 10) of the dimension block.      
  */
  OdGePoint3d dimBlockPosition() const;

  /*!DOM*/
  /** \details
    Sets the relative position of the dimension block referenced by this Dimension entity,
    in WCS (DXF 12). 
    
    \param dimBlockPosition [in] Dimension block position.
    \remarks
    For internal use only. Dimension update sets it to 0,0,0
  */
  void setDimBlockPosition(
    const OdGePoint3d& dimBlockPosition);


  /** \details
    Returns the rotation angle of the dimension block referenced by this Dimension entity (DXF 54).

    \remarks
    All angles are expressed in radians.    
  */
  double dimBlockRotation() const;

  /*!DOM*/
  /** \details
    Sets the rotation of the dimension block referenced by this Dimension entity (DXF 54). 
    
    \remarks
    For internal use only. Dimension update sets it to 0.
    \param dimBlockRotation [in] Dimension block rotation in radians.
  */
  void setDimBlockRotation(
    double dimBlockRotation);

  /** \details
    Returns the scale of the dimension block referenced by this Dimension entity.
    
    \remarks
    This is not saved to a DXF file.
  */
  OdGeScale3d dimBlockScale() const;

  /*!DOM*/
  /** \details
    Sets the scale of the dimension block referenced by this Dimension entity.
    
    \remarks
    For internal use only. Dimension update sets it to 1,1,1
    \param dimBlockScale [in] Dimension block scale.
  */
  void setDimBlockScale(
    const OdGeScale3d& dimBlockScale);

  /** \details
    Returns the transformation matrix applied to dimension block referenced by this Dimension entity.
    
    \remarks
    The transformation consists of position, scale, rotation angle and normal.
    It does not include block's base point.
  */
  OdGeMatrix3d dimBlockTransform() const;
  
  /** \details
    Recomputes the dimension block referenced by this Dimension entity.
    
    \remarks
    The OdDbBlockTableRecord of the dimension block is updated to reflect any changes made to this Dimension
    entity since the last time the block table record was updated.
    \param forceUpdate [in]  If true, the OdDbBlockTableRecord is updated even if the dimension has not been changed.
  */
  void recomputeDimBlock(
    bool forceUpdate = true);

  /** \details
    Returns the measurement (DXF 42) stored in this Dimension entity.
      
    \remarks
    Returns -1.0 if the stored measurement value is invalid. 

    Does not recompute the measurement.

    This Dimension entity need not be opened for write to call this function.
    \sa
    <link OdDbDimension::measurement, measurement() method>
  */
  double getMeasurement() const;

  /** \details
    Returns the measurement (DXF 42) for this Dimension entity.
    
    \remarks
    Recomputes the measurement if the stored measurement value is invalid (-1.0).
    
    This Dimension entity must be opened for write to call this function.

    \sa
    <link OdDbDimension::getMeasurement@const, getMeasurement() method>
  */
  double measurement();


  // 2005-2006 support

  /** \details
    Returns the background text color and flags for this Dimension entity. The
    background text flags value specifies the background fill of dimension text.

    \param bgrndTxtColor [out]  Receives the background text color.

    \returns Background text flags.

    \remarks
    Background text flags value can be one of the following:
    <table>
    Value   Description
    0       No fill
    1       Drawing background color
    2       Color specified by DIMTFILLCLR
    </table>

    \sa
    <link db_dimensions_text_formatting, Formatting Dimension Text>
  */
  OdUInt16 getBgrndTxtColor(OdCmColor& bgrndTxtColor) const;

  /** \details
    Sets the background text color and flags for this Dimension entity. The
    background text flags value specifies the background fill of dimension text.

    \param bgrndTxtColor [in]  Background text color.
    \param bgrndTxtFlags [in]  Background text flags.

    \remarks
    Background text flags value can be one of the following:
    <table>
    Value   Description
    0       No fill
    1       Drawing background color
    2       Color specified by DIMTFILLCLR
    </table>

    \sa
    <link db_dimensions_text_formatting, Formatting Dimension Text>
  */
  void setBgrndTxtColor(const OdCmColor& bgrndTxtColor, OdUInt16 bgrndTxtFlags );

  /** \details
    Returns the extension line fixed length enable flag for this Dimension entity.
  */
  bool getExtLineFixLenEnable() const;

  /** \details
    Controls the extension line fixed length enable flag for this Dimension entity.

    \param extLineFixLenEnable [in]  Extension line fixed length flag.
  */
  void setExtLineFixLenEnable(bool extLineFixLenEnable);

  /** \details
    Returns the extension line fixed length for this Dimension entity.
  */
  double getExtLineFixLen() const;

  /** \details
    Sets the extension line fixed length for this Dimension entity.

    \param extLineFixLen [in]  Extension line fixed length.
  */
  void setExtLineFixLen(double extLineFixLen);

  /** \details
    Returns the Object ID of the dimension line linetype for this Dimension entity.
  */
  OdDbObjectId getDimLinetype() const;

  /** \details
    Sets the Object ID of the dimension line linetype for this Dimension entity.

    \param linetypeId [in]  Dimension line linetype Object ID.
  */
  void setDimLinetype(const OdDbObjectId& linetypeId);

  /** \details
    Returns the Object ID of the first extension line linetype for this Dimension entity.
  */
  OdDbObjectId getDimExt1Linetype() const;

  /** \details
    Sets the Object ID of the first extension line linetype for this Dimension entity.
    
    \param linetypeId [in]  First extension line linetype Object ID.
  */
  void setDimExt1Linetype(const OdDbObjectId& linetypeId);

  /** \details
    Returns the Object ID of the second extension line linetype for this Dimension entity.
  */
  OdDbObjectId getDimExt2Linetype() const;

  /** \details
    Sets the Object ID of the second extension line linetype for this Dimension entity.
    
    \param linetypeId [in]  Second extension line linetype Object ID.
  */
  void setDimExt2Linetype(const OdDbObjectId& linetypeId);

  /** \details
    Returns the flip flag of the first arrowhead for this Dimension entity.
  */
  bool getArrowFirstIsFlipped() const;

  /** \details
    Returns the flip flag of the second arrowhead for this Dimension entity.
  */
  bool getArrowSecondIsFlipped() const;
  
  /** \details
    Sets the flip flag of the first arrowhead for this Dimension entity.

    \param bIsFlipped [in]  Flip flag value.
  */
  void setArrowFirstIsFlipped(bool bIsFlipped);

  /** \details
    Sets the flip flag of the second arrowhead for this Dimension entity.

    \param bIsFlipped [in]  Flip flag value.
  */
  void setArrowSecondIsFlipped(bool bIsFlipped);

  /** \details
    Defines dimension inspection settings.
  */
  enum DimInspect 
  {
    /** No inspection label displays. */
    kShapeRemove  = 0x00,      
    
    /** Inspection label displays with a rounded end (this is the default). */
    kShapeRound   = 0x01,      
    
    /** Inspection label displays with an angular end. */
    kShapeAngular = 0x02,      
    
    /** Inspection label displays without a bounding shape. */
    kShapeNone    = 0x04,      
    
    /** Inspection label displays with a prefix, including a separator and alpha field. */
    kShapeLabel   = 0x10,      
    
    /** Inspection label displays at a specified frequency. */
    kShapeRate    = 0x20       
  };

  /** \details
    Checks the inspection flag for this dimension.

    \returns true if the flag is set to true, false - otherwise.
  */
  bool inspection() const;

  /** \details
    Sets a value to the inspection flag for this dimension.

    \param val [in] New inspection flag value.
  */
  void setInspection(bool val);

  /** \detials
    Returns the inspection frame value.
  */
  int inspectionFrame() const;

  /** \detials
    Sets inspection frame value.
    
    \param frame [in] New inspection frame value.
  */
  void setInspectionFrame(int frame);

  /** \detials
    Returns the inspection label value.
  */
  OdString inspectionLabel() const;
  
  /** \detials
    Sets the inspection label value.

    \param label [in] New inspection label value.
  */
  void setInspectionLabel(const OdString& label);

  /** \detials
    Returns the inspection rate value.
  */
  OdString inspectionRate() const;
  
  /** \detials
    Sets the inspection rate value.

    \param label [in] New inspection rate value.
  */
  void setInspectionRate(const OdString& label);

/**DOM-IGNORE-BEGIN*/
#define VAR_DEF(type, name, dxf, def_value, metric_def_value, reserve1, reserve2)\
  /** Description: Returns the DIM##NAME value of this Dimension entity.  \
    Remarks: ##remarks \
    \
    Remarks: \
    The value from the dimension style of this Dimension entity will be returned unless \
    that value has been overridden, in which case, the override value will be returned. \
    \
    Notes: ##notes \
    See Also: ##see_also \
  */ \
  virtual type dim##name() const; \
\
  /** Description: Sets the DIM##NAME override for this Dimension entity. \
    Arguments: val (I) New value for DIM##NAME.  ##RANGE \
    Remarks: ##remarks \
    Notes: ##notes \
    See Also: ##see_also \
  */ \
virtual void setDim##name(type val);
/**DOM-IGNORE-END*/

#include "DimVarDefs.h"
#undef  VAR_DEF

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  OdResult subGetClassID(void* pClsid) const;

  OdUInt32 subSetAttributes(
    OdGiDrawableTraits* pTraits) const;

  bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  void subViewportDraw(
    OdGiViewportDraw* pVd) const;

  /* Replace OdRxObjectPtrArray */
  
  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const ODRX_OVERRIDE; 

  void subClose();

  virtual void modified (
    const OdDbObject* pObject);

  /** \details
    Applies the specified 3D transformation matrix to this entity. Only available if transformation is uniform.

    \param xfm [in]  3D transformation matrix.

    \returns
      Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm) ODRX_OVERRIDE;
  
  /** \details
    Applies a transformation matrix to a copy of this entity. Only available if transformation is uniform.

    \param xfm   [in] Transformation matrix to be applied.
    \param pCopy [out] Copy of this entity with a transformation matrix applied to it.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  virtual OdResult subGetTransformedCopy(
    const OdGeMatrix3d& xfm, 
    OdDbEntityPtr& pCopy) const ODRX_OVERRIDE;

  /** \details
    Applies the default properties of the specified database to this entity.
    \param pDb [in]  Pointer to the database whose default values are to be used.
    \param doSubents [in]  Set to true if subentities should be processed

    \remarks
      If pDb is null, the database containing this entity is used.

    The following properties are set
    * color
    * layer
    * linetype
    * linetype scale
    * lineweight
    * plotstyle
    * visibility
  */
  void subSetDatabaseDefaults(
    OdDbDatabase *pDb,
    bool doSubents);

  virtual void appendToOwner(
    OdDbIdPair& idPair, 
    OdDbObject* pOwnerObject, 
    OdDbIdMapping& ownerIdMap) ODRX_OVERRIDE;

  virtual OdDbObjectPtr subWblockClone(OdDbIdMapping& ownerIdMap, OdDbObject*, bool bPrimary) const ODRX_OVERRIDE;

  /** \details
    Returns the formatted measurement value.
    
    \param formattedMeasurement [out]  Receives the formatted measurement.
    \param measurementValue [in]  Measurement value to be formatted.
    \param dimensionText [in]  Dimension text for formatting
    
    \remarks
    measurementValue is formatted per the specified dimension text and the 
    effective dimension style.
  */
  void formatMeasurement(
    OdString& formattedMeasurement, 
    double measurementValue, 
    const OdString& dimensionText);

  /** \details
    Explodes this entity into a set of simpler entities.

    \param entitySet [out] Array of simpler entities.
    \returns eOk if successful, or an appropriate error code otherwise.
  */
  OdResult explodeGeometry(OdRxObjectPtrArray& entitySet) const;

  /** \details
    Returns opened object that is returned by dimBlockId().

    \param openMode [in] Mode in which to open the object.
  */
  OdDbObjectPtr dimBlock(OdDb::OpenMode openMode = OdDb::kForRead);

  /*
      void generateLayout();
      void getOsnapPoints(OdDb::OsnapMode osnapMode, const OdDbFullSubentPath& subentId,
                          const OdGePoint3d& pickPoint, const OdGePoint3d& lastPoint,
                          const OdGeMatrix3d& viewXform, OdGePoint3dArray& snapPoints,
                          OdDbIntArray& geomIds ) const;
  */

  virtual OdResult subGetCompoundObjectTransform(OdGeMatrix3d & xM) const ODRX_OVERRIDE;

  // Dynamic dimensions support
  // (The methods are NOT related to constraint support)
  
  /** \details
    Checks if this dimension object is dynamic.

    \returns true if this dimension object is dynamic, false - otherwise.
  */
  bool isDynamicDimension() const;

  /** \details
    Sets a flag that indicates whether this dimension is dynamic or not.
    
    \param bDynamic [in] Flag value.
  */
  void setDynamicDimension(
    bool bDynamic);

  // Dimensional constraint support

  /** \details
    Checks if the object is constrained.

    \returns true if the object is constrained, false - otherwise.
  */
  virtual bool isConstraintObject() const;

  /** \details
    Not implemented yet.
  */
  OdResult isConstraintObject(
    bool& isConstraintObject,
    bool& hasExpression,
    bool& isReferenceConstraint) const;

  /** \details
    Checks if the object's constraint is dynamic.

    \returns true if the object's contstraint is dynamic, false - otherwise.
  */
  bool  isConstraintDynamic() const;

  /** \details
    Sets a flag that indicates whether the constraint is dynamic or not.

    \param bDynamic [in] New flag value.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  OdResult setConstraintDynamic(
    bool bDynamic);

  /** \details
    Checks if this dimension should participate in OPM.
    
    \returns true if this dimension should participate in OPM, false - otherwise.
  */
  bool  shouldParticipateInOPM() const;

  /** \details
    Sets a flag that indicates whether this dimension should participate in OPM or not.

    \param bShouldParticipate [in] New flag value.
  */
  void  setShouldParticipateInOPM(
    bool bShouldParticipate);
  
  /** \details
    Sets DIMTALN flag value.

    \param val [in] New flag value.
    \remarks
      Tolerance divided flag. Value true means '~' and false means '^'
  */
  void setDIMTALN(bool val);

  /** \details
    Checks the value of the DIMTALN flag.

    \returns true if the DIMTALN flag is set to true, false - otherwise.
    \remarks
      Tolerance divided flag. Value true means '~' and false means '^'
  */
  bool getDIMTALN() const;

  /** \details
    Removes text field from this dimension object and deletes it from dictionary.
  */
  void removeTextField();

  /** \details
    If this object has a text field, this method copies the field and pastes the copy to pDimMText.

    \param pDimMText [in] OdDbMText object, to which the text will be copied.
  */
  void fieldToMText(OdDbMTextPtr& pDimMText);

  /** \details
    If pDimMText has a text field, this method copies that field and adds the copy to this object. 

    \param pDimMText [in] OdDbMText object, from which the text will be copied.
  */
  void fieldFromMText(OdDbMTextPtr& pDimMText);

  /** \details
    Returns the matrix which maps the plane coordinate system to the WCS.
  */
  OdGeMatrix3d getEcs() const;
    
  /** \details
    Resets defined width and defined height for MText.
  */
  void resetTextDefinedSize();

  /** \details
    Sets defined width and defined height for MText

    \param width [in]  MText width
    \param height [in]  MText height
  */
  void setTextDefinedSize(
    double width,
    double height);

  /** \details
    Gets defined width and defined height for MText

    \param width [out]  MText width
    \param height [out]  MText height
  */
  void textDefinedSize(
    double& width,
    double& height) const;

};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbDimension object pointers.
*/
typedef OdSmartPtr<OdDbDimension> OdDbDimensionPtr;

class OdDbDimensionObjectContextData;

  /** \details
    This class is the Dimension Recompute Protocol Extension class.
   
    
    <group OdDb_Classes>
  */
class DBDIM_EXPORT OdDbDimensionRecomputePE : public OdRxObject
{
public:
  /** \details
    Sets the measurement value for the specified Dimension entity.
    
    \param pDimension [in]  Pointer to the dimension entity.
    \param measurementValue [in]  Measurement value.
  */
  static void setMeasurementValue(OdDbDimension* pDimension, double measurementValue);
  /** \details
    Resets the dimension block insertion parameters for the specified Dimension entity.
    
    \param pDimension [in]  Pointer to the dimension entity.

    \remarks
    * The insertion point is set to (0,0).
    * The X-, Y- and Z-scale factors are set 1.0.
    * The rotation angle is set to 0.0. 
  */
  static void resetDimBlockInsertParams(OdDbDimension* pDimension);

  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbDimensionRecomputePE);
  //DOM-IGNORE-END

  /** \details
    Recomputes the dimension measurement for the specified Dimension entity.
    
    \param pDimension [in]  Pointer to the dimension entity.
  */
  virtual void recomputeDimMeasurement(OdDbDimension* pDimension) = 0;

  /** \details
    Recomputes the dimension block referenced by the specified Dimension entity.
    
    \remarks
    The OdDbBlockTableRecord of the dimension block is updated to reflect any changes made to this Dimension
    entity since the last time the block table record was updated.

    \param pDimension [in]  Pointer to the dimension entity.
  */
  virtual void recomputeDimBlock(OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx) = 0;


  /** \details
    Returns the formatted measurement value.
    
    \param pDimension [in]  Pointer to the dimension entity.
    \param formattedMeasurement [out]  Receives the formatted measurement.
    \param measurementValue [in]  Measurement value to be formatted.
    \param dimensionText [in]  Dimension text for formatting
    
    \remarks
    measurementValue is formatted per the specified dimension text and the 
    effective dimension style.
  */
  virtual void formatMeasurement(OdDbDimension* pDimension, OdString& formattedMeasurement, 
     double measurementValue, const OdString& dimensionText ) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbDimensionRecomputePE object pointers.
*/
typedef OdSmartPtr<OdDbDimensionRecomputePE> OdDbDimensionRecomputePEPtr;

#include "TD_PackPop.h"

#endif

