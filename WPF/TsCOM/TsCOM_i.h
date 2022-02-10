

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 20:14:07 2038
 */
/* Compiler settings for TsCOM.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
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

#ifndef __TsCOM_i_h__
#define __TsCOM_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPlayer_FWD_DEFINED__
#define __IPlayer_FWD_DEFINED__
typedef interface IPlayer IPlayer;

#endif 	/* __IPlayer_FWD_DEFINED__ */


#ifndef __IPosition_FWD_DEFINED__
#define __IPosition_FWD_DEFINED__
typedef interface IPosition IPosition;

#endif 	/* __IPosition_FWD_DEFINED__ */


#ifndef __ITile_FWD_DEFINED__
#define __ITile_FWD_DEFINED__
typedef interface ITile ITile;

#endif 	/* __ITile_FWD_DEFINED__ */


#ifndef __IGameDescriptor_FWD_DEFINED__
#define __IGameDescriptor_FWD_DEFINED__
typedef interface IGameDescriptor IGameDescriptor;

#endif 	/* __IGameDescriptor_FWD_DEFINED__ */


#ifndef __ITurnCallback_FWD_DEFINED__
#define __ITurnCallback_FWD_DEFINED__
typedef interface ITurnCallback ITurnCallback;

#endif 	/* __ITurnCallback_FWD_DEFINED__ */


#ifndef __IGameView_FWD_DEFINED__
#define __IGameView_FWD_DEFINED__
typedef interface IGameView IGameView;

#endif 	/* __IGameView_FWD_DEFINED__ */


#ifndef __IEngine_FWD_DEFINED__
#define __IEngine_FWD_DEFINED__
typedef interface IEngine IEngine;

#endif 	/* __IEngine_FWD_DEFINED__ */


#ifndef __Player_FWD_DEFINED__
#define __Player_FWD_DEFINED__

#ifdef __cplusplus
typedef class Player Player;
#else
typedef struct Player Player;
#endif /* __cplusplus */

#endif 	/* __Player_FWD_DEFINED__ */


#ifndef __Position_FWD_DEFINED__
#define __Position_FWD_DEFINED__

#ifdef __cplusplus
typedef class Position Position;
#else
typedef struct Position Position;
#endif /* __cplusplus */

#endif 	/* __Position_FWD_DEFINED__ */


#ifndef __Tile_FWD_DEFINED__
#define __Tile_FWD_DEFINED__

#ifdef __cplusplus
typedef class Tile Tile;
#else
typedef struct Tile Tile;
#endif /* __cplusplus */

#endif 	/* __Tile_FWD_DEFINED__ */


#ifndef __GameDescriptor_FWD_DEFINED__
#define __GameDescriptor_FWD_DEFINED__

#ifdef __cplusplus
typedef class GameDescriptor GameDescriptor;
#else
typedef struct GameDescriptor GameDescriptor;
#endif /* __cplusplus */

#endif 	/* __GameDescriptor_FWD_DEFINED__ */


#ifndef __TurnCallback_FWD_DEFINED__
#define __TurnCallback_FWD_DEFINED__

#ifdef __cplusplus
typedef class TurnCallback TurnCallback;
#else
typedef struct TurnCallback TurnCallback;
#endif /* __cplusplus */

#endif 	/* __TurnCallback_FWD_DEFINED__ */


#ifndef __GameView_FWD_DEFINED__
#define __GameView_FWD_DEFINED__

#ifdef __cplusplus
typedef class GameView GameView;
#else
typedef struct GameView GameView;
#endif /* __cplusplus */

#endif 	/* __GameView_FWD_DEFINED__ */


#ifndef __Engine_FWD_DEFINED__
#define __Engine_FWD_DEFINED__

#ifdef __cplusplus
typedef class Engine Engine;
#else
typedef struct Engine Engine;
#endif /* __cplusplus */

#endif 	/* __Engine_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_TsCOM_0000_0000 */
/* [local] */ 

typedef /* [public][public][public] */ 
enum __MIDL___MIDL_itf_TsCOM_0000_0000_0001
    {
        Local	= 0,
        Computer	= ( Local + 1 ) ,
        Remote	= ( Computer + 1 ) 
    } 	PlayerType;

typedef /* [public][public][public][public][public][public] */ 
enum __MIDL___MIDL_itf_TsCOM_0000_0000_0002
    {
        Home	= 0,
        Away	= ( Home + 1 ) 
    } 	PlayerTeam;

