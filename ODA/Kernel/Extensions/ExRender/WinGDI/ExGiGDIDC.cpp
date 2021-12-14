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

#include "OdaCommon.h"
#include "ExGiGDIDC.h"
#include "../ExGsHelpers.h"

//GDI pen style
ExGiGDIDC::OdGDIStyler::GDIPenStyle::GDIPenStyle()
{
  m_isCustom = false;
  m_sz = 0;
  memset( m_style, 0, sizeof( DWORD ) * 16 );
  m_pair = PenStylePair( 1, 1 );
  m_nStyleBase = 0;
  m_width = 1;
}

ExGiGDIDC::OdGDIStyler::GDIPenStyle::GDIPenStyle( const PenStylePair& p )
{
  m_isCustom = false;
  m_sz = 0;
  memset( m_style, 0, sizeof( DWORD ) * 16 );
  m_pair = p;
  m_nStyleBase = 0;
  m_width = 1;
}

ExGiGDIDC::OdGDIStyler::GDIPenStyle::GDIPenStyle( DWORD sz, const DWORD* pStyle )
{
  m_isCustom = sz > 0 && pStyle != NULL;
  if( !m_isCustom ) return;
  if( sz > 15 )
  {
    ODA_FAIL();
    sz = 15;
  }
  m_sz = sz;
  if( m_sz == 0 )
    memset( m_style, 0, sizeof( DWORD ) * 16 );
  else
  {
    memcpy( m_style, pStyle, sizeof( DWORD ) * m_sz );
  }
  m_nStyleBase = 0;
  for( DWORD i = 0; i < m_sz; ++i )
  {
    m_nStyleBase += m_style[ i ];
  }
  m_pair = PenStylePair( 1, 1 );
  m_width = 1;
}

void ExGiGDIDC::OdGDIStyler::GDIPenStyle::set( const PenStylePair& p )
{
  m_isCustom = false;
  m_pair = p;
  m_width = 1;
}

void ExGiGDIDC::OdGDIStyler::GDIPenStyle::set( DWORD sz, const DWORD* pStyle )
{
  m_width = 1;
  m_isCustom = sz > 0 && pStyle != NULL;
  if( !m_isCustom ) return;
  if( sz > 16 )
  {
    ODA_FAIL();
    sz = 16;
  }
  m_sz = sz;
  if( m_sz == 0 )
    memset( m_style, 0, sizeof( DWORD ) * 16 );
  else
  {
    memcpy( m_style, pStyle, sizeof( DWORD ) * m_sz );
  }
  m_nStyleBase = 0;

  //Zero value
  for( DWORD i = 0; i < m_sz; ++i )
  {
    if( m_style[ i ] == 0 )
    {
      m_style[ i ] = 1;
      if( i > 0 ) m_style[ i - 1 ] -= 1;
      else if( i < m_sz - 1 ) m_style[ i + 1 ] -= 1;
    }
  }
  for( DWORD i = 0; i < m_sz; ++i )
  {
    m_nStyleBase += m_style[ i ];
  }
}

bool ExGiGDIDC::OdGDIStyler::GDIPenStyle::isPixelVisible( OdUInt32 nPixel ) const
{
  if( !m_isCustom )
  {
    if( m_pair.second == 1 ) return m_pair.first != 0;
    OdUInt32 n = nPixel % m_pair.second;
    OdUInt32 bit = 1 << n;
    return GETBIT( m_pair.first, bit );
  }
  bool bOdd = ( m_sz % 2 == 0 );
  OdUInt32 n = nPixel % m_nStyleBase;
  OdUInt32 nSw = nPixel / m_nStyleBase;
  bool bVisible = /*bOdd ? ( nSw % 2 == 0 ) :*/ true;
  OdUInt32 nCur = 0;
  //Threat 0 as 1
  if( n == 0 ) return bVisible;
  OdUInt32 nStep = 0;
  while( nCur < n && nStep < m_sz )
  {
    nCur += m_style[ nStep ];
    if( nCur >= n ) return bVisible;
    bVisible = !bVisible;
    nStep++;
  }
  ODA_FAIL();
  return true;
}

//GDI brush style
ExGiGDIDC::OdGDIStyler::GDIBrushStyle::GDIBrushStyle()
{
  m_type = kMono;
  m_bVisibleMono = true;
  m_curPattern = 0;
  memset( m_pattern, 0, sizeof( OdUInt8 ) * 8 );
}

ExGiGDIDC::OdGDIStyler::GDIBrushStyle::~GDIBrushStyle()
{

}

void ExGiGDIDC::OdGDIStyler::GDIBrushStyle::set( const LOGBRUSH& lb )
{
  if( lb.lbStyle == BS_SOLID || lb.lbStyle == BS_HOLLOW )
  {
    m_type = kMono;
    m_bVisibleMono = lb.lbStyle == BS_SOLID;
  }
  else if( lb.lbStyle == BS_PATTERN )
  {
    m_type = kPattern;
    if( m_curPattern != lb.lbHatch )
    {
      HDC dc = ::CreateCompatibleDC( NULL );
      HBITMAP bmp = (HBITMAP)( lb.lbHatch );
      HGDIOBJ old = ::SelectObject( dc, bmp );

      for( OdUInt8 i = 0; i < 8; ++i )
      {
        for( OdUInt8 j = 0; j < 8; ++j )
        {
          COLORREF cl = ::GetPixel( dc, j, i );
          bool bInVis = GetRValue( cl ) != 0 || GetGValue( cl ) != 0 || GetBValue( cl ) != 0;
          OdUInt8 nBit = 1 << j;
          SETBIT( m_pattern[ i ], nBit, !bInVis );
        }
      }

      ::SelectObject( dc, old );
      ::DeleteDC( dc );
      m_curPattern = lb.lbHatch;
    }
  }
#ifndef _WIN32_WCE
  else if( lb.lbStyle == BS_HATCHED )
  {
    m_type = kPattern;
    if( m_curPattern != lb.lbHatch )
    {
      createPatternForHatch( (OdUInt8)( lb.lbHatch ) );
      m_curPattern = lb.lbHatch;
    }
  }
#endif
  else
  {
    ODA_FAIL();
    m_type = kMono;
    m_bVisibleMono = true;
  }
}

