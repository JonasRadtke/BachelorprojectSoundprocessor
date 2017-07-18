#include "perepherie.h"


uint8_t readsettings(uint8_t settings)
{
	uint8_t input = 0;
	
	twi_package_t input_I2C =	{
		.addr			= 0x0,		//!!!Adresse muss noch angepasst werden!!!!
		.addr_length	= 0,
		.chip			= 0x38,	
		.buffer			= &input,		
		.length			= 1			 };
		
	while(twi_master_read(TWI2, &input_I2C)  != TWI_SUCCESS );
	
	//Kompatibilitaet der Modi sichern
	
	
	
	//
	
	return settings;
}

void  writeLed(uint8_t* settings)
{
	twi_package_t writeled =	{
		.addr			= 0x0,		//!!!Adresse muss noch angepasst werden!!!!
		.addr_length	= 0,
		.chip			= 0x38,
		.buffer			= settings,
		.length			= 1			 };
	
	

	while(twi_master_write(TWI2, &writeled) != TWI_SUCCESS);
	
	return;
}

