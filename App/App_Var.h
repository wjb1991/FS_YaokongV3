#ifndef __APP_VAR_H__
#define __APP_VAR_H__

typedef enum { LowSpeedMode = 0, HighSpeedMode }SPEEDMODE_TYPE;

typedef enum { Forward = 0, Reverse }DIRMODE_TYPE;

typedef enum { RF_Disconnect = 0, RF_Connect }RF_TYPE;

typedef enum { RF_WorkMode = 0, RF_AddrSendMode }RF_TYPE1;

INT8U Led1Time = 0;
INT8U Led2Time = 0;

INT8U SpeedMode = LowSpeedMode;
INT8U DirMode = Forward;
INT8U FirstRunFlag = TRUE;
INT8U RPPowUpReset = TRUE;
INT8U FirstRunCnt = 0;
INT8U BatteryNum = 0;

INT8U SysTick = 0;
INT8U SendTick = 0;
INT16S	Ad_Base = 0;
INT8U TX_Buff[10] = { 0 };

INT32U AutoPowerDownTime = 0;

INT8U ADCPowVal_Cnt = 0;
INT16U ADCPowVal_Max = 0;
INT16U ADCPowVal_Min = 0;
INT16U ADCPowVal_Sum = 0;
INT8U ADCPowVal_LPFlag = 0;
INT16U ADCPowVal_Fix = 0;
//const INT8U TX_Addr[] = {0x15,0x59,0x23,0xC6,0x29};
//const INT8U Common_Addr[]={0xa1,0xa2,0xa3,0xa4,0xa5};

INT8U TX_Addr[] = { 0,0,0,0,0 };
INT8U Common_Addr[] = { 0x1,0x59,0x23,0xC6,0x29 };
INT8U RX1_Addr[] = { 0x10,0x56,0x24,0xCD,0x78 };

INT8U RF_DataProcess(INT8U len, INT8U *pBuff);

INT8U RF_WDog = 0;
INT8U RF_Cont = 0;
INT8U RF_Mode = RF_WorkMode;
INT8U RF_State = RF_Disconnect;

INT8U chargecnt = 0;
INT8U stdbcnt = 0;
INT8U keycnt = 0;
INT8U keytime = 0;
INT16U keytime1 = 0;
#define GetKEYStatus()    ( keycnt == 0x81 )

INT8U WakeupFlag = TRUE;
INT8U PoweFlag = TRUE;
INT8U ChargeFlag = FALSE;

INT8U err_code = 0;
INT8U err_rp_flag = 0;

INT16S RP_ADValue = 0; //µçÎ»Æ÷adÖµ
INT8U AD_Cnt = 0;
INT16U AD_Buff[10] = { 512,512,512,512,512,
                      512,512,512,512,512 };


#endif