#ifndef _WIN32_WCE
void ExGiGDIDC::OdGDIStyler::GDIBrushStyle::createPatternForHatch( OdUInt8 pat, bool bClear )
{
  if( bClear ) memset( m_pattern, 0, sizeof( OdUInt8 ) * 8 );
  if( pat == HS_HORIZONTAL )
  {
    m_pattern[ 0 ] = 0xFF;
  }
  else if( pat == HS_VERTICAL )
  {
    for( OdUInt8 i = 0; i < 8; ++i ) m_pattern[ i ] |= 1;
  }
  else if( pat == HS_FDIAGONAL )
  {
    for( OdUInt8 i = 0; i < 8; ++i )
    {
      OdUInt8 nBit = 1 << i;
      SETBIT_1( m_pattern[ i ], nBit );
    }
  }
  else if( pat == HS_BDIAGONAL )
  {
    for( OdUInt8 i = 0; i < 8; ++i )
    {
      OdUInt8 nBit = 0x80 >> i;
      SETBIT_1( m_pattern[ i ], nBit );
    }
  }
  else if( pat == HS_CROSS )
  {
    createPatternForHatch( HS_HORIZONTAL, false );
    createPatternForHatch( HS_VERTICAL, false );
  }
  else if( pat == HS_DIAGCROSS )
  {
    createPatternForHatch( HS_FDIAGONAL, false );
    createPatternForHatch( HS_BDIAGONAL, false );
  }
  else
  {
    ODA_FAIL();
    memset( m_pattern, 0, sizeof( OdUInt8 ) * 8 );
  }
}
#endif

bool ExGiGDIDC::OdGDIStyler::GDIBrushStyle::isPixelVisible( OdUInt32 nRow, OdUInt32 nCol ) const
{
  if( m_type == kMono ) return m_bVisibleMono;
  nRow = nRow % 8;
  nCol = nCol % 8;
  OdUInt8 nBit = 1 << nCol;
  return GETBIT( m_pattern[ nRow ], nBit );
}


//Styler
ExGiGDIDC::OdGDIStyler::OdGDIStyler()
{
  prePenStyles[ kSolid ] = PenStylePair( 1, 1 ); //solid
  prePenStyles[ kDash ] = PenStylePair( 7, 4 ); //dash
  prePenStyles[ kDot ] = PenStylePair( 1, 2 );//dot
  prePenStyles[ kDashDot ] = PenStylePair( 0x575D75D7, 32 ); //Dash-dot
  prePenStyles[ kDashDotDot ] = PenStylePair( 0x57575757, 32 ); //Dash-dot-dot
  prePenStyles[ kDot3 ] = PenStylePair( 7, 6 ); //3 pixels Dot
  prePenStyles[ kNull ] = PenStylePair( 0, 1 ); //invisible
  m_penStyle = GDIPenStyle( prePenStyles[ kSolid ] );
}

ExGiGDIDC::OdGDIStyler::~OdGDIStyler()
{

}

#ifndef _WIN32_WCE
void ExGiGDIDC::OdGDIStyler::setGDIPenStyle( const EXTLOGPEN& elp )
{
  DWORD style = elp.elpPenStyle;
  m_penStyle.setWidth( 1 );
  if( style == 0 )
  {
    m_penStyle.set( prePenStyles[ kSolid ] );
    m_penStyle.setWidth( elp.elpWidth );
  }
  else if( (style & PS_STYLE_MASK) == PS_DASH )
    m_penStyle.set( prePenStyles[ kDash ] );
  else if( ( style & PS_STYLE_MASK ) == PS_DOT )
    m_penStyle.set( prePenStyles[ kDot3 ] ); //I am do not understand, why GDI use 3 pixel Dot in PS_DOT
  else if( ( style & PS_STYLE_MASK ) == PS_DASHDOT )
    m_penStyle.set( prePenStyles[ kDashDot ] );
  else if( ( style & PS_STYLE_MASK ) == PS_DASHDOTDOT )
    m_penStyle.set( prePenStyles[ kDashDotDot ] );
  else if( ( style & PS_STYLE_MASK ) == PS_NULL )
    m_penStyle.set( prePenStyles[ kNull ] );
  else if( ( style & PS_GEOMETRIC ) )
  {
    m_penStyle.set( prePenStyles[ kSolid ] );
  }
  else
  {
    //unsupported now
    ODA_FAIL_ONCE();
    m_penStyle = prePenStyles[ kSolid ];
  }
}
#endif

void ExGiGDIDC::OdGDIStyler::setGDIPenStyle( const LOGPEN& pen )
{
  DWORD style = pen.lopnStyle;
  m_penStyle.setWidth( 1 );
  if( style == 0 )
  {
    m_penStyle.set( prePenStyles[ kSolid ] );
    m_penStyle.setWidth( pen.lopnWidth.x );
  }
  else if( style == PS_DASH )
    m_penStyle.set( prePenStyles[ kDash ] );
  else if( style == PS_NULL )
    m_penStyle.set( prePenStyles[ kNull ] );
  else
  {
    //unsupported now
    ODA_FAIL_ONCE();
    m_penStyle = prePenStyles[ kSolid ];
  }
}

void ExGiGDIDC::OdGDIStyler::setCustomPenStyle( DWORD sz, const DWORD* pStyle )
{
  m_penStyle.set( sz, pStyle );
}

void ExGiGDIDC::OdGDIStyler::setGDIBrushStyle( const LOGBRUSH& lb )
{
  m_brushStyle.set( lb );
}

bool ExGiGDIDC::OdGDIStyler::isPenPixelVisible( OdUInt32 nPixel ) const
{
  return m_penStyle.isPixelVisible( nPixel );
}

bool ExGiGDIDC::OdGDIStyler::isBrushPixelVisible( OdUInt32 nRow, OdUInt32 nPixel ) const
{
  return m_brushStyle.isPixelVisible( nRow, nPixel );
}

#define START_MEM_DC_PROCESSING                       \
  if( isMirrorMode() )                                \
  {                                                   \
    HDC memDC = memoryDC();                           \
    if( !memDC ) throw eNotApplicable;                

#define END_MEM_DC_PROCESSING                         \
  }

ExGiGDIDC::ExGiGDIDC( ExGDIDCTransparencyMode transparencyMode )
{
  m_hTargetDC = NULL;
  m_hMemDC = NULL;
  m_memBitmap = NULL;
  m_flags = 0;
  m_width = -1;
  m_height = -1;

  if( transparencyMode == kMirrorTransparency )
  {
    SETBIT_1( m_flags, kMirrorMode );
  }
  else if( transparencyMode == kDirectTransparency )
  {
    SETBIT_1( m_flags, kDirectMode );
  }

  m_memObjects.resize( kAllObjects );

  for( OdUInt32 i = 0; i < kAllObjects; ++i )
  {
    m_memObjects[ i ] = NULL;
  }
  m_bCheckFlag = kNone;

  m_styler.setDC( this );
}

