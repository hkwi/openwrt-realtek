/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/

//============================================================
// include files
//============================================================

//#include "Mp_Precomp.h"

#include "../odm_precomp.h"

#if (RTL8192E_SUPPORT == 1)
#ifdef CONFIG_HW_ANTENNA_DIVERSITY
	
#define	TX_BY_REG	0

//2  Init 8192E Antenna Diversity=============================================================================  begin
VOID
ODM_AntennaDiversityInit_8192E(
	IN		PDM_ODM_T		pDM_Odm
)
{
	if(pDM_Odm->SupportICType != ODM_RTL8192E)
		return;

        
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("pDM_Odm->AntDivType=%d\n",pDM_Odm->AntDivType));
	//ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("pDM_Odm->bIsMPChip=%s\n",(pDM_Odm->bIsMPChip?"TRUE":"FALSE")));

	if(pDM_Odm->AntDivType == CGCS_RX_HW_ANTDIV)
		odm_RX_HWAntDivInit_8192E(pDM_Odm);
	else if(pDM_Odm->AntDivType == CG_TRX_HW_ANTDIV)
		odm_TRX_HWAntDivInit_8192E(pDM_Odm);
	else if(pDM_Odm->AntDivType == CG_TRX_SMART_ANTDIV)
		odm_FastAntTrainingInit_8192E(pDM_Odm);

}

// 1. CGCS_RX_HW_ANTDIV------------------------------------------------------------
VOID
odm_RX_HWAntDivInit_8192E(
	IN		PDM_ODM_T		pDM_Odm
)
{
	//u4Byte value32;
	
#if (MP_DRIVER == 1)
        pDM_Odm->AntDivType = CGCS_RX_SW_ANTDIV;
        ODM_SetBBReg(pDM_Odm, 0xc50 , BIT7, 0); // disable HW AntDiv (OFDM)
        ODM_SetBBReg(pDM_Odm, 0xa00 , BIT15, 0); // disable HW AntDiv (CCK)
        ODM_SetBBReg(pDM_Odm, 0xc50 , BIT8, 0); //r_rxdiv_enable_anta  Regc50[8]=1'b0  0: control by c50[9]
        ODM_SetBBReg(pDM_Odm, 0xc50 , BIT9, 1);  // 1:CG, 0:CS
        return;
#endif

	 ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("odm_RX_HWAntDivInit_8192E() \n"));
	
	 //Pin Settings
	 ODM_SetBBReg(pDM_Odm, 0x870 , BIT8, 0);//Reg870[8]=1'b0,    // "antsel" is controled by HWs
	 ODM_SetBBReg(pDM_Odm, 0xc50 , BIT8, 1); //Regc50[8]=1'b1  //" CS/CG switching" is controled by HWs
	 ODM_SetBBReg(pDM_Odm, 0xc50 , BIT9, 1); //1:CG, 0:CS //output at CG only
	 ODM_SetBBReg(pDM_Odm, 0x914 , 0xFFFF, 0x0100); //antenna mapping table
	  
	 //OFDM Settings
	 //ODM_SetBBReg(pDM_Odm, 0xca4 , bMaskDWord, 0x000000a0);
	 ODM_SetBBReg(pDM_Odm, 0xca4 , 0x7FF, 0xA0); //thershold
	 ODM_SetBBReg(pDM_Odm, 0xca4 , 0x7FF000, 0x0); //bias
	 ODM_SetBBReg(pDM_Odm, 0xc50 , BIT7, 1);	//enable HW AntDiv (OFDM)
	 
	 //CCK Settings
	 ODM_SetBBReg(pDM_Odm, 0xa04 , 0xF000000, 0); //Select which path to receive for CCK_1 & CCK_12
	 ODM_SetBBReg(pDM_Odm, 0xb34 , BIT30, 1); //(92E) ANTSEL_CCK_opt = r_en_antsel_cck? ANTSEL_CCK: 1'b0
	 ODM_SetBBReg(pDM_Odm, 0xa74 , BIT7, 1); //Fix CCK PHY status report issue
	 ODM_SetBBReg(pDM_Odm, 0xa0c , BIT4, 1); //CCK complete HW AntDiv within 64 samples 
	 ODM_SetBBReg(pDM_Odm, 0xa00 , BIT15, 1);// enable HW AntDiv (CCK)
	 
	 ODM_UpdateRxIdleAnt_8192E(pDM_Odm, MAIN_ANT);

}

