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

//输入处理
void InPut_Proce(void)  
{
    if (Bsp_STDBY_Read())                 //充电充满端口检测
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


    if (Bsp_CHRG_Read())                 //正在充电端口检测
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

    //Delay200us();                   //延时200us 等待按键端口电平稳定

    if (Bsp_POWKEY_Read())                   //按键端口检测
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
//充电端口处理
void Charge_Proce(void)             
{
    if (chargecnt == 0x00)
        ChargeFlag = TRUE;          //充电端口正在充电
    else if (chargecnt == 0x80)
    {
        if (stdbcnt == 0x00)
            ChargeFlag = TRUE;      //充电端口正在充电且电充满
        else
            ChargeFlag = FALSE;     //充电端口未充电
    }
}
//切换速度模式
static void Switch_SpeedMode(void)      
{
    if (FirstRunFlag == TRUE)           //电位器是否动作过 没有动作过则切换速度模式
    {
        if (SpeedMode == LowSpeedMode)
            SpeedMode = HighSpeedMode;
        else
            SpeedMode = LowSpeedMode;
    }
}
//切换电机方向    
static void Switch_RunDir(void)         
{
    if (FirstRunFlag == FALSE)          //电位器是否动作过 动作过则切换方向
    {                                   
        if (DirMode == Forward)         //    
            DirMode = Reverse;
        else
            DirMode = Forward;
    }
}

//切换遥控模块到发送地址模式 用于配对
void SwitchRFtoAddrSendMode(void)   
{
    RF_Mode = RF_AddrSendMode;
    SetTXAddr(Common_Addr); //公共地址发
}
//切换遥控模块到工作模式
void SwitchRFtoWorkMode(void)       
{
    RF_Mode = RF_WorkMode;
    SetTXAddr(TX_Addr);     //私用地址发
}

//第一次运行处理    
static void FirstRunFlag_Proce(void)    
{
    if ( RPPowUpReset == TRUE )                     //电位器上电时要先复位 回到中间位置
    {
        if ( RP_ADValue < 60 )                      //小于阀值
            RPPowUpReset = FALSE;                   //回到中间位置
    }
    else
    {
        if (RP_ADValue > 60 || RP_ADValue < -60)    //电位器前推 或 刹车
        {
            if (FirstRunCnt >= 3)                   //3*10ms = 30ms 超过30ms
            {
                FirstRunFlag = FALSE;               //改为电位器已经动作过
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
    if (keycnt == 0x00)    //按键释放
    {
        if (keytime != 0 && keytime < 50)       //按键按下的时间小于 50*20ms = 1s 
        {                                       //短按    
            Switch_SpeedMode();                 //切换速度模式
            Switch_RunDir();                    //切换电机方向
        }
        keytime = 0;
    }

    if (keytime == 200)                         //按键按下的时间到达 200*10ms = 2s  
    {
        PoweFlag = FALSE;                       //断电
        Bsp_Beep_On();                          //蜂鸣器滴一声    
        Delay_Ms(200);
        Bsp_Beep_Off();
    }

    //if( keytime == 252 )      //250*20ms = 5s  
    //{
      //对码用
    //}
}

//Time2中断服务处理
void Tim4_Updata_Irq(void)                      
{
    static UINT16 ms_cnt = 0;

    if (SendTick < 255)                     //系统时间片累加
    SendTick++;

    if(ms_cnt%5 == 0)
    {
        if (SysTick < 250)                      //系统时间片累加
            SysTick++;

        if (RF_WDog < 255)                      //无线短线时间累加    //20170123从255改为250，为了修改有时遥控器失去和主控的联系后12号灯不闪，但改为250之后经常会遥控器灯老会显示和主控失去联系但事实遥控器又能控制主控
            RF_WDog++;

        if (AutoPowerDownTime < 10000000)       //自动关机时间累加
            AutoPowerDownTime++;
    }
}

/*
//按键中断服务处理函数 唤醒用
void KEY_INTERRUPT_IRQ(void)                    
{
    UINT8 i = 0;
    for (i = 0; i < 128; i++)                   //换新后 先进行 128次输入采集
    {
        InPut_Proce();                          //输入处理             
        Delay_Ms(1);                            //延时1ms
    }

    if (keycnt == 0x80)                         //判断按键是否按下
    {
        WakeupFlag = TRUE;                      //开启唤醒
    }
    else
    {                                           //按键抖动
        WakeupFlag = FALSE;                     //不唤醒
        PoweFlag = FALSE;                       //不上电    
    }
}
//充电端口中断处理服务函数 唤醒用
void CHARGE_INTERRUPT_IRQ(void)             
{
    UINT8 i = 0;                                        
    for (i = 0; i < 128; i++)               //换新后 先进行 128次输入采集          
    {                                       
        InPut_Proce();                      //输入处理             
        Delay_Ms(2);                        //延时2ms
    }                                       
                                            
    if (chargecnt == 0x00)                  //判断按键是否按下
        ChargeFlag = TRUE;                  //充电端口唤醒  
    else                                    //开启唤醒
        ChargeFlag = FALSE;                 //充电端口抖动  
}                                           

void Wakeup_Enable(void)
{
    EXTI->CR1 &= 0x3c;            //PA口上升沿中断PD口下降沿中断
    EXTI->CR1 |= 0x81;

    PA_CR1 &= (~KEYPIN);          //浮空输入
    PA_CR2 |= (KEYPIN);           //开启按键输入口中断  
    PA_DDR &= (~KEYPIN);          //按键输入

    PD_CR1 &= (~CHRGEPIN);        //浮空输入
    PD_CR2 |= (CHRGEPIN);         //开启按键输入口中断  
    PD_DDR &= (~CHRGEPIN);        //按键输入

    //ITC->ISPR1 &= 0x3f;
    //ITC->ISPR1 |= 0x80;
}

void Wakeup_Disable(void)
{
    EXTI->CR1 &= 0x3c;            //PA口上升沿中断PD口下降沿中断
    EXTI->CR1 |= 0x81;

    PA_CR1 &= (~KEYPIN);          //浮空输入  
    PA_CR2 &= (~KEYPIN);          //关闭按键输入口中断
    PA_DDR &= (~KEYPIN);          //按键输入

    PD_CR1 &= (~CHRGEPIN);        //浮空输入
    PD_CR2 &= (~CHRGEPIN);        //关闭按键输入口中断  
    PD_DDR &= (~CHRGEPIN);        //按键输入

    //ITC->ISPR1 &= 0x3f;           //中断优先级 
}
 */


//获取ADC偏置值
void ADC_GetBase(void)                         
{
    UINT8 i;
    UINT16 	Ad_val = 0;
    UINT8	ad_cnt = 0;
    UINT16	ad_sum = 0;
    UINT16	ad_max = 0;
    UINT16	ad_min = 0;
    while (ad_cnt < 34)                         //取34次结果
    {
        ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE,
                              ADC1_CHANNEL_9,
                              ADC1_ALIGN_RIGHT);

        ADC1_StartConversion();                 //开启转换
        
        while (ADC1_GetFlagStatus(ADC1_FLAG_EOC) != SET)        //等待转换结束
        {
            
        }
        
        Ad_val = ADC1_GetConversionValue();     //获取ADC的值

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
    Ad_Base = ad_sum >> 5;                  //算出平均值
    for (i = 0; i < 10; i++)            
    {
        AD_Buff[i] = Ad_Base;               //填充buff
    }
}

//ADC处理
void ADC_Proce(void)                    
{
    UINT16 times = 0;                    //20170220 增加，为了解决遥控器失联的问题
		
    INT16 Ad_val = 0;
    
    ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE,
                          ADC1_CHANNEL_9,
                          ADC1_ALIGN_RIGHT);

    ADC1_StartConversion();             //开启转换
    while (ADC1_GetFlagStatus(ADC1_FLAG_EOC) != SET)      //20170220 增加 为了解决遥控器失联的问题
    {
        times++;                           //20170220 增加，为了解决遥控器失联的问题
        if(times > 1000)                   //20170220 增加，为了解决遥控器失联的问题
        {
            times = 0;
            return;                         //20170220 增加， 为了解决遥控器失联的问题
        }
    }
    
    Ad_val = ADC1_GetConversionValue(); //获取ADC转换结果
    RP_ADValue = Ad_Base - Ad_val;      //偏置量减去采集量  获得电位器位置

    if (((UINT16)(Ad_val)) < 40 || ((UINT16)(Ad_val)) > (1024 - 40))//ADC转换结果太小 或 太大 出发遥控器短线报警
    {
        if (err_rp_flag == 0)
            err_rp_flag = 1;                //遥控器短线报警
    }

    ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE,
                          ADC1_CHANNEL_8,
                          ADC1_ALIGN_RIGHT);
    ADC1_StartConversion();                 //开启转换
    
    while (ADC1_GetFlagStatus(ADC1_FLAG_EOC) != SET)       //20170220 增加 为了解决遥控器失联的问题
    {
        times++;                           //20170220 增加，为了解决遥控器失联的问题
        if(times > 1000)                   //20170220 增加，为了解决遥控器失联的问题
        {
            times = 0;
            return;                         //20170220 增加， 为了解决遥控器失联的问题
        }
    }
    
    Ad_val = ADC1_GetConversionValue();     //获取ADC转换结 果

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



//遥控器发送数据
void RF_SendData(void)              
{
    static UINT8 RfTick = 0;
    UINT8 i;
    //if (RfTick >= 1)     //10ms * 10 = 100ms    //20170111从10改为2收发周期从100ms改为20ms   主控是收2次发1次  //20170215改1 //20170219 改0配对不上
    if(1)
    {
        RfTick = 0;
        SwitchToTxMode();           //切换到发送模式
        for (i = 0; i < 20; i++);     //等5us

        if (RF_Mode == RF_AddrSendMode)     //在发送地址模式 配对模式    
        {

            TX_Buff[0] = 0xcc;              //帧头    
            TX_Buff[1] = TX_Addr[0];        //发送地址
            TX_Buff[2] = TX_Addr[1];        
            TX_Buff[3] = TX_Addr[2];
            TX_Buff[4] = TX_Addr[3];
            TX_Buff[5] = TX_Addr[4];
            TX_Buff[6] = TX_Buff[0] + TX_Buff[1] + TX_Buff[2]
                + TX_Buff[3] + TX_Buff[4] + TX_Buff[5]; //校验

            Send_Packet(WR_TX_PLOAD, TX_Buff, 7);   //发送数据包

        }
        else
        {                                   //工作模式
            UINT8 i;                        //AD取平均  已不用
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
						
            if (RP_ADValue < 58 && RP_ADValue > -58) //20170122增，减少遥控器的死区，这样就可以不改主控
              RP_ADValue = 10;                       //20170122增，减少遥控器的死区，这样就可以不改主控

            TX_Buff[0] = 0xa5;                          //帧头
            if (RPPowUpReset == FALSE &&  err_rp_flag == 0)
            {                                               //复位完成 并且 电位器未报警 输出电位器的值                                                
                TX_Buff[1] = (UINT8)(RP_ADValue >> 8);      //电位器高8位
                TX_Buff[2] = (UINT8)(RP_ADValue & 0x00ff);  //电位器低8位                
            }
            else
            {                       //复位未完成输出0
                TX_Buff[1] = 0;     //电位器高8位
                TX_Buff[2] = 0;  //电位器低8位                
            }

            TX_Buff[3] = ((SpeedMode & 0x01) << 0) | ((DirMode & 0x01) << 1 | (err_code == 4) << 2);    //b0 速度模式 b1 电机方向 b2 关机指令
            TX_Buff[4] = 0xa5 + TX_Buff[1] + TX_Buff[2] + TX_Buff[3];   //校验和
            Send_Packet(WR_TX_PLOAD, TX_Buff, 5);       //发送数据包   
					//	Send_Packet(WR_TX_PLOAD, TX_Buff, 5);       //发送数据包    20170121增加一次发送，导致遥控器1或2号LED闪烁
        }

       // Delay_Ms(10);  //等待10ms 20170121原10改12，增加发送的时间，导致遥控器1或2号LED闪烁 //20170218增，解决遥控器和主控失联的问题
    }
    else
    {
        RfTick++;
    }

   //if (RfTick == 1)      //20170215增
    SwitchToRxMode();                                   //切换到接受模式
}

//遥控器数据处理
UINT8 dBuff[10] = { 0 };
UINT8 RF_DataProcess(UINT8 len, UINT8 *pBuff)   
{
    UINT8 i;
    for (i = 0; i < 9; i++)         //拷贝数据
        dBuff[i] = pBuff[i];

    if (len == 5)                   //长度5 普通通信
    {
        UINT8 checksum = dBuff[0] + dBuff[1] + dBuff[2] + dBuff[3];     //校验和
        UINT8	temp = dBuff[1] & 0x07;                                 //主电池电量


        if (checksum == dBuff[4] && dBuff[0] == 0xaa)                   //帧头和校验和
        {
           // RF_Cont++;      //20170220删。 为了消除遥控 器失联
          //  if (RF_Cont >= 1)    //20170220删。 为了解决遥控器失联的问题           //遥控器断线计数
            {
                RF_WDog = 0;                                            //重置计时
                RF_State = RF_Connect;                                  //遥控器连接
            }
            switch (temp)
            {
            case 4:
                BatteryNum = 0;                                         //电量0    
                break;
            case 0:
                BatteryNum = 10;                                        //电量10
                break;
            case 1:
                BatteryNum = 40;                                        //电量40
                break;
            case 2:
                BatteryNum = 70;                                        //电量70
                break;
            case 3:
                BatteryNum = 100;                                       //电量100
                break;
            default:
                break;
            }

            //0x01控制出错R轴 0x02:温度65(主机没有)  0x04:温度75(主机没有) 0x08:参数错误R轴 
            //0x10控制出错L轴 0x20:温度65(从机才有)  0x40:温度75(从机才有) 0x80:参数错误L轴
            if (dBuff[2] != 0)
            {
                if (dBuff[2] == 0x40)                                   //过温关机
                {
                    err_code = 4;                                       
                }
                else if (err_code == 0)
                {
                    if (dBuff[2] == 0x01)                               //0x01控制出错R轴
                    {
                        err_code = 1;                                   //闪1次
                    }
                    if (dBuff[2] == 0x10)                               //0x10控制出错L轴        
                    {
                        err_code = 2;                                   //闪2次
                    }

                    if (dBuff[2] == 0x20)                               //0x20:温度65(从机才有)    
                    {
                        err_code = 3;                                   //闪3次
                    }

                    if (dBuff[2] == 0x08)                               //0x08:参数错误R轴 
                    {
                        err_code = 5;                                   //闪5次
                    }

                    if (dBuff[2] == 0x80)                               //0x80:参数错误L轴    
                    {
                        err_code = 6;                                   //闪6次
                    }
                }
            }
        }
    }
    else if (len == 7)                                                  //长度7 地址数据
    {
        UINT8 checksum = dBuff[0] + dBuff[1] + dBuff[2] + dBuff[3]      //校验和
            + dBuff[4] + dBuff[5];
        if (dBuff[0] == 0xcc
            && dBuff[1] == TX_Addr[0]
            && dBuff[2] == TX_Addr[1]
            && dBuff[3] == TX_Addr[2]
            && dBuff[4] == TX_Addr[3]
            && dBuff[5] == TX_Addr[4]
            && dBuff[6] == checksum)
        {

            SwitchRFtoWorkMode();                                       //切换到工作模式
            ADC_GetBase();                                             //获取ADC偏置值
            Bsp_Beep_On();                                              //嘀 嘀 嘀~
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

//遥控处理
void RF_Proce(void)                                     
{
    if (ChargeFlag == FALSE)                            //是否在充电 
    {                                                   //不充电时才接受和发送数据
        //Receive_Packet();                               //接受数据
        //RF_SendData();                                  //发送数据
        if(Receive_Packet() != 0) 
        {
                RF_SendData();
                SendTick = 0;
        }

        if (RF_WDog == 250)                             //10ms * 250 = 2.5s 内没有接受到数据
        {
            RF_State = RF_Disconnect;                   //无线模块状态为断线
            RF_Cont = 0;
            BatteryNum = 0;                             //电池电量为0
        }
    }
}

//获取遥控器的ID
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

//LED处理
void LED_Proce(void)                                                    
{
#if 0
    static UINT8 led_time = 0;
    led_time++;
    if (ChargeFlag == FALSE)                                //是否在充电
    {
        /**/                                                //不在充电 正常工作
        if (err_code != 0 || err_rp_flag == 1)              //有错误  
        {
            static UINT8 led_cnt = 0;                       //led计数
            if (err_code != 4)                              //不是过温度报警
            {                                                   
                if (led_cnt == 0xff)                        //长灭一次       
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
                    if ((err_rp_flag == 0 && led_cnt < err_code)    //短闪err_code的次数
                        || (err_rp_flag == 1 && led_cnt < 4))       //电位器报警闪烁4次    
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
                            led_cnt++;                              //闪烁次数增加    
                        }
                    }
                    else
                    {
                        led_cnt = 0xff;
                    }
                }
            }
            else
            {                                               //过温度报警 所有LED闪烁 蜂鸣器叫
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
            if (RF_Mode == RF_WorkMode)                     //工作模式
            {
                if (SpeedMode == LowSpeedMode)              //低速模式
                {
                    if (RF_State == RF_Disconnect)          //断开连接闪LED1
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
                        OpenLed1();                         //连接时常量LED1
                    CloseLed2();                            //关闭LED2
                }
                else if (SpeedMode == HighSpeedMode)        //高速模式
                {
                    if (RF_State == RF_Disconnect)          //断开连接闪LED2
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
                        OpenLed2();                         //连接时常量LED2
                    CloseLed1();                            //关闭LED1
                }
                else
                {

                }
                //led 34
                if (BatteryNum == 0)                        //根据主电池电量点亮LED LED5需要考虑遥控器电量
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
            {                                               //配对模式
                static UINT8 i;
                if (++i >= 10)                              
                    i = 0;

                if (i >= 5)                                 //快闪LED
                {
                    if (SpeedMode == LowSpeedMode)          //根据速度模式决定快闪哪个LED
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


        //led 5 的处理
        /**/
        if (err_code != 4)
        {   //没有过温报警
            if (ADCPowVal_LPFlag && (BatteryNum > 0 && BatteryNum <= 10))   //遥控器没电 且 主电池没电
            {
                //慢闪加快闪
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
                    //遥控器没电慢闪
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
                    //主电池没电遥控器快闪
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
            else if (ADCPowVal_LPFlag)                              //遥控器没电
            {
                //遥控器没电慢闪
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
            else                                                    //其他情况
            {
                if (RF_State == RF_Connect)
                {
                    if (BatteryNum == 0)
                    {
                        CloseLed5();
                       // Beep_Disable();   //20170220删，为了使主电池没有电量时蜂鸣器快速叫
											 if (led_time < 10)     //20170220删，为了使主电池没有电量时蜂鸣器快速叫 
                        {
                            Beep_Enable();    //20170220删，为了使主电池没有电量时蜂鸣器快速叫 
                        }
                        else if (led_time < 20)   //20170220删，为了使主电池没有电量时蜂鸣器快速叫
                        {
                            Beep_Disable();       //20170220删，为了使主电池没有电量时蜂鸣器快速叫
                        }
                        else
                        {
                            led_time = 0;         //20170220删，为了使主电池没有电量时蜂鸣器快速叫
                        }
                    }
                    else if (BatteryNum <= 10)
                    {
                        //主电池没电遥控器快闪
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
    {   //充电LED闪烁 
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


//进入掉电模式
void ToPowdown(void)        
{
    Bsp_IntDis();           //关闭中断
    //CloseLed1(); CloseLed2(); CloseLed3(); CloseLed4(); CloseLed5();    //关闭LED
    BK2425_SetToPD();       //设置遥控模块到掉电模式
    Delay_Ms(10);           //延时

    PoweFlag = FALSE;       //掉电

    Delay_Ms(10);           //延时

    Bsp_POW_Low();          //关闭电源
    while(1){}
}


void AutoPowerOff_Proce(void)
{
    if (RP_ADValue > 40 || RP_ADValue < -40 || ChargeFlag == TRUE)  //遥控器有动作或者在充电
    {
        AutoPowerDownTime = 0;
    }

    if (AutoPowerDownTime > 90000)//10ms * (100 * 60 * 15)
    {
        //CloseLed1(); CloseLed2(); CloseLed3(); CloseLed4(); CloseLed5();    //关闭LED
        Bsp_Beep_On();              //蜂鸣器滴一声
        Delay_Ms(200);
        Bsp_Beep_Off();
        ToPowdown();                //烧完程序直接进掉电模式
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

//系统初始化
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
    BK2425_Initialize((void*)RF_DataProcess, TX_Addr, RX1_Addr, TX_Addr);     //自己的地址发送接收 
    

    /* De-Init ADC peripheral*/
    ADC1_DeInit();

    /* Init ADC2 peripheral */
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_9, ADC1_PRESSEL_FCPU_D2, \
              ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL9,\
              DISABLE);

    TIM4_Config();          
              

    Value_Init();  
    
    Bsp_IntEn();      //开启中断

}


void main(void)
{
    System_Init();                  //系统初始化
    ADC_GetBase();                 //获取ADC偏置值

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
            if (WakeupFlag == TRUE)     //唤醒
            {
                if (keycnt == 0x00)     //按键释放
                {
                    WakeupFlag = FALSE; //关闭唤醒
                    PoweFlag = FALSE;   //掉电
                    keytime = 0;        //按键计时清0
                    keytime1 = 0;       //按键计时1清0    
                }

                if (keycnt == 0x80)  //按键按下
                {
                    if (keytime < 255)
                    keytime++;      //按键计时自增 0-255 2550ms
                }

                InPut_Proce();

                if (keytime == 200)     //200*10ms = 2s     长按2s后 启动
                {
                    AutoPowerDownTime = 0;
                    PoweFlag = TRUE;    //上电
                    err_code = 0;       //清除故障
                    err_rp_flag = 0;    //清除电位器故障
                    WakeupFlag = FALSE;         //关闭唤醒
                    RF_State = RF_Disconnect;   //默认遥控器断线    
                    RF_Cont = 0;                    
                    RF_WDog = 0;
                    BatteryNum = 0;
                    keytime1 = 1;                   //按键计时1开启计时
                    FirstRunFlag = TRUE;            //第一次运行
                    FirstRunCnt = 0;                //第一次运行计数清零 
                    RPPowUpReset = TRUE;            //电位器 上电是否在中间
                    SwitchRFtoWorkMode();           //无线遥控转入工作模式
                    
                    //OpenLed1(); OpenLed2(); OpenLed3(); OpenLed4(); OpenLed5();         //灯闪一下
                    
                    Bsp_Beep_On();                  //遥控器滴一声
                    Delay_Ms(200);
                    Bsp_Beep_Off();
                    Delay_Ms(800);
                    //CloseLed1(); CloseLed2(); CloseLed3(); CloseLed4(); CloseLed5();
                }
            }
            else
            {
                if (PoweFlag == FALSE && ChargeFlag == FALSE)                           //关闭遥控器 且 不在充电
                {
                    //CloseLed1(); CloseLed2(); CloseLed3(); CloseLed4(); CloseLed5();
                  
                    Bsp_Beep_Off();
                    ToPowdown();                //烧完程序直接进掉电模式
                }
                else
                {
                    if (keycnt == 0x80)  //按键按下
                    {
                        if (keytime < 255)
                            keytime++;        //按键计时自增 0-255 2550ms

                        if (keytime1 != 0 && keytime1 < 500)
                        {
                            keytime1++;      //按键计时1自增 
                        }

                        if (keytime1 == 400)   //400 *10ms = 4s 连续长按后进入配对模式
                        {
                            SwitchRFtoAddrSendMode();           //切换到地址发送模式
                            RF_State = RF_Disconnect;           //切换到掉线状态
                            RF_Cont = 0;
                            BatteryNum = 0;                     //电池点亮清除
                        }
                    }

                    if (keycnt == 0x00)    //按键释放
                    {
                        keytime1 = 0;
                    }

                    AutoPowerOff_Proce();                       //自动关机处理
                    //RF_Proce();                                 //遥控器处理
                    InPut_Proce();                              //输入处理
                    Charge_Proce();                             //充电处理
                    Key_Proce();                                //按键处理
                    // RF_Proce_1();                               //遥控器接收处理    
                    ADC_Proce();                                //ADC处理
                    FirstRunFlag_Proce();                       //第一次运行处理
                    LED_Proce();                                //LED灯处理

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
