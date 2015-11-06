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

#define ADC_SAMPLES 128

uint16_t micADCBuffer[ADC_SAMPLES];
//uint16_t auxADCBuffer[ADC_SAMPLES];
uint16_t confADCBuffer;

struct adc_module conf_instanceADC;
struct adc_module mic_instanceADC;
struct adc_module aux_instanceADC;
struct adc_module accx_instanceADC;
struct adc_module accy_instanceADC;

void ADC_init( void );

#endif /* ADC_CFG_H_ */