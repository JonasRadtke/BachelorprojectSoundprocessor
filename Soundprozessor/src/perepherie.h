#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

struct Settings
{
	uint8_t waveform;
	uint8_t burst;
	uint8_t arpeggio;
	uint8_t Release;
	uint8_t Sustain;
};

void readkeys(uint8_t*,uint8_t*,twi_package_t);
Settings readSettings(Settings);
void writeLed(Settings*);

uint8_t getReleaseValue();
uint8_t getSustainTonValue();
uint8_t getArpeggioValue();
uint8_t getDutyCycleVaule();
