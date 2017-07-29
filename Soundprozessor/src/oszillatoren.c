/*
 * oszillatoren.c
 *
 * Created: 23.05.2017 12:30:01
 *  Author: Radtke
 */ 

#include "oszillatoren.h"

chan channel[8];

uint32_t sustainVolume = 0xFFFFFFFF/2;
uint32_t delayzeit = 100;
uint32_t releasezeit = 100;



float notes[88] = {  27.5000,	  29.1352,	  30.8677,	  32.7031,	  34.6478,	  36.7080,	  38.8908,	  41.2034,	  43.6535,	  46.2493,	  48.9994, 
							  51.9130,	  55	 ,    58.2704,	  61.7354,	  65.4063,	  69.2956,	  73.4161,    77.7817,	  82.4068,	  87.3070,	  92.4986,    
							  97.9988,    103.8261,  110	 ,   116.5409,	 123.4708,	 130.8127,	 138.5913,	 146.8323,	 155.5634,	 164.8137,	 174.6141,	 
							 184.9972,    195.9977,	 207.6523,	 220	 ,   233.0818,	 246.9416,	 261.6255,	 277.1826,	 293.6647,	 311.1269,	 329.6275,	  
							 349.2282,	 369.9944,	 391.9954,	 415.3046,	 440	 ,   466.1637,	 493.8833,	 523.2511,	 554.3652,	 587.3295,	 622.2539, 
							 659.2551,	 698.4564,	 739.9888,	 783.9908,	 830.6093,	 880	 ,   932.3275,	 987.7666,	1046.5022,	1108.7305,	1174.6590,	 
							 1244.5079,	1318.5102,	1396.9129,  1479.9776,	1567.9817,	1661.2187,	1760	 ,  1864.6550,	1975.5332,	2093.0045,	2217.4610,	 
							 2349.3181,	2489.0158,	2637.0204,	2793.8258,	2959.9553,	3135.9634,	3322.4375,	3520	 ,  3729.3100,	3951.0664,	4186.0090}; 
uint8_t triangletab[TRITAB] = { 0 , 32 , 64 , 96 , 128 , 160 , 192 , 224, 255 , 224 , 192 , 160 , 128 , 96 , 64 , 32}; // Triangle tab
uint16_t divider[16] = { 1, 2, 4, 8, 16, 24, 32, 40, 50, 63, 95, 127, 190, 254, 508, 1017 }; // Divider for the Noise LFSR




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
	rc = (SystemCoreClock /2 /223721);	//
	tc_write_rc(TC, 2, rc);				// RC einstellen
	NVIC_EnableIRQ(TC2_IRQn);			// Interrupt einschalten
	tc_start(TC, 2);
	
	return;
}



// DIE TIMER BZW. KLANGERZEUGUNG!
void TC0_Handler()
{
	static uint32_t test;

	oscillator(&channel[0]); // Oscillator Channel
	oscillator(&channel[1]); // Oscillator Channel
	oscillator(&channel[2]); // Oscillator Channel
	oscillator(&channel[3]); // Oscillator Channel
	oscillator(&channel[4]); // Oscillator Channel
	oscillator(&channel[5]); // Oscillator Channel
	oscillator(&channel[6]); // Oscillator Channel
	oscillator(&channel[7]); // Oscillator Channel
	
	dac_out = channel[0].chan_out/2 + channel[1].chan_out/2; // Accumulate Oscillator
	
	// Timer Statusregister lesen, muss gemacht werden, keine Ahnung wieso
	test = TC->TC_CHANNEL[0].TC_SR;
	if (test){}

}

void TC1_Handler()
{
	static uint32_t test;
	uint32_t dac_temp;
	
	// Phaseaccu for Triangle Table (DDS)
	channel[0].dds_counter.counter += channel[0].tri_stepsize;
	channel[1].dds_counter.counter += channel[1].tri_stepsize;
	channel[2].dds_counter.counter += channel[2].tri_stepsize;
	channel[3].dds_counter.counter += channel[3].tri_stepsize;
	channel[4].dds_counter.counter += channel[4].tri_stepsize;
	channel[5].dds_counter.counter += channel[5].tri_stepsize;
	channel[6].dds_counter.counter += channel[6].tri_stepsize;
	channel[7].dds_counter.counter += channel[7].tri_stepsize;
	
	// Write DAC
	dac_temp = 0;

	PIOA->PIO_SODR = DAC_NWRITE; // Write Befehl zurück nehmen

	dac_temp = (((uint32_t)dac_out) << 5);		// Ausgangsbyte um 5 verschieben PA5
	dac_temp =	dac_temp | ((((uint32_t)dac_out) >> 2) &  0x00000011);
	
//	PIOA->PIO_CODR = 0x00001E63;					// Ausgänge 0 Setzen
//	PIOA->PIO_SODR = dac_temp & 0x00001E63;		// Neuen Wert setzen, maskiert
	
	PIOA->PIO_CODR = DAC_NWRITE;	// Write Befehl an DAC
	
	
	// Timer Statusregister lesen, muss gemacht werden, keine Ahnung wieso
	test = TC->TC_CHANNEL[1].TC_SR; 
	if (test){}
	
}

