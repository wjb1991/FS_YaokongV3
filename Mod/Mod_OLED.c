//*******************************************************
//CONTROL IC:SSD1306
//Model style:128*64
//1/64 duty    Vop:14.0V  VDD:3.3V  1/11bias
//FINISH DATA:
//SERIEA:6800/8080/SPI
//*******************************************************

//#include <stdio.h>        
//#include <ctype.h>
//#include <string.h>

#include "Bsp.h"

#define Uchar unsigned char
#define Uint  unsigned int

#define MHz 12                      //晶振频率

#define  Write_Command     Write_Command_SPI
#define  Write_Data        Write_Data_SPI

unsigned char const pic[];
void Delay_1ms(unsigned char delay); // 基准为1ms的延时函数,最大255ms
void Init_Lcd(void);
void Display(Uchar Data1,Uchar Data2);
void Display1(Uchar Data1,Uchar Data2);
void DisplayPicture(Uchar *PIC);
void Delay(Uint t);
void Write_Command_SPI(Uchar Command);
void Write_Data_SPI(Uchar Data);
#if 0
//*******************************************************
//函数名称：main
//功能：主函数
//参数：无
//*******************************************************
void main( )
  {
   Init_Lcd();
   while(1)
   {
    Display(0xFF,0xFF);            
    Delay(50);
    //WaitKey();
    
    Display(0x00,0x00);    
    Delay(50); 
    
    Display1(0x33,0xCC);
    Delay(50);      

    Display(0xAA,0xAA);  
    Delay(50); 
        
    Display(0xFF,0x00);
    Delay(50);    

    Display(0x55,0xAA);
    Delay(50); 
    
  
     
    DisplayPicture(pic);
    Delay(50);  
    //WaitKey();
    

   }
  }
#endif
//*******************************************************
//函数名称：Init_LCD
//功能：设置LCD所用芯片中各寄存器的值
//参数：无
//*******************************************************
void Init_Lcd(void)
{   
    
	Bsp_RES_High();
	Delay_1ms(100);
	Bsp_RES_Low();
	Delay_1ms(100);
	Bsp_RES_High();
	Delay_1ms(100);

	Write_Command(0xAE);//set display display ON/OFF,AFH/AEH

	Write_Command(0x40);//set display start line:COM0

	Write_Command(0x20);//set memory addressing mode
	Write_Command(0x02);//page addressing mode

	Write_Command(0x81);//set contrast control
	Write_Command(0x7F);

	Write_Command(0xAD);//External or internal Vcomh or Iref select
    Write_Command(0x00);//Vcomh:internal  Iref:External

    Write_Command(0xA0);//set segment re-map		

	Write_Command(0xA4);//entire display on: A4H:OFF/A5H:ON

	Write_Command(0xA6);//set normal/inverse display: A6H:normal/A7H:inverse

	Write_Command(0xA8);//set multiplex ratio
	Write_Command(0x1F);//32

	Write_Command(0xC0);//set com output scan direction

	Write_Command(0xD3);//set display  offset
	Write_Command(0x00);//

	Write_Command(0xDA);//set seg pins hardware configuration
	Write_Command(0x12);//

	Write_Command(0xD5);//set display  clock divide ratio/oscillator frequency
	Write_Command(0x80);//FR=105HZ

	Write_Command(0xD9);//set pre-charge period
	Write_Command(0x22);//

	Write_Command(0xDB);//set vcomh deselect level
	Write_Command(0x30);//0.83*VCC	
	
	Write_Command(0x8D);//charge pump setting
	Write_Command(0x15);//enable charge pump, select 7.5V charge pump output
	Delay_1ms(100);		

	Write_Command(0xAF);//set display display ON/OFF,AFH/AEH	
	
      
}


//*******************************************************
//函数名称：Delay_1ms
//功能：延时函数,基准值为1ms,最大255毫秒 
//参数：delay
//*******************************************************
void Delay_1ms(unsigned char delay)
{ 
  unsigned char i,j,tmp;
  tmp = (char)((100*MHz)/12+53);  //根据晶振频率计算内层循环初值,调整23可以对延时时间进行微调
  for(i=0;i<delay;i++)
   {
    for(j=0;j<tmp;j++);
   }
}

//*******************************************************
//函数名称：Delay
//功能：延时函数
//参数：t
//*******************************************************
void Delay(Uint t)
{
  unsigned char i;
  unsigned int j;
  for(i=0;i<=t;i++)
  for(j=0;j<2000;j++);
}




//*******************************************************
//函数名称：Write_Command_SPI
//功能：串口写指令
//参数：Command
//*******************************************************
void Write_Command_SPI(Uchar Command)
{
    int i;
    Bsp_CS_Low();
    Bsp_DC_Low();
    for(i=0;i<8;i++)
    {
        Bsp_SCLK_Low();
        if((Command&0x80)==0)
            Bsp_SDIN_Low();
        else
            Bsp_SDIN_High();
        Bsp_SCLK_High();
        Command=Command<<1;
    }
 }

//*******************************************************
//函数名称：Write_Data_SPI
//功能：串口写数据
//参数：Data
//*******************************************************
void Write_Data_SPI(Uchar Data)
{
    int i;
    Bsp_CS_Low();
    Bsp_DC_High();
    for(i=0;i<8;i++)
    {
        Bsp_SCLK_Low();
        if((Data&0x80)==0)
            Bsp_SDIN_Low();
        else
            Bsp_SDIN_High();
        Bsp_SCLK_High();
        Data=Data<<1;
    }
}
 
//*******************************************************
//函数名称：Display
//功能：显示图案
//参数：Data1,Data2
//*******************************************************
void Display(Uchar Data1,Uchar Data2)
{
	int i,j;     
    for(i=0;i<4;i++)
	 {	
	  Write_Command(0xB0+i);//set pageaddress start adress
	  Write_Command(0x00);  //set lower column start address
	  Write_Command(0x10);  //set higher column start address	  
	  for(j=0;j<64;j++)
	    { 	      
	      Write_Data(Data1);
	      Write_Data(Data2);
	    }	  
     }
     
    
}

//*******************************************************
//函数名称：Display
//功能：显示图案
//参数：Data1,Data2
//*******************************************************
void Display1(Uchar Data1,Uchar Data2)
{
	int i,j;     
    for(i=0;i<4;i++)
	 {	
	  Write_Command(0xB0+i);//set pageaddress start adress
	  Write_Command(0x00);  //set lower column start address
	  Write_Command(0x10);  //set higher column start address	  
	  for(j=0;j<32;j++)
	    { 	      
	      Write_Data(Data1);
	      Write_Data(Data1);    

	      Write_Data(Data2);
	      Write_Data(Data2);
	    
	    }	  
     }
     
    
}




