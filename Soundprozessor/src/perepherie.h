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
	uint8_t halleffekt;
	uint8_t anhaltenderton;
};

uint8_t readsettings(Settings);
void writeLed(Settings*);
uint8_t getVolumeValue();
uint8_t getAnhaltenderTonValue();
uint8_t getArpeggioValue();
uint8_t getDutyCycleVaule();