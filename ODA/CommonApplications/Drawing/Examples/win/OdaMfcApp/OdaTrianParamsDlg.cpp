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

// OdaTrianParamsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "OdaTrianParamsDlg.h"

OdaTrianParamsDlg::OdaTrianParamsDlg(CWnd* pParent /*=NULL*/, wrTriangulationParams *params /*=NULL*/, double *facetRes/* = NULL*/)
  : CDialog(OdaTrianParamsDlg::IDD, pParent), m_params(params), m_dFacetRes(facetRes)
{
}

OdaTrianParamsDlg::~OdaTrianParamsDlg()
{
}

void OdaTrianParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void OdaTrianParamsDlg::fillTriangulationParametersFields()
{
  CButton *m_ctlCheck = (CButton*)GetDlgItem(IDC_TP_USENEWTESSELLATION);
  m_ctlCheck->SetCheck(m_params->bUseTesselation);
  m_ctlCheck = (CButton*)GetDlgItem(IDC_TP_RECALCSURFTOL);
  m_ctlCheck->SetCheck(m_params->bRecalculateSurfaceTolerance);

  OdString sBuf;
  SetDlgItemText(IDC_TP_BETWEENKNOTS, sBuf.format(L"%d", m_params->BetweenKnots).c_str());
  SetDlgItemText(IDC_TP_MAXFACETEDGELENGTH, odDToStr(m_params->maxFacetEdgeLength, 'g', 8).c_str());
  SetDlgItemText(IDC_TP_MAXNUMGRIDLINES, sBuf.format(L"%d", m_params->maxNumGridLines).c_str());
  SetDlgItemText(IDC_TP_NORMALTOL, odDToStr(m_params->normalTolerance, 'g', 8).c_str());
  SetDlgItemText(IDC_TP_POINTSPEREDGE, sBuf.format(L"%d", m_params->PointsPerEdge).c_str());
  SetDlgItemText(IDC_TP_SURFACETOL, odDToStr(m_params->surfaceTolerance, 'g', 8).c_str());

  SetDlgItemText(IDC_FACETRES, odDToStr(*m_dFacetRes, 'g', 8).c_str());

  if (m_params->bUseTesselation)
  {
    GetDlgItem(IDC_TP_BETWEENKNOTS)->EnableWindow(FALSE);
    GetDlgItem(IDC_TP_MAXFACETEDGELENGTH)->EnableWindow(TRUE);
    GetDlgItem(IDC_TP_MAXNUMGRIDLINES)->EnableWindow(FALSE);
    GetDlgItem(IDC_TP_NORMALTOL)->EnableWindow(TRUE);
    GetDlgItem(IDC_TP_POINTSPEREDGE)->EnableWindow(FALSE);
    GetDlgItem(IDC_TP_SURFACETOL)->EnableWindow(!m_params->bRecalculateSurfaceTolerance);
    GetDlgItem(IDC_TP_RECALCSURFTOL)->EnableWindow(TRUE);
    GetDlgItem(IDC_FACETRES)->EnableWindow(TRUE);
  }
  else
  {
    GetDlgItem(IDC_TP_BETWEENKNOTS)->EnableWindow(TRUE);
    GetDlgItem(IDC_TP_MAXFACETEDGELENGTH)->EnableWindow(FALSE);
    GetDlgItem(IDC_TP_MAXNUMGRIDLINES)->EnableWindow(TRUE);
    GetDlgItem(IDC_TP_NORMALTOL)->EnableWindow(TRUE);
    GetDlgItem(IDC_TP_POINTSPEREDGE)->EnableWindow(TRUE);
    GetDlgItem(IDC_TP_SURFACETOL)->EnableWindow(FALSE);
    GetDlgItem(IDC_TP_RECALCSURFTOL)->EnableWindow(FALSE);
    GetDlgItem(IDC_FACETRES)->EnableWindow(m_params->bRecalculateSurfaceTolerance);
  }
}

BOOL OdaTrianParamsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  fillTriangulationParametersFields();
  return TRUE;
}

BEGIN_MESSAGE_MAP(OdaTrianParamsDlg, CDialog)
  ON_BN_CLICKED(IDOK, &OdaTrianParamsDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDC_TP_USENEWTESSELLATION, &OdaTrianParamsDlg::OnBnClickedTpUsenewtessellation)
  ON_BN_CLICKED(IDC_TP_RECALCSURFTOL, &OdaTrianParamsDlg::OnBnClickedRecalcSurfTol)
  ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &OdaTrianParamsDlg::OnBnClickedButtonDefault)
END_MESSAGE_MAP()