typedef /* [public][public][public] */ 
enum __MIDL___MIDL_itf_TsCOM_0000_0000_0003
    {
        None	= 0,
        Square	= 1,
        Line	= ( Square + 1 ) ,
        BackwardsL	= ( Line + 1 ) ,
        LShape	= ( BackwardsL + 1 ) ,
        Downstep	= ( LShape + 1 ) ,
        Upstep	= ( Downstep + 1 ) ,
        Podium	= ( Upstep + 1 ) 
    } 	TetradShape;

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_TsCOM_0000_0000_0004
    {
        NoDirection	= 0,
        Up	= ( NoDirection + 1 ) ,
        Down	= ( Up + 1 ) ,
        Left	= ( Down + 1 ) ,
        Right	= ( Left + 1 ) 
    } 	Direction;



extern RPC_IF_HANDLE __MIDL_itf_TsCOM_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_TsCOM_0000_0000_v0_0_s_ifspec;

#ifndef __IPlayer_INTERFACE_DEFINED__
#define __IPlayer_INTERFACE_DEFINED__

/* interface IPlayer */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IPlayer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("018886A8-D3F7-4497-ACF2-AF5C6CE0CDF9")
    IPlayer : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ PlayerType *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Type( 
            /* [in] */ PlayerType newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Identity( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Identity( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Team( 
            /* [retval][out] */ PlayerTeam *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Team( 
            /* [in] */ PlayerTeam newVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE HasTetrad( 
            /* [in] */ TetradShape shape,
            /* [retval][out] */ ULONG *result) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_RgbColor( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_RgbColor( 
            /* [in] */ ULONG newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IPlayerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPlayer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPlayer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPlayer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPlayer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPlayer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPlayer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPlayer * This,
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IPlayer * This,
            /* [retval][out] */ PlayerType *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Type )( 
            IPlayer * This,
            /* [in] */ PlayerType newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Identity )( 
            IPlayer * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Identity )( 
            IPlayer * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Team )( 
            IPlayer * This,
            /* [retval][out] */ PlayerTeam *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Team )( 
            IPlayer * This,
            /* [in] */ PlayerTeam newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *HasTetrad )( 
            IPlayer * This,
            /* [in] */ TetradShape shape,
            /* [retval][out] */ ULONG *result);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RgbColor )( 
            IPlayer * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RgbColor )( 
            IPlayer * This,
            /* [in] */ ULONG newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IPlayer * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IPlayer * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IPlayerVtbl;

    interface IPlayer
    {
        CONST_VTBL struct IPlayerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPlayer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPlayer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPlayer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPlayer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPlayer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPlayer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPlayer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IPlayer_get_Type(This,pVal)	\
    ( (This)->lpVtbl -> get_Type(This,pVal) ) 

#define IPlayer_put_Type(This,newVal)	\
    ( (This)->lpVtbl -> put_Type(This,newVal) ) 

#define IPlayer_get_Identity(This,pVal)	\
    ( (This)->lpVtbl -> get_Identity(This,pVal) ) 

#define IPlayer_put_Identity(This,newVal)	\
    ( (This)->lpVtbl -> put_Identity(This,newVal) ) 

#define IPlayer_get_Team(This,pVal)	\
    ( (This)->lpVtbl -> get_Team(This,pVal) ) 

#define IPlayer_put_Team(This,newVal)	\
    ( (This)->lpVtbl -> put_Team(This,newVal) ) 

#define IPlayer_HasTetrad(This,shape,result)	\
    ( (This)->lpVtbl -> HasTetrad(This,shape,result) ) 

#define IPlayer_get_RgbColor(This,pVal)	\
    ( (This)->lpVtbl -> get_RgbColor(This,pVal) ) 

#define IPlayer_put_RgbColor(This,newVal)	\
    ( (This)->lpVtbl -> put_RgbColor(This,newVal) ) 

#define IPlayer_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#define IPlayer_put_Name(This,newVal)	\
    ( (This)->lpVtbl -> put_Name(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPlayer_INTERFACE_DEFINED__ */


#ifndef __IPosition_INTERFACE_DEFINED__
#define __IPosition_INTERFACE_DEFINED__

/* interface IPosition */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IPosition;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("365A8944-25B4-45B2-B65D-680E05D11D2E")
    IPosition : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_X( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_X( 
            /* [in] */ SHORT newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Y( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Y( 
            /* [in] */ SHORT newVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IPositionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPosition * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPosition * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPosition * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPosition * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPosition * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPosition * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPosition * This,
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_X )( 
            IPosition * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_X )( 
            IPosition * This,
            /* [in] */ SHORT newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Y )( 
            IPosition * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Y )( 
            IPosition * This,
            /* [in] */ SHORT newVal);
        
        END_INTERFACE
    } IPositionVtbl;

    interface IPosition
    {
        CONST_VTBL struct IPositionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPosition_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPosition_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPosition_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPosition_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPosition_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPosition_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPosition_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IPosition_get_X(This,pVal)	\
    ( (This)->lpVtbl -> get_X(This,pVal) ) 

#define IPosition_put_X(This,newVal)	\
    ( (This)->lpVtbl -> put_X(This,newVal) ) 

#define IPosition_get_Y(This,pVal)	\
    ( (This)->lpVtbl -> get_Y(This,pVal) ) 

#define IPosition_put_Y(This,newVal)	\
    ( (This)->lpVtbl -> put_Y(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPosition_INTERFACE_DEFINED__ */


#ifndef __ITile_INTERFACE_DEFINED__
#define __ITile_INTERFACE_DEFINED__

/* interface ITile */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ITile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B2A02345-3072-43CA-BDD0-E9117184B0BE")
    ITile : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Id( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [retval][out] */ IPosition **pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Position( 
            /* [in] */ IPosition *newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Team( 
            /* [retval][out] */ PlayerTeam *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTargets( 
            /* [retval][out] */ SAFEARRAY * *targets) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITile * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITile * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITile * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITile * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITile * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITile * This,
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            ITile * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            ITile * This,
            /* [retval][out] */ IPosition **pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Position )( 
            ITile * This,
            /* [in] */ IPosition *newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Team )( 
            ITile * This,
            /* [retval][out] */ PlayerTeam *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTargets )( 
            ITile * This,
            /* [retval][out] */ SAFEARRAY * *targets);
        
        END_INTERFACE
    } ITileVtbl;

    interface ITile
    {
        CONST_VTBL struct ITileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITile_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITile_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITile_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITile_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITile_get_Id(This,pVal)	\
    ( (This)->lpVtbl -> get_Id(This,pVal) ) 

#define ITile_get_Position(This,pVal)	\
    ( (This)->lpVtbl -> get_Position(This,pVal) ) 

#define ITile_put_Position(This,newVal)	\
    ( (This)->lpVtbl -> put_Position(This,newVal) ) 

#define ITile_get_Team(This,pVal)	\
    ( (This)->lpVtbl -> get_Team(This,pVal) ) 

#define ITile_GetTargets(This,targets)	\
    ( (This)->lpVtbl -> GetTargets(This,targets) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITile_INTERFACE_DEFINED__ */


#ifndef __IGameDescriptor_INTERFACE_DEFINED__
#define __IGameDescriptor_INTERFACE_DEFINED__

/* interface IGameDescriptor */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IGameDescriptor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0DB9F163-CC13-4AAB-B320-FB0A15F4D0E5")
    IGameDescriptor : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Id( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_FriendlyName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTiles( 
            /* [retval][out] */ SAFEARRAY * *tiles) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetHomePlayer( 
            /* [retval][out] */ IPlayer **player) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAwayPlayer( 
            /* [retval][out] */ IPlayer **player) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTurn( 
            /* [retval][out] */ PlayerTeam *team) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IGameDescriptorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGameDescriptor * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGameDescriptor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGameDescriptor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGameDescriptor * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGameDescriptor * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGameDescriptor * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGameDescriptor * This,
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IGameDescriptor * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FriendlyName )( 
            IGameDescriptor * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTiles )( 
            IGameDescriptor * This,
            /* [retval][out] */ SAFEARRAY * *tiles);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetHomePlayer )( 
            IGameDescriptor * This,
            /* [retval][out] */ IPlayer **player);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAwayPlayer )( 
            IGameDescriptor * This,
            /* [retval][out] */ IPlayer **player);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTurn )( 
            IGameDescriptor * This,
            /* [retval][out] */ PlayerTeam *team);
        
        END_INTERFACE
    } IGameDescriptorVtbl;

    interface IGameDescriptor
    {
        CONST_VTBL struct IGameDescriptorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGameDescriptor_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGameDescriptor_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGameDescriptor_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGameDescriptor_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IGameDescriptor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IGameDescriptor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IGameDescriptor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IGameDescriptor_get_Id(This,pVal)	\
    ( (This)->lpVtbl -> get_Id(This,pVal) ) 

#define IGameDescriptor_get_FriendlyName(This,pVal)	\
    ( (This)->lpVtbl -> get_FriendlyName(This,pVal) ) 

#define IGameDescriptor_GetTiles(This,tiles)	\
    ( (This)->lpVtbl -> GetTiles(This,tiles) ) 

#define IGameDescriptor_GetHomePlayer(This,player)	\
    ( (This)->lpVtbl -> GetHomePlayer(This,player) ) 

#define IGameDescriptor_GetAwayPlayer(This,player)	\
    ( (This)->lpVtbl -> GetAwayPlayer(This,player) ) 

#define IGameDescriptor_GetTurn(This,team)	\
    ( (This)->lpVtbl -> GetTurn(This,team) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGameDescriptor_INTERFACE_DEFINED__ */


#ifndef __ITurnCallback_INTERFACE_DEFINED__
#define __ITurnCallback_INTERFACE_DEFINED__

/* interface ITurnCallback */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ITurnCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FEF9704D-75B0-46E5-A320-15ADEC9C869D")
    ITurnCallback : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LocalTurnComplete( 
            /* [in] */ ITile *tile,
            IPosition *target) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AnimationComplete( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetRemoteGameComplete( 
            /* [in] */ BSTR game) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE PutRemoteGameComplete( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DeleteRemoteGameComplete( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITurnCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITurnCallback * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITurnCallback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITurnCallback * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITurnCallback * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITurnCallback * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITurnCallback * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITurnCallback * This,
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LocalTurnComplete )( 
            ITurnCallback * This,
            /* [in] */ ITile *tile,
            IPosition *target);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AnimationComplete )( 
            ITurnCallback * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetRemoteGameComplete )( 
            ITurnCallback * This,
            /* [in] */ BSTR game);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *PutRemoteGameComplete )( 
            ITurnCallback * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DeleteRemoteGameComplete )( 
            ITurnCallback * This);
        
        END_INTERFACE
    } ITurnCallbackVtbl;

    interface ITurnCallback
    {
        CONST_VTBL struct ITurnCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITurnCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITurnCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITurnCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITurnCallback_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITurnCallback_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITurnCallback_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITurnCallback_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITurnCallback_LocalTurnComplete(This,tile,target)	\
    ( (This)->lpVtbl -> LocalTurnComplete(This,tile,target) ) 

#define ITurnCallback_AnimationComplete(This)	\
    ( (This)->lpVtbl -> AnimationComplete(This) ) 

#define ITurnCallback_GetRemoteGameComplete(This,game)	\
    ( (This)->lpVtbl -> GetRemoteGameComplete(This,game) ) 

#define ITurnCallback_PutRemoteGameComplete(This)	\
    ( (This)->lpVtbl -> PutRemoteGameComplete(This) ) 

#define ITurnCallback_DeleteRemoteGameComplete(This)	\
    ( (This)->lpVtbl -> DeleteRemoteGameComplete(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITurnCallback_INTERFACE_DEFINED__ */


#ifndef __IGameView_INTERFACE_DEFINED__
#define __IGameView_INTERFACE_DEFINED__

/* interface IGameView */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IGameView;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("637C808C-E009-4832-B4D6-1B3EDB6B53FC")
    IGameView : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GameResumed( 
            IGameDescriptor *game,
            PlayerTeam turn) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GameSuspended( 
            IGameDescriptor *game) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GameOver( 
            /* [in] */ IPlayer *winner) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StateChanged( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE TetradFound( 
            /* [in] */ ITurnCallback *callback,
            IPlayer *player,
            TetradShape shape,
            /* [in] */ SAFEARRAY * tileIds) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DoLocalTurn( 
            /* [in] */ ITurnCallback *callback) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AnimateTurn( 
            /* [in] */ ITurnCallback *callback,
            /* [in] */ SAFEARRAY * tileIds,
            /* [in] */ Direction direction,
            /* [in] */ UCHAR distance) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Log( 
            /* [in] */ BSTR msg,
            /* [in] */ BSTR file,
            /* [in] */ int line) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAuthToken( 
            /* [retval][out] */ BSTR *token) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetRemoteGames( 
            /* [retval][out] */ SAFEARRAY * *games) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE NewRemoteGame( 
            /* [in] */ BSTR localId,
            /* [in] */ BSTR remoteId,
            /* [retval][out] */ BSTR *gameId) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetRemoteGame( 
            /* [in] */ ITurnCallback *callback,
            /* [in] */ BSTR gameId) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE PutRemoteGame( 
            /* [in] */ ITurnCallback *callback,
            /* [in] */ BSTR gameId,
            /* [in] */ BSTR game) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DeleteRemoteGame( 
            /* [in] */ ITurnCallback *callback,
            /* [in] */ BSTR gameId) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IGameViewVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGameView * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGameView * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGameView * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGameView * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGameView * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGameView * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGameView * This,
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GameResumed )( 
            IGameView * This,
            IGameDescriptor *game,
            PlayerTeam turn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GameSuspended )( 
            IGameView * This,
            IGameDescriptor *game);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GameOver )( 
            IGameView * This,
            /* [in] */ IPlayer *winner);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StateChanged )( 
            IGameView * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *TetradFound )( 
            IGameView * This,
            /* [in] */ ITurnCallback *callback,
            IPlayer *player,
            TetradShape shape,
            /* [in] */ SAFEARRAY * tileIds);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DoLocalTurn )( 
            IGameView * This,
            /* [in] */ ITurnCallback *callback);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AnimateTurn )( 
            IGameView * This,
            /* [in] */ ITurnCallback *callback,
            /* [in] */ SAFEARRAY * tileIds,
            /* [in] */ Direction direction,
            /* [in] */ UCHAR distance);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Log )( 
            IGameView * This,
            /* [in] */ BSTR msg,
            /* [in] */ BSTR file,
            /* [in] */ int line);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAuthToken )( 
            IGameView * This,
            /* [retval][out] */ BSTR *token);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetRemoteGames )( 
            IGameView * This,
            /* [retval][out] */ SAFEARRAY * *games);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *NewRemoteGame )( 
            IGameView * This,
            /* [in] */ BSTR localId,
            /* [in] */ BSTR remoteId,
            /* [retval][out] */ BSTR *gameId);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetRemoteGame )( 
            IGameView * This,
            /* [in] */ ITurnCallback *callback,
            /* [in] */ BSTR gameId);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *PutRemoteGame )( 
            IGameView * This,
            /* [in] */ ITurnCallback *callback,
            /* [in] */ BSTR gameId,
            /* [in] */ BSTR game);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DeleteRemoteGame )( 
            IGameView * This,
            /* [in] */ ITurnCallback *callback,
            /* [in] */ BSTR gameId);
        
        END_INTERFACE
    } IGameViewVtbl;

    interface IGameView
    {
        CONST_VTBL struct IGameViewVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGameView_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGameView_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGameView_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGameView_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IGameView_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IGameView_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IGameView_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IGameView_GameResumed(This,game,turn)	\
    ( (This)->lpVtbl -> GameResumed(This,game,turn) ) 

#define IGameView_GameSuspended(This,game)	\
    ( (This)->lpVtbl -> GameSuspended(This,game) ) 

#define IGameView_GameOver(This,winner)	\
    ( (This)->lpVtbl -> GameOver(This,winner) ) 

#define IGameView_StateChanged(This)	\
    ( (This)->lpVtbl -> StateChanged(This) ) 

#define IGameView_TetradFound(This,callback,player,shape,tileIds)	\
    ( (This)->lpVtbl -> TetradFound(This,callback,player,shape,tileIds) ) 

#define IGameView_DoLocalTurn(This,callback)	\
    ( (This)->lpVtbl -> DoLocalTurn(This,callback) ) 

#define IGameView_AnimateTurn(This,callback,tileIds,direction,distance)	\
    ( (This)->lpVtbl -> AnimateTurn(This,callback,tileIds,direction,distance) ) 

#define IGameView_Log(This,msg,file,line)	\
    ( (This)->lpVtbl -> Log(This,msg,file,line) ) 

#define IGameView_GetAuthToken(This,token)	\
    ( (This)->lpVtbl -> GetAuthToken(This,token) ) 

#define IGameView_GetRemoteGames(This,games)	\
    ( (This)->lpVtbl -> GetRemoteGames(This,games) ) 

#define IGameView_NewRemoteGame(This,localId,remoteId,gameId)	\
    ( (This)->lpVtbl -> NewRemoteGame(This,localId,remoteId,gameId) ) 

#define IGameView_GetRemoteGame(This,callback,gameId)	\
    ( (This)->lpVtbl -> GetRemoteGame(This,callback,gameId) ) 

#define IGameView_PutRemoteGame(This,callback,gameId,game)	\
    ( (This)->lpVtbl -> PutRemoteGame(This,callback,gameId,game) ) 

#define IGameView_DeleteRemoteGame(This,callback,gameId)	\
    ( (This)->lpVtbl -> DeleteRemoteGame(This,callback,gameId) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGameView_INTERFACE_DEFINED__ */


#ifndef __IEngine_INTERFACE_DEFINED__
#define __IEngine_INTERFACE_DEFINED__

/* interface IEngine */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IEngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CCC8E027-77CD-49E3-A1E4-232DA8016A17")
    IEngine : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ IGameView *view) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateGame( 
            /* [in] */ IPlayer *player1,
            /* [in] */ IPlayer *player2,
            /* [retval][out] */ IGameDescriptor **game) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SuspendGame( 
            /* [in] */ IGameDescriptor *game) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ResumeGame( 
            /* [in] */ IGameDescriptor *game) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGameDescriptors( 
            /* [in] */ BSTR localId,
            /* [retval][out] */ SAFEARRAY * *games) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IEngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEngine * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEngine * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEngine * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEngine * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEngine * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEngine * This,
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Init )( 
            IEngine * This,
            /* [in] */ IGameView *view);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateGame )( 
            IEngine * This,
            /* [in] */ IPlayer *player1,
            /* [in] */ IPlayer *player2,
            /* [retval][out] */ IGameDescriptor **game);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SuspendGame )( 
            IEngine * This,
            /* [in] */ IGameDescriptor *game);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ResumeGame )( 
            IEngine * This,
            /* [in] */ IGameDescriptor *game);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGameDescriptors )( 
            IEngine * This,
            /* [in] */ BSTR localId,
            /* [retval][out] */ SAFEARRAY * *games);
        
        END_INTERFACE
    } IEngineVtbl;

    interface IEngine
    {
        CONST_VTBL struct IEngineVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEngine_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEngine_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEngine_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEngine_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IEngine_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IEngine_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IEngine_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IEngine_Init(This,view)	\
    ( (This)->lpVtbl -> Init(This,view) ) 

#define IEngine_CreateGame(This,player1,player2,game)	\
    ( (This)->lpVtbl -> CreateGame(This,player1,player2,game) ) 

#define IEngine_SuspendGame(This,game)	\
    ( (This)->lpVtbl -> SuspendGame(This,game) ) 

#define IEngine_ResumeGame(This,game)	\
    ( (This)->lpVtbl -> ResumeGame(This,game) ) 

#define IEngine_GetGameDescriptors(This,localId,games)	\
    ( (This)->lpVtbl -> GetGameDescriptors(This,localId,games) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEngine_INTERFACE_DEFINED__ */



#ifndef __TsCOMLib_LIBRARY_DEFINED__
#define __TsCOMLib_LIBRARY_DEFINED__

/* library TsCOMLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_TsCOMLib;

EXTERN_C const CLSID CLSID_Player;

#ifdef __cplusplus

class DECLSPEC_UUID("3ED31C2A-FFA4-45F7-8982-2EA15C2FA6AC")
Player;
#endif

EXTERN_C const CLSID CLSID_Position;

#ifdef __cplusplus

class DECLSPEC_UUID("839F06F0-D412-42F7-9983-D9285FCF28C8")
Position;
#endif

EXTERN_C const CLSID CLSID_Tile;

#ifdef __cplusplus

class DECLSPEC_UUID("80BFC225-119F-42BF-96CE-41213B0BEDE8")
Tile;
#endif

EXTERN_C const CLSID CLSID_GameDescriptor;

#ifdef __cplusplus

class DECLSPEC_UUID("D827CB47-E145-40BE-8231-FC1BD5490E6D")
GameDescriptor;
#endif

EXTERN_C const CLSID CLSID_TurnCallback;

#ifdef __cplusplus

class DECLSPEC_UUID("0060F09E-6365-4AD2-BD16-A2CC37E9343F")
TurnCallback;
#endif

EXTERN_C const CLSID CLSID_GameView;

#ifdef __cplusplus

class DECLSPEC_UUID("8A398FB8-093E-468D-A3C4-4BC132DB8DAB")
GameView;
#endif

EXTERN_C const CLSID CLSID_Engine;

#ifdef __cplusplus

class DECLSPEC_UUID("F84334B0-7BFD-4F36-8509-9CABBD35AB41")
Engine;
#endif
#endif /* __TsCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize64(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal64(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal64(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree64(     unsigned long *, LPSAFEARRAY * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


