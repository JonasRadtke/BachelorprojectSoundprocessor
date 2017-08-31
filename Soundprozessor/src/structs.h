/*
 * structs.h
 *
 * Created: 22.08.2017 00:49:42
 *  Author: Radtke
 */ 

#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#ifndef STRUCTS_H_
#define STRUCTS_H_

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

#endif /* STRUCTS_H_ */

#define RECTANGLE			(1)
#define TRIANGLE			(2)
#define NOISE				(3)