//void writeMemDC( const OdString& file, HBITMAP hBmp, HDC hdc );

ExGiGDIDC::~ExGiGDIDC()
{
  m_hTargetDC = NULL;
  /*if( isMirrorMode() )
  {
    writeMemDC( OD_T( "D:\\memDC.bmp" ), m_memBitmap, m_hMemDC );
  }*/
  releaseMemDC();
  for( OdUInt32 i = 0; i < kAllObjects; ++i )
  {
    if( m_memObjects[ i ] != NULL )
    {
      ODA_FAIL();
    }
  }
  m_memObjects.clear();
}

bool ExGiGDIDC::initMemDC() const
{
  if( !isMirrorMode() ) return true;
  if( m_hMemDC ) return true;
  if( m_width < 0 || m_height < 0 )
  {
    ODA_FAIL();
    return false;
  }
  m_hMemDC = CreateCompatibleDC( m_hTargetDC );
  if( m_hMemDC == NULL )
  {
    ODA_FAIL();
    m_hMemDC = CreateCompatibleDC( NULL );
  }
  if( m_hMemDC == NULL ) return false;

  //m_memBitmap = CreateCompatibleBitmap( m_hMemDC, m_width, m_height );

  BITMAPINFO bmi;
  memset( &bmi, 0, sizeof bmi );
  bmi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
  bmi.bmiHeader.biWidth = m_width;
  bmi.bmiHeader.biHeight = m_height;
  bmi.bmiHeader.biPlanes = 1; // we are assuming that there is only one plane
  bmi.bmiHeader.biBitCount = 24;
  OdUInt8* pBits;
  m_memBitmap = CreateDIBSection( m_hMemDC, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0 );

  if( m_memBitmap == NULL ) return false;

  m_memObjects[ kBitmap ] = ::SelectObject( m_hMemDC, m_memBitmap );
  return true;
}

void ExGiGDIDC::releaseMemDC( bool bResetSize ) const
{
  if( bResetSize )
  {
    m_width = -1;
    m_height = -1;
  }
  if( !isMirrorMode() ) return;
  if( m_hMemDC == NULL ) return;

  if( m_memBitmap != NULL )
  {
    ::SelectObject( m_hMemDC, m_memObjects[ kBitmap ] );
    m_memObjects[ kBitmap ] = NULL;
    ::DeleteObject( m_memBitmap );
    m_memBitmap = NULL;
  }

  ::DeleteDC( m_hMemDC );
  m_hMemDC = NULL;
}

HGDIOBJ ExGiGDIDC::selectObject( HGDIOBJ obj, ExDCObjects type )
{
  setNeedSync();
  HGDIOBJ res = ::SelectObject( m_hTargetDC, obj );
  START_MEM_DC_PROCESSING
    if( type == kUseTheSame )
      ::SelectObject( memDC, obj );
    else
    {
      if( m_memObjects[ type ] == NULL )
      {
        ODA_FAIL();
      }
      ::SelectObject( memDC, m_memObjects[ type ] );
    }
  END_MEM_DC_PROCESSING
  return res;
}

BOOL ExGiGDIDC::deleteMirrorObject( ExDCObjects type )
{
  BOOL res = FALSE;
  START_MEM_DC_PROCESSING
    if( type < kUseTheSame && m_memObjects[ type ] )
    {
      res = ::DeleteObject( m_memObjects[ type ] );
      m_memObjects[ type ] = NULL;
    }
  END_MEM_DC_PROCESSING
    return res == TRUE;
}

COLORREF ExGiGDIDC::setBkColor( COLORREF color )
{
  COLORREF res = ::SetBkColor( m_hTargetDC, color );
  START_MEM_DC_PROCESSING
    ::SetBkColor( memDC, color );
  END_MEM_DC_PROCESSING
  return res;
}

int ExGiGDIDC::setBkMode( int mode )
{
  int res = ::SetBkMode( m_hTargetDC, mode );
  START_MEM_DC_PROCESSING
    ::SetBkMode( memDC, mode );
  END_MEM_DC_PROCESSING
  return res;
}

bool ExGiGDIDC::lineTo( int x, int y )
{
  if( isAlphaMode() )
  {
    return alphaLineTo( x, y );
  }
  BOOL res = ::LineTo( m_hTargetDC, x, y );
  START_MEM_DC_PROCESSING
    ::LineTo( memDC, x, y );
  END_MEM_DC_PROCESSING
  return res == TRUE;
}

bool ExGiGDIDC::moveToEx( int x, int y, LPPOINT lpp )
{
  BOOL res = ::MoveToEx( m_hTargetDC, x, y, lpp );
  START_MEM_DC_PROCESSING
    ::MoveToEx( memDC, x, y, lpp );
  END_MEM_DC_PROCESSING
  return res == TRUE;
}

COLORREF ExGiGDIDC::setPixel( int x, int y, COLORREF color )
{
  COLORREF res = ::SetPixel( m_hTargetDC, x, y, color );
  START_MEM_DC_PROCESSING
    ::SetPixel( memDC, x, y, color );
  END_MEM_DC_PROCESSING
  return res;
}

bool ExGiGDIDC::polygon( const POINT* apt, int cpt )
{
  if( cpt == 3 )
  {
    return triangle( apt );
  }
  BOOL res = ::Polygon( m_hTargetDC, apt, cpt );
  START_MEM_DC_PROCESSING
    ::Polygon( memDC, apt, cpt );
  END_MEM_DC_PROCESSING
  return res == TRUE;
}

bool ExGiGDIDC::rectangle( int left, int top, int right, int bottom )
{
  BOOL res = ::Rectangle( m_hTargetDC, left, top, right, bottom );
  START_MEM_DC_PROCESSING
    ::Rectangle( memDC, left, top, right, bottom );
  END_MEM_DC_PROCESSING
  return res == TRUE;
}

bool ExGiGDIDC::polyline( const POINT* apt, int cpt )
{
  if( isAlphaMode() ) return alphaPolyline( apt, cpt );
  BOOL res = ::Polyline( m_hTargetDC, apt, cpt );
  START_MEM_DC_PROCESSING
    ::Polyline( memDC, apt, cpt );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}
