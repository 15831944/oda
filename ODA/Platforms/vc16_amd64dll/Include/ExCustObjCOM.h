

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 20:14:07 2038
 */
/* Compiler settings for ..\..\..\..\..\..\Drawing\Examples\win\ExCustObjCOM\ExCustObjCOM.idl:
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


#ifndef __ExCustObjCOM_h__
#define __ExCustObjCOM_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IExCustEntityWrapper_FWD_DEFINED__
#define __IExCustEntityWrapper_FWD_DEFINED__
typedef interface IExCustEntityWrapper IExCustEntityWrapper;

#endif 	/* __IExCustEntityWrapper_FWD_DEFINED__ */


#ifndef __ExCustEntityWrapper_FWD_DEFINED__
#define __ExCustEntityWrapper_FWD_DEFINED__

#ifdef __cplusplus
typedef class ExCustEntityWrapper ExCustEntityWrapper;
#else
typedef struct ExCustEntityWrapper ExCustEntityWrapper;
#endif /* __cplusplus */

#endif 	/* __ExCustEntityWrapper_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __ExCustObjCOMLib_LIBRARY_DEFINED__
#define __ExCustObjCOMLib_LIBRARY_DEFINED__

/* library ExCustObjCOMLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ExCustObjCOMLib;

#ifndef __IExCustEntityWrapper_INTERFACE_DEFINED__
#define __IExCustEntityWrapper_INTERFACE_DEFINED__

/* interface IExCustEntityWrapper */
/* [object][oleautomation][helpstring][dual][uuid] */ 


EXTERN_C const IID IID_IExCustEntityWrapper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BFF2426C-A401-45D2-A5CA-63C756B4983E")
    IExCustEntityWrapper : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Count( 
            /* [in] */ SHORT newVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IExCustEntityWrapperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IExCustEntityWrapper * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IExCustEntityWrapper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IExCustEntityWrapper * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IExCustEntityWrapper * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IExCustEntityWrapper * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IExCustEntityWrapper * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IExCustEntityWrapper * This,
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
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IExCustEntityWrapper * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Count )( 
            IExCustEntityWrapper * This,
            /* [in] */ SHORT newVal);
        
        END_INTERFACE
    } IExCustEntityWrapperVtbl;

    interface IExCustEntityWrapper
    {
        CONST_VTBL struct IExCustEntityWrapperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IExCustEntityWrapper_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IExCustEntityWrapper_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IExCustEntityWrapper_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IExCustEntityWrapper_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IExCustEntityWrapper_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IExCustEntityWrapper_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IExCustEntityWrapper_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IExCustEntityWrapper_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IExCustEntityWrapper_put_Count(This,newVal)	\
    ( (This)->lpVtbl -> put_Count(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IExCustEntityWrapper_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ExCustEntityWrapper;

#ifdef __cplusplus

class DECLSPEC_UUID("8D0E4AE8-D8B1-4C26-92E9-264D1C9D5C79")
ExCustEntityWrapper;
#endif
#endif /* __ExCustObjCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


