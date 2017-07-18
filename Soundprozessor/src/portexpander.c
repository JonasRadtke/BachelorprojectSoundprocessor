/*
 * portexpander.c
 *
 * Created: 23.05.2017 11:58:21
 *  Author: Radtke
 */ 

#include "portexpander.h"

void twiInit(){
	// I2C aktivieren
	pmc_enable_periph_clk(ID_PIOB);	// Clock auf PIOB
	pio_set_peripheral 	( PIOB, PIO_PERIPH_B, PIO_PB0 | PIO_PB1 );	// TWI pins aktivieren, Pins werden auf B Spalte gesetzt, siehe Datenblatt
	
	twi_master_options_t opt = {
		.speed = 100000,
		.chip  = 0x51
	};
	twi_master_setup( TWI2, &opt);
	
	delay_us(500);
}

void portexpander_einlesen(uint8_t x[])
{	// Tasten einlesen
	uint8_t daten1 = 0;
	uint8_t daten2 = 0;
	delay_us(twiwaitus);
	twi_package_t packet_read_port_1 = {
		.addr         = 0x00,      // TWI slave memory address data
		.addr_length  = 0, //keine internal adresse, nur lesen
		.chip         = 0x38,      // TWI slave bus address
		.buffer       = &daten1, // transfer data source buffer
		.length       = 2,  // transfer data size (bytes)
	};
	while(twi_master_read(TWI2, &packet_read_port_1)  != TWI_SUCCESS );
	x[0] = ~daten1;
	delay_us(twiwaitus);
	twi_package_t packet_read_port_2 = {
		.addr         = 0x00,      // TWI slave memory address data
		.addr_length  = 0, //keine internal adresse, nur lesen
		.chip         = 0x3F,      // TWI slave bus address
		.buffer       = &daten2, // transfer data source buffer
		.length       = 1  // transfer data size (bytes)
	};
	while(twi_master_read(TWI2, &packet_read_port_2)  != TWI_SUCCESS );
	x[1] = ~daten2;
	return;
}