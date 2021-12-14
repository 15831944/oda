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
// GLES2 device special display codes processing helpers

#ifndef ODTRVISDISPLAYCODEPROC
#define ODTRVISDISPLAYCODEPROC

#include "TD_PackPush.h"

#include "TrVisRendition.h"

/** \details
    <group ExRender_Classes>
*/
class OdTrVisDisplayCodeKeeper
{
  public:
    template <typename TArray>
    static void pushZero(TArray &arry)
    {
      arry.push_back(OdTrVisRendition::kDisplayCodeZero);
    }
    // Highlighting
    template <typename TArray>
    static void pushHighlight(TArray &arry)
    {
      arry.push_back(OdTrVisRendition::kDisplayCodeHighlight);
    }
    template <typename TArray>
    static void pushUnhighlight(TArray &arry)
    {
      arry.push_back(OdTrVisRendition::kDisplayCodeUnhighlight);
    }
    template <typename TArray>
    static void pushHighlight(TArray &arry, bool bHighlight)
    {
      arry.push_back((bHighlight) ? OdTrVisRendition::kDisplayCodeHighlight : OdTrVisRendition::kDisplayCodeUnhighlight);
    }
    // Transform
    template <typename TArray>
    static void pushPushMatrix(TArray &arry, const OdGeMatrix3d &mtx)
    { typedef OD_TYPENAME TArray::value_type ValType;
      arry.push_back(OdTrVisRendition::kDisplayCodePushMatrix);
      const ValType *pMtxRepresentation = reinterpret_cast<const ValType*>(mtx.entry);
      arry.insert(arry.end(), pMtxRepresentation, pMtxRepresentation + (sizeof(OdGeMatrix3d) / sizeof(ValType)));
    }
    template <typename TArray>
    static void pushPopMatrix(TArray &arry)
    {
      arry.push_back(OdTrVisRendition::kDisplayCodePopMatrix);
    }
    // Fading
    template <typename TArray>
    static void pushFade(TArray &arry, long nFadeSlot)
    { typedef OD_TYPENAME TArray::value_type ValType;
      arry.push_back(OdTrVisRendition::kDisplayCodeFade);
      arry.push_back((ValType)(OdIntPtr)nFadeSlot);
    }
    template <typename TArray>
    static void pushUnfade(TArray &arry, long nFadeSlot)
    { typedef OD_TYPENAME TArray::value_type ValType;
      arry.push_back(OdTrVisRendition::kDisplayCodeUnfade);
      arry.push_back((ValType)(OdIntPtr)nFadeSlot);
    }
    template <typename TArray>
    static void pushFade(TArray &arry, long nFadeSlot, bool bFade)
    {
      if (bFade) pushFade<TArray>(arry, nFadeSlot);
      else pushUnfade<TArray>(arry, nFadeSlot);
    }
    // Markers
    template <typename TArray>
    static void pushMarkers(TArray &arry, const void *pBranch, OdTrVisStateBranchPropertyDef::BranchType brType)
    { typedef OD_TYPENAME TArray::value_type ValType;
      arry.push_back((!brType) ? OdTrVisRendition::kDisplayCodeHlMarkers : OdTrVisRendition::kDisplayCodeVsMarkers);
      arry.push_back((ValType)(OdIntPtr)pBranch);
    }
    template <typename TArray>
    static void pushHlMarkers(TArray &arry, const void *pHlBranch)
    {
      pushMarkers<TArray>(arry, pHlBranch, OdTrVisStateBranchPropertyDef::kHl);
    }
    /** \details
      Keeps visibilities markers
    */
    template <typename TArray>
    static void pushVsMarkers( TArray &arry, const void *pVsBranch )
    {
      pushMarkers<TArray>(arry, pVsBranch, OdTrVisStateBranchPropertyDef::kVs);
    }
    // Drawing order
    template <typename TArray>
    static void pushDrawOrder(TArray &arry, double drawOrder)
    { typedef OD_TYPENAME TArray::value_type ValType;
      arry.push_back(OdTrVisRendition::kDisplayCodeDrawOrder);
      arry.push_back(*reinterpret_cast<ValType*>(&drawOrder));
    }
    template <typename TArray>
    static void pushMultiOrder(TArray &arry)
    {
      pushDrawOrder<TArray>(arry, OdTrVisRendition::kDisplayMultiDrawOrderMarker);
    }
    // Exploded unions
    template <typename TArray>
    static void pushExploded(TArray &arry)
    {
      arry.push_back(OdTrVisRendition::kDisplayCodeExploded);
    }
    template <typename TArray>
    static void pushNonExploded(TArray &arry)
    {
      arry.push_back(OdTrVisRendition::kDisplayCodeNonExploded);
    }
    template <typename TArray>
    static void pushExploded(TArray &arry, bool bExploded)
    {
      arry.push_back((bExploded) ? OdTrVisRendition::kDisplayCodeExploded : OdTrVisRendition::kDisplayCodeNonExploded);
    }
    // Selection style
    template <typename TArray>
    static void pushSelStyle(TArray& arry, OdUInt32 nSelStyle)
    { typedef OD_TYPENAME TArray::value_type ValType;
      arry.push_back(OdTrVisRendition::kDisplayCodeSelStyle);
      arry.push_back((ValType)(OdIntPtr)nSelStyle);
    }
    /** \details
      Sets id of current rendering object
      \param arry [in] Display list
      \param pID [in] Id to add in display list
    */
    template <typename TArray>
    static void pushSelectionID(TArray &arry, const void *pID)
    { typedef OD_TYPENAME TArray::value_type ValType;
      arry.push_back(OdTrVisRendition::kDisplayCodeSelectionID);
      arry.push_back((ValType)(OdIntPtr)pID);
    }
};

