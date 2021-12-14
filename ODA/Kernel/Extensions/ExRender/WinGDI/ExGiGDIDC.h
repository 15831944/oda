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
#ifndef EXGI_GDI_DC
#define EXGI_GDI_DC

#include "TD_PackPush.h"
#include "OdVector.h"
#define STL_USING_SET
#define STL_USING_MAP
#include "OdaSTL.h"
/*
  <group ExRender_Windows_Classes>
*/
class OdExGiGDIBytesReflector
{
  friend class ExGiGDIDC;
  void** pIn;
  void* pMirror;
public:
  OdExGiGDIBytesReflector( void** pBytes )
  {
    pIn = pBytes;
    pMirror = NULL;
  }

  void fillBy( const void* pSrc, size_t Size )
  {
    memcpy( *pIn, pSrc, Size );
    if( pMirror )
      memcpy( pMirror, pSrc, Size );
  }

  const void* in() const { return ( pIn ? *pIn : NULL ); }
  const void* mirror() const { return pMirror; }
};

/*
  <group ExRender_Windows_Classes>
*/
class ExGiGDIDC
{
  typedef std::pair< OdUInt32, OdUInt32 > PenStylePair; //<BitMask, Base>
  class OdGDIStyler
  {
    class GDIPenStyle
    {
      PenStylePair m_pair;
      DWORD m_sz;
      DWORD m_style[ 17 ];
      bool m_isCustom;
      OdUInt32 m_nStyleBase;
      DWORD m_width;
    public:
      GDIPenStyle();
      GDIPenStyle( const PenStylePair& );
      GDIPenStyle( DWORD sz, const DWORD* pStyle );

      void set( const PenStylePair& );
      void set( DWORD sz, const DWORD* pStyle );
      bool isPixelVisible( OdUInt32 nPixel ) const;

      void setWidth( DWORD w ) { m_width = w; }
      DWORD width() const { return m_width; }
    };

    class GDIBrushStyle
    {
      enum BrushType
      {
        kMono,
        kPattern
      };
      BrushType m_type;
      bool m_bVisibleMono;
      ULONG_PTR m_curPattern;
      OdUInt8 m_pattern[ 8 ];
#ifndef _WIN32_WCE
      void createPatternForHatch( OdUInt8 pat, bool bClear = true );
#endif
    public:
      GDIBrushStyle();
      ~GDIBrushStyle();

      void set( const LOGBRUSH& lb );
      bool isPixelVisible( OdUInt32 nRow, OdUInt32 nCol ) const;
    };

    GDIPenStyle m_penStyle;
    GDIBrushStyle m_brushStyle;

    enum GDIPenStyles
    {
      kSolid = 0,
      kDash,
      kDot,
      kDashDot,
      kDashDotDot,
      kDot3,
      kNull,
      kLastPenStyle
    };
    
    PenStylePair prePenStyles[ kLastPenStyle ];

    ExGiGDIDC* m_exdc;

  public:
    OdGDIStyler();
    ~OdGDIStyler();

    void setDC( ExGiGDIDC* pDc ) { m_exdc = pDc; }
#ifndef _WIN32_WCE
    void setGDIPenStyle( const EXTLOGPEN& elp );
#endif
    void setGDIPenStyle( const LOGPEN& pen );
    void setCustomPenStyle( DWORD sz, const DWORD* pStyle );
    DWORD getPenWidth() const { return m_penStyle.width(); }

    void setGDIBrushStyle( const LOGBRUSH& lb );

    bool isPenPixelVisible( OdUInt32 nPixel ) const;
    bool isBrushPixelVisible( OdUInt32 nRow, OdUInt32 nPixel ) const;
  };

  HDC m_hTargetDC;

  mutable HDC m_hMemDC;
  mutable HBITMAP m_memBitmap;
  mutable int m_width;
  mutable int m_height;

  enum ExDCFlags
  {
    kMirrorMode = 1,
    kAlphaMode = 1 << 1,
    kHasChanges = 1 << 2,
    kDirectMode = 1 << 3
  };

  OdGDIStyler m_styler;

  inline void setNeedSync( bool bNeed = true ) const { SETBIT( m_flags, kHasChanges, bNeed ); }
  inline bool needSync() const { return GETBIT( m_flags, kHasChanges ); }

  mutable OdVector< void* > m_memObjects;

  mutable OdUInt8 m_flags;
  OdUInt8 m_alpha;
  COLORREF m_color;

  bool initMemDC() const;
  void releaseMemDC( bool bResetSize = true ) const;

  //presented points processing
  struct presentedPoint
  {
    int x;
    int y;
    presentedPoint( int _x, int _y ) : x( _x ), y( _y ) {}
    bool operator<( const presentedPoint& pt ) const
    {
      if( y < pt.y ) return true;
      else if( y == pt.y )
      {
        if( x < pt.x ) return true;
      }
      return false;
    }
  };

  std::set< presentedPoint > m_presentedPoints;
  OdUInt8 m_bCheckFlag;
  enum PresentedPointsCheck
  {
    kNone = 0,
    kAll = 1,
    kEnd = 2
  };

  bool processPresentedPoint( int x, int y );

  //Styles
  void syncDCStyles();

  //Save/Restore DC
  std::map< int, int > m_memStack;

  inline bool isYOnScreen( int y ) const
  {
    return y >= 0 && y <= m_height;
  }

  inline bool isXOnScreen( int x ) const
  {
    return x >= 0 && x <= m_width;
  }

  inline bool isPointOnScreen( int x, int y ) const
  {
    return isXOnScreen( x ) && isYOnScreen( y );
  }

