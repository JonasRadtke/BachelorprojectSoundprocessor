/*
 * uart.c
 *
 * Created: 22.05.2017 11:01:25
 *  Author: Jonas Radtke
 */ 

#include "uartt.h"

uint8_t uartrxbuffer[250] = {0};
char eingabebuffer[250] = {0};
volatile uint8_t uartindex = 0;


void uartInit(){
	//UART initalisieren
	pmc_enable_periph_clk(ID_UART1); // Clock auf UART
	pio_set_peripheral	( PIOB, PIO_PERIPH_A, serielltxd | seriellrxd); // UART Pins aktivieren
	sam_uart_opt_t uartoptionen  = {
		.ul_baudrate = 115200,
		.ul_mode =  UART_MR_PAR_NO,
		.ul_mck = SystemCoreClock
	}; // Daten für den UART
	uart_init(seriell, &uartoptionen);
	uart_enable_interrupt(UART1, UART_IER_RXRDY);
	NVIC_EnableIRQ(seriell_IRQ);
	
	return;
}


// UART Zeichen senden
// Zeichen wird als Char übergeben
void uartsendbyte (uint8_t c) // Uart senden eines Bytes
{
	while (!uart_is_tx_ready(seriell) && !uart_is_tx_empty(seriell)); // Solang UART beschäftigt und Sendebuffer nicht leer ist WARTEN
	uart_write 	(seriell, c);	// Zeichen senden
	return;
}

// UART String senden
// String wird als char Array übgerben
void uartsendstring(char s[])	// Uart senden eines Strings
{
	uint8_t x = 0;
	while (s[x] != 0) {			// Solang kein Ende des Strings gefunden wurde weiter senden
		uartsendbyte(s[x]);		// Zeichen senden
		x++;					// nächstes zeichen
	}
	return;
}

// Uart Zeichen holen!
void UART1_Handler()   // Uart Interrupt bei Zeichen angekommen
{
	if(uart_is_rx_ready(seriell)) // Wenn Zeichen im Buffer
	{
		uart_read(seriell, &uartrxbuffer[uartindex]); // Zeichen aus Buffer holen
		if (uartrxbuffer[uartindex] != 0)	// Solange nicht Ende des Strings erreicht
		{
			if(uartrxbuffer[uartindex] == '\n' && uartrxbuffer[uartindex-1] == '\r' && uartindex >=1) // Wenn letztes Zeichen angekommen
			{
				uartrxbuffer[uartindex-1] = 0;			// Wenn ende erreicht letztes Zeichen mit 0 überschreiben
				strcpy(eingabebuffer, &uartrxbuffer);	// ganzen eingangsbuffer kopieren
				//uartrxbuffer[0] = 0;
				uartindex = 0;							// Index wieder auf Anfang
			}
			else
			{
				uartindex++;  //Index um eins erhöhen fall noch kein Ende erreicht wurde
			}
		}
	}
}