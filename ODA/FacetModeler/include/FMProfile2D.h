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

#ifndef __FMPROFILE2D_H__
#define __FMPROFILE2D_H__

#include "FMContour2D.h"

namespace FacetModeler
{

  /** \details
     Describes a 2D profile.
  */
  class FMGEOMETRY_API Profile2D
  {
  public:
    /** \details
       Default constructor.
    */
    Profile2D();

    /** \details
       Constructor. Creates a profile object with one specified contour.

       \param rSrcCont [in] 2D contour.
    */
    explicit Profile2D(const Contour2D & rSrcCont);

    /** \details
       Copy constructor.

       \param rSrc [in] Profile to copy from.
    */
    Profile2D(const Profile2D & rSrc);

    /** \details
       Copy operator.

       \param rSrc [in] Profile to copy from.
       \returns Reference to the resulting profile.
    */
    Profile2D& operator = (const Profile2D & rSrc);

    //DOM-IGNORE-BEGIN
    typedef       Contour2D   value_type;
    typedef const Contour2D & const_reference;
    typedef       Contour2D & reference;

    typedef const Contour2D * const_pointer;
    typedef       Contour2D * pointer;
    typedef       OdUInt32    size_type;

    typedef const Contour2D * const_iterator;
    typedef       Contour2D * iterator;

    size_type       size() const;

    bool            empty() const;

    size_type       capacity() const;

    size_type       max_size() const;

    const_reference operator [] (size_type iPos) const;
    reference       operator [] (size_type iPos);

    const_reference front() const;
    reference       front();

    const_reference back() const;
    reference       back();

    const_iterator  begin() const;
    iterator        begin();

    const_iterator  end() const;
    iterator        end();
    //DOM-IGNORE-END

    /** \details
       Clears the profile.
    */
    void clear();

    /** \details
       Resizes the profile.

       \param iSize [in] New size.
    */
    void resize(size_type iSize);

    //DOM-IGNORE-BEGIN
    void reserve(size_type iNewCapacity);

    Contour2D & push_back();
    Contour2D & push_back(ContourImplClass eCC);

    void push_back(const Contour2D & rVal);
    void push_back(const Profile2D & rProfile);

    void pop_back();

    void     erase(size_type iPos);
    iterator erase(iterator itWhere);
    iterator erase(iterator itFirst, iterator itLast);

    void     insert(size_type iPos, const Contour2D & rVal);
    iterator insert(iterator itWhere, const Contour2D & rVal);
    void     insert(iterator itWhere, const_iterator itFirst, const_iterator itLast);
    //DOM-IGNORE-END

    /** \details
       Swaps all data of this profile with the specified one.

       \param rAnother [in] Profile to swap with.
    */
    void swap(Profile2D& rAnother);

    /** \details
       Appends the extents of the profile to the specified extents structure.

       \param extExtents [in/out] Extents to add to.
       \returns eOk if the extents addition succeeded, or a corresponding error type otherwise.
    */
    Result addExtents(OdGeExtents2d& extExtents) const;

    /** \details
       Checks whether this profile contains at least one non-empty contour.

       \returns true if this profile contains at least one non-empty contour, or false otherwise.
    */
    bool hasNonEmptyContours() const;

    /** \details
       Reverses all contours.
    */
    void reverse();

    /** \details
       Transforms all contours.
    */
    Result transformBy(const OdGeMatrix2d& mMatrix,
      const DeviationParams& devDeviation = FMGeGbl::gDefDev);

    /** \details
       Removes redundant segments with zero length from all contours.

       \param gTol [in] Tolerance.
    */
    void deleteCoincident(const OdGeTol& gTol = FMGeGbl::gTol);

    /** \details
       Merges adjacent segments for all contours.

       \param iMergeFlags [in] Combination of AECGe::FilletType flags (emfMergeArcs, emfIgnoreMetadata, emfMergeOrigin).
       \param gTol        [in] Tolerance.
    */
    void mergeSegments(int iMergeFlags = 0, const OdGeTol & gTol = FMGeGbl::gTol);

    /** \details
       Explodes all contours.

       \param devDeviation [in] Resulting profile.
       \param uArcMetadata [in] (Optional) Metadata to set.
       \returns eOk if the exploding succeeded, or a corresponding error type otherwise.
    */
    Result explode(const DeviationParams& devDeviation = FMGeGbl::gDefDev,
      OdUInt32 uArcMetadata = 0);

