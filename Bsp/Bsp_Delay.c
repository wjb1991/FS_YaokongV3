#include "App_Include.h"

//ʾ����ʵ��200us
void Delay200us(void)
{
    u16 i;
    for (i = 262; i > 0; i--);	//ԭ���Ź��� ʵ��200us����
}

void Delay_Ms(u16 time)
{
    u16 i, j;
    for (j = 0; j < time; j++)
    {
        for (i = 262; i > 0; i--);	//ԭ���Ź��� ʵ��200us����
        for (i = 262; i > 0; i--);	//ԭ���Ź��� ʵ��200us����
        for (i = 262; i > 0; i--);	//ԭ���Ź��� ʵ��200us����
        for (i = 262; i > 0; i--);	//ԭ���Ź��� ʵ��200us����
			//	for (i = 262; i > 0; i--);	//ԭ���Ź��� ʵ��200us����     //20170121�����ﵽ10MS,ԭ8MS
    }
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