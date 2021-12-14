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

#ifndef _ExKWIndexData_h_Included_
#define _ExKWIndexData_h_Included_

#include "TD_PackPush.h"

#include "OdString.h"
#include "Ed/EdUserIO.h"
#include "OdArray.h"


/** \details
    This class implements keyword index data for ExDbCommandContext objects.

	<group ExServices_Classes> 
*/
class KWIndexData
{
public:
  /** \details
    Default constructor.
  */
  KWIndexData()
  {}
  OdString m_sKey;   // Key1
  OdString m_sKey2;  // Key2
  OdString m_sKword; // Keyword
  int m_nPKey;       // part Key1 start
  int m_nPKey2;      // part Key2 start

  /** \details
    \param n [in] maximum number of characters to return.
    \returns the leftmost n characters of Key1.
  */
  OdString key1(int n) const
  {
    return m_sKey.left(odmin(n,m_sKey.getLength()));
  }
  /** \details
    \param n [in] maximum number of characters to return.
    \returns the leftmost n characters of partial Key1.
  */
  OdString pKey1(int n) const
  {
    if(m_nPKey==0)
      return OdString::kEmpty;
    return m_sKey.mid(m_nPKey, odmin(n,m_sKey.getLength()-m_nPKey));
  }
  /** \details
    \param n [in] maximum number of characters to return.
    \returns the leftmost n characters of Key2.
  */
  OdString key2(int n) const
  {
    return m_sKey2.left(odmin(n,m_sKey2.getLength()));
  }
  /** \details
    \param [in] maximum number of characters to return.
    \returns the leftmost n characters of partial Key1.
  */
  OdString pKey2(int n) const
  {
    if(m_nPKey2==0)
      return OdString::kEmpty;
    return m_sKey2.mid(m_nPKey2, odmin(n,m_sKey2.getLength()-m_nPKey2));
  }
  /** \details
    \returns true if and only if the specified string
    matches the beginning of Key1 or it's part Key2 or it's part.
    Start of partial key is encoded by uppercase letter.
    \param str [in] string to test keyword matching.
    \remarks
    The comparison is case-insensitive. 
    Example:
    If (Key1 == "noRth") || (Key2 == "South") then
    match("NOR") == true and match("SOUTHWEST") == false;
    match("RT") == true and match("UT") == false.
  */
  bool match(const OdString& str) const
  {
    int len = str.getLength();
    return key1(len).iCompare(str)==0 || pKey1(len).iCompare(str)==0 ||
           key2(len).iCompare(str)==0 || pKey2(len).iCompare(str)==0;
  }
  /** \details
    \returns the keyword for this KWIndexData object.
  */
  OdString keyword() const
  {
    return m_sKword;
  }
};

/** \details
    This class implements a keyword index for ExDbCommandContext objects.

	<group ExServices_Classes>
*/
class KWIndex : public OdArray<KWIndexData> 
{
public:
  /** \details
    \param keywords [in]  Keyword list.
  */
  KWIndex(const OdString& keywords);
  /** \details
      \param input [in]  Input to be matched.
      \returns the index of the keyword matched by the specified input
      or -1 if no match was found.
      \remarks
  */
  int find(const OdString& input) const;
  /** \details
      Throws an exception if and only if the specified input matches a keyword.
      \param input [in]  Input to be checked.
      \remarks
      Throws:
      ::OdEdKeyword(keywordIndex, Keyword)
  */
  void check(const OdString& input) const;
};

#include "TD_PackPop.h"

#endif // _ExKWIndexData_h_Included_
