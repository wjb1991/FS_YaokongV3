//==================================================================================================
//| 文件名称 | Bsp_Gpio.c
//|----------|--------------------------------------------------------------------------------------
//| 文件描述 | 普通输入输出IO的初始化 外设的IO初始化在对应的外设的文件中 STM32版本
//|----------|--------------------------------------------------------------------------------------
//| 版权声明 |
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|----------|-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.12.02  |  wjb      | 初版
//==================================================================================================
#include "Bsp.h"

BOOL Bsp_GpioInit(void)
{
    INT32U  i;
    
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOB);
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOD);
    GPIO_DeInit(GPIOE);
    GPIO_DeInit(GPIOF);
    
    Bsp_EXPOW_Low();
    
    
    //GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_FAST);
    //GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST); 
    GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);   //beep
    GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);   //motor
    GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_IN_PU_IT);           //irq
    
    GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);        //MISO
    GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_FAST);   //MOSI
    GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);   //SCK
    GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);   //CE
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);   //POW
    GPIO_Init(GPIOC, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);        //POWKEY
    
    GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);   //CSN
    
    GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_SLOW);   //D/C
    GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW);   //RES
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_SLOW);   //SCLK
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_SLOW);   //SDIN
    GPIO_Init(GPIOB, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_SLOW);   //CS
    GPIO_Init(GPIOB, GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_SLOW);   //EXPOW
    
    GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);        //CHRG
    GPIO_Init(GPIOF, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);        //STDBY
    
    /*  Init GPIO for ADC2 */
    GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);
    
    return TRUE;
}




