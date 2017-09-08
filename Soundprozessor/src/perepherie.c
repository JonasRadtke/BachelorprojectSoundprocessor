#include "perepherie.h"

void twiInit(){
	// I2C activate
	pmc_enable_periph_clk(ID_PIOB);	// Clock auf PIOB
	pio_set_peripheral 	( PIOB, PIO_PERIPH_B, PIO_PB0 | PIO_PB1 );	// TWI pins aktivieren, Pins werden auf B Spalte gesetzt, siehe Datenblatt

	sysclk_enable_peripheral_clock(ID_TWI2);
	// Disable TWI interrupts
	TWI2->TWI_IDR = ~0UL;
	// Dummy read in status register
	TWI2->TWI_SR;
	// Reset TWI peripheral
	// Set SWRST bit to reset TWI peripheral
	TWI2->TWI_CR = TWI_CR_SWRST;
	TWI2->TWI_RHR;
	// Set Master Disable bit and Slave Disable bit
	TWI2->TWI_CR = TWI_CR_MSDIS;
	TWI2->TWI_CR = TWI_CR_SVDIS;
	// Set Master Enable bit
	TWI2->TWI_CR = TWI_CR_MSEN;
	/* set clock waveform generator register */
	TWI2->TWI_CWGR = TWI_CWGR_CLDIV(236) | TWI_CWGR_CHDIV(236) | TWI_CWGR_CKDIV(0);
	
	delay_us(500);
}

void adcInit(){
	//ADC Init
	pmc_enable_periph_clk(ADC);
	adc_enable();

	ADC->ADC_MR =  (ADC_MR_PRESCAL(SystemCoreClock /(2 * 6000000) - 1)) | ADC_MR_TRACKTIM(2) | ADC_MR_TRANSFER(2) | ADC_STARTUP_TIME_4;
	ADC->ADC_MR |= ADC_10_BITS;
	ADC->ADC_EMR &= ~ADC_EMR_OSR_Msk;
	
	adc_set_trigger(ADC, ADC_TRIG_SW);
	adc_channel_enable(ADC, ADC_CHANNEL_0);
	adc_channel_enable(ADC, ADC_CHANNEL_1);
	adc_channel_enable(ADC, ADC_CHANNEL_2);
	adc_channel_enable(ADC, ADC_CHANNEL_3);
}


