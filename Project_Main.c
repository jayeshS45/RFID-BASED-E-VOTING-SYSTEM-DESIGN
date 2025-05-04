#include <lpc21xx.h>

#include <stdio.h>

#include <string.h>

#include <stdint.h>

#include "types.h"

#include "kpm.h"

#include "i2c.h"

#include "lcd_defines.h"

#include "defines.h"

#include "delay.h"

#include "lcd.h"

#include "uart_int_test.h"

#define PASSWORD_LENGTH 4

u8 storedPassword[PASSWORD_LENGTH] = "1234";

u8 officerPassword[PASSWORD_LENGTH] = "4321";


extern int i;
extern char r_flag;
// Custom character bit patterns for each party symbol

unsigned char cycleSymbol[8] = {0x04,0x1F,0x15,0x1F,0x04,0x0A,0x00,0x00};// cycle(TDP)

unsigned char glassSymbol[8] = {0x1F, 0x11, 0x11, 0x0E, 0x0A, 0x0A, 0x0E, 0x00}; // Glass (JSP)

unsigned char lotusSymbol[8] = {0x04, 0x0E, 0x1F, 0x15, 0x0E, 0x04, 0x0A, 0x00}; // Lotus (BJP)

unsigned char handSymbol[8] = {0x04, 0x0E, 0x0E, 0x0E, 0x0E, 0x04, 0x04, 0x00};  // Hand (INC)

void LoadCustomCharacter(unsigned char, unsigned char*) ;

void DisplayCustomSymbolsWithNames(void) ;

void EnterPassword(u8*);

//extern unsigned char r_flag;


#define PASSWORD_LENGTH 4


int main() 

