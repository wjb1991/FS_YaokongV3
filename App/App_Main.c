/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/

#include "App_Include.h"
#include "App_Var.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//���봦��
void InPut_Proce(void)  
{
    if (STDBYVal())                 //�������˿ڼ��
    {
        if (stdbcnt & 0x80)
        {
            if (stdbcnt != 0x80)
                stdbcnt--;
        }
        else
        {
            stdbcnt = 0x85;
        }
    }
    else
    {
        if (!(stdbcnt & 0x80))
        {
            if (stdbcnt != 0x00)
                stdbcnt--;
        }
        else
        {
            stdbcnt = 0x05;
        }
    }


    if (CHRGEVal())                 //���ڳ��˿ڼ��
    {
        if (chargecnt & 0x80)
        {
            if (chargecnt != 0x80)
                chargecnt--;
        }
        else
        {
            chargecnt = 0x85;
        }
    }
    else
    {
        if (!(chargecnt & 0x80))
        {
            if (chargecnt != 0x00)
                chargecnt--;
        }
        else
        {
            chargecnt = 0x05;
        }
    }

    Delay200us();                   //��ʱ200us �ȴ������˿ڵ�ƽ�ȶ�

    if (KEYVal())                   //�����˿ڼ��
    {
        if (keycnt & 0x80)
        {
            if (keycnt != 0x80)
                keycnt--;
        }
        else
        {
            keycnt = 0x85;
        }
    }
    else
    {
        if (!(keycnt & 0x80))
        {
            if (keycnt != 0x00)
                keycnt--;
        }
        else
        {
            keycnt = 0x05;
        }
    }
}
//���˿ڴ���
void Charge_Proce(void)             
{
    if (chargecnt == 0x00)
        ChargeFlag = TRUE;          //���˿����ڳ��
    else if (chargecnt == 0x80)
    {
        if (stdbcnt == 0x00)
            ChargeFlag = TRUE;      //���˿����ڳ���ҵ����
        else
            ChargeFlag = FALSE;     //���˿�δ���
    }
}
//�л��ٶ�ģʽ
static void Switch_SpeedMode(void)      
{
    if (FirstRunFlag == TRUE)           //��λ���Ƿ����� û�ж��������л��ٶ�ģʽ
    {
        if (SpeedMode == LowSpeedMode)
            SpeedMode = HighSpeedMode;
        else
            SpeedMode = LowSpeedMode;
    }
}
//�л��������    
static void Switch_RunDir(void)         
{
    if (FirstRunFlag == FALSE)          //��λ���Ƿ����� ���������л�����
    {                                   
        if (DirMode == Forward)         //    
            DirMode = Reverse;
        else
            DirMode = Forward;
    }
}
//��һ�����д���    
static void FirstRunFlag_Proce(void)    
{
    if ( RPPowUpReset == TRUE )                     //��λ���ϵ�ʱҪ�ȸ�λ �ص��м�λ��
    {
        if ( RP_ADValue < 60 )                      //С�ڷ�ֵ
            RPPowUpReset = FALSE;                   //�ص��м�λ��
    }
    else
    {
        if (RP_ADValue > 60 || RP_ADValue < -60)    //��λ��ǰ�� �� ɲ��
        {
            if (FirstRunCnt >= 3)                   //3*10ms = 30ms ����30ms
            {
                FirstRunFlag = FALSE;               //��Ϊ��λ���Ѿ�������
            }
            else
            {
                FirstRunCnt++;
            }
        }
        else
        {
            FirstRunCnt = 0;
        }        
    }
}