void TC2_Handler()
{
	static uint32_t test;
	// 8 LFSR for Noise Channel
	PIOA->PIO_CODR =D0;

	noise(&channel[0]);
//	noise(&channel[1]);
//	noise(&channel[2]);
//	noise(&channel[3]);
//	noise(&channel[4]);
//	noise(&channel[5]);
//	noise(&channel[6]);
//	noise(&channel[7]);
	
	
	// Timer Statusregister lesen, muss gemacht werden, keine Ahnung wieso
	test = TC->TC_CHANNEL[2].TC_SR;
	if (test){}
	PIOA->PIO_SODR = D0;
}

void oscillator(chan *x){

	if (x->oscillator_on >= 1) // Falls der Oszillator eingeschaltet ist wird das Signal generiert
	{
		switch(x->waveform) {
			//rectangle
			// Hier wird das Rechtecktsignal erzeugt. Es wird mit Zählwert 0 High gesetzt und mit dem rect_low signal auf Low gesetzt
			// Rect_low wurde in der Main berechnet, je nach Duty Cycle
			// Der Endwert bestimmt die Frequenz. Der Zählwert wird auf 0 gesetzt.
			case RECTANGLE:
				if (x->rect_count <= x->rect_low) // Wenn Dutycycle noch nicht erreicht Rechteck High!
				{
					x->chan_out = (uint8_t)(x->envelopeVolume >> 24);
				}
				else
				{
					x->chan_out = 0;
				}
		
				x->rect_count++; // Zaehlwert erhoehen
		
				if (x->rect_count >= x->rect_end) // Am Ende wieder zuücksetzen
				{
					x->rect_count = 0;
				}
			break;
		
			//triangle
			// hier wird der Wert aus der Tabelle ausgegeben. Es wird der Index aus dem obersten Byte des Phasenakkumulators verwendet.
			case TRIANGLE:
				if (x->dds_counter.phase > 15)
				{
					x->dds_counter.phase = 0;
				}

				x->chan_out = triangletab[x->dds_counter.phase];
			break;
		
			//noise
			case NOISE:
				if ((x->noise_lfsr & 0x01) == 1)
				{
					x->chan_out = (uint8_t)(x->envelopeVolume >> 24);
				}
				else
				{
					x->chan_out = 0;
				}
			break;
		
			default:
			break;
		}
	}
	else //Falls der Oszillator nicht belegt ist wird der Ausgangswert gelöscht.
	{
		x->chan_out = 0;
	}

}


void noise(chan *x){
	x->noise_cnt++;
	if (x->noise_cnt >= x->noise_divider)
	{
		x->noise_cnt = 0;
		// Hier LFSR weiter tüdeln
		if ((x->noise_metal) >= 1)
		{
			x->noise_bit  = ((x->noise_lfsr >> 0) ^ (x->noise_lfsr >> 6)) & 1; // 0 Bit Mit dem 6 Bit XOR ( Metallischer Klang )
		}
		else
		{
			x->noise_bit  = ((x->noise_lfsr >> 0) ^ (x->noise_lfsr >> 1)) & 1;	// 0 bit mit 1 bit XOR ( Normales Rauschen )
		}
		x->noise_lfsr =  (x->noise_lfsr >> 1) | (x->noise_bit << 14);	// Rechts schieben und vorne das Ergebnis einfügen
	}
}

void activateChannel(uint8_t key[] ,chan x[], float note[], uint16_t div[]){
	uint8_t i = 0;
	uint8_t keyTemp = 0;
	uint8_t j = 0;
	uint8_t keyNumberTemp = 0;
	int8_t freeChannel = 0;
	
	
	// Search for pressed Key
	for (i = 0; i<6; i++)
	{
		keyNumberTemp = 0;
		keyTemp = key[i]; 
		if ( keyTemp > 0) // If Pressed Key in Byte
		{
			// Search for pressed Key in 1 Byte
			for (j=1; j<8; j++)
			{
				if ( (keyTemp & 0x01) == 1 ) // If Key is pressed
				{
					keyNumberTemp = i*8 + j;	// Calculate Key Number
					freeChannel = _searchFreeChannel(x, keyNumberTemp);	// looking for free channel
					if (freeChannel == 100)	// If no Channel ist free, return
					{
						return;
					}
					else if (freeChannel < 100) // If free channel is found
					{
						_calculateChannelSettings(x, freeChannel, keyNumberTemp, note, div); // Calculate the Settings for the specific channel
					}
					else{}
				}
			keyTemp = keyTemp >> 1; // Shift left, next key
			}
		}
	}
	
}

