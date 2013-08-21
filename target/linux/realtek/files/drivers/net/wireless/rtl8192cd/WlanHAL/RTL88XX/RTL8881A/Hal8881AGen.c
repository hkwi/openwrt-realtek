/*++
Copyright (c) Realtek Semiconductor Corp. All rights reserved.

Module Name:
	Hal8881AGen.c
	
Abstract:
	Defined RTL8881A HAL Function
	    
Major Change History:
	When       Who               What
	---------- ---------------   -------------------------------
	2012-03-23 Filen            Create.	
--*/

#if !defined(__ECOS) && !defined(CONFIG_COMPAT_WIRELESS)
#include "HalPrecomp.h"
#else
#include "../../HalPrecomp.h"
#endif

#include "data_AGC_TAB_8881A.c"
#include "data_MAC_REG_8881A.c"
#include "data_PHY_REG_8881A.c"
#include "data_RadioA_8881A.c"
#include "data_AGC_TAB_8881Am.c"
#include "data_MAC_REG_8881Am.c"
#include "data_PHY_REG_8881Am.c"
#include "data_RadioA_8881Am.c"
#include "data_PHY_REG_PG_8881Am.c"
#include "data_AGC_TAB_8881ABP.c"
#include "data_AGC_TAB_8881AMP.c"
#include "data_RadioA_8881ABP.c"
#include "data_RadioA_8881AMP.c"
#if 1   //Filen, file below should be updated
#include "data_PHY_REG_1T_8881A.c"
#include "data_PHY_REG_MP_8881A.c"
#include "data_PHY_REG_PG_8881A.c"
#include "data_RTL8881FW_Test_T.c"
#include "data_RTL8881TXBUF_Test_T.c"
#include "data_RTL8881FW_A_CUT_T.c"
#include "data_RTL8881TXBUF_A_CUT_T.c"


#endif

#define VAR_MAPPING(dst,src) \
	u1Byte *data_##dst##_start = &data_##src[0]; \
	u1Byte *data_##dst##_end   = &data_##src[sizeof(data_##src)];

VAR_MAPPING(AGC_TAB_8881A, AGC_TAB_8881A);
VAR_MAPPING(MAC_REG_8881A, MAC_REG_8881A);
VAR_MAPPING(PHY_REG_8881A, PHY_REG_8881A);
VAR_MAPPING(RadioA_8881A, RadioA_8881A);
VAR_MAPPING(AGC_TAB_8881Am, AGC_TAB_8881Am);
VAR_MAPPING(MAC_REG_8881Am, MAC_REG_8881Am);
VAR_MAPPING(PHY_REG_8881Am, PHY_REG_8881Am);
VAR_MAPPING(RadioA_8881Am, RadioA_8881Am);
VAR_MAPPING(PHY_REG_PG_8881Am, PHY_REG_PG_8881Am);
VAR_MAPPING(AGC_TAB_8881ABP, AGC_TAB_8881ABP);
VAR_MAPPING(AGC_TAB_8881AMP, AGC_TAB_8881AMP);
VAR_MAPPING(RadioA_8881ABP, RadioA_8881ABP);
VAR_MAPPING(RadioA_8881AMP, RadioA_8881AMP);

#if 1   //Filen, file below should be updated
VAR_MAPPING(PHY_REG_1T_8881A, PHY_REG_1T_8881A);
VAR_MAPPING(PHY_REG_PG_8881A, PHY_REG_PG_8881A);
VAR_MAPPING(PHY_REG_MP_8881A, PHY_REG_MP_8881A);
VAR_MAPPING(RTL8881FW_Test_T, RTL8881FW_Test_T);
VAR_MAPPING(RTL8881TXBUF_Test_T,RTL8881TXBUF_Test_T);
VAR_MAPPING(RTL8881FW_A_CUT_T,RTL8881FW_A_CUT_T);
VAR_MAPPING(RTL8881TXBUF_A_CUT_T,RTL8881TXBUF_A_CUT_T);

#endif


RT_STATUS
StopHW8881A(
    IN  HAL_PADAPTER Adapter
)
{

    // TODO: 
    return  RT_STATUS_SUCCESS;
}


