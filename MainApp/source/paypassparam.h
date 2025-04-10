#ifndef _PAYPASS_PARAM_H_
#define _PAYPASS_PARAM_H_

ST_TERMAIDLIST MASTERCARDAID= 
{
  7,
  "\xA0\x00\x00\x00\x04\x10\x10",
  0,
  PAYPASS,
  0x00,  
};


ST_TERMAIDLIST MESTROAID= 
{
  7,
  "\xA0\x00\x00\x00\x04\x30\x60",
  0,
  PAYPASS,
  0x00,  
};


ST_TERMAIDLIST MCTESTAID= 
{
  5,
  "\xB0\x12\x34\x56\x78",
  0,
  PAYPASS,
  0x00,  
};

typedef enum
{
    PPS_MChip1               ,
    PPS_Bal                ,
    PPS_Bal2               ,
    PPS_CVM_1              ,
    PPS_CVM_2              ,
    PPS_Limit_1            ,
    PPS_Limit_1b           ,
    PPS_Limit_2            ,
    PPS_Limit_3            ,
    PPS_MC_ATM             ,
    //10
    PPS_MC_notATM          ,
    PPS_MC_prop            ,
    PPS_MCerrKernelConf    ,
    PPS_MChip2             ,
    PPS_MChip3             ,
    PPS_MChip4             ,
    PPS_MChip5             ,
    PPS_MChip6             ,
    PPS_MChip7             ,
    PPS_MChip8             ,
    //20
    PPS_MCnoDefault_1      ,
    PPS_MCnoDefault_2      ,
    PPS_MCnoDefault_3      ,
    PPS_MCnoDefault_4      ,
    PPS_MCnoDefault_5      ,
    PPS_MCnoMSI            ,
    PPS_MConlineonly       ,
    PPS_perf_MC            ,
    PPS_PostBal            ,
    PPS_PostBal2           ,
    //30
    PPS_PreBal               ,
    PPS_PreBal2            ,
    PPS_RRP                ,
    PPS_RRP2               ,
    PPS_RRP3               ,
    PPS_RRP4               ,
    PPS_RRP5               ,
    PPS_RRP6               ,
    PPS_Torn_Bal           ,
    PPS_Torn3min           ,
    //40
    PPS_Torn5sec           ,
    PPS_TornMax1           ,
    PPS_TornMax1_RRP       ,
    PPS_TornMax1_contact   ,
    PPS_TornMax2           ,
    PPS_TrxTypeOther       ,
    PPMS_DEreferce           ,
    PPMS_DE1               ,
    PPMS_DE2               ,
    PPMS_DE3               ,
    //50
    PPMS_DE4               ,
    PPMS_DE5               ,
    PPMS_DE6               ,
    PPMS_DE7               ,
    PPMS_DE8               ,
    PPMC_DEreferce           ,
    PPMC_DETTR_referce       ,
    PPMC_DE1               ,
    PPMC_DE2               ,
    PPMC_DE3               ,
    //60
    PPMC_DE4               ,
    PPMC_DE5               ,
    PPMC_DE6               ,
    PPMC_DE7               ,
    PPMC_DE8               ,
    PPMC_DE9               ,
    PPMC_DE10               ,
    PPMC_DE_RRP            ,
    PPMC_DE_Torn_Empty       ,
    PPMC_IDS_Torn           ,
    //70
    PPMC_IDS_TornBalance   ,
    PPMC_IDS_TornMax1       , 
    PPMC_SDS_Torn           ,
    PPMC_DETTR1            ,
    PPMC_DETTR2            ,
    PPMC_DETTR3            ,
    PPMC_DETTR4            ,
    PPMC_DETTR5            ,
    PPMC_DETTR6            ,
    PPMC_DETTR7            ,
    //80
    PPMC_DETTR8            ,
    PPMC_DETTR9            ,
    PPMC_DETTR10           ,
    PPMC_DETTR11           ,
    PPMC_DETTR12           ,
    PPMC_DETTR13           ,
    PPMC_DETTR14           ,
    PPMC_DETTR15           ,
    PPMC_DETTR16           ,
    PPMC_DETTR17           ,
    //90
    PPMC_DETTR18           ,
    PPMC_DETTR19           ,
    PPMC_DETTR20           ,
    PPMC_DETTR21           ,
    PPMC_DETTR22           ,
    PPMC_DETTR23           ,
    PPMC_DETTR24           ,
    PPMC_DETTR25           ,
    PPMC_DETTR26           ,
    PPMC_DETTR27           ,
    //100
    PPMC_DETTR28           ,
    PPMC_DETTR29           ,
    PPMC_DETTR_RRP           ,
    PPMC_SDSTTR_Torn       ,
    PPS_MStripe1           ,
    PPS_MS_HT0                 ,
    PPS_MS_Limit_1         ,
    PPS_MS_Limit_2         ,
    PPS_MS_Limit_3         ,
    PPS_MS_Limit_3b        ,
    //110
    PPS_MS_prop            ,
    PPS_NoDefault_1        ,
    PPS_NoDefault_2        ,
    PPS_NoDefault_3        ,
    PPS_noMSI               ,
    PPS_perf_MS            ,
    PPS_Select1            ,

}MC_TEST_CONFIG;

#endif