    /** \details
       Explodes all contours.

       \param rDestProfile [out] Resulting profile.
       \param devDeviation  [in] Deviation parameters.
       \param uArcMetadata  [in] (Optional) Metadata to set.
       \returns eOk if the exploding succeeded, or a corresponding error type otherwise.
    */
    Result explodeTo(Profile2D& rDestProfile,
      const DeviationParams& devDeviation = FMGeGbl::gDefDev,
      OdUInt32 uArcMetadata = 0) const;

    /** \details
       Gets the perimeter of the profile.

       \returns The perimeter of the profile.
    */
    double perimeter() const;

    /** \details
       Gets the adjusted profile.

       \param dWidth       [in] Width of desired extents.
       \param dDepth       [in] Depth of desired extents.
       \param bAdjustWidth [in] Adjusts the width to extents.
       \param bAdjustDepth [in] Adjusts the depth to extents.
       \param bMirrorInX   [in] Mirror in X.
       \param bMirrorInY   [in] Mirror in Y.
       \param dRotation    [in] Rotation around Z axis.
       \param rResult     [out] Resulting profile.
    */
    void getAdjustedProfile(double dWidth, double dDepth,
      bool bAdjustWidth, bool bAdjustDepth,
      bool bMirrorInX, bool bMirrorInY,
      double dRotation, Profile2D& rResult) const;

    /** \details
       Adjusts the profile to a bounding box extents.

       \param extBoundingBox [in] Bounding box to fit in.
       \param devDeviation   [in] Deviation parameters.
    */
    void fit(const OdGeExtents2d& extBoundingBox,
      const DeviationParams& devDeviation = FMGeGbl::gDefDev);

    /** \details
       Adjusts the profile to a bounding box extents.

       \param extBoundingBox [in] Bounding box to fit in.
       \param bFitX          [in] true to fit in the X bounds of the box, or false to ignore.
       \param bFitY          [in] true to fit in the Y bounds of the box, or false to ignore.
       \param devDeviation   [in] Deviation parameters.
    */
    void fit(const OdGeExtents2d& extBoundingBox,
      bool bFitX, bool bFitY,
      const DeviationParams& devDeviation = FMGeGbl::gDefDev);

