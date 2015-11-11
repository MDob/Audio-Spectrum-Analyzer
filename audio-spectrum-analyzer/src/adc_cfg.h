/*************************************************************************
 *  File Name:      adc_cfg.h
 *  Author:         Michael Dobinson
 *  Date Created:   2015-10-27
 *
 *  Sub-files:
 *      adc_cfg.c
 *
 *  Brief:
 *      Provides configuration settings for the ADC
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

#ifndef ADC_CFG_H_
#define ADC_CFG_H_

/*======================================================================*/
/*                          GLOBAL DEPENDENCIES                         */
/*======================================================================*/
#include "compiler.h"
#include "user_board.h"

/*======================================================================*/
/*                      GLOBAL CONSTANT DEFINITIONS                     */
/*======================================================================*/
#define ADC_SAMPLES 1024

/*======================================================================*/
/*                      GLOBAL VARIABLE DEFINITIONS                     */
/*======================================================================*/
uint16_t audioADCBuffer[ADC_SAMPLES];
uint16_t confADCBuffer;

struct adc_module conf_instanceADC;
struct adc_module mic_instanceADC;
struct adc_module aux_instanceADC;
struct adc_module accx_instanceADC;
struct adc_module accy_instanceADC;

/*======================================================================*/
/*                      FUNCTION PROTOTYPES                             */
/*======================================================================*/
void ADC_init( void );
void TASK_adcFFT( void *pvParameters );

#endif /* ADC_CFG_H_ */