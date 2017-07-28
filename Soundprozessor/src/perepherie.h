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
Settings readSettings(Settings);
void writeLed(Settings);

uint8_t getReleaseValue();
uint8_t getSustainTonValue();
uint8_t getArpeggioValue();
uint8_t getDutyCycleVaule();
