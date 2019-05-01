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
    if (Bsp_STDBY_Read())                 //�������˿ڼ��
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


    if (Bsp_CHRG_Read())                 //���ڳ��˿ڼ��
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

    //Delay200us();                   //��ʱ200us �ȴ������˿ڵ�ƽ�ȶ�

    if (Bsp_POWKEY_Read())                   //�����˿ڼ��
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

//�л�ң��ģ�鵽���͵�ַģʽ �������
void SwitchRFtoAddrSendMode(void)   
{
    RF_Mode = RF_AddrSendMode;
    SetTXAddr(Common_Addr); //������ַ��
}
//�л�ң��ģ�鵽����ģʽ
void SwitchRFtoWorkMode(void)       
{
    RF_Mode = RF_WorkMode;
    SetTXAddr(TX_Addr);     //˽�õ�ַ��
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
        Bsp_Beep_On();                          //��������һ��    
        Delay_Ms(200);
        Bsp_Beep_Off();
    }

    //if( keytime == 252 )      //250*20ms = 5s  
    //{
      //������
    //}
}

//Time2�жϷ�����
void Tim4_Updata_Irq(void)                      
{
    static UINT16 ms_cnt = 0;

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

/*
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
 */


//��ȡADCƫ��ֵ
void ADC_GetBase(void)                         
{
    UINT8 i;
    UINT16 	Ad_val = 0;
    UINT8	ad_cnt = 0;
    UINT16	ad_sum = 0;
    UINT16	ad_max = 0;
    UINT16	ad_min = 0;
    while (ad_cnt < 34)                         //ȡ34�ν��
    {
        ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE,
                              ADC1_CHANNEL_9,
                              ADC1_ALIGN_RIGHT);

        ADC1_StartConversion();                 //����ת��
        
        while (ADC1_GetFlagStatus(ADC1_FLAG_EOC) != SET)        //�ȴ�ת������
        {
            
        }
        
        Ad_val = ADC1_GetConversionValue();     //��ȡADC��ֵ

        if (ad_cnt == 0)
        {
            ad_max = ad_min = ad_sum = Ad_val;
        }
        else
        {
            ad_sum += Ad_val;

            if (Ad_val > ad_max)
                ad_max = Ad_val;
            if (Ad_val < ad_min)
                ad_min = Ad_val;

        }

        ad_cnt++;
        if (ad_cnt >= 34)
        {
            ad_sum = ad_sum - ad_max - ad_min;
        }

    }
    Ad_Base = ad_sum >> 5;                  //���ƽ��ֵ
    for (i = 0; i < 10; i++)            
    {
        AD_Buff[i] = Ad_Base;               //���buff
    }
}

//ADC����
void ADC_Proce(void)                    
{
    UINT16 times = 0;                    //20170220 ���ӣ�Ϊ�˽��ң����ʧ��������
		
    INT16 Ad_val = 0;
    
    ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE,
                          ADC1_CHANNEL_9,
                          ADC1_ALIGN_RIGHT);

    ADC1_StartConversion();             //����ת��
    while (ADC1_GetFlagStatus(ADC1_FLAG_EOC) != SET)      //20170220 ���� Ϊ�˽��ң����ʧ��������
    {
        times++;                           //20170220 ���ӣ�Ϊ�˽��ң����ʧ��������
        if(times > 1000)                   //20170220 ���ӣ�Ϊ�˽��ң����ʧ��������
        {
            times = 0;
            return;                         //20170220 ���ӣ� Ϊ�˽��ң����ʧ��������
        }
    }
    
    Ad_val = ADC1_GetConversionValue(); //��ȡADCת�����
    RP_ADValue = Ad_Base - Ad_val;      //ƫ������ȥ�ɼ���  ��õ�λ��λ��

    if (((UINT16)(Ad_val)) < 40 || ((UINT16)(Ad_val)) > (1024 - 40))//ADCת�����̫С �� ̫�� ����ң�������߱���
    {
        if (err_rp_flag == 0)
            err_rp_flag = 1;                //ң�������߱���
    }

    ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE,
                          ADC1_CHANNEL_8,
                          ADC1_ALIGN_RIGHT);
    ADC1_StartConversion();                 //����ת��
    
    while (ADC1_GetFlagStatus(ADC1_FLAG_EOC) != SET)       //20170220 ���� Ϊ�˽��ң����ʧ��������
    {
        times++;                           //20170220 ���ӣ�Ϊ�˽��ң����ʧ��������
        if(times > 1000)                   //20170220 ���ӣ�Ϊ�˽��ң����ʧ��������
        {
            times = 0;
            return;                         //20170220 ���ӣ� Ϊ�˽��ң����ʧ��������
        }
    }
    
    Ad_val = ADC1_GetConversionValue();     //��ȡADCת���� ��

    if (ADCPowVal_Cnt == 0)                 
    {
        ADCPowVal_Sum = 0;
        ADCPowVal_Max = Ad_val;
        ADCPowVal_Min = Ad_val;
    }
    else
    {
        if (Ad_val < ADCPowVal_Min)
            ADCPowVal_Min = Ad_val;
        if (Ad_val > ADCPowVal_Max)
            ADCPowVal_Max = Ad_val;
    }
    ADCPowVal_Sum += Ad_val;

    ADCPowVal_Cnt++;
    if (ADCPowVal_Cnt >= 10)
    {

        ADCPowVal_Cnt = 0;
        ADCPowVal_Fix = (ADCPowVal_Sum - ADCPowVal_Min - ADCPowVal_Max) >> 3;
        if (ADCPowVal_Fix > 540)
            ADCPowVal_LPFlag = TRUE;
        else
            ADCPowVal_LPFlag = FALSE;

    }
}



