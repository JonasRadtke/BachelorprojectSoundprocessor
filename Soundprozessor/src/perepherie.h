#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "uartt.h"

#define twiwaitus			50		// us, Wait for TWI

typedef struct 
{
	uint8_t waveform;
	uint8_t burst;
	uint8_t arpeggio;
	uint8_t Release;
	uint8_t Sustain;
	uint8_t preinput;
	uint32_t dutyValue;
	uint32_t arpValue;
	uint32_t sustainValue;
	uint32_t releaseValue;
} Settings;

void adcInit(void);
void twiInit(void);

void readkeys(uint8_t*,uint8_t*,Settings*);
uint8_t readSettings(Settings*);
uint8_t writeLed(Settings*);

uint32_t getReleaseValue(void);
uint32_t getSustainValue(void);
uint32_t getArpeggioValue(void);
uint32_t getDutyCycleValue(void);

uint32_t sendPortexpander(Twi *p_twi, twi_packet_t *p_packet);
uint32_t receivePortexpander(Twi *p_twi, twi_packet_t *p_packet);
