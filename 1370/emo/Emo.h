/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:Emo.h
********************************************************************
* Project/Product:LXT9820
* Title:
* Author:zyg
*********************************************************************
* Description:
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
*     1.0       2021-09-06        zyg            Original
*
*
********************************************************************
*END_FILE_HDR*/
#ifndef EMO_CFG_H
#define EMO_CFG_H

/* Includes*/
/******************************************************************/
#include "tle_device.h"
#include "GlobalVars.h"

/* Defines*/
/******************************************************************/
/* Scaling constant for FOC sinus table */
#define EMO_CFG_FOC_TABLE_SCALE (0.117553711)

#define M_Angle_2Pi (65536.0)

#define M_Isr_Time (GPT12E_T2_TIME)        /* unit:us */

#define M_High_Task_Time (GPT12E_T2_TIME)  /* unit:us */
#define M_Low_Task_Time  (3000.0)          /* unit:us */

#define M_High_Task_Freq    (1000000.0 / M_High_Task_Time)
#define M_Low_Task_Freq     (1000000.0 / M_Low_Task_Time)
#define M_LOW_TASK_TIME_NUM ((M_Low_Task_Time)/1000u)    /* (M_Low_Task_Time)/1000u) */

/*******************************************************************************
**                      Derived Global Macro Definitions not to be changed    **
*******************************************************************************/
/** \ingroup emo_type_definitions
 *  \brief TEmo_Status
 *  Types involved in the interrupt loop are placed here.
 */
typedef struct
{
    uint16 HighTaskTime;
    uint16 LowTaskTime;
    uint8 MotorState;               /**< \brief Motor state */
    uint16 MotorStartError;         /**<\brief Start Error bits */
} TEmo_Status;
typedef struct 
{	
	s16 Ld;	/*unit:uH*/
	s16 Lq;	/*unit:uH*/
	s16 RatedFlux; /*unit:uWb*/
	
	s16 K;
	
	s16 Is;
	s16 Id;
	s16 Iq;
	
	u8 En;
}Mtpa_t;
/* Variables */
/******************************************************************/
extern TEmo_Status Emo_Status;
/*消除polyspace警告,2021-09-22-by zyg*/
extern Mtpa_t MTPA;
/* Function declaration */
/******************************************************************/
extern void Emo_Init(void);
extern void Emo_StartMotor(void);
extern void Emo_StopMotor(void);
extern void Emo_IShuntLimit(void);

#endif  /* EMO_CFG_H */