//ң������������
void RF_SendData(void)              
{
    static UINT8 RfTick = 0;
    UINT8 i;
    //if (RfTick >= 1)     //10ms * 10 = 100ms    //20170111��10��Ϊ2�շ����ڴ�100ms��Ϊ20ms   ��������2�η�1��  //20170215��1 //20170219 ��0��Բ���
    if(1)
    {
        RfTick = 0;
        SwitchToTxMode();           //�л�������ģʽ
        for (i = 0; i < 20; i++);     //��5us

        if (RF_Mode == RF_AddrSendMode)     //�ڷ��͵�ַģʽ ���ģʽ    
        {

            TX_Buff[0] = 0xcc;              //֡ͷ    
            TX_Buff[1] = TX_Addr[0];        //���͵�ַ
            TX_Buff[2] = TX_Addr[1];        
            TX_Buff[3] = TX_Addr[2];
            TX_Buff[4] = TX_Addr[3];
            TX_Buff[5] = TX_Addr[4];
            TX_Buff[6] = TX_Buff[0] + TX_Buff[1] + TX_Buff[2]
                + TX_Buff[3] + TX_Buff[4] + TX_Buff[5]; //У��

            Send_Packet(WR_TX_PLOAD, TX_Buff, 7);   //�������ݰ�

        }
        else
        {                                   //����ģʽ
            UINT8 i;                        //ADȡƽ��  �Ѳ���
            UINT16 max = AD_Buff[0];
            UINT16 min = AD_Buff[0];
            UINT16 sum = 0;

            for (i = 0; i < 10; i++)
            {
                if (AD_Buff[i] > max)
                    max = AD_Buff[i];
            }

            for (i = 0; i < 10; i++)
            {
                if (AD_Buff[i] < min)
                    min = AD_Buff[i];
            }

            for (i = 0; i < 10; i++)
            {
                sum += AD_Buff[i];
            }

            sum = sum - max - min;
            //RP_ADValue = sum/8;
            //RP_ADValue = Ad_Base - RP_ADValue;
						
            if (RP_ADValue < 58 && RP_ADValue > -58) //20170122��������ң�����������������Ϳ��Բ�������
              RP_ADValue = 10;                       //20170122��������ң�����������������Ϳ��Բ�������

            TX_Buff[0] = 0xa5;                          //֡ͷ
            if (RPPowUpReset == FALSE &&  err_rp_flag == 0)
            {                                               //��λ��� ���� ��λ��δ���� �����λ����ֵ                                                
                TX_Buff[1] = (UINT8)(RP_ADValue >> 8);      //��λ����8λ
                TX_Buff[2] = (UINT8)(RP_ADValue & 0x00ff);  //��λ����8λ                
            }
            else
            {                       //��λδ������0
                TX_Buff[1] = 0;     //��λ����8λ
                TX_Buff[2] = 0;  //��λ����8λ                
            }

            TX_Buff[3] = ((SpeedMode & 0x01) << 0) | ((DirMode & 0x01) << 1 | (err_code == 4) << 2);    //b0 �ٶ�ģʽ b1 ������� b2 �ػ�ָ��
            TX_Buff[4] = 0xa5 + TX_Buff[1] + TX_Buff[2] + TX_Buff[3];   //У���
            Send_Packet(WR_TX_PLOAD, TX_Buff, 5);       //�������ݰ�   
					//	Send_Packet(WR_TX_PLOAD, TX_Buff, 5);       //�������ݰ�    20170121����һ�η��ͣ�����ң����1��2��LED��˸
        }

       // Delay_Ms(10);  //�ȴ�10ms 20170121ԭ10��12�����ӷ��͵�ʱ�䣬����ң����1��2��LED��˸ //20170218�������ң����������ʧ��������
    }
    else
    {
        RfTick++;
    }

   //if (RfTick == 1)      //20170215��
    SwitchToRxMode();                                   //�л�������ģʽ
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

            SwitchRFtoWorkMode();                                       //�л�������ģʽ
            ADC_GetBase();                                             //��ȡADCƫ��ֵ
            Bsp_Beep_On();                                              //�� �� ��~
            Delay_Ms(100);
            Bsp_Beep_Off();
            Delay_Ms(100);
            Bsp_Beep_On();
            Delay_Ms(100);
            Bsp_Beep_Off();
            Delay_Ms(100);
            Bsp_Beep_On();
            Delay_Ms(1000);
            Bsp_Beep_Off();
        }
    }
    return 0;
}