int8_t _searchFreeChannel(chan x[], uint8_t key){
	uint8_t i = 0;
	
	// Looking for pressed key has already a active channel
	for (i = 0; i<8; i++)
	{
		if(x[i].pushed_key == key)
		{
			return 101;
		}
	}
	
	// Looking for Free Channel
	for (i = 0; i<8; i++)
	{
		if (x[i].oscillator_on == 0)
		{
			return i; // Return Number of free Channel
		}
	}
	return 100; // Return no free Channel found
}

void _calculateChannelSettings(chan x[], uint8_t channelIndex, uint8_t key, float note[], uint16_t div[]){
	uint8_t tempWaveform = 1;
	channel[channelIndex].dutycycle = 50; // MUSS NOCH GEÄNDERT WERDEN
	
	
	
	switch (tempWaveform){
		
		// Settings for Rectangle Waveform
		case RECTANGLE:
		channel[channelIndex].frequency = note[key-1 + 37];
		channel[channelIndex].rect_end = SAMPLEFREQ / channel[channelIndex].frequency;
		channel[channelIndex].rect_low = ((uint32_t)channel[channelIndex].rect_end * channel[channelIndex].dutycycle )/ 100;
		channel[channelIndex].waveform = RECTANGLE;
		break;
		
		// Settings for Triangle Waveform
		case TRIANGLE:
		channel[channelIndex].frequency = note[key-1 + 37];
		channel[channelIndex].tri_stepsize = (BIT24*channel[channelIndex].frequency*TRITAB)/PHASEAKKU_FREQ; //Zaehler für Phasenakkumulator
		channel[channelIndex].waveform = TRIANGLE;
		break;
		
		// Settings for Noise
		case NOISE:
		channel[channelIndex].noise_divider = div[(key-1)%16]; // Only 16 divider, repeats after 16 Keys
		channel[channelIndex].waveform = NOISE;
		break;
		
		default:
		break;
	}
	channel[channelIndex].envelopeVolume = 0xFFFFFFFF; // Preset for envelope Volume
	channel[channelIndex].delayTime = delayzeit;
	channel[channelIndex].releaseTime = releasezeit;
	channel[channelIndex].sustainVol = sustainVolume;
	channel[channelIndex].envelopeStep = (channel[channelIndex].envelopeVolume-channel[channelIndex].sustainVol) / channel[channelIndex].delayTime;
	channel[channelIndex].adsrCnt = channel[channelIndex].releaseTime + channel[channelIndex].delayTime;
	channel[channelIndex].pushed_key = key;  // Write pushed key in Channel struct
	channel[channelIndex].oscillator_on = 1;	
}

void envelopChannel(uint8_t key[] ,chan x[]){
	uint8_t i = 0;
	uint8_t keyInByte = 0;
	uint8_t keyIndex = 0;
	
	for (i = 0; i<8; i++)
	{
		keyIndex = x[i].pushed_key / 8;
		keyInByte = x[i].pushed_key % 8;
		
		if ((key[keyIndex] & (1 << (keyInByte-1))) == 0)
		{
			if (x[i].oscillator_on >= 1) // If Oscillator is On
			{
				//x[i].oscillator_on = 0;
				// If Sustain already active
				if (x[i].adsrCnt <= x[i].delayTime)
				{
					x[i].envelopeStep = x[i].sustainVol / x[i].releaseTime;
				}
				else // Sustain not active, release Volume from actual Delay Volume
				{
					x[i].envelopeStep = x[i].envelopeVolume / x[i].releaseTime;
				}
				x[i].releaseActiv = 1;
			}
			 
		}
	}
	
	// Generate ADSR
	for (i = 0; i<8; i++)
	{
		if (x[i].oscillator_on >= 1) // If Oscillator is On
		{
			if (x[i].adsrCnt >= (x[i].delayTime)) // Delayphase
			{
				x[i].envelopeVolume -= x[i].envelopeStep;
				x[i].adsrCnt--;
			}
			else if ((x[i].adsrCnt <= x[i].delayTime) & !x[i].releaseActiv) // Sustainphase (Constant Volume)
			{
				x[i].envelopeVolume = x[i].sustainVol;
				x[i].adsrCnt = x[i].delayTime;
			}
			else if (x[i].releaseActiv)		// Releasephase
			{
				x[i].adsrCnt--;
				x[i].envelopeVolume -= x[i].envelopeStep;		
			}
		
			if (x[i].adsrCnt <= 0) // End of Tone
			{
				x[i].oscillator_on = 0;
				x[i].releaseActiv = 0;
				x[i].pushed_key = 0;
				x[i].waveform = 0;
				x[i].chan_out = 0;
			}
		}	
	}
	
}