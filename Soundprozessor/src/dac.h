/*
 * dac.h
 *
 * Created: 17.08.2017 21:46:52
 *  Author: Radtke
 */ 


#ifndef DAC_H_
#define DAC_H_

#include <asf.h>

#define SPIMOSI		     	PIO_PA13A_MOSI
#define SPICLK			    PIO_PA14A_SPCK
#define SPINSS				PIO_PA11A_NPCS0



void spiInit(void);

#endif /* DAC_H_ */