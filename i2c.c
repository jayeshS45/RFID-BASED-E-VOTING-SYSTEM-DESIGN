#include <LPC21xx.h>
#include <stdint.h>
#include <stdio.h>
#include "types.h"			
#include "delay.h"	
#include "defines.h"
#include "lcd.h"
#include "i2c.h"
#include "i2c_defines.h"

int EEPROM[EEPROM_SIZE]; 
char lcdBuffer[16];	

void init_i2c(void)			
{			
	 //Configure I/O pin for SCL & SDA functions using PINSEL0		
		 PINSEL0|=SCL_EN|SDA_EN;		

	 //Configure Speed for I2C Serial Communication		
	 //Using I2CSCLL		
		 I2SCLL=LOADVAL;		

     //& I2CSCLH			
  	 I2SCLH=LOADVAL;		

	//I2C Peripheral Enable for Communication		
	 I2CONSET=1<<I2EN_BIT; 		
}			

void i2c_start(void)			

{			

	// start condition		

  I2CONSET=1<<STA_BIT;			

	//wait for start bit status		

	while(((I2CONSET>>SI_BIT)&1)==0);		

	// clear start condition		

	I2CONCLR=1<<STA_BIT;		

}			

void i2c_restart(void)			

{			

	// start condition		

	I2CONSET=1<<STA_BIT;		

	//clr SI_BIT		

	I2CONCLR=1<<SI_BIT;		

	//wait for SI bit status		

	while(((I2CONSET>>SI_BIT)&1)==0); 	

	// clear start condition		

	I2CONCLR=1<<STA_BIT;		

}			

void i2c_write(u8 dat)			

{			

	//put data into I2DAT		

	I2DAT=dat;		

	//clr SI_BIT		

	I2CONCLR = 1<<SI_BIT;		

	//wait for SI bit status		

	while(((I2CONSET>>SI_BIT)&1)==0);		

}			

void i2c_stop(void)			

{			

	// issue stop condition		

	I2CONSET=1<<STO_BIT;		

  // clr SI bit status			

	I2CONCLR = 1<<SI_BIT;		

	//stop will cleared automatically		

	//while(((I2CONSET>>STO_BIT)&1));		

}			

u8 i2c_nack(void)			

{			

	I2CONSET = 0x00; //Assert Not of Ack		

    I2CONCLR = 1<<SI_BIT;			

	while(((I2CONSET>>SI_BIT)&1)==0);		

    return I2DAT;			

}			

u8 i2c_masterack(void)			

{			

	I2CONSET = 0x04; //Assert Ack		

  I2CONCLR = 1<<SI_BIT;			

	while(((I2CONSET>>SI_BIT)&1)==0);		

	I2CONCLR = 0x04; //Clear Assert Ack		

 	return I2DAT;		

}			

void i2c_eeprom_write(u8 slaveAddr,u16 wBuffAddr,u8 dat)			

{			

    i2c_start();			

    i2c_write(slaveAddr<<1); //slaveAddr + w			

	i2c_write((wBuffAddr>>8)&0xFF);    //wBuffAddr		

	i2c_write((wBuffAddr)&0xFF);		

	i2c_write(dat);    //wBuffAddr		

	i2c_stop();		

	delay_ms(10);		

			

}			 

u8 i2c_eeprom_read(u8 slaveAddr,u16 rBuffAddr)			

{			

	u8 dat;		

	i2c_start();		

    i2c_write(slaveAddr<<1); //slaveAddr + w			

	i2c_write((rBuffAddr>>8)&0xFF);    //rBuffAddr		

	i2c_write((rBuffAddr)&0xFF);		

	i2c_restart();		

	i2c_write(slaveAddr<<1|1); //slaveAddr + r		

    dat=i2c_nack();			

	i2c_stop();		

	return dat;		

}			

void i2c_eeprom_page_write(u8 slaveAddr,u16 wBuffStartAddr,u8 *p,u8 nBytes)			

{			

    u8 i;			

    i2c_start();			

    i2c_write(slaveAddr<<1);    //slaveAddr + w			

	i2c_write((wBuffStartAddr>>8)&0xFF); 		

	i2c_write((wBuffStartAddr)&0xFF);  //wBuffStartAddr		

	for(i=0;i<nBytes;i++)		

	{		

	   i2c_write(p[i]);             //wBuffAddr		

	}		

	i2c_stop();		

	delay_ms(10);		

}			 

void i2c_eeprom_seq_read(u8 slaveAddr,u16 rBuffStartAddr,u8 *p,u8 nBytes)			

{			

	u8 i;		

	i2c_start();		

    i2c_write(slaveAddr<<1); //slaveAddr + w			

	i2c_write((rBuffStartAddr>>8)&0xFF);   		

	i2c_write((rBuffStartAddr)&0xFF);  //rBuffAddr		

	i2c_restart();		

	i2c_write(slaveAddr<<1|1); //slaveAddr + r		

	for(i=0;i<nBytes-1;i++)		

	{		

         p[i]=i2c_masterack();			

	}		

	p[i]=i2c_nack();		

	i2c_stop();		

}	


// Function to read the password from EEPROM			

			

