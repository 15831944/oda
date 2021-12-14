/////////////////////////////////////////////////////////////////////////////// 
// Created by Open Design Alliance 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDFIMPORTBASEOBJECTGRABBER_INCLUDED_
#define _PDFIMPORTBASEOBJECTGRABBER_INCLUDED_

#include "public/fpdfview.h"
#include <vector>
#include <cstdint>

struct FPDF_EXPORT  OdPdfImportPoint
{
  OdPdfImportPoint();
  OdPdfImportPoint(double x_in, double y_in);

  double x;
  double y;
};

enum FPDF_EXPORT OdPdfImportPtType { OdLineTo, OdBezierTo, OdMoveTo };

struct FPDF_EXPORT OdPdfImportPathData
{
  OdPdfImportPathData();
  OdPdfImportPoint   m_Point;
  OdPdfImportPtType  m_Type;
  bool               m_CloseFigure;
};

struct FPDF_EXPORT OdPdfImportMatrix
{
  OdPdfImportMatrix();
  OdPdfImportMatrix(double a_in, double b_in, double c_in, double d_in, double e_in, double f_in);
  double a;
  double b;
  double c;
  double d;
  double e;
  double f;
};

struct FPDF_EXPORT OdPdfImportColor
{
  OdPdfImportColor(unsigned char r_in, unsigned char g_in, unsigned char b_in, unsigned char alfa_in);
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char alfa;
};

struct FPDF_EXPORT OdPdfImportRect
{
  OdPdfImportRect();
  OdPdfImportRect(double l, double t, double r, double b);

  double left;
  double top;
  double right;
  double bottom;
};

struct FPDF_EXPORT BmiHeaderInfo
{
  int width;
  int height;
  int pitch;
  int bpp;
  void * palette;
  int palette_size;
};

struct FPDF_EXPORT LineTypeInfo
{
  float* dash_array;
  int dash_count;
  float dash_phase;
};

struct FPDF_EXPORT PdfImportFontParams
{
  const char* in_font_face_name{nullptr};
  bool is_bold{false};
  bool is_italic{false};
  double font_height{0.0};
  double text_width{0.0};
  bool is_char_space{false};
  bool is_space{false};
  float underline_point_X{0.0};
  float underline_point_Y{0.0};
  bool isType3Font{false};
  bool isChanged{false};
  uint8_t* embedded_font_data{nullptr};
  uint32_t embedded_font_data_size{ 0 };
  const char* font_file_name{ nullptr };
};

enum ImportHyperlinkMethod
{
  eImportAsEntityHyperlink = 0,
  eImportAsTrasparentRegion,
  eMixedModeImport
};

class FPDF_EXPORT OdPdfImportBaseObjectGrabber
{
public:
  OdPdfImportBaseObjectGrabber() {}
  virtual ~OdPdfImportBaseObjectGrabber() {};
  virtual bool needToAddGrathics(bool* ret) { return false; }
  virtual void addPath(const OdPdfImportPathData* path_data, const unsigned long path_point_count, const OdPdfImportColor* fill_color, 
    const OdPdfImportColor* stroke_color, const float* line_width, const bool is_object_visible, const wchar_t* layer_name, 
    const LineTypeInfo* line_type = nullptr, const char* hyperlink = nullptr) {}
  virtual void addCosmeticLine(const OdPdfImportPoint* point1, const OdPdfImportPoint* point2, const OdPdfImportColor* color, 
    const bool is_object_visible, const wchar_t* layer_name, const char* hyperlink = nullptr) {}
  virtual void addPixel(const OdPdfImportPoint* point, const OdPdfImportColor* color, const bool is_object_visible, 
    const wchar_t* layer_name, const char* hyperlink = nullptr) {}

  virtual bool getClipBoundary(std::vector<OdPdfImportPathData>& path_data) { return false; }
  virtual bool needToAddImage(bool* ret) { return false;  }
  virtual bool isImageClippingEnabled() { return false; }
  virtual void addImage(const BmiHeaderInfo* bmi, const unsigned char* imagedata, const OdPdfImportRect* rect, 
    const OdPdfImportMatrix* object_matrix, const bool is_object_visible, const wchar_t* layer_name, const char* hyperlink = nullptr) {}

  virtual bool needToAddText(bool* ret) { return false; }
  virtual void addText(const wchar_t* text, const PdfImportFontParams* const font_params, const OdPdfImportColor* color, 
    const OdPdfImportMatrix* object_matrix, const bool is_object_visible, const wchar_t* layer_name, const char* hyperlink = nullptr) {}
  virtual bool combineTextObjects() const { return false; }
  virtual void getSpaceFactors(double& char_space_factor, double& space_factor) const { char_space_factor = 0.6;  space_factor = 1.5; }
  virtual bool extractEmbeddedFonts() const { return false; }


  virtual bool needToAddShading(bool* ret) { return false; }
  virtual void addShading(const BmiHeaderInfo* bmi, const unsigned char* imagedata, const OdPdfImportRect* rect,
    const OdPdfImportMatrix* object_matrix, const bool is_object_visible, const wchar_t* layer_name, const char* hyperlink = nullptr) {}

  virtual void addClipPath(const OdPdfImportPathData* path_data, const unsigned long path_point_count) {}

  virtual void SaveClipState() {}
  virtual void RestoreClipState(bool bKeepSaved) {}
  virtual bool isClippingEnabled() { return false; }

  virtual float getWidth() = 0;
  virtual float getHeight() = 0;

  virtual ImportHyperlinkMethod getImportHyperlinkMethod() { return ImportHyperlinkMethod::eImportAsEntityHyperlink; }
  virtual void addHyperlinkAsTransparentRegion(const OdPdfImportPathData* path_data, const unsigned long path_point_count, const char* hyperlink) {}

  virtual bool getPreferableFont(const char* font_name, const char* font_family, const bool is_found, char* preferable_font_name, unsigned int& font_name_size)
  {
    return false;
  };
};


#endif // _PDFIMPORTBASEOBJECTGRABBER_INCLUDED_