static bool preValidate(const OdString &pStr, const OdChar *syms)
{
  const OdChar *pIn = pStr.c_str();
  while (*pIn)
  {
    const OdChar *pOp = syms;
    while (*pOp)
    {
      if (*pIn == *pOp)
        break;
      pOp++;
    }
    if (!*pOp)
      return false;
    pIn++;
  }
  return true;
}

bool OdaTrianParamsDlg::validateField(int hDlg, double minVal, double maxVal, double &value, const OdString name, bool isInteger /*= false*/)
{
  bool bValid = false;
  CString tmpStr;
  GetDlgItemText(hDlg, tmpStr);
  OdString sValIn(tmpStr);
  if (preValidate(sValIn, isInteger ? OD_T("-+0123456789")  : OD_T("-+.eE0123456789")))
  {
    double dVal = odStrToD(tmpStr.GetBuffer());
    if (dVal >= minVal && dVal <= maxVal)
    {
      bValid = true;
      value = dVal;
    }
  }
  if (!bValid)
  {
    OdString message = name + " must be a " + (isInteger ? "integer" : "double") + " in range %.15f to %.15f.";
    MessageBox(OdString().format(message.c_str(), minVal, maxVal).c_str(), OD_T("Input Validation Error"), MB_ICONWARNING);
  }
  return bValid;
}

void OdaTrianParamsDlg::OnBnClickedOk()
{
  m_params->bUseTesselation = ((CButton*)GetDlgItem(IDC_TP_USENEWTESSELLATION))->GetCheck() != 0;
  m_params->bRecalculateSurfaceTolerance = ((CButton*)GetDlgItem(IDC_TP_RECALCSURFTOL))->GetCheck() != 0;

  double value;
  if (validateField(IDC_TP_NORMALTOL, 1., 360., value, OD_T("normalTolerance"), false))
    m_params->normalTolerance = value;
  else return;
  if (validateField(IDC_TP_BETWEENKNOTS, 0., 20., value, OD_T("BetweenKnots"), true))
    m_params->BetweenKnots = value;
  else return;
  if (validateField(IDC_TP_POINTSPEREDGE, 0., 100., value, OD_T("PointsPerEdge"), true))
    m_params->PointsPerEdge = value;
  else return;
  if (validateField(IDC_TP_MAXNUMGRIDLINES, 0., 10000., value, OD_T("maxNumGridLines"), true))
    m_params->maxNumGridLines = value;
  else return;
  if (validateField(IDC_TP_MAXFACETEDGELENGTH, 0., 100000., value, OD_T("maxFacetEdgeLength"), false))
    m_params->maxFacetEdgeLength = value;
  else return;
  if (validateField(IDC_TP_SURFACETOL, -100000., 100000., value, OD_T("surfaceTolerance"), false))
    m_params->surfaceTolerance = value;
  else return;

  if (validateField(IDC_FACETRES, m_params->bUseTesselation ? 1e-2 : 0., 10., value, OD_T("FacetRes"), false))
    *m_dFacetRes = value;
  else return;

  CDialog::OnOK();
}

void copyParams(wrTriangulationParams *from, wrTriangulationParams *to)
{
  to->BetweenKnots = from->BetweenKnots;
  to->bRecalculateSurfaceTolerance = from->bRecalculateSurfaceTolerance;
  to->bUseTesselation = from->bUseTesselation;
  to->gridAspectRatio = from->gridAspectRatio;
  to->maxFacetEdgeLength = from->maxFacetEdgeLength;
  to->maxNumGridLines = from->maxNumGridLines;
  to->normalTolerance = from->normalTolerance;
  to->PointsPerEdge = from->PointsPerEdge;
  to->surfaceTolerance = from->surfaceTolerance;
}

void OdaTrianParamsDlg::OnBnClickedTpUsenewtessellation()
{
  bool checked = ((CButton *)GetDlgItem(IDC_TP_USENEWTESSELLATION))->GetCheck() != 0;
  wrTriangulationParams triangulationParams(checked);
  copyParams(&triangulationParams, m_params);
  fillTriangulationParametersFields();
}

void OdaTrianParamsDlg::OnBnClickedRecalcSurfTol()
{
  bool checked = ((CButton *)GetDlgItem(IDC_TP_RECALCSURFTOL))->GetCheck() != 0;
  GetDlgItem(IDC_TP_SURFACETOL)->EnableWindow(m_params->bUseTesselation && !checked);
}

void OdaTrianParamsDlg::OnBnClickedButtonDefault()
{
  wrTriangulationParams triangulationParams; //default values in constructor
  copyParams(&triangulationParams, m_params);
  *m_dFacetRes = 0.5;
  fillTriangulationParametersFields();
}
