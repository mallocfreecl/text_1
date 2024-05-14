/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:SdkTypes.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.定义数据类型
* 2.定义编译器相关的宏
*
* (Requirements, pseudo code and etc.)
*********************************************************************
* Limitations:
*
* (limitations)
*********************************************************************
*********************************************************************
* Revision History：
*
* Version       Date         	  Author       Descriptions
* ----------    --------------  ------------   ------------
* 1.0           2021-08-17      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/
#ifndef SDK_TYPES_H
#define SDK_TYPES_H

/* Includes*/
/******************************************************************/
#define U8_MAX (0xFFu)
#define U16_MAX (0xFFFFu)
/* 修改错误的声明 22.08.17 by jxj */
#define S16_MAX (s16)(0x7FFF)
#define S16_MIN (s16)(0x8000)
#define U32_MAX (0xFFFFFFFFuL)

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed long s32;
typedef unsigned long u32;
typedef signed long long s64;
typedef unsigned long long u64;

typedef volatile signed char vs8;
typedef volatile unsigned char vu8;
typedef volatile signed short vs16;
typedef volatile unsigned short vu16;
typedef volatile signed long vs32;
typedef volatile unsigned long vu32;

typedef volatile signed char vs8_t;
typedef volatile unsigned char vu8_t;
typedef volatile signed short vs16_t;
typedef volatile unsigned short vu16_t;
typedef volatile signed long vs32_t;
typedef volatile unsigned long vu32_t;

typedef u32 Addr_t;
typedef Addr_t *pAddr_t;

typedef struct
{
  s16 Real; /**< \brief Real part */
  s16 Imag; /**< \brief Imaginary part */
} TComplex;

typedef struct
{
  s16 A; /**< \brief Phase A current */
  s16 B; /**< \brief Phase B current */
} TPhaseCurr;

#define STATIC_INLINE static inline
#define SDK_STATIC_INLINE static inline

#endif
