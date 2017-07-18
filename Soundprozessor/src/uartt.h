/*
 * uart.h
 *
 * Created: 22.05.2017 11:00:07
 *  Author: Jonas Radtke
 */ 


#ifndef UARTT_H_
#define UARTT_H_

#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define seriell				UART1
#define serielltxd			PIO_PB3A_UTXD1
#define seriellrxd			PIO_PB2A_URXD1
#define seriell_IRQ			UART1_IRQn


void uartsendbyte (uint8_t c);
void uartsendstring(char s[]);
void uartInit(void);




#endif /* UART_H_ */