#ifndef _WIN32_WCE
bool ExGiGDIDC::polyPolygon( const POINT* apt, const int* asz, int csz )
{
  BOOL res = ::PolyPolygon( m_hTargetDC, apt, asz, csz );
  START_MEM_DC_PROCESSING
    ::PolyPolygon( memDC, apt, asz, csz );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}

int ExGiGDIDC::setPolyFillMode( int mode )
{
  int res = ::SetPolyFillMode( m_hTargetDC, mode );
  START_MEM_DC_PROCESSING
    ::SetPolyFillMode( memDC, mode );
  END_MEM_DC_PROCESSING
    return res;
}

bool ExGiGDIDC::beginPath()
{
  BOOL res = ::BeginPath( m_hTargetDC );
  START_MEM_DC_PROCESSING
    ::BeginPath( memDC );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}

bool ExGiGDIDC::polyDraw( const POINT* apt, const BYTE* aj, int cpt )
{
  BOOL res = ::PolyDraw( m_hTargetDC, apt, aj, cpt );
  START_MEM_DC_PROCESSING
    ::PolyDraw( memDC, apt, aj, cpt );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}

bool ExGiGDIDC::endPath()
{
  BOOL res = ::EndPath( m_hTargetDC );
  START_MEM_DC_PROCESSING
    ::EndPath( memDC );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}

bool ExGiGDIDC::strokeAndFillPath()
{
  BOOL res = ::StrokeAndFillPath( m_hTargetDC );
  START_MEM_DC_PROCESSING
    ::StrokeAndFillPath( memDC );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}

int ExGiGDIDC::setTextCharacterExtra( int extra )
{
  int res = ::SetTextCharacterExtra( m_hTargetDC, extra );
  START_MEM_DC_PROCESSING
    ::SetTextCharacterExtra( memDC, extra );
  END_MEM_DC_PROCESSING
    return res;
}
#endif

UINT ExGiGDIDC::setTextAlign( UINT align )
{
  UINT res = ::SetTextAlign( m_hTargetDC, align );
  START_MEM_DC_PROCESSING
    ::SetTextAlign( memDC, align );
  END_MEM_DC_PROCESSING
    return res;
}

COLORREF ExGiGDIDC::setTextColor( COLORREF color )
{
  COLORREF res = ::SetTextColor( m_hTargetDC, color );
  START_MEM_DC_PROCESSING
    ::SetTextColor( memDC, color );
  END_MEM_DC_PROCESSING
    return res;
}

bool ExGiGDIDC::extTextOutW( int x, int y, UINT options, const RECT* lprect, LPCWSTR lpString, UINT c, const INT* lpDx )
{
  BOOL res = ::ExtTextOutW( m_hTargetDC, x, y, options, lprect, lpString, c, lpDx );
  START_MEM_DC_PROCESSING
    ::ExtTextOutW( memDC, x, y, options, lprect, lpString, c, lpDx );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}
#ifndef _WIN32_WCE
bool ExGiGDIDC::setGraphicsMode( int mode )
{
  BOOL res = ::SetGraphicsMode( m_hTargetDC, mode );
  START_MEM_DC_PROCESSING
    ::SetGraphicsMode( memDC, mode );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}

bool ExGiGDIDC::setWorldTransform( const XFORM* tr )
{
  BOOL res = ::SetWorldTransform( m_hTargetDC, tr );
  START_MEM_DC_PROCESSING
    ::SetWorldTransform( memDC, tr );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}

bool ExGiGDIDC::modifyWorldTransform( const XFORM* tr, DWORD mode )
{
  BOOL res = ::ModifyWorldTransform( m_hTargetDC, tr, mode );
  START_MEM_DC_PROCESSING
    ::ModifyWorldTransform( memDC, tr, mode );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}
#endif
int ExGiGDIDC::saveDC()
{
  int res = ::SaveDC( m_hTargetDC );
  START_MEM_DC_PROCESSING
    m_memStack[ res ] = ::SaveDC( memDC );
  END_MEM_DC_PROCESSING
    return res;
}

bool ExGiGDIDC::restoreDC( int dc )
{
  BOOL res = ::RestoreDC( m_hTargetDC, dc );
  START_MEM_DC_PROCESSING
    std::map< int, int >::iterator it = m_memStack.find( dc );
    if( it != m_memStack.end() )
    {
      ::RestoreDC( memDC, it->second );
      m_memStack.erase( it );
    }
    else
    {
      ODA_FAIL();
    }
  END_MEM_DC_PROCESSING
    return res == TRUE;
}
#ifndef _WIN32_WCE
bool ExGiGDIDC::selectClipPath( int mode )
{
  BOOL res = ::SelectClipPath( m_hTargetDC, mode );
  START_MEM_DC_PROCESSING
    ::SelectClipPath( memDC, mode );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}
#endif

int ExGiGDIDC::intersectClipRect( int left, int top, int right, int bottom )
{
  int res = ::IntersectClipRect( m_hTargetDC, left, top, right, bottom );
  START_MEM_DC_PROCESSING
    ::IntersectClipRect( memDC, left, top, right, bottom );
  END_MEM_DC_PROCESSING
    return res;
}

int ExGiGDIDC::fillRect( const RECT* lprc, HBRUSH hbr )
{
  int res = ::FillRect( m_hTargetDC, lprc, hbr );
  START_MEM_DC_PROCESSING
    ::FillRect( memDC, lprc, hbr );
  END_MEM_DC_PROCESSING
    return res;
}

#ifndef _WIN32_WCE
int ExGiGDIDC::setStretchBltMode( int mode )
{
  int res = ::SetStretchBltMode( m_hTargetDC, mode );
  START_MEM_DC_PROCESSING
    ::SetStretchBltMode( memDC, mode );
  END_MEM_DC_PROCESSING
    return res;
}
#endif

int ExGiGDIDC::stretchDIBits( int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth, int SrcHeight,
  const void* lpBits, const BITMAPINFO* lpbmi, UINT iUsage, DWORD rop )
{
  int res = ::StretchDIBits( m_hTargetDC, xDest, yDest, DestWidth, DestHeight, xSrc, ySrc, SrcWidth, SrcHeight, lpBits, lpbmi, iUsage, rop );
  START_MEM_DC_PROCESSING
    ::StretchDIBits( memDC, xDest, yDest, DestWidth, DestHeight, xSrc, ySrc, SrcWidth, SrcHeight, lpBits, lpbmi, iUsage, rop );
  END_MEM_DC_PROCESSING
    return res;
}

