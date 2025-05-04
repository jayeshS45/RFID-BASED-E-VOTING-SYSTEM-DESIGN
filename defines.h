#include "types.h"							


#define WRITENIBBLE(WORD,STARTBITPOS,NIBBLE) WORD=((WORD&~(15<<STARTBITPOS))|(NIBBLE<<STARTBITPOS))														

						 							

#define READNIBBLE(WORD,STARTBITPOS) ((WORD>>STARTBITPOS)&15)								

								

#define WRITEBYTE(WORD,STARTBITPOS,BYTE) WORD=((WORD&~(255<<STARTBITPOS))|(BYTE<<STARTBITPOS))																

								

#define READBYTE(WORD,STARTBITPOS) ((WORD>>STARTBITPOS)&255)								

										  								

#define SETBIT(WORD,BIT) (WORD |= 1<<BIT)								

										  								

#define SSETBIT(WORD,BIT) (WORD = 1<<BIT)								

										  								

#define CLRBIT(WORD,BIT) (WORD &= ~(1<<BIT))								

										  								

#define SCLRBIT(WORD,BIT) (WORD = 1<<BIT)								

										  								

#define CPLBIT(WORD,BIT) (WORD ^= 1<<BIT)  								

										  								

#define WRITEBIT(WORD,BIT,BITLEVEL)  WORD=((WORD &~(1<<BIT)) | (BITLEVEL<<BIT))								

										  								

#define READBIT(WORD,BIT) ((WORD>>BIT)&1)								

										  								

#define READWRITEBIT(DWORD,DBIT,SWORD,SBIT) DWORD=((DWORD &~(1<<DBIT))| (((SWORD>>SBIT)&1)<<DBIT))								

								


#define PASSWORD_LENGTH 4

#define UART_INT_ENABLE 1

#define RED_LED_PIN (1 << 4)


#define PARTY1_VOTE_ADDRESS 0x0100

#define PARTY2_VOTE_ADDRESS 0x0101

#define PARTY3_VOTE_ADDRESS 0x0102

#define PARTY4_VOTE_ADDRESS 0x0103

#define VOTER_COUNT_ADDRESS 0x150


#define PASSWORD_EEPROM_ADDRESS 0x200


/*#define PARTY1_ADDRESS 0x101																

#define PARTY2_ADDRESS 0x102								

#define PARTY3_ADDRESS 0x103						

#define PARTY4_ADDRESS 0x104
 */

#define UART_INT_ENABLE 1