//ң�ش���
void RF_Proce(void)                                     
{
    if (ChargeFlag == FALSE)                            //�Ƿ��ڳ�� 
    {                                                   //�����ʱ�Ž��ܺͷ�������
        //Receive_Packet();                               //��������
        //RF_SendData();                                  //��������
        if(Receive_Packet() != 0) 
        {
                RF_SendData();
                SendTick = 0;
        }

        if (RF_WDog == 250)                             //10ms * 250 = 2.5s ��û�н��ܵ�����
        {
            RF_State = RF_Disconnect;                   //����ģ��״̬Ϊ����
            RF_Cont = 0;
            BatteryNum = 0;                             //��ص���Ϊ0
        }
    }
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

//LED����
void LED_Proce(void)                                                    
{
#if 0
    static UINT8 led_time = 0;
    led_time++;
    if (ChargeFlag == FALSE)                                //�Ƿ��ڳ��
    {
        /**/                                                //���ڳ�� ��������
        if (err_code != 0 || err_rp_flag == 1)              //�д���  
        {
            static UINT8 led_cnt = 0;                       //led����
            if (err_code != 4)                              //���ǹ��¶ȱ���
            {                                                   
                if (led_cnt == 0xff)                        //����һ��       
                {
                    if (led_time < 40)
                    {
                        CloseLed1();
                        CloseLed2();
                    }
                    else
                    {
                        led_time = 0;
                        led_cnt = 0;
                    }
                }
                else
                {
                    static UINT8 led_flag = 0;
                    if ((err_rp_flag == 0 && led_cnt < err_code)    //����err_code�Ĵ���
                        || (err_rp_flag == 1 && led_cnt < 4))       //��λ��������˸4��    
                    {
                        if (led_time < 25)
                        {
                            OpenLed1();
                            OpenLed2();

                        }
                        else if (led_time < 50)
                        {
                            CloseLed1();
                            CloseLed2();

                        }
                        else
                        {
                            led_time = 0;
                            led_cnt++;                              //��˸��������    
                        }
                    }
                    else
                    {
                        led_cnt = 0xff;
                    }
                }
            }
            else
            {                                               //���¶ȱ��� ����LED��˸ ��������
                if (led_time < 25)                          
                {
                    OpenLed1();
                    OpenLed2();
                    OpenLed3();
                    OpenLed4();
                    OpenLed5();
                    Beep_Enable();
                }
                else if (led_time < 50)
                {
                    CloseLed1();
                    CloseLed2();
                    CloseLed3();
                    CloseLed4();
                    CloseLed5();
                    Beep_Disable();
                }
                else
                {
                    led_time = 0;
                }
                /*
                if(led_cnt == 0xff)
                {
                  if( led_time < 25 )
                  {
                    CloseLed1();
                    CloseLed2();
                  }
                  else
                  {
                    led_time = 0;
                    led_cnt = 0;
                  }
                }
                else
                {
                  static UINT8 led_flag = 0;
                  if((err_rp_flag == 0 && led_cnt < err_code)
                    ||(err_rp_flag == 1 && led_cnt < 10) )
                  {
                    if( led_time < 25 )
                    {
                      OpenLed1();
                      OpenLed2();
                    }
                    else if(led_time < 50)
                    {
                      CloseLed1();
                      CloseLed2();
                    }
                    else
                    {
                      led_time = 0;
                      led_cnt++;
                    }
                  }
                  else
                  {
                    led_cnt = 0xff;
                  }
                }
                */
            }
        }
        else
        {
            if (RF_Mode == RF_WorkMode)                     //����ģʽ
            {
                if (SpeedMode == LowSpeedMode)              //����ģʽ
                {
                    if (RF_State == RF_Disconnect)          //�Ͽ�������LED1
                    {
                        if (led_time < 50)
                        {
                            CloseLed1();
                        }
                        else if (led_time < 100)
                        {
                            OpenLed1();
                        }
                        else
                        {
                            led_time = 0;
                        }
                    }
                    else
                        OpenLed1();                         //����ʱ����LED1
                    CloseLed2();                            //�ر�LED2
                }
                else if (SpeedMode == HighSpeedMode)        //����ģʽ
                {
                    if (RF_State == RF_Disconnect)          //�Ͽ�������LED2
                    {
                        if (led_time < 50)
                        {
                            CloseLed2();
                        }
                        else if (led_time < 100)
                        {
                            OpenLed2();
                        }
                        else
                        {
                            led_time = 0;
                        }
                    }
                    else
                        OpenLed2();                         //����ʱ����LED2
                    CloseLed1();                            //�ر�LED1
                }
                else
                {

                }
                //led 34
                if (BatteryNum == 0)                        //��������ص�������LED LED5��Ҫ����ң��������
                {
                    CloseLed4();
                    CloseLed3();
                }
                else if (BatteryNum <= 10)
                {
                    CloseLed4();
                    CloseLed3();
                }
                else if (BatteryNum <= 40)
                {
                    CloseLed4();
                    CloseLed3();
                }
                else if (BatteryNum <= 70)
                {
                    OpenLed4();
                    CloseLed3();
                }
                else if (BatteryNum <= 100)
                {
                    OpenLed4();
                    OpenLed3();
                }
                else
                {

                }


            }
            else        
            {                                               //���ģʽ
                static UINT8 i;
                if (++i >= 10)                              
                    i = 0;

                if (i >= 5)                                 //����LED
                {
                    if (SpeedMode == LowSpeedMode)          //�����ٶ�ģʽ���������ĸ�LED
                    {
                        OpenLed1();
                    }
                    else
                    {
                        OpenLed2();
                    }
                }
                else
                {
                    CloseLed1();
                    CloseLed2();
                }

                CloseLed3();
                CloseLed4();

            }

        }


        //led 5 �Ĵ���
        /**/
        if (err_code != 4)
        {   //û�й��±���
            if (ADCPowVal_LPFlag && (BatteryNum > 0 && BatteryNum <= 10))   //ң����û�� �� �����û��
            {
                //�����ӿ���
                static uint8_t led_time1 = 0;
                static uint8_t led_cnt = 0;
                static uint8_t led_status = 0;

                if (led_status == 0)  //1s
                {
                    if (++led_time1 >= 100)
                    {
                        led_time1 = 0;
                        led_status = 1;
                    }
                    CloseLed5();
                    Beep_Disable();

                }
                else if (led_status == 1)
                {
                    //ң����û������
                    led_time1++;
                    if (led_time1 < 50)
                    {
                        OpenLed5();
                        Beep_Enable();
                    }
                    else if (led_time1 < 100)
                    {
                        CloseLed5();
                        Beep_Disable();
                    }
                    else
                    {
                        led_time1 = 0;
                        if (++led_cnt >= 3)
                        {
                            led_cnt = 0;
                            led_status = 2;
                        }
                    }
                }
                else if (led_status == 2)  //1s
                {
                    if (++led_time1 >= 100)
                    {
                        led_time1 = 0;
                        led_status = 3;
                    }

                    CloseLed5();
                    Beep_Disable();

                }
                else
                {
                    //�����û��ң��������
                    led_time1++;
                    if (led_time1 < 20)
                    {
                        OpenLed5();
                        Beep_Enable();
                    }
                    else if (led_time1 < 40)
                    {
                        CloseLed5();
                        Beep_Disable();
                    }
                    else
                    {
                        led_time1 = 0;
                        if (++led_cnt >= 6)
                        {
                            led_cnt = 0;
                            led_status = 0;
                        }
                    }
                }
            }
            else if (ADCPowVal_LPFlag)                              //ң����û��
            {
                //ң����û������
                static uint8_t led_time1 = 0;

                if (++led_time1 > 100)
                {
                    led_time1 = 0;
                }

                if (led_time1 < 50)
                {
                    OpenLed5();
                    Beep_Enable();
                }
                else if (led_time1 < 100)
                {
                    CloseLed5();
                    Beep_Disable();
                }


            }
            else                                                    //�������
            {
                if (RF_State == RF_Connect)
                {
                    if (BatteryNum == 0)
                    {
                        CloseLed5();
                       // Beep_Disable();   //20170220ɾ��Ϊ��ʹ�����û�е���ʱ���������ٽ�
											 if (led_time < 10)     //20170220ɾ��Ϊ��ʹ�����û�е���ʱ���������ٽ� 
                        {
                            Beep_Enable();    //20170220ɾ��Ϊ��ʹ�����û�е���ʱ���������ٽ� 
                        }
                        else if (led_time < 20)   //20170220ɾ��Ϊ��ʹ�����û�е���ʱ���������ٽ�
                        {
                            Beep_Disable();       //20170220ɾ��Ϊ��ʹ�����û�е���ʱ���������ٽ�
                        }
                        else
                        {
                            led_time = 0;         //20170220ɾ��Ϊ��ʹ�����û�е���ʱ���������ٽ�
                        }
                    }
                    else if (BatteryNum <= 10)
                    {
                        //�����û��ң��������
                        if (led_time < 20)
                        {
                            OpenLed5();
                            Beep_Enable();
                        }
                        else if (led_time < 40)
                        {
                            CloseLed5();
                            Beep_Disable();
                        }
                        else
                        {
                            led_time = 0;
                        }
                    }
                    else
                    {
                        Beep_Disable();
                        OpenLed5();
                    }
                }
                else
                {
                    Beep_Disable();
                    OpenLed5();
                }

            }
        }
        /*
            if(err_code == 4 )//|| ADCPowVal_LPFlag)
            {
              static beep_cnt = 0;
              if(++beep_cnt >= 40)
              {
                beep_cnt = 0;
              }

              if( beep_cnt < 20)
              {
                Beep_Enable();
              }
              else
              {
                Beep_Disable();
              }
            }
        */
    }
    else                                    
    {   //���LED��˸ 
        static UINT8 ledtime = 0;

        if (stdbcnt == 0x00)
        {
            OpenLed1(); OpenLed2(); OpenLed3();
            OpenLed4(); OpenLed5();
        }
        else
        {
            //2.75~4.2  base 2.8 dv 0.23
            //2.8(540)3.15(510) 3.5(466) 3.85(432) 4.2(409)
            //2.8(540)3.03(522) 3.26(499) 3.5(464) 3.72(447)3.95(430)
            ++ledtime;
            if (ledtime < 40)
            {
                CloseLed1(); CloseLed2(); CloseLed3();
                CloseLed4(); CloseLed5();
            }
            else if (ledtime < 80)
            {
                CloseLed1(); CloseLed2(); CloseLed3();
                CloseLed4(); OpenLed5();
            }
            else if (ledtime < 120)
            {
                CloseLed1(); CloseLed2(); CloseLed3();
                OpenLed4(); OpenLed5();
            }
            else if (ledtime < 160)
            {
                CloseLed1(); CloseLed2(); OpenLed3();
                OpenLed4(); OpenLed5();
            }
            else if (ledtime < 200)
            {
                CloseLed1(); OpenLed2(); OpenLed3();
                OpenLed4(); OpenLed5();
            }
            else if (ledtime < 240)
            {
                OpenLed1(); OpenLed2(); OpenLed3();
                OpenLed4(); OpenLed5();
            }
            else
            {
            //2.75~4.2  base 2.8 dv 0.23
            //2.8(540)3.15(510) 3.5(466) 3.85(432) 4.2(409)
            //2.8(540)3.03(522) 3.26(499) 3.5(464) 3.72(447)3.95(430)
                if (ADCPowVal_Fix > 540)                //2.8V
                    ledtime = 0;
                else if (ADCPowVal_Fix > 510)           //3.15V
                    ledtime = 40;
                else if (ADCPowVal_Fix > 466)           //3.5V
                    ledtime = 80;
                else if (ADCPowVal_Fix > 432)           //3.85V
                    ledtime = 120;
                else if (ADCPowVal_Fix > 409)           //4.2V
                    ledtime = 160;
                else
                    ledtime = 160;
            }
        }


        /*
        if( stdbcnt == 0x00 )
        {
          OpenLed1();
          CloseLed2();
          CloseLed3();
          CloseLed4();
          CloseLed5();
        }
        else
        {
          if( led_time < 50 )
          {
            CloseLed1();
            CloseLed2();
          }
          else if( led_time < 100 )
          {
            CloseLed1();
            OpenLed2();
          }
          else if( led_time < 150 )
          {
            OpenLed1();
            OpenLed2();
          }
                else
          {
            led_time = 0;
          }
                OpenLed3();
                OpenLed4();
                OpenLed5();
        }
        */
        Beep_Disable();
    }

#endif
}


//�������ģʽ
void ToPowdown(void)        
{
    Bsp_IntDis();           //�ر��ж�
    //CloseLed1(); CloseLed2(); CloseLed3(); CloseLed4(); CloseLed5();    //�ر�LED
    BK2425_SetToPD();       //����ң��ģ�鵽����ģʽ
    Delay_Ms(10);           //��ʱ

    PoweFlag = FALSE;       //����

    Delay_Ms(10);           //��ʱ

    Bsp_POW_Low();          //�رյ�Դ
    while(1){}
}


void AutoPowerOff_Proce(void)
{
    if (RP_ADValue > 40 || RP_ADValue < -40 || ChargeFlag == TRUE)  //ң�����ж��������ڳ��
    {
        AutoPowerDownTime = 0;
    }

    if (AutoPowerDownTime > 90000)//10ms * (100 * 60 * 15)
    {
        //CloseLed1(); CloseLed2(); CloseLed3(); CloseLed4(); CloseLed5();    //�ر�LED
        Bsp_Beep_On();              //��������һ��
        Delay_Ms(200);
        Bsp_Beep_Off();
        ToPowdown();                //�������ֱ�ӽ�����ģʽ
    }
}

void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, 249);   //(249+1) / 125000 = 2ms
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
    /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}