bool ExGiGDIDC::stretchBlt( int xDest, int yDest, int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop )
{
  if( isAlphaMode() )
  {
    return alphaStretchBlt( xDest, yDest, wDest, hDest, hdcSrc, xSrc, ySrc, wSrc, hSrc, rop );
  }
  BOOL res = ::StretchBlt( m_hTargetDC, xDest, yDest, wDest, hDest, hdcSrc, xSrc, ySrc, wSrc, hSrc, rop );
  START_MEM_DC_PROCESSING
    ::StretchBlt( memDC, xDest, yDest, wDest, hDest, hdcSrc, xSrc, ySrc, wSrc, hSrc, rop );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}

bool ExGiGDIDC::bitBlt( int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop )
{
  if( isAlphaMode() )
  {
    return alphaStretchBlt( x, y, cx, cy, hdcSrc, x1, y1, cx, cy, rop );
  }
  BOOL res = ::BitBlt( m_hTargetDC, x, y, cx, cy, hdcSrc, x1, y1, rop );
  START_MEM_DC_PROCESSING
    ::BitBlt( memDC, x, y, cx, cy, hdcSrc, x1, y1, rop );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}

bool ExGiGDIDC::maskBlt( int xDest, int yDest, int width, int height,
  HDC hdcSrc, int xSrc, int ySrc, HBITMAP hbmMask, int xMask, int yMask, DWORD rop )
{
  BOOL res = ::MaskBlt( m_hTargetDC, xDest, yDest, width, height, hdcSrc, xSrc, ySrc, hbmMask, xMask, yMask, rop );
  START_MEM_DC_PROCESSING
    ::MaskBlt( memDC, xDest, yDest, width, height, hdcSrc, xSrc, ySrc, hbmMask, xMask, yMask, rop );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}

HBITMAP ExGiGDIDC::createDIBSection( const BITMAPINFO* pbmi, UINT usage, OdExGiGDIBytesReflector& refl, HANDLE hSection, DWORD offset )
{
  HBITMAP res = ::CreateDIBSection( m_hTargetDC, pbmi, usage, refl.pIn, hSection, offset );
  START_MEM_DC_PROCESSING
    m_memObjects[ kDIBSectionBitmap ] = ::CreateDIBSection( memDC, pbmi, usage, &refl.pMirror, hSection, offset );
  END_MEM_DC_PROCESSING
    return res;
}

int ExGiGDIDC::setDIBitsToDevice( int xDest, int yDest, DWORD w, DWORD h, int xSrc,
  int ySrc, UINT StartScan, UINT cLines, const OdExGiGDIBytesReflector& refl, const BITMAPINFO* lpbmi, UINT ColorUse )
{
  int res = ::SetDIBitsToDevice( m_hTargetDC, xDest, yDest, w, h, xSrc, ySrc, StartScan, cLines, refl.in(), lpbmi, ColorUse );
  START_MEM_DC_PROCESSING
    ::SetDIBitsToDevice( memDC, xDest, yDest, w, h, xSrc, ySrc, StartScan, cLines, refl.mirror(), lpbmi, ColorUse );
  END_MEM_DC_PROCESSING
    return res;
}

#ifndef _WIN32_WCE
bool ExGiGDIDC::playEnhMetaFile( HENHMETAFILE hmf, const RECT* lprect )
{
  BOOL res = ::PlayEnhMetaFile( m_hTargetDC, hmf, lprect );
  START_MEM_DC_PROCESSING
    ::PlayEnhMetaFile( memDC, hmf, lprect );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}
#endif

//Other
namespace GDIExDCHelpers
{
  inline void swap( int& a, int& b )
  {
    int t;
    t = a;
    a = b;
    b = t;
  }
};

bool ExGiGDIDC::triangle( const POINT* apt )
{
  if( isAlphaMode() ) return alphaTriangle( apt );
  BOOL res = ::Polygon( m_hTargetDC, apt, 3 );
  START_MEM_DC_PROCESSING
    ::Polygon( memDC, apt, 3 );
  END_MEM_DC_PROCESSING
    return res == TRUE;
}

bool ExGiGDIDC::alphaLineTo( int x2, int y2 )
{
  if( !isAlphaMode() ) return lineTo( x2, y2 );
  POINT start;
  if( GetCurrentPositionEx( m_hTargetDC, &start ) != TRUE )
  {
    ODA_FAIL();
    return false;
  }
  int x1 = start.x; int y1 = start.y;
  bool bRet = alphaLine( x1, y1, x2, y2 );
  moveToEx( x2, y2, 0 );
  return bRet;
}

bool ExGiGDIDC::alphaLine( int x1, int y1, int x2, int y2, bool bCheckWidth )
{
  if( bCheckWidth )
  {
    syncDCStyles();
    DWORD w = m_styler.getPenWidth();
    if( w > 1 )
    {
      OdGePoint2d A( x1, y1 );
      OdGePoint2d B( x2, y2 );
      OdGeVector2d AB( B - A );
      OdGeVector2d n = AB.rotateBy( OdaPI2 );
      if( !n.isZeroLength() ) n.normalize();
      bool bDrawFirst = w % 2 == 1;
      int nLines = w / 2;
      for( int i = bDrawFirst ? nLines : nLines - 1; i > 0; --i )
      {
        OdGePoint2d pt1 = A + n * i;
        OdGePoint2d pt2 = B + n * i;
        alphaLine( floor( pt1.x + 0.5 ), floor( pt1.y + 0.5 ), floor( pt2.x + 0.5 ), floor( pt2.y + 0.5 ), false );
      }
      alphaLine( x1, y1, x2, y2, false );
      for( int i = 1; i <= nLines; ++i )
      {
        OdGePoint2d pt1 = A - n * i;
        OdGePoint2d pt2 = B - n * i;
        alphaLine( floor( pt1.x + 0.5 ), floor( pt1.y + 0.5 ), floor( pt2.x + 0.5 ), floor( pt2.y + 0.5 ), false );
      }
      return true;
    }
  }
  int deltaX = Od_abs( x1 - x2 );
  int deltaY = Od_abs( y1 - y2 );
  int length = odmax( deltaX, deltaY );

  int xMin = x1;
  int xMax = x2;
  if( xMax < xMin ) GDIExDCHelpers::swap( xMax, xMin );
  int yMin = y1;
  int yMax = y2;
  if( yMin > yMax ) GDIExDCHelpers::swap( yMin, yMax );

  if( length == 0 )
  {
    alphaSetPixel( x1, y1, m_color );
    return true;
  }

  double dX = double( x2 - x1 ) / (double)length;
  double dY = double( y2 - y1 ) / (double)length;

  double x = x1;
  double y = y1;

  OdUInt32 nPix = 0;

  length++;
  while( length-- )
  {
    x += dX;
    y += dY;
    int resX = floor( x + 0.5 );
    int resY = floor( y + 0.5 );
    int dx = resX - x1;
    int dy = resY - y1;
    nPix = floor( sqrt( (double)( dx * dx + dy * dy ) ) + 0.5 );
    bool bDraw = isPointOnScreen( resX, resY ) && m_styler.isPenPixelVisible( nPix );
    if( resX < xMin || resX > xMax || resY < yMin || resY > yMax ) bDraw = false;
    if( bDraw )
      alphaSetPixel( resX, resY, m_color );
  }
  return true;
}

