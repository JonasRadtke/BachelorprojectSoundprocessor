#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "uartt.h"
#include "structs.h"

#define twiwaitus			50		// us, Wait for TWI

void adcInit(void);
void twiInit(void);

void readkeys(uint8_t*,Settings*);
uint8_t readSettings(Settings*);
uint8_t writeLed(Settings*);

uint32_t getReleaseValue(void);
uint32_t getSustainValue(void);
uint32_t getArpeggioValue(void);
uint32_t getDutyCycleValue(void);

uint32_t sendPortexpander(Twi *p_twi, twi_packet_t *p_packet);
uint32_t receivePortexpander(Twi *p_twi, twi_packet_t *p_packet);
