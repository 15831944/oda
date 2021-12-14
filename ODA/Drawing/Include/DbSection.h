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


#ifndef _OdDbSection_h_Included_
#define _OdDbSection_h_Included_

#include "DbEntity.h"
#include "SmartPtr.h"
#include "DbHatch.h"
#include "TD_PackPush.h"
#include "ModelerGeometry.h"

class OdDbSectionSettings;
typedef OdSmartPtr<OdDbSectionSettings> OdDbSectionSettingsPtr;

class OdDbSection;
typedef OdSmartPtr<OdDbSection> OdDbSectionPtr;

/** \details
    This class represents a section plane.
    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbSection : public OdDbEntity
{
public:
  /** \details
    Defines the section plane type.
  */
  enum State
  {
    /** Section plane is unbounded. */
    kPlane              = 0x01,

    /** Section plane is bounded from front, sides and back, but has no top or bottom bounds. */
    kBoundary           = 0x02,

    /** Section plane is bounde from front, sides, back, top and bottom. */
    kVolume             = 0x04
  };

  /** \details
    Defines subitems for section plane.
  */
  enum SubItem
  {
    /** No subitem. */
    kNone               = 0x00,

    /** Line subitem. */
    kSectionLine        = 0x01,

    /** Line top subitem. */
    kSectionLineTop     = 0x02,

    /** Line bottom subitem. */
    kSectionLineBottom  = 0x04,

    /** Back line subitem. */
    kBackLine           = 0x08,

    /** Back line top subitem. */
    kBackLineTop        = 0x10,

    /** Back line bottom  subitem. */
    kBackLineBottom     = 0x20,

    /** Vertical line top subitem. */
    kVerticalLineTop    = 0x40,

    /** Vertical line bottom subitem. */
    kVerticalLineBottom = 0x80
  };

  /** \details
    Defines height states for section plane.
  */
  enum Height
  {
    /** Height of the section plane above the section line.*/
    kHeightAboveSectionLine = 0x1,
    
    /** Height of the section plane below the section line.*/
    kHeightBelowSectionLine = 0x2
  };

