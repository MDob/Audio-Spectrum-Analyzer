/*
 * adc_cfg.c
 *
 * Created: 2015-10-27 8:05:44 PM
 *  Author: Michael
 */ 

#include "adc_cfg.h"
#include "dma_cfg.h"
#include "adc.h"
#include "adc_callback.h"

void ADC_configure( void );
void ADC_configureCallbacks( void );
void adc_complete_callback( struct adc_module *const module );


void adc_complete_callback( struct adc_module *const module )
{
    //char output[8] = {0};
    
    //sprintf(output, "\r%d", adc_buffer);
    
    //memset(FTDI_TxBuffer, 0x00, sizeof(FTDI_TxBuffer));
    //for(uint8_t i = 0; i < 8; i++)
    //{
    //    FTDI_TxBuffer[i] = output[i];
    //}
    
    //dma_start_transfer_job(&zDMA_FTDIResourceTx);
}

void ADC_init( void )
{
    ADC_configure();
    ADC_configureCallbacks();
}

void ADC_configureCallbacks( void )
{
    adc_register_callback(&sw_adc, adc_complete_callback, ADC_CALLBACK_READ_BUFFER);
    adc_enable_callback(&sw_adc, ADC_CALLBACK_READ_BUFFER);
}

void ADC_configure( void )
{
    struct adc_config config_adc;
    struct system_pinmux_config config;
    
    system_pinmux_get_config_defaults(&config);
    adc_get_config_defaults(&config_adc);
    
    config.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;
    config.mux_position = MUX_PA11B_ADC_AIN19;
    config.mux_position = MUX_PA04B_AC_AIN0;
    
    system_pinmux_pin_set_config(CONF_ADC_PIN, &config);
    
    config_adc.resolution           = ADC_RESOLUTION_CUSTOM;
    config_adc.divide_result        = ADC_DIVIDE_RESULT_32;
    config_adc.accumulate_samples   = ADC_ACCUMULATE_SAMPLES_32;
    config_adc.gain_factor          = ADC_GAIN_FACTOR_4X;
    config_adc.clock_prescaler      = ADC_CLOCK_PRESCALER_DIV8;
    config_adc.reference            = ADC_REFERENCE_INTVCC1;
    config_adc.negative_input       = ADC_NEGATIVE_INPUT_IOGND;
    config_adc.positive_input       = ADC_POSITIVE_INPUT_PIN19;
    //config_adc.positive_input       = ADC_POSITIVE_INPUT_PIN0;
    config_adc.differential_mode    = false;
    
    adc_init(&sw_adc, ADC, &config_adc);
    
    adc_enable(&sw_adc);
}