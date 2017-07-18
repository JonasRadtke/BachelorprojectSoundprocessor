/*
 * portexpander.h
 *
 * Created: 23.05.2017 11:58:08
 *  Author: Radtke
 */ 


#ifndef PORTEXPANDER_H_
#define PORTEXPANDER_H_

#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define twiwaitus			50					// us wartezeit auf TWI

void twiInit(void);
void portexpander_einlesen(uint8_t []);

#endif /* PORTEXPANDER_H_ */