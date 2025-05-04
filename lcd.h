//lcd.h

#include "types.h"

void WriteLCD(u8);

void CmdLCD(u8);

void CharLCD(u8);

void InitLCD(void);

void StrLCD(s8 *);

void U32LCD(u32);

void S32LCD(s32);

void F32LCD(f32,u32);

void HexLCD(u32);

void BinLCD(u32,u32);

void BuildCGRAM(u8 *p,u32 );
