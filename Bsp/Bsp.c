//==================================================================================================
//| �ļ����� | Bsp.c
//|--------- |--------------------------------------------------------------------------------------
//| �ļ����� | Bsp.c �弶�������ܵ�ʵ��
//|--------- |--------------------------------------------------------------------------------------
//| ��Ȩ���� | 
//|----------|--------------------------------------------------------------------------------------
//|  �汾    |  ʱ��       |  ����     | ����
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | ����
//==================================================================================================
#include  "Bsp.h"
  
void  Bsp_Init (void)
{
    /*High speed internal clock prescaler: 1*/
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    UART1_DeInit();
    /* UART1 configuration ------------------------------------------------------*/
    /* UART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Receive and transmit enabled
        - UART1 Clock disabled
    */
    UART1_Init((uint32_t)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
              UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

    Bsp_GpioInit();
}

void Bsp_DelayMs(INT16U ui_Delay)
{
    INT16U i = 0; 
    INT8U k = 0; 
    INT16U j = 0;
    for(j = 0; j < ui_Delay; j++)
    {
        for(i = 0; i < 2300; i++)
        {
            for( k = 0 ; k < 200; k--)
            {
              
            }
        }
    }
}

void Bsp_Printf(char * string)
{
    int len = strlen(string);
    int i;
    for(i = 0; i < len; i++)
    {
        UART1_SendData8(string[i]);
        /* Loop until the end of transmission */
        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
    }
}

