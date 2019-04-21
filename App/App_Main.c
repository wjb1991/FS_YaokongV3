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
#include "Bsp.h"



/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void main(void)
{
    Bsp_Init();
    Bsp_DelayMs(1000);
    Bsp_Motor_On();
    
    Mod_OLEDInit();
    Mod_OLEDDisplay((INT8U*)bkgpic);
    
    while (1)
    {
        Bsp_Beep_On();
        Bsp_DelayMs(10);
        Bsp_Beep_Off();
        Bsp_DelayMs(10);
        
        Mod_OLEDDispIcon_8x8(96,0,(INT8U*)ChrgeIcon);
        Mod_OLEDDispIcon_8x8(84,0,(INT8U*)RfStateIcon);
        
        Mod_OLEDDispIcon_8x8(24,0,(INT8U*)BoardChrgeIcon);
        Mod_OLEDDispIcon_8x8(24,1,(INT8U*)HighSpeedIcom);
        Mod_OLEDDispIcon_8x8(24,2,(INT8U*)LowSpeedIcom);
        Mod_OLEDDispIcon_8x8(24,3,(INT8U*)PatrolIcom);
        
        Mod_OLEDDispNum_32x16(40,5);
        Mod_OLEDDispNum_32x16(60,1);
    }   
}



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