void Key_Proce(void)
{
    if (keycnt == 0x00)    //�����ͷ�
    {
        if (keytime != 0 && keytime < 50)       //�������µ�ʱ��С�� 50*20ms = 1s 
        {                                       //�̰�    
            Switch_SpeedMode();                 //�л��ٶ�ģʽ
            Switch_RunDir();                    //�л��������
        }
        keytime = 0;
    }

    if (keytime == 200)                         //�������µ�ʱ�䵽�� 200*10ms = 2s  
    {
        PoweFlag = FALSE;                       //�ϵ�
        Beep_Enable();                          //��������һ��    
        Delay_Ms(200);
        Beep_Disable();
    }

    //if( keytime == 252 )      //250*20ms = 5s  
    //{
      //������
    //}
}
//Time2�жϷ�����
void TIM2_UPDATE_IRQ(void)                      
{
    static UINT16 ms_cnt = 0;
		
		if (TIM2_SR1 & (1 << UIF))
    {
        TIM2_SR1 &= ~(1 << UIF);
				ms_cnt++;
				
				if (SendTick < 255)                     //ϵͳʱ��Ƭ�ۼ�
            SendTick++;
				
        if(ms_cnt%5 == 0)
				{
					if (SysTick < 250)                      //ϵͳʱ��Ƭ�ۼ�
							SysTick++;
	
					if (RF_WDog < 255)                      //���߶���ʱ���ۼ�    //20170123��255��Ϊ250��Ϊ���޸���ʱң����ʧȥ�����ص���ϵ��12�ŵƲ���������Ϊ250֮�󾭳���ң�������ϻ���ʾ������ʧȥ��ϵ����ʵң�������ܿ�������
							RF_WDog++;
	
					if (AutoPowerDownTime < 10000000)       //�Զ��ػ�ʱ���ۼ�
							AutoPowerDownTime++;
				}
    }
}
//�����жϷ������� ������
void KEY_INTERRUPT_IRQ(void)                    
{
    UINT8 i = 0;
    for (i = 0; i < 128; i++)                   //���º� �Ƚ��� 128������ɼ�
    {
        InPut_Proce();                          //���봦��             
        Delay_Ms(1);                            //��ʱ1ms
    }

    if (keycnt == 0x80)                         //�жϰ����Ƿ���
    {
        WakeupFlag = TRUE;                      //��������
    }
    else
    {                                           //��������
        WakeupFlag = FALSE;                     //������
        PoweFlag = FALSE;                       //���ϵ�    
    }
}
//���˿��жϴ�������� ������
void CHARGE_INTERRUPT_IRQ(void)             
{
    UINT8 i = 0;                                        
    for (i = 0; i < 128; i++)               //���º� �Ƚ��� 128������ɼ�          
    {                                       
        InPut_Proce();                      //���봦��             
        Delay_Ms(2);                        //��ʱ2ms
    }                                       
                                            
    if (chargecnt == 0x00)                  //�жϰ����Ƿ���
        ChargeFlag = TRUE;                  //���˿ڻ���  
    else                                    //��������
        ChargeFlag = FALSE;                 //���˿ڶ���  
}                                           
 
void Wakeup_Enable(void)
{
    EXTI->CR1 &= 0x3c;            //PA���������ж�PD���½����ж�
    EXTI->CR1 |= 0x81;

    PA_CR1 &= (~KEYPIN);          //��������
    PA_CR2 |= (KEYPIN);           //��������������ж�  
    PA_DDR &= (~KEYPIN);          //��������

    PD_CR1 &= (~CHRGEPIN);        //��������
    PD_CR2 |= (CHRGEPIN);         //��������������ж�  
    PD_DDR &= (~CHRGEPIN);        //��������

    //ITC->ISPR1 &= 0x3f;
    //ITC->ISPR1 |= 0x80;
}

void Wakeup_Disable(void)
{
    EXTI->CR1 &= 0x3c;            //PA���������ж�PD���½����ж�
    EXTI->CR1 |= 0x81;

    PA_CR1 &= (~KEYPIN);          //��������  
    PA_CR2 &= (~KEYPIN);          //�رհ���������ж�
    PA_DDR &= (~KEYPIN);          //��������

    PD_CR1 &= (~CHRGEPIN);        //��������
    PD_CR2 &= (~CHRGEPIN);        //�رհ���������ж�  
    PD_DDR &= (~CHRGEPIN);        //��������

    //ITC->ISPR1 &= 0x3f;           //�ж����ȼ� 
}




