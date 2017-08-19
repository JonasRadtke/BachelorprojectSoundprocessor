#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "uartt.h"

typedef struct 
{
	uint8_t waveform;
	uint8_t burst;
	uint8_t arpeggio;
	uint8_t Release;
	uint8_t Sustain;
	uint8_t preinput;
} Settings;

void adcInit(void);

void readkeys(uint8_t*,uint8_t*,Settings*);
uint8_t readSettings(Settings*);
uint8_t writeLed(Settings*);

uint32_t getReleaseValue(void);
uint32_t getSustainValue(void);
uint32_t getArpeggioValue(void);
uint32_t getDutyCycleVaule(void);

uint32_t sendPortexpander(Twi *p_twi, twi_packet_t *p_packet);
uint32_t receivePortexpander(Twi *p_twi, twi_packet_t *p_packet);
