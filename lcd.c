//lcd.c			

#include<LPC21xx.h>			

#include "defines.h"			

#include "delay.h"			

#include "types.h"			

#include "lcd_defines.h"			

void WriteLCD(u8 Dat)			

{			

	WRITEBYTE(IOPIN0,LCD_DATA,Dat);		

	SSETBIT(IOSET0,LCD_EN);		

	delay_us(1);		

	SCLRBIT(IOCLR0,LCD_EN);		

	delay_ms(2);		

}			

			

void CmdLCD(u8 cmd)			

{			

	SCLRBIT(IOCLR0,LCD_RS);		

	WriteLCD(cmd);		

}			

			

void CharLCD(u8 asciiVal)			

{			

  SSETBIT(IOSET0,LCD_RS);			

  WriteLCD(asciiVal);			

}			

			

void InitLCD(void)			

{			

	//cfg p0.8 to p0.15,p0.16,p0.17		

	IODIR0|=0xFF<<LCD_DATA|1<<LCD_RS|1<<LCD_EN;		

	delay_ms(15);		

	CmdLCD(0x30);		

	delay_ms(5);		

	CmdLCD(0x30);		

	delay_us(100);		

	CmdLCD(0x30);		

	CmdLCD(MODE_8BIT_2LINE);		

	CmdLCD(DSP_ON_CUR_OFF);		

	CmdLCD(CLEAR_LCD);		

	CmdLCD(SHIFT_CUR_RIGHT);		

}			

			

void StrLCD(s8 *p)			

{			

	while(*p)		

		CharLCD(*p++);	

}			

			

			

void U32LCD(u32 n)			

{			

	s32 i=0;		

	u8 a[10];		

	if(n==0)		

	{		

		CharLCD('0');	

	}		

  else			

  {			

		while(n!=0)	

		{	

			a[i++]=(n%10)+48;

			n/=10;

		}	

		for(--i;i>=0;i--)	

		{	

			CharLCD(a[i]);

		}	

  }			

}			

			

void S32LCD(s32 n)			

{			

	if(n<0)		

	{		

		CharLCD('-');	

		n=-n;	

	}		

	U32LCD(n);		

}			

			

void F32LCD(f32 f,u32 nDP)			

{			

	u32 n,i;		

	if(f<0.0)		

	{		

		CharLCD('-');	

		f=-f;	

	}		

	n=f;		

	U32LCD(n);		

	CharLCD('.');		

	for(i=0;i<nDP;i++)		

	{		

		f=(f-n)*10;	

		n=f;	

		CharLCD(n+48);	

	}		

}			

			

void HexLCD(u32 n)			

{			

	s32 i=0;		

	u8 a[8],t;		

	if(n==0)		

	{		

		CharLCD('0');	

	}		

  else			

  {			

		while(n!=0)	

		{	

			t=(n%16);

			t=((t>9)?((t-10)+'A'):(t+48));

			a[i++]=t;

			n/=16;

		}	

		for(--i;i>=0;i--)	

		{	

			CharLCD(a[i]);

		}	

  }			

}			

			

void BinLCD(u32 n,u32 nBD)			

{			

	s32 i;		

	for(i=(nBD-1);i>=0;i--)		

	   CharLCD(READBIT(n,i)+48);		

}			

			

void BuildCGRAM(u8 *p,u32 nBytes)			

{			

	s32 i;		

	//point cursor/address pointer to		

	//cgram start		

	CmdLCD(GOTO_CGRAM_START);		

	for(i=0;i<nBytes;i++)		

	{		

		//write to cgram via data reg	

		CharLCD(p[i]);	

	}		

	//reposition cursor/address pointer to		

	//ddram start		

	CmdLCD(GOTO_LINE1_POS0);		

}			