bool ExGiGDIDC::alphaTriangleLine( int x1, int x2, int y, OdUInt32 nRow )
{
  syncDCStyles();
  if( x1 < 0 )
  {
    x1 = 0;
  }
  if( x2 > m_width ) x2 = m_width;

  int x = x1;
  while( x <= x2 )
  {
    bool bDraw = m_styler.isBrushPixelVisible( y, x );
    if( m_bCheckFlag == kAll )
    {
      bDraw &= processPresentedPoint( x, y );
    }
    else if( m_bCheckFlag == kEnd )
    {
      if( x == x1 || x == x2 )
      {
        bDraw &= processPresentedPoint( x, y );
      }
    }
    if( bDraw )
      alphaSetPixel( x, y, m_color );
    ++x;
  }
  return true;
}

COLORREF ExGiGDIDC::alphaSetPixel( int x, int y, COLORREF color )
{
  if( !isAlphaMode() ) return setPixel( x, y, color );
  COLORREF bgColor = getPixelColorForAlpha( x, y );
  color = ::OdMergeRGBAlpha( bgColor, color, m_alpha );
  COLORREF res = ::SetPixel( m_hTargetDC, x, y, color );
  if( isMirrorMode() )
  {
    HDC memDC = memoryDC();
    ::SetPixel( memDC, x, y, color );
  }
  return res;
}

COLORREF ExGiGDIDC::getPixelColorForAlpha( int x, int y )
{
  COLORREF color = CLR_INVALID;
  if( isMirrorMode() )
  {
    HDC memDC = memoryDC();
    color = ::GetPixel( memDC, x, y );
  }
  else
  {
    color = ::GetPixel( m_hTargetDC, x, y );
  }
  ODA_ASSERT_ONCE( color != CLR_INVALID );
  return color;
}

bool ExGiGDIDC::alphaTriangle( const POINT* apt )
{
  if( !isAlphaMode() ) return triangle( apt );
  if( false )
  {
    return false;
  }
  OdUInt32 nRow = 0;
  int x1 = apt[ 0 ].x, y1 = apt[ 0 ].y, x2 = apt[ 1 ].x, y2 = apt[ 1 ].y, x3 = apt[ 2 ].x, y3 = apt[ 2 ].y;
  if( y2 < y1 ) 
  {
    GDIExDCHelpers::swap( y1, y2 );
    GDIExDCHelpers::swap( x1, x2 );
  }
  if( y3 < y1 ) 
  {
    GDIExDCHelpers::swap( y1, y3 );
    GDIExDCHelpers::swap( x1, x3 );
  }
  if( y2 > y3 ) 
  {
    GDIExDCHelpers::swap( y2, y3 );
    GDIExDCHelpers::swap( x2, x3 );
  }
  int xMAX = x1; if( xMAX < x2 ) xMAX = x2; if( xMAX < x3 ) xMAX = x3;
  int xMIN = x1; if( xMIN > x2 ) xMIN = x2; if( xMIN > x3 ) xMIN = x3;
  double dx13 = 0, dx12 = 0, dx23 = 0;
  if( y3 != y1 ) 
  {
    dx13 = (double)( x3 - x1 ) / (double)( y3 - y1 );
  }
  else
  {
    dx13 = 0;
  }

  if( y2 != y1 ) 
  {
    dx12 = (double)( x2 - x1 ) / (double)( y2 - y1 );
  }
  else
  {
    dx12 = 0;
  }

  if( y3 != y2 ) 
  {
    dx23 = (double)( x3 - x2 ) / (double)( y3 - y2 );
  }
  else
  {
    dx23 = 0;
  }
  m_bCheckFlag = kAll;
  double wx1 = x1;
  double wx2 = wx1;
  for( int i = y1; i < y2; i++ )
  {
    int xMin = floor( wx1 + 0.5 );
    int xMax = floor( wx2 + 0.5 );
    if( xMin > xMax )
    {
      GDIExDCHelpers::swap( xMin, xMax );
    }
    if( xMax > xMAX ) xMax = xMAX;
    if( xMin < xMIN ) xMin = xMIN;
    if( isYOnScreen( i ) )
      alphaTriangleLine( xMin, xMax, i, nRow++ );
    else
      nRow++;
    wx1 += dx13;
    wx2 += dx12;
  }
  if( y1 == y2 )
  {
    wx1 = x1;
    wx2 = x2;
  }
  for( int i = y2; i <= y3; i++ )
  {
    int xMin = floor( wx1 + 0.5 );
    int xMax = floor( wx2 + 0.5 );
    if( xMin > xMax )
    {
      GDIExDCHelpers::swap( xMin, xMax );
    }
    if( xMax > xMAX ) xMax = xMAX;
    if( xMin < xMIN ) xMin = xMIN;
    if( isYOnScreen( i ) )
      alphaTriangleLine( xMin, xMax, i, nRow++ );
    else
      nRow++;
    wx1 += dx13;
    wx2 += dx23;
  }
  m_bCheckFlag = kNone;
  return true;
}

bool ExGiGDIDC::alphaPolyline( const POINT* apt, int cpt )
{
  if( !isAlphaMode() ) return polyline( apt, cpt );
  for( int i = 1; i < cpt; ++i )
  {
    alphaLine( apt[ i - 1 ].x, apt[ i - 1 ].y, apt[ i ].x, apt[ i ].y );
  }
  return true;
}

