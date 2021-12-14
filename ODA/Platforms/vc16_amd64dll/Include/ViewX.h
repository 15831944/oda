

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 20:14:07 2038
 */
/* Compiler settings for ..\..\..\..\..\..\Drawing\Examples\ActiveX\ViewX\ViewX.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ViewX_h__
#define __ViewX_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IOdaView_FWD_DEFINED__
#define __IOdaView_FWD_DEFINED__
typedef interface IOdaView IOdaView;

#endif 	/* __IOdaView_FWD_DEFINED__ */


#ifndef __IOdaDevice_FWD_DEFINED__
#define __IOdaDevice_FWD_DEFINED__
typedef interface IOdaDevice IOdaDevice;

#endif 	/* __IOdaDevice_FWD_DEFINED__ */


#ifndef __IOdaViewer_FWD_DEFINED__
#define __IOdaViewer_FWD_DEFINED__
typedef interface IOdaViewer IOdaViewer;

#endif 	/* __IOdaViewer_FWD_DEFINED__ */


#ifndef ___IOdaViewEvents_FWD_DEFINED__
#define ___IOdaViewEvents_FWD_DEFINED__
typedef interface _IOdaViewEvents _IOdaViewEvents;

#endif 	/* ___IOdaViewEvents_FWD_DEFINED__ */


#ifndef __OdaViewer_FWD_DEFINED__
#define __OdaViewer_FWD_DEFINED__

#ifdef __cplusplus
typedef class OdaViewer OdaViewer;
#else
typedef struct OdaViewer OdaViewer;
#endif /* __cplusplus */

#endif 	/* __OdaViewer_FWD_DEFINED__ */


#ifndef __OdaDevice_FWD_DEFINED__
#define __OdaDevice_FWD_DEFINED__

#ifdef __cplusplus
typedef class OdaDevice OdaDevice;
#else
typedef struct OdaDevice OdaDevice;
#endif /* __cplusplus */

#endif 	/* __OdaDevice_FWD_DEFINED__ */


#ifndef __OdaView_FWD_DEFINED__
#define __OdaView_FWD_DEFINED__

#ifdef __cplusplus
typedef class OdaView OdaView;
#else
typedef struct OdaView OdaView;
#endif /* __cplusplus */

#endif 	/* __OdaView_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "odax.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_ViewX_0000_0000 */
/* [local] */ 

typedef /* [uuid] */  DECLSPEC_UUID("B8DC775B-D482-4A2C-BCF7-9BCF57A06A05") 
enum Projection
    {
        kParallel	= 0,
        kPerspective	= 1
    } 	OdaProjection;

typedef /* [public][public][uuid] */  DECLSPEC_UUID("DF82EC11-FE14-49B9-9BDC-BFE0FC0D5688") 
enum __MIDL___MIDL_itf_ViewX_0000_0000_0001
    {
        kBoundingBox	= -1,
        k2DOptimized	= 0,
        kWireframe	= 1,
        kHiddenLine	= 2,
        kFlatShaded	= 3,
        kGouraudShaded	= 4,
        kFlatShadedWithWireframe	= 5,
        kGouraudShadedWithWireframe	= 6
    } 	OdaRenderMode;



extern RPC_IF_HANDLE __MIDL_itf_ViewX_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ViewX_0000_0000_v0_0_s_ifspec;

#ifndef __IOdaView_INTERFACE_DEFINED__
#define __IOdaView_INTERFACE_DEFINED__

