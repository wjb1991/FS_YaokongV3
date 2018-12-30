//==================================================================================================
//| �ļ����� | Bsp.h
//|--------- |--------------------------------------------------------------------------------------
//| �ļ����� | Bsp.c����Ӧ��ͷ�ļ�
//|--------- |--------------------------------------------------------------------------------------
//| ��Ȩ���� | 
//|----------|--------------------------------------------------------------------------------------
//|  �汾    |  ʱ��       |  ����     | ����
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | ����V1.0
//|  V1.01   | 2018.10.31  |  wjb      | �ڳ��������ע�� ��ӿ����ж� �� ��ӡ���� V1.01    
//==================================================================================================
#ifndef  __BSP_H__
#define  __BSP_H__


#ifdef __cplusplus
extern  "C" {                                  /* See Note #1.                                         */
#endif
  
#include  "stm8s.h"
  
#include  <stdarg.h>
//#include  <stdint.h>
#include  <stdio.h>
#include  <string.h>
#include  <math.h>

  
  
#include  "Bsp_Datatype.h"
#include  "Bsp_Converter.h"
#include  "Bsp_Gpio.h"

#include  "Mod_OLED.h"

/*__CMSIS_IAR_H__ ���г��õĺ궨�� */
  
void Bsp_Init(void);

inline void Bsp_Nop(void){nop();}
inline void Bsp_IntDis(void){ disableInterrupts(); }
inline void Bsp_IntEn(void){ enableInterrupts(); }

void Bsp_Printf(char * string);
void Bsp_DelayMs(INT16U ui_Dealy);
//#define	Bsp_Printf		Bsp_UartPrintf

//#define	TRACE_DBG 	    //Bsp_UartPrintf

#ifdef __cplusplus
}                                              /* End of 'extern'al C lang linkage.                    */
#endif

#endif                                                          /* End of module include.                               */

