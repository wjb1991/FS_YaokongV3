#ifndef __MOD_OLED_H__
#define __MOD_OLED_H__

extern const unsigned char bkgpic[];
extern const unsigned char BoardChrgeIcon[8];
extern const unsigned char ChrgeIcon[8];
extern const unsigned char RfStateIcon[8];
extern const unsigned char HighSpeedIcom[8];
extern const unsigned char LowSpeedIcom[8];
extern const unsigned char PatrolIcom[8];

extern void Mod_OLEDInit(void);
extern BOOL Mod_OLEDDisplay(INT8U* p_Data);
extern BOOL Mod_OLEDClear(void);
extern BOOL Mod_OLEDDispIcon_8x8(INT8U line,INT8U page,INT8U* p_Data);
extern BOOL Mod_OLEDDispNum_32x16(INT8U line,INT8U num);

#endif