{
	int f=0,j=0;
    char cardID[10] = {0};

   	const char *expectedVoterCardID[] ={"12540123","12611820","12529332"};
	
//	u8 expectedVoterCardID[]= "12540123";

	//	const char* card[]={"12626214","12534588","12534288"}
	
    u8 expectedOfficerCardID[] = "12555657";

    u32 choice, partyChoice;

    u32 voteCount;

    u16 address;

    u8 enteredPassword[PASSWORD_LENGTH + 1]; // Buffer to store password

    u8 newPassword[PASSWORD_LENGTH + 1];

    u8 confirmPassword[PASSWORD_LENGTH + 1];

    u32 voterNumber;

    int k;


    // Initialize peripherals

    InitUART0();

    InitLCD();

    Init_KPM();

    init_i2c();

    IODIR0 |= RED_LED_PIN; // Set red LED pin as output


	for(i=0;i<3;i++)
	{
		i2c_eeprom_write(0x50,VOTER_COUNT_ADDRESS+i,0);		
	}
    // Read current voter count from EEPROM

    //voterNumber = i2c_eeprom_read(0x50, VOTER_COUNT_ADDRESS);


    StrLCD("RFID E-Voting System");

    delay_ms(2000);

    CmdLCD(0x01); // Clear LCD


    while (1) 
	{

        CmdLCD(0x01);

        StrLCD("Waiting for Card...");

        //delay_ms(2000);

		i=0,r_flag=0,memset(buff,'\0',10);
        RFID_ReadCard(); // Wait for card data
		memset(cardID,'\0',10);
        strcpy(cardID, buff); // Copy card ID from buffer
				  
        CmdLCD(0x01); // Clear LCD display

        StrLCD(cardID); // Display card ID on LCD
		delay_ms(2000);
		f=0;
		j=0;
		k=0;
		for(i=0;i<3;i++)
		{
        if (strcmp((char*)cardID, (char*)expectedVoterCardID[i]) == 0) 
		{ // Voter 
			if((i2c_eeprom_read(0x50, VOTER_COUNT_ADDRESS+i)==0))
			{
        /*if (strcmp((char*)repcardID[i], (char*)expectedVoterCardID[i]) == 0) 
		{// Voter 
		v++;
		}
			if(v>=1)
			{
	            StrLCD("Vote ");
			}*/
			f=1;
			j=1;
	
	       //i2c_eeprom_write(0x50, VOTER_COUNT_ADDRESS+i,1); // Save updated voter count

            

            // Display voter number on LCD

            CmdLCD(0x01);

            StrLCD("Welcome Voter ");

            CharLCD('0' + voterNumber);

            delay_ms(1000); // Show message briefly, then proceed to menu

            CmdLCD(0x01); // Clear LCD for main menu

            StrLCD("1. VOTING");

            CmdLCD(0xC0);

            StrLCD("2. EDIT PASSWORD");

            CmdLCD(0x94);

            StrLCD("3. EXIT");


            choice = KeyScan();

            switch(choice) {

                case '1': // Voting

                    CmdLCD(0x01);

                    StrLCD("Enter Password:");

                    delay_ms(1000);

                    

                    EnterPassword(enteredPassword);  // Use ReadNum to get password


                    if (ComparePassword(enteredPassword, storedPassword)) {
						
						
                        voterNumber++; // Increment voter count if vote is cast

                        CmdLCD(0x01);

                        StrLCD("Select Party:");

                        delay_ms(2000);

                        DisplayCustomSymbolsWithNames(); // Display party symbols with name

                        partyChoice = KeyScan();


                        if (partyChoice >= '1' && partyChoice <= '4') {

                            address = PARTY1_VOTE_ADDRESS + ((partyChoice-48) - 1);

                            voteCount = i2c_eeprom_read(0x50, address);

                            i2c_eeprom_write(0x50, address, voteCount + 1);
						

                            CmdLCD(0x01);

                            StrLCD("Vote Casted");

                            delay_ms(1000);

                        } else {

                            CmdLCD(0x01);

                            StrLCD("Invalid Party Choice");

                            delay_ms(1000);

                        }

                    } else {

                        IOSET0 = RED_LED_PIN;

                        CmdLCD(0x01);

                        StrLCD("Incorrect Pwd");

                        delay_ms(1000);

                        IOCLR0 = RED_LED_PIN;

                    }

                    break;


                case '2': // Change Password

                    CmdLCD(0x01);

                    StrLCD("Enter Old Pwd:");

                    delay_ms(1000);

                    

                    EnterPassword(enteredPassword);  // Use ReadNum for old password entry


                    if (ComparePassword(enteredPassword, storedPassword)) {

                        CmdLCD(0x01);
										   
                        StrLCD("Enter New Pwd:");

                        delay_ms(1000);

                        EnterPassword(newPassword);  // Use ReadNum for new password entry


                        CmdLCD(0x01);

                        StrLCD("Confirm Pwd:");

                        delay_ms(1000);

                        EnterPassword(confirmPassword);  // Use ReadNum to confirm password


                        if (ComparePassword(newPassword, confirmPassword)) {

                            SavePasswordToEEPROM(PASSWORD_EEPROM_ADDRESS, newPassword);

                            CmdLCD(0x01);

                            StrLCD("Pwd Updated");

                            delay_ms(1000);

                        } else {

                            CmdLCD(0x01);

                            StrLCD("Mismatch");

                            delay_ms(1000);

                        }

                    } else {

                        IOSET0 = RED_LED_PIN;

                        CmdLCD(0x01);

                        StrLCD("Incorrect Pwd");

                        delay_ms(1000);

                        IOCLR0 = RED_LED_PIN;

                    }

                    break;


                case '3': // Exit

                    CmdLCD(0x01);

                    StrLCD("Exiting...");

                    delay_ms(1000);
					goto end;
                    //break;


                default:

                    CmdLCD(0x01);

                    StrLCD("Invalid Choice");

                    delay_ms(1000);

                    break;

            }
			i2c_eeprom_write(0x50, VOTER_COUNT_ADDRESS+i,1);
			break;

        }
		else 
		{

            CmdLCD(0x01);

            StrLCD("vote already done");

            delay_ms(1000);

        } 
	}
		
	}
if(j==0)
{
 // if ((f == 0) && (strcmp(cardID, expectedOfficerCardID) == 0)) 
if(((f==0) && (strcmp((char*)cardID, (char*)expectedOfficerCardID)==0)))
{ // Officer ID

    CmdLCD(0x01);
	k=1;

    StrLCD("1. VOTING RESULT");

    CmdLCD(0xC0);

    StrLCD("2. VOTING RESET");

    CmdLCD(0x94);

    StrLCD("3. EXIT");

    delay_ms(2000);


    choice = KeyScan();

    switch (choice) {

        case '1': // Display Voting Results

            CmdLCD(0x01);

            StrLCD("Enter Security Pwd:");

            delay_ms(1000);


            EnterPassword(enteredPassword); // Use ReadNum for password entry


            if (ComparePassword(enteredPassword, officerPassword)) {  // Assuming `officerPassword` is pre-stored

                DisplayVotingResults(); // Display results
				
				delay_ms(5000);


            } else {

                IOSET0 = RED_LED_PIN; // Turn on red LED to indicate error

                CmdLCD(0x01);

                StrLCD("Incorrect Pwd");

                delay_ms(1000);

                IOCLR0 = RED_LED_PIN; // Turn off red LED

            }

            break;


        case '2': // Reset Votes

            CmdLCD(0x01);

            StrLCD("Enter Security Pwd:");

            delay_ms(1000);


            EnterPassword(enteredPassword); // Use ReadNum for password entry


            if (ComparePassword(enteredPassword, officerPassword)) {

                ResetVotingResults();

                // Reset each party vote count in EEPROM

                i2c_eeprom_write(0xA0, PARTY1_VOTE_ADDRESS, 0);

                i2c_eeprom_write(0xA0, PARTY2_VOTE_ADDRESS, 0);

                i2c_eeprom_write(0xA0, PARTY3_VOTE_ADDRESS, 0);

                i2c_eeprom_write(0xA0, PARTY4_VOTE_ADDRESS, 0);

                CmdLCD(0x01);

                StrLCD("Voting Reset Done");

                delay_ms(1000);

            } else {

                IOSET0 = RED_LED_PIN; // Turn on red LED to indicate error

                CmdLCD(0x01);

                StrLCD("Incorrect Pwd");

                delay_ms(1000);

                IOCLR0 = RED_LED_PIN; // Turn off red LED

            }

            break;


        case '3': // Exit

            CmdLCD(0x01);

            StrLCD("Exiting...");

            delay_ms(1000);

            break;


        default:

            CmdLCD(0x01);

            StrLCD("Invalid Choice");

            delay_ms(1000);

            break;

    }

}

/*else if((i==3) && (k==0))
{

            CmdLCD(0x01);

            StrLCD("Invalid Card");

            delay_ms(1000);

        } */
	   }
	   else if((i==3) && (k==0))
		{

            CmdLCD(0x01);

            StrLCD("Invalid Card");

            delay_ms(1000);

        }
		end: i=0;
    }

}


