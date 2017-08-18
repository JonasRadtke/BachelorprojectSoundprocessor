/*
 * dac.c
 *
 * Created: 17.08.2017 21:46:37
 *  Author: Radtke
 */ 

#include "dac.h"

void spiInit(){
	// SPI Chip 0 configured Mode 0
	pmc_enable_periph_clk(ID_PIOA);	
	pmc_enable_periph_clk(ID_SPI); // Clock SPI
	pio_set_peripheral	( PIOA, PIO_PERIPH_A,  SPIMOSI | SPICLK | SPINSS ); // SPI Periph A Activation
		
	SPI->SPI_CSR[0] = 0x01011982;     
	
	SPI->SPI_CR = SPI_CR_SPIEN; // Activate SPI
	SPI->SPI_MR = SPI_MR_MSTR | SPI_MR_MODFDIS; // Master Mode, Device 0, Mode Fault Detection
}