void readkeys(uint8_t keys[], Settings* settingspointer)
{
	uint8_t i=0, j, k, keynumber=0, newkeynumber=0, oldkeynumber=0, notkeynumbermax=1, notoldkey, input, multiplex, oldkeys[8];
	static uint32_t twiStep;
	uint32_t status = 0;
	
	twi_package_t input_I2C =	{
		.addr			= 0x00,
		.addr_length	= 0,
		.chip			= 0x39,
		.buffer			= &input,
		.length			= 1		 };
	
	twi_package_t multiplex_I2C =	{			//I2C Struktur zum multiplexen
		.addr			= 0x00,		
		.addr_length	= 0,
		.chip			= 0x38,
		.buffer			= &multiplex,
		.length			= 1		 };
	
	if (twiStep == 0 || twiStep > 18)
	{
		twiStep = 1;
	}
	
	switch(twiStep) {
		case 1:
			multiplex=~0x01;
			if (sendPortexpander(TWI2, &multiplex_I2C) == 1){
				twiStep ++;
			}
		break;
		case 2:		
			if (receivePortexpander(TWI2, &input_I2C) == 1){
				twiStep ++;
				keys[0] &= 0xC0;
				keys[0] |= (~input & 0x3F);
			}
		break;
		
		case 3:
			multiplex=~0x02;
			if (sendPortexpander(TWI2, &multiplex_I2C) == 1){
				twiStep ++;
			}
		break;
		case 4:
			if (receivePortexpander(TWI2, &input_I2C) == 1){
				twiStep ++;
				keys[0] &= 0x3F;
				keys[0] |= ((~input << 6) & 0xC0);
				
				keys[1] &= 0xF0;
				keys[1] |= ((~input >> 2) & 0x0F);
			}
		break;
		
		case 5:
			multiplex=~0x04;
			if (sendPortexpander(TWI2, &multiplex_I2C) == 1){
				twiStep ++;
			}
		break;
		case 6:
			if (receivePortexpander(TWI2, &input_I2C) == 1){
				twiStep ++;
				keys[1] &= 0x0F;
				keys[1] |= ((~input << 4) & 0xF0);
				
				keys[2] &= 0xFC;
				keys[2] |= ((~input >> 4) & 0x03);
			}
		break;
		
		case 7:
			multiplex=~0x08;
			if (sendPortexpander(TWI2, &multiplex_I2C) == 1){
				twiStep ++;
			}
		break;
		case 8:
			if (receivePortexpander(TWI2, &input_I2C) == 1){
				twiStep ++;
				keys[2] &= 0x03;
				keys[2] |= ((~input << 2) & 0xFC);
			}
		break;
		
		case 9:
			multiplex=~0x10;
			if (sendPortexpander(TWI2, &multiplex_I2C) == 1){
				twiStep ++;
			}
		break;
		case 10:
			if (receivePortexpander(TWI2, &input_I2C) == 1){
				twiStep ++;
				keys[3] &= 0xC0;
				keys[3] |= (~input & 0x3F);
			}
		break;
		
		case 11:
			multiplex=~0x20;
			if (sendPortexpander(TWI2, &multiplex_I2C) == 1){
				twiStep ++;
			}
		break;
		case 12:
			if (receivePortexpander(TWI2, &input_I2C) == 1){
				twiStep ++;
				keys[3] &= 0x3F;
				keys[3] |= ((~input << 6) & 0xC0);
				
				keys[4] &= 0xF0;
				keys[4] |= ((~input >> 2) & 0x0F);
			}
		break;

		case 13:
			multiplex=~0x40;
			if (sendPortexpander(TWI2, &multiplex_I2C) == 1){
				twiStep ++;
			}
		break;
		case 14:
			if (receivePortexpander(TWI2, &input_I2C) == 1){
				twiStep ++;
				keys[4] &= 0x0F;
				keys[4] |= ((~input << 4) & 0xF0);
				
				keys[5] &= 0xFC;
				keys[5] |= ((~input >> 4) & 0x03);
			}
		break;
		
		case 15:
			multiplex=~0x80;
			if (sendPortexpander(TWI2, &multiplex_I2C) == 1){
				twiStep ++;
			}
		break;
		case 16:
			if (receivePortexpander(TWI2, &input_I2C) == 1){
				twiStep++;
				keys[5] &= 0x03;
				keys[5] |= ((~input << 2) & 0xFC);
			}
		break;
		case 17:
			if (readSettings(settingspointer) == 1){
				twiStep = 18;
			}
		break;
		case 18:
			if (writeLed(settingspointer) == 1){
				twiStep = 1;
			}
		break;
		default: break;
	}

/*	while(i<6 && notkeynumbermax)
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
	}*/
}

uint8_t readSettings(Settings* settingspointer)
{
	uint8_t input = 0;
	Settings newSettings = *settingspointer;
	uint32_t status = 0;
	
	twi_package_t settings_I2C =	{
		.addr			= 0x00,		//!!!Adresse muss noch angepasst werden!!!!
		.addr_length	= 0,
		.chip			= 0x3A,	
		.buffer			= &input,		
		.length			= 1			 };
		
	if (receivePortexpander(TWI2, &settings_I2C) == 1)
	{
		//Aufgrund der Eingabe sich ergebende Modi in "newSettings" speichern
		if((~input & newSettings.preinput) & 0x4)
		{
			newSettings.Sustain = !newSettings.Sustain;
		}
	
		if((~input & newSettings.preinput) & 0x8)
		{
			newSettings.arpeggio = !newSettings.arpeggio;
		}
	
		if((~input & newSettings.preinput) & 0x1)
		{
			newSettings.burst = !newSettings.burst;
		}
	
		if((~input & newSettings.preinput) & 0x2)
		{
			newSettings.Release = !newSettings.Release;
		}
		
		if (((~input) & 0x10))
		{
			newSettings.waveform = RECTANGLE;
		}
		else
		{
			newSettings.waveform = TRIANGLE;
		}
		
		
		//Kompatibilitaet der Modi sichern
	
		if(newSettings.arpeggio == 1 && (*settingspointer).arpeggio == 0)		//Fall 1: Arpeggio vorher aus, nachher an --> anhaltenderton aus und halleffekt aus => (Prioritaet bei arpeggio)
		{
			newSettings.Sustain = 0;
			newSettings.Release = 0;
			newSettings.burst = 0;
		}
	
		if(newSettings.arpeggio == 1 && (newSettings.Sustain == 1 || newSettings.Release == 1 || newSettings.burst == 1))		//Fall 2: Fall 1 nicht eingetreten und Arpeggio nachher an und anhaltenderton oder halleffekt nachher an --> arpeggio aus 
		{
			newSettings.arpeggio = 0;
		}
		newSettings.preinput = input;
		*settingspointer = newSettings;
		
		return 1;
	}
	return 0;
}