void Value_Init(void)
{
    keytime = 0;
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

    Mod_OLEDDisp_ChrgeIcon(0);
    Mod_OLEDDisp_RfStateIcon(0);
    Mod_OLEDDisp_BoardChrgeIcon(0);
    Mod_OLEDDisp_HighSpeedIcom(0);
    Mod_OLEDDisp_LowSpeedIcom(0);
    Mod_OLEDDisp_PatrolIcom(0);


    Delay_Ms(100);
   
    GetChipID();
    BK2425_Initialize((void*)RF_DataProcess, TX_Addr, RX1_Addr, TX_Addr);     //�Լ��ĵ�ַ���ͽ��� 
    

    /* De-Init ADC peripheral*/
    ADC1_DeInit();

    /* Init ADC2 peripheral */
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_9, ADC1_PRESSEL_FCPU_D2, \
              ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL9,\
              DISABLE);

    TIM4_Config();          
              

    Value_Init();  
    
    Bsp_IntEn();      //�����ж�

}


void main(void)
{
    System_Init();                  //ϵͳ��ʼ��
    ADC_GetBase();                 //��ȡADCƫ��ֵ

    while(1)
    {
        if (SysTick)                //10ms
        {  
            SysTick = 0;
            
            
            InPut_Proce();
        }
    }

#if 0
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
                    RPPowUpReset = TRUE;            //��λ�� �ϵ��Ƿ����м�
                    SwitchRFtoWorkMode();           //����ң��ת�빤��ģʽ
                    
                    //OpenLed1(); OpenLed2(); OpenLed3(); OpenLed4(); OpenLed5();         //����һ��
                    
                    Bsp_Beep_On();                  //ң������һ��
                    Delay_Ms(200);
                    Bsp_Beep_Off();
                    Delay_Ms(800);
                    //CloseLed1(); CloseLed2(); CloseLed3(); CloseLed4(); CloseLed5();
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
