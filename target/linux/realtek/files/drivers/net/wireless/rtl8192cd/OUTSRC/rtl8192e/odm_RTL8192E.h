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
#ifndef	__ODM_RTL8192E_H__
#define __ODM_RTL8192E_H__

#define	MAIN_ANT_CG_TRX	1
#define	AUX_ANT_CG_TRX        0
#define	MAIN_ANT_CGCS_RX    0
#define	AUX_ANT_CGCS_RX	1

VOID
ODM_AntennaDiversityInit_8192E(
	IN		PDM_ODM_T		pDM_Odm
);

VOID
odm_RX_HWAntDivInit_8192E(
	IN		PDM_ODM_T		pDM_Odm
);

VOID
odm_TRX_HWAntDivInit_8192E(
	IN 	PDM_ODM_T	 pDM_Odm
);

VOID
odm_FastAntTrainingInit_8192E(
	IN 	PDM_ODM_T	 pDM_Odm
);



VOID
ODM_AntselStatistics_8192E(
	IN		PDM_ODM_T		pDM_Odm,
	IN		u1Byte			antsel_tr_mux,
	IN		u4Byte			MacId,
	IN		u4Byte			RxPWDBAll
);

VOID
ODM_AntennaDiversity_8192E
(
	IN		PDM_ODM_T		pDM_Odm
);


VOID
odm_HWAntDiv_8192E(
	IN		PDM_ODM_T		pDM_Odm
);

VOID
odm_FastAntTraining_8192E(
	IN		PDM_ODM_T		pDM_Odm
);

VOID
ODM_UpdateRxIdleAnt_8192E(
	IN 	PDM_ODM_T 	pDM_Odm, 	IN 	u1Byte	 Ant
);

//#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
//VOID
//ODM_SetTxAntByTxInfo_8192E(
//	IN		PDM_ODM_T		pDM_Odm,
//	IN		pu1Byte			pDesc,
//	IN		u1Byte			macId	
//);
//#else// (DM_ODM_SUPPORT_TYPE == ODM_AP)

//VOID
//ODM_SetTxAntByTxInfo_8192E(
//	IN		PDM_ODM_T		pDM_Odm	
//);

//#endif
#endif