// 2. CG_TRX_HW_ANTDIV------------------------------------------------------------
VOID
odm_TRX_HWAntDivInit_8192E(
	IN 	PDM_ODM_T	 pDM_Odm
)
{
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("odm_TRX_HWAntDivInit_8192E() \n"));


	
}

// 3. CG_TRX_SMART_ANTDIV------------------------------------------------------------
VOID
odm_FastAntTrainingInit_8192E(
	IN 	PDM_ODM_T	 pDM_Odm
)
{
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("odm_FastAntTrainingInit_8192E() \n"));


	
}
//2 Init 8192E Antenna Diversity===============================================================================  end





VOID
ODM_AntselStatistics_8192E(
	IN		PDM_ODM_T		pDM_Odm,
	IN		u1Byte			antsel_tr_mux,
	IN		u4Byte			MacId,
	IN		u4Byte			RxPWDBAll
)
{
	pFAT_T	pDM_FatTable = &pDM_Odm->DM_FatTable;


	if(pDM_Odm->AntDivType == CG_TRX_HW_ANTDIV)
	{
		if(antsel_tr_mux == MAIN_ANT_CG_TRX)
		{

			pDM_FatTable->MainAnt_Sum[MacId]+=RxPWDBAll;
			pDM_FatTable->MainAnt_Cnt[MacId]++;
		}
		else
		{
			pDM_FatTable->AuxAnt_Sum[MacId]+=RxPWDBAll;
			pDM_FatTable->AuxAnt_Cnt[MacId]++;

		}
	}
	
	else if(pDM_Odm->AntDivType == CGCS_RX_HW_ANTDIV)
	{
		if(antsel_tr_mux == MAIN_ANT_CGCS_RX)
		{

			pDM_FatTable->MainAnt_Sum[MacId]+=RxPWDBAll;
			pDM_FatTable->MainAnt_Cnt[MacId]++;
		}
		else
		{
			pDM_FatTable->AuxAnt_Sum[MacId]+=RxPWDBAll;
			pDM_FatTable->AuxAnt_Cnt[MacId]++;

		}
	}


}





//2 8192E Antenna Diversity================================================================================  begin
VOID
ODM_AntennaDiversity_8192E
(
	IN		PDM_ODM_T		pDM_Odm
)
{

pFAT_T	pDM_FatTable = &pDM_Odm->DM_FatTable;
static  u4Byte  antdiv_i=0;
	if((pDM_Odm->SupportICType != ODM_RTL8192E) || (!(pDM_Odm->SupportAbility & ODM_BB_ANT_DIV)))
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("ODM_AntennaDiversity_8192E: Not Support 92E AntDiv\n"));
		return;	
	}

	if(!pDM_Odm->bLinked) //bLinked==False
	{
		//ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("ODM_AntennaDiversity_92E(): No Link.\n"));
		if (pDM_Odm->antdiv_rssi)
			panic_printk("[ No Link!!! ] \n");
		if(pDM_FatTable->bBecomeLinked == TRUE)
		{
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("Need to Turn off HW AntDiv\n"));
			ODM_SetBBReg(pDM_Odm, 0xc50 , BIT7, 0);	//disable HW AntDiv (OFDM)
			ODM_SetBBReg(pDM_Odm, 0xa00 , BIT15, 0); //disable HW AntDiv (CCK)
			if(pDM_Odm->AntDivType == CG_TRX_HW_ANTDIV)
				ODM_SetBBReg(pDM_Odm, 0x80c , BIT21, 0); //Reg80c[21]=1'b0//from TX Reg, TX Path Selection
				
			pDM_FatTable->bBecomeLinked = pDM_Odm->bLinked;
		}
		return;
	}
	else//bLinked==True
	{
		if(pDM_FatTable->bBecomeLinked ==FALSE)
		{
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("Need to Turn on HW AntDiv\n"));
			//Because HW AntDiv is disabled before Link, we enable HW AntDiv after link
			ODM_SetBBReg(pDM_Odm, 0xc50 , BIT7, 1);	//RegC50[7]=1'b1 		//enable HW AntDiv
			ODM_SetBBReg(pDM_Odm, 0xa00 , BIT15, 1); //Enable CCK AntDiv
			//ODM_SetMACReg(pDM_Odm, 0x7B4 , BIT18, 1); //Response Tx by current HW antdiv
			if(pDM_Odm->AntDivType == CG_TRX_HW_ANTDIV)
			{
#if TX_BY_REG
				ODM_SetBBReg(pDM_Odm, 0x80c , BIT21, 0); //Reg80c[21]=1'b0		//from Reg
#else
				ODM_SetBBReg(pDM_Odm, 0x80c , BIT21, 1); //Reg80c[21]=1'b1		//from TX Info
#endif
			}
			pDM_FatTable->bBecomeLinked = pDM_Odm->bLinked;
		}
	}

	if((pDM_Odm->AntDivType == CG_TRX_HW_ANTDIV)||(pDM_Odm->AntDivType == CGCS_RX_HW_ANTDIV))
		odm_HWAntDiv_8192E(pDM_Odm);
