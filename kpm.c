//kpm2.c			

#include <LPC21xx.h>			

#include "types.h"			

#include "defines.h"			

#include "kpm_defines.h"			

#include "lcd.h"	

#include "delay.h"		

			

u8 kpmLUT[4][4]=			

{			

	{'1','2','3','A'},		

	{'4','5','6','B'},		

	{'7','8','9','C'},		

	{'*','0','#','='}					

};			

			

u32 ColScan(void)			

{			

	u32 t;		

	t=(READNIBBLE(IOPIN1,COL0) < 15) ? 0 : 1;		

	return t;		

}			

			

u32 RowCheck(void)			

{			

	u32 rNo;		

	for(rNo=0;rNo<4;rNo++)		

	{		

		//grounding 0th to 3rd row iterative	

    WRITENIBBLE(IOPIN1,ROW0,~(1<<rNo));			

    if(ColScan()==0)			

       break;			

	}		

	WRITENIBBLE(IOPIN1,ROW0,0);		

	return rNo;		

}			

			

u32 ColCheck(void)			

{			

	u32 cNo;		

	for(cNo=0;cNo<=3;cNo++)		

	{		

		if(READBIT(IOPIN1,COL0+cNo)==0) 	

       break;			

	}		

	return cNo;		

}			

			

void Init_KPM(void)			

{			

	WRITENIBBLE(IODIR1,ROW0,15);		

	//ground all rows		

			

	//let all columns be high		

	//default column lines are input high		

}			

			

u8 KeyScan(void)			

{			

	u32 rNo,cNo;		

  //wait to detect any key press			

	while(ColScan());		

	//if any key pressed was detected & 		

	//while key is in pressed state		

	//proceed to identify row in which key was 		

	//pressed		
	rNo=RowCheck();		

	//proceed to identify col in which key was		

	//presed		

	cNo=ColCheck();		

	//extract key value from LUT for key pressed		

	return kpmLUT[rNo][cNo];		

}			

			

u32 ReadNum(void)			

{			

	u8 keyV;		

	u32 sum=0;		

	while(1)		

	{		

		keyV=KeyScan();	

		CharLCD(keyV);	

		if(keyV>='0' && keyV<='9')	

		{	

			sum=(sum*10)+(keyV-48);

			while(ColScan()==0);
			delay_ms(500);

		}	

		else	

		{	

			break;

		}	

	}		

	return sum;		

}			

