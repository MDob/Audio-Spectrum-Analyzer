/*
 * led_ctrl.h
 *
 * Created: 2015-10-24 1:09:18 PM
 *  Author: Michael
 */ 


#ifndef LED_CTRL_H_
#define LED_CTRL_H_

#include "compiler.h"

#define LED_TYPE	WS2812B
#define LED_NUM		144

#define LED_1_CODE			0b11100000000000000000000000000000
#define LED_0_CODE			0b10000000000000000000000000000000

#define CODES_PER_32BITS	6
#define LED_NUM				144
#define LED_BITS_PER_BIT	4
#define LED_BITS_PER_LED	24
#define SPI_BITS_PER_LED	LED_BITS_PER_BIT * LED_BITS_PER_LED

//const uint32_t* spiLEDData = ( uint32_t* ) malloc( (uint32_t) ( SPI_BITS_PER_LED * LED_NUM ) / sizeof( uint32_t ) );


#endif /* LED_CTRL_H_ */