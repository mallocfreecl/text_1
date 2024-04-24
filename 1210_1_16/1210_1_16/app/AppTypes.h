/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:AppTypes.h
********************************************************************
* Project/Product:LXT9818
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.Define user types, all dandwritten code should use those types
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
* Version       Date         	  Author         Descriptions
* ----------    --------------  ------------   ----------------------------------------
*     1.0       2021-07-23      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/
#ifndef APP_TYPES_H
#define APP_TYPES_H

#define U8_MAX (0xFFu)
#define U16_MAX (0xFFFFu)
#define S16_MAX (0x7FFF)
#define S16_MIN (0xFFFF)
#define U32_MAX (0xFFFFFFFFuL)

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed long s32;
typedef unsigned long u32;
typedef signed long long s64;
typedef unsigned long long u64;

typedef u32 Addr_t;
typedef Addr_t* pAddr_t;

#endif