//ң�������ݴ���
UINT8 dBuff[10] = { 0 };
UINT8 RF_DataProcess(UINT8 len, UINT8 *pBuff)   
{
    UINT8 i;
    for (i = 0; i < 9; i++)         //��������
        dBuff[i] = pBuff[i];

    if (len == 5)                   //����5 ��ͨͨ��
    {
        UINT8 checksum = dBuff[0] + dBuff[1] + dBuff[2] + dBuff[3];     //У���
        UINT8	temp = dBuff[1] & 0x07;                                 //����ص���


        if (checksum == dBuff[4] && dBuff[0] == 0xaa)                   //֡ͷ��У���
        {
           // RF_Cont++;      //20170220ɾ�� Ϊ������ң�� ��ʧ��
          //  if (RF_Cont >= 1)    //20170220ɾ�� Ϊ�˽��ң����ʧ��������           //ң�������߼���
            {
                RF_WDog = 0;                                            //���ü�ʱ
                RF_State = RF_Connect;                                  //ң��������
            }
            switch (temp)
            {
            case 4:
                BatteryNum = 0;                                         //����0    
                break;
            case 0:
                BatteryNum = 10;                                        //����10
                break;
            case 1:
                BatteryNum = 40;                                        //����40
                break;
            case 2:
                BatteryNum = 70;                                        //����70
                break;
            case 3:
                BatteryNum = 100;                                       //����100
                break;
            default:
                break;
            }

            //0x01���Ƴ���R�� 0x02:�¶�65(����û��)  0x04:�¶�75(����û��) 0x08:��������R�� 
            //0x10���Ƴ���L�� 0x20:�¶�65(�ӻ�����)  0x40:�¶�75(�ӻ�����) 0x80:��������L��
            if (dBuff[2] != 0)
            {
                if (dBuff[2] == 0x40)                                   //���¹ػ�
                {
                    err_code = 4;                                       
                }
                else if (err_code == 0)
                {
                    if (dBuff[2] == 0x01)                               //0x01���Ƴ���R��
                    {
                        err_code = 1;                                   //��1��
                    }
                    if (dBuff[2] == 0x10)                               //0x10���Ƴ���L��        
                    {
                        err_code = 2;                                   //��2��
                    }

                    if (dBuff[2] == 0x20)                               //0x20:�¶�65(�ӻ�����)    
                    {
                        err_code = 3;                                   //��3��
                    }

                    if (dBuff[2] == 0x08)                               //0x08:��������R�� 
                    {
                        err_code = 5;                                   //��5��
                    }

                    if (dBuff[2] == 0x80)                               //0x80:��������L��    
                    {
                        err_code = 6;                                   //��6��
                    }
                }
            }
        }
    }
    else if (len == 7)                                                  //����7 ��ַ����
    {
        UINT8 checksum = dBuff[0] + dBuff[1] + dBuff[2] + dBuff[3]      //У���
            + dBuff[4] + dBuff[5];
        if (dBuff[0] == 0xcc
            && dBuff[1] == TX_Addr[0]
            && dBuff[2] == TX_Addr[1]
            && dBuff[3] == TX_Addr[2]
            && dBuff[4] == TX_Addr[3]
            && dBuff[5] == TX_Addr[4]
            && dBuff[6] == checksum)
        {
#if  0
            SwitchRFtoWorkMode();                                       //�л�������ģʽ
            ADC1_GetBase();                                             //��ȡADCƫ��ֵ
            Beep_Enable();                                              //�� �� ��~
            Delay_Ms(100);
            Beep_Disable();
            Delay_Ms(100);
            Beep_Enable();
            Delay_Ms(100);
            Beep_Disable();
            Delay_Ms(100);
            Beep_Enable();
            Delay_Ms(1000);
            Beep_Disable();
#endif
        }
    }
    return 0;
}



