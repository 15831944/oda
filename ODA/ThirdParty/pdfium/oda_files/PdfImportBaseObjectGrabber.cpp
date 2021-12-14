/////////////////////////////////////////////////////////////////////////////// 
// Created by Open Design Alliance 
///////////////////////////////////////////////////////////////////////////////

#include "oda_files/PdfImportBaseObjectGrabber.h"

OdPdfImportPoint::OdPdfImportPoint()
  :x(0)
  ,y(0)
{

}

OdPdfImportPoint::OdPdfImportPoint(double x_in, double y_in)
  :x(x_in)
  ,y(y_in)
{

}

OdPdfImportPathData::OdPdfImportPathData()
  :m_Type(OdPdfImportPtType::OdMoveTo)
  ,m_CloseFigure(false)
{

}

OdPdfImportMatrix::OdPdfImportMatrix()
  :a(1)
  ,b(0)
  ,c(0)
  ,d(1)
  ,e(0)
  ,f(0)
{

}

OdPdfImportMatrix::OdPdfImportMatrix(double a_in, double b_in, double c_in, double d_in, double e_in, double f_in) 
  :a(a_in)
  ,b(b_in)
  ,c(c_in)
  ,d(d_in)
  ,e(e_in)
  ,f(f_in)
{

}

OdPdfImportColor::OdPdfImportColor(unsigned char r_in, unsigned char g_in, unsigned char b_in, unsigned char alfa_in)
  :r(r_in)
  ,g(g_in)
  ,b(b_in)
  ,alfa(alfa_in)
{

}

OdPdfImportRect::OdPdfImportRect() 
  :left(0)
  ,top(0)
  ,right(0)
  ,bottom(0)
{

}

OdPdfImportRect::OdPdfImportRect(double l, double t, double r, double b)
  :left(l)
  ,top(t)
  ,right(r)
  ,bottom(b)
{

}
