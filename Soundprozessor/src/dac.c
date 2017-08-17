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
	pio_set_peripheral	( PIOA, PIO_PERIPH_A,  SPIMOSI | SPICLK | SPINSS); // SPI Periph A Activation
	
	
	SPI->SPI_CR = SPI_CR_SPIEN;
	
	SPI->SPI_MR = SPI_MR_MSTR | SPI_MR_PCS(0)| SPI_MR_MODFDIS;
	
	int16_t baud_div = div_ceil(SystemCoreClock, 2000000); // Baudrate Divider, 2Mhz
	
	//	spi_set_transfer_delay(p_spi, device->id, CONFIG_SPI_MASTER_DELAY_BS,CONFIG_SPI_MASTER_DELAY_BCT);
	SPI->SPI_CSR[0] &= ~(SPI_CSR_DLYBS_Msk | SPI_CSR_DLYBCT_Msk);
	SPI->SPI_CSR[0] |= SPI_CSR_DLYBS(0) | SPI_CSR_DLYBCT(0);
	
	// spi_set_bits_per_transfer(p_spi, device->id, CONFIG_SPI_MASTER_BITS_PER_TRANSFER);
	SPI->SPI_CSR[0] &= (~SPI_CSR_BITS_Msk);
	SPI->SPI_CSR[0] |=  SPI_CSR_BITS_16_BIT;
	
	// spi_set_baudrate_div(p_spi, device->id, baud_div);
	SPI->SPI_CSR[0] &= (~SPI_CSR_SCBR_Msk);
	SPI->SPI_CSR[0] |= SPI_CSR_SCBR(baud_div);
	
	// spi_configure_cs_behavior(p_spi, device->id, SPI_CS_KEEP_LOW);
	SPI->SPI_CSR[0] |= SPI_CSR_CSAAT; // Chip Select after transfer active
	
	// spi_set_clock_polarity(p_spi, device->id, flags >> 1);
	SPI->SPI_CSR[0] |= ~SPI_CSR_CPOL; // Clock Polarity
	
	// spi_set_clock_phase(p_spi, device->id, ((flags & 0x1) ^ 0x1));
	SPI->SPI_CSR[0] |= SPI_CSR_NCPHA; // Clock phase
	
}