#if (!(DM_ODM_SUPPORT_TYPE == ODM_CE))
	else if(pDM_Odm->AntDivType == CG_TRX_SMART_ANTDIV)
		odm_FastAntTraining_8192E(pDM_Odm);
#endif
}


// 1. CGCS_RX  &   CG_TRX  HW_ANTDIV------------------------------------------------------------
VOID
odm_HWAntDiv_8192E(
	IN		PDM_ODM_T		pDM_Odm
)
{
	u4Byte	i, MinMaxRSSI=0xFF, AntDivMaxRSSI=0, MaxRSSI=0, LocalMinRSSI, LocalMaxRSSI;
	u4Byte	Main_RSSI, Aux_RSSI, pkt_ratio_m=0, pkt_ratio_a=0,pkt_threshold=5;
	u1Byte	RxIdleAnt=0, TargetAnt=7;
	pFAT_T	pDM_FatTable = &pDM_Odm->DM_FatTable;
	pDIG_T	pDM_DigTable = &pDM_Odm->DM_DigTable;
	PSTA_INFO_T   	pEntry;


	ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("ODM_AntennaDiversity_8192E() =>\n"));

	//if(pDM_Odm->AntType != ODM_AUTO_ANT)
	//{
	//	ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("Fix Antenna at %s\n",
	//		(pDM_Odm->AntType == ODM_FIX_MAIN_ANT)?"MAIN":"AUX"));
	//	return;
	//}
	   
	for (i=0; i<ODM_ASSOCIATE_ENTRY_NUM; i++)
	{
		pEntry = pDM_Odm->pODM_StaInfo[i];
		if(IS_STA_VALID(pEntry))
		{
			//2 Caculate RSSI per Antenna
			Main_RSSI = (pDM_FatTable->MainAnt_Cnt[i]!=0)?(pDM_FatTable->MainAnt_Sum[i]/pDM_FatTable->MainAnt_Cnt[i]):0;
			Aux_RSSI = (pDM_FatTable->AuxAnt_Cnt[i]!=0)?(pDM_FatTable->AuxAnt_Sum[i]/pDM_FatTable->AuxAnt_Cnt[i]):0;
			TargetAnt = (Main_RSSI==Aux_RSSI)?pDM_FatTable->RxIdleAnt:((Main_RSSI>=Aux_RSSI)?MAIN_ANT:AUX_ANT);
			if( pDM_FatTable->MainAnt_Cnt[i]!=0 && pDM_FatTable->AuxAnt_Cnt[i]!=0 )
			{
			pkt_ratio_m=( pDM_FatTable->MainAnt_Cnt[i] / pDM_FatTable->AuxAnt_Cnt[i] );
			pkt_ratio_a=( pDM_FatTable->AuxAnt_Cnt[i] / pDM_FatTable->MainAnt_Cnt[i] );
				if (pkt_ratio_m >= pkt_threshold)
					TargetAnt=MAIN_ANT;
				else if(pkt_ratio_a >= pkt_threshold)
					TargetAnt=AUX_ANT;
			}
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("MacID=%lu, MainAnt_Sum=%lu, MainAnt_Cnt=%lu\n", i, pDM_FatTable->MainAnt_Sum[i], pDM_FatTable->MainAnt_Cnt[i]));
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("MacID=%lu, AuxAnt_Sum=%lu, AuxAnt_Cnt=%lu\n",i, pDM_FatTable->AuxAnt_Sum[i], pDM_FatTable->AuxAnt_Cnt[i]));
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("MacID=%lu, Main_RSSI= %lu, Aux_RSSI= %lu\n", i, Main_RSSI, Aux_RSSI));
			if (pDM_Odm->antdiv_rssi)
				{
				panic_printk("*** Client[ %lu ] , Main_Cnt = (( %lu ))  , Main_RSSI= ((  %lu )) \n",i, pDM_FatTable->MainAnt_Cnt[i], Main_RSSI);
				panic_printk("*** Client[ %lu ] , Aux_Cnt   = (( %lu ))  , Aux_RSSI = ((  %lu )) \n" ,i, pDM_FatTable->AuxAnt_Cnt[i] , Aux_RSSI);
				panic_printk("*** TargetAnt = (( %s )) \n ", ( TargetAnt ==MAIN_ANT)?"MAIN_ANT":"AUX_ANT");
				}
			LocalMaxRSSI = (Main_RSSI>=Aux_RSSI) ? Main_RSSI : Aux_RSSI;
			LocalMinRSSI  = (Main_RSSI>Aux_RSSI) ? Aux_RSSI  : Main_RSSI;
			//2 Select MaxRSSI for DIG
			
			if((LocalMaxRSSI > AntDivMaxRSSI) && (LocalMaxRSSI < 40))
				AntDivMaxRSSI = LocalMaxRSSI;
			if(LocalMaxRSSI > MaxRSSI)
				MaxRSSI = LocalMaxRSSI;

			//2 Select RX Idle Antenna
		
			if (LocalMaxRSSI != 0)
			{
				if(LocalMaxRSSI < MinMaxRSSI)
				{
				RxIdleAnt = TargetAnt;
					MinMaxRSSI = LocalMaxRSSI;
			}	
			}
			//2 Select TRX Antenna

			if(pDM_Odm->AntDivType == CGCS_RX_HW_ANTDIV)
			{
				if(TargetAnt == MAIN_ANT)
					pDM_FatTable->antsel_a[i] = MAIN_ANT_CGCS_RX;
				else
					pDM_FatTable->antsel_a[i] = AUX_ANT_CGCS_RX;
			}
			
			else if(pDM_Odm->AntDivType == CG_TRX_HW_ANTDIV)
			{
				if(TargetAnt == MAIN_ANT)
					pDM_FatTable->antsel_a[i] = MAIN_ANT_CG_TRX;
				else
					pDM_FatTable->antsel_a[i] = AUX_ANT_CG_TRX;
			}

			ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("Tx from TxInfo, TargetAnt=%s\n", 
								(TargetAnt==MAIN_ANT)?"MAIN_ANT":"AUX_ANT"));
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD,("pDM_FatTable->antsel_a[%lu] = %d\n", i, pDM_FatTable->antsel_a[i]));
				
		}
		pDM_FatTable->MainAnt_Sum[i] = 0;
		pDM_FatTable->AuxAnt_Sum[i] = 0;
		pDM_FatTable->MainAnt_Cnt[i] = 0;
		pDM_FatTable->AuxAnt_Cnt[i] = 0;
	}
       
	//2 Set RX Idle Antenna
	ODM_UpdateRxIdleAnt_8192E(pDM_Odm, RxIdleAnt);

	pDM_DigTable->AntDiv_RSSI_max = AntDivMaxRSSI;
	pDM_DigTable->RSSI_max = MaxRSSI;
}