    /** \details
       Returns the profile that is shifted in the normal direction from this closed(!) contour.
       E.g., if dOffset is negative, a counter-clockwise circle is extended.
       If dOffset is positive and small enough, it is contracted, 
       and it becomes empty if dOffset > radius.

       \param dOffset     [in] Signed offset along the normal direction. Positive offset is towards the inside for CCW contours.
       \param rResult    [out] Resulting profile.
       \param eFilletType [in] Gap filling strategy for the segments sliding apart.
       \param gTol        [in] Tolerance.
       \returns Success status.
    */
    Result getOffsetProfile(double dOffset, Profile2D& rResult,
      FilletType eFilletType = eftExtend,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Gets all hole contours reversed.

       \param rHolesProfile [out] Resulting profile.
    */
    void getHoles(Profile2D& rHolesProfile) const;

    /** \details
       Gets all non-hole contours as a profile.

       \param rNoHolesProfile [out] Resulting profile.
    */
    void getWithoutHoles(Profile2D& rNoHolesProfile) const;

    /** \details
       Gets all profiles. Each of the returned profiles bounds a connected domain
       and contains one (the first one) big CCW contour and possibly few smaller CW "holes."

       \param aFaces [out] Array of resulting faces as std::vector<Profile2D>.
       \returns eOk if the operation succeeded, or a corresponding error type otherwise.

       \remarks
       This profile should be "regular" (no self-intersections, valid nesting of CCW/CW).
    */
    Result getFaces(std::vector<Profile2D>& aFaces) const;

    /** \details
       Splits the profile with a line.

       \param lCutter [in] Cut line.
       \param rRght  [out] Right resulting profile.
       \param rLeft  [out] Left resulting profile.

       \remarks
       The left side is the half-plane of the profile, which is located to the left of the straight line direction.
    */
    void split(const OdGeLine2d& lCutter,
      Profile2D& rRght,
      Profile2D& rLeft) const;

    /** \details
       Updates metadata for all segments in all contours.

       \param uNewData      [in] New value of metadata to set.
       \param uBitsToModify [in] Bit mask to set.
       \returns eOk if the update succeeded, or a corresponding error type otherwise.
    */
    Result setAllMetadata(OdUInt32 uNewData,
      OdUInt32 uBitsToModify = UINT_MAX);

    /** \details
       Performs a boolean operation on 2D profiles.

       \param eOperation [in] Operation type from BooleanOperation enum.
       \param rArg1      [in] First profile.
       \param rArg2      [in] Second profile.
       \param rResult   [out] Resulting profile.
       \param tol        [in] Tolerance.
       \returns true if the boolean operation succeeded, or false otherwise.

       \remarks
       Each argument is a set of points on the plane, not necessary convex or connected.
       All contours must be non-selfintersecting and closed.
       Segments in contours usually go counterclockwise, otherwise they are counted as holes in other contours.
       The method performs the operation on arguments and returns true if the operation is successful.
    */
    static bool PerformOperation(BooleanOperation eOperation,
      const Profile2D& rArg1, const Profile2D& rArg2,
      Profile2D& rResult, const OdGeTol& tol = FMGeGbl::gTol);

    /** \details
       Removes self-intersecting parts oriented clockwise
       and leaves counter-clockwise parts intact.

       \param rDest [out] Resulting contour.
       \param gTol   [in] Tolerance.
       \returns eOk if the contour regularization succeeded, or a corresponding error type otherwise.
    */
    Result regularizeTo(Profile2D& rDest,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Checks whether this profile contains only closed contours
       and coincides with a directional boundary of an open 2D region.

       \param gTol [in] Tolerance.
       \returns
       * false for empty and non-closed contours.
       * false if there are coincident bounds with opposite directions (e.g., O-O shape).
       * false if there are sub-contours with incorrect orientations (e.g., 8 shape).
       * true otherwise.
    */
    bool isValidRegion(const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Gets the signed area.

       \returns Signed area value.
    */
    double signedArea() const;

    /** \details
       Gets the single contour describing the profile with bridges between initial contours.

       \param rDest [out] Resulting contour.
       \param gTol   [in] Tolerance.
       \returns eOk if the contour calculation succeeded, or a corresponding error type otherwise.

       \remarks
       The contour is not self-intersecting unless the initial contours intersect.

       This profile should be "regular" (no self-intersections and valid nesting of
       CW/CCW contours).
    */
    Result getSingleContour(Contour2D& rDest,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       Checks whether the specified point lies inside the profile or on its border.

       \param ptTest     [in] Point to check.
       \param pOnBorder [out] (Optional) true if the point lies on the border of the contour.
       \param gTol       [in] Tolerance.
       \returns true if the point lies inside the profile or on its border, or false otherwise.

       \remarks
       Contours of the profile should be closed.
       The return value is equal to (iNumCCW - iNumCW > 0), where iNum(C)CW
       is the number of (C)CW contours containing the point.
       The default value (e.g., an empty profile) is true.
    */
    bool contains(const OdGePoint2d& ptTest, bool* pOnBorder = NULL,
      const OdGeTol& gTol = FMGeGbl::gTol) const;

    /** \details
       An empty static profile.
    */
    FMGEOMETRY_API_STATIC static const Profile2D kNull;

    /** \details 
      Destructor.
    */
    ~Profile2D();

  private:
    //DOM-IGNORE-BEGIN
    Contour2D* _insertItems(size_type iPos, size_type iCount = 1);

    void _uninsertItems(size_type iPos, size_type iCount = 1);
    //DOM-IGNORE-END

    OdUInt32    m_iSize;     // Current size.
    OdUInt32    m_iCapacity; // Reserved capacity.
    Contour2D * m_pContours; // Pointer to the data array.
  };

  //DOM-IGNORE-BEGIN
  Result FMGEOMETRY_API GetProfileWithOffsetSegment(const Profile2D& rSrc, Profile2D& rDest, OdUInt32 uContour, OdUInt32 uSegment, double dNormalOffset);
  //DOM-IGNORE-END
};

#endif  //__FMPROFILE2D_H__