//ϵͳ��ʼ��
void System_Init(void)          
{
    Bsp_Init();
    Bsp_DelayMs(1000);
    Bsp_Motor_Off();
    Bsp_Beep_Off();
    
    Mod_OLEDInit();
    Mod_OLEDDisplay((INT8U*)bkgpic);

    Delay_Ms(100);
   
#if 0
    
    GetChipID();
    BK2425_Initialize((void*)RF_DataProcess, TX_Addr, RX1_Addr, TX_Addr);     //�Լ��ĵ�ַ���ͽ��� 


    ADC1_Initialize();      //ADC��ʼ��
    ADC1_Base_Copy();
    ADC1_StartConversion(); //ADC��ʼת��


    tim2ini();              //��ʱ��2��ʼ��
    tim2start();            //������ʱ��2

    Value_Init();           

    enableinterrupt();      //�����ж�
#endif
}

//��ȡң������ID
void GetChipID(void)                
{
    TX_Addr[0] = *((const UINT8*)0x9fe0);
    TX_Addr[1] = *((const UINT8*)0x9fe1);
    TX_Addr[2] = *((const UINT8*)0x9fe2);
    TX_Addr[3] = *((const UINT8*)0x9fe3);
    TX_Addr[4] = *((const UINT8*)0x9fe4);

    //TX_Addr[0] = 0x16;
    //TX_Addr[1] = 0x59;
    //TX_Addr[2] = 0x23;
    //TX_Addr[3] = 0xC6;
    //TX_Addr[4] = 0x29;

}

void main(void)
{
    System_Init();                  //ϵͳ��ʼ��
    while(1)
    {
    
    }
#if 0
    ADC1_GetBase();                 //��ȡADCƫ��ֵ
    while (1)
    {
        if (SysTick)                //10ms
        {
            SysTick = 0;
            if (WakeupFlag == TRUE)     //����
            {
                if (keycnt == 0x00)     //�����ͷ�
                {
                    WakeupFlag = FALSE; //�رջ���
                    PoweFlag = FALSE;   //����
                    keytime = 0;        //������ʱ��0
                    keytime1 = 0;       //������ʱ1��0    
                }

                if (keycnt == 0x80)  //��������
                {
                    if (keytime < 255)
                    keytime++;      //������ʱ���� 0-255 2550ms
                }

                InPut_Proce();

                if (keytime == 200)     //200*10ms = 2s     ����2s�� ����
                {
                    AutoPowerDownTime = 0;
                    PoweFlag = TRUE;    //�ϵ�
                    err_code = 0;       //�������
                    err_rp_flag = 0;    //�����λ������
                    WakeupFlag = FALSE;         //�رջ���
                    RF_State = RF_Disconnect;   //Ĭ��ң��������    
                    RF_Cont = 0;                    
                    RF_WDog = 0;
                    BatteryNum = 0;
                    keytime1 = 1;                   //������ʱ1������ʱ
                    FirstRunFlag = TRUE;            //��һ������
                    FirstRunCnt = 0;                //��һ�����м������� 
                    RPPowUpReset = TRUE;           //��λ�� �ϵ��Ƿ����м�
                    SwitchRFtoWorkMode();           //����ң��ת�빤��ģʽ
                    
                    //OpenLed1(); OpenLed2(); OpenLed3(); OpenLed4(); OpenLed5();         //����һ��
                    
                    Bsp_Beep_On();                  //ң������һ��
                    Delay_Ms(200);
                    Bsp_Beep_Off();
                    Delay_Ms(800);
                    CloseLed1(); CloseLed2(); CloseLed3(); CloseLed4(); CloseLed5();
                }
            }
            else
            {
                if (PoweFlag == FALSE && ChargeFlag == FALSE)                           //�ر�ң���� �� ���ڳ��
                {
                    //CloseLed1(); CloseLed2(); CloseLed3(); CloseLed4(); CloseLed5();
                  
                    Bsp_Beep_Off();
                    ToPowdown();                //�������ֱ�ӽ�����ģʽ
                }
                else
                {
                    if (keycnt == 0x80)  //��������
                    {
                        if (keytime < 255)
                            keytime++;        //������ʱ���� 0-255 2550ms

                        if (keytime1 != 0 && keytime1 < 500)
                        {
                            keytime1++;      //������ʱ1���� 
                        }

                        if (keytime1 == 400)   //400 *10ms = 4s ����������������ģʽ
                        {
                            SwitchRFtoAddrSendMode();           //�л�����ַ����ģʽ
                            RF_State = RF_Disconnect;           //�л�������״̬
                            RF_Cont = 0;
                            BatteryNum = 0;                     //��ص������
                        }
                    }

                    if (keycnt == 0x00)    //�����ͷ�
                    {
                    keytime1 = 0;
                    }

                    AutoPowerOff_Proce();                       //�Զ��ػ�����
                    //RF_Proce();                                 //ң��������
                    InPut_Proce();                              //���봦��
                    Charge_Proce();                             //��紦��
                    Key_Proce();                                //��������
                    // RF_Proce_1();                               //ң�������մ���    
                    ADC_Proce();                                //ADC����
                    FirstRunFlag_Proce();                       //��һ�����д���
                    LED_Proce();                                //LED�ƴ���

                }
            }
            
            if(WakeupFlag != TRUE)                          //20170221
            {
                RF_Proce();
                if(SendTick >= 7)                             //20170221 7x2=14ms
                {
                    SendTick = 0;
                    RF_SendData();
                }
            }
        }
    }
#endif
}