// 2. CG_TRX_SMART_ANTDIV------------------------------------------------------------
VOID
odm_FastAntTraining_8192E(
	IN		PDM_ODM_T		pDM_Odm
)
{
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("==>odm_FastAntTraining()\n"));

	
}
//2 8192E Antenna Diversity =================================================================================  end


VOID
ODM_UpdateRxIdleAnt_8192E(IN PDM_ODM_T pDM_Odm, IN u1Byte Ant)
{
	pFAT_T	pDM_FatTable = &pDM_Odm->DM_FatTable;
	u4Byte	DefaultAnt, OptionalAnt;


	if(pDM_FatTable->RxIdleAnt != Ant)
		{
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("Need to Update Rx Idle Ant\n"));
			if (pDM_Odm->antdiv_rssi)
				panic_printk("***[ Update RXIdle-Ant ] RxIdleAnt  = (( %s ))\n\n", ( Ant ==MAIN_ANT)?"MAIN_ANT":"AUX_ANT");
			if(Ant == MAIN_ANT)
			{
				DefaultAnt = (pDM_Odm->AntDivType == CG_TRX_HW_ANTDIV)?MAIN_ANT_CG_TRX:MAIN_ANT_CGCS_RX;
				OptionalAnt = (pDM_Odm->AntDivType == CG_TRX_HW_ANTDIV)?AUX_ANT_CG_TRX:AUX_ANT_CGCS_RX;

			}
			else //(Ant == AUX_ANT)
			{
				DefaultAnt = (pDM_Odm->AntDivType == CG_TRX_HW_ANTDIV)?AUX_ANT_CG_TRX:AUX_ANT_CGCS_RX;
				OptionalAnt = (pDM_Odm->AntDivType == CG_TRX_HW_ANTDIV)?MAIN_ANT_CG_TRX:MAIN_ANT_CGCS_RX;
				
			}
			
			//CG_TRX_HW_ANTDIV 
			if(pDM_Odm->AntDivType == CG_TRX_HW_ANTDIV)
			{
				//ODM_SetBBReg(pDM_Odm, 0xb38 , BIT5|BIT4|BIT3, DefaultAnt);	//Default RX
				//ODM_SetBBReg(pDM_Odm, 0xb38 , BIT8|BIT7|BIT6, OptionalAnt);		//Optional RX
				//ODM_SetBBReg(pDM_Odm, ODM_REG_ANTSEL_CTRL_11N , BIT14|BIT13|BIT12, DefaultAnt);	//Default TX
				//ODM_SetMACReg(pDM_Odm, ODM_REG_RESP_TX_11N , BIT6|BIT7, DefaultAnt);	//Resp Tx
				
			}
			//CGCS_RX_HW_ANTDIV
			else if(pDM_Odm->AntDivType == CGCS_RX_HW_ANTDIV)
			{
				ODM_SetBBReg(pDM_Odm, 0xb38 , BIT5|BIT4|BIT3, DefaultAnt);	//Default RX
				ODM_SetBBReg(pDM_Odm, 0xb38 , BIT8|BIT7|BIT6, OptionalAnt);	//Optional RX
			}
		}
	else
		{
		if (pDM_Odm->antdiv_rssi)
			panic_printk("***[ Stay ORI-Ant ] RxIdleAnt  = (( %s ))\n\n", ( Ant ==MAIN_ANT)?"MAIN_ANT":"AUX_ANT");
		}
		pDM_FatTable->RxIdleAnt = Ant;
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("RxIdleAnt=%s\n",(Ant==MAIN_ANT)?"MAIN_ANT":"AUX_ANT"));

			
}
	