public:
  //DOM-IGNORE-BEGIN
  ODDB_DECLARE_MEMBERS(OdDbSection);
  //DOM-IGNORE-END

  /** \details
    Constructor. Sets field values to defaults.
  */
  OdDbSection();

  /** \details
    Creates a section plane from passed parameters.

    \param pts         [in] Vertices of the section line.
    \param verticalDir [in] Vertical direction vector, normal vector to the section line.
  */
  static OdDbSectionPtr createObject(const OdGePoint3dArray& pts,
    const OdGeVector3d& verticalDir);

  /** \details
    Creates a section plane from passed parameters.

    \param pts           [in] Vertices of the section line.
    \param verticalDir   [in] Vertical direction vector, normal vector to the section line.
    \param vecViewingDir [in] Viewing direction vector.
  */
  static OdDbSectionPtr createObject(const OdGePoint3dArray& pts,
    const OdGeVector3d& verticalDir, const OdGeVector3d& vecViewingDir);

  /** \details
    Retruns the section plane type as enum value.

    \remarks
      Possible types are:
        <table>
          kPlane    - 0x01
          kBoundary - 0x02
          kVolume   - 0x04
        </table>
  */
  OdDbSection::State state() const;

  /** \details
    Sets a section plane type from an enum value.

    \param nState [in] Section plane type.
    \remarks
      Possible types are:
        <table>
          kPlane    - 0x01
          kBoundary - 0x02
          kVolume   - 0x04
        </table>
  */
  void setState(OdDbSection::State nState);

  /** \details
    Returns the name of this section plane.
  */
  const OdString getName() const;

  /** \details
    Returns the name of this section plane.

    \param sName [out] Name of this section plane.
  */
  void getName(OdString& sName) const;

  /** \details
    Sets name for this section plane.

    \param pszName [out] New name for this section plane.
  */
  void setName(const OdString& pszName);

  /** \details
    Returns the viewing direction vector of this section plane.
  */
  OdGeVector3d viewingDirection () const;

  /** \details
    Sets the viewing direction vector of this section plane.

    \param dir [in] New viewing direction vector.
  */
  OdResult setViewingDirection(const OdGeVector3d& dir);

  /** \details
    Returns the vertical direction vector of this section plane.
  */
  OdGeVector3d verticalDirection() const;

  /** \details
    Sets the vertical direction vector of this section plane.

    \param dir [in] New normal vector.
  */
  OdResult setVerticalDirection(const OdGeVector3d& dir);

  /** \details
    Returns the normal vector of this section plane.
  */
  OdGeVector3d normal() const;
  
  /** \details
    Returns vectors that define the section plane.

    \param uAxis [out] Vector on the section line that starts from the first vertex. 
    \param vAxis [out] Vector on the section plane, perpendicular to the section line.
  */
  OdResult plane(OdGeVector3d& uAxis, OdGeVector3d& vAxis) const;

  /** \details
    Returns the indicator transparency value of this section plane (1 to 100).
  */
  int indicatorTransparency() const;

  /** \details
    Sets new indicator transparency value of this section plane.

    \param nTransparency [in] Transparency value (1 to 100).
  */
  void setIndicatorTransparency(int nTransparency);

  /** \details
    Retruns the indicator fill color of this section plane.
  */
  OdCmColor indicatorFillColor  () const;

  /** \details
    Sets the indicator fill color of this section plane.

    \param color [in] New fill color.
  */
  void setIndicatorFillColor(const OdCmColor& color);

  /** \details
    Returns the number of vertices on the section line.
  */
  int numVertices() const;

  /** \details
    Returns a section line vertex by its index.

    \param nIndex [in]  Index of a vertex.
    \param pt     [out] Vertex with a given index.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  OdResult getVertex(int nIndex, 
    OdGePoint3d& pt) const;

  /** \details
    Sets a section line vertex with an index.

    \param nIndex [in] Index of a vertex.
    \param pt     [in] Point that will be set as a vertex.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  OdResult setVertex(int nIndex, 
    const OdGePoint3d& pt);

  /** \details
    Adds a vertex to the section line.

    \param nInserAt [in] Index that vertex will be set at.
    \param pt       [in] Point that will be set as a vertex.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  OdResult addVertex(int nInsertAt, 
    const OdGePoint3d& pt);

  /** \details
    Removes a vertex at a given vertex.

    \param nIndex [in] Index of a vertex to be removed.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  OdResult removeVertex(int nIndex);
  
  /** \details
    Fecthes an array of vertices of the section line.

    \param pts [out] Array of vertices.
  */
  void  getVertices(OdGePoint3dArray& pts) const;

  /** \details
    Sets vertices of the section line from an array.

    \param pts [in] Array of vertices.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  OdResult setVertices(const OdGePoint3dArray& pts);

  /** \details
    Calculates desired height.

    \param nHeightType [in] Height type to be calculated.
    \remarks 
      HeighType can have the following values:
      <table>
      kHeightAboveSectionLine = 0x1
      kHeightBelowSectionLine = 0x2
      </table>
  */
  double height(OdDbSection::Height nHeightType) const;

  /** \details
    Sets value for the desired height.

    \param nHeightType [in] Height type.
    \param fHeight [in] Height value.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
    \remarks 
      HeighType can have the following values:
      <table>
      kHeightAboveSectionLine = 0x1
      kHeightBelowSectionLine = 0x2
      </table>
  */
  OdResult setHeight(OdDbSection::Height nHeightType, double fHeight);

  //bool  hitTest(const OdGePoint3d& ptHit, int* pSegmentIndex, OdGePoint3d* ptOnSegment, OdDbSection::SubItem* pSubItem) const;

  /** \details
    Creates a jog at the specified point.

    \param ptOnSection [in] Point on the section line.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  OdResult createJog   (const OdGePoint3d& ptOnSection);

  /** \details
    Checks if there are any jogs on this section.
  */
  bool hasJogs() const;

  /** \details
    Returns settings of this section.
  */
  OdDbObjectId getSettings() const;

  /** \deails
    Returns settings of this section in specified mode.

    \param mode [in] Mode to open the settings in.
    \remarks
      Possible modes:
      <table>
      kNotOpen   = -1,
      kForRead   = 0,  
      kForWrite  = 1,  
      kForNotify = 2
      </table>
  */
  OdDbSectionSettingsPtr getSettings(OdDb::OpenMode mode) const;

  /** \details
    Checks if live sectioning is enabled.

    \returns true if live sectioning is enabled.
  */
  bool isLiveSectionEnabled() const;

  /** \details
    Enables or disables live sectioning, depending on parameter.

    \param bEnable [in] Flag, determines whether to enable live sectioning.
  */
  void enableLiveSection(bool bEnable);

  /** \details
    Checks if an objects is a slice.

    \returns true if an object is of slice type.
  */
  bool isSlice() const;

  /** \details
    Sets a flag whether an object is a slice.

    \param bIsSlice [in] Flag value.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  OdResult setIsSlice( bool bIsSlice );

  /** \details
    Returns section thickness depth for a slice, returns 0.0 for other types.
  */
  double thicknessDepth() const;
  
  /** \details
    Sets thickness depth.

    \param depth [in] New depth value.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  OdResult setThicknessDepth( double depth );

  /** \details
    Returns the distance of the section plane offset.
  */
  double sectionPlaneOffset() const;
  
  /** \details
    Sets the distance of the section plane offset.

    \param offset [in] New distance.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  OdResult setSectionPlaneOffset( double offset );

public:
  // Base class overrides

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual void subViewportDraw(OdGiViewportDraw* pVd) const;

  virtual OdResult subGetClassID(void* pClsid) const;
  
  virtual void subClose();
  
  /** \details
    Applies a transformation matrix to this XData object. Only available if transformation is uniform.

    \param xform [in] Transformation matrix to be applied.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  virtual OdResult subTransformBy(const OdGeMatrix3d& xform);

  /** \details
    Applies a transformation matrix to a copy of this XData obejct. Only available if transformation is uniform.

    \param mat   [in] Transformation matrix to be applied.
    \param pCopy [out] Copy of this entity with a transformation matrix applied to it.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  virtual OdResult subGetTransformedCopy(const OdGeMatrix3d& mat, OdDbEntityPtr& pCopy) const;

  //virtual OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  /** \details
    Generates section geometry.

    \param pEnt              [in] Sectionable entity.
    \param intBoundaryEnts   [out] Intersection boundary entities.
    \param intFillEnts       [out] Intersection fill annotation entities.
    \param backgroundEnts    [out] Background entities.
    \param foregroundEnts    [out] Foreground entities.
    \param curveTangencyEnts [out] Curve tangency entities.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  OdResult generateSectionGeometry (OdDbEntity*             pEnt,
                                    OdArray<OdDbEntityPtr>& intBoundaryEnts,
                                    OdArray<OdDbEntityPtr>& intFillEnts,
                                    OdArray<OdDbEntityPtr>& backgroundEnts,
                                    OdArray<OdDbEntityPtr>& foregroundEnts,
                                    OdArray<OdDbEntityPtr>& curveTangencyEnts) const;

  // INTERNAL USE ONLY
  //DOM-IGNORE-BEGIN
  OdResult generateSectionGeometry (OdDbEntity*             pEnt,
                                    OdArray<OdDbEntityPtr>& intBoundaryEnts,
                                    OdArray<OdDbEntityPtr>& intFillEnts,
                                    OdArray<OdDbEntityPtr>& backgroundEnts,
                                    OdArray<OdDbEntityPtr>& foregroundEnts,
                                    OdArray<OdDbEntityPtr>& curveTangencyEnts,
                                    bool                    bIsSetProperties) const;
  //DOM-IGNORE-END

  /** \details
    Returns height of the section plane above the section line.
  */
  double topPlane() const;
  
  /** \details
    Sets height of the section plane above the section line.

    \param double [in] New height value.
  */
  OdResult  setTopPlane(double);

  /** \details
    Returns height of the section plane below the section line.
  */
  double bottomPlane() const;

  /** \details
    Sets height of the section plane below the section line.

    \param double [in] New height value.
  */
  OdResult setBottomPlane(double);
  
  /** \details
    Returns the z-coordinate elevation.
  */
  double elevation() const;

  /** \details
    Sets the z-coordinate elevation.

    \param double [in] New elevation value.
  */
  OdResult setElevation( double);
};