#if 0
void main(void)
{
    int i = 0, j = 0,k = 0;
    Bsp_Init();
    Bsp_DelayMs(1000);
    Bsp_Motor_On();
    
    Mod_OLEDInit();
    Mod_OLEDDisplay((INT8U*)bkgpic);
    
    while (1)
    {
        //Bsp_Beep_On();
        Bsp_DelayMs(300);
        //Bsp_Beep_Off();
        Bsp_DelayMs(300);
        
        Mod_OLEDDisp_BatteryBar(i);
        
        Mod_OLEDDisp_PowBar(j);
        
        if(++i > 5)
            i = 0;
        
        if(++j > 6)
            j = 0;
        
        if(++k > 99)
            k = 0;
        
        Mod_OLEDDisp_ChrgeIcon(i%2);
        Mod_OLEDDisp_RfStateIcon(i%2);
        Mod_OLEDDisp_BoardChrgeIcon(i%2);
        Mod_OLEDDisp_HighSpeedIcom(i%2);
        Mod_OLEDDisp_LowSpeedIcom(i%2);
        Mod_OLEDDisp_PatrolIcom(i%2);
        
        Mod_OLEDDisp_SpeedNum(k);
        
        /*
        Mod_OLEDDispIcon_8x8(96,0,(INT8U*)ChrgeIcon);
        Mod_OLEDDispIcon_8x8(84,0,(INT8U*)RfStateIcon);
        
        Mod_OLEDDispIcon_8x8(24,0,(INT8U*)BoardChrgeIcon);
        Mod_OLEDDispIcon_8x8(24,1,(INT8U*)HighSpeedIcom);
        Mod_OLEDDispIcon_8x8(24,2,(INT8U*)LowSpeedIcom);
        Mod_OLEDDispIcon_8x8(24,3,(INT8U*)PatrolIcom);
        
        Mod_OLEDDispNum_32x16(40,5);
        Mod_OLEDDispNum_32x16(60,1);*/
    }   
}
#endif


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
