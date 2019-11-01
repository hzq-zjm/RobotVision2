/*! \file ZCVCORE_TIME.h
\brief ZCVCORE_TIME.h document.
*/

/**********************************************************************************************
-----------------------------------------------------------------------------------------------
Name :                  ZCVCORE_Time.h
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


#ifndef _ZCVCORE_TIME_H_
#define _ZCVCORE_TIME_H_

#include "ZCVCORE_Common.h"


//! \defgroup group_zcv_time 时间统计
//! \ingroup group_zcv
//!  时间统计数据类型与函数
//! \{
//! \}


//! \ingroup group_zcv_time
//! \{

//! \brief 时间统计打印宏。
//! \param func           用于时间统计的函数调用
//! \param _name          打印时用于显示的名称
#if defined __GNUC__

#define ZCV_TIME_IN( func, _name )                                          \
    {                                                                       \
	struct timeval start, end;                                              \
    double time_s, time_e, time_u;										    \
	gettimeofday( &start, NULL );                                           \
	func;                                                                   \
	gettimeofday( &end, NULL );												\
    time_s = 1000000.0*start.tv_sec + start.tv_usec;                        \
    time_e = 1000000.0*end.tv_sec + end.tv_usec;                            \
    time_u = time_e - time_s;                                               \
	printf( "%s time %f ms \n", _name, time_u/1000 );                       \
    }

#else

#define ZCV_TIME_IN( func, _name )                                          \
    {                                                                       \
	LARGE_INTEGER start, end, freq;                                         \
    double time_s, time_e, time_f, time_u;									\
	QueryPerformanceCounter( &start );                                      \
	func;                                                                   \
	QueryPerformanceCounter( &end );										\
	QueryPerformanceFrequency( &freq );                                     \
    time_s = start.QuadPart ;                                               \
    time_e = end.QuadPart ;                                                 \
    time_f = freq.QuadPart;                                                 \
    time_u = (time_e-time_s)/time_f;                                        \
	printf( "%s time %f ms \n", _name, time_u*1000 );                       \
    }

#endif

//! \brief 设置时间消耗数据的实时打印模式。
//! \param flag           是否打印到控制台
//! 	                  - 0: 不打印
//! 	                  - 1: 打印
ZCVAPI void TimeStatSetPrint( int flag );

//! \brief 设置时间消耗数据的记录统计模式。
//! \param flag           是否对事件数据进行记录和统计
//! 	                  - 0: 不记录和统计
//! 	                  - 1: 记录和统计
ZCVAPI void TimeStatSetStatis( int flag );

//! \brief 清空时间消耗的记录和统计数据。
ZCVAPI void TimeStatClearStatis();

//! \brief 根据时间消耗的记录数据进行统计计算。
ZCVAPI void TimeStatCalculate();

//! \brief 将时间消耗数据的统计结果打印到文件或者控制台。
//! \param filnam         如果文件路径有效则打印到文件，否则打印到控制台
ZCVAPI void TimeStatPrintStatis( char *filnam );

//! \brief 时间消耗记录的开始计时函数。
//! \return double        计时开始的起始时间
ZCVAPI double TimeStatTimeBegin();

//! \brief 时间消耗记录的停止计时函数。
//! \param uname          代码片段或者函数的名称标识
//! \param time_s         计时开始的起始时间，由函数“TimeStatTimeBegin”返回
ZCVAPI void TimeStatTimeEnd( char *uname, double time_s );

//! \}


#ifdef __cplusplus
extern "C"
{
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
// 时间相关数据类型

typedef struct
{
	uint32_t time;
	uint32_t date;
} ZSYSTIME;

// time from 1-1-1970
typedef struct
{
	uint32_t sec;	// 秒
	uint32_t msec;	// 毫秒
} TIMEF1970;

// 时间相关数据类型
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif


#endif

