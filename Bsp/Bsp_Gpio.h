//==================================================================================================
//| �ļ����� | Bsp_Gpio.h
//|----------|--------------------------------------------------------------------------------------
//| �ļ����� | ��ͨ�������IO�ĳ�ʼ�� �����IO��ʼ���ڶ�Ӧ��������ļ��� STM32�汾
//|----------|--------------------------------------------------------------------------------------
//| ��Ȩ���� |
//|----------|--------------------------------------------------------------------------------------
//|  �汾    |  ʱ��       |  ����     | ����
//|----------|-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.12.02  |  wjb      | ����
//==================================================================================================

#ifndef     __BSP_GPIO_H__
#define     __BSP_GPIO_H__

#define     Bsp_Motor_On()          GPIO_WriteHigh(GPIOD,GPIO_PIN_3)   
#define     Bsp_Motor_Off()         GPIO_WriteLow(GPIOD,GPIO_PIN_3)

#define     Bsp_Beep_On()           GPIO_WriteHigh(GPIOD,GPIO_PIN_4)   
#define     Bsp_Beep_Off()          GPIO_WriteLow(GPIOD,GPIO_PIN_4)

#define     Bsp_MISO_Read()         GPIO_ReadInputPin(GPIOC,GPIO_PIN_7)

#define     Bsp_MOSI_High()         GPIO_WriteHigh(GPIOC,GPIO_PIN_6)   
#define     Bsp_MOSI_Low()          GPIO_WriteLow(GPIOC,GPIO_PIN_6)

#define     Bsp_SCK_High()          GPIO_WriteHigh(GPIOC,GPIO_PIN_5)   
#define     Bsp_SCK_Low()           GPIO_WriteLow(GPIOC,GPIO_PIN_5)

#define     Bsp_CE_High()           GPIO_WriteHigh(GPIOC,GPIO_PIN_4)   
#define     Bsp_CE_Low()            GPIO_WriteLow(GPIOC,GPIO_PIN_4)

#define     Bsp_CE_High()           GPIO_WriteHigh(GPIOC,GPIO_PIN_4)   
#define     Bsp_CE_Low()            GPIO_WriteLow(GPIOC,GPIO_PIN_4)

#define     Bsp_POW_High()          GPIO_WriteHigh(GPIOC,GPIO_PIN_3)   
#define     Bsp_POW_Low()           GPIO_WriteLow(GPIOC,GPIO_PIN_3)

#define     Bsp_POWKEY_Read()       GPIO_ReadInputPin(GPIOC,GPIO_PIN_2)  

#define     Bsp_SCK_High()          GPIO_WriteHigh(GPIOC,GPIO_PIN_5)   
#define     Bsp_SCK_Low()           GPIO_WriteLow(GPIOC,GPIO_PIN_5)

#define     Bsp_CSN_High()          GPIO_WriteHigh(GPIOE,GPIO_PIN_5)   
#define     Bsp_CSN_Low()           GPIO_WriteLow(GPIOE,GPIO_PIN_5)

#define     Bsp_DC_High()           GPIO_WriteHigh(GPIOB,GPIO_PIN_2)   
#define     Bsp_DC_Low()            GPIO_WriteLow(GPIOB,GPIO_PIN_2)

#define     Bsp_RES_High()          GPIO_WriteHigh(GPIOB,GPIO_PIN_3)   
#define     Bsp_RES_Low()           GPIO_WriteLow(GPIOB,GPIO_PIN_3)

#define     Bsp_SCLK_High()         GPIO_WriteHigh(GPIOB,GPIO_PIN_4)   
#define     Bsp_SCLK_Low()          GPIO_WriteLow(GPIOB,GPIO_PIN_4)

#define     Bsp_SDIN_High()         GPIO_WriteHigh(GPIOB,GPIO_PIN_5)   
#define     Bsp_SDIN_Low()          GPIO_WriteLow(GPIOB,GPIO_PIN_5)

#define     Bsp_CS_High()           GPIO_WriteHigh(GPIOB,GPIO_PIN_6)   
#define     Bsp_CS_Low()            GPIO_WriteLow(GPIOB,GPIO_PIN_6)

#define     Bsp_EXPOW_High()        GPIO_WriteHigh(GPIOB,GPIO_PIN_7)   
#define     Bsp_EXPOW_Low()         GPIO_WriteLow(GPIOB,GPIO_PIN_7)

#define     Bsp_CHRG_Read()         GPIO_ReadInputPin(GPIOA,GPIO_PIN_3)  
#define     Bsp_STDBY_Read()        GPIO_ReadInputPin(GPIOF,GPIO_PIN_4)


BOOL Bsp_GpioInit(void);

#endif
