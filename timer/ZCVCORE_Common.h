/*! \file ZCVCORE_Common.h
\brief ZCVCORE_Common.h document.
*/


/**********************************************************************************************
-----------------------------------------------------------------------------------------------
Name :                  ZCVCORE_Common.h
Description :           Framework
Revision :              V1.0
Module :                ZIVS
System :                ZIVS
Language :              ANSI C
Develop Environment :   MSVC, CODEBLOCKS
Running Environment :   Windows, Linux
------------------------------------------------------------------------------------------------
Creation Data :         <01-01-2013  17:00>
Chief Editor :          <Zeng Jianping>
Participant :           <参与者>
Audit :                 <检查者>
Edit Log :              <编辑记录>
Release :               <01-01-2013>
------------------------------------------------------------------------------------------------
Copyright (C) 2013, ZIVS Corporation, All Rights Reserved.
------------------------------------------------------------------------------------------------
***********************************************************************************************/


#ifndef _ZCVCORE_COMMON_H_
#define _ZCVCORE_COMMON_H_

// to avoid multiple Visual Studio 2005 warnings
#if !defined _CRT_SECURE_NO_DEPRECATE && _MSC_VER > 1300
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>
#include <time.h>
#if defined __GNUC__
#include <sys/time.h>
#elif defined _WIN32
#include <windows.h>
#endif


//! \defgroup group_zcv ZCV算法模块
//!  ZCV算法模块数据类型与函数
//! \{
//! \}

//! \defgroup group_zcv_core 基础设施
//! \ingroup group_zcv
//!  基础设施数据类型与函数
//! \{
//! \}


#if defined WIN32 || defined _WIN32
    #define ZCV_CDECL __cdecl
    #define ZCV_STDCALL __stdcall
#else
    #define ZCV_CDECL
    #define ZCV_STDCALL
#endif

#ifdef __cplusplus
    #define ZCV_EXTERN_C extern "C"
    #define ZCV_DEFAULT(val) = val
#else
    #define ZCV_EXTERN_C
    #define ZCV_DEFAULT(val)
#endif

#ifdef __cplusplus
    #define ZCV_EXTERN_C_FUNCPTR(x) extern "C" { typedef x; }
#else
    #define ZCV_EXTERN_C_FUNCPTR(x) typedef x
#endif

#if defined __cplusplus
    #define ZCV_INLINE inline
#elif (defined WIN32 || defined _WIN32 || defined WINCE) && !defined __GNUC__
    #define ZCV_INLINE __inline
#else
    #define ZCV_INLINE static __inline__
#endif

#if (defined WIN32 || defined _WIN32 || defined WINCE) && defined ZCVAPI_EXPORTS
    #define ZCV_EXPORTS __declspec(dllexport)
#else
    #define ZCV_EXPORTS
#endif

//#define ZCVAPI(rettype) ZCV_EXTERN_C ZCV_EXPORTS rettype ZCV_CDECL
#define ZCVAPI      ZCV_EXTERN_C ZCV_EXPORTS
#define ZCV_IMPL    ZCV_EXTERN_C

#define ZCV_MAKE_VERSION( type_m, type_s, seri_m, seri_s ) \
		( 0	\
		| ( (((uint32_t)type_m)&0xFF)<<0 ) \
		| ( (((uint32_t)type_s)&0xFF)<<8 ) \
		| ( (((uint32_t)seri_m)&0xFF)<<16 ) \
		| ( (((uint32_t)seri_s)&0xFF)<<24 ) \
		)

#define ZCV_MAKE_VERSION2( type_m, type_s, seri_m1, seri_m2, seri_s ) \
		( 0	\
		| ( (((uint32_t)type_m)&0xFF)<<0 ) \
		| ( (((uint32_t)type_s)&0xFF)<<8 ) \
		| ( (((uint32_t)seri_m1)&0x0F)<<16 ) \
		| ( (((uint32_t)seri_m2)&0x0F)<<20 ) \
		| ( (((uint32_t)seri_s)&0xFF)<<24 ) \
		)

#define ZCV_VER_TYPM( ver )     ( (((uint32_t)ver)>>0)&0xFF )
#define ZCV_VER_TYPS( ver )     ( (((uint32_t)ver)>>8)&0xFF )
#define ZCV_VER_SERM( ver )     ( (((uint32_t)ver)>>16)&0xFF )
#define ZCV_VER_SERM1( ver )    ( (((uint32_t)ver)>>16)&0x0F )
#define ZCV_VER_SERM2( ver )    ( (((uint32_t)ver)>>20)&0x0F )
#define ZCV_VER_SERS( ver )     ( (((uint32_t)ver)>>24)&0xFF )

#ifdef _DEBUG
    #define ZCV_ASSERT(x) ( assert(x) )
#else
    #define ZCV_ASSERT(x)
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef NULL
    #define NULL 0L
#endif

#ifndef PI
#define PI 3.141592653589793
#endif

#ifndef max
//#define max(a,b)  (((a) > (b)) ? (a) : (b))
//#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#define NAME_BUF_LEN 32
#define URL_BUF_LEN	256

typedef unsigned char        uint8_t;
#if !(defined __GNUC__ && defined __int8_t_defined)
typedef signed char          int8_t;
#endif
typedef unsigned short       uint16_t;
typedef short                int16_t;
typedef unsigned int         uint32_t;
typedef int                  int32_t;
typedef void                 *MDLHANDLE;


#endif // _ZCVCORE_COMMON_H_

