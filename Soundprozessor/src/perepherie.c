#include "perepherie.h"


void readkeys(uint8_t* keys,uint8_t* newkeys)
{
	uint8_t i=0, j, k, keynumber=0, newkeynumber=0, oldkeynumber=0, notkeynumbermax=1, notoldkey, input, multiplex, oldkeys[8];
	
	twi_package_t input_I2C =	{
		.addr			= 0x0,		//!!!Adresse muss noch angepasst werden!!!!
		.addr_length	= 0,
		.chip			= 0x38,
		.buffer			= &input,
		.length			= 1		 };
	
	twi_package_t multiplex_I2C =	{			//I2C Struktur zum multiplexen
		.addr			= 0x0,		//!!!Adresse muss noch angepasst werden!!!!
		.addr_length	= 0,
		.chip			= 0x38,
		.buffer			= &multiplex,
		.length			= 1		 };
	
	while(i<6 && notkeynumbermax)
	{
		multiplex=~(0x1<<i);
		while(twi_master_write(TWI2, &multiplex_I2C)  != TWI_SUCCESS );
		while(twi_master_read(TWI2, &input_I2C)  != TWI_SUCCESS );
		
		j=0;
		while(j<8 && notkeynumbermax)		//	
		{
			if(input & (0x1 << j))
			{
				notoldkey=1;
				k=0;
				
				while(k<8 || !notoldkey)
				{
					if(keys[k]==(i+1)*(j+1))
					{
						oldkeys[oldkeynumber]=keys[k];
						notoldkey=0;
						oldkeynumber++;
					}
					k++;
				}
				
				if(notoldkey)
				{
					newkeys[newkeynumber]=(i+1)*(j+1);
					newkeynumber++;
				}
				
				keynumber=oldkeynumber+newkeynumber;
				
				if(keynumber>7)
				{
					notkeynumbermax=0;
				}
			}
			j++;		
		}
	}
}

Settings readSettings(Settings settings)
{
	uint8_t input = 0;
	Settings newSettings = settings;
	
	twi_package_t settings_I2C =	{
		.addr			= 0x0,		//!!!Adresse muss noch angepasst werden!!!!
		.addr_length	= 0,
		.chip			= 0x38,	
		.buffer			= &input,		
		.length			= 1			 };
		
	while(twi_master_read(TWI2, &settings_I2C)  != TWI_SUCCESS );
	
	//Aufgrund der Eingabe sich ergebenede Modi in "newSettings" speichern
	if(~input & 0x1)
	{
		newSettings.Sustain = !newSettings.Sustain;
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
		newSettings.Release = !newSettings.Release;
	}
	
	//Kompatibilitaet der Modi sichern
	
	if(newSettings.arpeggio == 1 && settings.arpeggio == 0)		//Fall 1: Arpeggio vorher aus, nachher an --> anhaltenderton aus und halleffekt aus => (Prioritaet bei arpeggio)
	{
		newSettings.Sustain = 0;
		newSettings.Release = 0;
	}
	
	if(newSettings.arpeggio == 1 && (newSettings.Sustain == 1 || newSettings.Release == 1))		//Fall 2: Fall 1 nicht eingetreten und Arpeggio nachher an und anhaltenderton oder halleffekt nachher an --> arpeggio aus 
	{
		newSettings.arpeggio = 0;
	}

	return newSettings;
}

void  writeLed(Settings settings)
{
	uint8_t output = settings.Sustain + settings.arpeggio*2 + settings.burst*4 + settings.Release*8;
	
	twi_package_t writeled =	{
		.addr			= 0x0,		//!!!Adresse muss noch angepasst werden!!!!
		.addr_length	= 0,
		.chip			= 0x38,
		.buffer			= &output,
		.length			= 1			 };
	
	while(twi_master_write(TWI2, &writeled) != TWI_SUCCESS);
	
	return;
}

uint8_t getReleaseValue()	//Datentyp klaeren
{
	adc_start_software_conversion(ADC);
	while (!(adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_2)));	//Channel klaeren
	return adc_channel_get_value(ADC, ADC_CHANNEL_2);
}

uint8_t getSustainValue()
{
	adc_start_software_conversion(ADC);
	while (!(adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_2)));	//Channel klaeren
	return adc_channel_get_value(ADC, ADC_CHANNEL_2);
}

uint8_t getArpeggioValue()
{
	adc_start_software_conversion(ADC);
	while (!(adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_2)));	//Channel klaeren
	return adc_channel_get_value(ADC, ADC_CHANNEL_2);
}

uint8_t getDutyCycleVaule()
{
	adc_start_software_conversion(ADC);
	while (!(adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_2)));	//Channel klaeren
	return adc_channel_get_value(ADC, ADC_CHANNEL_2);
}