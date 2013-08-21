#ifndef __HAL8881A_DEF_H__
#define __HAL8881A_DEF_H__

/*++
Copyright (c) Realtek Semiconductor Corp. All rights reserved.

Module Name:
	Hal8881ADef.h
	
Abstract:
	Defined HAL 8881A data structure & Define
	    
Major Change History:
	When       Who               What
	---------- ---------------   -------------------------------
	2012-03-23 Filen            Create.	
--*/


extern u1Byte *data_AGC_TAB_8881A_start,    *data_AGC_TAB_8881A_end;
extern u1Byte *data_MAC_REG_8881A_start,    *data_MAC_REG_8881A_end;
extern u1Byte *data_PHY_REG_8881A_start,    *data_PHY_REG_8881A_end;
extern u1Byte *data_RadioA_8881A_start,     *data_RadioA_8881A_end;

extern u1Byte *data_AGC_TAB_8881Am_start,    *data_AGC_TAB_8881Am_end;
extern u1Byte *data_MAC_REG_8881Am_start,    *data_MAC_REG_8881Am_end;
extern u1Byte *data_PHY_REG_8881Am_start,    *data_PHY_REG_8881Am_end;
extern u1Byte *data_RadioA_8881Am_start,     *data_RadioA_8881Am_end;
extern u1Byte *data_PHY_REG_PG_8881Am_start, *data_PHY_REG_PG_8881Am_end;

extern u1Byte *data_AGC_TAB_8881ABP_start,    *data_AGC_TAB_8881ABP_end;
extern u1Byte *data_AGC_TAB_8881AMP_start,    *data_AGC_TAB_8881AMP_end;
extern u1Byte *data_RadioA_8881ABP_start,     *data_RadioA_8881ABP_end;
extern u1Byte *data_RadioA_8881AMP_start,     *data_RadioA_8881AMP_end;
#if 1   //Filen, file below should be updated
extern u1Byte *data_PHY_REG_1T_8881A_start, *data_PHY_REG_1T_8881A_end;
extern u1Byte *data_PHY_REG_MP_8881A_start, *data_PHY_REG_MP_8881A_end;
extern u1Byte *data_PHY_REG_PG_8881A_start, *data_PHY_REG_PG_8881A_end;
extern u1Byte *data_RTL8881FW_Test_T_start, *data_RTL8881FW_Test_T_end;
extern u1Byte *data_RTL8881TXBUF_Test_T_start, *data_RTL8881TXBUF_Test_T_end;
extern u1Byte *data_RTL8881FW_A_CUT_T_start, *data_RTL8881FW_A_CUT_T_end;
extern u1Byte *data_RTL8881TXBUF_A_CUT_T_start, *data_RTL8881TXBUF_A_CUT_T_end;

#endif


RT_STATUS
StopHW8881A(
    IN  HAL_PADAPTER Adapter
);


RT_STATUS
hal_Associate_8881A(
    HAL_PADAPTER            Adapter,
    BOOLEAN			    IsDefaultAdapter
);


RT_STATUS
InitPON8881A(
    IN  HAL_PADAPTER Adapter,
    IN  u4Byte     	ClkSel        
);












#endif  //__HAL8881A_DEF_H__
