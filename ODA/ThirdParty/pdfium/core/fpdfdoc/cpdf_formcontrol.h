// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef CORE_FPDFDOC_CPDF_FORMCONTROL_H_
#define CORE_FPDFDOC_CPDF_FORMCONTROL_H_

#include "core/fpdfdoc/cpdf_aaction.h"
#include "core/fpdfdoc/cpdf_action.h"
#include "core/fpdfdoc/cpdf_annot.h"
#include "core/fpdfdoc/cpdf_annotlist.h"
#include "core/fpdfdoc/cpdf_apsettings.h"
#include "core/fpdfdoc/cpdf_defaultappearance.h"
#include "core/fpdfdoc/cpdf_formfield.h"
#include "core/fpdfdoc/cpdf_iconfit.h"
#include "core/fpdfdoc/ipdf_formnotify.h"
#include "core/fxcrt/fx_coordinates.h"
#include "core/fxcrt/fx_string.h"
#include "core/fxge/fx_dib.h"

#define TEXTPOS_CAPTION 0
#define TEXTPOS_ICON 1
#define TEXTPOS_BELOW 2
#define TEXTPOS_ABOVE 3
#define TEXTPOS_RIGHT 4
#define TEXTPOS_LEFT 5
#define TEXTPOS_OVERLAID 6

class CFX_RenderDevice;
class CPDF_Dictionary;
class CPDF_Font;
class CPDF_FormField;
class CPDF_InterForm;
class CPDF_OCContext;
class CPDF_RenderOptions;
class CPDF_Stream;

class CPDF_FormControl {
 public:
  enum HighlightingMode { None = 0, Invert, Outline, Push, Toggle };

  CPDF_FormControl(CPDF_FormField* pField, CPDF_Dictionary* pWidgetDict);
  ~CPDF_FormControl();

  CPDF_FormField::Type GetType() const { return m_pField->GetType(); }
  const CPDF_InterForm* GetInterForm() const { return m_pForm.Get(); }
  CPDF_FormField* GetField() const { return m_pField; }
  CPDF_Dictionary* GetWidget() const { return m_pWidgetDict.Get(); }
  CFX_FloatRect GetRect() const { return m_pWidgetDict->GetRectFor("Rect"); }

  void DrawControl(CFX_RenderDevice* pDevice,
                   CFX_Matrix* pMatrix,
                   CPDF_Page* pPage,
                   CPDF_Annot::AppearanceMode mode,
                   const CPDF_RenderOptions* pOptions = nullptr);

  ByteString GetCheckedAPState();
  WideString GetExportValue() const;

  bool IsChecked() const;
  bool IsDefaultChecked() const;

  HighlightingMode GetHighlightingMode();
  bool HasMKEntry(const ByteString& csEntry) const;
  int GetRotation();

  FX_ARGB GetBorderColor(int& iColorType) { return GetColor(iColorType, "BC"); }

  float GetOriginalBorderColor(int index) {
    return GetOriginalColor(index, "BC");
  }

  void GetOriginalBorderColor(int& iColorType, float fc[4]) {
    GetOriginalColor(iColorType, fc, "BC");
  }

  FX_ARGB GetBackgroundColor(int& iColorType) {
    return GetColor(iColorType, "BG");
  }

  float GetOriginalBackgroundColor(int index) {
    return GetOriginalColor(index, "BG");
  }

  void GetOriginalBackgroundColor(int& iColorType, float fc[4]) {
    GetOriginalColor(iColorType, fc, "BG");
  }

  WideString GetNormalCaption() { return GetCaption("CA"); }
  WideString GetRolloverCaption() { return GetCaption("RC"); }
  WideString GetDownCaption() { return GetCaption("AC"); }

  CPDF_Stream* GetNormalIcon() { return GetIcon("I"); }
  CPDF_Stream* GetRolloverIcon() { return GetIcon("RI"); }
  CPDF_Stream* GetDownIcon() { return GetIcon("IX"); }
  CPDF_IconFit GetIconFit();

  int GetTextPosition();
  CPDF_Action GetAction();
  CPDF_AAction GetAdditionalAction();
  CPDF_DefaultAppearance GetDefaultAppearance();

  CPDF_Font* GetDefaultControlFont();
  int GetControlAlignment();

  ByteString GetOnStateName() const;
  void CheckControl(bool bChecked);

 private:
  void SetOnStateName(const ByteString& csOn);
  FX_ARGB GetColor(int& iColorType, const ByteString& csEntry);
  float GetOriginalColor(int index, const ByteString& csEntry);
  void GetOriginalColor(int& iColorType,
                        float fc[4],
                        const ByteString& csEntry);

  WideString GetCaption(const ByteString& csEntry);
  CPDF_Stream* GetIcon(const ByteString& csEntry);
  CPDF_ApSettings GetMK() const;

  CPDF_FormField* const m_pField;
  UnownedPtr<CPDF_Dictionary> const m_pWidgetDict;
  UnownedPtr<const CPDF_InterForm> const m_pForm;
};

#endif  // CORE_FPDFDOC_CPDF_FORMCONTROL_H_
