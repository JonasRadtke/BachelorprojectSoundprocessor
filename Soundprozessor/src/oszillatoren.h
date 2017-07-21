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
#define D2					(PIO_PA6)//(PIO_PA7)
#define D3					(PIO_PA6)//(PIO_PA8)
#define D4					(PIO_PA9)
#define D5					(PIO_PA10)
#define D6					(PIO_PA11)
#define D7					(PIO_PA12)
#define TRITAB				(16)
#define BIT24				(16777216)

#define RECTANGLE			(1)
#define TRIANGLE			(2)
#define NOISE				(3)

volatile uint8_t dac_out;

union dds_cnt_t{
	struct{
		uint8_t byte1;
		uint8_t byte2;
		uint8_t byte3;
		uint8_t phase;
	};
	uint32_t counter;
} ;

typedef struct chan1 {
	uint8_t oscillator_on;	// Chanel On/off
	uint8_t waveform;		// Waveform, RECTANGLE, TRIANGLE, NOISE
	uint8_t pushed_key;		// Number of pushed Key in this Oscillator
	float frequency;		// Frequency of Note
	
	uint8_t dutycycle;       // Duty Cycle in percent 0-100%
	uint16_t rect_low;	   	// Rectangle, this is the Dutycycle 
	uint16_t rect_end;		// Rectangle, end
	uint16_t rect_count;	 // Rectangle counter, for internal counting
	
	uint8_t tri_table_index; // Indexing the Table of Triangle
	uint32_t tri_stepsize;	 // Vorberechnete Schrittweite
	
	union dds_cnt_t dds_counter;
	
	uint8_t chan_out;		// Output of Chanel
	
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



void timerInit (void);
void oscillator(chan *);


#endif /* OSZILLATOREN_H_ */