// Use ReadNum to enter the password

void EnterPassword(u8 *passwordBuffer) {

u8 keyV;
int i=0;
while(1)
{

keyV=KeyScan();	
delay_ms(100);

		if(keyV>='0' && keyV<='9')	
		{
		
			passwordBuffer[i]=keyV;
			CmdLCD(0xC0+i);
			CharLCD(keyV);
			delay_ms(100);
			CmdLCD(0xC0+i);
			CharLCD('*');
			i++;
			}
			else if(keyV=='C')
			{
			if (i==0)
			{
			continue;
			}
			i--;
			CmdLCD(0xC0+i);
			CharLCD(' ');
			CmdLCD(0xC0+i);
			}
			else if(keyV == '=')
			{
			break;
			}
		

}
/*	  int i;

    u32 numInput = ReadNum();  // Read numeric input using ReadNum

    sprintf((char *)passwordBuffer, "%04u", numInput);  // Store the input as a string, zero-padded if necessary

    CmdLCD(0xC0); // Move cursor to the next line for displaying asterisks

    for (i = 0; i < PASSWORD_LENGTH; i++) {

        CharLCD('*');  // Display '*' for each character in password*/

    

}





// Function to load custom characters into CGRAM

void LoadCustomCharacter(unsigned char location, unsigned char *pattern) {

    int i;

    location &= 0x07; // Only 8 CGRAM locations are available (0-7)

    CmdLCD(0x40 | (location << 3)); // Set CGRAM address

    

    for (i = 0; i < 8; i++) {

        CharLCD(pattern[i]); // Write custom character pattern to CGRAM

    }

}


// Function to display custom party symbols with names on LCD

void DisplayCustomSymbolsWithNames() {

    // Load custom symbols for each party

    LoadCustomCharacter(0, cycleSymbol);   // BAN symbol

    LoadCustomCharacter(1, glassSymbol);  // VEC symbol

    LoadCustomCharacter(2, lotusSymbol);  // TOR symbol

    LoadCustomCharacter(3, handSymbol);   // HYD symbol


    // Display symbols with names on the LCD

    CmdLCD(0x80);  // Move to the first line

    CharLCD(0);    // Display Congress flag symbol

    StrLCD("1.BAN ");


    CmdLCD(0xC0);  // Move to the second line

    CharLCD(1);    // Display TDP glass symbol

    StrLCD("2.VEC Party: ");


    CmdLCD(0x94);  // Move to the third line

    CharLCD(2);    // Display BJP lotus symbol

    StrLCD("3.TOR Party: ");


    CmdLCD(0xD4);  // Move to the fourth line

    CharLCD(3);    // Display Jana Sena hand symbol

    StrLCD("4.HYD Party:");

}
   



