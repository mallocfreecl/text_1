/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:ADCSample.h
********************************************************************
* Project/Product:t3670
* Title:
* Author:yhd
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
* ----------    --------------  ------------   ---------------------
*     1.0       2022-02-28      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "McPhaseOffset.h"
#include "Common.h"
#include "Mc.h"
#include "McCfg.h"

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:
*
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
*
*
********************************************************************
END_FUNCTION_HDR*/
void Mc_calcPhaseOffset(void)
{
  if (McCtrl.AutoPhaseOffsetEn != 0u)
  {
    if (ABS(McAPI.ActSpeed) < 700)
    {
      McCtrl.PhaseOffset = 0;
    }
    else
    {
      McCtrl.PhaseOffset = (McAPI.ActSpeed) * 10 / 3;
      /* 修改相移值 22.08.17 by jxj */
      if (McCtrl.PhaseOffset < -6000)
      {
        McCtrl.PhaseOffset = -6000;
      }
    }
  }
}