void ReadPasswordFromEEPROM(uint16_t address, uint8_t *password) {			

			

	int i;		

			

    for (i = 0; i < PASSWORD_LENGTH; i++) {			

			

        password[i] = ReadFromEEPROM(address + i);  // Read each byte from EEPROM			

			

    }			

			

}			

			

			

// Function to save the password to EEPROM			

int SavePasswordToEEPROM(uint16_t address, uint8_t *password) {			

			

   int i; 			

			

	 for (i = 0; i < PASSWORD_LENGTH; i++) {		

			

        WriteToEEPROM(address + i, password[i]);  // Write each byte to EEPROM			

			

    }			

			

    return 1;			

			

}			

			

 			

			

// Function to compare two passwords			

			

int ComparePassword(uint8_t *inputPassword, uint8_t *storedPassword) {			

			

  int i;  			

			

	for (i = 0; i < PASSWORD_LENGTH; i++) {		

			

        if (inputPassword[i] != storedPassword[i]) {			

			

            return 0; // Return false if any byte does not match			

			

        }			

			

    }			

			

    return 1; // Return true if all bytes match			

			

}			

			

			

// Function to write data to EEPROM			

			

void WriteToEEPROM(uint16_t address, uint8_t data) {			

			

    if (address < EEPROM_SIZE) { // Ensure address is within EEPROM bounds			

			

        EEPROM[address] = data;			

			

    }			

			

}			

			

			

// Function to read data from EEPROM			

			

uint8_t ReadFromEEPROM(uint16_t address) {			

			

    if (address < EEPROM_SIZE) { // Ensure address is within EEPROM bounds			

			

        return EEPROM[address];			

			

    }			

			

    return 0; // Return 0 if address is out of bounds			

			

}			


void DisplayVotingResults() {								

								

    uint8_t party1Votes, party2Votes, party3Votes, party4Votes;								

								

								

    // Read the vote count for each party from EEPROM								

								

   /* party1Votes = ReadFromEEPROM(PARTY1_ADDRESS);								

								

    party2Votes = ReadFromEEPROM(PARTY2_ADDRESS);								

								

    party3Votes = ReadFromEEPROM(PARTY3_ADDRESS);								

								

    party4Votes = ReadFromEEPROM(PARTY4_ADDRESS);								
	 */

	party1Votes = i2c_eeprom_read(0x50,PARTY1_VOTE_ADDRESS);								
	party2Votes = i2c_eeprom_read(0x50,PARTY2_VOTE_ADDRESS);
	party3Votes = i2c_eeprom_read(0x50,PARTY3_VOTE_ADDRESS);
	party4Votes = i2c_eeprom_read(0x50,PARTY4_VOTE_ADDRESS);
								

    // Display results on LCD								

								

    CmdLCD(0x01); // Clear LCD screen								

								

    StrLCD("Voting Results:");								

								

								

    // Display Party 1 and Party 2 votes on line 2								

								

    CmdLCD(0xC0); // Move to line 2								
								
    sprintf(lcdBuffer, "P1:%d     P2:%d", party1Votes, party2Votes);								
								
    StrLCD(lcdBuffer);								
						
    // Display Party 3 and Party 4 votes on line 3								
					
    CmdLCD(0x94); // Move to line 3								
							
    sprintf(lcdBuffer, "P3:%d     P4:%d", party3Votes, party4Votes);								
							
    StrLCD(lcdBuffer);								

								

								

    // Keep the result on screen for a few seconds								

								

    delay_ms(5000);								

								

    CmdLCD(0x01); // Clear LCD after displaying								
	if((party1Votes>party2Votes)&& (party1Votes>party3Votes) && (party1Votes>party4Votes))
	{
		StrLCD("Party 1 wins");
		delay_ms(2000);	
	}
	else if((party2Votes>party1Votes)&& (party2Votes>party3Votes) && (party1Votes>party4Votes))
	{
		StrLCD("Party 2 wins");
		delay_ms(2000);	
	}

	else if((party3Votes>party1Votes)&& (party3Votes>party2Votes) && (party1Votes>party4Votes))
	{
		StrLCD("Party 3 wins");
		delay_ms(2000);	
	}

	else if((party4Votes>party1Votes)&& (party2Votes>party2Votes) && (party4Votes>party3Votes))
	{
		StrLCD("Party 4 wins");
		delay_ms(2000);	
	}

								

}								

								

																		
// Function to reset voting results in EEPROM								
void ResetVotingResults() 
{
    // Reset the vote count for each party in EEPROM								
	int j;							
 /*   WriteToEEPROM(PARTY1_ADDRESS, 0);								
 								
    WriteToEEPROM(PARTY2_ADDRESS, 0);								
 						
    WriteToEEPROM(PARTY3_ADDRESS, 0);								
 							
    WriteToEEPROM(PARTY4_ADDRESS, 0);													
*/		
	for(j=0;j<=3;j++)
	{
		i2c_eeprom_write(0x50,PARTY1_VOTE_ADDRESS+j,0);		
	}				
    // Confirm reset on the LCD								
							
    CmdLCD(0x01); // Clear LCD								
			
    StrLCD("Votes Reset to 0");								
								
    delay_ms(1000); // Delay to display the confirmation message								
								
    CmdLCD(0x01); // Clear LCD after displaying confirmation								
}	

