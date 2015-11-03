/*
 * adc_cfg.h
 *
 * Created: 2015-10-27 8:05:32 PM
 *  Author: Michael
 */ 


#ifndef ADC_CFG_H_
#define ADC_CFG_H_

#include "compiler.h"
#include "user_board.h"

#define ADC_SAMPLES 256

uint16_t adc_buffer;
struct adc_module sw_adc;

void ADC_init( void );


#endif /* ADC_CFG_H_ */