/*
 * oszillatoren.c
 *
 * Created: 23.05.2017 12:30:01
 *  Author: Radtke
 */ 

#include "oszillatoren.h"

volatile oszillator oszillatoren[8];
volatile chan chanel[8];
union dds_cnt_t dds_counter[8] = {0};

volatile uint16_t noise_cnt = 0;
volatile uint16_t noise_divider = 0;
volatile uint16_t noise_lfsr = 1 ; // SEED = 1
volatile uint16_t noise_bit = 0;

volatile uint16_t ausgang = 128;

volatile uint32_t temp_ausgabe = 0;
volatile uint32_t temp_ausgabe2 = 0;

volatile uint16_t noten[88] = {4186,3951,3729,3520,3322,3136,2960,2794,2637,2489,2349,2217,2093,1976,1865,1760,1661,1568,1480,1397,1319,1245,1175,1109,1047,988,932,880,831,784,740,698,659,622,587,554,523,494,466,440,415,392,370,349,330,311,294,277,262,247,233,220,208,196,185,175,165,156,147,139,131,123,117,110,104,98,92,87,82,78,73,69,65,62,58,55,52,49,46,44,41,39,37,35,33,31,29,28};
volatile uint8_t dreiecktabelle[TRITAB] = { 0 , 32 , 64 , 96 , 128 , 160 , 192 , 224, 255 , 224 , 192 , 160 , 128 , 96 , 64 , 32};

uint8_t tasten[2];


void timerInit (void){
	uint32_t rc;
	// Timer für Oszillatoren
	sysclk_enable_peripheral_clock(ID_TC0); // Clock auf Timer 0
	tc_init(TC, 0,
	TC_CMR_TCCLKS_TIMER_CLOCK1		// Waveform Clock Selection
	| TC_CMR_WAVE					// Waveform mode
	| TC_CMR_WAVSEL_UP_RC			// Bei RC zurücksetzen
	);
	tc_enable_interrupt(TC, 0, TC_IER_CPCS); // Compare RC Interrupt einschalten
	rc = (SystemCoreClock /2 /SAMPLEFREQ);	// Samplefrequenz
	tc_write_rc(TC, 0, rc);				// RC einstellen
	NVIC_EnableIRQ(TC0_IRQn);			// Interrupt einschalten
	tc_start(TC, 0);					// Timer starten
	
	// Timer für
	sysclk_enable_peripheral_clock(ID_TC1); // Clock auf Timer 0
	tc_init(TC, 1,
	TC_CMR_TCCLKS_TIMER_CLOCK1		// Waveform Clock Selection
	| TC_CMR_WAVE					// Waveform mode
	| TC_CMR_WAVSEL_UP_RC			// Bei RC zurücksetzen
	);
	tc_enable_interrupt(TC, 1, TC_IER_CPCS); // Compare RC Interrupt einschalten
	rc = (SystemCoreClock /2 /PHASEAKKU_FREQ);	//
	tc_write_rc(TC, 1, rc);				// RC einstellen
	NVIC_EnableIRQ(TC1_IRQn);			// Interrupt einschalten
	tc_start(TC, 1);

	// Timer für NOISE
	sysclk_enable_peripheral_clock(ID_TC2); // Clock auf Timer 0
	tc_init(TC, 2,
	TC_CMR_TCCLKS_TIMER_CLOCK1		// Waveform Clock Selection
	| TC_CMR_WAVE					// Waveform mode
	| TC_CMR_WAVSEL_UP_RC			// Bei RC zurücksetzen
	);
	tc_enable_interrupt(TC, 2, TC_IER_CPCS); // Compare RC Interrupt einschalten
	rc = (SystemCoreClock /2 /207826);	//
	tc_write_rc(TC, 2, rc);				// RC einstellen
	NVIC_EnableIRQ(TC2_IRQn);			// Interrupt einschalten
	tc_start(TC, 2);
	
	return;
}



