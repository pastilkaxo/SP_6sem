

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 06:14:07 2038
 */
 /* Compiler settings for IComTest.idl:
     Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0628
     protocol : dce , ms_ext, c_ext, robust
     error checks: allocation ref bounds_check enum stub_data
     VC __declspec() decoration level:
          __declspec(uuid()), __declspec(selectany), __declspec(novtable)
          DECLSPEC_UUID(), MIDL_INTERFACE()
 */
 /* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#ifndef __IComTest_h__
#define __IComTest_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */

#ifndef __IComTest_FWD_DEFINED__
#define __IComTest_FWD_DEFINED__
typedef interface IComTest IComTest;

#endif 	/* __IComTest_FWD_DEFINED__ */


#ifndef __ILVO_FWD_DEFINED__
#define __ILVO_FWD_DEFINED__
typedef interface ILVO ILVO;

#endif 	/* __ILVO_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C" {
#endif 


#ifndef __IComTest_INTERFACE_DEFINED__
#define __IComTest_INTERFACE_DEFINED__

    /* interface IComTest */
    /* [helpstring][version][uuid][object] */


    EXTERN_C const IID IID_IComTest;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C0C62619-3BC1-4095-9B9A-84503E37DAA5")
        IComTest : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE WhoAmI(
            /* [out] */ LPWSTR * pwszWhoAmI) = 0;

    };


#else 	/* C style interface */

    typedef struct IComTestVtbl
    {
        BEGIN_INTERFACE

            DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
            HRESULT(STDMETHODCALLTYPE* QueryInterface)(
                IComTest* This,
                /* [in] */ REFIID riid,
                /* [annotation][iid_is][out] */
                _COM_Outptr_  void** ppvObject);

        DECLSPEC_XFGVIRT(IUnknown, AddRef)
            ULONG(STDMETHODCALLTYPE* AddRef)(
                IComTest* This);

        DECLSPEC_XFGVIRT(IUnknown, Release)
            ULONG(STDMETHODCALLTYPE* Release)(
                IComTest* This);

        DECLSPEC_XFGVIRT(IComTest, WhoAmI)
            HRESULT(STDMETHODCALLTYPE* WhoAmI)(
                IComTest* This,
                /* [out] */ LPWSTR* pwszWhoAmI);

        END_INTERFACE
    } IComTestVtbl;

    interface IComTest
    {
        CONST_VTBL struct IComTestVtbl* lpVtbl;
    };



#ifdef COBJMACROS


#define IComTest_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IComTest_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IComTest_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IComTest_WhoAmI(This,pwszWhoAmI)	\
    ( (This)->lpVtbl -> WhoAmI(This,pwszWhoAmI) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IComTest_INTERFACE_DEFINED__ */


#ifndef __ILVO_INTERFACE_DEFINED__
#define __ILVO_INTERFACE_DEFINED__

    /* interface ILVO */
    /* [helpstring][version][uuid][object] */


    EXTERN_C const IID IID_ILVO;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A7F2B8F3-95D2-4D7B-A1CC-3A20D8769AC6")
        ILVO : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE PowerOfN(
            /* [in] */ double x,
            /* [out] */ double* x5,
            /* [out] */ double* x13) = 0;

    };


#else 	/* C style interface */

    typedef struct ILVOVtbl
    {
        BEGIN_INTERFACE

            DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
            HRESULT(STDMETHODCALLTYPE* QueryInterface)(
                ILVO* This,
                /* [in] */ REFIID riid,
                /* [annotation][iid_is][out] */
                _COM_Outptr_  void** ppvObject);

        DECLSPEC_XFGVIRT(IUnknown, AddRef)
            ULONG(STDMETHODCALLTYPE* AddRef)(
                ILVO* This);

        DECLSPEC_XFGVIRT(IUnknown, Release)
            ULONG(STDMETHODCALLTYPE* Release)(
                ILVO* This);

        DECLSPEC_XFGVIRT(ILVO, PowerOfN)
            HRESULT(STDMETHODCALLTYPE* PowerOfN)(
                ILVO* This,
                /* [in] */ double x,
                /* [out] */ double* x5,
                /* [out] */ double* x13);

        END_INTERFACE
    } ILVOVtbl;

    interface ILVO
    {
        CONST_VTBL struct ILVOVtbl* lpVtbl;
    };



#ifdef COBJMACROS


#define ILVO_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILVO_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILVO_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILVO_PowerOfN(This,x,x5,x13)	\
    ( (This)->lpVtbl -> PowerOfN(This,x,x5,x13) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILVO_INTERFACE_DEFINED__ */


    /* Additional Prototypes for ALL interfaces */

    /* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