//*************************************************************************
// OdDbSectionSettings
//*************************************************************************

/** \details
    This class represent section plane settings.
    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbSectionSettings : public OdDbObject
{
public:
  /** \details
    Defines section types for section generation.
  */
  enum SectionType
  {
    /** Live section. */
    kLiveSection                = 0x1,
    
    /** 2-dimensional section.*/
    k2dSection                  = 0x2,
    
    /** 3-dimensional section.*/
    k3dSection                  = 0x4 
  };

  /** \details
    Defines geometry properties for section generation.
  */
  enum Geometry
  {
    /** Intersection boundary properties. */
    kIntersectionBoundary       = 0x1,

    /** Intersection fill properties. */
    kIntersectionFill           = 0x2,

    /** Background geometry properties. */
    kBackgroundGeometry         = 0x4,

    /** Foreground geometry properties. */
    kForegroundGeometry         = 0x8,

    /** Curve tangency lines properties. */
    kCurveTangencyLines         = 0x10 
  };

  /** \details
    Generation properties for section generation.
  */
  enum Generation
  {
    /** Section planes cuts through all objects. */
    kSourceAllObjects           = 0x1,
    
    /** Section plane cuts only through selected objects. */
    kSourceSelectedObjects      = 0x2,

    /** Section will be created as a new block. */
    kDestinationNewBlock        = 0x10,
    
    /** Section will replace an existing block. */
    kDestinationReplaceBlock    = 0x20,
    
    /** Section will be saved in a file. */
    kDestinationFile            = 0x40
  };