  bool checkPixel( const OdUInt8* pBits, int w, OdUInt8 format, OdUInt8 bpp, int x, int y, COLORREF& color, OdUInt8& alpha );
  COLORREF processPixelColorRasterOperation( int x, int y, COLORREF color, DWORD rop );

public:
  enum ExGDIDCTransparencyMode
  {
    kNoTransparency = 0,      //No transparency support
    kMirrorTransparency = 1,  //Transparency using additional DC
    kDirectTransparency = 2   //Transparency using the same DC
  };

  enum ExDCObjects
  {
    kBitmap = 0,
    kDIBSectionBitmap = 1,

    kUseTheSame,
    kAllObjects
  };

  ExGiGDIDC( ExGDIDCTransparencyMode transparencyMode );
  ~ExGiGDIDC();

  inline bool isMirrorMode() const { return GETBIT( m_flags, kMirrorMode ); }
  inline bool isDirectMode() const { return GETBIT( m_flags, kDirectMode ); }

  HDC targetDC() const { return m_hTargetDC; }
  HDC memoryDC() const { initMemDC(); return m_hMemDC; }

  HDC& setDC( HDC hdc ) { m_hTargetDC = hdc; releaseMemDC(); return m_hTargetDC; }

  void setSizes( int w, int h )
  { 
    m_width = w; m_height = h;
  }

  //Wrapped members
  HGDIOBJ selectObject( HGDIOBJ obj, ExDCObjects type = kUseTheSame );
  BOOL deleteMirrorObject( ExDCObjects type );
  COLORREF setBkColor( COLORREF color);
  int setBkMode( int mode );

  //Wrapped geometry methods
  bool lineTo( int x, int y );
  bool moveToEx( int x, int y, LPPOINT lpp );
  COLORREF setPixel( int x, int y, COLORREF color );
  bool polygon( const POINT* apt, int cpt );
  bool rectangle( int left, int top, int right, int bottom );
  bool polyline( const POINT* apt, int cpt );
#ifndef _WIN32_WCE
  bool polyPolygon( const POINT* apt, const int* asz, int csz );
  int setPolyFillMode( int mode );
  bool beginPath();
  bool polyDraw( const POINT* apt, const BYTE* aj, int cpt );
  bool endPath();
  bool strokeAndFillPath();
  int setTextCharacterExtra( int extra );
#endif
  UINT setTextAlign( UINT align );
  COLORREF setTextColor( COLORREF color );
  bool extTextOutW( int x, int y, UINT options, const RECT* lprect, LPCWSTR lpString, UINT c, const INT* lpDx );
#ifndef _WIN32_WCE
  bool setGraphicsMode( int mode );
  bool setWorldTransform( const XFORM* tr );
  bool modifyWorldTransform( const XFORM* tr, DWORD mode );
#endif
  int saveDC();
  bool restoreDC( int dc );
#ifndef _WIN32_WCE
  bool selectClipPath( int mode );
#endif
  int intersectClipRect( int left, int top, int right, int bottom );

  int fillRect( const RECT* lprc, HBRUSH hbr );
#ifndef _WIN32_WCE
  int setStretchBltMode( int mode );
#endif
  int stretchDIBits( int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth, int SrcHeight,
    const void* lpBits, const BITMAPINFO* lpbmi, UINT iUsage, DWORD rop );
  bool stretchBlt( int xDest, int yDest, int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop );
  bool bitBlt( int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop );
  bool maskBlt( int xDest, int yDest, int width, int height,
    HDC hdcSrc, int xSrc, int ySrc, HBITMAP hbmMask, int xMask, int yMask, DWORD rop );

  HBITMAP createDIBSection( const BITMAPINFO* pbmi, UINT usage, OdExGiGDIBytesReflector& refl, HANDLE hSection, DWORD offset );
  int setDIBitsToDevice( int xDest, int yDest, DWORD w, DWORD h, int xSrc,
     int ySrc, UINT StartScan, UINT cLines, const OdExGiGDIBytesReflector& refl, const BITMAPINFO* lpbmi, UINT ColorUse );
#ifndef _WIN32_WCE
  bool playEnhMetaFile( HENHMETAFILE hmf, const RECT* lprect );
#endif
  //Additional geometry methods
  bool triangle( const POINT* apt );

  //Transparency suport
  void setAlpha( OdUInt8 alpha, COLORREF color ) 
  { 
    m_alpha = alpha; 
    SETBIT( m_flags, kAlphaMode, alpha < 255 );
    m_color = color;
  }

  OdUInt8 getAlpha() const
  {
    return m_alpha;
  }

  inline bool isAlphaMode() const { return GETBIT( m_flags, kAlphaMode ) && ( isMirrorMode() || isDirectMode() ); }
  COLORREF getPixelColorForAlpha( int x, int y );

  bool alphaLineTo( int x, int y );
  bool alphaLine( int x1, int y1, int x2, int y2, bool bCheckWidth = true );
  bool alphaTriangleLine( int x1, int x2, int y, OdUInt32 nRow );
  COLORREF alphaSetPixel( int x, int y, COLORREF color );
  bool alphaTriangle( const POINT* apt );
  bool alphaPolyline( const POINT* apt, int cpt );

  bool alphaStretchBlt( int xDest, int yDest, int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop );
  bool alphaStretchBits( int xDest, int yDest, int wDest, int hDest, const OdUInt8* pBits, int w, OdUInt8 format, OdUInt8 bpp, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop );
  bool stretchBitsMemDC( int xDest, int yDest, int wDest, int hDest, const OdUInt8* pBits, int w, OdUInt8 format, OdUInt8 bpp, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop );

  void clearPresentedPoints() { m_presentedPoints.clear(); }
};

#include "TD_PackPop.h"
#endif //EXGI_GDI_DC
