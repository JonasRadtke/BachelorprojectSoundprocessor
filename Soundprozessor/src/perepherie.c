#include "perepherie.h"



Settings readsettings(Settings settings)
{
	uint8_t input = 0;
	Settings newSettings = settings;
	
	twi_package_t input_I2C =	{
		.addr			= 0x0,		//!!!Adresse muss noch angepasst werden!!!!
		.addr_length	= 0,
		.chip			= 0x38,	
		.buffer			= &input,		
		.length			= 1			 };
		
	while(twi_master_read(TWI2, &input_I2C)  != TWI_SUCCESS );
	
	//Aufgrund der Eingabe sich ergebenede Modi in "newSettings" speichern
	if(~input & 0x1)
	{
		newSettings.anhaltenderton = !newSettings.anhaltenderton;
	}
	
	if(~input & 0x2)
	{
		newSettings.arpeggio = !newSettings.arpeggio;
	}
	
	if(~input & 0x4)
	{
		newSettings.burst = !newSettings.burst;
	}
	
	if(~input & 0x8)
	{
		newSettings.halleffekt = !newSettings.halleffekt;
	}
	
	//Kompatibilitaet der Modi sichern
	
	if(newSettings.arpeggio == 1 && settings.arpeggio == 0)		//Fall 1: Arpeggio vorher aus, nachher an --> anhaltenderton aus und halleffekt aus => (Prioritaet bei arpeggio)
	{
		newSettings.anhaltenderton = 0;
		newSettings.halleffekt = 0;
	}
	
	if(newSettings.arpeggio == 1 && (newSettings.anhaltenderton == 1 || newSettings.halleffekt == 1))		//Fall 2: Fall 1 nicht eingetreten und Arpeggio nachher an und anhaltenderton oder halleffekt nachher an --> arpeggio aus 
	{
		newSettings.arpeggio = 0;
	}

	return newSettings;
}

void  writeLed(Settings settings)
{
	uint8_t output = settings.anhaltenderton + settings.arpeggio*2 + settings.burst*4 + settings.halleffekt*8;
	
	twi_package_t writeled =	{
		.addr			= 0x0,		//!!!Adresse muss noch angepasst werden!!!!
		.addr_length	= 0,
		.chip			= 0x38,
		.buffer			= &output,
		.length			= 1			 };
	
	while(twi_master_write(TWI2, &writeled) != TWI_SUCCESS);
	
	return;
}

uint8_t getVolumeValue();		//Datentyp klaeren
{
	adc_start_software_conversion(ADC);
	while (!(adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_2)));	//Channel klaeren
	return adc_channel_get_value(ADC, ADC_CHANNEL_2);
}

uint8_t getAnhaltenderTonValue();
{
	adc_start_software_conversion(ADC);
	while (!(adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_2)));	//Channel klaeren
	return adc_channel_get_value(ADC, ADC_CHANNEL_2);
}

uint8_t getArpeggioValue();
{
	adc_start_software_conversion(ADC);
	while (!(adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_2)));	//Channel klaeren
	return adc_channel_get_value(ADC, ADC_CHANNEL_2);
}

uint8_t getDutyCycleVaule();
{
	adc_start_software_conversion(ADC);
	while (!(adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_2)));	//Channel klaeren
	return adc_channel_get_value(ADC, ADC_CHANNEL_2);
}