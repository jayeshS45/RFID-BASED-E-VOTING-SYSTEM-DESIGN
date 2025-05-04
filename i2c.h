#include "types.h"

#include <stdint.h>

void init_i2c(void);

void i2c_start(void);

void i2c_restart(void);

void i2c_write(u8 dat);

void i2c_stop(void);

u8 i2c_nack(void);

u8 i2c_masterack(void);

void i2c_eeprom_write(u8 ,u16 ,u8 );

u8 i2c_eeprom_read(u8,u16);

void i2c_eeprom_page_write(u8 ,u16 ,u8 *,u8);

void i2c_eeprom_seq_read(u8 ,u16 ,u8 *,u8);

void ReadPasswordFromEEPROM(uint16_t , uint8_t *);

int SavePasswordToEEPROM(uint16_t, uint8_t *);

int ComparePassword(uint8_t *, uint8_t *);

void WriteToEEPROM(uint16_t, uint8_t );

uint8_t ReadFromEEPROM(uint16_t );

void ResetVotingResults(void);

void DisplayVotingResults(void);

void LoadCustomCharacter(unsigned char, unsigned char*);

void DisplayCustomSymbolsWithNames(void);