int recalcCoordinate( int x, int wOrig, int wCur )
{
  if( wOrig == wCur ) return x;
  double scale = wCur / (double)( wOrig );
  return floor( x * scale + 0.5 );
}

COLORREF ExGiGDIDC::processPixelColorRasterOperation( int x, int y, COLORREF color, DWORD rop )
{
  if( rop == 0 ) return color;
  if( rop & SRCCOPY ) return color;
  if( rop & BLACKNESS ) return 0;
  if( rop == WHITENESS ) return RGB( 255, 255, 255 );
  OdUInt8 r = GetRValue( color );
  OdUInt8 g = GetGValue( color );
  OdUInt8 b = GetBValue( color );
  COLORREF cl = getPixelColorForAlpha( x, y );
  OdUInt8 rOrig = GetRValue( cl );
  OdUInt8 gOrig = GetGValue( cl );
  OdUInt8 bOrig = GetBValue( cl );
  if( rop == SRCAND )
  {
    r = r & rOrig;
    b = b & bOrig;
    g = g & gOrig;
    return RGB( r, g, b );
  }
  if( rop == SRCPAINT )
  {
    r = r | rOrig;
    b = b | bOrig;
    g = g | gOrig;
    return RGB( r, g, b );
  }
  if( rop == SRCINVERT )
  {
    r = r ^ rOrig;
    b = b ^ bOrig;
    g = g ^ gOrig;
    return RGB( r, g, b );
  }
  if( rop == SRCERASE )
  {
    r = 255 - r;
    b = 255 - g;
    g = 255 - b;
    r = r & rOrig;
    b = b & bOrig;
    g = g & gOrig;
    return RGB( r, g, b );
  }
  if( rop == DSTINVERT )
  {
    r = 255 - rOrig;
    b = 255 - gOrig;
    g = 255 - bOrig;
    return RGB( r, g, b );
  }
  if( rop == MERGECOPY )
  {
    rOrig = GetRValue( m_color );
    bOrig = GetGValue( m_color );
    gOrig = GetBValue( m_color );
    r = r & rOrig;
    b = b & bOrig;
    g = g & gOrig;
    return RGB( r, g, b );
  }
  if( rop & MERGEPAINT )
  {
    r = 255 - r;
    b = 255 - g;
    g = 255 - b;
    r = r | rOrig;
    b = b | bOrig;
    g = g | gOrig;
    return RGB( r, g, b );
  }
  if( rop & NOTSRCCOPY )
  {
    return RGB( 255-r, 255-g, 255-b );
  }
  if( rop & NOTSRCERASE )
  {
    r = r | rOrig;
    b = b | bOrig;
    g = g | gOrig;
    return RGB( 255 - r, 255 - g, 255 - b );
  }
  ODA_FAIL();
  return color;
}

bool ExGiGDIDC::alphaStretchBlt( int xDest, int yDest, int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop )
{
  if( !isAlphaMode() ) return stretchBlt( xDest, yDest, wDest, hDest, hdcSrc, xSrc, ySrc, wSrc, hSrc, rop );
  bool bSameSize = ( wDest == wSrc && hDest == hSrc );
  for( int x = 0; x < wDest; ++x )
  {
    for( int y = 0; y < hDest; ++y )
    {
      int targetX = x + xDest;
      int targetY = y + yDest;
      if( !isPointOnScreen( targetX, targetY ) ) continue;
      int x1 = x + xSrc;
      int y1 = y + ySrc;
      if( !bSameSize )
      {
        x1 = xSrc + recalcCoordinate( x, wDest, wSrc );
        y1 = ySrc + recalcCoordinate( y, hDest, hSrc );
      }
      COLORREF color = ::GetPixel( hdcSrc, x1, y1 );
      color = processPixelColorRasterOperation( targetX, targetY, color, rop );
      alphaSetPixel( targetX, targetY, color );
    }
  }
  return true;
}

bool ExGiGDIDC::alphaStretchBits( int xDest, int yDest, int wDest, int hDest, const OdUInt8* pBits, int w, OdUInt8 format, OdUInt8 bpp, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop )
{
  if( !isAlphaMode() ) return false;
  bool bSameSize = ( wDest == wSrc && hDest == hSrc );
  for( int x = 0; x < wDest; ++x )
  {
    for( int y = 0; y < hDest; ++y )
    {
      int targetX = x + xDest;
      int targetY = y + yDest;
      if( !isPointOnScreen( targetX, targetY ) ) continue;
      int x1 = x + xSrc;
      int y1 = y + ySrc;
      if( !bSameSize )
      {
        x1 = xSrc + recalcCoordinate( x, wDest, wSrc );
        y1 = ySrc + recalcCoordinate( y, hDest, hSrc );
      }
      COLORREF color;
      OdUInt8 alpha = 255;
      if( !checkPixel( pBits, w, format, bpp, x1, y1, color, alpha ) ) continue;
      COLORREF bgColor = getPixelColorForAlpha( targetX, targetY );
      color = ::OdMergeRGBAlpha( bgColor, color, alpha );
      color = processPixelColorRasterOperation( targetX, targetY, color, rop );
      alphaSetPixel( targetX, targetY, color );
    }
  }
  return true;
}

bool ExGiGDIDC::stretchBitsMemDC( int xDest, int yDest, int wDest, int hDest, const OdUInt8* pBits, int w, OdUInt8 format, OdUInt8 bpp, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop )
{
  if( !isMirrorMode() ) return false;
  HDC memDC = memoryDC();
  if( isAlphaMode() )
  {
    ODA_FAIL();
    return false;
  }
  bool bSameSize = ( wDest == wSrc && hDest == hSrc );
  for( int x = 0; x < wDest; ++x )
  {
    for( int y = 0; y < hDest; ++y )
    {
      int targetX = x + xDest;
      int targetY = y + yDest;
      if( !isPointOnScreen( targetX, targetY ) ) continue;
      int x1 = x + xSrc;
      int y1 = y + ySrc;
      if( !bSameSize )
      {
        x1 = xSrc + recalcCoordinate( x, wDest, wSrc );
        y1 = ySrc + recalcCoordinate( y, hDest, hSrc );
      }
      COLORREF color;
      OdUInt8 alpha = 255;
      if( !checkPixel( pBits, w, format, bpp, x1, y1, color, alpha ) ) continue;
      COLORREF bgColor = getPixelColorForAlpha( targetX, targetY );
      color = ::OdMergeRGBAlpha( bgColor, color, alpha );
      color = processPixelColorRasterOperation( targetX, targetY, color, rop );
      ::SetPixel( memDC, targetX, targetY, color );
    }
  }
  return true;
}


