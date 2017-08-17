/**
 * \file
 *
 * \brief Empty user application template
 *
 */

#include "main.h"



// Externe Globale Variablen
extern chan channel[8];

extern float notes[88];
extern uint8_t triangletab[TRITAB];
extern uint16_t divider[16]; 

extern uint8_t tasten[8];


volatile uint32_t ticks = 0; // Systemzeit seit Start in Millisekunden

uint32_t wt = 0;



int main (void)
{
	pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL);   // enable external 32.768kHz crystal
	while (!pmc_osc_is_ready_32kxtal());        // wait until oscillator is ready
	sysclk_init();				// 48 Mhz einstellen
	wdt_disable(WDT);			// Watchdog ausschalten
	SystemCoreClockUpdate();	// Systemclock akualisieren
	fpu_enable ();				// Floatingpoint Unit aktivieren, Achtung richtige Optimierung einstellen!
	delay_init(SystemCoreClock);	
	//SystemCoreClock = 44300000;
	SysTick_Config(SystemCoreClock / 1000);      /* Configure SysTick to generate an interrupt every millisecond */

	pmc_enable_periph_clk(ID_PIOA);	
	pmc_enable_periph_clk(ID_PIOB);
//	pio_set_output 	( 	PIOA, D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7 | DAC_NWRITE ,LOW,DISABLE,DISABLE); // Setze Ausgänge
	
	// Initialize Noise Channel (Seed)
	uint8_t i;
	for(i=0; i<8; i++){
		channel[i].noise_lfsr = 1;
	}
	
	// Timer initalisieren! 
	timerInit();

	//TWI I2C Portexpander initalisieren
	twiInit();

	//UART initalisieren
	uartInit();
	
	//SPI Init
	spiInit();
	
	for (i=0; i<8; i++)
	{
		tasten[i]=0;
	}
	
	for (i=0; i<8; i++)
	{
		channel[i].oscillator_on = 0;
		channel[i].releaseActiv = 0;
	}
	

	//ADC Init
	//	pmc_enable_periph_clk(ADC);
	//	adc_enable();
	//	adc_get_config_defaults(&adc_cfg);
	//	adc_init(ADC, &adc_cfg);
	//	adc_set_trigger(ADC, ADC_TRIG_SW);
	//	adc_channel_enable(ADC, ADC_CHANNEL_2);

	uint32_t delaytasten = 0;
	uint32_t delayenv = 0;
	dac_out = 0;
	
	// ZUM OSZILLATOR TESTEN!

	//
	

	uint8_t newkeys[8] = {};	//Array für die Nummern der neu gedrückten Tasten
	uint8_t keys[8] = {0};		//Array für die Nummern der gedrückten tasten
	Settings settings ={.Sustain=0,.arpeggio=0,.burst=0,.Release=0,.waveform=0}; //Standardmodi einstellen

	while(1)
	{
		
		if ((ticks) >= delaytasten+10)
		{
			delaytasten = ticks;
			SPI->SPI_TDR = 	0x0001A0A0;
		
		//printf("test");	
	//		portexpander_einlesen(tasten[0]);
			
	//	print_tasten();
	//	activateChannel(tasten ,channel, notes, divider);
	//	envelopChannel(tasten ,channel);
		
		}
	
		if ((ticks) >= delayenv+1)
		{
			delayenv = ticks;	
		
	//		envelopChannel(tasten ,channel);
			

		}

	


	}
}

//


// Systemtakt 1ms
void SysTick_Handler(){
	ticks++;	
}


void print_tasten()
{
	char text[200];
	sprintf(text,   " Tasten 1: %d  \n", tasten[0]);
	uartsendstring(text);
	sprintf(text,	" Tasten 2: %d  \n\n", tasten[1]);
	uartsendstring(text);
	return;
}