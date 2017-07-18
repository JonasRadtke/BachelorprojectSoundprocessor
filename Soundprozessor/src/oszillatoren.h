/*
 * oszillatoren.h
 *
 * Created: 23.05.2017 12:30:14
 *  Author: Radtke
 */ 


#ifndef OSZILLATOREN_H_
#define OSZILLATOREN_H_

#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define SAMPLEFREQ			(38000)
#define PHASEAKKU_FREQ		(100000)
#define DAC_NWRITE			(PIO_PA4) 
#define D0					(PIO_PA5)
#define D1				    (PIO_PA6)
#define D2					(PIO_PA7)
#define D3					(PIO_PA8)
#define D4					(PIO_PA9)
#define D5					(PIO_PA10)
#define D6					(PIO_PA11)
#define D7					(PIO_PA12)
#define TRITAB				(16)
#define BIT24				(16777216)
volatile uint8_t dac_out;


typedef struct __attribute__((__packed__)) oszi {
	uint8_t ein;
	uint8_t tastennummer;
	uint8_t waveform;
	float frequenz;
	float duty_cycle;
} oszillator;

typedef struct chan1 {
	uint16_t rect_low;		// Recheck Low, ab welchem Zählerstand er low wird
	uint16_t rect_end;		// Rechteck Ende
	uint16_t rect_count;	 // Rechteck Zähler aktuell
	
	uint8_t tri_table_index; // Dreickecktabelle akutelle Index
	uint32_t tri_stepsize;	 // Vorberechnete Schrittweite
	
	uint8_t chan_out;
	
	//struct unpacker		trackup;
	//u8			tnum;
	//s8			transp;
	//u8			tnote;
	//u8			lastinstr;
	//u8			inum;
	//u16			iptr;
	//u8			iwait;
	//u8			inote;
	//s8			bendd;
	//s16			bend;
	//s8			volumed;
	//s16			dutyd;
	//u8			vdepth;
	//u8			vrate;
	//u8			vpos;
	//s16			inertia;
	//u16			slur;
} chan;

union dds_cnt_t{
	struct{
		uint8_t byte1;
		uint8_t byte2;
		uint8_t byte3;
		uint8_t phase;
	};
	uint32_t counter;
} ;

void timerInit (void);


#endif /* OSZILLATOREN_H_ */