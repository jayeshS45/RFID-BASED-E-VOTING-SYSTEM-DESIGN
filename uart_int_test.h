void UART0_isr(void) __irq;

void InitUART0 (void);

void UART0_Tx(char);

char UART0_Rx(void);

void UART0_Str(char*);

void UART0_Int(unsigned int);

void UART0_Float(float);

void DelayS(unsigned int);

void RFID_ReadCard(void);

extern char buff[10]; /* Increased buffer size to prevent overflow */		

extern char r_flag,dummy,ch;

