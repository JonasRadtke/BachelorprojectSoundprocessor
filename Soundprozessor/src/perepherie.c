#include "perepherie.h"


Settings readsettings(Settings settings)
{
	uint8_t input = 0;
	Settings newSettings;
	
	twi_package_t input_I2C =	{
		.addr			= 0x0,		//!!!Adresse muss noch angepasst werden!!!!
		.addr_length	= 0,
		.chip			= 0x38,	
		.buffer			= &input,		
		.length			= 1			 };
		
	while(twi_master_read(TWI2, &input_I2C)  != TWI_SUCCESS );
	
	//Kompatibilitaet der Modi sichern
	
	
	newSettings = {.anhaltenderton= input & 0x1,.arpeggio=input & 0x2,.burst=input & 0x4,.halleffekt=input & 0x8}
	
	//
	
	return settings;
}

void  writeLed(Settings settings)
{
	uint8_t output = settings.anhaltenderton + settings.arpeggio + settings.burst + settings.halleffekt;
	
	twi_package_t writeled =	{
		.addr			= 0x0,		//!!!Adresse muss noch angepasst werden!!!!
		.addr_length	= 0,
		.chip			= 0x38,
		.buffer			= &output,
		.length			= 1			 };
	
	while(twi_master_write(TWI2, &writeled) != TWI_SUCCESS);
	
	return;
}