// DIE TIMER BZW. KLANGERZEUGUNG!
void TC0_Handler()
{
	static uint32_t test;
	if (oszillatoren[0].ein >= 1) // Falls der Oszillator eingeschaltet ist wird das Signal generiert
	{
		switch(oszillatoren[0].waveform) {
			//rectangle
			// Hier wird das Rechtecktsignal erzeugt. Es wird mit Zählwert 0 High gesetzt und mit dem rect_low signal auf Low gesetzt
			// Rect_low wurde in der Main berechnet, je nach Duty Cycle
			// Der Endwert bestimmt die Frequenz. Der Zählwert wird auf 0 gesetzt.
			case 1:
			if (chanel[0].rect_count <= chanel[0].rect_low) // Wenn Dutycycle noch nicht erreicht Rechteck High!
			{
				chanel[0].chan_out = 128;
			}
			else
			{
				chanel[0].chan_out = 0;
			}
			
			chanel[0].rect_count++; // Zaehlwert erhoehen
			
			if (chanel[0].rect_count >= chanel[0].rect_end) // Am Ende wieder zuücksetzen
			{
				chanel[0].rect_count = 0;
			}
			ausgang = chanel[0].chan_out;
			break;
			
			//triangle
			// hier wird der Wert aus der Tabelle ausgegeben. Es wird der Index aus dem obersten Byte des Phasenakkumulators verwendet.
			case 2:
			if (dds_counter[0].phase > 15)
			{
				dds_counter[0].phase = 0;
			}

			chanel[0].chan_out = dreiecktabelle[dds_counter[0].phase];
			ausgang = chanel[0].chan_out;
			break;
			
			//noise
			case 3:
			if ((noise_lfsr & 0x01) == 1)
			{
				ausgang = 255;
			}
			else
			{
				ausgang = 0;
			}
			break;
			
			default:
			break;
		}
	}
	else //Falls der Oszillator nicht belegt ist wird der Ausgangswert gelöscht.
	{
		chanel[0].chan_out = 0;
	}

	test = TC->TC_CHANNEL[0].TC_SR;
}

void TC1_Handler()
{
	static uint32_t test;
	
	dds_counter[0].counter = dds_counter[0].counter + chanel[0].tri_stepsize;
	
	PIOA->PIO_SODR = DAC_NWRITE;
	temp_ausgabe2 = ausgang;
	temp_ausgabe = 0;
	
	

	temp_ausgabe = (temp_ausgabe2 & 0x0F) << 17;
	temp_ausgabe = temp_ausgabe + ((temp_ausgabe2 & 0x30) << 7);
	temp_ausgabe = temp_ausgabe + ((temp_ausgabe2 & 0x40) >> 6);
	temp_ausgabe = temp_ausgabe + ((temp_ausgabe2 & 0x80) >> 4);
	
	PIOA->PIO_CODR = (~temp_ausgabe) & 0x001E1809;
	PIOA->PIO_SODR = (temp_ausgabe) & 0x001E1809;
	
	PIOA->PIO_CODR = DAC_NWRITE;
	
	
	
	test = TC->TC_CHANNEL[1].TC_SR;
	//aktuellerwert = TC->TC_CHANNEL[0].TC_CV;
}

void TC2_Handler()
{
	static uint32_t test;
	
	noise_cnt++;
	if (noise_cnt >= noise_divider)
	{
		noise_cnt = 0;
		// Hier LFSR weiter tüdeln
		if ((tasten[1] & 0x80) >= 1)
		{
			noise_bit  = ((noise_lfsr >> 0) ^ (noise_lfsr >> 6)) & 1; // 0 Bit Mit dem 6 Bit XOR ( Metallischer Klang )
		}
		else
		{
			noise_bit  = ((noise_lfsr >> 0) ^ (noise_lfsr >> 1)) & 1;	// 0 bit mit 1 bit XOR ( Normales Rauschen )
		}
		noise_lfsr =  (noise_lfsr >> 1) | (noise_bit << 14);	// Rechts schieben und vorne das Ergebnis einfügen
	}
	
	test = TC->TC_CHANNEL[2].TC_SR;
	//aktuellerwert = TC->TC_CHANNEL[0].TC_CV;
}