RT_STATUS
InitPON8881A(
    IN  HAL_PADAPTER Adapter,     
    IN  u4Byte     	ClkSel    
)
{
    RT_TRACE_F( COMP_INIT, DBG_LOUD, ("\n") );

    HAL_RTL_W8(REG_RSV_CTRL, 0x00);

	if(ClkSel == XTAL_CLK_SEL_25M) {
		HAL_RTL_W8(REG_AFE_CTRL2, 5);
	} else if (ClkSel == XTAL_CLK_SEL_40M){
		HAL_RTL_W8(REG_AFE_CTRL2, 1);
	}	

	if (!HalPwrSeqCmdParsing88XX(Adapter, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK,
			PWR_INTF_PCI_MSK, rtl8881A_card_enable_flow))
    {
        RT_TRACE( COMP_INIT, DBG_SERIOUS, ("%s %d, HalPwrSeqCmdParsing init fail!!!\n", __FUNCTION__, __LINE__));
        return RT_STATUS_FAILURE;
    }

    return RT_STATUS_SUCCESS;    
}


RT_STATUS	
hal_Associate_8881A(
    HAL_PADAPTER            Adapter,
    BOOLEAN			        IsDefaultAdapter
)
{
    PHAL_INTERFACE              pHalFunc = GET_HAL_INTERFACE(Adapter);
    PHAL_DATA_TYPE              pHalData = _GET_HAL_DATA(Adapter);
    

    //
    //Initialization Related
    //
    pHalData->AccessSwapCtrl        = HAL_ACCESS_SWAP_MEM;
    
    pHalFunc->InitPONHandler        = InitPON8881A;
    pHalFunc->InitMACHandler        = InitMAC88XX;
    pHalFunc->InitFirmwareHandler   = InitFirmware88XX;
    pHalFunc->InitHCIDMAMemHandler  = InitHCIDMAMem88XX;
    pHalFunc->InitHCIDMARegHandler  = InitHCIDMAReg88XX;    
#if CFG_HAL_SUPPORT_MBSSID        
    pHalFunc->InitMBSSIDHandler     = InitMBSSID88XX;
#endif  //CFG_HAL_SUPPORT_MBSSID
    pHalFunc->InitVAPIMRHandler     = InitVAPIMR88XX;


    //
    //Stop Related
    //
#if CFG_HAL_SUPPORT_MBSSID            
    pHalFunc->StopMBSSIDHandler     = StopMBSSID88XX;
#endif  //CFG_HAL_SUPPORT_MBSSID
    pHalFunc->StopHWHandler         = StopHW88XX;
    pHalFunc->StopSWHandler         = StopSW88XX;
    pHalFunc->DisableVXDAPHandler   = DisableVXDAP88XX;


    //
    //ISR Related
    //
    pHalFunc->InitIMRHandler                    = InitIMR88XX;
    pHalFunc->EnableIMRHandler                  = EnableIMR88XX;
    pHalFunc->InterruptRecognizedHandler        = InterruptRecognized88XX;
    pHalFunc->GetInterruptHandler               = GetInterrupt88XX;
    pHalFunc->AddInterruptMaskHandler           = AddInterruptMask88XX;
    pHalFunc->RemoveInterruptMaskHandler        = RemoveInterruptMask88XX;
    pHalFunc->DisableRxRelatedInterruptHandler  = DisableRxRelatedInterrupt88XX;
    pHalFunc->EnableRxRelatedInterruptHandler   = EnableRxRelatedInterrupt88XX;    


    //
    //Tx Related
    //
    pHalFunc->PrepareTXBDHandler            = PrepareTXBD88XX;    
    pHalFunc->FillTxHwCtrlHandler           = FillTxHwCtrl88XX;
    pHalFunc->SyncSWTXBDHostIdxToHWHandler  = SyncSWTXBDHostIdxToHW88XX;
    pHalFunc->TxPollingHandler              = TxPolling88XX;
    pHalFunc->SigninBeaconTXBDHandler       = SigninBeaconTXBD88XX;
    pHalFunc->SetBeaconDownloadHandler      = SetBeaconDownload88XX;
    pHalFunc->FillBeaconDescHandler         = FillBeaconDesc88XX;    
    pHalFunc->GetTxQueueHWIdxHandler        = GetTxQueueHWIdx88XX;
    pHalFunc->MappingTxQueueHandler         = MappingTxQueue88XX;
    pHalFunc->QueryTxConditionMatchHandler  = QueryTxConditionMatch88XX;
#if CFG_HAL_TX_SHORTCUT
//    pHalFunc->GetShortCutTxDescHandler      = GetShortCutTxDesc88XX;
//    pHalFunc->ReleaseShortCutTxDescHandler  = ReleaseShortCutTxDesc88XX;
    pHalFunc->GetShortCutTxBuffSizeHandler  = GetShortCutTxBuffSize88XX;
    pHalFunc->SetShortCutTxBuffSizeHandler  = SetShortCutTxBuffSize88XX;    
    pHalFunc->CopyShortCutTxDescHandler     = CopyShortCutTxDesc88XX;
    pHalFunc->FillShortCutTxHwCtrlHandler   = FillShortCutTxHwCtrl88XX;    
#endif // CFG_HAL_TX_SHORTCUT


    //
    //Rx Related
    //
    pHalFunc->PrepareRXBDHandler            = PrepareRXBD88XX;
    pHalFunc->QueryRxDescHandler            = QueryRxDesc88XX;
    pHalFunc->UpdateRXBDInfoHandler         = UpdateRXBDInfo88XX;
    pHalFunc->UpdateRXBDHWIdxHandler        = UpdateRXBDHWIdx88XX;
    pHalFunc->UpdateRXBDHostIdxHandler      = UpdateRXBDHostIdx88XX;      

    //
    // General operation
    //
    pHalFunc->GetChipIDMIMOHandler          =   GetChipIDMIMO88XX;
    pHalFunc->SetHwRegHandler               =   SetHwReg88XX;
    pHalFunc->GetHwRegHandler               =   GetHwReg88XX;
    pHalFunc->SetMACIDSleepHandler          =   SetMACIDSleep88XX;


    //
    // Security Related		
    //
    pHalFunc->CAMReadMACConfigHandler       =   CAMReadMACConfig88XX;
    pHalFunc->CAMEmptyEntryHandler          =   CAMEmptyEntry88XX;
    pHalFunc->CAMFindUsableHandler          =   CAMFindUsable88XX;
    pHalFunc->CAMProgramEntryHandler        =   CAMProgramEntry88XX;


    //
    // PHY/RF Related
    //
    pHalFunc->PHYSetCCKTxPowerHandler       = PHYSetCCKTxPower88XX_AC;
    pHalFunc->PHYSetOFDMTxPowerHandler      = PHYSetOFDMTxPower88XX_AC;
    pHalFunc->PHYUpdateBBRFValHandler       = UpdateBBRFVal88XX_AC;
    pHalFunc->PHYSwBWModeHandler            = SwBWMode88XX_AC;
    // TODO: 8881A Power Tracking should be done
    pHalFunc->TXPowerTrackingHandler        = TXPowerTracking_ThermalMeter_Tmp8881A;

    //
    // Firmware CMD IO related
    //
    pHalData->H2CBufPtr88XX     = 0;
    pHalData->bFWReady          = _FALSE;
    pHalFunc->FillH2CCmdHandler             = FillH2CCmd88XX;
    pHalFunc->UpdateHalRAMaskHandler        = UpdateHalRAMask88XX;
    pHalFunc->UpdateHalMSRRPTHandler        = UpdateHalMSRRPT88XX;    
    pHalFunc->SetAPOffloadHandler           = SetAPOffload88XX;
    pHalFunc->SetRsvdPageHandler	        = SetRsvdPage88XX;
    pHalFunc->GetRsvdPageLocHandler	        = GetRsvdPageLoc88XX;
    pHalFunc->DownloadRsvdPageHandler	    = DownloadRsvdPage88XX;
    pHalFunc->C2HHandler                    = C2HHandler88XX;

    pHalFunc->DumpRxBDescTestHandler = DumpRxBDesc88XX;
    pHalFunc->DumpTxBDescTestHandler = DumpTxBDesc88XX;
    
    return  RT_STATUS_SUCCESS;    
}