/* interface IOdaView */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IOdaView;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8C1328BF-AEE8-401F-965A-A80D68A59174")
    IOdaView : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LensLength( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LensLength( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ViewportBorderVisible( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ViewportBorderVisible( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Viewport( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Viewport( 
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsPerspective( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Update( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Zoom( 
            double Factor) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Pan( 
            double X,
            double Y) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Orbit( 
            double X,
            double Y) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Roll( 
            double Angle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Dolly( 
            double X,
            double Y,
            double Z) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ZoomExtents( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ZoomAll( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ViewportBorderWeight( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ViewportBorderWeight( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ViewportBorderColor( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ViewportBorderColor( 
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMode( 
            OdaRenderMode rm) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DatabaseViewport( 
            /* [retval][out] */ IDispatch **pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IOdaViewVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOdaView * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOdaView * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOdaView * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOdaView * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOdaView * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOdaView * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOdaView * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LensLength )( 
            IOdaView * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LensLength )( 
            IOdaView * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ViewportBorderVisible )( 
            IOdaView * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ViewportBorderVisible )( 
            IOdaView * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Viewport )( 
            IOdaView * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Viewport )( 
            IOdaView * This,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsPerspective )( 
            IOdaView * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Update )( 
            IOdaView * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Zoom )( 
            IOdaView * This,
            double Factor);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Pan )( 
            IOdaView * This,
            double X,
            double Y);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Orbit )( 
            IOdaView * This,
            double X,
            double Y);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Roll )( 
            IOdaView * This,
            double Angle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Dolly )( 
            IOdaView * This,
            double X,
            double Y,
            double Z);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ZoomExtents )( 
            IOdaView * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ZoomAll )( 
            IOdaView * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ViewportBorderWeight )( 
            IOdaView * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ViewportBorderWeight )( 
            IOdaView * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ViewportBorderColor )( 
            IOdaView * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ViewportBorderColor )( 
            IOdaView * This,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMode )( 
            IOdaView * This,
            OdaRenderMode rm);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DatabaseViewport )( 
            IOdaView * This,
            /* [retval][out] */ IDispatch **pVal);
        
        END_INTERFACE
    } IOdaViewVtbl;

    interface IOdaView
    {
        CONST_VTBL struct IOdaViewVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOdaView_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOdaView_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOdaView_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOdaView_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOdaView_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOdaView_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOdaView_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IOdaView_get_LensLength(This,pVal)	\
    ( (This)->lpVtbl -> get_LensLength(This,pVal) ) 

#define IOdaView_put_LensLength(This,newVal)	\
    ( (This)->lpVtbl -> put_LensLength(This,newVal) ) 

#define IOdaView_get_ViewportBorderVisible(This,pVal)	\
    ( (This)->lpVtbl -> get_ViewportBorderVisible(This,pVal) ) 

#define IOdaView_put_ViewportBorderVisible(This,newVal)	\
    ( (This)->lpVtbl -> put_ViewportBorderVisible(This,newVal) ) 

#define IOdaView_get_Viewport(This,pVal)	\
    ( (This)->lpVtbl -> get_Viewport(This,pVal) ) 

#define IOdaView_put_Viewport(This,newVal)	\
    ( (This)->lpVtbl -> put_Viewport(This,newVal) ) 

#define IOdaView_get_IsPerspective(This,pVal)	\
    ( (This)->lpVtbl -> get_IsPerspective(This,pVal) ) 

#define IOdaView_Update(This)	\
    ( (This)->lpVtbl -> Update(This) ) 

#define IOdaView_Zoom(This,Factor)	\
    ( (This)->lpVtbl -> Zoom(This,Factor) ) 

#define IOdaView_Pan(This,X,Y)	\
    ( (This)->lpVtbl -> Pan(This,X,Y) ) 

#define IOdaView_Orbit(This,X,Y)	\
    ( (This)->lpVtbl -> Orbit(This,X,Y) ) 

#define IOdaView_Roll(This,Angle)	\
    ( (This)->lpVtbl -> Roll(This,Angle) ) 

#define IOdaView_Dolly(This,X,Y,Z)	\
    ( (This)->lpVtbl -> Dolly(This,X,Y,Z) ) 

#define IOdaView_ZoomExtents(This)	\
    ( (This)->lpVtbl -> ZoomExtents(This) ) 

#define IOdaView_ZoomAll(This)	\
    ( (This)->lpVtbl -> ZoomAll(This) ) 

#define IOdaView_get_ViewportBorderWeight(This,pVal)	\
    ( (This)->lpVtbl -> get_ViewportBorderWeight(This,pVal) ) 

#define IOdaView_put_ViewportBorderWeight(This,newVal)	\
    ( (This)->lpVtbl -> put_ViewportBorderWeight(This,newVal) ) 

#define IOdaView_get_ViewportBorderColor(This,pVal)	\
    ( (This)->lpVtbl -> get_ViewportBorderColor(This,pVal) ) 

#define IOdaView_put_ViewportBorderColor(This,newVal)	\
    ( (This)->lpVtbl -> put_ViewportBorderColor(This,newVal) ) 

#define IOdaView_SetMode(This,rm)	\
    ( (This)->lpVtbl -> SetMode(This,rm) ) 

#define IOdaView_get_DatabaseViewport(This,pVal)	\
    ( (This)->lpVtbl -> get_DatabaseViewport(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOdaView_INTERFACE_DEFINED__ */


#ifndef __IOdaDevice_INTERFACE_DEFINED__
#define __IOdaDevice_INTERFACE_DEFINED__

/* interface IOdaDevice */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IOdaDevice;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FDFF0AAA-6631-4BDB-8A89-7799B33F3C6B")
    IOdaDevice : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ IOdaView **pView) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ViewCount( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetupActiveLayoutViews( 
            /* [in] */ IAcadDatabase *pBase,
            /* [retval][out] */ IOdaView **pView) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetupLayoutViews( 
            /* [in] */ IAcadLayout *pLayout,
            /* [retval][out] */ IOdaView **pView) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Update( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnSize( 
            long Width,
            long Height) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BackgroundColor( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BackgroundColor( 
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveView( 
            /* [retval][out] */ IOdaView **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UseCache( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_UseCache( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Regen( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IOdaDeviceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOdaDevice * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOdaDevice * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOdaDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOdaDevice * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOdaDevice * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOdaDevice * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOdaDevice * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Item )( 
            IOdaDevice * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ IOdaView **pView);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ViewCount )( 
            IOdaDevice * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetupActiveLayoutViews )( 
            IOdaDevice * This,
            /* [in] */ IAcadDatabase *pBase,
            /* [retval][out] */ IOdaView **pView);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetupLayoutViews )( 
            IOdaDevice * This,
            /* [in] */ IAcadLayout *pLayout,
            /* [retval][out] */ IOdaView **pView);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Update )( 
            IOdaDevice * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IOdaDevice * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnSize )( 
            IOdaDevice * This,
            long Width,
            long Height);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BackgroundColor )( 
            IOdaDevice * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BackgroundColor )( 
            IOdaDevice * This,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveView )( 
            IOdaDevice * This,
            /* [retval][out] */ IOdaView **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_UseCache )( 
            IOdaDevice * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_UseCache )( 
            IOdaDevice * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Regen )( 
            IOdaDevice * This);
        
        END_INTERFACE
    } IOdaDeviceVtbl;

    interface IOdaDevice
    {
        CONST_VTBL struct IOdaDeviceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOdaDevice_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOdaDevice_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOdaDevice_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOdaDevice_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOdaDevice_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOdaDevice_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOdaDevice_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IOdaDevice_Item(This,index,pView)	\
    ( (This)->lpVtbl -> Item(This,index,pView) ) 

#define IOdaDevice_get_ViewCount(This,pVal)	\
    ( (This)->lpVtbl -> get_ViewCount(This,pVal) ) 

#define IOdaDevice_SetupActiveLayoutViews(This,pBase,pView)	\
    ( (This)->lpVtbl -> SetupActiveLayoutViews(This,pBase,pView) ) 

#define IOdaDevice_SetupLayoutViews(This,pLayout,pView)	\
    ( (This)->lpVtbl -> SetupLayoutViews(This,pLayout,pView) ) 

#define IOdaDevice_Update(This)	\
    ( (This)->lpVtbl -> Update(This) ) 

#define IOdaDevice_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define IOdaDevice_OnSize(This,Width,Height)	\
    ( (This)->lpVtbl -> OnSize(This,Width,Height) ) 

#define IOdaDevice_get_BackgroundColor(This,pVal)	\
    ( (This)->lpVtbl -> get_BackgroundColor(This,pVal) ) 

#define IOdaDevice_put_BackgroundColor(This,newVal)	\
    ( (This)->lpVtbl -> put_BackgroundColor(This,newVal) ) 

#define IOdaDevice_get_ActiveView(This,pVal)	\
    ( (This)->lpVtbl -> get_ActiveView(This,pVal) ) 

#define IOdaDevice_get_UseCache(This,pVal)	\
    ( (This)->lpVtbl -> get_UseCache(This,pVal) ) 

#define IOdaDevice_put_UseCache(This,newVal)	\
    ( (This)->lpVtbl -> put_UseCache(This,newVal) ) 

#define IOdaDevice_Regen(This)	\
    ( (This)->lpVtbl -> Regen(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOdaDevice_INTERFACE_DEFINED__ */


#ifndef __IOdaViewer_INTERFACE_DEFINED__
#define __IOdaViewer_INTERFACE_DEFINED__

/* interface IOdaViewer */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IOdaViewer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3B08F722-E57E-4D0B-BCF5-1361D12CE817")
    IOdaViewer : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NewDevice( 
            /* [in] */ VARIANT DevicePath,
            /* [retval][out] */ IOdaDevice **pDev) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IOdaViewerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOdaViewer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOdaViewer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOdaViewer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOdaViewer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOdaViewer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOdaViewer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOdaViewer * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NewDevice )( 
            IOdaViewer * This,
            /* [in] */ VARIANT DevicePath,
            /* [retval][out] */ IOdaDevice **pDev);
        
        END_INTERFACE
    } IOdaViewerVtbl;

    interface IOdaViewer
    {
        CONST_VTBL struct IOdaViewerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOdaViewer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOdaViewer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOdaViewer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOdaViewer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOdaViewer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOdaViewer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOdaViewer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IOdaViewer_NewDevice(This,DevicePath,pDev)	\
    ( (This)->lpVtbl -> NewDevice(This,DevicePath,pDev) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOdaViewer_INTERFACE_DEFINED__ */



#ifndef __VIEWXLib_LIBRARY_DEFINED__
#define __VIEWXLib_LIBRARY_DEFINED__

/* library VIEWXLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_VIEWXLib;

#ifndef ___IOdaViewEvents_DISPINTERFACE_DEFINED__
#define ___IOdaViewEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IOdaViewEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IOdaViewEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9BB70937-3D24-4706-87CB-5E720EEF1D28")
    _IOdaViewEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IOdaViewEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IOdaViewEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IOdaViewEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IOdaViewEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IOdaViewEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IOdaViewEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IOdaViewEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IOdaViewEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _IOdaViewEventsVtbl;

    interface _IOdaViewEvents
    {
        CONST_VTBL struct _IOdaViewEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IOdaViewEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IOdaViewEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IOdaViewEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IOdaViewEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IOdaViewEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IOdaViewEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IOdaViewEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IOdaViewEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_OdaViewer;

#ifdef __cplusplus

class DECLSPEC_UUID("AF7504B8-9930-4975-97B9-12613B151212")
OdaViewer;
#endif

EXTERN_C const CLSID CLSID_OdaDevice;

#ifdef __cplusplus

class DECLSPEC_UUID("288323C9-137F-4265-87E4-53932C1E99D9")
OdaDevice;
#endif

EXTERN_C const CLSID CLSID_OdaView;

#ifdef __cplusplus

class DECLSPEC_UUID("25F3621F-5727-45D8-9873-5230FB11E5CE")
OdaView;
#endif
#endif /* __VIEWXLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

unsigned long             __RPC_USER  VARIANT_UserSize64(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal64(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal64(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree64(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


