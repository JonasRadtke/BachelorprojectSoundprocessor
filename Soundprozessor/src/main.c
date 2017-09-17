/**
 * \file
 *
 * \brief Empty user application template
 *
 */

#include "main.h"



// Externe Globale Variablen
extern chan channel[8];
extern noiseChan singleNoise;

extern float notes[88];
extern uint32_t triangletab[TRITAB];
extern uint32_t divider[16]; 

volatile uint32_t ticks = 0; // Time since Start (ms)

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
	SysTick_Config(SystemCoreClock / 1000);      /* Configure SysTick to generate an interrupt every millisecond */

	pmc_enable_periph_clk(ID_PIOA);	
	pmc_enable_periph_clk(ID_PIOB);
	pio_set_output 	( 	PIOA, LDAC ,LOW,DISABLE,DISABLE); // Setze Ausgänge
	
	// Initialize Noise Channel (Seed)
	uint8_t i,j;
	for(i=0; i<8; i++){
		channel[i].noise_lfsr = 1;
	}
	singleNoise.noise_divider = divider[1-1]; // Noise Divider
	singleNoise.noise_lfsr = 1; // Noise Seed
	
	// Timer initalisieren! 
	timerInit();

	//TWI I2C Portexpander initalisieren
	twiInit();

	//UART initalisieren
	uartInit();
	
	//SPI Init
	spiInit();
	
	//ADC Init
	adcInit();
		
	for (i=0; i<8; i++)
	{
		channel[i].oscillator_on = 0;
		channel[i].releaseActive = 0;
		channel[i].noise_divider = divider[15];
		
		for (j=0; j<50; j++)
		{
			channel[i].arpegNotes[j]=0;
		}
		
		channel[i].arpegNoteNumber=0;
		channel[i].arpegPlayCounter=0;
		channel[i].arpegModeActive=0;
		channel[i].loadNextNote=1;
		channel[i].arpegAttackActive=1;
		channel[i].arpegReleaseActive=0;
	}

	
	// Delay Scheduler
	uint32_t delaytasten = 0;
	uint32_t delayenv = 0;
	
	uint8_t keys[6];	//Array für die Nummern der gedrückten tasten
	Settings settings ={.Sustain=0,.arpeggio=0,.burst=0,.Release=0,.waveform=0, .dutyValue = 512,}; //Standardmodi einstellen
		
	for (i=0; i<6; i++)
	{
		keys[i]=0;
	}
	
	
	
	while(1)
	{
		
		if ((ticks) >= delaytasten+10)
		{
			delaytasten = ticks;	// New Timer Value
			activateChannel(keys,settings ,channel, notes, divider);
		//	envelopChannel(tasten ,channel);
			
			// Get the ADC Values, Raw Data 10bit
			settings.releaseValue = getReleaseValue();
			settings.sustainValue = getSustainValue();
			settings.dutyValue = getDutyCycleValue();
			settings.arpValue = getArpeggioValue();
		}
	
		if ((ticks) >= delayenv+1)
		{
			
			arpeggiator(channel,settings);
						
			delayenv = ticks;	// New Timer Value
			envelopChannel(keys ,channel, settings);
		}
		
		// Read all Keys, Write all LED
		readkeys( keys, &settings); // Programmed as Statemachine

	}
}

// Systemtakt 1ms
void SysTick_Handler(){
	ticks++;	
}

void print_tasten()
{
	uint8_t keys[2];
	char text[200];
	sprintf(text,   " Tasten 1: %d  \n", keys[0]);
	uartsendstring(text);
	sprintf(text,	" Tasten 2: %d  \n\n", keys[1]);
	uartsendstring(text);
	return;
}