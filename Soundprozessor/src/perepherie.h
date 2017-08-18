#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct 
{
	uint8_t waveform;
	uint8_t burst;
	uint8_t arpeggio;
	uint8_t Release;
	uint8_t Sustain;
} Settings;



void readkeys(uint8_t*,uint8_t*);
uint8_t readSettings(Settings*,uint8_t);
void writeLed(Settings);

uint8_t getReleaseValue(void);
uint8_t getSustainValue(void);
uint8_t getArpeggioValue(void);
uint8_t getDutyCycleVaule(void);