bool ExGiGDIDC::checkPixel( const OdUInt8* pBits, int w, OdUInt8 format, OdUInt8 bpp, int x, int y, COLORREF& color, OdUInt8& alpha )
{
  if( !pBits || w < 1 || !( bpp == 24 || bpp == 32 ) ) return false;
  if( format != BI_RGB ) return false;
  int pos = y * w + x;
  if( bpp == 24 )
  {
    pos *= 3;
    color = RGB( pBits[ pos + 2 ], pBits[ pos + 1 ], pBits[ pos ] );
    alpha = 255;
  }
  //32 bpp
  pos *= 4;
  color = RGB( pBits[ pos + 2 ], pBits[ pos + 1 ], pBits[ pos ] );
  alpha = pBits[ pos + 3 ];
  return true;
}

bool ExGiGDIDC::processPresentedPoint( int x, int y )
{
  presentedPoint pt( x, y );
  std::pair<std::set< presentedPoint >::iterator, bool> insRes = m_presentedPoints.insert( pt );
  return insRes.second;
}

void ExGiGDIDC::syncDCStyles()
{
  if( !needSync() ) return;
  setNeedSync( false );
  HGDIOBJ curPen = ::GetCurrentObject( m_hTargetDC, OBJ_PEN );
  if( !curPen )
  {
    ODA_FAIL();
    return;
  }
#ifndef _WIN32_WCE
  int sizeRequired = ::GetObject( curPen, 0, NULL );
  OdVector<OdUInt8> buff; buff.resize( sizeRequired );
  if( ::GetObject( curPen, buff.size(), buff.asArrayPtr() ) == 0 )
  {
    ODA_FAIL();
    return;
  }
  const EXTLOGPEN& elp = *reinterpret_cast<const EXTLOGPEN*>( buff.asArrayPtr() );

  if( (elp.elpPenStyle & PS_STYLE_MASK ) != PS_USERSTYLE )
    m_styler.setGDIPenStyle( elp );
  else
    m_styler.setCustomPenStyle( elp.elpNumEntries, elp.elpStyleEntry );
#else
  LOGPEN lp;
  GetObject( curPen, sizeof( LOGPEN ), &lp );
  m_styler.setGDIPenStyle( lp );
#endif

  HGDIOBJ curBrush = ::GetCurrentObject( m_hTargetDC, OBJ_BRUSH );
  if( !curBrush )
  {
    ODA_FAIL();
    return;
  }
  LOGBRUSH lb;
  if( ::GetObject( curBrush, sizeof( LOGBRUSH ), &lb ) == 0 )
  {
    ODA_FAIL();
    return;
  }
  m_styler.setGDIBrushStyle( lb );
}
/*
/////// DEBUG STUFF

PBITMAPINFO CreateBitmapInfo( HBITMAP hBmp )
{
  BITMAP bmp;
  PBITMAPINFO pbmi;

  GetObject( hBmp, sizeof( BITMAP ), &bmp );

  pbmi = static_cast<PBITMAPINFO>( LocalAlloc( LPTR, sizeof( BITMAPINFOHEADER ) ) );

  pbmi->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
  pbmi->bmiHeader.biWidth = bmp.bmWidth;
  pbmi->bmiHeader.biHeight = bmp.bmHeight;
  pbmi->bmiHeader.biPlanes = bmp.bmPlanes; // we are assuming that there is only one plane
  pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

  // no compression this is an rgb bitmap
  pbmi->bmiHeader.biCompression = BI_RGB;

  // calculate size and align to a DWORD (8bit), we are assuming there is only one plane.
  pbmi->bmiHeader.biSizeImage = ( ( pbmi->bmiHeader.biWidth * bmp.bmBitsPixel + 31 ) & -31 ) * pbmi->bmiHeader.biHeight;

  // all device colours are important
  pbmi->bmiHeader.biClrImportant = 0;

  return pbmi;
}

void
WriteBmpTofile( const bool remote, LPCWSTR pszFile, PBITMAPINFO pbi, HBITMAP hBmp, HDC hDC )
{
  HANDLE hFile;
  BITMAPFILEHEADER hdr;
  PBITMAPINFOHEADER pbih;
  LPBYTE lpBits;
  DWORD dwTemp;

  pbih = (PBITMAPINFOHEADER)pbi;
  lpBits = (LPBYTE)GlobalAlloc( GMEM_FIXED, pbih->biSizeImage );

  if( !lpBits )
  {
    return; // could not allocate bitmap
  }

  GetDIBits( hDC, hBmp, 0, (WORD)pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS );

  hFile = CreateFile( pszFile,
    GENERIC_READ | GENERIC_WRITE,
    0,
    NULL,
    CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    NULL );

  if( hFile == INVALID_HANDLE_VALUE )
  {
    return; // Could not open screenshot file
  }

  // type == BM
  hdr.bfType = 0x4d42;

  hdr.bfSize = ( sizeof( BITMAPFILEHEADER ) + pbih->biSize + pbih->biClrUsed * sizeof( RGBQUAD ) + pbih->biSizeImage );
  hdr.bfReserved1 = 0;
  hdr.bfReserved2 = 0;

  hdr.bfOffBits = sizeof( BITMAPFILEHEADER ) + pbih->biSize + pbih->biClrUsed * sizeof( RGBQUAD );

  // write the bitmap file header to file
  WriteFile( hFile, (LPVOID)&hdr, sizeof( BITMAPFILEHEADER ), &dwTemp, NULL );

  // write the bitmap header to file
  WriteFile( hFile, (LPVOID)pbih, sizeof( BITMAPINFOHEADER ) + pbih->biClrUsed * sizeof( RGBQUAD ), &dwTemp, NULL );

  // copy the bitmap colour data into the file
  WriteFile( hFile, (LPSTR)lpBits, pbih->biSizeImage, &dwTemp, NULL );

  CloseHandle( hFile );

  GlobalFree( (HGLOBAL)lpBits );
}


void writeMemDC( const OdString& file, HBITMAP hBmp, HDC hdc )
{
  if( !hdc || !hBmp ) return;

  PBITMAPINFO pInf = CreateBitmapInfo( hBmp );

  WriteBmpTofile( false, file.c_str(), pInf, hBmp, hdc );

}*/


