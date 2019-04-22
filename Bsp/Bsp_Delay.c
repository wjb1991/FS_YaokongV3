#include "App_Include.h"

//示波器实测200us
void Delay200us(void)
{
    u16 i;
    for (i = 262; i > 0; i--);	//原先张工的 实测200us左右
}

void Delay_Ms(u16 time)
{
    u16 i, j;
    for (j = 0; j < time; j++)
    {
        for (i = 262; i > 0; i--);	//原先张工的 实测200us左右
        for (i = 262; i > 0; i--);	//原先张工的 实测200us左右
        for (i = 262; i > 0; i--);	//原先张工的 实测200us左右
        for (i = 262; i > 0; i--);	//原先张工的 实测200us左右
			//	for (i = 262; i > 0; i--);	//原先张工的 实测200us左右     //20170121增，达到10MS,原8MS
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