uint8_t writeLed(Settings* settings)
{
	uint8_t output = ~((settings->burst & 0x01) | ((settings->Release & 0x01) << 1) | ((settings->Sustain & 0x01) << 2) | ((settings->arpeggio & 0x01) << 3));
	
	twi_package_t writeled =	{
		.addr			= 0x00,		
		.addr_length	= 0,
		.chip			= 0x3B,
		.buffer			= &output,
		.length			= 1			 };
	
	if (sendPortexpander(TWI2, &writeled) == 1){
		return 1;
	}
	return 0;
}

uint32_t getReleaseValue()
{
	adc_start_software_conversion(ADC);
	while (!(adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_1)));
	return adc_channel_get_value(ADC, ADC_CHANNEL_1);
}

uint32_t getSustainValue()
{
	adc_start_software_conversion(ADC);
	while (!(adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_3)));
	return adc_channel_get_value(ADC, ADC_CHANNEL_3);
}

uint32_t getArpeggioValue()
{
	adc_start_software_conversion(ADC);
	while (!(adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_2)));
	return adc_channel_get_value(ADC, ADC_CHANNEL_2);
}

uint32_t getDutyCycleValue()
{
	adc_start_software_conversion(ADC);
	while (!(adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_0)));	
	return adc_channel_get_value(ADC, ADC_CHANNEL_0);
}

uint32_t sendPortexpander(Twi *p_twi, twi_packet_t *p_packet){
	static uint32_t step = 1;
	uint32_t status;
	uint8_t *buffer = p_packet->buffer;
	
	switch(step){
		case 1:
			p_twi->TWI_MMR = 0;
			p_twi->TWI_MMR = TWI_MMR_DADR(p_packet->chip) | ((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) & TWI_MMR_IADRSZ_Msk);
			
			status = p_twi->TWI_SR;
			
			step = 2;
		break;
		
		case 2:
			status = p_twi->TWI_SR;
			if ((status & TWI_SR_TXRDY))
			{
				p_twi->TWI_THR = *buffer;
				step = 3;
			}
		break;
		
		case 3:
		status = p_twi->TWI_SR;
		if ((status & TWI_SR_TXRDY))
		{
			p_twi->TWI_CR = TWI_CR_STOP;
			while (!(p_twi->TWI_SR & TWI_SR_TXCOMP)) {
			}
			step = 1;
			return 1;
		}
		break;
		default: break;	
	}
	return 0;
}

uint32_t receivePortexpander(Twi *p_twi, twi_packet_t *p_packet){
	static uint32_t step = 1;
	uint32_t status = 0;
	uint8_t *buffer = p_packet->buffer;
	
	switch(step){
		case 1:
			/* Set read mode, slave address and 3 internal address byte lengths */
			p_twi->TWI_MMR = 0;
			p_twi->TWI_MMR = TWI_MMR_MREAD | TWI_MMR_DADR(p_packet->chip) |
			((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) &
			TWI_MMR_IADRSZ_Msk);
	
			/* Set internal address for remote chip */
			p_twi->TWI_IADR = 0;
			p_twi->TWI_IADR = twi_mk_addr(p_packet->addr, p_packet->addr_length);
	
			p_twi->TWI_CR = TWI_CR_START | TWI_CR_STOP;
			step = 2;
		break;
		
		case 2:
			status = p_twi->TWI_SR;
			if ((status & TWI_SR_RXRDY)) {
				step = 3;
			}
		break;
		
		case 3:
			*buffer = p_twi->TWI_RHR;
			while (!(p_twi->TWI_SR & TWI_SR_TXCOMP)) {
			}
			p_twi->TWI_SR;
			step = 1;
			return 1;
		break;
		default: break;
	}
		return 0;
}