#endif	

/*
#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
VOID
ODM_SetTxAntByTxInfo_8821A(
	IN		PDM_ODM_T		pDM_Odm,
	IN		pu1Byte			pDesc,
	IN		u1Byte			macId	
)
{
	pFAT_T	pDM_FatTable = &pDM_Odm->DM_FatTable;

	if((pDM_Odm->SupportICType != ODM_RTL8821)||(!(pDM_Odm->SupportAbility & ODM_BB_ANT_DIV)))
		return;

	SET_TX_DESC_ANTSEL_A_8812(pDesc, pDM_FatTable->antsel_a[macId]);
	//ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("ODM_SetTxAntByTxInfo_88E_WIN(): MacID=%d, antsel_tr_mux=3'b%d%d%d\n", 
	//	macId, pDM_FatTable->antsel_c[macId], pDM_FatTable->antsel_b[macId], pDM_FatTable->antsel_a[macId]));
}
#else// (DM_ODM_SUPPORT_TYPE == ODM_AP)

VOID
ODM_SetTxAntByTxInfo_8821A(
	IN		PDM_ODM_T		pDM_Odm	
)
{

	if(pDM_Odm->AntDivType == CG_TRX_SMART_ANTDIV)
		return;
	else  // (CGCS_RX_HW_ANTDIV and   CG_TRX_SMART_ANTDIV)
	{
	
	}

}
#endif
*/

#endif //#if (RTL8192_SUPPORT == 1)

