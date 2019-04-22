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
    if (STDBYVal())                 //充电充满端口检测
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


    if (CHRGEVal())                 //正在充电端口检测
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

    Delay200us();                   //延时200us 等待按键端口电平稳定

    if (KEYVal())                   //按键端口检测
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
        Beep_Enable();                          //蜂鸣器滴一声    
        Delay_Ms(200);
        Beep_Disable();
    }

    //if( keytime == 252 )      //250*20ms = 5s  
    //{
      //对码用
    //}
}
//Time2中断服务处理
void TIM2_UPDATE_IRQ(void)                      
{
    static UINT16 ms_cnt = 0;
		
		if (TIM2_SR1 & (1 << UIF))
    {
        TIM2_SR1 &= ~(1 << UIF);
				ms_cnt++;
				
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
}
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
#if  0
            SwitchRFtoWorkMode();                                       //切换到工作模式
            ADC1_GetBase();                                             //获取ADC偏置值
            Beep_Enable();                                              //嘀 嘀 嘀~
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



//系统初始化
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
    BK2425_Initialize((void*)RF_DataProcess, TX_Addr, RX1_Addr, TX_Addr);     //自己的地址发送接收 


    ADC1_Initialize();      //ADC初始化
    ADC1_Base_Copy();
    ADC1_StartConversion(); //ADC开始转换


    tim2ini();              //定时器2初始化
    tim2start();            //开启定时器2

    Value_Init();           

    enableinterrupt();      //开启中断
#endif
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

void main(void)
{
    System_Init();                  //系统初始化
    while(1)
    {
    
    }
#if 0
    ADC1_GetBase();                 //获取ADC偏置值
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
                    RPPowUpReset = TRUE;           //电位器 上电是否在中间
                    SwitchRFtoWorkMode();           //无线遥控转入工作模式
                    
                    //OpenLed1(); OpenLed2(); OpenLed3(); OpenLed4(); OpenLed5();         //灯闪一下
                    
                    Bsp_Beep_On();                  //遥控器滴一声
                    Delay_Ms(200);
                    Bsp_Beep_Off();
                    Delay_Ms(800);
                    CloseLed1(); CloseLed2(); CloseLed3(); CloseLed4(); CloseLed5();
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