public:
  //DOM-IGNORE-BEGIN
  ODDB_DECLARE_MEMBERS(OdDbSectionSettings);
  //DOM-IGNORE-END

  /** \details
    Constructor. Sets default values to fields.
  */
  OdDbSectionSettings();

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;

  /** \details
    Resets all settings for all section types to default values.
  */
  void reset();
  
  /** \details
    Resets all settings for a specific section type to default values.

    \param nSecType [in] Section type for which settings will be reset.
     \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
  */
  void reset(OdDbSectionSettings::SectionType nSecType);

  /** \details
    Returns an enum value stating the current section type.

    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
  */
  OdDbSectionSettings::SectionType currentSectionType() const;

  /** \details
    Sets an enum value stating the current section type.

    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
  */
  void setCurrentSectionType(OdDbSectionSettings::SectionType nSecType);

  /** \details
    Returns an enum value stating the current generation options for specified section type.

    \param nSecType [in] Section type for which generation options will be returned.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible generation options are:
      <table>
        kSourceAllObjects        - 0x1,
        kSourceSelectedObjects   - 0x2,
        kDestinationNewBlock     - 0x10,   
        kDestinationReplaceBlock - 0x20,
        kDestinationFile         - 0x40
      </table>
  */
  OdDbSectionSettings::Generation generationOptions(OdDbSectionSettings::SectionType nSecType) const;

  /** \details
    Sets an enum value stating the current generation options.

    \param nSecType [in] Section type for which generation options will be set.
    \param nOptions [in] Generation options.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible generation options are:
      <table>
        kSourceAllObjects        - 0x1,
        kSourceSelectedObjects   - 0x2,
        kDestinationNewBlock     - 0x10,
        kDestinationReplaceBlock - 0x20,
        kDestinationFile         - 0x40
      </table>
  */
  void setGenerationOptions(OdDbSectionSettings::SectionType nSecType,
    OdDbSectionSettings::Generation nOptions);

  /** \details
    Returns an array of source objects for kSourceSelectedObjects section type.

    \param nSecType [in] Section type.
    \param ids      [out] Array of source objects.
  */
  void getSourceObjects(OdDbSectionSettings::SectionType nSecType,
    OdDbObjectIdArray& ids) const;

  /** \details
    Sets an array of source objects for kSourceSelectedObjects section type.

    \param nSecType [in] Section type.
    \param ids      [in] Array of source objects.
  */
  void setSourceObjects(OdDbSectionSettings::SectionType nSecType,
    const OdDbObjectIdArray& ids);

  /** \details
    Returns ID of a block that will be replaced after section, if 
    generation option is set to kDestinationReplaceBlock.

    \param nSecType [in] Section type enum value.
  */
  OdDbObjectId destinationBlock(OdDbSectionSettings::SectionType nSecType) const;

  /** \details
    Sets ID of a block that will be replaced after section, 
    if the generation option is set to kDestinationReplaceBlock.

    \param nSecType [in] Section type enum value.
    \param id       [in] ID of a block that will be replaced.
  */
  void setDestinationBlock(OdDbSectionSettings::SectionType nSecType,
    const OdDbObjectId& id);
  
  /** \details
    Returns the name of the file to which the generated section will be saved,
    if the generation option is set to kDestinationFile.

    \param nSecType [in] Section type enum value.
  */
  const OdString destinationFile(OdDbSectionSettings::SectionType nSecType) const;
  
  /** \details
    Sets the name of the file to which the generated section will be saved,
    if the generation option is set to kDestinationFile.

    \param nSecType    [in] Section type enum value.
    \param pszFileName [in] File name to be set.
  */
  void setDestinationFile(OdDbSectionSettings::SectionType nSecType,
    const OdString& pszFileName);

  /** \details
    Checks if the specified geometry is visible.

    \param nSecType  [in] Section type enum value.
    \param nGeometry [in] Geometry to check.
    \returns true if the geometry is visible, false - otherwise.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  bool  visibility(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  
  /** \details
    Sets a visibility flag of the specified geometry.

    \param nSecType  [in] Section type enum value.
    \param nGeometry [in] Geometry, which visibility will be set.
    \param bVisible  [in] Flag value.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setVisibility(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    bool bVisible);

  /** \details
    Returns color of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which color will be returned.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  OdCmColor color(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets color of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which color will be changed.
    \param color     [in] Color to be set.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setColor(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, const OdCmColor& color);

  /** \details
    Returns the name of the layer of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which layer name will be returned.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  OdString layer(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets the name of the layer of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which layer name will be set.
    \param pszLayer  [in] Name of the layer.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setLayer(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    const OdString& pszLayer);

  /** \details
    Returns the linetype of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which linetype will be returned.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  OdString linetype(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets the linetype of the specified geometry.

    \param nSecType    [in] Section type.
    \param nGeometry   [in] Geometry, which linetype will be set.
    \param pszLinetype [in] Linetype to set.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setLinetype (OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    const OdString& pszLinetype);

  /** \details
    Returns the linetype scale of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which linetype scale will be returned.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  double linetypeScale(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets the linetype scale of the specified geometry.

    \param nSecType    [in] Section type.
    \param nGeometry   [in] Geometry, which linetype scale will be set.
    \param fScale      [in] Linetype scale value to set.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setLinetypeScale(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    double fScale);

  /** \details
    Returns the plot style name of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which plot style name will be returned.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  OdString plotStyleName(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets the plot style name of the specified geometry.

    \param nSecType         [in] Section type.
    \param nGeometry        [in] Geometry, which plot style name will be set.
    \param pszPlotStyleName [in] Plot style name to set.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setPlotStyleName(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    const OdString& pszPlotStyleName);

  /** \details
    Returns the line weight of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which line weight will be returned.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  OdDb::LineWeight lineWeight(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets the line weight of the specified geometry.

    \param nSecType    [in] Section type.
    \param nGeometry   [in] Geometry, which line weight will be set.
    \param nLineWeight [in] Line weight value to be set.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setLineWeight(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    OdDb::LineWeight nLineWeight);

  /** \details
    Returns face transparency value of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which face transparency value will be returned.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  int faceTransparency(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets face transparency value of the specified geometry.

    \param nSecType      [in] Section type.
    \param nGeometry     [in] Geometry, which face transparency value will be set.
    \param nTransparency [in] Face transparency value to be set.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setFaceTransparency(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    int nTransparency);

  /** \details
    Returns edge transparency value of the specified geometry.
    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which edge transparency value will be returned.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  int edgeTransparency(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets edge transparency value of the specified geometry.
    \param nSecType      [in] Section type.
    \param nGeometry     [in] Geometry, which face transparency value will be set.
    \param nTransparency [in] Edge transparency value to be set.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setEdgeTransparency(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    int nTransparency);

  /** \details
    Checks if the hatch is visible in the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which hatch visibility will be checked.
    \returns true if the hatch is visible, false - otherwise.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  bool  hatchVisibility(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets the hatch visibility flag for the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which hatch visibility will be set.
    \param bVisible  [in] Visibility flag value.
    \returns true if the hatch is visible, false - otherwise.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setHatchVisibility(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    bool bVisible);
  
  /** \details
    Retrieves hatch pattern and its name from the specified geometry.

    \param nSecType       [in] Section type.
    \param nGeometry      [in] Geometry, from which hatch pattern is retrieved.
    \param nPatternType   [out] Hatch pattern type.
    \param pszPatternName [out] Hatch pattern name.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
      Possible hatch pattern types are:
      <table>
        kUserDefined     - 0,  User-defined hatch.
        kPreDefined      - 1,  Defined in acad.pat and acadiso.pat.
        kCustomDefined   - 2,  In its own PAT file.
      </table>
  */
  void getHatchPattern(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    OdDbHatch::HatchPatternType& nPatternType, 
    OdString& pszPatternName) const;

  /** \details
    Sets hatch pattern and its name to the specified geometry.

    \param nSecType       [in] Section type.
    \param nGeometry      [in] Geometry, which hatch pattern will be set.
    \param nPatternType   [in] Hatch pattern type.
    \param pszPatternName [in] Hatch pattern name.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
      Possible hatch pattern types are:
      <table>
        kUserDefined     - 0,  User-defined hatch.
        kPreDefined      - 1,  Defined in acad.pat and acadiso.pat.
        kCustomDefined   - 2,  In its own PAT file.
      </table>
  */
  void setHatchPattern(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    OdDbHatch::HatchPatternType nPatternType, 
    const OdString& pszPatternName);

  /** \details
    Returns the hatch angle value of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which hatch angle value will be returned.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  double hatchAngle(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets the hatch angle value of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which hatch angle value will be set.
    \param fAngle    [in] Hatch angle value to be set.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setHatchAngle(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    double fAngle);

  /** \details
    Returns the hatch spacing value of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which hatch spacing value will be returned.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  double hatchSpacing(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets the hatch spacing value of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which hatch spacing value will be set.
    \param fAngle    [in] Hatch spacing value to be set.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setHatchSpacing(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    double fSpacing);

  /** \details
    Returns the hatch scale value of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which hatch scale value will be returned.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  double hatchScale(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets the hatch scale value of the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, which hatch scale value will be set.
    \param fAngle    [in] Hatch scale value to be set.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setHatchScale(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    double fScale);

  /** \details
    Checks if hidden lines are visible for the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, for which the hidden lines visibility will be checked.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  bool  hiddenLine(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets the hidden lines visibility flag for this geometry.

    \param nSecType    [in] Section type.
    \param nGeometry   [in] Geometry, for which the hidden lines visibility will be set.
    \param bHiddenLine [in] New hidden line visibility flag value.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setHiddenLine(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    bool bHiddenLine);

  /** \details
    Checks if division lines are visible for the specified geometry.

    \param nSecType  [in] Section type.
    \param nGeometry [in] Geometry, for which the division lines visibility will be checked.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  bool  divisionLines(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;

  /** \details
    Sets the division lines visibility flag for this geometry.

    \param nSecType    [in] Section type.
    \param nGeometry   [in] Geometry, for which the division lines visibility will be set.
    \param bShow       [in] New division lines visibility flag value.
    \remarks
      Possible section types are:
      <table>
        kLiveSection - 0x1,
        k2dSection   - 0x2,
        k3dSection   - 0x4
      </table>
      Possible geometries are:
      <table>
        kIntersectionBoundary - 0x1,
        kIntersectionFill     - 0x2,
        kBackgroundGeometry   - 0x4,
        kForegroundGeometry   - 0x8,
        kCurveTangencyLines   - 0x10
      </table>
  */
  void setDivisionLines(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    bool bShow);
};


#include "TD_PackPop.h"

#endif //_OdDbSection_h_Included_
