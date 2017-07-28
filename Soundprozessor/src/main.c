/**
 * \file
 *
 * \brief Empty user application template
 *
 */

#include "main.h"



// Externe Globale Variablen
extern volatile chan channel[8];
extern union dds_cnt_t dds_counter[8];

extern volatile uint16_t noten;
extern volatile uint8_t dreiecktabelle;

extern uint8_t tasten[2];

volatile uint32_t ticks = 0; // Systemzeit seit Start in Millisekunden
uint32_t wt = 0;

int main (void)
{
	
	sysclk_init();		// 48 Mhz einstellen
	wdt_disable(WDT);	// Watchdog ausschalten
	SystemCoreClockUpdate();	// Systemclock akualisieren
	fpu_enable ();	// Floatingpoint Unit aktivieren, Achtung richtige Optimierung einstellen!
	delay_init(SystemCoreClock);	
	SystemCoreClock = 44300000;
	SysTick_Config(SystemCoreClock / 1000);      /* Configure SysTick to generate an interrupt every millisecond */

	pmc_enable_periph_clk(ID_PIOA);	
	pmc_enable_periph_clk(ID_PIOB);
	pio_set_output 	( 	PIOA, D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7 | DAC_NWRITE ,LOW,DISABLE,DISABLE); // Setze Ausgänge
	
	// Timer initalisieren! 
	timerInit();

	//TWI I2C Portexpander initalisieren
	twiInit();

	//UART initalisieren
	uartInit();


	//ADC Init
//	pmc_enable_periph_clk(ADC);
//	adc_enable();
//	adc_get_config_defaults(&adc_cfg);
//	adc_init(ADC, &adc_cfg);
//	adc_set_trigger(ADC, ADC_TRIG_SW);
//	adc_channel_enable(ADC, ADC_CHANNEL_2);

	uint32_t delaytemp = 0;
	//delay_ms(1000);
	dac_out = 0;
	
	// ZUM OSZILLATOR TESTEN!
	channel[0].oscillator_on = 1;
	channel[0].waveform = TRIANGLE; // RECTANGLE/TRIANGLE / NOISE
	channel[0].frequency = 880;
	//
	
	uint8_t newkeys[8] = {};	//Array für die Nummern der neu gedrückten Tasten
	uint8_t keys[8] = {0};		//Array für die Nummern der gedrückten tasten
	Settings settings ={.Sustain=0,.arpeggio=0,.burst=0,.Release=0,.waveform=0}; //Standardmodi einstellen
	
	while(1)
	{
		
		

	//	portexpander_einlesen(tasten[0]);

	char text[200];
	sprintf(text,   " Tasten 1: \n");
	uartsendstring(text);
	
	delaytemp = ticks;
	
	while (!((ticks) >= delaytemp+1000));

	
	//	print_tasten();
		// Der Phasenakkumulator ist 32 bit, die höchsten 8 bit werden zum springen der Tabelle benutzt, 24bit sind quasi Nachkommastellen.
		// In einer eigenen ISR, vermutlich 100khz, werden zu den 24bit immer die Stepsize dazu addiert. Diese wird einfach vorberechnet. 
		// Bei jeden Überlauf der 24bit erhöht sich dadurch der Index der Tabelle. 0000 0001 + 24bit. Durch anpassen der Stepsize geschieht dies schneller oder langsamer.
		channel[0].rect_end = SAMPLEFREQ / channel[0].frequency;
		channel[0].rect_low = channel[0].rect_end / 2 ;
		channel[0].tri_stepsize = (BIT24*channel[0].frequency*TRITAB)/PHASEAKKU_FREQ; //Zaehler für Phasenakkumulator
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