/** \details
    <group ExRender_Classes>
*/
class OdTrVisFastComparator
{
  public:
    template <typename TObj>
    static bool isEqual(const TObj &obj1, const TObj &obj2)
    {
      return !::memcmp(&obj1, &obj2, sizeof(TObj));
    }
    static bool isIdentity(const OdGeMatrix3d &mtx)
    {
      return isEqual(mtx, OdGeMatrix3d::kIdentity);
    }
};

/** \details
    <group ExRender_Classes>
*/
class OdTrVisDisplayListIterator
{
  OdTrVisDisplayId *m_pList;
  OdUInt32 m_nListData, m_nCaret;
  public:
    OdTrVisDisplayListIterator(const OdTrVisDisplayId *pList, OdUInt32 nData)
      : m_pList(const_cast<OdTrVisDisplayId*>(pList)), m_nListData(nData), m_nCaret(0)
    {
    }
    bool isEOF() const { return m_nCaret >= m_nListData; }
    bool isCode() const { return m_pList[m_nCaret] < OdTrVisRendition::kDisplayCodeRange; }
    OdTrVisRendition::DisplayIdCodes code() const { return (OdTrVisRendition::DisplayIdCodes)m_pList[m_nCaret]; }
    static OdUInt32 skipCodeSize(OdTrVisRendition::DisplayIdCodes dispCode)
    { OdUInt32 nSteps = 1;
      switch (dispCode) { case OdTrVisRendition::kDisplayCodePushMatrix: nSteps += sizeof(OdGeMatrix3d) / sizeof(double); break;
                          case OdTrVisRendition::kDisplayCodeFade:      case OdTrVisRendition::kDisplayCodeUnfade:
                          case OdTrVisRendition::kDisplayCodeHlMarkers: case OdTrVisRendition::kDisplayCodeVsMarkers:
                          case OdTrVisRendition::kDisplayCodeSelStyle:  case OdTrVisRendition::kDisplayCodeSelectionID:
                          case OdTrVisRendition::kDisplayCodeDrawOrder : nSteps += 1; break; default: break; }
      return nSteps; }
    void skipCode() { m_nCaret += skipCodeSize(code()); }
    OdTrVisDisplayId &cur() { return m_pList[m_nCaret]; }
    const OdTrVisDisplayId &cur() const { return m_pList[m_nCaret]; }
    OdTrVisDisplayId &next(OdUInt32 n = 1) { return m_pList[m_nCaret + n]; }
    const OdTrVisDisplayId &next(OdUInt32 n = 1) const { return m_pList[m_nCaret + n]; }
    bool step() { if (isCode()) skipCode(); else m_nCaret++; return isEOF(); }
    // Access extensions
    const double *codeAsDoublePtr(OdUInt32 n = 1) const { return reinterpret_cast<const double*>(m_pList + n); }
    const OdGeMatrix3d *codeAsMatrixPtr(OdUInt32 n = 1) const { return reinterpret_cast<const OdGeMatrix3d*>(m_pList + n); }
    const OdTrVisId *codeAsIdPtr(OdUInt32 n = 1) const { return reinterpret_cast<const OdTrVisId*>(m_pList + n); }
    const void *codeAsPtr(OdUInt32 n = 1) const { return OdTrVisIdToPtr(const void, m_pList[n]); }
};

#include "TD_PackPop.h"

#endif // ODTRVISDISPLAYCODEPROC
