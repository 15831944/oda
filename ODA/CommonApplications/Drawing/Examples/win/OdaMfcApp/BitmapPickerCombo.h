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

#if !defined(AFX_CBITMAPPICKERCOMBO_H__8AAE34F7_7B02_11D3_A615_0060085FE616__INCLUDED_)
#define AFX_CBITMAPPICKERCOMBO_H__8AAE34F7_7B02_11D3_A615_0060085FE616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000 

//----------------------------------------------------------------------------
// @class         CBitmapPickerCombo |
//                combobox to select bitmaps from
// @base          public | CComboBox
//----------------------------------------------------------------------------
class CBitmapPickerCombo : public CComboBox
{
  // @access Public Member Functions and Variables
public:

  // @cmember
  // constructor
  CBitmapPickerCombo();

  // @cmember,mfunc
  // destructor
  virtual ~CBitmapPickerCombo() {}

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::AddBitmap
// @mfunc   adds bitmap (and string) item to combobox
// @rdesc   index of item (-1 on failure) (int)
// @parm    const CBitmap | *bitmap | bitmap to add
// @parm    const char    | *string | string to add (default NULL)
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
  int AddBitmap(const CBitmap *bitmap, LPCTSTR string = NULL);

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::InsertBitmap
// @mfunc   adds bitmap (and string) item to combobox at specified index
// @rdesc   index of item (-1 on failure) (int)
// @parm    int           | nIndex  | index at which to insert
// @parm    const CBitmap | *bitmap | bitmap to add
// @parm    const char    | *string | string to add (default NULL)
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
  int InsertBitmap(int nIndex, const CBitmap *bitmap, LPCTSTR string = NULL);

  //void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
  // @access Protected Member Functions and Variables
protected:

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::DrawItem
// @mfunc   Called by MFC when visual aspect of combo box changes 
// @rdesc   nothing
// @parm    LPDRAWITEMSTRUCT | lpDIS | standard parameter
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::MeasureItem
// @mfunc   Called by MFC when combo box is created
// @rdesc   nothing
// @parm    LPMEASUREITEMSTRUCT | lpMIS | standard parameter
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
  virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);

  // @cmember,mfunc
  // strings cannot be added
  virtual int AddString(LPCTSTR ) { return -1; }

  // @cmember,mfunc
  // strings cannot be added
  virtual int InsertString(int , LPCTSTR ) { return -1; }

  // @cmember,mfunc
  // strings cannot be deleted
 // virtual int DeleteString(int nIndex) { return -1; }

#ifdef _DEBUG
  // @cmember,mfunc
  // for assertion only
  virtual void PreSubclassWindow();
#endif

// @access Private Member Functions and Variables
private:

  int m_nItemWidth, m_nItemHeight; // @cmember size of items

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::OutputBitmap
// @mfunc   draws bitmap (and string) in item
// @rdesc   nothing
// @parm    LPDRAWITEMSTRUCT | lpDIS    | item data
// @parm    bool             | selected | is the item selected?
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
  void OutputBitmap(LPDRAWITEMSTRUCT lpDIS, bool selected);

//----------------------------------------------------------------------------
// Function CBitmapPickerCombo::SetSize
// @mfunc   sets overall item size
// @rdesc   nothing
// @parm    int | width  | width of item
// @parm    int | height | height of item
// @xref    <c CBitmapPickerCombo>
//----------------------------------------------------------------------------
  void SetSize(int width, int height);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBITMAPPICKERCOMBO_H__8AAE34F7_7B02_11D3_A615_0060085FE616__INCLUDED_)
