//#include <LPC21xx.H>  /* LPC21xx definitions */		

//#include <string.h>		

#include<lpc21xx.h>		
#include "uart_int_test.h"

#define UART_INT_ENABLE 1	
int i=0;
//char buff[10];
char ch;
char r_flag=0;
char dummy;
		
void InitUART0(void); /* Initialize Serial Interface */		
void UART0_Tx(char ch);		

char UART0_Rx(void);		
		
void UART0_isr(void) __irq		
{		
    if((U0IIR & 0x04)) // Check if receive interrupt		
    {		
        ch = U0RBR; /* Read to Clear Receive Interrupt */		
		if(ch==0x02)
		{
		   r_flag=1;
		}

        else if((ch != 0x03) && (r_flag == 1))// && i < sizeof(buff) - 1) /* Prevent buffer overflow */		
        {		
            buff[i++] = ch;		
        }		
        else		
        {		

            buff[i] = '\0'; /* Null-terminate the string */		
            i = 0;		
            r_flag = 2;		
        }		
    }		
    else		
    {		
        dummy = U0IIR; // Clear transmit interrupt		
    }		
    VICVectAddr = 0; /* Dummy write to signal end of ISR */		
}		

void InitUART0 (void) /* Initialize Serial Interface */		
{		
    PINSEL0 |= (5<<0); /* Enable RxD0 and TxD0 */		
    U0LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit */		
    U0DLL = 97;          /* Set Baud Rate for 9600 (check clock settings) */		
    U0LCR = 0x03;        /* DLAB = 0 */		
    #if UART_INT_ENABLE > 0

    VICIntSelect = 0x00000000; /* Set to IRQ */		

    VICVectAddr0 = (unsigned)UART0_isr;		

    VICVectCntl0 = 0x20 | 6;   /* UART0 Interrupt */		

    VICIntEnable = 1 << 6;     /* Enable UART0 Interrupt */		

    U0IER = 0x03;              /* Enable UART0 RX and THRE Interrupts */		

    #endif		

}		

void UART0_Tx(char ch) /* Write character to Serial Port */		
{		

    while (!(U0LSR & 0x20));		

    U0THR = ch;		

}		

char UART0_Rx(void) /* Read character from Serial Port */		

{		

    while (!(U0LSR & 0x01));		

    return (U0RBR);		

}		

void UART0_Str(char *s)		
{		

    while(*s)		

        UART0_Tx(*s++);		

}		

void UART0_Int(unsigned int n)		
{		

    unsigned char a[10] = {0};		

    int i = 0;		

    if(n == 0)		

    {		

        UART0_Tx('0');		

        return;		

    }		

    else		

    {		

        while(n > 0)		

        {		

            a[i++] = (n % 10) + '0';		

            n = n / 10;		

        }		

        --i;		

        for(; i >= 0; i--)		

        {		

            UART0_Tx(a[i]);		

        }		

    }		

}		

		

void UART0_Float(float f)		

{		

    int x;		

    float temp;		

    x = (int)f; /* Extract integer part */		

    UART0_Int(x);		

    UART0_Tx('.');		

    temp = (f - x) * 100; /* Get two decimal places */		

    x = (int)temp;		

    UART0_Int(x);		

}		

		

void DelayS(unsigned int dly)		

{		

    unsigned int i;		

    for(;dly > 0; dly--)		

        for(i = 1200000; i > 0; i--); /* Rough delay, consider timers */		

}		

		

// Function to wait and read RFID card data		

void RFID_ReadCard() 
{		

    while (r_flag != 2); // Wait until data is ready		

    r_flag = 0;          // Reset flag for next read		

}		

/*int main()		

{		

    InitUART0();		

     while(1)		

    {		

        i = 0; r_flag = 0;		

        while(r_flag != 2); // Wait for reception 		

        UART0_Str(buff); // Echo back received string 		

        UART0_Str("\n\r");		

        //UART0_Float(245.21);// Send float over UART 		

        UART0_Str("\n\r");		

        delay_ms(1000); // Delay for 1 second 		

    }		

}		

  */		
